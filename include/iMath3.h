#ifndef IMATH3_H
#define IMATH3_H

#include "xMath3.h"

struct _xBox;
struct _xCylinder;

void iMath3Init();
void iMath3Exit();
void iSphereIsectVec(const xSphere* s, const xVec3* v, xIsect* isx);
void iSphereIsectRay(const xSphere* s, const _xRay3* r, xIsect* isx);
void iSphereIsectSphere(const xSphere* s, const xSphere* p, xIsect* isx);
void iSphereInitBoundOBB(xSphere* s, const _xBox* b, const xMat4x3* m);
void iCylinderIsectVec(const _xCylinder* c, const xVec3* v, xIsect* isx);
void iCylinderIsectRay(const _xCylinder* c, const _xRay3* r, xIsect* isx);
void iBoxVecDist(const _xBox* box, const xVec3* v, xIsect* isx);
void iBoxIsectVec(const _xBox* b, const xVec3* v, xIsect* isx);
void iBoxIsectRay(const _xBox* b, const _xRay3* r, xIsect* isx);
void iBoxIsectSphere(const _xBox* box, const xSphere* p, xIsect* isx);
void iBoxInitBoundVec(_xBox* b, const xVec3* v);
void iBoxBoundVec(_xBox* o, const _xBox* b, const xVec3* v);

#endif
