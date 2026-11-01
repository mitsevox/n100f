#include "xEnt.h"

#include "xBound.h"
#include "xDebug.h"
#include "xEvent.h"
#include "xFX.h"
#include "xFFX.h"
#include "xMath.h"
#include "xMathInlines.h"
#include "xVec3Inlines.h"
#include "xMemMgr.h"
#include "xPad.h"
#include "xScene.h"
#include "xShadow.h"
#include "xstransvc.h"
#include "xSurface.h"
#include "iCollide.h"
#include "iMath3.h"
#include "iModel.h"
#include "iShadow.h"
#include "iTime.h"
#include "zGlobals.h"
#include "zGroup.h"
#include "zLight.h"
#include "zScene.h"
#include "zSurface.h"

#include <rwcore.h>
#include <stdio.h>
#include <string.h>

// One record per model the debug collision timer has seen: who owns it, how many entities
// use it, and the time spent colliding against it.
struct xModCollRec
{
    _xEnt* owner;
    U32 nusing;
    S64 ctime;
    S64 ltime;
    F32 frac;
};

void xBaseSetup(xBase* xb);

static void xEntUpdateTimer(_xEnt* ent);
static void xEntCollCheckEnv(_xEnt* p, _xScene* sc);
static void xEntRenderCullBack(_xEnt* ent);
static void xEntDebugInit();
static void xEntDebugCB();
static void xEntDebugWrite(const _xEnt* xent);
static void xEntDebugIPad(_xEnt* xent);
static void xModCollInit();
static void xModCollAdd(_xEnt* xent);
static void xModCollCB();
void xModCollAddSample(_xEnt* xent, S64 elapsed);

static inline F32 icos(F32 x)
{
    return cos(x);
}

static inline F32 xVec3Dist2(const xVec3* a, const xVec3* b)
{
    F32 dx = a->x - b->x;
    F32 dy = a->y - b->y;
    F32 dz = a->z - b->z;

    return dx * dx + dy * dy + dz * dz;
}

static F32 nsn_angle = DEG2RAD(30);
static F32 sEntityTimePassed;
static _xBox all_ents_box;
static S32 all_ents_box_init;
static _xEnt** dbg_xents;
static U16 dbg_num;
static S16 dbg_idx = -1;
static S32 dbg_mode;
static S32 dbg_holding_inc;
static S32 dbg_holding_dec;
static F32 dbg_tmr;
static F32 dbg_ltm;
static xModCollRec* mcrecs;
static S32 num_mcrecs;
static S64 mctime;
static S32 mcmode = 2;
static xModCollRec smcrecs[101];

static void xEntUpdateTimer(_xEnt* ent)
{
    ent->updateTimer = 0.0f;
    ent->updateAcc = 0.0f;
}

void xEntSetTimePassed(F32 sec)
{
    sEntityTimePassed = sec;
}

void xEntSceneInit()
{
    xEntDebugInit();
    xModCollInit();
    all_ents_box_init = 1;
}

void xEntSceneExit()
{
    xEntDebugInit();
    xModCollInit();
}

// Render callback for one entity (asset id 0x67751F69): draws it with back faces culled, then
// turns culling off again.
static void xEntRenderCullBack(_xEnt* ent)
{
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
    xEntRender(ent);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
}

void xEntInit(_xEnt* ent, p2EntAsset* asset)
{
    xBaseInit(ent, asset);

    ent->asset = asset;
    ent->update = xEntUpdate;
    ent->bupdate = xEntDefaultBoundUpdate;
    ent->render = xEntRender;
    ent->updateTimerFunc = xEntUpdateTimer;
    ent->updateAcc = 0.0f;
    ent->move = NULL;
    ent->transl = xEntDefaultTranslate;
    ent->flags = asset->flags;
    ent->moreFlags = asset->moreFlags;
    ent->subType = asset->subtype;
    ent->pflags = asset->pflags;
    ent->ffx = NULL;
    ent->num_ffx = 0;
    ent->driver = NULL;
    ent->model = NULL;
    ent->frame = NULL;
    ent->collis = NULL;
    ent->surf = NULL;
    ent->material_alpha = 255.0f;
    ent->fade_tmr = 0.0f;
    ent->ent_shadow = NULL;
}

void xEntInitForType(_xEnt* ent)
{
    ent->update = xEntUpdate;
    ent->render = xEntRender;
    ent->updateTimerFunc = xEntUpdateTimer;
    ent->updateAcc = 0.0f;

    if (ent->id == 0x67751F69)
    {
        ent->render = xEntRenderCullBack;
    }

    if (ent->collType == 0x1)
    {
        ent->pflags &= (U8)~0x3;
        ent->chkby = 0x18;
        ent->penby = 0;
    }
    else if (ent->collType == 0x2)
    {
        ent->pflags &= (U8)~0x3;
        ent->chkby = 0x18;
        ent->penby = 0x18;
    }
    else if (ent->collType == 0x4)
    {
        ent->pflags |= 0x1;
        ent->move = NULL;
        ent->frame = (_xEntFrame*)xMemAlloc(gActiveHeap, sizeof(_xEntFrame));
        memset(ent->frame, 0, sizeof(_xEntFrame));
        ent->pflags &= (U8)~0x2;
        ent->chkby = 0x18;
        ent->penby = 0x18;
    }
    else if (ent->collType == 0x8)
    {
        ent->pflags |= 0x1;
        ent->move = NULL;
        ent->frame = (_xEntFrame*)xMemAlloc(gActiveHeap, sizeof(_xEntFrame));
        memset(ent->frame, 0, sizeof(_xEntFrame));
        ent->pflags |= 0x2;
        ent->chkby = 0x10;
        ent->penby = 0x10;
        ent->collis = (_xEntCollis*)xMemAlloc(gActiveHeap, sizeof(_xEntCollis));
        ent->collis->chk = 0x26;
        ent->collis->pen = 0x26;
        ent->collis->post = NULL;
        ent->collis->depenq = NULL;
    }
    else if (ent->collType == 0x10)
    {
        ent->pflags |= 0x1;
        ent->move = NULL;
        ent->frame = (_xEntFrame*)xMemAlloc(gActiveHeap, sizeof(_xEntFrame));
        memset(ent->frame, 0, sizeof(_xEntFrame));
        ent->pflags |= 0x2;
        ent->chkby = 0;
        ent->penby = 0;
        ent->collis = (_xEntCollis*)xMemAlloc(gActiveHeap, sizeof(_xEntCollis));
        ent->collis->chk = 0x2F;
        ent->collis->pen = 0x2E;
        ent->collis->post = NULL;
        ent->collis->depenq = NULL;
    }

    // Entities with a bound go on the debug list and into the collision timer.
    if (ent->bound.type)
    {
        if (dbg_num < 319)
        {
            dbg_xents[dbg_num++] = ent;
            xModCollAdd(ent);
        }
    }
}

void xEntSetup(_xEnt* ent)
{
    S32 i;
    p2LinkAsset* la;
    _xEnt* dent;

    xBaseSetup(ent);

    if (ent->asset->surfaceID)
    {
        ent->surf = (xSurface*)xSceneResolvID(g_xSceneCur, ent->asset->surfaceID);

        if (ent->surf)
        {
            ent->surf->type = 1;
            ent->surf->ent = ent;
        }
    }
    else
    {
        ent->surf = NULL;
    }

    for (i = 0; i < ent->linkCount; i++)
    {
        la = &ent->link[i];

        if (la->dstEvent == eEventDrivenby)
        {
            dent = (_xEnt*)xSceneResolvID(g_xSceneCur, la->dstAssetID);

            if (dent)
            {
                ent->driver = dent;
            }
        }
    }
}

void xEntSave(_xEnt* ent, xSerial* s)
{
    xBaseSave(ent, s);

    if (xEntIsVisible(ent))
    {
        s->Write_b1(1);
    }
    else
    {
        s->Write_b1(0);
    }
}

void xEntLoad(_xEnt* ent, xSerial* s)
{
    S32 b;

    xBaseLoad(ent, s);

    b = 0;
    s->Read_b1(&b);

    if (b)
    {
        xEntShow(ent);
    }
    else
    {
        xEntHide(ent);
    }
}

void xEntReset(_xEnt* ent)
{
    xMat4x3 frame;

    xBaseReset(ent, ent->asset);

    ent->flags = ent->asset->flags;
    ent->moreFlags = ent->asset->moreFlags;

    xMat3x3Euler((xMat3x3*)&frame, ent->asset->ang.x, ent->asset->ang.y, ent->asset->ang.z);

    xVec3SMul(&frame.right, &frame.right, ent->asset->scale.x);
    xVec3SMul(&frame.up, &frame.up, ent->asset->scale.y);
    xVec3SMul(&frame.at, &frame.at, ent->asset->scale.z);
    xVec3Copy(&frame.pos, &ent->asset->pos);

    frame.flags = 0;

    if (ent->model)
    {
        xMat4x3Copy((xMat4x3*)ent->model->Mat, &frame);

        if (ent->bound.type == 2)
        {
            iBoxForModel(&ent->bound.box.box, ent->model);
        }
        else if (ent->bound.type == 4)
        {
            iBoxForModelLocal(&ent->bound.box.box, ent->model);
        }
    }

    if (ent->frame)
    {
        xMat4x3Copy(&ent->frame->mat, &frame);
        xVec3Copy(&ent->frame->oldpos, &ent->frame->mat.pos);
        xVec3Copy(&ent->frame->vel, &g_O3);
        xVec3Copy(&ent->frame->oldvel, &g_O3);
        xVec3Copy(&ent->frame->rot.axis, &ent->asset->ang);

        ent->frame->rot.angle = 0.0f;

        xRotCopy(&ent->frame->oldrot, &ent->frame->rot);
    }

    switch (ent->bound.type)
    {
    case 0:
        break;
    case 1:
        ent->bound.sph.center.x = ent->asset->pos.x;
        ent->bound.sph.center.y = ent->asset->pos.y;
        ent->bound.sph.center.z = ent->asset->pos.z;
        break;
    }

    ent->num_updates = irand() & 127;

    if (ent->bound.type)
    {
        if (all_ents_box_init)
        {
            iBoxInitBoundVec(&all_ents_box, &frame.pos);
            all_ents_box_init = 0;
        }
        else
        {
            iBoxBoundVec(&all_ents_box, &all_ents_box, &frame.pos);
        }
    }

    if (ent->collType == 0x10 || ent->collType == 0x8)
    {
        for (S32 i = 0; i < 8; i++)
        {
            ent->collis->trigs_cur[i] = 0;
            ent->collis->trigs_prev[i] = 0;
            ent->collis->trigs_entered[i] = 0;
            ent->collis->trigs_exited[i] = 0;
        }
    }
}

xModelInstance* xEntLoadModel(_xEnt* ent, RpAtomic* imodel)
{
    xModelInstance* model;

    model = xModelInstanceAlloc(imodel, ent, 0, 0, NULL);

    while (imodel = iModelFile_RWMultiAtomic(imodel))
    {
        xModelInstanceAttach(xModelInstanceAlloc(imodel, ent, 0x8, 0, NULL), model);
    }

    if (ent)
    {
        ent->model = model;
    }

    return model;
}

void xEntUpdate(_xEnt* ent, _xScene* sc, F32 dt)
{
    xEntBeginUpdate(ent, sc, dt);

    if (ent->pflags & 0x2)
    {
        xEntApplyPhysics(ent, sc, dt);
    }

    if (ent->pflags & 0x1)
    {
        xEntMove(ent, sc, dt);
    }

    xFFXApply(ent, sc, dt);

    if (ent->collis)
    {
        xEntCollide(ent, sc, dt);
    }

    xEntEndUpdate(ent, sc, dt);
}

void xEntBeginUpdate(_xEnt* ent, _xScene* sc, F32 dt)
{
    if (ent->model)
    {
        xModelUpdate(ent->model, dt);

        if (ent->frame)
        {
            xVec3Copy(&ent->frame->oldvel, &ent->frame->vel);
            xVec3Copy(&ent->frame->oldpos, &ent->frame->mat.pos);
            xRotCopy(&ent->frame->oldrot, &ent->frame->rot);
            xMat4x3Copy(&ent->frame->mat, xModelGetFrame(ent->model));

            ent->frame->mode = 0;
        }
    }

    if (ent->fade_tmr > 0.0f)
    {
        ent->fade_tmr -= dt;
    }
}

void xEntEndUpdate(_xEnt* ent, _xScene* sc, F32 dt)
{
    ent->num_updates++;

    if (ent->model)
    {
        if (ent->frame)
        {
            if (!(ent->frame->mode & 0x20000))
            {
                xMat3x3Copy((xMat3x3*)ent->model->Mat, (xMat3x3*)&ent->frame->mat);
            }

            if (!(ent->frame->mode & 0x10000))
            {
                xVec3* mpos = (xVec3*)&ent->model->Mat->pos;
                xVec3Copy(mpos, &ent->frame->mat.pos);
            }
        }

        if (ent->bupdate)
        {
            xVec3* upos = (xVec3*)&ent->model->Mat->pos;
            ent->bupdate(ent, upos);
        }

        xModelEval(ent->model);
    }
}

void xEntDefaultBoundUpdate(_xEnt* ent, xVec3* pos)
{
    if (ent->bound.type == 1)
    {
        xVec3Copy(&ent->bound.sph.center, pos);

        ent->bound.sph.center.y += 0.7f;
        ent->bound.sph.r = 0.7f;
    }

    xBoundUpdate(&ent->bound);
}

void xEntDefaultTranslate(_xEnt* ent, xVec3* dpos)
{
    if (ent->model)
    {
        xVec3AddTo(xEntGetPos(ent), dpos);
    }

    if (ent->frame)
    {
        xVec3AddTo(&ent->frame->mat.pos, dpos);
    }

    xVec3AddTo(xBoundCenter(&ent->bound), dpos);
}

// Applies the frame's rotation: Euler angles when mode bit 0x400 is set, an axis and angle
// otherwise.
static void xEntRotationToMatrix(_xEntFrame* frame)
{
    if (frame->mode & 0x20)
    {
        if (frame->mode & 0x400)
        {
            xVec3AddTo(&frame->rot.axis, &frame->drot.axis);
            xMat3x3Euler((xMat3x3*)&frame->mat, frame->rot.axis.x, frame->rot.axis.y, frame->rot.axis.z);
        }
        else
        {
            frame->rot.angle = xAngleClamp(frame->rot.angle + frame->drot.angle);

            xMat3x3RotC((xMat3x3*)&frame->mat, frame->rot.axis.x, frame->rot.axis.y, frame->rot.axis.z,
                        frame->rot.angle);
        }
    }
}

void xEntMotionToMatrix(_xEnt* ent, _xEntFrame* frame)
{
    if (frame->mode & 0x80)
    {
        if (frame->mode & 0x800)
        {
            xMat4x3RMul3x3(&frame->mat, (xMat3x3*)&frame->dmat, &frame->mat);
        }
        else
        {
            xMat4x3LMul3x3(&frame->mat, &frame->mat, (xMat3x3*)&frame->dmat);
        }
    }
    else if (frame->mode & 0x40)
    {
        if (frame->mode & 0x800)
        {
            xMat4x3Mul(&frame->mat, &frame->dmat, &frame->mat);
        }
        else
        {
            xMat4x3Mul(&frame->mat, &frame->mat, &frame->dmat);
        }
    }

    if (frame->mode & 0x1000)
    {
        xEntRotationToMatrix(frame);
    }

    if (frame->mode & 0x2)
    {
        if (frame->mode & 0x800)
        {
            xMat3x3RMulVec(&frame->dpos, (xMat3x3*)&frame->mat, &frame->dpos);
        }

        xEntAddToPos(ent, &frame->dpos);
    }

    if (frame->mode & 0x8)
    {
        if (frame->mode & 0x800)
        {
            xMat3x3RMulVec(&frame->dvel, (xMat3x3*)&frame->mat, &frame->dvel);
        }

        xVec3AddTo(&frame->vel, &frame->dvel);
    }

    if (!(frame->mode & 0x1000))
    {
        xEntRotationToMatrix(frame);
    }
}

void xEntMove(_xEnt* ent, _xScene* sc, F32 dt)
{
    ent->move(ent, sc, dt, ent->frame);

    xEntMotionToMatrix(ent, ent->frame);

    if (ent->driver)
    {
        _xEntFrame* dframe = ent->driver->frame;
        xVec3 dpos;

        xVec3Sub(&dpos, &dframe->mat.pos, &dframe->oldpos);

        ent->transl(ent, &dpos);
    }
}

void xEntApplyPhysics(_xEnt* ent, _xScene* sc, F32 dt)
{
    xVec3 dposvel;

    if (ent->pflags & 0x4 && sc->flags & 0x1)
    {
        ent->frame->vel.y += sc->gravity * dt;
    }

    if (ent->pflags & 0x10 && sc->flags & 0x2)
    {
        F32 tfric = -(sc->friction * dt - 1.0f);
        xVec3SMul(&ent->frame->vel, &ent->frame->vel, tfric);
    }

    if (ent->pflags & 0x8 && sc->flags & 0x4)
    {
        F32 tdrag = -(sc->drag * dt - 1.0f);
        xVec3SMul(&ent->frame->vel, &ent->frame->vel, tdrag);
    }

    xVec3Add(&dposvel, &ent->frame->vel, &ent->frame->oldvel);
    xVec3SMul(&dposvel, &dposvel, 0.5f * dt);

    // Limit a falling step to 0.63 units per 30th of a second.
    if (dposvel.y < 0.0f)
    {
        F32 scale = (0.63f * (30.0f * dt)) / xVec3Length(&dposvel);

        if (scale < 1.0f)
        {
            xVec3SMul(&dposvel, &dposvel, scale);
        }
    }

    xEntAddToPos(ent, &dposvel);
}

void xEntCollide(_xEnt* ent, _xScene* sc, F32 dt)
{
    if (ent->model)
    {
        if (ent->collis->chk & 0x2E)
        {
            xEntBeginCollide(ent, sc, dt);
        }

        if (ent->collis->chk & 0x8)
        {
            xEntCollCheckNPCs(ent, sc, xEntCollCheckOneEntNoDepen);
        }

        if (ent->collis->chk & 0x4)
        {
            xEntCollCheckDyns(ent, sc, xEntCollCheckOneEntNoDepen);
        }

        if (ent->collis->chk & 0x2)
        {
            xEntCollCheckStats(ent, sc, xEntCollCheckOneEntNoDepen);
        }

        if (ent->collis->chk & 0x20)
        {
            xEntCollCheckEnv(ent, sc);
        }

        if (ent->collis->chk & 0x2E)
        {
            _xCollis* coll = &ent->collis->colls[0];
            F32 h_dot_n = (ent->bound.type == 1) ? ent->bound.sph.r : 0.7f;

            if (ent->pflags & 0x80 && coll->flags & 0x1)
            {
                F32 depen_len = xVec3Dot(&coll->hdng, &coll->norm);

                if (depen_len > 0.0f)
                {
                    xVec3Inv(&coll->norm, &coll->norm);
                    depen_len = -depen_len;
                }

                depen_len = depen_len * coll->dist + h_dot_n;

                if (depen_len < 0.0f || depen_len > h_dot_n)
                {
                    depen_len = CLAMP(depen_len, 0.0f, h_dot_n);
                }

                xVec3SMul(&coll->depen, &coll->norm, depen_len);
            }

            if (ent->frame->vel.y <= 0.0f)
            {
                xEntCollideFloor(ent, sc, dt);
            }
            else
            {
                xEntCollideCeiling(ent, sc, dt);
            }

            {
                _xCollis* coll = &ent->collis->colls[0];

                // The player keeps a floor below it even when nothing was hit this frame.
                if (!(coll->flags & 0x1) && ent->collType == 0x10)
                {
                    _xRay3 flray;

                    xVec3Copy(&flray.origin, xBoundCenter(&ent->bound));
                    xVec3Copy(&flray.dir, &g_NY3);
                    flray.max_t = 10.0f;
                    flray.flags = 0x800;

                    xRayHitsScene(sc, &flray, coll);

                    coll->flags &= ~0x1;
                }
            }

            xEntCollideWalls(ent, sc, dt);
            xEntEndCollide(ent, sc, dt);
        }

        if (ent->collis->chk & 0x1)
        {
            xEntBeginCollCheckTrigs(ent);
            xSceneForAllTriggers(sc, xEntCollCheckOneTrigNoDepen, ent);
            xEntEndCollCheckTrigs(ent);
        }
    }
}

void xEntBeginCollide(_xEnt* ent, _xScene* sc, F32 dt)
{
    _xCollis* coll;
    U8 idx;

    if (ent->bupdate)
    {
        ent->bupdate(ent, &ent->frame->mat.pos);
    }

    for (idx = 0; idx < 18; idx++)
    {
        coll = &ent->collis->colls[idx];

        coll->flags = 0x1F00;
        coll->optr = NULL;
        coll->dist = 1e19f;
    }

    ent->collis->idx = 6;
    ent->collis->stat_sidx = 6;
    ent->collis->stat_eidx = 6;
    ent->collis->dyn_sidx = 6;
    ent->collis->dyn_eidx = 6;
    ent->collis->npc_sidx = 6;
    ent->collis->npc_eidx = 6;
    ent->collis->env_sidx = 6;
    ent->collis->env_eidx = 6;
}

void xEntEndCollide(_xEnt* ent, _xScene* sc, F32 dt)
{
    if (ent->collis->post)
    {
        ent->collis->post(ent, sc, dt, ent->collis);
    }
}

static void xEntCollCheckEnv(_xEnt* p, _xScene* sc)
{
    _xCollis* coll;
    U8 ncolls;

    p->collis->env_sidx = p->collis->idx;

    coll = &p->collis->colls[p->collis->idx];
    coll->flags = 0x1F00;

    ncolls = 18 - p->collis->idx;

    p->collis->idx += (U8)iSphereHitsEnv3(&p->bound.sph, sc->env, coll, ncolls, 0.7853982f);
    p->collis->env_eidx = p->collis->idx;
}

void xEntCollCheckStats(_xEnt* p, _xScene* sc, _xEnt* (*hitIt)(_xEnt*, _xScene*, void*))
{
    p->collis->stat_sidx = p->collis->idx;
    xSceneForAllStatics(sc, hitIt, p);
    p->collis->stat_eidx = p->collis->idx;
}

void xEntCollCheckDyns(_xEnt* p, _xScene* sc, _xEnt* (*hitIt)(_xEnt*, _xScene*, void*))
{
    p->collis->dyn_sidx = p->collis->idx;
    xSceneForAllDynamics(sc, hitIt, p);
    p->collis->dyn_eidx = p->collis->idx;
}

void xEntCollCheckNPCs(_xEnt* p, _xScene* sc, _xEnt* (*hitIt)(_xEnt*, _xScene*, void*))
{
    p->collis->npc_sidx = p->collis->idx;
    xSceneForAllNPCs(sc, hitIt, p);
    p->collis->npc_eidx = p->collis->idx;
}

void xEntBeginCollCheckTrigs(_xEnt* ent)
{
    for (U16 i = 0; i < 8; i++)
    {
        ent->collis->trigs_prev[i] = ent->collis->trigs_cur[i];
    }

    ent->collis->trig_idx = 0;
}

void xEntEndCollCheckTrigs(_xEnt* ent)
{
    for (U16 i = 0; i < 8; i++)
    {
        ent->collis->trigs_entered[i] = ent->collis->trigs_cur[i] & ~ent->collis->trigs_prev[i];
        ent->collis->trigs_exited[i] = ent->collis->trigs_prev[i] & ~ent->collis->trigs_cur[i];
    }
}

_xEnt* xEntCollCheckOneEntNoDepen(_xEnt* ent, _xScene* sc, void* data)
{
    _xEnt* p = (_xEnt*)data;
    _xCollis* coll;
    U32 modl_coll = 0;

    if (p->collis->idx >= 15)
    {
        return NULL;
    }

    if (!(ent->chkby & p->collType))
    {
        return ent;
    }

    if (ent->id == p->id)
    {
        return ent;
    }

    coll = &p->collis->colls[p->collis->idx];

    if (ent->collLev == 5 && p->collType & 0x18)
    {
        modl_coll = 1;
    }

    if (modl_coll)
    {
        coll->flags = 0;
    }
    else
    {
        coll->flags = 0x1F00;
    }

    xBoundHitsBound(&p->bound, &ent->bound, coll);

    if (coll->flags & 0x1)
    {
        if (modl_coll)
        {
            S64 t0;
            U8 ncolls;
            S64 t1;
            U8 idx;

            coll->flags = 0x1F00;

            t0 = iTimeGet();
            ncolls = iSphereHitsModel3(&p->bound.sph, ent->model, coll, 15 - p->collis->idx,
                                       0.7853982f);
            t1 = iTimeGet();

            xModCollAddSample(ent, t1 - t0);

            for (idx = 0; idx < ncolls; idx++)
            {
                coll[idx].optr = ent;
                p->collis->idx++;
            }

            return ent;
        }
        else
        {
            coll->oid = ent->id;
            coll->optr = ent;

            p->collis->idx++;

            // Standing on a sphere: push straight up out of it.
            if (ent->pflags & 0x20 && ent->bound.type == 1 && p->bound.type == 1 &&
                coll->hdng.y < -0.866025f)
            {
                F32 rsum = p->bound.sph.r + ent->bound.sph.r;
                F32 dx = p->bound.sph.center.x - ent->bound.sph.center.x;
                F32 dy = p->bound.sph.center.y - ent->bound.sph.center.y;
                F32 dz = p->bound.sph.center.z - ent->bound.sph.center.z;
                F32 hsqr = SQR(rsum) - (SQR(dx) + SQR(dz));

                if (hsqr >= 0.0f)
                {
                    coll->depen.x = 0.0f;
                    coll->depen.y = xsqrt(hsqr) - dy;
                    coll->depen.z = 0.0f;
                    coll->dist = 0.7f - coll->depen.y;
                    coll->hdng.x = 0.0f;
                    coll->hdng.y = -1.0f;
                    coll->hdng.z = 0.0f;
                }
            }
        }
    }

    return ent;
}

_xEnt* xEntCollCheckOneTrigNoDepen(_xEnt* ent, _xScene* sc, void* data)
{
    _xEnt* p = (_xEnt*)data;
    U8 idx = p->collis->trig_idx++;

    p->collis->trigs_cur[idx / 8] &= (U8)~(1 << (idx % 8));

    return ent;
}

void xEntCollideFloor(_xEnt* p, _xScene* sc, F32 dt)
{
    _xCollis* coll = &p->collis->colls[0];
    U8 idx;
    _xCollis* ml = coll;
    xVec3 motion;
    F32 mlen;
    S32 stepping = 0;
    F32 sbr;

    if (p->bound.type == 1)
    {
        sbr = p->bound.sph.r;
    }
    else
    {
        sbr = 0.7f;
    }

    xVec3Copy(&motion, &p->frame->mat.pos);
    motion.x -= p->frame->oldpos.x;
    motion.z -= p->frame->oldpos.z;
    motion.y = 0.0f;
    mlen = xVec3Length(&motion);

    for (idx = 6; idx < p->collis->idx; idx++)
    {
        _xCollis* mf = &p->collis->colls[idx];

        if (mf->flags & 0x1)
        {
            _xEnt* fent = (_xEnt*)mf->optr;

            if (fent)
            {
                if ((fent->collType == 0x4 || fent->collType == 0x2) ||
                    (fent->pflags & 0x20 && 0.0f == mf->hdng.x && 0.0f == mf->hdng.z))
                {
                    if (!((p->collis->depenq) ?
                              p->collis->depenq(p, fent, sc, dt, mf) :
                              (fent->collType & p->collis->pen && fent->penby & p->collType)))
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
            else if (!(p->collis->pen & 0x20))
            {
                continue;
            }

            if (mf->dist < ml->dist)
            {
                if (mf->hdng.y < -icos(PI / 3) &&
                    (mf->norm.y > icos(nsn_angle) ||
                     p->frame->oldpos.y > dt * (sc->gravity * dt) + p->frame->mat.pos.y))
                {
                    ml = mf;
                    stepping = 0;
                }
                else if (mlen > 0.001f && mf->hdng.y < 0.65f / sbr - 1.0f &&
                         mf->norm.y > icos(nsn_angle))
                {
                    stepping = 1;
                    ml = mf;
                }
            }
        }
    }

    if (ml != coll)
    {
        F32 flr_dist = ml->dist * iabs(ml->hdng.y);

        *coll = *ml;

        if (flr_dist < sbr)
        {
            ml->flags |= 0x6;

            if (stepping)
            {
                p->frame->mat.pos.y += 1.5f * dt;
                p->frame->mat.pos.x += ml->depen.x;
                p->frame->mat.pos.z += ml->depen.z;
            }
            else
            {
                p->frame->mat.pos.y += ml->depen.y;
            }

            p->frame->vel.y = 0.0f;
        }
    }
}

void xEntCollideCeiling(_xEnt* p, _xScene* sc, F32 dt)
{
    _xCollis* coll = &p->collis->colls[1];
    U8 idx;
    _xCollis* ml = coll;
    F32 sbr;

    if (p->bound.type == 1)
    {
        sbr = p->bound.sph.r;
    }
    else
    {
        sbr = 0.7f;
    }

    for (idx = 6; idx < p->collis->idx; idx++)
    {
        _xCollis* mf = &p->collis->colls[idx];
        _xEnt* fent = (_xEnt*)mf->optr;

        if (fent)
        {
            if (!((p->collis->depenq) ?
                      p->collis->depenq(p, fent, sc, dt, mf) :
                      (fent->collType & p->collis->pen && fent->penby & p->collType)))
            {
                continue;
            }
        }
        else if (!(p->collis->pen & 0x20))
        {
            continue;
        }

        if (mf->hdng.y > icos(0.78539819f) && mf->dist < ml->dist)
        {
            ml = mf;
        }
    }

    if (ml != coll)
    {
        F32 ceil_dist = ml->dist * iabs(ml->hdng.y);

        *coll = *ml;

        ml->flags |= 0xA;

        if (ceil_dist < sbr)
        {
            p->frame->mat.pos.y -= sbr - ceil_dist;
            p->frame->vel.y = 0.0f;
        }
    }
}

void xEntCollideWalls(_xEnt* p, _xScene* sc, F32 dt)
{
    _xCollis* coll;
    _xEnt* cent;
    U8 idx, sidx;
    F32 sbr;

    if (p->bound.type == 1)
    {
        sbr = p->bound.sph.r;
    }
    else
    {
        sbr = 0.7f;
    }

    if (p->collis->pen & 0x8)
    {
        idx = p->collis->npc_eidx;
        sidx = p->collis->npc_sidx;

        while (sidx < idx)
        {
            coll = &p->collis->colls[sidx];
            cent = (_xEnt*)coll->optr;

            if (!(coll->flags & 0x2) && coll->dist < sbr &&
                ((p->collis->depenq) ? p->collis->depenq(p, cent, sc, dt, coll) :
                                       cent->penby & p->collType))
            {
                if (0.0f != coll->depen.x || 0.0f != coll->depen.z)
                {
                    coll->depen.y = 0.0f;
                }

                xEntAddToPos(p, &coll->depen);
            }

            sidx++;
        }
    }

    if (p->collis->pen & 0x4)
    {
        idx = p->collis->dyn_eidx;
        sidx = p->collis->dyn_sidx;

        while (sidx < idx)
        {
            coll = &p->collis->colls[sidx];
            cent = (_xEnt*)coll->optr;

            if (!(coll->flags & 0x2) && coll->dist < sbr &&
                ((p->collis->depenq) ? p->collis->depenq(p, cent, sc, dt, coll) :
                                       cent->penby & p->collType))
            {
                coll->depen.y = 0.0f;

                xEntAddToPos(p, &coll->depen);
            }

            sidx++;
        }
    }

    if (p->collis->pen & 0x2)
    {
        idx = p->collis->stat_eidx;
        sidx = p->collis->stat_sidx;

        while (sidx < idx)
        {
            coll = &p->collis->colls[sidx];
            cent = (_xEnt*)coll->optr;

            if (!(coll->flags & 0x2) && coll->dist < sbr &&
                ((p->collis->depenq) ? p->collis->depenq(p, cent, sc, dt, coll) :
                                       cent->penby & p->collType))
            {
                coll->depen.y = 0.0f;

                xEntAddToPos(p, &coll->depen);
            }

            sidx++;
        }
    }

    if (p->collis->pen & 0x20)
    {
        idx = p->collis->env_eidx;
        sidx = p->collis->env_sidx;

        while (sidx < idx)
        {
            coll = &p->collis->colls[sidx];

            if (!(coll->flags & 0x2) && coll->dist < sbr)
            {
                coll->depen.y = 0.0f;

                xEntAddToPos(p, &coll->depen);
            }

            sidx++;
        }
    }
}

// Draws a visible, unculled entity: fades its material alpha in or out, applies the surface's
// texture animation and UV scrolling, tints it by the asset's colour multipliers and lights it,
// then restores the shared model state and draws the shadow of the player and NPCs.
void xEntRender(_xEnt* ent)
{
    S32 we_altered_the_alpha_on_the_material;
    S32 setMaterialTextureRestore;
    S32 setPipeline;
    RxPipeline* oldPipe;
    S32 reset_material_colors;
    F32 rad;

    if (!ent->model || !xEntIsVisible(ent))
    {
        return;
    }

    if (iModelCull(ent->model->Data, ent->model->Mat))
    {
        return;
    }

    we_altered_the_alpha_on_the_material = 0;

    if (ent->flags & 0x4)
    {
        if (ent->fade_tmr <= 0.0f)
        {
            ent->flags ^= 0x4;
            ent->fade_tmr = 0.0f;
        }

        static S32 fadeout_type = -1;

        if (fadeout_type == 0)
        {
            ent->material_alpha -= ent->asset->seeThruSpeed * sEntityTimePassed;

            if (ent->asset->seeThru < 0.3f)
            {
                if (ent->material_alpha <= 0.0f)
                {
                    ent->material_alpha = 0.0f;
                }
            }
            else if (ent->material_alpha <= 255.0f * ent->asset->seeThru)
            {
                ent->material_alpha = 255.0f * ent->asset->seeThru;
            }
        }
        else if (fadeout_type == 1)
        {
            ent->material_alpha -= 2000.0f * sEntityTimePassed;

            if (ent->material_alpha <= 0.0f)
            {
                ent->material_alpha = 0.0f;
            }
        }
        else if (fadeout_type == 2)
        {
            ent->material_alpha -= 150.0f * sEntityTimePassed;

            if (ent->material_alpha <= 0.0f)
            {
                ent->material_alpha = 0.0f;
            }
        }
        else
        {
            ent->material_alpha -= ent->asset->seeThruSpeed * sEntityTimePassed;

            if (ent->material_alpha <= 255.0f * ent->asset->seeThru)
            {
                ent->material_alpha = 255.0f * ent->asset->seeThru;
            }
        }

        if (!globals.cmgr)
        {
            xModelSetMaterialAlpha(ent->model, (U8)ent->material_alpha);
            we_altered_the_alpha_on_the_material = 1;
        }
    }
    else if (ent->material_alpha < 255.0f)
    {
        ent->material_alpha += ent->asset->seeThruSpeed * sEntityTimePassed;

        if (ent->material_alpha > 255.0f)
        {
            ent->material_alpha = 255.0f;
        }

        if (!globals.cmgr)
        {
            xModelSetMaterialAlpha(ent->model, (U8)ent->material_alpha);
            we_altered_the_alpha_on_the_material = 1;
        }
    }

    setMaterialTextureRestore = 0;
    setPipeline = 0;

    if (ent->surf)
    {
        zSurfaceProps* pp = (zSurfaceProps*)ent->surf->moprops;

        if (pp)
        {
            if (pp->texanim_flags & 0x1)
            {
                _zGroup* g = (_zGroup*)zSceneFindObject(pp->texanim[0].group);

                if (g)
                {
                    U32 texid = zGroupGetItem(g, pp->texanim[0].group_idx);
                    RwTexture* texptr = (RwTexture*)xSTFindAsset(texid, NULL);

                    if (texptr)
                    {
                        xModelSetMaterialTexture(ent->model, texptr);
                        setMaterialTextureRestore = 1;
                    }
                }
            }

            if (pp->texanim_flags & 0x2)
            {
                _zGroup* g = (_zGroup*)zSceneFindObject(pp->texanim[1].group);

                if (g)
                {
                    U32 texid = zGroupGetItem(g, pp->texanim[1].group_idx);
                    RwTexture* texptr = (RwTexture*)xSTFindAsset(texid, NULL);

                    if (texptr)
                    {
                        xFXanimUV2PSetTexture(texptr);
                        setPipeline = 1;
                    }
                }
            }
            else if (pp->uvfx_flags & 0x2)
            {
                RwTexture* texptr = (RwTexture*)xSTFindAsset(pp->asset->matfx.dualmapID, NULL);

                if (texptr)
                {
                    xFXanimUV2PSetTexture(texptr);
                    setPipeline = 1;
                }
            }
            else
            {
                xFXanimUV2PSetTexture(NULL);
            }

            if (pp->uvfx_flags & 0x3)
            {
                setPipeline = 1;
                xFXanimUVSetTranslation(&pp->uvfx[0].trans);
                xFXanimUV2PSetTranslation(&pp->uvfx[1].trans);
                xFXanimUVSetScale(&pp->uvfx[0].scale);
                xFXanimUV2PSetScale(&pp->uvfx[1].scale);
                xFXanimUVSetAngle((1.0f / (ONEEIGHTY * PI)) * pp->uvfx[0].rot);
                xFXanimUV2PSetAngle((1.0f / (ONEEIGHTY * PI)) * pp->uvfx[1].rot);
            }

            if (setPipeline)
            {
                oldPipe = ent->model->Data->pipeline;
                xFXanimUVAtomicSetup(ent->model->Data);
            }
        }
    }

    reset_material_colors = 0;

    if (ent->asset->redMult != 1.0f || ent->asset->greenMult != 1.0f || ent->asset->blueMult != 1.0f)
    {
        reset_material_colors = 1;
        xModelMaterialMul(ent->model, ent->asset->redMult, ent->asset->greenMult, ent->asset->blueMult);
    }

    zLightAddLocal(ent);
    xModelRender(ent->model);
    zLightRemoveLocal();

    if (reset_material_colors)
    {
        xModelResetMaterial(ent->model);
    }

    if (we_altered_the_alpha_on_the_material)
    {
        xModelSetMaterialAlpha(ent->model, 255);
    }

    if (setMaterialTextureRestore)
    {
        xModelSetMaterialTextureRestore(ent->model);
    }

    if (setPipeline)
    {
        ent->model->Data->pipeline = oldPipe;
    }

    if (ent->baseType == 2 || ent->baseType == 3)
    {
        xVec3* pos = (xVec3*)&ent->model->Mat->pos;
        xVec3* campos = &globals.camera.hi_cam.mat.pos;

        if (xVec3Dist2(pos, campos) < ShadowThreshold || ent->baseType == 3)
        {
            xShadowRender(ent);
        }
        else
        {
            rad = ent->bound.sph.r;

            if (ent->baseType == 2)
            {
                switch (ent->subType)
                {
                case 0x2D:
                case 6:
                    rad *= 1.75f;
                    break;
                }
            }

            iShadowCollide(&ent->ishadow, ent, rad, 1.0f);
            iShadowAdd(&ent->ishadow, 0x50);
        }
    }
}

void xEntSetMaterialMul(_xEnt* ent, F32 r, F32 g, F32 b)
{
    ent->asset->redMult = r;
    ent->asset->greenMult = g;
    ent->asset->blueMult = b;
}

void xEntSetNostepNormAngle(F32 angle)
{
    nsn_angle = angle;
}

_xBox* xEntGetAllEntsBox()
{
    return &all_ents_box;
}

static void xEntDebugCB()
{
    _xEnt* xent;

    if (dbg_idx != -1 && dbg_num)
    {
        xent = dbg_xents[dbg_idx];

        xEntDebugWrite(xent);
        xEntDebugIPad(xent);
    }
}

// The six collision slots' names, in _xCollsIdx order.
static char scoll_name[6][6] = { "floor", "ceil", "front", "left", "rear", "right" };

// Appends the name of every collision type set in type to str.
static inline char* xEntCollTypeStrAll(char* str, U8 type)
{
    str[0] = '\0';

    if (type & 0x1)
    {
        strcat(str, "trig ");
    }
    if (type & 0x2)
    {
        strcat(str, "stat ");
    }
    if (type & 0x4)
    {
        strcat(str, "dyn ");
    }
    if (type & 0x8)
    {
        strcat(str, "npc ");
    }
    if (type & 0x10)
    {
        strcat(str, "pc ");
    }
    if (type & 0x20)
    {
        strcat(str, "env ");
    }
    if (type & 0x40)
    {
        strcat(str, "walls ");
    }

    return str;
}

// Debug text for the selected entity. Mode 1 shows its flags, position, velocity, scale and
// angles; modes 2 and 3 show its collision setup, then the hit counts (mode 2) or the six
// collision slots (mode 3).
static void xEntDebugWrite(const _xEnt* xent)
{
    char str[40];

    xprintf("name:         %s", xSceneID2Name(g_xSceneCur, xent->id));
    xprintf("   id: 0x%08x\n", xent->id);

    if (dbg_mode == 1)
    {
        xprintf("is enabled:   %s\n", xbtoa(xent->baseFlags & 0x1));
        xprintf("is visible:   %s\n", xbtoa(xent->flags & 0x1));
        xprintf("is moving:    %s\n", xbtoa(xent->pflags & 0x1));
        xprintf("has velocity: %s\n", xbtoa(xent->pflags & 0x2));
        xprintf("has gravity:  %s\n", xbtoa(xent->pflags & 0x4));
        xprintf("has drag:     %s\n", xbtoa(xent->pflags & 0x8));

        const xMat3x3* mat = xent->model ? (const xMat3x3*)(xMat4x3*)xent->model->Mat
                             : xent->frame ? (const xMat3x3*)&xent->frame->mat
                                           : NULL;
        const xVec3* pos = xent->model ? &((xMat4x3*)xent->model->Mat)->pos
                           : xent->frame ? &xent->frame->mat.pos
                                         : NULL;

        if (pos)
        {
            xprintf("pos:   <%8.3f %8.3f %8.3f>\n", pos->x, pos->y, pos->z);

            if (xent->pflags & 0x1)
            {
                xprintf("dpos:  <%8.3f %8.3f %8.3f>\n", pos->x - xent->frame->oldpos.x,
                        pos->y - xent->frame->oldpos.y, pos->z - xent->frame->oldpos.z);
            }
        }

        if (xent->pflags & 0x2)
        {
            const xVec3* vel = &xent->frame->vel;
            const xVec3* ovel = &xent->frame->oldvel;
            F32 dx = vel->x - ovel->x;
            F32 dy = vel->y - ovel->y;
            F32 dz = vel->z - ovel->z;

            xprintf("vel:   <%8.3f %8.3f %8.3f>\n", xent->frame->vel.x, xent->frame->vel.y,
                    xent->frame->vel.z);
            xprintf("dvel:  <%8.3f %8.3f %8.3f>\n", dx, dy, dz);
        }

        if (mat)
        {
            xVec3 scale;
            xMat3x3 mat_norm;
            xVec3 ang;

            xMat3x3GetScale(mat, &scale);
            xprintf("scale: <%8.3f %8.3f %8.3f>\n", scale.x, scale.y, scale.z);

            xMat3x3Normalize(&mat_norm, mat);
            xMat3x3GetEuler(&mat_norm, &ang);
            xprintf("ang:   <%6.1f   %6.1f   %6.1f  >   Y P R\n",
                    180.0f * xDangleClamp(ang.x) / PI, 180.0f * xDangleClamp(ang.y) / PI,
                    180.0f * xDangleClamp(ang.z) / PI);
        }
    }

    if (dbg_mode == 2 || dbg_mode == 3)
    {
        const char* name;

        switch (xent->collType)
        {
        case 0x1:
            name = "trig";
            break;
        case 0x2:
            name = "stat";
            break;
        case 0x4:
            name = "dyn";
            break;
        case 0x8:
            name = "npc";
            break;
        case 0x10:
            name = "pc";
            break;
        case 0x20:
            name = "env";
            break;
        case 0x40:
            name = "walls";
            break;
        default:
            name = "???";
            break;
        }

        xprintf("colltype:     %s\n", name);

        switch (xent->collLev)
        {
        case 0:
            name = "fastest";
            break;
        case 1:
            name = "aabb (box)";
            break;
        case 2:
            name = "sph";
            break;
        case 3:
            name = "obb";
            break;
        case 4:
            name = "bound";
            break;
        case 5:
            name = "model";
            break;
        default:
            name = "???";
            break;
        }

        xprintf("coll-lev:     %s\n", name);

        xEntCollTypeStrAll(str, xent->chkby);
        xprintf("chkby:        %s\n", str);
        xEntCollTypeStrAll(str, xent->penby);
        xprintf("penby:        %s\n", str);

        switch (xent->bound.type)
        {
        case 0:
            name = "none";
            break;
        case 1:
            name = "sph";
            break;
        case 2:
            name = "box";
            break;
        case 3:
            name = "cyl";
            break;
        case 4:
            name = "obb";
            break;
        default:
            name = "???";
            break;
        }

        xprintf("bound: %s", name);

        str[0] = '\0';

        switch (xent->bound.type)
        {
        case 0:
            break;
        case 1:
            sprintf(str, "<%.3f %.3f %.3f> %.3f", xent->bound.sph.center.x, xent->bound.sph.center.y,
                    xent->bound.sph.center.z, xent->bound.sph.r);
            break;
        case 2:
            sprintf(str, "<%.3f %.3f %.3f> <%.3f %.3f %.3f>", xent->bound.box.box.lower.x,
                    xent->bound.box.box.lower.y, xent->bound.box.box.lower.z,
                    xent->bound.box.box.upper.x, xent->bound.box.box.upper.y,
                    xent->bound.box.box.upper.z);
            break;
        case 4:
            sprintf(str, "<%.3f %.3f %.3f> <%.3f %.3f %.3f>", xent->bound.box.box.lower.x,
                    xent->bound.box.box.lower.y, xent->bound.box.box.lower.z,
                    xent->bound.box.box.upper.x, xent->bound.box.box.upper.y,
                    xent->bound.box.box.upper.z);
            break;
        }

        xprintf("   %s\n", str);

        if (xent->collis)
        {
            if (dbg_mode == 2)
            {
                xEntCollTypeStrAll(str, xent->collis->chk);
                xprintf("chk:        %s\n", str);
                xEntCollTypeStrAll(str, xent->collis->pen);
                xprintf("pen:        %s\n", str);
                xprintf("hits:       stat:%d", xent->collis->stat_eidx - xent->collis->stat_sidx);
                xprintf("   dyn:%d", xent->collis->dyn_eidx - xent->collis->dyn_sidx);
                xprintf("   npc:%d", xent->collis->npc_eidx - xent->collis->npc_sidx);
                xprintf("   env:%d\n", xent->collis->env_eidx - xent->collis->env_sidx);
            }
            else
            {
                _xCollis* scoll = xent->collis->colls;

                for (S32 i = 0; i < 6; i++, scoll++)
                {
                    if (scoll->flags & 0x1)
                    {
                        xprintf("%s-hit", scoll_name[i]);
                    }
                    else
                    {
                        xprintf("no %s", scoll_name[i]);
                    }

                    if (scoll->dist < 1e19f)
                    {
                        xVec3 lhdng;

                        xprintf("   %.3f", scoll->dist);
                        xprintf(" <%.3f %.3f %.3f>\n", scoll->hdng.x, scoll->hdng.y, scoll->hdng.z);
                        xprintf("%s-oid:  %u", scoll_name[i], scoll->oid);
                        xMat3x3LMulVec(&lhdng, (const xMat3x3*)&xent->frame->mat, &scoll->hdng);
                        xprintf("   lhdng: <%.3f %.3f %.3f>\n", lhdng.x, lhdng.y, lhdng.z);

                        if (scoll->optr)
                        {
                            xprintf("%s-optr: %d", scoll_name[i], scoll->optr);
                            xprintf("   (%s)\n",
                                    xSceneID2Name(g_xSceneCur, ((xBase*)scoll->optr)->id));
                        }
                        else
                        {
                            xprintf("\n");
                        }
                    }
                    else
                    {
                        xprintf("\n\n\n");
                    }
                }
            }
        }
        else
        {
            xprintf("no checking\n");
        }
    }
}

// Debug pad: tap or hold one button to step forward through the entity list, another to step
// back; a third cycles the display mode and marks the selected entity.
static void xEntDebugIPad(_xEnt* xent)
{
    F32 now = iTimeDiffSec(iTimeGet());

    dbg_tmr += now - dbg_ltm;
    dbg_ltm = now;

    if (gDebugPad->on & 0x20)
    {
        if (gDebugPad->pressed & 0x20)
        {
            dbg_holding_inc = 1;
            dbg_tmr = 0.0f;
        }
        else if (dbg_holding_inc)
        {
            if (dbg_tmr >= 0.25f)
            {
                dbg_holding_inc = 0;
                goto next;
            }
        }
        else if (dbg_tmr >= 0.065f)
        {
            goto next;
        }
    }
    else if (dbg_holding_inc)
    {
        dbg_holding_inc = 0;

    next:
        dbg_tmr = 0.0f;
        dbg_idx++;

        if (dbg_idx >= dbg_num)
        {
            dbg_idx = 0;
        }
    }
    else if (gDebugPad->on & 0x80)
    {
        if (gDebugPad->pressed & 0x80)
        {
            dbg_holding_dec = 1;
            dbg_tmr = 0.0f;
        }
        else if (dbg_holding_dec)
        {
            if (dbg_tmr >= 0.25f)
            {
                dbg_holding_dec = 0;
                goto prev;
            }
        }
        else if (dbg_tmr >= 0.065f)
        {
            goto prev;
        }
    }
    else if (dbg_holding_dec)
    {
        dbg_holding_dec = 0;

    prev:
        dbg_tmr = 0.0f;
        dbg_idx--;

        if (dbg_idx < 0)
        {
            dbg_idx = dbg_num - 1;
        }
    }

    if (gDebugPad->pressed & 0x40000)
    {
        if (dbg_mode == 1)
        {
            dbg_mode = 2;
            dbg_xents[dbg_idx]->flags |= 0x80;
        }
        else if (dbg_mode == 2)
        {
            dbg_mode = 3;
        }
        else
        {
            dbg_mode = 1;
            dbg_xents[dbg_idx]->flags &= (U8)~0x80;
        }
    }
}

static void xEntDebugInit()
{
    xDebugModeRegisterCallback(DM_ENT, xEntDebugCB);

    dbg_num = 0;
    dbg_xents = (_xEnt**)xMemAlloc(gActiveHeap, 319 * sizeof(_xEnt*));
    dbg_idx = 0;
    dbg_mode = 1;
    dbg_holding_inc = 0;
    dbg_holding_dec = 0;
}

static void xModCollInit()
{
    S32 i;

    xDebugModeRegisterCallback(DM_COLLISION, xModCollCB);

    mcrecs = (xModCollRec*)xMemAlloc(gActiveHeap, 101 * sizeof(xModCollRec));
    num_mcrecs = 0;
    mctime = 0;
    mcmode = 2;

    for (i = 0; i < 101; i++)
    {
        smcrecs[i].owner = NULL;
    }
}

static void xModCollAdd(_xEnt* xent)
{
    xModCollRec* mc;
    xModCollRec* end;

    if (num_mcrecs >= 101)
    {
        return;
    }

    if (!xent || !xent->model || !xent->model->Data)
    {
        return;
    }

    end = mcrecs + num_mcrecs;

    for (mc = mcrecs; mc < end; mc++)
    {
        if (mc->owner->model->Data == xent->model->Data)
        {
            mc->nusing++;
            return;
        }
    }

    num_mcrecs++;

    mc->owner = xent;
    mc->ctime = 0;
    mc->ltime = 0;
    mc->frac = 0.0f;
    mc->nusing = 1;
}

void xModCollAddSample(_xEnt* xent, S64 elapsed)
{
    xModCollRec* mc;
    xModCollRec* end;

    if (xent && xent->model && xent->model->Data)
    {
        end = mcrecs + num_mcrecs;

        for (mc = mcrecs; mc < end; mc++)
        {
            if (mc->owner->model->Data == xent->model->Data)
            {
                mc->ctime += elapsed;
                mc->ltime += elapsed;
            }
        }

        mctime += elapsed;
    }
}

// Collision-timer report for the collision debug mode; empty in this build.
static void xModCollCB()
{
}

U32 xEntIsVisible(_xEnt* ent)
{
    if (!(ent->flags & 0x1))
    {
        return 0;
    }

    if (globals.cmgr && !(ent->baseFlags & 0x8))
    {
        return 0;
    }

    return 1;
}
