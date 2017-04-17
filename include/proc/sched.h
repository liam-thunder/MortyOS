#ifndef SCHED_H
#define SCHED_H

#include "proc/proc.h"

void wakeup_proc(struct proc* p);
void schedule();

#endif