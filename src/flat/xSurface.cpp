#include "xSurface.h"
#include "xMemMgr.h"

static xSurface* surfs;
static U16 nsurfs;

// Each surface records its own index in the table.
void xSurfaceInit(U16 num_surfs)
{
    nsurfs = num_surfs;
    if (num_surfs != 0)
    {
        surfs = (xSurface*)xMemAlloc(gActiveHeap, num_surfs * sizeof(xSurface));
        for (U16 i = 0; i < nsurfs; i++)
        {
            surfs[i].idx = i;
        }
    }
    else
    {
        surfs = NULL;
    }
}

void xSurfaceExit()
{
}

void xSurfaceSave(xSurface* ent, xSerial* s)
{
    xBaseSave(ent, s);
}

void xSurfaceLoad(xSurface* ent, xSerial* s)
{
    xBaseLoad(ent, s);
}

void xSurfaceReset(xSurface* ent)
{
}

U16 xSurfaceGetNumSurfaces()
{
    return nsurfs;
}

xSurface* xSurfaceGetByIdx(U16 n)
{
    return surfs != NULL ? &surfs[n] : NULL;
}
