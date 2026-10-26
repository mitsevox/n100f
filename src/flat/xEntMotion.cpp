#include "xEntMotion.h"

#define k_XENTMOTION_STOPPED 0x4

static inline void xEntMotionRun(_xEntMotion* motion)
{
    motion->flags &= (U16)~k_XENTMOTION_STOPPED;
}

void xEntMechForward(_xEntMotion* motion)
{
    p2EntMotionMechData* mech = &(motion->asset->mech);
    p2EntMotionAsset* mkasst = motion->asset;

    xEntMotionRun(motion);

    if ((motion->mech.state != 0) && (motion->mech.state != 1) && (motion->mech.state != 2))
    {
        if (motion->mech.state == 3)
        {
            motion->mech.ss = -motion->mech.ss;
            motion->mech.sr = -motion->mech.sr;
            motion->tmr = mkasst->mech.sld_tm - motion->tmr;
            motion->mech.state = 0;
        }
        else if (motion->mech.state == 4)
        {
            motion->mech.ss = -motion->mech.ss;
            motion->mech.sr = -motion->mech.sr;
            motion->tmr = mkasst->mech.rot_tm - motion->tmr;
            motion->mech.state = 1;
        }
        else if ((motion->mech.state != 5) && (motion->mech.state != 6) &&
                 (motion->mech.state == 7))
        {
            motion->mech.ss = -motion->mech.ss;
            motion->mech.sr = -motion->mech.sr;
            motion->tmr = 0.0f;

            if ((mech->type == 0) || (mech->type == 2) || (mech->type == 4))
            {
                motion->mech.state = 0;
            }
            else
            {
                motion->mech.state = 1;
            }
        }
    }
}

void xEntMechReverse(_xEntMotion* motion)
{
    p2EntMotionMechData* mech = &motion->asset->mech;
    p2EntMotionAsset* mkasst = motion->asset;

    xEntMotionRun(motion);

    if (motion->mech.state == 0)
    {
        motion->mech.ss = -motion->mech.ss;
        motion->mech.sr = -motion->mech.sr;
        motion->tmr = mkasst->mech.sld_tm - motion->tmr;
        motion->mech.state = 3;
    }
    else if (motion->mech.state == 1)
    {
        motion->mech.ss = -motion->mech.ss;
        motion->mech.sr = -motion->mech.sr;
        motion->tmr = mkasst->mech.rot_tm - motion->tmr;
        motion->mech.state = 4;
    }
    else if ((motion->mech.state != 2) && (motion->mech.state != 3) && (motion->mech.state != 4) &&
             (motion->mech.state != 5) && (motion->mech.state == 6))
    {
        motion->mech.ss = -motion->mech.ss;
        motion->mech.sr = -motion->mech.sr;
        motion->tmr = 0.0f;

        if ((mech->type == 0) || (mech->type == 2) || (mech->type == 4))
        {
            motion->mech.state = 3;
        }
        else
        {
            motion->mech.state = 4;
        }
    }
}

static _xEntMotion** dbg_xems;
static U16 dbg_num;
static U16 dbg_num_allocd;
static S16 dbg_idx = -1;

void xEntMotionDebugExit()
{
    dbg_num = 0;
    dbg_xems = NULL;
    dbg_num_allocd = 0;
    dbg_idx = -1;
}
