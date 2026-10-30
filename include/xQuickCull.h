#ifndef XQUICKCULL_H
#define XQUICKCULL_H

#include "xEnt.h"

void xQuickCullForOBB(xQCData* q, const _xBox* b, const xMat4x3* m);
void xQuickCullForBox(xQCData* q, const _xBox* box);
void xQuickCullForSphere(xQCData* q, const xSphere* s);
void xQuickCullForRay(xQCData* q, const xRay3* r);
void xQuickCullForBound(xQCData* q, const _xBound* b);
S32 xQuickCullIsects(const xQCData* a, const xQCData* b);
void xQuickCullInit(const _xBox* box);
void xQuickCullInit(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax);

#endif
