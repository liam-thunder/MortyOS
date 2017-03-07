#include "schedule.h"
#include "heap.h"
#include "pmm.h"


task_struct_t *running_proc_head = NULL;
task_struct_t *wait_proc_head = NULL;
task_struct_t *current = NULL;

void init_schedule() {
	// put the current struct to the bottom of kern stack
	current = (task_struct_t*)(kern_stack_top - STACK_SIZE);
	current->state = TASK_RUNNABLE;
	current->pid = now_pid++;
	current->stack = current;
	// no need to process mm in kernel thread
	current->mm = NULL;

	// create loop linked list 
	current->next = current;
	running_proc_head = current;
}

void schedule() {
	if(current) {
		change_task_to(current->next);
	}
}

void change_task_to(task_struct_t *next) {
	if(current != next) {
		task_struct_t *prev = current;
		current = next;
		switch_to(&(prev->context), &(current->context));
	}
}
