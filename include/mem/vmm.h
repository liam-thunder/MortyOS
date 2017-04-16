#ifndef VMM_H
#define VMM_H
#include "types.h"
#include "idt.h"

// Page Global Directory
typedef uint32_t pgd_t;

// Page Table Entry
typedef uint32_t pte_t;

// offset of kernel
#define PAGE_OFFSET 0xC0000000

// Ref: http://wiki.osdev.org/Paging
// to understand of these bits
#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

#define PAGE_SIZE 4096

// get higher 10 bits of an linear address
// which is the physical addr of page table 
#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)

// get middle 10 bits of an linear address
// which is the physical addr of physical mem frame
#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)

// get lower 12 bits of and linear address
// which is the offset in a page
#define OFFSET_INDEX(x) ((x) & 0xFFF)

/**
 * num of page dir entry 
 * use higher 10 bits to address page table addr,
 * so the size of page directory table is 1024(0 ~ 2^10-1)
 */
//#define PGD_SIZE (PAGE_SIZE / sizeof(pte_t))
#define PGD_SIZE 1024

/**
 * num of page table entry 
 * use middle 10 bits to address physical page addr,
 * so the size is also 1024(0 ~ 2^10-1)
 */
// #define PTE_SIZE (PAGE_SIZE / sizeof(uint32_t))
#define PTE_SIZE 1024

/**
 * num of page table
 * since we manage the 512MB of memory at most
 * so there will be at most 512MB / 4096K = 128 page tables
 */ 
#define PTE_COUNT 128

// use mask to clean the last 12 bits
#define PAGE_MASK 0xFFFFF000

void init_vmm();

void page_fault(registers_t *regs);

// map and unmap physical addr to virtual addr
void map(pgd_t* pgd, uint32_t v_addr, uint32_t p_addr, uint32_t flags);
void unmap(pgd_t* pgd, uint32_t v_addr);
// get map info
uint32_t getMapping(pgd_t* pgd, uint32_t v_addr, uint32_t* p_addr_ptr);

extern pgd_t pgd_kern[PGD_SIZE];

extern pgd_t* current_ptr;

void clone_pgd(pgd_t* pgd_dst, pgd_t* pgd_src);

void create_init_uvm(pgd_t* pgd, char* init_code, uint32_t size);

#endif