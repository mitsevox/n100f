#include "iDraw.h"

#include <dolphin/gx.h>

// A mask of all ones stops colour and alpha writes; zero turns them back on. Other masks are
// ignored on this platform.
void iDrawSetFBMSK(U32 FBMSK)
{
    if (FBMSK == 0xFFFFFFFF)
    {
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_FALSE);
    }
    else if (FBMSK == 0)
    {
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_TRUE);
    }
}

// The debug line queue does nothing on this platform.
void iDrawBegin()
{
}

void iQueueLineList(iColor_tag col, RwV3d* verts, U32* indices, RwMatrixTag* mat, U32 nverts,
                    U32 nindices)
{
}

void iDrawEnd()
{
}
