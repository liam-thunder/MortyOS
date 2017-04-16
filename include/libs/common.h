#ifndef	COMMON_H 
#define COMMON_H

#include "types.h"

// EFlags Define
#define FL_IF     0x00000200  // Interrupt Flag


// write a byte to the port
void outb(uint16_t port, uint8_t value);

// read a byte from the port
uint8_t inb(uint16_t port);

// read a word from the port
uint16_t inw(uint16_t port);

// enable interrupt
void enable_interrupt();

// disable interrupt
void disable_interrupt();

// read eflags
uint32_t read_eflags();

#endif