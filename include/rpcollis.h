#ifndef RPCOLLIS_H
#define RPCOLLIS_H

#include <rpworld.h>

typedef struct RpCollBSPTree RpCollBSPTree;

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
U32 _rpCollBSPTreeMemGetSize(S32 numLeafNodes);
void _rpCollBSPTreeDestroy(RpCollBSPTree* tree);
RpCollBSPTree* _rpCollBSPTreeStreamWrite(RpCollBSPTree* tree, RwStream* stream);
RpCollBSPTree* _rpCollBSPTreeStreamRead(RpCollBSPTree* tree, RwStream* stream);
S32 _rpCollBSPTreeStreamGetSize(RpCollBSPTree* tree);

#ifdef __cplusplus
}
#endif

#endif
