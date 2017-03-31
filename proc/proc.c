#include "proc.h"
#include "mem/pmm.h"

// todo: need lock to protect
static proc_t proc_table[MAX_PROC];
static proc_t* init_proc;
static int32_t cur_pid = 0;

extern void common_ret();

static proc_t* alloc_proc();


void proc_init() {
    memset(proc_table, 0, sizeof(proc_t) * MAX_PROC);
    proc_t *p = alloc_proc();
    init_proc = p;

    p->pgd = (pgd_t*)(pmm_alloc_page() + PAGE_OFFSET);
    clone_pgd(p->pgd, pgd_kern);

    return;
    

}

/*void proc_create_inituser() {

}*/


proc_t* alloc_proc() {

    proc_t* p = proc_table;
    for( ; p < &proc_table[MAX_PROC]; p++)
        if(p->state == P_UNUSED) 
            break;
    if(p == &proc_table[MAX_PROC]) return NULL;

    p->state = P_EMBRYO;
    p->pid = cur_pid++;

    // alloc space for kernel stack
    if((p->kstack = (char*)(pmm_alloc_page() + PAGE_OFFSET)) == 0) {
        p->state = P_UNUSED;
        return NULL;
    }
    char* stack_ptr = p->kstack + PAGE_SIZE;

    // for saved regs
    stack_ptr -= sizeof(*p->reg);
    p->reg = (registers_t*) stack_ptr;

    stack_ptr -= 4;
    *(uint32_t*) stack_ptr = (uint32_t)common_ret;

    stack_ptr -= sizeof(*p->context);
    p->context = (context_t*) stack_ptr;
    memset(p->context, 0, sizeof(*p->context));
    //p->context->eip = (uint32_t)forkret;

    return p;

}