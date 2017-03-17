#ifndef PROC_H
#define PROC_H

#include "types.h"
#include "vmm.h"

enum proc_state 
{
	P_UNUSED = 0, 
	P_EMBRYO = 1, 
	P_SLEEPING = 2, 
	P_RUNNABLE = 3,
	P_RUNNING = 4,
	P_ZOMBIE = 5,
};

typedef struct proc {
	// size of proc memory (bytes)
	uint32_t size;
	pgd_t* pgd;
	// bottom of the kernel stack for this proc
	char* kstack;  
	enum proc_state state;
	int32_t pid;
	struct proc* parent;
	struct trapframe *tf;

} proc_t;

#endif