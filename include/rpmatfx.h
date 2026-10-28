#ifndef RPMATFX_H
#define RPMATFX_H

#include <rpworld.h>

typedef enum RpMatFXMaterialFlags
{
    rpMATFXEFFECTNULL = 0,
    rpMATFXEFFECTBUMPMAP = 1,
    rpMATFXEFFECTENVMAP = 2,
    rpMATFXEFFECTBUMPENVMAP = 3,
    rpMATFXEFFECTDUAL = 4,
    rpMATFXEFFECTMAX,
    rpMATFXNUMEFFECTS = rpMATFXEFFECTMAX - 1,
    rpMATFXFORCEENUMSIZEINT = 0x7FFFFFFF
} RpMatFXMaterialFlags;

#ifdef __cplusplus
extern "C" {
#endif

RpAtomic* RpMatFXAtomicEnableEffects(RpAtomic* atomic);
RpWorldSector* RpMatFXWorldSectorEnableEffects(RpWorldSector* worldSector);
RpMaterial* RpMatFXMaterialSetEffects(RpMaterial* material, RpMatFXMaterialFlags flags);
RpMaterial* RpMatFXMaterialSetupBumpMap(RpMaterial* material, RwTexture* texture, RwFrame* frame,
                                        F32 coef);
RpMaterial* RpMatFXMaterialSetupEnvMap(RpMaterial* material, RwTexture* texture, RwFrame* frame,
                                       S32 useFrameBufferAlpha, F32 coef);
RpMatFXMaterialFlags RpMatFXMaterialGetEffects(const RpMaterial* material);
RpMaterial* RpMatFXMaterialSetBumpMapTexture(RpMaterial* material, RwTexture* texture);
RpMaterial* RpMatFXMaterialSetBumpMapFrame(RpMaterial* material, RwFrame* frame);
RpMaterial* RpMatFXMaterialSetBumpMapCoefficient(RpMaterial* material, F32 coef);
RwFrame* RpMatFXMaterialGetBumpMapFrame(const RpMaterial* material);
F32 RpMatFXMaterialGetBumpMapCoefficient(const RpMaterial* material);
RpMaterial* RpMatFXMaterialSetEnvMapTexture(RpMaterial* material, RwTexture* texture);
RpMaterial* RpMatFXMaterialSetEnvMapFrame(RpMaterial* material, RwFrame* frame);
RpMaterial* RpMatFXMaterialSetEnvMapFrameBufferAlpha(RpMaterial* material,
                                                     S32 useFrameBufferAlpha);
RpMaterial* RpMatFXMaterialSetEnvMapCoefficient(RpMaterial* material, F32 coef);
RwTexture* RpMatFXMaterialGetEnvMapTexture(const RpMaterial* material);
RwFrame* RpMatFXMaterialGetEnvMapFrame(const RpMaterial* material);
S32 RpMatFXMaterialGetEnvMapFrameBufferAlpha(const RpMaterial* material);
F32 RpMatFXMaterialGetEnvMapCoefficient(const RpMaterial* material);
RpMaterial* RpMatFXMaterialSetDualTexture(RpMaterial* material, RwTexture* texture);
RpMaterial* RpMatFXMaterialSetDualBlendModes(RpMaterial* material, RwBlendFunction srcBlendMode,
                                             RwBlendFunction dstBlendMode);
RwTexture* RpMatFXMaterialGetDualTexture(const RpMaterial* material);
const RpMaterial* RpMatFXMaterialGetDualBlendModes(const RpMaterial* material,
                                                   RwBlendFunction* srcBlendMode,
                                                   RwBlendFunction* dstBlendMode);

#ifdef __cplusplus
}
#endif

#endif
