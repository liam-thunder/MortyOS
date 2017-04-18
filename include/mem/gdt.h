#ifndef GDT_H
#define GDT_H

#include "types.h"

// null entry, code and data segment for kernel, code and data segment for user
// and a tss segment, so the size is six
#define GDT_LEN 6

// segment selector index
#define SEL_NULL_IDX 0
#define SEL_KCODE_IDX 1
#define SEL_KDATA_IDX 2
#define SEL_UCODE_IDX 3
#define SEL_UDATA_IDX 4
#define SEL_TSS_IDX 5

// Requested Privilege Level 
#define DPL_K (0)
#define DPL_U (3)

#define SEL_KCODE ((SEL_KCODE_IDX << 3) | DPL_K)
#define SEL_KDATA ((SEL_KDATA_IDX << 3) | DPL_K)
#define SEL_UCODE ((SEL_UCODE_IDX << 3) | DPL_U)
#define SEL_UDATA ((SEL_UDATA_IDX << 3) | DPL_U)

// Ref http://wiki.osdev.org/Global_Descriptor_Table
// __attribute__ ((packed)) tells gcc that the 
//     structure may use as little memory as possible
typedef struct gdt_entry_t
{
    // Segment Limit(Low) 0:15
    uint16_t limit_low;
    // low Base Address 16:31   
    uint16_t base_low;
    // mid Base 0:7
    uint8_t  base_mid; 
    // Type 8:11, S 12:12, DPL 13:14, P 15:15
    uint8_t access;  
    // Segment Limit(High) 16:19, AVL 20:20, L 21:21
    // D/B 22:22, G 23:23
    uint8_t granularity;
    // high Base 24:31
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t; 

typedef struct tss_entry_t {
    uint32_t link;         // segment selector for the TSS of the previous task
    uint32_t esp0;         // stack pointers and segment selectors
    uint16_t ss0;          // after an increase in privilege level
    uint16_t padding1;
    uint32_t esp1;
    uint16_t ss1;
    uint16_t padding2;
    uint32_t esp2;
    uint16_t ss2;
    uint16_t padding3;
    uint32_t cr3;          // page directory base
    uint32_t eip;          // saved state from last task switch
    uint32_t eflags;
    uint32_t eax;          // more saved state (registers)
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint16_t es;           // even more saved state (segment selectors)
    uint16_t padding4;
    uint16_t cs;
    uint16_t padding5;
    uint16_t ss;
    uint16_t padding6;
    uint16_t ds;
    uint16_t padding7;
    uint16_t fs;
    uint16_t padding8;
    uint16_t gs;
    uint16_t padding9;
    uint16_t ldt;
    uint16_t padding10;
    uint16_t t;              // trap on task switch
    uint16_t io_map;         // i/o map base address
} __attribute__((packed)) tss_entry_t;

typedef struct gdt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void init_gdt();

void set_esp0(uintptr_t esp0);

#endif