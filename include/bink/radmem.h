#ifndef BINK_RADMEM_H
#define BINK_RADMEM_H

#include "types.h"

typedef void* (*RADMEMALLOC)(U32 bytes);
typedef void (*RADMEMFREE)(void* ptr);

void* radmalloc(U32 size);
void radfree(void* ptr);

#endif
