#ifndef BINK_RADMEM_H
#define BINK_RADMEM_H

#include "types.h"

void* radmalloc(U32 size);
void radfree(void* ptr);

#endif
