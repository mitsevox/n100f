#include <rwcore.h>

typedef S32 (*TextureMipmapNameFn)(char* name, char* maskName, U8 mipLevel, S32 format);
typedef RwRaster* (*TextureBuildMipmapsFn)(RwRaster* raster, RwImage* image);

// The texture module's data in the engine instance. Partial: the words at +0x20 and +0x24 are
// not declared.
typedef struct TextureModuleData TextureModuleData;
struct TextureModuleData
{
    RwLinkList texDictList;
    RwFreeList* textureFreeList;
    RwFreeList* texDictFreeList;
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

#define TEXTUREFROMDICTLINK(link) \
    ((RwTexture*)((U8*)(link) - (S32)&((RwTexture*)0)->lInDictionary))

// The object type of a texture dictionary.
#define TEXDICTIONARY_OBJECTTYPE 6

// The longest texture or mask name, with its terminator.
#define TEXTURE_NAMESIZE 32

RwPluginRegistry textureTKList = { sizeof(RwTexture), sizeof(RwTexture) };
RwPluginRegistry texDictTKList = { sizeof(RwTexDictionary), sizeof(RwTexDictionary) };
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

RwTexDictionary* RwTexDictionaryCreate(void)
{
    RwTexDictionary* dict =
        (RwTexDictionary*)RWSRCGLOBAL(memoryAlloc)(TEXTURE_DATA(texDictFreeList));

    if (!dict)
    {
        return NULL;
    }

    rwObjectInitialize(dict, TEXDICTIONARY_OBJECTTYPE, 0);
    rwLinkListAddLLLink(&TEXTURE_DATA(texDictList), &dict->lInInstance);
    rwLinkListInitialize(&dict->texturesInDict);

    _rwPluginRegistryInitObject(&texDictTKList, dict);

    return dict;
}

S32 RwTexDictionaryDestroy(RwTexDictionary* dict)
{
    if (TEXTURE_DATA(currentDict) == dict)
    {
        TEXTURE_DATA(currentDict) = NULL;
    }

    RwTexDictionaryForAllTextures(dict, (RwTextureCallBack)RwTextureDestroy, NULL);

    _rwPluginRegistryDeInitObject(&texDictTKList, dict);
    rwLinkListRemoveLLLink(&dict->lInInstance);

    RWSRCGLOBAL(memoryFree)(TEXTURE_DATA(texDictFreeList), dict);

    return 1;
}

const RwTexDictionary* RwTexDictionaryForAllTextures(const RwTexDictionary* dict,
                                                    RwTextureCallBack fpCallBack, void* pData)
{
    const RwLLLink* next;
    const RwLLLink* cur = dict->texturesInDict.link.next;
    const RwLLLink* end = &dict->texturesInDict.link;

    while (cur != end)
    {
        next = cur->next;

        if (!fpCallBack(TEXTUREFROMDICTLINK(cur), pData))
        {
            break;
        }

        cur = next;
    }

    return dict;
}

RwTexture* RwTextureCreate(RwRaster* raster)
{
    RwTexture* texture = (RwTexture*)RWSRCGLOBAL(memoryAlloc)(TEXTURE_DATA(textureFreeList));

    if (texture)
    {
        texture->dict = NULL;
        texture->name[0] = '\0';
        texture->mask[0] = '\0';
        texture->raster = raster;
        texture->refCount = 1;
        texture->filterAddressing = 0;
        RwTextureSetAddressing(texture, rwTEXTUREADDRESSWRAP);
        RwTextureSetFilterMode(texture, rwFILTERNEAREST);

        _rwPluginRegistryInitObject(&textureTKList, texture);
    }

    return texture;
}

S32 RwTextureDestroy(RwTexture* texture)
{
    texture->refCount--;

    if (texture->refCount <= 0)
    {
        texture->refCount++;

        _rwPluginRegistryDeInitObject(&textureTKList, texture);

        if (texture->dict)
        {
            rwLinkListRemoveLLLink(&texture->lInDictionary);
        }

        if (texture->raster)
        {
            RwRasterDestroy(texture->raster);
            texture->raster = NULL;
        }

        texture->refCount--;

        RWSRCGLOBAL(memoryFree)(TEXTURE_DATA(textureFreeList), texture);
    }

    return 1;
}

RwTexture* RwTextureSetName(RwTexture* texture, const char* name)
{
    rwstrncpy(texture->name, name, TEXTURE_NAMESIZE);

    if (rwstrlen(name) >= TEXTURE_NAMESIZE)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000001E, name, TEXTURE_NAMESIZE, TEXTURE_NAMESIZE - 1,
                                 name[TEXTURE_NAMESIZE - 1]);
        RwErrorSet(&err);
        texture->name[TEXTURE_NAMESIZE - 1] = '\0';
    }

    return texture;
}

RwTexture* RwTextureSetMaskName(RwTexture* texture, const char* maskName)
{
    rwstrncpy(texture->mask, maskName, TEXTURE_NAMESIZE);

    if (rwstrlen(maskName) >= TEXTURE_NAMESIZE)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000001E, maskName, TEXTURE_NAMESIZE, TEXTURE_NAMESIZE - 1,
                                 maskName[TEXTURE_NAMESIZE - 1]);
        RwErrorSet(&err);
        texture->mask[TEXTURE_NAMESIZE - 1] = '\0';
    }

    return texture;
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
