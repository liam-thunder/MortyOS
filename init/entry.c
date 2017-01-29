#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"

int kernEntry() {
	initGDT();
	initIDT();

    consoleClear();
    initTimer(200);

    printf("Hello Morty OS New!\n");
    printf("kernel in memory start: 0x%08X\n", kern_start);
    printf("kernel in memory end:   0x%08X\n", kern_end);
    printf("kernel in memory used:   %d KB\n\n", (kern_end - kern_start + 1023) / 1024);
    showMemMap();

    initPMM();

    printf("\nThe Count of Physical Memory Page is: %u\n\n", phy_page_cnt);

    uint32_t allc_addr = NULL;
    printf("Test Physical Memory Alloc :\n");
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmmAllocPage();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    return 0;
}