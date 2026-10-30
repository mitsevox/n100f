#ifndef IDRAW_H
#define IDRAW_H

#include "types.h"
#include "iColor.h"

#include <rwcore.h>

void iDrawSetFBMSK(U32 FBMSK);
void iDrawBegin();
void iQueueLineList(iColor_tag col, RwV3d* verts, U32* indices, RwMatrixTag* mat, U32 nverts,
                    U32 nindices);
void iDrawEnd();

#endif
