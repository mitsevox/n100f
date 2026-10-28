#ifndef ZENTHANGABLE_H
#define ZENTHANGABLE_H

#include "zEnt.h"

struct _xScene;

struct p2EntHangableAsset
{
    U32 flags;
    F32 pivotOffset;
    F32 leverArm;
    F32 gravity;
    F32 accel;
    F32 decay;
    F32 grabDelay;
    F32 stopDecel;
};

struct _zEntHangable : _zEnt
{
    p2EntHangableAsset* hangInfo;
    xVec3 pivot;
    xVec3 endpos;
    xVec3 vel;
    xVec3 swingplane;
    F32 grabTimer;
    F32 spin;
    U32 state;
    _zEnt* shaggy;
    S32 enabled;
    _zEnt* follow;
    S32 moving;
    F32 candle_timer;
    S32 candle_state;
};

struct xSerial;

void zEntHangable_Init(void* a, void* b);
void zEntHangable_Init(_zEntHangable* ent, p2EntAsset* asset);
S32 zEntHangableEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);
void zEntHangable_Save(_zEntHangable* ent, xSerial* s);
void zEntHangable_Load(_zEntHangable* ent, xSerial* s);
void zEntHangable_Reset(_zEntHangable* ent);
void zEntHangable_FollowUpdate(_zEntHangable* ent);
void zEntHangable_SetShaggy(_zEntHangable* ent, _zEnt* shaggy);
void zEntHangable_SetFollow(_zEntHangable* ent, _zEnt* follow);
void zEntHangable_SetMatrix(_zEntHangable* ent, F32 dt);
void zEntHangable_Update(_zEntHangable* ent, _xScene* sc, F32 dt);
void zEntHangable_SetupFX();

#endif
