#include <rpworld.h>

// The lock-mode bit that invalidates the geometry's mesh.
#define GEOMLOCK_POLYGONS 0x01

static RwPluginRegistry geometryTKList = { sizeof(RpGeometry), sizeof(RpGeometry) };

S32 _rpMeshDestroy(RpMeshHeader* mesh);

RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack,
                                      void* pData)
{
    S32 numMaterials = geometry->matList.numMaterials;
    S32 i;

    for (i = 0; i < numMaterials; i++)
    {
        if (!fpCallBack(geometry->matList.materials[i], pData))
        {
            return geometry;
        }
    }

    return geometry;
}

RpGeometry* RpGeometryLock(RpGeometry* geometry, S32 lockMode)
{
    geometry->lockedSinceLastInst |= lockMode;

    if (lockMode & GEOMLOCK_POLYGONS)
    {
        if (geometry->mesh)
        {
            _rpMeshDestroy(geometry->mesh);
            geometry->mesh = NULL;
        }
    }

    return geometry;
}

S32 RpGeometryRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                             RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&geometryTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpGeometryRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                   RwPluginDataChunkWriteCallBack writeCB,
                                   RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&geometryTKList, pluginID, readCB, writeCB,
                                            getSizeCB);
}
