#include "task.h"
#include "string.h"
#include "pmm.h"
#include "heap.h"
#include "sched.h"
#include "stdio.h"

pid_t now_pid = 0;

int32_t kernel_thread(int (*f)(void *), void *arg) {
    task_struct_t *new_task = (task_struct_t*) kmalloc(STACK_SIZE);
    bzero(new_task, sizeof(task_struct_t));

    new_task->state = TASK_RUNNABLE;
    new_task->stack = current;
    new_task->pid = now_pid++;
    new_task->mm = NULL;

    uint32_t *stack_top = (uint32_t*)((uint32_t)new_task + STACK_SIZE);

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
/*
void move_stack(void *new_stack_start, uint32_t size, uint32_t stack_top) {
    int32_t old_stack_ptr; 
    asm volatile("mov %%esp, %0" : "=r" (old_stack_ptr));
    uint32_t old_base_ptr;  
    asm volatile("mov %%ebp, %0" : "=r" (old_base_ptr));
    
    uint32_t stack_ptr_offset = (uint32_t)new_stack_start - stack_top;
    uint32_t new_stack_ptr = old_stack_ptr + stack_ptr_offset;
    uint32_t new_base_ptr = old_base_ptr + stack_ptr_offset;
    
    // memcpy the used stack
    memcpy((void*)new_stack_ptr, (void*)old_stack_ptr, stack_top - old_stack_ptr);
    for(uint32_t i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start - size; i -= 4) {
        uint32_t tmp = *(uint32_t*)i;
        if((old_stack_ptr < tmp) && (tmp < stack_top)) {
            tmp += stack_ptr_offset;
            uint32_t *tmp2 = (uint32_t*)i;
            *tmp2 = tmp;
        }
    }
    // reset the stack
    asm volatile("mov %0, %%esp" : : "r" (new_stack_ptr));
    asm volatile("mov %0, %%ebp" : : "r" (new_base_ptr));
}
*/

