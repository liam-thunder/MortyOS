#ifndef PROC_H
#define PROC_H

#include "types.h"
#include "mem/vmm.h"
#include "idt.h"

#define MAX_PROC 64

enum proc_state 
{
    P_UNUSED = 0, 
    P_EMBRYO = 1, 
    P_SLEEPING = 2, 
    P_RUNNABLE = 3,
    P_RUNNING = 4,
    P_ZOMBIE = 5,
};

typedef struct context {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
} context_t;

typedef struct proc {
    // size of proc memory (bytes)
    uint32_t size;
    pgd_t* pgd;
    // bottom of the kernel stack for this proc
    char* kstack;  
    enum proc_state state;
    int32_t pid;
    struct proc* parent;
    registers_t* reg;
    context_t* context;
    // process name
    char name[16];
} proc_t;

void proc_init();
//void proc_create_inituser();

#endif