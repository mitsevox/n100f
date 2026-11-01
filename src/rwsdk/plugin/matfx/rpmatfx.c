#include <rpmatfx.h>
#include <string.h>

#define MATFX_PLUGIN_ID 0x120

static MatFXEffect* MatFXMaterialFindEffect(const RpMaterial* material,
                                            RpMatFXMaterialFlags effect)
{
    MatFXMaterialData* data = MATFXMATERIALGETDATA(material);
    U8 i;

    for (i = 0; i < MATFX_NUMSLOTS; i++)
    {
        if (data->effects[i].effect == effect)
        {
            return &data->effects[i];
        }
    }

    return NULL;
}

static MatFXMaterialData* MatFXMaterialDataCreate(RpMaterial* material)
{
    MatFXMaterialData* data =
        (MatFXMaterialData*)RWSRCGLOBAL(memoryAlloc)(MatFXInfo.materialFreeList);

    if (!data)
    {
        return NULL;
    }

    memset(data, 0, sizeof(MatFXMaterialData));
    MATFXMATERIALGETDATA(material) = data;

    return data;
}

S32 fn_80140AF4(void);

// The last engine instance to close destroys the effect pipelines and the material data list.
static void* MatFXClose(void* instance, S32 offset, S32 size)
{
    if (--MatFXInfo.numInstances == 0)
    {
        _rpMatFXPipelinesDestroy();
    }

    if (MatFXInfo.materialFreeList)
    {
        RwFreeListDestroy(MatFXInfo.materialFreeList);
        MatFXInfo.materialFreeList = NULL;
    }

    return instance;
}

// The first engine instance to open creates the material data list (fifty per block) and the
// effect pipelines.
static void* MatFXOpen(void* instance, S32 offset, S32 size)
{
    if (MatFXInfo.numInstances == 0)
    {
        MatFXInfo.materialFreeList = RwFreeListCreate(sizeof(MatFXMaterialData), 50, 4);
        if (!MatFXInfo.materialFreeList)
        {
            return NULL;
        }

        if (!fn_80140AF4())
        {
            return NULL;
        }
    }

    MatFXInfo.numInstances++;

    return instance;
}

static void* MatFXMaterialConstructor(void* object, S32 offset, S32 size)
{
    MATFXMATERIALGETDATA(object) = NULL;

    return object;
}

// Releases the textures each effect holds and the material's effect data.
static void* MatFXMaterialDestructor(void* object, S32 offset, S32 size)
{
    U8 i;
    MatFXEffect* effect;
    MatFXMaterialData* data = MATFXMATERIALGETDATA(object);

    if (data)
    {
        for (i = 0; i < MATFX_NUMSLOTS; i++)
        {
            effect = &data->effects[i];

            switch (effect->effect)
            {
            case rpMATFXEFFECTNULL:
                break;
            case rpMATFXEFFECTBUMPMAP:
                if (effect->data.bumpMap.bumpTexture)
                {
                    RwTextureDestroy(effect->data.bumpMap.bumpTexture);
                }
                if (effect->data.bumpMap.texture)
                {
                    RwTextureDestroy(effect->data.bumpMap.texture);
                }
                break;
            case rpMATFXEFFECTENVMAP:
                if (effect->data.envMap.texture)
                {
                    RwTextureDestroy(effect->data.envMap.texture);
                }
                break;
            case rpMATFXEFFECTDUAL:
                if (effect->data.dual.texture)
                {
                    RwTextureDestroy(effect->data.dual.texture);
                }
                break;
            }
        }

        memset(data, 0, sizeof(MatFXMaterialData));
        RWSRCGLOBAL(memoryFree)(MatFXInfo.materialFreeList, data);
        MATFXMATERIALGETDATA(object) = NULL;
    }

    return object;
}

// Gives the destination material the source's effects, with the same settings; the bump map
// textures are shared (their reference counts raised), the others set through the setters.
static void* MatFXMaterialCopy(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    U8 i;
    MatFXMaterialData* dstData;
    MatFXMaterialData* srcData;
    MatFXEffect* srcEffect;

    srcData = MATFXMATERIALGETDATA(srcObject);
    if (!srcData)
    {
        return NULL;
    }

    dstData = MATFXMATERIALGETDATA(dstObject);
    if (!dstData)
    {
        dstData = MatFXMaterialDataCreate((RpMaterial*)dstObject);
    }

    if (!dstData)
    {
        return NULL;
    }

    RpMatFXMaterialSetEffects((RpMaterial*)dstObject, srcData->flags);

    for (i = 0; i < MATFX_NUMSLOTS; i++)
    {
        srcEffect = &srcData->effects[i];

        switch (srcEffect->effect)
        {
        case rpMATFXEFFECTNULL:
            break;
        case rpMATFXEFFECTBUMPMAP:
        {
            MatFXEffect* dstEffect = &dstData->effects[i];
            RwFrame* frame = RpMatFXMaterialGetBumpMapFrame((const RpMaterial*)srcObject);
            F32 coef = RpMatFXMaterialGetBumpMapCoefficient((const RpMaterial*)srcObject);

            RpMatFXMaterialSetBumpMapFrame((RpMaterial*)dstObject, frame);
            RpMatFXMaterialSetBumpMapCoefficient((RpMaterial*)dstObject, coef);

            dstEffect->data.bumpMap.bumpTexture = srcEffect->data.bumpMap.bumpTexture;
            dstEffect->data.bumpMap.texture = srcEffect->data.bumpMap.texture;

            if (dstEffect->data.bumpMap.bumpTexture)
            {
                dstEffect->data.bumpMap.bumpTexture->refCount++;
            }
            if (dstEffect->data.bumpMap.texture)
            {
                dstEffect->data.bumpMap.texture->refCount++;
            }
            break;
        }
        case rpMATFXEFFECTENVMAP:
        {
            RwTexture* texture = RpMatFXMaterialGetEnvMapTexture((const RpMaterial*)srcObject);
            RwFrame* frame = RpMatFXMaterialGetEnvMapFrame((const RpMaterial*)srcObject);
            F32 coef = RpMatFXMaterialGetEnvMapCoefficient((const RpMaterial*)srcObject);
            S32 useFrameBufferAlpha =
                RpMatFXMaterialGetEnvMapFrameBufferAlpha((const RpMaterial*)srcObject);

            if (texture)
            {
                RpMatFXMaterialSetEnvMapTexture((RpMaterial*)dstObject, texture);
            }
            RpMatFXMaterialSetEnvMapFrame((RpMaterial*)dstObject, frame);
            RpMatFXMaterialSetEnvMapFrameBufferAlpha((RpMaterial*)dstObject, useFrameBufferAlpha);
            RpMatFXMaterialSetEnvMapCoefficient((RpMaterial*)dstObject, coef);
            break;
        }
        case rpMATFXEFFECTDUAL:
        {
            RwTexture* texture = RpMatFXMaterialGetDualTexture((const RpMaterial*)srcObject);
            RwBlendFunction srcBlendMode;
            RwBlendFunction dstBlendMode;

            RpMatFXMaterialGetDualBlendModes((const RpMaterial*)srcObject, &srcBlendMode,
                                             &dstBlendMode);
            if (texture)
            {
                RpMatFXMaterialSetDualTexture((RpMaterial*)dstObject, texture);
            }
            RpMatFXMaterialSetDualBlendModes((RpMaterial*)dstObject, srcBlendMode, dstBlendMode);
            break;
        }
        }
    }

    return dstObject;
}

// Writes whether a texture follows, then the texture; returns the bytes written.
S32 _rpMatFXStreamWriteTexture(RwStream* stream, const RwTexture* texture)
{
    S32 size;
    S32 hasTexture = (texture != NULL);

    RwStreamWrite(stream, &hasTexture, sizeof(S32));
    size = sizeof(S32);

    if (hasTexture)
    {
        S32 textureSize = RwTextureStreamGetSize(texture) + 12;

        RwTextureStreamWrite(texture, stream);
        size += textureSize;
    }

    return size;
}

// Reads whether a texture follows, then the texture; returns the bytes the texture took.
S32 _rpMatFXStreamReadTexture(RwStream* stream, RwTexture** texture)
{
    S32 size;
    S32 hasTexture;
    U32 chunkSize;

    RwStreamRead(stream, &hasTexture, sizeof(S32));
    size = sizeof(S32);

    if (hasTexture)
    {
        if (RwStreamFindChunk(stream, rwID_TEXTURE, &chunkSize, NULL))
        {
            *texture = RwTextureStreamRead(stream);
        }

        size = chunkSize + 12 + sizeof(S32);
    }

    return size;
}

S32 _rpMatFXStreamSizeTexture(const RwTexture* texture)
{
    S32 size = sizeof(S32);

    if (texture)
    {
        size = RwTextureStreamGetSize(texture) + 12 + sizeof(S32);
    }

    return size;
}

// fn_8013DEC4, fn_8013E188 and fn_8013E5CC are the material's plugin stream callbacks (write, read
// and size), and fn_8013E70C to fn_8013E954 the atomic's and the world sector's plugin callbacks
// (constructor, destructor, copy, stream write, stream read, stream size), in bfbb's sequence for
// this file; their names are not established, so they keep their labels.

// Writes the material's effect flags and, for each effect slot, the effect and its settings: a
// bump map's coefficient (negated) and textures, an environment map's frame buffer alpha flag,
// coefficient and texture, a dual pass's blend modes and texture.
static RwStream* fn_8013DEC4(RwStream* stream, S32 binaryLength, const void* object, S32 offset,
                             S32 size)
{
    MatFXMaterialData* data = MATFXMATERIALGETDATA(object);
    RpMatFXMaterialFlags flags;
    U8 i;

    flags = data ? data->flags : rpMATFXEFFECTNULL;

    if (!RwStreamWrite(stream, &flags, sizeof(flags)))
    {
        return NULL;
    }

    if (!flags)
    {
        return NULL;
    }

    for (i = 0; i < MATFX_NUMSLOTS; i++)
    {
        MatFXEffect* effect = &data->effects[i];
        RpMatFXMaterialFlags type = effect->effect;

        if (!RwStreamWrite(stream, &type, sizeof(type)))
        {
            return NULL;
        }

        switch (type)
        {
        case rpMATFXEFFECTBUMPMAP:
        {
            F32 coef = -effect->data.bumpMap.coef;

            if (!RwStreamWrite(stream, &coef, sizeof(coef)))
            {
                return NULL;
            }

            _rpMatFXStreamWriteTexture(stream, effect->data.bumpMap.bumpTexture);
            _rpMatFXStreamWriteTexture(stream, effect->data.bumpMap.texture);
            break;
        }
        case rpMATFXEFFECTENVMAP:
            if (!RwStreamWrite(stream, &effect->data.envMap.coef, sizeof(F32)))
            {
                return NULL;
            }

            if (!RwStreamWrite(stream, &effect->data.envMap.useFrameBufferAlpha, sizeof(S32)))
            {
                return NULL;
            }

            _rpMatFXStreamWriteTexture(stream, effect->data.envMap.texture);
            break;
        case rpMATFXEFFECTDUAL:
            if (!RwStreamWrite(stream, &effect->data.dual.srcBlendMode, sizeof(RwBlendFunction)))
            {
                return NULL;
            }

            if (!RwStreamWrite(stream, &effect->data.dual.dstBlendMode, sizeof(RwBlendFunction)))
            {
                return NULL;
            }

            _rpMatFXStreamWriteTexture(stream, effect->data.dual.texture);
            break;
        case rpMATFXEFFECTNULL:
        default:
            break;
        }
    }

    return stream;
}

// Reads what fn_8013DEC4 writes and sets the effects up through the material setters.
static RwStream* fn_8013E188(RwStream* stream, S32 binaryLength, void* object, S32 offset,
                             S32 size)
{
    RpMaterial* material = (RpMaterial*)object;
    MatFXMaterialData* data = MATFXMATERIALGETDATA(material);
    RpMatFXMaterialFlags flags;
    U8 i;

    if (!data)
    {
        data = MatFXMaterialDataCreate(material);
    }

    if (!data)
    {
        return NULL;
    }

    if (!RwStreamRead(stream, &flags, sizeof(flags)))
    {
        return NULL;
    }

    RwMemNative32(&flags, sizeof(flags));
    RpMatFXMaterialSetEffects(material, flags);

    if (!data->flags)
    {
        return NULL;
    }

    for (i = 0; i < MATFX_NUMSLOTS; i++)
    {
        RpMatFXMaterialFlags type;

        if (!RwStreamRead(stream, &type, sizeof(type)))
        {
            return NULL;
        }

        RwMemNative32(&type, sizeof(type));

        switch (type)
        {
        case rpMATFXEFFECTBUMPMAP:
        {
            RwTexture* bumpTexture = NULL;
            RwTexture* texture = NULL;
            MatFXEffect* effect = &data->effects[i];
            F32 coef;

            if (!RwStreamRead(stream, &coef, sizeof(coef)))
            {
                return NULL;
            }

            RwMemNative32(&coef, sizeof(coef));

            _rpMatFXStreamReadTexture(stream, &bumpTexture);
            _rpMatFXStreamReadTexture(stream, &texture);

            if (bumpTexture)
            {
                effect->data.bumpMap.bumpTexture = bumpTexture;
                effect->data.bumpMap.texture = texture;
                effect->data.bumpMap.invBumpWidth =
                    1.0f / (U32)effect->data.bumpMap.bumpTexture->raster->width;
            }
            else if (texture)
            {
                RpMatFXMaterialSetBumpMapTexture(material, texture);
                RwTextureDestroy(texture);
            }
            else
            {
                effect->data.bumpMap.bumpTexture = NULL;
                effect->data.bumpMap.texture = NULL;
            }

            RpMatFXMaterialSetBumpMapCoefficient(material, coef);
            break;
        }
        case rpMATFXEFFECTENVMAP:
        {
            RwTexture* texture = NULL;
            F32 coef;
            S32 useFrameBufferAlpha;

            if (!RwStreamRead(stream, &coef, sizeof(coef)))
            {
                return NULL;
            }

            RwMemNative32(&coef, sizeof(coef));

            if (!RwStreamRead(stream, &useFrameBufferAlpha, sizeof(useFrameBufferAlpha)))
            {
                return NULL;
            }

            RwMemNative32(&useFrameBufferAlpha, sizeof(useFrameBufferAlpha));

            _rpMatFXStreamReadTexture(stream, &texture);

            if (texture)
            {
                RpMatFXMaterialSetEnvMapTexture(material, texture);
                RwTextureDestroy(texture);
            }

            RpMatFXMaterialSetEnvMapCoefficient(material, coef);
            RpMatFXMaterialSetEnvMapFrameBufferAlpha(material, useFrameBufferAlpha);
            break;
        }
        case rpMATFXEFFECTDUAL:
        {
            RwTexture* texture = NULL;
            RwBlendFunction srcBlendMode;
            RwBlendFunction dstBlendMode;

            if (!RwStreamRead(stream, &srcBlendMode, sizeof(srcBlendMode)))
            {
                return NULL;
            }

            RwMemNative32(&srcBlendMode, sizeof(srcBlendMode));

            if (!RwStreamRead(stream, &dstBlendMode, sizeof(dstBlendMode)))
            {
                return NULL;
            }

            RwMemNative32(&dstBlendMode, sizeof(dstBlendMode));

            _rpMatFXStreamReadTexture(stream, &texture);

            if (texture)
            {
                RpMatFXMaterialSetDualTexture(material, texture);
                RwTextureDestroy(texture);
            }

            RpMatFXMaterialSetDualBlendModes(material, srcBlendMode, dstBlendMode);
            break;
        }
        case rpMATFXEFFECTNULL:
        default:
            break;
        }
    }

    return stream;
}

// The bytes fn_8013DEC4 writes for the material: 0 when it has no effects.
static S32 fn_8013E5CC(const void* object, S32 offset, S32 size)
{
    MatFXMaterialData* data = MATFXMATERIALGETDATA(object);
    S32 total;
    U8 i;

    if (!data || !data->flags)
    {
        return 0;
    }

    total = sizeof(RpMatFXMaterialFlags);

    for (i = 0; i < MATFX_NUMSLOTS; i++)
    {
        MatFXEffect* effect = &data->effects[i];

        total += sizeof(RpMatFXMaterialFlags);

        switch (effect->effect)
        {
        case rpMATFXEFFECTNULL:
            break;
        case rpMATFXEFFECTBUMPMAP:
            total += sizeof(F32);
            total += _rpMatFXStreamSizeTexture(effect->data.bumpMap.bumpTexture);
            total += _rpMatFXStreamSizeTexture(effect->data.bumpMap.texture);
            break;
        case rpMATFXEFFECTENVMAP:
            total += sizeof(F32) + sizeof(S32);
            total += _rpMatFXStreamSizeTexture(effect->data.envMap.texture);
            break;
        case rpMATFXEFFECTDUAL:
            total += sizeof(RwBlendFunction) * 2;
            total += _rpMatFXStreamSizeTexture(effect->data.dual.texture);
            break;
        }
    }

    return total;
}

static void* fn_8013E70C(void* object, S32 offset, S32 size)
{
    MATFXATOMICGETDATA(object) = 0;

    return object;
}

static void* fn_8013E71C(void* object, S32 offset, S32 size)
{
    MATFXATOMICGETDATA(object) = 0;

    return object;
}

static void* fn_8013E72C(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    if (MATFXATOMICGETDATA(srcObject))
    {
        RpMatFXAtomicEnableEffects((RpAtomic*)dstObject);
    }

    return dstObject;
}

static RwStream* fn_8013E76C(RwStream* stream, S32 binaryLength, const void* object, S32 offset,
                             S32 size)
{
    if (!RwStreamWrite(stream, (const S32*)((const U8*)object + MatFXAtomicDataOffset), sizeof(S32)))
    {
        return NULL;
    }

    return stream;
}

static RwStream* fn_8013E7B8(RwStream* stream, S32 binaryLength, void* object, S32 offset,
                             S32 size)
{
    S32 enabled;

    if (!RwStreamRead(stream, &enabled, sizeof(enabled)))
    {
        return NULL;
    }

    RwMemNative32(&enabled, sizeof(enabled));

    if (enabled)
    {
        RpMatFXAtomicEnableEffects((RpAtomic*)object);
    }

    return stream;
}

static S32 fn_8013E82C(const void* object, S32 offset, S32 size)
{
    return sizeof(S32);
}

static void* fn_8013E834(void* object, S32 offset, S32 size)
{
    MATFXWORLDSECTORGETDATA(object) = 0;

    return object;
}

static void* fn_8013E844(void* object, S32 offset, S32 size)
{
    MATFXWORLDSECTORGETDATA(object) = 0;

    return object;
}

static void* fn_8013E854(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    if (MATFXWORLDSECTORGETDATA(srcObject))
    {
        RpMatFXWorldSectorEnableEffects((RpWorldSector*)dstObject);
    }

    return dstObject;
}

static RwStream* fn_8013E894(RwStream* stream, S32 binaryLength, const void* object, S32 offset,
                             S32 size)
{
    if (!RwStreamWrite(stream, (const S32*)((const U8*)object + MatFXWorldSectorDataOffset), sizeof(S32)))
    {
        return NULL;
    }

    return stream;
}

static RwStream* fn_8013E8E0(RwStream* stream, S32 binaryLength, void* object, S32 offset,
                             S32 size)
{
    S32 enabled;

    if (!RwStreamRead(stream, &enabled, sizeof(enabled)))
    {
        return NULL;
    }

    RwMemNative32(&enabled, sizeof(enabled));

    if (enabled)
    {
        RpMatFXWorldSectorEnableEffects((RpWorldSector*)object);
    }

    return stream;
}

static S32 fn_8013E954(const void* object, S32 offset, S32 size)
{
    return sizeof(S32);
}

// The addressing mode a texture uses in both directions, or 0 when the two differ.
#define MATFXTEXTUREADDRESSING(_tex)                                                               \
    ((((_tex)->filterAddressing >> 8) & 0xF) == (((_tex)->filterAddressing >> 12) & 0xF)           \
         ? (((_tex)->filterAddressing >> 12) & 0xF)                                                \
         : 0)

// Builds a bump map texture: the base texture's image (or plain white without one) masked by the
// effect texture's image, resampled to the base size, in a raster of the base raster's mipmap
// flags, with the base texture's addressing and filtering and a name interleaving the two
// textures' names.
RwTexture* _rpMatFXTextureMaskCreate(const RwTexture* baseTexture, const RwTexture* effectTexture)
{
    RwRaster* effectRaster = effectTexture->raster;
    RwRaster* baseRaster = NULL;
    S32 effectWidth = effectRaster->width;
    S32 effectHeight = effectRaster->height;
    S32 width;
    S32 height;
    RwImage* maskImage;
    RwImage* image;
    RwRaster* raster;
    RwTexture* texture;
    S32 rasterWidth;
    S32 rasterHeight;
    S32 rasterDepth;
    S32 rasterFormat;
    U32 addressing;
    U32 filter;

    maskImage = RwImageCreate(effectWidth, effectHeight, 32);
    RwImageAllocatePixels(maskImage);
    RwImageSetFromRaster(maskImage, effectRaster);

    if (baseTexture)
    {
        baseRaster = baseTexture->raster;
        width = baseRaster->width;
        height = baseRaster->height;
        image = RwImageCreate(width, height, 32);
        RwImageAllocatePixels(image);
        RwImageSetFromRaster(image, baseRaster);
    }
    else
    {
        S32 x;
        S32 y;

        width = effectRaster->width;
        height = effectRaster->height;
        image = RwImageCreate(width, height, 32);
        RwImageAllocatePixels(image);

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                *(U32*)(image->cpPixels + image->stride * y + x * 4) = 0xFFFFFFFF;
            }
        }
    }

    if (width != effectWidth || height != effectHeight)
    {
        RwImage* resampled = RwImageCreate(width, height, 32);

        RwImageAllocatePixels(resampled);
        RwImageResample(resampled, maskImage);
        RwImageDestroy(maskImage);
        maskImage = resampled;
    }

    RwImageMakeMask(maskImage);
    RwImageApplyMask(image, maskImage);

    RwImageFindRasterFormat(image, rwRASTERTYPETEXTURE, &rasterWidth, &rasterHeight, &rasterDepth,
                            &rasterFormat);

    if (baseTexture)
    {
        rasterFormat |= RwRasterGetFormat(baseRaster) &
                        (rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);
    }
    else
    {
        rasterFormat |= RwRasterGetFormat(effectRaster) &
                        (rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP);
    }

    raster = RwRasterCreate(rasterWidth, rasterHeight, rasterDepth, rasterFormat);
    RwRasterSetFromImage(raster, image);
    texture = RwTextureCreate(raster);

    if (baseTexture)
    {
        addressing = MATFXTEXTUREADDRESSING(baseTexture);
        filter = baseTexture->filterAddressing & rwTEXTUREFILTERMODEMASK;
    }
    else
    {
        addressing = MATFXTEXTUREADDRESSING(effectTexture);
        filter = effectTexture->filterAddressing & rwTEXTUREFILTERMODEMASK;
    }

    RwTextureSetAddressing(texture, addressing);
    RwTextureSetFilterMode(texture, filter);

    RwImageDestroy(image);
    RwImageDestroy(maskImage);

    {
        char name[32] = "";
        const char* baseName;
        const char* effectName;
        char* dst;
        S32 i;

    if (baseTexture)
    {
        baseName = baseTexture->name;
        effectName = effectTexture->name;
    }
    else
    {
        baseName = effectName = effectTexture->name;
    }

    dst = name;
    i = 0;
    while (i < 31 && (*baseName || *effectName))
    {
        if (*baseName)
        {
            *dst++ = *baseName++;
            i++;
        }

        if (*effectName)
        {
            *dst++ = *effectName++;
            i++;
        }
    }

    RwTextureSetName(texture, name);
    }

    return texture;
}

S32 _rpMultiTexturePlatformPluginsAttach(void);

// Registers the material effects plugin with the engine, materials, atomics and world sectors,
// with their stream callbacks, then the platform's multitexture plugins.
S32 RpMatFXPluginAttach(void)
{
    if (RwEngineRegisterPlugin(0, MATFX_PLUGIN_ID, MatFXOpen, MatFXClose) < 0)
    {
        return FALSE;
    }

    MatFXMaterialDataOffset =
        RpMaterialRegisterPlugin(sizeof(MatFXMaterialData*), MATFX_PLUGIN_ID,
                                 MatFXMaterialConstructor, MatFXMaterialDestructor,
                                 MatFXMaterialCopy);
    if (MatFXMaterialDataOffset < 0)
    {
        return FALSE;
    }

    if (RpMaterialRegisterPluginStream(MATFX_PLUGIN_ID, fn_8013E188, fn_8013DEC4, fn_8013E5CC) < 0)
    {
        return FALSE;
    }

    MatFXAtomicDataOffset = RpAtomicRegisterPlugin(sizeof(S32), MATFX_PLUGIN_ID, fn_8013E70C,
                                                   fn_8013E71C, fn_8013E72C);
    if (MatFXAtomicDataOffset < 0)
    {
        return FALSE;
    }

    if (RpAtomicRegisterPluginStream(MATFX_PLUGIN_ID, fn_8013E7B8, fn_8013E76C, fn_8013E82C) < 0)
    {
        return FALSE;
    }

    MatFXWorldSectorDataOffset = RpWorldSectorRegisterPlugin(
        sizeof(S32), MATFX_PLUGIN_ID, fn_8013E834, fn_8013E844, fn_8013E854);
    if (MatFXWorldSectorDataOffset < 0)
    {
        return FALSE;
    }

    if (RpWorldSectorRegisterPluginStream(MATFX_PLUGIN_ID, fn_8013E8E0, fn_8013E894,
                                          fn_8013E954) < 0)
    {
        return FALSE;
    }

    return _rpMultiTexturePlatformPluginsAttach() != 0;
}

RpAtomic* RpMatFXAtomicEnableEffects(RpAtomic* atomic)
{
    S32* effectsEnabled = &MATFXATOMICGETDATA(atomic);

    if (!*effectsEnabled)
    {
        if (!_rpMatFXPipelineAtomicSetup(atomic))
        {
            return NULL;
        }

        *effectsEnabled = TRUE;
    }

    return atomic;
}

RpWorldSector* RpMatFXWorldSectorEnableEffects(RpWorldSector* worldSector)
{
    S32* effectsEnabled = &MATFXWORLDSECTORGETDATA(worldSector);

    if (!*effectsEnabled)
    {
        if (!_rpMatFXPipelineWorldSectorSetup(worldSector))
        {
            return NULL;
        }

        *effectsEnabled = TRUE;
    }

    return worldSector;
}

RpMaterial* RpMatFXMaterialSetEffects(RpMaterial* material, RpMatFXMaterialFlags flags)
{
    MatFXMaterialData* data = MATFXMATERIALGETDATA(material);

    if (!data)
    {
        data = MatFXMaterialDataCreate(material);
    }

    if (!data)
    {
        return NULL;
    }

    if (flags == rpMATFXEFFECTNULL || (data->flags != rpMATFXEFFECTNULL && data->flags != flags))
    {
        U8 i;

        for (i = 0; i < MATFX_NUMSLOTS; i++)
        {
            MatFXEffect* effect = &data->effects[i];

            switch (effect->effect)
            {
            case rpMATFXEFFECTNULL:
                break;
            case rpMATFXEFFECTBUMPMAP:
                if (effect->data.bumpMap.bumpTexture)
                {
                    RwTextureDestroy(effect->data.bumpMap.bumpTexture);
                }

                if (effect->data.bumpMap.texture)
                {
                    RwTextureDestroy(effect->data.bumpMap.texture);
                }

                break;
            case rpMATFXEFFECTENVMAP:
                if (effect->data.envMap.texture)
                {
                    RwTextureDestroy(effect->data.envMap.texture);
                }

                break;
            case rpMATFXEFFECTDUAL:
                if (effect->data.dual.texture)
                {
                    RwTextureDestroy(effect->data.dual.texture);
                }

                break;
            }
        }

        memset(data, 0, sizeof(MatFXMaterialData));
    }

    data->flags = flags;

    switch (data->flags)
    {
    case rpMATFXEFFECTNULL:
        break;
    case rpMATFXEFFECTBUMPMAP:
        data->effects[0].effect = rpMATFXEFFECTBUMPMAP;
        fn_80140D10(material);
        break;
    case rpMATFXEFFECTENVMAP:
        data->effects[0].effect = rpMATFXEFFECTENVMAP;
        fn_80140D0C(material);
        break;
    case rpMATFXEFFECTBUMPENVMAP:
        data->effects[0].effect = rpMATFXEFFECTBUMPMAP;
        data->effects[1].effect = rpMATFXEFFECTENVMAP;
        fn_80140D10(material);
        fn_80140D0C(material);
        break;
    case rpMATFXEFFECTDUAL:
        data->effects[0].effect = rpMATFXEFFECTDUAL;
        fn_80140D14(material);
        RpMatFXMaterialSetDualBlendModes(material, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
        break;
    }

    return material;
}

RpMaterial* RpMatFXMaterialSetupBumpMap(RpMaterial* material, RwTexture* texture, RwFrame* frame,
                                        F32 coef)
{
    if (RpMatFXMaterialSetBumpMapTexture(material, texture) == NULL)
    {
        return NULL;
    }

    if (RpMatFXMaterialSetBumpMapFrame(material, frame) == NULL)
    {
        return NULL;
    }

    if (RpMatFXMaterialSetBumpMapCoefficient(material, coef) == NULL)
    {
        return NULL;
    }

    return material;
}

RpMaterial* RpMatFXMaterialSetupEnvMap(RpMaterial* material, RwTexture* texture, RwFrame* frame,
                                       S32 useFrameBufferAlpha, F32 coef)
{
    if (RpMatFXMaterialSetEnvMapTexture(material, texture) == NULL)
    {
        return NULL;
    }

    if (RpMatFXMaterialSetEnvMapFrame(material, frame) == NULL)
    {
        return NULL;
    }

    if (RpMatFXMaterialSetEnvMapFrameBufferAlpha(material, useFrameBufferAlpha) == NULL)
    {
        return NULL;
    }

    if (RpMatFXMaterialSetEnvMapCoefficient(material, coef) == NULL)
    {
        return NULL;
    }

    return material;
}

RpMatFXMaterialFlags RpMatFXMaterialGetEffects(const RpMaterial* material)
{
    MatFXMaterialData* data = MATFXMATERIALGETDATA(material);

    if (data == NULL)
    {
        return rpMATFXEFFECTNULL;
    }

    return data->flags;
}

RpMaterial* RpMatFXMaterialSetBumpMapTexture(RpMaterial* material, RwTexture* texture)
{
    MatFXBumpMapData* bumpMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTBUMPMAP)->data.bumpMap;
    RwRaster* raster;

    if (bumpMap->texture)
    {
        RwTextureDestroy(bumpMap->texture);
        bumpMap->texture = NULL;
    }

    if (bumpMap->bumpTexture)
    {
        RwTextureDestroy(bumpMap->bumpTexture);
        bumpMap->bumpTexture = NULL;
        bumpMap->invBumpWidth = 0.0f;
    }

    if (texture)
    {
        RwTexDictionary* dict;

        bumpMap->bumpTexture = _rpMatFXSetupBumpMapTexture(material->texture, texture);

        if (!bumpMap->bumpTexture)
        {
            return NULL;
        }

        bumpMap->texture = texture;
        bumpMap->texture->refCount++;

        dict = RwTexDictionaryGetCurrent();

        if (dict)
        {
            RwTexDictionaryAddTexture(dict, bumpMap->bumpTexture);
        }

        raster = bumpMap->bumpTexture->raster;
    }
    else
    {
        raster = material->texture->raster;
    }

    bumpMap->invBumpWidth = 1.0f / (U32)raster->width;

    return material;
}

RpMaterial* RpMatFXMaterialSetBumpMapFrame(RpMaterial* material, RwFrame* frame)
{
    MatFXBumpMapData* bumpMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTBUMPMAP)->data.bumpMap;

    bumpMap->frame = frame;

    return material;
}

RpMaterial* RpMatFXMaterialSetBumpMapCoefficient(RpMaterial* material, F32 coef)
{
    MatFXBumpMapData* bumpMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTBUMPMAP)->data.bumpMap;

    bumpMap->coef = -coef;

    return material;
}

RwFrame* RpMatFXMaterialGetBumpMapFrame(const RpMaterial* material)
{
    const MatFXBumpMapData* bumpMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTBUMPMAP)->data.bumpMap;

    return bumpMap->frame;
}

F32 RpMatFXMaterialGetBumpMapCoefficient(const RpMaterial* material)
{
    const MatFXBumpMapData* bumpMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTBUMPMAP)->data.bumpMap;

    return -bumpMap->coef;
}

RpMaterial* RpMatFXMaterialSetEnvMapTexture(RpMaterial* material, RwTexture* texture)
{
    MatFXEnvMapData* envMap = &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    texture->refCount++;

    if (envMap->texture)
    {
        RwTextureDestroy(envMap->texture);
        envMap->texture = NULL;
    }

    envMap->texture = texture;

    return material;
}

RpMaterial* RpMatFXMaterialSetEnvMapFrame(RpMaterial* material, RwFrame* frame)
{
    MatFXEnvMapData* envMap = &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    envMap->frame = frame;

    return material;
}

RpMaterial* RpMatFXMaterialSetEnvMapFrameBufferAlpha(RpMaterial* material,
                                                     S32 useFrameBufferAlpha)
{
    MatFXEnvMapData* envMap = &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    envMap->useFrameBufferAlpha = useFrameBufferAlpha;

    return material;
}

RpMaterial* RpMatFXMaterialSetEnvMapCoefficient(RpMaterial* material, F32 coef)
{
    MatFXEnvMapData* envMap = &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    envMap->coef = coef;

    return material;
}

RwTexture* RpMatFXMaterialGetEnvMapTexture(const RpMaterial* material)
{
    const MatFXEnvMapData* envMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    return envMap->texture;
}

RwFrame* RpMatFXMaterialGetEnvMapFrame(const RpMaterial* material)
{
    const MatFXEnvMapData* envMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    return envMap->frame;
}

S32 RpMatFXMaterialGetEnvMapFrameBufferAlpha(const RpMaterial* material)
{
    const MatFXEnvMapData* envMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    return envMap->useFrameBufferAlpha;
}

F32 RpMatFXMaterialGetEnvMapCoefficient(const RpMaterial* material)
{
    const MatFXEnvMapData* envMap =
        &MatFXMaterialFindEffect(material, rpMATFXEFFECTENVMAP)->data.envMap;

    return envMap->coef;
}

RpMaterial* RpMatFXMaterialSetDualTexture(RpMaterial* material, RwTexture* texture)
{
    MatFXDualData* dual = &MatFXMaterialFindEffect(material, rpMATFXEFFECTDUAL)->data.dual;

    texture->refCount++;

    if (dual->texture)
    {
        RwTextureDestroy(dual->texture);
        dual->texture = NULL;
    }

    dual->texture = texture;

    _rpMatFXSetupDualRenderState(dual, rwRENDERSTATETEXTUREADDRESS);
    _rpMatFXSetupDualRenderState(dual, rwRENDERSTATETEXTURERASTER);

    return material;
}

RpMaterial* RpMatFXMaterialSetDualBlendModes(RpMaterial* material, RwBlendFunction srcBlendMode,
                                             RwBlendFunction dstBlendMode)
{
    MatFXDualData* dual = &MatFXMaterialFindEffect(material, rpMATFXEFFECTDUAL)->data.dual;

    dual->srcBlendMode = srcBlendMode;
    dual->dstBlendMode = dstBlendMode;

    _rpMatFXSetupDualRenderState(dual, rwRENDERSTATESRCBLEND);
    _rpMatFXSetupDualRenderState(dual, rwRENDERSTATEDESTBLEND);

    return material;
}

RwTexture* RpMatFXMaterialGetDualTexture(const RpMaterial* material)
{
    const MatFXDualData* dual = &MatFXMaterialFindEffect(material, rpMATFXEFFECTDUAL)->data.dual;

    return dual->texture;
}

const RpMaterial* RpMatFXMaterialGetDualBlendModes(const RpMaterial* material,
                                                   RwBlendFunction* srcBlendMode,
                                                   RwBlendFunction* dstBlendMode)
{
    const MatFXDualData* dual = &MatFXMaterialFindEffect(material, rpMATFXEFFECTDUAL)->data.dual;

    *srcBlendMode = dual->srcBlendMode;
    *dstBlendMode = dual->dstBlendMode;

    return material;
}
