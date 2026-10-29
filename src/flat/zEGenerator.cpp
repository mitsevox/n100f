#include "zEGenerator.h"

#include "xAnim.h"
#include "xEvent.h"
#include "xModel.h"
#include "zLightning.h"
#include "zScene.h"

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
        for (S32 i = 0; i < 2; i++)
        {
            if (egen->lfx[i])
                zLightningModifyEndpoints(egen->lfx[i], &egen->src_pos, &egen->dst_pos);
        }
    }
    if (egen->tmr > 0.0f)
    {
        egen->tmr -= dt;
        if (egen->tmr <= 0.0f)
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
            zEntEvent(egen, eEventOff);
        }
    }
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
