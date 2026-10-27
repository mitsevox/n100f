#ifndef BINK_RADMEM_H
#define BINK_RADMEM_H

#include "types.h"

typedef void* (*RADMEMALLOC)(U32 size);
typedef void (*RADMEMFREE)(void* mem);

void* radmalloc(U32 size);
void radfree(void* ptr);

#endif
