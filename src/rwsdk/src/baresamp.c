#include <rwcore.h>

// The image flag that says the pixels have been gamma corrected.
#define IMAGE_GAMMACORRECTED 0x2

// Sample positions are 16.16 fixed point.
#define RESAMPLE_ONE 0x10000
#define RESAMPLE_TOREAL (1.0f / 65536.0f)
#define RESAMPLE_TOFIXED 65536.0f

// The scale from a colour channel to the range 0 to 1.
#define RESAMPLE_TOUNIT (1.0f / 255.0f)

// Averages the pixels of one row of a 32-bit image between two fixed-point x positions, each
// pixel weighted by how much of it the span covers.
static void ImageResampleGetSpan(const RwImage* image, S32 x0, S32 x1, S32 y, RwRGBAReal* result)
{
    S32 first;
    S32 last;
    F32 width;
    const RwRGBA* pixel;
    F32 scale;

    width = RESAMPLE_TOREAL * (F32)(x1 - x0);
    pixel = (const RwRGBA*)(image->cpPixels + (y >> 16) * image->stride + (x0 >> 16) * 4);
    last = x1 >> 16;
    first = x0 >> 16;

    if (first == last)
    {
        result->red = RESAMPLE_TOUNIT * pixel->red;
        result->green = RESAMPLE_TOUNIT * pixel->green;
        result->blue = RESAMPLE_TOUNIT * pixel->blue;
        result->alpha = RESAMPLE_TOUNIT * pixel->alpha;

        result->red *= width;
        result->green *= width;
        result->blue *= width;
        result->alpha *= width;
    }
    else
    {
        F32 alpha;
        F32 blue;
        F32 green;
        F32 red;
        S32 i;
        S32 x;

        x = (S32)(RESAMPLE_TOFIXED * (F32)(first + 1));

        result->red = RESAMPLE_TOUNIT * pixel->red;
        result->green = RESAMPLE_TOUNIT * pixel->green;
        result->blue = RESAMPLE_TOUNIT * pixel->blue;
        result->alpha = RESAMPLE_TOUNIT * pixel->alpha;

        result->red *= RESAMPLE_TOREAL * (F32)(x - x0);
        result->green *= RESAMPLE_TOREAL * (F32)(x - x0);
        result->blue *= RESAMPLE_TOREAL * (F32)(x - x0);
        result->alpha *= RESAMPLE_TOREAL * (F32)(x - x0);

        pixel++;

        for (i = x >> 16; i != last; i++)
        {
            green = RESAMPLE_TOUNIT * pixel->green;
            blue = RESAMPLE_TOUNIT * pixel->blue;
            alpha = RESAMPLE_TOUNIT * pixel->alpha;
            red = pixel->red;
            pixel++;
            x += RESAMPLE_ONE;
            result->red += RESAMPLE_TOUNIT * red;
            result->green += green;
            result->blue += blue;
            result->alpha += alpha;
        }

        red = RESAMPLE_TOUNIT * pixel->red;
        green = RESAMPLE_TOUNIT * pixel->green;
        blue = RESAMPLE_TOUNIT * pixel->blue;
        alpha = RESAMPLE_TOUNIT * pixel->alpha;
        scale = RESAMPLE_TOREAL * (F32)(x1 - x);
        red *= scale;
        green *= scale;
        blue *= scale;
        alpha *= scale;
        result->red += red;
        result->green += green;
        result->blue += blue;
        result->alpha += alpha;
    }

    scale = 1.0f / width;
    result->red *= scale;
    result->green *= scale;
    result->blue *= scale;
    result->alpha *= scale;
}

// Averages the pixels of a 32-bit image inside a fixed-point box, each row weighted by how much
// of it the box covers.
static void ImageResampleGetAvgPixel(const RwImage* image, S32 x0, S32 x1, S32 y0, S32 y1,
                                     RwRGBAReal* result)
{
    S32 i;
    S32 first = y0 >> 16;
    S32 last = y1 >> 16;
    F32 height = RESAMPLE_TOREAL * (F32)(y1 - y0);
    F32 scale;

    if (first == last)
    {
        ImageResampleGetSpan(image, x0, x1, y0, result);

        result->red *= height;
        result->green *= height;
        result->blue *= height;
        result->alpha *= height;
    }
    else
    {
        S32 y = (S32)(RESAMPLE_TOFIXED * (F32)(first + 1));
        F32 part;
        RwRGBAReal span;

        ImageResampleGetSpan(image, x0, x1, y0, result);

        part = RESAMPLE_TOREAL * (F32)(y - y0);
        result->red *= part;
        result->green *= part;
        result->blue *= part;
        result->alpha *= part;

        for (i = y >> 16; i != last; i++)
        {
            ImageResampleGetSpan(image, x0, x1, y, &span);
            y += RESAMPLE_ONE;
            result->red = span.red + result->red;
            result->green = span.green + result->green;
            result->blue = span.blue + result->blue;
            result->alpha = span.alpha + result->alpha;
        }

        ImageResampleGetSpan(image, x0, x1, y, &span);
        part = RESAMPLE_TOREAL * (F32)(y1 - y);
        span.red *= part;
        span.green *= part;
        span.blue *= part;
        span.alpha *= part;
        result->red += span.red;
        result->green += span.green;
        result->blue += span.blue;
        result->alpha += span.alpha;
    }

    scale = 1.0f / height;
    result->red *= scale;
    result->green *= scale;
    result->blue *= scale;
    result->alpha *= scale;
}

RwImage* RwImageResample(RwImage* dstImage, const RwImage* srcImage)
{
    S32 srcX;
    S32 lastY;
    S32 x;
    S32 y;
    S32 xStep;
    RwRGBA* pixel;
    S32 srcY;
    S32 yStep;
    S32 width;
    S32 height;
    S32 srcWidth;
    S32 srcHeight;
    RwRGBAReal colour;

    width = dstImage->width;
    height = dstImage->height;
    srcY = 0;

    srcWidth = srcImage->width;
    srcHeight = srcImage->height;

    dstImage->flags |= srcImage->flags & IMAGE_GAMMACORRECTED;

    xStep = (S32)(RESAMPLE_TOFIXED * ((F32)srcWidth / (F32)width));
    yStep = (S32)(RESAMPLE_TOFIXED * ((F32)srcHeight / (F32)height));

    for (y = 0; y < height; y++)
    {
        srcX = 0;
        lastY = srcY + yStep - 1;
        pixel = (RwRGBA*)(dstImage->cpPixels + dstImage->stride * y);

        for (x = 0; x < width; x++)
        {
            ImageResampleGetAvgPixel(srcImage, srcX, srcX + xStep - 1, srcY, lastY,
                                     &colour);
            srcX += xStep;

            pixel->red = (U8)(0.5f + 255.0f * colour.red);
            pixel->green = (U8)(0.5f + 255.0f * colour.green);
            pixel->blue = (U8)(0.5f + 255.0f * colour.blue);
            pixel->alpha = (U8)(0.5f + 255.0f * colour.alpha);
            pixel++;
        }

        srcY += yStep;
    }

    return dstImage;
}

// FAKEMATCH: ImageResampleRowP and ImageResampleP are a copy of RwImageResample used only by
// RwImageCreateResample, so that its two inlined copies number their registers like the target:
// the row loop sits in its own helper and its x and srcX counters are the caller's locals, reached
// through pointers. Real form believed to be RwImageCreateResample calling RwImageResample
// directly; no plain form found gives both the standalone function and the inlined copies.
// Fills one row of a 32-bit image with box-averaged source pixels.
static void ImageResampleRowP(RwRGBA* pixel, S32 lastY, const RwImage* srcImage, S32 width,
                              S32 xStep, S32 srcY, S32* x, S32* srcX)
{
    RwRGBAReal colour;

    *srcX = 0;

    for (*x = 0; *x < width; (*x)++)
    {
        ImageResampleGetAvgPixel(srcImage, *srcX, *srcX + xStep - 1, srcY, lastY,
                                 &colour);
        *srcX += xStep;

        pixel->red = (U8)(0.5f + 255.0f * colour.red);
        pixel->green = (U8)(0.5f + 255.0f * colour.green);
        pixel->blue = (U8)(0.5f + 255.0f * colour.blue);
        pixel->alpha = (U8)(0.5f + 255.0f * colour.alpha);
        pixel++;
    }
}

// FAKEMATCH copy of RwImageResample (see ImageResampleRowP).
static RwImage* ImageResampleP(RwImage* dstImage, const RwImage* srcImage)
{
    S32 x;
    S32 y;
    S32 srcX;
    S32 xStep;
    S32 srcY;
    S32 yStep;
    S32 width;
    S32 height;
    S32 srcWidth;
    S32 srcHeight;

    width = dstImage->width;
    height = dstImage->height;
    srcY = 0;

    srcWidth = srcImage->width;
    srcHeight = srcImage->height;

    dstImage->flags |= srcImage->flags & IMAGE_GAMMACORRECTED;

    xStep = (S32)(RESAMPLE_TOFIXED * ((F32)srcWidth / (F32)width));
    yStep = (S32)(RESAMPLE_TOFIXED * ((F32)srcHeight / (F32)height));

    for (y = 0; y < height; y++)
    {
        ImageResampleRowP((RwRGBA*)(dstImage->cpPixels + dstImage->stride * y),
                          srcY + yStep - 1, srcImage, width, xStep, srcY, &x, &srcX);
        srcY += yStep;
    }

    return dstImage;
}

RwImage* RwImageCreateResample(const RwImage* srcImage, S32 width, S32 height)
{
    RwImage* dstImage = RwImageCreate(width, height, 32);

    if (!dstImage)
    {
        return NULL;
    }

    if (!RwImageAllocatePixels(dstImage))
    {
        RwImageDestroy(dstImage);
        return NULL;
    }

    if (srcImage->depth != 32)
    {
        RwImage* tmpImage = RwImageCreate(srcImage->width, srcImage->height, 32);

        if (!tmpImage)
        {
            RwImageFreePixels(dstImage);
            RwImageDestroy(dstImage);
            return NULL;
        }

        if (!RwImageAllocatePixels(tmpImage))
        {
            RwImageDestroy(tmpImage);
            RwImageFreePixels(dstImage);
            RwImageDestroy(dstImage);
            return NULL;
        }

        RwImageCopy(tmpImage, srcImage);

        if (!ImageResampleP(dstImage, tmpImage))
        {
            RwImageFreePixels(tmpImage);
            RwImageDestroy(tmpImage);
            RwImageFreePixels(dstImage);
            RwImageDestroy(dstImage);
            return NULL;
        }

        RwImageFreePixels(tmpImage);
        RwImageDestroy(tmpImage);
    }
    else if (!ImageResampleP(dstImage, srcImage))
    {
        RwImageFreePixels(dstImage);
        RwImageDestroy(dstImage);
        return NULL;
    }

    return dstImage;
}
