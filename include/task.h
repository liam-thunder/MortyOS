#ifndef TASK_H
#define TASK_H

#include "vmm.h"

#include "types.h"

typedef enum task_state 
{
	TASK_UNIINIT = 0, // task uninitialized
	TASK_SLEEPING = 1, // sleeping
	TASK_RUNNABLE = 2, // runnable or running
	TASK_ZOMBIE = 3,   // zombie state
} task_state_t;

typedef struct context
{
	uint32_t esp;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t esi;
	uint32_t edi;
	uint32_t eflags;
} context_t;

typedef struct mm_struct {
	pgd_t* pgd_dir;
} mm_struct_t;

// PCB
typedef struct task_struct
{	
	volatile task_state_t state;
	pid_t pid;
	void *stack;
	mm_struct_t *mm;
	context_t context;
	struct task_struct *next;
} task_struct_t;

extern pid_t now_pid;

int32_t kernel_thread(int (*f)(void *), void *arg);

void kthread_exit();


#endif