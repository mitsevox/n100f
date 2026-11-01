#include <rwdlgcn.h>
#include <string.h>
#include <dolphin/os.h>

// The GameCube native texture stream: a texture's sampling settings, then its raster's layout,
// palette and texels exactly as GX reads them, so a reader only copies them back.

// The platform a native texture was written for. Written for this project.
#define DLTEXDIC_PLATFORMGCN 6

// The chunk header, the oldest and newest library versions read, and the versioned write.
// Written for this project.
#define DLTEXDIC_STRUCT 1
#define DLTEXDIC_MINVERSION 0x31000
#define DLTEXDIC_MAXVERSION 0x33002
#define DLTEXDIC_BUILDNUM 0xFFFF

// The size of the stream before the palette and texels, and its size without a raster.
// Written for this project.
#define DLTEXDIC_HEADERSIZE 0x74
#define DLTEXDIC_NORASTERSIZE 0x64

// What a native texture starts with: the platform, the filter and addressing modes, the GX LOD
// settings, and the texture and mask names. Written for this project.
typedef struct DlTextureStreamHeader DlTextureStreamHeader;
struct DlTextureStreamHeader
{
    S32 platform;
    U32 filterAddressing;
    S32 maxAniso;
    S32 biasClamp;
    S32 edgeLOD;
    F32 lodBias;
    char name[32];
    char mask[32];
};

// Its raster: the raster format and type, the size and depth, the mipmap count, the GX texel and
// palette formats and the raster flags. Written for this project.
typedef struct DlRasterStreamHeader DlRasterStreamHeader;
struct DlRasterStreamHeader
{
    U32 format;
    U16 width;
    U16 height;
    U8 depth;
    U8 numLevels;
    U8 texFormat;
    U8 tlutFormat;
    U32 flags;
};

#define DLTEXDIC_PALETTED(_raster) (RwRasterGetFormat(_raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
#define DLTEXDIC_PALETTESIZE(_raster) ((1 << (_raster)->depth) * 2)

S32 RwRasterGetNumLevels(RwRaster* raster);

S32 _rwDlNativeTextureGetSize(S32* size, RwTexture* texture)
{
    RwRaster* raster = texture->raster;
    S32 rasterSize;

    if (!raster)
    {
        *size = DLTEXDIC_NORASTERSIZE;
        return TRUE;
    }

    rasterSize = DLTEXDIC_HEADERSIZE;

    if (DLTEXDIC_PALETTED(raster))
    {
        rasterSize += DLTEXDIC_PALETTESIZE(raster);
    }

    *size = rasterSize + (S32)sizeof(S32) + fn_80179C7C(raster);

    return TRUE;
}

// Writes a native texture.
S32 fn_8017C9E4(RwStream* stream, RwTexture* texture)
{
    S32 size;
    DlTextureStreamHeader header;
    DlRasterStreamHeader rasterHeader;
    S32 dataSize;
    RwRaster* raster;
    RwGCRasterExt* rasterExt;
    RwGCTextureExt* textureExt;

    _rwDlNativeTextureGetSize(&size, texture);
    size -= 0xC;

    if (!_rwStreamWriteVersionedChunkHeader(stream, DLTEXDIC_STRUCT, size, DLTEXDIC_MAXVERSION,
                                            DLTEXDIC_BUILDNUM))
    {
        return FALSE;
    }

    header.platform = DLTEXDIC_PLATFORMGCN;
    textureExt = RWGCTEXTUREEXT(texture);
    header.filterAddressing = texture->filterAddressing & rwTEXTUREFILTERMODEMASK;
    header.filterAddressing = (header.filterAddressing & ~rwTEXTUREADDRESSINGUMASK) |
                              (texture->filterAddressing & rwTEXTUREADDRESSINGUMASK);
    header.filterAddressing = (header.filterAddressing & ~rwTEXTUREADDRESSINGVMASK) |
                              (texture->filterAddressing & rwTEXTUREADDRESSINGVMASK);

    if (textureExt->flags & RWGCTEXTURE_DEFAULTLOD)
    {
        header.biasClamp = GX_TRUE;
        header.maxAniso = GX_ANISO_1;
        header.edgeLOD = GX_TRUE;
        header.lodBias = 0.0f;
    }
    else
    {
        header.maxAniso = GXGetTexObjMaxAniso(&textureExt->texObj);
        header.biasClamp = GXGetTexObjBiasClamp(&textureExt->texObj);
        header.edgeLOD = GXGetTexObjEdgeLOD(&textureExt->texObj);
        header.lodBias = fn_8017CF40(&textureExt->texObj);
    }

    memcpy(header.name, texture->name, sizeof(header.name));
    memcpy(header.mask, texture->mask, sizeof(header.mask));

    if (!RwStreamWrite(stream, &header, sizeof(header)))
    {
        return FALSE;
    }

    size -= sizeof(header);
    raster = texture->raster;
    rasterExt = RWGCRASTEREXT(raster);
    rasterHeader.format = (*(U32*)&raster->cType & ~0xFF00) | ((raster->cFormat << 8) & 0xFF00);
    rasterHeader.width = raster->width;
    rasterHeader.height = raster->height;
    rasterHeader.depth = raster->depth;
    rasterHeader.numLevels = RwRasterGetNumLevels(raster);
    rasterHeader.texFormat = rasterExt->format;
    rasterHeader.tlutFormat = rasterExt->tlutFormat;
    rasterHeader.flags = rasterExt->flags & RWGCRASTER_ALPHA;

    if (!RwStreamWrite(stream, &rasterHeader, sizeof(rasterHeader)))
    {
        return FALSE;
    }

    size -= sizeof(rasterHeader);

    if (DLTEXDIC_PALETTED(raster))
    {
        S32 paletteSize = DLTEXDIC_PALETTESIZE(raster);

        if (!RwStreamWrite(stream, rasterExt->palette, paletteSize))
        {
            return FALSE;
        }

        size -= paletteSize;
    }

    dataSize = fn_80179C7C(raster);

    if (!RwStreamWrite(stream, &dataSize, sizeof(dataSize)))
    {
        return FALSE;
    }

    size -= sizeof(dataSize);

    if (!RwStreamWrite(stream, rasterExt->pixels, dataSize))
    {
        return FALSE;
    }

    size -= dataSize;

    return TRUE;
}

// Reads a native texture.
S32 fn_8017CCBC(RwStream* stream, RwTexture** textureRef)
{
    U32 length;
    U32 version;
    DlTextureStreamHeader header;
    DlRasterStreamHeader rasterHeader;
    S32 dataSize;
    RwRaster* raster;
    RwGCRasterExt* rasterExt;
    RwTexture* texture;
    U32 autoMipmap;

    if (!RwStreamFindChunk(stream, DLTEXDIC_STRUCT, &length, &version))
    {
        return FALSE;
    }

    if (version < DLTEXDIC_MINVERSION || version > DLTEXDIC_MAXVERSION)
    {
        return FALSE;
    }

    if (RwStreamRead(stream, &header, sizeof(header)) != sizeof(header))
    {
        return FALSE;
    }

    if (header.platform != DLTEXDIC_PLATFORMGCN)
    {
        return FALSE;
    }

    if (RwStreamRead(stream, &rasterHeader, sizeof(rasterHeader)) != sizeof(rasterHeader))
    {
        return FALSE;
    }

    raster = RwRasterCreate(rasterHeader.width, rasterHeader.height, rasterHeader.depth,
                            rasterHeader.format | rwRASTERDONTALLOCATE);

    if (!raster)
    {
        return FALSE;
    }

    if (!fn_80179D54(raster, rasterHeader.texFormat, rasterHeader.tlutFormat, rasterHeader.flags,
                     rasterHeader.depth, rasterHeader.numLevels))
    {
        RwRasterDestroy(raster);
        return FALSE;
    }

    raster->cFlags &= ~rwRASTERDONTALLOCATE;
    rasterExt = RWGCRASTEREXT(raster);

    if (DLTEXDIC_PALETTED(raster))
    {
        U32 paletteSize = DLTEXDIC_PALETTESIZE(raster);

        if (RwStreamRead(stream, rasterExt->palette, paletteSize) != paletteSize)
        {
            return FALSE;
        }

        DCFlushRange(rasterExt->palette, DLTEXDIC_PALETTESIZE(raster));
    }

    autoMipmap = raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8);
    raster->cFormat &= ~autoMipmap;

    if (RwStreamRead(stream, &dataSize, sizeof(dataSize)) != sizeof(dataSize))
    {
        return FALSE;
    }

    if (RwStreamRead(stream, rasterExt->pixels, dataSize) != (U32)dataSize)
    {
        return FALSE;
    }

    DCFlushRange(rasterExt->pixels, dataSize);
    GXInvalidateTexAll();
    raster->cFormat |= autoMipmap;

    texture = RwTextureCreate(raster);

    if (!texture)
    {
        RwRasterDestroy(raster);
        return FALSE;
    }

    texture->filterAddressing = (texture->filterAddressing & ~rwTEXTUREFILTERMODEMASK) |
                                (header.filterAddressing & rwTEXTUREFILTERMODEMASK);
    texture->filterAddressing = (texture->filterAddressing & ~rwTEXTUREADDRESSINGUMASK) |
                                (header.filterAddressing & rwTEXTUREADDRESSINGUMASK);
    texture->filterAddressing = (texture->filterAddressing & ~rwTEXTUREADDRESSINGVMASK) |
                                (header.filterAddressing & rwTEXTUREADDRESSINGVMASK);
    RwTextureSetName(texture, header.name);
    RwTextureSetMaskName(texture, header.mask);
    RwGameCubeTextureSetLOD(texture, header.biasClamp, header.edgeLOD, header.maxAniso,
                            header.lodBias);
    *textureRef = texture;

    return TRUE;
}
