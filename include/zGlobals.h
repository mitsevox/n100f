#ifndef ZGLOBALS_H
#define ZGLOBALS_H

#include "types.h"
#include "zEntPlayer.h"
#include "iCamera.h"
#include "iTime.h"

struct zScene;
struct _zCutsceneMgr;

// Partial: only player, cmgr, sceneCur and the fog state are declared. The members before and
// between them are left opaque.
struct zGlobals
{
    U8 opaque_0[0x1F8];
    zPlayerGlobals player;
    U8 opaque_4D0[0x860 - 0x1F8 - sizeof(zPlayerGlobals)];
    _zCutsceneMgr* cmgr;
    U8 opaque_864[0xB64 - 0x864];
    zScene* sceneCur;
    U8 opaque_B68[0xB90 - 0xB68];
    iFogParams fog;
    iFogParams fogA;
    iFogParams fogB;
    iTime fog_t0;
    iTime fog_t1;
};

extern zGlobals globals;

#endif
