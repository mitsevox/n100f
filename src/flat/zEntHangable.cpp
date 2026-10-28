#include "zEntHangable.h"

#include "iMath.h"
#include "xAnim.h"
#include "xEvent.h"
#include "xString.h"
#include "zEntShaggy.h"
#include "zGlobals.h"
#include "zParEmitter.h"

static _zParEmitter* sCandleEmitter;
static _zParEmitter* sCandleSmokeEmitter;
static U32 sChandelierHash;
static _zParEmitter* sMountEmitter;

void zEntHangable_SetupFX()
{
    sCandleEmitter = zParEmitterFind("PAREMIT_CHAND_CANDLE");
    sCandleSmokeEmitter = zParEmitterFind("PAREMIT_CHAND_CANDLE_SMOKE");
    sChandelierHash = xStrHash("rlii0006");
    sMountEmitter = zParEmitterFind("PAREMIT_HANGABLE_MOUNT");
}

static void HangableSetup(_zEntHangable* ent, p2EntAsset* asset)
{
    p2EntHangableAsset* hangAsset = (p2EntHangableAsset*)(&asset[1]);
    xVec3* center;
    xMat3x3 hackMat;

    ent->pflags = 0;
    ent->chkby = 0;
    ent->penby = 0;
    ent->update = (void (*)(_xEnt*, _xScene*, F32))zEntHangable_Update;
    ent->move = NULL;
    ent->eventFunc = zEntHangableEventCB;
    ent->hangInfo = hangAsset;
    center = &ent->asset->pos;
    ent->pivot.x = center[0].x;
    ent->pivot.y = center[0].y + hangAsset->pivotOffset;
    ent->pivot.z = center[0].z;
    ent->endpos.x = center[0].x;
    ent->endpos.y = ent->pivot.y - hangAsset->leverArm;
    ent->endpos.z = center[0].z;
    xVec3Init(&ent->vel, 0.0f, 0.0f, 0.0f);
    ent->grabTimer = 0.0f;
    ent->spin = 0.0f;
    ent->state = 0;
    ent->shaggy = NULL;
    ent->follow = NULL;
    ent->enabled = 1;
    ent->moving = 0;
    ent->candle_timer = 3.0f;
    ent->candle_state = 0;
    center = &ent->asset->ang;
    xMat3x3Euler(&hackMat, center[0].x, center[0].y, center[0].z);
    xVec3Copy(&ent->swingplane, &hackMat.right);
}

void zEntHangable_Init(void* a, void* b)
{
    zEntHangable_Init((_zEntHangable*)a, (p2EntAsset*)b);
}

void zEntHangable_Init(_zEntHangable* ent, p2EntAsset* asset)
{
    zEntInit(ent, asset, 'HANG');

    // The links follow the hangable asset, which follows the entity asset.
    if (ent->linkCount)
    {
        ent->link = (p2LinkAsset*)((p2EntHangableAsset*)(&ent->asset[1]) + 1);
    }
    else
    {
        ent->link = NULL;
    }

    HangableSetup(ent, asset);
}

static void zEntHangable_UpdateFX(_zEntHangable* ent)
{
    // Candle positions around the chandelier, in model space.
    xVec3 offset_rlii0006[8] = {
        { 0.0f, 0.561f, 0.613f },
        { 0.43345639f, 0.432f, 0.43345639f },
        { 0.613f, 0.543f, 0.0f },
        { 0.43345639f, 0.526f, -0.43345639f },
        { 0.0f, 0.526f, -0.613f },
        { -0.43345639f, 0.505f, -0.43345639f },
        { -0.613f, 0.543f, 0.0f },
        { -0.43345639f, 0.48f, 0.43345639f },
    };
    xVec3* local_offset;
    _zParEmitterCustomSettings info;
    _zParEmitter* emitter;
    S32 i;
    xVec3 offset;

    if ((ent->flags & 1) && ent->asset->modelInfoID == sChandelierHash)
    {
        local_offset = offset_rlii0006;

        if (ent->candle_state == 2)
        {
            return;
        }

        info.custom_flags = 0x20;

        if (ent->candle_state == 0)
        {
            emitter = sCandleEmitter;
        }
        else
        {
            emitter = sCandleSmokeEmitter;
        }

        for (i = 0; i < 8; i++)
        {
            xMat3x3RMulVec(&offset, (xMat3x3*)ent->model->Mat, &local_offset[i]);
            xVec3Add(&info.pos, &offset, xEntGetPos(ent));
            zParEmitterEmitCustom(emitter, 1.0f / 30.0f, &info);
        }
    }
}

void zEntHangable_Update(_zEntHangable* ent, _xScene*, F32 dt)
{
    xVec3 sub;
    xVec3 unitHang;
    F32 dot;

    xVec3Copy(&ent->frame->oldpos, (xVec3*)&ent->model->Mat->pos);
    xMat3x3GetEuler((xMat3x3*)ent->model->Mat, &ent->frame->oldrot.axis);

    if (ent->enabled < 0 && (ent->enabled++, ent->enabled == 0))
    {
        ent->enabled = 1;
    }

    zEntHangable_UpdateFX(ent);

    if (!(ent->hangInfo->flags & 0x80000000))
    {
        ent->hangInfo->leverArm -= globals.player.ent.bound.cyl.r;
        ent->hangInfo->flags |= 0x80000000;
    }

    ent->grabTimer = 0.0f > ent->grabTimer - dt ? 0.0f : ent->grabTimer - dt;

    if (globals.player.HangEnt == ent)
    {
        return;
    }

    zEntHangable_FollowUpdate(ent);

    unitHang.x = 0.0f;
    unitHang.y = -ent->hangInfo->gravity;
    unitHang.z = 0.0f;

    xVec3Sub(&sub, &ent->endpos, &ent->pivot);
    xVec3Normalize(&sub, &sub);

    dot = xVec3Dot(&sub, &unitHang);
    unitHang.x -= dot * sub.x;
    unitHang.y -= dot * sub.y;
    unitHang.z -= dot * sub.z;

    ent->vel.x = (unitHang.x * dt) + ent->vel.x;
    ent->vel.y = (unitHang.y * dt) + ent->vel.y;
    ent->vel.z = (unitHang.z * dt) + ent->vel.z;

    dot = xVec3Dot(&sub, &ent->vel);
    ent->vel.x -= dot * sub.x;
    ent->vel.y -= dot * sub.y;
    ent->vel.z -= dot * sub.z;

    xVec3SMul(&ent->vel, &ent->vel, 0.97f);

    if (xVec3Length2(&ent->vel) < 0.001f)
    {
        ent->vel.x = 0.0f;
        ent->vel.y = 0.0f;
        ent->vel.z = 0.0f;
    }

    ent->endpos.x = ent->vel.x * dt + ent->endpos.x;
    ent->endpos.y = ent->vel.y * dt + ent->endpos.y;
    ent->endpos.z = ent->vel.z * dt + ent->endpos.z;

    xVec3Sub(&sub, &ent->endpos, &ent->pivot);
    xVec3Normalize(&sub, &sub);
    xVec3SMul(&sub, &sub, ent->hangInfo->leverArm);
    xVec3Add(&ent->endpos, &ent->pivot, &sub);

    zEntHangable_SetMatrix(ent, dt);
}

static void zEntHangableMountFX(_zEntHangable* ent)
{
    _zParEmitterCustomSettings info;

    if (sMountEmitter != NULL)
    {
        info.custom_flags = 0x60;
        for (S32 i = 0; i < 10; i++)
        {
            info.pos = ent->pivot;
            info.vel.x = 0.0f;
            info.vel.y = -2.0f - iurand() * 2.0f;
            info.vel.z = 0.0f;
            zParEmitterEmitCustom(sMountEmitter, 1.0f / 30.0f, &info);
        }
    }
}

S32 zEntHangableEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    _zEntHangable* ent = (_zEntHangable*)to;
    U32 oldvisible = xEntIsVisible(ent);
    switch (toEvent)
    {
    case eEventVisible:
        xEntShow(ent);
        break;
    case eEventInvisible:
        xEntHide(ent);
        break;
    case eEventCollisionOn:
        ent->enabled = 1;
        break;
    case eEventMount:
        zEntHangableMountFX(ent);
        if (ent->asset->modelInfoID == sChandelierHash && ent->candle_state == 0)
        {
            ent->candle_timer = 3.0f;
        }
        break;
    case eEventDismount:
        ent->enabled = -2;
        break;
    case eEventShaggyMount:
    {
        _zEnt* shagent = (_zEnt*)toParamWidget;

        if (shagent != NULL && shagent->subType == 0x1D)
        {
            xAnimPlaySetState(shagent->model->Anim->Single,
                              xAnimTableGetState(shagent->model->Anim->Table, "Hang"), 0.0f);
            zEntHangable_SetShaggy(ent, shagent);
            Shaggy1_SetHangable(shagent, ent);
            zEntHangableMountFX(ent);
        }
        break;
    }
    case eEventCollisionOff:
        ent->enabled = 0;
        break;
    case eEventCollision_Visible_On:
        xEntShow(ent);
        ent->enabled = 1;
        break;
    case eEventCollision_Visible_Off:
        xEntHide(ent);
        ent->enabled = 0;
        break;
    case eEventReset:
        zEntHangable_Reset(ent);
        break;
    case eEventSwingerFollow:
    {
        _zEnt* follow = (_zEnt*)from;

        zEntHangable_SetFollow(ent, follow);
        break;
    }
    }

    if (oldvisible == 0 && xEntIsVisible(ent) != 0)
    {
        ent->vel.x = 0.0f;
        ent->vel.y = 0.0f;
        ent->vel.z = 0.0f;
        ent->endpos.x = ent->pivot.x;
        ent->endpos.y = ent->pivot.y - ent->hangInfo->leverArm;
        ent->endpos.z = ent->pivot.z;
    }

    return 1;
}

static S32 HangableIsMovingTooMuch(xVec3* a, xVec3* b, xVec3* c, xVec3* d)
{
    F32 dist = (d->x * d->x + d->y * d->y + d->z * d->z) - (c->x * c->x + c->y * c->y + c->z * c->z);
    if (dist > 20.0f || dist < -20.0f)
    {
        return 1;
    }
    return 0;
}

void zEntHangable_SetMatrix(_zEntHangable* ent, F32 dt)
{
    xMat4x3 tmpMat;
    xMat3x3 spinMat;
    F32 height;
    S32 moving;
    xVec3* opos;
    xVec3* pos;
    xVec3* orot;
    xVec3 rot;
    _zEnt* shaggy;

    xVec3Sub(&tmpMat.up, &ent->pivot, &ent->endpos);
    xVec3Normalize(&tmpMat.up, &tmpMat.up);

    opos = &tmpMat.at;
    opos->x = 0.0f;
    opos->y = 0.0f;
    opos->z = 1.0f;

    xVec3Cross(&tmpMat.right, &tmpMat.up, opos);
    xVec3Normalize(&tmpMat.right, &tmpMat.right);
    xMat3x3RotC(&spinMat, tmpMat.up.x, tmpMat.up.y, tmpMat.up.z, ent->spin);
    xMat3x3RMulVec(&tmpMat.right, &spinMat, &tmpMat.right);
    xVec3Cross(opos, &tmpMat.right, &tmpMat.up);

    tmpMat.pos.x = 0.0f;
    tmpMat.pos.y = -ent->hangInfo->pivotOffset;
    tmpMat.pos.z = 0.0f;

    xMat3x3RMulVec(&tmpMat.pos, (xMat3x3*)&tmpMat, &tmpMat.pos);

    tmpMat.pos.x += ent->pivot.x;
    tmpMat.pos.y += ent->pivot.y;
    tmpMat.pos.z += ent->pivot.z;
    tmpMat.flags = 0;

    *(xMat4x3*)ent->model->Mat = tmpMat;

    shaggy = ent->shaggy;
    if (shaggy != NULL)
    {
        // Shaggy hangs below the end of the lever, along its axis.
        height = globals.player.Shaggy1.HangTrans.y - globals.player.Shaggy1.ReleaseTrans.y -
                 globals.player.ent.bound.cyl.r;
        shaggy->model->Mat->pos.x = height * tmpMat.up.x + ent->endpos.x;
        shaggy->model->Mat->pos.y = height * tmpMat.up.y + ent->endpos.y;
        shaggy->model->Mat->pos.z = height * tmpMat.up.z + ent->endpos.z;

        xMat3x3RotC(&spinMat, tmpMat.up.x, tmpMat.up.y, tmpMat.up.z, zEntShaggy_GetSpin(shaggy));
        xMat3x3RMulVec(&tmpMat.right, &spinMat, &tmpMat.right);
        xVec3Cross(&tmpMat.at, &tmpMat.right, &tmpMat.up);

        *(xMat3x3*)shaggy->model->Mat = *(xMat3x3*)&tmpMat;
    }

    opos = &ent->frame->oldpos;
    orot = &ent->frame->oldrot.axis;
    pos = (xVec3*)&ent->model->Mat->pos;
    xMat3x3GetEuler((xMat3x3*)ent->model->Mat, &rot);

    if (opos->x != pos->x || opos->y != pos->y || opos->z != pos->z || orot->x != rot.x ||
        orot->y != rot.y || orot->z != rot.z)
    {
        moving = 1;
        if (ent->moving != 1)
        {
            zEntEvent(ent, eEventStartMoving);
        }
    }
    else
    {
        moving = 0;
        if (ent->moving != 0)
        {
            zEntEvent(ent, eEventStopMoving);
        }
    }

    ent->moving = moving;

    if (ent->asset->modelInfoID != sChandelierHash)
    {
        return;
    }

    if (ent->candle_state == 0)
    {
        ent->candle_timer -= dt;
        if (ent->candle_timer <= 0.0f)
        {
            ent->candle_timer = 0.0f;
            moving = HangableIsMovingTooMuch(orot, &rot, opos, pos);
            if (moving != 0)
            {
                ent->candle_state = 1;
                ent->candle_timer = iurand() * 10.0f + 10.0f;
            }
        }
    }
    else if (ent->candle_state == 1)
    {
        ent->candle_timer -= dt;
        if (ent->candle_timer <= 0.0f)
        {
            ent->candle_timer = iurand() * 5.0f + 10.0f;
            ent->candle_state = 2;
        }
    }
    else
    {
        ent->candle_timer -= dt;
        if (ent->candle_timer <= 0.0f)
        {
            ent->candle_timer = 0.0f;
            moving = HangableIsMovingTooMuch(orot, &rot, opos, pos);
            if (moving == 0)
            {
                ent->candle_timer = 3.0f;
                ent->candle_state = 0;
            }
        }
    }
}

void zEntHangable_Save(_zEntHangable* ent, xSerial* s)
{
    zEntSave(ent, s);
}

void zEntHangable_Load(_zEntHangable* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

void zEntHangable_Reset(_zEntHangable* ent)
{
    zEntHangable_SetShaggy(ent, NULL);
    zEntHangable_SetFollow(ent, NULL);
    xEntReset(ent);
    HangableSetup(ent, ent->asset);
}

void zEntHangable_SetShaggy(_zEntHangable* ent, _zEnt* shaggy)
{
    if (shaggy != NULL)
    {
        if (ent->shaggy == NULL)
        {
            ent->shaggy = shaggy;
            ent->hangInfo->leverArm += 2.2f;
            ent->endpos.y -= 2.2f;
            shaggy->frame->mode = 1;
            shaggy->frame->vel.x = 0.0f;
            shaggy->frame->vel.y = 0.0f;
            shaggy->frame->vel.z = 0.0f;
            shaggy->frame->mat.pos.x = ent->model->Mat->pos.x;
            shaggy->frame->mat.pos.z = ent->model->Mat->pos.z;
            shaggy->frame->mat.pos.y = ent->endpos.y + globals.player.Shaggy1.HangTrans.y -
                                       globals.player.Shaggy1.ReleaseTrans.y -
                                       globals.player.ent.bound.cyl.r;
            *shaggy->model->Mat = *(RwMatrix*)&shaggy->frame->mat;
        }
    }
    else if (ent->shaggy != NULL)
    {
        ent->shaggy = NULL;
        ent->hangInfo->leverArm -= 2.2f;
        ent->endpos.y += 2.2f;
    }
}

void zEntHangable_FollowUpdate(_zEntHangable* ent)
{
    xVec3* center;
    xVec3 delta;
    xVec3 pivot;

    if (ent->follow == NULL)
    {
        return;
    }

    p2EntHangableAsset* hang = (p2EntHangableAsset*)(&ent->asset[1]);

    center = (xVec3*)(&ent->follow->model->Mat->pos);
    pivot.x = center->x;
    pivot.y = center->y + hang->pivotOffset;
    pivot.z = center->z;

    delta.x = pivot.x - ent->pivot.x;
    delta.y = pivot.y - ent->pivot.y;
    delta.z = pivot.z - ent->pivot.z;

    xVec3Copy(&ent->pivot, &pivot);

    if (globals.player.HangEnt == ent)
    {
        xVec3Copy(&globals.player.HangPivot, &pivot);
        globals.player.ent.model->Mat->pos.x += delta.x;
        globals.player.ent.model->Mat->pos.y += delta.y;
        globals.player.ent.model->Mat->pos.z += delta.z;
    }

    ent->endpos.x += delta.x;
    ent->endpos.y += delta.y;
    ent->endpos.z += delta.z;

    ent->model->Mat->pos.x += delta.x;
    ent->model->Mat->pos.y += delta.y;
    ent->model->Mat->pos.z += delta.z;

    if (ent->shaggy != NULL)
    {
        ent->shaggy->model->Mat->pos.x += delta.x;
        ent->shaggy->model->Mat->pos.y += delta.y;
        ent->shaggy->model->Mat->pos.z += delta.z;
    }
}

void zEntHangable_SetFollow(_zEntHangable* ent, _zEnt* follow)
{
    if (follow != NULL)
    {
        ent->follow = follow;
        zEntHangable_FollowUpdate(ent);
    }
    else if (ent->follow != NULL)
    {
        ent->follow = NULL;
    }
}
