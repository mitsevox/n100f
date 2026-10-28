#include "zEntTrigger.h"
#include "xEvent.h"
#include "zScene.h"
#include "iMath3.h"

void zEntTriggerUpdate(zEntTrigger* trig, _xScene* scene, F32 dt);
S32 zEntTriggerEventCB(xBase* from, xBase* to, U32 toEvent, F32* param, xBase* widget);
void zEntReset(_zEnt* ent);

// Set up the trigger's update and event handlers and its links.
void zEntTriggerInit(void* ent, void* asset)
{
    zEntTrigger* trig = (zEntTrigger*)ent;
    zEntInit(trig, (p2EntAsset*)asset, 'TRIG');
    trig->move = NULL;
    trig->update = (void (*)(_xEnt*, _xScene*, F32))zEntTriggerUpdate;
    trig->eventFunc = zEntTriggerEventCB;
    if (trig->linkCount)
    {
        trig->link = (p2LinkAsset*)((p2EntTriggerAsset*)(trig->asset + 1) + 1);
    }
    else
    {
        trig->link = NULL;
    }
}

// Track link transitions as their checked objects enter or leave the trigger.
void zEntTriggerUpdate(zEntTrigger* trig, _xScene*, F32)
{
    if (!(trig->baseFlags & 1))
        return;

    p2LinkAsset* link = trig->link;
    p2EntTriggerAsset* tasset = (p2EntTriggerAsset*)(trig->asset + 1);
    for (U32 i = 0; i < trig->linkCount; ++i, ++link)
    {
        if (!link->chkAssetID || link->srcEvent < 212 || link->srcEvent > 215)
            continue;

        S32 collide = 0;
        _xEnt* checked = (_xEnt*)zSceneFindObject(link->chkAssetID);
        if (checked)
        {
            xVec3* pos;
            if (checked->bound.type == 1)
                pos = &checked->bound.sph.center;
            else
                pos = (xVec3*)&checked->model->Mat->pos;

            switch (trig->subType)
            {
            case 0:
            {
                _xBox box;
                box.upper = tasset->p[1];
                box.lower = tasset->p[0];
                _xIsect isect;
                iBoxIsectVec(&box, pos, &isect);
                collide = isect.penned <= 0.0f;
                break;
            }
            case 1:
            {
                xSphere sphere;
                sphere.center = tasset->p[0];
                sphere.r = tasset->p[1].x;
                _xIsect isect;
                iSphereIsectVec(&sphere, pos, &isect);
                collide = isect.penned <= 0.0f;
                break;
            }
            case 2:
            {
                _xCylinder cylinder;
                cylinder.center = tasset->p[0];
                cylinder.r = tasset->p[1].x;
                cylinder.h = tasset->p[1].y;
                _xIsect isect;
                iCylinderIsectVec(&cylinder, pos, &isect);
                collide = isect.penned <= 0.0f;
                break;
            }
            case 3:
            {
                xSphere sphere;
                sphere.center = tasset->p[0];
                sphere.r = tasset->p[1].x;
                _xIsect isect;
                iSphereIsectVec(&sphere, pos, &isect);
                collide = isect.penned <= 0.0f;
                break;
            }
            }
        }

        if (collide)
        {
            if (link->srcEvent == 212)
            {
                link->srcEvent = 214;
                xBase* widget = zSceneFindObject(link->paramWidgetAssetID);
                xBase* to = zSceneFindObject(link->dstAssetID);
                zEntEvent(trig, 212, to, link->dstEvent, link->param, widget, 0);
            }
            if (link->srcEvent == 213)
                link->srcEvent = 215;
        }
        else
        {
            if (link->srcEvent == 215)
            {
                link->srcEvent = 213;
                xBase* widget = zSceneFindObject(link->paramWidgetAssetID);
                xBase* to = zSceneFindObject(link->dstAssetID);
                zEntEvent(trig, 213, to, link->dstEvent, link->param, widget, 0);
            }
            if (link->srcEvent == 214)
                link->srcEvent = 212;
        }
    }
}

// Reset the trigger when it receives a reset event.
S32 zEntTriggerEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    switch (toEvent)
    {
    case eEventReset:
        zEntReset((zEntTrigger*)to);
        break;
    }
    return 1;
}

// Save the trigger's entity state.
void zEntTriggerSave(zEntTrigger* ent, xSerial* s)
{
    zEntSave(ent, s);
}

// Restore the trigger's entity state.
void zEntTriggerLoad(zEntTrigger* ent, xSerial* s)
{
    zEntLoad(ent, s);
}
