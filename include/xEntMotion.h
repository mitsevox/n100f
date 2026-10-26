#ifndef XENTMOTION_H
#define XENTMOTION_H

#include "xVec3.h"

struct p2EntMotionMechData
{
    U8 type;
    U8 flags;
    U8 sld_axis;
    U8 rot_axis;
    F32 sld_dist;
    F32 sld_tm;
    F32 sld_acc_tm;
    F32 sld_dec_tm;
    F32 rot_dist;
    F32 rot_tm;
    F32 rot_acc_tm;
    F32 rot_dec_tm;
    F32 ret_delay;
    F32 post_ret_delay;
};

// Partial: only the mech member of the union is declared, so sizeof(p2EntMotionAsset)
// is not its size.
struct p2EntMotionAsset
{
    U8 type;
    U8 pad;
    U16 flags;
    union
    {
        p2EntMotionMechData mech;
    };
};

struct _xEntMechData
{
    xVec3 apos;
    xVec3 bpos;
    xVec3 dir;
    F32 arot;
    F32 brot;
    F32 ss;
    F32 sr;
    S32 state;
    F32 tsfd;
    F32 trfd;
    F32 tsbd;
    F32 trbd;
    F32* rotptr;
};

// Partial: only the mech member of the union is declared and the members after it are
// left out, so sizeof(_xEntMotion) is not its size.
struct _xEntMotion
{
    p2EntMotionAsset* asset;
    U8 type;
    U8 pad;
    U16 flags;
    F32 t;
    F32 tmr;
    F32 d;
    union
    {
        _xEntMechData mech;
    };
};

void xEntMechForward(_xEntMotion* motion);
void xEntMechReverse(_xEntMotion* motion);
void xEntMotionDebugExit();

#endif
