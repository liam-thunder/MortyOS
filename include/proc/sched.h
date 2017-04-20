#ifndef SCHED_H
#define SCHED_H

#include "idt.h"
#include "proc/proc.h"

void wakeup_proc(struct proc* p);
void schedule();
void timer_callback(registers_t *regs);

#endif