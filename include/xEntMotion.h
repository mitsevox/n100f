#ifndef XENTMOTION_H
#define XENTMOTION_H

#include "xMath3.h"

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

struct p2EntMotionERData
{
    xVec3 ret_pos;
    xVec3 ext_dpos;
    F32 ext_tm;
    F32 ext_wait_tm;
    F32 ret_tm;
    F32 ret_wait_tm;
};

// Partial: only the er and mech members of the union are declared, so sizeof(p2EntMotionAsset)
// is not its size.
struct p2EntMotionAsset
{
    U8 type;
    U8 pad;
    U16 flags;
    union
    {
        p2EntMotionERData er;
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

struct _xEntPenData
{
    xVec3 top;
    F32 w;
    xMat4x3 omat;
};

#define k_XENTMOTION_STOPPED 0x4

struct _xEnt;

// Partial: only the mech and pen members of the union are declared. pen is the largest
// member, so sizeof(_xEntMotion) is its size.
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
        _xEntPenData pen;
    };
    _xEnt* owner;
    _xEnt* target;
};

struct _xScene;
struct _xEntFrame;

void xEntMotionInit(_xEntMotion* motion, _xEnt* owner, p2EntMotionAsset* asset);
void xEntMotionReset(_xEntMotion* motion, _xScene* sc);
void xEntMotionSetup(_xEntMotion* motion, _xScene* sc);
void xEntMotionMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
void xEntMechForward(_xEntMotion* motion);
void xEntMechReverse(_xEntMotion* motion);
void xEntMotionDebugExit();

inline void xEntMotionRun(_xEntMotion* motion)
{
    motion->flags &= (U16)~k_XENTMOTION_STOPPED;
}

inline void xEntMotionStop(_xEntMotion* motion)
{
    motion->flags |= k_XENTMOTION_STOPPED;
}

inline U32 xEntMotionIsStopped(const _xEntMotion* motion)
{
    return motion->flags & k_XENTMOTION_STOPPED;
}

#endif
