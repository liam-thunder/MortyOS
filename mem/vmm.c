#include "vmm.h"
#include "pmm.h"
#include "string.h"
#include "stdio.h"

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

	// Ref: http://wiki.osdev.org/Exceptions#Error_code
	if(regs->errorCode & 0x1) printf("Page-protection Violation\n");
	else printf("Non-present Page\n");

	if(regs->errorCode & 0x2) printf("Write Error\n");
	else printf("Read Error\n");

	if(regs->errorCode & 0x4) printf("Kernel Model\n");
	else printf("User Mode\n");

	if(regs->errorCode & 0x8) printf("Reserved Write\n");

	if(regs->errorCode & 0x10) printf("Instruction Fetch\n");

	while(1) ;
}

void map(pgd_t* pgd, uint32_t v_addr, uint32_t p_addr, uint32_t flags) {
	uint32_t pgd_idx = PGD_INDEX(v_addr);
	uint32_t pte_idx = PTE_INDEX(v_addr);

	pte_t *pte = (pte_t*) (pgd[pgd_idx] & PAGE_MASK);
	// create a new pte
	if(!pte) {
		// alloc physical page for this pte
		pte = (pte_t*) pmmAllocPage();
		pgd[pgd_idx] = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE;
		// change the pte to kernel linear addr
		pte = (pte_t*)((uint32_t)pte + PAGE_OFFSET);
		bzero(pte, PAGE_SIZE);
	} else {
		pte = (pte_t*)((uint32_t)pte + PAGE_OFFSET);
	}

	pte[pte_idx] = (p_addr & PAGE_MASK) | flags;
	// clear page cache
	asm volatile ("invlpg (%0)" : : "a" (v_addr));
}

void unmap(pgd_t* pgd, uint32_t v_addr) {
	uint32_t pgd_idx = PGD_INDEX(v_addr);
	uint32_t pte_idx = PTE_INDEX(v_addr);

	pte_t *pte = (pte_t*) (pgd[pgd_idx] & PAGE_MASK);

	pte = (pte_t*) ((uint32_t)pte + PAGE_OFFSET);

	if(!pte) return;
	pte[pte_idx] = 0;

	asm volatile ("invlpg (%0)" : : "a" (v_addr));
}

uint32_t getMapping(pgd_t* pgd, uint32_t v_addr, uint32_t* p_addr_ptr) {
	uint32_t pgd_idx = PGD_INDEX(v_addr);
	uint32_t pte_idx = PTE_INDEX(v_addr);

	pte_t *pte = (pte_t*) (pgd[pgd_idx] & PAGE_MASK);

	if(!pte) return 0;

	pte = (pte_t*)((uint32_t)pte + PAGE_OFFSET);

	if(pte[pte_idx] != 0 && p_addr_ptr) {
		*p_addr_ptr = pte[pte_idx] & PAGE_MASK;
		return 1;
	}
	return 0;
}