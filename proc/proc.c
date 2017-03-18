#include "proc.h"
#include "pmm.h"

// todo: need lock to protect
static proc_t proc_table[MAX_PROC];
static proc_t* init_proc;
static int32_t cur_pid = 0;


static proc_t* alloc_proc();


void proc_init() {
    memset(proc_table, 0, sizeof(proc_t) * MAX_PROC);

}

void proc_create_inituser() {
    proc_t *p = alloc_proc();
    init_proc = p;

}


proc_t* alloc_proc() {
    proc_t* p = proc_table;
    char* stack_ptr = NULL;
    for( ; p < &proc_table[MAX_PROC]; p++)
        if(p->state == P_UNUSED) 
            break;
    if(p == &proc_table[MAX_PROC]) return NULL;

    p->state = P_EMBRYO;
    p->pid = cur_pid++;

    // alloc space for kernel stack
    if((p->kstack = (char*)pmm_alloc_page()) == 0) {
        p->state = P_UNUSED;
        return NULL;
    }
    stack_ptr = p->kstack + PAGE_SIZE;

    // for saved regs
    stack_ptr -= sizeof(*p->reg);
    p->reg = (registers_t*) stack_ptr;

    return p;

}