#include "gdt.h"
#include "string.h"

// code and data segment for kernel, code and data segment for user
// and a null entry, so the size is five
#define GDT_LEN 5

gdt_entry_t gdtEntries[GDT_LEN];

gdt_ptr_t gdtPtr;

extern uint32_t stack;

static void gdtSetGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdtEntries[num].baseLow = (base & 0xFFFF);
	gdtEntries[num].baseMid = (base >> 16) & 0xFF;
	gdtEntries[num].baseHigh = (base >> 24) & 0xFF;

	gdtEntries[num].limitLow = (limit & 0xFFFF);
	gdtEntries[num].granularity = (limit >> 16) & 0x0F;

	gdtEntries[num].granularity |= gran & 0xF0;
	gdtEntries[num].access = access;
}

void initGDT() {
	gdtPtr.limit = sizeof(gdt_entry_t) * GDT_LEN - 1;
	gdtPtr.base = (uint32_t) &gdtEntries; // why & of array name

	// null entry
	gdtSetGate(0, 0, 0, 0, 0); 
	// ref to OSDev Wiki to understand the meaning of 'access' 
	// code seg for kernel
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	// data seg for kernel
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	// code seg for uesr
	gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	// data seg for user
	gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdtFlush((uint32_t)&gdtPtr);
}