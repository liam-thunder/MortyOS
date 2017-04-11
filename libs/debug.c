#include "elf.h"
#include "common/string.h"
#include "common/stdio.h"
#include "debug.h"
#include "mem/vmm.h"

static void print_stack_trace();
static elf_t kernel_elf;

elf_t get_elfinfo(multiboot_t* m) {
    elf_t elf;
    // get pointer to elf section array
    elf_section_header_t* sh = (elf_section_header_t*)(m->addr + PAGE_OFFSET);

    // addr of section 
    uint32_t shstrtab = sh[m->shndx].addr;
    for(int i = 0; i < m->num; i++) {
        const char* name = (const char *)(shstrtab + sh[i].name) + PAGE_OFFSET;
        // strs represent the names associated with symbol table entries
        if(strcmp(name, ".strtab") == 0) {
            elf.strtab = (const char*)(sh[i].addr + PAGE_OFFSET);
            elf.strtabsz = sh[i].size;
        }
        // symbol table, holds information needed to locate and 
        //  relocate a program's symbolic definitions and references
        if(strcmp(name, ".symtab") == 0) {
            elf.symtab = (elf_symbol_t*)(sh[i].addr + PAGE_OFFSET);
            elf.symtabsz = sh[i].size;
        }
    }
    return elf;
}

const char* elf_lookup_symbol(uint32_t addr, elf_t* elf) {
    for(int i = 0; i < (elf->symtabsz/ sizeof(elf_symbol_t)); i++) {
        if(ELF32_ST_TYPE(elf->symtab[i].info) != STT_FUNC) continue;
        if ((addr >= elf->symtab[i].value) && (addr < (elf->symtab[i].value + elf->symtab[i].size))) {
            return (const char *)((uint32_t)elf->strtab + elf->symtab[i].name);
        }   
    }
    return NULL;
}

void panic(const char* msg) {
    printf("---System Panic: %s---\n", msg);
    print_stack_trace();
    printf("-----------\n");
    while (1) {
        asm volatile ("hlt");
    }
}

void init_debug() {
    kernel_elf = get_elfinfo(glb_mboot_ptr);
}

static void print_stack_trace() {
    uint32_t *ebp, *eip;
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    while (ebp) {
        eip = ebp + 1;
        printf("   [0x%x] %s\n", *eip, elf_lookup_symbol(*eip, &kernel_elf));
        ebp = (uint32_t*)*ebp;
    }
}