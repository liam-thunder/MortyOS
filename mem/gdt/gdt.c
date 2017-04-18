#include "mem/gdt.h"

static gdt_entry_t gdt_entries[GDT_LEN] __attribute__ ((aligned(8)));
static gdt_ptr_t gdt_ptr;
static tss_entry_t tss_entry __attribute__ ((aligned(8)));
extern uint32_t stack;
extern void gdt_flush(uint32_t);

static void gdt_set_segment(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

static void gdt_set_tss(int32_t num, uint16_t ss0, uint32_t esp0);
void init_gdt() {
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LEN - 1;
    gdt_ptr.base = (uint32_t) &gdt_entries; // why & of array name

    // null entry
    gdt_set_segment(SEL_NULL_IDX, 0, 0, 0, 0); 
    // ref to OSDev Wiki to understand the meaning of 'access' 
    // code seg for kernel
    gdt_set_segment(SEL_KCODE_IDX, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // data seg for kernel
    gdt_set_segment(SEL_KDATA_IDX, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // code seg for uesr
    gdt_set_segment(SEL_UCODE_IDX, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // data seg for user
    gdt_set_segment(SEL_UDATA_IDX, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // tss
    gdt_set_tss(SEL_TSS_IDX, SEL_KDATA, 0);

    gdt_flush((uint32_t)&gdt_ptr);
}

void set_esp0(uintptr_t esp0) {
    tss_entry.esp0 = esp0;
}

static void gdt_set_segment(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_mid = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}


static void gdt_set_tss(int32_t num, uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    gdt_set_segment(num, base, limit, 0x89, 0x40);
    
    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = SEL_UCODE;
    tss_entry.ss = SEL_UDATA;
    tss_entry.ds = SEL_UDATA;
    tss_entry.gs = SEL_UDATA;
    tss_entry.fs = SEL_UDATA;
    tss_entry.es = SEL_UDATA;
}


