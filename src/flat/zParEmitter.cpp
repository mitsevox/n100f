#include "zParEmitter.h"

#include "xEnt.h"
#include "xEvent.h"
#include "xModel.h"
#include "xString.h"
#include "zGame.h"
#include "zGlobals.h"
#include "zParSys.h"
#include "zScene.h"
#include <string.h>

_zParEmitter* zParEmitterFind(U32 asset_id)
{
    zScene* s = globals.sceneCur;
    if (!s)
        return NULL;
    _zParEmitter* pe = (_zParEmitter*)s->baseList[37];
    for (S32 i = 0; i < s->baseCount[37]; ++i, ++pe)
        if (pe->tasset->id == asset_id)
            return pe;
    return NULL;
}

_zParEmitter* zParEmitterFind(char* asset_name)
{
    return zParEmitterFind(xStrHash(asset_name));
}

void zParEmitterUpdate(xBase* to, _xScene*, F32 dt)
{
    _zParEmitter* pe = (_zParEmitter*)to;
    if (!zGameIsPaused() && (pe->emit_flags & 1))
        zParEmitterEmit(pe, dt);
}

S32 zParEmitterEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    _zParEmitter* t = (_zParEmitter*)to;
    switch (toEvent)
    {
    case eEventReset:
        xBaseReset(t, t->tasset);
        t->emit_flags = t->tasset->emit_flags;
        t->emit_interval_current = t->tasset->interval;
        break;
    case eEventOn:
        t->emit_flags |= 1;
        break;
    case eEventOff:
        if (t->emit_flags & 1)
            t->emit_flags ^= 1;
        break;
    case 250:
    {
        _zParEmitterCustomSettings fake_settings;
        memset(&fake_settings, 0, sizeof(fake_settings));
        zParEmitterEmitCustom(t, 1.0f / 30.0f, &fake_settings);
        break;
    }
    }
    return 1;
}

void zParEmitterSetup(_zParEmitter* t)
{
    if (t->parSys)
        t->group = ((zParSys*)t->parSys)->group;
}

void zParEmitterInit(xBase* b, zParEmitterAsset* tasset)
{
    _zParEmitter* t = (_zParEmitter*)b;
    xBaseInit(t, tasset);
    t->eventFunc = zParEmitterEventCB;
    t->tasset = tasset;
    t->link = t->linkCount ? (p2LinkAsset*)(tasset + 1) : NULL;
    t->emit_flags = tasset->emit_flags;
    t->emit_interval_current = tasset->interval;
    if (tasset->attachToID && tasset->emit_type == 9)
    {
        _xEnt* ent = (_xEnt*)zSceneFindObject(tasset->attachToID);
        F32* pos = (F32*)tasset->opaque_10;
        iModelTagSetup(&t->tag, ent->model->Data, pos[0], pos[1], pos[2]);
    }
    t->attachTo = tasset->attachToID ? zSceneFindObject(tasset->attachToID) : NULL;
    t->parSys = tasset->parSysID ? zSceneFindObject(tasset->parSysID) : NULL;
    t->emit_volume = tasset->emit_type == 6 ? zSceneFindObject(*(U32*)tasset->opaque_10) : NULL;
}
