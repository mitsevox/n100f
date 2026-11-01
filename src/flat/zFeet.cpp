#include "zFeet.h"
#include "xDebug.h"
#include "xEnv.h"
#include "xCollideFast.h"
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
#include "zMenu.h"
#include <string.h>

struct _xEnt;

#if VERSION_GIHE78_REV0
extern void* lbl_8025EEC0;
extern xBase* lbl_8025EEB8;
extern xBase* lbl_8025EEBC;
extern U32 lbl_8022C678[23];
extern F32 lbl_801A4140[];
extern RwIm3DVertex lbl_8022C6D4[];
#define SKID_RASTER lbl_8025EEC0
#define PAWPRINT_EMITTER lbl_8025EEB8
#define FOOTPRINT_EMITTER lbl_8025EEBC
#define STEP_SOUND_IDS lbl_8022C678
#define SKID_WIDTHS lbl_801A4140
#define RAY_COLLIDE_DATA gCurXEnv
#define STRIP_VERTS lbl_8022C6D4
#elif VERSION_GIHE78_REV1
extern void* lbl_80270D58;
extern xBase* lbl_80270D50;
extern xBase* lbl_80270D54;
extern U32 lbl_8023CD28[23];
extern F32 lbl_801A12A8[];
extern RwIm3DVertex lbl_8023CD84[];
#define SKID_RASTER lbl_80270D58
#define PAWPRINT_EMITTER lbl_80270D50
#define FOOTPRINT_EMITTER lbl_80270D54
#define STEP_SOUND_IDS lbl_8023CD28
#define SKID_WIDTHS lbl_801A12A8
#define RAY_COLLIDE_DATA gCurXEnv
#define STRIP_VERTS lbl_8023CD84
#endif
extern void zSurfaceGetName(S32, char*);
void DrawSkidMarkStrip(_tagFeetSkidMark*, _tagSkidEdge*, U8, U8, U8);
U32 zFeetStepCB(U32, xAnimActiveEffect*, xAnimSingle*, void*);
void xAnimTableRemoveEffectByCB(xAnimTable*, U32 (*)(U32, xAnimActiveEffect*, xAnimSingle*, void*));

// Defined in reverse address order: the deferred profile emits the last-defined function first.
void zFeetRenderSkidMark(_tagFeetSkidMark* m);
void DrawSkidMarkStrip(_tagFeetSkidMark* m, _tagSkidEdge* edges, U8 red, U8 green, U8 blue);
void zFeetUpdateSkidMark(_tagFeetSkidMark* m, F32 seconds);
void zFeetAddSkidMark(_xEnt* ent, _tagFeetSkidMark* m, F32 seconds);
void zFeetInitSkidMark(_tagFeetSkidMark* m, S32 total_feet);
void zFeetDebugCB();
U32 zFeetStepVillainCB(U32, xAnimActiveEffect* active, xAnimSingle*, void*);
U32 zFeetStepCB(U32, xAnimActiveEffect* active, xAnimSingle*, void*);
static inline void zFeetAddSteps(xAnimTable* table, char* name, tagzFeet* feet, S32* tag_idx,
                                 F32* step_time,
                                 U32 (*cb)(U32, xAnimActiveEffect*, xAnimSingle*, void*));
void zFeetInit(S32 ent_type, _xEnt* owner, tagzFeet* feet, S32 feet_subtype, xAnimTable* table);
void zFeetExit(_xEnt*, xAnimTable* table);
void zFeetGetIDs();

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
        STEP_SOUND_IDS[i] = xStrHash(sound);
    }

    PAWPRINT_EMITTER = zSceneFindObject(xStrHash("PAREMIT_SD_PAWPRINT"));
    FOOTPRINT_EMITTER = zSceneFindObject(xStrHash("PAREMIT_VIL_FOOTPRINT"));
    void* texture = xSTFindAsset(xStrHash("SKIDMARKTEX"), 0);
    if (texture)
    {
        SKID_RASTER = *(void**)texture;
    }
    xDebugModeAdd("Feet", zFeetDebugCB);
}

// Remove the player footstep effect from the animation table.
void zFeetExit(_xEnt*, xAnimTable* table)
{
    xAnimTableRemoveEffectByCB(table, zFeetStepCB);
}

// Hook the footstep callbacks into the walk, run, land and other movement animations. The player
// (ent_type -1) has one timing set per subtype, with four feet in most of them; the villains with
// footsteps have two feet stepping in their Move and Chase states.
void zFeetInit(S32 ent_type, _xEnt* owner, tagzFeet* feet, S32 feet_subtype, xAnimTable* table)
{
    F32 step_time[6];
    S32 tag_idx[6];

    feet->owner = owner;
    feet->entity_type = ent_type;

    if (feet->entity_type == -1)
    {
        for (S32 i = 0; i < 6; i++)
        {
            tag_idx[i] = i;
        }

        feet->total = 4;

        switch (feet_subtype)
        {
        case 0:
            step_time[0] = 0.69825f;
            step_time[1] = 0.03325f;
            step_time[2] = 1.33f;
            step_time[3] = 0.56525f;
            zFeetAddSteps(table, "Walk_brave", feet, tag_idx, step_time, zFeetStepCB);
            zFeetAddSteps(table, "Walk_normal", feet, tag_idx, step_time, zFeetStepCB);
            zFeetAddSteps(table, "Walk_scared", feet, tag_idx, step_time, zFeetStepCB);
            zFeetAddSteps(table, "Walk_boots", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 1:
            step_time[0] = 0.17434211f;
            step_time[1] = 0.12453008f;
            step_time[2] = 0.32377818f;
            step_time[3] = 0.32377818f;
            zFeetAddSteps(table, "Run_brave", feet, tag_idx, step_time, zFeetStepCB);
            zFeetAddSteps(table, "Run_normal", feet, tag_idx, step_time, zFeetStepCB);
            zFeetAddSteps(table, "Run_scared", feet, tag_idx, step_time, zFeetStepCB);
            zFeetAddSteps(table, "Run_boots", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 2:
            feet->total = 2;
            step_time[0] = 0.5657377f;
            step_time[1] = 1.5973771f;
            tag_idx[0] = 2;
            tag_idx[1] = 3;
            zFeetAddSteps(table, "Walk_sneak", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 3:
            step_time[0] = 0.349125f;
            step_time[1] = 0.016625f;
            step_time[2] = 0.665f;
            step_time[3] = 0.282625f;
            zFeetAddSteps(table, "Walk_sticky", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 4:
            step_time[0] = 0.12531328f;
            step_time[1] = 0.12531328f;
            step_time[2] = 0.25062656f;
            step_time[3] = 0.25062656f;
            zFeetAddSteps(table, "Action_attackrun", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 5:
            step_time[0] = 0.29333332f;
            step_time[1] = 0.26666668f;
            step_time[2] = 0.026666667f;
            step_time[3] = 0.08f;
            zFeetAddSteps(table, "Land_normal", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 6:
            step_time[0] = 0.02505f;
            step_time[1] = 0.00835f;
            step_time[2] = 0.32565f;
            step_time[3] = 0.2505f;
            zFeetAddSteps(table, "Land_running", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 7:
            step_time[0] = 0.067058824f;
            step_time[1] = 0.13411765f;
            step_time[2] = 0.38f;
            step_time[3] = 0.38f;
            zFeetAddSteps(table, "Land_walk", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 8:
            step_time[0] = 0.100250624f;
            step_time[1] = 0.3508772f;
            step_time[2] = 0.5012531f;
            step_time[3] = 0.7017544f;
            zFeetAddSteps(table, "Walk_slippery", feet, tag_idx, step_time, zFeetStepCB);
            break;
        case 9:
            step_time[0] = 0.06f;
            step_time[1] = 0.12f;
            step_time[2] = 0.18f;
            step_time[3] = 0.24f;
            zFeetAddSteps(table, "Run_slippery", feet, tag_idx, step_time, zFeetStepCB);
            break;
        }
    }
    else
    {
        feet->total = 2;

        for (S32 i = 0; i < 6; i++)
        {
            tag_idx[i] = i;
        }

        switch (ent_type)
        {
        case eEntVillainTypeCreeper:
            step_time[0] = 0.31935483f;
            step_time[1] = 0.07096774f;
            zFeetAddSteps(table, "Move", feet, tag_idx, step_time, zFeetStepVillainCB);
            step_time[0] = 0.17741935f;
            step_time[1] = 0.46129033f;
            zFeetAddSteps(table, "Chase", feet, tag_idx, step_time, zFeetStepVillainCB);
            break;
        case eEntVillainTypeGhostDiver:
            step_time[0] = 1.0f;
            step_time[1] = 0.516129f;
            zFeetAddSteps(table, "Move", feet, tag_idx, step_time, zFeetStepVillainCB);
            step_time[0] = 0.06923077f;
            step_time[1] = 0.5192308f;
            zFeetAddSteps(table, "Chase", feet, tag_idx, step_time, zFeetStepVillainCB);
            break;
        case eEntVillainTypeRobot:
            step_time[0] = 1.625f;
            step_time[1] = 0.75f;
            zFeetAddSteps(table, "Move", feet, tag_idx, step_time, zFeetStepVillainCB);
            step_time[0] = 0.46875f;
            step_time[1] = 0.25f;
            zFeetAddSteps(table, "Chase", feet, tag_idx, step_time, zFeetStepVillainCB);
            break;
        case eEntVillainTypeSpaceKook:
            step_time[0] = 0.46666667f;
            step_time[1] = 1.1f;
            zFeetAddSteps(table, "Move", feet, tag_idx, step_time, zFeetStepVillainCB);
            step_time[0] = 0.46666667f;
            step_time[1] = 1.1f;
            zFeetAddSteps(table, "Chase", feet, tag_idx, step_time, zFeetStepVillainCB);
            break;
        case eEntVillainTypeWolfman:
            step_time[0] = 0.945f;
            step_time[1] = 0.21f;
            zFeetAddSteps(table, "Move", feet, tag_idx, step_time, zFeetStepVillainCB);
            step_time[0] = 0.57857144f;
            step_time[1] = 0.9f;
            zFeetAddSteps(table, "Chase", feet, tag_idx, step_time, zFeetStepVillainCB);
            break;
        // Villain types without footsteps.
        case eEntVillainTypeBat:
        case eEntVillainGroundsKeeper:
            break;
        }
    }
}

// Adds a footstep effect to the named animation state for each foot: foot i steps at
// step_time[i] with model tag tag_idx[i]. The helper's name is this project's.
static inline void zFeetAddSteps(xAnimTable* table, char* name, tagzFeet* feet, S32* tag_idx,
                                 F32* step_time,
                                 U32 (*cb)(U32, xAnimActiveEffect*, xAnimSingle*, void*))
{
    xAnimState* state = xAnimTableGetState(table, name);

    for (S32 i = 0; i < feet->total; i++)
    {
        _tagFoot foot;
        foot.feet = feet;
        foot.idx = tag_idx[i];
        memcpy(xAnimStateNewEffect(state, 1, step_time[i], 0.0f, cb, sizeof(_tagFoot)) + 1, &foot,
               sizeof(_tagFoot));
    }
}

// Play a player footstep and leave a paw print when a downward ray reaches the ground.
U32 zFeetStepCB(U32, xAnimActiveEffect* active, xAnimSingle*, void*)
{
    if (zMenuRunning())
    {
        return 0;
    }

    _tagFoot* foot = (_tagFoot*)(active->Effect + 1);
    tagzFeet* feet = foot->feet;
    _xEnt* ent = feet->owner;
    if (!xEntIsVisible(ent))
    {
        return 0;
    }

    _zParEmitterCustomSettings info;
    info.custom_flags = 0x10020;
    iModelTagEval(ent->model->Data, &globals.player.HangPawTag[foot->idx], ent->model->Mat, &info.pos);

    _xRay3 ray;
    ray.origin = info.pos;
    ray.origin.y = ent->model->Mat->pos.y + 0.05f;
    ray.dir.x = 0.0f;
    ray.dir.y = -0.1f;
    ray.dir.z = 0.0f;
    ray.flags = 0;
    S32 collided_with_ground = xRayHitsEnvFast(&ray, RAY_COLLIDE_DATA) != 0;
    info.pos.y = ent->model->Mat->pos.y + 0.05f;

    S32 surface_type = 0;
    xSurface* surface = zSurfaceGetSurface(&ent->collis->colls[0]);
    if (surface && surface->state == 0 && surface->moprops &&
        ((zSurfaceProps*)surface->moprops)->asset)
    {
        surface_type = ((zSurfaceProps*)surface->moprops)->asset->surf_type;
    }
    if (surface_type == 22)
    {
        return 0;
    }

    if (STEP_SOUND_IDS[surface_type])
    {
        F32 pitch;
        feet->pitch = zPitchChange(feet->pitch, &pitch, 45, 3.0f);
        if (xSndPlay3D(STEP_SOUND_IDS[surface_type], 0.25f, pitch, 0, 2, &info.pos, 25.0f) == 0xFFFFFFFF)
        {
            STEP_SOUND_IDS[surface_type] = 0;
        }
    }

    if (collided_with_ground && !(&ent->collis->colls[0] && ent->collis->colls[0].optr) &&
        globals.player.JumpState == 0)
    {
        xVec3 angles;
        xMat3x3GetEuler((xMat3x3*)ent->model->Mat, &angles);
        info.rot[0] = (U8)(255.0f * (angles.x / 6.2831855f));
        info.rot[1] = (U8)(255.0f * (angles.y / 6.2831855f));
        info.rot[2] = (U8)(255.0f * (angles.z / 6.2831855f));
        zParEmitterEmitCustom((_zParEmitter*)PAWPRINT_EMITTER, 0.0f, &info);
        info.custom_flags = 0x20;
        zParEmitterEmitCustom(gPlayerfx.emit_land[surface_type], 0.033333335f, &info);
    }
    return 0;
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
    xSurface* surface = zSurfaceGetSurface(&ent->collis->colls[0]);
    if (surface && surface->state == 0 && surface->moprops &&
        ((zSurfaceProps*)surface->moprops)->asset)
    {
        surface_type = ((zSurfaceProps*)surface->moprops)->asset->surf_type;
    }
    if (surface_type == 22)
    {
        return 0;
    }

    if (STEP_SOUND_IDS[surface_type])
    {
        F32 pitch;
        feet->pitch = zPitchChange(feet->pitch, &pitch, 45, 3.0f);
        if (xSndPlay3D(STEP_SOUND_IDS[surface_type], 0.25f, pitch, 0, 2, &info.pos, 25.0f) == 0xFFFFFFFF)
        {
            STEP_SOUND_IDS[surface_type] = 0;
        }
    }

    info.custom_flags = 0x10020;
    xVec3 angles;
    xMat3x3GetEuler((xMat3x3*)ent->model->Mat, &angles);
    info.rot[0] = (U8)(255.0f * (angles.x / 6.2831855f));
    info.rot[1] = (U8)(255.0f * (angles.y / 6.2831855f));
    info.rot[2] = (U8)(255.0f * (angles.z / 6.2831855f));
    zParEmitterEmitCustom((_zParEmitter*)FOOTPRINT_EMITTER, 0.0f, &info);
    info.custom_flags = 0x20;
    zParEmitterEmitCustom(gPlayerfx.emit_land[surface_type], 0.033333335f, &info);
    return 0;
}

// The registered feet debug callback has no work in this build.
void zFeetDebugCB()
{
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
            edge->width = SKID_WIDTHS[i];
        }
        m->total_edges++;
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

// Turn the sampled edges into a textured triangle strip.
void DrawSkidMarkStrip(_tagFeetSkidMark* m, _tagSkidEdge* edges, U8 red, U8 green, U8 blue)
{
    U32 vert_idx = 0;
    S32 i = m->first_idx;
    do
    {
        _tagSkidEdge* edge = &edges[i];
        F32 u;
        if (i == m->first_idx)
        {
            u = 0.0f;
        }
        else if (i == m->total_edges - 1)
        {
            u = 1.0f;
        }
        else
        {
            u = (i & 1) ? 0.2f : 0.4f;
        }

        U8 alpha = (i == m->first_idx) ? (U8)(m->remove_timer * 128.0f) : 128;
        for (S32 side = 0; side < 2; side++)
        {
            F32 width = side ? edge->width : -edge->width;
            RwIm3DVertex* vertex = &STRIP_VERTS[vert_idx++];
            vertex->x = edge->centre.x + edge->local_x.x * width;
            vertex->y = edge->centre.y + edge->local_x.y * width + 0.2f;
            vertex->z = edge->centre.z + edge->local_x.z * width;
            vertex->r = red;
            vertex->g = green;
            vertex->b = blue;
            vertex->a = alpha;
            vertex->u = u;
            vertex->v = (F32)side;
            if (vert_idx >= 256)
            {
                break;
            }
        }
        if (vert_idx >= 256)
        {
            break;
        }
        i++;
    } while (i < m->total_edges);

    if (RwIm3DTransform(STRIP_VERTS, vert_idx, 0, 0x19))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
        RwIm3DEnd();
    }
}

// Draw each foot's strip once there are enough edges to form it.
void zFeetRenderSkidMark(_tagFeetSkidMark* m)
{
    if (m->total_edges >= 2)
    {
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, SKID_RASTER);
        for (S32 i = 0; i < m->total_feet; i++)
        {
            DrawSkidMarkStrip(m, m->edge[i], 20, 18, 3);
        }
    }
}
