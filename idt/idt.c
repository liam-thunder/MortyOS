#include "string.h"
#include "idt.h"
#include "stdio.h"

#define IDT_LEN 256


idt_entry_t idt_entries[IDT_LEN];

idt_ptr_t idt_ptr;

interrupt_handler_t interruptHandlers[IDT_LEN];

static void idtSetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
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
extern void idtFlush(uint32_t);

void initIDT() {
    RemapPIC();
	memset(&interruptHandlers, 0, sizeof(interrupt_handler_t) * IDT_LEN);

	idt_ptr.limit = sizeof(idt_entry_t) * IDT_LEN - 1;
	idt_ptr.base = (uint32_t) &idt_entries;

	memset(&idt_entries, 0, sizeof(idt_entry_t) * IDT_LEN);
	
    // 0 - 31 CPU Reserved ISR
	idtSetGate(0, (uint32_t)isr0, SEL_KDATA, 0x8E);
	idtSetGate(1, (uint32_t)isr1, SEL_KDATA, 0x8E);
	idtSetGate(2, (uint32_t)isr2, SEL_KDATA, 0x8E);
	idtSetGate(3, (uint32_t)isr3, SEL_KDATA, 0x8E);
	idtSetGate(4, (uint32_t)isr4,  SEL_KDATA, 0x8E);
    idtSetGate(5, (uint32_t)isr5,  SEL_KDATA, 0x8E);
    idtSetGate(6, (uint32_t)isr6,  SEL_KDATA, 0x8E);
    idtSetGate(7, (uint32_t)isr7,  SEL_KDATA, 0x8E);
    idtSetGate(8, (uint32_t)isr8,  SEL_KDATA, 0x8E);
    idtSetGate(9, (uint32_t)isr9,  SEL_KDATA, 0x8E);
    idtSetGate(10, (uint32_t)isr10, SEL_KDATA, 0x8E);
    idtSetGate(11, (uint32_t)isr11, SEL_KDATA, 0x8E);
    idtSetGate(12, (uint32_t)isr12, SEL_KDATA, 0x8E);
    idtSetGate(13, (uint32_t)isr13, SEL_KDATA, 0x8E);
    idtSetGate(14, (uint32_t)isr14, SEL_KDATA, 0x8E);
    idtSetGate(15, (uint32_t)isr15, SEL_KDATA, 0x8E);
    idtSetGate(16, (uint32_t)isr16, SEL_KDATA, 0x8E);
    idtSetGate(17, (uint32_t)isr17, SEL_KDATA, 0x8E);
    idtSetGate(18, (uint32_t)isr18, SEL_KDATA, 0x8E);
    idtSetGate(19, (uint32_t)isr19, SEL_KDATA, 0x8E);
    idtSetGate(20, (uint32_t)isr20, SEL_KDATA, 0x8E);
    idtSetGate(21, (uint32_t)isr21, SEL_KDATA, 0x8E);
    idtSetGate(22, (uint32_t)isr22, SEL_KDATA, 0x8E);
    idtSetGate(23, (uint32_t)isr23, SEL_KDATA, 0x8E);
    idtSetGate(24, (uint32_t)isr24, SEL_KDATA, 0x8E);
    idtSetGate(25, (uint32_t)isr25, SEL_KDATA, 0x8E);
    idtSetGate(26, (uint32_t)isr26, SEL_KDATA, 0x8E);
    idtSetGate(27, (uint32_t)isr27, SEL_KDATA, 0x8E);
    idtSetGate(28, (uint32_t)isr28, SEL_KDATA, 0x8E);
    idtSetGate(29, (uint32_t)isr29, SEL_KDATA, 0x8E);
    idtSetGate(30, (uint32_t)isr30, SEL_KDATA, 0x8E);
    idtSetGate(31, (uint32_t)isr31, SEL_KDATA, 0x8E);

    // 32 - 47 IRQ
    idtSetGate(IRQ0, (uint32_t)irq0, SEL_KDATA, 0x8E);
    idtSetGate(IRQ1, (uint32_t)irq1, SEL_KDATA, 0x8E);
    idtSetGate(IRQ2, (uint32_t)irq2, SEL_KDATA, 0x8E);
    idtSetGate(IRQ3, (uint32_t)irq3, SEL_KDATA, 0x8E);
    idtSetGate(IRQ4, (uint32_t)irq4, SEL_KDATA, 0x8E);
    idtSetGate(IRQ5, (uint32_t)irq5, SEL_KDATA, 0x8E);
    idtSetGate(IRQ6, (uint32_t)irq6, SEL_KDATA, 0x8E);
    idtSetGate(IRQ7, (uint32_t)irq7, SEL_KDATA, 0x8E);
    idtSetGate(IRQ8, (uint32_t)irq8, SEL_KDATA, 0x8E);
    idtSetGate(IRQ9, (uint32_t)irq9, SEL_KDATA, 0x8E);
    idtSetGate(IRQ10, (uint32_t)irq10, SEL_KDATA, 0x8E);
    idtSetGate(IRQ11, (uint32_t)irq11, SEL_KDATA, 0x8E);
    idtSetGate(IRQ12, (uint32_t)irq12, SEL_KDATA, 0x8E);
    idtSetGate(IRQ13, (uint32_t)irq13, SEL_KDATA, 0x8E);
    idtSetGate(IRQ14, (uint32_t)irq14, SEL_KDATA, 0x8E);
    idtSetGate(IRQ15, (uint32_t)irq15, SEL_KDATA, 0x8E);

    idtSetGate(128, (uint32_t)isr128, SEL_KDATA, 0x8E);

    idtFlush((uint32_t)&idt_ptr);
}

void isrHandler(registers_t *regs) {
    if(interruptHandlers[regs->irpNumber]) 
        interruptHandlers[regs->irpNumber](regs);
    else 
        printf("Unhandled interrupt: %d\n", regs->irpNumber);
}

void irqHandler(registers_t *regs) {
    // interrupt lager than 40 is handled by slave PIC
    if(regs->irpNumber >= 40) outb(0xA0, 0x20);
    outb(0x20, 0x20);
    if(interruptHandlers[regs->irpNumber])
        interruptHandlers[regs->irpNumber](regs);
}

void registersInterruptHandler(uint8_t n, interrupt_handler_t h) {
    interruptHandlers[n] = h;
}