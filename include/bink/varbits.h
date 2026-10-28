#ifndef BINK_VARBITS_H
#define BINK_VARBITS_H

#include "types.h"

typedef struct VARBITS
{
    U32* cur;
    void* start;
    U32 bits;
    U32 bitlen;
} VARBITS;

extern const U32 VarBitsLens[33];

void VarBitsCopy(VARBITS* dest, VARBITS* src, U32 size);

#endif
