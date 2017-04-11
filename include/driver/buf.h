#ifndef BUF_H
#define BUF_H

#include "types.h"

#define B_BUSY 0x1   // buffer has been locked
#define B_VALID 0x2  // buffer has been read from disk
#define B_DIRTY 0x3  // buffer needs to be written to disk

// defined here temporarily
// identical to the IDEs sector size 
#define B_SIZE 512 

struct buf {
    int32_t flags;
    uint32_t dev; 
    uint32_t block_num; 
    struct buf* prev;
    struct buf* next;
    struct buf* qnext; 
    uint8_t data[B_SIZE];
};


#endif