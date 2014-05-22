/*
 * SO2 Pitix filesystem driver
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>

#include "pitix.h"

MODULE_DESCRIPTION("Pitix filesystem");
MODULE_AUTHOR("Andrei Petre");
MODULE_LICENSE("GPL");

static struct pitix_inode* pitix_find_ino(struct super_block *s,
					  unsigned long ino,
					  struct buffer_head **bh1,
					  struct buffer_head **bh2)
{
	struct buffer_head *bh;
	unsigned int block_index, offset;
	unsigned int bytes_to_read;
	int remaining_bytes;
	char *start_addr, *last_addr, *final_mii;

	final_mii = kzalloc(sizeof(struct pitix_inode), GFP_KERNEL);
	if (!final_mii) {
		printk(LOG_LEVEL "not enough memory\n");
		goto out;
	}

	block_index = ino * inode_size(s) / s->s_blocksize;
	bh = sb_bread(s, pitix_sbi(s)->izone_block + block_index);
	*bh1 = bh;
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		goto out_free_final;
	}

	offset = (ino * inode_size(s)) % s->s_blocksize;
	start_addr = ((char*) bh->b_data) + offset;
	last_addr = ((char*) bh->b_data) + s->s_blocksize;

	/* Compute number of bytes we can read from current block. There
	 * is the possiblity that the size left in the block is smaller
	 * than the inode's actual size, meaning we need to read another
	 * block to actuall read the entire inode from disk.
	 */
	bytes_to_read = MIN(abs(last_addr - start_addr + 1), inode_size(s));
	printk(LOG_LEVEL "memcpy1 %p %p %d\n", final_mii, start_addr, bytes_to_read);
	memcpy(final_mii, start_addr, bytes_to_read);
	remaining_bytes = MAX(0, inode_size(s) - bytes_to_read);

	/* Read the right next block in case we still haven't read
	 * inode_size(s). */
	if (remaining_bytes) {
		bh = sb_bread(s, pitix_sbi(s)->izone_block + block_index + 1);
		*bh2 = bh;
	}

	if (bh == NULL) {
		printk(LOG_LEVEL "could not read 2nd block\n");
		goto out_free_final;
	}
	printk(LOG_LEVEL "memcpy %p %p %d\n", final_mii + bytes_to_read, bh->b_data, remaining_bytes);
	memcpy(final_mii + bytes_to_read, bh->b_data, remaining_bytes);

	return (struct pitix_inode*) final_mii;

out_free_final:
	kfree(final_mii);
out:
	return NULL;
}

struct inode *pitix_iget(struct super_block *s, unsigned long ino)
{
	struct pitix_inode *mi;
	struct inode *inode;
	struct pitix_inode_info *mii;
	struct buffer_head *bh1 = NULL, *bh2 = NULL;

	/* assert that the ino number is correct. */
	if (ino > get_inodes(s)) {
		printk(LOG_LEVEL "error bad inode index\n");
		goto out;
	}

	/* allocate VFS inode */
	inode = iget_locked(s, ino);
	if (inode == NULL) {
		printk(LOG_LEVEL "error aquiring inode\n");
		return ERR_PTR(-ENOMEM);
	}
	if (!(inode->i_state & I_NEW))
		return inode;

	/* See whether reading once is enough, sometimes
	 * we need to read two blocks.
	 */
	mi = pitix_find_ino(s, ino, &bh1, &bh2);
	if (mi == NULL) {
		printk(LOG_LEVEL "error finding the inode\n");
		goto out_bad_sb;
	}

	/* fill VFS inode */
	inode->i_mode = mi->mode;
	inode->i_uid = mi->uid;
	inode->i_gid = mi->gid;
	inode->i_size = mi->size;
	inode->i_blocks = 0;
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;

	if (S_ISDIR(inode->i_mode)) {
		inode->i_op = &simple_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;

		/* directory inodes start off with i_nlink == 2 */
		inc_nlink(inode);
	}

	/* fill data for mii */
	mii = container_of(inode, struct pitix_inode_info, vfs_inode);
	memcpy(mii->data_blocks, mi->data_blocks,
	       pitix_sbi(s)->inode_data_blocks * sizeof(__u16));

	printk(LOG_LEVEL "found inode %lu %d\n", mii->vfs_inode.i_ino, mi->size);


	/* free resources */
	if (bh1)
		brelse(bh1);
	if (bh2)
		brelse(bh2);
	unlock_new_inode(inode);

	printk(LOG_LEVEL "got inode %lu\n", ino);

	return inode;

out_bad_sb:
	iget_failed(inode);
	if (bh1)
		brelse(bh1);
	if (bh2)
		brelse(bh2);
out:
	return NULL;
}

static struct inode *pitix_sb_alloc_inode(struct super_block *s)
{
	struct pitix_inode_info *mii;
	int ino_blocks;

	/* allocate pitix_inode_info and init inode */
	mii = (struct pitix_inode_info *)
		kzalloc(sizeof(struct pitix_inode_info), GFP_KERNEL);
	if (!mii)
		goto out;

	/* allocate memory for data blocks too. */
	ino_blocks = pitix_sbi(s)->inode_data_blocks;
	mii->data_blocks = kzalloc(ino_blocks * sizeof(__u16), GFP_KERNEL);
	if (!mii->data_blocks)
		goto out_free;

	inode_init_once(&mii->vfs_inode);

	return &mii->vfs_inode;

out_free:
	kfree(mii);
out:
	return NULL;
}

static void pitix_sb_destroy_inode(struct inode *inode)
{
	/* free pitix_inode_info */
	struct pitix_inode_info *mii = container_of(inode,
		struct pitix_inode_info, vfs_inode);
	kfree(mii->data_blocks);
	kfree(mii);
}

static const struct super_operations pitix_ops = {
	.statfs         = simple_statfs,
	.alloc_inode    = pitix_sb_alloc_inode,
	.destroy_inode  = pitix_sb_destroy_inode,
};

struct inode *myfs_get_inode(struct super_block *sb, int mode)
{
	struct inode *inode = new_inode(sb);

	if (!inode)
		return NULL;

	/* TODO 3: fill inode structure
	 *     - mode, uid, gid (or use inode_init_owner)
	 *     - atime,ctime,mtime
	 */
	inode_init_owner(inode, NULL, mode);
	inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;

	/*
	 * TODO 3: inode operations for root inode (use S_ISDIR macro)
	 *     directory link count should be incremented (use inc_nlink)
	 */
	if (S_ISDIR(mode)) {
		inode->i_op = &simple_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;
		inc_nlink(inode);
	}

	return inode;
}

int pitix_fill_super(struct super_block *sb, void *data, int silent)
{
	struct pitix_sb_info *sbi;
	struct pitix_super_block *psb;
	struct inode *root_inode;
	struct dentry *root_dentry;
	struct buffer_head *bh;
	int ret = -EINVAL;

	printk(LOG_LEVEL "pitix fill super\n");

	sbi = kzalloc(sizeof(struct pitix_sb_info), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;
	sb->s_fs_info = sbi;

	/* set block size for superblock */
	if (!sb_set_blocksize(sb, PITIX_S_BLOCKSIZE))
		goto out_bad_blocksize;

	/* read block with superblock (1st block, index 0) */
	if (!(bh = sb_bread(sb, 0)))
		goto out_bad_sb;

	/* interpret read data as pitix_super_block */
	psb = (struct pitix_super_block *) bh->b_data;

	/* check magic number; jump to out_bad_magic if not suitable */
	if (psb->magic != PITIX_MAGIC)
		goto out_bad_magic;

	/* fill sbi with information from disk superblock */
	sbi->version = psb->version;
	sbi->inode_data_blocks = psb->inode_data_blocks;
	sbi->imap_block = psb->imap_block;
	sbi->dmap_block = psb->dmap_block;
	sbi->izone_block = psb->izone_block;
	sbi->dzone_block = psb->dzone_block;
	sbi->bfree = psb->bfree;
	sbi->ffree = psb->ffree;
	/* set the right block size on superblock (now that we know it ). */
	sb_set_blocksize(sb, 1 << psb->block_size_bits);

	/* fill super_block with magic_number, super_operations */
	sb->s_magic = PITIX_MAGIC;
	sb->s_op = &pitix_ops;

	/* allocate root inode and root dentry */
	/* use myfs_get_inode instead of pitix_iget */
	root_inode = pitix_iget(sb, 0);
	//root_inode = myfs_get_inode(sb, S_IFDIR | S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	//root_inode->i_ino = 1;
	if (!root_inode)
		goto out_bad_inode;

	root_dentry = d_make_root(root_inode);
	if (!root_dentry)
		goto out_iput;
	sb->s_root = root_dentry;

	brelse(bh);

	return 0;

out_iput:
	iput(root_inode);
out_bad_inode:
	printk(LOG_LEVEL "bad inode\n");
out_bad_magic:
	printk(LOG_LEVEL "bad magic number\n");
	brelse(bh);
out_bad_sb:
	printk(LOG_LEVEL "error reading buffer_head\n");
out_bad_blocksize:
	printk(LOG_LEVEL "bad block size\n");
	sb->s_fs_info = NULL;
	kfree(sbi);
	return ret;
}

static struct dentry *pitix_mount(struct file_system_type *fs_type, int flags,
		const char *dev_name, void *data)
{
	/* register & unregister - call superblock initialization */
	return mount_bdev(fs_type, flags, dev_name, data, pitix_fill_super);
}

static struct file_system_type pitix_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "pitix",
	.mount		= pitix_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};

static int __init pitix_init(void)
{
	int err;

	err = register_filesystem(&pitix_fs_type);
	if (err) {
		printk(LOG_LEVEL "register_filesystem failed\n");
		return err;
	}

	return 0;
}

static void __exit pitix_exit(void)
{
	unregister_filesystem(&pitix_fs_type);
}

module_init(pitix_init);
module_exit(pitix_exit);
