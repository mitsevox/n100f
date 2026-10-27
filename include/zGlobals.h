#ifndef ZGLOBALS_H
#define ZGLOBALS_H

#include "types.h"

struct zScene;

// Partial: only sceneCur is declared. The members before it are left opaque; the GC
// layout differs from SLUS-20349's, where sceneCur is at 0xB94.
struct zGlobals
{
    U8 opaque_0[0xB64];
    zScene* sceneCur;
};

extern zGlobals globals;

#endif
