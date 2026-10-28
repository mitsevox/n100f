#ifndef BINK_BINKNGC_H
#define BINK_BINKNGC_H

#include "types.h"

U32 mult64anddiv(U32 a, U32 b, U32 d);
U32 mult64andshift(U32 a, U32 b, U32 shift);
U32 RADTimerRead(void);
void* radaudiomalloc(U32 size);
void radaudiofree(void* ptr);
void radmemset16(void* dest, U16 value, U32 size);

#endif
