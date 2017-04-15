#include "libs/stdio.h"
#include "mem/vmm.h"
static uint16_t *videoMemory = (uint16_t *)(0xB8000 + PAGE_OFFSET);

static uint8_t cursorX = 0;
static uint8_t cursorY = 0;

static void moveCursor() {
	uint16_t cursorLocation = cursorX + cursorY * 80;

	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);
}

static void scrollScreen() {
	uint8_t attrByte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attrByte << 8);

	if(cursorY >= 25) {
		for (int i = 0 * 80; i < 24 * 80; ++i)
			videoMemory[i] = videoMemory[i+80];
		for(int i = 24 * 80; i < 25 * 80; ++i)
			videoMemory[i] = blank;
		cursorY = 24;
	}
}

static void consolePutColor(char c, realColor_t back, realColor_t fore) {
	uint8_t backColor = (uint8_t)back;
	uint8_t foreColor = (uint8_t)fore;

	uint8_t attrByte = (backColor << 4) | (foreColor & 0x0F);
	uint16_t attr = attrByte << 8;

	if(c == 0x08 && cursorX) cursorX--;
	else if(c == 0x09) cursorX = (cursorX+8) & ~(8-1);
	else if(c == '\r') cursorX = 0;
	else if(c == '\n') {
		cursorX = 0;
		cursorY++;
	} else if(c >= ' ') {
		videoMemory[cursorY*80 + cursorX] = c | attr;
		cursorX++;
	}

	if(cursorX >= 80) {
		cursorX = 0;
		cursorY++;
	}

	scrollScreen();

	moveCursor();
}


static int32_t getint(va_list *args, int32_t lflag) {
	if(lflag >= 2) return va_arg(*args, long);
	else if(lflag) return va_arg(*args, long);
	else return va_arg(*args, int);
}

static uint32_t getuint(va_list *args, int lflag) {
	if(lflag >= 2) return va_arg(*args, unsigned long);
	else if(lflag) return va_arg(*args, unsigned long);
	else return va_arg(*args, unsigned int);
}

static void printnum(uint32_t num, uint32_t base, int32_t width, int32_t padc, int32_t huflag) {
	if(num >= base) printnum(num / base, base, width - 1, padc, huflag);
	else {
		while(--width > 0) 
			consolePutColor(padc, PRINTF_BACK, PRINTF_FORE);
	}
	if(huflag == 0)
		consolePutColor("0123456789abcdef"[num % base], PRINTF_BACK, PRINTF_FORE);
	else 
		consolePutColor("0123456789ABCDEF"[num % base], PRINTF_BACK, PRINTF_FORE);

}

static void printfmt(const char* str, va_list args) {
	int32_t ch, width, precision, base, lflag, altflag, hexUpperFlag;
	char padc;
	const char* p;
	uint32_t num;
	while(1) {
		while((ch = *(unsigned char*) str++) != '%') {
			if(ch == '\0') return;
			consolePutColor(ch, PRINTF_BACK, PRINTF_FORE);
		}
		padc = ' ', width = -1, precision = -1, lflag = 0, altflag = 0, hexUpperFlag = 0;
	reswitch:
		switch(ch = *(unsigned char *) str++) {
			case '-':
				padc = '-';
				goto reswitch;
			case '0':
				padc = '0';
				goto reswitch;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				for(precision = 0; ; str++) {
					precision = precision * 10 + ch - '0';
					ch = *str;
					if(ch < '0' || ch > '9') break;
				}
				goto process_precision;
			case '*':
				precision = va_arg(args, int);
				goto process_precision;
			case '.':
				if(width < 0) width = 0;
				goto reswitch;
			case '#':
				altflag = 1;
				goto reswitch;
			process_precision:
				if(width < 0) {
					width = precision;
					precision = -1;
				}
				goto reswitch;
			case 'l':
				lflag++;
				goto reswitch;
			case 'c':
				consolePutColor(va_arg(args, int), PRINTF_BACK, PRINTF_FORE);
				break;
			case 's':
				if((p = va_arg(args, char *)) == NULL) p = "(null)";
				if(width > 0 && padc != '-')
					for(width -= strnlen(p, precision); width > 0; width--)
						consolePutColor(padc, PRINTF_BACK, PRINTF_FORE);
				for (; (ch = *p++) != '\0' && (precision < 0 || --precision >= 0); width--)
					if (altflag && (ch < ' ' || ch > '~'))
						consolePutColor('?', PRINTF_BACK, PRINTF_FORE);
					else
						consolePutColor(ch, PRINTF_BACK, PRINTF_FORE);
				for (; width > 0; width--)
					consolePutColor(' ', PRINTF_BACK, PRINTF_FORE);
				break;
			case 'd':
				num = getint(&args, lflag);
				if((int32_t) num < 0) {
					consolePutColor('-', PRINTF_BACK, PRINTF_FORE);
					num = -(int32_t) num;
				}
				base = 10;
				goto number;
			case 'u':
				num = getuint(&args, lflag);
				base = 10;
				goto number;
			case 'o':
				num = getuint(&args, lflag);
				base = 8;
				goto number;
			case 'p':
				consolePutColor('0', PRINTF_BACK, PRINTF_FORE);
				consolePutColor('x', PRINTF_BACK, PRINTF_FORE);
				num = (uint64_t) (uintptr_t) va_arg(args, void *);
				base = 16;
				goto number;
			case 'X':
				num = getuint(&args, lflag);
				base = 16;
				hexUpperFlag = 1;
				goto number;
			case 'x':
				num = getuint(&args, lflag);
				base = 16;
			number:
				printnum(num, base, width, padc, hexUpperFlag);
				break;
			case '%':
				consolePutColor(ch, PRINTF_BACK, PRINTF_FORE);	
				break;
			default:
				consolePutColor('%', PRINTF_BACK, PRINTF_FORE);
				for(str--; str[-1] != '%'; str--)
					/* NULL */;
				break;
		}
	}
	return;	
}

void printf(const char* str, ...) {
	va_list args;
	va_start(args, str);
	printfmt(str, args);
	va_end(args);
}

void consoleClear() {
	uint8_t attrByte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attrByte << 8);
	for(int i = 0; i < 80 * 25; i++)
		videoMemory[i] = blank;
	cursorX = 0, cursorY = 0;
	moveCursor();
}