#ifndef BINK_BITPLANE_H
#define BINK_BITPLANE_H

#include "types.h"
#include "bink/varbits.h"

extern const U8 patterns[16][64];

void ReadBPLossless(S16* coefs, VARBITS* vb);
void ReadBPLossyWithMotion(U8* dest, U32 pitch, VARBITS* vb, U32 q, U8* motion);

#endif
