#include "vmm.h"

pgd_t pgd_kern[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

void initVMM() {
	uint32_t pte_start_idx = PGD_INDEX(PAGE_OFFSET);
    for(int i = pte_start_idx, j = 0; i < PTE_COUNT + pte_start_idx; i++, j++) {
    	// pte_kern[j] presents the virtual addr of each page table
		// so here set up the map between page dir and page table
        pgd_kern[i] = ((uint32_t) pte_kern[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
    }
    uint32_t *pte = (uint32_t *) pte_kern;
    for(int i = 1; i < PTE_COUNT * PTE_SIZE; i++) {
        pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    uint32_t pgd_kern_phy_addr = (uint32_t)pgd_kern - PAGE_OFFSET;

    // reg page fault interrupt
    registersInterruptHandler(14, &pageFault);

    switchPGD(pgd_kern_phy_addr);
}

void switchPGD(uint32_t pgd_addr) {
	asm volatile ("mov %0, %%cr3" : : "r" (pgd_addr));
}

void pageFault(registers_t *regs) {
	uint32_t cr2;
	asm volatile("mov %%cr2, %0" : "=r" (cr2));

	printf("Page fault at 0x%x, virtual faulting address 0x%x\n", regs->eip, cr2);
	printf("Error Code: %x\n", regs->errorCode);
}

void map(pgd_t* pgd, uint32_t v_addr, uint32_t p_addr, uint32_t flag) {
	uint32_t pgd_idx = PGD_INDEX(v_addr);
	uint32_t pte_idx = PTE_INDEX(v_addr);

	pte_t *p = (pte_t*) (pgd[pgd_idx] & PAGE_MASK);
	if(!p) {
		p = (pte_t*) pmmAllocPage();
	}
}