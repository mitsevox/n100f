#include <rwcore.h>

// The image flag that says the image owns its pixel and palette memory.
#define IMAGE_ALLOCATED 0x1

// The standard device function that converts a pixel value to a colour.
#define STDFUNC_RGBAFROMPIXEL 3

// The callback ImagePathForAllFullNames calls with each candidate file name.
typedef S32 (*ImagePathCallBack)(const char* fullName, void* data);

void ImagePathForAllFullNames(const char* imageName, S32 maxLength, ImagePathCallBack callBack,
                              void* data);
S32 ImageDetermineExtender(const char* fullName, void* data);

RwImage* RwImageFreePixels(RwImage* image)
{
    RWSRCGLOBAL(memoryFuncs).rwfree(image->cpPixels);

    image->cpPixels = NULL;
    image->palette = NULL;
    image->flags &= ~IMAGE_ALLOCATED;

    return image;
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
