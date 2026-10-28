#ifndef XSND_H
#define XSND_H

#include "types.h"

#include "iSnd.h"
#include "xVec3.h"

struct xSndVol
{
    F32 volL;
    F32 volR;
};

// The PS2 layout (0x44 bytes) plus the GC iSndInfo's lastStreamBuffer.
struct xSndVoiceInfo
{
    U32 assetID;
    U32 sndID;
    U32 parentID;
    xVec3* parentPos;
    S32 internalID;
    U16 flags;
    U16 priority;
    F32 vol;
    F32 pitch;
    U32 sample_rate;
    U32 deadct;
    xVec3 pos;
    F32 radius2;
    iSndInfo ps;
};

// The PS2 layout with 64 voices instead of 48 (0x1270 bytes).
struct xSndGlobals
{
    U32 stereo;
    U32 SndCount;
    xSndVol mastervol;
    xSndVol streamvol;
    xSndVol fxvol;
    xSndVoiceInfo voice[64];
    xVec3 right;
    xVec3 up;
    xVec3 at;
    xVec3 pos;
    F32 radius2;
    xVec3 nearpos;
    xVec3 farpos;
    U32 suspendCD;
};

extern xSndGlobals gSnd;

U32 xSndPlay3D(U32 id, F32 vol, F32 pitch, U32 priority, U32 flags, xVec3* pos, F32 radius);
U32 xSndPlay(U32 id, F32 volL, F32 volR, F32 pitch, U32 priority, U32 flags, U32 parentID);

#endif
