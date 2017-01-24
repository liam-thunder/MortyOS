#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"

int kernEntry() {
	initGDT();
	initIDT();
    consoleClear();
    initTimer(200);
    asm volatile("sti");
    printf("Hello Morty OS New!\n");
	return 0;
}