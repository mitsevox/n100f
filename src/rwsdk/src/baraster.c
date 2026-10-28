#include <rwcore.h>
#include <string.h>

// The standard device functions the raster calls.
#define STDFUNC_RASTERCREATE 4
#define STDFUNC_RASTERDESTROY 5
#define STDFUNC_RASTERLOCK 15
#define STDFUNC_RASTERUNLOCK 16
#define STDFUNC_RASTERSHOWRASTER 20
#define STDFUNC_RASTERLOCKPALETTE 23
#define STDFUNC_RASTERUNLOCKPALETTE 24
#define STDFUNC_RASTERGETMIPLEVELS 28

#define rwRASTERFORMATMIPMAP 0x8000
#define rwRASTERPALETTELOCKEDREAD 0x08
#define rwRASTERPALETTELOCKEDWRITE 0x10
#define rwRASTERPALETTELOCKED (rwRASTERPALETTELOCKEDREAD | rwRASTERPALETTELOCKEDWRITE)

// The raster module's data in the engine instance. emptyRaster is a raster with no pixels that
// _rwRasterOpen sets up and points unk0 at.
typedef struct RasterModuleData RasterModuleData;
struct RasterModuleData
{
    RwRaster* unk0;
    U8 unk4[0x24];
    S32 unk28;
    RwRaster emptyRaster;
    RwFreeList* freeList;
};

#define RASTER_DATA(var) \
    (((RasterModuleData*)((U8*)RwEngineInstance + rasterModule.globalsOffset))->var)

static RwPluginRegistry rasterTKList = { sizeof(RwRaster), sizeof(RwRaster) };
static RwModuleInfo rasterModule;

RwRaster* RwRasterUnlock(RwRaster* raster)
{
    RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERUNLOCK](NULL, raster, 0);

    return raster;
}

RwRaster* RwRasterUnlockPalette(RwRaster* raster)
{
    RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERUNLOCKPALETTE](NULL, raster, 0);
    raster->privateFlags &= ~rwRASTERPALETTELOCKED;

    return raster;
}

S32 RwRasterDestroy(RwRaster* raster)
{
    _rwPluginRegistryDeInitObject(&rasterTKList, raster);
    RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERDESTROY](NULL, raster, 0);
    RWSRCGLOBAL(memoryFree)(RASTER_DATA(freeList), raster);

    return TRUE;
}

S32 RwRasterRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&rasterTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

U8* RwRasterLockPalette(RwRaster* raster, S32 lockMode)
{
    U8* palette;

    if (RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERLOCKPALETTE](&palette, raster, lockMode))
    {
        return palette;
    }

    return NULL;
}

S32 RwRasterGetNumLevels(RwRaster* raster)
{
    S32 numLevels;

    if (!((raster->cFormat << 8) & rwRASTERFORMATMIPMAP))
    {
        return 1;
    }

    if (RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERGETMIPLEVELS](&numLevels, raster, 0))
    {
        return numLevels;
    }

    return -1;
}

RwRaster* RwRasterShowRaster(RwRaster* raster, void* dev, U32 flags)
{
    RwStandardFunc showRaster = RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERSHOWRASTER];

    _rwResourcesPurge();

    if (showRaster(raster, dev, flags))
    {
        return raster;
    }

    return NULL;
}

RwRaster* RwRasterCreate(S32 width, S32 height, S32 depth, S32 flags)
{
    RwRaster* raster = (RwRaster*)RWSRCGLOBAL(memoryAlloc)(RASTER_DATA(freeList));

    if (raster)
    {
        RwStandardFunc rasterCreate = RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERCREATE];

        raster->privateFlags = 0;
        raster->cFlags = 0;
        raster->width = width;
        raster->height = height;
        raster->nOffsetX = 0;
        raster->nOffsetY = 0;
        raster->depth = depth;
        raster->parent = raster;
        raster->cpPixels = NULL;
        raster->palette = NULL;

        if (!rasterCreate(NULL, raster, flags))
        {
            RWSRCGLOBAL(memoryFree)(RASTER_DATA(freeList), raster);
            return NULL;
        }

        _rwPluginRegistryInitObject(&rasterTKList, raster);

        return raster;
    }

    return NULL;
}

U8* RwRasterLock(RwRaster* raster, U8 level, S32 lockMode)
{
    U8* pixels;

    if (RWSRCGLOBAL(stdFunc)[STDFUNC_RASTERLOCK](&pixels, raster, lockMode + ((S32)level << 8)))
    {
        return pixels;
    }

    return NULL;
}

void* _rwRasterClose(void* instance, S32 offset, S32 size)
{
    if (RASTER_DATA(freeList))
    {
        RwFreeListDestroy(RASTER_DATA(freeList));
        RASTER_DATA(freeList) = NULL;
    }

    rasterModule.numInstances--;

    return instance;
}

void* _rwRasterOpen(void* instance, S32 offset, S32 size)
{
    rasterModule.globalsOffset = offset;

    memset(&RASTER_DATA(emptyRaster), 0, sizeof(RwRaster));
    RASTER_DATA(emptyRaster).width = 0;
    RASTER_DATA(emptyRaster).height = 0;
    RASTER_DATA(emptyRaster).depth = 0;
    RASTER_DATA(emptyRaster).cFlags = rwRASTERDONTALLOCATE;
    RASTER_DATA(emptyRaster).cpPixels = NULL;
    RASTER_DATA(emptyRaster).palette = NULL;
    RASTER_DATA(emptyRaster).cType = rwRASTERTYPENORMAL;
    RASTER_DATA(unk28) = 0;
    RASTER_DATA(unk0) = &RASTER_DATA(emptyRaster);

    RASTER_DATA(freeList) = RwFreeListCreate(rasterTKList.sizeOfStruct, 20, 0);

    if (!RASTER_DATA(freeList))
    {
        return NULL;
    }

    rasterModule.numInstances++;

    return instance;
}
