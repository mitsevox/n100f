#include <rpworld.h>

// What WorldFindSector looks for and whether it found it.
typedef struct WorldSectorSearch WorldSectorSearch;
struct WorldSectorSearch
{
    RpWorldSector* sector;
    S32 found;
};

RwPluginRegistry worldTKList = { sizeof(RpWorld), sizeof(RpWorld) };

RpWorldSector* WorldSectorRender(RpWorldSector* worldSector, void* data);
RwCamera* RwCameraForAllSectorsInFrustum(RwCamera* camera, RpWorldSectorCallBack callBack,
                                         void* pData);

static RpWorldSector* WorldFindSector(RpWorldSector* worldSector, void* data)
{
    WorldSectorSearch* search = (WorldSectorSearch*)data;

    if (search->sector == worldSector)
    {
        search->found = 1;
        return NULL;
    }

    return worldSector;
}

RpWorld* RpWorldRender(RpWorld* world)
{
    RwCameraForAllSectorsInFrustum((RwCamera*)RWSRCGLOBAL(curCamera), WorldSectorRender, world);

    return world;
}

S32 RpWorldRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&worldTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpWorldRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&worldTKList, pluginID, readCB, writeCB, getSizeCB);
}
