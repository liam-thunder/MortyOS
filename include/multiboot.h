#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "types.h"

// Ref: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
typedef struct multiboot_h
{
	// version info of multiboot
	uint32_t flags; 

	// the amount of lower memory and upper memory
	uint32_t mem_lower;
	uint32_t mem_upper;
	
	// bios disk device that 
	// boot loader loaded the OS image from
	uint32_t boot_device;
	// the physical address of the 
	// command line to be passed to the kernel
	uint32_t cmdline;
	// the number of modules loaded
	uint32_t mods_count;
	// the physical address of the first module structure
	uint32_t mods_addr;

	// the bit 5 in the flag is set, so the following four fields
	// indicate where the section header table from an ELF kernel is, 
	// the size of each entry, number of entries, 
	// and the string table used as the index of names.
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;

	// the bit 6 in the flag is set, so the the 'mmap_*' fields are valid, 
	// and indicate the address and length of a buffer containing a memory map of the machine 
	// provided by the bios. 'mmap_addr' is the address, and ‘mmap_length’ is the total size of the buffer.
	uint32_t mmap_length;
	uint32_t mmap_addr;

	uint32_t drives_length;
    uint32_t drives_addr;      
    uint32_t config_table;     
    uint32_t boot_loader_name; 
    uint32_t apm_table;        
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;
} __attribute__((packed)) multiboot_t;

typedef struct mmp_entry_t
{
	uint32_t size;
	// use uint32_t to present 64bits type
	uint32_t addrLow;
	uint32_t addrHigh;
	uint32_t lenLow;
	uint32_t lenHigh;
	uint32_t type;
} __attribute__((packed)) mmap_entry_t;

extern multiboot_t *glb_mboot_ptr;

#endif