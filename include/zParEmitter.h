#ifndef ZPAREMITTER_H
#define ZPAREMITTER_H

#include "types.h"
#include "xVec3.h"
#include "xBase.h"
#include "iModel.h"

struct xPar;
struct _xScene;

// Shapes other than the offset point and volume are left opaque.
struct zParEmitterAsset : p2BaseAsset
{
    U8 emit_flags;
    U8 emit_type;
    U8 count;
    U8 count_variation;
    F32 interval;
    union
    {
        U8 opaque_10[0x2C - 0x10];
        struct { xVec3 offset; } e_offsetp;
        struct { U32 emit_volumeID; } e_volume;
        struct { F32 radius; } e_circle;
    };
    U32 attachToID;
    U32 parSysID;
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
    U8 pad_emit[2];
    U8 cull_mode;
    F32 cull_dist_sqr;
    U8 max_emit;
};

struct xParGroup;

struct _zParEmitter : xBase
{
    zParEmitterAsset* tasset;
    xParGroup* group;
    U8 emit_flags;
    U8 emit_pad[3];
    F32 emit_interval_current;
    U8 rot[3];
    iModelTag tag;
    void* attachTo;
    void* parSys;
    void* emit_volume;
};

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

void zParEmitterInit(xBase* b, zParEmitterAsset* tasset);
void zParEmitterSetup(_zParEmitter* t);
void zParEmitterUpdate(xBase* to, _xScene* sc, F32 dt);
xPar* zParEmitterEmit(_zParEmitter* pe, F32 dt);
_zParEmitter* zParEmitterFind(char* asset_name);
_zParEmitter* zParEmitterFind(U32 asset_id);
xPar* zParEmitterEmitCustom(_zParEmitter* p, F32 dt, _zParEmitterCustomSettings* info);

#endif
