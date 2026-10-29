#ifndef RPMTEX_H
#define RPMTEX_H

#include <rpworld.h>

// A named multitexture effect, shared by reference and kept in an effect dictionary.
typedef struct RpMTEffect RpMTEffect;
struct RpMTEffect
{
    S32 platform;
    U32 refCount;
    char name[32];
    RwLLLink inDictLink;
};

typedef struct RpMTEffectDict RpMTEffectDict;

typedef struct RpMultiTexture RpMultiTexture;

// The multitexture plugin's word block in the engine instance.
typedef struct MTEffectGlobals MTEffectGlobals;
struct MTEffectGlobals
{
    RwLinkList dictList;
    RpMTEffectDict* currentDict;
    U32 pathSize;
    char* path;
    char* fullPath;
};

extern RwModuleInfo _rpMultiTextureModule;

#ifdef __cplusplus
extern "C" {
#endif

S32 _rpMultiTexturePluginAttach(void);
S32 _rpMaterialRegisterMultiTexturePlugin(S32 platform, U32 pluginID, U32 size);
RpMultiTexture* RpMultiTextureSetEffect(RpMultiTexture* multiTexture, RpMTEffect* effect);
RpMTEffect* RpMultiTextureGetEffect(RpMultiTexture* multiTexture);
RpMultiTexture* RpMultiTextureSetTexture(RpMultiTexture* multiTexture, U32 index,
                                         RwTexture* texture);
RwTexture* RpMultiTextureGetTexture(RpMultiTexture* multiTexture, U32 index);
RpMultiTexture* RpMultiTextureSetCoords(RpMultiTexture* multiTexture, U32 index, U32 coords);
U32 RpMultiTextureGetCoords(RpMultiTexture* multiTexture, U32 index);
RpMultiTexture* RpMaterialGetMultiTexture(RpMaterial* material, S32 platform);

S32 _rpMTEffectSystemInit(void);
S32 _rpMTEffectOpen(void);
S32 _rpMTEffectClose(void);
RpMTEffectDict* RpMTEffectDictCreate(void);
void RpMTEffectDictDestroy(RpMTEffectDict* dict);
RpMTEffectDict* RpMTEffectDictAddEffect(RpMTEffectDict* dict, RpMTEffect* effect);
RpMTEffect* RpMTEffectDictRemoveEffect(RpMTEffect* effect);
RpMTEffect* RpMTEffectCreateDummy(void);
void RpMTEffectDestroy(RpMTEffect* effect);
RpMTEffect* RpMTEffectFind(const char* name);
RpMTEffect* RpMTEffectSetName(RpMTEffect* effect, const char* name);
RpMTEffect* RpMTEffectAddRef(RpMTEffect* effect);

RwStream* _rpMatFXStreamWriteTexture(RwStream* stream, const RwTexture* texture);
RwStream* _rpMatFXStreamReadTexture(RwStream* stream, RwTexture** texture);
U32 _rpMatFXStreamSizeTexture(const RwTexture* texture);

#ifdef __cplusplus
}
#endif

#endif
