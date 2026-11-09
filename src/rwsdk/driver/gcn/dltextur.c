#include <rwdlgcn.h>

// The GameCube texture plugin: each texture keeps a GX texture object, rebuilt when its filter
// or addressing modes change, and _rwDlTextureSet loads it (and its palette) into a GX texture
// map, skipping the load when that map already holds the texture.

// The GX minification and magnification filters for each RenderWare filter mode, and the GX wrap
// mode for each addressing mode.
GXTexFilter _RwDlFilterModeConvTable[7][2] = {
    { GX_NEAR, GX_NEAR },
    { GX_NEAR, GX_NEAR },
    { GX_LINEAR, GX_LINEAR },
    { GX_NEAR_MIP_NEAR, GX_NEAR },
    { GX_LIN_MIP_NEAR, GX_LINEAR },
    { GX_NEAR_MIP_LIN, GX_NEAR },
    { GX_LIN_MIP_LIN, GX_LINEAR },
};
GXTexWrapMode _RwDlAddressConvTable[5] = { GX_CLAMP, GX_REPEAT, GX_MIRROR, GX_CLAMP, GX_CLAMP };

// The texture each of the eight GX texture maps holds.
RwTexture* _RwDlTextureCache[GX_MAX_TEXMAP] = { NULL };

// The offset of the plugin's extension in each texture.
S32 _RwGameCubeTextureExtOffset;

// The draw sync token of the current frame.
extern volatile U16 _RwDlTokenCurrent;

// The texture and raster that stand in for no texture.
extern RwRaster* _RwDlRasterWhite;
extern RwTexture* _RwDlTexture;


#define DLTEXTUR_PALETTED(_raster)                                                                 \
    (RwRasterGetFormat(_raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))

// Stores the three LOD bias conversion constants. Nothing calls it (project-own name and body).
// FAKEMATCH: unreferenced stand-in that only places these constants ahead of 0.0f in .sdata2; real form believed to be code not in this build that used them first
static void DlLODBiasPoolStandIn(F32* offset, F32* scale, F32* step)
{
    *offset = 4096.0f;
    *scale = -1024.0f;
    *step = 1.0f / 1024.0f;
}

// The texture's LOD bias as GX reports it, rescaled: (4096 + bias) / -1024 when negative,
// bias / 1024 otherwise.
F32 DlTextureGetLODBias(const GXTexObj* texObj)
{
    F32 bias = GXGetTexObjLODBias(texObj);

    if (bias < 0.0f)
    {
        return (4096.0f + bias) / -1024.0f;
    }

    bias *= 1.0f / 1024.0f;

    return bias;
}

void* _rwDlTextureConst(void* object, S32 offset, S32 size)
{
    RWGCTEXTUREEXT(object)->flags = RWGCTEXTURE_DEFAULTLOD;

    return object;
}

void* _rwDlTextureDest(void* object, S32 offset, S32 size)
{
    S32 i;

    for (i = 0; i < 8; i++)
    {
        if (object == _RwDlTextureCache[i])
        {
            _RwDlTextureCache[i] = NULL;
        }
    }

    return object;
}

S32 _rwDlTexturePluginAttach(void)
{
    _RwGameCubeTextureExtOffset = RwTextureRegisterPlugin(sizeof(RwGCTextureExt), 0x40C, _rwDlTextureConst,
                                           _rwDlTextureDest, NULL);

    return _RwGameCubeTextureExtOffset;
}

// Rebuilds the texture's GX texture object for its raster, filter and addressing modes and the
// given LOD settings; a palettized raster uses the given palette slot unless it is preloaded.
#define DLTEXTUR_REBUILD(texture, tlut, biasClamp, edgeLOD, maxAniso, lodBias)                      \
    RwRaster* raster = texture->raster;                                                             \
    RwGCTextureExt* textureExt = RWGCTEXTUREEXT(texture);                                           \
    RwGCRasterExt* rasterExt = RWGCRASTEREXT(raster->parent);                                       \
    U32 format = RwRasterGetFormat(raster);                                                         \
    GXTexFilter minFilter;                                                                          \
    GXTexFilter magFilter;                                                                          \
                                                                                                    \
    if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))                                         \
    {                                                                                               \
        S32 filter;                                                                                 \
        U32 tlutName;                                                                               \
                                                                                                    \
        if (textureExt->flags & RWGCTEXTURE_PRELOADED)                                              \
        {                                                                                           \
            tlutName = GXGetTexObjTlut(&textureExt->texObj);                                        \
        }                                                                                           \
        else                                                                                        \
        {                                                                                           \
            tlutName = tlut;                                                                        \
        }                                                                                           \
                                                                                                    \
        GXInitTexObjCI(&textureExt->texObj, rasterExt->pixels, raster->width, raster->height,       \
                       rasterExt->format,                                                           \
                       _RwDlAddressConvTable[(texture->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8],   \
                       _RwDlAddressConvTable[(texture->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12],  \
                       (format & rwRASTERFORMATMIPMAP) != 0, tlutName);                             \
                                                                                                    \
        filter = texture->filterAddressing & rwTEXTUREFILTERMODEMASK;                               \
                                                                                                    \
        if (filter == rwFILTERLINEARMIPLINEAR || filter == rwFILTERLINEARMIPNEAREST)                \
        {                                                                                           \
            minFilter = _RwDlFilterModeConvTable[rwFILTERLINEARMIPNEAREST - 1][0];                              \
            magFilter = _RwDlFilterModeConvTable[rwFILTERLINEARMIPNEAREST - 1][1];                              \
        }                                                                                           \
        else                                                                                        \
        {                                                                                           \
            minFilter = _RwDlFilterModeConvTable[filter][0];                                                    \
            magFilter = _RwDlFilterModeConvTable[filter][1];                                                    \
        }                                                                                           \
    }                                                                                               \
    else                                                                                            \
    {                                                                                               \
        GXInitTexObj(&textureExt->texObj, rasterExt->pixels, raster->width, raster->height,         \
                     rasterExt->format,                                                             \
                     _RwDlAddressConvTable[(texture->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8],     \
                     _RwDlAddressConvTable[(texture->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12],    \
                     (format & rwRASTERFORMATMIPMAP) ? GX_TRUE : GX_FALSE);                         \
        minFilter = _RwDlFilterModeConvTable[texture->filterAddressing & rwTEXTUREFILTERMODEMASK][0];           \
        magFilter = _RwDlFilterModeConvTable[texture->filterAddressing & rwTEXTUREFILTERMODEMASK][1];           \
    }                                                                                               \
                                                                                                    \
    GXInitTexObjLOD(&textureExt->texObj, minFilter, magFilter, 0.0f, rasterExt->maxLevel, lodBias,  \
                    biasClamp, edgeLOD, maxAniso);                                                  \
    textureExt->flags = ((textureExt->flags & 0xFFFF0000) | (texture->filterAddressing & 0xFFFF)) & \
                        ~RWGCTEXTURE_DEFAULTLOD;

// Rebuilds the texture object with the given LOD settings.
static inline void DlTextureInitObj(RwTexture* texture, U32 tlut, S32 biasClamp, S32 edgeLOD,
                                    GXAnisotropy maxAniso, F32 lodBias)
{
    DLTEXTUR_REBUILD(texture, tlut, biasClamp, edgeLOD, maxAniso, lodBias)
}

// Rebuilds the texture object, keeping the LOD settings its current texture object holds.
static inline void DlTextureRefresh(RwTexture* texture, RwGCTextureExt* current, U32 tlut)
{
    GXAnisotropy maxAniso = GXGetTexObjMaxAniso(&current->texObj);
    S32 edgeLOD = GXGetTexObjEdgeLOD(&current->texObj);
    S32 biasClamp = GXGetTexObjBiasClamp(&current->texObj);
    F32 lodBias = DlTextureGetLODBias(&current->texObj);
    DLTEXTUR_REBUILD(texture, tlut, biasClamp, edgeLOD, maxAniso, lodBias)
}

// Loads the texture into the given GX texture map (no texture loads the stand-in), rebuilding its
// texture object first if its settings changed.
void _rwDlTextureSet(RwTexture* texture, U32 texMap)
{
    RwRaster* raster;
    RwGCTextureExt* textureExt;
    RwGCRasterExt* rasterExt;

    if (!texture)
    {
        texture = _RwDlTexture;
        _rwDlTextureSetRaster(_RwDlTexture, _RwDlRasterWhite, 0);
    }

    raster = texture->raster;
    textureExt = RWGCTEXTUREEXT(texture);
    rasterExt = RWGCRASTEREXT(raster->parent);
    rasterExt->syncToken = _RwDlTokenCurrent;

    if (DLTEXTUR_PALETTED(raster))
    {
        if (textureExt->flags & RWGCTEXTURE_DEFAULTLOD)
        {
            DlTextureInitObj(texture, texMap, GX_TRUE, GX_TRUE, GX_ANISO_1, 0.0f);
        }
        else if ((U16)texture->filterAddressing != (U16)textureExt->flags)
        {
            DlTextureRefresh(texture, textureExt, texMap);
        }
        else if (texture != _RwDlTextureCache[texMap])
        {
            if (!(textureExt->flags & RWGCTEXTURE_PRELOADED) &&
                texMap != GXGetTexObjTlut(&textureExt->texObj))
            {
                DlTextureRefresh(texture, textureExt, texMap);
            }
        }
        else
        {
            return;
        }

        if (!(textureExt->flags & RWGCTEXTURE_PRELOADED))
        {
            GXLoadTlut(&rasterExt->tlut, texMap);
        }
    }
    else if (textureExt->flags & RWGCTEXTURE_DEFAULTLOD)
    {
        DlTextureInitObj(texture, 0, GX_TRUE, GX_TRUE, GX_ANISO_1, 0.0f);
    }
    else if ((U16)texture->filterAddressing != (U16)textureExt->flags)
    {
        DlTextureRefresh(texture, textureExt, 0);
    }
    else if (texture == _RwDlTextureCache[texMap])
    {
        return;
    }

    if (!(textureExt->flags & RWGCTEXTURE_PRELOADED))
    {
        GXLoadTexObj(&textureExt->texObj, texMap);
    }
    else
    {
        GXLoadTexObjPreLoaded(&textureExt->texObj, rasterExt->region, texMap);
    }

    _RwDlTextureCache[texMap] = texture;
}

void RwGameCubeTextureSetLOD(RwTexture* texture, F32 lodBias, S32 biasClamp, S32 edgeLOD,
                             GXAnisotropy maxAniso)
{
    DlTextureInitObj(texture, 0, biasClamp, edgeLOD, maxAniso, lodBias);
}
