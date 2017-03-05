#ifndef TASK_H
#define TASK_H

#include "types.h"

void move_stack(void *new_stack_start, uint32_t size, uint32_t kern_stack_top);

#endif