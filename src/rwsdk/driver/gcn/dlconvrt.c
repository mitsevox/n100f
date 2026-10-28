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
