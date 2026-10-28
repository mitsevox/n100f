#ifndef XSURFACE_H
#define XSURFACE_H

#include "types.h"
#include "xBase.h"

struct _xEnt;

struct xSurface : xBase
{
    U32 idx;
    U32 type;
    union
    {
        U32 mat_idx;
        _xEnt* ent;
        void* obj;
    };
    F32 friction;
    U8 state;
    U8 pad[3];
    void* moprops;
};

void xSurfaceExit();
xSurface* xSurfaceGetByIdx(U16 n);

#endif
