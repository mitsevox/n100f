#include "zParEmitter.h"

#include "xEnt.h"
#include "xEvent.h"
#include "xPar.h"
#include "iMath.h"
#include "xModel.h"
#include "xString.h"
#include "zGame.h"
#include "zGlobals.h"
#include "zParSys.h"
#include "zScene.h"
#include <string.h>
#include <math.h>

// Search the current scene's emitter assets by ID.
_zParEmitter* zParEmitterFind(U32 asset_id)
{
    zScene* s = globals.sceneCur;
    if (!s)
        return NULL;
    _zParEmitter* pe = (_zParEmitter*)s->baseList[38];
    for (S32 i = 0; i < (S32)s->baseCount[38]; ++i, ++pe)
        if (pe->tasset->id == asset_id)
            return pe;
    return NULL;
}

// Names use the same hash as asset IDs.
_zParEmitter* zParEmitterFind(char* asset_name)
{
    return zParEmitterFind(xStrHash(asset_name));
}

// Emit only while the game is running and this emitter is enabled.
void zParEmitterUpdate(xBase* to, _xScene*, F32 dt)
{
    _zParEmitter* pe = (_zParEmitter*)to;
    if (!zGameIsPaused() && (pe->emit_flags & 1))
        zParEmitterEmit(pe, dt);
}

static _zParEmitterCustomSettings sSaveEmmiterSettings;

xPar* zParEmitterEmitCustom(_zParEmitter* p, F32 dt, _zParEmitterCustomSettings* info)
{
    U32 flags = info->custom_flags;
    zParEmitterAsset* pe = p->tasset;
    _zParEmitterCustomSettings* s = &sSaveEmmiterSettings;

    if (flags & 0x4000)
    {
        s->count = pe->count;
        s->count_variation = pe->count_variation;
        s->interval = pe->interval;
        s->attachToID = pe->attachToID;
        s->pos = pe->pos;
        s->vel = pe->vel;
        s->vel_angle_variation = pe->vel_angle_variation;
        for (S32 i = 0; i < 4; ++i)
        {
            s->color_birth[i] = pe->color_birth[i];
            s->color_death[i] = pe->color_death[i];
        }
        s->size_birth = pe->size_birth;
        s->size_birth_variation = pe->size_birth_variation;
        s->size_death = pe->size_death;
        s->life = pe->life;
        s->life_variation = pe->life_variation;
        s->emit_interval_current = p->emit_interval_current;
        s->rot[0] = p->rot[0];
        s->rot[1] = p->rot[1];
        s->rot[2] = p->rot[2];
        s->xzcircle_radius = pe->e_circle.radius;
    }
    if (flags & 0x4) pe->count = info->count;
    if (flags & 0x8) pe->count_variation = info->count_variation;
    if (flags & 0x10) pe->interval = info->interval;
    if (flags & 0x800) p->emit_interval_current = 0.0f;
    if (flags & 0x8000)
    {
        pe->attachToID = info->attachToID;
        p->attachTo = pe->attachToID ? zSceneFindObject(pe->attachToID) : NULL;
    }
    if (flags & 0x20) pe->pos = info->pos;
    if (flags & 0x10000)
    {
        p->rot[0] = info->rot[0];
        p->rot[1] = info->rot[1];
        p->rot[2] = info->rot[2];
    }
    if (flags & 0x20000) pe->e_circle.radius = info->xzcircle_radius;
    if (flags & 0x40) pe->vel = info->vel;
    if (flags & 0x2) pe->vel_angle_variation = info->vel_angle_variation;
    if (flags & 0x80)
        for (S32 i = 0; i < 4; ++i) pe->color_birth[i] = info->color_birth[i];
    if (flags & 0x100)
        for (S32 i = 0; i < 4; ++i) pe->color_death[i] = info->color_death[i];
    if (flags & 0x200) pe->size_birth = info->size_birth;
    if (flags & 0x2000) pe->size_birth_variation = info->size_birth_variation;
    if (flags & 0x400) pe->size_death = info->size_death;
    if (flags & 0x1) pe->life = info->life;
    if (flags & 0x1000) pe->life_variation = info->life_variation;

    xPar* emitted_par = zParEmitterEmit(p, dt);
    if (flags & 0x4000)
    {
        pe->count = s->count;
        pe->count_variation = s->count_variation;
        pe->interval = s->interval;
        pe->attachToID = s->attachToID;
        p->attachTo = pe->attachToID ? zSceneFindObject(pe->attachToID) : NULL;
        pe->pos = s->pos;
        pe->vel = s->vel;
        pe->vel_angle_variation = s->vel_angle_variation;
        for (S32 i = 0; i < 4; ++i)
        {
            pe->color_birth[i] = s->color_birth[i];
            pe->color_death[i] = s->color_death[i];
        }
        pe->size_birth = s->size_birth;
        pe->size_birth_variation = s->size_birth_variation;
        pe->size_death = s->size_death;
        pe->life = s->life;
        pe->life_variation = s->life_variation;
        p->emit_interval_current = s->emit_interval_current;
        p->rot[0] = s->rot[0];
        p->rot[1] = s->rot[1];
        p->rot[2] = s->rot[2];
        pe->e_circle.radius = s->xzcircle_radius;
    }
    return emitted_par;
}

// Place particles on a sphere and point their velocity along the selected direction.
static void zParEmitterEmitSphereEdge(xPar* p, zParEmitterAsset* a, F32 dt, S32 subtype)
{
    xVec3 dirvec = { 0.0f, 0.0f, 1.0f };
    xVec3 offset = { 0.0f, 0.0f, 0.0f };
    xMat3x3 mat_rot = {};

    p->m_vel = a->vel;
    xVec3SMul(&p->m_vel, &p->m_vel, dt);
    switch (subtype)
    {
    case 7:
    {
        F32 roll = 6.2831855f * iurand();
        F32 pitch = 6.2831855f * iurand();
        F32 yaw = 6.2831855f * iurand();
        xMat3x3Euler(&mat_rot, yaw, pitch, roll);
        break;
    }
    case 10:
    {
        F32 pitch = 3.1415927f * iurand();
        F32 yaw = 6.2831855f * iurand();
        xMat3x3Euler(&mat_rot, yaw, pitch, 0.0f);
        break;
    }
    case 11:
    {
        F32 roll = 6.2831855f * iurand();
        F32 pitch = 3.1415927f * iurand() + 3.1415927f;
        F32 yaw = 6.2831855f * iurand();
        xMat3x3Euler(&mat_rot, yaw, pitch, roll);
        break;
    }
    }
    xMat3x3LMulVec(&dirvec, &mat_rot, &dirvec);
    F32 magnitude = sqrtf(xVec3Length2(&p->m_vel));
    xVec3SMul(&p->m_vel, &dirvec, magnitude);
    xVec3SMul(&offset, &dirvec, a->e_sphere.radius);
    xVec3Add(&p->m_pos, &p->m_pos, &offset);
    if (a->vel_angle_variation != 0.0f)
    {
        xMat3x3 variation = {};
        F32 ang[3];
        F32 cone = a->vel_angle_variation;
        ang[0] = cone * (iurand() - 0.5f);
        ang[1] = cone * (iurand() - 0.5f);
        ang[2] = cone * (iurand() - 0.5f);
        xMat3x3Euler(&variation, ang[0], ang[1], ang[2]);
        xMat3x3LMulVec(&p->m_vel, &variation, &p->m_vel);
    }
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

// Cache the particle group after the particle system has been set up.
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
        iModelTagSetup(&t->tag, ent->model->Data, tasset->e_offsetp.offset.x,
                       tasset->e_offsetp.offset.y, tasset->e_offsetp.offset.z);
    }
    t->attachTo = tasset->attachToID ? zSceneFindObject(tasset->attachToID) : NULL;
    t->parSys = tasset->parSysID ? zSceneFindObject(tasset->parSysID) : NULL;
    t->emit_volume = tasset->emit_type == 6 ? zSceneFindObject(tasset->e_volume.emit_volumeID) : NULL;
}
