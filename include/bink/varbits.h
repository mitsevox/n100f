#ifndef BINK_VARBITS_H
#define BINK_VARBITS_H

#include "types.h"

typedef struct VARBITS
{
    U32* cur;
    U32* init;
    U32 bits;
    U32 bitlen;
} VARBITS;

void VarBitsCopy(VARBITS* dest, VARBITS* src, U32 size);

#endif
