#include "task.h"
#include "common/string.h"
#include "mem/pmm.h"
#include "mem/heap.h"
#include "sched.h"
#include "common/stdio.h"

pid_t now_pid = 0;

int32_t kernel_thread(int (*f)(void *), void *arg) {
    task_struct_t *new_task = (task_struct_t*) kmalloc(STACK_SIZE);
    bzero(new_task, sizeof(task_struct_t));

    new_task->state = TASK_RUNNABLE;
    // set the kernel stack
    new_task->stack = current;
    new_task->pid = now_pid++;
    new_task->mm = NULL;

    uint32_t *stack_top = (uint32_t*)((uint32_t)new_task + STACK_SIZE);

    // save the arguments of thread function to the new stack
    *(--stack_top) = (uint32_t) arg;
    *(--stack_top) = (uint32_t)kthread_exit;
    *(--stack_top) = (uint32_t)f;

    // esp is for stack top
    new_task->context.esp = (uint32_t)new_task + STACK_SIZE - sizeof(uint32_t) * 3;
    // CPU will handle maskable hardware interrupts
    new_task->context.eflags = 0x200;

    new_task->next = running_proc_head;
    // insert the current task into the list's end
    task_struct_t *tail = running_proc_head;
    while(tail->next != running_proc_head) 
        tail = tail->next;
    tail->next = new_task;

    return new_task->pid;
}

void kthread_exit() {
    register uint32_t val asm("eax");
    printf("Thread Exited with %d\n", val);
    while(1);
}


