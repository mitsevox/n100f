#include "zMovePoint.h"
#include "zGlobals.h"
#include "zScene.h"
#include "iCollide.h"
#include "xMathInlines.h"
#include "xMemMgr.h"

static _zMovePoint* g_mvpt_list;
static S32 g_mvpt_cnt;

static S32 MVPT_dflt_chklos(void*, xVec3* mypos, const _zMovePoint* mvpt);
_zMovePoint* zMovePointFindNearest(S32 flg_opts, xVec3* yourpos, F32 okdist, F32 nohope, void* yourdata,
                                   S32 (*losfunc)(void*, xVec3*, const _zMovePoint*), S32* idx_resume,
                                   _zMovePoint** prev_best);
_zMovePoint* zMovePointFindNearest(S32 flg_opts, xVec3* yourpos, void* yourdata,
                                   S32 (*losfunc)(void*, xVec3*, const _zMovePoint*));
S32 zMovePointIsOn(const _zMovePoint* m);
F32 zMovePointGetDelay(const _zMovePoint* m);
xVec3* zMovePointGetPos(const _zMovePoint* m);
F32 zMovePointGetNext(const _zMovePoint* current, const _zMovePoint* prev, _zMovePoint** next, xVec3* hdng);
S32 zMovePointEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);
void zMovePointLoad(_zMovePoint* ent, xSerial* s);
void zMovePointSave(_zMovePoint* ent, xSerial* s);
_zMovePoint* zMovePoint_From_xAssetID(U32 aid);
void zMovePointSetup(_zMovePoint* mvpt, zScene* scn);
_zMovePoint* zMovePoint_GetInst(S32 n);
void zMovePointInit(_zMovePoint* m, p2MovePointAsset* asset);
_zMovePoint* zMovePoint_GetMemPool(S32 cnt);

_zMovePoint* zMovePoint_GetMemPool(S32 cnt)
{
    g_mvpt_list = cnt ? (_zMovePoint*)xMemAlloc(gActiveHeap, cnt * sizeof(_zMovePoint)) : NULL;
    g_mvpt_cnt = cnt;
    return g_mvpt_list;
}

void zMovePointInit(_zMovePoint* m, p2MovePointAsset* asset)
{
    xMovePointInit(m, asset);
    m->eventFunc = zMovePointEventCB;

    if (m->linkCount)
    {
        m->link = (p2LinkAsset*)((U32*)asset + sizeof(p2MovePointAsset) / 4 + asset->numPoints);
    }
    else
    {
        m->link = NULL;
    }
}

_zMovePoint* zMovePoint_GetInst(S32 n)
{
    return &g_mvpt_list[n];
}

void zMovePointSetup(_zMovePoint* mvpt, zScene* scn)
{
    xMovePointSetup(mvpt, scn);
}

_zMovePoint* zMovePoint_From_xAssetID(U32 aid)
{
    _zMovePoint* found = NULL;

    for (S32 i = 0; i < g_mvpt_cnt; i++)
    {
        _zMovePoint* da_mvpt = &g_mvpt_list[i];
        if (da_mvpt->asset->id == aid)
        {
            found = da_mvpt;
            break;
        }
    }

    return found;
}

void zMovePointSave(_zMovePoint* ent, xSerial* s)
{
    xMovePointSave(ent, s);
}

void zMovePointLoad(_zMovePoint* ent, xSerial* s)
{
    xMovePointLoad(ent, s);
}

S32 zMovePointEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zMovePoint* m = (_zMovePoint*)to;

    switch (toEvent)
    {
    case 38:
        m->on = 1;
        break;
    case 39:
        m->on = 0;
        break;
    case 31:
        break;
    case 10:
        xMovePointReset(m);
        break;
    }

    return 1;
}

F32 zMovePointGetNext(const _zMovePoint* current, const _zMovePoint* prev, _zMovePoint** next, xVec3* hdng)
{
    return xMovePointGetNext(current, prev, (xMovePoint**)next, hdng);
}

xVec3* zMovePointGetPos(const _zMovePoint* m)
{
    return xMovePointGetPos(m);
}

F32 zMovePointGetDelay(const _zMovePoint* m)
{
    return m->delay;
}

S32 zMovePointIsOn(const _zMovePoint* m)
{
    return m ? m->on : 0;
}

_zMovePoint* zMovePointFindNearest(S32 flg_opts, xVec3* yourpos, void* yourdata,
                                   S32 (*losfunc)(void*, xVec3*, const _zMovePoint*))
{
    return zMovePointFindNearest(flg_opts, yourpos, -1.0f, 999.0f, yourdata, losfunc, NULL, NULL);
}

// flg_opts: 1 accepts points that are on, 2 points that are off, 4 measures in the ground plane
// (within one unit of height), 8 only points with property 1, 16 tests line of sight, 32 searches
// in slices of 50 points, resuming from idx_resume and prev_best.
_zMovePoint* zMovePointFindNearest(S32 flg_opts, xVec3* yourpos, F32 okdist, F32 nohope, void* yourdata,
                                   S32 (*losfunc)(void*, xVec3*, const _zMovePoint*), S32* idx_resume,
                                   _zMovePoint** prev_best)
{
    xVec3 delta = {};
    F32 ds2_far;
    F32 ds2_best = 999999.0f;
    F32 ds2_nohope = nohope * nohope;
    F32 ds2_ok = okdist * okdist;
    _zMovePoint* found = NULL;
    S32 suspend = 0;
    S32 eidx;
    S32 sidx;
    S32 i;
    _zMovePoint* cur_best = NULL;
    _zMovePoint* da_mvpt;

    if (!idx_resume || !prev_best)
    {
        flg_opts &= ~0x20;
    }

    if (!losfunc && (flg_opts & 0x10))
    {
        losfunc = MVPT_dflt_chklos;
    }
    else if (!(flg_opts & 0x10))
    {
        losfunc = NULL;
    }

    if (flg_opts & 0x20)
    {
        sidx = *idx_resume;
        sidx = (sidx < 0) ? 0 : (sidx < g_mvpt_cnt ? sidx : g_mvpt_cnt);
        eidx = (sidx + 50 < g_mvpt_cnt) ? sidx + 50 : g_mvpt_cnt;

        if (prev_best && (cur_best = *prev_best) != NULL)
        {
            xVec3Sub(&delta, cur_best->pos, yourpos);
            if (flg_opts & 4)
            {
                ds2_best = delta.x * delta.x + delta.z * delta.z;
            }
            else
            {
                ds2_best = xVec3Length2(&delta);
            }
        }
    }
    else
    {
        sidx = 0;
        eidx = g_mvpt_cnt;
    }

    for (i = sidx; i < eidx; i++)
    {
        da_mvpt = &g_mvpt_list[i];

        if (!da_mvpt->pos)
        {
            continue;
        }
        if (!(flg_opts & 1) && da_mvpt->on)
        {
            continue;
        }
        if (!(flg_opts & 2) && !da_mvpt->on)
        {
            continue;
        }
        if ((flg_opts & 8) && !(da_mvpt->flg_props & 1))
        {
            continue;
        }

        xVec3Sub(&delta, da_mvpt->pos, yourpos);
        if (flg_opts & 4)
        {
            if (delta.y > 1.0f || delta.y < -1.0f)
            {
                continue;
            }
            ds2_far = delta.x * delta.x + delta.z * delta.z;
        }
        else
        {
            ds2_far = xVec3Length2(&delta);
        }

        if (nohope > 0.0f && ds2_far > ds2_nohope)
        {
            continue;
        }
        if (cur_best && ds2_far > ds2_best)
        {
            continue;
        }
        if (flg_opts & 0x10)
        {
            suspend = 1;
            if (!losfunc(yourdata, yourpos, da_mvpt))
            {
                continue;
            }
        }

        ds2_best = ds2_far;
        cur_best = da_mvpt;

        if (okdist > 0.0f && ds2_far < ds2_ok)
        {
            found = da_mvpt;
            break;
        }
        if ((flg_opts & 0x20) && suspend)
        {
            break;
        }
    }

    if (found)
    {
        if (idx_resume)
        {
            *idx_resume = -2;
        }
        if (prev_best)
        {
            *prev_best = found;
        }
    }
    else if (i == g_mvpt_cnt)
    {
        found = cur_best;
        if (idx_resume)
        {
            *idx_resume = -2;
        }
        if (prev_best)
        {
            *prev_best = cur_best;
        }
    }
    else if ((flg_opts & 0x20) && i == eidx)
    {
        *prev_best = cur_best;
        *idx_resume = i;
    }
    else if ((flg_opts & 0x20) && suspend)
    {
        *prev_best = cur_best;
        *idx_resume = i + 1;
    }

    return found;
}

// The default line-of-sight test: a ray from the move point towards the caller's position must
// reach it without hitting the environment.
static S32 MVPT_dflt_chklos(void*, xVec3* mypos, const _zMovePoint* mvpt)
{
    _xRay3 ray_los = {};
    xVec3 vec_tgt = {};
    _xCollis colrec = {};

    ray_los.origin = *mvpt->pos;
    xVec3Sub(&vec_tgt, mypos, &ray_los.origin);

    F32 dist = xVec3Length2(&vec_tgt);
    if (dist > 0.0f)
    {
        dist = xsqrt(dist);
    }

    xVec3Normalize(&ray_los.dir, &vec_tgt);
    ray_los.flags = 0xC00;
    ray_los.max_t = dist;
    ray_los.min_t = 0.0f;

    iRayHitsEnv(&ray_los, globals.sceneCur->env, &colrec);

    if (colrec.optr)
    {
        return 0;
    }

    return 1;
}
