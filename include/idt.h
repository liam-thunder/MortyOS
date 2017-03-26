#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "common.h"

// kernel data segment selector (IDX(2) TI(0) PRL(0))
#define SEL_KDATA 0x8

// Gate Type
#define GATE_INT_32 0xE   // 32 bits Interrupt Gate
#define GATE_TRAP_32 0xF  // 32 bits Trap Gate
#define GATE_TASK_32 0x5  // 32 bits Task Gate

// Descriptor Privilege Level
#define DPL_U 0x70

// Present and Storage Segment
#define P_USED 0x80

// Ref http://wiki.osdev.org/Interrupt_Descriptor_Table
typedef struct idt_entry {
    uint16_t offset_low;   // lower 16bits offset
    uint16_t offset_high;  // higher 16bits offset
    uint16_t seg_sel;      // segment selector
    uint8_t zero_part;     // reserved zero
    uint8_t type_attr;     // type and attributes     
} __attribute__((packed)) idt_entry_t;


typedef struct idt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

typedef struct registers {
    // segment selector
    uint32_t gs, fs, es, ds;
    // pusha regs
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    // Interrupt number and error code
    uint32_t int_number, error_code;
    // Pushed by the processor automatically
    uint32_t eip, cs, eflags, useresp, ss; 
} registers_t;


#define  ISR0      0
#define  ISR1      1
#define  ISR2      2
#define  ISR3      3
#define  ISR4      4
#define  ISR5      5
#define  ISR6      6
#define  ISR7      7
#define  ISR8      8
#define  ISR9      9
#define  ISR10    10
#define  ISR11    11
#define  ISR12    12
#define  ISR13    13
#define  ISR14    14
#define  ISR15    15
#define  ISR16    16
#define  ISR17    17
#define  ISR18    18
#define  ISR19    19
#define  ISR20    20
#define  ISR21    21
#define  ISR22    22
#define  ISR23    23
#define  ISR24    24
#define  ISR25    25
#define  ISR26    26
#define  ISR27    27
#define  ISR28    28
#define  ISR29    29
#define  ISR30    30
#define  ISR31    31

#define  IRQ0     32
#define  IRQ1     33
#define  IRQ2     34
#define  IRQ3     35
#define  IRQ4     36
#define  IRQ5     37
#define  IRQ6     38
#define  IRQ7     39
#define  IRQ8     40
#define  IRQ9     41
#define  IRQ10    42
#define  IRQ11    43
#define  IRQ12    44
#define  IRQ13    45
#define  IRQ14    46
#define  IRQ15    47

void init_idt();

// interrupt handler function pointer
typedef void (*interrupt_handler_t)(registers_t *);

void registersInterruptHandler(uint8_t n, interrupt_handler_t h);


void trap_handler(registers_t *regs);

// Interrupt Service Routines handle function
void isr_handler(registers_t *regs);

// Interrupt Requests handle function
void irq_handler(registers_t *regs);

// Interrupt service routine CPU Reserved
// 0 - 19 Special, CPU-dedicated interrupts
void isr0();        // 0 #DE Division by zero exception
void isr1();        // 1 #DB Debug exception 
void isr2();        // 2 NMI Non maskable interrupt
void isr3();        // 3 BP  Breakpoint exception
void isr4();        // 4 #OF Into detected overflow
void isr5();        // 5 #BR Out of bounds exception 
void isr6();        // 6 #UD Invalid opcode exception
void isr7();        // 7 #NM No coprocessor exception 
void isr8();        // 8 #DF Double fault (pushes an error code)
void isr9();        // 9 Coprocessor segment overrun 
void isr10();       // 10 #TS Bad TSS (pushes an error code)
void isr11();       // 11 #NP Segment not present (pushes an error code)
void isr12();       // 12 #SS Stack falut (pushes an error code) 
void isr13();       // 13 #GP General protection fault (pushes an error code) 
void isr14();       // 14 #PF Page fault (pushes an error code) 
void isr15();       // 15 Unknown interrupt exception
void isr16();       // 16 #MF Coprocessor fault
void isr17();       // 17 #AC Alignment check exception
void isr18();       // 18 #MC Machine check exception
void isr19();       // 19 #XM SIMD flat exception

// 20 - 31 Intel reserved
void isr20();      
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32 - 255 User Defined Interrupts
//void isr255();
void isr128();

void irq0();     // Programmable Interrupt Timer Interrupt
void irq1();     // Keyboard Interrupt
void irq2();     // Cascade (used internally by the two PICs. never raised)
void irq3();     // COM2 (if enabled)    
void irq4();     // COM1 (if enabled)
void irq5();     // LPT2 (if enabled) 
void irq6();     // Floppy Disk
void irq7();     // LPT1 / Unreliable "spurious" interrupt (usually)
void irq8();     // CMOS real-time clock (if enabled)
void irq9();     // Free for peripherals / legacy SCSI / NIC
void irq10();    // Free for peripherals / SCSI / NIC
void irq11();    // Free for peripherals / SCSI / NIC
void irq12();    // PS2 Mouse
void irq13();    // FPU / Coprocessor / Inter-processor
void irq14();    // Primary ATA Hard Disk
void irq15();    // Secondary ATA Hard Disk

#endif