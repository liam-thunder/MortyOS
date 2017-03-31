#include "common/string.h"

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

void bzero(void *dest, uint32_t len) {
	memset(dest, 0, len);
}

char* strcpy(char* dest, const char* src) {
    if (!dest || !src) return NULL;
    char* tmp = dest;
    while ((*tmp++ = *src++) != 0)
        ;
    return dest;
}

char* strncpy(char* dest, const char* src, size_t num) {
    if (!dest || !src) return NULL;
    char* tmp = dest;
    while (num-- > 0 && (*tmp++ = *src++) != 0)
        ;
    return dest;
}

int32_t strcmp(const char* s1, const char* s2) {
    for (; *s1 == *s2; s1++, s2++)
        if (*s1 == '\0')
            return 0;
    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}