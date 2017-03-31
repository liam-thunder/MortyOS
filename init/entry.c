#include "common/stdio.h"
#include "mem/gdt.h"
#include "idt.h"
#include "timer.h"
#include "mem/pmm.h"
#include "common/string.h"
#include "mem/vmm.h"
#include "mem/heap.h"
#include "initrd.h"
#include "vfs.h"
//#include "task.h"
//#include "sched.h"
#include "proc.h"

int kern_init();

multiboot_t* glb_mboot_ptr;

char kern_stack[STACK_SIZE];
uint32_t kern_stack_top;

// Ref: http://wiki.osdev.org/Higher_Half_Kernel
// 0x00000000 - 0xBFFFFFFF for user
// 0xC0000000 - 0xFFFFFFFF for kernel
// temp page table and kernEnrty() are stored in .init.data and .init.text section
// temp page directory starts at addr 0x1000
__attribute__((section(".init.data"))) pgd_t *pgd_tmp  = (pgd_t *)0x1000;

// temp page table low and high start at 0x2000 and 0x3000
// all the temp page dir/table are aligned with 4096KB(0x1000)
__attribute__((section(".init.data"))) pgd_t *pte_low  = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

__attribute__((section(".init.text"))) void kernEntry(uint32_t stack_addr) {
    // we only need two page table for temp paging
    // low page table entry
    pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
    // map high page table entry to PGD_INDEX(0xC0000000)
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;

    // Map 4MB to virual addr 0x00000000-0x00400000 
    // and 0xC0000000-0xC0400000 in two page table
    for(int i = 0; i < 1024; i++) {
        pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
        pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
    }

    // copy the location of page directory into the CR3 register
    asm volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));
    uint32_t cr0;
    // enable paging
    asm volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r" (cr0));

    // set stack top
    kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
    asm volatile ("mov %0, %%esp\n\t"
            "xor %%ebp, %%ebp" : : "r" (kern_stack_top));
    
    // move multiboot pointer, need to convert mboot_ptr_tmp to uint32_t type first
    glb_mboot_ptr = (multiboot_t*)((uint32_t)mboot_ptr_tmp + PAGE_OFFSET);
    kern_init();
}

void test_heap() {
    printf("Test kmalloc and kfree\n");
    void* addr1 = kmalloc(50);
    printf("malloc 50 bytes in 0x%X\n", addr1);
    //showHeapDbg();
    void* addr2 = kmalloc(500);
    printf("malloc 500 bytes in 0x%X\n", addr2);
    //showHeapDbg();
    void* addr3 = kmalloc(5000);
    //printf("malloc 5000 bytes in 0x%X\n", addr3); 
    showHeapDbg();
    kfree(addr3);
    printf("free in 0x%X\n", addr3);
    showHeapDbg();
    kfree(addr2);
    printf("free in 0x%X\n", addr2);
    showHeapDbg();
    kfree(addr1);
    printf("free in 0x%X\n", addr1);
    showHeapDbg();
}

void test_phy_mem_alloc() {
    printf("kernel in memory start: 0x%08X\n", kern_start);
    printf("kernel in memory end:   0x%08X\n", kern_end);
    printf("kernel in memory used:   %d KB\n", (kern_end - kern_start + 1023) / 1024);
    showMemMap();
    printf("\nThe Count of Physical Memory Page is: %u\n", phy_page_cnt);

    uint32_t allc_addr = NULL;
    printf("Test Physical Memory Alloc :\n");
    allc_addr = pmm_alloc_page();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
    allc_addr = pmm_alloc_page();
    printf("Alloc Physical Addr: 0x%08X\n", allc_addr);
}

void test_initrd_filesystem() {
    // need to add PAGE_OFFSET to all the address provided by glb_mboot_ptr
    printf("Mod Count: %d\n", glb_mboot_ptr->mods_count);
    printf("initrd starts at 0x%08X\n", *(uint32_t*)(glb_mboot_ptr->mods_addr + PAGE_OFFSET) + PAGE_OFFSET); 
    printf("initrd ends at 0x%08X\n", *(uint32_t*)(glb_mboot_ptr->mods_addr + PAGE_OFFSET + 4) + PAGE_OFFSET);

    uint32_t initrd_addr = (*(uint32_t*)(glb_mboot_ptr->mods_addr + PAGE_OFFSET)) + PAGE_OFFSET;

    fs_root = init_initrd(initrd_addr);

    uint32_t i = 0;
    struct dirent *node = 0;
    while((node = readdir_fs(fs_root, i)) != 0) {
        printf("Found: %s\n", node->name);
        fs_node_t* fsnode = finddir_fs(fs_root, node->name);
        if ((fsnode->file_type & 0x7) == FS_DIR) printf("\t(directory)\n");
        else {
            printf("\t contents: ");
            char buffer[256];
            read_fs(fsnode, 0, 256, buffer);
            printf("%s\n", buffer);
        }
        i++;
    }
}
/*
int flag = 0;
int worker() {
    int test_var = 1024;
    while(1) {
        if(flag == 1) {
            printf("New Stack: 0x%08X\n", &test_var);
            flag = 0;
        }
    }
    return 0;
}

void test_process() {
    enable_interrupt();
    initTimer(200);
    init_schedule();
    kernel_thread(worker, NULL);

    while(1) {
        int test_var = 2048;
        if(flag == 0) {
            printf("Old Stack: 0x%08X\n", &test_var);
            flag = 1;
        }
    }
}*/


int kern_init() {

    consoleClear();

    printf("Hello Morty OS New!\n");
    init_gdt();
    init_idt();


    initPMM();
    //test_phy_mem_alloc();
    init_vmm();  
    //test_heap();
    //test_initrd_filesystem();

    /*pgd_t* new_pgd = (pgd_t*) pmm_alloc_page();
    pgd_t* new_pgd_vaddr = (pgd_t*)((uint32_t)new_pgd + PAGE_OFFSET);
    clone_pgd(new_pgd_vaddr, pgd_kern);*/
    enable_interrupt();
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    initTimer(200);
    //test_process();
    //proc_init();
    while (1) {
        asm volatile ("hlt");
    }
    return 0;
}