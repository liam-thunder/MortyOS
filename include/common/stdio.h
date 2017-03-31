#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"
#include "common/stdarg.h"
#include "common/string.h"
#include "common/common.h"

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

#endif