#ifndef STRING_H
#define STRING_H

#include "types.h"

void* memcpy(void *dest, const void * src, size_t num);

void* memset(void* ptr, int32_t value, size_t num);

size_t strlen(const char* str);

size_t strnlen(const char* str, size_t maxlen);

char* strcpy(char* dest, const char* src);

char* strncpy(char* dest, const char* src, size_t num);

int32_t strcmp(const char* s1, const char* s2);

void bzero(void *dest, uint32_t len);

#endif