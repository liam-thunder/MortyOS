#include "task.h"
#include "string.h"
#include "pmm.h"
#include "heap.h"

pid_t now_pid = 0;
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

