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
	//root_inode = pitix_iget(sb, 0);
	root_inode = myfs_get_inode(sb, S_IFDIR | S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	root_inode->i_ino = 1;
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
