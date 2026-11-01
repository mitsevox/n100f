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
#include "zGroup.h"
#include "zVolume.h"
#include "xMath2.h"
#include "xMovePoint.h"
#include "xParGroup.h"
#include "xstransvc.h"
#include <string.h>
#include <math.h>

S32 zParEmitterEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*);

// Set up the emitter from its asset and find its attach target, particle system and volume.
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

// Cache the particle group after the particle system has been set up.
void zParEmitterSetup(_zParEmitter* t)
{
    if (t->parSys)
        t->group = ((zParSys*)t->parSys)->group;
}

// Handle reset, on, off and single-emission events.
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

// Turn the particle's velocity by a random angle of up to half the asset's variation per axis.
static void zParEmitterAngleVariation(xPar* p, zParEmitterAsset* a)
{
    xMat3x3 mat_rot = {};
    F32 ang[3] = {};
    F32 ang_debrisCone = a->vel_angle_variation;

    ang[0] = ang_debrisCone * (iurand() - 0.5f);
    ang[1] = ang_debrisCone * (iurand() - 0.5f);
    ang[2] = ang_debrisCone * (iurand() - 0.5f);
    xMat3x3Euler(&mat_rot, ang[0], ang[1], ang[2]);
    xMat3x3LMulVec(&p->m_vel, &mat_rot, &p->m_vel);
}

// Place the particle on the edge of the emitter's XZ circle, optionally pushing it outward.
static void zParEmitterEmitCircleEdge(xPar* p, zParEmitterAsset* a, F32 dt)
{
    xVec3Copy(&p->m_vel, &a->vel);
    xVec3SMulBy(&p->m_vel, dt);

    F32 rot_amount = 360.0f * iurand();
    F32 ct = cos(rot_amount);
    F32 st = sin(rot_amount);
    xVec2 vec2 = { 0.0f, 1.0f };
    xMat2x2 rot_mat = { 1.0f, 0.0f, 0.0f, 1.0f };

    rot_mat.right.x = ct;
    rot_mat.right.y = st;
    rot_mat.up.x = -st;
    rot_mat.up.y = ct;

    F32 x = rot_mat.right.x * vec2.x + rot_mat.right.y * vec2.y;
    F32 z = rot_mat.up.x * vec2.x + rot_mat.up.y * vec2.y;
    x *= a->e_circle.radius;
    z *= a->e_circle.radius;
    p->m_pos.x += x;
    p->m_pos.z += z;

    if (a->vel_angle_variation != 0.0f)
    {
        zParEmitterAngleVariation(p, a);
    }

    if (a->e_circle.deflection != 0.0f)
    {
        F32 dt_radius = a->e_circle.deflection * (a->e_circle.radius * dt);
        p->m_vel.x += x * dt_radius;
        p->m_vel.z += z * dt_radius;
    }
}

// Place the particle at a random distance inside the emitter's XZ circle.
static void zParEmitterEmitCircle(xPar* p, zParEmitterAsset* a, F32 dt)
{
    xVec3Copy(&p->m_vel, &a->vel);
    xVec3SMulBy(&p->m_vel, dt);

    F32 rot_amount = 360.0f * iurand();
    F32 ct = cos(rot_amount);
    F32 st = sin(rot_amount);
    xVec2 vec2 = { 0.0f, 1.0f };
    xMat2x2 rot_mat = { 1.0f, 0.0f, 0.0f, 1.0f };

    rot_mat.right.x = ct;
    rot_mat.right.y = st;
    rot_mat.up.x = -st;
    rot_mat.up.y = ct;

    vec2.y = a->e_circle.radius * iurand();

    F32 x = rot_mat.right.x * vec2.x + rot_mat.right.y * vec2.y;
    F32 z = rot_mat.up.x * vec2.x + rot_mat.up.y * vec2.y;
    p->m_pos.x += x;
    p->m_pos.z += z;

    if (a->vel_angle_variation != 0.0f)
    {
        zParEmitterAngleVariation(p, a);
    }

    if (a->e_circle.deflection != 0.0f)
    {
        F32 dt_radius = a->e_circle.deflection * (a->e_circle.radius * dt);
        p->m_vel.x += x * dt_radius;
        p->m_vel.z += z * dt_radius;
    }
}

// Place the particle at a random distance inside the emitter's sphere. The helper's name is this
// project's; the SLUS-20349 build has no out-of-line copy.
static void zParEmitterEmitSphere(xPar* p, zParEmitterAsset* a, F32 dt)
{
    xVec3 dirvec = { 0.0f, 0.0f, 1.0f };
    xVec3 offset = { 0.0f, 0.0f, 0.0f };
    xMat3x3 mat_rot = {};

    xVec3Copy(&p->m_vel, &a->vel);
    xVec3SMulBy(&p->m_vel, dt);

    F32 roll = 6.2831855f * iurand();
    F32 pitch = 6.2831855f * iurand();
    F32 yaw = 6.2831855f * iurand();
    xMat3x3Euler(&mat_rot, yaw, pitch, roll);
    xMat3x3LMulVec(&dirvec, &mat_rot, &dirvec);

    F32 magnitude = sqrtf(xVec3Length2(&p->m_vel));
    xVec3SMul(&offset, &dirvec, a->e_sphere.radius * iurand());
    xVec3Add(&p->m_pos, &p->m_pos, &offset);

    if (a->vel_angle_variation != 0.0f)
    {
        zParEmitterAngleVariation(p, a);
    }
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
        zParEmitterAngleVariation(p, a);
    }
}

static _zParEmitterCustomSettings sSaveEmmiterSettings;

// Emit once with the flagged settings overridden, restoring the saved settings when asked.
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

// Emit this interval's particles: count them (with variation, capped by the particle system),
// then start each one at the emitter or attached object, give it its life, size and color
// rates, place it by the emitter shape, and kill it at once if the camera distance culls it.
// An emitter attached to a group emits from each member in turn, or from a random one.
xPar* zParEmitterEmit(_zParEmitter* pe, F32 dt)
{
    zParEmitterAsset* a = pe->tasset;
    xPar* last_p = NULL;

    if (pe->emit_interval_current > 0.0f)
    {
        pe->emit_interval_current -= dt;

        if (pe->emit_interval_current > 0.0f)
        {
            return NULL;
        }

        pe->emit_interval_current = pe->tasset->interval;
    }

    zParSys* ps = (zParSys*)pe->parSys;

    if (ps && ps->group)
    {
        if (ps->tasset->maxPar && ps->group->m_num_of_particles >= ps->tasset->maxPar)
        {
            return NULL;
        }

        F32 life = a->life;
        F32 size_birth = a->size_birth;
        U32 count = a->count;
        U8 variation = pe->tasset->count_variation;

        if (variation)
        {
            S32 delta = irand() % (variation * 2) - variation;
            count = a->count + (delta > 0 ? delta : 0);
        }

        if (ps->tasset->maxPar && ps->group->m_num_of_particles + count >= ps->tasset->maxPar)
        {
            count = ps->tasset->maxPar - ps->group->m_num_of_particles;
        }

        xBase* emitObj = (xBase*)pe->attachTo;
        S32 attachGroupIndex = -1;
        S32 attachGroupTotal = -1;
        S32 emitAgain = 0;

        do
        {
            xBase* attachObject = NULL;
            S32 marker = 0;
            xVec3 pos;

            if (emitObj)
            {
                if (emitObj->baseType == 0x11) // a group
                {
                    _zGroup* g = (_zGroup*)emitObj;

                    if (attachGroupIndex == -1)
                    {
                        attachGroupTotal = zGroupGetCount(g);
                        attachGroupIndex = 0;
                    }

                    if (g->asset->groupFlags & 1)
                    {
                        U32 get_rnd_group_idx = irand() % attachGroupTotal;
                        attachObject = zGroupGetItemPtr(g, get_rnd_group_idx);
                        emitAgain = 0;

                        if (!attachObject)
                        {
                            attachObject = (xBase*)xSTFindAsset(zGroupGetItem(g, get_rnd_group_idx), NULL);

                            if (attachObject)
                            {
                                marker = 1;
                            }
                        }
                    }
                    else
                    {
                        attachObject = zGroupGetItemPtr(g, attachGroupIndex);

                        if (!attachObject)
                        {
                            attachObject = (xBase*)xSTFindAsset(zGroupGetItem(g, attachGroupIndex), NULL);

                            if (attachObject)
                            {
                                marker = 1;
                            }
                        }

                        attachGroupIndex++;
                        emitAgain = attachGroupIndex < attachGroupTotal;
                    }
                }
                else
                {
                    attachObject = emitObj;
                }
            }

            if (attachObject)
            {
                if (marker)
                {
                    pos = *(xVec3*)attachObject;
                }
                else
                {
                    switch (attachObject->baseType)
                    {
                    case 0xD: // a move point
                        pos = *((xMovePoint*)attachObject)->pos;
                        break;
                    default:
                        pos = *(xVec3*)&((_xEnt*)attachObject)->model->Mat->pos;
                        break;
                    }
                }
            }
            else
            {
                pos = a->pos;
            }

            for (S32 i = 0; i < (S32)count; i++)
            {
                xPar* p = xParGroupAddPar(ps->group);

                if (!p)
                {
                    continue;
                }

                last_p = p;

                if (a->life_variation != 0.0f)
                {
                    life = 2.0f * (a->life_variation * iurand()) + a->life - a->life_variation;

                    if (life < 0.0f)
                    {
                        life = a->life;
                    }
                }

                if (a->size_birth_variation != 0.0f)
                {
                    size_birth = 2.0f * (a->size_birth_variation * iurand()) + a->size_birth -
                                 a->size_birth_variation;
                }

                p->m_asset = pe->tasset;
                p->m_lifetime = life;
                p->m_size = size_birth;

                if (a->size_death == size_birth)
                {
                    p->m_sizeVel = 0.0f;
                }
                else
                {
                    p->m_sizeVel = (a->size_death - size_birth) / life;
                }

                p->m_flag = 0;
                p->m_rotdeg[0] = pe->rot[0];
                p->m_rotdeg[1] = pe->rot[1];
                p->m_rotdeg[2] = pe->rot[2];

                for (S32 j = 0; j < 4; j++)
                {
                    p->m_c[j] = a->color_birth[j];
                    p->m_cvel[j] = a->color_death[j] - a->color_birth[j];
                    p->m_cvel[j] /= life;
                }

                p->m_pos = pos;

                switch (a->emit_type)
                {
                case 0:
                    xVec3Copy(&p->m_vel, &a->vel);
                    xVec3SMulBy(&p->m_vel, dt);

                    if (a->vel_angle_variation != 0.0f)
                    {
                        zParEmitterAngleVariation(p, a);
                    }
                    break;
                case 1:
                    zParEmitterEmitCircleEdge(p, a, dt);
                    break;
                case 2:
                    zParEmitterEmitCircle(p, a, dt);
                    break;
                case 4:
                    xVec3Copy(&p->m_vel, &a->vel);
                    xVec3SMulBy(&p->m_vel, dt);
                    p->m_pos.x += 2.0f * (a->e_rect.x_len * iurand()) - a->e_rect.x_len;
                    p->m_pos.z += 2.0f * (a->e_rect.z_len * iurand()) - a->e_rect.z_len;

                    if (a->vel_angle_variation != 0.0f)
                    {
                        zParEmitterAngleVariation(p, a);
                    }
                    break;
                case 5:
                {
                    xVec3Copy(&p->m_vel, &a->vel);
                    xVec3SMulBy(&p->m_vel, dt);

                    xVec3 dir;
                    xVec3Sub(&dir, &a->e_line.pos2, &a->e_line.pos1);
                    F32 along = xVec3Normalize(&dir, &dir) * iurand();
                    p->m_pos.x = dir.x * along + a->e_line.pos1.x;
                    p->m_pos.y = dir.y * along + a->e_line.pos1.y;
                    p->m_pos.z = dir.z * along + a->e_line.pos1.z;

                    if (a->vel_angle_variation != 0.0f)
                    {
                        zParEmitterAngleVariation(p, a);
                    }
                    break;
                }
                case 7:
                    zParEmitterEmitSphereEdge(p, a, dt, 7);
                    break;
                case 10:
                    zParEmitterEmitSphereEdge(p, a, dt, 10);
                    break;
                case 11:
                    zParEmitterEmitSphereEdge(p, a, dt, 11);
                    break;
                case 8:
                    zParEmitterEmitSphere(p, a, dt);
                    break;
                case 6:
                {
                    _zVolume* vol = (_zVolume*)pe->emit_volume;

                    xVec3Copy(&p->m_vel, &a->vel);
                    xVec3SMulBy(&p->m_vel, dt);

                    if (vol)
                    {
                        zVolumeAsset* va = vol->asset;

                        if (va->bound.type == 2)
                        {
                            const _xBox* box = &va->bound.box.box;
                            F32 x = (box->upper.x - box->lower.x) * iurand();
                            F32 y = (box->upper.y - box->lower.y) * iurand();
                            F32 z = (box->upper.z - box->lower.z) * iurand();
                            p->m_pos.x = x + box->lower.x;
                            p->m_pos.y = y + box->lower.y;
                            p->m_pos.z = z + box->lower.z;
                        }
                    }

                    if (a->vel_angle_variation != 0.0f)
                    {
                        zParEmitterAngleVariation(p, a);
                    }
                    break;
                }
                case 9:
                {
                    _xEnt* ent = (_xEnt*)pe->attachTo;
                    iModelTagEval(ent->model->Data, &pe->tag, ent->model->Mat, &p->m_pos);

                    if (a->vel_angle_variation != 0.0f)
                    {
                        zParEmitterAngleVariation(p, a);
                    }
                    break;
                }
                }

                S32 cull;

                if (pe->tasset->emit_flags & 2)
                {
                    F32 dx = globals.camera.hi_cam.mat.pos.x - p->m_pos.x;
                    F32 dy = globals.camera.hi_cam.mat.pos.y - p->m_pos.y;
                    F32 dz = globals.camera.hi_cam.mat.pos.z - p->m_pos.z;
                    F32 d2 = dx * dx + dy * dy + dz * dz;

                    if (pe->tasset->cull_mode == 3)
                    {
                        cull = d2 > pe->tasset->cull_dist_sqr;
                    }
                    else
                    {
                        cull = d2 < pe->tasset->cull_dist_sqr;
                    }
                }
                else
                {
                    cull = 0;
                }

                if (cull)
                {
                    xParGroupKillPar(ps->group, p);
                }
            }
        } while (emitAgain);
    }

    return last_p;
}

// Emit only while the game is running and this emitter is enabled.
void zParEmitterUpdate(xBase* to, _xScene*, F32 dt)
{
    _zParEmitter* pe = (_zParEmitter*)to;
    if (!zGameIsPaused() && (pe->emit_flags & 1))
        zParEmitterEmit(pe, dt);
}

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
