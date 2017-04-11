#ifndef ELF_H
#define ELF_H

#include "types.h"
#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i)&0xf)

//This symbol is associated with a function or other executable code
#define STT_FUNC 02

typedef struct elf_section_header {
	uint32_t name;			// string table index
	uint32_t type;
	uint32_t flags;
	uint32_t addr;			// section virtual addr at execution
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addr_align;
	uint32_t ent_size;
} __attribute__((packed)) elf_section_header_t;

typedef struct elf_symbol {
	uint32_t name;  
	uint32_t value;
	uint32_t size;
	uint8_t info;
	uint8_t other;
	uint16_t shndx;
} __attribute__((packed)) elf_symbol_t;

typedef struct elf
{
	elf_symbol_t* symtab;	// symbol table address
	uint32_t symtabsz;		// symbol table size
	const char *strtab;		// string table address
	uint32_t strtabsz;		// string table size
} elf_t;

elf_t get_elfinfo(multiboot_t* m);

const char* elf_lookup_symbol(uint32_t addr, elf_t* elf);

#endif