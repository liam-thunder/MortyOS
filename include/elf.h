#ifndef ELF_H
#define ELF_H

#include "types.h"

typedef struct elf_section_header {
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
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
	elf_symbol_t* symtab;
	uint32_t symtabsz;
	const char *strtab;
	uint32_t strtabsz;
} elf_t;

#endif