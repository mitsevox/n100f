#include <rwcore.h>

typedef S32 (*TextureMipmapNameFn)(char* name, char* maskName, U8 mipLevel, S32 format);
typedef RwRaster* (*TextureBuildMipmapsFn)(RwRaster* raster, RwImage* image);

// The texture module's data in the engine instance. Partial: only the members reconstructed
// code reaches are declared.
typedef struct TextureModuleData TextureModuleData;
struct TextureModuleData
{
    U8 unk0[0x10];
    RwTexDictionary* currentDict;
    RwTextureCallBackRead readFn;
    S32 mipmapping;
    S32 autoMipmapping;
    U8 unk20[0x8];
    TextureBuildMipmapsFn buildMipmapsFn;
    TextureMipmapNameFn mipmapNameFn;
};

#define TEXTURE_DATA(var) \
    (((TextureModuleData*)((U8*)RwEngineInstance + textureModule.globalsOffset))->var)

RwPluginRegistry textureTKList = { sizeof(RwTexture), sizeof(RwTexture) };
static RwModuleInfo textureModule;

RwTexture* TextureDefaultMipmapRead(const char* name, const char* maskName);
RwTexture* TextureDefaultNormalRead(const char* name, const char* maskName);

static RwTexture* TextureDefaultRead(const char* name, const char* maskName)
{
    if (TEXTURE_DATA(mipmapping))
    {
        return TextureDefaultMipmapRead(name, maskName);
    }

    return TextureDefaultNormalRead(name, maskName);
}

S32 RwTextureSetReadCallBack(RwTextureCallBackRead callBack)
{
    TEXTURE_DATA(readFn) = callBack;

    return 1;
}

S32 RwTextureSetMipmapping(S32 enable)
{
    TEXTURE_DATA(mipmapping) = enable;

    return 1;
}

S32 RwTextureGetMipmapping(void)
{
    return TEXTURE_DATA(mipmapping);
}

S32 RwTextureSetAutoMipmapping(S32 enable)
{
    TEXTURE_DATA(autoMipmapping) = enable;

    return 1;
}

S32 RwTextureGetAutoMipmapping(void)
{
    return TEXTURE_DATA(autoMipmapping);
}

RwTexture* RwTexDictionaryAddTexture(RwTexDictionary* dict, RwTexture* texture)
{
    if (texture->dict)
    {
        texture->lInDictionary.prev->next = texture->lInDictionary.next;
        texture->lInDictionary.next->prev = texture->lInDictionary.prev;
    }

    texture->dict = dict;
    texture->lInDictionary.next = dict->texturesInDict.link.next;
    texture->lInDictionary.prev = &dict->texturesInDict.link;
    dict->texturesInDict.link.next->prev = &texture->lInDictionary;
    dict->texturesInDict.link.next = &texture->lInDictionary;

    return texture;
}

RwTexture* RwTexDictionaryRemoveTexture(RwTexture* texture)
{
    if (texture->dict)
    {
        texture->dict = NULL;
        texture->lInDictionary.prev->next = texture->lInDictionary.next;
        texture->lInDictionary.next->prev = texture->lInDictionary.prev;
    }

    return texture;
}

RwTexDictionary* RwTexDictionaryGetCurrent(void)
{
    return TEXTURE_DATA(currentDict);
}

S32 RwTextureGenerateMipmapName(char* name, char* maskName, U8 mipLevel, S32 format)
{
    if (TEXTURE_DATA(mipmapNameFn))
    {
        return TEXTURE_DATA(mipmapNameFn)(name, maskName, mipLevel, format);
    }

    return 0;
}

S32 RwTextureRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                            RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&textureTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RwTextureRasterGenerateMipmaps(RwRaster* raster, RwImage* image)
{
    return TEXTURE_DATA(buildMipmapsFn)(raster, image) != NULL;
}
