#include "zEntDestructObj.h"
#include "zGlobals.h"
#include "zParEmitter.h"
#include "zRumble.h"
#include "xBound.h"
#include "xEvent.h"
#include "xserializer.h"

_zParEmitter* sEmitDust;
_zParEmitter* sEmitXplo;
_zParEmitter* sEmitWeb;

void zEntDestructObj_FindFX()
{
    sEmitDust = zParEmitterFind("PAREMIT_DOBJ_DUST");
    sEmitXplo = zParEmitterFind("PAREMIT_DOBJ_XPLO");
    sEmitWeb = zParEmitterFind("PAREMIT_DOBJ_WEB");
}

void zEntDestructObj_Init(void* ent, void* asset)
{
    zEntDestructObj_Init((_zEntDestructObj*)ent, (p2EntAsset*)asset);
}

void zEntDestructObj_Init(_zEntDestructObj* ent, p2EntAsset* asset)
{
    zEntInit(ent, asset, 'DSTR');

    zEntDestructObjAsset* dasset = (zEntDestructObjAsset*)(asset + 1);

    ent->dasset = dasset;
    ent->healthCnt = dasset->health;

    ent->penby |= 0x10;
    ent->state = 0;
    if (ent->dasset->collType & 0x2)
    {
        ent->chkby |= 0x18;
    }
    else
    {
        ent->chkby = 0;
    }

    ent->move = (void (*)(_xEnt*, _xScene*, F32, _xEntFrame*))zEntDextructObj_Move;
    ent->eventFunc = zEntDestructObjEventCB;
    ent->update = (void (*)(_xEnt*, _xScene*, F32))zEntDestructObj_Update;

    if (ent->linkCount)
    {
        ent->link = (p2LinkAsset*)((zEntDestructObjAsset*)(ent->asset + 1) + 1);
    }
    else
    {
        ent->link = NULL;
    }

    ent->eventFunc = zEntDestructObjEventCB;

    xEntReset(ent);
}

void zEntDextructObj_Move(_zEntDestructObj*, _xScene*, F32, _xEntFrame*)
{
}

void zEntDestructObj_Update(_zEntDestructObj* ent, _xScene* sc, F32 dt)
{
    _zParEmitterCustomSettings info;

    xEntUpdate(ent, sc, dt);

    if (ent->healthCnt > 1)
    {
        ent->state = 0;
    }
    else
    {
        ent->state = 1;
    }

    if (ent->fx_emitter && ent->fx_timer > 0.0f)
    {
        ent->fx_timer -= dt;
        info.custom_flags = 0x20;
        info.pos = *xBoundCenter(&ent->bound);
        zParEmitterEmitCustom(ent->fx_emitter, dt, &info);
    }
}

U32 zEntDestructObj_ButtBounce(_zEntDestructObj* ent)
{
    if (ent->dasset->dflags & 0x2)
    {
        return 3;
    }
    return 0;
}

U32 zEntDestructObj_Headbutt(_zEntDestructObj* ent)
{
    if (ent->dasset->dflags & 0x1)
    {
        return (ent->dasset->dflags & 0x8) ? 5 : 4;
    }
    return 0;
}

void zEntDestructObj_Save(_zEntDestructObj* ent, xSerial* s)
{
    zEntSave(ent, s);
    s->Write(ent->state);
}

void zEntDestructObj_Load(_zEntDestructObj* ent, xSerial* s)
{
    zEntLoad(ent, s);
    s->Read(&ent->state);
}

U32 zEntDestructObj_CheckState(_zEntDestructObj* ent)
{
    return ent->state;
}

void zEntDestructObj_DestroyFX(_zEntDestructObj* o)
{
    _tagSDRumbleType rt = SDR_Total;

    o->fx_timer = 0.33f;

    switch (o->dasset->fxType)
    {
    case 0:
        o->fx_emitter = NULL;
        break;
    case 1:
        rt = SDR_DustDestroyedObj;
        o->fx_emitter = sEmitDust;
        break;
    case 2:
        rt = SDR_XploDestroyedObj;
        o->fx_emitter = sEmitXplo;
        break;
    case 3:
        rt = SDR_WebDestroyed;
        o->fx_emitter = sEmitWeb;
        break;
    }

    if (rt != SDR_Total)
    {
        xVec3 d;
        xVec3Sub(&d, xEntGetPos(o), xEntGetPos(&globals.player.ent));
        if (xVec3Dot(&d, &d) <= 25.0f)
        {
            zRumbleStart(0, rt);
        }
    }
}

S32 zEntDestructObjEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*)
{
    _zEntDestructObj* s = (_zEntDestructObj*)to;

    switch (toEvent)
    {
    case eEventVisible:
        xEntShow(s);
        break;
    case eEventInvisible:
        xEntHide(s);
        break;
    case eEventCollisionOn:
        s->chkby |= 0x18;
        break;
    case eEventCollisionOff:
        s->chkby &= (U8)~0x18;
        break;
    case eEventCollision_Visible_On:
        s->chkby |= 0x18;
        xEntShow(s);
        break;
    case eEventCollision_Visible_Off:
        s->chkby &= (U8)~0x18;
        xEntHide(s);
        break;
    case eEventDestroy:
        s->chkby &= (U8)~0x18;
        xEntHide(s);
        zEntDestructObj_DestroyFX(s);
        break;
    case eEventReset:
        xEntReset(s);
        s->healthCnt = s->dasset->health;
        s->pflags = 0;
        s->penby |= 0x10;
        xEntShow(s);
        if (s->dasset->collType & 0x2)
        {
            s->chkby |= 0x18;
        }
        else
        {
            s->chkby = 0;
        }
        s->fx_timer = 0.0f;
        s->fx_emitter = NULL;
        break;
    case eEventAnimPlay:
    case eEventAnimPlayLoop:
    case eEventAnimStop:
    case eEventAnimPause:
    case eEventAnimResume:
    case eEventAnimTogglePause:
    case eEventAnimPlayRandom:
    case eEventAnimPlayMaybe:
        zEntAnimEvent(s, toEvent, toParam);
        break;
    case eEventTranslucentOn:
        s->moreFlags |= 0x1;
        break;
    case eEventTranslucentOff:
        s->moreFlags &= (U8)~0x1;
        s->flags &= (U8)~0x4;
        s->fade_tmr = 0.0f;
        break;
    case eEventTranslucentToggle:
        s->moreFlags ^= 0x1;
        if (!(s->moreFlags & 0x1))
        {
            s->flags &= (U8)~0x4;
            s->fade_tmr = 0.0f;
        }
        break;
    case eEventHit:
        s->healthCnt--;
        if (s->healthCnt == 0)
        {
            zEntEvent(s, s, eEventDestroy);
        }
        break;
    }

    return 1;
}
