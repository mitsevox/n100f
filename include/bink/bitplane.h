#ifndef BINK_BITPLANE_H
#define BINK_BITPLANE_H

#include "types.h"
#include "bink/varbits.h"

void ReadBPLossless(S16* coefs, VARBITS* vb);
void ReadBPLossyWithMotion(U8* dest, U32 pitch, VARBITS* vb, U32 q, U8* motion);

#endif
