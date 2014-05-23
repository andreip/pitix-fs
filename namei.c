#include <linux/buffer_head.h>

#include "pitix.h"

/*
 * find dentry in parent folder; return parent folder's data buffer_head
 */

static struct pitix_dir_entry *pitix_find_entry(struct dentry *dentry,
		struct buffer_head **bhp)
{
	struct buffer_head *bh;
	struct inode *dir = dentry->d_parent->d_inode;
	struct pitix_inode_info *mii = container_of(dir,
			struct pitix_inode_info, vfs_inode);
	struct super_block *sb = dir->i_sb;
	const char *name = dentry->d_name.name;
	struct pitix_dir_entry *final_de = NULL;
	struct pitix_dir_entry *de;
	size_t i;

	/* read parent folder data block (contains dentries) */
	bh = sb_bread(sb, pitix_sbi(sb)->dzone_block + mii->data_blocks[0]);
	if (bh == NULL) {
		printk(LOG_LEVEL "could not read block\n");
		return NULL;
	}
	*bhp = bh;

	printk(LOG_LEVEL "Looking for dentry name %s in parent folder %s\n",
	       name, dentry->d_parent->d_name.name);
	/* traverse all entries */
	for (i = 0; i < dir_entries_per_block(sb); i++) {
		de = ((struct pitix_dir_entry *) bh->b_data) + i;
		if (de->ino != 0) {
			/* found it */
			if (strcmp(name, de->name) == 0) {
				printk(LOG_LEVEL "Found entry %s on position: %zd\n",
						name, i);
				final_de = de;
				break;
			}
		}
	}

	/* bh needs to be released by caller */
	return final_de;
}

struct dentry *pitix_lookup(struct inode *dir,
		struct dentry *dentry, unsigned int flags)
{
	struct super_block *sb = dir->i_sb;
	struct pitix_dir_entry *de;
	struct buffer_head *bh = NULL;
	struct inode *inode = NULL;

	dentry->d_op = sb->s_root->d_op;

	de = pitix_find_entry(dentry, &bh);
	if (de != NULL) {
		printk(LOG_LEVEL "getting entry: name: %s, ino: %d\n",
				de->name, de->ino);
		inode = pitix_iget(sb, de->ino);
		if (IS_ERR(inode))
			return ERR_CAST(inode);
	}

	d_add(dentry, inode);
	brelse(bh);

	printk(LOG_LEVEL "looked up dentry %s\n", dentry->d_name.name);

	return NULL;
}
