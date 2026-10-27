#ifndef ZGLOBALS_H
#define ZGLOBALS_H

#include "types.h"
#include "zEntPlayer.h"

struct zScene;

// Partial: only player and sceneCur are declared. The members before and between them
// are left opaque.
struct zGlobals
{
    U8 opaque_0[0x1F8];
    zPlayerGlobals player;
    U8 opaque_4D0[0xB64 - 0x1F8 - sizeof(zPlayerGlobals)];
    zScene* sceneCur;
};

extern zGlobals globals;

#endif
