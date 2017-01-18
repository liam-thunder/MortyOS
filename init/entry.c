#include "console.h"
#include "gdt.h"
#include "idt.h"
int kernEntry() {
	initGDT();
	initIDT();
    consoleClear();
    printf("Hello Morty OS!\n");
	return 0;
}