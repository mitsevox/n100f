#ifndef ZLIGHTNING_H
#define ZLIGHTNING_H

#include "types.h"
#include "xVec3.h"

enum _tagLightningType
{
    eLT_Line,
    eLT_Rotating,
    eLT_Total
};

struct _tagLightningAdd
{
    _tagLightningType type;
    F32 setup_degrees;
    F32 move_degrees;
    F32 rot_radius;
    xVec3* start;
    xVec3* end;
    S32 total_points;
    F32 time;
    F32 arc_height;
    F32 thickness;
    U8 r;
    U8 g;
    U8 b;
    U8 alpha_pad;
    F32 rand_radius;
    U32 flags;
};

struct _tagLightningLine
{
    F32 unused;
};

struct _tagLightningRot
{
    F32 deg[16];
    F32 degrees;
    F32 height;
};

struct _tagLightning
{
    _tagLightningType type;
    U32 flags;
    xVec3 base_point[16];
    xVec3 point[16];
    S32 total_points;
    F32 arc_height;
    F32 thickness;
    union
    {
        _tagLightningLine line;
        _tagLightningRot rot;
    };
    U8 r;
    U8 g;
    U8 b;
    U8 alpha_pad;
    F32 rand_radius;
    F32 time_left;
    F32 time_total;
};

void zLightningInit();
_tagLightning* zLightningAdd(_tagLightningAdd* add);
void zLightningUpdate(F32 seconds);
void zLightningRender();
void zLightningKill(_tagLightning* l);
void zLightningModifyEndpoints(_tagLightning* l, xVec3* start, xVec3* end);

#endif
