#ifndef BINK_RADMEM_H
#define BINK_RADMEM_H

#include "types.h"

typedef void* (*RADMEMALLOC)(U32 size);
typedef void (*RADMEMFREE)(void* mem);

#ifdef __cplusplus
extern "C" {
#endif

void* radmalloc(U32 size);
void radfree(void* ptr);

#ifdef __cplusplus
}
#endif

#endif
