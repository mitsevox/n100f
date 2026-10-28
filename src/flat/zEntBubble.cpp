#include "zEntBubble.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zEntVillain.h"
#include "zParEmitter.h"
#include "xParGroup.h"
#include "xPar.h"
#include "xBound.h"
#include "xMath.h"
#include "xMathInlines.h"
#include "xMemMgr.h"
#include "xModel.h"
#include "xstransvc.h"
#include "xString.h"
#include "iMath.h"
#include "iModel.h"
#include <rpworld.h>
#include <string.h>
#include <math.h>

static RpAtomic* gBubbleModel;
static _zParEmitter* sEmitGumWad;
static _zParEmitter* sEmitSoapFlight;

static void zEntBubble_FX(_zEntBubble* ent, F32 dt);

void zEntBubble_SceneInit(zScene* scene)
{
    U32 bufsize;
    _zEntBubble* bubbleBlock = (_zEntBubble*)xMemAlloc(gActiveHeap, 20 * sizeof(_zEntBubble));
    _xEntFrame* frameBlock = (_xEntFrame*)xMemAlloc(gActiveHeap, 20 * sizeof(_xEntFrame));

    scene->bubbles = (_zEnt**)xMemAlloc(gActiveHeap, 20 * sizeof(_zEnt*));
    scene->num_bubbles = 0;

    gBubbleModel = (RpAtomic*)xSTFindAsset(xStrHash("sphere.dff"), &bufsize);

    for (U32 idx = 0; idx < 20; idx++)
    {
        scene->bubbles[idx] = &bubbleBlock[idx];

        bubbleBlock[idx].id = 0;
        bubbleBlock[idx].baseType = 0xF;
        bubbleBlock[idx].baseFlags = 0x5;
        bubbleBlock[idx].linkCount = 0;
        bubbleBlock[idx].link = NULL;
        bubbleBlock[idx].eventFunc = zEntBubbleEventCB;

        bubbleBlock[idx].flags = 0x1;
        bubbleBlock[idx].collType = 0xFF;
        bubbleBlock[idx].collLev = 4;
        bubbleBlock[idx].pflags = 0x3;
        bubbleBlock[idx].chkby = 0x10;
        bubbleBlock[idx].penby = 0x10;
        bubbleBlock[idx].idx = idx;
        bubbleBlock[idx].model = xModelInstanceAlloc(gBubbleModel, &bubbleBlock[idx], 0, 0, NULL);
        bubbleBlock[idx].update = (void (*)(_xEnt*, _xScene*, F32))zEntBubble_Update;
        bubbleBlock[idx].bupdate = xEntDefaultBoundUpdate;
        bubbleBlock[idx].move = (void (*)(_xEnt*, _xScene*, F32, _xEntFrame*))zEntBubble_Move;
        bubbleBlock[idx].frame = &frameBlock[idx];
        bubbleBlock[idx].collis = NULL;
        bubbleBlock[idx].bound.type = 1;
        bubbleBlock[idx].collLev = 4;
    }

    sEmitGumWad = zParEmitterFind("PAREMIT_GUMWAD");
    if (sEmitGumWad && sEmitGumWad->group)
    {
        xParGroupSetStealing(sEmitGumWad->group, 0);
    }

    sEmitSoapFlight = zParEmitterFind("PAREMIT_SOAP_FLIGHT");
    if (sEmitSoapFlight && sEmitSoapFlight->group)
    {
        xParGroupSetStealing(sEmitSoapFlight->group, 0);
    }
}

static _zEnt* AutoTarget()
{
    U32 i;
    F32 dirAngle;
    F32 minusAngle;
    F32 plusAngle;
    F32 pitchSin;
    F32 px, py, pz;
    F32 nearDist = 0.0f;
    _zEnt* near = NULL;
    _zEnt** vill = (_zEnt**)globals.sceneCur->npcs;
    F32 dx, dy, dz;
    F32 xzdist;
    F32 spitch;
    xVec3 dir, minus, plus;

    px = globals.player.ent.bound.sph.center.x;
    py = globals.player.ent.bound.sph.center.y;
    pz = globals.player.ent.bound.sph.center.z;

    dirAngle = globals.player.ent.frame->rot.angle;
    minusAngle = dirAngle - (PI / 180.0f) * globals.player.s.FireTargetYaw - PI / 2;
    plusAngle = PI / 2 + (dirAngle + (PI / 180.0f) * globals.player.s.FireTargetYaw);

    dir.x = (F32)sin(dirAngle);
    dir.z = (F32)cos(dirAngle);
    minus.x = (F32)sin(minusAngle);
    minus.z = (F32)cos(minusAngle);
    plus.x = (F32)sin(plusAngle);
    plus.z = (F32)cos(plusAngle);

    pitchSin = (F32)sin((PI / 180.0f) * globals.player.s.FireTargetPitch);

    for (i = 0; i < globals.sceneCur->num_npcs; i++)
    {
        if (xEntIsVisible(vill[i]) && (vill[i]->baseFlags & 0x1))
        {
            dx = vill[i]->bound.sph.center.x - px;
            dy = vill[i]->bound.sph.center.y - py;
            dz = vill[i]->bound.sph.center.z - pz;

            xzdist = dx * dir.x + dz * dir.z;
            if (xzdist > globals.player.s.FireTargetDist &&
                dx * minus.x + dz * minus.z < globals.player.s.FireTargetRadius &&
                dx * plus.x + dz * plus.z < globals.player.s.FireTargetRadius)
            {
                spitch = dy / sqrtf(dx * dx + dy * dy + dz * dz);
                if (iabs(spitch) <= pitchSin)
                {
                    if (!near || xzdist < nearDist)
                    {
                        near = vill[i];
                        nearDist = xzdist;
                    }
                }
            }
        }
    }

    globals.player.FireTarget = near;
    return near;
}

void zEntBubble_EnableCollision()
{
    S32 i;
    zScene* sc = globals.sceneCur;
    _zEntBubble** bub = (_zEntBubble**)sc->bubbles;

    for (i = 0; i < (S32)sc->num_bubbles; i++)
    {
        if (bub[i]->state == 0x10 && bub[i]->timer != 0.0f && sc->num_dyns < sc->num_dyns_allocd)
        {
            sc->dyns[sc->num_dyns++] = bub[i];
        }
    }
}

void zEntBubble_DisableCollision()
{
    S32 i;
    zScene* sc = globals.sceneCur;

    for (i = 0; i < sc->num_dyns; i++)
    {
        if (sc->dyns[i]->baseType == 0xF)
        {
            sc->num_dyns = i;
            return;
        }
    }
}

void zEntBubble_Launch(F32 wait)
{
    zScene* sc = globals.sceneCur;

    if (sc->num_bubbles < 20)
    {
        _zEntBubble* bub = (_zEntBubble*)sc->bubbles[sc->num_bubbles++];

        bub->subType = globals.player.LastProjectile;
        bub->fx_timer = 0.0f;

        memcpy(bub->model->Mat, &g_I3, sizeof(xMat4x3));
        bub->model->Mat->pos.x = bub->bound.sph.center.x = globals.player.ent.bound.sph.center.x;
        bub->model->Mat->pos.y = bub->bound.sph.center.y = globals.player.ent.bound.sph.center.y;
        bub->model->Mat->pos.z = bub->bound.sph.center.z = globals.player.ent.bound.sph.center.z;
        bub->bound.sph.r = 0.5f;
        memcpy(bub->frame, bub->model->Mat, sizeof(xMat4x3));

        bub->frame->rot.axis.x = 0.0f;
        bub->frame->rot.axis.y = 1.0f;
        bub->frame->rot.axis.z = 0.0f;
        bub->frame->rot.angle = 0.0f;

        bub->frame->vel.x = 0.0f;
        bub->frame->vel.y = 0.0f;
        bub->frame->vel.z = 0.0f;

        bub->state = 1;
        bub->villain = AutoTarget();
        bub->bubbleScale = 1.0f;
        bub->timer = wait;
        bub->stuckTimer = 0.0f;

        bub->fx_scale = 0.25f;
        bub->model->Scale.x = bub->fx_scale;
        bub->model->Scale.y = bub->fx_scale;
        bub->model->Scale.z = bub->fx_scale;

        bub->model->Flags &= ~0x1;

        globals.player.Inv_Ammo[globals.player.LastProjectile] =
            CLAMP(globals.player.Inv_Ammo[globals.player.LastProjectile] - 1, 0,
                  globals.player.Inv_AmmoMax[globals.player.LastProjectile]);

        if (globals.player.LastProjectile == 1)
        {
            bub->pflags |= 0x20;
            bub->particle = NULL;
        }
        else
        {
            bub->pflags &= 0xDF;
        }
    }
}

void zEntBubble_Update(_zEntBubble* ent, _xScene* sc, F32 dt)
{
    _xEntCollis coll;
    F32 flyspeed;
    F32 dx, dy, dz;
    F32 mag;
    F32 spitch;
    F32 cpitch;
    _zParEmitterCustomSettings info;
    F32 lerp;
    xVec3 vc;

    memset(&coll, 0, sizeof(_xEntCollis));
    coll.chk = 0x2E;
    ent->collis = &coll;

    switch (ent->state)
    {
    case 1:
        ent->timer -= dt;
        if (ent->timer < 0.0f)
        {
            flyspeed = 10.0f;
            ent->state = 4;

            iModelTagEval(globals.player.ent.model->Data, &globals.player.BubbleTag,
                          globals.player.ent.model->Mat, (xVec3*)&ent->model->Mat->pos);
            memcpy(&ent->frame->mat.pos, &ent->model->Mat->pos, sizeof(xVec3));
            globals.player.FireTarget = NULL;

            ent->frame->vel.x = (F32)sin(globals.player.ent.frame->rot.angle);
            ent->frame->vel.y = 0.0f;
            ent->frame->vel.z = (F32)cos(globals.player.ent.frame->rot.angle);

            if (ent->villain)
            {
                dx = ent->villain->bound.sph.center.x - ent->model->Mat->pos.x;
                dy = ent->villain->bound.sph.center.y - ent->model->Mat->pos.y;
                dz = ent->villain->bound.sph.center.z - ent->model->Mat->pos.z;

                mag = sqrtf(dx * dx + dy * dy + dz * dz);
                if (mag > 1e-6f)
                {
                    spitch = dy / mag;
                    cpitch = 1.0f - spitch * spitch;
                    if (cpitch < 0.0f)
                    {
                        cpitch = 0.0f;
                    }
                    else
                    {
                        cpitch = sqrtf(cpitch);
                    }

                    ent->frame->vel.x *= cpitch;
                    ent->frame->vel.y = spitch;
                    ent->frame->vel.z *= cpitch;
                }
            }

            switch (ent->subType)
            {
            case 0:
                flyspeed = globals.player.s.FireGumSpeed;
                ent->timer = globals.player.s.FireGumTimer;

                if (sEmitGumWad)
                {
                    info.custom_flags = 0x20;
                    info.pos.x = ent->model->Mat->pos.x;
                    info.pos.y = ent->model->Mat->pos.y;
                    info.pos.z = ent->model->Mat->pos.z;
                    ent->particle = zParEmitterEmitCustom(sEmitGumWad, 1.0f / 30.0f, &info);
                }
                break;
            case 1:
                ent->model->Flags |= 0x1;
                flyspeed = globals.player.s.FireSoapSpeed;
                ent->timer = globals.player.s.FireSoapTimer;
                break;
            }

            ent->frame->vel.x *= flyspeed;
            ent->frame->vel.y *= flyspeed;
            ent->frame->vel.z *= flyspeed;
        }
        break;
    case 0x10:
        ent->timer += dt;

        ent->model->Scale.x = 0.0f;
        ent->model->Scale.y = 0.0f;
        ent->model->Scale.z = 0.0f;

        lerp = 5.0f * ent->timer;
        if (lerp > 1.0f)
        {
            lerp = 1.0f;
        }

        mag = 2.2f * ent->villain->model->Data->boundingSphere.radius;
        ent->model->Mat->right.x = ent->model->Mat->up.y = ent->model->Mat->at.z = xlerp(1.0f, mag, lerp);

        vc = ent->villain->bound.sph.center;

        ent->model->Mat->pos.x = xlerp(ent->envelopStart.x, vc.x, lerp);
        ent->model->Mat->pos.y = xlerp(ent->envelopStart.y, vc.y, lerp);
        ent->model->Mat->pos.z = xlerp(ent->envelopStart.z, vc.z, lerp);

        ent->bound.sph.r = 0.5f * ent->model->Mat->right.x;
        ent->bound.sph.center = vc;

        xBoundUpdate(&ent->bound);
        break;
    case 4:
    {
        _xCollis* coll;
        _xCollis* endcoll;
        _zEnt* vill;

        xEntBeginUpdate(ent, sc, dt);
        zEntBubble_FX(ent, dt);

        ent->timer -= dt;
        if (ent->timer < 0.0f)
        {
            ent->state = 0x80;
            if (ent->particle)
            {
                ent->particle->m_lifetime = 0.0f;
                ent->particle = NULL;
            }
            break;
        }

        if (ent->pflags & 0x2)
        {
            xEntApplyPhysics(ent, sc, dt);
        }
        if (ent->pflags & 0x1)
        {
            xEntMove(ent, sc, dt);
        }

        xEntCollide(ent, sc, dt);

        if (ent->collis->npc_eidx > ent->collis->npc_sidx)
        {
            coll = &ent->collis->colls[ent->collis->npc_sidx];
            endcoll = &ent->collis->colls[ent->collis->npc_eidx];
            for (; coll < endcoll; coll++)
            {
                if (ent->particle)
                {
                    ent->particle->m_lifetime = 0.0f;
                    ent->particle = NULL;
                }

                vill = (_zEnt*)coll->optr;
                switch (zEntVillain_Bubble((st_zEntVillain*)vill, ent))
                {
                case ZVIL_BUBBLE_ENVELOP:
                    ent->state = 0x10;
                    ent->villain = vill;
                    ent->timer = 0.0f;
                    ent->envelopStart = ent->frame->mat.pos;
                    coll = endcoll;
                    break;
                case ZVIL_BUBBLE_BREAK:
                    ent->state = 8;
                    ent->timer = 0.0f;
                    coll = endcoll;
                    break;
                case ZVIL_BUBBLE_BOUNCE:
                case ZVIL_BUBBLE_NONE:
                    break;
                }
            }
        }

        xEntEndUpdate(ent, sc, dt);

        // Stuck: the bubble moved less than half a unit per second this frame.
        if ((ent->frame->oldpos.x - ent->frame->mat.pos.x) * (ent->frame->oldpos.x - ent->frame->mat.pos.x) +
                (ent->frame->oldpos.y - ent->frame->mat.pos.y) * (ent->frame->oldpos.y - ent->frame->mat.pos.y) +
                (ent->frame->oldpos.z - ent->frame->mat.pos.z) * (ent->frame->oldpos.z - ent->frame->mat.pos.z) <
            SQ(0.5f * dt))
        {
            ent->stuckTimer += dt;
            if (ent->stuckTimer > 0.2f)
            {
                ent->state = 8;
            }
        }
        else
        {
            ent->stuckTimer = 0.0f;
        }
        break;
    }
    case 2:
        break;
    case 8:
    case 0x40:
    default:
        ent->state = 0x80;
        break;
    }
}

void zEntBubble_Move(_zEntBubble* ent, _xScene* sc, F32 dt, _xEntFrame* frame)
{
    frame->mode = 0;
}

void zEntBubble_Pop(_zEntBubble* ent)
{
    ent->state = 0x40;
    ent->timer = 0.0f;
}

void zEntBubble_Cleanup()
{
    U32 i;
    zScene* sc = globals.sceneCur;
    _zEnt* tmp;

    for (i = 0; i < sc->num_bubbles;)
    {
        if (((_zEntBubble*)sc->bubbles[i])->state == 0x80)
        {
            sc->num_bubbles--;
            tmp = sc->bubbles[i];
            sc->bubbles[i] = sc->bubbles[sc->num_bubbles];
            sc->bubbles[sc->num_bubbles] = tmp;
        }
        else
        {
            i++;
        }
    }
}

void zEntBubble_Render()
{
    U32 i;
    zScene* sc = globals.sceneCur;

    for (i = 0; i < sc->num_bubbles; i++)
    {
        if (sc->bubbles[i]->subType == 1)
        {
            RwRGBA colsoap = { 100, 100, 128, 128 };
            sc->bubbles[i]->model->Data->geometry->matList.materials[0]->color = colsoap;
            xModelRender(sc->bubbles[i]->model);
        }
    }
}

S32 zEntBubbleEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    return 0;
}

static void zEntBubble_FX(_zEntBubble* ent, F32 dt)
{
    _zParEmitterCustomSettings info;

    if (ent->subType == 1)
    {
        ent->fx_timer -= dt;
        if (ent->fx_timer <= 0.0f && sEmitSoapFlight)
        {
            info.custom_flags = 0x20;
            info.pos.x = ent->model->Mat->pos.x;
            info.pos.y = ent->model->Mat->pos.y;
            info.pos.z = ent->model->Mat->pos.z;
            ent->fx_timer = 0.0f;
            zParEmitterEmitCustom(sEmitSoapFlight, 1.0f / 30.0f, &info);
        }

        ent->fx_scale += dt;
        if (ent->fx_scale > 1.0f)
        {
            ent->fx_scale = 1.0f;
        }

        ent->model->Scale.x = ent->fx_scale;
        ent->model->Scale.y = ent->fx_scale;
        ent->model->Scale.z = ent->fx_scale;
    }
    else
    {
        ent->fx_timer -= dt;
        if (ent->fx_timer <= 0.0f)
        {
            ent->fx_timer = 0.1f;
        }

        if (ent->particle)
        {
            ent->particle->m_pos.x = ent->model->Mat->pos.x;
            ent->particle->m_pos.y = ent->model->Mat->pos.y;
            ent->particle->m_pos.z = ent->model->Mat->pos.z;
        }
    }
}
