#include "zGroup.h"
#include "xEvent.h"
#include "iMath.h"
#include "zEnt.h"
#include "zScene.h"

// Groups list the ids of their members after the asset header; an event sent to a group goes to
// every member, or to one random member when groupFlags bit 0 is set.
void zGroupInit(void* b, void* asset)
{
    xBaseInit((_zGroup*)b, (zGroupAsset*)asset);
    ((_zGroup*)b)->eventFunc = zGroupEventCB;
    ((_zGroup*)b)->asset = (zGroupAsset*)asset;
    if (((_zGroup*)b)->linkCount)
    {
        ((_zGroup*)b)->link =
            (p2LinkAsset*)((U8*)((_zGroup*)b)->asset + sizeof(zGroupAsset) +
                           ((zGroupAsset*)asset)->itemCount * sizeof(U32));
    }
    else
    {
        ((_zGroup*)b)->link = NULL;
    }
}

void zGroupSave(_zGroup* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void zGroupLoad(_zGroup* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

S32 zGroupEventCB(xBase*, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget)
{
    _zGroup* g = (_zGroup*)to;

    if (toEvent == eEventReset)
    {
        xBaseReset(g, g->asset);
    }
    else if (toEvent == eEventDisableGroupContents)
    {
        toEvent = eEventDisable;
    }

    S32 rand = -1;
    if (g->asset->groupFlags & 1)
    {
        rand = irand() % g->asset->itemCount;
    }

    U32* item = (U32*)(g->asset + 1);
    for (S32 i = 0; i < g->asset->itemCount; i++)
    {
        if (rand == -1 || rand == i)
        {
            xBase* b = zSceneFindObject(item[i]);
            if (b)
            {
                zEntEvent(b, toEvent, toParam, toParamWidget);
            }
            else
            {
                zSceneGetName(g);
            }
        }
    }

    return 1;
}

U32 zGroupGetCount(_zGroup* g)
{
    return g->asset->itemCount;
}

xBase* zGroupGetItemPtr(_zGroup* g, U32 index)
{
    return zSceneFindObject(zGroupGetItem(g, index));
}

U32 zGroupGetItem(_zGroup* g, U32 index)
{
    return ((U32*)(g->asset + 1))[index];
}
