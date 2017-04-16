#ifndef TYPES_H
#define TYPES_H

#ifndef NULL
    #define NULL 0
#endif 

#ifndef TRUE
    #define TRUE 1
    #define FALSE 0
#endif

typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

typedef uint32_t size_t; 

typedef int32_t pid_t;

// get the offset of 'member' relative to the beginning of its struct
#define OFFSETOF(type, member)  ((size_t)(&((type *)0)->member))

// get the struct from 'ptr'
#define TO_STRUCT(ptr, type, member) ((type *)((char *)(ptr) - OFFSETOF(type, member)))

#endif