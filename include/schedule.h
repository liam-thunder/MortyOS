#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "types.h"
#include "task.h"

extern task_struct_t *running_proc_head;
extern task_struct_t *wait_proc_head;
extern task_struct_t *current;

void init_schedule();

void schedule();

void change_task_to(task_struct_t *next);

void switch_to(context_t *prev, context_t *next);

#endif