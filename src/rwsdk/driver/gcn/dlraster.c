#include <rwdlgcn.h>
#include <string.h>
#include <dolphin/os.h>

// The GameCube raster driver: rasters keep their texels in GX's tiled layout, 32-byte aligned,
// with every mipmap level after the first and the palette after the texels. Locking a level
// hands out a linear copy (untiled on a read lock), and unlocking tiles it back.

// The draw sync token to issue next, and the last token the GPU has passed.
extern U16 lbl_8025E800;
extern U16 lbl_8025F890;

// The texture that stands in for no texture.
extern RwTexture* lbl_8025F87C;

// "Failed to create surface for texture".
extern char lbl_801DE368[];

S32 _rwDlTokenQueryDone(U16 token);
S32 RwTextureRasterGenerateMipmaps(RwRaster* raster, RwImage* image);

// Draw sync tokens wrap at this value. Written for this project.
#define DLRASTER_TOKENWRAP 0xE000

// The raster types the driver gives texels, and the size of a level that does not shrink below
// one texel. Written for this project.
#define DLRASTER_HASTEXELS(_type)                                                                  \
    ((_type) == rwRASTERTYPENORMAL || (_type) == rwRASTERTYPETEXTURE ||                           \
     (_type) == rwRASTERTYPECAMERATEXTURE)
#define DLRASTER_NUMLEVELS(_raster)                                                                \
    ((RwRasterGetFormat(_raster) & rwRASTERFORMATMIPMAP)                                           \
         ? ((_raster)->width > (_raster)->height ? (U8)((U8)_rwDlFindMSB((_raster)->width) + 1)   \
                                                 : (U8)((U8)_rwDlFindMSB((_raster)->height) + 1)) \
         : (U8)1)
#define DLRASTER_LEVELSIZE(_size, _level) (((_size) >> (_level)) ? ((_size) >> (_level)) : 1)

// Issues a draw sync token if the GPU may still be reading the raster, then waits until the GPU
// has passed the raster's token. Written for this project.
#define DLRASTER_WAITDONE(_ext)                                                                    \
    do                                                                                             \
    {                                                                                              \
        if ((_ext)->syncToken == lbl_8025E800)                                                     \
        {                                                                                          \
            GXSetDrawSync(lbl_8025E800);                                                           \
            lbl_8025E800 = (lbl_8025E800 + 1) % DLRASTER_TOKENWRAP;                                \
        }                                                                                          \
        while (!_rwDlTokenQueryDone((_ext)->syncToken))                                            \
        {                                                                                          \
        }                                                                                          \
    } while (0)

#define DLRASTER_ERROR(_code)                                                                      \
    do                                                                                             \
    {                                                                                              \
        RwError err;                                                                               \
        err.pluginID = 1;                                                                          \
        err.errorCode = _rwerror _code;                                                            \
        RwErrorSet(&err);                                                                          \
    } while (0)

S32 _rwDlRasterPluginAttach(void)
{
    lbl_8025F870 = RwRasterRegisterPlugin(sizeof(RwGCRasterExt), 0x40C, NULL, NULL, NULL);

    return lbl_8025F870;
}

// Copies linear texels into GX tiles: 8x8 texels per 32-byte tile at 4 bits, 8x4 at 8 bits and
// 4x4 at 16 bits; at 32 bits each 4x4 tile is two 32-byte halves, alpha and red first, then
// green and blue.
void DlRasterTile(U8* dst, U8* src, S32 width, S32 height, S32 depth, S32 stride)
{
    S32 x;
    S32 y;
    S32 tilesW;

    switch (depth)
    {
    case 4:
        tilesW = ((width + 7) & ~7) >> 3;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x += 8)
            {
                memcpy(dst + ((U32)(((tilesW * (y >> 3) + (x >> 3)) << 6) + ((y & 7) << 3)) >> 1),
                       src + y * stride + (x >> 1), 4);
            }
        }
        break;
    case 8:
        tilesW = ((width + 7) & ~7) >> 3;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x += 8)
            {
                memcpy(dst + (((tilesW * (y >> 2) + (x >> 3)) << 5) + ((y & 3) << 3)),
                       src + y * stride + x, 8);
            }
        }
        break;
    case 16:
        tilesW = ((width + 3) & ~3) >> 2;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x += 4)
            {
                memcpy(dst + ((((tilesW * (y >> 2) + (x >> 2)) << 4) + ((y & 3) << 2)) << 1),
                       src + y * stride + (x << 1), 8);
            }
        }
        break;
    case 32:
        tilesW = ((width + 3) & ~3) >> 2;

        for (y = 0; y < height; y++)
        {
            U8* row = src + y * stride;
            S32 tile = 0;

            for (x = 0; x < width; x++)
            {
                S32 texel;
                U8* pixel = row + (x << 2);

                if (!(x & 3))
                {
                    tile = (tilesW * (y >> 2) + (x >> 2)) << 4;
                }

                texel = ((y & 3) << 2) + (x & 3);
                *(U16*)(dst + (tile << 2) + (texel << 1)) = *(U16*)pixel;
                *(U16*)(dst + (tile << 2) + (texel << 1) + 0x20) = *(U16*)(pixel + 2);
            }
        }
        break;
    case 24:
    default:
        DLRASTER_ERROR((0x8000000C));
        break;
    }
}

// The reverse of DlRasterTile.
void DlRasterUntile(U8* dst, U8* src, S32 width, S32 height, S32 depth, S32 stride)
{
    S32 x;
    S32 y;
    S32 tilesW;

    switch (depth)
    {
    case 4:
        tilesW = ((width + 7) & ~7) >> 3;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x += 8)
            {
                memcpy(dst + y * stride + (x >> 1),
                       src + ((U32)(((tilesW * (y >> 3) + (x >> 3)) << 6) + ((y & 7) << 3)) >> 1), 4);
            }
        }
        break;
    case 8:
        tilesW = ((width + 7) & ~7) >> 3;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x += 8)
            {
                memcpy(dst + y * stride + x,
                       src + (((tilesW * (y >> 2) + (x >> 3)) << 5) + ((y & 3) << 3)), 8);
            }
        }
        break;
    case 16:
        tilesW = ((width + 3) & ~3) >> 2;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x += 4)
            {
                memcpy(dst + y * stride + (x << 1),
                       src + ((((tilesW * (y >> 2) + (x >> 2)) << 4) + ((y & 3) << 2)) << 1), 8);
            }
        }
        break;
    case 32:
        tilesW = ((width + 3) & ~3) >> 2;

        for (y = 0; y < height; y++)
        {
            U8* row = dst + y * stride;
            S32 tile = 0;

            for (x = 0; x < width; x++)
            {
                S32 texel;
                U8* pixel = row + (x << 2);

                if (!(x & 3))
                {
                    tile = (tilesW * (y >> 2) + (x >> 2)) << 4;
                }

                texel = ((y & 3) << 2) + (x & 3);
                *(U16*)pixel = *(U16*)(src + (tile << 2) + (texel << 1));
                *(U16*)(pixel + 2) = *(U16*)(src + (tile << 2) + (texel << 1) + 0x20);
            }
        }
        break;
    case 24:
    default:
        DLRASTER_ERROR((0x8000000C));
        break;
    }
}

// The size of one mipmap level of the raster's texels, in GX tiles, rounded to 32 bytes. A
// locked raster keeps its full size.
S32 fn_80179064(RwRaster* raster, U8 level)
{
    RwRaster* top = raster->parent;
    S32 width;
    U32 height;
    S32 size = 0;

    if (raster->privateFlags & rwRASTERPIXELLOCKED)
    {
        width = top->width;
        height = top->height;
    }
    else
    {
        width = top->width >> level;
        height = top->height >> level;
        width = width ? width : 1;
        height = height ? height : 1;
    }

    switch (raster->depth)
    {
    case 4:
        width = (width + 7) & ~7;
        height = (height + 7) & ~7;
        size = (width * height) >> 1;
        break;
    case 8:
        width = (width + 7) & ~7;
        height = (height + 3) & ~3;
        size = width * height;
        break;
    case 16:
        width = (width + 3) & ~3;
        height = (height + 3) & ~3;
        size = (width * height) << 1;
        break;
    case 32:
        width = (width + 3) & ~3;
        height = (height + 3) & ~3;
        size = (width * height) << 2;
        break;
    case 24:
    default:
        DLRASTER_ERROR((0x8000000C));
        break;
    }

    return (size + 0x1F) & ~0x1F;
}

// Locks one mipmap level ((flags >> 8) & 0xFF) for reading or writing; a raw lock hands out the
// tiled texels themselves.
S32 fn_80179198(void** pixelsOut, RwRaster* raster, S32 flags)
{
    U8 level = (flags >> 8) & 0xFF;
    RwRaster* top = raster->parent;
    RwGCRasterExt* ext = RWGCRASTEREXT(top);
    S32 stride;
    S32 offset;
    U8 l;
    U8* pixels;

    switch (raster->cType & rwRASTERTYPEMASK)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
    case rwRASTERTYPECAMERATEXTURE:
        {
            S32 width = DLRASTER_LEVELSIZE(top->width, level);

            switch (raster->depth)
            {
            case 4:
                stride = ((width + 7) & ~7) >> 1;
                break;
            case 8:
                stride = (width + 7) & ~7;
                break;
            case 16:
                stride = ((width + 3) & ~3) << 1;
                break;
            case 32:
                stride = ((width + 3) & ~3) << 2;
                break;
            case 24:
            default:
                DLRASTER_ERROR((0x8000000C));
                break;
            }
        }

        offset = 0;

        for (l = level; l--;)
        {
            offset += fn_80179064(raster, l);
        }

        pixels = (U8*)ext->pixels + offset;

        if (ext->format != GX_TF_CMPR && !(flags & rwRASTERLOCKRAW))
        {
            ext->lockBuffer = RwMalloc(stride * DLRASTER_LEVELSIZE(top->height, level));

            if (!ext->lockBuffer)
            {
                return FALSE;
            }
        }

        if (top == raster)
        {
            S32 width = raster->width >> level;
            S32 height = raster->height >> level;

            width = width ? width : 1;
            height = height ? height : 1;

            raster->originalWidth = raster->width;
            raster->originalHeight = raster->height;
            raster->width = width;
            raster->height = height;

            if (ext->format != GX_TF_CMPR && !(flags & rwRASTERLOCKRAW))
            {
                raster->cpPixels = ext->lockBuffer;
            }
            else
            {
                raster->cpPixels = pixels;
            }
        }
        else if (ext->format != GX_TF_CMPR && !(flags & rwRASTERLOCKRAW))
        {
            switch (raster->depth)
            {
            case 4:
                raster->cpPixels =
                    (U8*)ext->lockBuffer + stride * raster->nOffsetY + (raster->nOffsetX >> 1);
                break;
            case 8:
                raster->cpPixels = (U8*)ext->lockBuffer + stride * raster->nOffsetY + raster->nOffsetX;
                break;
            case 16:
                raster->cpPixels =
                    (U8*)ext->lockBuffer + stride * raster->nOffsetY + (raster->nOffsetX << 1);
                break;
            case 32:
                raster->cpPixels =
                    (U8*)ext->lockBuffer + stride * raster->nOffsetY + (raster->nOffsetX << 2);
                break;
            case 24:
            default:
                DLRASTER_ERROR((0x8000000C));
                break;
            }
        }
        else
        {
            DLRASTER_ERROR((0x8000000E));
            return FALSE;
        }

        ext->lockPixels = pixels;
        raster->stride = stride;
        ext->lockedLevel = level;

        if (flags & rwRASTERLOCKREAD)
        {
            if ((raster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
            {
                DCFlushRange(ext->lockPixels, fn_80179064(raster, level));
            }

            raster->privateFlags |= rwRASTERPIXELLOCKEDREAD;
            top->privateFlags |= rwRASTERPIXELLOCKEDREAD;

            if (ext->format != GX_TF_CMPR && !(flags & rwRASTERLOCKRAW))
            {
                DlRasterUntile(ext->lockBuffer, ext->lockPixels, top->width, top->height,
                               raster->depth, raster->stride);
            }
        }

        if (flags & rwRASTERLOCKWRITE)
        {
            raster->privateFlags |= rwRASTERPIXELLOCKEDWRITE;
            top->privateFlags |= rwRASTERPIXELLOCKEDWRITE;
            DLRASTER_WAITDONE(ext);
        }

        if (flags & rwRASTERLOCKRAW)
        {
            raster->privateFlags |= rwRASTERPIXELLOCKEDRAW;
            top->privateFlags |= rwRASTERPIXELLOCKEDRAW;
        }

        break;
    case rwRASTERTYPEZBUFFER:
    case rwRASTERTYPECAMERA:
    default:
        DLRASTER_ERROR((0x8000000E));
        return FALSE;
    }

    *pixelsOut = raster->cpPixels;

    return TRUE;
}

// Unlocks the raster: tiles back what was written, frees the linear copy and, after a write to
// the top level of an automipmapped raster, rebuilds the other levels.
S32 fn_80179614(void* out, RwRaster* raster, S32 flags)
{
    RwRaster* top = raster->parent;
    RwGCRasterExt* ext = RWGCRASTEREXT(top);

    switch (raster->cType & rwRASTERTYPEMASK)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
    case rwRASTERTYPECAMERATEXTURE:

        if (raster->privateFlags & rwRASTERPIXELLOCKEDWRITE)
        {
            if (ext->format != GX_TF_CMPR && !(raster->privateFlags & rwRASTERPIXELLOCKEDRAW))
            {
                DlRasterTile(ext->lockPixels, ext->lockBuffer, top->width, top->height, raster->depth,
                             raster->stride);
            }

            DCFlushRange(ext->lockPixels, fn_80179064(raster, ext->lockedLevel));
            GXInvalidateTexAll();
        }

        if (top == raster)
        {
            raster->width = raster->originalWidth;
            raster->height = raster->originalHeight;
        }

        if (ext->format != GX_TF_CMPR && !(raster->privateFlags & rwRASTERPIXELLOCKEDRAW))
        {
            RwFree(ext->lockBuffer);
            ext->lockBuffer = NULL;
        }

        raster->stride = 0;
        raster->cpPixels = NULL;

        if ((raster->privateFlags & rwRASTERPIXELLOCKEDWRITE) &&
            (raster->cFormat & (rwRASTERFORMATAUTOMIPMAP >> 8)) && ext->lockedLevel == 0)
        {
            ext->lockedLevel = 0xFF;
            raster->privateFlags &= ~(rwRASTERPIXELLOCKED | rwRASTERPIXELLOCKEDRAW);
            top->privateFlags &= ~(rwRASTERPIXELLOCKED | rwRASTERPIXELLOCKEDRAW);
            RwTextureRasterGenerateMipmaps(raster, NULL);
        }
        else
        {
            ext->lockedLevel = 0xFF;
            raster->privateFlags &= ~(rwRASTERPIXELLOCKED | rwRASTERPIXELLOCKEDRAW);
            top->privateFlags &= ~(rwRASTERPIXELLOCKED | rwRASTERPIXELLOCKEDRAW);
        }

        break;
    case rwRASTERTYPEZBUFFER:
    case rwRASTERTYPECAMERA:
    default:
        DLRASTER_ERROR((0x80000011));
        return FALSE;
    }

    return TRUE;
}

// Locks the palette of a top-level palettized raster; the palette is stored after the texels.
S32 fn_801797E0(void** paletteOut, RwRaster* raster, S32 flags)
{
    switch (raster->cType & rwRASTERTYPEMASK)
    {
    case rwRASTERTYPECAMERA:
        return FALSE;
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
    case rwRASTERTYPECAMERATEXTURE:
        if (raster == raster->parent && !raster->palette)
        {
            RwGCRasterExt* ext = RWGCRASTEREXT(raster);

            if (RwRasterGetFormat(raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
            {
                if (flags & rwRASTERLOCKREAD)
                {
                    raster->privateFlags |= rwRASTERPALETTELOCKEDREAD;
                }

                if (flags & rwRASTERLOCKWRITE)
                {
                    raster->privateFlags |= rwRASTERPALETTELOCKEDWRITE;
                }

                raster->palette = ext->palette;
                *paletteOut = raster->palette;
            }
            else
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    return FALSE;
}

S32 fn_8017989C(void* out, RwRaster* raster, S32 flags)
{
    switch (raster->cType & rwRASTERTYPEMASK)
    {
    case rwRASTERTYPECAMERA:
        return FALSE;
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
    case rwRASTERTYPECAMERATEXTURE:
        if (raster == raster->parent)
        {
            RwGCRasterExt* ext = RWGCRASTEREXT(raster);

            if (raster->privateFlags & rwRASTERPALETTELOCKEDWRITE)
            {
                DCFlushRange(ext->palette, (1 << raster->depth) * 2);
            }

            raster->privateFlags &= ~rwRASTERPALETTELOCKED;
            raster->palette = NULL;
        }

        break;
    default:
        return FALSE;
    }

    return TRUE;
}

// Sets the raster's type and format from the create flags, choosing the depth (and, with no
// pixel format given, the format) the driver uses for it.
S32 fn_8017995C(RwRaster* raster, S32 flags)
{
    U32 format = flags & rwRASTERFORMATMASK;

    raster->cType = flags & rwRASTERTYPEMASK;
    raster->cFlags = flags & 0xF8;

    switch (raster->cType)
    {
    case rwRASTERTYPENORMAL:
    case rwRASTERTYPETEXTURE:
        if (!(format & rwRASTERFORMATPIXELFORMATMASK))
        {
            switch (raster->depth)
            {
            case 0:
                format |= rwRASTERFORMAT1555;

                if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
                {
                    raster->depth = (format & rwRASTERFORMATPAL4) ? 4 : 8;
                }
                else
                {
                    raster->depth = 16;
                }
                break;
            case 4:
                raster->depth = 4;
                format |= rwRASTERFORMATPAL4 | rwRASTERFORMAT1555;
                break;
            case 8:
                raster->depth = 8;
                format |= rwRASTERFORMATPAL8 | rwRASTERFORMAT1555;
                break;
            case 16:
                raster->depth = 16;
                format |= rwRASTERFORMAT1555;
                break;
            case 32:
                raster->depth = 32;
                format |= rwRASTERFORMAT8888;
                break;
            default:
                DLRASTER_ERROR((0x8000000C));
                break;
            }
        }
        else if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
        {
            raster->depth = (format & rwRASTERFORMATPAL4) ? 4 : 8;
        }
        else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888 ||
                 (format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888)
        {
            raster->depth = 32;
        }
        else
        {
            raster->depth = 16;
        }
        break;
    case rwRASTERTYPECAMERATEXTURE:
        if (!(format & rwRASTERFORMATPIXELFORMATMASK))
        {
            raster->depth = 16;
            format |= rwRASTERFORMAT565;
        }
        else if ((format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT888 ||
                 (format & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888)
        {
            raster->depth = 32;
        }
        else
        {
            raster->depth = 16;
        }
        break;
    case rwRASTERTYPEZBUFFER:
    case rwRASTERTYPECAMERA:
        if (!(format & rwRASTERFORMATPIXELFORMATMASK))
        {
            if (_RwDlRenderMode->aa)
            {
                raster->depth = 16;
                format = rwRASTERFORMAT565;
            }
            else
            {
                raster->depth = 32;
                format = rwRASTERFORMAT888;
            }
        }
        else if (format != rwRASTERFORMAT16 || !_RwDlRenderMode->aa ||
                 (format == rwRASTERFORMAT24 && !_RwDlRenderMode->aa))
        {
            DLRASTER_ERROR((0x8000000D));
            return FALSE;
        }
        break;
    default:
        DLRASTER_ERROR((0x8000000D));
        return FALSE;
    }

    raster->cFormat = format >> 8;

    return TRUE;
}

// The number of mipmap levels: the stored count, or every level down to 1x1 for a mipmapped
// raster.
S32 fn_80179BDC(void* out, void* inOut, S32 flags)
{
    RwRaster* raster = (RwRaster*)inOut;
    U8 maxLevel = RWGCRASTEREXT(raster)->maxLevel;

    if (maxLevel != 0xFF)
    {
        *(S32*)out = maxLevel + 1;
    }
    else
    {
        *(S32*)out = DLRASTER_NUMLEVELS(raster);
    }

    return TRUE;
}

// The size of all the raster's mipmap levels.
S32 fn_80179C7C(RwRaster* raster)
{
    S32 size = 0;
    S32 numLevels;

    fn_80179BDC(&numLevels, raster, 0);

    while (numLevels--)
    {
        size += fn_80179064(raster, numLevels);
    }

    return size;
}

// Allocates the raster's texels (and palette) for the given GX texel and palette formats and
// number of mipmap levels.
S32 fn_80179D54(RwRaster* raster, S32 format, S32 tlutFormat, S32 flags, U8 depth, S32 numLevels)
{
    RwGCRasterExt* ext;
    S32 size;

    raster->depth = depth;
    ext = RWGCRASTEREXT(raster);

    if (flags == RWGCRASTER_ALPHA)
    {
        ext->flags |= RWGCRASTER_ALPHA;
    }

    ext->maxLevel = numLevels - 1;
    ext->format = format;
    size = fn_80179C7C(raster);

    if (RwRasterGetFormat(raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        S32 paletteSize;

        ext->tlutFormat = tlutFormat;
        paletteSize = (1 << raster->depth) * 2;
        ext->unaligned = RwMalloc(size + paletteSize + 0x1F);

        if (!ext->unaligned)
        {
            DLRASTER_ERROR((0x80000013, size + paletteSize + 0x1F));
            return FALSE;
        }

        ext->pixels = (void*)(((U32)ext->unaligned + 0x1F) & ~0x1F);
        ext->palette = (U8*)ext->pixels + size;
        GXInitTlutObj(&ext->tlut, ext->palette, (GXTlutFmt)ext->tlutFormat,
                      (U16)(1 << raster->depth));
    }
    else
    {
        ext->unaligned = RwMalloc(size + 0x1F);

        if (!ext->unaligned)
        {
            DLRASTER_ERROR((0x80000013, size + 0x1F));
            return FALSE;
        }

        ext->pixels = (void*)(((U32)ext->unaligned + 0x1F) & ~0x1F);

        if (raster->cType == rwRASTERTYPECAMERATEXTURE)
        {
            DCInvalidateRange(ext->pixels, size);
        }
    }

    return TRUE;
}

// Picks the GX texel and palette formats for the raster's format and allocates its texels.
S32 fn_80179FA0(RwRaster* raster)
{
    U32 format = RwRasterGetFormat(raster);
    S32 texFormat = 0;
    S32 tlutFormat = 0;
    U32 alpha = 0;
    S32 numLevels;

    if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        switch (format & rwRASTERFORMATPIXELFORMATMASK)
        {
        case rwRASTERFORMAT555:
            tlutFormat = GX_TL_RGB5A3;
            alpha = 0;
            break;
        case rwRASTERFORMAT4444:
        case rwRASTERFORMAT1555:
            tlutFormat = GX_TL_RGB5A3;
            alpha = 1;
            break;
        case rwRASTERFORMAT565:
            tlutFormat = GX_TL_RGB565;
            alpha = 0;
            break;
        default:
            DLRASTER_ERROR((0x8000000D));
            break;
        }

        texFormat = GX_TF_C8 - ((format & rwRASTERFORMATPAL4) >> 14);
    }
    else
    {
        switch (format & rwRASTERFORMATPIXELFORMATMASK)
        {
        case rwRASTERFORMAT555:
            texFormat = GX_TF_RGB5A3;
            alpha = 0;
            break;
        case rwRASTERFORMAT4444:
        case rwRASTERFORMAT1555:
            texFormat = GX_TF_RGB5A3;
            alpha = 1;
            break;
        case rwRASTERFORMAT565:
            texFormat = GX_TF_RGB565;
            alpha = 0;
            break;
        case rwRASTERFORMAT888:
            texFormat = GX_TF_RGBA8;
            alpha = 0;
            break;
        case rwRASTERFORMAT8888:
            texFormat = GX_TF_RGBA8;
            alpha = 1;
            break;
        default:
            DLRASTER_ERROR((0x8000000D));
            break;
        }
    }

    numLevels = DLRASTER_NUMLEVELS(raster);

    fn_80179D54(raster, texFormat, tlutFormat, alpha, raster->depth, numLevels);

    return TRUE;
}

// Creates a raster: sets its format and, for a texture with a size, allocates its texels.
S32 fn_8017A374(void* out, RwRaster* raster, S32 flags)
{
    RwGCRasterExt* ext;

    raster->stride = 0;
    ext = RWGCRASTEREXT(raster);
    ext->format = 0xFF;
    ext->tlutFormat = 0xFF;
    ext->flags = 0;
    ext->unaligned = NULL;
    ext->pixels = NULL;
    ext->palette = NULL;
    ext->lockPixels = NULL;
    ext->lockBuffer = NULL;
    ext->region = NULL;
    ext->syncToken = lbl_8025F890;
    ext->maxLevel = 0xFF;
    ext->lockedLevel = 0xFF;

    if (!fn_8017995C(raster, flags))
    {
        return FALSE;
    }

    if (raster->width && raster->height)
    {
        switch (raster->cType)
        {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
            if (!(raster->cFlags & rwRASTERDONTALLOCATE) && !fn_80179FA0(raster))
            {
                DLRASTER_ERROR((2, lbl_801DE368));
                return FALSE;
            }
            break;
        case rwRASTERTYPEZBUFFER:
        case rwRASTERTYPECAMERA:
            raster->cFlags = rwRASTERDONTALLOCATE;
            break;
        default:
            DLRASTER_ERROR((0x8000000D));
            return FALSE;
        }
    }
    else
    {
        raster->cFlags = rwRASTERDONTALLOCATE;
        return TRUE;
    }

    return TRUE;
}

// Destroys a raster: once the GPU is done with it, frees its texels.
S32 fn_8017A4D8(void* out, RwRaster* raster, S32 flags)
{
    if (raster->parent == raster && !(raster->cFlags & rwRASTERDONTALLOCATE))
    {
        switch (raster->cType)
        {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE:
        {
            RwGCRasterExt* ext = RWGCRASTEREXT(raster);

            DLRASTER_WAITDONE(ext);

            if (raster == lbl_8025F87C->raster)
            {
                _rwDlTextureSetRaster(lbl_8025F87C, NULL, 0);
            }

            RwFree(ext->unaligned);
            break;
        }
        case rwRASTERTYPEZBUFFER:
        case rwRASTERTYPECAMERA:
            break;
        default:
            DLRASTER_ERROR((0x8000000D));
            break;
        }
    }

    return TRUE;
}

S32 _rwDlTextureSetRaster(RwTexture* texture, RwRaster* raster, S32 flags)
{
    texture->raster = raster;
    RWGCTEXTUREEXT(texture)->flags = RWGCTEXTURE_DEFAULTLOD;

    return TRUE;
}

S32 _rwDlRasterSubRaster(RwRaster* raster, RwRaster* parent, S32 flags)
{
    raster->stride = parent->stride;
    raster->depth = parent->depth;
    raster->cType = parent->cType;
    raster->cFormat = parent->cFormat;
    raster->cpPixels = NULL;

    return TRUE;
}
