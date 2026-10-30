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

struct _xEnt;

void xSndInit();
void xSndSuspend();
void xSndResume();
void xSndSetMasterVol(F32 left, F32 right);
void xSndSetStreamVol(F32 left, F32 right);
void xSndSetFXVol(F32 left, F32 right);
void xSndPauseAll(U32 pause_effects, U32 pause_streams);
void xSndStopAll();
void xSndUpdate(F32 time_elapsed);
void xSndExit();
void xSndSetFrame(xVec3* right, xVec3* up, xVec3* at, xVec3* pos, F32 radius);
void xSndSetFrameFar(xVec3* pos);
void xSndParentDied(U32 pid);
void xSndStopChildren(U32 pid);
void xSndStop(U32 snd);
void xSndPause(U32 snd, U32 pause);
U32 xSndPlay3D(U32 id, F32 vol, F32 pitch, U32 priority, U32 flags, xVec3* pos, F32 radius);
U32 xSndPlay3D(U32 id, F32 vol, F32 pitch, U32 priority, U32 flags, _xEnt* parent, F32 radius);
U32 xSndPlay(U32 id, F32 volL, F32 volR, F32 pitch, U32 priority, U32 flags, U32 parentID);

#endif
