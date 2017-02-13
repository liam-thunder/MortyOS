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
// which is the page table address
#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)

// get middle 10 bits of an linear address
// which is the physical page address
#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)

// get lower 12 bits of and linear address
// which is the offset in a page
#define OFFSET_INDEX(x) ((x) & 0xFFF)

// num of page dir entry
#define PGD_SIZE (PAGE_SIZE / sizeof(pte_t))

// num of page table entry
#define PTE_SIZE (PAGE_SIZE / sizeof(uint32_t))

// num of page table (512MB / 4096K = 128)
#define PTE_COUNT 128

// use mask to clean the last 12 bits
#define PAGE_MASK 0xFFFFF000

void initVMM();
void pageFault(registers_t *regs);
void switchPGD(uint32_t pgd_addr);
// map and unmap physical addr to virtual addr
void map(pgd_t* pgd, uint32_t v_addr, uint32_t p_addr, uint32_t flags);
void unmap(pgd_t* pgd, uint32_t v_addr);
// get map info
uint32_t getMapping(pgd_t* pgd, uint32_t v_addr, uint32_t* p_addr_ptr);

extern pgd_t pgd_kern[PGD_SIZE];

#endif