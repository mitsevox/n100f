#include <rpmtex.h>
#include <string.h>

#define MATFX_PLUGIN_ID 0x120
#define MULTITEXTURE_PLUGIN_ID 0x12C
#define MULTITEXTURE_NUMPLATFORMS 9

RwModuleInfo _rpMultiTextureModule;
MultiTextureRegEntry RegEntries[MULTITEXTURE_NUMPLATFORMS];

#define MULTITEXTUREGETDATA(object, offset) (*(RpMultiTexture**)((U8*)(object) + (offset)))

static RpMultiTexture* MultiTextureCreate(MultiTextureRegEntry* regEntry, U32 numTextures)
{
    U32 size = sizeof(RpMultiTexture) + regEntry->size;
    RpMultiTexture* multiTexture = (RpMultiTexture*)RwMalloc(size);

    if (!multiTexture)
    {
        RwError err;

        err.pluginID = MATFX_PLUGIN_ID;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
        return NULL;
    }

    memset(multiTexture, 0, size);
    multiTexture->regEntry = regEntry;
    multiTexture->numTextures = numTextures;

    if (regEntry->size)
    {
        multiTexture->platformData = multiTexture + 1;
    }

    return multiTexture;
}

static void MultiTextureDestroy(RpMultiTexture* multiTexture)
{
    U32 i;

    for (i = 0; i < multiTexture->numTextures; i++)
    {
        if (multiTexture->textures[i])
        {
            RwTextureDestroy(multiTexture->textures[i]);
            multiTexture->textures[i] = NULL;
        }
    }

    if (multiTexture->effect)
    {
        RpMTEffectDestroy(multiTexture->effect);
        multiTexture->effect = NULL;
    }

    RwFree(multiTexture);
}

static void* MultiTextureOpen(void* instance, S32 offset, S32 size)
{
    _rpMultiTextureModule.numInstances++;
    _rpMTEffectOpen();

    return instance;
}

static void* MultiTextureClose(void* instance, S32 offset, S32 size)
{
    _rpMTEffectClose();
    _rpMultiTextureModule.numInstances--;

    return instance;
}

static void* MultiTextureConstructor(void* object, S32 offset, S32 size)
{
    MULTITEXTUREGETDATA(object, offset) = NULL;

    return object;
}

static void* MultiTextureDestructor(void* object, S32 offset, S32 size)
{
    RpMultiTexture* multiTexture = MULTITEXTUREGETDATA(object, offset);

    if (multiTexture)
    {
        MultiTextureDestroy(multiTexture);
        MULTITEXTUREGETDATA(object, offset) = NULL;
    }

    return object;
}

static void* MultiTextureCopy(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    U32 i;
    RpMultiTexture* dstMultiTexture;
    RpMultiTexture* srcMultiTexture = MULTITEXTUREGETDATA(srcObject, offset);

    if (!srcMultiTexture)
    {
        return dstObject;
    }

    dstMultiTexture = MultiTextureCreate(srcMultiTexture->regEntry, srcMultiTexture->numTextures);

    if (!dstMultiTexture)
    {
        return NULL;
    }

    MULTITEXTUREGETDATA(dstObject, offset) = dstMultiTexture;

    for (i = 0; i < srcMultiTexture->numTextures; i++)
    {
        RpMultiTextureSetTexture(dstMultiTexture, i, RpMultiTextureGetTexture(srcMultiTexture, i));
        RpMultiTextureSetCoords(dstMultiTexture, i, RpMultiTextureGetCoords(srcMultiTexture, i));
    }

    RpMultiTextureSetEffect(dstMultiTexture, RpMultiTextureGetEffect(srcMultiTexture));

    return dstObject;
}

static S32 MultiTextureStreamGetSize(const void* object, S32 offset, S32 size)
{
    const RpMultiTexture* multiTexture = MULTITEXTUREGETDATA(object, offset);
    S32 total = 0;

    if (multiTexture)
    {
        U32 i;

        total = multiTexture->numTextures + 2 * sizeof(S32);

        for (i = 0; i < multiTexture->numTextures; i++)
        {
            total += _rpMatFXStreamSizeTexture(multiTexture->textures[i]);
        }

        if (multiTexture->effect)
        {
            total += _rwStringStreamGetSize(multiTexture->effect->name) + 12;
        }
    }

    return total;
}

static RwStream* MultiTextureStreamWrite(RwStream* stream, S32 length, const void* object,
                                         S32 offset, S32 size)
{
    const RpMultiTexture* multiTexture = MULTITEXTUREGETDATA(object, offset);

    if (multiTexture)
    {
        S32 version;
        U8 header[4];
        U32 i;

        version = RwEngineGetVersion();

        if (!RwStreamWriteInt32(stream, &version, sizeof(S32)))
        {
            return NULL;
        }

        header[0] = multiTexture->regEntry->platform;
        header[1] = multiTexture->numTextures;
        header[2] = multiTexture->effect != NULL;
        header[3] = 0;

        if (!RwStreamWrite(stream, header, sizeof(header)))
        {
            return NULL;
        }

        if (multiTexture->numTextures)
        {
            if (!RwStreamWrite(stream, multiTexture->coords, multiTexture->numTextures))
            {
                return NULL;
            }

            for (i = 0; i < multiTexture->numTextures; i++)
            {
                _rpMatFXStreamWriteTexture(stream, multiTexture->textures[i]);
            }
        }

        if (multiTexture->effect)
        {
            if (!_rwStringStreamWrite(multiTexture->effect->name, stream))
            {
                return NULL;
            }
        }
    }

    return stream;
}

static RwStream* MultiTextureStreamRead(RwStream* stream, S32 length, void* object, S32 offset,
                                        S32 size)
{
    S32 version;
    U8 header[4];
    char name[32];
    RpMultiTexture* multiTexture;
    MultiTextureRegEntry* regEntry;
    U32 i;

    if (!RwStreamReadInt32(stream, &version, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamRead(stream, header, sizeof(header)))
    {
        return NULL;
    }

    regEntry = &RegEntries[header[0]];
    multiTexture = MultiTextureCreate(regEntry, header[1]);

    if (!multiTexture)
    {
        return NULL;
    }

    if (header[1])
    {
        if (!RwStreamRead(stream, multiTexture->coords, multiTexture->numTextures))
        {
            MultiTextureDestroy(multiTexture);
            return NULL;
        }

        for (i = 0; i < multiTexture->numTextures; i++)
        {
            _rpMatFXStreamReadTexture(stream, &multiTexture->textures[i]);
        }
    }

    if (header[2] & 1)
    {
        RpMTEffect* effect;

        if (!_rwStringStreamFindAndRead(name, stream))
        {
            MultiTextureDestroy(multiTexture);
            return NULL;
        }

        effect = RpMTEffectFind(name);

        if (!effect)
        {
            effect = RpMTEffectCreateDummy();

            if (!effect)
            {
                MultiTextureDestroy(multiTexture);
                return NULL;
            }

            RpMTEffectSetName(effect, name);
        }

        RpMultiTextureSetEffect(multiTexture, effect);
        RpMTEffectDestroy(effect);
    }

    MULTITEXTUREGETDATA(object, offset) = multiTexture;

    return stream;
}

S32 _rpMultiTexturePluginAttach(void)
{
    if (!_rpMTEffectSystemInit())
    {
        return FALSE;
    }

    memset(RegEntries, 0, sizeof(RegEntries));

    _rpMultiTextureModule.globalsOffset =
        RwEngineRegisterPlugin(sizeof(MTEffectGlobals), MULTITEXTURE_PLUGIN_ID, MultiTextureOpen,
                               MultiTextureClose);

    if (_rpMultiTextureModule.globalsOffset < 0)
    {
        return FALSE;
    }

    return TRUE;
}

S32 _rpMaterialRegisterMultiTexturePlugin(S32 platform, U32 pluginID, U32 size)
{
    S32 offset = RpMaterialRegisterPlugin(sizeof(RpMultiTexture*), pluginID,
                                          MultiTextureConstructor, MultiTextureDestructor,
                                          MultiTextureCopy);
    MultiTextureRegEntry* regEntry;

    if (offset < 0)
    {
        return FALSE;
    }

    regEntry = &RegEntries[platform];
    regEntry->offset = offset;

    if (RpMaterialRegisterPluginStream(pluginID, MultiTextureStreamRead, MultiTextureStreamWrite,
                                       MultiTextureStreamGetSize) < 0)
    {
        return FALSE;
    }

    regEntry->platform = platform;
    regEntry->pluginID = pluginID;
    regEntry->size = size;

    return TRUE;
}

RpMultiTexture* RpMultiTextureSetEffect(RpMultiTexture* multiTexture, RpMTEffect* effect)
{
    if (multiTexture->effect)
    {
        RpMTEffectDestroy(multiTexture->effect);
    }

    multiTexture->effect = effect;

    if (effect)
    {
        RpMTEffectAddRef(multiTexture->effect);
    }

    return multiTexture;
}

RpMTEffect* RpMultiTextureGetEffect(RpMultiTexture* multiTexture)
{
    return multiTexture->effect;
}

RpMultiTexture* RpMultiTextureSetTexture(RpMultiTexture* multiTexture, U32 index,
                                         RwTexture* texture)
{
    if (multiTexture->textures[index])
    {
        RwTextureDestroy(multiTexture->textures[index]);
    }

    multiTexture->textures[index] = texture;

    if (texture)
    {
        texture->refCount++;
    }

    return multiTexture;
}

RwTexture* RpMultiTextureGetTexture(RpMultiTexture* multiTexture, U32 index)
{
    return multiTexture->textures[index];
}

RpMultiTexture* RpMultiTextureSetCoords(RpMultiTexture* multiTexture, U32 index, U32 coords)
{
    multiTexture->coords[index] = coords;

    return multiTexture;
}

U32 RpMultiTextureGetCoords(RpMultiTexture* multiTexture, U32 index)
{
    return multiTexture->coords[index];
}

RpMultiTexture* RpMaterialGetMultiTexture(RpMaterial* material, S32 platform)
{
    MultiTextureRegEntry* regEntry = &RegEntries[platform];

    if (regEntry->pluginID)
    {
        return MULTITEXTUREGETDATA(material, regEntry->offset);
    }

    return NULL;
}
