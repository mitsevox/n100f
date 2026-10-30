#include <rwcore.h>
#include <string.h>

S32 _rwDlFindMSB(S32 num)
{
    S32 msb = -1;

    while (num)
    {
        num >>= 1;
        msb++;
    }

    return msb;
}

static U32 _rwDlConv8888To555(const RwRGBA* rgba)
{
    return 0x8000 | ((rgba->red << 7) & 0x7C00) | ((rgba->green << 2) & 0x3E0) | ((rgba->blue >> 3) & 0x1F);
}

static U32 _rwDlConv8888To565(const RwRGBA* rgba)
{
    return ((rgba->red << 8) & 0xF800) | ((rgba->green << 3) & 0x7E0) | ((rgba->blue >> 3) & 0x1F);
}

static U32 _rwDlConv8888To555or3444(const RwRGBA* rgba)
{
    if (rgba->alpha != 0xFF)
    {
        return ((rgba->alpha << 7) & 0x7000) | ((rgba->red << 4) & 0xF00) | (rgba->green & 0xF0) |
               ((rgba->blue >> 4) & 0xF);
    }

    return _rwDlConv8888To555(rgba);
}

static U32 _rwDlConv8888ToDl888(const RwRGBA* rgba)
{
    return 0xFF000000 | (rgba->red << 16) | (rgba->green << 8) | rgba->blue;
}

static U32 _rwDlConv8888ToDl8888(const RwRGBA* rgba)
{
    return (rgba->alpha << 24) | (rgba->red << 16) | (rgba->green << 8) | rgba->blue;
}

S32 _rwDlRGBToPixel(U32* pixelOut, const RwRGBA* rgba, S32 format)
{
    U32 pixel;

    switch (format & rwRASTERFORMATPIXELFORMATMASK)
    {
    case rwRASTERFORMATDEFAULT:
        pixel = _rwDlConv8888To565(rgba);
        break;
    case rwRASTERFORMATLUM8:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(2, "rwRASTERFORMATLUM8 not yet supported");
        RwErrorSet(&err);
        break;
    }
    case rwRASTERFORMAT1555:
    case rwRASTERFORMAT4444:
        pixel = _rwDlConv8888To555or3444(rgba);
        break;
    case rwRASTERFORMAT555:
        pixel = _rwDlConv8888To555(rgba);
        break;
    case rwRASTERFORMAT565:
        pixel = _rwDlConv8888To565(rgba);
        break;
    case rwRASTERFORMAT8888:
        pixel = _rwDlConv8888ToDl8888(rgba);
        break;
    case rwRASTERFORMAT888:
        pixel = _rwDlConv8888ToDl888(rgba);
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000000D);
        RwErrorSet(&err);
        break;
    }
    }

    *pixelOut = pixel;

    return TRUE;
}

static void _rwDlConv555To8888(RwRGBA* rgba, U32 pixel)
{
    rgba->red = ((pixel >> 10) & 0x1F) << 3;
    rgba->green = ((pixel >> 5) & 0x1F) << 3;
    rgba->blue = (pixel & 0x1F) << 3;
    rgba->alpha = 0xFF;
}

static void _rwDlConv565To8888(RwRGBA* rgba, U32 pixel)
{
    rgba->red = ((pixel >> 11) & 0x1F) << 3;
    rgba->green = ((pixel >> 5) & 0x3F) << 2;
    rgba->blue = (pixel & 0x1F) << 3;
    rgba->alpha = 0xFF;
}

static void _rwDlConv1555To8888(RwRGBA* rgba, U32 pixel)
{
    if (pixel & 0x8000)
    {
        _rwDlConv555To8888(rgba, pixel);
    }
    else
    {
        rgba->red = ((pixel >> 8) & 0xF) << 4;
        rgba->green = ((pixel >> 4) & 0xF) << 4;
        rgba->blue = (pixel & 0xF) << 4;
        rgba->alpha = 0;
    }
}

static void _rwDlConv4444To8888(RwRGBA* rgba, U32 pixel)
{
    if (pixel & 0x8000)
    {
        _rwDlConv555To8888(rgba, pixel);
    }
    else
    {
        rgba->red = ((pixel >> 8) & 0xF) << 4;
        rgba->green = ((pixel >> 4) & 0xF) << 4;
        rgba->blue = (pixel & 0xF) << 4;
        rgba->alpha = ((pixel >> 12) & 0x7) << 5;
    }
}

static void _rwDlConvDl888To8888(RwRGBA* rgba, U32 pixel)
{
    rgba->alpha = 0xFF;
    rgba->red = (U8)(pixel >> 16);
    rgba->green = (U8)(pixel >> 8);
    rgba->blue = (U8)pixel;
}

static void _rwDlConvDl8888To8888(RwRGBA* rgba, U32 pixel)
{
    rgba->alpha = (U8)(pixel >> 24);
    rgba->red = (U8)(pixel >> 16);
    rgba->green = (U8)(pixel >> 8);
    rgba->blue = (U8)pixel;
}

S32 _rwDlPixelToRGB(RwRGBA* rgba, const U32* pixelIn, S32 format)
{
    U32 pixel = *pixelIn;

    switch (format & rwRASTERFORMATPIXELFORMATMASK)
    {
    case rwRASTERFORMATDEFAULT:
        _rwDlConv565To8888(rgba, pixel);
        break;
    case rwRASTERFORMATLUM8:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(2, "rwRASTERFORMATLUM8 not yet supported");
        RwErrorSet(&err);
        break;
    }
    case rwRASTERFORMAT555:
        _rwDlConv555To8888(rgba, pixel);
        break;
    case rwRASTERFORMAT1555:
        _rwDlConv1555To8888(rgba, pixel);
        break;
    case rwRASTERFORMAT565:
        _rwDlConv565To8888(rgba, pixel);
        break;
    case rwRASTERFORMAT4444:
        _rwDlConv4444To8888(rgba, pixel);
        break;
    case rwRASTERFORMAT888:
        _rwDlConvDl888To8888(rgba, pixel);
        break;
    case rwRASTERFORMAT8888:
        _rwDlConvDl8888To8888(rgba, pixel);
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000000D);
        RwErrorSet(&err);
        break;
    }
    }

    return TRUE;
}

S32 _rwDlImage4GetFromRaster(RwImage* image, RwRaster* raster);
S32 _rwDlImage8GetFromRaster(RwImage* image, RwRaster* raster);
S32 _rwDlImage32GetFromRaster(RwImage* image, RwRaster* raster);
void _rwDlRasterPalletized4SetFromImage(RwRaster* raster, RwImage* image);
void _rwDlRasterPalletized8SetFromImage(RwRaster* raster, RwImage* image);

// Reads a raster into an image of the image's depth, locking the raster's pixels and palette for
// reading if they are not locked already.
S32 _rwDlImageGetFromRaster(RwImage* image, RwRaster* raster)
{
    S32 pixelsLocked = FALSE;
    S32 paletteLocked = FALSE;

    if (!(raster->privateFlags & rwRASTERPIXELLOCKEDREAD))
    {
        RwRasterLock(raster, 0, rwRASTERLOCKREAD);
        pixelsLocked = TRUE;
    }

    if ((RwRasterGetFormat(raster) & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8)) &&
        !(raster->privateFlags & rwRASTERPALETTELOCKEDREAD))
    {
        RwRasterLockPalette(raster, rwRASTERLOCKREAD);
        paletteLocked = TRUE;
    }

    switch (image->depth)
    {
    case 4:
        _rwDlImage4GetFromRaster(image, raster);
        break;
    case 8:
        _rwDlImage8GetFromRaster(image, raster);
        break;
    case 32:
        _rwDlImage32GetFromRaster(image, raster);
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000008);
        RwErrorSet(&err);
        break;
    }
    }

    if (paletteLocked == TRUE)
    {
        RwRasterUnlockPalette(raster);
    }

    if (pixelsLocked == TRUE)
    {
        RwRasterUnlock(raster);
    }

    return TRUE;
}

// Writes an image into a 16-bit raster, converting each palette entry or pixel to the raster's
// pixel format.
void _rwDlRaster16SetFromImage(RwRaster* raster, RwImage* image)
{
    U32 (*convert)(const RwRGBA*) = NULL;
    U16 palette[256];
    S32 i;
    S32 y;
    S32 x;

    switch (RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK)
    {
    case rwRASTERFORMATLUM8:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(2, "rwRASTERFORMATLUM8 not yet supported");
        RwErrorSet(&err);
        break;
    }
    case rwRASTERFORMAT555:
        convert = _rwDlConv8888To555;
        break;
    case rwRASTERFORMAT565:
        convert = _rwDlConv8888To565;
        break;
    case rwRASTERFORMAT1555:
    case rwRASTERFORMAT4444:
        convert = _rwDlConv8888To555or3444;
        break;
    case rwRASTERFORMAT888:
        convert = _rwDlConv8888ToDl888;
        break;
    case rwRASTERFORMAT8888:
        convert = _rwDlConv8888ToDl8888;
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000000D);
        RwErrorSet(&err);
        break;
    }
    }

    switch (image->depth)
    {
    case 4:
    case 8:
        for (i = 0; i < (1 << image->depth); i++)
        {
            palette[i] = convert(&image->palette[i]);
        }

        for (y = 0; y < raster->height; y++)
        {
            U8* src = image->cpPixels + image->stride * y;
            U16* dst = (U16*)(raster->cpPixels + raster->stride * y);

            for (x = 0; x < raster->width; x++)
            {
                *dst = palette[*src];
                src++;
                dst++;
            }
        }
        break;
    case 32:
        for (y = 0; y < raster->height; y++)
        {
            RwRGBA* src = (RwRGBA*)(image->cpPixels + image->stride * y);
            U16* dst = (U16*)(raster->cpPixels + raster->stride * y);

            for (x = 0; x < raster->width; x++)
            {
                *dst = convert(src);
                dst++;
                src++;
            }
        }
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000008);
        RwErrorSet(&err);
        break;
    }
    }
}

// Writes an image into a 32-bit raster, converting each palette entry or pixel to the raster's
// pixel format.
void _rwDlRaster32SetFromImage(RwRaster* raster, RwImage* image)
{
    U32 (*convert)(const RwRGBA*) = NULL;
    U32 palette[256];
    S32 i;
    S32 y;
    S32 x;

    switch (RwRasterGetFormat(raster) & rwRASTERFORMATPIXELFORMATMASK)
    {
    case rwRASTERFORMATLUM8:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(2, "rwRASTERFORMATLUM8 not yet supported");
        RwErrorSet(&err);
        break;
    }
    case rwRASTERFORMAT555:
        convert = _rwDlConv8888To555;
        break;
    case rwRASTERFORMAT565:
        convert = _rwDlConv8888To565;
        break;
    case rwRASTERFORMAT1555:
    case rwRASTERFORMAT4444:
        convert = _rwDlConv8888To555or3444;
        break;
    case rwRASTERFORMAT888:
        convert = _rwDlConv8888ToDl888;
        break;
    case rwRASTERFORMAT8888:
        convert = _rwDlConv8888ToDl8888;
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000000D);
        RwErrorSet(&err);
        break;
    }
    }

    switch (image->depth)
    {
    case 4:
    case 8:
        for (i = 0; i < (1 << image->depth); i++)
        {
            palette[i] = convert(&image->palette[i]);
        }

        for (y = 0; y < raster->height; y++)
        {
            U8* src = image->cpPixels + image->stride * y;
            U32* dst = (U32*)(raster->cpPixels + raster->stride * y);

            for (x = 0; x < raster->width; x++)
            {
                *dst = palette[*src];
                src++;
                dst++;
            }
        }
        break;
    case 32:
        for (y = 0; y < raster->height; y++)
        {
            RwRGBA* src = (RwRGBA*)(image->cpPixels + image->stride * y);
            U32* dst = (U32*)(raster->cpPixels + raster->stride * y);

            for (x = 0; x < raster->width; x++)
            {
                *dst = convert(src);
                dst++;
                src++;
            }
        }
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000008);
        RwErrorSet(&err);
        break;
    }
    }
}

// Writes an image into a raster of the raster's depth, locking the raster's pixels and palette for
// writing if they are not locked already.
S32 _rwDlRasterSetFromImage(RwRaster* raster, RwImage* image)
{
    S32 paletteLocked = FALSE;
    S32 pixelsLocked = FALSE;
    S32 format = RwRasterGetFormat(raster);

    if (raster->privateFlags & rwRASTERPIXELLOCKEDWRITE)
    {
        pixelsLocked = TRUE;
    }

    if (!pixelsLocked && !RwRasterLock(raster, 0, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH))
    {
        return FALSE;
    }

    if (format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8))
    {
        if (raster->privateFlags & rwRASTERPALETTELOCKEDWRITE)
        {
            paletteLocked = TRUE;
        }

        if (!paletteLocked &&
            !RwRasterLockPalette(raster, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH))
        {
            return FALSE;
        }
    }

    switch (raster->depth)
    {
    case 4:
        _rwDlRasterPalletized4SetFromImage(raster, image);
        break;
    case 8:
        _rwDlRasterPalletized8SetFromImage(raster, image);
        break;
    case 16:
        _rwDlRaster16SetFromImage(raster, image);
        break;
    case 24:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000000C);
        RwErrorSet(&err);
        break;
    }
    case 32:
        _rwDlRaster32SetFromImage(raster, image);
        break;
    default:
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x8000000C);
        RwErrorSet(&err);
        break;
    }
    }

    if ((format & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8)) && !paletteLocked)
    {
        RwRasterUnlockPalette(raster);
    }

    if (!pixelsLocked)
    {
        RwRasterUnlock(raster);
    }

    return TRUE;
}

// Chooses the raster format an image converts to: a palette format for 4- and 8-bit images,
// and 1555, 4444 or 565 by the alpha the image's pixels (or palette entries) use.
S32 _rwDlImageFindFormat(RwImage* image)
{
    S32 depth = image->depth;
    S32 hasAlpha = FALSE;
    S32 format;
    S32 x;
    S32 y;

    if (depth == 4 || depth == 8)
    {
        S32 width = image->width;
        S32 height = image->height;
        U8* row = image->cpPixels;
        RwRGBA* palette = image->palette;

        for (y = 0; y < height; y++)
        {
            U8* pixel = row;

            for (x = 0; x < width; x++)
            {
                U8 alpha = palette[*pixel].alpha;

                if (alpha != 0xFF)
                {
                    hasAlpha = TRUE;

                    if (alpha > 0xF)
                    {
                        format = rwRASTERFORMAT4444;

                        if (depth == 4)
                        {
                            return format | rwRASTERFORMATPAL4;
                        }

                        return format | rwRASTERFORMATPAL8;
                    }
                }

                pixel++;
            }

            row += image->stride;
        }
    }
    else
    {
        S32 width = image->width;
        S32 height = image->height;
        U8* row = image->cpPixels;

        for (y = 0; y < height; y++)
        {
            RwRGBA* pixel = (RwRGBA*)row;

            for (x = 0; x < width; x++)
            {
                U8 alpha = pixel->alpha;

                if (alpha != 0xFF)
                {
                    hasAlpha = TRUE;

                    if (alpha > 0xF)
                    {
                        return rwRASTERFORMAT4444;
                    }
                }

                pixel++;
            }

            row += image->stride;
        }
    }

    format = rwRASTERFORMAT565;

    if (hasAlpha)
    {
        format = rwRASTERFORMAT1555;
    }

    if (depth == 4)
    {
        return format | rwRASTERFORMATPAL4;
    }

    if (depth == 8)
    {
        return format | rwRASTERFORMATPAL8;
    }

    return format;
}
