#ifndef DOLPHIN_TYPES_H
#define DOLPHIN_TYPES_H

// Dolphin SDK base types, limited to what reconstructed units use (docs/libraries.md).

typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef float f32;

typedef int BOOL;

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

#define AT_ADDRESS(addr) : (addr)

#endif
