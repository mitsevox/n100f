#ifndef _DOLPHIN_GXGET
#define _DOLPHIN_GXGET

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

GXTexFmt GXGetTexObjFmt(const GXTexObj* obj);
f32 GXGetTexObjLODBias(const GXTexObj* obj);
GXBool GXGetTexObjBiasClamp(const GXTexObj* obj);
GXBool GXGetTexObjEdgeLOD(const GXTexObj* obj);
GXAnisotropy GXGetTexObjMaxAniso(const GXTexObj* obj);
void GXGetProjectionv(f32* p);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXGET
