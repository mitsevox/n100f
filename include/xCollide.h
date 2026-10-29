#ifndef XCOLLIDE_H
#define XCOLLIDE_H

#include "types.h"
#include "xMath3.h"

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

#endif
