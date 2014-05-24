#include <linux/buffer_head.h>

#include "pitix.h"

int pitix_get_block(struct inode *inode, sector_t iblock, struct buffer_head *bh, int create )
{
	struct pitix_inode_info *pii = container_of(inode,
			struct pitix_inode_info, vfs_inode);
	struct super_block *sb = inode->i_sb;

	int block = pii->data_blocks[iblock];

	printk(LOG_LEVEL "get_block inode=[%ld] iblock=[%ld]\n", inode->i_ino, iblock);
	if (block < get_blocks(sb))
		map_bh(bh, inode->i_sb, pitix_sbi(sb)->dzone_block + block);

	return 0;
}

static int pitix_readpage(struct file *file, struct page *page)
{
	return block_read_full_page(page,pitix_get_block);
}

static sector_t pitix_bmap(struct address_space *mapping, sector_t block)
{
	return generic_block_bmap(mapping,block,pitix_get_block);
}

struct address_space_operations pitix_aops = {
	.readpage	= pitix_readpage,
	.bmap		= pitix_bmap,
};
