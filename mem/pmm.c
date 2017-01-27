#include "multiboot.h"
#include "stdio.h"

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