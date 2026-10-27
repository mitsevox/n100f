#ifndef ZENTPLAYER_H
#define ZENTPLAYER_H

#include "zEnt.h"

struct _zEntHangable;

struct zShaggy1Globals
{
    _zEnt* GrabTry;
    _zEnt* Grab;
    F32 GrabRot;
    F32 GrabOffset;
    F32 GrabTimer;
    xVec3 ReleaseTrans;
    xVec3 HangTrans;
    xVec3 LastDelta;
    U32 ThrowEnded;
    F32 ExtraTransY;
    xVec3 ThrowPos;
    xVec3 ThrowVel;
    U32 CatchMe;
    U32 CatchYou;
    _zEnt* HangTarget;
    F32 ThrowTimer;
    xVec3 HangStart;
    xVec3 HangRelative;
    U32 SetDown;
    U32 GrabOn;
    U32 ThrowState;
    F32 ThrowRot;
    F32 CatchHeight;
    F32 CatchVel;
    xBase* ThrowTgt;
    F32 ThrowTgtAngle;
    F32 ThrowTgtPeak;
    F32 ThrowTgtOffset;
    U32 ThrowTgtAdjust;
    xVec3 ThrowTgtVec;
    F32 ThrowTgtSpeed;
    F32 ThrowAccel;
    _zEnt* Hangable[10];
};

// Partial: members up to HangEnt. The members between ent and JumpState, and between
// JumpState and Shaggy1, are left opaque.
struct zPlayerGlobals
{
    _zEnt ent;
    U8 opaque_EC[0x114 - 0xEC];
    S32 JumpState;
    U8 opaque_118[0x1E0 - 0x118];
    zShaggy1Globals Shaggy1;
    _zEnt* Shaggy8Call;
    F32 Shaggy8CallTimer;
    _zEntHangable* HangFound;
    _zEntHangable* HangEnt;
};

#endif
