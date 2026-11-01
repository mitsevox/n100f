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
#include "zVolume.h"
#include "zEntPlayer.h"
#include "xBound.h"
#include "xMathInlines.h"

#include <stdio.h>
#include <string.h>

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

// An empty function between zParCmdGetUpdateFunc and xParCmdKillDistance_Update that nothing in
// the target references; the PS2 builds have no function here, so its name is unknown.
extern "C" void fn_800C4AB8()
{
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

// Pulls particles inside the radius toward the centre point.
void xParCmdOrbitPoint_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdOrbitPoint* cmd = (xParCmdOrbitPoint*)c->tasset;
    F32 mdt = cmd->gravity * dt;

    while (p)
    {
        xVec3 d;
        xVec3Sub(&d, &cmd->center, &p->m_pos);

        F32 rSqr = xVec3Length2(&d);
        if (rSqr < cmd->maxRadiusSqr)
        {
            F32 force = mdt / (xVec3LengthFast(d.x, d.y, d.z) + (rSqr + cmd->epsilon));

            p->m_vel.x += d.x * force;
            p->m_vel.y += d.y * force;
            p->m_vel.z += d.z * force;
        }

        p = p->m_next;
    }
}

// Pulls particles inside the radius toward the nearest point of the line through p along axis.
void xParCmdOrbitLine_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdOrbitLine* cmd = (xParCmdOrbitLine*)c->tasset;
    F32 mdt = cmd->gravity * dt;

    while (p)
    {
        xVec3 rel;
        xVec3 perp;
        xVec3 onLine;
        xVec3 d;

        xVec3Sub(&rel, &p->m_pos, &cmd->p);
        xVec3Cross(&perp, &rel, &cmd->axis);
        xVec3Cross(&onLine, &cmd->axis, &perp);
        xVec3Sub(&d, &onLine, &rel);

        F32 rSqr = xVec3Length2(&d);
        if (rSqr < cmd->maxRadiusSqr)
        {
            F32 force = mdt / (xVec3LengthFast(d.x, d.y, d.z) + (rSqr + cmd->epsilon));

            p->m_vel.x += d.x * force;
            p->m_vel.y += d.y * force;
            p->m_vel.z += d.z * force;
        }

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

// Moves the whole group by one random offset per frame, up to dir either way on each axis.
void xParCmdMoveRandom_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p;
    xParCmdMove* cmd = (xParCmdMove*)c->tasset;
    xVec3 d;

    d.x = cmd->dir.x * (2.0f * iurand());
    d.y = cmd->dir.y * (2.0f * iurand());
    d.z = cmd->dir.z * (2.0f * iurand());
    d.x = (d.x - cmd->dir.x) * dt;
    d.y = (d.y - cmd->dir.y) * dt;
    d.z = (d.z - cmd->dir.z) * dt;

    p = ps->m_root;
    while (p)
    {
        p->m_pos.x += d.x;
        p->m_pos.y += d.y;
        p->m_pos.z += d.z;
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

// Alpha over the particle's life from one of two built-in cubics in the elapsed fraction.
void xParCmdSmokeAlpha_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p;
    F32 constant;
    F32 a;
    F32 a2;
    F32 a3;

    switch (((xParCmdSmokeAlpha*)c->tasset)->curve)
    {
    case 0:
        constant = 0.0f;
        a = 8.06722736f;
        a2 = -14.789916f;
        a3 = 6.72268915f;
        break;
    case 1:
        constant = 0.0f;
        a = 1.26666665f;
        a2 = 3.4000001f;
        a3 = -4.66666651f;
        break;
    }

    p = ps->m_root;
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

        F32 alpha = 255.0f * (constant + a * t + a2 * t2 + a3 * (t2 * t));
        if (alpha > 255.0f)
        {
            alpha = 255.0f;
        }
        p->m_c[3] = (S32)alpha;
        p->m_cvel[3] = 0.0f;
        p = p->m_next;
    }
}

// Size over the particle's life from one of two built-in cubics in the elapsed fraction, scaled by
// its emitter's death size.
void xParCmdScale_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p;
    F32 constant;
    F32 a;
    F32 a2;
    F32 a3;

    switch (((xParCmdScale*)c->tasset)->curve)
    {
    case 0:
        constant = 0.0f;
        a = 1.26666701f;
        a2 = 3.4000001f;
        a3 = -4.66666698f;
        break;
    case 1:
        constant = 0.200000003f;
        a = 5.61580515f;
        a2 = -11.6466627f;
        a3 = 5.83085823f;
        break;
    }

    p = ps->m_root;
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

        p->m_size = (constant + a * t + a2 * t2 + a3 * (t2 * t)) * p->m_asset->size_death;
        p->m_sizeVel = 0.0f;
        p = p->m_next;
    }
}

// Kills particles inside any of the scene's clip volumes.
void xParCmdClipVolumes_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    _xCollis collis;

    for (xPar* p = ps->m_root; p != NULL; p = p->m_next)
    {
        for (S32 i = 0; i < sClipVolumeTotal; i++)
        {
            xVecHitsBound(&p->m_pos, &sClipVolume[i]->asset->bound, &collis);
            if (collis.flags & 0x1)
            {
                p->m_lifetime = -1.0f;
            }
        }
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

// Kills particles whose step this frame hits the player, while the elapsed fraction of their life
// is between min and max.
void xParCmdPlayerCollision_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xParCmdPlayerCollision* cmd = (xParCmdPlayerCollision*)c->tasset;
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

        if (t >= cmd->min && t <= cmd->max)
        {
            _xRay3 ray;
            ray.origin = p->m_pos;
            ray.dir = p->m_vel;
            ray.max_t = xVec3Normalize(&ray.dir, &ray.dir);
            ray.min_t = ray.max_t * cmd->min;
            if (cmd->max < ray.max_t)
            {
                ray.max_t = cmd->max;
            }
            if (zRayHitsPlayer(&ray))
            {
                p->m_lifetime = -1.0f;
            }
        }

        p = p->m_next;
    }
}

// Turns each particle's velocity by a random rotation of up to x, y and z radians a second.
void xParCmdRandomVelocityPar_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xParCmdRandomVelocityPar* cmd = (xParCmdRandomVelocityPar*)c->tasset;
    F32 rx = cmd->x * dt;
    F32 ry = cmd->y * dt;
    F32 rz = cmd->z * dt;

    while (p)
    {
        xMat3x3 mat;

        F32 y = 2.0f * (rx * iurand()) - rx;
        F32 x = 2.0f * (ry * iurand()) - ry;
        F32 z = 2.0f * (rz * iurand()) - rz;

        xMat3x3Euler(&mat, x, y, z);
        xMat3x3LMulVec(&p->m_vel, &mat, &p->m_vel);

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

// Spins each particle's three rotation angles by a random rate between min and max degrees a
// second; a full turn is 255 units.
void xParCmdRotPar_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xParCmdRotPar* cmd = (xParCmdRotPar*)c->tasset;
    F32 rangeX = 255.0f * ((cmd->max.x - cmd->min.x) / 360.0f);
    F32 rangeY = 255.0f * ((cmd->max.y - cmd->min.y) / 360.0f);
    F32 rangeZ = 255.0f * ((cmd->max.z - cmd->min.z) / 360.0f);
    F32 baseX = 255.0f * (cmd->min.x / 360.0f);
    F32 baseY = 255.0f * (cmd->min.y / 360.0f);
    F32 baseZ = 255.0f * (cmd->min.z / 360.0f);

    for (xPar* p = ps->m_root; p != NULL; p = p->m_next)
    {
        p->m_rotdeg[0] = (S32)(dt * (rangeX * iurand() + baseX) + p->m_rotdeg[0]);
        p->m_rotdeg[1] = (S32)(dt * (rangeY * iurand() + baseY) + p->m_rotdeg[1]);
        p->m_rotdeg[2] = (S32)(dt * (rangeZ * iurand() + baseZ) + p->m_rotdeg[2]);
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

// Accelerates particles along the camera's right, up and forward axes.
void xParCmdApplyCamMat_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xParCmdApplyCamMat* cmd = (xParCmdApplyCamMat*)c->tasset;
    xMat4x3* mat = &globals.camera.hi_cam.mat;
    xPar* p;
    F32 mul;

    if (cmd->apply.x != 0.0f)
    {
        mul = dt * cmd->apply.x;
        for (p = ps->m_root; p != NULL; p = p->m_next)
        {
            p->m_vel.x += mat->right.x * mul;
            p->m_vel.y += mat->right.y * mul;
            p->m_vel.z += mat->right.z * mul;
        }
    }

    if (cmd->apply.y != 0.0f)
    {
        mul = dt * cmd->apply.y;
        for (p = ps->m_root; p != NULL; p = p->m_next)
        {
            p->m_vel.x += mat->up.x * mul;
            p->m_vel.y += mat->up.y * mul;
            p->m_vel.z += mat->up.z * mul;
        }
    }

    if (cmd->apply.z != 0.0f)
    {
        mul = dt * cmd->apply.z;
        for (p = ps->m_root; p != NULL; p = p->m_next)
        {
            p->m_vel.x += mat->at.x * mul;
            p->m_vel.y += mat->at.y * mul;
            p->m_vel.z += mat->at.z * mul;
        }
    }
}

// Pulls particles toward a point one unit above the player. Particles that arrive die; particles
// within two units are slowed so they settle instead of overshooting.
void xParCmdAnimalMagentism_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;
    xVec3 pos = *(xVec3*)&globals.player.ent.model->Mat->pos;
    pos.y += 1.0f;
    F32 mul = dt * -((xParCmdAnimalMagnetism*)c->tasset)->strength;

    while (p)
    {
        xVec3 vec;
        xVec3Sub(&vec, &p->m_pos, &pos);
        F32 dist = xVec3Normalize(&vec, &vec);

        p->m_vel.x += vec.x * mul;
        p->m_vel.y += vec.y * mul;
        p->m_vel.z += vec.z * mul;

        if (dist < 0.25f)
        {
            p->m_lifetime = -1.0f;
        }
        else if (dist < 2.0f)
        {
            F32 damp = dist * 0.49f;
            p->m_vel.x *= damp;
            p->m_vel.y *= damp;
            p->m_vel.z *= damp;
        }

        p = p->m_next;
    }
}

// Hurts the player when a live particle is inside the player's bound. Only every tenth particle
// is tested each frame, starting one further along each frame.
void xParCmdDamagePlayer_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    static S32 last_idx = 0;
    xPar* p;
    _xBound* pbound = &globals.player.ent.bound;
    S32 i;

    last_idx++;
    if (last_idx >= 10)
    {
        last_idx = 0;
    }

    p = ps->m_root;
    for (i = 0; i < last_idx; i++)
    {
        if (p)
        {
            p = p->m_next;
        }
    }

    while (p)
    {
        if (p->m_lifetime > 0.1f)
        {
            _xCollis collis = { 0 };
            xVecHitsBound(&p->m_pos, pbound, &collis);
            if (collis.flags & 0x1)
            {
                zEntPlayer_Damage(NULL, 1, NULL);
            }
        }

        for (i = 0; i < 10; i++)
        {
            if (p)
            {
                p = p->m_next;
            }
        }
    }
}

// Swings particles around the vertical axis through pos at yaw degrees a second, while their
// distance from it grows by radius_growth a second.
void xParCmdRotateAround_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xParCmdRotateAround* cmd = (xParCmdRotateAround*)c->tasset;
    F32 radius_growth = dt * cmd->radius_growth;
    xPar* p = ps->m_root;
    F32 yaw = (3.14159274f * (dt * cmd->yaw)) / 180.0f;

    while (p)
    {
        xVec3 at;
        xMat3x3 lookmat;
        xVec3 angles;
        xMat3x3 rotmat;
        xVec3 offset;
        xVec3 out_offset;

        xVec3Sub(&at, &cmd->pos, &p->m_pos);
        at.y = 0.0f;
        F32 radius = xMat3x3LookVec(&lookmat, &at);
        xMat3x3GetEuler(&lookmat, &angles);
        angles.x += yaw;
        xMat3x3Euler(&rotmat, angles.x, angles.y, angles.z);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = radius + radius_growth;
        xMat3x3RMulVec(&out_offset, &rotmat, &offset);
        p->m_pos.x = out_offset.x + cmd->pos.x;
        p->m_pos.z = out_offset.z + cmd->pos.z;
        p = p->m_next;
    }
}

// Pushes particles inside the radius along acc, with x and z pointing away from the jet's centre
// (the player, in mode 0), weaker with distance.
void xParCmdJet_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xParCmdJet* cmd = (xParCmdJet*)c->tasset;
    F32 mdt = cmd->gravity * dt;
    xVec3 center;

    if (cmd->mode == 0)
    {
        center = *(xVec3*)&globals.player.ent.model->Mat->pos;
    }
    else
    {
        center = cmd->center;
    }

    for (xPar* p = ps->m_root; p != NULL; p = p->m_next)
    {
        xVec3 d;
        xVec3Sub(&d, &center, &p->m_pos);

        F32 rSqr = xVec3Length2(&d);
        if (rSqr < cmd->radiusSqr)
        {
            F32 oorSqr = mdt / xsqrt(rSqr + cmd->epsilon);
            F32 sx = (d.x < 0.0f) ? 1.0f : -1.0f;
            F32 sz = (d.z < 0.0f) ? 1.0f : -1.0f;

            p->m_vel.x += sx * (cmd->acc.x * oorSqr);
            p->m_vel.y += cmd->acc.y * oorSqr;
            p->m_vel.z += sz * (cmd->acc.z * oorSqr);
        }
    }
}

// Texture commands are read by the particle renderer; they have no per-frame update.
void xParCmdTex_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
}

// Steps each particle through the texture's cells: random (mode 0), forward or backward along a
// row (1, 2), or backward or forward down a column (3, 4). The wrap mode says what happens at the
// end of a row or column: move on to the next one (1), the previous one (2), or stop on the last
// cell (3, row-forward mode only). A throttle time limits how often the cells change.
void xParCmdTexAnim_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p;
    xParCmdTexAnim* cmd = (xParCmdTexAnim*)c->tasset;
    xParCmdTex* tex = ps->m_cmdTex;

    if (!tex)
    {
        return;
    }

    if (cmd->throttle_time > 0.0f)
    {
        cmd->throttle_time_elapsed -= dt;

        if (cmd->throttle_time_elapsed > 0.0f)
        {
            return;
        }

        cmd->throttle_time_elapsed = cmd->throttle_time;
    }

    p = ps->m_root;

    if (cmd->anim_mode == 0)
    {
        while (p)
        {
            p->m_texIdx[0] = irand() % tex->cols;
            p->m_texIdx[1] = irand() % tex->rows;

            p = p->m_next;
        }
    }
    else if (cmd->anim_mode == 1)
    {
        if (tex->cols > 1)
        {
            while (p)
            {
                p->m_texIdx[0]++;

                if (p->m_texIdx[0] >= tex->cols)
                {
                    p->m_texIdx[0] = 0;

                    if (cmd->anim_wrap_mode == 1)
                    {
                        p->m_texIdx[1]++;

                        if (p->m_texIdx[1] >= tex->rows)
                        {
                            p->m_texIdx[1] = 0;
                        }
                    }
                    else if (cmd->anim_wrap_mode == 2)
                    {
                        if (p->m_texIdx[1] == 0)
                        {
                            p->m_texIdx[1] = tex->rows - 1;
                        }
                        else
                        {
                            p->m_texIdx[1]--;
                        }
                    }
                    else if (cmd->anim_wrap_mode == 3)
                    {
                        p->m_texIdx[1]++;

                        if (p->m_texIdx[1] >= tex->rows)
                        {
                            p->m_texIdx[1] = tex->rows - 1;
                            p->m_texIdx[0] = tex->cols - 1;
                        }
                    }
                }

                p = p->m_next;
            }
        }
    }
    else if (cmd->anim_mode == 2)
    {
        if (tex->cols > 1)
        {
            while (p)
            {
                if (p->m_texIdx[0] == 0)
                {
                    p->m_texIdx[0] = tex->cols - 1;

                    if (cmd->anim_wrap_mode == 1)
                    {
                        p->m_texIdx[1]++;

                        if (p->m_texIdx[1] >= tex->rows)
                        {
                            p->m_texIdx[1] = 0;
                        }
                    }
                    else if (cmd->anim_wrap_mode == 2)
                    {
                        if (p->m_texIdx[1] == 0)
                        {
                            p->m_texIdx[1] = tex->rows - 1;
                        }
                        else
                        {
                            p->m_texIdx[1]--;
                        }
                    }
                }
                else
                {
                    p->m_texIdx[0]--;
                }

                p = p->m_next;
            }
        }
    }
    else if (cmd->anim_mode == 3)
    {
        if (tex->rows > 1)
        {
            while (p)
            {
                if (p->m_texIdx[1] == 0)
                {
                    p->m_texIdx[1] = tex->rows - 1;

                    if (cmd->anim_wrap_mode == 1)
                    {
                        p->m_texIdx[0]++;

                        if (p->m_texIdx[0] >= tex->cols)
                        {
                            p->m_texIdx[0] = 0;
                        }
                    }
                    else if (cmd->anim_wrap_mode == 2)
                    {
                        if (p->m_texIdx[0] == 0)
                        {
                            p->m_texIdx[0] = tex->cols - 1;
                        }
                        else
                        {
                            p->m_texIdx[0]--;
                        }
                    }
                }
                else
                {
                    p->m_texIdx[1]--;
                }

                p = p->m_next;
            }
        }
    }
    else if (cmd->anim_mode == 4)
    {
        if (tex->rows > 1)
        {
            while (p)
            {
                p->m_texIdx[1]++;

                if (p->m_texIdx[1] >= tex->rows)
                {
                    p->m_texIdx[1] = 0;

                    if (cmd->anim_wrap_mode == 1)
                    {
                        p->m_texIdx[0]++;

                        if (p->m_texIdx[0] >= tex->cols)
                        {
                            p->m_texIdx[0] = 0;
                        }
                    }
                    else if (cmd->anim_wrap_mode == 2)
                    {
                        if (p->m_texIdx[0] == 0)
                        {
                            p->m_texIdx[0] = tex->cols - 1;
                        }
                        else
                        {
                            p->m_texIdx[0]--;
                        }
                    }
                }

                p = p->m_next;
            }
        }
    }
}

// Grass: particles within two units of the player shrink toward a tenth of their size; others
// grow back to full size.
void xParCmdCustom_Grass_Update(zParCmd*, xParGroup* ps, F32 dt)
{
    xVec3* pos = (xVec3*)&globals.player.ent.model->Mat->pos;

    for (xPar* p = ps->m_root; p != NULL; p = p->m_next)
    {
        xVec3 d;
        xVec3Sub(&d, pos, &p->m_pos);
        if (xVec3Length2(&d) < 2.0f)
        {
            p->m_size -= 4.0f * dt;
            if (p->m_size < 0.1f)
            {
                p->m_size = 0.1f;
            }
        }
        else
        {
            p->m_size += 0.01f * dt;
            if (p->m_size > 1.0f)
            {
                p->m_size = 1.0f;
            }
        }
    }
}

// Hard-coded behaviours picked by type. Type 0 sizes particles by their distance from the player,
// type 2 stops them and type 10 is the grass.
void xParCmdCustom_Update(zParCmd* c, xParGroup* ps, F32 dt)
{
    xPar* p = ps->m_root;

    switch (((xParCmdCustom*)c->tasset)->type)
    {
    case 0:
    {
        F32 mdt = 4.0f * dt;
        xVec3 pos = *(xVec3*)&globals.player.ent.model->Mat->pos;
        for (; p != NULL; p = p->m_next)
        {
            xVec3 d;
            xVec3Sub(&d, &pos, &p->m_pos);

            F32 rSqr = xVec3Length2(&d);
            if (rSqr < 16.0f)
            {
                F32 oorSqr = mdt / xsqrt(rSqr + 0.001f);
                F32 sx = (d.x < 0.0f) ? 1.0f : -1.0f;
                F32 sz = (d.z < 0.0f) ? 1.0f : -1.0f;
                p->m_size = 1.0f / (10.0f * oorSqr * (sx + sz));
            }
        }
        break;
    }
    case 2:
        for (; p != NULL; p = p->m_next)
        {
            p->m_vel.x = 0.0f;
            p->m_vel.y = 0.0f;
            p->m_vel.z = 0.0f;
        }
        break;
    case 10:
        xParCmdCustom_Grass_Update(c, ps, dt);
        break;
    }
}
