#include "proc/proc.h"
#include "mem/heap.h"
#include "mem/pmm.h"
#include "mem/vmm.h"
#include "debug.h"
#include "mem/gdt.h"
#include "libs/common.h"
#include "libs/stdio.h"

// static function declaration

static struct proc* alloc_proc();

static int32_t setup_mem(uint32_t clone_flag, struct proc *p);

static int32_t setup_stack(struct proc *p, uintptr_t stack, registers_t *reg);

static void forkret();

static int32_t get_pid();

static int32_t init_thread_fun(void* arg);

// static variable
static uint32_t proc_num = 0; 

struct proc *idle_proc = NULL;
struct proc *cur_proc = NULL;
struct proc *start_proc = NULL;

static uint32_t static_pid = 0;

list_node_t proc_list_head;

void init_proc() {
    init_list_head(&proc_list_head);
    idle_proc = NULL;

    // allocate a idle process
    if((idle_proc = alloc_proc()) == NULL)
        panic("Alloc Process Fail");

    idle_proc->pid = get_pid();
    idle_proc->state = P_RUNNABLE;
    idle_proc->kstack = kern_stack_ptr;
    idle_proc->need_resched = TRUE;
    set_proc_name(idle_proc, "idle");

    proc_num++;
    cur_proc = idle_proc;

    int32_t pid = init_kernel_thread(init_thread_fun, "Hello Proc", 0);
    if(pid < 0) panic("init kernel thread error");

    start_proc = find_proc(pid);
    set_proc_name(start_proc, "start_proc");
}

int32_t init_kernel_thread(int (*fn)(void *), void *arg, uint32_t clone_flag) {
    // setup register for process to trap into kernel
    registers_t reg;
    memset(&reg, 0, sizeof(registers_t));
    reg.cs = SEL_KCODE;
    reg.ds = reg.es = reg.ss = SEL_KDATA;

    reg.ebx = (uint32_t)fn;
    reg.edx = (uint32_t)arg;

    reg.eip = (uint32_t) kernel_thread_entry;
    return do_fork(clone_flag | CLONE_VM, 0, &reg);
}

void set_proc_name(struct proc* p, const char* name) {
    strcpy(p->name, name);
}

int32_t do_exit(int32_t error) {
    return -1;
}

// stack = 0 means fork a kernel thread
int32_t do_fork(uint32_t clone_flag, uintptr_t stack, registers_t* reg) {
    // simplified version of error value
    int32_t ret = -1;
    struct proc* p;
    if(proc_num >= MAX_PROC) return -1;
    if((p = alloc_proc()) == NULL) return -1;
    
    p->parent = cur_proc;

    // set up stack and regs
    if(setup_stack(p, stack, reg) < 0) {
        kfree(p);
        return -1;
    }

    // set up pgd for process
    if(setup_mem(clone_flag, p) < 0) {
        kfree(p);
        return -1;
    }

    int32_t interrupt_state = save_interrupt();

    p->pid = get_pid();
    list_add(&p->proc_node, &proc_list_head);
    proc_num++;

    recover_interrupt(interrupt_state);
    // need to wake up the process
    // todo...

    ret = p->pid;

    return ret;
}

struct proc* find_proc(int32_t pid) {
    if(pid < 0 || pid >= MAX_PID) return NULL;
    for(list_node_t* n = list_next(&proc_list_head); n != &proc_list_head; n = list_next(n)) {
        struct proc *p = NODE2PROC(n);
        if(p->pid == pid) return p;
    }
    return NULL;
}

void proc_run(struct proc* p) {
    if(p == cur_proc) return;
    struct proc* prev = cur_proc, *next = p;
    int32_t interrupt_state = save_interrupt();

    cur_proc = p;
    // todo switch...

}

// static function imple

/**
 * alloc_proc is used to allocate a new process struct
 * whose fields are initialized.
 */
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
    p->reg = NULL;
    p->cr3 = 0;
    p->flag = 0;
    memset(p->name, 0, PROC_NAME_LEN);
    return p;
}

/**
 * setup pgd struct for process,
 * needed when clone flag is set
 */
int32_t setup_mem(uint32_t clone_flag, struct proc *p) {
    // todo...
    return 0;
}

/**
 * setup stack for new process, and initialize context
 */
int32_t setup_stack(struct proc *p, uintptr_t stack, registers_t *reg) {
    // set up stack
    uintptr_t alloc_stack = (uintptr_t) pmm_alloc_page();
    if(alloc_stack == NULL) {
        kfree(p);
        return -1;
    }
    p->kstack = alloc_stack;

    p->reg = (registers_t*)(p->kstack + STACK_SIZE) - 1;
    *(p->reg) = *reg;

    p->reg->eax = 0;
    p->reg->esp = stack;
    // enable interrupt
    p->reg->eflags |= FL_IF;

    p->ctx.eip = (uint32_t)forkret;
    p->ctx.esp = (uint32_t)(p->reg);

    return 0;

}

void forkret() {
    // set esp to cur_proc's esp
    asm volatile("mov %0, %%esp" : : "r" (cur_proc->reg->esp));
    common_ret();
}

// very naive way to allocate pid
int32_t get_pid() {
    if(static_pid == MAX_PID) 
        panic("Out Of PID, Why Not Naive?");
    int32_t ret = static_pid;
    static_pid++;
    return ret;
}

// function for kernel thread
int32_t init_thread_fun(void* arg) {
    printf("To U: %s\n", (const char *)arg);
    return -1;
}