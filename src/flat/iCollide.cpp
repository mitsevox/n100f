#include "iCollide.h"

#include "iMath3.h"
#include "xCollide.h"
#include "xEnt.h"
#include "xEnv.h"
#include "xMath.h"
#include "xMath3.h"
#include "xMathInlines.h"
#include "xModel.h"
#include "xVec3Inlines.h"

#include <rpcollis.h>
#include <rpworld.h>
#include <rwcore.h>
#include <string.h>

struct _xScene;

enum Dimension
{
    XDIM,
    YDIM,
    ZDIM
};

static F32 cbath;
static const xMat3x3* cbmat;
static U8 cbnumcs;
static U8 cbmaxcs;
static U8 FLOOR;
static U8 NEXT2;
static U8 OTHER;
static xRay3 cbray;
static RpIntersection cbisx_local;

static inline void xMat4x3Tolocal(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    o->x = v->x - m->pos.x;
    o->y = v->y - m->pos.y;
    o->z = v->z - m->pos.z;
    xMat3x3Tolocal(o, (const xMat3x3*)m, o);
}

void iCollideInit(_xScene* sc)
{
}

// Even-odd test of a point against a triangle, projected onto the plane the normal is most aligned with.
static S32 PointWithinTriangle(RwV3d* pt, RwV3d** tri, RwV3d* normal)
{
    Dimension dimension;
    F32 absX = iabs(normal->x);
    F32 absY = iabs(normal->y);
    F32 absZ = iabs(normal->z);
    S32 inside = 0;
    S32 i;
    S32 j;

    if (absZ > absY)
    {
        if (absZ > absX)
        {
            dimension = ZDIM;
        }
        else
        {
            dimension = XDIM;
        }
    }
    else if (absY > absX)
    {
        dimension = YDIM;
    }
    else
    {
        dimension = XDIM;
    }

    switch (dimension)
    {
    case XDIM:
        for (i = 0, j = 2; i < 3; j = i, i++)
        {
            if (((tri[i]->y <= pt->y && pt->y < tri[j]->y) || (tri[j]->y <= pt->y && pt->y < tri[i]->y)) &&
                (pt->z < tri[i]->z + ((pt->y - tri[i]->y) * (tri[j]->z - tri[i]->z)) / (tri[j]->y - tri[i]->y)))
            {
                inside = !inside;
            }
        }
        break;
    case YDIM:
        for (i = 0, j = 2; i < 3; j = i, i++)
        {
            if (((tri[i]->z <= pt->z && pt->z < tri[j]->z) || (tri[j]->z <= pt->z && pt->z < tri[i]->z)) &&
                (pt->x < tri[i]->x + ((pt->z - tri[i]->z) * (tri[j]->x - tri[i]->x)) / (tri[j]->z - tri[i]->z)))
            {
                inside = !inside;
            }
        }
        break;
    case ZDIM:
        for (i = 0, j = 2; i < 3; j = i, i++)
        {
            if (((tri[i]->y <= pt->y && pt->y < tri[j]->y) || (tri[j]->y <= pt->y && pt->y < tri[i]->y)) &&
                (pt->x < tri[i]->x + ((pt->y - tri[i]->y) * (tri[j]->x - tri[i]->x)) / (tri[j]->y - tri[i]->y)))
            {
                inside = !inside;
            }
        }
        break;
    }

    return inside;
}

static void FindNearestPointOnLine(RwV3d* result, RwV3d* point, RwV3d* start, RwV3d* end)
{
    RwV3d localResult;
    F32 mu;
    F32 lineLength2;

    mu = (point->x * (end->x - start->x) + point->y * (end->y - start->y) + point->z * (end->z - start->z)) -
         (start->x * (end->x - start->x) + start->y * (end->y - start->y) + start->z * (end->z - start->z));

    if (mu <= 0.0f)
    {
        localResult = *start;
    }
    else
    {
        lineLength2 = SQR(end->x - start->x) + SQR(end->y - start->y) + SQR(end->z - start->z);

        if (mu < lineLength2)
        {
            mu /= lineLength2;
            localResult.x = mu * (end->x - start->x);
            localResult.x += start->x;
            localResult.y = mu * (end->y - start->y);
            localResult.y += start->y;
            localResult.z = mu * (end->z - start->z);
            localResult.z += start->z;
        }
        else
        {
            localResult = *end;
        }
    }

    *result = localResult;
}

// Distance from a sphere centre to a triangle: to the plane when the projected centre lies inside the
// triangle, otherwise to the nearest edge point within the radius.
static void properSphereIsectTri(const xVec3* center, F32 radius, xVec3* tohit, F32* dist_ptr,
                                 RpCollisionTriangle* tri)
{
    xVec3 projPoint;
    F32 dist2plane;
    F32 dist;
    F32 dist2;
    F32 radius2;
    U32 i;
    xVec3 vertClosestPoint;
    xVec3 temp;
    F32 vertDist2;

    dist = *dist_ptr;
    dist2plane = xVec3Dot((xVec3*)&tri->normal, (xVec3*)&tri->point) - xVec3Dot((xVec3*)&tri->normal, center);

    xVec3SMul(&projPoint, (xVec3*)&tri->normal, dist2plane);
    xVec3Copy(tohit, &projPoint);
    xVec3AddTo(&projPoint, center);

    if (PointWithinTriangle((RwV3d*)&projPoint, tri->vertices, &tri->normal))
    {
        dist = iabs(dist2plane);
    }
    else
    {
        dist2 = SQ(dist);
        radius2 = SQ(radius);

        for (i = 0; i < 3; i++)
        {
            FindNearestPointOnLine((RwV3d*)&vertClosestPoint, (RwV3d*)&projPoint, tri->vertices[i],
                                   tri->vertices[(i + 1) % 3]);
            xVec3Sub(&temp, &vertClosestPoint, center);
            vertDist2 = xVec3Length2(&temp);

            if (vertDist2 < dist2 && vertDist2 < radius2)
            {
                dist2 = vertDist2;
                xVec3Copy(tohit, &temp);
            }
        }

        dist = xsqrt(dist2);
    }

    *dist_ptr = dist;
}

// Sorts sphere contacts into one floor slot and up to two wall slots (OTHER and NEXT2).
static RpCollisionTriangle* sphereHitsEnv3CB(RpIntersection* isx, RpWorldSector* sector,
                                             RpCollisionTriangle* tri, F32 dist, void* data)
{
    _xCollis* colls = (_xCollis*)data;
    F32 radius = isx->t.sphere.radius;
    xVec3 tohit;
    xVec3 hdng;
    U8 idx;
    F32 dot;
    F32 odot;
    F32 ndot;
    F32 scale;

    dist = 1e19f;
    properSphereIsectTri((xVec3*)&isx->t.sphere.center, radius, &tohit, &dist, tri);

    if (0.0f == dist)
    {
        return tri;
    }

    if (dist > radius)
    {
        return tri;
    }

    if (SQ(tohit.y) > SQ(tohit.x) + SQ(tohit.z))
    {
        if (FLOOR == 0xFF)
        {
            idx = cbnumcs;
            FLOOR = cbnumcs;
            cbnumcs++;
        }
        else if (tohit.y < 0.0f)
        {
            if (colls[FLOOR].hdng.y > 0.0f || dist < colls[FLOOR].dist ||
                (iabs(dist - colls[FLOOR].dist) < 0.001f && tri->normal.y > colls[FLOOR].norm.y))
            {
                idx = FLOOR;
            }
            else
            {
                return tri;
            }
        }
        else if (colls[FLOOR].hdng.y > 0.0f &&
                 (dist < colls[FLOOR].dist ||
                  (iabs(dist - colls[FLOOR].dist) < 0.001f && tri->normal.y > colls[FLOOR].norm.y)))
        {
            idx = FLOOR;
        }
        else
        {
            return tri;
        }

        xVec3SMul(&hdng, &tohit, 1.0f / dist);
    }
    else if (NEXT2 == 0xFF)
    {
        if (OTHER == 0xFF)
        {
            idx = cbnumcs;
            OTHER = cbnumcs;
            cbnumcs++;
            xVec3SMul(&hdng, &tohit, 1.0f / dist);
        }
        else
        {
            xVec3SMul(&hdng, &tohit, 1.0f / dist);
            dot = xVec3Dot(&hdng, &colls[OTHER].hdng);

            if (iabs(dot) > 0.70710677f)
            {
                if (dist < colls[OTHER].dist)
                {
                    idx = OTHER;
                }
                else
                {
                    return tri;
                }
            }
            else
            {
                idx = cbnumcs;
                NEXT2 = cbnumcs;
                cbnumcs++;

                if (dist < colls[OTHER].dist)
                {
                    NEXT2 = OTHER;
                    OTHER = idx;
                }
            }
        }
    }
    else
    {
        if (dist > colls[NEXT2].dist)
        {
            return tri;
        }

        xVec3SMul(&hdng, &tohit, 1.0f / dist);
        odot = xVec3Dot(&hdng, &colls[OTHER].hdng);

        if (dist < colls[OTHER].dist)
        {
            if (iabs(odot) > 0.70710677f)
            {
                ndot = xVec3Dot(&hdng, &colls[NEXT2].hdng);

                if (iabs(ndot) > 0.70710677f)
                {
                    if (NEXT2 < OTHER)
                    {
                        OTHER = NEXT2;
                    }

                    NEXT2 = 0xFF;
                    idx = OTHER;
                    cbnumcs--;
                    colls[cbnumcs].flags &= ~1;
                    colls[cbnumcs].dist = 1e19f;
                }
                else
                {
                    idx = OTHER;
                }
            }
            else
            {
                idx = NEXT2;
            }
        }
        else
        {
            if (iabs(odot) > 0.70710677f)
            {
                return tri;
            }

            idx = NEXT2;
        }
    }

    colls[idx].dist = dist;
    xVec3Copy(&colls[idx].hdng, &hdng);

    if (colls->flags & 0x200)
    {
        xVec3Copy(&colls[idx].norm, (xVec3*)&tri->normal);
    }

    if (colls->flags & 0x400)
    {
        xVec3Copy(&colls[idx].tohit, &tohit);
    }

    if (colls->flags & 0x800)
    {
        scale = dist - radius;
        xVec3Copy(&colls[idx].depen, &hdng);
        xVec3SMulBy(&colls[idx].depen, scale);
    }

    colls[idx].flags |= 1;

    if (sector)
    {
        colls[idx].oid = sector->polygons[tri->index].matIndex;
    }
    else
    {
        colls[idx].oid = tri->index;
    }

    if (cbnumcs == cbmaxcs)
    {
        return NULL;
    }

    return tri;
}

// Keeps the nearest contact in each of the six direction slots around cbmat.
static RpCollisionTriangle* sphereHitsEnv4CB(RpIntersection* isx, RpWorldSector* sector,
                                             RpCollisionTriangle* tri, F32 dist, void* data)
{
    _xCollis* colls = (_xCollis*)data;
    F32 radius = isx->t.sphere.radius;
    xVec3 tohit;
    U8 idx;
    _xCollis* c;
    F32 ddist;

    dist = 1e19f;
    properSphereIsectTri((xVec3*)&isx->t.sphere.center, radius, &tohit, &dist, tri);

    if (0.0f == dist)
    {
        return tri;
    }

    if (dist > radius)
    {
        return tri;
    }

    idx = xCollideGetCollsIdx(&tohit, cbmat);
    c = &colls[idx];
    ddist = dist - c->dist;

    if (!(ddist < 0.0f || (ddist < 0.001f && c->norm.y > tri->normal.y)))
    {
        return tri;
    }

    c->dist = dist;
    c->tohit = tohit;
    xVec3Copy(&c->norm, (xVec3*)&tri->normal);
    c->flags |= 1;

    if (sector)
    {
        c->oid = sector->polygons[tri->index].matIndex;
    }
    else
    {
        c->oid = tri->index;
    }

    return tri;
}

static RpCollisionTriangle* sphereHitsModel3CB(RpIntersection* isx, RpCollisionTriangle* tri, F32 dist,
                                               void* data)
{
    return sphereHitsEnv3CB(&cbisx_local, NULL, tri, dist, data);
}

static RpCollisionTriangle* rayHitsEnvCB(RpIntersection* isx, RpWorldSector* sector, RpCollisionTriangle* tri,
                                         F32 dist, void* data)
{
    _xCollis* coll = (_xCollis*)data;

    dist *= cbray.max_t;

    if (dist >= coll->dist)
    {
        return tri;
    }

    if (sector)
    {
        coll->oid = sector->polygons[tri->index].matIndex;
    }
    else
    {
        coll->oid = tri->index;
    }

    coll->dist = dist;
    coll->flags |= 1;

    if (coll->flags & 0x200)
    {
        xVec3Copy(&coll->norm, (xVec3*)&tri->normal);
    }

    return tri;
}

static RpCollisionTriangle* rayHitsEnvBackwardCB(RpIntersection* isx, RpWorldSector* sector,
                                                 RpCollisionTriangle* tri, F32 dist, void* data)
{
    _xCollis* coll = (_xCollis*)data;

    dist = 1.0f - dist;
    dist = dist * cbray.max_t;

    if (dist >= coll->dist)
    {
        return tri;
    }

    if (sector)
    {
        coll->oid = sector->polygons[tri->index].matIndex;
    }
    else
    {
        coll->oid = tri->index;
    }

    coll->dist = dist;
    coll->flags |= 1;

    return tri;
}

static RpCollisionTriangle* rayHitsModelCB(RpIntersection* isx, RpCollisionTriangle* tri, F32 dist, void* data)
{
    return rayHitsEnvCB(isx, NULL, tri, dist, data);
}

static RpCollisionTriangle* rayHitsModelBackwardCB(RpIntersection* isx, RpCollisionTriangle* tri, F32 dist,
                                                   void* data)
{
    return rayHitsEnvBackwardCB(isx, NULL, tri, dist, data);
}

S32 iSphereHitsEnv3(const xSphere* b, const _xEnv* env, _xCollis* colls, U8 ncolls, F32 sth)
{
    RpIntersection isx;
    U8 idx;

    isx.type = rpINTERSECTSPHERE;
    memcpy(&isx.t.sphere, b, sizeof(xSphere));

    for (idx = 0; idx < ncolls; idx++)
    {
        colls[idx].flags &= ~1;
        colls[idx].dist = 1e19f;
    }

    cbath = sth;
    cbnumcs = 0;
    cbmaxcs = ncolls;
    OTHER = 0xFF;
    NEXT2 = 0xFF;
    FLOOR = 0xFF;

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, sphereHitsEnv3CB, colls);

    if (env->geom->collision)
    {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, sphereHitsEnv3CB, colls);
    }

    return cbnumcs;
}

S32 iSphereHitsEnv4(const xSphere* b, const _xEnv* env, const xMat3x3* mat, _xCollis* colls)
{
    RpIntersection isx;
    _xCollis* c;
    _xCollis* cend;
    S32 numcs;
    F32 s;

    isx.type = rpINTERSECTSPHERE;
    memcpy(&isx.t.sphere, b, sizeof(xSphere));

    cbmat = mat;
    cend = colls + 6;

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, sphereHitsEnv4CB, colls);

    for (c = colls; c < cend; c++)
    {
        if (c->flags & 1)
        {
            c->oid += 0x10000;
        }
    }

    if (env->geom->collision)
    {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, sphereHitsEnv4CB, colls);

        for (c = colls; c < cend; c++)
        {
            if (c->flags & 1)
            {
                if (c->oid < 0x10000)
                {
                    c->oid += 0x10000;
                }
                else
                {
                    c->oid -= 0x10000;
                }
            }
        }
    }

    numcs = 0;

    for (c = colls; c < cend; c++)
    {
        if (c->flags & 1)
        {
            numcs++;

            s = 1.0f / c->dist;
            c->hdng.x = c->tohit.x * s;
            c->hdng.y = c->tohit.y * s;
            c->hdng.z = c->tohit.z * s;

            s = c->dist - b->r;
            s = (0.0f < s) ? 0.0f : s;

            c->depen.x = c->hdng.x * s;
            c->depen.y = c->hdng.y * s;
            c->depen.z = c->hdng.z * s;

            if (xVec3Dot(&c->hdng, &c->norm) > 0.0f)
            {
                c->norm.x = -c->norm.x;
                c->norm.y = -c->norm.y;
                c->norm.z = -c->norm.z;
            }
        }
    }

    return numcs;
}

S32 iSphereHitsModel3(const xSphere* b, const xModelInstance* m, _xCollis* colls, U8 ncolls, F32 sth)
{
    RpIntersection isx;
    xMat4x3* mat;
    F32 mscale;
    xMat3x3 mnormal;
    U8 idx;

    isx.type = rpINTERSECTSPHERE;
    memcpy(&isx.t.sphere, b, sizeof(xSphere));

    mat = (xMat4x3*)m->Mat;
    RwFrameTransform((RwFrame*)m->Data->object.object.parent, (RwMatrix*)mat, rwCOMBINEREPLACE);
    mscale = xVec3Length(&mat->right);
    xMat3x3Normalize(&mnormal, (const xMat3x3*)mat);
    xMat4x3Tolocal((xVec3*)&cbisx_local.t.sphere.center, mat, &b->center);
    cbisx_local.t.sphere.radius = b->r / mscale;

    for (idx = 0; idx < ncolls; idx++)
    {
        colls[idx].flags &= ~1;
        colls[idx].dist = 1e19f;
    }

    cbath = sth;
    cbnumcs = 0;
    cbmaxcs = ncolls;
    OTHER = 0xFF;
    NEXT2 = 0xFF;
    FLOOR = 0xFF;

    RpAtomicForAllIntersections(m->Data, &isx, sphereHitsModel3CB, colls);

    if (colls->flags & 0x1F00)
    {
        for (idx = 0; idx < cbnumcs; idx++)
        {
            if (colls[idx].flags & 1)
            {
                xMat3x3RMulVec(&colls[idx].tohit, (const xMat3x3*)mat, &colls[idx].tohit);
                xMat3x3RMulVec(&colls[idx].depen, (const xMat3x3*)mat, &colls[idx].depen);
                xMat3x3RMulVec(&colls[idx].hdng, &mnormal, &colls[idx].hdng);
                xMat3x3RMulVec(&colls[idx].norm, &mnormal, &colls[idx].norm);
                colls[idx].dist *= mscale;
            }
        }
    }

    return cbnumcs;
}

U32 iRayHitsEnv(const xRay3* r, const _xEnv* env, _xCollis* coll)
{
    RpIntersection isx;
    F32 len;

    isx.type = rpINTERSECTLINE;

    if (r->flags & 0x400)
    {
        xVec3SMul((xVec3*)&isx.t.line.start, &r->dir, r->min_t);
        xVec3Add((xVec3*)&isx.t.line.start, &r->origin, (xVec3*)&isx.t.line.start);
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

    coll->flags &= ~1;
    coll->dist = 1e19f;
    cbray = *r;

    if (!(r->flags & 0x800))
    {
        cbray.max_t = 1.0f;
    }

    if (r->flags & 0x400)
    {
        cbray.max_t -= cbray.min_t;
    }

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvCB, coll);

    if (env->geom->collision)
    {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, rayHitsEnvCB, coll);
    }

    {
        RwV3d temp = isx.t.line.start;
        isx.t.line.start = isx.t.line.end;
        isx.t.line.end = temp;
    }

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvBackwardCB, coll);

    if (env->geom->collision)
    {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, rayHitsEnvBackwardCB, coll);
    }

    if (r->flags & 0x400)
    {
        coll->dist += cbray.min_t;
    }

    return coll->flags & 1;
}

U32 iRayHitsCamEnv(const xRay3* r, const _xEnv* env, _xCollis* coll)
{
    RpIntersection isx;
    F32 len;

    isx.type = rpINTERSECTLINE;

    if (r->flags & 0x400)
    {
        xVec3SMul((xVec3*)&isx.t.line.start, &r->dir, r->min_t);
        xVec3Add((xVec3*)&isx.t.line.start, &r->origin, (xVec3*)&isx.t.line.start);
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

    coll->flags &= ~1;
    coll->dist = 1e19f;
    cbray = *r;

    if (!(r->flags & 0x800))
    {
        cbray.max_t = 1.0f;
    }

    if (r->flags & 0x400)
    {
        cbray.max_t -= cbray.min_t;
    }

    if (env->geom->camera)
    {
        RpCollisionWorldForAllIntersections(env->geom->camera, &isx, rayHitsEnvCB, coll);
    }
    else
    {
        RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvCB, coll);
    }

    {
        RwV3d temp = isx.t.line.start;
        isx.t.line.start = isx.t.line.end;
        isx.t.line.end = temp;
    }

    if (env->geom->camera)
    {
        RpCollisionWorldForAllIntersections(env->geom->camera, &isx, rayHitsEnvBackwardCB, coll);
    }
    else
    {
        RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvBackwardCB, coll);
    }

    if (r->flags & 0x400)
    {
        coll->dist += cbray.min_t;
    }

    return coll->flags & 1;
}

U32 iRayHitsModel(const xRay3* r, const xModelInstance* m, _xCollis* coll)
{
    RpIntersection isx;
    F32 len;
    xMat4x3* mat;

    isx.type = rpINTERSECTLINE;

    if (r->flags & 0x400)
    {
        xVec3SMul((xVec3*)&isx.t.line.start, &r->dir, r->min_t);
        xVec3Add((xVec3*)&isx.t.line.start, &r->origin, (xVec3*)&isx.t.line.start);
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

    cbray = *r;

    if (!(r->flags & 0x800))
    {
        cbray.max_t = 1.0f;
    }

    if (r->flags & 0x400)
    {
        cbray.max_t -= cbray.min_t;
    }

    mat = (xMat4x3*)m->Mat;
    RwFrameTransform((RwFrame*)m->Data->object.object.parent, (RwMatrix*)mat, rwCOMBINEREPLACE);
    len = xVec3Length(&mat->right);
    cbray.max_t /= len;

    coll->flags &= ~1;
    coll->dist = 1e19f;

    RpAtomicForAllIntersections(m->Data, &isx, rayHitsModelCB, coll);

    {
        RwV3d temp = isx.t.line.start;
        isx.t.line.start = isx.t.line.end;
        isx.t.line.end = temp;
    }

    RpAtomicForAllIntersections(m->Data, &isx, rayHitsModelBackwardCB, coll);

    if (r->flags & 0x400)
    {
        coll->dist += cbray.min_t;
    }

    return coll->flags & 1;
}

void iBoxForModel(_xBox* o, const xModelInstance* m)
{
    iBoxForModelLocal(o, m);
    xVec3AddTo(&o->upper, (xVec3*)&m->Mat->pos);
    xVec3AddTo(&o->lower, (xVec3*)&m->Mat->pos);
}

void iBoxForModelLocal(_xBox* o, const xModelInstance* m)
{
    RpGeometry* geom = m->Data->geometry;
    RpMorphTarget* mtgt;
    U16 idx;

    if (geom->numMorphTargets > 1 || geom->numMorphTargets >= 1)
    {
        mtgt = geom->morphTarget;
        iBoxInitBoundVec(o, (xVec3*)mtgt->verts);

        for (idx = 1; idx < geom->numVertices; idx++)
        {
            iBoxBoundVec(o, o, (xVec3*)&mtgt->verts[idx]);
        }
    }
}
