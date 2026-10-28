#include <rwcore.h>

// The image flag that says the image owns its pixel and palette memory.
#define IMAGE_ALLOCATED 0x1

// The standard device function that converts a pixel value to a colour.
#define STDFUNC_RGBAFROMPIXEL 3

// The callback ImagePathForAllFullNames calls with each candidate file name.
typedef S32 (*ImagePathCallBack)(const char* fullName, void* data);

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
S32 ImageDetermineExtender(const char* fullName, void* data);

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
