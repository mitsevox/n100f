#include "iCollideFast.h"

#include "xEnv.h"
#include "xMath3.h"
#include "xModel.h"

#include <rpcollis.h>
#include <rpworld.h>
#include <rwcore.h>

static U32 hitzhit;

void iCollideFastInit(_xScene* sc)
{
}

// The fast ray tests only ask whether anything is hit, so the first triangle ends the walk.
static RpCollisionTriangle* rayHitsModelFastCB(RpIntersection* isx, RpCollisionTriangle* tri,
                                               F32 dist, void* data)
{
    hitzhit = 1;
    return NULL;
}

static RpCollisionTriangle* rayHitsEnvFastCB(RpIntersection* isx, RpWorldSector* sector,
                                             RpCollisionTriangle* tri, F32 dist, void* data)
{
    hitzhit = 1;
    return NULL;
}

// Hit-or-miss ray against one model. The line is tested from both ends so that triangles
// facing away from the ray's origin count too.
U32 iRayHitsModelFast(const xRay3* r, const xModelInstance* m)
{
    RpIntersection isx;
    F32 len;

    isx.type = rpINTERSECTLINE;

    if (r->flags & 0x400)
    {
        xVec3 ofs;
        xVec3SMul(&ofs, &r->dir, r->min_t);
        xVec3Add((xVec3*)&isx.t.line.start, &r->origin, &ofs);
    }
    else
    {
        isx.t.line.start.x = r->origin.x;
        isx.t.line.start.y = r->origin.y;
        isx.t.line.start.z = r->origin.z;
    }

    if (r->flags & 0x800)
    {
        if (r->flags & 0x400)
        {
            len = r->max_t - r->min_t;
        }
        else
        {
            len = r->max_t;
        }

        isx.t.line.end.x = r->dir.x * len;
        isx.t.line.end.y = r->dir.y * len;
        isx.t.line.end.z = r->dir.z * len;
    }
    else
    {
        isx.t.line.end.x = r->dir.x;
        isx.t.line.end.y = r->dir.y;
        isx.t.line.end.z = r->dir.z;
    }

    isx.t.line.end.x = isx.t.line.start.x + isx.t.line.end.x;
    isx.t.line.end.y = isx.t.line.start.y + isx.t.line.end.y;
    isx.t.line.end.z = isx.t.line.start.z + isx.t.line.end.z;

    hitzhit = 0;
    RpAtomicForAllIntersections(m->Data, &isx, rayHitsModelFastCB, NULL);

    if (hitzhit)
    {
        return 1;
    }

    {
        RwV3d temp = isx.t.line.start;
        isx.t.line.start = isx.t.line.end;
        isx.t.line.end = temp;
    }

    RpAtomicForAllIntersections(m->Data, &isx, rayHitsModelFastCB, NULL);

    return hitzhit;
}

// Hit-or-miss ray against the level's world geometry only, from both ends.
U32 iRayHitsEnvFast(const xRay3* r, const _xEnv* env)
{
    RpIntersection isx;
    F32 len;

    isx.type = rpINTERSECTLINE;

    if (r->flags & 0x400)
    {
        xVec3 ofs;
        xVec3SMul(&ofs, &r->dir, r->min_t);
        xVec3Add((xVec3*)&isx.t.line.start, &r->origin, &ofs);
    }
    else
    {
        isx.t.line.start.x = r->origin.x;
        isx.t.line.start.y = r->origin.y;
        isx.t.line.start.z = r->origin.z;
    }

    if (r->flags & 0x800)
    {
        if (r->flags & 0x400)
        {
            len = r->max_t - r->min_t;
        }
        else
        {
            len = r->max_t;
        }

        isx.t.line.end.x = r->dir.x * len;
        isx.t.line.end.y = r->dir.y * len;
        isx.t.line.end.z = r->dir.z * len;
    }
    else
    {
        isx.t.line.end.x = r->dir.x;
        isx.t.line.end.y = r->dir.y;
        isx.t.line.end.z = r->dir.z;
    }

    isx.t.line.end.x = isx.t.line.start.x + isx.t.line.end.x;
    isx.t.line.end.y = isx.t.line.start.y + isx.t.line.end.y;
    isx.t.line.end.z = isx.t.line.start.z + isx.t.line.end.z;

    hitzhit = 0;
    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);

    if (hitzhit)
    {
        return 1;
    }

    {
        RwV3d temp = isx.t.line.start;
        isx.t.line.start = isx.t.line.end;
        isx.t.line.end = temp;
    }

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);

    return hitzhit;
}

// As iRayHitsEnvFast, but also tests the level's separate collision-only world when it has one.
U32 iRayHitsEnvSpecialFast(const xRay3* r, const _xEnv* env)
{
    RpIntersection isx;
    F32 len;

    isx.type = rpINTERSECTLINE;

    if (r->flags & 0x400)
    {
        xVec3 ofs;
        xVec3SMul(&ofs, &r->dir, r->min_t);
        xVec3Add((xVec3*)&isx.t.line.start, &r->origin, &ofs);
    }
    else
    {
        isx.t.line.start.x = r->origin.x;
        isx.t.line.start.y = r->origin.y;
        isx.t.line.start.z = r->origin.z;
    }

    if (r->flags & 0x800)
    {
        if (r->flags & 0x400)
        {
            len = r->max_t - r->min_t;
        }
        else
        {
            len = r->max_t;
        }

        isx.t.line.end.x = r->dir.x * len;
        isx.t.line.end.y = r->dir.y * len;
        isx.t.line.end.z = r->dir.z * len;
    }
    else
    {
        isx.t.line.end.x = r->dir.x;
        isx.t.line.end.y = r->dir.y;
        isx.t.line.end.z = r->dir.z;
    }

    isx.t.line.end.x = isx.t.line.start.x + isx.t.line.end.x;
    isx.t.line.end.y = isx.t.line.start.y + isx.t.line.end.y;
    isx.t.line.end.z = isx.t.line.start.z + isx.t.line.end.z;

    hitzhit = 0;
    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);

    if (hitzhit)
    {
        return 1;
    }

    if (env->geom->collision)
    {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, rayHitsEnvFastCB, NULL);
    }

    if (hitzhit)
    {
        return 1;
    }

    {
        RwV3d temp = isx.t.line.start;
        isx.t.line.start = isx.t.line.end;
        isx.t.line.end = temp;
    }

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);

    if (hitzhit)
    {
        return 1;
    }

    if (env->geom->collision)
    {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, rayHitsEnvFastCB, NULL);
    }

    return hitzhit;
}
