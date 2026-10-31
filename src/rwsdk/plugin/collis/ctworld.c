#include <rpcollis.h>
#include <rtintsec.h>

// The plane bit of a collision sector node, and the masks of its axis and polygon count.
#define COLLSECTOR_PLANE 0x80
#define COLLSECTOR_NUMPOLYGONS 0x1F

// The byte offset of a collision sector node's split axis in an RwV3d.
#define COLLSECTOR_AXIS(_cType) (((_cType) >> 3) & 0xC)

#define COLLSECTOR_STACK_SIZE 16
#define WORLDSECTOR_STACK_SIZE 64

// The coordinate of _vect at byte offset _offset, the way a plane's type names an axis.
#define V3D_COORD(_vect, _offset) (*(F32*)((U8*)&(_vect) + (_offset)))

// Sets _out to the point where _line crosses _value on the axis _type, stepping from the
// line's start along the gradient.
#define LINE_SPLIT(_out, _line, _grad, _type, _value)                                              \
    do                                                                                             \
    {                                                                                              \
        F32 delta;                                                                                 \
                                                                                                   \
        switch (_type)                                                                             \
        {                                                                                          \
        case 0:                                                                                    \
            delta = (_value) - (_line).start.x;                                                    \
            (_out).x = (_value);                                                                   \
            (_out).y = (_line).start.y + (_grad)->dydx * delta;                                    \
            (_out).z = (_line).start.z + (_grad)->dzdx * delta;                                    \
            break;                                                                                 \
        case 4:                                                                                    \
            delta = (_value) - (_line).start.y;                                                    \
            (_out).y = (_value);                                                                   \
            (_out).x = (_line).start.x + (_grad)->dxdy * delta;                                    \
            (_out).z = (_line).start.z + (_grad)->dzdy * delta;                                    \
            break;                                                                                 \
        case 8:                                                                                    \
            delta = (_value) - (_line).start.z;                                                    \
            (_out).z = (_value);                                                                   \
            (_out).x = (_line).start.x + (_grad)->dxdz * delta;                                    \
            (_out).y = (_line).start.y + (_grad)->dydz * delta;                                    \
            break;                                                                                 \
        }                                                                                          \
    } while (0)

typedef union IntersectionCallBack IntersectionCallBack;
union IntersectionCallBack
{
    RpWorldSector* (*sectorCB)(RpIntersection* intersection, RpWorldSector* sector, void* data);
    RpIntersectionCallBackWorldTriangle worldCB;
    RpAtomic* (*atomicCB)(RpIntersection* intersection, RpWorldSector* sector, RpAtomic* atomic,
                          F32 distance, void* data);
};

typedef struct CallBackParam CallBackParam;
struct CallBackParam
{
    RpIntersection* intersection;
    IntersectionCallBack u;
    void* data;
};

typedef struct TestSphere TestSphere;
struct TestSphere
{
    RwSphere* sphere;
    F32 recipRadius;
};

typedef struct PolyTestParam PolyTestParam;
struct PolyTestParam
{
    RwBBox bbox;
    RpWorldSector* worldSector;
    S32 (*leafTest)(S32 numPolygons, S32 polyOffset, void* data);
    S32 (*v303leafTest)(S32 numPolygons, S32 polyOffset, void* data);
    void* leafTestData;
    CallBackParam* cbParam;
};

typedef struct PolyLineTestParam PolyLineTestParam;
struct PolyLineTestParam
{
    RpV3dGradient grad;
    RwLine clippedLine;
    RpWorldSector* worldSector;
    RwV3d* start;
    RwV3d delta;
    CallBackParam* cbParam;
};

// Walks the world's sector tree along the line, calling back for each world sector it passes
// through, nearest first. Each plane has a left and a right value that may overlap, so the line
// is clipped to each side; the clipped piece is passed out through clippedLineOut.
static RpWorld* WorldForAllLineWorldSectorIntersections(RpWorld* world, RwLine* testLine,
                                                        RpV3dGradient* grad,
                                                        RpWorldSectorCallBack callBack,
                                                        void* data, RwLine* clippedLineOut)
{
    S32 nStack;
    RpSector* sector;
    RpSector* sectorStack[WORLDSECTOR_STACK_SIZE];
    RwLine line;
    RwLine lineStack[WORLDSECTOR_STACK_SIZE];

    line = *testLine;
    nStack = 0;
    sector = world->rootSector;

    while (nStack >= 0)
    {
        if (sector->type < 0)
        {
            if (clippedLineOut)
            {
                *clippedLineOut = line;
            }

            if (!callBack((RpWorldSector*)sector, data))
            {
                return world;
            }

            line = lineStack[nStack];
            sector = sectorStack[nStack];
            nStack--;
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;
            RwSplitBits lStart;
            RwSplitBits lEnd;
            RwSplitBits rStart;
            RwSplitBits rEnd;

            rStart.nReal = V3D_COORD(line.start, plane->type) - plane->rightValue;
            lStart.nReal = V3D_COORD(line.start, plane->type) - plane->leftValue;
            lEnd.nReal = V3D_COORD(line.end, plane->type) - plane->leftValue;
            rEnd.nReal = V3D_COORD(line.end, plane->type) - plane->rightValue;

            if (rStart.nInt < 0 && rEnd.nInt < 0)
            {
                // All of the line is left of the right plane.
                sector = plane->leftSubTree;
            }
            else if (lStart.nInt >= 0 && lEnd.nInt >= 0)
            {
                // All of the line is right of the left plane.
                sector = plane->rightSubTree;
            }
            else if (!((lStart.nInt ^ lEnd.nInt) & 0x80000000) &&
                     !((rStart.nInt ^ rEnd.nInt) & 0x80000000))
            {
                // The line lies in the overlap: both sides take all of it, the nearer first.
                if (rStart.nInt < rEnd.nInt)
                {
                    nStack++;
                    lineStack[nStack] = line;
                    sectorStack[nStack] = plane->rightSubTree;
                    sector = plane->leftSubTree;
                }
                else
                {
                    nStack++;
                    lineStack[nStack] = line;
                    sectorStack[nStack] = plane->leftSubTree;
                    sector = plane->rightSubTree;
                }
            }
            else if (((lStart.nInt ^ lEnd.nInt) & 0x80000000) && rStart.nInt >= 0 &&
                     rEnd.nInt >= 0)
            {
                // The line crosses only the left plane.
                RwV3d vSplit;

                LINE_SPLIT(vSplit, line, grad, plane->type, plane->leftValue);

                if (lStart.nInt < 0)
                {
                    nStack++;
                    lineStack[nStack] = line;
                    sectorStack[nStack] = plane->rightSubTree;
                    sector = plane->leftSubTree;
                    line.end = vSplit;
                }
                else
                {
                    nStack++;
                    lineStack[nStack].start = vSplit;
                    lineStack[nStack].end = line.end;
                    sectorStack[nStack] = plane->leftSubTree;
                    sector = plane->rightSubTree;
                }
            }
            else if (((rStart.nInt ^ rEnd.nInt) & 0x80000000) && lStart.nInt < 0 &&
                     lEnd.nInt < 0)
            {
                // The line crosses only the right plane.
                RwV3d vSplit;

                LINE_SPLIT(vSplit, line, grad, plane->type, plane->rightValue);

                if (rStart.nInt < 0)
                {
                    nStack++;
                    lineStack[nStack].start = vSplit;
                    lineStack[nStack].end = line.end;
                    sectorStack[nStack] = plane->rightSubTree;
                    sector = plane->leftSubTree;
                }
                else
                {
                    nStack++;
                    lineStack[nStack] = line;
                    sectorStack[nStack] = plane->leftSubTree;
                    sector = plane->rightSubTree;
                    line.end = vSplit;
                }
            }
            else
            {
                // The line crosses both planes.
                RwV3d vSplitRight;
                RwV3d vSplitLeft;

                LINE_SPLIT(vSplitRight, line, grad, plane->type, plane->rightValue);
                LINE_SPLIT(vSplitLeft, line, grad, plane->type, plane->leftValue);

                if (lStart.nInt < 0)
                {
                    nStack++;
                    lineStack[nStack].start = vSplitRight;
                    lineStack[nStack].end = line.end;
                    sectorStack[nStack] = plane->rightSubTree;
                    sector = plane->leftSubTree;
                    line.end = vSplitLeft;
                }
                else
                {
                    nStack++;
                    lineStack[nStack].start = vSplitLeft;
                    lineStack[nStack].end = line.end;
                    sectorStack[nStack] = plane->leftSubTree;
                    sector = plane->rightSubTree;
                    line.end = vSplitRight;
                }
            }
        }
    }

    return world;
}

// Tests the line against the polygons of a world sector without a collision tree, walking the
// sector's own split tree and clipping the line to each side it visits.
static RpWorldSector* OldWorldSectorForAllLinePolyIntersections(RpWorldSector* worldSector,
                                                                PolyLineTestParam* isData)
{
    RpCollSector* collSector = worldSector->colSectorRoot;

    if (collSector)
    {
        CallBackParam* cbParam = isData->cbParam;
        RwV3d* vertices = worldSector->vertices;
        S32 cut = 1;
        S32 nStack = 0;
        S32 cutStack[COLLSECTOR_STACK_SIZE];
        RwLine line;
        RwLine lineStack[COLLSECTOR_STACK_SIZE];

        line = isData->clippedLine;

        while (nStack >= 0)
        {
            RpCollSector* sector = &collSector[cut];
            S32 numPolygons;
            S32 polyOffset;
            RpPolygon* poly;

            if (sector->cType & COLLSECTOR_PLANE)
            {
                S32 plane = COLLSECTOR_AXIS(sector->cType);
                F32 value;
                RwSplitBits start;
                RwSplitBits end;

                polyOffset = sector->start;
                numPolygons = sector->cType & COLLSECTOR_NUMPOLYGONS;
                value = V3D_COORD(vertices[sector->vertex], plane);
                start.nReal = V3D_COORD(line.start, plane) - value;
                end.nReal = V3D_COORD(line.end, plane) - value;

                if ((start.nInt ^ end.nInt) & 0x80000000)
                {
                    // The line crosses the plane: the far piece waits on the stack.
                    RwV3d vSplit;

                    LINE_SPLIT(vSplit, line, &isData->grad, plane, value);

                    nStack++;
                    cutStack[nStack] = (start.nInt < 0) ? cut + cut + 1 : cut + cut;
                    lineStack[nStack].start = vSplit;
                    lineStack[nStack].end = line.end;
                    line.end = vSplit;
                }

                cut = (start.nInt < 0) ? cut + cut : cut + cut + 1;
            }
            else
            {
                polyOffset = sector->start;
                numPolygons = sector->cType;
                line = lineStack[nStack];
                cut = cutStack[nStack];
                nStack--;
            }

            poly = &worldSector->polygons[polyOffset];

            while (numPolygons--)
            {
                RwV3d* v0 = &vertices[poly->vertIndex[0]];
                RwV3d* v1 = &vertices[poly->vertIndex[1]];
                RwV3d* v2 = &vertices[poly->vertIndex[2]];
                S32 result;
                F32 distance;

                LINETRIANGLE_INTERSECT(result, isData->start, &isData->delta, v0, v1, v2,
                                       distance);

                if (result)
                {
                    RpCollisionTriangle collTri;

                    collTri.point = *v0;
                    collTri.index = polyOffset;
                    TRIANGLE_NORMAL(&collTri.normal, v0, v1, v2);
                    collTri.vertices[0] = v0;
                    collTri.vertices[1] = v1;
                    collTri.vertices[2] = v2;

                    if (!cbParam->u.worldCB(cbParam->intersection, worldSector, &collTri,
                                            distance, cbParam->data))
                    {
                        return NULL;
                    }
                }

                polyOffset++;
                poly++;
            }
        }
    }

    return worldSector;
}

// Tests the box against polygons of a world sector without a collision tree; a box hit has no
// distance.
static S32 OldLeafNodeForAllBoxPolyIntersections(S32 numPolygons, S32 polyOffset, void* data)
{
    PolyTestParam* isData = (PolyTestParam*)data;
    RpWorldSector* worldSector = isData->worldSector;
    CallBackParam* cbParam = isData->cbParam;
    RwV3d* vertices = worldSector->vertices;
    RpPolygon* poly = &worldSector->polygons[polyOffset];

    while (numPolygons--)
    {
        RwV3d* v0 = &vertices[poly->vertIndex[0]];
        RwV3d* v1 = &vertices[poly->vertIndex[1]];
        RwV3d* v2 = &vertices[poly->vertIndex[2]];

        if (RtIntersectionBBoxTriangle(&isData->bbox, v0, v1, v2))
        {
            RpCollisionTriangle collTri;

            collTri.point = *v0;
            collTri.index = polyOffset;
            TRIANGLE_NORMAL(&collTri.normal, v0, v1, v2);
            collTri.vertices[0] = v0;
            collTri.vertices[1] = v1;
            collTri.vertices[2] = v2;

            if (!cbParam->u.worldCB(cbParam->intersection, worldSector, &collTri, 0.0f,
                                    cbParam->data))
            {
                return FALSE;
            }
        }

        polyOffset++;
        poly++;
    }

    return TRUE;
}

// Tests the sphere against polygons of a world sector without a collision tree.
static S32 OldLeafNodeForAllSpherePolyIntersections(S32 numPolygons, S32 polyOffset, void* data)
{
    PolyTestParam* isData = (PolyTestParam*)data;
    RpWorldSector* worldSector = isData->worldSector;
    TestSphere* testSphere = (TestSphere*)isData->leafTestData;
    CallBackParam* cbParam = isData->cbParam;
    RwV3d* vertices = worldSector->vertices;
    RpPolygon* poly = &worldSector->polygons[polyOffset];

    while (numPolygons--)
    {
        RwV3d* v0 = &vertices[poly->vertIndex[0]];
        RwV3d* v1 = &vertices[poly->vertIndex[1]];
        RwV3d* v2 = &vertices[poly->vertIndex[2]];
        RpCollisionTriangle collTri;
        F32 distance;

        if (RtIntersectionSphereTriangle(testSphere->sphere, v0, v1, v2, &collTri.normal,
                                         &distance))
        {
            collTri.point = *v0;
            collTri.index = polyOffset;
            collTri.vertices[0] = v0;
            collTri.vertices[1] = v1;
            collTri.vertices[2] = v2;
            distance *= testSphere->recipRadius;

            if (!cbParam->u.worldCB(cbParam->intersection, worldSector, &collTri, distance,
                                    cbParam->data))
            {
                return FALSE;
            }
        }

        polyOffset++;
        poly++;
    }

    return TRUE;
}

// Tests the line against the polygons of one leaf of a world sector's collision tree.
static S32 LeafNodeForAllLinePolyIntersections(S32 numPolygons, S32 polyOffset, void* data)
{
    PolyLineTestParam* isData = (PolyLineTestParam*)data;
    RpWorldSector* worldSector = isData->worldSector;
    CallBackParam* cbParam = isData->cbParam;
    RwV3d* vertices = worldSector->vertices;
    RpPolygon* polygons = worldSector->polygons;
    U16* polyIndex =
        COLLISIONDATA(worldSector, _rpCollisionWorldSectorDataOffset)->triangleMap + polyOffset;

    while (numPolygons--)
    {
        S32 index = *polyIndex;
        RpPolygon* poly = &polygons[index];
        RwV3d* v0 = &vertices[poly->vertIndex[0]];
        RwV3d* v1 = &vertices[poly->vertIndex[1]];
        RwV3d* v2 = &vertices[poly->vertIndex[2]];
        S32 result;
        F32 distance;

        LINETRIANGLE_INTERSECT(result, isData->start, &isData->delta, v0, v1, v2, distance);

        if (result)
        {
            RpCollisionTriangle collisionTri;

            collisionTri.point = *v0;
            collisionTri.index = index;
            TRIANGLE_NORMAL(&collisionTri.normal, v0, v1, v2);
            collisionTri.vertices[0] = v0;
            collisionTri.vertices[1] = v1;
            collisionTri.vertices[2] = v2;

            if (!cbParam->u.worldCB(cbParam->intersection, worldSector, &collisionTri, distance,
                                    cbParam->data))
            {
                return FALSE;
            }
        }

        polyIndex++;
    }

    return TRUE;
}

// Tests the sphere against the polygons of one leaf of a world sector's collision tree.
static S32 LeafNodeForAllSpherePolyIntersections(S32 numPolygons, S32 polyOffset, void* data)
{
    PolyTestParam* isData = (PolyTestParam*)data;
    RpWorldSector* worldSector = isData->worldSector;
    CallBackParam* cbParam = isData->cbParam;
    TestSphere* testSphere = (TestSphere*)isData->leafTestData;
    RwV3d* vertices = worldSector->vertices;
    RpPolygon* polygons = worldSector->polygons;
    U16* polyIndex =
        COLLISIONDATA(worldSector, _rpCollisionWorldSectorDataOffset)->triangleMap + polyOffset;

    while (numPolygons--)
    {
        RpPolygon* poly = &polygons[*polyIndex];
        RwV3d* v0 = &vertices[poly->vertIndex[0]];
        RwV3d* v1 = &vertices[poly->vertIndex[1]];
        RwV3d* v2 = &vertices[poly->vertIndex[2]];
        RpCollisionTriangle collisionTri;
        F32 distance;

        if (RtIntersectionSphereTriangle(testSphere->sphere, v0, v1, v2, &collisionTri.normal,
                                         &distance))
        {
            collisionTri.point = *v0;
            collisionTri.index = *polyIndex;
            collisionTri.vertices[0] = v0;
            collisionTri.vertices[1] = v1;
            collisionTri.vertices[2] = v2;
            distance *= testSphere->recipRadius;

            if (!cbParam->u.worldCB(cbParam->intersection, worldSector, &collisionTri, distance,
                                    cbParam->data))
            {
                return FALSE;
            }
        }

        polyIndex++;
    }

    return TRUE;
}

// Tests the box against the polygons of one leaf of a world sector's collision tree; a box hit
// has no distance.
static S32 LeafNodeForAllBoxPolyIntersections(S32 numPolygons, S32 polyOffset, void* data)
{
    PolyTestParam* isData = (PolyTestParam*)data;
    RpWorldSector* worldSector = isData->worldSector;
    CallBackParam* cbParam = isData->cbParam;
    RwV3d* vertices = worldSector->vertices;
    RpPolygon* polygons = worldSector->polygons;
    U16* polyIndex =
        COLLISIONDATA(worldSector, _rpCollisionWorldSectorDataOffset)->triangleMap + polyOffset;

    while (numPolygons--)
    {
        RpPolygon* poly = &polygons[*polyIndex];
        RwV3d* v0 = &vertices[poly->vertIndex[0]];
        RwV3d* v1 = &vertices[poly->vertIndex[1]];
        RwV3d* v2 = &vertices[poly->vertIndex[2]];

        if (RtIntersectionBBoxTriangle(&isData->bbox, v0, v1, v2))
        {
            RpCollisionTriangle collisionTri;

            collisionTri.point = *v0;
            collisionTri.index = *polyIndex;
            TRIANGLE_NORMAL(&collisionTri.normal, v0, v1, v2);
            collisionTri.vertices[0] = v0;
            collisionTri.vertices[1] = v1;
            collisionTri.vertices[2] = v2;

            if (!cbParam->u.worldCB(cbParam->intersection, worldSector, &collisionTri, 0.0f,
                                    cbParam->data))
            {
                return FALSE;
            }
        }

        polyIndex++;
    }

    return TRUE;
}

// Tests the line against the polygons of a world sector, through its collision tree when it
// has one.
static RpWorldSector* WorldSectorForAllLinePolyIntersections(RpWorldSector* worldSector,
                                                             void* data)
{
    PolyLineTestParam* isData = (PolyLineTestParam*)data;
    RpCollisionData* collData = COLLISIONDATA(worldSector, _rpCollisionWorldSectorDataOffset);

    isData->worldSector = worldSector;

    if (!collData)
    {
        return OldWorldSectorForAllLinePolyIntersections(worldSector, isData);
    }

    if (!_rpCollBSPTreeForAllLineLeafNodeIntersections(collData->tree, &isData->clippedLine,
                                                       &isData->grad,
                                                       LeafNodeForAllLinePolyIntersections,
                                                       isData))
    {
        return NULL;
    }

    return worldSector;
}

// Walks a world sector without a collision tree down its own split tree, testing the polygons
// of each node the box reaches.
static inline RpWorldSector* OldWorldSectorForAllBoxedPrimitivePolyIntersections(
    RpWorldSector* worldSector, PolyTestParam* isData)
{
    RpCollSector* collSector = worldSector->colSectorRoot;

    if (collSector)
    {
        RwV3d* vertices = worldSector->vertices;
        S32 cut = 1;
        S32 nStack = 0;
        S32 cutStack[COLLSECTOR_STACK_SIZE];

        while (nStack >= 0)
        {
            RpCollSector* sector = &collSector[cut];
            S32 numPolygons;
            S32 polyOffset;

            if (sector->cType & COLLSECTOR_PLANE)
            {
                S32 plane = COLLSECTOR_AXIS(sector->cType);
                F32 value;
                RwSplitBits left;
                RwSplitBits right;

                numPolygons = sector->cType & COLLSECTOR_NUMPOLYGONS;
                polyOffset = sector->start;
                value = V3D_COORD(vertices[sector->vertex], plane);
                left.nReal = V3D_COORD(isData->bbox.inf, plane) - value;
                right.nReal = value - V3D_COORD(isData->bbox.sup, plane);

                if (left.nInt < 0)
                {
                    if (right.nInt < 0)
                    {
                        nStack++;
                        cutStack[nStack] = cut + cut + 1;
                    }

                    cut = cut + cut;
                }
                else if (right.nInt < 0)
                {
                    cut = cut + cut + 1;
                }
            }
            else
            {
                numPolygons = sector->cType;
                polyOffset = sector->start;
                cut = cutStack[nStack];
                nStack--;
            }

            if (!isData->v303leafTest(numPolygons, polyOffset, isData))
            {
                return NULL;
            }
        }
    }

    return worldSector;
}

// Tests the box of a boxed primitive against the polygons of a world sector, through its
// collision tree when it has one.
static RpWorldSector* WorldSectorForAllBoxedPrimitivePolyIntersections(RpWorldSector* worldSector,
                                                                       void* data)
{
    PolyTestParam* isData = (PolyTestParam*)data;
    RpCollisionData* collData = COLLISIONDATA(worldSector, _rpCollisionWorldSectorDataOffset);

    isData->worldSector = worldSector;

    if (!collData)
    {
        return OldWorldSectorForAllBoxedPrimitivePolyIntersections(worldSector, isData);
    }

    if (!_rpCollBSPTreeForAllBoxLeafNodeIntersections(collData->tree, &isData->bbox,
                                                      isData->leafTest, isData))
    {
        return NULL;
    }

    return worldSector;
}

// Walks the world's sector tree, calling back for each world sector the box overlaps.
static inline RpWorld* WorldForAllBoxWorldSectorIntersections(RpWorld* world, RwBBox* box,
                                                       RpWorldSectorCallBack callBack,
                                                       void* data)
{
    S32 nStack = 0;
    RpSector* sector = world->rootSector;
    RpSector* sectorStack[WORLDSECTOR_STACK_SIZE];

    while (nStack >= 0)
    {
        if (sector->type < 0)
        {
            if (!callBack((RpWorldSector*)sector, data))
            {
                return world;
            }

            sector = sectorStack[nStack--];
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            if (V3D_COORD(box->inf, plane->type) < plane->leftValue)
            {
                sector = plane->leftSubTree;

                if (V3D_COORD(box->sup, plane->type) >= plane->rightValue)
                {
                    sectorStack[++nStack] = plane->rightSubTree;
                }
            }
            else
            {
                sector = plane->rightSubTree;
            }
        }
    }

    return world;
}

RpWorld* RpCollisionWorldForAllIntersections(RpWorld* world, RpIntersection* intersection,
                                             RpIntersectionCallBackWorldTriangle callBack,
                                             void* data)
{
    CallBackParam cbParam;

    cbParam.u.worldCB = callBack;
    cbParam.intersection = intersection;
    cbParam.data = data;

    switch (intersection->type)
    {
    case rpINTERSECTPOINT:
    {
        RwError err;

        err.pluginID = COLLIS_PLUGIN_ID;
        err.errorCode = _rwerror(COLLIS_ERR_BADINTERSECTION);
        RwErrorSet(&err);
        return NULL;
    }
    case rpINTERSECTLINE:
    {
        PolyLineTestParam isData;
        F32 recip;

        isData.start = &intersection->t.line.start;
        RwV3dSubMacro(&isData.delta, &intersection->t.line.end, &intersection->t.line.start);
        isData.cbParam = &cbParam;

        recip = (isData.delta.x != 0.0f) ? 1.0f / isData.delta.x : 0.0f;
        isData.grad.dydx = isData.delta.y * recip;
        isData.grad.dzdx = isData.delta.z * recip;

        recip = (isData.delta.y != 0.0f) ? 1.0f / isData.delta.y : 0.0f;
        isData.grad.dxdy = isData.delta.x * recip;
        isData.grad.dzdy = isData.delta.z * recip;

        recip = (isData.delta.z != 0.0f) ? 1.0f / isData.delta.z : 0.0f;
        isData.grad.dxdz = isData.delta.x * recip;
        isData.grad.dydz = isData.delta.y * recip;

        WorldForAllLineWorldSectorIntersections(world, &intersection->t.line, &isData.grad,
                                                WorldSectorForAllLinePolyIntersections, &isData,
                                                &isData.clippedLine);
        return world;
    }
    case rpINTERSECTSPHERE:
    {
        TestSphere testSphere;
        PolyTestParam isData;

        isData.bbox.sup = intersection->t.sphere.center;
        isData.bbox.inf = intersection->t.sphere.center;
        testSphere.sphere = &intersection->t.sphere;
        isData.leafTest = LeafNodeForAllSpherePolyIntersections;
        isData.v303leafTest = OldLeafNodeForAllSpherePolyIntersections;
        isData.leafTestData = &testSphere;
        isData.cbParam = &cbParam;
        testSphere.recipRadius = 1.0f / intersection->t.sphere.radius;
        isData.bbox.inf.x -= intersection->t.sphere.radius;
        isData.bbox.inf.y -= intersection->t.sphere.radius;
        isData.bbox.inf.z -= intersection->t.sphere.radius;
        isData.bbox.sup.x += intersection->t.sphere.radius;
        isData.bbox.sup.y += intersection->t.sphere.radius;
        isData.bbox.sup.z += intersection->t.sphere.radius;

        WorldForAllBoxWorldSectorIntersections(world, &isData.bbox,
                                               WorldSectorForAllBoxedPrimitivePolyIntersections,
                                               &isData);
        return world;
    }
    case rpINTERSECTBOX:
    {
        PolyTestParam isData;

        isData.bbox = intersection->t.box;
        isData.leafTest = LeafNodeForAllBoxPolyIntersections;
        isData.v303leafTest = OldLeafNodeForAllBoxPolyIntersections;
        isData.cbParam = &cbParam;

        WorldForAllBoxWorldSectorIntersections(world, &isData.bbox,
                                               WorldSectorForAllBoxedPrimitivePolyIntersections,
                                               &isData);
        return world;
    }
    case rpINTERSECTATOMIC:
    {
        RpAtomic* atomic = (RpAtomic*)intersection->t.object;
        RwSphere sphere;
        TestSphere testSphere;
        PolyTestParam isData;

        sphere = *RpAtomicGetWorldBoundingSphere(atomic);
        isData.bbox.sup = sphere.center;
        isData.bbox.inf = sphere.center;
        testSphere.sphere = &sphere;
        testSphere.recipRadius = 1.0f / sphere.radius;
        isData.bbox.inf.x -= sphere.radius;
        isData.bbox.inf.y -= sphere.radius;
        isData.bbox.inf.z -= sphere.radius;
        isData.bbox.sup.x += sphere.radius;
        isData.bbox.sup.y += sphere.radius;
        isData.bbox.sup.z += sphere.radius;
        isData.leafTest = LeafNodeForAllSpherePolyIntersections;
        isData.v303leafTest = OldLeafNodeForAllSpherePolyIntersections;
        isData.leafTestData = &testSphere;
        isData.cbParam = &cbParam;

        RpAtomicForAllWorldSectors(atomic, WorldSectorForAllBoxedPrimitivePolyIntersections,
                                   &isData);
        return world;
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
