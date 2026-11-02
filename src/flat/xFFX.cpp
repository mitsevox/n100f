#include "xFFX.h"
#include "xEnt.h"
#include "xMemMgr.h"

#include <math.h>

static U32 psize;
static xFFX* pool;
static xFFX* alist;
static U32 shake_psize;
static xFFXShakeState* shake_pool;
static xFFXShakeState* shake_alist;
static U32 rot_match_psize;
static xFFXRotMatchState* rot_match_pool;
static xFFXRotMatchState* rot_match_alist;

// Each pool is a free list chained from the last element down to the first.
void xFFXPoolInit(U32 num_ffx)
{
    psize = num_ffx;
    pool = (xFFX*)xMemAlloc(gActiveHeap, num_ffx * sizeof(xFFX));
    pool[0].next = NULL;
    for (U32 i = 1; i < psize; i++)
    {
        pool[i].next = &pool[i - 1];
    }
    alist = &pool[psize - 1];
}

xFFX* xFFXAlloc()
{
    xFFX* alist2;
    alist2 = alist;
    if (alist == NULL)
    {
        return NULL;
    }
    alist = alist->next;
    return alist2;
}

// Flag 0x1 marks an effect as running.
void xFFXTurnOn(xFFX* f)
{
    f->flags |= 1;
}

void xFFXTurnOff(xFFX* f)
{
    f->flags &= 0xfffffffe;
}

// Puts the effect at the head of the entity's list and returns its index.
S16 xFFXAddEffect(_xEnt* ent, xFFX* f)
{
    U8 numFFX;
    f->next = ent->ffx;
    f->flags = f->flags | 1;
    ent->ffx = f;
    numFFX = ent->num_ffx;
    ent->num_ffx = numFFX + 1;
    return numFFX;
}

S16 xFFXAddEffect(_xEnt* ent, void (*dof)(_xEnt*, _xScene*, F32, void*), void* fd)
{
    xFFX* f = xFFXAlloc();
    if (f == NULL)
    {
        return -1;
    }

    f->doEffect = dof;
    f->fdata = fd;
    return xFFXAddEffect(ent, f);
}

// Runs the list from its tail, so effects run in the order they were added.
static void xFFXApplyOne(xFFX* ffx, _xEnt* ent, _xScene* sc, F32 dt)
{
    if (ffx->next != NULL)
    {
        xFFXApplyOne(ffx->next, ent, sc, dt);
    }
    if ((ffx->flags & 1) != 0)
    {
        ffx->doEffect(ent, sc, dt, ffx->fdata);
    }
}

void xFFXApply(_xEnt* ent, _xScene* sc, F32 dt)
{
    if (ent->ffx != NULL)
    {
        xFFXApplyOne(ent->ffx, ent, sc, dt);
    }
}

static inline void xFFXFree(xFFX* f)
{
    f->next = alist;
    alist = f;
}

static inline void xFFXRemoveEffectByFData(_xEnt* ent, void* fdata)
{
    xFFX** link = &ent->ffx;
    while (*link != NULL)
    {
        xFFX* f = *link;
        if (fdata == f->fdata)
        {
            xFFX* next = f->next;
            ent->num_ffx--;
            xFFXFree(*link);
            *link = next;
            return;
        }
        link = &f->next;
    }
}

// Shakes the entity along disp with a decaying sine. Once the duration is over it stops at the
// next zero crossing and frees itself.
void xFFXShakeUpdateEnt(_xEnt* ent, _xScene* sc, F32 dt, void* fdata)
{
    xFFXShakeState* ss = (xFFXShakeState*)fdata;
    F32 tnext = ss->tmr + dt;
    F32 mag = (F32)sin(ss->freq * tnext) * (F32)exp(ss->alpha * tnext);

    if (ss->tmr == 0.0f)
    {
        ss->lval = 0.0f;
    }
    else if (ss->tmr >= ss->dur && ss->lval * mag < 0.0f)
    {
        xFFXRemoveEffectByFData(ent, fdata);
        xFFXShakeFree(ss);
        return;
    }

    xVec3 dpos;
    xVec3SMul(&dpos, &ss->disp, mag - ss->lval);
    xVec3AddTo(&ent->frame->mat.pos, &dpos);
    ss->lval = mag;
    ss->tmr = tnext;
}

void xFFXShakePoolInit(U32 num)
{
    shake_psize = num;
    shake_pool = (xFFXShakeState*)xMemAlloc(gActiveHeap, num * sizeof(xFFXShakeState));
    shake_pool->next = NULL;
    for (S32 i = 1; i < shake_psize; i++)
    {
        shake_pool[i].next = &shake_pool[i - 1];
    }
    shake_alist = &shake_pool[shake_psize - 1];
}

xFFXShakeState* xFFXShakeAlloc()
{
    xFFXShakeState* alist2;
    alist2 = shake_alist;
    if (shake_alist == NULL)
    {
        return NULL;
    }
    shake_alist = shake_alist->next;
    return alist2;
}

void xFFXShakeFree(xFFXShakeState* s)
{
    s->next = shake_alist;
    shake_alist = s;
}

void xFFXRotMatchPoolInit(U32 num)
{
    rot_match_psize = num;

    rot_match_pool = (xFFXRotMatchState*)xMemAlloc(gActiveHeap, num * sizeof(xFFXRotMatchState));

    rot_match_pool->next = NULL;
    for (U32 i = 1; i < rot_match_psize; ++i)
    {
        rot_match_pool[i].next = &rot_match_pool[i - 1];
    }

    rot_match_alist = &rot_match_pool[rot_match_psize - 1];
}

xFFXRotMatchState* xFFXRotMatchAlloc()
{
    xFFXRotMatchState* alist2;
    alist2 = rot_match_alist;
    if (rot_match_alist == NULL)
    {
        return NULL;
    }
    rot_match_alist = rot_match_alist->next;
    return alist2;
}
