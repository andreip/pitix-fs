#include <linux/buffer_head.h>

#include "pitix.h"

static int pitix_add_link(struct dentry *dentry, struct inode *inode)
{
	struct buffer_head *bh;
	struct inode *dir = dentry->d_parent->d_inode;
	struct pitix_inode_info *mii = container_of(dir,
			struct pitix_inode_info, vfs_inode);
	struct super_block *sb = dir->i_sb;
	struct pitix_dir_entry *de;
	int i;
	int err = 0;

	/* Read the data zone of the dir (the inode's parent). */
	bh = sb_bread(sb, pitix_sbi(sb)->dzone_block + mii->data_blocks[0]);
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		err = -ENOMEM;
		goto out_bad_sb;
	}

	for (i = 0; i < dir_entries_per_block(sb); i++) {
		de = (struct pitix_dir_entry *) bh->b_data + i;
		if (de->ino == 0)
			break;
	}

	if (i == dir_entries_per_block(sb)) {
		err = -ENOSPC;
		goto out;
	}

	de->ino = inode->i_ino;
	memcpy(de->name, dentry->d_name.name, PITIX_NAME_LEN);
	dir->i_mtime = dir->i_ctime = CURRENT_TIME;

	mark_buffer_dirty(bh);
out:
	brelse(bh);
out_bad_sb:
	return err;
}

int pitix_create(struct inode *dir, struct dentry *dentry,
		umode_t mode, bool excl)
{
	struct inode *inode;
	struct pitix_inode_info *mii;
	int err;

	inode = pitix_new_inode(dir->i_sb);
	if (inode == NULL) {
		printk(LOG_LEVEL "error allocating new inode\n");
		err = -ENOMEM;
		goto err_new_inode;
	}

	inode->i_mode = mode;
	inode->i_op = &pitix_file_inode_operations;
	inode->i_fop = &pitix_file_operations;
	mii = container_of(inode, struct pitix_inode_info, vfs_inode);

	err = pitix_add_link(dentry, inode);
	if (err != 0)
		goto err_add_link;

	d_instantiate(dentry, inode);
	mark_inode_dirty(inode);

	printk(LOG_LEVEL "new file inode created (ino = %lu)\n", inode->i_ino);

	return 0;

err_add_link:
	inode_dec_link_count(inode);
	iput(inode);
err_new_inode:
	return err;
}


struct file_operations pitix_file_operations = {
	.read		= do_sync_read,
	.aio_read	= generic_file_aio_read,
	.write		= do_sync_write,
	.aio_write	= generic_file_aio_write,
	.mmap		= generic_file_mmap,
	.fsync		= noop_fsync,
	.splice_read	= generic_file_splice_read,
	.splice_write	= generic_file_splice_write,
	.llseek		= generic_file_llseek,
};

struct inode_operations pitix_file_inode_operations = {
	.getattr	= simple_getattr,
};
