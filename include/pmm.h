#ifndef PMM_H
#define PMM_H
#include "multiboot.h"
#include "stdio.h"

// size of process stack
#define STACK_SIZE 8192

// max size of physical memory (512MB)
#define PMM_MAX_SIZE 0x20000000

// size of physical memory page frame (4KB)
#define PMM_PAGE_SIZE 0x1000

// max number of physical memory page
#define PMM_PAGE_MAX_NUM (PMM_MAX_SIZE / PMM_PAGE_SIZE)

// start address and end address of kernel in physical memory
extern uint8_t kern_start[];
extern uint8_t kern_end[];

extern uint32_t phy_page_cnt;

extern uint32_t kern_stack_top;

void showMemMap();

void initPMM();

uint32_t pmmAllocPage();

void pmmFreePage(uint32_t p);

#endif