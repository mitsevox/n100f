#include <rwcore.h>
#include <string.h>

// The image flag that says the image owns its pixel and palette memory.
#define IMAGE_ALLOCATED 0x1

// The standard device function that converts a pixel value to a colour.
#define STDFUNC_RGBAFROMPIXEL 3

// The image flag that says the pixels have been gamma corrected.
#define IMAGE_GAMMACORRECTED 0x2

// The callback ImagePathForAllFullNames calls with each candidate file name. It returns the name
// to go on searching, or NULL to stop.
typedef const char* (*ImagePathCallBack)(const char* fullName, void* data);

// The reader ImageAttempRead tries on an existing file, and the image it read.
typedef RwImage* (*ImageReadFunc)(const char* fullName);

typedef struct ImageReadData ImageReadData;
struct ImageReadData
{
    ImageReadFunc readFunc;
    RwImage* image;
};

// The image module's data in the engine instance. Partial: only the free list is declared.
typedef struct ImageModuleData ImageModuleData;
struct ImageModuleData
{
    RwFreeList* freeList;
};

#define IMAGE_DATA(var) \
    (((ImageModuleData*)((U8*)RwEngineInstance + imageModule.globalsOffset))->var)

static RwPluginRegistry imageTKList = { sizeof(RwImage), sizeof(RwImage) };
static RwModuleInfo imageModule;

void ImagePathForAllFullNames(const char* imageName, S32 maxLength, ImagePathCallBack callBack,
                              void* data);
const char* ImageDetermineExtender(const char* fullName, void* data);
RwImage* ImageConvertDepth(RwImage* destImage, const RwImage* sourceImage);

RwImage* RwImageCreate(S32 width, S32 height, S32 depth)
{
    RwImage* image = (RwImage*)RWSRCGLOBAL(memoryAlloc)(IMAGE_DATA(freeList));

    if (!image)
    {
        return NULL;
    }

    image->width = width;
    image->height = height;
    image->depth = depth;
    image->cpPixels = NULL;
    image->palette = NULL;
    image->flags = 0;

    _rwPluginRegistryInitObject(&imageTKList, image);

    return image;
}

S32 RwImageDestroy(RwImage* image)
{
    if (image->flags & IMAGE_ALLOCATED)
    {
        RwImageFreePixels(image);
    }

    _rwPluginRegistryDeInitObject(&imageTKList, image);
    RWSRCGLOBAL(memoryFree)(IMAGE_DATA(freeList), image);

    return TRUE;
}

RwImage* RwImageAllocatePixels(RwImage* image)
{
    S32 palettized = FALSE;
    S32 paletteSize;
    S32 pixelSize;
    S32 size;

    if (image->depth == 4 || image->depth == 8)
    {
        palettized = TRUE;
    }

    image->stride = (image->depth + 7) >> 3;
    paletteSize = palettized ? (1 << image->depth) * sizeof(RwRGBA) : 0;
    image->stride *= image->width;
    image->stride = (image->stride + 3) & ~3;

    pixelSize = image->stride * image->height;
    size = pixelSize + paletteSize;

    image->cpPixels = (U8*)RwMalloc(size);

    if (!image->cpPixels)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
        return NULL;
    }

    image->palette = palettized ? (RwRGBA*)(image->cpPixels + pixelSize) : NULL;
    image->flags |= IMAGE_ALLOCATED;

    return image;
}

RwImage* RwImageFreePixels(RwImage* image)
{
    RWSRCGLOBAL(memoryFuncs).rwfree(image->cpPixels);

    image->cpPixels = NULL;
    image->palette = NULL;
    image->flags &= ~IMAGE_ALLOCATED;

    return image;
}

RwImage* RwImageMakeMask(RwImage* image)
{
    switch (image->depth)
    {
    case 4:
    case 8:
    {
        RwRGBA* palette = image->palette;
        S32 numColours = 1 << image->depth;
        S32 i;

        for (i = 0; i < numColours; i++)
        {
            S32 alpha = palette->red;

            if (palette->green > alpha)
            {
                alpha = palette->green;
            }

            if (palette->blue > alpha)
            {
                alpha = palette->blue;
            }

            palette->alpha = alpha;
            palette++;
        }

        break;
    }
    case 32:
    {
        U8* row = image->cpPixels;
        S32 y;

        for (y = 0; y < image->height; y++)
        {
            RwRGBA* pixel = (RwRGBA*)row;
            S32 x;

            for (x = 0; x < image->width; x++)
            {
                S32 alpha = pixel->red;

                if (pixel->green > alpha)
                {
                    alpha = pixel->green;
                }

                if (pixel->blue > alpha)
                {
                    alpha = pixel->blue;
                }

                pixel->alpha = alpha;
                pixel++;
            }

            row += image->stride;
        }

        break;
    }
    }

    return image;
}

const char* ImageAttempRead(const char* fullName, void* data)
{
    ImageReadData* readData = (ImageReadData*)data;

    if (RWSRCGLOBAL(fileFuncs).rwfexist(fullName))
    {
        readData->image = readData->readFunc(fullName);

        if (readData->image)
        {
            return NULL;
        }
    }

    return fullName;
}

const char* RwImageFindFileType(const char* imageName)
{
    const char* extender = NULL;

    ImagePathForAllFullNames(imageName, 20, ImageDetermineExtender, &extender);

    return extender;
}

RwRGBA* RwRGBASetFromPixel(RwRGBA* rgbOut, U32 pixelValue, S32 rasterFormat)
{
    RWSRCGLOBAL(stdFunc)[STDFUNC_RGBAFROMPIXEL](rgbOut, &pixelValue, rasterFormat);

    return rgbOut;
}

RwImage* RwImageCopy(RwImage* destImage, const RwImage* sourceImage)
{
    if (destImage->depth == sourceImage->depth)
    {
        S32 y;
        const U8* srcPixels;
        U8* dstPixels;
        S32 rowBytes;

        if (destImage->palette && sourceImage->palette && sourceImage->depth <= 8)
        {
            memcpy(destImage->palette, sourceImage->palette,
                   (1 << sourceImage->depth) * sizeof(RwRGBA));
        }

        rowBytes = ((destImage->depth + 7) >> 3) * destImage->width;
        srcPixels = sourceImage->cpPixels;
        dstPixels = destImage->cpPixels;

        for (y = 0; y < destImage->height; y++)
        {
            memcpy(dstPixels, srcPixels, rowBytes);
            dstPixels += destImage->stride;
            srcPixels += sourceImage->stride;
        }
    }
    else if (!ImageConvertDepth(destImage, sourceImage))
    {
        destImage = NULL;
    }

    destImage->flags &= ~IMAGE_GAMMACORRECTED;
    destImage->flags |= sourceImage->flags & IMAGE_GAMMACORRECTED;

    return destImage;
}
