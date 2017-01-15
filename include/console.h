#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"
#include "stdarg.h"
#include "string.h"
#include "common.h"

typedef enum color {
	black = 0, blue = 1, green = 2, cyan = 3,
	red = 4, magenta = 5, brown = 6, lightGrey = 7,
	darkGrey = 8, lightBlue = 9, lightGreen = 10, lightCyan = 11,
	lightRed = 12, lightMagenta = 13, lightBrown = 14, white = 15,
} realColor_t;

#define PRINTF_BACK black
#define PRINTF_FORE white

void consoleClear();

void printf(const char* str, ...);


//void consoleWrite(char *cstr);

//void consoleWriteColor(char *cstr, realColor_t back, realColor_t fore);

//void consoleWriteHex(uint32_t n, realColor_t back, realColor_t fore);

//void consoleWriteDec(uint32_t n, realColor_t back, realColor_t fore);

//void consolePutColor(char c, realColor_t back, realColor_t fore);

//void printfmt(const char* str, va_list args);

#endif