#ifndef __page_combine_H__
#define __page_combine_H__

#include "config.h"

void combine_page();
uint8_t * read_bin_file(const char* filename);
void pr_buffer(uint8_t *buffer);
#define blk_width  4
#define blk_height 4
#define blk_num_per_patch_width  (patch_width/4)
#define blk_num_per_patch_height (patch_height/4)

typedef struct 
{
    uint8_t red[blk_height][blk_width];
    uint8_t green[blk_height][blk_width];
    uint8_t blue[blk_height][blk_width];
}block_t;

typedef struct 
{
    int blk_num;
    block_t blkList[blk_num_per_patch_height][blk_num_per_patch_width];
}patch_t;





#endif //__page_combine_H__
 