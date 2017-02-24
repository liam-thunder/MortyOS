#include "vsfs.h"

const uint32_t root = 2;

inode_t* get_inode_ptr(super_block_t* vsb, uint32_t inode_idx) {
    return (inode_t*)((uint32_t)vsb->inode_region + BLOCK_SIZE * inode_idx);
}

block_t* get_block_ptr(super_block_t* vsb, uint32_t block_idx) {
    return (block_t*)((uint32_t)vsb->data_region + BLOCK_SIZE * block_idx);
}

void* get_block_data_ptr(super_block_t* vsb, uint32_t block_idx) {
    return (void*)((uint32_t)vsb->data_region + BLOCK_SIZE * block_idx + sizeof(block_t*));
}
