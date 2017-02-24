#ifndef VSFS_H 
#define VSFS_H

#include "types.h"
#include "vfs.h"

// Based on Operating Systems: Three Easy Pieces
// Ref http://pages.cs.wisc.edu/~remzi/OSTEP/

#define BLOCK_SIZE 4096
#define BLOCK_NUM 64
#define BITMAP_SIZE (BLOCK_SIZE / sizeof(uint32_t))
#define DIR_MAX_NUM 32
#define MAX_FILE_NAME_LEN 15

typedef struct bitmap {
    uint32_t bit_space[BITMAP_SIZE];
} bitmap_t;

typedef struct inode {
    uint16_t file_type;
    uint32_t file_size;
    uint32_t block_num;
    uint32_t block_idx[15];
    //pad to 256Bytes
    uint32_t padding[46];
} inode_t;

typedef struct block {
    uint8_t file_type;
    char file_name[DIR_MAX_NUM][MAX_FILE_NAME_LEN];
    uint32_t file_inode[DIR_MAX_NUM];
    uint32_t file_num;
    uint32_t block_size;
} block_t;

typedef struct super_block
{
    uint32_t inode_num;
    uint32_t block_num;
    bitmap_t* inode_bitmap;
    bitmap_t* data_bitmap;
    inode_t* inode_region;
    block_t* data_region;
} super_block_t;

#define FREE_BLOCK_SIZE (BLOCK_SIZE - sizeof(block_t))

inode_t* get_inode_ptr(super_block_t* vsb, uint32_t inode_idx);
block_t* get_block_ptr(super_block_t* vsb, uint32_t block_idx);
void* get_block_data_ptr(super_block_t* vsb, uint32_t block_idx);

#endif