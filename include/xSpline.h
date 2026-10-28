#ifndef XSPLINE_H
#define XSPLINE_H

#include "types.h"
#include "xVec3.h"

struct xSpline3;

xSpline3* xSpline3_Bezier(xVec3* points, F32* time, U32 numpoints, U32 numalloc, xVec3* p1, xVec3* p2);
void xSpline3_ArcInit(xSpline3* spl, U32 sample);

#endif
