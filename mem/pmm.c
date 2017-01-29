#include "pmm.h"

static uint32_t pmm_stack[PMM_PAGE_MAX_NUM + 1];

static uint32_t pmm_stack_top;

uint32_t phy_page_cnt;

void showMemMap() {
	uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
	uint32_t mmap_length = glb_mboot_ptr->mmap_length;

	printf("Memory Map:\n");

	mmap_entry_t* mmap = (mmap_entry_t *) mmap_addr;
	for(; (uint32_t)mmap < mmap_addr + mmap_length; mmap++)
		printf("base address = 0x%X%08X, length = 0x%X%08X, type = 0x%X\n",
				(uint32_t)mmap->addrHigh, (uint32_t)mmap->addrLow, 
				(uint32_t)mmap->lenHigh, (uint32_t)mmap->lenLow,
				(uint32_t)mmap->type);
}

void initPMM() {
	phy_page_cnt = 0;
	pmm_stack_top = 0;
	mmap_entry_t* start_addr = (mmap_entry_t*) glb_mboot_ptr->mmap_addr;
	mmap_entry_t* end_addr = (mmap_entry_t*) glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length;
	mmap_entry_t* ptr;
	for(ptr = start_addr; ptr < end_addr; ptr++) {
		// type 1 presents useable physical memory area
		if(ptr->type == 1 && ptr->addrLow == 0x100000) {
			uint32_t page_addr = ptr->addrLow + (uint32_t)(kern_end - kern_start);
			uint32_t page_len = ptr->addrLow + ptr->lenLow;
			while(page_addr < page_len && page_addr <= PMM_MAX_SIZE) {
				pmmFreePage(page_addr);
				page_addr += PMM_PAGE_SIZE;
				phy_page_cnt++;
			}
		} 
	}
}

uint32_t pmmAllocPage() {
	if(pmm_stack_top == 0) return 0;
	uint32_t page = pmm_stack[pmm_stack_top--];
	return page;
}

void pmmFreePage(uint32_t p) {
	if(pmm_stack_top == PMM_PAGE_MAX_NUM) return;
	pmm_stack[++pmm_stack_top] = p;
}