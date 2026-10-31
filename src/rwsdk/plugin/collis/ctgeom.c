#include <rpcollis.h>
#include <rtintsec.h>

// Sets _out to the point _blend of the way from _start to _end.
#define V3D_BLEND(_out, _start, _end, _blend)                                                      \
    do                                                                                             \
    {                                                                                              \
        RwV3dSubMacro((_out), (_end), (_start));                                                   \
        RwV3dScaleMacro((_out), (_out), (_blend));                                                 \
        RwV3dAddMacro((_out), (_out), (_start));                                                   \
    } while (0)

typedef struct GeomCallBack GeomCallBack;
struct GeomCallBack
{
    RpIntersection* intersection;
    RpIntersectionCallBackGeometryTriangle func;
    void* data;
};

typedef struct LineCollParam LineCollParam;
struct LineCollParam
{
    RpGeometry* geometry;
    GeomCallBack* callBack;
    RwLine* line;
    RwV3d delta;
};

typedef struct SphereCollParam SphereCollParam;
struct SphereCollParam
{
    RpGeometry* geometry;
    GeomCallBack* callBack;
    RwSphere* sphere;
    F32 recipRadius;
};

// Tests the line against the triangles of one leaf of a geometry's collision tree, calling back
// for each hit.
static S32 GeomLeafNodeForAllLineIntersections(S32 numTriangles, S32 triOffset, void* data)
{
    LineCollParam* isData = (LineCollParam*)data;
    RpGeometry* geometry = isData->geometry;
    RwV3d* vertices = geometry->morphTarget->verts;
    RpTriangle* triangles = geometry->triangles;
    GeomCallBack* cb = isData->callBack;
    U16* triIndex = COLLISIONDATA(geometry, _rpCollisionGeometryDataOffset)->triangleMap + triOffset;

    while (numTriangles--)
    {
        S32 index = *triIndex;
        RpTriangle* tri = &triangles[index];
        RwV3d* v0 = &vertices[tri->vertIndex[0]];
        RwV3d* v1 = &vertices[tri->vertIndex[1]];
        RwV3d* v2 = &vertices[tri->vertIndex[2]];
        S32 result;
        F32 distance;

        LINETRIANGLE_INTERSECT(result, &isData->line->start, &isData->delta, v0, v1, v2, distance);

        if (result)
        {
            RpCollisionTriangle collisionTri;

            collisionTri.point = *v0;
            collisionTri.index = index;
            TRIANGLE_NORMAL(&collisionTri.normal, v0, v1, v2);
            collisionTri.vertices[0] = v0;
            collisionTri.vertices[1] = v1;
            collisionTri.vertices[2] = v2;

            if (!cb->func(cb->intersection, &collisionTri, distance, cb->data))
            {
                return FALSE;
            }
        }

        triIndex++;
    }

    return TRUE;
}

// Tests the sphere against the triangles of one leaf of a geometry's collision tree, calling
// back for each hit.
static S32 GeomLeafNodeForAllSphereIntersections(S32 numTriangles, S32 triOffset, void* data)
{
    SphereCollParam* isData = (SphereCollParam*)data;
    RpGeometry* geometry = isData->geometry;
    RwV3d* vertices = geometry->morphTarget->verts;
    RpTriangle* triangles = geometry->triangles;
    GeomCallBack* cb = isData->callBack;
    U16* triIndex = COLLISIONDATA(geometry, _rpCollisionGeometryDataOffset)->triangleMap + triOffset;

    while (numTriangles--)
    {
        RpTriangle* tri = &triangles[*triIndex];
        RwV3d* v0 = &vertices[tri->vertIndex[0]];
        RwV3d* v1 = &vertices[tri->vertIndex[1]];
        RwV3d* v2 = &vertices[tri->vertIndex[2]];
        RpCollisionTriangle collisionTri;
        F32 distance;

        if (RtIntersectionSphereTriangle(isData->sphere, v0, v1, v2, &collisionTri.normal,
                                         &distance))
        {
            collisionTri.point = *v0;
            collisionTri.index = *triIndex;
            collisionTri.vertices[0] = v0;
            collisionTri.vertices[1] = v1;
            collisionTri.vertices[2] = v2;
            distance *= isData->recipRadius;

            if (!cb->func(cb->intersection, &collisionTri, distance, cb->data))
            {
                return FALSE;
            }
        }

        triIndex++;
    }

    return TRUE;
}

// Calls back for each triangle of the geometry's base morph target the line hits, through the
// collision tree when the geometry has one.
static RpGeometry* GeometryForAllLineIntersections(RpGeometry* geometry, RwLine* testLine,
                                                   GeomCallBack* cb)
{
    RpCollisionData* collData = COLLISIONDATA(geometry, _rpCollisionGeometryDataOffset);

    if (collData)
    {
        RpV3dGradient grad;
        LineCollParam isData;
        F32 recip;

        isData.callBack = cb;
        isData.geometry = geometry;
        isData.line = testLine;
        RwV3dSubMacro(&isData.delta, &testLine->end, &testLine->start);

        recip = (isData.delta.x != 0.0f) ? 1.0f / isData.delta.x : 0.0f;
        grad.dydx = isData.delta.y * recip;
        grad.dzdx = isData.delta.z * recip;

        recip = (isData.delta.y != 0.0f) ? 1.0f / isData.delta.y : 0.0f;
        grad.dxdy = isData.delta.x * recip;
        grad.dzdy = isData.delta.z * recip;

        recip = (isData.delta.z != 0.0f) ? 1.0f / isData.delta.z : 0.0f;
        grad.dxdz = isData.delta.x * recip;
        grad.dydz = isData.delta.y * recip;

        _rpCollBSPTreeForAllLineLeafNodeIntersections(collData->tree, testLine, &grad,
                                                      GeomLeafNodeForAllLineIntersections,
                                                      &isData);
    }
    else
    {
        RwV3d* vertices = geometry->morphTarget->verts;
        RpTriangle* triangle = geometry->triangles;
        RwV3d delta;
        S32 iTri;

        RwV3dSubMacro(&delta, &testLine->end, &testLine->start);

        for (iTri = 0; iTri < geometry->numTriangles; iTri++, triangle++)
        {
            RwV3d* v0 = &vertices[triangle->vertIndex[0]];
            RwV3d* v1 = &vertices[triangle->vertIndex[1]];
            RwV3d* v2 = &vertices[triangle->vertIndex[2]];
            S32 result;
            F32 distance;

            LINETRIANGLE_INTERSECT(result, &testLine->start, &delta, v0, v1, v2, distance);

            if (result)
            {
                RpCollisionTriangle collisionTri;

                collisionTri.point = *v0;
                collisionTri.index = iTri;
                TRIANGLE_NORMAL(&collisionTri.normal, v0, v1, v2);
                collisionTri.vertices[0] = v0;
                collisionTri.vertices[1] = v1;
                collisionTri.vertices[2] = v2;

                if (!cb->func(cb->intersection, &collisionTri, distance, cb->data))
                {
                    return NULL;
                }
            }
        }
    }

    return geometry;
}

// Calls back for each triangle of the geometry's base morph target the sphere touches, through
// the collision tree when the geometry has one.
static RpGeometry* GeometryForAllSphereIntersections(RpGeometry* geometry, RwSphere* testSphere,
                                                     GeomCallBack* cb)
{
    RpCollisionData* collData = COLLISIONDATA(geometry, _rpCollisionGeometryDataOffset);

    if (collData)
    {
        SphereCollParam isData;
        RwBBox bbox;

        isData.callBack = cb;
        isData.geometry = geometry;
        isData.sphere = testSphere;
        isData.recipRadius = 1.0f / testSphere->radius;

        bbox.sup = testSphere->center;
        bbox.inf = testSphere->center;
        bbox.inf.x -= testSphere->radius;
        bbox.inf.y -= testSphere->radius;
        bbox.inf.z -= testSphere->radius;
        bbox.sup.x += testSphere->radius;
        bbox.sup.y += testSphere->radius;
        bbox.sup.z += testSphere->radius;

        _rpCollBSPTreeForAllBoxLeafNodeIntersections(collData->tree, &bbox,
                                                     GeomLeafNodeForAllSphereIntersections,
                                                     &isData);
    }
    else
    {
        F32 recipRadius = 1.0f / testSphere->radius;
        RwV3d* vertices = geometry->morphTarget->verts;
        RpTriangle* triangle = geometry->triangles;
        S32 iTri;

        for (iTri = 0; iTri < geometry->numTriangles; iTri++, triangle++)
        {
            RwV3d* v0 = &vertices[triangle->vertIndex[0]];
            RwV3d* v1 = &vertices[triangle->vertIndex[1]];
            RwV3d* v2 = &vertices[triangle->vertIndex[2]];
            RpCollisionTriangle collisionTri;
            F32 distance;

            if (RtIntersectionSphereTriangle(testSphere, v0, v1, v2, &collisionTri.normal,
                                             &distance))
            {
                collisionTri.point = *v0;
                collisionTri.index = iTri;
                collisionTri.vertices[0] = v0;
                collisionTri.vertices[1] = v1;
                collisionTri.vertices[2] = v2;
                distance *= recipRadius;

                if (!cb->func(cb->intersection, &collisionTri, distance, cb->data))
                {
                    return NULL;
                }
            }
        }
    }

    return geometry;
}

// Calls back for each triangle of the atomic the line hits. The line is moved into the
// atomic's space, and an atomic between two morph targets is tested at its blended shape.
static RpAtomic* AtomicForAllLineIntersections(RpAtomic* atomic, RwLine* testLine,
                                               GeomCallBack* cb)
{
    RpGeometry* geom = atomic->geometry;
    RwLine localLine;
    RwMatrix inverseLTM;
    RwV3d v0;
    RwV3d v1;
    RwV3d v2;
    RwV3d* vp0;
    RwV3d* vp1;
    RwV3d* vp2;
    RpInterpolator* interpolator;
    RwV3d* startFramePoints;
    RwV3d* endFramePoints;
    F32 blendFactor;
    RwV3d delta;
    S32 iTri;
    RpTriangle* triangle;

    inverseLTM.flags = 1;
    RwMatrixInvert(&inverseLTM, RwFrameGetLTM(RpAtomicGetFrame(atomic)));
    RwV3dTransformPoints(&localLine.start, &testLine->start, 2, &inverseLTM);

    if (geom->numMorphTargets == 1)
    {
        if (!GeometryForAllLineIntersections(geom, &localLine, cb))
        {
            return NULL;
        }
    }
    else
    {
        vp0 = NULL;
        vp1 = NULL;
        vp2 = NULL;
        RwV3dSubMacro(&delta, &localLine.end, &localLine.start);
        interpolator = &atomic->interpolator;
        blendFactor = 0.0f;

        if (interpolator->startMorphTarget == interpolator->endMorphTarget ||
            interpolator->startMorphTarget >= geom->numMorphTargets ||
            interpolator->endMorphTarget >= geom->numMorphTargets)
        {
            if (interpolator->startMorphTarget < geom->numMorphTargets &&
                interpolator->endMorphTarget < geom->numMorphTargets)
            {
                startFramePoints = geom->morphTarget[interpolator->startMorphTarget].verts;
            }
            else
            {
                startFramePoints = geom->morphTarget[0].verts;
            }

            endFramePoints = startFramePoints;
        }
        else
        {
            vp0 = &v0;
            blendFactor = interpolator->recipTime * interpolator->position;
            startFramePoints = geom->morphTarget[interpolator->startMorphTarget].verts;
            vp1 = &v1;
            endFramePoints = geom->morphTarget[interpolator->endMorphTarget].verts;
            vp2 = &v2;
        }

        triangle = geom->triangles;

        for (iTri = 0; iTri < geom->numTriangles; triangle++, iTri++)
        {
            S32 result;
            F32 distance;

            if (startFramePoints == endFramePoints)
            {
                vp0 = &startFramePoints[triangle->vertIndex[0]];
                vp1 = &startFramePoints[triangle->vertIndex[1]];
                vp2 = &startFramePoints[triangle->vertIndex[2]];
            }
            else
            {
                RwV3d* startVert;
                RwV3d* endVert;

                startVert = &startFramePoints[triangle->vertIndex[0]];
                endVert = &endFramePoints[triangle->vertIndex[0]];
                V3D_BLEND(&v0, startVert, endVert, blendFactor);

                startVert = &startFramePoints[triangle->vertIndex[1]];
                endVert = &endFramePoints[triangle->vertIndex[1]];
                V3D_BLEND(&v1, startVert, endVert, blendFactor);

                startVert = &startFramePoints[triangle->vertIndex[2]];
                endVert = &endFramePoints[triangle->vertIndex[2]];
                V3D_BLEND(&v2, startVert, endVert, blendFactor);
            }

            LINETRIANGLE_INTERSECT(result, &localLine.start, &delta, vp0, vp1, vp2, distance);

            if (result)
            {
                RpCollisionTriangle collisionTri;

                TRIANGLE_NORMAL(&collisionTri.normal, vp0, vp1, vp2);
                collisionTri.point = *vp0;
                collisionTri.index = iTri;
                collisionTri.vertices[0] = vp0;
                collisionTri.vertices[1] = vp1;
                collisionTri.vertices[2] = vp2;

                if (!cb->func(cb->intersection, &collisionTri, distance, cb->data))
                {
                    return atomic;
                }
            }
        }

    }

    return atomic;
}

// Calls back for each triangle of the atomic the sphere touches. The sphere is moved into the
// atomic's space, its radius scaled with the matrix, and an atomic between two morph targets is
// tested at its blended shape.
static RpAtomic* AtomicForAllSphereIntersections(RpAtomic* atomic, RwSphere* testSphere,
                                                 GeomCallBack* cb)
{
    RpGeometry* geom = atomic->geometry;
    RwSphere localSphere;
    RwMatrix inverseLTM;
    RwV3d v0;
    RwV3d v1;
    RwV3d v2;
    RwV3d* vp0;
    RwV3d* vp1;
    RwV3d* vp2;
    RpInterpolator* interpolator;
    RwV3d* startFramePoints;
    RwV3d* endFramePoints;
    F32 blendFactor;
    F32 recipSphereRad;
    F32 scaleSq;
    F32 _result;
    S32 iTri;
    RpTriangle* triangle;

    inverseLTM.flags = 1;
    RwMatrixInvert(&inverseLTM, RwFrameGetLTM(RpAtomicGetFrame(atomic)));
    RwV3dTransformPoints(&localSphere.center, &testSphere->center, 1, &inverseLTM);
    scaleSq = RwV3dDotProductMacro(&inverseLTM.at, &inverseLTM.at);
    _result = _rwSqrt(scaleSq);
    localSphere.radius = _result * testSphere->radius;

    if (geom->numMorphTargets == 1)
    {
        if (!GeometryForAllSphereIntersections(geom, &localSphere, cb))
        {
            return NULL;
        }
    }
    else
    {
        vp0 = NULL;
        vp1 = NULL;
        vp2 = NULL;
        recipSphereRad = 1.0f / localSphere.radius;
        interpolator = &atomic->interpolator;
        blendFactor = 0.0f;

        if (interpolator->startMorphTarget == interpolator->endMorphTarget ||
            interpolator->startMorphTarget >= geom->numMorphTargets ||
            interpolator->endMorphTarget >= geom->numMorphTargets)
        {
            if (interpolator->startMorphTarget < geom->numMorphTargets &&
                interpolator->endMorphTarget < geom->numMorphTargets)
            {
                startFramePoints = geom->morphTarget[interpolator->startMorphTarget].verts;
            }
            else
            {
                startFramePoints = geom->morphTarget[0].verts;
            }

            endFramePoints = startFramePoints;
        }
        else
        {
            vp0 = &v0;
            blendFactor = interpolator->recipTime * interpolator->position;
            startFramePoints = geom->morphTarget[interpolator->startMorphTarget].verts;
            vp1 = &v1;
            endFramePoints = geom->morphTarget[interpolator->endMorphTarget].verts;
            vp2 = &v2;
        }

        triangle = geom->triangles;

        for (iTri = 0; iTri < geom->numTriangles; triangle++, iTri++)
        {
            RpCollisionTriangle collisionTri;
            F32 distance;

            if (startFramePoints == endFramePoints)
            {
                vp0 = &startFramePoints[triangle->vertIndex[0]];
                vp1 = &startFramePoints[triangle->vertIndex[1]];
                vp2 = &startFramePoints[triangle->vertIndex[2]];
            }
            else
            {
                RwV3d* startVert;
                RwV3d* endVert;

                startVert = &startFramePoints[triangle->vertIndex[0]];
                endVert = &endFramePoints[triangle->vertIndex[0]];
                V3D_BLEND(&v0, startVert, endVert, blendFactor);

                startVert = &startFramePoints[triangle->vertIndex[1]];
                endVert = &endFramePoints[triangle->vertIndex[1]];
                V3D_BLEND(&v1, startVert, endVert, blendFactor);

                startVert = &startFramePoints[triangle->vertIndex[2]];
                endVert = &endFramePoints[triangle->vertIndex[2]];
                V3D_BLEND(&v2, startVert, endVert, blendFactor);
            }

            if (RtIntersectionSphereTriangle(&localSphere, vp0, vp1, vp2, &collisionTri.normal,
                                             &distance))
            {
                collisionTri.point = *vp0;
                collisionTri.index = iTri;
                collisionTri.vertices[0] = vp0;
                collisionTri.vertices[1] = vp1;
                collisionTri.vertices[2] = vp2;
                distance *= recipSphereRad;

                if (!cb->func(cb->intersection, &collisionTri, distance, cb->data))
                {
                    return atomic;
                }
            }
        }

    }

    return atomic;
}

RpAtomic* RpAtomicForAllIntersections(RpAtomic* atomic, RpIntersection* intersection,
                                      RpIntersectionCallBackGeometryTriangle callBack, void* data)
{
    GeomCallBack cb;

    cb.func = callBack;
    cb.data = data;
    cb.intersection = intersection;

    switch (intersection->type)
    {
    case rpINTERSECTLINE:
        AtomicForAllLineIntersections(atomic, &intersection->t.line, &cb);
        return atomic;
    case rpINTERSECTSPHERE:
        AtomicForAllSphereIntersections(atomic, &intersection->t.sphere, &cb);
        return atomic;
    case rpINTERSECTATOMIC:
    {
        RpAtomic* intersectionAtomic = (RpAtomic*)intersection->t.object;

        if (intersectionAtomic)
        {
            RwSphere* boundingSphere =
                (RwSphere*)RpAtomicGetWorldBoundingSphere(intersectionAtomic);

            AtomicForAllSphereIntersections(atomic, boundingSphere, &cb);
            return atomic;
        }
        else
        {
            RwError err;

            err.pluginID = COLLIS_PLUGIN_ID;
            err.errorCode = _rwerror(0x80000016);
            RwErrorSet(&err);
            return NULL;
        }
    }
    default:
    {
        RwError err;

        err.pluginID = COLLIS_PLUGIN_ID;
        err.errorCode = _rwerror(COLLIS_ERR_BADINTERSECTION);
        RwErrorSet(&err);
        return NULL;
    }
    }
}
