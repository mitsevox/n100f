#ifndef BINK_BINKNGC_H
#define BINK_BINKNGC_H

#include "types.h"

U32 RADTimerRead(void);
void* radaudiomalloc(U32 size);
void radaudiofree(void* ptr);

#endif
