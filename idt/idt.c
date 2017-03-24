#include "string.h"
#include "idt.h"
#include "stdio.h"

#define IDT_LEN 256


idt_entry_t idt_entries[IDT_LEN];

idt_ptr_t idt_ptr;

interrupt_handler_t interruptHandlers[IDT_LEN];

static void idt_setgate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt_entries[num].base_low = base & 0xFFFF;
	idt_entries[num].base_high = (base >> 16) & 0xFFFF;

	idt_entries[num].seg_sel = sel;
	idt_entries[num].zero_part = 0;

	idt_entries[num].flags = flags;
}

static void RemapPIC() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // Master starts from 32 Interrupt
    outb(0x21, 0x20);

    // Salve starts from 40 Interrupt
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}
extern void idt_flush(uint32_t);

void init_idt() {
    RemapPIC();
	memset(&interruptHandlers, 0, sizeof(interrupt_handler_t) * IDT_LEN);

	idt_ptr.limit = sizeof(idt_entry_t) * IDT_LEN - 1;
	idt_ptr.base = (uint32_t) &idt_entries;

	memset(&idt_entries, 0, sizeof(idt_entry_t) * IDT_LEN);
	
    // 0 - 31 CPU Reserved ISR
	idt_setgate(ISR0, (uint32_t)isr0, SEL_KDATA, 0x8E);
	idt_setgate(ISR1, (uint32_t)isr1, SEL_KDATA, 0x8E);
	idt_setgate(ISR2, (uint32_t)isr2, SEL_KDATA, 0x8E);
	idt_setgate(ISR3, (uint32_t)isr3, SEL_KDATA, 0x8E);
	idt_setgate(ISR4, (uint32_t)isr4,  SEL_KDATA, 0x8E);
    idt_setgate(ISR5, (uint32_t)isr5,  SEL_KDATA, 0x8E);
    idt_setgate(ISR6, (uint32_t)isr6,  SEL_KDATA, 0x8E);
    idt_setgate(ISR7, (uint32_t)isr7,  SEL_KDATA, 0x8E);
    idt_setgate(ISR8, (uint32_t)isr8,  SEL_KDATA, 0x8E);
    idt_setgate(ISR9, (uint32_t)isr9,  SEL_KDATA, 0x8E);
    idt_setgate(ISR10, (uint32_t)isr10, SEL_KDATA, 0x8E);
    idt_setgate(ISR11, (uint32_t)isr11, SEL_KDATA, 0x8E);
    idt_setgate(ISR12, (uint32_t)isr12, SEL_KDATA, 0x8E);
    idt_setgate(ISR13, (uint32_t)isr13, SEL_KDATA, 0x8E);
    idt_setgate(ISR14, (uint32_t)isr14, SEL_KDATA, 0x8E);
    idt_setgate(ISR15, (uint32_t)isr15, SEL_KDATA, 0x8E);
    idt_setgate(ISR16, (uint32_t)isr16, SEL_KDATA, 0x8E);
    idt_setgate(ISR17, (uint32_t)isr17, SEL_KDATA, 0x8E);
    idt_setgate(ISR18, (uint32_t)isr18, SEL_KDATA, 0x8E);
    idt_setgate(ISR19, (uint32_t)isr19, SEL_KDATA, 0x8E);
    idt_setgate(ISR20, (uint32_t)isr20, SEL_KDATA, 0x8E);
    idt_setgate(ISR21, (uint32_t)isr21, SEL_KDATA, 0x8E);
    idt_setgate(ISR22, (uint32_t)isr22, SEL_KDATA, 0x8E);
    idt_setgate(ISR23, (uint32_t)isr23, SEL_KDATA, 0x8E);
    idt_setgate(ISR24, (uint32_t)isr24, SEL_KDATA, 0x8E);
    idt_setgate(ISR25, (uint32_t)isr25, SEL_KDATA, 0x8E);
    idt_setgate(ISR26, (uint32_t)isr26, SEL_KDATA, 0x8E);
    idt_setgate(ISR27, (uint32_t)isr27, SEL_KDATA, 0x8E);
    idt_setgate(ISR28, (uint32_t)isr28, SEL_KDATA, 0x8E);
    idt_setgate(ISR29, (uint32_t)isr29, SEL_KDATA, 0x8E);
    idt_setgate(ISR30, (uint32_t)isr30, SEL_KDATA, 0x8E);
    idt_setgate(ISR31, (uint32_t)isr31, SEL_KDATA, 0x8E);

    // 32 - 47 IRQ
    idt_setgate(IRQ0, (uint32_t)irq0, SEL_KDATA, 0x8E);
    idt_setgate(IRQ1, (uint32_t)irq1, SEL_KDATA, 0x8E);
    idt_setgate(IRQ2, (uint32_t)irq2, SEL_KDATA, 0x8E);
    idt_setgate(IRQ3, (uint32_t)irq3, SEL_KDATA, 0x8E);
    idt_setgate(IRQ4, (uint32_t)irq4, SEL_KDATA, 0x8E);
    idt_setgate(IRQ5, (uint32_t)irq5, SEL_KDATA, 0x8E);
    idt_setgate(IRQ6, (uint32_t)irq6, SEL_KDATA, 0x8E);
    idt_setgate(IRQ7, (uint32_t)irq7, SEL_KDATA, 0x8E);
    idt_setgate(IRQ8, (uint32_t)irq8, SEL_KDATA, 0x8E);
    idt_setgate(IRQ9, (uint32_t)irq9, SEL_KDATA, 0x8E);
    idt_setgate(IRQ10, (uint32_t)irq10, SEL_KDATA, 0x8E);
    idt_setgate(IRQ11, (uint32_t)irq11, SEL_KDATA, 0x8E);
    idt_setgate(IRQ12, (uint32_t)irq12, SEL_KDATA, 0x8E);
    idt_setgate(IRQ13, (uint32_t)irq13, SEL_KDATA, 0x8E);
    idt_setgate(IRQ14, (uint32_t)irq14, SEL_KDATA, 0x8E);
    idt_setgate(IRQ15, (uint32_t)irq15, SEL_KDATA, 0x8E);

    idt_setgate(128, (uint32_t)isr128, SEL_KDATA, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}

void isr_handler(registers_t *regs) {
    if(interruptHandlers[regs->int_number]) 
        interruptHandlers[regs->int_number](regs);
    else 
        printf("Unhandled interrupt: %d\n", regs->int_number);
}

void irq_handler(registers_t *regs) {
    // interrupt lager than 40 is handled by slave PIC
    if(regs->int_number >= 40) outb(0xA0, 0x20);
    outb(0x20, 0x20);
    if(interruptHandlers[regs->int_number])
        interruptHandlers[regs->int_number](regs);
}

void trap_handler(registers_t *regs) {
    if(regs->int_number <= 31) isr_handler(regs);
    else irq_handler(regs);
}

void registersInterruptHandler(uint8_t n, interrupt_handler_t h) {
    interruptHandlers[n] = h;
}