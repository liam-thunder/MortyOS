#ifndef PMM_H
#define PMM_H
#include "multiboot.h"
#include "common/stdio.h"

// size of process stack
#define STACK_SIZE 8192

// max size of physical memory supported here(512MB)
#define PMM_MAX_SIZE 0x20000000

// size of physical memory page frame (4KB)
#define PMM_PAGE_SIZE 0x1000

// max number of physical memory page
#define PMM_PAGE_MAX_NUM (PMM_MAX_SIZE / PMM_PAGE_SIZE)

// start address and end address of kernel in physical memory
extern uint8_t kern_start[];
extern uint8_t kern_end[];

extern uint32_t phy_page_cnt;

extern uintptr_t kern_stack_top;
extern uintptr_t kern_stack_ptr;

void showMemMap();

void init_pmm();

uint32_t pmm_alloc_page();

void pmm_free_page(uint32_t p);

#endif