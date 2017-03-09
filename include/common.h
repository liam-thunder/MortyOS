#ifndef COMMON_H
#define COMMON_H

#include "types.h"

// write a byte to the port
void outb(uint16_t port, uint8_t value);

// read a byte from the port
uint8_t inb(uint16_t port);

// read a word from the port
uint16_t inw(uint16_t port);

// enable interrupt
void enable_interrupt();

#endif