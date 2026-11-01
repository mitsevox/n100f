#include <rwcore.h>
#include <math.h>
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

// A registered image file format: its file extender in lower and upper case, and its reader and
// writer (RwImageRegisterImageFormat, RwImageRead and RwImageWrite are in the PS2 prototype's
// symbols; this game only reads).
typedef struct ImageFileFormat ImageFileFormat;
struct ImageFileFormat
{
    char lowerExtender[20];
    char upperExtender[20];
    ImageReadFunc readFunc;
    void* writeFunc;
    ImageFileFormat* next;
};

// The image module's data in the engine instance: the image free list, the search path for image
// files, the gamma tables, a scratch buffer for building file names, and the registered formats.
typedef struct ImageModuleData ImageModuleData;
struct ImageModuleData
{
    RwFreeList* freeList;
    char* imagePath;
    S32 imagePathSize;
    U8 gammaTable[256];
    U8 invGammaTable[256];
    F32 gammaValue;
    char* scratchMem;
    S32 scratchMemSize;
    RwFreeList* formatFreeList;
    ImageFileFormat* formats;
};

#define IMAGE_DATA(var) \
    (((ImageModuleData*)((U8*)RwEngineInstance + imageModule.globalsOffset))->var)

// The default sizes of the image path and the scratch buffer.
#define IMAGE_PATHSIZE 256
#define IMAGE_SCRATCHSIZE 256

static RwPluginRegistry imageTKList = { sizeof(RwImage), sizeof(RwImage) };
static RwModuleInfo imageModule;

const char* ImagePathForAllFullNames(const char* imageName, S32 maxLength,
                                     ImagePathCallBack callBack, void* data);
const char* ImageDetermineExtender(const char* fullName, void* data);
S32 ImageConvertDepth(RwImage* destImage, const RwImage* sourceImage);
S32 RwImageSetGamma(F32 gammaValue);
S32 _rwpathisabsolute(const char* path);

// Raises an engine error for the image module.
static void ImageError(S32 code)
{
    RwError err;

    err.pluginID = 1;
    err.errorCode = _rwerror(code);
    RwErrorSet(&err);
}

void* _rwImageOpen(void* instance, S32 offset, S32 size)
{
    imageModule.globalsOffset = offset;

    IMAGE_DATA(freeList) = RwFreeListCreate(imageTKList.sizeOfStruct, 20, 0);

    if (!IMAGE_DATA(freeList))
    {
        return NULL;
    }

    IMAGE_DATA(formatFreeList) = RwFreeListCreate(sizeof(ImageFileFormat), 3, 0);

    if (!IMAGE_DATA(formatFreeList))
    {
        RwFreeListDestroy(IMAGE_DATA(freeList));
        IMAGE_DATA(freeList) = NULL;
        return NULL;
    }

    IMAGE_DATA(imagePathSize) = IMAGE_PATHSIZE;
    IMAGE_DATA(imagePath) = (char*)RwMalloc(IMAGE_DATA(imagePathSize));

    if (!IMAGE_DATA(imagePath))
    {
        RwFreeListDestroy(IMAGE_DATA(formatFreeList));
        IMAGE_DATA(formatFreeList) = NULL;
        RwFreeListDestroy(IMAGE_DATA(freeList));
        IMAGE_DATA(freeList) = NULL;
        return NULL;
    }

    IMAGE_DATA(imagePath)[0] = '\0';

    imageModule.numInstances++;

    RwImageSetGamma(1.0f);

    IMAGE_DATA(formats) = NULL;

    IMAGE_DATA(scratchMemSize) = IMAGE_SCRATCHSIZE;
    IMAGE_DATA(scratchMem) = (char*)RwMalloc(IMAGE_DATA(scratchMemSize));

    if (!IMAGE_DATA(scratchMem))
    {
        RwFree(IMAGE_DATA(imagePath));
        IMAGE_DATA(imagePath) = NULL;
        IMAGE_DATA(imagePathSize) = 0;
        RwFreeListDestroy(IMAGE_DATA(formatFreeList));
        IMAGE_DATA(formatFreeList) = NULL;
        RwFreeListDestroy(IMAGE_DATA(freeList));
        IMAGE_DATA(freeList) = NULL;
        return NULL;
    }

    return instance;
}

void* _rwImageClose(void* instance, S32 offset, S32 size)
{
    if (IMAGE_DATA(scratchMem))
    {
        RwFree(IMAGE_DATA(scratchMem));
        IMAGE_DATA(scratchMem) = NULL;
        IMAGE_DATA(scratchMemSize) = 0;
    }

    if (IMAGE_DATA(imagePath))
    {
        RwFree(IMAGE_DATA(imagePath));
        IMAGE_DATA(imagePath) = NULL;
        IMAGE_DATA(imagePathSize) = 0;
    }

    while (IMAGE_DATA(formats))
    {
        ImageFileFormat* format = IMAGE_DATA(formats);

        IMAGE_DATA(formats) = format->next;
        RWSRCGLOBAL(memoryFree)(IMAGE_DATA(formatFreeList), format);
    }

    if (IMAGE_DATA(formatFreeList))
    {
        RwFreeListDestroy(IMAGE_DATA(formatFreeList));
        IMAGE_DATA(formatFreeList) = NULL;
    }

    if (IMAGE_DATA(freeList))
    {
        RwFreeListDestroy(IMAGE_DATA(freeList));
        IMAGE_DATA(freeList) = NULL;
    }

    imageModule.numInstances--;

    return instance;
}

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

// Copies the mask's alpha into the image, which must be the same size. A palettized image is
// first converted to 32 bits through a temporary copy; the mask can be palettized or 32-bit.
RwImage* RwImageApplyMask(RwImage* image, const RwImage* mask)
{
    S32 width = image->width;
    S32 height = image->height;

    if (width != mask->width || height != mask->height)
    {
        ImageError(0x8000000A);
        return NULL;
    }

    switch (image->depth)
    {
    case 4:
    case 8:
    {
        RwImage* tempImage = RwImageCreate(width, height, image->depth);

        if (!tempImage)
        {
            return NULL;
        }

        if (!RwImageAllocatePixels(tempImage))
        {
            RwImageDestroy(tempImage);
            return NULL;
        }

        RwImageCopy(tempImage, image);

        if (image->flags & IMAGE_ALLOCATED)
        {
            RwImageFreePixels(image);
        }

        image->depth = 32;
        RwImageAllocatePixels(image);
        RwImageCopy(image, tempImage);

        RwImageFreePixels(tempImage);
        RwImageDestroy(tempImage);
    }
        // Fall through to apply the mask to the converted image.
    case 32:
    {
        const U8* maskRow = mask->cpPixels;
        U8* imageRow = image->cpPixels;
        S32 y;

        for (y = 0; y < image->height; y++)
        {
            RwRGBA* pixel = (RwRGBA*)imageRow;

            switch (mask->depth)
            {
            case 4:
            case 8:
            {
                const U8* maskPixel = maskRow;
                S32 x;

                for (x = 0; x < image->width; x++)
                {
                    pixel->alpha = mask->palette[*maskPixel++].alpha;
                    pixel++;
                }

                break;
            }
            case 32:
            {
                const RwRGBA* maskPixel = (const RwRGBA*)maskRow;
                S32 x;

                for (x = 0; x < image->width; x++)
                {
                    pixel->alpha = maskPixel->alpha;
                    maskPixel++;
                    pixel++;
                }

                break;
            }
            }

            imageRow += image->stride;
            maskRow += mask->stride;
        }

        return image;
    }
    default:
        ImageError(0x80000009);
        return NULL;
    }
}

// Returns the scratch buffer grown to at least size bytes, or NULL when it cannot grow.
static char* ImageGetScratch(S32 size)
{
    if (size > IMAGE_DATA(scratchMemSize))
    {
        char* mem;

        if (IMAGE_DATA(scratchMem))
        {
            mem = (char*)RwRealloc(IMAGE_DATA(scratchMem), size);
        }
        else
        {
            mem = (char*)RwMalloc(size);
        }

        if (!mem)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);
            return NULL;
        }

        IMAGE_DATA(scratchMem) = mem;
        IMAGE_DATA(scratchMemSize) = size;
    }

    return IMAGE_DATA(scratchMem);
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

// Tries each registered format's lower- and upper-case extender on the end of the name; when such
// a file exists, stores the extender through data and stops the search.
const char* ImageDetermineExtender(const char* fullName, void* data)
{
    char* end = (char*)fullName + rwstrlen(fullName);
    ImageFileFormat* format = IMAGE_DATA(formats);

    while (format)
    {
        rwstrcpy(end, format->lowerExtender);

        if (RWSRCGLOBAL(fileFuncs).rwfexist(fullName))
        {
            *(const char**)data = format->lowerExtender;
            return NULL;
        }

        rwstrcpy(end, format->upperExtender);

        if (RWSRCGLOBAL(fileFuncs).rwfexist(fullName))
        {
            *(const char**)data = format->upperExtender;
            return NULL;
        }

        format = format->next;
    }

    return fullName;
}

const char* RwImageFindFileType(const char* imageName)
{
    const char* extender = NULL;

    ImagePathForAllFullNames(imageName, 20, ImageDetermineExtender, &extender);

    return extender;
}

// Reads an image with the format registered for its file extender, searching the image path. The
// target inlines it into RwImageReadMaskedImage and has no copy of its own.
RwImage* RwImageRead(const char* imageName)
{
    const char* name;
    const char* separator;
    const char* extender;
    ImageFileFormat* format;

    separator = rwstrrchr(imageName, ':');
    name = separator ? separator : imageName;

    separator = rwstrrchr(name, '/');

    if (separator)
    {
        name = separator;
    }

    separator = rwstrrchr(name, '\\');

    if (separator)
    {
        name = separator;
    }

    extender = rwstrrchr(name, '.');

    if (!extender)
    {
        return NULL;
    }

    for (format = IMAGE_DATA(formats); format; format = format->next)
    {
        if (!rwstrcmp(format->lowerExtender, extender) || !rwstrcmp(format->upperExtender, extender))
        {
            ImageReadData readData;

            if (!format->readFunc)
            {
                return NULL;
            }

            readData.readFunc = format->readFunc;
            readData.image = NULL;
            ImagePathForAllFullNames(imageName, 5, ImageAttempRead, &readData);

            return readData.image;
        }
    }

    return NULL;
}

// Reads an image and, when a mask name is given, a mask image, turns the mask's brightness into
// alpha and applies it to the image.
RwImage* RwImageReadMaskedImage(const char* imageName, const char* maskName)
{
    RwImage* image = RwImageRead(imageName);

    if (!image)
    {
        return NULL;
    }

    if (maskName && *maskName)
    {
        RwImage* mask = RwImageRead(maskName);

        if (!mask)
        {
            RwImageDestroy(image);
            return NULL;
        }

        if (!RwImageMakeMask(mask))
        {
            RwImageDestroy(image);
            RwImageDestroy(mask);
            return NULL;
        }

        if (!RwImageApplyMask(image, mask))
        {
            RwImageDestroy(image);
            RwImageDestroy(mask);
            return NULL;
        }

        RwImageDestroy(mask);
    }

    return image;
}

RwRGBA* RwRGBASetFromPixel(RwRGBA* rgbOut, U32 pixelValue, S32 rasterFormat)
{
    RWSRCGLOBAL(stdFunc)[STDFUNC_RGBAFROMPIXEL](rgbOut, &pixelValue, rasterFormat);

    return rgbOut;
}

// Copies pixels between images of different depths. Equal depths pass without copying, 4-bit
// indices widen to 8-bit ones byte for byte, and palettized pixels expand to 32 bits through the
// palette; anything else is an error.
S32 ImageConvertDepth(RwImage* destImage, const RwImage* sourceImage)
{
    S32 result = FALSE;
    S32 width = destImage->width;
    S32 height = destImage->height;
    const RwRGBA* palette = sourceImage->palette;
    const U8* srcRow = sourceImage->cpPixels;
    U8* dstRow = destImage->cpPixels;
    S32 x;
    S32 y;

    switch ((sourceImage->depth << 8) | destImage->depth)
    {
    case (4 << 8) | 4:
    case (8 << 8) | 8:
    case (32 << 8) | 32:
        result = TRUE;
        break;
    case (4 << 8) | 8:
        for (y = 0; y < height; y++)
        {
            memcpy(dstRow, srcRow, width);
            srcRow += sourceImage->stride;
            dstRow += destImage->stride;
        }

        result = TRUE;
        break;
    case (4 << 8) | 32:
    case (8 << 8) | 32:
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                ((RwRGBA*)dstRow)[x] = palette[srcRow[x]];
            }

            srcRow += sourceImage->stride;
            dstRow += destImage->stride;
        }

        result = TRUE;
        break;
    default:
        ImageError(0x80000009);
        break;
    }

    return result;
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

// Runs numPixels colors through the gamma table, keeping their alpha.
static void ImageGammaCorrectPixels(RwRGBA* out, const RwRGBA* in, S32 numPixels)
{
    const U8* table = IMAGE_DATA(gammaTable);

    while (numPixels--)
    {
        out->red = table[in->red];
        out->green = table[in->green];
        out->blue = table[in->blue];
        out->alpha = in->alpha;
        in++;
        out++;
    }
}

// Gamma corrects a palettized image's palette or a 32-bit image's pixels in place and marks the
// image corrected.
RwImage* RwImageGammaCorrect(RwImage* image)
{
    switch (image->depth)
    {
    case 4:
    case 8:
    {
        RwRGBA* palette = image->palette;
        S32 numColours = 1 << image->depth;

        if (!palette)
        {
            ImageError(0x80000016);
            return NULL;
        }

        ImageGammaCorrectPixels(palette, palette, numColours);
        break;
    }
    case 32:
    {
        U8* row = image->cpPixels;
        S32 width = image->width;
        S32 y;

        if (!row)
        {
            ImageError(0x80000016);
            return NULL;
        }

        for (y = 0; y < image->height; y++)
        {
            ImageGammaCorrectPixels((RwRGBA*)row, (const RwRGBA*)row, width);
            row += image->stride;
        }

        break;
    }
    default:
        ImageError(0x80000008);
        return NULL;
    }

    image->flags |= IMAGE_GAMMACORRECTED;

    return image;
}

// Sets the gamma value and rebuilds the gamma table (raising to 1 / gamma) and its inverse.
S32 RwImageSetGamma(F32 gammaValue)
{
    F32 invGamma = 1.0f / gammaValue;
    S32 i;

    IMAGE_DATA(gammaValue) = gammaValue;
    IMAGE_DATA(gammaTable)[0] = 0;
    IMAGE_DATA(invGammaTable)[0] = 0;

    for (i = 1; i < 256; i++)
    {
        F32 value = (F32)i / 255.0f;

        IMAGE_DATA(gammaTable)[i] = (U8)(0.5f + 255.0f * (F32)pow(value, invGamma));
        IMAGE_DATA(invGammaTable)[i] = (U8)(0.5f + 255.0f * (F32)pow(value, gammaValue));
    }

    return TRUE;
}
// Calls callBack with the image name as it is when it is absolute or there is no image path, or
// else with the name behind each entry of the semicolon-separated path until callBack returns
// NULL. maxLength is the room to leave after the name. Returns NULL when the scratch buffer cannot
// grow, or else the image name.
//
// The target places this function before ImageAttempRead, but defined there the compiler inlines
// it into RwImageFindFileType and RwImageRead, which the target does not. It stays at the end of
// the file until the cause is found; the unit is not linked, so only its order in the object
// differs.
const char* ImagePathForAllFullNames(const char* imageName, S32 maxLength,
                                     ImagePathCallBack callBack, void* data)
{
    const char* path = IMAGE_DATA(imagePath);

    if (_rwpathisabsolute(imageName) || !path || !*path)
    {
        char* fullName = ImageGetScratch(rwstrlen(imageName) + maxLength);

        if (!fullName)
        {
            return NULL;
        }

        rwstrcpy(fullName, imageName);
        callBack(fullName, data);
        return imageName;
    }

    while (path && *path)
    {
        const char* next;
        S32 pathLength;
        char* fullName;

        // The target makes this call twice and keeps only the second result.
        rwstrchr(path, ';');
        next = rwstrchr(path, ';');

        if (next)
        {
            pathLength = next - path;
            next++;
        }
        else
        {
            pathLength = rwstrlen(path);
        }

        fullName = ImageGetScratch(pathLength + (rwstrlen(imageName) + maxLength));

        if (!fullName)
        {
            return NULL;
        }

        memcpy(fullName, path, pathLength);
        rwstrcpy(fullName + pathLength, imageName);

        if (!callBack(fullName, data))
        {
            return imageName;
        }

        path = next;
    }

    return imageName;
}

