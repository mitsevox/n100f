#ifndef RPCOLLIS_H
#define RPCOLLIS_H

#include <rpworld.h>

typedef struct RpV3dGradient RpV3dGradient;
struct RpV3dGradient
{
    F32 dydx;
    F32 dzdx;
    F32 dxdy;
    F32 dzdy;
    F32 dxdz;
    F32 dydz;
};

typedef struct RpCollBSPBranchNode RpCollBSPBranchNode;
struct RpCollBSPBranchNode
{
    U16 type;
    U8 leftType;
    U8 rightType;
    U16 leftNode;
    U16 rightNode;
    F32 leftValue;
    F32 rightValue;
};

typedef struct RpCollBSPLeafNode RpCollBSPLeafNode;
struct RpCollBSPLeafNode
{
    U16 numPolygons;
    U16 firstPolygon;
};

typedef struct RpCollBSPTree RpCollBSPTree;
struct RpCollBSPTree
{
    U32 numLeafNodes;
    RpCollBSPBranchNode* branchNodes;
    RpCollBSPLeafNode* leafNodes;
    U8 pad[4];
};

typedef struct RpCollisionData RpCollisionData;
struct RpCollisionData
{
    S32 flags;
    RpCollBSPTree* tree;
    S32 numTriangles;
    U16* triangleMap;
};

typedef enum RpIntersectType
{
    rpINTERSECTNONE = 0,
    rpINTERSECTLINE = 1,
    rpINTERSECTPOINT = 2,
    rpINTERSECTSPHERE = 3,
    rpINTERSECTBOX = 4,
    rpINTERSECTATOMIC = 5,
    rpINTERSECTTYPEFORCEENUMSIZEINT = 0x7FFFFFFF
} RpIntersectType;

typedef union RpIntersectData RpIntersectData;
union RpIntersectData
{
    RwLine line;
    RwV3d point;
    RwSphere sphere;
    RwBBox box;
    void* object;
};

typedef struct RpIntersection RpIntersection;
struct RpIntersection
{
    RpIntersectData t;
    RpIntersectType type;
};

typedef struct RpCollisionTriangle RpCollisionTriangle;
struct RpCollisionTriangle
{
    RwV3d normal;
    RwV3d point;
    S32 index;
    RwV3d* vertices[3];
};

typedef RpCollisionTriangle* (*RpIntersectionCallBackGeometryTriangle)(
    RpIntersection* intersection, RpCollisionTriangle* collTriangle, F32 distance, void* data);

typedef RpCollisionTriangle* (*RpIntersectionCallBackWorldTriangle)(
    RpIntersection* intersection, RpWorldSector* sector, RpCollisionTriangle* collTriangle,
    F32 distance, void* data);

// Shared by the plugin's own units.
#define COLLIS_PLUGIN_ID 0x11D

// The plugin's own error codes.
#define COLLIS_ERR_BADINTERSECTION 0

#define COLLISIONDATA(object, offset) (*(RpCollisionData**)((U8*)(object) + (offset)))

// A line meets a triangle only when the determinant is above this, so the line is not parallel
// to the triangle or hitting its back.
#define LINETRIANGLE_DETEPSILON 1e-8f

// The tolerance on the edges, as a fraction of the determinant.
#define LINETRIANGLE_EDGEEPSILON 1e-5f

// Tests the line from _start along _delta against the triangle _v0, _v1, _v2; on a hit _result
// is TRUE and _position is how far along _delta the hit is, from 0 to 1.
#define LINETRIANGLE_INTERSECT(_result, _start, _delta, _v0, _v1, _v2, _position)               \
    do                                                                                             \
    {                                                                                              \
        RwV3d edge1;                                                                               \
        RwV3d edge2;                                                                               \
        RwV3d pvec;                                                                                \
        F32 det;                                                                                   \
                                                                                                   \
        RwV3dSubMacro(&edge1, (_v1), (_v0));                                                       \
        RwV3dSubMacro(&edge2, (_v2), (_v0));                                                       \
        RwV3dCrossProductMacro(&pvec, (_delta), &edge2);                                           \
        det = RwV3dDotProductMacro(&edge1, &pvec);                                                 \
        (_result) = det > LINETRIANGLE_DETEPSILON;                                                 \
                                                                                                   \
        if (_result)                                                                               \
        {                                                                                          \
            RwV3d tvec;                                                                            \
            F32 lo = -det * LINETRIANGLE_EDGEEPSILON;                                              \
            F32 hi = det - lo;                                                                     \
            F32 u;                                                                                 \
                                                                                                   \
            RwV3dSubMacro(&tvec, (_start), (_v0));                                                 \
            u = RwV3dDotProductMacro(&tvec, &pvec);                                                \
            (_result) = u >= lo && u <= hi;                                                        \
                                                                                                   \
            if (_result)                                                                           \
            {                                                                                      \
                RwV3d qvec;                                                                        \
                F32 v;                                                                             \
                                                                                                   \
                RwV3dCrossProductMacro(&qvec, &tvec, &edge1);                                      \
                v = RwV3dDotProductMacro((_delta), &qvec);                                         \
                (_result) = v >= lo && u + v <= hi;                                                \
                                                                                                   \
                if (_result)                                                                       \
                {                                                                                  \
                    (_position) = RwV3dDotProductMacro(&edge2, &qvec);                             \
                    (_result) = (_position) >= lo && (_position) <= hi;                            \
                                                                                                   \
                    if (_result)                                                                   \
                    {                                                                              \
                        (_position) /= det;                                                        \
                    }                                                                              \
                }                                                                                  \
            }                                                                                      \
        }                                                                                          \
    } while (0)

// Sets _normal to the unit normal of the triangle _v0, _v1, _v2.
#define TRIANGLE_NORMAL(_normal, _v0, _v1, _v2)                                                    \
    do                                                                                             \
    {                                                                                              \
        RwV3d a;                                                                                   \
        RwV3d b;                                                                                   \
        F32 lengthSq;                                                                              \
        F32 _result;                                                                               \
                                                                                                   \
        RwV3dSubMacro(&a, (_v1), (_v0));                                                           \
        RwV3dSubMacro(&b, (_v2), (_v0));                                                           \
        RwV3dCrossProductMacro((_normal), &a, &b);                                                 \
        lengthSq = RwV3dDotProductMacro((_normal), (_normal));                                     \
        _result = _rwInvSqrt(lengthSq);                                                            \
        RwV3dScaleMacro((_normal), (_normal), _result);                                            \
    } while (0)

#ifdef __cplusplus
extern "C" {
#endif

extern S32 _rpCollisionGeometryDataOffset;
extern S32 _rpCollisionWorldSectorDataOffset;

RpWorld* RpCollisionWorldForAllIntersections(RpWorld* world, RpIntersection* intersection,
                                             RpIntersectionCallBackWorldTriangle callBack,
                                             void* data);

RpAtomic* RpAtomicForAllIntersections(RpAtomic* atomic, RpIntersection* intersection,
                                      RpIntersectionCallBackGeometryTriangle callBack, void* data);

RpCollBSPTree* _rpCollBSPTreeInit(RpCollBSPTree* tree, S32 numLeafNodes);
S32 _rpCollBSPTreeMemGetSize(S32 numLeafNodes);
void _rpCollBSPTreeDestroy(RpCollBSPTree* tree);
RpCollBSPTree* _rpCollBSPTreeStreamWrite(RpCollBSPTree* tree, RwStream* stream);
RpCollBSPTree* _rpCollBSPTreeStreamRead(RpCollBSPTree* tree, RwStream* stream);
S32 _rpCollBSPTreeStreamGetSize(RpCollBSPTree* tree);
RpCollBSPTree* _rpCollBSPTreeForAllLineLeafNodeIntersections(RpCollBSPTree* tree, RwLine* line,
                                                             RpV3dGradient* grad,
                                                             S32 (*callBack)(S32, S32, void*),
                                                             void* data);
RpCollBSPTree* _rpCollBSPTreeForAllBoxLeafNodeIntersections(RpCollBSPTree* tree, RwBBox* box,
                                                            S32 (*callBack)(S32, S32, void*),
                                                            void* data);

#ifdef __cplusplus
}
#endif

#endif
