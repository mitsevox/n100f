#include <rpskin.h>
#include <string.h>

#define SKIN_PLUGIN_ID 0x116
#define MATFX_PLUGIN_ID 0x120
#define TOON_PLUGIN_ID 0x12E


static RpSkin* SkinCreate(U32 numBones, U32 numVertices)
{
    RpSkin* skin = (RpSkin*)RWSRCGLOBAL(memoryAlloc)(_rpSkinGlobals.freeList);
    U32 size;

    memset(skin, 0, sizeof(RpSkin));

    size = numVertices * (sizeof(U32) + sizeof(RwMatrixWeights)) + numBones * sizeof(RwMatrix) + 15;

    skin->unaligned = RwMalloc(size);
    memset(skin->unaligned, 0, size);

    skin->skinToBoneMatrices = (RwMatrix*)(((U32)skin->unaligned + 15) & ~15);
    skin->vertexBoneIndices = (U32*)(skin->skinToBoneMatrices + numBones);
    skin->vertexBoneWeights = (RwMatrixWeights*)(skin->vertexBoneIndices + numVertices);
    skin->numBones = numBones;

    return skin;
}

static void* SkinOpen(void* instance, S32 offset, S32 size)
{
    if (_rpSkinGlobals.module.numInstances == 0)
    {
        U32 pipes = 1;

        if (RwEngineGetPluginOffset(MATFX_PLUGIN_ID) != -1)
        {
            pipes |= 2;
        }

        if (RwEngineGetPluginOffset(TOON_PLUGIN_ID) != -1)
        {
            pipes |= 4;
        }

        _rpSkinPipelinesCreate(pipes);

        _rpSkinGlobals.freeList = RwFreeListCreate(sizeof(RpSkin), 20, 0);

        _rpSkinGlobals.matrixCacheUnaligned = RwMalloc(256 * sizeof(RwMatrix) + 15);
        memset(_rpSkinGlobals.matrixCacheUnaligned, 0, 256 * sizeof(RwMatrix) + 15);
        _rpSkinGlobals.matrixCache =
            (RwMatrix*)(((U32)_rpSkinGlobals.matrixCacheUnaligned + 15) & ~15);
    }

    _rpSkinGlobals.module.numInstances++;

    return instance;
}

static void* SkinClose(void* instance, S32 offset, S32 size)
{
    if (--_rpSkinGlobals.module.numInstances == 0)
    {
        _rpSkinPipelinesDestroy();

        RwFreeListDestroy(_rpSkinGlobals.freeList);
        _rpSkinGlobals.freeList = NULL;

        RwFree(_rpSkinGlobals.matrixCacheUnaligned);
        _rpSkinGlobals.matrixCacheUnaligned = NULL;
    }

    return instance;
}

static void* SkinGeometryConstructor(void* object, S32 offset, S32 size)
{
    SKINGEOMETRYGETDATA(object) = NULL;

    return object;
}

static void* SkinGeometryDestructor(void* object, S32 offset, S32 size)
{
    RpSkin* skin = SKINGEOMETRYGETDATA(object);

    if (skin)
    {
        _rpSkinDeinitialize((RpGeometry*)object);
        SKINGEOMETRYGETDATA(object) = RpSkinDestroy(skin);
    }

    return object;
}

static void* SkinGeometryCopy(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    return dstObject;
}

static void* SkinAtomicConstructor(void* object, S32 offset, S32 size)
{
    SKINATOMICGETDATA(object) = NULL;

    return object;
}

static void* SkinAtomicDestructor(void* object, S32 offset, S32 size)
{
    if (SKINATOMICGETDATA(object))
    {
        SKINATOMICGETDATA(object) = NULL;
    }

    return object;
}

static void* SkinAtomicCopy(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    SKINATOMICGETDATA(dstObject) = SKINATOMICGETDATA(srcObject);

    return dstObject;
}

static RpAtomic* SkinAtomicSetType(RpAtomic* atomic, RpSkinType type)
{
    RpSkinType attachType;

    if (RwEngineGetPluginOffset(MATFX_PLUGIN_ID) == -1 && type == rpSKINTYPEMATFX)
    {
        attachType = rpSKINTYPEGENERIC;
    }
    else if (RwEngineGetPluginOffset(TOON_PLUGIN_ID) == -1 && type == rpSKINTYPETOON)
    {
        attachType = rpSKINTYPEGENERIC;
    }
    else
    {
        attachType = type;
    }

    return _rpSkinPipelinesAttach(atomic, attachType);
}

static S32 SkinAtomicAlways(void* object, S32 offset, S32 size)
{
    RpSkinType type = rpSKINTYPEGENERIC;
    RpAtomic* atomic = (RpAtomic*)object;

    if (RwEngineGetPluginOffset(MATFX_PLUGIN_ID) != -1 &&
        *((U8*)atomic + RpAtomicGetPluginOffset(MATFX_PLUGIN_ID)))
    {
        type = rpSKINTYPEMATFX;
    }

    if (atomic->geometry && RpSkinGeometryGetSkin(atomic->geometry))
    {
        SkinAtomicSetType(atomic, type);
    }

    return TRUE;
}

static S32 SkinAtomicRights(void* object, S32 offset, S32 size, U32 extraData)
{
    RpAtomic* atomic = (RpAtomic*)object;

    if (atomic->geometry && RpSkinGeometryGetSkin(atomic->geometry))
    {
        SkinAtomicSetType(atomic, (RpSkinType)extraData);
    }

    return TRUE;
}

static S32 SkinGeometrySize(const void* object, S32 offset, S32 size)
{
    const RpGeometry* geometry = (const RpGeometry*)object;
    const RpSkin* skin = SKINGEOMETRYGETDATA(geometry);
    S32 total = 0;

    if (skin)
    {
        if (!(geometry->flags & rpGEOMETRYNATIVE))
        {
            total = sizeof(U32);
            total += geometry->numVertices * sizeof(U32);
            total += geometry->numVertices * sizeof(RwMatrixWeights);
            total += skin->numBones * sizeof(RwMatrix);
            total += skin->numBones * sizeof(U32);
        }
        else
        {
            total = _rpSkinGeometryNativeSize(geometry);
        }
    }

    return total;
}

static RwStream* SkinGeometryWrite(RwStream* stream, S32 length, const void* object, S32 offset,
                                   S32 size)
{
    const RpGeometry* geometry = (const RpGeometry*)object;
    const RpSkin* skin = SKINGEOMETRYGETDATA(geometry);

    if (skin)
    {
        if (!(geometry->flags & rpGEOMETRYNATIVE))
        {
            S32 numVertices = geometry->numVertices;
            U32 i;

            if (!RwStreamWriteInt32(stream, (const S32*)&skin->numBones, sizeof(S32)))
            {
                return NULL;
            }

            if (!RwStreamWriteInt32(stream, (const S32*)skin->vertexBoneIndices,
                                    numVertices * sizeof(U32)))
            {
                return NULL;
            }

            if (!RwStreamWriteReal(stream, (const F32*)skin->vertexBoneWeights,
                                   numVertices * sizeof(RwMatrixWeights)))
            {
                return NULL;
            }

            for (i = 0; i < skin->numBones; i++)
            {
                S32 marker = 0xDEADDEAD;
                const RwMatrix* matrix = &skin->skinToBoneMatrices[i];

                if (!RwStreamWriteInt32(stream, &marker, sizeof(S32)))
                {
                    return NULL;
                }

                if (!RwStreamWriteReal(stream, (const F32*)matrix, sizeof(RwMatrix)))
                {
                    return NULL;
                }
            }
        }
        else
        {
            if (!_rpSkinGeometryNativeWrite(stream, geometry))
            {
                return NULL;
            }
        }
    }

    return stream;
}

static RwStream* SkinGeometryRead(RwStream* stream, S32 length, void* object, S32 offset, S32 size)
{
    RpGeometry* geometry = (RpGeometry*)object;

    if (!(geometry->flags & rpGEOMETRYNATIVE))
    {
        S32 numBones;
        S32 numVertices;
        RpSkin* skin;
        U32 i;

        if (!RwStreamReadInt32(stream, &numBones, sizeof(S32)))
        {
            return NULL;
        }

        numVertices = geometry->numVertices;
        skin = SkinCreate(numBones, numVertices);

        if (!RwStreamReadInt32(stream, (S32*)skin->vertexBoneIndices, numVertices * sizeof(U32)))
        {
            return NULL;
        }

        if (!RwStreamReadReal(stream, (F32*)skin->vertexBoneWeights,
                              numVertices * sizeof(RwMatrixWeights)))
        {
            return NULL;
        }

        for (i = 0; i < skin->numBones; i++)
        {
            if (!RwStreamSkip(stream, sizeof(S32)))
            {
                return NULL;
            }

            if (!RwStreamReadReal(stream, (F32*)&skin->skinToBoneMatrices[i], sizeof(RwMatrix)))
            {
                return NULL;
            }
        }

        RpSkinGeometrySetSkin(geometry, skin);
    }
    else
    {
        if (!_rpSkinGeometryNativeRead(stream, geometry))
        {
            return NULL;
        }
    }

    return stream;
}

static RwStream* SkinAtomicRead(RwStream* stream, S32 length, void* object, S32 offset, S32 size)
{
    RpGeometry* geometry = ((RpAtomic*)object)->geometry;

    if (!RpSkinGeometryGetSkin(geometry))
    {
        S32 numBones;
        S32 numVertices;
        RpSkin* skin;
        U32 i;

        if (!RwStreamReadInt32(stream, &numBones, sizeof(S32)))
        {
            return NULL;
        }

        numVertices = geometry->numVertices;
        skin = SkinCreate(numBones, numVertices);

        if (!RwStreamSkip(stream, sizeof(S32)))
        {
            return NULL;
        }

        if (!RwStreamReadInt32(stream, (S32*)skin->vertexBoneIndices, numVertices * sizeof(U32)))
        {
            return NULL;
        }

        if (!RwStreamReadReal(stream, (F32*)skin->vertexBoneWeights,
                              numVertices * sizeof(RwMatrixWeights)))
        {
            return NULL;
        }

        for (i = 0; i < skin->numBones; i++)
        {
            if (!RwStreamSkip(stream, 3 * sizeof(S32)))
            {
                return NULL;
            }

            if (!RwStreamReadReal(stream, (F32*)&skin->skinToBoneMatrices[i], sizeof(RwMatrix)))
            {
                return NULL;
            }
        }

        RpSkinGeometrySetSkin(geometry, skin);
    }
    else
    {
        if (!RwStreamSkip(stream, size))
        {
            return NULL;
        }
    }

    return stream;
}

static RwStream* SkinAtomicWrite(RwStream* stream, S32 length, const void* object, S32 offset,
                                 S32 size)
{
    return stream;
}

static S32 SkinAtomicGetSize(const void* object, S32 offset, S32 size)
{
    return 0;
}

S32 RpSkinPluginAttach(void)
{
    _rpSkinGlobals.engineOffset = RwEngineRegisterPlugin(0, SKIN_PLUGIN_ID, SkinOpen, SkinClose);

    _rpSkinGlobals.atomicOffset =
        RpAtomicRegisterPlugin(sizeof(RpHAnimHierarchy*), SKIN_PLUGIN_ID, SkinAtomicConstructor,
                               SkinAtomicDestructor, SkinAtomicCopy);
    RpAtomicRegisterPluginStream(SKIN_PLUGIN_ID, SkinAtomicRead, SkinAtomicWrite,
                                 SkinAtomicGetSize);
    RpAtomicSetStreamAlwaysCallBack(SKIN_PLUGIN_ID, SkinAtomicAlways);
    RpAtomicSetStreamRightsCallBack(SKIN_PLUGIN_ID, SkinAtomicRights);

    _rpSkinGlobals.geometryOffset =
        RpGeometryRegisterPlugin(sizeof(RpSkin*), SKIN_PLUGIN_ID, SkinGeometryConstructor,
                                 SkinGeometryDestructor, SkinGeometryCopy);
    RpGeometryRegisterPluginStream(SKIN_PLUGIN_ID, SkinGeometryRead, SkinGeometryWrite,
                                   SkinGeometrySize);

    return TRUE;
}

RpAtomic* RpSkinAtomicSetHAnimHierarchy(RpAtomic* atomic, RpHAnimHierarchy* hierarchy)
{
    SKINATOMICGETDATA(atomic) = hierarchy;

    return atomic;
}

RpSkin* RpSkinGeometryGetSkin(RpGeometry* geometry)
{
    return SKINGEOMETRYGETDATA(geometry);
}

RpGeometry* RpSkinGeometrySetSkin(RpGeometry* geometry, RpSkin* skin)
{
    if (skin != SKINGEOMETRYGETDATA(geometry))
    {
        if (SKINGEOMETRYGETDATA(geometry))
        {
            _rpSkinDeinitialize(geometry);
        }

        SKINGEOMETRYGETDATA(geometry) = skin;

        if (skin)
        {
            _rpSkinInitialize(geometry);
        }
    }

    return geometry;
}

RpSkin* RpSkinDestroy(RpSkin* skin)
{
    if (skin->unaligned)
    {
        RwFree(skin->unaligned);
    }

    skin->unaligned = NULL;
    skin->skinToBoneMatrices = NULL;
    skin->vertexBoneIndices = NULL;
    skin->vertexBoneWeights = NULL;
    skin->numBones = 0;

    RWSRCGLOBAL(memoryFree)(_rpSkinGlobals.freeList, skin);

    return NULL;
}

U32 RpSkinGetNumBones(RpSkin* skin)
{
    return skin->numBones;
}

const RwMatrixWeights* RpSkinGetVertexBoneWeights(RpSkin* skin)
{
    return skin->vertexBoneWeights;
}

const U32* RpSkinGetVertexBoneIndices(RpSkin* skin)
{
    return skin->vertexBoneIndices;
}

const RwMatrix* RpSkinGetSkinToBoneMatrices(RpSkin* skin)
{
    return skin->skinToBoneMatrices;
}

RpAtomic* RpSkinAtomicSetType(RpAtomic* atomic, RpSkinType type)
{
    return SkinAtomicSetType(atomic, type);
}
