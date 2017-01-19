#ifndef GDT_H
#define GDT_H

#include "types.h"
// Ref http://wiki.osdev.org/Global_Descriptor_Table
// __attribute__ ((packed)) tells gcc that the 
//     structure may use as little memory as possible
typedef struct gdt_entry_t
{
	// Segment Limit(Low) 0:15
	uint16_t limitLow;
	// low Base Address 16:31   
	uint16_t baseLow;
	// mid Base 0:7
	uint8_t  baseMid; 
	// Type 8:11, S 12:12, DPL 13:14, P 15:15
	uint8_t access;  
	// Segment Limit(High) 16:19, AVL 20:20, L 21:21
	// D/B 22:22, G 23:23
	uint8_t granularity;
	// high Base 24:31
	uint8_t baseHigh;
} __attribute__((packed)) gdt_entry_t; 

typedef struct gdt_ptr_t {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void initGDT();


#endif