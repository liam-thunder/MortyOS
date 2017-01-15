#include "string.h"

void* memcpy(void *dest, const void * src, size_t num) {
    if (dest == NULL || src == NULL) return NULL;
    void* ret = dest;
    char* cdest = (char*) dest, *csrc = (char*) src;
    for (; num != 0; num--)
        *cdest++ = *csrc++;
    return ret;
}

void* memset(void* ptr, int32_t value, size_t num) {
	unsigned char ch = (unsigned char) value;
	unsigned char* cPtr = (unsigned char*) ptr;
	while(num > 0) {
		*cPtr = ch;
		cPtr++;
		num--;
	}
	return ptr;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while(*str != '\0') {
		len++;
		str++;
	}
	return len;
}

size_t strnlen(const char* str, size_t maxlen) {
	size_t ret = 0;
	while(*str != '\0' && maxlen > 0) {
		str++;
		maxlen--;
		ret++;
	}
	return ret;
}
