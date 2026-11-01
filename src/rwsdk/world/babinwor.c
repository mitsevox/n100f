#include <rpworld.h>

static U32 lastSeenWorldRightsPluginId;
static S32 lastSeenWorldExtraData;
static U32 lastSeenSectRightsPluginId;
static S32 lastSeenSectExtraData;
static RwModuleInfo binWorldModule;

RwStream* _rpReadWorldRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size)
{
    if (!RwStreamReadInt32(stream, (S32*)&lastSeenWorldRightsPluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (length == 8)
    {
        if (!RwStreamReadInt32(stream, &lastSeenWorldExtraData, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

RwStream* _rpWriteWorldRights(RwStream* stream, S32 length, const void* object, S32 offset,
                              S32 size)
{
    const RpWorld* world = (const RpWorld*)object;

    if (!RwStreamWriteInt32(stream, (const S32*)&world->pipeline->pluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, (const S32*)&world->pipeline->pluginData, sizeof(S32)))
    {
        return NULL;
    }

    return stream;
}

S32 _rpSizeWorldRights(const void* object, S32 offset, S32 size)
{
    const RpWorld* world = (const RpWorld*)object;

    if (world->pipeline && world->pipeline->pluginId)
    {
        return 8;
    }

    return 0;
}

RwStream* _rpReadSectRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size)
{
    if (!RwStreamReadInt32(stream, (S32*)&lastSeenSectRightsPluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (length == 8)
    {
        if (!RwStreamReadInt32(stream, &lastSeenSectExtraData, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

RwStream* _rpWriteSectRights(RwStream* stream, S32 length, const void* object, S32 offset,
                             S32 size)
{
    const RpWorldSector* sector = (const RpWorldSector*)object;

    if (!RwStreamWriteInt32(stream, (const S32*)&sector->pipeline->pluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, (const S32*)&sector->pipeline->pluginData, sizeof(S32)))
    {
        return NULL;
    }

    return stream;
}

S32 _rpSizeSectRights(const void* object, S32 offset, S32 size)
{
    const RpWorldSector* sector = (const RpWorldSector*)object;

    if (sector->pipeline && sector->pipeline->pluginId)
    {
        return 8;
    }

    return 0;
}

// What a world sector's struct chunk holds: its material list base, polygon and vertex counts, its
// bounding box (minimum corner first) and whether a collision tree follows
// (project-own type name; members that fill an SLUS-20349 DWARF field take its name, the rest are this project's own).
typedef struct SectorStreamHeader SectorStreamHeader;
struct SectorStreamHeader
{
    S32 matListWindowBase;
    S32 numPolygons;
    S32 numVertices;
    RwV3d inf;
    RwV3d sup;
    S32 hasCollTree;
    S32 pad;
};

// What a plane sector's struct chunk holds: the split axis and value, whether each side is a
// world sector, and the extents of the two sides
// (project-own type name; members that fill an SLUS-20349 DWARF field take its name, the rest are this project's own).
typedef struct PlaneStreamHeader PlaneStreamHeader;
struct PlaneStreamHeader
{
    S32 type;
    F32 value;
    S32 leftIsLeaf;
    S32 rightIsLeaf;
    F32 leftValue;
    F32 rightValue;
};

// What a world's struct chunk holds: whether the root is a world sector, the world origin
// negated, the surface properties, the polygon, vertex, plane sector and world sector counts, the
// collision trees' total size and the world's flags
// (project-own type name; members that fill an SLUS-20349 DWARF field take its name, the rest are this project's own).
typedef struct WorldStreamHeader WorldStreamHeader;
struct WorldStreamHeader
{
    S32 rootIsLeaf;
    RwV3d negOrigin;
    RwSurfaceProperties surfaceProps;
    S32 numPolygons;
    S32 numVertices;
    S32 planeCount;
    S32 sectorCount;
    S32 collTreeBytes;
    S32 flags;
};

// A polygon as streams before version 0x30400 stored it, with byte indices (project-own type name;
// the members follow the DWARF RpPolygon).
typedef struct OldPolygon OldPolygon;
struct OldPolygon
{
    U8 matIndex;
    U8 vertIndex[3];
};

// Streams before this version stored polygons with byte indices. Written for this project.
#define BINWORLD_U16POLYGONVERSION 0x30400

// The world texture coordinate set count and the plane-sector extents flag. Written for this
// project.
#define BINWORLD_TEXCOORDSETSMASK 0x00FF0000
#define BINWORLD_PLANEEXTENTS 0x40000000

// Whether a chunk version is one this library reads.
#define BINWORLD_VERSIONOK(_version)                                                              \
    ((_version) >= rwLIBRARYBASEVERSION && (_version) <= rwLIBRARYCURRENTVERSION)

extern RwPluginRegistry sectorTKList;
extern RwPluginRegistry worldTKList;

S32 _rpWorldFindBBox(RpWorld* world, RwBBox* boundingBox);
RpWorld* _rpWorldSetupSectorBoundingBoxes(RpWorld* world);
void _rpWorldRegisterWorld(RpWorld* world, U32 size);
void _rpWorldUnregisterWorld(RpWorld* world);
RpWorld* RpWorldUnlock(RpWorld* world);
S32 _rpCollSectorFindSize(RpCollSector* root);
RpWorld* RpWorldSetSectorRenderCallBack(RpWorld* world, RpWorldSectorCallBackRender fpCallBack);
void* RwMemNative16(void* mem, U32 size);
RpPlaneSector* PlaneSectorStreamRead(RwStream* stream, U8** memory, RpWorld* world, U32 flags);

// Takes the next size bytes of the world's single allocation.
static void* BinWorldTake(U8** memory, U32 size)
{
    U8* piece = *memory;

    *memory = piece + size;

    return piece;
}

// Reads a world sector from its chunk into the world's single allocation (memory is the next
// free byte): its header, then, unless the world is native, its vertices, normals, prelight
// colors, texture coordinates and polygons (widening old byte-indexed polygons), then its
// collision tree and plugin data.
RpWorldSector* WorldSectorStreamRead(RwStream* stream, U8** memory, RpWorld* world, U32 flags)
{
    U32 version;
    SectorStreamHeader header;
    RpWorldSector* sector;
    RpCollSector collBuffer[128];

    if (!RwStreamFindChunk(stream, rwID_STRUCT, NULL, &version))
    {
        return NULL;
    }

    if (!BINWORLD_VERSIONOK(version))
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }

    if (RwStreamRead(stream, &header, sizeof(header)) != sizeof(header))
    {
        return NULL;
    }

    RwMemNative32(&header, sizeof(header));
    RwMemFloat32ToReal(&header.inf, sizeof(header.inf));
    RwMemFloat32ToReal(&header.sup, sizeof(header.sup));

    sector = (RpWorldSector*)BinWorldTake(memory, sectorTKList.sizeOfStruct);

    if (!sector)
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000013, sectorTKList.sizeOfStruct);
        RwErrorSet(&err);
        return NULL;
    }

    sector->type = -1;
    sector->tightBoundingBox.inf = header.inf;
    sector->tightBoundingBox.sup = header.sup;
    sector->matListWindowBase = header.matListWindowBase;
    sector->numPolygons = header.numPolygons;
    sector->numVertices = header.numVertices;
    sector->polygons = NULL;
    sector->vertices = NULL;
    sector->preLitLum = NULL;
    sector->normals = NULL;
    memset(sector->texCoords, 0, sizeof(sector->texCoords));
    sector->colSectorRoot = NULL;
    sector->pipeline = NULL;
    sector->repEntry = NULL;
    sector->mesh = NULL;
    rwLinkListInitialize(&sector->collAtomicsInWorldSector);
    rwLinkListInitialize(&sector->noCollAtomicsInWorldSector);
    rwLinkListInitialize(&sector->lightsInWorldSector);

    if (!(world->flags & rpWORLDNATIVE))
    {
        if (header.numVertices)
        {
            U32 size = header.numVertices * sizeof(RwV3d);

            sector->vertices = (RwV3d*)BinWorldTake(memory, size);

            if (!sector->vertices)
            {
                RwError err;
                err.pluginID = 2;
                err.errorCode = _rwerror(0x80000013, size);
                RwErrorSet(&err);
                return NULL;
            }

            if (!RwStreamReadReal(stream, (F32*)sector->vertices, size))
            {
                return NULL;
            }

            if (flags & rpWORLDNORMALS)
            {
                size = header.numVertices * sizeof(RpVertexNormal);
                sector->normals = (RpVertexNormal*)BinWorldTake(memory, size);

                if (!sector->normals)
                {
                    RwError err;
                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x80000013, size);
                    RwErrorSet(&err);
                    return NULL;
                }

                if (RwStreamRead(stream, sector->normals, size) != size)
                {
                    return NULL;
                }
            }

            if (flags & rpWORLDPRELIT)
            {
                size = header.numVertices * sizeof(RwRGBA);
                sector->preLitLum = (RwRGBA*)BinWorldTake(memory, size);

                if (!sector->preLitLum)
                {
                    RwError err;
                    err.pluginID = 2;
                    err.errorCode = _rwerror(0x80000013, size);
                    RwErrorSet(&err);
                    return NULL;
                }

                if (!RwStreamRead(stream, sector->preLitLum, size))
                {
                    return NULL;
                }
            }

            if (world->numTexCoordSets > 0)
            {
                S32 i;

                size = sector->numVertices * sizeof(RwTexCoords);

                for (i = 0; i < world->numTexCoordSets; i++)
                {
                    sector->texCoords[i] = (RwTexCoords*)BinWorldTake(memory, size);

                    if (!sector->texCoords[i])
                    {
                        RwError err;
                        err.pluginID = 2;
                        err.errorCode = _rwerror(0x80000013, size);
                        RwErrorSet(&err);
                        return NULL;
                    }

                    if (!RwStreamReadReal(stream, (F32*)sector->texCoords[i], size))
                    {
                        return NULL;
                    }
                }
            }
        }

        if (header.numPolygons)
        {
            U32 size = header.numPolygons * sizeof(RpPolygon);
            U32 readSize;

            sector->polygons = (RpPolygon*)BinWorldTake(memory, size);

            if (!sector->polygons)
            {
                RwError err;
                err.pluginID = 2;
                err.errorCode = _rwerror(0x80000013, size);
                RwErrorSet(&err);
                return NULL;
            }

            readSize = size;

            if (version < BINWORLD_U16POLYGONVERSION)
            {
                readSize = (S32)size / 2;
            }

            if (RwStreamRead(stream, sector->polygons, readSize) != readSize)
            {
                return NULL;
            }

            if (version < BINWORLD_U16POLYGONVERSION)
            {
                S32 numPolygons = sector->numPolygons;
                S32 oldSize = numPolygons * sizeof(OldPolygon);
                RpPolygon* polygons = sector->polygons;
                OldPolygon* oldPolygons = (OldPolygon*)RwMalloc(oldSize);
                S32 i;

                memcpy(oldPolygons, polygons, oldSize);

                for (i = 0; i < numPolygons; i++)
                {
                    polygons[i].matIndex = oldPolygons[i].matIndex;
                    polygons[i].vertIndex[0] = oldPolygons[i].vertIndex[0];
                    polygons[i].vertIndex[1] = oldPolygons[i].vertIndex[1];
                    polygons[i].vertIndex[2] = oldPolygons[i].vertIndex[2];
                }

                RwFree(oldPolygons);
            }
            else
            {
                RwMemNative16(sector->polygons, size);
            }
        }
    }

    if (header.hasCollTree)
    {
        S32 size;

        if (RwStreamRead(stream, collBuffer, sizeof(collBuffer)) != sizeof(collBuffer))
        {
            return NULL;
        }

        size = (_rpCollSectorFindSize(collBuffer) + 3) & ~3;
        sector->colSectorRoot = (RpCollSector*)BinWorldTake(memory, size);

        if (!sector->colSectorRoot)
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);
            return NULL;
        }

        memcpy(sector->colSectorRoot, collBuffer, size);
    }
    else
    {
        sector->colSectorRoot = NULL;
    }

    _rwPluginRegistryInitObject(&sectorTKList, sector);

    lastSeenSectRightsPluginId = 0;
    lastSeenSectExtraData = 0;

    if (!_rwPluginRegistryReadDataChunks(&sectorTKList, stream, sector))
    {
        return NULL;
    }

    if (lastSeenSectRightsPluginId)
    {
        _rwPluginRegistryInvokeRights(&sectorTKList, lastSeenSectRightsPluginId, sector,
                                      lastSeenSectExtraData);
    }

    return sector;
}

// Reads a plane sector and, recursively, its two subtrees from their chunks into the world's
// single allocation. The side extents are read only when the world has them; otherwise both take
// the split value.
RpPlaneSector* PlaneSectorStreamRead(RwStream* stream, U8** memory, RpWorld* world, U32 flags)
{
    U32 size;
    U32 version;
    PlaneStreamHeader header;
    RpPlaneSector* plane;
    RpSector* subTree;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (!BINWORLD_VERSIONOK(version))
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }

    memset(&header, 0, sizeof(header));

    if (RwStreamRead(stream, &header, size) != size)
    {
        return NULL;
    }

    RwMemNative32(&header, sizeof(header));
    RwMemFloat32ToReal(&header.value, sizeof(header.value));
    RwMemFloat32ToReal(&header.leftValue, sizeof(header.leftValue));
    RwMemFloat32ToReal(&header.rightValue, sizeof(header.rightValue));

    plane = (RpPlaneSector*)BinWorldTake(memory, sizeof(RpPlaneSector));

    if (!plane)
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000013, sizeof(RpPlaneSector));
        RwErrorSet(&err);
        return NULL;
    }

    plane->type = header.type;
    plane->value = header.value;

    if (flags & BINWORLD_PLANEEXTENTS)
    {
        plane->leftValue = header.leftValue;
        plane->rightValue = header.rightValue;
    }
    else
    {
        plane->leftValue = header.value;
        plane->rightValue = header.value;
    }

    if (header.leftIsLeaf)
    {
        if (!RwStreamFindChunk(stream, rwID_ATOMICSECT, NULL, &version))
        {
            return NULL;
        }

        if (!BINWORLD_VERSIONOK(version))
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
            return NULL;
        }

        subTree = (RpSector*)WorldSectorStreamRead(stream, memory, world, flags);
        plane->leftSubTree = subTree;

        if (!subTree)
        {
            return NULL;
        }
    }
    else
    {
        if (!RwStreamFindChunk(stream, rwID_PLANESECT, NULL, &version))
        {
            return NULL;
        }

        if (!BINWORLD_VERSIONOK(version))
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
            return NULL;
        }

        subTree = (RpSector*)PlaneSectorStreamRead(stream, memory, world, flags);
        plane->leftSubTree = subTree;

        if (!subTree)
        {
            return NULL;
        }
    }

    if (header.rightIsLeaf)
    {
        if (!RwStreamFindChunk(stream, rwID_ATOMICSECT, NULL, &version))
        {
            return NULL;
        }

        if (!BINWORLD_VERSIONOK(version))
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
            return NULL;
        }

        subTree = (RpSector*)WorldSectorStreamRead(stream, memory, world, flags);
        plane->rightSubTree = subTree;

        if (!subTree)
        {
            return NULL;
        }
    }
    else
    {
        if (!RwStreamFindChunk(stream, rwID_PLANESECT, NULL, &version))
        {
            return NULL;
        }

        if (!BINWORLD_VERSIONOK(version))
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
            return NULL;
        }

        subTree = (RpSector*)PlaneSectorStreamRead(stream, memory, world, flags);
        plane->rightSubTree = subTree;

        if (!subTree)
        {
            return NULL;
        }
    }

    return plane;
}

// Frees a world that failed to read.
#define BINWORLD_FAIL(_world)                                                                     \
    do                                                                                             \
    {                                                                                              \
        _rpWorldUnregisterWorld(_world);                                                           \
        RwFree(_world);                                                                            \
    } while (0)

// Reads a world from its chunk: sizes and makes one allocation for the world, its sectors and
// (unless native) their geometry and collision trees, then reads the material list and the
// sector tree, links the lists, finds the bounding boxes, restores each material's texture
// reference, reads the plugin data and unlocks the world to build its meshes.
RpWorld* RpWorldStreamRead(RwStream* stream)
{
    U32 size;
    U32 version;
    WorldStreamHeader header;
    U8* memory;
    RpWorld* world;
    RpSector* rootSector;
    U32 numTexCoordSets;
    S32 texCoordSets;
    S32 format;
    U32 worldSize;
    S32 i;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (!BINWORLD_VERSIONOK(version))
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }

    memset(&header, 0, sizeof(header));

    if (RwStreamRead(stream, &header, size) != size)
    {
        return NULL;
    }

    RwMemNative32(&header, sizeof(header));
    RwMemFloat32ToReal(&header.negOrigin, sizeof(header.negOrigin));
    RwMemFloat32ToReal(&header.surfaceProps, sizeof(header.surfaceProps));

    format = header.flags;

    worldSize = worldTKList.sizeOfStruct + header.planeCount * sizeof(RpPlaneSector) +
                header.sectorCount * sectorTKList.sizeOfStruct;

    texCoordSets = format & BINWORLD_TEXCOORDSETSMASK;

    if (texCoordSets)
    {
        numTexCoordSets = texCoordSets >> 16;
    }
    else if (format & rpWORLDTEXTURED2)
    {
        numTexCoordSets = 2;
    }
    else
    {
        numTexCoordSets = (format & rpWORLDTEXTURED) ? 1 : 0;
    }

    if (!(format & rpWORLDNATIVE))
    {
        worldSize += header.numVertices * sizeof(RwV3d);

        if (format & rpWORLDNORMALS)
        {
            worldSize += header.numVertices * sizeof(RpVertexNormal);
        }

        if (format & rpWORLDPRELIT)
        {
            worldSize += header.numVertices * sizeof(RwRGBA);
        }

        if (numTexCoordSets)
        {
            worldSize += numTexCoordSets * header.numVertices * sizeof(RwTexCoords);
        }

        worldSize += header.numPolygons * sizeof(RpPolygon);
    }

    worldSize += header.collTreeBytes;

    world = (RpWorld*)RwMalloc(worldSize);

    if (!world)
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000013, worldSize);
        RwErrorSet(&err);
        return NULL;
    }

    memset(world, 0, worldSize);
    memory = (U8*)world + worldTKList.sizeOfStruct;

    rwObjectInitialize(world, rpWORLD, 0);
    world->object.privateFlags = rpWORLDSINGLEMALLOC;

    _rpWorldRegisterWorld(world, worldSize);

    world->flags = format;
    world->renderOrder = rpWORLDRENDERBACK2FRONT;
    _rpWorldSetSurfaceProperties(world, &header.surfaceProps);

    RwV3dScaleMacro(&world->worldOrigin, &header.negOrigin, -1.0f);

    world->numTexCoordSets = numTexCoordSets;
    world->rootSector = NULL;
    world->pipeline = NULL;

    if (!RwStreamFindChunk(stream, rwID_MATLIST, NULL, &version))
    {
        BINWORLD_FAIL(world);
        return NULL;
    }

    if (!BINWORLD_VERSIONOK(version))
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }

    if (!_rpMaterialListStreamRead(stream, &world->matList))
    {
        BINWORLD_FAIL(world);
        return NULL;
    }

    if (header.rootIsLeaf)
    {
        if (!RwStreamFindChunk(stream, rwID_ATOMICSECT, NULL, &version))
        {
            BINWORLD_FAIL(world);
            return NULL;
        }

        if (!BINWORLD_VERSIONOK(version))
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
            BINWORLD_FAIL(world);
            return NULL;
        }

        rootSector = (RpSector*)WorldSectorStreamRead(stream, &memory, world, format);
        world->rootSector = rootSector;

        if (!rootSector)
        {
            BINWORLD_FAIL(world);
            return NULL;
        }
    }
    else
    {
        if (!RwStreamFindChunk(stream, rwID_PLANESECT, NULL, &version))
        {
            BINWORLD_FAIL(world);
            return NULL;
        }

        if (!BINWORLD_VERSIONOK(version))
        {
            RwError err;
            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000004);
            RwErrorSet(&err);
            BINWORLD_FAIL(world);
            return NULL;
        }

        rootSector = (RpSector*)PlaneSectorStreamRead(stream, &memory, world, format);
        world->rootSector = rootSector;

        if (!rootSector)
        {
            BINWORLD_FAIL(world);
            return NULL;
        }
    }

    rwLinkListInitialize(&world->clumpList);
    world->numClumpsInWorld = 0;
    world->currentClumpLink = rwLinkListGetTerminator(&world->clumpList);
    rwLinkListInitialize(&world->lightList);
    rwLinkListInitialize(&world->directionalLightList);

    _rpWorldFindBBox(world, &world->boundingBox);
    _rpWorldSetupSectorBoundingBoxes(world);
    RpWorldSetSectorRenderCallBack(world, NULL);

    for (i = 0; i < world->matList.numMaterials; i++)
    {
        RpMaterial* material = world->matList.materials[i];

        if (material->texture)
        {
            RpMaterialSetTexture(material, material->texture);
        }
    }

    _rwPluginRegistryInitObject(&worldTKList, world);

    lastSeenWorldRightsPluginId = 0;
    lastSeenWorldExtraData = 0;

    if (!_rwPluginRegistryReadDataChunks(&worldTKList, stream, world))
    {
        BINWORLD_FAIL(world);
        return NULL;
    }

    if (lastSeenWorldRightsPluginId)
    {
        _rwPluginRegistryInvokeRights(&worldTKList, lastSeenWorldRightsPluginId, world,
                                      lastSeenWorldExtraData);
    }

    if (!RpWorldUnlock(world))
    {
        RpWorldDestroy(world);
        return NULL;
    }

    return world;
}

void* _rpBinaryWorldClose(void* instance, S32 offset, S32 size)
{
    binWorldModule.numInstances--;

    return instance;
}

void* _rpBinaryWorldOpen(void* instance, S32 offset, S32 size)
{
    binWorldModule.numInstances++;

    return instance;
}
