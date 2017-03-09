#include "timer.h"
#include "stdio.h"
#include "idt.h"
#include "sched.h"

void timerCallback(registers_t *regs) {
    //static uint32_t tick = 0;
    //printf("Tick: %d\n", tick++);
    schedule();
}

void initTimer(uint32_t freq) {
    registersInterruptHandler(IRQ0, timerCallback);

    // Intel 8253/8254 PIT Chip
    // The value send to PIT is input clock(1193180) / our required frequency 
    uint32_t divisor = 1193180 / freq;

    // send the command byte to PIT to set it working on mode 3
    outb(0x43, 0x36);

    // split the value into upper/lower bytes
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, low);
    outb(0x40, high);
}
