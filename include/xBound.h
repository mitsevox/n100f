#ifndef XBOUND_H
#define XBOUND_H

#include "xEnt.h"

void xBoundUpdate(_xBound* b);
xVec3* xBoundCenter(const _xBound* b);
void xBoundHitsBound(const _xBound* a, const _xBound* b, _xCollis* c);
void xRayHitsBound(const _xRay3* r, const _xBound* b, _xCollis* c);
void xRayHitsBound(const _xRay3* r, const xQCData* q, const _xBound* b, _xCollis* c);
void xVecHitsBound(const xVec3* v, const _xBound* b, _xCollis* c);
void xBoundHitsModel(const _xBound* b, const xModelInstance* m, _xCollis* colls, U8 ncolls, F32 sth);

#endif
