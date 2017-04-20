#include "proc/sched.h"
#include "libs/list.h"
#include "types.h"
#include "libs/common.h"
#include "libs/stdio.h"
#include "debug.h"

void timer_callback(registers_t *regs) {
    //printf("fucker?\n");
    schedule();
}

void wakeup_proc(struct proc* p) {
    if(p->state == P_RUNNABLE || p->state == P_ZOMBIE) 
        panic("Wakeup Failed");
    p->state = P_RUNNABLE;
}

void schedule() {
    if(!cur_proc->need_resched) return;
    // disable interrupt
    int32_t interrupt_state = save_interrupt();

    cur_proc->need_resched = FALSE;
    // find runnable process 
    struct proc* next = NULL;
    list_node_t *last = (cur_proc == idle_proc) ? &proc_list_head : &(cur_proc->proc_node);
    for(list_node_t *ptr = list_next(last); ptr != &proc_list_head; ptr = list_next(ptr)) {
        next = NODE2PROC(ptr);
        if(next->state == P_RUNNABLE) 
            break;
    }
    if(next == NULL || next->state != P_RUNNABLE) next = idle_proc;
    next->run_time++;
    if(next != cur_proc)
        proc_run(next);

    // enable interrupt
    recover_interrupt(interrupt_state);
}