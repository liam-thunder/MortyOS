#include "vsfs.h"

//const uint32_t root = 2;

static void init_bitmap(void* s_addr);

inode_t* get_inode_ptr(super_block_t* vsb, uint32_t inode_idx) {
    return (inode_t*)((uint32_t)vsb->inode_region + BLOCK_SIZE * inode_idx);
}

block_t* get_block_ptr(super_block_t* vsb, uint32_t block_idx) {
    return (block_t*)((uint32_t)vsb->data_region + BLOCK_SIZE * block_idx);
}

void* get_block_data_ptr(super_block_t* vsb, uint32_t block_idx) {
    return (void*)((uint32_t)vsb->data_region + BLOCK_SIZE * block_idx + sizeof(block_t));
}

super_block_t* init_superblock(uint32_t s_addr) {
    super_block_t* vsb = (super_block_t*) s_addr;

    vsb->block_num = FREE_BLOCK_NUM;
    vsb->inode_num = (INODE_BLOCK_NUM * BLOCK_SIZE) / sizeof(inode_t);

    vsb->inode_bitmap = (bitmap_t*)((uint32_t)s_addr + 4096);
    vsb->data_bitmap = (bitmap_t*)((uint32_t)s_addr + 4096 * 2);
    vsb->inode_region = (inode_t*)((uint32_t)s_addr + 4096 * 3);
    vsb->data_region = (block_t*)((uint32_t)s_addr + 4096 * 8);

    init_bitmap(vsb->inode_bitmap);
    init_bitmap(vsb->data_bitmap);
    return vsb;
}

static void init_bitmap(void* s_addr) {
    bitmap_t* ptr = (bitmap_t*) s_addr;
    for (int i = 0; i < BITMAP_SIZE; i++)
        ptr->bit_space[i] = 0x0;
}