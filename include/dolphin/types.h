#ifndef DOLPHIN_TYPES_H
#define DOLPHIN_TYPES_H

// Dolphin SDK base types, limited to what reconstructed units use (docs/libraries.md).

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef int BOOL;

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

#ifdef __MWERKS__
#define AT_ADDRESS(addr) : (addr)
#define ATTRIBUTE_ALIGN(num) __attribute__((aligned(num)))
#else
#define AT_ADDRESS(addr)
#endif

#endif
