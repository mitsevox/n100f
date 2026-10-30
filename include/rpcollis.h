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

#ifdef __cplusplus
extern "C" {
#endif

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
