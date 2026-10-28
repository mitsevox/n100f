#include "xSurface.h"

static xSurface* surfs;

xSurface* xSurfaceGetByIdx(U16 n)
{
    return surfs != NULL ? &surfs[n] : NULL;
}
