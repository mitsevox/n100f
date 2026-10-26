#ifndef RWCORE_H
#define RWCORE_H

#include "types.h"

// RenderWare types are opaque until code that accesses their members is reconstructed.
struct RwMatrixTag;
typedef struct RwMatrixTag RwMatrix;
struct RwCamera;
struct RwFrame;

#ifdef __cplusplus
extern "C" {
#endif

RwCamera* RwCameraShowRaster(RwCamera* camera, void* pDev, U32 flags);
void _rwFrameSyncDirty();

#ifdef __cplusplus
}
#endif

#endif
