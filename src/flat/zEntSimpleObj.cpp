#include "zEntSimpleObj.h"

#include "xEvent.h"
#include "zEntPickup.h"
#include "zPlatform.h"
#include "zScene.h"

struct p2SimpleObjAsset
{
    F32 animSpeed;
    U32 initAnimState;
    U8 collType;
    U8 flags;
};

void zPlatform_MakeDiggable(_zPlatform* plat);

S32 zEntSimpleObjEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*);
void zEntSimpleObj_Init(_zEntSimpleObj* ent, p2EntAsset* asset);
static void DiggableSetup(_zEntSimpleObj* ent);

void zEntSimpleObj_Init(void* ent, void* asset)
{
    zEntSimpleObj_Init((_zEntSimpleObj*)ent, (p2EntAsset*)asset);
}

// A simple object ('SIMP'): an entity whose asset is followed by its p2SimpleObjAsset and then its
// links. Collision follows the asset's collision type.
void zEntSimpleObj_Init(_zEntSimpleObj* ent, p2EntAsset* asset)
{
    zEntInit(ent, asset, 'SIMP');

    ent->sasset = (p2SimpleObjAsset*)(asset + 1);
    ent->sflags = 0;
    ent->pflags = 0;
    ent->penby |= 0x10;

    if (ent->sasset->collType & 0x2)
    {
        ent->chkby = 0x18;
    }
    else
    {
        ent->chkby = 0;
    }

    ent->move = NULL;
    ent->eventFunc = zEntSimpleObjEventCB;

    if (ent->linkCount)
    {
        ent->link = (p2LinkAsset*)((p2SimpleObjAsset*)(ent->asset + 1) + 1);
    }
    else
    {
        ent->link = NULL;
    }

    ent->eventFunc = zEntSimpleObjEventCB;
    zEntReset(ent);
}

// The object's dig links: a pickup it is linked to on Digup becomes droppable from its position,
// and a linked platform becomes diggable. Marks the object as a dig spot when it has any.
static void DiggableSetup(_zEntSimpleObj* ent)
{
    U32 i;
    p2LinkAsset* curlink = ent->link;
    U32 dug = 0;

    for (i = 0; i < ent->linkCount; i++, curlink++)
    {
        if (curlink->srcEvent == eEventDigup)
        {
            xBase* tgt = zSceneFindObject(curlink->dstAssetID);

            switch (tgt->baseType)
            {
            case 4:
                zEntPickup_MakeDroppable((_zEntPickup*)tgt, &ent->asset->pos, ent);
                break;
            case 6:
                zPlatform_MakeDiggable((_zPlatform*)tgt);
                break;
            }

            dug = 1;
        }
    }

    if (dug)
    {
        ent->sflags |= 0x1;
    }
}

void zEntSimpleObj_Setup(_zEntSimpleObj* ent)
{
    zEntSetup(ent);
    DiggableSetup(ent);
}

void zEntSimpleObj_Save(_zEntSimpleObj* ent, xSerial* s)
{
    zEntSave(ent, s);
}

void zEntSimpleObj_Load(_zEntSimpleObj* ent, xSerial* s)
{
    zEntLoad(ent, s);
}

// Show, hide, collision, reset, animation and translucency events.
S32 zEntSimpleObjEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase*)
{
    _zEntSimpleObj* s = (_zEntSimpleObj*)to;

    switch (toEvent)
    {
    case eEventVisible:
        xEntShow(s);
        break;
    case eEventInvisible:
        xEntHide(s);
        break;
    case eEventCollision_Visible_On:
        xEntShow(s);
    case eEventCollisionOn:
        s->chkby = 0x18;
        break;
    case eEventCollision_Visible_Off:
        xEntHide(s);
    case eEventCollisionOff:
        s->chkby = 0;
        break;
    case eEventReset:
        zEntReset(s);
        if (s->sasset->collType & 0x2)
        {
            s->chkby = 0x18;
        }
        else
        {
            s->chkby = 0;
        }
        DiggableSetup(s);
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
    }

    return 1;
}
