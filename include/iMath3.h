#ifndef IMATH3_H
#define IMATH3_H

#include "xMath3.h"

struct _xBox;
struct _xCylinder;

void iMath3Init();
void iMath3Exit();
void iBoxBoundVec(_xBox* o, const _xBox* b, const xVec3* v);
void iBoxInitBoundVec(_xBox* b, const xVec3* v);
void iBoxIsectVec(const _xBox* b, const xVec3* v, xIsect* isx);
void iSphereIsectVec(const xSphere* s, const xVec3* v, xIsect* isx);
void iCylinderIsectVec(const _xCylinder* c, const xVec3* v, xIsect* isx);

#endif
