#ifndef ZPAREMITTER_H
#define ZPAREMITTER_H

#include "types.h"
#include "xVec3.h"

struct _zParEmitter;
struct xPar;

struct _zParEmitterCustomSettings
{
    U32 custom_flags;
    U8 count;
    U8 count_variation;
    F32 interval;
    U32 attachToID;
    xVec3 pos;
    xVec3 vel;
    F32 vel_angle_variation;
    U8 color_birth[4];
    U8 color_death[4];
    F32 size_birth;
    F32 size_birth_variation;
    F32 size_death;
    F32 life;
    F32 life_variation;
    U8 rot[3];
    F32 xzcircle_radius;
    F32 emit_interval_current;
};

_zParEmitter* zParEmitterFind(char* asset_name);
xPar* zParEmitterEmitCustom(_zParEmitter* p, F32 dt, _zParEmitterCustomSettings* info);

#endif
