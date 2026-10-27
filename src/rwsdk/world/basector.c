#include <rpworld.h>

RwPluginRegistry sectorTKList = { sizeof(RpWorldSector), sizeof(RpWorldSector) };
static RwModuleInfo sectorModule;

void* _rpSectorOpen(void* instance, S32 offset, S32 size)
{
    sectorModule.numInstances++;

    return instance;
}

void* _rpSectorClose(void* instance, S32 offset, S32 size)
{
    sectorModule.numInstances--;

    return instance;
}

S32 RpWorldSectorGetNumVertices(const RpWorldSector* sector)
{
    return sector->numVertices;
}

RpWorldSector* RpWorldSectorRender(RpWorldSector* worldSector)
{
    return ((RpWorld*)RWSRCGLOBAL(curWorld))->renderCallBack(worldSector);
}

S32 RpWorldSectorRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                                RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&sectorTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpWorldSectorRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                      RwPluginDataChunkWriteCallBack writeCB,
                                      RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&sectorTKList, pluginID, readCB, writeCB, getSizeCB);
}
