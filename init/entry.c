#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"

int kernEntry() {
	initGDT();
	initIDT();
    consoleClear();
    initTimer(200);
    printf("Hello Morty OS New!\n");
    showMemMap();
	return 0;
}