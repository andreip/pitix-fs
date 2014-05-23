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

/* This function searches for an inode (ino)
 * inside an izone area and also covers the
 * case when an inode is on two different blocks
 * and it needs to read twice. This is why
 * it returns two zones, two buffer heads and
 * two lengths to read from those zones.
 *
 * You may do whatever you please with it :).
 * Note you need to free those bh's.
 */
static void pitix_find_inode(struct super_block *s, unsigned long ino,
			     struct buffer_head **bh1, struct buffer_head **bh2,
			     char **zone1, unsigned int *len1,
			     char **zone2, unsigned int *len2)
{
	struct buffer_head *bh;
	unsigned int block_index, offset;
	unsigned int bytes_to_read;
	int remaining_bytes;
	char *start_addr1 = NULL, *start_addr2 = NULL, *last_addr;
	struct pitix_inode *test;

	block_index = ino * inode_size(s) / s->s_blocksize;
	printk(LOG_LEVEL "find_inode %lu block_index %d %d\n", ino, block_index, pitix_sbi(s)->izone_block);
	bh = sb_bread(s, pitix_sbi(s)->izone_block + block_index);
	*bh1 = bh;
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		goto out_free;
	}

	test = (struct pitix_inode*) (((char*) bh->b_data) + inode_size(s));
	printk(LOG_LEVEL "test %d %d\n", test->data_blocks[0], test->data_blocks[1]);
	printk(LOG_LEVEL "test mode %05o uid %d gid %d size %d time %d\n", test->mode, test->uid, test->gid, test->size, test->time);

	offset = (ino * inode_size(s)) % s->s_blocksize;
	start_addr1 = ((char*) bh->b_data) + offset;
	last_addr = ((char*) bh->b_data) + s->s_blocksize;
	printk(LOG_LEVEL "find_inode off %u st %p lst %p\n", offset, start_addr1, last_addr);

	/* Compute number of bytes we can read from current block. There
	 * is the possiblity that the size left in the block is smaller
	 * than the inode's actual size, meaning we need to read another
	 * block to actuall read the entire inode from disk.
	 */
	bytes_to_read = MIN(abs(last_addr - start_addr1 + 1), inode_size(s));
	remaining_bytes = MAX(0, inode_size(s) - bytes_to_read);

	printk(LOG_LEVEL "reached here0, bh %p\n", bh);

	/* Read the right next block in case we still haven't read
	 * inode_size(s). */
	if (remaining_bytes) {
		bh = sb_bread(s, pitix_sbi(s)->izone_block + block_index + 1);
		*bh2 = bh;
		start_addr2 = bh->b_data;
	}

	if (bh == NULL) {
		printk(LOG_LEVEL "could not read 2nd block\n");
		goto out_free;
	}

	*zone1 = start_addr1;
	*len1 = bytes_to_read;
	*zone2 = start_addr2;
	*len2 = remaining_bytes;

	printk(LOG_LEVEL "reached here1 bh1 %p bh2 %p z1 z2 %p %p l1 l2 %u %u\n", *bh1, *bh2, *zone1, *zone2, *len1, *len2);


	return;

out_free:
	printk(LOG_LEVEL "find_inode out_free\n");
	if (*bh1)
		brelse(*bh1);
	if (*bh2)
		brelse(*bh2);
	*bh1 = *bh2 = NULL;
	*zone1 = *zone2 = NULL;
}

static struct pitix_inode* pitix_get_inode(struct super_block *s,
					   unsigned long ino)
{
	struct buffer_head *bh1 = NULL, *bh2 = NULL;
	char *zone1 = NULL, *zone2 = NULL;
	int len1 = 0, len2 = 0;
	char *final_mii;

	/* Find the inode specified through two zones and their
	 * lengths from where memcpy it into another location.
	 * This way it can be casted to a (struct pitix_inode*).
	 */
	pitix_find_inode(s, ino, &bh1, &bh2, &zone1, &len1, &zone2, &len2);

	final_mii = kzalloc(inode_size(s), GFP_KERNEL);
	if (!final_mii) {
		printk(LOG_LEVEL "not enough memory\n");
		return NULL;
	}

	printk(LOG_LEVEL "memcpy1 %p %p %d\n", final_mii, zone1, len1);
	memcpy(final_mii, zone1, len1);
	printk(LOG_LEVEL "memcpy %p %p %d\n", final_mii + len1, zone2, len2);
	memcpy(final_mii + len1, zone2, len2);

	return (struct pitix_inode*) final_mii;
}

struct inode *pitix_iget(struct super_block *s, unsigned long ino)
{
	struct pitix_inode *mi;
	struct inode *inode;
	struct pitix_inode_info *mii;
	int i;

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
	mi = pitix_get_inode(s, ino);
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
		inode->i_op = &pitix_dir_inode_operations;
		inode->i_fop = &pitix_dir_operations;
	}

	/* fill data for mii */
	printk(LOG_LEVEL "----\n");
	mii = container_of(inode, struct pitix_inode_info, vfs_inode);
	for (i = 0 ; i < pitix_sbi(s)->inode_data_blocks; ++i) {
		printk(LOG_LEVEL "iget %d %d\n", i, mii->data_blocks[i]);
	}
	printk(LOG_LEVEL "----\n");
	memcpy(mii->data_blocks, mi->data_blocks,
	       pitix_sbi(s)->inode_data_blocks * sizeof(__u16));

	printk(LOG_LEVEL "found inode %lu %d\n", mii->vfs_inode.i_ino, mi->size);


	/* free resources */
	kfree(mi);
	unlock_new_inode(inode);

	printk(LOG_LEVEL "got inode %lu\n", ino);

	return inode;

out_bad_sb:
	iget_failed(inode);
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

/*
 * write VFS inode contents to disk inode
 */
int pitix_write_inode(struct inode *inode, struct
		writeback_control *wbc)
{
	struct super_block *sb = inode->i_sb;
	struct pitix_inode mi;
	struct pitix_inode_info *mii = container_of(inode,
			struct pitix_inode_info, vfs_inode);
	struct buffer_head *bh1 = NULL, *bh2 = NULL;
	char *zone1 = NULL, *zone2 = NULL;
	int err = 0, len1 = 0, len2 = 0, i;

	printk(LOG_LEVEL "in write before finding inode %lu\n", inode->i_ino);
	pitix_find_inode(sb, inode->i_ino, &bh1, &bh2, &zone1, &len1, &zone2,
			 &len2);
	printk(LOG_LEVEL "in write after; bh1 %p bh2 %p z1 %p z2 %p len1 %d len2 %d\n", bh1, bh2, zone1, zone2, len1, len2);
	if (bh1 == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		err = -ENOMEM;
		goto out;
	}

	/* fill disk inode */
	mi.mode = inode->i_mode;
	mi.uid = inode->i_uid;
	mi.gid = inode->i_gid;
	mi.size = inode->i_size;
	printk(LOG_LEVEL "nr inodes %d\n", pitix_sbi(sb)->inode_data_blocks);
	for (i = 0; i < pitix_sbi(sb)->inode_data_blocks; ++i)
		printk(LOG_LEVEL "nr inode (%d) %d\n", i, mii->data_blocks[i]);

	//for (i = 0; i < pitix_sbi(sb)->inode_data_blocks; ++i)
	//	mi.data_blocks[i] = mii->data_blocks[i];
	//memcpy(mi.data_blocks, mii->data_blocks,
	//       pitix_sbi(sb)->inode_data_blocks * sizeof(__u16));

	printk(LOG_LEVEL "mode is %05o; data_blocks at %d\n", mi.mode, mii->data_blocks[1]);
	printk(LOG_LEVEL "z1 %p z2 %p l1 %d l2 %d\n", zone1, zone2, len1, len2);
	printk(LOG_LEVEL "bh1  %p bh2 %p\n", bh1, bh2);

	memcpy(zone1, &mi, len1);
	printk(LOG_LEVEL "after memcpy1\n");
	memcpy(zone2, ((char*) &mi) + len1, len2);
	printk(LOG_LEVEL "after memcpy2\n");

	if (bh1) {
	        mark_buffer_dirty(bh1);
	        brelse(bh1);
	}
	if (bh2) {
	        mark_buffer_dirty(bh2);
	        brelse(bh2);
	}

	printk(LOG_LEVEL "wrote inode %lu\n", inode->i_ino);

out:
	return err;
}

static void pitix_put_super(struct super_block *sb)
{
	struct pitix_sb_info *sbi = sb->s_fs_info;

	/* free superblock buffer head */
	mark_buffer_dirty(sbi->sb_bh);
	brelse(sbi->sb_bh);

	printk(LOG_LEVEL "released superblock resources\n");
}

static const struct super_operations pitix_ops = {
	.statfs         = simple_statfs,
	.alloc_inode    = pitix_sb_alloc_inode,
	.destroy_inode  = pitix_sb_destroy_inode,
	//.write_inode	= pitix_write_inode,
	.put_super	= pitix_put_super,
};

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

	/* store superblock buffer_head for further use */
	sbi->sb_bh = bh;

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
