#include "proc/proc.h"
#include "mem/heap.h"
#include "mem/pmm.h"
#include "debug.h"
#include "mem/gdt.h"

//static struct proc proc_table[MAX_PROC];

static struct proc* alloc_proc();

static int32_t setup_mem(uint32_t clone_flag, struct proc *p);

static uint32_t proc_num = 0; 

struct proc *idle_proc = NULL;
struct proc *cur_proc = NULL;

list_node_t proc_list;

void init_proc() {
    init_list_head(&proc_list);
    idle_proc = NULL;
    //memset(proc_table, 0, sizeof(struct proc) * MAX_PROC);

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

    reg.ebx = (uint32_t)fn;
    reg.edx = (uint32_t)arg;

    reg.eip = (uint32_t) kernel_thread_entry;

}

void set_proc_name(struct proc* p, const char* name) {
    strcpy(p->name, name);
}

int32_t do_exit(int32_t error) {
    return -1;
}

int32_t do_fork(uint32_t clone_flag, uintptr_t stack, registers_t* reg) {
    // simplified version of error value
    int32_t ret = -1;
    struct proc* p;
    if(proc_num >= MAX_PROC) return -1;
    if((p = alloc_proc()) == NULL) return -1;
    //p->parent = current;

    // set up stack
    // todo: ptr to free
    uintptr_t orig_ptr;
    uintptr_t alloc_stack = kmalloc_align(STACK_SIZE, PMM_PAGE_SIZE, &orig_ptr);
    if(alloc_stack == NULL) return -1;
    p->kstack = alloc_stack;

    // set up pgd for 


    return -1;
}

struct proc* alloc_proc() {
    struct proc *p = kmalloc(sizeof(struct proc));
    if(!p) return p;
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

int32_t setup_mem(uint32_t clone_flag, struct proc *p) {
    // todo...
    return 0;
}
