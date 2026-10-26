#ifndef TYPES_H
#define TYPES_H

typedef signed char S8;
typedef signed short S16;
typedef signed int S32;
typedef signed long long S64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;

typedef float F32;
typedef double F64;

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#endif
