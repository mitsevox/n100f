#ifndef BINK_POPMAL_H
#define BINK_POPMAL_H

#include "types.h"

void pushmalloc(void** ptr, U32 amount);
void* popmalloc(U32 extra);

#endif
