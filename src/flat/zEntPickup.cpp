#include "zEntPickup.h"

#include "iAnim.h"
#include "iModel.h"
#include "iShadow.h"
#include "libc/math.h"
#include "libc/stdio.h"
#include "xBound.h"
#include "xEvent.h"
#include "xMath.h"
#include "xMath3.h"
#include "xModel.h"
#include "xPar.h"
#include "xParGroup.h"
#include "xserializer.h"
#include "xSnd.h"
#include "xstransvc.h"
#include "xString.h"
#include "zCounter.h"
#include "zEntPlayer.h"
#include "zGlobals.h"
#include "zParEmitter.h"
#include "zEntSimpleObj.h"
#include "zGameExtras.h"
#include "zParSys.h"
#include "zScene.h"

struct zUIAsset : p2EntAsset
{
    U32 uiFlags;
    U16 dim[2];
    U32 textureID;
    F32 uva[2];
    F32 uvb[2];
    F32 uvc[2];
    F32 uvd[2];
};

struct _zUI : _zEnt
{
    zUIAsset* sasset;
    U32 uiFlags;
    U32 uiButton;
};

enum _tagGiveFXType
{
    GFXT_Unknown,
    GFXT_None,
    GFXT_Crumbs,
    GFXT_Mmmm,
    GFXT_QMark,
    GFXT_Gum,
    GFXT_Bubbles,
    GFXT_Ghost,
    GFXT_SDLogo,
    GFXT_PowerupSpecial
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
void zSceneGameStatsTally_token();
xMat4x3* xCameraGetCurMatrix(_tagxCamFrame* cam);
F32 xFilterCurvedInterp2(F32 in_t);
void zHud_showSnack();
void zHud_showHealthandStatus();
void zEntPlayer_GiveScoobySnack(U32 quantity);
S32 zEntPlayer_HasScoobySnack(U32 quantity);
void zEntPlayer_TakeScoobySnack(_xEnt* ent, U32 quantity);
void zEntPlayer_GiveClue(U32 quantity);
void zEntPlayer_GiveChance(U32 quantity);
void zEntPlayer_GiveHealth(U32 quantity);
void zEntPlayer_GiveHotsauce(U32 value);

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

// Gives the player what a pickup holds, by type and index in the pickup table. Returns 0 when
// nothing was taken (full health, ammo at its limit, too few snacks to pay), and for a clue,
// which plays no pickup effect.
S32 zPickup_Give(_zEntPickup* ent, zAssetPickup* p, U16 value)
{
    switch (p->pickupType)
    {
    case 1:
        switch (p->pickupIndex)
        {
        case 7:
            zEntPlayer_GiveClue(p->quantity);
            return 0;
        case 5:
            if (keyCounter && (keyCounter->baseFlags & 0x1))
            {
                zEntPickup_ShowKeyCnt(keyCounter->asset->count - keyCounter->count,
                                      keyCounter->asset->count);
            }
            break;
        case 4:
            zHud_showSnack();
            zEntPlayer_GiveScoobySnack(5);
            zEntPlayer_Notify(0x24);
            break;
        case 0:
            zEntPlayer_GiveScoobySnack(p->quantity);
            break;
        case 1:
            if (globals.player.Health >= 5)
            {
                zHud_showHealthandStatus();
                return 0;
            }
            zEntPlayer_GiveHealth(p->quantity);
            zEntPlayer_Notify(0x22);
            break;
        case 2:
            zEntPlayer_GiveChance(p->quantity);
            break;
        case 3:
            zEntPlayer_GiveHotsauce(value);
            break;
        case 9:
            if (!zEntPlayer_HasScoobySnack(value))
            {
                return 0;
            }
            zEntPlayer_TakeScoobySnack(ent, value);
            break;
        }
        break;
    case 2:
        globals.player.Inv_Specials |= 1 << p->pickupIndex;
        globals.player.SpecialReceived = p->pickupIndex;
        break;
    case 3:
        if (globals.player.Inv_Ammo[p->pickupIndex] >= globals.player.Inv_AmmoMax[p->pickupIndex])
        {
            return 0;
        }
        globals.player.Inv_Ammo[p->pickupIndex] =
            CLAMP(globals.player.Inv_Ammo[p->pickupIndex] + (S32)p->quantity, 0,
                  globals.player.Inv_AmmoMax[p->pickupIndex]);
        break;
    case 5:
        globals.player.Inv_AmmoMax[p->pickupIndex] += p->quantity;
        globals.player.Inv_Ammo[p->pickupIndex] =
            CLAMP(globals.player.Inv_Ammo[p->pickupIndex] + (S32)p->quantity, 0,
                  globals.player.Inv_AmmoMax[p->pickupIndex]);
        break;
    case 4:
        zSceneGameStatsTally_token();
        globals.player.Inv_Mtokens[p->pickupIndex >> 5] |= 1 << (p->pickupIndex & 31);
        break;
    }

    zEntPickup_GiveFX(ent);
    return 1;
}

// Moves a grabbed pickup from where it was grabbed to a point in front of the camera, by the
// HUD: a key flies to its own slot in the key row, everything else to the left. The pickup's
// effect shrinks on the way.
void zEntPickup_FlyToInterface(_zEntPickup* ent, F32)
{
    F32 scale;
    xVec3* src;
    F32 perc;
    xMat4x3 dest;
    xVec3 delta;

    dest = *xCameraGetCurMatrix(&globals.camera);

    if (ent->p->pickupIndex == 5)
    {
        xMat4x3MoveLocalUp(&dest, 0.225f);

        switch (keyCounter->asset->count - keyCounter->count)
        {
        case 1:
            xMat4x3MoveLocalRight(&dest, 0.17f);
            break;
        case 2:
            xMat4x3MoveLocalRight(&dest, 0.08f);
            break;
        case 3:
            xMat4x3MoveLocalRight(&dest, -0.08f);
            break;
        case 4:
        default:
            xMat4x3MoveLocalRight(&dest, -0.17f);
            break;
        }

        xMat4x3MoveLocalAt(&dest, 0.5f);
    }
    else
    {
        xMat4x3MoveLocalUp(&dest, 0.225f);
        xMat4x3MoveLocalRight(&dest, -0.25f);
        xMat4x3MoveLocalAt(&dest, 0.5f);
    }

    perc = 1.0f - ent->timer / 0.4f;

    if (perc < 0.0f)
    {
        perc = 0.0f;
    }
    else if (perc > 1.0f)
    {
        perc = 1.0f;
    }

    perc = xFilterCurvedInterp2(perc);

    src = &ent->grab_pos;
    delta.x = dest.pos.x - src->x;
    delta.y = dest.pos.y - src->y;
    delta.z = dest.pos.z - src->z;
    ent->model->Mat->pos.x = src->x + delta.x * perc;
    ent->model->Mat->pos.y = src->y + delta.y * perc;
    ent->model->Mat->pos.z = src->z + delta.z * perc;
    scale = 0.9f * perc;
    ent->fx_scale = 1.0f - scale;

    ent->vel.x = 0.0f;
    ent->vel.y = 0.0f;
    ent->vel.z = 0.0f;
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

// Pickup effects run unless a cutscene is playing and has hidden them.
static S32 zEntPickup_CanShowFX(_zEntPickup* ent)
{
    if (!globals.cmgr)
    {
        return 1;
    }

    if (!(ent->flyflags & 0x2))
    {
        return 1;
    }

    return 0;
}

// Runs a pickup's particle effects: the exhaust puffs of the Mystery Machine pickup, and the
// glow, bubbles or gum that each pickup type gives off, re-emitted whenever fx_timer runs out.
// A timer of 999 means no effect.
static void zEntPickup_UpdateFX(_zEntPickup* ent, _xScene*, F32 dt)
{
    _zParEmitterCustomSettings info;
    _zParEmitterCustomSettings info2;
    S32 powerup = 0;
    U8 pupr;
    U8 pupg;
    U8 pupb;

    if (sEmitMiniVanPuff && ent->p->pickupType == 1 && ent->p->pickupIndex == 6)
    {
        if (zEntPickup_CanShowFX(ent))
        {
            info.custom_flags = 0x64;
            info.pos.x = ent->model->Mat->pos.x;
            info.pos.y = ent->model->Mat->pos.y;
            info.pos.z = ent->model->Mat->pos.z;
            info.pos.x += -0.35f * ent->model->Mat->right.x;
            info.pos.y += 0.05f;
            info.pos.z += -0.35f * ent->model->Mat->right.z;
            info.pos.x += 0.4f * ent->model->Mat->at.x;
            info.pos.z += 0.4f * ent->model->Mat->at.z;

            if (ent->state & 0x20)
            {
                info.count = 5;
            }
            else
            {
                info.count = 2;
            }

            info.vel.x = -0.2f * -ent->model->Mat->right.x;
            info.vel.y = 0.2f;
            info.vel.z = -0.2f * -ent->model->Mat->right.z;
            zParEmitterEmitCustom(sEmitMiniVanPuff, dt, &info);
        }
    }
    else if (!zEntPickup_CanShowFX(ent) && ent->fx_par)
    {
        if (!(ent->flyflags & 0x2))
        {
            ent->flyflags |= 0x2;
            xVec3Copy(&ent->fx_par->m_pos, (xVec3*)&ent->model->Mat->pos);
            ent->fx_par->m_pos.y += 10000.0f;
            return;
        }
    }
    else
    {
        ent->flyflags &= ~0x2;
    }

    if (999.0f == ent->fx_timer)
    {
        if (ent->fx_par)
        {
            xVec3Copy(&ent->fx_par->m_pos, (xVec3*)&ent->model->Mat->pos);
        }
        return;
    }

    ent->fx_timer -= dt;

    if (ent->fx_timer > 0.0f)
    {
        return;
    }

    info2.custom_flags = 0x1a0;
    pupr = 0x42;
    pupg = 0xb8;
    pupb = 0xfc;
    info2.pos.x = ent->model->Mat->pos.x;
    info2.pos.y = ent->model->Mat->pos.y;
    info2.pos.z = ent->model->Mat->pos.z;

    if (ent->p->pickupType == 1)
    {
        switch (ent->p->pickupIndex)
        {
        case 2:
        default:
            powerup = 1;
            break;
        case 5:
        case 6:
        case 7:
            powerup = 1;
            pupr = 0xff;
            pupg = 0x00;
            pupb = 0xff;
            break;
        case 0:
            ent->fx_timer = 999.0f;
            return;
        case 3:
        case 4:
            powerup = 1;
            pupr = 0xff;
            pupg = 0xff;
            pupb = 0x00;
            break;
        case 1:
            powerup = 1;
            pupr = 0xff;
            pupg = 0xff;
            pupb = 0x00;
            break;
        case 8:
        case 9:
            powerup = 0;
            break;
        }
    }
    else if (ent->p->pickupType == 2)
    {
        powerup = 1;
    }
    else if (ent->p->pickupType == 3 || ent->p->pickupType == 5)
    {
        if (ent->p->pickupIndex == 1)
        {
            ent->fx_timer = 0.3f * iurand() + 0.1f;
            info2.pos.y += 0.1f;
            ent->fx_par = zParEmitterEmitCustom(sEmitBubble, 1.0f / 30.0f, &info2);
            ent->fx_emit = sEmitBubble;
        }
        else
        {
            ent->fx_timer = 1.0f;
            ent->fx_par = zParEmitterEmitCustom(sEmitGum, 1.0f / 30.0f, &info2);
            ent->fx_emit = sEmitGum;
        }
    }
    else if (ent->p->pickupType == 4)
    {
        info2.custom_flags |= 0x600;
        powerup = 1;
        info2.size_death = 5.0f;
        info2.size_birth = 5.0f;
    }
    else
    {
        powerup = 1;
    }

    if (powerup && ent->fx_timer && sEmitGlow)
    {
        info2.color_birth[0] = pupr;
        info2.color_birth[1] = pupg;
        info2.color_birth[2] = pupb;
        info2.color_death[0] = pupr;
        info2.color_death[1] = pupg;
        info2.color_death[2] = pupb;
        ent->fx_timer = 999.0f;
        ent->fx_par = zParEmitterEmitCustom(sEmitGlow, 1.0f / 30.0f, &info2);
        ent->fx_emit = sEmitGlow;

        if (!ent->fx_par)
        {
            ent->fx_timer = 0.0f;
            ent->fx_emit = NULL;
        }
    }
}

// Draws the visible pickups at their effect scale, and gives the ones near the player a
// shadow that fades out between 8 and 10 units away.
void zEntPickup_RenderList(_zEntPickup* plist, U32 pcount)
{
    S32 i;
    RpAtomic* imodel;
    RwMatrix* imat;
    RwFrame* frame;
    RwV3d vec_scale;
    S32 alpha;
    F32 dist2;

    for (i = 0; i < (S32)pcount; plist++, i++)
    {
        if (!xEntIsVisible(plist))
        {
            continue;
        }

        imodel = plist->model->Data;
        imat = plist->model->Mat;

        if (!iModelCull(imodel, imat))
        {
            frame = RpAtomicGetFrame(imodel);
            frame->ltm = *imat;

            if (1.0f != plist->fx_scale)
            {
                vec_scale.x = plist->fx_scale;
                vec_scale.y = plist->fx_scale;
                vec_scale.z = plist->fx_scale;
                RwMatrixScale(&frame->ltm, &vec_scale, rwCOMBINEPRECONCAT);
            }

            RwMatrixUpdate(&frame->ltm);
            imodel->renderCallBack(imodel);
        }

        if (plist->state & 0x23)
        {
            dist2 = (plist->model->Mat->pos.x - globals.player.ent.model->Mat->pos.x) *
                        (plist->model->Mat->pos.x - globals.player.ent.model->Mat->pos.x) +
                    (plist->model->Mat->pos.z - globals.player.ent.model->Mat->pos.z) *
                        (plist->model->Mat->pos.z - globals.player.ent.model->Mat->pos.z);

            if (dist2 < 100.0f)
            {
                if (dist2 > 64.0f)
                {
                    alpha = 0.5f * (80.0f * (10.0f - sqrtf(dist2)));

                    if (alpha > 80)
                    {
                        alpha = 80;
                    }

                    if (alpha < 0)
                    {
                        alpha = 1;
                    }
                }
                else
                {
                    alpha = 80;
                }

                iShadowCollide(&plist->ishadow, plist, 0.667f, 1.0f);
                iShadowAdd(&plist->ishadow, alpha);
            }
        }
    }
}

// Plays the burst of particles for a grabbed pickup (daisies instead on Anna's birthday), the
// Scooby Doobie Doo line for specials and tokens, and takes the pickup's glow away.
void zEntPickup_GiveFX(_zEntPickup* ent)
{
    xVec3* pos;
    S32 kill_glow = 1;
    S32 pickup_type;
    S32 pickup_idx;
    _tagGiveFXType type;
    _zParEmitterCustomSettings info;
    _zParEmitterCustomSettings info2;

    pickup_type = ent->p->pickupType;
    pickup_idx = ent->p->pickupIndex;

    if (pickup_type == 2 || pickup_type == 4)
    {
        xSndPlay(xStrHash("Scooby_Doobie_Doo"), 0.5f, 0.5f, 0.0f, 0x7f, 0, 0);
    }

    pos = (xVec3*)&ent->model->Mat->pos;
    type = GFXT_Unknown;

    if (pickup_type == 1)
    {
        switch (pickup_idx)
        {
        case 0:
        case 3:
            type = GFXT_Crumbs;
            break;
        case 4:
            type = GFXT_None;
            break;
        case 1:
            type = GFXT_Mmmm;
            break;
        case 2:
            type = GFXT_SDLogo;
            break;
        case 6:
        case 7:
            kill_glow = 0;
            type = GFXT_QMark;
            break;
        case 5:
        case 8:
            type = GFXT_QMark;
            break;
        case 9:
            type = GFXT_QMark;
            break;
        }
    }
    else if (pickup_type == 2)
    {
        type = GFXT_PowerupSpecial;
    }
    else if (pickup_type == 3 || pickup_type == 5)
    {
        if (pickup_idx == 0)
        {
            type = GFXT_Gum;
        }
        else
        {
            type = GFXT_Bubbles;
        }
    }
    else
    {
        type = GFXT_Ghost;
    }

    info.custom_flags = 0x20;
    info.pos.x = pos->x;
    info.pos.y = pos->y;
    info.pos.z = pos->z;

    if (zGameExtraIsAnnaBday())
    {
        if (type != GFXT_None)
        {
            info2.custom_flags = 0x20;
            info2.pos.x = ent->model->Mat->pos.x;
            info2.pos.y = ent->model->Mat->pos.y;
            info2.pos.z = ent->model->Mat->pos.z;
            zParEmitterEmitCustom(sEmitAkmalDaisy, 1.0f / 30.0f, &info2);
        }
    }
    else
    {
        switch (type)
        {
        case GFXT_Unknown:
        case GFXT_None:
        case GFXT_Crumbs:
            break;
        case GFXT_Mmmm:
            if (sEmitGrabMmm)
            {
                zParEmitterEmitCustom(sEmitGrabMmm, 1.0f / 30.0f, &info);
            }
            break;
        case GFXT_QMark:
            if (sEmitGrabQMark)
            {
                zParEmitterEmitCustom(sEmitGrabQMark, 1.0f / 30.0f, &info);
            }
            break;
        case GFXT_Gum:
            if (sEmitGrabGum)
            {
                zParEmitterEmitCustom(sEmitGrabGum, 1.0f / 30.0f, &info);
            }
            break;
        case GFXT_Bubbles:
            if (sEmitGrabBubbles)
            {
                zParEmitterEmitCustom(sEmitGrabBubbles, 1.0f / 30.0f, &info);
            }
            break;
        case GFXT_Ghost:
            if (sEmitGrabGhost)
            {
                zParEmitterEmitCustom(sEmitGrabGhost, 1.0f / 30.0f, &info);
            }
            break;
        case GFXT_SDLogo:
            if (sEmitGrabSDLogo)
            {
                zParEmitterEmitCustom(sEmitGrabSDLogo, 1.0f / 30.0f, &info);
            }
            break;
        case GFXT_PowerupSpecial:
            if (sEmitGrabPowerup)
            {
                zParEmitterEmitCustom(sEmitGrabPowerup, 1.0f / 30.0f, &info);
            }
            break;
        }
    }

    if (ent->fx_par && kill_glow)
    {
        xParGroupKillPar(((zParSys*)ent->fx_emit->parSys)->group, ent->fx_par);
    }
}

void enableWarpPoint(U32 index)
{
    zWarpPointTable[index].status = 1;
}

U32 zEntPickup_WarpIsOpen(U32 index)
{
    return zWarpPointTable[index].status;
}

// Moves every pickup that is flying to the HUD, then rattles the key icons that are shaking:
// each step flips the offset's side and shrinks it to 85% around the icon's own position,
// until the shake timer runs out.
void zEntPickup_UpdateFlyToInterface(_zEntPickup* ent, U32 pcount, F32 dt)
{
    S32 i;
    xVec3* ent_pos;
    F32 xdist;
    F32 ydist;

    for (i = 0; i < (S32)pcount; ent++, i++)
    {
        if ((ent->state & 0x4) && (ent->flyflags & 0x1))
        {
            zEntPickup_FlyToInterface(ent, dt);
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (sKeyShake[i].shake_timer > 0.0f)
        {
            sKeyShake[i].shake_timer -= dt;

            if (sKeyShake[i].shake_timer <= 0.0f)
            {
                sKeyShake[i].shake_timer = 0.0f;
            }

            if (sKeyShake[i].ui)
            {
                ent_pos = &sKeyShake[i].ui->sasset->pos;
                sKeyShake[i].shake_it = -0.85f * sKeyShake[i].shake_it;
                xdist = sKeyShake[i].shake_it;
                ydist = xdist;

                if (irand() & 1)
                {
                    ydist = -xdist;
                }

                ent_pos->x = xdist + sKeyShake[i].orig_pos.x;
                ent_pos->y = ydist + sKeyShake[i].orig_pos.y;
            }
        }
    }
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

// Lights the first `collected` of `total` key icons on the HUD and turns the rest off; a
// full set starts the timer that later hides them.
void zEntPickup_ShowKeyCnt(U32 collected, U32 total)
{
    S32 i;
    char s[64];

    for (i = 1; i <= (S32)total; i++)
    {
        if (i > (S32)collected)
        {
            sprintf(s, "KEY OFF %d UI", i);
            zEntEvent(s, eEventVisible);
            sprintf(s, "KEY ON %d UI", i);
            zEntEvent(s, eEventInvisible);
        }
        else
        {
            sprintf(s, "KEY OFF %d UI", i);
            zEntEvent(s, eEventInvisible);
            sprintf(s, "KEY ON %d UI", i);
            zEntEvent(s, eEventVisible);
        }
    }

    if (collected == total)
    {
        sKeysOffTimer = 1.5f;
    }
}

void zEntPickup_SceneUpdate(F32 dt)
{
    sKeysOffTimer -= dt;
}
