/*
 * SO2 Lab - Filesystem drivers (part 2)
 * Type #2 (dev filesystem)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/pagemap.h>
#include <linux/buffer_head.h>
#include <linux/slab.h>

#include "minfs.h"

MODULE_DESCRIPTION("Simple filesystem");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");


#define LOG_LEVEL	KERN_ALERT
#define DEBUG		1

#if DEBUG == 1
#define dprintk(fmt, ...)	\
	printk(LOG_LEVEL fmt, ##__VA_ARGS__)
#else
#define dprintk(fmt, ...)	\
	do {} while (0)
#endif

struct minfs_sb_info {
	__u8 version;
	unsigned long imap;
	struct buffer_head *sbh;
};

struct minfs_inode_info {
	__u16 data_block;
	struct inode vfs_inode;
};

/*
 * forward declarations
 */

static const struct address_space_operations minfs_aops;
static const struct file_operations minfs_file_operations;
static struct inode_operations minfs_file_inode_operations;
static const struct file_operations minfs_dir_operations;
static struct inode_operations minfs_dir_inode_operations;

static struct inode *minfs_iget(struct super_block *s, unsigned long ino)
{
	struct minfs_inode *mi;
	struct buffer_head *bh;
	struct inode *inode;
	struct minfs_inode_info *mii;

	/* allocate VFS inode */
	inode = iget_locked(s, ino);
	if (inode == NULL) {
		printk(LOG_LEVEL "error aquiring inode\n");
		return ERR_PTR(-ENOMEM);
	}
	if (!(inode->i_state & I_NEW))
		return inode;

	/* read disk inode block */
	bh = sb_bread(s, MINFS_INODE_BLOCK);
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		goto out_bad_sb;
	}

	/* extract disk inode */
	mi = ((struct minfs_inode *) bh->b_data) + ino;

	/* fill VFS inode */
	inode->i_mode = mi->mode;
	inode->i_uid = mi->uid;
	inode->i_gid = mi->gid;
	inode->i_size = mi->size;
	inode->i_blocks = 0;
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
	inode->i_mapping->a_ops = &minfs_aops;

	if (S_ISDIR(inode->i_mode)) {
		inode->i_op = &minfs_dir_inode_operations;
		inode->i_fop = &minfs_dir_operations;

		/* directory inodes start off with i_nlink == 2 */
		inc_nlink(inode);
	}
	if (S_ISREG(inode->i_mode)) {
		inode->i_op = &minfs_file_inode_operations;
		inode->i_fop = &minfs_file_operations;
	}

	/* fill data for mii */
	mii = container_of(inode, struct minfs_inode_info, vfs_inode);
	mii->data_block = mi->data_block;

	/* free resources */
	brelse(bh);
	unlock_new_inode(inode);

	dprintk("got inode %lu\n", ino);

	return inode;

out_bad_sb:
	iget_failed(inode);
	return NULL;
}

static const struct address_space_operations minfs_aops = {
	.readpage       = simple_readpage,
	.write_begin    = simple_write_begin,
	.write_end      = simple_write_end,
};

static const struct file_operations minfs_file_operations = {
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

static struct inode_operations minfs_file_inode_operations = {
	.getattr	= simple_getattr,
};

static int minfs_readdir(struct file *filp, struct dir_context *ctx)
{
	struct buffer_head *bh;
	struct minfs_dir_entry *de;
	struct inode *inode = file_inode(filp);
	struct minfs_inode_info *mii = container_of(inode,
			struct minfs_inode_info, vfs_inode);
	struct super_block *sb = inode->i_sb;
	int err = 0;
	int over;

	/* read data block for directory inode */
	bh = sb_bread(sb, mii->data_block);
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		err = -ENOMEM;
		goto out_bad_sb;
	}
	dprintk("Read data block %d for folder %s\n", mii->data_block,
			filp->f_path.dentry->d_name.name);

	for (; ctx->pos < MINFS_NUM_ENTRIES; ctx->pos++) {
		de = (struct minfs_dir_entry *) bh->b_data + ctx->pos;
		if (de->ino != 0) {
			over = dir_emit(ctx, de->name, MINFS_NAME_LEN,
					de->ino, DT_UNKNOWN);
			if (over) {
				dprintk("Read %s from folder %s, ctx->pos: %lld\n",
						de->name,
						filp->f_path.dentry->d_name.name,
						ctx->pos);
				ctx->pos += 1;
				goto done;
			}
		}
	}

done:
	brelse(bh);
out_bad_sb:
	return err;
}

static const struct file_operations minfs_dir_operations = {
	.read		= generic_read_dir,
	.iterate	= minfs_readdir,
};

/*
 * find dentry in parent folder; return parent folder's data buffer_head
 */

static struct minfs_dir_entry *minfs_find_entry(struct dentry *dentry,
		struct buffer_head **bhp)
{
	struct buffer_head *bh;
	struct inode *dir = dentry->d_parent->d_inode;
	struct minfs_inode_info *mii = container_of(dir,
			struct minfs_inode_info, vfs_inode);
	struct super_block *sb = dir->i_sb;
	const char *name = dentry->d_name.name;
	struct minfs_dir_entry *final_de = NULL;
	struct minfs_dir_entry *de;
	size_t i;

	/* read parent folder data block (contains dentries) */
	bh = sb_bread(sb, mii->data_block);
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		return NULL;
	}
	*bhp = bh;

	dprintk("Looking for dentry name %s in parent folder %s\n",
			name, dentry->d_parent->d_name.name);
	/* traverse all entries */
	for (i = 0; i < MINFS_NUM_ENTRIES; i++) {
		de = ((struct minfs_dir_entry *) bh->b_data) + i;
		if (de->ino != 0) {
			/* found it */
			if (strcmp(name, de->name) == 0) {
				dprintk("Found entry %s on position: %zd\n",
						name, i);
				final_de = de;
				break;
			}
		}
	}

	/* bh needs to be released by caller */
	return final_de;
}

static struct dentry *minfs_lookup(struct inode *dir,
		struct dentry *dentry, unsigned int flags)
{
	struct super_block *sb = dir->i_sb;
	struct minfs_dir_entry *de;
	struct buffer_head *bh = NULL;
	struct inode *inode = NULL;

	dentry->d_op = sb->s_root->d_op;

	de = minfs_find_entry(dentry, &bh);
	if (de != NULL) {
		dprintk("getting entry: name: %s, ino: %d\n",
				de->name, de->ino);
		inode = minfs_iget(sb, de->ino);
		if (IS_ERR(inode))
			return ERR_CAST(inode);
	}

	d_add(dentry, inode);
	brelse(bh);

	dprintk("looked up dentry %s\n", dentry->d_name.name);

	return NULL;
}

/*
 * create a new VFS inode, do basic initialization and fill imap
 */

static struct inode *minfs_new_inode(struct inode *dir)
{
	struct super_block *sb = dir->i_sb;
	struct minfs_sb_info *sbi = sb->s_fs_info;
	struct inode *inode;
	int idx;

	idx = find_first_zero_bit(&sbi->imap, MINFS_NUM_INODES);
	if (idx < 0) {
		printk(LOG_LEVEL "no space left in imap\n");
		return NULL;
	}

	__test_and_set_bit(idx, &sbi->imap);
	mark_buffer_dirty(sbi->sbh);

	inode = new_inode(sb);
	inode->i_uid = current_fsuid();
	inode->i_gid = current_fsgid();
	inode->i_ino = idx;
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
	inode->i_blocks = 0;

	insert_inode_hash(inode);

	return inode;
}

/*
 * add dentry link on parent inode disk structure
 */

static int minfs_add_link(struct dentry *dentry, struct inode *inode)
{
	struct buffer_head *bh;
	struct inode *dir = dentry->d_parent->d_inode;
	struct minfs_inode_info *mii = container_of(dir,
			struct minfs_inode_info, vfs_inode);
	struct super_block *sb = dir->i_sb;
	struct minfs_dir_entry *de;
	int i;
	int err = 0;

	bh = sb_bread(sb, mii->data_block);

	for (i = 0; i < MINFS_NUM_ENTRIES; i++) {
		de = (struct minfs_dir_entry *) bh->b_data + i;
		if (de->ino == 0)
			break;
	}

	if (i == MINFS_NUM_ENTRIES) {
		err = -ENOSPC;
		goto out;
	}

	de->ino = inode->i_ino;
	memcpy(de->name, dentry->d_name.name, MINFS_NAME_LEN);
	dir->i_mtime = dir->i_ctime = CURRENT_TIME;

	mark_buffer_dirty(bh);
out:
	brelse(bh);

	return err;
}

/*
 * create a VFS file inode; use simple operations
 */

static int minfs_create(struct inode *dir, struct dentry *dentry,
		umode_t mode, bool excl)
{
	struct inode *inode;
	struct minfs_inode_info *mii;
	int err;

	inode = minfs_new_inode(dir);
	if (inode == NULL) {
		printk(LOG_LEVEL "error allocating new inode\n");
		err = -ENOMEM;
		goto err_new_inode;
	}

	inode->i_mode = mode;
	inode->i_op = &minfs_file_inode_operations;
	inode->i_fop = &minfs_file_operations;
	mii = container_of(inode, struct minfs_inode_info, vfs_inode);
	mii->data_block = MINFS_FIRST_DATA_BLOCK + inode->i_ino;

	err = minfs_add_link(dentry, inode);
	if (err != 0)
		goto err_add_link;

	d_instantiate(dentry, inode);
	mark_inode_dirty(inode);

	dprintk("new file inode created (ino = %lu)\n", inode->i_ino);

	return 0;

err_add_link:
	inode_dec_link_count(inode);
	iput(inode);
err_new_inode:
	return err;
}

static struct inode_operations minfs_dir_inode_operations = {
	.lookup		= minfs_lookup,
	.create		= minfs_create,
};

/*
 * allocate and initialize VFS inode; filling is done in new_inode or
 * minfs_iget
 */

static struct inode *minfs_alloc_inode(struct super_block *s)
{
	struct minfs_inode_info *mii;

	/* allocate minfs_inode_info */
	mii = kzalloc(sizeof(struct minfs_inode_info), GFP_KERNEL);
	if (mii == NULL)
		return NULL;

	inode_init_once(&mii->vfs_inode);

	return &mii->vfs_inode;
}

static void minfs_destroy_inode(struct inode *inode)
{
	kfree(container_of(inode, struct minfs_inode_info, vfs_inode));
}

/*
 * write VFS inode contents to disk inode
 */

static int minfs_write_inode(struct inode *inode, struct
		writeback_control *wbc)
{
	struct super_block *sb = inode->i_sb;
	struct minfs_inode *mi;
	struct minfs_inode_info *mii = container_of(inode,
			struct minfs_inode_info, vfs_inode);
	struct buffer_head *bh;
	int err = 0;

	bh = sb_bread(sb, MINFS_INODE_BLOCK);
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		err = -ENOMEM;
		goto out;
	}

	mi = (struct minfs_inode *) bh->b_data + inode->i_ino;

	/* fill disk inode */
	mi->mode = inode->i_mode;
	mi->uid = inode->i_uid;
	mi->gid = inode->i_gid;
	mi->size = inode->i_size;
	mi->data_block = mii->data_block;

	dprintk("mode is %05o; data_block is %d\n", mi->mode, mii->data_block);

	mark_buffer_dirty(bh);
	brelse(bh);

	dprintk("wrote inode %lu\n", inode->i_ino);

out:
	return err;
}

static void minfs_put_super(struct super_block *sb)
{
	struct minfs_sb_info *sbi = sb->s_fs_info;

	/* free superblock buffer head */
	mark_buffer_dirty(sbi->sbh);
	brelse(sbi->sbh);

	dprintk("released superblock resources\n");
}

static const struct super_operations minfs_ops = {
	.statfs		= simple_statfs,
	.alloc_inode	= minfs_alloc_inode,
	.destroy_inode	= minfs_destroy_inode,
	.write_inode	= minfs_write_inode,
	.put_super	= minfs_put_super,
};

static int minfs_fill_super(struct super_block *s, void *data, int silent)
{
	struct minfs_sb_info *sbi;
	struct minfs_super_block *ms;
	struct inode *root_inode;
	struct dentry *root_dentry;
	struct buffer_head *bh;
	int ret = -EINVAL;

	sbi = kzalloc(sizeof(struct minfs_sb_info), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;
	s->s_fs_info = sbi;

	/* set block size for superblock */
	if (!sb_set_blocksize(s, MINFS_BLOCK_SIZE))
		goto out_bad_blocksize;

	/* read first block from disk (contains disk superblock) */
	bh = sb_bread(s, MINFS_SUPER_BLOCK);
	if (bh == NULL)
		goto out_bad_sb;

	/* extract disk superblock */
	ms = (struct minfs_super_block *) bh->b_data;

	/* fill sbi with information from disk superblock */
	if (ms->magic == MINFS_MAGIC) {
		sbi->version = ms->version;
		sbi->imap = ms->imap;
	} else
		goto out_bad_magic;

	s->s_magic = MINFS_MAGIC;
	s->s_op = &minfs_ops;

	/* allocate root inode and root dentry */
	root_inode = minfs_iget(s, MINFS_ROOT_INODE);
	if (!root_inode)
		goto out_bad_inode;

	root_dentry = d_make_root(root_inode);
	if (!root_dentry)
		goto out_iput;
	s->s_root = root_dentry;

	/* store superblock buffer_head for further use */
	sbi->sbh = bh;

	dprintk("superblock filled\n");

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
	s->s_fs_info = NULL;
	kfree(sbi);
	return ret;
}

static struct dentry *minfs_mount(struct file_system_type *fs_type,
		int flags, const char *dev_name, void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data,
			minfs_fill_super);
}

static struct file_system_type minfs_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "minfs",
	.mount		= minfs_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};

static int __init minfs_init(void)
{
	int err;

	err = register_filesystem(&minfs_fs_type);
	if (err) {
		printk(LOG_LEVEL "register_filesystem failed\n");
		return err;
	}

	dprintk("registered filesystem\n");

	return 0;
}

static void __exit minfs_exit(void)
{
	unregister_filesystem(&minfs_fs_type);
	dprintk("unregistered filesystem\n");
}

module_init(minfs_init);
module_exit(minfs_exit);
