#include <rpmatfx.h>
#include <string.h>

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
