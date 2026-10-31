#include "xCollideFast.h"
#include "iCollideFast.h"
#include "iMath3.h"
#include "xEnt.h"
#include "xMath3.h"
#include "xMathInlines.h"
#include "xQuickCull.h"
#include "xScene.h"

// The fast tests only answer whether a ray hits something, without the distance or the
// surface, for line-of-sight and ground checks.

// Bound types: 1 sphere, 2 axis-aligned box, 3 cylinder, 4 oriented box.

inline U32 xRayHitsSphereFast(const xRay3* r, const xSphere* s)
{
    xVec3 length;
    xVec3Sub(&length, &r->origin, &s->center);
    F32 dVar3 = xVec3Dot(&length, &length) - SQ(s->r);
    if (dVar3 <= 0.0f)
    {
        return 1;
    }

    if (r->flags & (1 << 11) && dVar3 > r->max_t * (2.0f * s->r + r->max_t))
    {
        return 0;
    }

    F32 dot = xVec3Dot(&length, &r->dir);
    return (dot >= 0.0f) ? 0 : (SQ(dot) >= dVar3);
}

inline U32 xRayHitsBoxFast(const xRay3* r, const _xBox* b)
{
    xIsect isect;
    iBoxIsectRay(b, r, &isect);
    return isect.penned <= 0.0f || isect.contained <= 0.0f;
}

// The cylinder form of the test above; the name is this project's.
inline U32 xRayHitsCylinderFast(const xRay3* r, const _xCylinder* c)
{
    xIsect isect;
    iCylinderIsectRay(c, r, &isect);
    return isect.penned <= 0.0f || isect.contained <= 0.0f;
}

// Whether the ray hits any static, dynamic or NPC entity that can be collided with, or the
// level geometry. Static and dynamic entities whose collision level is 5 must also hit their
// model; NPCs count on their bound alone.
U32 xRayHitsSceneFast(const _xRay3* r, const _xScene* sc)
{
    xQCData qcr;
    S32 i;

    xQuickCullForRay(&qcr, r);

    for (i = 0; i < sc->num_stats; i++)
    {
        _xEnt* ent = sc->stats[i];
        if (ent->chkby && xQuickCullIsects(&qcr, &ent->bound.qcd) &&
            xRayHitsBoundFast(r, &ent->bound))
        {
            if (ent->collLev == 5)
            {
                if (iRayHitsModelFast(r, ent->model))
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
    }

    for (i = 0; i < sc->num_dyns; i++)
    {
        _xEnt* ent = sc->dyns[i];
        if (ent->chkby && xQuickCullIsects(&qcr, &ent->bound.qcd) &&
            xRayHitsBoundFast(r, &ent->bound))
        {
            if (ent->collLev == 5)
            {
                if (iRayHitsModelFast(r, ent->model))
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
    }

    for (i = 0; i < sc->num_npcs; i++)
    {
        _xEnt* ent = sc->npcs[i];
        if (ent->chkby && xQuickCullIsects(&qcr, &ent->bound.qcd) &&
            xRayHitsBoundFast(r, &ent->bound))
        {
            return 1;
        }
    }

    return iRayHitsEnvSpecialFast(r, sc->env);
}

// Whether the ray hits the level geometry; the platform layer does the test.
U32 xRayHitsEnvFast(const _xRay3* r, const _xEnv* env)
{
    return iRayHitsEnvFast(r, env);
}

// Whether the ray crosses the bound. An oriented box is tested in its own space, with the ray
// brought in by the box's matrix and its length scaled by the matrix's scale.
U32 xRayHitsBoundFast(const _xRay3* r, const _xBound* b)
{
    xRay3 lr;
    xMat3x3 mn;

    if (b->type == 1)
    {
        return xRayHitsSphereFast(r, &b->sph);
    }
    else if (b->type == 4)
    {
        F32 scale = xsqrt(xVec3Length2(&b->mat->right));
        xMat3x3Normalize(&mn, (const xMat3x3*)b->mat);
        xVec3Sub(&lr.origin, &r->origin, &b->mat->pos);
        xMat3x3Tolocal(&lr.origin, (const xMat3x3*)b->mat, &lr.origin);
        xMat3x3Tolocal(&lr.dir, &mn, &r->dir);
        lr.max_t = r->max_t / scale;
        lr.min_t = r->min_t / scale;
        lr.flags = r->flags;
        return xRayHitsBoxFast(&lr, &b->box.box);
    }
    else if (b->type == 2)
    {
        return xRayHitsBoxFast(r, &b->box.box);
    }
    else if (b->type == 3)
    {
        return xRayHitsCylinderFast(r, &b->cyl);
    }

    return 0;
}

// Fast-collision setup for a scene; the platform layer does the work.
void xCollideFastInit(_xScene* sc)
{
    iCollideFastInit(sc);
}
