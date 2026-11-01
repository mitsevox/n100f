#include <rpcollis.h>

// RpCollisionData.flags: the tree and the triangle map share the data's own allocation.
#define COLLISIONDATA_SINGLEBLOCK 0x1

S32 _rpCollisionWorldSectorDataOffset;
S32 _rpCollisionGeometryDataOffset;
S32 _rpCollisionAtomicDataOffset;
S32 _rpCollisionGlobalsOffset;
S32 _rpCollisionNumInstances;

// The plugin's word in the engine instance, cleared when the plugin opens.
#define COLLISIONENGINEDATA() (*(void**)((U8*)RwEngineInstance + _rpCollisionGlobalsOffset))

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
    RpCollisionData** extData = &COLLISIONDATA(object, offset);

    if (*extData)
    {
        CollisionDataFree(*extData);
        *extData = NULL;
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
    S32 numTri;
    const U16* triIndex;

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

    numTri = data->numTriangles;
    triIndex = data->triangleMap;

    while (numTri--)
    {
        S32 i = *triIndex++;

        if (!RwStreamWriteInt32(stream, &i, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

static RwStream* CollisionDataStreamRead(RwStream* stream, S32 binaryLength, void* object,
                                         S32 offset, S32 size)
{
    if (binaryLength)
    {
        S32 numLeaves;
        S32 numTris;
        S32 treeSize;
        RpCollisionData* data;
        U32 dataSize;
        S32 numTri;
        U16* triIndex;

        if (!RwStreamReadInt32(stream, &numLeaves, sizeof(S32)) ||
            !RwStreamReadInt32(stream, &numTris, sizeof(S32)))
        {
            return NULL;
        }

        treeSize = _rpCollBSPTreeMemGetSize(numLeaves);
        dataSize = sizeof(RpCollisionData) + treeSize + numTris * sizeof(U16);
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
        data->numTriangles = numTris;
        data->triangleMap = (U16*)((U8*)data->tree + treeSize);

        _rpCollBSPTreeInit(data->tree, numLeaves);

        if (!_rpCollBSPTreeStreamRead(data->tree, stream))
        {
            RwFree(data);
            return NULL;
        }

        numTri = data->numTriangles;
        triIndex = data->triangleMap;

        while (numTri--)
        {
            S32 i;

            if (!RwStreamReadInt32(stream, &i, sizeof(S32)))
            {
                RwFree(data);
                return NULL;
            }

            *triIndex++ = i;
        }

        COLLISIONDATA(object, offset) = data;
    }

    return stream;
}

static S32 CollisionDataStreamGetSize(const void* object, S32 offsetInObject, S32 size)
{
    const RpCollisionData* data = COLLISIONDATA(object, offsetInObject);

    if (data)
    {
        U32 binarySize = _rpCollBSPTreeStreamGetSize(data->tree) + 2 * sizeof(S32);

        binarySize += data->numTriangles * sizeof(S32);

        return binarySize;
    }

    return 0;
}

static S32 CollisionWorldPluginAttach(void)
{
    S32 result;

    _rpCollisionGlobalsOffset =
        RwEngineRegisterPlugin(sizeof(void*), COLLIS_PLUGIN_ID, CollisionOpen, CollisionClose);
    result = _rpCollisionGlobalsOffset >= 0;

    if (result)
    {
        _rpCollisionWorldSectorDataOffset =
            RpWorldSectorRegisterPlugin(sizeof(RpCollisionData*), COLLIS_PLUGIN_ID,
                                        CollisionDataCreate, CollisionDataDestroy, NULL);
        result = _rpCollisionWorldSectorDataOffset >= 0;

        if (result)
        {
            result = RpWorldSectorRegisterPluginStream(COLLIS_PLUGIN_ID, CollisionDataStreamRead,
                                                       CollisionDataStreamWrite,
                                                       CollisionDataStreamGetSize) >= 0;
        }
    }

    return result;
}

static S32 CollisionGeometryPluginAttach(void)
{
    S32 result;

    _rpCollisionAtomicDataOffset =
        RpAtomicRegisterPlugin(sizeof(S32), COLLIS_PLUGIN_ID, CollisionAtomicInit, NULL, NULL);
    result = _rpCollisionAtomicDataOffset >= 0;

    if (result)
    {
        _rpCollisionGeometryDataOffset =
            RpGeometryRegisterPlugin(sizeof(RpCollisionData*), COLLIS_PLUGIN_ID,
                                     CollisionDataCreate, CollisionDataDestroy, NULL);
        result = _rpCollisionGeometryDataOffset >= 0;

        if (result)
        {
            result = RpGeometryRegisterPluginStream(COLLIS_PLUGIN_ID, CollisionDataStreamRead,
                                                    CollisionDataStreamWrite,
                                                    CollisionDataStreamGetSize) >= 0;
        }
    }

    return result;
}

S32 RpCollisionPluginAttach(void)
{
    S32 result = CollisionWorldPluginAttach();

    if (result)
    {
        result = CollisionGeometryPluginAttach();
    }

    return result;
}
