#include "zPlatform.h"
#include "xFFX.h"
#include "xAnim.h"
#include "xBound.h"
#include "xEvent.h"
#include "xMemMgr.h"
#include "xMath.h"
#include "xMovePoint.h"
#include "xstransvc.h"
#include "zGlobals.h"
#include "zParEmitter.h"
#include "zScene.h"
#include <string.h>

void xFFXShakeFree(xFFXShakeState* s);
void xFFXShakeUpdateEnt(_xEnt* ent, _xScene* sc, F32 dt, void* fdata);
S16 xFFXAddEffect(_xEnt* ent, xFFX* effect);
extern "C" void fn_80026A1C(_xEntMotion* motion, const xVec3* dpos);
extern "C" void fn_8002E174(xVec3* out, const xMat3x3* mat, const xVec3* v);

static _zParEmitter* sEmitTremble;
static _zParEmitter* sEmitBreakaway;
static void zPlatformTranslate(_xEnt* ent, xVec3* dpos);
void zPlatform_Move(_xEnt* entplat, _xScene* sc, F32 dt, _xEntFrame* frame);
void zPlatform_Init(_zPlatform* plat, p2EntAsset* asset);

// Initialize a platform from its entity asset.
void zPlatform_Init(void* plat, void* asset)
{
    zPlatform_Init((_zPlatform*)plat, (p2EntAsset*)asset);
}

// Install platform callbacks and initialize motion and subtype resources.
void zPlatform_Init(_zPlatform* plat, p2EntAsset* asset)
{
    p2PlatformAsset* passet = (p2PlatformAsset*)(asset + 1);
    p2EntMotionAsset* emasset = (p2EntMotionAsset*)(passet + 1);

    zEntInit(plat, asset, 'PLAT');
    plat->passet = passet;
    plat->subType = passet->type;
    if (plat->linkCount)
    {
        plat->link = (p2LinkAsset*)(emasset + 1);
    }
    else
    {
        plat->link = NULL;
    }
    plat->update = zPlatform_Update;
    plat->move = zPlatform_Move;
    plat->eventFunc = zPlatformEventCB;
    plat->transl = zPlatformTranslate;
    plat->am = NULL;
    plat->bm = NULL;
    plat->state = 0;
    plat->plat_flags = 0;

    if (plat->subType == 9)
    {
        plat->collis = (_xEntCollis*)xMemAlloc(gActiveHeap, sizeof(_xEntCollis));
        plat->collis->chk = 0;
        plat->collis->pen = 0;
        plat->collis->post = NULL;
        plat->am = plat->model;
        U32 size;
        void* buf = xSTFindAsset(passet->ba.bustModelID, &size);
        if (buf)
        {
            xEntLoadModel(plat, (RpAtomic*)buf);
            plat->bm = plat->model;
        }
        else
        {
            plat->bm = NULL;
        }
        plat->model = plat->am;
    }
    else if (plat->subType == 10 && passet->sb.animID[0])
    {
        U32 size;
        void* buf = xSTFindAsset(passet->sb.animID[0], &size);
        if (buf)
        {
            xAnimFile* afile = xAnimFileNew(buf, "", 0, NULL);
            plat->atbl = xAnimTableNew("", NULL);
            xAnimTableNewState(plat->atbl, "Idle", 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
            xAnimTableAddFile(plat->atbl, afile, "Idle");
            xAnimPoolAlloc(&globals.sceneCur->mempool, plat, plat->atbl, plat->model);
        }
    }
    xEntMotionInit(&plat->motion, plat, emasset);
}

// Set up motion and find the emitters used by breakaway platforms.
void zPlatform_Setup(_zPlatform* plat, _xScene* sc)
{
    zEntSetup(plat);
    xEntMotionSetup(&plat->motion, sc);
    sEmitTremble = zParEmitterFind("PAREMIT_PLAT_TREMBLE");
    sEmitBreakaway = zParEmitterFind("PAREMIT_PLAT_BREAKAWAY");
}

// Save the platform's base entity state.
void zPlatform_Save(_zPlatform* ent, xSerial* s)
{
    zEntSave(ent, s);
}

// Load the platform's base entity state.
void zPlatform_Load(_zPlatform* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

// Restore a platform's motion and subtype-specific state.
void zPlatform_Reset(_zPlatform* plat, _xScene* sc)
{
    zEntReset(plat);
    xEntMotionReset(&plat->motion, sc);
    plat->plat_flags = 1;

    if (plat->subType == 0)
    {
        plat->state = 3;
    }
    else if (plat->subType == 9)
    {
        plat->tmr = plat->passet->ba.ba_delay;
        plat->state = 0;
        plat->pflags &= (U8)~6;
        plat->collis->chk = 0;
        plat->frame->vel = g_O3;
        plat->model = plat->am;
        plat->bound.mat = (xMat4x3*)plat->model->Mat;
    }
    else if (plat->subType == 4)
    {
        plat->state = plat->motion.mech.state;
    }
    else if (plat->subType == 10)
    {
        plat->tmr = -1.0f;
        plat->ctr = 0;
    }

    if (plat->motion.type == 3)
    {
        plat->src = plat->motion.mp.src;
    }

    if (!(plat->passet->flags & 4))
    {
        plat->chkby = 0;
    }
    else
    {
        plat->chkby = 0x18;
    }
    plat->moving = 0;
}

// Update platform-specific motion, breakaway state, and movement events.
void zPlatform_Update(_xEnt* entplat, _xScene* sc, F32 dt)
{
    _zPlatform* plat = (_zPlatform*)entplat;
    xEntUpdate(plat, sc, dt);

    if (plat->subType == 0)
    {
        plat->state = plat->motion.er.state;
    }
    else if (plat->subType == 9)
    {
        if (plat->state == 1)
        {
            plat->tmr -= dt;
            if (plat->tmr <= 0.0f)
            {
                _zParEmitterCustomSettings info;
                info.custom_flags = 0x20;
                info.pos = *xBoundCenter(&plat->bound);
                info.pos.y += 0.5f;
                for (S32 i = 0; i < 25; i++)
                    zParEmitterEmitCustom(sEmitBreakaway, 1.0f / 30.0f, &info);
                plat->state = 2;
                plat->pflags |= 6;
                plat->tmr = plat->passet->ba.reset_delay;
                if (plat->bm)
                {
                    plat->model = plat->bm;
                    memcpy(plat->bm->Mat, plat->am->Mat, sizeof(xMat4x3));
                    plat->bound.mat = (xMat4x3*)plat->model->Mat;
                }
                zEntEvent(plat, 0x22);
            }
        }
        else if (plat->state == 2)
        {
            plat->tmr -= dt;
            if (plat->collis->colls[0].flags & 1)
            {
                plat->state = 3;
                plat->pflags &= (U8)~6;
                plat->collis->chk = 0;
                plat->frame->vel = g_O3;
            }
            else if (plat->tmr <= 0.0f)
            {
                zPlatform_Reset(plat, sc);
            }
            else if (plat->passet->ba.reset_delay - plat->tmr >= 25.0f)
            {
                plat->state = 4;
                plat->pflags &= (U8)~6;
                plat->collis->chk = 0;
                plat->frame->vel = g_O3;
            }
        }
        else if (plat->state == 3)
        {
            plat->tmr -= dt;
            if (plat->tmr <= 0.0f)
                zPlatform_Reset(plat, sc);
        }
    }
    else if (plat->subType == 4)
    {
        U16 state = plat->motion.mech.state;
        if (state != plat->state)
        {
            if (plat->state == 2)
                zEntEvent(plat, 0x39);
            else if (state == 6 || state == 7 ||
                     (!(plat->motion.asset->mech.flags & 2) && (state == 0 || state == 1)))
                zEntEvent(plat, 0x1F);
            plat->state = state;
        }
    }
    else if (plat->subType == 10)
    {
        plat->tmr -= dt;
    }
    else if (plat->subType == 6)
    {
        _xEntDrive* drv = &globals.player.drv;
        if (plat == drv->odriver || plat == drv->driver)
        {
            F32 s = plat == drv->driver ? drv->s : drv->os;
            _xEnt* p = drv->driven;
            xMat4x3* mat = (xMat4x3*)plat->model->Mat;
            xVec3SMul(&p->frame->dpos, &mat->right, s * plat->passet->cb.speed * dt);
            xVec3* pos = (xVec3*)&p->model->Mat->pos;
            xVec3Add(pos, pos, &p->frame->dpos);
        }
    }
    else if (plat->subType == 11)
    {
        _xEntDrive* drv = &globals.player.drv;
        if (plat != drv->odriver && plat != drv->driver)
        {
            F32 ctilt = plat->frame->rot.axis.z;
            F32 itilt = plat->asset->ang.z;
            F32 dtilt = xabs((PI * plat->passet->teet.invmass / 180.0f) * dt);
            if (ctilt != itilt)
            {
                if (ctilt < itilt)
                    dtilt = MIN(dtilt, itilt - ctilt);
                else if (ctilt > itilt)
                    dtilt = MAX(-dtilt, itilt - ctilt);
                plat->frame->rot.axis.z += dtilt;
                xMat4x3* mat = (xMat4x3*)plat->model->Mat;
                xMat3x3Euler((xMat3x3*)mat, plat->frame->rot.axis.x,
                             plat->frame->rot.axis.y, plat->frame->rot.axis.z);
                xVec3SMul(&mat->right, &mat->right, plat->asset->scale.x);
                xVec3SMul(&mat->up, &mat->up, plat->asset->scale.y);
                xVec3SMul(&mat->at, &mat->at, plat->asset->scale.z);
            }
        }
        else
        {
            F32 s = plat == drv->driver ? drv->s : drv->os;
            _xEnt* p = drv->driven;
            xMat4x3* mat = (xMat4x3*)plat->model->Mat;
            xVec3 lpos;
            xVec3Sub(&lpos, &p->frame->mat.pos, &mat->pos);
            fn_8002E174(&lpos, (xMat3x3*)mat, &lpos);
            F32 cangle = plat->frame->rot.axis.z;
            F32 dangle = -s * (PI * plat->passet->teet.invmass / 180.0f) * lpos.x * dt;
            F32 mangle = xabs(plat->passet->teet.maxtilt);
            F32 tangle = xabs(dangle + cangle) - mangle;
            if (tangle > 0.0f)
            {
                if (dangle < 0.0f)
                    dangle += tangle;
                else if (dangle > 0.0f)
                    dangle -= tangle;
            }
            plat->frame->rot.axis.z += dangle;
            xMat3x3Euler((xMat3x3*)mat, plat->frame->rot.axis.x,
                         plat->frame->rot.axis.y, plat->frame->rot.axis.z);
            xVec3SMul(&mat->right, &mat->right, plat->asset->scale.x);
            xVec3SMul(&mat->up, &mat->up, plat->asset->scale.y);
            xVec3SMul(&mat->at, &mat->at, plat->asset->scale.z);
        }
    }

    if (plat->motion.type == 3)
    {
        _xMovePoint* src = plat->motion.mp.src;
        if (src != plat->src)
        {
            zEntEvent(plat, src, 0x1F);
            plat->src = src;
        }
    }

    xVec3* opos = &plat->frame->oldpos;
    xVec3* pos = &plat->frame->mat.pos;
    xVec3* orot = &plat->frame->oldrot.axis;
    xVec3* rot = &plat->frame->rot.axis;
    S32 moving;
    if (plat->subType == 11)
        moving = orot->x != rot->x || orot->y != rot->y || orot->z != rot->z;
    else
        moving = opos->x != pos->x || opos->y != pos->y || opos->z != pos->z ||
                 orot->x != rot->x || orot->y != rot->y || orot->z != rot->z;
    if (moving != plat->moving)
        zEntEvent(plat, moving ? eEventStartMoving : eEventStopMoving);
    plat->moving = moving;
}

// Move the platform according to its configured motion.
void zPlatform_Move(_xEnt* entplat, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    _zPlatform* plat = (_zPlatform*)entplat;
    xEntMotionMove(&plat->motion, sc, dt, frame);
}

// Add a vertical shake to the platform.
void zPlatform_Shake(_zPlatform* plat, F32 unused, F32 ampl, F32 freq)
{
    xFFXShakeState* ss;
    xFFX* sfkt;

    ss = xFFXShakeAlloc();
    if (ss != NULL)
    {
        sfkt = xFFXAlloc();
        if (sfkt == NULL)
        {
            xFFXShakeFree(ss);
            ss = NULL;
        }
        else
        {
            ss->disp.x = 0.0f;
            ss->disp.y = -ampl;
            ss->disp.z = 0.0f;
            ss->dur = 1.0f;
            ss->alpha = -7.0f / ss->dur;
            ss->freq = freq;
            ss->tmr = 0.0f;
            sfkt->doEffect = xFFXShakeUpdateEnt;
            sfkt->fdata = ss;
            xFFXAddEffect(plat, sfkt);
        }
    }
}

static void zPlatform_Tremble(_zPlatform* plat, F32 ampl, F32 freq, F32 dur)
{
    xFFXShakeState* ss = xFFXShakeAlloc();
    if (ss != NULL)
    {
        xFFX* sfkt = xFFXAlloc();
        if (sfkt == NULL)
        {
            xFFXShakeFree(ss);
            return;
        }

        xVec3SMul(&ss->disp, &globals.camera.hi_cam.mat.right, ampl);
        ss->dur = dur;
        ss->alpha = 1.0f / dur;
        ss->freq = freq;
        ss->tmr = 0.0f;
        sfkt->doEffect = xFFXShakeUpdateEnt;
        sfkt->fdata = ss;
        xFFXAddEffect(plat, sfkt);
    }

    _zParEmitterCustomSettings info;
    info.custom_flags = 0x20;
    info.pos = *xBoundCenter(&plat->bound);
    for (S32 i = 0; i < 25; i++)
    {
        zParEmitterEmitCustom(sEmitTremble, 1.0f / 30.0f, &info);
    }
}

// Begin the breakaway platform's tremble when it is mounted.
void zPlatform_Mount(_zPlatform* plat)
{
    if (plat->subType == 9)
    {
        if (plat->state == 0)
        {
            plat->state = 1;
        }
        zPlatform_Tremble(plat, 0.06f, 25.132742f, 1.0f + plat->passet->ba.ba_delay);
    }
}

// No dismount action is needed for this platform.
void zPlatform_Dismount(_zPlatform* ent)
{
}

// Clear the entity's visible and collision flags and set an asset flag.
void zPlatform_MakeDiggable(_zPlatform* plat)
{
    plat->flags &= (U8)~1;
    plat->chkby = 0;
    plat->passet->flags |= 2;
}

// Translate the entity and its motion together.
static void zPlatformTranslate(_xEnt* ent, xVec3* dpos)
{
    _zPlatform* plat = (_zPlatform*)ent;
    xEntDefaultTranslate(ent, dpos);
    fn_80026A1C(&plat->motion, dpos);
}

// Apply scene events to platform motion, visibility, and animation.
S32 zPlatformEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    _zPlatform* plat = (_zPlatform*)to;
    switch (toEvent)
    {
    case eEventOn:
        if (!(plat->plat_flags & 1))
            plat->plat_flags |= 1;
        break;
    case eEventOff:
        if (plat->plat_flags & 1)
            plat->plat_flags &= (U16)~1;
        break;
    case eEventToggle:
        if (plat->plat_flags & 1)
            plat->plat_flags &= (U16)~1;
        else
            plat->plat_flags |= 1;
        break;
    case eEventRun:
        if (plat->subType == 4 &&
            (plat->motion.mech.state == 6 || plat->motion.mech.state == 7))
        {
            xEntMotionReset(&plat->motion, globals.sceneCur);
        }
        xEntMotionRun(&plat->motion);
        break;
    case eEventStop:
        xEntMotionStop(&plat->motion);
        break;
    case 0x12A:
        if (plat->subType == 4)
            xEntMechForward(&plat->motion);
        break;
    case 0x12B:
        if (plat->subType == 4)
            xEntMechReverse(&plat->motion);
        break;
    case eEventReset:
        zPlatform_Reset(plat, globals.sceneCur);
        break;
    case eEventVisible:
        plat->flags |= 1;
        break;
    case eEventInvisible:
        plat->flags &= (U8)~1;
        break;
    case eEventCollision_Visible_On:
        plat->flags |= 1;
    case eEventCollisionOn:
        plat->chkby = 0x18;
        break;
    case eEventCollision_Visible_Off:
        plat->flags &= (U8)~1;
    case eEventCollisionOff:
        plat->chkby = 0;
        break;
    case eEventAnimPlay:
    case eEventAnimPlayLoop:
    case eEventAnimStop:
    case eEventAnimPause:
    case eEventAnimResume:
    case eEventAnimTogglePause:
    case eEventAnimPlayRandom:
    case eEventAnimPlayMaybe:
        zEntAnimEvent(plat, toEvent, toParam);
        break;
    case eEventTranslucentOn:
        plat->moreFlags |= 1;
        break;
    case eEventTranslucentOff:
        plat->moreFlags &= (U8)~1;
        plat->flags &= (U8)~4;
        plat->fade_tmr = 0.0f;
        break;
    case eEventTranslucentToggle:
        plat->moreFlags ^= 1;
        if (!(plat->moreFlags & 1))
        {
            plat->flags &= (U8)~4;
            plat->fade_tmr = 0.0f;
        }
        break;
    case 0xCB:
        if (plat->subType == 3)
            plat->motion.mp.speed = MAX(0.0f, toParam[0]);
        break;
    case 0xCC:
        if (plat->subType == 3)
        {
            F32 speed = plat->motion.mp.speed + toParam[0];
            plat->motion.mp.speed = MAX(0.0f, speed);
        }
        break;
    }
    return 1;
}
