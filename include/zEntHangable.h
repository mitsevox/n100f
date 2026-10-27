#ifndef ZENTHANGABLE_H
#define ZENTHANGABLE_H

#include "zEnt.h"

struct p2EntHangableAsset;

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

void zEntHangable_SetShaggy(_zEntHangable* ent, _zEnt* shaggy);

#endif
