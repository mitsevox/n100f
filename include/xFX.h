#ifndef XFX_H
#define XFX_H

#include "xMath3.h"

#include <rpworld.h>

extern RpLight* MainLight;
extern F32 EnvMapShininess;

void xFXInit();
void xFX_SceneEnter(RpWorld* world);
void xFX_SceneExit(RpWorld* world);
void xFXUpdate(F32 dt);
RpMaterial* MaterialSetEnvMap2(RpMaterial* material, void* data);
void iModelSetMaterialTexture(RpAtomic* model, void* texture);
void iModelSetMaterialTextureRestore(RpAtomic* model);
void iModelSetMaterialAlpha(RpAtomic* model, S32 alpha);
void iModelMaterialMul(RpAtomic* model, F32 rm, F32 gm, F32 bm);
void iModelResetMaterial(RpAtomic* model);
RpAtomic* xFXanimUVAtomicSetup(RpAtomic* atomic);
void xFXanimUV2PSetTexture(RwTexture* texture);
void xFXanimUV2PSetAngle(F32 angle);
void xFXanimUV2PSetScale(xVec3* scale);
void xFXanimUV2PSetTranslation(xVec3* trans);
void xFXanimUVSetAngle(F32 angle);
void xFXanimUVSetScale(xVec3* scale);
void xFXanimUVSetTranslation(xVec3* trans);

#endif
