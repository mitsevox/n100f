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
    S32 first = x0 >> 16;
    S32 last = x1 >> 16;
    F32 width = RESAMPLE_TOREAL * (F32)(x1 - x0);
    const RwRGBA* pixel = (const RwRGBA*)(image->cpPixels + (y >> 16) * image->stride + first * 4);
    F32 scale;

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
        S32 x = (S32)(RESAMPLE_TOFIXED * (F32)(first + 1));
        S32 i;
        F32 part;

        result->red = RESAMPLE_TOUNIT * pixel->red;
        result->green = RESAMPLE_TOUNIT * pixel->green;
        result->blue = RESAMPLE_TOUNIT * pixel->blue;
        result->alpha = RESAMPLE_TOUNIT * pixel->alpha;

        part = RESAMPLE_TOREAL * (F32)(x - x0);
        result->red *= part;
        result->green *= part;
        result->blue *= part;
        result->alpha *= part;

        pixel++;

        for (i = x >> 16; i != last; i++)
        {
            result->red += RESAMPLE_TOUNIT * pixel->red;
            result->green += RESAMPLE_TOUNIT * pixel->green;
            result->blue += RESAMPLE_TOUNIT * pixel->blue;
            result->alpha += RESAMPLE_TOUNIT * pixel->alpha;
            pixel++;
            x += RESAMPLE_ONE;
        }

        part = RESAMPLE_TOREAL * (F32)(x1 - x);
        result->red += RESAMPLE_TOUNIT * pixel->red * part;
        result->green += RESAMPLE_TOUNIT * pixel->green * part;
        result->blue += RESAMPLE_TOUNIT * pixel->blue * part;
        result->alpha += RESAMPLE_TOUNIT * pixel->alpha * part;
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
        S32 i;
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
            result->red += span.red;
            result->green += span.green;
            result->blue += span.blue;
            result->alpha += span.alpha;
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
    S32 width = dstImage->width;
    S32 height = dstImage->height;
    S32 srcY = 0;
    S32 xStep;
    S32 yStep;
    S32 y;

    dstImage->flags |= srcImage->flags & IMAGE_GAMMACORRECTED;

    xStep = (S32)(RESAMPLE_TOFIXED * ((F32)srcImage->width / (F32)width));
    yStep = (S32)(RESAMPLE_TOFIXED * ((F32)srcImage->height / (F32)height));

    for (y = 0; y < height; y++)
    {
        S32 srcX = 0;
        S32 x;
        RwRGBA* pixel = (RwRGBA*)(dstImage->cpPixels + dstImage->stride * y);

        for (x = 0; x < width; x++)
        {
            RwRGBAReal colour;

            ImageResampleGetAvgPixel(srcImage, srcX, srcX + xStep - 1, srcY, srcY + yStep - 1,
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

        if (!RwImageResample(dstImage, tmpImage))
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
    else if (!RwImageResample(dstImage, srcImage))
    {
        RwImageFreePixels(dstImage);
        RwImageDestroy(dstImage);
        return NULL;
    }

    return dstImage;
}
