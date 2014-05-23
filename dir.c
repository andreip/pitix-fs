#include <linux/buffer_head.h>

#include "pitix.h"

static int pitix_readdir(struct file *filp, struct dir_context *ctx)
{
	struct buffer_head *bh = NULL;
	struct pitix_dir_entry *de;
	struct inode *inode = file_inode(filp);
	struct pitix_inode_info *mii = container_of(inode,
			struct pitix_inode_info, vfs_inode);
	struct super_block *sb = inode->i_sb;
	int err = 0;
	int over, i;

	/* read data block for directory inode */
	bh = sb_bread(sb, pitix_sbi(sb)->dzone_block + mii->data_blocks[0] );
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		err = -ENOMEM;
		goto out_bad_sb;
	}
	printk("Read data block %d for folder %s\n", mii->data_blocks[0],
			filp->f_path.dentry->d_name.name);

	
	for (; ctx->pos < dir_entries_per_block(sb); ctx->pos++) {
		de = (struct pitix_dir_entry *) bh->b_data + ctx->pos;
		if (de->ino != 0) {
			over = dir_emit(ctx, de->name, PITIX_NAME_LEN,
					de->ino, DT_UNKNOWN);
			if (over) {
				printk(LOG_LEVEL "Read %s from folder %s, ctx->pos: %lld\n",
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

struct file_operations pitix_dir_operations = {
	.read		= generic_read_dir,
	.iterate	= pitix_readdir,
};

struct inode_operations pitix_dir_inode_operations =
{
	.lookup         = pitix_lookup,
};
