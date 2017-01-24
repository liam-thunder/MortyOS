#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "common.h"

typedef struct idt_entry_t {
	uint16_t baseLow;
	uint16_t segSelector;
	uint8_t zeroPart;
	uint8_t flags;
	uint16_t baseHigh;	
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_t {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

typedef struct registers {
   // Data segment selector
   uint32_t ds;                  
   // pusha regs
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
   // Interrupt number and error code
   uint32_t irpNumber, errorCode;
   // Pushed by the processor automatically
   uint32_t eip, cs, eflags, useresp, ss; 
} registers_t;

// interrupt handler function pointer
typedef void (*interrupt_handler_t)(registers_t *);

void registersInterruptHandler(uint8_t n, interrupt_handler_t h);

// ISR Part

void isrHandler(registers_t *regs);

void initIDT();

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

// 32 - 255 User defined exception
void isr255();


// IRQ Part

// Interrupt Request handle function
void irqHandler(registers_t *regs);

#define  IRQ0     32    // System timer
#define  IRQ1     33    // Keyboard
#define  IRQ2     34    // Cascade interrupt for IRQs 8-15
#define  IRQ3     35    // COM2
#define  IRQ4     36    // COM1
#define  IRQ5     37    // Sound Card
#define  IRQ6     38    // Floppy disk controller
#define  IRQ7     39    // First parallel port
#define  IRQ8     40    // Real time clock
#define  IRQ9     41    // Open interrupt
#define  IRQ10    42    // Open interrupt
#define  IRQ11    43    // Open interrupt
#define  IRQ12    44    // PS/2 mouse
#define  IRQ13    45    // CPU co-processor
#define  IRQ14    46    // IDE0
#define  IRQ15    47    // IDE1

void irq0();
void irq1();
void irq2();
void irq3();
void irq4(); 
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

#endif