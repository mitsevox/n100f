#include <rpworld.h>
#include <string.h>

// What WorldFindSector looks for and whether it found it.
typedef struct WorldSectorSearch WorldSectorSearch;
struct WorldSectorSearch
{
    RpWorldSector* sector;
    S32 found;
};

// A world in the module's list of worlds, with the size of its block.
typedef struct WorldListEntry WorldListEntry;
struct WorldListEntry
{
    RpWorld* world;
    U32 size;
    RwLLLink link;
};

// The world module's data in the engine instance: the free list of world list entries and the
// list of worlds.
typedef struct WorldModuleData WorldModuleData;
struct WorldModuleData
{
    RwFreeList* worldListFreeList;
    RwLinkList worldList;
};

#define WORLD_DATA(var) \
    (((WorldModuleData*)((U8*)RwEngineInstance + worldModule.globalsOffset))->var)

// The sector types besides planes, which are 0 and up.
#define SECTORTYPE_WORLDSECTOR -1
#define SECTORTYPE_FREED -2

// The world plugin ids of the engine modules RpWorldPluginAttach registers.
#define MATERIALMODULE_PLUGINID 0x501
#define MESHMODULE_PLUGINID 0x502
#define GEOMETRYMODULE_PLUGINID 0x503
#define CLUMPMODULE_PLUGINID 0x504
#define LIGHTMODULE_PLUGINID 0x505
#define WORLDMODULE_PLUGINID 0x507
#define SECTORMODULE_PLUGINID 0x50A
#define BINWORLDMODULE_PLUGINID 0x50B

#define SECTOR_STACK_SIZE 64

#define SECTORCOORD(vec, type) (*(F32*)((U8*)&(vec) + (type)))

// The 3-byte nodes of a collision tree, and the flag that marks a plane node.
#define COLLSECTOR_SIZE 3
#define COLLSECTOR_PLANE 0x80
#define COLLSECTOR_STACK_SIZE 18

RwPluginRegistry worldTKList = { sizeof(RpWorld), sizeof(RpWorld) };
static RwModuleInfo worldModule;
static RwSurfaceProperties defaultSurfaceProps;

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

static RpWorldSector* WorldSectorRender(RpWorldSector* worldSector, void* data)
{
    if (RpWorldSectorRender(worldSector))
    {
        RwLLLink* end;
        RwLLLink* cur;

        cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
        end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);

        while (cur != end)
        {
            RpAtomic* atomic = ((WorldTie*)cur)->atomic;

            if (rwObjectTestFlags(atomic, rpATOMICRENDER) &&
                atomic->renderFrame != RWSRCGLOBAL(renderFrame))
            {
                if (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera),
                                              RpAtomicGetWorldBoundingSphere(atomic)))
                {
                    atomic->renderCallBack(atomic);
                }

                atomic->renderFrame = RWSRCGLOBAL(renderFrame);
            }

            cur = rwLLLinkGetNext(cur);
        }

        cur = rwLinkListGetFirstLLLink(&worldSector->noCollAtomicsInWorldSector);
        end = rwLinkListGetTerminator(&worldSector->noCollAtomicsInWorldSector);

        while (cur != end)
        {
            RpAtomic* atomic = ((WorldTie*)cur)->atomic;

            if (rwObjectTestFlags(atomic, rpATOMICRENDER) &&
                atomic->renderFrame != RWSRCGLOBAL(renderFrame))
            {
                if (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera),
                                              RpAtomicGetWorldBoundingSphere(atomic)))
                {
                    atomic->renderCallBack(atomic);
                }

                atomic->renderFrame = RWSRCGLOBAL(renderFrame);
            }

            cur = rwLLLinkGetNext(cur);
        }
    }

    return worldSector;
}

static void WorldSectorDeinstanceAll(RpSector* sector)
{
    switch (sector->type)
    {
        case SECTORTYPE_WORLDSECTOR:
        {
            RpWorldSector* worldSector = (RpWorldSector*)sector;
            RwLLLink* cur;
            RwLLLink* end;

            if (worldSector->repEntry)
            {
                RwResourcesFreeResEntry(worldSector->repEntry);
            }

            cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
            end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);

            while (cur != end)
            {
                WorldTie* tie = (WorldTie*)cur;

                cur = rwLLLinkGetNext(cur);
                _rpTieDestroy(tie);
            }

            cur = rwLinkListGetFirstLLLink(&worldSector->noCollAtomicsInWorldSector);
            end = rwLinkListGetTerminator(&worldSector->noCollAtomicsInWorldSector);

            while (cur != end)
            {
                WorldTie* tie = (WorldTie*)cur;

                cur = rwLLLinkGetNext(cur);
                _rpTieDestroy(tie);
            }

            cur = rwLinkListGetFirstLLLink(&worldSector->lightsInWorldSector);
            end = rwLinkListGetTerminator(&worldSector->lightsInWorldSector);

            while (cur != end)
            {
                LightTie* tie = (LightTie*)cur;

                cur = rwLLLinkGetNext(cur);
                _rpLightTieDestroy(tie);
            }

            _rwPluginRegistryDeInitObject(&sectorTKList, worldSector);
            break;
        }
        case SECTORTYPE_FREED:
            break;
        default:
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            WorldSectorDeinstanceAll(plane->leftSubTree);
            WorldSectorDeinstanceAll(plane->rightSubTree);
            break;
        }
    }
}

static void WorldSectorDestroyRecurse(RpSector* sector)
{
    switch (sector->type)
    {
        case SECTORTYPE_WORLDSECTOR:
        {
            RpWorldSector* worldSector = (RpWorldSector*)sector;
            RwLLLink* cur;
            RwLLLink* end;
            S32 i;

            if (worldSector->repEntry)
            {
                RwResourcesFreeResEntry(worldSector->repEntry);
            }

            cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
            end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);

            while (cur != end)
            {
                WorldTie* tie = (WorldTie*)cur;

                cur = rwLLLinkGetNext(cur);
                _rpTieDestroy(tie);
            }

            cur = rwLinkListGetFirstLLLink(&worldSector->noCollAtomicsInWorldSector);
            end = rwLinkListGetTerminator(&worldSector->noCollAtomicsInWorldSector);

            while (cur != end)
            {
                WorldTie* tie = (WorldTie*)cur;

                cur = rwLLLinkGetNext(cur);
                _rpTieDestroy(tie);
            }

            cur = rwLinkListGetFirstLLLink(&worldSector->lightsInWorldSector);
            end = rwLinkListGetTerminator(&worldSector->lightsInWorldSector);

            while (cur != end)
            {
                LightTie* tie = (LightTie*)cur;

                cur = rwLLLinkGetNext(cur);
                _rpLightTieDestroy(tie);
            }

            _rwPluginRegistryDeInitObject(&sectorTKList, worldSector);

            if (worldSector->vertices)
            {
                RwFree(worldSector->vertices);
                worldSector->vertices = NULL;
            }

            if (worldSector->normals)
            {
                RwFree(worldSector->normals);
                worldSector->normals = NULL;
            }

            if (worldSector->preLitLum)
            {
                RwFree(worldSector->preLitLum);
                worldSector->preLitLum = NULL;
            }

            if (worldSector->polygons)
            {
                RwFree(worldSector->polygons);
                worldSector->polygons = NULL;
            }

            if (worldSector->colSectorRoot)
            {
                RwFree(worldSector->colSectorRoot);
                worldSector->colSectorRoot = NULL;
            }

            for (i = 0; i < 8; i++)
            {
                if (worldSector->texCoords[i])
                {
                    RwFree(worldSector->texCoords[i]);
                    worldSector->texCoords[i] = NULL;
                }
            }

            RwFree(worldSector);
            break;
        }
        case SECTORTYPE_FREED:
            RwFree(sector);
            break;
        default:
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            WorldSectorDestroyRecurse(plane->leftSubTree);
            plane->leftSubTree = NULL;
            WorldSectorDestroyRecurse(plane->rightSubTree);
            plane->rightSubTree = NULL;
            RwFree(plane);
            break;
        }
    }
}

static void* WorldClose(void* instance, S32 offset, S32 size)
{
    if (WORLD_DATA(worldListFreeList))
    {
        RwFreeListDestroy(WORLD_DATA(worldListFreeList));
        WORLD_DATA(worldListFreeList) = NULL;
    }

    _rpWorldPipelineClose();

    worldModule.numInstances--;

    return instance;
}

static void* WorldOpen(void* instance, S32 offset, S32 size)
{
    worldModule.globalsOffset = offset;

    defaultSurfaceProps.ambient = 0.0f;
    defaultSurfaceProps.diffuse = 0.0f;
    defaultSurfaceProps.specular = 0.0f;

    if (!_rpWorldPipelineOpen())
    {
        return NULL;
    }

    WORLD_DATA(worldListFreeList) = RwFreeListCreate(sizeof(WorldListEntry), 3, 0);

    if (!WORLD_DATA(worldListFreeList))
    {
        return NULL;
    }

    rwLinkListInitialize(&WORLD_DATA(worldList));

    worldModule.numInstances++;

    return instance;
}

RpWorldSector* _rpSectorDefaultRenderCallBack(RpWorldSector* sector)
{
    RxPipeline* pipeline;

    if (!sector->numPolygons)
    {
        return sector;
    }

    if (sector->pipeline)
    {
        pipeline = sector->pipeline;
    }
    else if (((RpWorld*)RWSRCGLOBAL(curWorld))->pipeline)
    {
        pipeline = ((RpWorld*)RWSRCGLOBAL(curWorld))->pipeline;
    }
    else
    {
        pipeline = RXPIPELINEGLOBAL(defaultSectorPipeline);
    }

    if (!RxPipelineExecute(pipeline, sector, 1))
    {
        return NULL;
    }

    return sector;
}

S32 _rpWorldForAllGlobalLights(RpLightCallBack callBack, void* pData)
{
    RpWorld* world = (RpWorld*)RWSRCGLOBAL(curWorld);
    RwLLLink* cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
    RwLLLink* end = rwLinkListGetTerminator(&world->directionalLightList);

    while (cur != end)
    {
        RpLight* light = rwLLLinkGetData(cur, RpLight, inWorld);
        RwLLLink* next = rwLLLinkGetNext(cur);

        if (light && !callBack(light, pData))
        {
            return 1;
        }

        cur = next;
    }

    return 1;
}

RpWorldSector* _rpWorldSectorForAllLocalLights(RpWorldSector* sector, RpLightCallBack callBack,
                                               void* pData)
{
    RwLLLink* cur = rwLinkListGetFirstLLLink(&sector->lightsInWorldSector);
    RwLLLink* end = rwLinkListGetTerminator(&sector->lightsInWorldSector);

    while (cur != end)
    {
        LightTie* tie = (LightTie*)cur;
        RwLLLink* next = rwLLLinkGetNext(cur);

        if (tie->light && !callBack(tie->light, pData))
        {
            return sector;
        }

        cur = next;
    }

    return sector;
}

S32 _rpWorldFindBBox(RpWorld* world, RwBBox* boundingBox)
{
    RpSector* stack[SECTOR_STACK_SIZE];
    RpSector* sector = world->rootSector;
    S32 nStack = 0;
    S32 first = 0;

    do
    {
        if (sector->type < 0)
        {
            RpWorldSector* worldSector = (RpWorldSector*)sector;

            if (!first)
            {
                RwBBoxInitialize(boundingBox, &worldSector->tightBoundingBox.inf);
                first = 1;
            }
            else
            {
                RwBBoxAddPoint(boundingBox, &worldSector->tightBoundingBox.inf);
            }

            RwBBoxAddPoint(boundingBox, &worldSector->tightBoundingBox.sup);

            sector = stack[nStack--];
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            nStack++;
            sector = plane->leftSubTree;
            stack[nStack] = plane->rightSubTree;
        }
    } while (nStack >= 0);

    return 1;
}

RpWorld* _rpWorldSetupSectorBoundingBoxes(RpWorld* world)
{
    RwBBox bbox;
    RpSector* sectorStack[SECTOR_STACK_SIZE];
    RwBBox bboxStack[SECTOR_STACK_SIZE];
    S32 nStack = 0;
    RpSector* sector;

    bbox = world->boundingBox;
    sector = world->rootSector;

    while (nStack >= 0)
    {
        if (sector->type < 0)
        {
            ((RpWorldSector*)sector)->boundingBox = bbox;

            sector = sectorStack[nStack];
            bbox = bboxStack[nStack];
            nStack--;
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            nStack++;
            sectorStack[nStack] = plane->rightSubTree;
            bboxStack[nStack] = bbox;
            SECTORCOORD(bboxStack[nStack].inf, plane->type) = plane->rightValue;
            SECTORCOORD(bbox.sup, plane->type) = plane->leftValue;
            sector = plane->leftSubTree;
        }
    }

    return world;
}

void _rpWorldRegisterWorld(RpWorld* world, U32 size)
{
    WorldListEntry* entry =
        (WorldListEntry*)RWSRCGLOBAL(memoryAlloc)(WORLD_DATA(worldListFreeList));

    if (entry)
    {
        entry->world = world;
        entry->size = size;
        rwLinkListAddLLLink(&WORLD_DATA(worldList), &entry->link);
    }
}

void _rpWorldUnregisterWorld(RpWorld* world)
{
    RwLLLink* cur = rwLinkListGetFirstLLLink(&WORLD_DATA(worldList));
    RwLLLink* end = rwLinkListGetTerminator(&WORLD_DATA(worldList));

    while (cur != end)
    {
        WorldListEntry* entry = rwLLLinkGetData(cur, WorldListEntry, link);

        if (entry->world == world)
        {
            rwLinkListRemoveLLLink(&entry->link);
            RWSRCGLOBAL(memoryFree)(WORLD_DATA(worldListFreeList), entry);
            return;
        }

        cur = rwLLLinkGetNext(cur);
    }
}

static RpWorld* WorldBuildMeshAtomicSector(RpWorld* world, RpWorldSector* sector)
{
    RpMaterial** materials = world->matList.materials + sector->matListWindowBase;

    if (!sector->mesh)
    {
        RpMeshHeader* mesh;
        RpBuildMesh* buildMesh;
        S32 i;

        buildMesh = _rpBuildMeshCreate(sector->numPolygons);

        if (buildMesh)
        {
            for (i = 0; i < sector->numPolygons; i++)
            {
                RpPolygon* polygon = &sector->polygons[i];

                _rpBuildMeshAddTriangle(buildMesh, materials[polygon->matIndex],
                                        polygon->vertIndex[0], polygon->vertIndex[1],
                                        polygon->vertIndex[2]);
            }

            if (world->flags & rpWORLDTRISTRIP)
            {
                mesh = _rpMeshOptimise(buildMesh, 1);
            }
            else
            {
                mesh = _rpMeshOptimise(buildMesh, 0);
            }

            if (mesh)
            {
                sector->mesh = mesh;
            }
            else
            {
                _rpBuildMeshDestroy(buildMesh);
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }

    return world;
}

RpWorld* RpWorldUnlock(RpWorld* world)
{
    RpSector* stack[SECTOR_STACK_SIZE];
    RpSector* sector = world->rootSector;
    S32 nStack = 0;

    do
    {
        if (sector->type < 0)
        {
            world = WorldBuildMeshAtomicSector(world, (RpWorldSector*)sector);

            if (!world)
            {
                return NULL;
            }

            sector = stack[nStack--];
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            nStack++;
            sector = plane->leftSubTree;
            stack[nStack] = plane->rightSubTree;
        }
    } while (nStack >= 0);

    return world;
}

S32 _rpCollSectorFindSize(RpCollSector* root)
{
    S32 stack[COLLSECTOR_STACK_SIZE];
    S32 node;
    S32 maxNode;
    S32 nStack;

    if (root)
    {
        node = 1;
        maxNode = 1;
        nStack = 0;

        do
        {
            if (node > maxNode)
            {
                maxNode = node;
            }

            if (((U8*)root)[node * COLLSECTOR_SIZE] & COLLSECTOR_PLANE)
            {
                stack[++nStack] = node + node + 1;
                node = node + node;
            }
            else
            {
                node = stack[nStack--];
            }
        } while (nStack >= 0);

        return (maxNode + 1) * COLLSECTOR_SIZE;
    }

    return 0;
}

void _rpWorldSetSurfaceProperties(RpWorld* world, const RwSurfaceProperties* surfaceProps)
{
    world->surfaceProps = *surfaceProps;
}

RpWorld* RpWorldSectorGetWorld(const RpWorldSector* sector)
{
    RwLLLink* cur = rwLinkListGetFirstLLLink(&WORLD_DATA(worldList));
    RwLLLink* end = rwLinkListGetTerminator(&WORLD_DATA(worldList));

    while (cur != end)
    {
        WorldListEntry* entry = rwLLLinkGetData(cur, WorldListEntry, link);

        if (entry->world->object.privateFlags & rpWORLDSINGLEMALLOC)
        {
            if ((U8*)sector >= (U8*)entry->world &&
                (U8*)sector < (U8*)entry->world + entry->size)
            {
                return entry->world;
            }
        }
        else
        {
            WorldSectorSearch search;

            search.sector = (RpWorldSector*)sector;
            search.found = 0;

            RpWorldForAllWorldSectors(entry->world, WorldFindSector, &search);

            if (search.found)
            {
                return entry->world;
            }
        }

        cur = rwLLLinkGetNext(cur);
    }

    return NULL;
}

RpWorld* RpWorldRender(RpWorld* world)
{
    RwCameraForAllSectorsInFrustum((RwCamera*)RWSRCGLOBAL(curCamera), WorldSectorRender, world);

    return world;
}

S32 RpWorldDestroy(RpWorld* world)
{
    RpSector* stack[SECTOR_STACK_SIZE];
    S32 nStack;
    RpSector* sector;

    _rpWorldUnregisterWorld(world);

    nStack = 0;
    sector = world->rootSector;

    do
    {
        if (sector->type < 0)
        {
            RpWorldSector* worldSector = (RpWorldSector*)sector;

            if (worldSector->mesh)
            {
                _rpMeshDestroy(worldSector->mesh);
                worldSector->mesh = NULL;
            }

            sector = stack[nStack--];
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            stack[++nStack] = plane->rightSubTree;
            sector = plane->leftSubTree;
        }
    } while (nStack >= 0);

    _rpMaterialListDeinitialize(&world->matList);

    if (world->object.privateFlags & rpWORLDSINGLEMALLOC)
    {
        WorldSectorDeinstanceAll(world->rootSector);
        _rwPluginRegistryDeInitObject(&worldTKList, world);
        RwFree(world);
    }
    else
    {
        WorldSectorDestroyRecurse(world->rootSector);
        _rwPluginRegistryDeInitObject(&worldTKList, world);
        RwFree(world);
    }

    return 1;
}

RpWorld* RpWorldSetSectorRenderCallBack(RpWorld* world, RpWorldSectorCallBackRender fpCallBack)
{
    if (!fpCallBack)
    {
        fpCallBack = _rpSectorDefaultRenderCallBack;
    }

    world->renderCallBack = fpCallBack;

    return world;
}

RpWorld* RpWorldCreate(RwBBox* boundingBox)
{
    RpWorld* world;
    RpWorldSector* sector;

    world = (RpWorld*)RwMalloc(worldTKList.sizeOfStruct);

    if (!world)
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000013, worldTKList.sizeOfStruct);
        RwErrorSet(&err);

        return NULL;
    }

    rwObjectInitialize(world, rpWORLD, 0);

    _rpMaterialListInitialize(&world->matList);

    world->renderOrder = rpWORLDRENDERBACK2FRONT;
    world->flags = 0;

    sector = (RpWorldSector*)RwMalloc(sectorTKList.sizeOfStruct);

    if (!sector)
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000013, sizeof(sector));
        RwErrorSet(&err);

        RwFree(world);

        return NULL;
    }

    sector->type = SECTORTYPE_WORLDSECTOR;
    sector->colSectorRoot = NULL;
    sector->repEntry = NULL;
    sector->mesh = NULL;
    rwLinkListInitialize(&sector->collAtomicsInWorldSector);
    rwLinkListInitialize(&sector->noCollAtomicsInWorldSector);
    rwLinkListInitialize(&sector->lightsInWorldSector);
    sector->numVertices = 0;
    sector->numPolygons = 0;
    sector->vertices = NULL;
    sector->polygons = NULL;
    sector->normals = NULL;
    memset(sector->texCoords, 0, sizeof(sector->texCoords));
    sector->preLitLum = NULL;
    sector->boundingBox.inf = boundingBox->inf;
    sector->boundingBox.sup = boundingBox->sup;
    sector->tightBoundingBox.inf = boundingBox->inf;
    sector->tightBoundingBox.sup = boundingBox->sup;
    sector->pipeline = NULL;

    world->rootSector = (RpSector*)sector;
    world->numTexCoordSets = 0;
    world->worldOrigin.x = 0.0f;
    world->worldOrigin.y = 0.0f;
    world->worldOrigin.z = 0.0f;
    world->boundingBox.inf = boundingBox->inf;
    world->boundingBox.sup = boundingBox->sup;
    rwLinkListInitialize(&world->clumpList);
    world->numClumpsInWorld = 0;
    world->currentClumpLink = rwLinkListGetTerminator(&world->clumpList);
    world->surfaceProps = defaultSurfaceProps;
    rwLinkListInitialize(&world->lightList);
    rwLinkListInitialize(&world->directionalLightList);
    world->renderCallBack = _rpSectorDefaultRenderCallBack;
    world->pipeline = NULL;

    _rpWorldRegisterWorld(world, worldTKList.sizeOfStruct);

    _rwPluginRegistryInitObject(&worldTKList, world);
    _rwPluginRegistryInitObject(&sectorTKList, sector);

    if (!RpWorldUnlock(world))
    {
        RpWorldDestroy(world);
        return NULL;
    }

    return world;
}

RpWorld* RpWorldForAllWorldSectors(RpWorld* world, RpWorldSectorCallBack fpCallBack, void* pData)
{
    RpSector* stack[SECTOR_STACK_SIZE];
    S32 nStack = 0;
    RpSector* sector = world->rootSector;

    do
    {
        if (sector->type < 0)
        {
            if (!fpCallBack((RpWorldSector*)sector, pData))
            {
                return world;
            }

            sector = stack[nStack--];
        }
        else
        {
            RpPlaneSector* plane = (RpPlaneSector*)sector;

            stack[++nStack] = plane->rightSubTree;
            sector = plane->leftSubTree;
        }
    } while (nStack >= 0);

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

S32 RpWorldPluginAttach(void)
{
    S32 result;

    result = RwEngineRegisterPlugin(4, MATERIALMODULE_PLUGINID, _rpMaterialOpen, _rpMaterialClose);
    result |= RwEngineRegisterPlugin(0x30, MESHMODULE_PLUGINID, _rpMeshOpen, _rpMeshClose);
    result |= RwEngineRegisterPlugin(4, GEOMETRYMODULE_PLUGINID, _rpGeometryOpen, _rpGeometryClose);
    result |= RwEngineRegisterPlugin(8, CLUMPMODULE_PLUGINID, _rpClumpOpen, _rpClumpClose);
    result |= RwEngineRegisterPlugin(4, LIGHTMODULE_PLUGINID, _rpLightOpen, _rpLightClose);
    result |= RwEngineRegisterPlugin(0, SECTORMODULE_PLUGINID, _rpSectorOpen, _rpSectorClose);
    result |= RwEngineRegisterPlugin(sizeof(WorldModuleData), WORLDMODULE_PLUGINID, WorldOpen,
                                     WorldClose);
    result |= RwEngineRegisterPlugin(0, BINWORLDMODULE_PLUGINID, _rpBinaryWorldOpen,
                                     _rpBinaryWorldClose);

    if (result < 0)
    {
        return 0;
    }

    if (!_rpWorldObjRegisterExtensions())
    {
        return 0;
    }

    if (!_rpClumpRegisterExtensions())
    {
        return 0;
    }

    if (!_rxWorldDevicePluginAttach())
    {
        return 0;
    }

    return 1;
}
