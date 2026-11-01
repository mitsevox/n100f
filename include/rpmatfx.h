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

#define MATFX_NUMSLOTS 2

typedef struct MatFXBumpMapData MatFXBumpMapData;
struct MatFXBumpMapData
{
    RwFrame* frame;
    RwTexture* bumpTexture;
    RwTexture* texture;
    F32 coef;
    F32 invBumpWidth;
};

typedef struct MatFXEnvMapData MatFXEnvMapData;
struct MatFXEnvMapData
{
    RwFrame* frame;
    RwTexture* texture;
    F32 coef;
    S32 useFrameBufferAlpha;
};

typedef struct MatFXDualData MatFXDualData;
struct MatFXDualData
{
    RwTexture* texture;
    RwBlendFunction srcBlendMode;
    RwBlendFunction dstBlendMode;
};

// One effect of a material: the effect's data, then which effect it is.
typedef struct MatFXEffect MatFXEffect;
struct MatFXEffect
{
    union
    {
        MatFXBumpMapData bumpMap;
        MatFXEnvMapData envMap;
        MatFXDualData dual;
    } data;
    RpMatFXMaterialFlags effect;
};

typedef struct MatFXMaterialData MatFXMaterialData;
struct MatFXMaterialData
{
    MatFXEffect effects[MATFX_NUMSLOTS];
    RpMatFXMaterialFlags flags;
};

// The module's state: the material data comes from the free list at +0x8.
typedef struct MatFXModuleInfo MatFXModuleInfo;
struct MatFXModuleInfo
{
    S32 unk0;
    S32 numInstances;
    RwFreeList* materialFreeList;
    S32 unkC;
};

#define MATFXMATERIALGETDATA(material)                                                             \
    (*(MatFXMaterialData**)((U8*)(material) + MatFXMaterialDataOffset))
#define MATFXATOMICGETDATA(atomic) (*(S32*)((U8*)(atomic) + MatFXAtomicDataOffset))
#define MATFXWORLDSECTORGETDATA(worldSector)                                                       \
    (*(S32*)((U8*)(worldSector) + MatFXWorldSectorDataOffset))

#ifdef __cplusplus
extern "C" {
#endif

extern MatFXModuleInfo MatFXInfo;
extern S32 MatFXMaterialDataOffset;
extern S32 MatFXAtomicDataOffset;
extern S32 MatFXWorldSectorDataOffset;

RpAtomic* _rpMatFXPipelineAtomicSetup(RpAtomic* atomic);
RpWorldSector* _rpMatFXPipelineWorldSectorSetup(RpWorldSector* worldSector);
void fn_80140D0C(RpMaterial* material);
void fn_80140D10(RpMaterial* material);
void fn_80140D14(RpMaterial* material);
S32 _rpMatFXSetupDualRenderState(MatFXDualData* dualData, RwRenderState renderState);
RwTexture* _rpMatFXSetupBumpMapTexture(const RwTexture* baseTexture,
                                       const RwTexture* effectTexture);

#ifdef __cplusplus
}
#endif

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
