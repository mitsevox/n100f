#include "zEntPickup.h"

#include "iAnim.h"
#include "iModel.h"
#include "libc/math.h"
#include "libc/stdio.h"
#include "xBound.h"
#include "xEvent.h"
#include "xMath.h"
#include "xMath3.h"
#include "xModel.h"
#include "xParGroup.h"
#include "xserializer.h"
#include "xSnd.h"
#include "xstransvc.h"
#include "xString.h"
#include "zCounter.h"
#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zParEmitter.h"
#include "zScene.h"

struct _zUI;
struct p2SimpleObjAsset;

struct _zEntSimpleObj : _zEnt
{
    p2SimpleObjAsset* sasset;
    U32 sflags;
};

struct _tagKeyShake
{
    xVec3 orig_pos;
    F32 shake_timer;
    F32 shake_it;
    _zUI* ui;
};

U32 zSceneGetAreaSnacksCollected(U32 sceneID);
U32 zSceneGetAreaConstTotalSnacks(U32 sceneID);

extern _zParEmitter* sEmitDust;

static F32 sKeysOffTimer = -10.1f;
S32 zHudShowWarpGateInfo = -1;

static _tagKeyShake sKeyShake[4] = {};

static zEntPickupWarpPoint zWarpPointTable[27] = {
    { 0, 0, 'B004' },  { 1, 0, 'C004' },  { 2, 0, 'E004' },  { 3, 0, 'E006' },  { 4, 0, 'E009' },
    { 5, 0, 'F003' },  { 6, 0, 'F007' },  { 7, 0, 'F010' },  { 8, 0, 'G005' },  { 9, 0, 'G008' },
    { 10, 1, 'H001' }, { 11, 0, 'I003' }, { 12, 0, 'I006' }, { 13, 0, 'L014' }, { 14, 0, 'L018' },
    { 15, 0, 'O004' }, { 16, 0, 'O006' }, { 17, 0, 'P003' }, { 18, 0, 'P005' }, { 19, 0, 'R003' },
    { 20, 0, 'S002' }, { 21, 0, 'W022' }, { 22, 0, 'W026' }, { 23, 0, 'L015' }, { 24, 0, 'G001' },
    { 25, 0, 'H003' }, { 0, 0, 0 },
};

static S32 sKeyCounterEnabledBeforeGive;
static _zParEmitter* sEmitGlow;
static _zParEmitter* sEmitMmm;
static _zParEmitter* sEmitQMark;
static _zParEmitter* sEmitBubble;
static _zParEmitter* sEmitGum;
static _zParEmitter* sEmitOdour;
static _zParEmitter* sEmitGrabPowerup;
static _zParEmitter* sEmitGrabCrumb;
static _zParEmitter* sEmitGrabSnackBox;
static _zParEmitter* sEmitGrabMmm;
static _zParEmitter* sEmitGrabQMark;
static _zParEmitter* sEmitGrabGum;
static _zParEmitter* sEmitGrabBubbles;
static _zParEmitter* sEmitLoveHart;
static _zParEmitter* sEmitAkmalDaisy;
static _zParEmitter* sEmitGrabGhost;
static _zParEmitter* sEmitGrabSDLogo;
static _zParEmitter* sEmitWarpGate;
static _zParEmitter* sEmitMiniVanPuff;
static _zCounter* keyCounter;

static void zEntPickup_hideWarpGateInfo();
static void zEntPickup_resetKeyShake();

void zEntPickupInit(void* ent, void* asset)
{
    zEntPickupInit((_zEntPickup*)ent, (p2EntAsset*)asset);
}

void zEntPickupInit(_zEntPickup* ent, p2EntAsset* asset)
{
    p2EntPickupAsset* pickupAsset = (p2EntPickupAsset*)(asset + 1);
    U32 i;
    U32 tmpsize;
    zAssetPickup* ptbl;
    RpAtomic* modelData;
    void* animData;

    zEntInit(ent, asset, 'PKUP');

    if (ent->linkCount)
    {
        ent->link = (p2LinkAsset*)((p2EntPickupAsset*)(ent->asset + 1) + 1);
    }
    else
    {
        ent->link = NULL;
    }

    for (i = 0; i < ent->linkCount; i++)
    {
        if (ent->link[i].dstEvent == eEventDispatcher_SetSaveState_Saving)
        {
            ent->link[i].dstAssetID = ent->id;
        }
    }

    ent->pflags = 0;
    ent->chkby = 0;
    ent->penby = 0;
    ent->update = (void (*)(_xEnt*, _xScene*, F32))zEntPickup_Update;
    ent->move = NULL;
    ent->eventFunc = zEntPickupEventCB;
    ent->followTarget = NULL;
    ent->followOffset.x = 0.0f;
    ent->followOffset.y = 0.0f;
    ent->followOffset.z = 0.0f;
    ent->shake_timer = 0.0f;
    ent->snackGateInfoDisplayed = 0;
    ent->dropParent = NULL;

    // A hash missing from the table leaves ptbl on the first entry.
    ptbl = (zAssetPickup*)(globals.pickupTable + 1);

    for (i = 0; i < globals.pickupTable->Count; i++)
    {
        if (pickupAsset->pickupHash == ptbl[i].pickupHash)
        {
            ptbl += i;
            break;
        }
    }

    if (i == globals.pickupTable->Count)
    {
        printf("ERROR:  Pickup table entry not found for hash 0x%x\n", pickupAsset->pickupHash);
    }

    modelData = (RpAtomic*)xSTFindAsset(ptbl->modelID, &tmpsize);
    animData = NULL;

    if (ptbl->animID)
    {
        animData = xSTFindAsset(ptbl->animID, &tmpsize);

        if (iAnimBoneCount(animData) != iModelNumBones(modelData))
        {
            animData = NULL;
        }
    }

    ent->model = xModelInstanceAlloc(modelData, ent, 0, 0, NULL);
    ent->model->Flags &= (U16)~0x2;
    ent->state = 0x41;
    ent->p = ptbl;

    if (ent->p->pickupType == 1 && ent->p->pickupIndex == 2)
    {
        ent->p->pickupIndex = 1;
    }

    if (ent->p->pickupType == 1 && (ent->p->pickupIndex == 4 || ent->p->pickupIndex == 0))
    {
        ent->asset->baseFlags |= 0x2;
        ent->baseFlags |= 0x2;
    }

    ent->anim = animData;
    ent->animTime = 0.0f;
    ent->timer = 0.0f;
    ent->fx_timer = 0.0f;
    ent->fx_par = NULL;
    ent->fx_emit = NULL;
    ent->fx_scale = 1.0f;
    ent->flyflags = 0;

    zEntReset(ent);

    ent->bound.sph.r = 1.0f;
}

void zEntPickup_Setup(_zEntPickup* p)
{
    zEntSetup(p);
    keyCounter = (_zCounter*)zSceneFindObject(xStrHash("KEY_COUNTER"));
}

void zEntPickup_Setup()
{
    xVec3* srcPos;
    _zEntPickup* p;
    xBase* base;
    zScene* zsc;
    U32 i;
    U32 j;

    zsc = globals.sceneCur;

    // Every pickup that something drops starts hidden at the dropper's position.
    for (i = 0; i < zsc->num_ents; i++)
    {
        base = zsc->ents[i];

        if (base)
        {
            for (j = 0; j < base->linkCount; j++)
            {
                if (base->link[j].dstEvent == eEventDrop)
                {
                    p = (_zEntPickup*)zSceneFindObject(base->link[j].dstAssetID);

                    if (p && p->baseType == 4 && p->state != 0x10)
                    {
                        srcPos = &p->asset->pos;

                        switch (base->baseType)
                        {
                        case 2:
                            p->flg_opts |= 0x1;
                            p->flg_opts |= 0x2;
                            p->useThisEntPos = (_xEnt*)base;
                        case 0xB:
                        case 0xC:
                        case 0x18:
                        case 0x1B:
                        case 0x23:
                            srcPos = &((_xEnt*)base)->asset->pos;
                        }

                        zEntPickup_MakeDroppable(p, srcPos, NULL);
                    }
                }
            }
        }
    }

    sEmitGlow = zParEmitterFind("PAREMIT_PICKUP_GLOW");
    if (sEmitGlow && sEmitGlow->group)
    {
        xParGroupSetStealing(sEmitGlow->group, 0);
    }

    sEmitMmm = zParEmitterFind("PAREMIT_PICKUP_MMM");
    sEmitQMark = zParEmitterFind("PAREMIT_PICKUP_QMARK");

    sEmitBubble = zParEmitterFind("PAREMIT_PICKUP_BUBBLE");
    if (sEmitBubble && sEmitBubble->group)
    {
        xParGroupSetStealing(sEmitBubble->group, 0);
    }

    sEmitGum = zParEmitterFind("PAREMIT_PICKUP_GUM");
    if (sEmitGum && sEmitGum->group)
    {
        xParGroupSetStealing(sEmitGum->group, 0);
    }

    sEmitOdour = NULL;
    sEmitGrabPowerup = zParEmitterFind("PAREMIT_PICKUP_SUPER");
    sEmitGrabCrumb = zParEmitterFind("PAREMIT_GRAB_CRUMB");
    sEmitGrabSnackBox = NULL;
    sEmitGrabMmm = zParEmitterFind("PAREMIT_GRAB_MMM");
    sEmitGrabQMark = zParEmitterFind("PAREMIT_GRAB_QMARK");
    sEmitGrabGum = zParEmitterFind("PAREMIT_GRAB_GUM");
    sEmitGrabBubbles = zParEmitterFind("PAREMIT_GRAB_BUBBLES");
    sEmitGrabGhost = zParEmitterFind("PAREMIT_GRAB_GHOST");
    sEmitGrabSDLogo = NULL;
    sEmitWarpGate = zParEmitterFind("PAREMIT_WARP");
    sEmitMiniVanPuff = zParEmitterFind("PAREMIT_SMALL_VAN_PUFF");
    sEmitLoveHart = zParEmitterFind("PAREMIT_LOVEHART");
    sEmitAkmalDaisy = zParEmitterFind("PAREMIT_DAISY");

    zEntPickup_resetKeyShake();
}

S32 zEntPickupEventCB(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    _zEntPickup* p = (_zEntPickup*)to;

    switch (toEvent)
    {
    case eEventVisible:
        if (!(p->state & 0x8))
        {
            xEntShow(p);
        }
        break;
    case eEventInvisible:
        xEntHide(p);
        break;
    case eEventReset:
        zEntPickup_Reset(p);
        break;
    case eEventPickup:
        if (p->p->pickupType == 2 &&
            !(globals.player.Inv_Specials & (1 << p->p->pickupIndex)))
        {
            globals.player.Inv_Specials |= 1 << p->p->pickupIndex;
            globals.player.SpecialReceived = p->p->pickupIndex;
        }
        break;
    case eEventDrop:
        if (p->state == 0x10)
        {
            zEntPickup_Drop(p);
        }
        break;
    case eEventFollowTarget:
        p->followTarget = toParamWidget;
        p->followOffset.x = toParam[0];
        p->followOffset.y = toParam[1];
        p->followOffset.z = toParam[2];
        break;
    }

    return 1;
}

void zEntPickup_hideSnackGateInfo(_zEntPickup* ent)
{
    if (ent->snackGateInfoDisplayed != -1)
    {
        zEntEvent("MNU4 SNACKGATE PROMPT", eEventUIFocusOff);
        ent->snackGateInfoDisplayed = -1;
    }
}

void zEntPickup_Reset(_zEntPickup* ent)
{
    zEntReset(ent);

    ent->state = 0x41;
    ent->animTime = 0.0f;
    ent->timer = 0.0f;
    ent->vel.x = ent->vel.y = ent->vel.z = 0.0f;
    ent->bound.type = 1;
    ent->shake_timer = 0.0f;
    ent->fx_timer = 0.0f;
    ent->fx_par = NULL;
    ent->fx_emit = NULL;
    ent->fx_scale = 1.0f;
    ent->flyflags = 0;

    zEntPickup_hideWarpGateInfo();
    zEntPickup_hideSnackGateInfo(ent);
    zEntPickup_resetKeyShake();
}

static void zEntPickup_resetKeyShake()
{
    S32 i;

    for (i = 0; i < 4; i++)
    {
        sKeyShake[i].shake_timer = 0.0f;
        sKeyShake[i].ui = NULL;
    }

    sKeyCounterEnabledBeforeGive = 0;
}

static void zEntPickup_hideWarpGateInfo()
{
    if (zHudShowWarpGateInfo != -1)
    {
        zEntEvent("MNU4 WARP PROMPT", eEventInvisible);
        zEntEvent("MNU4 WARP NAME", eEventInvisible);
        zHudShowWarpGateInfo = -1;
    }
}

void zEntPickup_Save(_zEntPickup* ent, xSerial* s)
{
    zEntSave(ent, s);

    s->Write_b7(ent->state);
}

void zEntPickup_Load(_zEntPickup* ent, xSerial* s)
{
    S32 is_a_dropper = 0;

    zEntLoad(ent, s);

    if (ent->state & 0x10)
    {
        is_a_dropper = 1;
    }

    s->Read_b7(&ent->state);

    if (is_a_dropper && (ent->state & 0x23))
    {
        if (ent->dropParent && ent->dropParent->baseType == 0xB &&
            !(ent->dropParent->baseFlags & 0x2) && (((_zEntSimpleObj*)ent->dropParent)->sflags & 0x1))
        {
            printf("Put back  %f %f %f    %f %f %f\n", ent->model->Mat->pos.x,
                   ent->model->Mat->pos.y, ent->model->Mat->pos.z, ent->droppos.x, ent->droppos.y,
                   ent->droppos.z);

            ent->state = 0x10;

            xEntHide(ent);
        }
        else
        {
            xVec3Copy((xVec3*)&ent->model->Mat->pos, &ent->droppos);
        }
    }
}

void zEntPickup_MakeDroppable(_zEntPickup* ent, _xVec3* pos, xBase* parent)
{
    ent->droppos = *(xVec3*)&ent->model->Mat->pos;
    *(xVec3*)&ent->model->Mat->pos = *pos;
    ent->state = 0x10;

    xEntHide(ent);

    ent->dropParent = parent;
}

void zEntPickup_Drop(_zEntPickup* ent)
{
    ent->state = 0xC2;

    xEntShow(ent);

    if ((ent->flg_opts & 0x2) && ent->useThisEntPos)
    {
        xVec3Copy(&ent->droppos, xEntGetPos(ent->useThisEntPos));
        xVec3Copy((xVec3*)&ent->model->Mat->pos, xEntGetPos(ent->useThisEntPos));
    }

    F32 b = sqrtf(75.0f);
    F32 c = ent->model->Mat->pos.y - ent->droppos.y;

    ent->vel.x = 0.0f;
    ent->vel.y = b;
    ent->vel.z = 0.0f;

    if (!(ent->flg_opts & 0x1))
    {
        F32 sol[2];
        U32 solcnt = xMathSolveQuadratic(-7.5f, b, c, &sol[0], &sol[1]);

        if (solcnt)
        {
            if (solcnt == 2 && sol[1] > sol[0])
            {
                sol[0] = sol[1];
            }

            if (sol[0] > 0.0001f)
            {
                sol[0] = 1.0f / sol[0];

                ent->vel.x = sol[0] * (ent->droppos.x - ent->model->Mat->pos.x);
                ent->vel.z = sol[0] * (ent->droppos.z - ent->model->Mat->pos.z);
            }
        }
    }

    ent->timer = 1.2f;
}

void enableWarpPoint(U32 index)
{
    zWarpPointTable[index].status = 1;
}

U32 zEntPickup_WarpIsOpen(U32 index)
{
    return zWarpPointTable[index].status;
}

U32 zEntPickup_lastSnackInArea()
{
    if (zSceneGetAreaSnacksCollected(globals.sceneCur->sceneID) ==
        zSceneGetAreaConstTotalSnacks(globals.sceneCur->sceneID))
    {
        return 1;
    }

    return 0;
}

void zEntPickup_UnlockSnackGate(_xEnt* ent)
{
    _zParEmitterCustomSettings info;
    _zEntPickup* pickup = (_zEntPickup*)ent;

    pickup->state = (pickup->state & ~0x3F) | 0x4;
    zEntEvent(ent, ent, eEventPickup);
    zEntPickup_hideSnackGateInfo(pickup);

    xSndPlay(xStrHash("snackgate_activate"), 0.5f, 0.5f, 0.0f, 0x7F, 0, 0);

    if (sEmitDust)
    {
        info.custom_flags = 0x20;
        xVec3Copy(&info.pos, xBoundCenter(&ent->bound));
        zParEmitterEmitCustom(sEmitDust, 1.0f / 30.0f, &info);
    }

    zEntEvent("MNU4 SNACKGATE UNLOCKED PROMPT", eEventUIFocusOn);
    zEntPlayer_Notify(5);
}

void zEntPickupCheat_allWarps()
{
    U32 i;

    for (i = 0; i < sizeof(zWarpPointTable) / sizeof(zWarpPointTable[0]) - 1; i++)
    {
        zWarpPointTable[i].status = 1;
    }
}

void zEntPickupResetWarps()
{
    U32 i;

    // The last entry is the terminator.
    for (i = 0; i < sizeof(zWarpPointTable) / sizeof(zWarpPointTable[0]) - 1; i++)
    {
        zWarpPointTable[i].status = 0;
    }

    // Record 10 (H001) starts open.
    zWarpPointTable[10].status = 1;
}

void zEntPickup_SceneUpdate(F32 dt)
{
    sKeysOffTimer -= dt;
}
