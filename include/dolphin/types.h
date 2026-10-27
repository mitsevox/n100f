#ifndef _DOLPHIN_TYPES_H_
#define _DOLPHIN_TYPES_H_

// Dolphin SDK base types, limited to what reconstructed units use (docs/libraries.md).

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef int BOOL;

#define FALSE 0
#define TRUE 1

#ifndef NULL
#define NULL 0
#endif

#define AT_ADDRESS(addr) : (addr)

#endif
