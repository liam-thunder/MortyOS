#include "proc/proc.h"
#include "mem/heap.h"
#include "mem/pmm.h"
#include "debug.h"
#include "mem/gdt.h"

static struct proc proc_table[MAX_PROC];

static struct proc* alloc_proc();
static uint32_t proc_num = 0; 

struct proc *idle_proc = NULL;
struct proc *cur_proc = NULL;

void init_proc() {
    idle_proc = NULL;
    memset(proc_table, 0, sizeof(struct proc) * MAX_PROC);

    if((idle_proc = alloc_proc()) == NULL)
        panic("Alloc Process Fail");

    idle_proc->pid = 0;
    idle_proc->state = P_RUNNABLE;
    idle_proc->kstack = kern_stack_ptr;
    idle_proc->need_resched = TRUE;
    set_proc_name(idle_proc, "idle");

    proc_num++;
    cur_proc = idle_proc;

    //idle_proc->pgd = (pgd_t*)(pmm_alloc_page() + PAGE_OFFSET);
    //clone_pgd(idle_proc->pgd, pgd_kern);

    return;
}

int32_t init_kernel_thread(int (*fn)(void *), void *arg, uint32_t clone_flag) {
    registers_t reg;
    memset(&reg, 0, sizeof(registers_t));
    reg.cs = SEL_KCODE;
    reg.ds = reg.es = reg.ss = SEL_KDATA;
}


void set_proc_name(struct proc* p, const char* name) {
    strcpy(p->name, name);
}

struct proc* alloc_proc() {
    struct proc* p = proc_table;
    for( ; p < &proc_table[MAX_PROC]; p++)
        if(p->state == P_UNUSED) 
            break;
    if(p == &proc_table[MAX_PROC]) return NULL;
    p->state = P_UNUSED;
    p->pid = -1;
    p->need_resched = FALSE;
    p->run_time = 0;
    p->kstack = 0;
    p->parent = NULL;
    p->pgd = NULL;
    memset(&(p->ctx), 0, sizeof(p->ctx));
    p->regs = NULL;
    p->cr3 = 0;
    p->flag = 0;
    memset(p->name, 0, PROC_NAME_LEN);
    return p;
}
