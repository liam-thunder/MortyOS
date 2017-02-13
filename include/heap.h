#ifndef HEAP_H
#define HEAP_H

#include "types.h"

#define HEAP_START 0xE0000000

// intrusive list
// declared as bit fields
typedef struct header
{
	struct header *prev;
	struct header *next;

	// wheter the memory chuck has been used
	uint32_t allocated : 1;
	// size of this memory chuck
	uint32_t len : 31;
} header_t;

void initHeap();

void* kmalloc(uint32_t len);

void kfree(void* ptr);

void showHeapDbg();


#endif