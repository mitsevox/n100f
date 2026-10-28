#include <rwcore.h>

// The standard device functions the raster calls.
#define STDFUNC_RASTERDESTROY 5
#define STDFUNC_RASTERLOCK 15
#define STDFUNC_RASTERUNLOCK 16
#define STDFUNC_RASTERLOCKPALETTE 23
#define STDFUNC_RASTERUNLOCKPALETTE 24
#define STDFUNC_RASTERGETMIPLEVELS 28

#define rwRASTERFORMATMIPMAP 0x8000
#define rwRASTERPALETTELOCKEDREAD 0x08
#define rwRASTERPALETTELOCKEDWRITE 0x10
#define rwRASTERPALETTELOCKED (rwRASTERPALETTELOCKEDREAD | rwRASTERPALETTELOCKEDWRITE)

// The raster module's data in the engine instance. Partial: only the free list is declared.
typedef struct RasterModuleData RasterModuleData;
struct RasterModuleData
{
    U8 unk0[0x60];
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
