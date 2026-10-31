#ifndef IDRAW_H
#define IDRAW_H

#include "types.h"
#include "iColor.h"

#include <rwcore.h>

struct _xVec3;

void iDrawSetFBMSK(U32 FBMSK);
void iDrawBillboards(_xVec3* pts, U32 count, RwRaster* raster, F32 width, F32 height);
void iDrawBegin();
void iQueueLineList(iColor_tag col, RwV3d* verts, U32* indices, RwMatrixTag* mat, U32 nverts,
                    U32 nindices);
void iDrawEnd();

#endif
