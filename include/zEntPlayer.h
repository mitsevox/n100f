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

// Partial: members up to drv. The members between ent and drv that no reconstructed
// function uses are left opaque.
struct zPlayerGlobals
{
    _zEnt ent;
    U8 opaque_EC[0xF0 - 0xEC];
    U32 Health;
    S32 Speed;
    U8 opaque_F8[0x114 - 0xF8];
    S32 JumpState;
    U8 opaque_118[0x130 - 0x118];
    F32 DecelRunSpeed;
    U8 opaque_134[0x13C - 0x134];
    U32 HeadbuttHit;
    F32 ScareTimer;
    U8 opaque_144[0x148 - 0x144];
    U32 ScareShaggy;
    U8 opaque_14C[0x150 - 0x14C];
    F32 DamageTimer;
    U8 opaque_154[0x164 - 0x154];
    F32 FallDeathTimer;
    U8 opaque_168[0x1E0 - 0x168];
    zShaggy1Globals Shaggy1;
    _zEnt* Shaggy8Call;
    F32 Shaggy8CallTimer;
    _zEntHangable* HangFound;
    _zEntHangable* HangEnt;
    U8 opaque_2D8[0x3F8 - 0x2D8];
    _xEntDrive drv;
};

#endif
