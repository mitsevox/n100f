#ifndef ZGLOBALS_H
#define ZGLOBALS_H

#include "types.h"

struct zScene;

// Partial: only sceneCur is declared. The members before it are left opaque.
struct zGlobals
{
    U8 opaque_0[0xB64];
    zScene* sceneCur;
};

extern zGlobals globals;

#endif
