#include <rwdlgcn.h>

// The GameCube texture plugin: each texture keeps a GX texture object, rebuilt when its filter
// or addressing modes change, and _rwDlTextureSet loads it (and its palette) into a GX texture
// map, skipping the load when that map already holds the texture.

// The GX minification and magnification filters for each RenderWare filter mode, and the GX wrap
// mode for each addressing mode.
extern const GXTexFilter lbl_801DE5B8[7][2];
extern const GXTexWrapMode lbl_801DE5F0[5];

// The texture each GX texture map holds.
extern RwTexture* lbl_801DE604[9];

// The draw sync token of the current frame.
extern U16 lbl_8025E800;

// The texture and raster that stand in for no texture.
extern RwRaster* lbl_8025F878;
extern RwTexture* lbl_8025F87C;


#define DLTEXTUR_PALETTED(_raster)                                                                 \
    (RwRasterGetFormat(_raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))

// The texture's LOD bias, from GX's signed fixed point back to a float.
F32 fn_8017CF40(const GXTexObj* texObj)
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
        if (object == lbl_801DE604[i])
        {
            lbl_801DE604[i] = NULL;
        }
    }

    return object;
}

S32 _rwDlTexturePluginAttach(void)
{
    lbl_8025F888 = RwTextureRegisterPlugin(sizeof(RwGCTextureExt), 0x40C, _rwDlTextureConst,
                                           _rwDlTextureDest, NULL);

    return lbl_8025F888;
}

// Rebuilds the texture's GX texture object for its raster, filter and addressing modes and the
// given LOD settings; a palettized raster uses the given palette slot unless it is preloaded.
static inline void DlTextureInitObj(RwTexture* texture, U32 tlut, S32 biasClamp, S32 edgeLOD,
                                    GXAnisotropy maxAniso, F32 lodBias)
{
    RwRaster* raster = texture->raster;
    RwGCTextureExt* textureExt = RWGCTEXTUREEXT(texture);
    RwGCRasterExt* rasterExt = RWGCRASTEREXT(raster->parent);
    U32 format = RwRasterGetFormat(raster);
    GXTexFilter minFilter;
    GXTexFilter magFilter;

    if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        S32 filter;
        U32 tlutName;

        if (textureExt->flags & RWGCTEXTURE_PRELOADED)
        {
            tlutName = GXGetTexObjTlut(&textureExt->texObj);
        }
        else
        {
            tlutName = tlut;
        }

        GXInitTexObjCI(&textureExt->texObj, rasterExt->pixels, raster->width, raster->height,
                       rasterExt->format,
                       lbl_801DE5F0[(texture->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8],
                       lbl_801DE5F0[(texture->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12],
                       (format & rwRASTERFORMATMIPMAP) != 0, tlutName);

        filter = texture->filterAddressing & rwTEXTUREFILTERMODEMASK;

        if (filter == rwFILTERLINEARMIPLINEAR || filter == rwFILTERLINEARMIPNEAREST)
        {
            minFilter = lbl_801DE5B8[rwFILTERLINEARMIPNEAREST - 1][0];
            magFilter = lbl_801DE5B8[rwFILTERLINEARMIPNEAREST - 1][1];
        }
        else
        {
            minFilter = lbl_801DE5B8[filter][0];
            magFilter = lbl_801DE5B8[filter][1];
        }
    }
    else
    {
        GXInitTexObj(&textureExt->texObj, rasterExt->pixels, raster->width, raster->height,
                     rasterExt->format,
                     lbl_801DE5F0[(texture->filterAddressing & rwTEXTUREADDRESSINGUMASK) >> 8],
                     lbl_801DE5F0[(texture->filterAddressing & rwTEXTUREADDRESSINGVMASK) >> 12],
                     (format & rwRASTERFORMATMIPMAP) != 0);
        minFilter = lbl_801DE5B8[texture->filterAddressing & rwTEXTUREFILTERMODEMASK][0];
        magFilter = lbl_801DE5B8[texture->filterAddressing & rwTEXTUREFILTERMODEMASK][1];
    }

    GXInitTexObjLOD(&textureExt->texObj, minFilter, magFilter, 0.0f, rasterExt->maxLevel, lodBias,
                    biasClamp, edgeLOD, maxAniso);
    textureExt->flags = ((textureExt->flags & 0xFFFF0000) | (texture->filterAddressing & 0xFFFF)) &
                        ~RWGCTEXTURE_DEFAULTLOD;
}

// Rebuilds the texture object with its current LOD settings.
static inline void DlTextureRefresh(RwTexture* texture, U32 tlut)
{
    RwGCTextureExt* textureExt = RWGCTEXTUREEXT(texture);
    GXAnisotropy maxAniso = GXGetTexObjMaxAniso(&textureExt->texObj);
    GXBool edgeLOD = GXGetTexObjEdgeLOD(&textureExt->texObj);
    GXBool biasClamp = GXGetTexObjBiasClamp(&textureExt->texObj);
    F32 lodBias = fn_8017CF40(&textureExt->texObj);

    DlTextureInitObj(texture, tlut, biasClamp, edgeLOD, maxAniso, lodBias);
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
        texture = lbl_8025F87C;
        _rwDlTextureSetRaster(lbl_8025F87C, lbl_8025F878, 0);
    }

    raster = texture->raster;
    textureExt = RWGCTEXTUREEXT(texture);
    rasterExt = RWGCRASTEREXT(raster->parent);
    rasterExt->syncToken = lbl_8025E800;

    if (DLTEXTUR_PALETTED(raster))
    {
        if (textureExt->flags & RWGCTEXTURE_DEFAULTLOD)
        {
            DlTextureInitObj(texture, texMap, GX_TRUE, GX_TRUE, GX_ANISO_1, 0.0f);
        }
        else if ((U16)texture->filterAddressing != (U16)textureExt->flags)
        {
            DlTextureRefresh(texture, texMap);
        }
        else if (texture != lbl_801DE604[texMap])
        {
            if (!(textureExt->flags & RWGCTEXTURE_PRELOADED) &&
                texMap != GXGetTexObjTlut(&textureExt->texObj))
            {
                DlTextureRefresh(texture, texMap);
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
        DlTextureRefresh(texture, 0);
    }
    else if (texture == lbl_801DE604[texMap])
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

    lbl_801DE604[texMap] = texture;
}

void RwGameCubeTextureSetLOD(RwTexture* texture, S32 biasClamp, S32 edgeLOD,
                             GXAnisotropy maxAniso, F32 lodBias)
{
    DlTextureInitObj(texture, 0, biasClamp, edgeLOD, maxAniso, lodBias);
}
