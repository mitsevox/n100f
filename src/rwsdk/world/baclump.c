#include <rpworld.h>
#include <string.h>

// The extension the clump plugin adds to cameras and lights: the owning clump and the link in
// that clump's camera or light list.
typedef struct ClumpObjectExt ClumpObjectExt;
struct ClumpObjectExt
{
    RpClump* clump;
    RwLLLink inClumpLink;
};

#define CLUMPEXTFROMOBJECT(object, offset) ((ClumpObjectExt*)((U8*)(object) + (offset)))
#define OBJECTFROMCLUMPLINK(link, offset) \
    ((void*)((U8*)(link) - (S32)&((ClumpObjectExt*)0)->inClumpLink - (offset)))

static RwPluginRegistry atomicTKList = { sizeof(RpAtomic), sizeof(RpAtomic) };
static RwPluginRegistry clumpTKList = { sizeof(RpClump), sizeof(RpClump) };

static S32 _rpClumpCameraExtOffset;
static S32 _rpClumpLightExtOffset;

// The clump module's globals: the free lists that atomics and clumps come from.
typedef struct ClumpModuleData ClumpModuleData;
struct ClumpModuleData
{
    RwFreeList* atomicFreeList;
    RwFreeList* clumpFreeList;
};

#define CLUMP_DATA(var) \
    (((ClumpModuleData*)((U8*)RwEngineInstance + clumpModule.globalsOffset))->var)

// The object types of an atomic and a clump.
#define ATOMIC_OBJECTTYPE 1
#define CLUMP_OBJECTTYPE 2

// The first stream version whose atomic chunk carries a geometry index.
#define ATOMIC_GEOMINDEXVERSION 0x30400

// The geometries a clump chunk carries ahead of its atomics.
typedef struct GeometryList GeometryList;
struct GeometryList
{
    RpGeometry** geometries;
    S32 numGeoms;
};

// The atomic chunk's struct, and the older form without a geometry index.
typedef struct AtomicStreamData AtomicStreamData;
struct AtomicStreamData
{
    S32 frameIndex;
    S32 geomIndex;
    S32 flags;
    S32 unused;
};

typedef struct AtomicStreamDataOld AtomicStreamDataOld;
struct AtomicStreamDataOld
{
    S32 frameIndex;
    S32 flags;
    S32 unused;
};

// The last stream version whose clump struct holds only the atomic count.
#define CLUMP_ATOMICSONLYVERSION 0x33000

// The clump chunk's struct.
typedef struct ClumpStreamData ClumpStreamData;
struct ClumpStreamData
{
    S32 numAtomics;
    S32 numLights;
    S32 numCameras;
};

// Releases the geometries a clump chunk carried once its atomics hold their own references.
#define GEOMETRYLIST_DEINITIALIZE(geomList)                                                        \
    do                                                                                             \
    {                                                                                              \
        if ((geomList).numGeoms)                                                                   \
        {                                                                                          \
            S32 _i;                                                                                \
                                                                                                   \
            for (_i = 0; _i < (geomList).numGeoms; _i++)                                           \
            {                                                                                      \
                RpGeometryDestroy((geomList).geometries[_i]);                                      \
            }                                                                                      \
                                                                                                   \
            RwFree((geomList).geometries);                                                         \
        }                                                                                          \
    } while (0)

// The larger of two axis lengths, the first on a tie.
#define SCALEMAX(a, b) ((a) >= (b) ? (a) : (b))

static RwModuleInfo clumpModule;
static U32 lastSeenRightsPluginId;
static U32 lastSeenExtraData;

void _rpAtomicResyncInterpolatedSphere(RpAtomic* atomic);

static void ClumpTidyDestroyClump(void* clump, void* data)
{
    RpClumpDestroy((RpClump*)clump);
}

static void ClumpTidyDestroyAtomic(void* atomic, void* data)
{
    RpAtomicDestroy((RpAtomic*)atomic);
}

RwStream* _rpReadAtomicRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size)
{
    if (!RwStreamReadInt32(stream, (S32*)&lastSeenRightsPluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (length == 8)
    {
        if (!RwStreamReadInt32(stream, (S32*)&lastSeenExtraData, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

RwStream* _rpWriteAtomicRights(RwStream* stream, S32 length, const void* object, S32 offset,
                               S32 size)
{
    const RpAtomic* atomic = (const RpAtomic*)object;

    if (!RwStreamWriteInt32(stream, (const S32*)&atomic->pipeline->pluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, (const S32*)&atomic->pipeline->pluginData, sizeof(S32)))
    {
        return NULL;
    }

    return stream;
}

S32 _rpSizeAtomicRights(const void* object, S32 offset, S32 size)
{
    const RpAtomic* atomic = (const RpAtomic*)object;

    if (atomic->pipeline && atomic->pipeline->pluginId)
    {
        return 8;
    }

    return 0;
}

static RwObjectHasFrame* AtomicSync(RwObjectHasFrame* object)
{
    RpAtomic* atomic = (RpAtomic*)object;

    if (atomic->interpolator.flags & rpINTERPOLATORDIRTYSPHERE)
    {
        _rpAtomicResyncInterpolatedSphere(atomic);
    }

    atomic->object.object.privateFlags |= 1;

    return object;
}

RpAtomic* AtomicDefaultRenderCallBack(RpAtomic* atomic)
{
    RxPipeline* pipeline = atomic->pipeline;

    if (!pipeline)
    {
        pipeline = RXPIPELINEGLOBAL(defaultAtomicPipeline);
    }

    if (RxPipelineExecute(pipeline, atomic, 1))
    {
        return atomic;
    }

    return NULL;
}

static GeometryList* GeometryListStreamRead(RwStream* stream, GeometryList* geomList)
{
    S32 i;
    S32 numGeoms;
    U32 size;
    U32 version;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        if (RwStreamRead(stream, &numGeoms, sizeof(numGeoms)) != sizeof(numGeoms))
        {
            return NULL;
        }

        RwMemNative32(&numGeoms, sizeof(numGeoms));

        geomList->numGeoms = numGeoms;

        if (geomList->numGeoms > 0)
        {
            geomList->geometries = (RpGeometry**)RwMalloc(numGeoms * sizeof(RpGeometry*));

            if (!geomList->geometries)
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x80000013, numGeoms * sizeof(RpGeometry*));
                RwErrorSet(&err);

                return NULL;
            }
        }
        else
        {
            geomList->geometries = NULL;
        }

        for (i = 0; i < numGeoms; i++)
        {
            if (!RwStreamFindChunk(stream, rwID_GEOMETRY, NULL, &version))
            {
                return NULL;
            }

            if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
            {
                if (!(geomList->geometries[i] = RpGeometryStreamRead(stream)))
                {
                    return NULL;
                }
            }
            else
            {
                return NULL;
            }
        }

        return geomList;
    }
    else
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);

        return NULL;
    }
}

static void* ClumpInitCameraExt(void* object, S32 offset, S32 size)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(object, _rpClumpCameraExtOffset);

    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return object;
}

static void* ClumpDeInitCameraExt(void* object, S32 offset, S32 size)
{
    return object;
}

static void* ClumpInitLightExt(void* object, S32 offset, S32 size)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(object, _rpClumpLightExtOffset);

    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return object;
}

static void* ClumpDeInitLightExt(void* object, S32 offset, S32 size)
{
    return object;
}

static RpClump* ClumpCallBack(RpClump* clump, void* data)
{
    return clump;
}

static RpAtomic* ClumpAtomicStreamRead(RwStream* stream, rwFrameList* frameList,
                                       GeometryList* geomList)
{
    U32 size;
    U32 version;
    RpAtomic* atomic;
    RpGeometry* geometry;
    S32 status;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x8000001A);
        RwErrorSet(&err);

        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        if (version >= ATOMIC_GEOMINDEXVERSION)
        {
            AtomicStreamData data;

            memset(&data, 0, sizeof(data));

            status = (RwStreamRead(stream, &data, size) == size);

            if (!status)
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);

                return NULL;
            }

            RwMemNative32(&data, sizeof(data));

            atomic = RpAtomicCreate();

            if (!atomic)
            {
                return NULL;
            }

            rwObjectSetFlags(atomic, data.flags);

            if (frameList->numFrames)
            {
                RpAtomicSetFrame(atomic, frameList->frames[data.frameIndex]);
            }

            if (geomList->numGeoms)
            {
                RpAtomicSetGeometry(atomic, geomList->geometries[data.geomIndex], 0);
            }
            else
            {
                if (!RwStreamFindChunk(stream, rwID_GEOMETRY, NULL, &version))
                {
                    RpAtomicDestroy(atomic);
                    {
                        RwError err;

                        err.pluginID = 2;
                        err.errorCode = _rwerror(0x8000001A);
                        RwErrorSet(&err);
                    }

                    return NULL;
                }

                if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
                {
                    geometry = RpGeometryStreamRead(stream);

                    if (!geometry)
                    {
                        RpAtomicDestroy(atomic);
                        {
                            RwError err;

                            err.pluginID = 2;
                            err.errorCode = _rwerror(0x8000001A);
                            RwErrorSet(&err);
                        }

                        return NULL;
                    }

                    RpAtomicSetGeometry(atomic, geometry, 0);
                    RpGeometryDestroy(geometry);
                }
                else
                {
                    RpAtomicDestroy(atomic);
                    {
                        RwError err;

                        err.pluginID = 2;
                        err.errorCode = _rwerror(0x80000004);
                        RwErrorSet(&err);
                    }

                    return NULL;
                }
            }
        }
        else
        {
            AtomicStreamDataOld data;

            memset(&data, 0, sizeof(data));

            status = (RwStreamRead(stream, &data, size) == size);

            if (!status)
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);

                return NULL;
            }

            RwMemNative32(&data, sizeof(data));

            atomic = RpAtomicCreate();

            if (!atomic)
            {
                return NULL;
            }

            rwObjectSetFlags(atomic, data.flags);

            if (frameList->numFrames)
            {
                RpAtomicSetFrame(atomic, frameList->frames[data.frameIndex]);
            }

            if (!RwStreamFindChunk(stream, rwID_GEOMETRY, NULL, &version))
            {
                RpAtomicDestroy(atomic);
                {
                    RwError err;

                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x8000001A);
                    RwErrorSet(&err);
                }

                return NULL;
            }

            if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
            {
                geometry = RpGeometryStreamRead(stream);

                if (!geometry)
                {
                    RpAtomicDestroy(atomic);
                    {
                        RwError err;

                        err.pluginID = 2;
                        err.errorCode = _rwerror(0x8000001A);
                        RwErrorSet(&err);
                    }

                    return NULL;
                }

                RpAtomicSetGeometry(atomic, geometry, 0);
                RpGeometryDestroy(geometry);
            }
            else
            {
                RpAtomicDestroy(atomic);
                {
                    RwError err;

                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x80000004);
                    RwErrorSet(&err);
                }

                return NULL;
            }
        }

        lastSeenRightsPluginId = 0;
        lastSeenExtraData = 0;

        status = (_rwPluginRegistryReadDataChunks(&atomicTKList, stream, atomic) != NULL);

        if (!status)
        {
            RwError err;

            err.pluginID = 2;
            err.errorCode = _rwerror(0x8000001A);
            RwErrorSet(&err);

            return NULL;
        }

        if (lastSeenRightsPluginId)
        {
            _rwPluginRegistryInvokeRights(&atomicTKList, lastSeenRightsPluginId, atomic,
                                          lastSeenExtraData);
        }

        return atomic;
    }
    else
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);

        return NULL;
    }
}

void _rpAtomicResyncInterpolatedSphere(RpAtomic* atomic)
{
    RpGeometry* geometry = atomic->geometry;

    if (geometry)
    {
        if (atomic->interpolator.startMorphTarget == atomic->interpolator.endMorphTarget ||
            atomic->interpolator.startMorphTarget >= geometry->numMorphTargets ||
            atomic->interpolator.endMorphTarget >= geometry->numMorphTargets)
        {
            if (atomic->interpolator.startMorphTarget >= geometry->numMorphTargets ||
                atomic->interpolator.endMorphTarget >= geometry->numMorphTargets)
            {
                atomic->boundingSphere = geometry->morphTarget[0].boundingSphere;
            }
            else
            {
                atomic->boundingSphere =
                    geometry->morphTarget[atomic->interpolator.startMorphTarget].boundingSphere;
            }
        }
        else
        {
            RpMorphTarget* startMorph =
                &geometry->morphTarget[atomic->interpolator.startMorphTarget];
            RpMorphTarget* endMorph = &geometry->morphTarget[atomic->interpolator.endMorphTarget];
            F32 scale = atomic->interpolator.recipTime * atomic->interpolator.position;

            atomic->boundingSphere.radius =
                startMorph->boundingSphere.radius +
                scale * (endMorph->boundingSphere.radius - startMorph->boundingSphere.radius);

            RwV3dSubMacro(&atomic->boundingSphere.center, &endMorph->boundingSphere.center,
                          &startMorph->boundingSphere.center);
            RwV3dScaleMacro(&atomic->boundingSphere.center, &atomic->boundingSphere.center,
                            scale);
            RwV3dAddMacro(&atomic->boundingSphere.center, &atomic->boundingSphere.center,
                          &startMorph->boundingSphere.center);
        }

        atomic->interpolator.flags &= ~rpINTERPOLATORDIRTYSPHERE;
        atomic->object.object.privateFlags |= 1;
    }
}

const RwSphere* RpAtomicGetWorldBoundingSphere(RpAtomic* atomic)
{
    RwFrame* frame = RpAtomicGetFrame(atomic);

    if (atomic->interpolator.flags & rpINTERPOLATORDIRTYSPHERE)
    {
        _rpAtomicResyncInterpolatedSphere(atomic);
    }

    if (RwFrameDirty(frame) || (atomic->object.object.privateFlags & 1))
    {
        RwMatrix* ltm = RwFrameGetLTM(frame);

        RwV3dTransformPoints(&atomic->worldBoundingSphere.center,
                             &atomic->boundingSphere.center, 1, ltm);

        if ((ltm->flags & rwMATRIXTYPEORTHONORMAL) != rwMATRIXTYPEORTHONORMAL)
        {
            F32 rightLen = RwV3dDotProductMacro(&ltm->right, &ltm->right);
            F32 upLen = RwV3dDotProductMacro(&ltm->up, &ltm->up);
            F32 atLen = RwV3dDotProductMacro(&ltm->at, &ltm->at);

            atomic->worldBoundingSphere.radius =
                atomic->boundingSphere.radius *
                _rwSqrt(SCALEMAX(rightLen, SCALEMAX(upLen, atLen)));
        }
        else
        {
            atomic->worldBoundingSphere.radius = atomic->boundingSphere.radius;
        }

        atomic->object.object.privateFlags &= ~1;
    }

    return &atomic->worldBoundingSphere;
}

void* _rpClumpClose(void* instance, S32 offset, S32 size)
{
    RwFreeListForAllUsed(CLUMP_DATA(clumpFreeList), ClumpTidyDestroyClump, NULL);
    RwFreeListForAllUsed(CLUMP_DATA(atomicFreeList), ClumpTidyDestroyAtomic, NULL);

    RwFreeListDestroy(CLUMP_DATA(atomicFreeList));
    RwFreeListDestroy(CLUMP_DATA(clumpFreeList));

    CLUMP_DATA(atomicFreeList) = NULL;
    CLUMP_DATA(clumpFreeList) = NULL;

    clumpModule.numInstances--;

    return instance;
}

void* _rpClumpOpen(void* instance, S32 offset, S32 size)
{
    clumpModule.globalsOffset = offset;

    CLUMP_DATA(atomicFreeList) = RwFreeListCreate(atomicTKList.sizeOfStruct, 20, 0);

    if (CLUMP_DATA(atomicFreeList))
    {
        CLUMP_DATA(clumpFreeList) = RwFreeListCreate(clumpTKList.sizeOfStruct, 10, 0);

        if (CLUMP_DATA(clumpFreeList))
        {
            clumpModule.numInstances++;

            return instance;
        }

        RwFreeListDestroy(CLUMP_DATA(atomicFreeList));
        CLUMP_DATA(atomicFreeList) = NULL;
    }

    return NULL;
}

S32 _rpClumpRegisterExtensions(void)
{
    _rpClumpCameraExtOffset = RwCameraRegisterPlugin(sizeof(ClumpObjectExt), rwID_CLUMP,
                                                     ClumpInitCameraExt, ClumpDeInitCameraExt,
                                                     NULL);
    if (_rpClumpCameraExtOffset < 0)
    {
        return 0;
    }

    return (_rpClumpLightExtOffset =
                RpLightRegisterPlugin(sizeof(ClumpObjectExt), rwID_CLUMP, ClumpInitLightExt,
                                      ClumpDeInitLightExt, NULL)) >= 0;
}

RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* pData)
{
    RwLLLink* cur = clump->atomicList.link.next;
    RwLLLink* end = &clump->atomicList.link;

    while (cur != end)
    {
        RpAtomic* atomic = (RpAtomic*)((U8*)cur - (S32)&((RpAtomic*)0)->inClumpLink);
        RwLLLink* next = cur->next;

        if (!callback(atomic, pData))
        {
            return clump;
        }

        cur = next;
    }

    return clump;
}

RpClump* RpClumpForAllCameras(RpClump* clump, RwCameraCallBack callback, void* pData)
{
    RwLLLink* cur = clump->cameraList.link.next;
    RwLLLink* end = &clump->cameraList.link;

    while (cur != end)
    {
        RwCamera* camera = (RwCamera*)OBJECTFROMCLUMPLINK(cur, _rpClumpCameraExtOffset);
        RwLLLink* next = cur->next;

        if (!callback(camera, pData))
        {
            return clump;
        }

        cur = next;
    }

    return clump;
}

RpClump* RpClumpForAllLights(RpClump* clump, RpLightCallBack callback, void* pData)
{
    RwLLLink* cur = clump->lightList.link.next;
    RwLLLink* end = &clump->lightList.link;

    while (cur != end)
    {
        RpLight* light = (RpLight*)OBJECTFROMCLUMPLINK(cur, _rpClumpLightExtOffset);
        RwLLLink* next = cur->next;

        if (!callback(light, pData))
        {
            return clump;
        }

        cur = next;
    }

    return clump;
}

RpAtomic* RpAtomicCreate(void)
{
    RpAtomic* atomic = (RpAtomic*)RWSRCGLOBAL(memoryAlloc)(CLUMP_DATA(atomicFreeList));

    if (!atomic)
    {
        return NULL;
    }

    rwObjectInitialize(atomic, ATOMIC_OBJECTTYPE, 0);
    atomic->object.sync = AtomicSync;
    atomic->repEntry = NULL;

    rwObjectSetFlags(atomic, rpATOMICCOLLISIONTEST | rpATOMICRENDER);
    atomic->object.object.privateFlags = 1;

    RpAtomicSetFrame(atomic, NULL);

    atomic->geometry = NULL;

    atomic->boundingSphere.radius = 0.0f;
    atomic->boundingSphere.center.x = 0.0f;
    atomic->boundingSphere.center.y = 0.0f;
    atomic->boundingSphere.center.z = 0.0f;

    atomic->worldBoundingSphere.radius = 0.0f;
    atomic->worldBoundingSphere.center.x = 0.0f;
    atomic->worldBoundingSphere.center.y = 0.0f;
    atomic->worldBoundingSphere.center.z = 0.0f;

    atomic->renderCallBack = AtomicDefaultRenderCallBack;
    if (!atomic->renderCallBack)
    {
        atomic->renderCallBack = AtomicDefaultRenderCallBack;
    }

    atomic->interpolator.startMorphTarget = 0;
    atomic->interpolator.endMorphTarget = 0;
    atomic->interpolator.time = 1.0f;
    atomic->interpolator.recipTime = 1.0f;
    atomic->interpolator.position = 0.0f;
    atomic->interpolator.flags = rpINTERPOLATORDIRTYINSTANCE | rpINTERPOLATORDIRTYSPHERE;

    atomic->inClumpLink.prev = NULL;
    atomic->inClumpLink.next = NULL;
    atomic->clump = NULL;
    atomic->pipeline = NULL;

    rwLinkListInitialize(&atomic->llWorldSectorsInAtomic);

    _rwPluginRegistryInitObject(&atomicTKList, atomic);

    return atomic;
}

RpAtomic* RpAtomicSetGeometry(RpAtomic* atomic, RpGeometry* geometry, U32 flags)
{
    if (geometry != atomic->geometry)
    {
        if (geometry)
        {
            _rpGeometryAddRef(geometry);
        }

        if (atomic->geometry)
        {
            RpGeometryDestroy(atomic->geometry);
        }

        atomic->geometry = geometry;

        if (!(flags & 1))
        {
            RwFrame* frame;

            if (geometry)
            {
                atomic->boundingSphere = geometry->morphTarget[0].boundingSphere;
            }

            frame = RpAtomicGetFrame(atomic);

            if (frame && RpAtomicGetWorld(atomic))
            {
                RwFrameUpdateObjects(frame);
            }
        }
    }

    return atomic;
}

S32 RpAtomicDestroy(RpAtomic* atomic)
{
    _rwPluginRegistryDeInitObject(&atomicTKList, atomic);

    if (atomic->repEntry)
    {
        RwResourcesFreeResEntry(atomic->repEntry);
    }

    RpAtomicSetGeometry(atomic, NULL, 0);

    _rwObjectHasFrameReleaseFrame(atomic);

    RWSRCGLOBAL(memoryFree)(CLUMP_DATA(atomicFreeList), atomic);

    return 1;
}

static RpAtomic* DestroyClumpAtomic(RpAtomic* atomic, void* data)
{
    RpAtomicDestroy(atomic);

    return atomic;
}

static RpLight* DestroyClumpLight(RpLight* light, void* data)
{
    RpClumpRemoveLight(CLUMPEXTFROMOBJECT(light, _rpClumpLightExtOffset)->clump, light);
    RpLightDestroy(light);

    return light;
}

static RwCamera* DestroyClumpCamera(RwCamera* camera, void* data)
{
    RpClumpRemoveCamera(CLUMPEXTFROMOBJECT(camera, _rpClumpCameraExtOffset)->clump, camera);
    RwCameraDestroy(camera);

    return camera;
}

S32 RpClumpDestroy(RpClump* clump)
{
    RwFrame* frame;

    _rwPluginRegistryDeInitObject(&clumpTKList, clump);

    RpClumpForAllAtomics(clump, DestroyClumpAtomic, NULL);
    RpClumpForAllLights(clump, DestroyClumpLight, NULL);
    RpClumpForAllCameras(clump, DestroyClumpCamera, NULL);

    frame = RpClumpGetFrame(clump);

    if (frame)
    {
        RwFrameDestroyHierarchy(frame);
    }

    RWSRCGLOBAL(memoryFree)(CLUMP_DATA(clumpFreeList), clump);

    return 1;
}

RpClump* RpClumpCreate(void)
{
    RpClump* clump = (RpClump*)RWSRCGLOBAL(memoryAlloc)(CLUMP_DATA(clumpFreeList));

    if (!clump)
    {
        return NULL;
    }

    rwObjectInitialize(clump, CLUMP_OBJECTTYPE, 0);
    RpClumpSetFrame(clump, NULL);

    rwLinkListInitialize(&clump->atomicList);
    rwLinkListInitialize(&clump->lightList);
    rwLinkListInitialize(&clump->cameraList);

    clump->inWorldLink.prev = NULL;
    clump->inWorldLink.next = NULL;

    clump->callback = ClumpCallBack;

    _rwPluginRegistryInitObject(&clumpTKList, clump);

    return clump;
}

RpClump* RpClumpAddAtomic(RpClump* clump, RpAtomic* atomic)
{
    atomic->inClumpLink.next = clump->atomicList.link.next;
    atomic->inClumpLink.prev = &clump->atomicList.link;
    clump->atomicList.link.next->prev = &atomic->inClumpLink;
    clump->atomicList.link.next = &atomic->inClumpLink;
    atomic->clump = clump;

    return clump;
}

RpClump* RpClumpRemoveAtomic(RpClump* clump, RpAtomic* atomic)
{
    atomic->inClumpLink.prev->next = atomic->inClumpLink.next;
    atomic->inClumpLink.next->prev = atomic->inClumpLink.prev;
    atomic->clump = NULL;

    return clump;
}

RpClump* RpClumpRemoveLight(RpClump* clump, RpLight* light)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(light, _rpClumpLightExtOffset);

    ext->inClumpLink.prev->next = ext->inClumpLink.next;
    ext->inClumpLink.next->prev = ext->inClumpLink.prev;
    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return clump;
}

RpClump* RpClumpRemoveCamera(RpClump* clump, RwCamera* camera)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(camera, _rpClumpCameraExtOffset);

    ext->inClumpLink.prev->next = ext->inClumpLink.next;
    ext->inClumpLink.next->prev = ext->inClumpLink.prev;
    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return clump;
}

RpClump* RpClumpStreamRead(RwStream* stream)
{
    U32 size;
    U32 version;
    ClumpStreamData data;
    RpClump* clump;
    rwFrameList frameList;
    GeometryList geomList;
    S32 status;
    S32 i;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x8000001A);
        RwErrorSet(&err);

        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        if (version <= CLUMP_ATOMICSONLYVERSION)
        {
            S32 numAtomics;

            status = (RwStreamRead(stream, &numAtomics, sizeof(numAtomics)) == sizeof(numAtomics));

            if (!status)
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);

                return NULL;
            }

            RwMemNative32(&numAtomics, sizeof(numAtomics));

            data.numLights = 0;
            data.numAtomics = numAtomics;
            data.numCameras = 0;
        }
        else
        {
            status = (RwStreamRead(stream, &data, sizeof(data)) == sizeof(data));

            if (!status)
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);

                return NULL;
            }

            RwMemNative32(&data, sizeof(data));
        }

        clump = RpClumpCreate();

        if (!clump)
        {
            return NULL;
        }

        if (!RwStreamFindChunk(stream, rwID_FRAMELIST, NULL, &version))
        {
            RpClumpDestroy(clump);
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);
            }

            return NULL;
        }

        status = (_rwFrameListStreamRead(stream, &frameList) != NULL);

        if (!status)
        {
            RpClumpDestroy(clump);
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);
            }

            return NULL;
        }

        if (!RwStreamFindChunk(stream, rwID_GEOMETRYLIST, NULL, &version))
        {
            RwError err;

            err.pluginID = 2;
            err.errorCode = _rwerror(0x8000001A);
            RwErrorSet(&err);

            return NULL;
        }

        status = (GeometryListStreamRead(stream, &geomList) != NULL);

        if (!status)
        {
            _rwFrameListDeinitialize(&frameList);
            RpClumpDestroy(clump);
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);
            }

            return NULL;
        }

        RpClumpSetFrame(clump, frameList.frames[0]);

        for (i = 0; i < data.numAtomics; i++)
        {
            RpAtomic* atomic;

            status = RwStreamFindChunk(stream, rwID_ATOMIC, NULL, &version);

            if (status)
            {
                atomic = ClumpAtomicStreamRead(stream, &frameList, &geomList);
                status = (atomic != NULL);
            }

            if (!status)
            {
                GEOMETRYLIST_DEINITIALIZE(geomList);
                _rwFrameListDeinitialize(&frameList);
                RpClumpDestroy(clump);
                {
                    RwError err;

                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x8000001A);
                    RwErrorSet(&err);
                }

                return NULL;
            }

            RpClumpAddAtomic(clump, atomic);
        }

        for (i = 0; i < data.numLights; i++)
        {
            S32 frameIndex;
            RpLight* light;
            ClumpObjectExt* ext;

            status = RwStreamFindChunk(stream, rwID_STRUCT, NULL, NULL) &&
                     RwStreamReadInt32(stream, &frameIndex, sizeof(S32));

            if (status)
            {
                status = RwStreamFindChunk(stream, rwID_LIGHT, NULL, NULL);
            }

            if (status)
            {
                light = RpLightStreamRead(stream);
                status = (light != NULL);
            }

            if (!status)
            {
                GEOMETRYLIST_DEINITIALIZE(geomList);
                _rwFrameListDeinitialize(&frameList);
                RpClumpDestroy(clump);
                {
                    RwError err;

                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x8000001A);
                    RwErrorSet(&err);
                }

                return NULL;
            }

            RpLightSetFrame(light, frameList.frames[frameIndex]);

            ext = CLUMPEXTFROMOBJECT(light, _rpClumpLightExtOffset);
            rwLinkListAddLLLink(&clump->lightList, &ext->inClumpLink);
            ext->clump = clump;
        }

        for (i = 0; i < data.numCameras; i++)
        {
            S32 frameIndex;
            RwCamera* camera;
            ClumpObjectExt* ext;

            status = RwStreamFindChunk(stream, rwID_STRUCT, NULL, NULL) &&
                     RwStreamReadInt32(stream, &frameIndex, sizeof(S32));

            if (status)
            {
                status = RwStreamFindChunk(stream, rwID_CAMERA, NULL, NULL);
            }

            if (status)
            {
                camera = RwCameraStreamRead(stream);
                status = (camera != NULL);
            }

            if (!status)
            {
                GEOMETRYLIST_DEINITIALIZE(geomList);
                _rwFrameListDeinitialize(&frameList);
                RpClumpDestroy(clump);
                {
                    RwError err;

                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x8000001A);
                    RwErrorSet(&err);
                }

                return NULL;
            }

            RwCameraSetFrame(camera, frameList.frames[frameIndex]);

            ext = CLUMPEXTFROMOBJECT(camera, _rpClumpCameraExtOffset);
            rwLinkListAddLLLink(&clump->cameraList, &ext->inClumpLink);
            ext->clump = clump;
        }

        GEOMETRYLIST_DEINITIALIZE(geomList);
        _rwFrameListDeinitialize(&frameList);

        status = (_rwPluginRegistryReadDataChunks(&clumpTKList, stream, clump) != NULL);

        if (!status)
        {
            RpClumpDestroy(clump);
            {
                RwError err;

                err.pluginID = 2;
                err.errorCode = _rwerror(0x8000001A);
                RwErrorSet(&err);
            }

            return NULL;
        }

        return clump;
    }
    else
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);

        return NULL;
    }
}

S32 RpAtomicRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&atomicTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpClumpRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&clumpTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpAtomicRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                 RwPluginDataChunkWriteCallBack writeCB,
                                 RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&atomicTKList, pluginID, readCB, writeCB, getSizeCB);
}

S32 RpAtomicSetStreamAlwaysCallBack(U32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    return _rwPluginRegistryAddPlgnStrmlwysCB(&atomicTKList, pluginID, alwaysCB);
}

S32 RpAtomicSetStreamRightsCallBack(U32 pluginID, RwPluginDataChunkRightsCallBack rightsCB)
{
    return _rwPluginRegistryAddPlgnStrmRightsCB(&atomicTKList, pluginID, rightsCB);
}

S32 RpAtomicGetPluginOffset(U32 pluginID)
{
    return _rwPluginRegistryGetPluginOffset(&atomicTKList, pluginID);
}

RpAtomic* RpAtomicSetFrame(RpAtomic* atomic, RwFrame* frame)
{
    _rwObjectHasFrameSetFrame(atomic, frame);
    atomic->object.object.privateFlags |= 1;

    return atomic;
}
