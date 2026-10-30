#ifndef XCOLLIDE_H
#define XCOLLIDE_H

#include "types.h"
#include "xMath3.h"

struct _xCollis;

enum _xCollsIdx
{
    k_XCOLLS_IDX_FLOOR,
    k_XCOLLS_IDX_CEIL,
    k_XCOLLS_IDX_FRONT,
    k_XCOLLS_IDX_LEFT,
    k_XCOLLS_IDX_REAR,
    k_XCOLLS_IDX_RIGHT,
    k_XCOLLS_IDX_COUNT
};

_xCollsIdx xCollideGetCollsIdx(const xVec3* tohit, const xMat3x3* mat);
U32 xSphereHitsOBB(const xSphere* a, const _xBox* b, const xMat4x3* m, _xCollis* coll);
U32 xSphereHitsBox(const xSphere* a, const _xBox* b, _xCollis* coll);
U32 xSphereHitsSphere(const xSphere* a, const xSphere* b, _xCollis* coll);

#endif
