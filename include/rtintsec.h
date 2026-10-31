#ifndef RTINTSEC_H
#define RTINTSEC_H

#include "rwcore.h"

#ifdef __cplusplus
extern "C" {
#endif

S32 RtIntersectionSphereTriangle(const RwSphere* sphere, const RwV3d* v0, const RwV3d* v1,
                                 const RwV3d* v2, RwV3d* normal, F32* distance);
S32 RtIntersectionBBoxTriangle(const RwBBox* bbox, const RwV3d* v0, const RwV3d* v1,
                               const RwV3d* v2);

#ifdef __cplusplus
}
#endif

#endif
