#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "string.h"
#include "vmm.h"

int kernInit();

multiboot_t* glb_mboot_ptr;

char kern_stack[STACK_SIZE];

// Ref: http://wiki.osdev.org/Higher_Half_Kernel
// 0x00000000 - 0xBFFFFFFF for user
// 0xC0000000 - 0xFFFFFFFF for kernel
// temp page table and kernEnrty() are stored in .init.data and .init.text section
// temp page directory starts at addr 0x1000
__attribute__((section(".init.data"))) pgd_t *pgd_tmp  = (pgd_t *)0x1000;

// temp page table low and high start at 0x2000 and 0x3000
__attribute__((section(".init.data"))) pgd_t *pte_low  = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

__attribute__((section(".init.text"))) void kernEntry() {
    // low page table entry
    pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
    // map high page table entry to PGD_INDEX(0xC0000000)
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;

    // Map 4MB to virual addr 0x00000000-0x00400000 
    // and 0xC0000000-0xC0400000 in two page table
    for(int i = 0; i < 1024; i++) {
        pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
        pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }

    // copy the location of page directory into the CR3 register
    asm volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));
    uint32_t cr0;
    // enable paging
    asm volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r" (cr0));

    // set stack top
    uint32_t kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
    asm volatile ("mov %0, %%esp\n\t"
            "xor %%ebp, %%ebp" : : "r" (kern_stack_top));

    // move multiboot pointer
    glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

    kernInit();
}

int kernInit() {
    consoleClear();
    initTimer(200);

    printf("Hello Morty OS New!\n");
    
	initGDT();
	initIDT();

    consoleClear();
    initTimer(200);

    printf("Hello Morty OS New!\n");
    printf("kernel in memory start: 0x%08X\n", kern_start);
    printf("kernel in memory end:   0x%08X\n", kern_end);
    printf("kernel in memory used:   %d KB\n", (kern_end - kern_start + 1023) / 1024);
    //showMemMap();

    initPMM();

    printf("\nThe Count of Physical Memory Page is: %u\n", phy_page_cnt);

    /*uint32_t allc_addr = NULL;
    printf("Test Physical Memory Alloc :\n");
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);*/

    initVMM();  
    uint32_t *ptr = (uint32_t*) 0xA0000000;
    uint32_t test = *ptr;

    while (1) {
        asm volatile ("hlt");
    }
    return 0;
}