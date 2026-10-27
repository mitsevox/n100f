#ifndef DOLPHIN_TYPES_H
#define DOLPHIN_TYPES_H

// Dolphin SDK base types, limited to what reconstructed units use (docs/libraries.md).

typedef signed char s8;
typedef signed long s32;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef int BOOL;

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

#endif
