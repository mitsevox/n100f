#ifndef XBOUND_H
#define XBOUND_H

#include "xEnt.h"

void xBoundUpdate(_xBound* b);
xVec3* xBoundCenter(const _xBound* b);
void xBoundHitsBound(const _xBound* a, const _xBound* b, _xCollis* c);
void xRayHitsBound(const _xRay3* r, const _xBound* b, _xCollis* c);

#endif
