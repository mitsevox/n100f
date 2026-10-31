#ifndef IPARMGR_H
#define IPARMGR_H

#include "types.h"
#include "xMath3.h"

#include <rwcore.h>

struct xParGroup;

// The renderer's shared batch: indices and vertices queue here until a flush draws them as one
// triangle list. The camera's right and up vectors (negated) let particles face the camera.
struct tagiRenderInput
{
    U16* m_index;
    RxObjSpace3DVertex* m_vertex;
    F32* m_vertexTZ;
    U32 m_mode;
    S32 m_vertexType;
    S32 m_vertexTypeSize;
    S32 m_indexCount;
    S32 m_vertexCount;
    xMat4x3 m_camViewMatrix;
    xVec4 m_camViewR;
    xVec4 m_camViewU;
};

struct tagiRenderArrays
{
    U16 m_index[960];
    RxObjSpace3DVertex m_vertex[480];
    F32 m_vertexTZ[480];
};

extern tagiRenderArrays gRenderArr;
extern tagiRenderInput gRenderBuffer;

void iParMgrRenderParSys_Sprite(void* data, xParGroup* ps);
void iParMgrRenderParSys_Streak(void* data, xParGroup* ps);
void iParMgrRenderParSys_Flat(void* data, xParGroup* ps);
void iParMgrRenderParSys_Static(void* data, xParGroup* ps);
void iParMgrRenderParSys_Ground(void* data, xParGroup* ps);
void iParMgrRenderParSys_QuadStreak(void* data, xParGroup* ps);
void iParMgrRenderParSys_InvStreak(void* data, xParGroup* ps);
void iParMgrRender();
void iParMgrUpdate(F32 elapsedTime);
void iParMgrInit();

#endif
