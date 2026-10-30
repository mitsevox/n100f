#ifndef XPAR_H
#define XPAR_H

#include "types.h"
#include "xVec3.h"

struct zParEmitterAsset;

struct xPar
{
    xPar* m_next;
    xPar* m_prev;
    F32 m_lifetime;
    U8 m_c[4];
    xVec3 m_pos;
    F32 m_size;
    xVec3 m_vel;
    F32 m_sizeVel;
    U8 m_flag;
    U8 m_mode;
    U8 m_texIdx[2];
    U8 m_rotdeg[3];
    U8 pad8;
    U32 pad32;
    zParEmitterAsset* m_asset;
    F32 m_cvel[4];
};

void xParInit(xPar* p);
void xParFree(xPar* par);
xPar* xParAlloc();

#endif
