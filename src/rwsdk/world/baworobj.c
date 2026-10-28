#include <rpworld.h>

// The extension the world plugin adds to cameras: the sectors in the camera's frustum, the world
// and the camera callbacks the plugin replaces.
typedef struct WorldCameraExt WorldCameraExt;
struct WorldCameraExt
{
    RpWorldSector** sectors;
    S32 maxSectors;
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

// The standard device function a camera's world tells whether it renders front to back.
#define STDFUNC_RENDERORDER 22

// The plugin IDs this unit registers under: the world object extensions, the mesh streams and
// the native data streams.
#define WORLDOBJ_PLUGINID 0x509
#define MESH_PLUGINID 0x50E
#define NATIVE_PLUGINID 0x510

// How many entries a camera's sector list grows by when it fills.
#define SECTORLIST_GROWBY 50

// The depth of the stack the sector tree walks keep their pending subtrees on.
#define SECTOR_STACK_SIZE 64

// A plane sector's type is the byte offset of the coordinate it splits on.
#define SECTORCOORD(vec, type) (*(F32*)((U8*)&(vec) + (type)))

static RwModuleInfo worldObjModule;
static S32 lightExtOffset;
static S32 clumpExtOffset;
static S32 atomicExtOffset;
static S32 cameraExtOffset;

RwObjectHasFrame* WorldCameraSync(RwObjectHasFrame* object);
RwObjectHasFrame* WorldAtomicSync(RwObjectHasFrame* object);
RwObjectHasFrame* WorldLightSync(RwObjectHasFrame* object);
void WorldAttachAtomicSphere(RpWorld* world, RpAtomic* atomic);
RpWorld* WorldSyncCamera(RpWorld* world, RwCamera* camera);

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

RwStream* _rpReadAtomicRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size);
RwStream* _rpWriteAtomicRights(RwStream* stream, S32 length, const void* object, S32 offset,
                               S32 size);
S32 _rpSizeAtomicRights(const void* object, S32 offset, S32 size);
RwStream* _rpReadWorldRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size);
RwStream* _rpWriteWorldRights(RwStream* stream, S32 length, const void* object, S32 offset,
                              S32 size);
S32 _rpSizeWorldRights(const void* object, S32 offset, S32 size);
RwStream* _rpReadSectRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size);
RwStream* _rpWriteSectRights(RwStream* stream, S32 length, const void* object, S32 offset,
                             S32 size);
S32 _rpSizeSectRights(const void* object, S32 offset, S32 size);
S32 _rpWorldPipeAttach(void);

RpWorld* RpWorldSectorGetWorld(const RpWorldSector* sector);
RpWorld* RpWorldAddAtomic(RpWorld* world, RpAtomic* atomic);
RpWorld* RpWorldAddClump(RpWorld* world, RpClump* clump);
RpWorld* RpWorldAddLight(RpWorld* world, RpLight* light);

static void* WorldObjectOpen(void* instance, S32 offset, S32 size)
{
    worldObjModule.globalsOffset = offset;

    WORLDOBJ_DATA(tieFreeList) = RwFreeListCreate(sizeof(WorldTie), 100, 0);
    if (!WORLDOBJ_DATA(tieFreeList))
    {
        return NULL;
    }

    WORLDOBJ_DATA(lightTieFreeList) = RwFreeListCreate(sizeof(LightTie), 20, 0);
    if (!WORLDOBJ_DATA(lightTieFreeList))
    {
        RwFreeListDestroy(WORLDOBJ_DATA(tieFreeList));
        WORLDOBJ_DATA(tieFreeList) = NULL;
        return NULL;
    }

    RWSRCGLOBAL(renderFrame) = 1;

    worldObjModule.numInstances++;

    return instance;
}

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

RwObjectHasFrame* WorldCameraSync(RwObjectHasFrame* object)
{
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, object, cameraExtOffset);

    if (ext->sync(object))
    {
        RpWorld* world = ext->world;

        if (world)
        {
            RwStandardFunc renderOrderFunc = RWSRCGLOBAL(stdFunc)[STDFUNC_RENDERORDER];

            WorldSyncCamera(world, (RwCamera*)object);
            renderOrderFunc(NULL, NULL, world->renderOrder == rpWORLDRENDERFRONT2BACK);
        }

        return object;
    }

    return NULL;
}

static void* WorldInitCameraExt(void* object, S32 offset, S32 size)
{
    RwCamera* camera = (RwCamera*)object;
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);

    ext->sectors = NULL;
    ext->maxSectors = 0;
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
    dstExt->maxSectors = 0;
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
    ext->maxSectors = 0;
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

void WorldAttachAtomicSphere(RpWorld* world, RpAtomic* atomic)
{
    RpSector* stack[SECTOR_STACK_SIZE];
    S32 nStack = 0;
    RpSector* sector;
    RwV3d inf;
    RwV3d sup;
    const RwSphere* sphere = RpAtomicGetWorldBoundingSphere(atomic);

    inf = sphere->center;
    sup = sphere->center;

    inf.x -= sphere->radius;
    inf.y -= sphere->radius;
    inf.z -= sphere->radius;
    sup.x += sphere->radius;
    sup.y += sphere->radius;
    sup.z += sphere->radius;

    sector = world->rootSector;

    do
    {
        if (sector->type < 0)
        {
            WorldTie* tie = (WorldTie*)RWSRCGLOBAL(memoryAlloc)(WORLDOBJ_DATA(tieFreeList));

            tie->sector = (RpWorldSector*)sector;
            tie->atomic = atomic;

            if (rwObjectTestFlags(atomic, rpATOMICCOLLISIONTEST))
            {
                ADDLINK(((RpWorldSector*)sector)->collAtomicsInWorldSector, tie->sectorLink);
            }
            else
            {
                ADDLINK(((RpWorldSector*)sector)->noCollAtomicsInWorldSector, tie->sectorLink);
            }

            ADDLINK(atomic->llWorldSectorsInAtomic, tie->atomicLink);

            sector = stack[nStack--];
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            if (SECTORCOORD(inf, plane->type) < plane->leftValue)
            {
                sector = plane->leftSubTree;

                if (plane->rightValue < SECTORCOORD(sup, plane->type))
                {
                    stack[++nStack] = plane->rightSubTree;
                }
            }
            else if (plane->rightValue < SECTORCOORD(sup, plane->type))
            {
                sector = plane->rightSubTree;
            }
            else
            {
                sector = stack[nStack--];
            }
        }
    } while (nStack >= 0);
}

RwObjectHasFrame* WorldAtomicSync(RwObjectHasFrame* object)
{
    RpAtomic* atomic = (RpAtomic*)object;
    RwLLLink* end;
    RwLLLink* cur;
    WorldObjectExt* ext = WORLDEXTFROMOBJECT(WorldObjectExt, atomic, atomicExtOffset);
    RpWorld* world;

    if (ext->sync(object))
    {
        world = ext->world;

        if (world)
        {
            cur = atomic->llWorldSectorsInAtomic.link.next;
            end = &atomic->llWorldSectorsInAtomic.link;

            while (cur != end)
            {
                WorldTie* tie = TIEFROMATOMICLINK(cur);

                cur = cur->next;
                _rpTieDestroy(tie);
            }

            WorldAttachAtomicSphere(world, atomic);
        }

        return object;
    }

    return NULL;
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

RwObjectHasFrame* WorldLightSync(RwObjectHasFrame* object)
{
    RpWorld* world;
    RwFrame* frame;
    S32 nStack;
    RpSector* sector;
    RpLight* light;
    RwLLLink* end;
    RwLLLink* cur;
    WorldObjectExt* ext;
    F32 radius;
    RpSector* stack[SECTOR_STACK_SIZE];

    light = (RpLight*)object;
    ext = WORLDEXTFROMOBJECT(WorldObjectExt, light, lightExtOffset);

    if (ext->sync(object))
    {
        if (RpLightGetType(light) < rpLIGHTPOSITIONINGSTART)
        {
            return (RwObjectHasFrame*)light;
        }

        world = ext->world;
        frame = (RwFrame*)rwObjectGetParent(light);

        if (world && frame)
        {
            RwV3d inf;
            RwV3d sup;
            const RwV3d* pos;

            radius = light->radius;
            nStack = 0;
            cur = light->WorldSectorsInLight.link.next;
            end = &light->WorldSectorsInLight.link;

            while (cur != end)
            {
                LightTie* tie = TIEFROMLIGHTLINK(cur);

                cur = cur->next;
                _rpLightTieDestroy(tie);
            }

            pos = &RwFrameGetLTM(frame)->pos;

            inf = *pos;
            sup = *pos;

            sup.x += radius;
            sup.y += radius;
            sup.z += radius;
            inf.x -= radius;
            inf.y -= radius;
            inf.z -= radius;

            sector = world->rootSector;

            do
            {
                if (sector->type < 0)
                {
                    LightTie* tie =
                        (LightTie*)RWSRCGLOBAL(memoryAlloc)(WORLDOBJ_DATA(lightTieFreeList));

                    tie->sector = (RpWorldSector*)sector;
                    tie->light = light;

                    ADDLINK(((RpWorldSector*)sector)->lightsInWorldSector, tie->sectorLink);
                    ADDLINK(light->WorldSectorsInLight, tie->lightLink);

                    sector = stack[nStack--];
                }
                else
                {
                    RpPlaneSector* plane = (RpPlaneSector*)sector;

                    if (SECTORCOORD(inf, plane->type) < plane->leftValue)
                    {
                        sector = plane->leftSubTree;

                        if (plane->rightValue < SECTORCOORD(sup, plane->type))
                        {
                            stack[++nStack] = plane->rightSubTree;
                        }
                    }
                    else if (plane->rightValue < SECTORCOORD(sup, plane->type))
                    {
                        sector = plane->rightSubTree;
                    }
                    else
                    {
                        sector = stack[nStack--];
                    }
                }
            } while (nStack >= 0);
        }
    }
    else
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000016);
        RwErrorSet(&err);
    }

    return (RwObjectHasFrame*)light;
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

RpWorld* WorldSyncCamera(RpWorld* world, RwCamera* camera)
{
    RpSector* sector;
    const RwFrustumPlane* planes;
    WorldCameraExt* ext = WORLDEXTFROMOBJECT(WorldCameraExt, camera, cameraExtOffset);
    S32 backToFront;
    S32 nStack = 0;
    S32 numSectors = 0;
    RpSector* stack[SECTOR_STACK_SIZE];
    RwV3d pos;
    RwV3d inf;
    RwV3d sup;

    pos =RwFrameGetLTM(RwCameraGetFrame(camera))->pos;
    backToFront = (world->renderOrder == rpWORLDRENDERBACK2FRONT);
    planes = camera->frustumPlanes;
    inf = camera->frustumBoundBox.inf;
    sup = camera->frustumBoundBox.sup;

    sector = ext->world->rootSector;

    do
    {
        if (sector->type < 0)
        {
            const RwFrustumPlane* plane = planes;
            S32 outside;
            S32 i;
            const RwV3d* box = (const RwV3d*)&((RpWorldSector*)sector)->boundingBox;

            for (i = 0; i < 6; i++)
            {
                RwV3d corner;
                RwSplitBits dot;

                corner.x = box[plane->closestX].x;
                corner.y = box[plane->closestY].y;
                corner.z = box[plane->closestZ].z;
                dot.nReal = corner.x * plane->plane.normal.x + corner.y * plane->plane.normal.y +
                            corner.z * plane->plane.normal.z - plane->plane.distance;

                outside = dot.nInt > 0;
                if (outside)
                {
                    break;
                }

                plane++;
            }

            if (!outside)
            {
                if (numSectors >= ext->maxSectors)
                {
                    S32 grown;
                    S32 size = (ext->maxSectors + SECTORLIST_GROWBY) * sizeof(RpWorldSector*);
                    RpWorldSector** sectors;

                    if (ext->sectors)
                    {
                        sectors = (RpWorldSector**)RWSRCGLOBAL(memoryFuncs).rwrealloc(ext->sectors,
                                                                                    size);
                    }
                    else
                    {
                        sectors = (RpWorldSector**)RWSRCGLOBAL(memoryFuncs).rwmalloc(size);
                    }

                    if (sectors)
                    {
                        ext->sectors = sectors;
                        ext->maxSectors += SECTORLIST_GROWBY;
                        grown = 1;
                    }
                    else
                    {
                        RwError err;
                        err.pluginID = 2;
                        err.errorCode = _rwerror(0x80000013, size);
                        RwErrorSet(&err);
                        grown = 0;
                    }

                    if (!grown)
                    {
                        ext->numSectors = numSectors;
                        return world;
                    }
                }

                ext->sectors[numSectors++] = (RpWorldSector*)sector;
            }

            sector = stack[nStack--];
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;
            RwSplitBits toLeft;
            RwSplitBits toRight;

            toLeft.nReal = SECTORCOORD(inf, plane->type) - plane->leftValue;
            toRight.nReal = plane->rightValue - SECTORCOORD(sup, plane->type);

            if (toLeft.nInt < 0 && toRight.nInt < 0)
            {
                S32 rightOfPlane = SECTORCOORD(pos, plane->type) > plane->value;

                if ((backToFront && rightOfPlane) || (!backToFront && !rightOfPlane))
                {
                    stack[++nStack] = plane->rightSubTree;
                    sector = plane->leftSubTree;
                }
                else
                {
                    stack[++nStack] = plane->leftSubTree;
                    sector = plane->rightSubTree;
                }
            }
            else
            {
                sector = (toLeft.nInt < 0) ? plane->leftSubTree : plane->rightSubTree;
            }
        }
    } while (nStack >= 0);

    ext->numSectors = numSectors;

    return world;
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

S32 _rpWorldObjRegisterExtensions(void)
{
    S32 result;

    result = RwEngineRegisterPlugin(sizeof(WorldObjModuleData), WORLDOBJ_PLUGINID,
                                    WorldObjectOpen, WorldObjectClose);

    cameraExtOffset = RwCameraRegisterPlugin(sizeof(WorldCameraExt), WORLDOBJ_PLUGINID,
                                             WorldInitCameraExt, WorldDeInitCameraExt,
                                             WorldCopyCameraExt);
    result |= cameraExtOffset;

    atomicExtOffset = RpAtomicRegisterPlugin(sizeof(WorldObjectExt), WORLDOBJ_PLUGINID,
                                             WorldInitAtomicExt, WorldDeInitAtomicExt,
                                             WorldCopyAtomicExt);
    result |= atomicExtOffset;

    clumpExtOffset = RpClumpRegisterPlugin(sizeof(RpWorld*), WORLDOBJ_PLUGINID, WorldInitClumpExt,
                                           WorldDeInitClumpExt, WorldCopyClumpExt);
    result |= clumpExtOffset;

    lightExtOffset = RpLightRegisterPlugin(sizeof(WorldObjectExt), WORLDOBJ_PLUGINID,
                                           WorldInitLightExt, WorldDeInitLightExt,
                                           WorldCopyLightExt);
    result |= lightExtOffset;

    result |= RpGeometryRegisterPlugin(0, MESH_PLUGINID, NULL, NULL, NULL);
    result |= RpWorldSectorRegisterPlugin(0, MESH_PLUGINID, NULL, NULL, NULL);
    result |= RpGeometryRegisterPluginStream(MESH_PLUGINID, readGeometryMesh, writeGeometryMesh,
                                             sizeGeometryMesh);
    result |= RpWorldSectorRegisterPluginStream(MESH_PLUGINID, readSectorMesh, writeSectorMesh,
                                                sizeSectorMesh);

    result |= RpGeometryRegisterPlugin(0, NATIVE_PLUGINID, NULL, NULL, NULL);
    result |= RpWorldSectorRegisterPlugin(0, NATIVE_PLUGINID, NULL, NULL, NULL);
    result |= RpGeometryRegisterPluginStream(NATIVE_PLUGINID, readGeometryNative,
                                             writeGeometryNative, sizeGeometryNative);
    result |= RpWorldSectorRegisterPluginStream(NATIVE_PLUGINID, readWorldSectorNative,
                                                writeWorldSectorNative, sizeWorldSectorNative);

    result |= RpAtomicRegisterPlugin(0, rwID_RIGHTTORENDER, NULL, NULL, NULL);
    result |= RpAtomicRegisterPluginStream(rwID_RIGHTTORENDER, _rpReadAtomicRights,
                                           _rpWriteAtomicRights, _rpSizeAtomicRights);
    result |= RpWorldRegisterPlugin(0, rwID_RIGHTTORENDER, NULL, NULL, NULL);
    result |= RpWorldRegisterPluginStream(rwID_RIGHTTORENDER, _rpReadWorldRights,
                                          _rpWriteWorldRights, _rpSizeWorldRights);
    result |= RpWorldSectorRegisterPlugin(0, rwID_RIGHTTORENDER, NULL, NULL, NULL);
    result |= RpWorldSectorRegisterPluginStream(rwID_RIGHTTORENDER, _rpReadSectRights,
                                                _rpWriteSectRights, _rpSizeSectRights);

    if (result < 0)
    {
        return 0;
    }

    return _rpWorldPipeAttach() != 0;
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
        ext->maxSectors = 0;
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

RpWorld* RpWorldAddClump(RpWorld* world, RpClump* clump)
{
    RwFrame* frame;
    RpWorld** ext = WORLDEXTFROMOBJECT(RpWorld*, clump, clumpExtOffset);

    if (!*ext)
    {
        frame = RpClumpGetFrame(clump);

        ADDLINK(world->clumpList, clump->inWorldLink);
        world->numClumpsInWorld++;
        *ext = world;

        RpClumpForAllAtomics(clump, WorldAddClumpAtomic, world);
        RpClumpForAllLights(clump, WorldAddClumpLight, world);
        RpClumpForAllCameras(clump, WorldAddClumpCamera, world);

        if (frame)
        {
            RwMatrixOptimize(&frame->modelling, NULL);
            RwFrameUpdateObjects(frame);
        }

        return world;
    }

    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(3);
        RwErrorSet(&err);
    }

    return NULL;
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

    if (light->object.object.subType < rpLIGHTPOSITIONINGSTART)
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
