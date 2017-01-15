#include "types.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
int kernEntry() {
	initGDT();
    consoleClear();
    printf("No No No\n");
	return 0;
}