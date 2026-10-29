#include "zEGenerator.h"

#include "xAnim.h"
#include "xEvent.h"
#include "xModel.h"
#include "xMovePoint.h"
#include "iMath.h"
#include "zGlobals.h"

#include <string.h>
#include "zLightning.h"
#include "zScene.h"
#include "xstransvc.h"

struct zEGenAsset : p2EntAsset
{
    xVec3 src_dpos;
    U8 damage_type;
    U8 flags;
    F32 ontime;
    U32 onAnimID;
};

// Stop the animation and remove both arcs.
inline void zEGenerator_TurnOff(zEGenerator* egen)
{
    egen->flags &= ~1;
    if (egen->afile)
        egen->model->Anim->Single->CurrentSpeed = 0.0f;
    for (S32 i = 0; i < 2; i++)
    {
        if (egen->lfx[i])
        {
            zLightningKill(egen->lfx[i]);
            egen->lfx[i] = 0;
        }
    }
}

// Start an arc at a randomly selected destination.
void zEGenerator_TurnOn(zEGenerator* egen)
{
    egen->flags |= 1;
    zEGenAsset* zasset = (zEGenAsset*)egen->asset;
    if (egen->afile)
        egen->model->Anim->Single->CurrentSpeed = 1.0f;
    egen->tmr = zasset->ontime;
    xVec3Add(&egen->src_pos, &xModelGetFrame(egen->model)->pos, &zasset->src_dpos);
    if (egen->num_dsts)
    {
        U16 itgt = irand() % egen->num_dsts;
        U16 imp = 0;
        for (S32 i = 0; i < egen->linkCount; i++)
        {
            p2LinkAsset* link = &egen->link[i];
            xBase* b = zSceneFindObject(link->dstAssetID);
            if (b && link->dstEvent == 0xC0)
            {
                if (imp == itgt)
                {
                    if (b->baseType == 0xD)
                    {
                        egen->dst_pos = *xMovePointGetPos((xMovePoint*)b);
                        egen->dst = b;
                        break;
                    }
                    if (b->baseType == 6)
                    {
                        egen->dst_pos = *(xVec3*)&((_xEnt*)b)->model->Mat->pos;
                        egen->dst = b;
                        break;
                    }
                }
                else
                    imp++;
            }
        }
    }
    for (S32 i = 0; i < 2; i++)
    {
        if (egen->lfx[i])
        {
            zLightningKill(egen->lfx[i]);
            egen->lfx[i] = 0;
        }
    }
    _tagLightningAdd add;
    memset(&add, 0, sizeof(add));
    add.type = eLT_Line;
    add.total_points = 16;
    add.start = &egen->src_pos;
    add.end = &egen->dst_pos;
    add.thickness = 0.15f;
    add.r = 255;
    add.g = 255;
    add.b = 255;
    add.rand_radius = 5.0f;
    add.flags = 16;
    if (globals.sceneCur->sceneID == zSceneIDFromName("G009"))
    {
        add.thickness = 1.0f;
        add.r = 85;
        add.g = 243;
        add.b = 85;
        add.rand_radius = 10.0f;
    }
    egen->lfx[0] = zLightningAdd(&add);
    add.type = eLT_Rotating;
    add.setup_degrees = 20.0f + 90.0f * iurand();
    add.move_degrees = 180.0f + 360.0f * iurand();
    if (irand() & 1)
        add.move_degrees = -add.move_degrees;
    add.rot_radius = 0.5f;
    add.total_points = 16;
    add.thickness = 0.1f;
    add.r = 80;
    add.g = 80;
    add.b = 255;
    add.rand_radius = 3.0f;
    add.flags = 24;
    if (globals.sceneCur->sceneID == zSceneIDFromName("G009"))
    {
        add.thickness = 0.5f;
        add.r = 20;
        add.g = 243;
        add.b = 20;
        add.rot_radius = 1.5f;
    }
    egen->lfx[1] = zLightningAdd(&add);
}

// Render the generator through the ordinary entity renderer.
void zEGenerator_Render(zEGenerator* egen)
{
    xEntRender(egen);
}

// Keep a moving destination and the lightning endpoints together.
void zEGenerator_Update(zEGenerator* egen, _xScene* sc, F32 dt)
{
    xEntUpdate(egen, sc, dt);
    if (egen->dst && egen->dst->baseType == 6)
    {
        egen->dst_pos = *(xVec3*)&((_xEnt*)egen->dst)->model->Mat->pos;
        if (egen->lfx[0])
            zLightningModifyEndpoints(egen->lfx[0], &egen->src_pos, &egen->dst_pos);
        if (egen->lfx[1])
            zLightningModifyEndpoints(egen->lfx[1], &egen->src_pos, &egen->dst_pos);
    }
    if (egen->tmr > 0.0f)
    {
        egen->tmr -= dt;
        if (egen->tmr <= 0.0f)
        {
            zEGenerator_TurnOff(egen);
            zEntEvent(egen, eEventOff);
        }
    }
}

// Restore the generator's starting state from its asset.
void zEGenerator_Reset(zEGenerator* egen, _xScene*)
{
    zEntReset(egen);
    zEGenAsset* zasset = (zEGenAsset*)egen->asset;
    egen->flags = zasset->flags;
    if (egen->flags & 1)
        zEGenerator_TurnOn(egen);
    else
    {
        zEGenerator_TurnOff(egen);
    }
    if (zasset->moreFlags & 4)
        egen->chkby = 0;
    else
        egen->chkby = 0x18;
}

// Restore the shared entity state.
void zEGenerator_Load(zEGenerator* egen, xSerial* s)
{
    zEntLoad(egen, s);
}

// Save the shared entity state.
void zEGenerator_Save(zEGenerator* egen, xSerial* s)
{
    zEntSave(egen, s);
}

// Count valid destination links for later selection.
void zEGenerator_Setup(zEGenerator* egen, _xScene*)
{
    zEntSetup(egen);
    egen->num_dsts = 0;
    for (S32 i = 0; i < egen->linkCount; i++)
    {
        p2LinkAsset* link = &egen->link[i];
        xBase* dst = zSceneFindObject(link->dstAssetID);
        if (dst && link->dstEvent == 0xC0)
        {
            if (dst->baseType == 0xD)
                egen->num_dsts++;
            else if (dst->baseType == 6)
                egen->num_dsts++;
        }
    }
}

// Dispatch generator and visibility events.
S32 zEGeneratorEventCB(xBase*, xBase* to, U32 toEvent, F32*, xBase*)
{
    zEGenerator* egen = (zEGenerator*)to;
    switch (toEvent)
    {
    case eEventOn:
        zEGenerator_TurnOn(egen);
        break;
    case eEventOff:
        zEGenerator_TurnOff(egen);
        break;
    case eEventToggle:
        if (egen->flags & 1)
        {
            zEGenerator_TurnOff(egen);
        }
        else
            zEGenerator_TurnOn(egen);
        break;
    case eEventReset:
        zEGenerator_Reset(egen, 0);
        break;
    case eEventVisible:
        egen->_xEnt::flags |= 1;
        break;
    case eEventInvisible:
        egen->_xEnt::flags &= ~1;
        break;
    case eEventCollision_Visible_On:
        egen->_xEnt::flags |= 1;
    case eEventCollisionOn:
        egen->chkby = 0x18;
        break;
    case eEventCollision_Visible_Off:
        egen->_xEnt::flags &= ~1;
    case eEventCollisionOff:
        egen->chkby = 0;
        break;
    }
    return 1;
}

// Initialize the entity and its optional on animation.
void zEGenerator_Init(zEGenerator* egen, p2EntAsset* asset)
{
    zEntInit(egen, asset, 'EGEN');
    zEGenAsset* zasset = (zEGenAsset*)asset;
    if (egen->linkCount)
        egen->link = (p2LinkAsset*)(zasset + 1);
    else
        egen->link = 0;
    egen->update = (void (*)(_xEnt*, _xScene*, F32))zEGenerator_Update;
    egen->move = 0;
    egen->eventFunc = zEGeneratorEventCB;
    egen->render = (void (*)(_xEnt*))zEGenerator_Render;
    egen->afile = 0;
    if (zasset->onAnimID)
    {
        U32 size;
        void* buf = xSTFindAsset(zasset->onAnimID, &size);
        if (buf)
        {
            egen->afile = xAnimFileNew(buf, "", 0, 0);
            egen->atbl = xAnimTableNew("", 0);
            xAnimTableNewState(egen->atbl, "On", 0x10, 0, 1.0f, 0, 0, 0.0f, 0);
            xAnimTableAddFile(egen->atbl, egen->afile, "On");
            xAnimPoolAlloc(&globals.sceneCur->mempool, egen, egen->atbl, egen->model);
        }
    }
}
