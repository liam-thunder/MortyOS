#ifndef GDT_H
#define GDT_H

#include "types.h"

// code and data segment for kernel, code and data segment for user
// and a null entry, so the size is five
#define GDT_LEN 5

// segment selector index
#define SEL_NULL_IDX 0
#define SEL_KCODE_IDX 1
#define SEL_KDATA_IDX 2
#define SEL_UCODE_IDX 3
#define SEL_UDATA_IDX 4

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

typedef struct gdt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void init_gdt();

#endif