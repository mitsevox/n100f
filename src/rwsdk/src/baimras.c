#include <rwcore.h>

#define STDFUNC_IMAGESETFROMRASTER 6
#define STDFUNC_RASTERSETFROMIMAGE 7
#define STDFUNC_IMAGEFINDRASTERFORMAT 9

// The image flag that marks its pixels as gamma corrected.
#define IMAGE_GAMMACORRECTED 0x2

RwImage* RwImageSetFromRaster(RwImage* image, RwRaster* raster)
{
    if (RWSRCGLOBAL(stdFunc)[STDFUNC_IMAGESETFROMRASTER](image, raster, 0))
    {
        if (raster->privateFlags & rwRASTERGAMMACORRECTED)
        {
            image->flags |= IMAGE_GAMMACORRECTED;
        }

        return image;
    }

    return NULL;
}

RwRaster* RwRasterSetFromImage(RwRaster* raster, RwImage* image)
{
    if (RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERSETFROMIMAGE](raster, image, 0))
    {
        if (image->flags & IMAGE_GAMMACORRECTED)
        {
            raster->privateFlags |= rwRASTERGAMMACORRECTED;
        }

        return raster;
    }

    return NULL;
}

RwImage* RwImageFindRasterFormat(RwImage* image, S32 rasterType, S32* width, S32* height,
                                 S32* depth, S32* format)
{
    RwRaster raster;

    if (!RWSRCGLOBAL(stdFunc)[STDFUNC_IMAGEFINDRASTERFORMAT](&raster, image, rasterType))
    {
        return NULL;
    }

    *format = (raster.cFormat << 8) | raster.cType;
    *width = raster.width;
    *height = raster.height;
    *depth = raster.depth;

    return image;
}
