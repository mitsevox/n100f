#include "xEntMotion.h"

#include "xDebug.h"
#include "xEnt.h"
#include "xMath.h"
#include "xMemMgr.h"
#include "xModel.h"
#include "xMovePoint.h"
#include "xPad.h"
#include "xScene.h"
#include "xSpline.h"

#include <math.h>

static void xEntERMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
static U32 xEntERIsExtending(const _xEntMotion* motion);
static U32 xEntERIsExtended(const _xEntMotion* motion);
static U32 xEntERIsRetracting(const _xEntMotion* motion);
static U32 xEntERIsRetracted(const _xEntMotion* motion);
static void xEntOrbitMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
static void xEntMPGetNext(_xEntMotion* motion, _xMovePoint* prev, _xScene* sc);
static void xEntMPMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
static void xEntPenMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
static void xEntMechMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
static U32 xEntSldMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
static U32 xEntRotMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame);
static void xEntMotionDebugAdd(_xEntMotion* motion);
static void xEntMotionDebugIPad(_xEntMotion* xem);
void xEntMotionDebugWrite(const _xEntMotion* xem);
void xEntMotionDebugDraw(const _xEntMotion* xem);

static inline F32 isin(F32 x)
{
    return sin(x);
}

static inline F32 icos(F32 x)
{
    return cos(x);
}

static inline F32 xacos(F32 x)
{
    return acos(x);
}

static inline void xQuatFlip(xQuat* o, const xQuat* q)
{
    o->s = -q->s;
    xVec3Inv(&o->v, &q->v);
}

static inline U32 xVec3Equals(const xVec3* a, const xVec3* b)
{
    return a->x == b->x && a->y == b->y && a->z == b->z;
}

static inline U32 xQuatEquals(const xQuat* a, const xQuat* b)
{
    return a->s == b->s && xVec3Equals(&a->v, &b->v);
}

// Copy the motion asset's settings into the runtime motion and sanitize the mechanism timings.
void xEntMotionInit(_xEntMotion* motion, _xEnt* owner, p2EntMotionAsset* asset)
{
    motion->asset = asset;
    motion->type = asset->type;
    motion->flags = asset->flags;

    if (motion->type == XENTMOTION_TYPE_ER)
    {
        xVec3Copy(&motion->er.a, &asset->er.ret_pos);
        xVec3Add(&motion->er.b, &asset->er.ret_pos, &asset->er.ext_dpos);

        motion->er.et = asset->er.ext_tm;
        motion->er.wet = asset->er.ext_wait_tm;
        motion->er.rt = asset->er.ret_tm;
        motion->er.wrt = asset->er.ret_wait_tm;

        if (motion->er.p <= 0.0f)
        {
            motion->er.p = 1.0f;
        }
    }
    else if (motion->type == XENTMOTION_TYPE_ORB)
    {
        xVec3Copy(&motion->orb.c, &asset->orb.center);

        motion->orb.a = asset->orb.w;
        motion->orb.b = asset->orb.h;
        motion->orb.p = asset->orb.period;
        motion->orb.w = (2.0f * PI) / asset->orb.period;
    }
    else if (motion->type == XENTMOTION_TYPE_MP)
    {
    }
    else if (motion->type == XENTMOTION_TYPE_PEN)
    {
        motion->pen.w = (2.0f * PI) / asset->pen.period;
    }
    else if (motion->type == XENTMOTION_TYPE_MECH)
    {
        if (asset->mech.sld_tm < 0.0f)
        {
            asset->mech.sld_tm = 0.0f;
        }

        if (asset->mech.sld_acc_tm < 0.0f)
        {
            asset->mech.sld_acc_tm = 0.0f;
        }

        if (asset->mech.sld_dec_tm < 0.0f)
        {
            asset->mech.sld_dec_tm = 0.0f;
        }

        if (asset->mech.sld_acc_tm + asset->mech.sld_dec_tm > asset->mech.sld_tm)
        {
            asset->mech.sld_dec_tm = asset->mech.sld_acc_tm = 0.5f * asset->mech.sld_tm;
        }

        if (asset->mech.rot_tm < 0.0f)
        {
            asset->mech.rot_tm = 0.0f;
        }

        if (asset->mech.rot_acc_tm < 0.0f)
        {
            asset->mech.rot_acc_tm = 0.0f;
        }

        if (asset->mech.rot_dec_tm < 0.0f)
        {
            asset->mech.rot_dec_tm = 0.0f;
        }

        if (asset->mech.type == 2)
        {
            if (asset->mech.rot_tm != asset->mech.sld_tm)
            {
                asset->mech.rot_tm = asset->mech.sld_tm;
            }
        }

        if (asset->mech.rot_acc_tm + asset->mech.rot_dec_tm > asset->mech.rot_tm)
        {
            asset->mech.rot_dec_tm = asset->mech.rot_acc_tm = 0.5f * asset->mech.rot_tm;
        }
    }

    motion->owner = owner;
    motion->target = NULL;

    xEntMotionDebugAdd(motion);
}

// Work out the extend/retract phase boundaries: begin retract, end retract and the full period.
void xEntMotionSetup(_xEntMotion* motion, _xScene* sc)
{
    if (motion->type == XENTMOTION_TYPE_ER)
    {
        motion->er.brt = motion->er.et + motion->er.wet;
        motion->er.ert = motion->er.brt + motion->er.rt;
        motion->er.p = motion->er.ert + motion->er.wrt;
    }
}

// Put the motion back at its start: first move point, pendulum swing start, or mechanism rest.
void xEntMotionReset(_xEntMotion* motion, _xScene* sc)
{
    p2EntMotionPenData* aspen;
    F32 dangle;
    xMat3x3 pshrot;
    xVec3 totop;
    xVec3 pshtotop;
    xMat3x3* modlrot;
    xVec3* modlpos;
    _xEnt* ownr;
    p2EntMotionMechData* mkasst;
    _xEntMechData* mech;
    F32 drot;

    motion->flags = motion->asset->flags;
    motion->t = 0.0f;

    if (motion->type == XENTMOTION_TYPE_ER)
    {
        motion->er.state = 0;
    }
    else if (motion->type == XENTMOTION_TYPE_MP)
    {
        motion->mp.src = (_xMovePoint*)xSceneResolvID(sc, motion->asset->mp.mp_id);
        xEntMPSetSpeed(motion, motion->asset->mp.speed);
        xEntMPGetNext(motion, NULL, sc);
        xVec3Copy((xVec3*)&motion->owner->model->Mat->pos, xMovePointGetPos(motion->mp.src));
    }
    else if (motion->type == XENTMOTION_TYPE_ORB)
    {
        xVec3Copy(&motion->orb.orig, (xVec3*)&motion->owner->model->Mat->pos);
    }
    else if (motion->type == XENTMOTION_TYPE_PEN)
    {
        xVec3 c;

        modlpos = (xVec3*)&motion->owner->model->Mat->pos;
        modlrot = (xMat3x3*)motion->owner->model->Mat;
        aspen = &motion->asset->pen;

        dangle = aspen->range * isin(aspen->phase);
        xMat3x3Rot(&pshrot, &modlrot->at, dangle);

        totop.x = 0.0f;
        totop.y = aspen->len;
        totop.z = 0.0f;

        xMat3x3RMulVec(&pshtotop, &pshrot, &totop);
        xVec3Sub(&c, &totop, &pshtotop);
        xMat3x3Mul(modlrot, modlrot, &pshrot);
        xVec3AddTo(modlpos, &c);
        xVec3Add(&motion->pen.top, modlpos, &pshtotop);
        xMat4x3Copy(&motion->pen.omat, (xMat4x3*)modlrot);
    }
    else if (motion->type == XENTMOTION_TYPE_MP)
    {
        if (motion->asset->flags & 1)
        {
            xQuatFromMat(&motion->mp.aquat, (xMat3x3*)motion->owner->model->Mat);
            xQuatCopy(&motion->mp.bquat, &motion->mp.aquat);
        }
        motion->tmr = 0.0f;
    }
    else if (motion->type == XENTMOTION_TYPE_MECH)
    {
        ownr = motion->owner;
        mkasst = &motion->asset->mech;
        mech = &motion->mech;

        if (ownr && ownr->frame && ownr->model)
        {
            xVec3Copy(&mech->apos, (xVec3*)&ownr->model->Mat->pos);

            if (mkasst->sld_axis == 0)
            {
                xVec3Copy(&mech->dir, (xVec3*)&ownr->model->Mat->right);
            }
            else if (mkasst->sld_axis == 1)
            {
                xVec3Copy(&mech->dir, (xVec3*)&ownr->model->Mat->up);
            }
            else
            {
                xVec3Copy(&mech->dir, (xVec3*)&ownr->model->Mat->at);
            }

            xVec3SMul(&mech->bpos, &mech->dir, mkasst->sld_dist);
            xVec3AddTo(&mech->bpos, &mech->apos);

            mech->ss = mkasst->sld_dist / (mkasst->sld_tm - 0.5f * (mkasst->sld_acc_tm + mkasst->sld_dec_tm));
            mech->tsfd = mkasst->sld_tm - mkasst->sld_dec_tm;
            mech->tsbd = mkasst->sld_tm - mkasst->sld_acc_tm;

            if (mkasst->rot_axis == 0)
            {
                mech->rotptr = &ownr->frame->rot.axis.y;
            }
            else if (mkasst->rot_axis == 1)
            {
                mech->rotptr = &ownr->frame->rot.axis.x;
            }
            else
            {
                mech->rotptr = &ownr->frame->rot.axis.z;
            }

            mech->arot = *mech->rotptr;
            drot = DEG2RAD(mkasst->rot_dist);
            mech->brot = xAngleClamp(mech->arot + drot);
            mech->sr = drot / (mkasst->rot_tm - 0.5f * (mkasst->rot_acc_tm + mkasst->rot_dec_tm));
            mech->trfd = mkasst->rot_tm - mkasst->rot_dec_tm;
            mech->trbd = mkasst->rot_tm - mkasst->rot_acc_tm;

            if (mkasst->type == 0 || mkasst->type == 2 || mkasst->type == 3)
            {
                mech->state = 0;
            }
            else
            {
                mech->state = 1;
            }

            motion->tmr = 0.0f;
        }
    }
}

// Advance the motion by dt and write the resulting movement into the entity frame.
void xEntMotionMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    if (motion->flags & k_XENTMOTION_STOPPED)
    {
        return;
    }

    if (motion->type == XENTMOTION_TYPE_ER)
    {
        xEntERMove(motion, sc, dt, frame);
    }
    else if (motion->type == XENTMOTION_TYPE_ORB)
    {
        xEntOrbitMove(motion, sc, dt, frame);
    }
    else if (motion->type == XENTMOTION_TYPE_MP)
    {
        xEntMPMove(motion, sc, dt, frame);
    }
    else if (motion->type == XENTMOTION_TYPE_PEN)
    {
        xEntPenMove(motion, sc, dt, frame);
    }
    else if (motion->type == XENTMOTION_TYPE_MECH)
    {
        xEntMechMove(motion, sc, dt, frame);
    }
    else
    {
        frame->mode = 0;
    }

    motion->t = motion->t + dt;
}

// Extend/retract: slide out to b, wait, slide back to a, wait, repeat.
static void xEntERMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    F32 newt = motion->t - motion->er.p;

    if (newt > 0.0f)
    {
        motion->t = newt;
    }

    if (xEntERIsExtending(motion))
    {
        F32 rem = motion->er.et - motion->t;
        F32 scale = MIN(rem, dt) / motion->er.et;

        if (rem < dt)
        {
            xVec3Sub(&frame->dpos, &motion->er.b, &frame->mat.pos);
        }
        else
        {
            xVec3Sub(&frame->dpos, &motion->er.b, &motion->er.a);
            xVec3SMulBy(&frame->dpos, scale);
        }

        frame->mode = 2;
        motion->er.state = 0;
    }
    else if (xEntERIsExtended(motion))
    {
        motion->er.state = 1;
    }
    else if (xEntERIsRetracting(motion))
    {
        F32 rem = motion->er.ert - motion->t;
        F32 scale = MIN(rem, dt) / motion->er.rt;

        if (rem < dt)
        {
            xVec3Sub(&frame->dpos, &motion->er.a, &frame->mat.pos);
        }
        else
        {
            xVec3Sub(&frame->dpos, &motion->er.a, &motion->er.b);
            xVec3SMulBy(&frame->dpos, scale);
        }

        frame->mode = 2;
        motion->er.state = 2;
    }
    else if (xEntERIsRetracted(motion))
    {
        motion->er.state = 3;
    }
}

static inline U32 xEntERIsExtending(const _xEntMotion* motion)
{
    return motion->t < motion->er.et;
}

static inline U32 xEntERIsExtended(const _xEntMotion* motion)
{
    return motion->t >= motion->er.et && motion->t < motion->er.brt;
}

static inline U32 xEntERIsRetracting(const _xEntMotion* motion)
{
    return motion->t >= motion->er.brt && motion->t < motion->er.ert;
}

static inline U32 xEntERIsRetracted(const _xEntMotion* motion)
{
    return motion->t >= motion->er.ert;
}

// Orbit: move along an ellipse of half-widths a and b, snapping back to the start each period.
static void xEntOrbitMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    F32 rem = dt * 0.5f;
    F32 u = motion->orb.w * (motion->t + rem);
    F32 th = isin(motion->orb.w * rem);
    F32 newt = motion->t - motion->orb.p;

    if (newt > 0.0f)
    {
        motion->t = newt;
    }

    if (-newt < dt && -newt > 0.0f)
    {
        xVec3Sub(&frame->dpos, &motion->orb.orig, &frame->mat.pos);
    }
    else
    {
        frame->dpos.x = (2.0f * th) * motion->orb.a * isin(u);
        frame->dpos.y = 0.0f;
        frame->dpos.z = (-2.0f * th) * motion->orb.b * icos(u);
    }

    frame->mode = 2;
}

// Pick the next move point after src and set up the leg toward it (straight line or spline).
static void xEntMPGetNext(_xEntMotion* motion, _xMovePoint* prev, _xScene* sc)
{
    _xEntMPData* mp = &motion->mp;
    xVec3 tempdir;
    xMat3x3 mat;

    mp->spl = NULL;
    mp->curdist = 0.0f;
    mp->dist = xMovePointGetNext(mp->src, prev, &mp->dest, &tempdir);

    if (mp->dest == mp->src)
    {
        xEntMotionStop(motion);
    }
    else if (!mp->dest)
    {
        xEntMotionStop(motion);
    }
    else
    {
        if (motion->asset->flags & 1)
        {
            xQuatFromMat(&mp->aquat, (xMat3x3*)motion->owner->model->Mat);
            xVec3Inv(&tempdir, &tempdir);
            xMat3x3LookVec(&mat, &tempdir);
            xQuatFromMat(&mp->bquat, &mat);
        }

        if (mp->dest->spl)
        {
            mp->spl = mp->dest->spl;

            while (mp->dest->asset->bezIndex != 0)
            {
                mp->dest = mp->dest->nodes[0];
            }

            mp->dist = xSpline3_ArcTotal(mp->spl);
        }
    }
}

// Move point path: travel toward dest at mp.speed, then wait out the point's delay and move on.
static void xEntMPMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    _xEntMPData* mp = &motion->mp;

    if (mp->dest == mp->src || !mp->dest)
    {
        return;
    }

    if (motion->tmr > 0.0f)
    {
        motion->tmr -= dt;
        return;
    }

    frame->mode = 2;

    F32 newdist = mp->curdist + mp->speed * dt;

    if (newdist >= mp->dist)
    {
        frame->dpos.x = mp->dest->pos->x - frame->mat.pos.x;
        frame->dpos.y = mp->dest->pos->y - frame->mat.pos.y;
        frame->dpos.z = mp->dest->pos->z - frame->mat.pos.z;

        if (motion->asset->mp.flags & 1)
        {
            xEntMotionStop(motion);
        }

        _xMovePoint* prev = mp->src;
        mp->src = mp->dest;
        xEntMPGetNext(motion, prev, sc);

        motion->tmr = mp->src->asset->delay;
        return;
    }

    mp->curdist = newdist;

    F32 qdot;
    xVec3 tgt, dir;
    xQuat quat, qold;
    xMat3x3 tmpmat;

    if (mp->spl)
    {
        F32 u = xSpline3_EvalArcApprox(mp->spl, newdist, 0, &tgt);

        if (motion->asset->flags & 1)
        {
            xSpline3_EvalSeg(mp->spl, u, 1, &dir);

            if (xVec3Length2(&dir) < 0.000001f)
            {
                if (u < 0.1f)
                {
                    u += 0.01f;
                }
                else
                {
                    u -= 0.01f;
                }

                xSpline3_EvalSeg(mp->spl, u, 1, &dir);
            }

            xVec3Inv(&dir, &dir);
            xMat3x3LookVec(&tmpmat, &dir);
            xQuatFromMat(&quat, &tmpmat);
            xQuatFromMat(&qold, (xMat3x3*)&frame->mat);

            qdot = xQuatDot(&quat, &qold);

            if (qdot < 0.0f)
            {
                xQuatFlip(&quat, &quat);
                qdot = -qdot;
            }

            if (qdot > 1.0f)
            {
                qdot = 1.0f;
            }

            qdot = 2.0f * xacos(qdot);

            if (qdot <= PI * dt)
            {
                frame->mode |= 0x10;
                *(xMat3x3*)&frame->mat = tmpmat;
            }
            else
            {
                qdot = PI * dt / qdot;
                xQuatSlerp(&quat, &qold, &quat, qdot);
                xQuatNormalize(&quat, &quat);
                xQuatToMat(&quat, (xMat3x3*)&frame->mat);
            }
        }
    }
    else
    {
        xVec3Lerp(&tgt, mp->src->pos, mp->dest->pos, mp->curdist / mp->dist);

        if ((motion->asset->flags & 1) && !xQuatEquals(&mp->aquat, &mp->bquat))
        {
            xQuatSlerp(&quat, &mp->aquat, &mp->bquat, MIN(1.0f, 2.0f * (mp->curdist / mp->dist)));
            xQuatToMat(&quat, (xMat3x3*)&frame->mat);
            frame->mode |= 0x10;
        }
    }

    frame->dpos.x = tgt.x - frame->mat.pos.x;
    frame->dpos.y = tgt.y - frame->mat.pos.y;
    frame->dpos.z = tgt.z - frame->mat.pos.z;
}

// Pendulum: swing about the model's at axis around the pivot (pen.top), snapping back each period.
static void xEntPenMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    _xEntPenData* pen = &motion->pen;
    p2EntMotionPenData* aspen = &motion->asset->pen;
    F32 v = pen->w;
    F32 th = 0.5f * dt;
    F32 rem = motion->t + th;
    F32 dangle = (2.0f * aspen->range) * isin(th * v) * icos(rem * v + aspen->phase);
    F32 newt = motion->t - aspen->period;

    if (newt > 0.0f)
    {
        motion->t = newt;
    }

    if (-newt < dt && -newt > 0.0f)
    {
        xMat4x3Copy(&frame->mat, &motion->pen.omat);
        frame->mode = 0x11;
    }
    else
    {
        xMat4x3Rot(&frame->dmat, &frame->mat.at, dangle, &pen->top);
        frame->mode = 0x40;
    }
}

// Mechanism: a state machine of slides and rotations (out, wait, back, wait) driven by the asset.
static void xEntMechMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    p2EntMotionMechData* mkasst = &motion->asset->mech;
    _xEntMechData* mech = &motion->mech;
    frame->mode = 0;
    U32 last = 0;

    if (mech->state == 0)
    {
        last = xEntSldMove(motion, sc, dt, frame);

        if (mkasst->type == 2)
        {
            xEntRotMove(motion, sc, dt, frame);
        }

        if (last)
        {
            if (mkasst->type == 3)
            {
                mech->state = 1;
            }
            else if (mkasst->flags & 1)
            {
                mech->state = 2;
                mech->ss = -mech->ss;
                mech->sr = -mech->sr;
            }
            else if (mkasst->flags & 2)
            {
                mech->state = 6;
            }
            else
            {
                mech->state = 5;
            }
        }
    }
    else if (mech->state == 1)
    {
        last = xEntRotMove(motion, sc, dt, frame);

        if (last)
        {
            if (mkasst->type == 4)
            {
                mech->state = 0;
            }
            else if (mkasst->flags & 1)
            {
                mech->state = 2;
                mech->ss = -mech->ss;
                mech->sr = -mech->sr;
            }
            else if (mkasst->flags & 2)
            {
                mech->state = 6;
            }
            else if (mkasst->post_ret_delay != 0.0f)
            {
                mech->state = 5;
            }
        }
    }
    else if (mech->state == 2)
    {
        if (motion->tmr >= mkasst->ret_delay)
        {
            if (mkasst->type == 0 || mkasst->type == 2 || mkasst->type == 4)
            {
                mech->state = 3;
            }
            else
            {
                mech->state = 4;
            }

            last = 1;
        }
    }
    else if (mech->state == 3)
    {
        last = xEntSldMove(motion, sc, dt, frame);

        if (mkasst->type == 2)
        {
            xEntRotMove(motion, sc, dt, frame);
        }

        if (last)
        {
            if (mkasst->type == 4)
            {
                mech->state = 4;
            }
            else if (mkasst->flags & 2)
            {
                mech->state = 7;
            }
            else
            {
                mech->state = 5;
            }
        }
    }
    else if (mech->state == 4)
    {
        last = xEntRotMove(motion, sc, dt, frame);

        if (last)
        {
            if (mkasst->type == 3)
            {
                mech->state = 3;
            }
            else if (mkasst->flags & 2)
            {
                mech->state = 7;
            }
            else
            {
                mech->state = 5;
            }
        }
    }
    else if (mech->state == 5 && motion->tmr >= mkasst->post_ret_delay)
    {
        xEntMotionReset(motion, sc);
        xEntMotionRun(motion);
        last = 1;
    }

    if (last)
    {
        motion->tmr = 0.0f;
    }
    else
    {
        motion->tmr += dt;
    }
}

// Slide the mechanism along mech.dir, easing in over sld_acc_tm and out over sld_dec_tm.
static inline U32 xEntSldMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    _xEntMechData* mech = &motion->mech;
    p2EntMotionMechData* mkasst = &motion->asset->mech;
    U32 last;
    F32 rem = mkasst->sld_tm - motion->tmr;
    F32 speed;

    if (dt - rem > 0.0f)
    {
        if (mech->state == 0)
        {
            xVec3Sub(&frame->dpos, &mech->bpos, &frame->mat.pos);
        }
        else
        {
            xVec3Sub(&frame->dpos, &mech->apos, &frame->mat.pos);
        }

        last = 1;
    }
    else
    {
        if (mech->state == 0)
        {
            if (motion->tmr < mkasst->sld_acc_tm)
            {
                speed = mech->ss * motion->tmr / mkasst->sld_acc_tm;
            }
            else if (motion->tmr > mech->tsfd)
            {
                speed = mech->ss * rem / mkasst->sld_dec_tm;
            }
            else
            {
                speed = mech->ss;
            }
        }
        else if (motion->tmr < mkasst->sld_dec_tm)
        {
            speed = mech->ss * motion->tmr / mkasst->sld_dec_tm;
        }
        else if (motion->tmr > mech->tsbd)
        {
            speed = mech->ss * rem / mkasst->sld_acc_tm;
        }
        else
        {
            speed = mech->ss;
        }

        xVec3SMul(&frame->dpos, &mech->dir, speed * dt);

        last = 0;
    }

    frame->mode |= 2;

    return last;
}

// Turn the mechanism about its rotation axis, easing in over rot_acc_tm and out over rot_dec_tm.
static inline U32 xEntRotMove(_xEntMotion* motion, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    _xEntMechData* mech = &motion->mech;
    p2EntMotionMechData* mkasst = &motion->asset->mech;
    U32 last;
    F32 rem = mkasst->rot_tm - motion->tmr;
    F32 speed;
    _xEnt* ownr;
    p2EntAsset* easst;

    if (dt - rem > 0.0f)
    {
        if (mech->state == 1 || (mkasst->type == 2 && mech->state == 0))
        {
            *mech->rotptr = mech->brot;
        }
        else
        {
            *mech->rotptr = mech->arot;
        }

        last = 1;
    }
    else
    {
        if (mech->state == 1 || (mkasst->type == 2 && mech->state == 0))
        {
            if (motion->tmr < mkasst->rot_acc_tm)
            {
                speed = mech->sr * motion->tmr / mkasst->rot_acc_tm;
            }
            else if (motion->tmr > mech->trfd)
            {
                speed = mech->sr * rem / mkasst->rot_dec_tm;
            }
            else
            {
                speed = mech->sr;
            }
        }
        else if (motion->tmr < mkasst->rot_dec_tm)
        {
            speed = mech->sr * motion->tmr / mkasst->rot_dec_tm;
        }
        else if (motion->tmr > mech->trbd)
        {
            speed = mech->sr * rem / mkasst->rot_acc_tm;
        }
        else
        {
            speed = mech->sr;
        }

        *mech->rotptr = xAngleClamp(speed * dt + *mech->rotptr);

        last = 0;
    }

    xMat3x3Euler((xMat3x3*)&frame->mat, frame->rot.axis.x, frame->rot.axis.y, frame->rot.axis.z);

    ownr = motion->owner;

    if (ownr)
    {
        easst = ownr->asset;
        xVec3SMulBy(&frame->mat.right, easst->scale.x);
        xVec3SMulBy(&frame->mat.up, easst->scale.y);
        xVec3SMulBy(&frame->mat.at, easst->scale.z);
    }

    frame->mode |= 0x10;

    return last;
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

// Shift the motion's fixed points by dpos when the owner is moved (move points stay put).
void xEntMotionTranslate(_xEntMotion* motion, const xVec3* dpos)
{
    if (motion->type == XENTMOTION_TYPE_ER)
    {
        xVec3AddTo(&motion->er.a, dpos);
        xVec3AddTo(&motion->er.b, dpos);
    }
    else if (motion->type == XENTMOTION_TYPE_ORB)
    {
        xVec3AddTo(&motion->orb.orig, dpos);
        xVec3AddTo(&motion->orb.c, dpos);
    }
    else if (motion->type == XENTMOTION_TYPE_MP)
    {
        return;
    }
    else if (motion->type == XENTMOTION_TYPE_PEN)
    {
        xVec3AddTo(&motion->pen.top, dpos);
        xVec3AddTo(&motion->pen.omat.pos, dpos);
    }
    else if (motion->type == XENTMOTION_TYPE_MECH)
    {
        xVec3AddTo(&motion->mech.apos, dpos);
        xVec3AddTo(&motion->mech.bpos, dpos);
    }
}

static _xEntMotion** dbg_xems;
static U16 dbg_num;
static U16 dbg_num_allocd;
static S16 dbg_idx = -1;

void xEntMotionDebugCB();

// Make room to register num_xems motions for the DBG_XENTMOTION debug mode.
void xEntMotionDebugInit(U16 num_xems)
{
    if (num_xems)
    {
        xDebugModeAdd("DBG_XENTMOTION", xEntMotionDebugCB);
        dbg_num = 0;
        dbg_xems = (_xEntMotion**)xMemAlloc(gActiveHeap, num_xems * sizeof(_xEntMotion*));
        dbg_num_allocd = num_xems;
        dbg_idx = 0;
    }
}

void xEntMotionDebugExit()
{
    dbg_num = 0;
    dbg_xems = NULL;
    dbg_num_allocd = 0;
    dbg_idx = -1;
}

static void xEntMotionDebugAdd(_xEntMotion* motion)
{
    if (dbg_num < dbg_num_allocd)
    {
        dbg_xems[dbg_num++] = motion;
    }
}

// DBG_XENTMOTION debug mode: print and draw the selected motion, then read the debug pad.
void xEntMotionDebugCB()
{
    _xEntMotion* xem;

    if (dbg_idx != -1 && dbg_num != 0)
    {
        xem = dbg_xems[dbg_idx];
        xEntMotionDebugWrite(xem);
        xEntMotionDebugDraw(xem);
        xEntMotionDebugIPad(xem);
    }
}

// Debug pad: step through the registered motions, reset the selected one, or pause it.
static void xEntMotionDebugIPad(_xEntMotion* xem)
{
    if (gDebugPad->pressed & 0x20)
    {
        dbg_idx++;

        if (dbg_idx >= dbg_num)
        {
            dbg_idx = 0;
        }
    }

    if (gDebugPad->pressed & 0x80)
    {
        dbg_idx--;

        if (dbg_idx < 0)
        {
            dbg_idx = dbg_num - 1;
        }
    }

    if (gDebugPad->pressed & 0x10000)
    {
        if (xem->owner)
        {
            xEntReset(xem->owner);
        }

        xEntMotionReset(xem, g_xSceneCur);
    }

    if (gDebugPad->pressed & 0x20000)
    {
        if (xEntMotionIsStopped(xem))
        {
            xEntMotionRun(xem);
        }
        else
        {
            xEntMotionStop(xem);
        }
    }
}
