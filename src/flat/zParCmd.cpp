#include "zParCmd.h"
#include "xPar.h"
#include "xParGroup.h"
#include "xMath3.h"
#include "iMath.h"
#include "xClimate.h"
#include "xString.h"
#include "zGlobals.h"
#include "zParEmitter.h"
#include "zScene.h"

#include <stdio.h>
#include <string.h>

struct _zVolume;

struct zCmdInfo
{
    U32 type;
    U32 size;
    void (*func)(zParCmd*, xParGroup*, F32);
};

void xClimateGetRealWindDir(_tagClimate* climate, xVec3* dir);

void xParCmdCustom_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdTexAnim_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdTex_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdJet_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdRotateAround_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdDamagePlayer_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdAnimalMagentism_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdApplyCamMat_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdVelocityApply_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdRotPar_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdApplyWind_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdRandomVelocityPar_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdPlayerCollision_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdAlpha3rdPolyReg_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdClipVolumes_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdScale_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdSmokeAlpha_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdScale3rdPolyReg_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdMoveRandomPar_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdMoveRandom_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdMove_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdAccelerate_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdOrbitLine_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdOrbitPoint_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdFollow_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdAge_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdKillSlow_Update(zParCmd* c, xParGroup* ps, F32 dt);
void xParCmdKillDistance_Update(zParCmd* c, xParGroup* ps, F32 dt);

static S32 sClipVolumeTotal;
static _zVolume* sClipVolume[32];

// One entry per command type: the type id, the size of its asset and its update function. Type 5
// has no update on this platform.
static zCmdInfo sCmdInfo[29] = {
    { 0x00, 0x14, xParCmdMove_Update },
    { 0x01, 0x14, xParCmdMoveRandom_Update },
    { 0x02, 0x14, xParCmdAccelerate_Update },
    { 0x03, 0x08, xParCmdVelocityApply_Update },
    { 0x04, 0x2C, xParCmdJet_Update },
    { 0x05, 0x0C, NULL },
    { 0x06, 0x10, xParCmdKillSlow_Update },
    { 0x07, 0x10, xParCmdFollow_Update },
    { 0x08, 0x20, xParCmdOrbitPoint_Update },
    { 0x09, 0x2C, xParCmdOrbitLine_Update },
    { 0x0A, 0x14, xParCmdMoveRandomPar_Update },
    { 0x0B, 0x18, xParCmdScale3rdPolyReg_Update },
    { 0x0C, 0x24, xParCmdTex_Update },
    { 0x0D, 0x18, xParCmdTexAnim_Update },
    { 0x0E, 0x10, xParCmdPlayerCollision_Update },
    { 0x0F, 0x14, xParCmdRandomVelocityPar_Update },
    { 0x10, 0x1C, xParCmdCustom_Update },
    { 0x11, 0x10, xParCmdKillDistance_Update },
    { 0x12, 0x0C, xParCmdAge_Update },
    { 0x13, 0x18, xParCmdAlpha3rdPolyReg_Update },
    { 0x14, 0x0C, xParCmdApplyWind_Update },
    { 0x15, 0x20, xParCmdRotPar_Update },
    { 0x16, 0x14, xParCmdApplyCamMat_Update },
    { 0x17, 0x20, xParCmdRotateAround_Update },
    { 0x18, 0x0C, xParCmdSmokeAlpha_Update },
    { 0x19, 0x0C, xParCmdScale_Update },
    { 0x1A, 0x0C, xParCmdClipVolumes_Update },
    { 0x1B, 0x0C, xParCmdAnimalMagentism_Update },
    { 0x1C, 0x10, xParCmdDamagePlayer_Update },
};

// Collects the scene's clip volumes, named PARTICLE_CLIP_1 upwards, stopping at the first gap.
void zParCmdFindClipVolumes()
{
    char findname[64];

    memset(sClipVolume, 0, sizeof(sClipVolume));
    for (sClipVolumeTotal = 0; sClipVolumeTotal < 32; sClipVolumeTotal++)
    {
        sprintf(findname, "PARTICLE_CLIP_%d", sClipVolumeTotal + 1);
        _zVolume* vol = (_zVolume*)zSceneFindObject(xStrHash(findname));
        if (vol == NULL)
        {
            return;
        }
        sClipVolume[sClipVolumeTotal] = vol;
    }
}

U32 zParCmdGetSize(U32 parType)
{
    for (S32 i = 0; i < 29; i++)
    {
        if (sCmdInfo[i].type == parType)
        {
            return sCmdInfo[i].size;
        }
    }

    return 0;
}

void (*zParCmdGetUpdateFunc(U32 parType))(zParCmd*, xParGroup*, F32)
{
    for (S32 i = 0; i < 29; i++)
    {
        if (sCmdInfo[i].type == parType)
        {
            return sCmdInfo[i].func;
        }
    }

    return NULL;
}

// Kills particles farther from the camera than the limit, or nearer when kill_greater_than is
// clear. A lifetime of -1 marks a particle dead.
void xParCmdKillDistance_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdKillDistance* cmd = (xParCmdKillDistance*)c->tasset;
    xVec3* camera_pos = &globals.camera.hi_cam.mat.pos;

    if (cmd->kill_greater_than)
    {
        for (; p != NULL; p = p->m_next)
        {
            F32 dx = p->m_pos.x - camera_pos->x;
            F32 dy = p->m_pos.y - camera_pos->y;
            F32 dz = p->m_pos.z - camera_pos->z;
            if (dx * dx + dy * dy + dz * dz > cmd->dSqr)
            {
                p->m_lifetime = -1.0f;
            }
        }
    }
    else
    {
        for (; p != NULL; p = p->m_next)
        {
            F32 dx = p->m_pos.x - camera_pos->x;
            F32 dy = p->m_pos.y - camera_pos->y;
            F32 dz = p->m_pos.z - camera_pos->z;
            if (dx * dx + dy * dy + dz * dz < cmd->dSqr)
            {
                p->m_lifetime = -1.0f;
            }
        }
    }
}

void xParCmdKillSlow_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdKillSlow* cmd = (xParCmdKillSlow*)c->tasset;
    F32 speedLimit = cmd->speedLimitSqr * dt;

    if (cmd->kill_less_than)
    {
        while (p)
        {
            if (xVec3Length2(&p->m_vel) < speedLimit)
            {
                p->m_lifetime = -1.0f;
            }
            p = p->m_next;
        }
    }
    else
    {
        while (p)
        {
            if (xVec3Length2(&p->m_vel) > speedLimit)
            {
                p->m_lifetime = -1.0f;
            }
            p = p->m_next;
        }
    }
}

void xParCmdAge_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    F32 age_rate = ((xParCmdAge*)c->tasset)->age_rate * dt;
    xPar* p = ps->m_root;

    while (p)
    {
        p->m_lifetime -= age_rate;
        p = p->m_next;
    }
}

// Pulls each particle toward the next one in the list.
void xParCmdFollow_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdFollow* cmd = (xParCmdFollow*)c->tasset;
    F32 mdt = cmd->gravity * dt;

    while (p && p->m_next)
    {
        xVec3 d;
        xVec3Sub(&d, &p->m_next->m_pos, &p->m_pos);

        F32 lenSqr = xVec3Length2(&d);
        F32 force = mdt / ((lenSqr + cmd->epsilon) * xVec3LengthFast(d.x, d.y, d.z));

        p->m_vel.x += d.x * force;
        p->m_vel.y += d.y * force;
        p->m_vel.z += d.z * force;

        p = p->m_next;
    }
}

void xParCmdAccelerate_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdAccelerate* cmd = (xParCmdAccelerate*)c->tasset;
    F32 ax = cmd->acc.x * dt;
    F32 ay = cmd->acc.y * dt;
    F32 az = cmd->acc.z * dt;

    while (p)
    {
        p->m_vel.x += ax;
        p->m_vel.y += ay;
        p->m_vel.z += az;
        p = p->m_next;
    }
}

void xParCmdMove_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdMove* cmd = (xParCmdMove*)c->tasset;
    F32 dx = cmd->dir.x * dt;
    F32 dy = cmd->dir.y * dt;
    F32 dz = cmd->dir.z * dt;

    while (p)
    {
        p->m_pos.x += dx;
        p->m_pos.y += dy;
        p->m_pos.z += dz;
        p = p->m_next;
    }
}

// Jitters particles on the ground plane.
void xParCmdMoveRandomPar_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdMoveRandomPar* cmd = (xParCmdMoveRandomPar*)c->tasset;
    F32 hdt = dt * 0.5f;
    F32 dx = cmd->dim.x * hdt;
    F32 dz = cmd->dim.z * hdt;

    while (p)
    {
        p->m_pos.x += dx * (iurand() - 0.5f);
        p->m_pos.z += dz * (iurand() - 0.5f);
        p = p->m_next;
    }
}

// Size over the particle's life as a cubic in the elapsed fraction, scaled by its emitter's death
// size.
void xParCmdScale3rdPolyReg_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdScale3rdPolyReg* cmd = (xParCmdScale3rdPolyReg*)c->tasset;

    while (p)
    {
        F32 t = p->m_lifetime / p->m_asset->life;
        if (t < 0.0f)
        {
            t = 0.0f;
        }
        else if (t > 1.0f)
        {
            t = 1.0f;
        }
        t = 1.0f - t;
        F32 t2 = t * t;

        p->m_size =
            (cmd->constant + cmd->a * t + cmd->a2 * t2 + cmd->a3 * (t2 * t)) * p->m_asset->size_death;
        p->m_sizeVel = 0.0f;
        p = p->m_next;
    }
}

// Alpha over the particle's life as a cubic in the elapsed fraction.
void xParCmdAlpha3rdPolyReg_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xParCmdAlpha3rdPolyReg* cmd = (xParCmdAlpha3rdPolyReg*)c->tasset;
    xPar* p = ps->m_root;

    while (p)
    {
        F32 t = p->m_lifetime / p->m_asset->life;
        if (t < 0.0f)
        {
            t = 0.0f;
        }
        else if (t > 1.0f)
        {
            t = 1.0f;
        }
        t = 1.0f - t;
        F32 t2 = t * t;

        F32 alpha = 255.0f * (cmd->constant + cmd->a * t + cmd->a2 * t2 + cmd->a3 * (t2 * t));
        if (alpha > 255.0f)
        {
            alpha = 255.0f;
        }
        p->m_c[3] = (S32)alpha;
        p->m_cvel[3] = 0.0f;
        p = p->m_next;
    }
}

// Pushes particles along the wind on the ground plane.
void xParCmdApplyWind_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xParCmdApplyWind* cmd = (xParCmdApplyWind*)c->tasset;
    xVec3 wind_vec;

    xClimateGetRealWindDir(&gClimate, &wind_vec);
    F32 strength = cmd->strength * dt;
    wind_vec.x *= strength;
    wind_vec.z *= strength;

    for (xPar* p = ps->m_root; p != NULL; p = p->m_next)
    {
        p->m_vel.x += wind_vec.x;
        p->m_vel.z += wind_vec.z;
    }
}

void xParCmdVelocityApply_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;

    while (p)
    {
        p->m_pos.x += p->m_vel.x;
        p->m_pos.y += p->m_vel.y;
        p->m_pos.z += p->m_vel.z;
        p = p->m_next;
    }
}

// Texture commands are read by the particle renderer; they have no per-frame update.
void xParCmdTex_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
}
