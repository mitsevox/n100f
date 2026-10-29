#include "zFeet.h"
#include "xMemMgr.h"
#include "xAnim.h"
#include "rwcore.h"
#include "xString.h"
#include "xstransvc.h"
#include "zScene.h"
#include "zEntVillain.h"
#include "zParEmitter.h"
#include "zPitchChange.h"
#include "zSurface.h"
#include "xSnd.h"
#include "zGlobals.h"
#include <string.h>

struct _xEnt;
extern void* lbl_8025EEC0;
extern xBase* lbl_8025EEB8;
extern xBase* lbl_8025EEBC;
extern U32 lbl_8022C678[23];
extern _zParEmitter* lbl_802290A0[];
extern F32 lbl_801A4140[];
extern void zSurfaceGetName(S32, char*);
extern "C" xSurface* fn_800D2F34(const _xCollis*);
extern "C" void fn_80021B9C(const char*, void (*)());
extern void DrawSkidMarkStrip(_tagFeetSkidMark*, _tagSkidEdge*, U8, U8, U8);
extern U32 zFeetStepCB(U32, xAnimActiveEffect*, xAnimSingle*, void*);
extern "C" void fn_8001A198(xAnimTable*, U32 (*)(U32, xAnimActiveEffect*, xAnimSingle*, void*));

// Draw each foot's strip once there are enough edges to form it.
void zFeetRenderSkidMark(_tagFeetSkidMark* m)
{
    if (m->total_edges >= 2)
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, lbl_8025EEC0);
        for (S32 i = 0; i < m->total_feet; i++)
        {
            DrawSkidMarkStrip(m, m->edge[i], 20, 18, 3);
        }
    }
}

// Age the oldest edge until the strip has fewer than two edges left.
void zFeetUpdateSkidMark(_tagFeetSkidMark* m, F32 seconds)
{
    if (m->total_edges > 0)
    {
        m->remove_timer -= seconds;
        if (m->remove_timer <= 0.0f)
        {
            m->remove_timer = 1.0f;
            m->first_idx++;
            if (m->first_idx >= m->total_edges - 1)
            {
                m->first_idx = 0;
                m->total_edges = 0;
            }
        }
    }
}

// Sample each paw's position into the current skid strip while the player is grounded.
void zFeetAddSkidMark(_xEnt* ent, _tagFeetSkidMark* m, F32 seconds)
{
    m->sample_timer -= seconds;
    if (xEntIsVisible(ent) && !(&ent->collis->colls[0] && ent->collis->colls[0].optr) &&
        globals.player.JumpState == 0 && m->sample_timer <= 0.0f)
    {
        m->sample_timer = 0.0f;
        if (m->total_edges == 0)
        {
            m->remove_timer = 1.0f;
        }
        else if (m->total_edges >= 16)
        {
            m->total_edges = 15;
        }

        xVec3 feet_dir;
        feet_dir.x = ent->model->Mat->right.x;
        feet_dir.y = 0.0f;
        feet_dir.z = ent->model->Mat->right.z;
        xVec3Normalize(&feet_dir, &feet_dir);

        for (S32 i = 0; i < m->total_feet; i++)
        {
            _tagSkidEdge* edge = &m->edge[i][m->total_edges];
            iModelTagEval(ent->model->Data, &globals.player.HangPawTag[i], ent->model->Mat, &edge->centre);
            edge->centre.y = globals.player.ent.model->Mat->pos.y - 0.15f;
            edge->local_x = feet_dir;
            edge->width = lbl_801A4140[i];
        }
        m->total_edges++;
    }
}

// Allocate a sixteen-edge strip for each foot.
void zFeetInitSkidMark(_tagFeetSkidMark* m, S32 total_feet)
{
    m->total_feet = total_feet;
    for (S32 i = 0; i < m->total_feet; i++)
    {
        m->edge[i] = (_tagSkidEdge*)xMemAlloc(gActiveHeap, 16 * sizeof(_tagSkidEdge));
    }
    m->first_idx = 0;
    m->total_edges = 0;
}

// The registered feet debug callback has no work in this build.
void zFeetDebugCB()
{
}

// Play a villain's surface footstep and emit its footprint effects.
U32 zFeetStepVillainCB(U32, xAnimActiveEffect* active, xAnimSingle*, void*)
{
    _tagFoot* foot = (_tagFoot*)(active->Effect + 1);
    tagzFeet* feet = foot->feet;
    if (feet->total <= 0)
    {
        return 0;
    }

    _xEnt* ent = feet->owner;
    if (!(ent->baseFlags & 1) || !xEntIsVisible(ent))
    {
        return 0;
    }

    _zParEmitterCustomSettings info;
    st_zEntVillain* villain = (st_zEntVillain*)ent;
    iModelTagEval(ent->model->Data, &villain->tag_vert[foot->idx], ent->model->Mat, &info.pos);

    S32 surface_type = 0;
    xSurface* surface = fn_800D2F34(&ent->collis->colls[0]);
    if (surface && surface->state == 0 && surface->moprops &&
        ((zSurfaceProps*)surface->moprops)->asset)
    {
        surface_type = ((zSurfaceProps*)surface->moprops)->asset->surf_type;
    }
    if (surface_type == 22)
    {
        return 0;
    }

    if (lbl_8022C678[surface_type])
    {
        F32 pitch;
        feet->pitch = zPitchChange(feet->pitch, &pitch, 45, 3.0f);
        if (xSndPlay3D(lbl_8022C678[surface_type], 0.25f, pitch, 0, 2, &info.pos, 25.0f) == 0xFFFFFFFF)
        {
            lbl_8022C678[surface_type] = 0;
        }
    }

    info.custom_flags = 0x10020;
    xVec3 angles;
    xMat3x3GetEuler((xMat3x3*)ent->model->Mat, &angles);
    info.rot[0] = (U8)(255.0f * (angles.x / 6.2831855f));
    info.rot[1] = (U8)(255.0f * (angles.y / 6.2831855f));
    info.rot[2] = (U8)(255.0f * (angles.z / 6.2831855f));
    zParEmitterEmitCustom((_zParEmitter*)lbl_8025EEBC, 0.0f, &info);
    info.custom_flags = 0x20;
    zParEmitterEmitCustom(lbl_802290A0[surface_type], 0.033333335f, &info);
    return 0;
}

// Remove the player footstep effect from the animation table.
void zFeetExit(_xEnt*, xAnimTable* table)
{
    fn_8001A198(table, zFeetStepCB);
}

// Resolve the surface sounds, footprint emitters and skid-mark texture.
void zFeetGetIDs()
{
    char surface[128];
    char sound[128];
    for (S32 i = 0; i < 23; i++)
    {
        zSurfaceGetName(i, surface);
        strcpy(sound, "SNDFX_STEP_");
        strcat(sound, surface);
        lbl_8022C678[i] = xStrHash(sound);
    }

    lbl_8025EEB8 = zSceneFindObject(xStrHash("PAREMIT_SD_PAWPRINT"));
    lbl_8025EEBC = zSceneFindObject(xStrHash("PAREMIT_VIL_FOOTPRINT"));
    void* texture = xSTFindAsset(xStrHash("SKIDMARKTEX"), 0);
    if (texture)
    {
        lbl_8025EEC0 = *(void**)texture;
    }
    fn_80021B9C("Feet", zFeetDebugCB);
}
