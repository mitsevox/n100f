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

// Only the mech member of the motion-data union is declared.
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

// The GameCube code reaches the union at +0x14, where the PS2 DWARF puts it
// at +0x20. Only the mech member is declared, and the members after the
// union (owner, target) are left out until their GameCube offsets are known.
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
