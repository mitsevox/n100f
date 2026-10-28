#include <rpcollis.h>

#define COLLIS_PLUGIN_ID 0x11D

// RpCollisionData.flags: the tree and the triangle map share the data's own allocation.
#define COLLISIONDATA_SINGLEBLOCK 0x1

S32 _rpCollisionNumInstances;
S32 _rpCollisionGlobalsOffset;
S32 _rpCollisionAtomicDataOffset;
S32 _rpCollisionGeometryDataOffset;
S32 _rpCollisionWorldSectorDataOffset;

// The plugin's word in the engine instance, cleared when the plugin opens.
#define COLLISIONENGINEDATA() (*(void**)((U8*)RwEngineInstance + _rpCollisionGlobalsOffset))
#define COLLISIONDATA(object, offset) (*(RpCollisionData**)((U8*)(object) + (offset)))

static void* CollisionOpen(void* instance, S32 offset, S32 size)
{
    _rpCollisionNumInstances++;
    COLLISIONENGINEDATA() = NULL;

    return instance;
}

static void* CollisionClose(void* instance, S32 offset, S32 size)
{
    _rpCollisionNumInstances--;

    return instance;
}

static void CollisionDataFree(RpCollisionData* data)
{
    if (data)
    {
        if (!(data->flags & COLLISIONDATA_SINGLEBLOCK))
        {
            _rpCollBSPTreeDestroy(data->tree);
        }

        RwFree(data);
    }
}

static void* CollisionDataDestroy(void* object, S32 offset, S32 size)
{
    RpCollisionData* data = COLLISIONDATA(object, offset);

    if (data)
    {
        CollisionDataFree(data);
        COLLISIONDATA(object, offset) = NULL;
    }

    return object;
}

static void* CollisionDataCreate(void* object, S32 offset, S32 size)
{
    COLLISIONDATA(object, offset) = NULL;

    return object;
}

static void* CollisionAtomicInit(void* object, S32 offset, S32 size)
{
    *(S32*)((U8*)object + _rpCollisionAtomicDataOffset) = 0;

    return object;
}

static RwStream* CollisionDataStreamWrite(RwStream* stream, S32 length, const void* object,
                                          S32 offset, S32 size)
{
    const RpCollisionData* data = COLLISIONDATA(object, offset);
    S32 numTriangles;
    const U16* map;

    if (!data)
    {
        return stream;
    }

    if (!RwStreamWriteInt32(stream, (const S32*)data->tree, sizeof(S32)) ||
        !RwStreamWriteInt32(stream, &data->numTriangles, sizeof(S32)))
    {
        return NULL;
    }

    if (!_rpCollBSPTreeStreamWrite(data->tree, stream))
    {
        return NULL;
    }

    numTriangles = data->numTriangles;
    map = data->triangleMap;

    while (numTriangles--)
    {
        S32 index = *map++;

        if (!RwStreamWriteInt32(stream, &index, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

static RwStream* CollisionDataStreamRead(RwStream* stream, S32 length, void* object, S32 offset,
                                         S32 size)
{
    if (length)
    {
        S32 numLeafNodes;
        S32 numTriangles;
        U32 treeSize;
        RpCollisionData* data;
        U32 dataSize;
        S32 count;
        U16* map;

        if (!RwStreamReadInt32(stream, &numLeafNodes, sizeof(S32)) ||
            !RwStreamReadInt32(stream, &numTriangles, sizeof(S32)))
        {
            return NULL;
        }

        treeSize = _rpCollBSPTreeMemGetSize(numLeafNodes);
        dataSize = sizeof(RpCollisionData) + treeSize + numTriangles * sizeof(U16);
        data = (RpCollisionData*)RwMalloc(dataSize);

        if (!data)
        {
            RwError err;

            err.pluginID = COLLIS_PLUGIN_ID;
            err.errorCode = _rwerror(0x80000013, dataSize);
            RwErrorSet(&err);
            return NULL;
        }

        data->flags = COLLISIONDATA_SINGLEBLOCK;
        data->tree = (RpCollBSPTree*)(data + 1);
        data->numTriangles = numTriangles;
        data->triangleMap = (U16*)((U8*)data->tree + treeSize);

        _rpCollBSPTreeInit(data->tree, numLeafNodes);

        if (!_rpCollBSPTreeStreamRead(data->tree, stream))
        {
            RwFree(data);
            return NULL;
        }

        count = data->numTriangles;
        map = data->triangleMap;

        while (count--)
        {
            S32 index;

            if (!RwStreamReadInt32(stream, &index, sizeof(S32)))
            {
                RwFree(data);
                return NULL;
            }

            *map++ = index;
        }

        COLLISIONDATA(object, offset) = data;
    }

    return stream;
}

static S32 CollisionDataStreamGetSize(const void* object, S32 offset, S32 size)
{
    const RpCollisionData* data = COLLISIONDATA(object, offset);

    if (data)
    {
        S32 total = _rpCollBSPTreeStreamGetSize(data->tree) + 2 * sizeof(S32);

        total += data->numTriangles * sizeof(S32);

        return total;
    }

    return 0;
}

static S32 CollisionWorldPluginAttach(void)
{
    S32 success;

    _rpCollisionGlobalsOffset =
        RwEngineRegisterPlugin(sizeof(void*), COLLIS_PLUGIN_ID, CollisionOpen, CollisionClose);
    success = _rpCollisionGlobalsOffset >= 0;

    if (success)
    {
        _rpCollisionWorldSectorDataOffset =
            RpWorldSectorRegisterPlugin(sizeof(RpCollisionData*), COLLIS_PLUGIN_ID,
                                        CollisionDataCreate, CollisionDataDestroy, NULL);
        success = _rpCollisionWorldSectorDataOffset >= 0;

        if (success)
        {
            success = RpWorldSectorRegisterPluginStream(COLLIS_PLUGIN_ID, CollisionDataStreamRead,
                                                        CollisionDataStreamWrite,
                                                        CollisionDataStreamGetSize) >= 0;
        }
    }

    return success;
}

static S32 CollisionGeometryPluginAttach(void)
{
    S32 success;

    _rpCollisionAtomicDataOffset =
        RpAtomicRegisterPlugin(sizeof(S32), COLLIS_PLUGIN_ID, CollisionAtomicInit, NULL, NULL);
    success = _rpCollisionAtomicDataOffset >= 0;

    if (success)
    {
        _rpCollisionGeometryDataOffset =
            RpGeometryRegisterPlugin(sizeof(RpCollisionData*), COLLIS_PLUGIN_ID,
                                     CollisionDataCreate, CollisionDataDestroy, NULL);
        success = _rpCollisionGeometryDataOffset >= 0;

        if (success)
        {
            success = RpGeometryRegisterPluginStream(COLLIS_PLUGIN_ID, CollisionDataStreamRead,
                                                     CollisionDataStreamWrite,
                                                     CollisionDataStreamGetSize) >= 0;
        }
    }

    return success;
}

S32 RpCollisionPluginAttach(void)
{
    S32 success = CollisionWorldPluginAttach();

    if (success)
    {
        success = CollisionGeometryPluginAttach();
    }

    return success;
}
