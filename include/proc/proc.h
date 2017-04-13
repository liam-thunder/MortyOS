#ifndef PROC_H
#define PROC_H

#include "types.h"
#include "mem/vmm.h"
#include "common/string.h"
#include "idt.h"

#define PROC_NAME_LEN 15
#define MAX_PROC 128
#define MAX_PID (MAX_PROC * 2)

//extern struct proc *idle_proc, *init_proc, *cur_proc;

enum proc_state {
    P_UNUSED = 0, 
    P_EMBRYO = 1, 
    P_SLEEPING = 2, 
    P_RUNNABLE = 3,
    P_RUNNING = 4,
    P_ZOMBIE = 5,
};

struct context {
    uint32_t eip;
    uint32_t esp;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
};

struct proc {
    int32_t pid;                        // process id
    enum proc_state state;              // process state
    int32_t run_time;                   // running time ot this process
    uintptr_t kstack;                   // pointer to kernstack
    struct proc *parent;                // the parent process
    struct pgd_t *pgd;                  // Page Dir (simplified mm_struct)
    struct context ctx;                 // context for process switch
    struct registers_t *regs;           // saved regs for process
    uintptr_t cr3;                      // CR3 reg for page dir addr
    uint32_t flag;                      // process flag
    char name[PROC_NAME_LEN + 1];       // process name
    
};

void init_proc();

#endif