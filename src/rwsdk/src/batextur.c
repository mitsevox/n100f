#include <rwcore.h>
#include <string.h>

typedef S32 (*TextureMipmapNameFn)(char* name, char* maskName, U8 mipLevel, S32 format);
typedef RwRaster* (*TextureBuildMipmapsFn)(RwRaster* raster, RwImage* image);

// The texture module's data in the engine instance. The buffer and its size are freed and cleared
// at close; nothing in this unit fills them.
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
    void* buffer;
    S32 bufferSize;
    TextureBuildMipmapsFn buildMipmapsFn;
    TextureMipmapNameFn mipmapNameFn;
};

// The palette quantizer's state, as palquant.c defines it.
typedef struct PalQuant PalQuant;
struct PalQuant
{
    void* boxes;
    F32* variances;
    void* root;
    RwFreeList* nodeFreeList;
};

S32 _rwPalQuantInit(PalQuant* pq);
void _rwPalQuantTerm(PalQuant* pq);
void _rwPalQuantAddImage(PalQuant* pq, RwImage* image, F32 weight);
S32 _rwPalQuantResolvePalette(RwRGBA* palette, S32 maxcols, PalQuant* pq);
void _rwPalQuantMatchImage(U8* dstPixels, S32 dstStride, S32 dstDepth, S32 dstPacked,
                           PalQuant* pq, RwImage* img);

S32 RwTextureGenerateMipmapName(char* name, char* maskName, U8 mipLevel, S32 format);

// The longest file name the default readers build, with its terminator.
#define TEXTURE_FILENAMESIZE 256

// The most mipmap levels a texture can have.
#define TEXTURE_MAXMIPLEVELS 16

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

static RwTexDictionary* dummyTexDict;

// Appends "m" and the level's hex digit to the texture and mask names for mip levels 1 to 15;
// level 0 and levels past 15 leave the names alone.
static S32 TextureDefaultMipmapName(char* name, char* maskName, U8 mipLevel, S32 format)
{
    char suffix[3];

    suffix[0] = 'm';
    suffix[1] = (mipLevel > 0 && mipLevel < 16) ? "0123456789abcdef"[mipLevel] : '\0';
    suffix[2] = '\0';

    if (suffix[1])
    {
        rwstrcat(name, suffix);

        if (maskName && *maskName)
        {
            rwstrcat(maskName, suffix);
        }
    }

    return TRUE;
}

// Replaces a 32-bit image with a palettized copy of the given depth.
static S32 PalettizeImage(RwImage** image, S32 depth)
{
    PalQuant palQuant;
    RwRGBA palette[256];
    RwImage* palImage;
    S32 numColours;

    if (!_rwPalQuantInit(&palQuant))
    {
        return FALSE;
    }

    _rwPalQuantAddImage(&palQuant, *image, 1.0f);

    numColours = 1 << depth;
    _rwPalQuantResolvePalette(palette, numColours, &palQuant);

    palImage = RwImageCreate((*image)->width, (*image)->height, depth);

    if (!palImage)
    {
        return FALSE;
    }

    RwImageAllocatePixels(palImage);
    _rwPalQuantMatchImage(palImage->cpPixels, palImage->stride, palImage->depth, FALSE, &palQuant,
                          *image);
    memcpy(palImage->palette, palette, numColours * sizeof(RwRGBA));

    RwImageDestroy(*image);
    *image = palImage;

    _rwPalQuantTerm(&palQuant);

    return TRUE;
}

// Gives a mipmap chain one palette: when every level already has the first level's palette it is
// copied out; otherwise one palette is built from all the levels, each level is replaced by a
// palettized copy sharing it, and the replaced levels are destroyed unless they are keepImage.
static S32 PalettizeMipmaps(RwRGBA* palette, RwImage* keepImage, RwImage** images, S32 numLevels,
                            S32 depth)
{
    PalQuant palQuant;
    RwRGBA* firstPalette = images[0]->palette;
    S32 i;

    if (firstPalette)
    {
        S32 numColours = 1 << depth;

        for (i = 1; i < numLevels; i++)
        {
            RwRGBA* levelPalette = images[i]->palette;

            if (!firstPalette || !levelPalette)
            {
                i = 64;
            }
            else
            {
                S32 j;

                for (j = 0; j < numColours; j++)
                {
                    if (*(U32*)&firstPalette[j] != *(U32*)&levelPalette[j])
                    {
                        i = 64;
                        break;
                    }
                }
            }
        }

        if (i == numLevels)
        {
            memcpy(palette, firstPalette, (1 << images[0]->depth) * sizeof(RwRGBA));
            return TRUE;
        }
    }

    if (!_rwPalQuantInit(&palQuant))
    {
        return FALSE;
    }

    for (i = 0; i < numLevels; i++)
    {
        _rwPalQuantAddImage(&palQuant, images[i], 1.0f);
    }

    _rwPalQuantResolvePalette(palette, 1 << depth, &palQuant);

    for (i = 0; i < numLevels; i++)
    {
        RwImage* image = images[i];
        RwImage* palImage = RwImageCreate(image->width, image->height, depth);

        if (!palImage)
        {
            return FALSE;
        }

        RwImageAllocatePixels(palImage);
        _rwPalQuantMatchImage(palImage->cpPixels, palImage->stride, palImage->depth, FALSE,
                              &palQuant, image);
        palImage->palette = palette;
        images[i] = palImage;

        if (image != keepImage)
        {
            RwImageDestroy(image);
        }
    }

    _rwPalQuantTerm(&palQuant);

    return TRUE;
}

// Copies a name into a file name buffer, cutting it short (with an error) when it does not fit.
static void TextureCopyFileName(char* buffer, const char* name)
{
    rwstrncpy(buffer, name, TEXTURE_FILENAMESIZE);

    if (rwstrlen(name) >= TEXTURE_FILENAMESIZE)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000001E, name, TEXTURE_FILENAMESIZE, TEXTURE_FILENAMESIZE - 1,
                                 name[TEXTURE_FILENAMESIZE - 1]);
        RwErrorSet(&err);
        buffer[TEXTURE_FILENAMESIZE - 1] = '\0';
    }
}

// Reads the image (and mask) behind a texture name with the file extender found for each, picks
// a raster size and format for it when the caller has none, and resamples the image to that size
// (through 32 bits, palettizing again afterwards).
static RwImage* TextureImageReadAndSize(const char* name, const char* maskName, S32 rasterType,
                                        S32* width, S32* height, S32* depth, S32* format)
{
    char fileName[TEXTURE_FILENAMESIZE];
    char maskFileName[TEXTURE_FILENAMESIZE];
    const char* extender;
    RwImage* image;

    TextureCopyFileName(fileName, name);

    extender = RwImageFindFileType(name);

    if (extender)
    {
        rwstrcat(fileName, extender);
    }

    maskFileName[0] = '\0';

    if (maskName && maskName[0])
    {
        TextureCopyFileName(maskFileName, maskName);

        extender = RwImageFindFileType(maskName);

        if (extender)
        {
            rwstrcat(maskFileName, extender);
        }
    }

    image = RwImageReadMaskedImage(fileName, maskFileName);

    if (!image)
    {
        return NULL;
    }

    if ((!*width || !*height) &&
        !RwImageFindRasterFormat(image, rasterType, width, height, depth, format))
    {
        RwError err;

        RwImageDestroy(image);
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000009);
        RwErrorSet(&err);
        return NULL;
    }

    if (image->width != *width || image->height != *height)
    {
        RwImage* origImage = image;
        S32 origDepth = origImage->depth;
        RwImage* resampled;

        if (origDepth != 32)
        {
            image = RwImageCreate(origImage->width, origImage->height, 32);

            if (!image)
            {
                RwImageDestroy(origImage);
                return NULL;
            }

            if (!RwImageAllocatePixels(image))
            {
                RwImageDestroy(image);
                RwImageDestroy(origImage);
                return NULL;
            }

            RwImageCopy(image, origImage);
            RwImageDestroy(origImage);
        }

        resampled = RwImageCreate(*width, *height, 32);

        if (!resampled)
        {
            RwImageDestroy(image);
            return NULL;
        }

        if (!RwImageAllocatePixels(resampled))
        {
            RwImageDestroy(resampled);
            RwImageDestroy(image);
            return NULL;
        }

        RwImageResample(resampled, image);
        RwImageDestroy(image);
        image = resampled;

        if (origDepth == 4 || origDepth == 8)
        {
            PalettizeImage(&image, origDepth);
        }
    }

    return image;
}

// Reads a texture without mipmaps: the image (and mask), a raster to fit it, the image
// palettized for a palettized raster and gamma corrected, then a texture named after the inputs.
static RwTexture* TextureDefaultNormalRead(const char* name, const char* maskName)
{
    RwRGBA palette[256];
    char texName[TEXTURE_FILENAMESIZE];
    char texMask[TEXTURE_FILENAMESIZE];
    RwImage* image;
    RwRaster* raster;
    RwTexture* texture;
    S32 width;
    S32 height;
    S32 depth;
    S32 format;

    TextureCopyFileName(texName, name);
    texMask[0] = '\0';

    if (maskName && maskName[0])
    {
        TextureCopyFileName(texMask, maskName);
    }

    RwTextureGenerateMipmapName(texName, texMask, 0, rwRASTERTYPETEXTURE);

    width = 0;
    height = 0;
    image = TextureImageReadAndSize(texName, texMask, rwRASTERTYPETEXTURE, &width, &height, &depth,
                                    &format);

    if (!image)
    {
        return NULL;
    }

    raster = RwRasterCreate(width, height, depth, format);

    if (!raster)
    {
        RwImageDestroy(image);
        return NULL;
    }

    if (RwRasterGetFormat(raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        if (RwRasterGetFormat(raster) & rwRASTERFORMATPAL4)
        {
            PalettizeMipmaps(palette, NULL, &image, 1, 4);
        }
        else
        {
            PalettizeMipmaps(palette, NULL, &image, 1, 8);
        }

        image->palette = palette;
    }

    RwImageGammaCorrect(image);

    if (!RwRasterSetFromImage(raster, image))
    {
        RwRasterDestroy(raster);
        RwImageDestroy(image);
        return NULL;
    }

    RwImageDestroy(image);

    texture = RwTextureCreate(raster);

    if (!texture)
    {
        RwRasterDestroy(raster);
        return NULL;
    }

    RwTextureSetName(texture, name);

    if (maskName)
    {
        RwTextureSetMaskName(texture, maskName);
    }
    else
    {
        RwTextureSetMaskName(texture, "");
    }

    return texture;
}

// Reads a texture with mipmaps: a raster sized from the top level, then either the raster builds
// its own levels from that image, or each level's image is read under its mipmap name at the
// raster's level size, the chain is palettized together for a palettized raster, gamma corrected
// and written level by level.
static RwTexture* TextureDefaultMipmapRead(const char* name, const char* maskName)
{
    RwRGBA palette[256];
    char texName[TEXTURE_FILENAMESIZE];
    char texMask[TEXTURE_FILENAMESIZE];
    RwImage* images[TEXTURE_MAXMIPLEVELS];
    RwRaster* raster;
    RwTexture* texture;
    S32 rasterType;
    S32 width;
    S32 height;
    S32 depth;
    S32 format;

    TextureCopyFileName(texName, name);
    texMask[0] = '\0';

    if (maskName && maskName[0])
    {
        TextureCopyFileName(texMask, maskName);
    }

    rasterType = rwRASTERTYPETEXTURE;

    if (TEXTURE_DATA(mipmapping))
    {
        rasterType |= rwRASTERFORMATMIPMAP;

        if (TEXTURE_DATA(autoMipmapping))
        {
            rasterType |= rwRASTERFORMATAUTOMIPMAP;
        }
    }

    RwTextureGenerateMipmapName(texName, texMask, 0, rasterType);

    width = 0;
    height = 0;
    images[0] =
        TextureImageReadAndSize(texName, texMask, rasterType, &width, &height, &depth, &format);

    if (!images[0])
    {
        return NULL;
    }

    raster = RwRasterCreate(width, height, depth, format);

    if (!raster)
    {
        RwImageDestroy(images[0]);
        return NULL;
    }

    if (format & rwRASTERFORMATMIPMAP)
    {
        if (format & rwRASTERFORMATAUTOMIPMAP)
        {
            if (!RwRasterSetFromImage(raster, images[0]))
            {
                RwRasterDestroy(raster);
                RwImageDestroy(images[0]);
                return NULL;
            }

            RwImageDestroy(images[0]);
        }
        else
        {
            S32 numLevels = RwRasterGetNumLevels(raster);
            S32 i;

            for (i = 1; i < numLevels; i++)
            {
                TextureCopyFileName(texName, name);
                texMask[0] = '\0';

                if (maskName && maskName[0])
                {
                    TextureCopyFileName(texMask, maskName);
                }

                RwTextureGenerateMipmapName(texName, texMask, (U8)i, rasterType);

                RwRasterLock(raster, (U8)i, rwRASTERLOCKNOFETCH | rwRASTERLOCKWRITE);
                width = raster->width;
                height = raster->height;
                depth = raster->depth;
                format = raster->cType | RwRasterGetFormat(raster);
                RwRasterUnlock(raster);

                images[i] = TextureImageReadAndSize(texName, texMask, rasterType, &width, &height,
                                                    &depth, &format);

                if (!images[i])
                {
                    while (--i >= 0)
                    {
                        RwImageDestroy(images[i]);
                    }

                    RwRasterDestroy(raster);
                    return NULL;
                }
            }

            if (RwRasterGetFormat(raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
            {
                if (RwRasterGetFormat(raster) & rwRASTERFORMATPAL4)
                {
                    PalettizeMipmaps(palette, NULL, images, numLevels, 4);
                }
                else
                {
                    PalettizeMipmaps(palette, NULL, images, numLevels, 8);
                }

                RwImageGammaCorrect(images[0]);
            }
            else
            {
                for (i = 0; i < numLevels; i++)
                {
                    RwImageGammaCorrect(images[i]);
                }
            }

            for (i = 0; i < numLevels; i++)
            {
                if (RwRasterLock(raster, (U8)i, rwRASTERLOCKNOFETCH | rwRASTERLOCKWRITE))
                {
                    if (!RwRasterSetFromImage(raster, images[i]))
                    {
                        for (; i < numLevels; i++)
                        {
                            RwImageDestroy(images[i]);
                        }

                        RwRasterDestroy(raster);
                        return NULL;
                    }

                    RwRasterUnlock(raster);
                }

                RwImageDestroy(images[i]);
            }
        }
    }
    else
    {
        RwImageGammaCorrect(images[0]);

        if (!RwRasterSetFromImage(raster, images[0]))
        {
            RwRasterDestroy(raster);
            RwImageDestroy(images[0]);
            return NULL;
        }

        RwImageDestroy(images[0]);
    }

    texture = RwTextureCreate(raster);

    if (!texture)
    {
        RwRasterDestroy(raster);
        return NULL;
    }

    RwTextureSetName(texture, name);

    if (maskName)
    {
        RwTextureSetMaskName(texture, maskName);
    }
    else
    {
        RwTextureSetMaskName(texture, "");
    }

    return texture;
}

static RwTexture* TextureDefaultRead(const char* name, const char* maskName)
{
    if (TEXTURE_DATA(mipmapping))
    {
        return TextureDefaultMipmapRead(name, maskName);
    }

    return TextureDefaultNormalRead(name, maskName);
}

// Fills a raster's mip levels: the top level from the image (or from the raster itself without
// one) as 32 bits, each lower level resampled from the one above at the raster's level size, the
// chain palettized together for a palettized raster, gamma corrected and written level by level.
// The raster's automatic mipmap flag is off while it works.
static RwRaster* TextureRasterDefaultBuildMipmaps(RwRaster* raster, RwImage* image)
{
    RwImage* images[TEXTURE_MAXMIPLEVELS];
    RwRGBA palette[256];
    U8 autoMipmap;
    S32 numLevels;
    S32 i;

    if (!image)
    {
        images[0] = RwImageCreate(raster->width, raster->height, 32);

        if (images[0])
        {
            if (!RwImageAllocatePixels(images[0]))
            {
                return NULL;
            }

            RwImageSetFromRaster(images[0], raster);
        }
    }
    else if (image->depth != 32)
    {
        images[0] = RwImageCreate(image->width, image->height, 32);

        if (images[0])
        {
            if (!RwImageAllocatePixels(images[0]))
            {
                return NULL;
            }

            RwImageCopy(images[0], image);
        }
    }
    else
    {
        images[0] = image;
    }

    if (!images[0])
    {
        return NULL;
    }

    autoMipmap = raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8);
    raster->cFormat &= ~autoMipmap;

    numLevels = RwRasterGetNumLevels(raster);

    for (i = 1; i < numLevels; i++)
    {
        images[i] = NULL;

        if (RwRasterLock(raster, (U8)i, rwRASTERLOCKREAD))
        {
            images[i] = RwImageCreateResample(images[i - 1], raster->width, raster->height);
            RwRasterUnlock(raster);
        }

        if (!images[i])
        {
            while (--i >= 0)
            {
                if (images[i] != image)
                {
                    RwImageDestroy(images[i]);
                }
            }

            raster->cFormat |= autoMipmap;
            return NULL;
        }
    }

    if (RwRasterGetFormat(raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        S32 result;

        if (RwRasterGetFormat(raster) & rwRASTERFORMATPAL4)
        {
            result = PalettizeMipmaps(palette, image, images, numLevels, 4);
        }
        else
        {
            result = PalettizeMipmaps(palette, image, images, numLevels, 8);
        }

        if (!result)
        {
            // The clean-up returns on its first pass, so only the top level is destroyed.
            for (i = 0; i < numLevels; i++)
            {
                if (images[i] != image)
                {
                    RwImageDestroy(images[i]);
                }

                raster->cFormat |= autoMipmap;
                return NULL;
            }
        }

        RwImageGammaCorrect(images[0]);
    }
    else
    {
        for (i = 0; i < numLevels; i++)
        {
            RwImageGammaCorrect(images[i]);
        }
    }

    for (i = 0; i < numLevels; i++)
    {
        if (RwRasterLock(raster, (U8)i, rwRASTERLOCKNOFETCH | rwRASTERLOCKWRITE))
        {
            RwRasterSetFromImage(raster, images[i]);
            RwRasterUnlock(raster);
        }

        if (images[i] != image)
        {
            RwImageDestroy(images[i]);
        }
    }

    raster->cFormat |= autoMipmap;

    return raster;
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

// Compares two texture names ignoring the case of ASCII letters.
static S32 TextureNamesMatch(const char* name, const char* other)
{
    if (!name)
    {
        return FALSE;
    }

    while (*name && *other)
    {
        char c1 = *name;
        char c2 = *other;

        if (c1 >= 'a' && c1 <= 'z')
        {
            c1 -= 'a' - 'A';
        }

        if (c2 >= 'a' && c2 <= 'z')
        {
            c2 -= 'a' - 'A';
        }

        if (c1 != c2)
        {
            return FALSE;
        }

        name++;
        other++;
    }

    return *name == *other;
}

// Finds a texture in a dictionary by name. The target inlines it into RwTextureRead and has no
// copy of its own.
RwTexture* RwTexDictionaryFindNamedTexture(RwTexDictionary* dict, const char* name)
{
    RwLLLink* cur = rwLinkListGetFirstLLLink(&dict->texturesInDict);

    while (cur != rwLinkListGetTerminator(&dict->texturesInDict))
    {
        RwTexture* texture = TEXTUREFROMDICTLINK(cur);

        if (TextureNamesMatch(texture->name, name))
        {
            return texture;
        }

        cur = rwLLLinkGetNext(cur);
    }

    return NULL;
}

// Returns a texture by name: from the current dictionary, or from any dictionary when there is
// no current one, with its reference count raised; otherwise read through the read callback and
// added to the current dictionary.
RwTexture* RwTextureRead(const char* name, const char* maskName)
{
    RwTexture* texture;

    if (TEXTURE_DATA(currentDict))
    {
        texture = RwTexDictionaryFindNamedTexture(TEXTURE_DATA(currentDict), name);
    }
    else
    {
        RwLLLink* cur = rwLinkListGetFirstLLLink(&TEXTURE_DATA(texDictList));

        texture = NULL;

        while (cur != rwLinkListGetTerminator(&TEXTURE_DATA(texDictList)))
        {
            RwTexDictionary* dict = rwLLLinkGetData(cur, RwTexDictionary, lInInstance);

            texture = RwTexDictionaryFindNamedTexture(dict, name);

            if (texture)
            {
                break;
            }

            cur = rwLLLinkGetNext(cur);
        }
    }

    if (texture)
    {
        texture->refCount++;
        return texture;
    }

    texture = TEXTURE_DATA(readFn)(name, maskName);

    if (!texture)
    {
        RwError err;

        if (maskName)
        {
            err.pluginID = 1;
            err.errorCode = _rwerror(0x16, name, maskName);
            RwErrorSet(&err);
        }
        else
        {
            err.pluginID = 1;
            err.errorCode = _rwerror(0x16, name, "(null)");
            RwErrorSet(&err);
        }

        return NULL;
    }

    if (TEXTURE_DATA(currentDict))
    {
        RwTexDictionaryAddTexture(TEXTURE_DATA(currentDict), texture);
    }

    return texture;
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

void* _rwTextureClose(void* instance, S32 offset, S32 size)
{
    if (TEXTURE_DATA(buffer))
    {
        RwFree(TEXTURE_DATA(buffer));
        TEXTURE_DATA(buffer) = NULL;
        TEXTURE_DATA(bufferSize) = 0;
    }

    if (TEXTURE_DATA(textureFreeList) && TEXTURE_DATA(texDictFreeList))
    {
        RwTexDictionary* dummy = dummyTexDict;
        RwLLLink* cur = rwLinkListGetFirstLLLink(&TEXTURE_DATA(texDictList));

        while (cur != rwLinkListGetTerminator(&TEXTURE_DATA(texDictList)))
        {
            RwTexDictionary* dict = rwLLLinkGetData(cur, RwTexDictionary, lInInstance);

            cur = rwLLLinkGetNext(cur);

            if (dict == dummy)
            {
                RwTexDictionaryDestroy(dummy);
                dummyTexDict = NULL;
                break;
            }
        }
    }

    if (TEXTURE_DATA(textureFreeList))
    {
        RwFreeListDestroy(TEXTURE_DATA(textureFreeList));
        TEXTURE_DATA(textureFreeList) = NULL;
    }

    if (TEXTURE_DATA(texDictFreeList))
    {
        RwFreeListDestroy(TEXTURE_DATA(texDictFreeList));
        TEXTURE_DATA(texDictFreeList) = NULL;
    }

    textureModule.numInstances--;

    return instance;
}

void* _rwTextureOpen(void* instance, S32 offset, S32 size)
{
    textureModule.globalsOffset = offset;

    TEXTURE_DATA(textureFreeList) = RwFreeListCreate(textureTKList.sizeOfStruct, 20, 0);

    if (!TEXTURE_DATA(textureFreeList))
    {
        return NULL;
    }

    TEXTURE_DATA(texDictFreeList) = RwFreeListCreate(texDictTKList.sizeOfStruct, 5, 0);

    if (!TEXTURE_DATA(texDictFreeList))
    {
        RwFreeListDestroy(TEXTURE_DATA(textureFreeList));
        TEXTURE_DATA(textureFreeList) = NULL;
        return NULL;
    }

    rwLinkListInitialize(&TEXTURE_DATA(texDictList));

    textureModule.numInstances++;

    dummyTexDict = RwTexDictionaryCreate();
    TEXTURE_DATA(currentDict) = dummyTexDict;

    if (!TEXTURE_DATA(currentDict))
    {
        RwFreeListDestroy(TEXTURE_DATA(texDictFreeList));
        TEXTURE_DATA(texDictFreeList) = NULL;
        RwFreeListDestroy(TEXTURE_DATA(textureFreeList));
        TEXTURE_DATA(textureFreeList) = NULL;
        return NULL;
    }

    TEXTURE_DATA(mipmapping) = FALSE;
    TEXTURE_DATA(autoMipmapping) = FALSE;
    TEXTURE_DATA(readFn) = TextureDefaultRead;
    TEXTURE_DATA(buildMipmapsFn) = TextureRasterDefaultBuildMipmaps;
    TEXTURE_DATA(mipmapNameFn) = TextureDefaultMipmapName;
    TEXTURE_DATA(buffer) = NULL;
    TEXTURE_DATA(bufferSize) = 0;

    return instance;
}
