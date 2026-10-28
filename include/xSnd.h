#ifndef XSND_H
#define XSND_H

#include "types.h"

#include "xVec3.h"

struct xSndVol
{
    F32 volL;
    F32 volR;
};

// Partial: the voices and listener frame after the volumes are left opaque. The GC object is
// 0x1270 bytes.
struct xSndGlobals
{
    U32 stereo;
    U32 SndCount;
    xSndVol mastervol;
    xSndVol streamvol;
    xSndVol fxvol;
    U8 opaque_20[0x1270 - 0x20];
};

extern xSndGlobals gSnd;

U32 xSndPlay3D(U32 id, F32 vol, F32 pitch, U32 priority, U32 flags, xVec3* pos, F32 radius);
U32 xSndPlay(U32 id, F32 volL, F32 volR, F32 pitch, U32 priority, U32 flags, U32 parentID);

#endif
