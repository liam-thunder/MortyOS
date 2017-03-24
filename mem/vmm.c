#include "vmm.h"
#include "pmm.h"
#include "string.h"
#include "stdio.h"

// set the start addr of pgd_kern and pte_kern to allign with PAGE_SIZE
pgd_t pgd_kern[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

pgd_t* current_pgd;

static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

static void switch_pgd(uint32_t pgd_addr);
static void enable_paging();
static void tlb_flush(uint32_t v_addr);

void init_vmm() {
    // init page dir and page tables 
    // map all 512MB physical addr to virtual addr are between 0xC0000000 to 0xE0000000
    uint32_t pte_start_idx = PGD_INDEX(PAGE_OFFSET);
    for(int i = pte_start_idx, j = 0; i < PTE_COUNT + pte_start_idx; i++, j++) {
        // pte_kern[j] presents the virtual addr of each page table
        // so here set up the map between page dir and page table
        pgd_kern[i] = ((uint32_t) pte_kern[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
    }
    uint32_t *pte = (uint32_t *) pte_kern;
    // map all 512mb physical to the kernel page tables
    // from 0x1000 -> 0x20000000(512MB)
    for(int i = 1; i < PTE_COUNT * PTE_SIZE; i++) {
        pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }
    uint32_t pgd_kern_phy_addr = (uint32_t)pgd_kern - PAGE_OFFSET;
    // reg page fault interrupt
    registersInterruptHandler(14, &pageFault);

    // the addr of pgd_kern and pte_kern should be aligned with PAGE_SIZE
    // otherwise switch will cause bug
    enable_paging();
    switch_pgd(pgd_kern_phy_addr);

}

void pageFault(registers_t *regs) {
    uint32_t cr2;
    asm volatile("mov %%cr2, %0" : "=r" (cr2));

    printf("Page fault at 0x%x, virtual faulting address 0x%x\n", regs->eip, cr2);
    printf("Error Code: %x\n", regs->error_code);

    // Ref: http://wiki.osdev.org/Exceptions#Error_code
    if(regs->error_code & 0x1) printf("Page-protection Violation\n");
    else printf("Non-present Page\n");

    if(regs->error_code & 0x2) printf("Write Error\n");
    else printf("Read Error\n");

    if(regs->error_code & 0x4) printf("Kernel Model\n");
    else printf("User Mode\n");

    if(regs->error_code & 0x8) printf("Reserved Write\n");

    if(regs->error_code & 0x10) printf("Instruction Fetch\n");

    while(1) ;
}

void map(pgd_t* pgd, uint32_t v_addr, uint32_t p_addr, uint32_t flags) {
    uint32_t pgd_idx = PGD_INDEX(v_addr);
    uint32_t pte_idx = PTE_INDEX(v_addr);

    pte_t *pte = (pte_t*) (pgd[pgd_idx] & PAGE_MASK);
    // create a new pte
    if(!pte) {
        // alloc physical page for this pte
        pte = (pte_t*) pmm_alloc_page();
        pgd[pgd_idx] = (uint32_t)pte | PAGE_PRESENT | PAGE_WRITE;
        // change the pte to kernel linear addr
        pte = (pte_t*)((uint32_t)pte + PAGE_OFFSET);
        bzero(pte, PAGE_SIZE);
    } else {
        pte = (pte_t*)((uint32_t)pte + PAGE_OFFSET);
    }

    pte[pte_idx] = (p_addr & PAGE_MASK) | flags;

    tlb_flush(v_addr);
}

void unmap(pgd_t* pgd, uint32_t v_addr) {
    uint32_t pgd_idx = PGD_INDEX(v_addr);
    uint32_t pte_idx = PTE_INDEX(v_addr);

    pte_t *pte = (pte_t*) (pgd[pgd_idx] & PAGE_MASK);

    pte = (pte_t*) ((uint32_t)pte + PAGE_OFFSET);

    if(!pte) return;
    pte[pte_idx] = 0;

    tlb_flush(v_addr);
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

/**
 * we need paging to locate codes so it's impossible to disable paging here.
 * thus it's impossible to copy the physical frame page directly.
 * so here the physical addr is added with PAGE_OFFSET to get a virual addr
 * that lies between 0xC0000000 and 0xE0000000, which is already mapped in 
 * inital stage of vmm.
 */
void clone_pgd(pgd_t* pgd_dst, pgd_t* pgd_src) {
    for(uint32_t i = 0; i < PGD_SIZE; i++) {
        pte_t *pte_src = (pte_t*) (pgd_src[i] & PAGE_MASK);
        pte_t *pte_src_vir = (pte_t*)((uint32_t)pte_src + PAGE_OFFSET);
        if(!pte_src) continue;
        pte_t* pte_dst = (pte_t*) pmm_alloc_page();
        pgd_dst[i] = (uint32_t) pte_dst | PAGE_PRESENT | PAGE_WRITE;
        pte_t* pte_dst_vir = (pte_t*)((uint32_t)pte_dst + PAGE_OFFSET);
        for(uint32_t j = 0; j < PTE_SIZE; j++)
            pte_dst_vir[j] = pte_src_vir[j];
    }
}

static void enable_paging() {
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r" (cr0));
}

static void switch_pgd(uint32_t pgd_addr) {
    current_pgd = (pgd_t*)pgd_addr;
    asm volatile ("mov %0, %%cr3" : : "r" (pgd_addr));
}

static void tlb_flush(uint32_t v_addr) {
    asm volatile ("invlpg (%0)" : : "a" (v_addr));
}