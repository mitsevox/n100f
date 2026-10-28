#include <rpworld.h>

// The extension the world plugin adds to cameras: the sectors in the camera's frustum, the world
// and the camera callbacks the plugin replaces.
typedef struct WorldCameraExt WorldCameraExt;
struct WorldCameraExt
{
    RpWorldSector** sectors;
    U32 unk4;
    S32 numSectors;
    RpWorld* world;
    RwCamera* (*beginUpdate)(RwCamera*);
    RwCamera* (*endUpdate)(RwCamera*);
    RwObjectHasFrame* (*sync)(RwObjectHasFrame*);
};

// The extension the world plugin adds to atomics and lights: the world and the saved sync.
typedef struct WorldObjectExt WorldObjectExt;
struct WorldObjectExt
{
    RpWorld* world;
    RwObjectHasFrame* (*sync)(RwObjectHasFrame*);
};

#define WORLDEXTFROMOBJECT(type, object, offset) ((type*)((U8*)(object) + (offset)))

// Links an atomic into a sector it touches: one link in the sector's atomic list, one in the
// atomic's sector list.
typedef struct WorldTie WorldTie;
struct WorldTie
{
    RwLLLink sectorLink;
    RpAtomic* atomic;
    RwLLLink atomicLink;
    RpWorldSector* sector;
};

// Links a light into a sector it reaches, the same way.
typedef struct LightTie LightTie;
struct LightTie
{
    RwLLLink sectorLink;
    RpLight* light;
    RwLLLink lightLink;
    RpWorldSector* sector;
};

// The world object module's data in the engine instance.
typedef struct WorldObjModuleData WorldObjModuleData;
struct WorldObjModuleData
{
    RwFreeList* tieFreeList;
    RwFreeList* lightTieFreeList;
};

#define WORLDOBJ_DATA(var) \
    (((WorldObjModuleData*)((U8*)RwEngineInstance + worldObjModule.globalsOffset))->var)

#define TIEFROMATOMICLINK(link) ((WorldTie*)((U8*)(link) - (S32)&((WorldTie*)0)->atomicLink))
#define TIEFROMLIGHTLINK(link) ((LightTie*)((U8*)(link) - (S32)&((LightTie*)0)->lightLink))

#define UNLINK(lnk) ((lnk).prev->next = (lnk).next, (lnk).next->prev = (lnk).prev)

#define ADDLINK(list, lnk)                                         \
    ((lnk).next = (list).link.next, (lnk).prev = &(list).link,     \
     (list).link.next->prev = &(lnk), (list).link.next = &(lnk))

// Light subtypes from this value on are positioned by a frame; the ones below it are not.
#define LIGHT_POSITIONED_FIRST 0x80

static S32 cameraExtOffset;
static S32 atomicExtOffset;
static S32 clumpExtOffset;
static S32 lightExtOffset;
static RwModuleInfo worldObjModule;

RwObjectHasFrame* WorldCameraSync(RwObjectHasFrame* object);
RwObjectHasFrame* WorldAtomicSync(RwObjectHasFrame* object);
RwObjectHasFrame* WorldLightSync(RwObjectHasFrame* object);

RwStream* _rpMeshWrite(const RpMeshHeader* meshHeader, const void* object, RwStream* stream,
                       const RpMaterialList* matList);
RpMeshHeader* _rpMeshRead(RwStream* stream, const void* object, const RpMaterialList* matList);
S32 _rpMeshSize(const RpMeshHeader* meshHeader, const void* object);

S32 _rpGeometryNativeSize(const RpGeometry* geometry);
RwStream* _rpGeometryNativeWrite(RwStream* stream, const RpGeometry* geometry);
RwStream* _rpGeometryNativeRead(RwStream* stream, RpGeometry* geometry);
S32 _rpWorldSectorNativeSize(const RpWorldSector* sector);
RwStream* _rpWorldSectorNativeWrite(RwStream* stream, const RpWorldSector* sector);
RwStream* _rpWorldSectorNativeRead(RwStream* stream, RpWorldSector* sector);

RpWorld* RpWorldSectorGetWorld(const RpWorldSector* sector);
RpWorld* RpWorldAddAtomic(RpWorld* world, RpAtomic* atomic);
RpWorld* RpWorldAddClump(RpWorld* world, RpClump* clump);
RpWorld* RpWorldAddLight(RpWorld* world, RpLight* light);

static void* WorldObjectClose(void* instance, S32 offset, S32 size)
{
    if (WORLDOBJ_DATA(lightTieFreeList))
    {
        RwFreeListDestroy(WORLDOBJ_DATA(lightTieFreeList));
        WORLDOBJ_DATA(lightTieFreeList) = NULL;
    }

    if (WORLDOBJ_DATA(tieFreeList))
    {
        RwFreeListDestroy(WORLDOBJ_DATA(tieFreeList));
        WORLDOBJ_DATA(tieFreeList) = NULL;
    }

    worldObjModule.numInstances--;

    return instance;
}

static RwCamera* WorldCameraBeginUpdate(RwCamera* camera)
{
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);

    RWSRCGLOBAL(curWorld) = ext->world;
    RWSRCGLOBAL(renderFrame)++;

    return ext->beginUpdate(camera);
}

static RwCamera* WorldCameraEndUpdate(RwCamera* camera)
{
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);

    RWSRCGLOBAL(curWorld) = NULL;

    return ext->endUpdate(camera);
}

static void* WorldInitCameraExt(void* object, S32 offset, S32 size)
{
    RwCamera* camera = (RwCamera*)object;
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);

    ext->sectors = NULL;
    ext->unk4 = 0;
    ext->numSectors = 0;

    ext->beginUpdate = camera->beginUpdate;
    ext->endUpdate = camera->endUpdate;
    ext->sync = camera->object.sync;

    camera->object.sync = WorldCameraSync;
    camera->beginUpdate = WorldCameraBeginUpdate;
    camera->endUpdate = WorldCameraEndUpdate;

    ext->world = NULL;

    return object;
}

static void* WorldCopyCameraExt(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    WorldCameraExt* dstExt = WORLDEXTFROMOBJECT(WorldCameraExt, dstObject, cameraExtOffset);
    const WorldCameraExt* srcExt = WORLDEXTFROMOBJECT(WorldCameraExt, srcObject, cameraExtOffset);

    dstExt->sectors = NULL;
    dstExt->unk4 = 0;
    dstExt->numSectors = 0;

    if (srcExt->world)
    {
        RpWorldAddCamera(srcExt->world, (RwCamera*)dstObject);
    }

    return dstObject;
}

static void* WorldDeInitCameraExt(void* object, S32 offset, S32 size)
{
    RwCamera* camera = (RwCamera*)object;
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);

    if (ext->sectors)
    {
        RWSRCGLOBAL(memoryFuncs).rwfree(ext->sectors);
    }

    ext->sectors = NULL;
    ext->unk4 = 0;
    ext->numSectors = 0;

    camera->beginUpdate = ext->beginUpdate;
    camera->endUpdate = ext->endUpdate;
    camera->object.sync = ext->sync;

    return object;
}

S32 _rpLightTieDestroy(LightTie* tie)
{
    UNLINK(tie->lightLink);
    UNLINK(tie->sectorLink);

    RWSRCGLOBAL(memoryFree)(WORLDOBJ_DATA(lightTieFreeList), tie);

    return 1;
}

S32 _rpTieDestroy(WorldTie* tie)
{
    if (tie->atomic && tie->sector)
    {
        UNLINK(tie->atomicLink);
        UNLINK(tie->sectorLink);

        RWSRCGLOBAL(memoryFree)(WORLDOBJ_DATA(tieFreeList), tie);
    }

    return 1;
}

static void* WorldInitAtomicExt(void* object, S32 offset, S32 size)
{
    RpAtomic* atomic = (RpAtomic*)object;
    WorldObjectExt* ext = WORLDEXTFROMOBJECT(WorldObjectExt, atomic, atomicExtOffset);

    ext->world = NULL;
    atomic->renderFrame = RWSRCGLOBAL(renderFrame) - 1;

    ext->sync = atomic->object.sync;
    atomic->object.sync = WorldAtomicSync;

    return object;
}

static void* WorldCopyAtomicExt(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    return dstObject;
}

static void* WorldDeInitAtomicExt(void* object, S32 offset, S32 size)
{
    RpAtomic* atomic = (RpAtomic*)object;
    RwLLLink* end = &atomic->llWorldSectorsInAtomic.link;
    RwLLLink* cur = atomic->llWorldSectorsInAtomic.link.next;
    WorldObjectExt* ext = WORLDEXTFROMOBJECT(WorldObjectExt, atomic, atomicExtOffset);

    while (cur != end)
    {
        WorldTie* tie = TIEFROMATOMICLINK(cur);

        cur = cur->next;
        _rpTieDestroy(tie);
    }

    atomic->object.sync = ext->sync;

    return object;
}

static void* WorldInitClumpExt(void* object, S32 offset, S32 size)
{
    *WORLDEXTFROMOBJECT(RpWorld*, object, clumpExtOffset) = NULL;

    return object;
}

static void* WorldCopyClumpExt(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    RpWorld* world = *WORLDEXTFROMOBJECT(RpWorld*, srcObject, clumpExtOffset);

    if (world)
    {
        RpWorldAddClump(world, (RpClump*)dstObject);
    }

    return dstObject;
}

static void* WorldDeInitClumpExt(void* object, S32 offset, S32 size)
{
    return object;
}

static void* WorldInitLightExt(void* object, S32 offset, S32 size)
{
    RpLight* light = (RpLight*)object;
    WorldObjectExt* ext = WORLDEXTFROMOBJECT(WorldObjectExt, light, lightExtOffset);

    ext->world = NULL;
    ext->sync = light->object.sync;
    light->object.sync = WorldLightSync;

    return object;
}

static void* WorldCopyLightExt(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    const WorldObjectExt* srcExt = WORLDEXTFROMOBJECT(WorldObjectExt, srcObject, lightExtOffset);

    if (srcExt->world)
    {
        RpWorldAddLight(srcExt->world, (RpLight*)dstObject);
    }

    return dstObject;
}

static void* WorldDeInitLightExt(void* object, S32 offset, S32 size)
{
    RpLight* light = (RpLight*)object;
    RwLLLink* end = &light->WorldSectorsInLight.link;
    RwLLLink* cur = light->WorldSectorsInLight.link.next;

    while (cur != end)
    {
        LightTie* tie = TIEFROMLIGHTLINK(cur);

        cur = cur->next;
        _rpLightTieDestroy(tie);
    }

    return object;
}

static RpAtomic* WorldAddClumpAtomic(RpAtomic* atomic, void* data)
{
    RpWorldAddAtomic((RpWorld*)data, atomic);

    return atomic;
}

static RpLight* WorldAddClumpLight(RpLight* light, void* data)
{
    RpWorldAddLight((RpWorld*)data, light);

    return light;
}

static RwCamera* WorldAddClumpCamera(RwCamera* camera, void* data)
{
    RpWorldAddCamera((RpWorld*)data, camera);

    return camera;
}

static RwStream* writeGeometryMesh(RwStream* stream, S32 binaryLength, const void* object,
                                   S32 offsetInObject, S32 sizeInObject)
{
    const RpGeometry* geometry = (const RpGeometry*)object;

    return _rpMeshWrite(geometry->mesh, geometry, stream, &geometry->matList);
}

static RwStream* readGeometryMesh(RwStream* stream, S32 binaryLength, void* object,
                                  S32 offsetInObject, S32 sizeInObject)
{
    RpGeometry* geometry = (RpGeometry*)object;

    geometry->mesh = _rpMeshRead(stream, geometry, &geometry->matList);
    if (geometry->mesh)
    {
        return stream;
    }

    return NULL;
}

static S32 sizeGeometryMesh(const void* object, S32 offsetInObject, S32 sizeInObject)
{
    const RpGeometry* geometry = (const RpGeometry*)object;

    return _rpMeshSize(geometry->mesh, geometry);
}

static RwStream* writeGeometryNative(RwStream* stream, S32 binaryLength, const void* object,
                                     S32 offsetInObject, S32 sizeInObject)
{
    return _rpGeometryNativeWrite(stream, (const RpGeometry*)object);
}

static RwStream* readGeometryNative(RwStream* stream, S32 binaryLength, void* object,
                                    S32 offsetInObject, S32 sizeInObject)
{
    if (_rpGeometryNativeRead(stream, (RpGeometry*)object))
    {
        return stream;
    }

    return NULL;
}

static S32 sizeGeometryNative(const void* object, S32 offsetInObject, S32 sizeInObject)
{
    return _rpGeometryNativeSize((const RpGeometry*)object);
}

static RwStream* writeWorldSectorNative(RwStream* stream, S32 binaryLength, const void* object,
                                        S32 offsetInObject, S32 sizeInObject)
{
    return _rpWorldSectorNativeWrite(stream, (const RpWorldSector*)object);
}

static RwStream* readWorldSectorNative(RwStream* stream, S32 binaryLength, void* object,
                                       S32 offsetInObject, S32 sizeInObject)
{
    if (_rpWorldSectorNativeRead(stream, (RpWorldSector*)object))
    {
        return stream;
    }

    return NULL;
}

static S32 sizeWorldSectorNative(const void* object, S32 offsetInObject, S32 sizeInObject)
{
    return _rpWorldSectorNativeSize((const RpWorldSector*)object);
}

static RwStream* writeSectorMesh(RwStream* stream, S32 binaryLength, const void* object,
                                 S32 offsetInObject, S32 sizeInObject)
{
    const RpWorldSector* sector = (const RpWorldSector*)object;
    const RpWorld* world = RpWorldSectorGetWorld(sector);

    return _rpMeshWrite(sector->mesh, world, stream, &world->matList);
}

static RwStream* readSectorMesh(RwStream* stream, S32 binaryLength, void* object,
                                S32 offsetInObject, S32 sizeInObject)
{
    RpWorldSector* sector = (RpWorldSector*)object;
    const RpWorld* world = RpWorldSectorGetWorld(sector);

    sector->mesh = _rpMeshRead(stream, world, &world->matList);
    if (sector->mesh)
    {
        return stream;
    }

    return NULL;
}

static S32 sizeSectorMesh(const void* object, S32 offsetInObject, S32 sizeInObject)
{
    const RpWorldSector* sector = (const RpWorldSector*)object;

    return _rpMeshSize(sector->mesh, RpWorldSectorGetWorld(sector));
}

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera)
{
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);
    RwFrame* frame = RwCameraGetFrame(camera);

    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    ext->world = world;

    return world;
}

RpWorld* RpWorldRemoveCamera(RpWorld* world, RwCamera* camera)
{
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);

    if (ext->world)
    {
        if (ext->sectors)
        {
            RWSRCGLOBAL(memoryFuncs).rwfree(ext->sectors);
        }

        ext->sectors = NULL;
        ext->unk4 = 0;
        ext->numSectors = 0;
        ext->world = NULL;

        return world;
    }

    return NULL;
}

RpWorld* RpWorldAddAtomic(RpWorld* world, RpAtomic* atomic)
{
    WorldObjectExt* ext = WORLDEXTFROMOBJECT(WorldObjectExt, atomic, atomicExtOffset);
    RwFrame* frame = RpAtomicGetFrame(atomic);

    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    ext->world = world;

    return world;
}

RpWorld* RpAtomicGetWorld(const RpAtomic* atomic)
{
    return WORLDEXTFROMOBJECT(WorldObjectExt, atomic, atomicExtOffset)->world;
}

RpAtomic* RpAtomicForAllWorldSectors(RpAtomic* atomic, RpWorldSectorCallBack callback,
                                     void* pData)
{
    RwLLLink* cur = atomic->llWorldSectorsInAtomic.link.next;
    RwLLLink* end = &atomic->llWorldSectorsInAtomic.link;

    while (cur != end)
    {
        RwLLLink* next = cur->next;
        WorldTie* tie = TIEFROMATOMICLINK(cur);

        if (!callback(tie->sector, pData))
        {
            return atomic;
        }

        cur = next;
    }

    return atomic;
}

RwCamera* RwCameraForAllSectorsInFrustum(RwCamera* camera, RpWorldSectorCallBack callBack,
                                         void* pData)
{
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);
    RpWorldSector** sectors = ext->sectors;
    S32 count = ext->numSectors;

    while (count)
    {
        if (!callBack(*sectors, pData))
        {
            return camera;
        }

        sectors++;
        count--;
    }

    return camera;
}

RpWorld* RpWorldAddLight(RpWorld* world, RpLight* light)
{
    *WORLDEXTFROMOBJECT(RpWorld*, light, lightExtOffset) = world;

    if (light->object.object.subType < LIGHT_POSITIONED_FIRST)
    {
        ADDLINK(world->directionalLightList, light->inWorld);
    }
    else
    {
        RwFrame* frame = (RwFrame*)rwObjectGetParent(light);

        if (frame)
        {
            RwFrameUpdateObjects(frame);
        }

        ADDLINK(world->lightList, light->inWorld);
    }

    return world;
}

RpWorld* RpWorldRemoveLight(RpWorld* world, RpLight* light)
{
    RwLLLink* end;
    RwLLLink* cur;

    *WORLDEXTFROMOBJECT(RpWorld*, light, lightExtOffset) = NULL;

    end = &light->WorldSectorsInLight.link;
    cur = light->WorldSectorsInLight.link.next;

    while (cur != end)
    {
        LightTie* tie = TIEFROMLIGHTLINK(cur);

        cur = cur->next;
        _rpLightTieDestroy(tie);
    }

    UNLINK(light->inWorld);

    return world;
}
