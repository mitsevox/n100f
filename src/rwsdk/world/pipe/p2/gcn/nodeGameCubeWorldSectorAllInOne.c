#include <rpworld.h>
#include <string.h>
#include <dolphin/os.h>

// The GameCube world sector all-in-one node: instances a sector into one resource entry
// (display lists and vertex arrays in the GX formats), collects the lights that reach it and
// draws it, each step through a callback kept in the node's private data. Sectors are never
// reinstanced.

// The last draw sync token the GPU has passed.
extern volatile U16 _RwDlTokenLastSeen;

S32 _rxGCWorldSectorAllInOneNode(RxPipelineNode* self, register RxPipelineNodeParam* params);
S32 _rxGCWorldSectorAllInOnePipelineInit(RxPipelineNode* node);

void _rxGCResEntryWaitDone(RwResEntry* resEntry);
S32 rwGCNVertexGetSize(const RpGameCubeVtxFmt* vtxFmt, U32 flags, S32 numVerts, U8 numTexCoords);
U32 GCNDisplayListsGetSize(const RpMeshHeader* header, S32 numTexCoords, S32 indexSize, U32 flags);
void GCNDisplayListsBuild(GCNDisplayList* lists, U8* dlMem, const RpMeshHeader* header,
                          S32 numTexCoords, S32 indexSize, U32 flags);
void _rwDlWorldSectorInstanceFast(RpWorldSector* sector, GCNVertexArrays* arrays,
                                  const RpGameCubeVtxFmt* vtxFmt, U32 flags, U8 numTexCoords,
                                  U8* dst);

// The world flag that keeps the entry out of the resource arena. Written for this project.
#define ALLINONE_WORLDOWNENTRY 0x02000000

#define ALLINONE_ALIGN32(_size) (((_size) + 0x1F) & ~0x1F)

typedef RpWorldSector* (*AllInOneSectorCB)(RpWorldSector* sector, GCNObjectData* objectData);

// The default lighting callback: clears the lighting data, then loads the global lights and the
// sector's local lights, and clamps the summed ambient color.
RpWorldSector* _rxGCWorldSectorDefaultLightingCallback(RpWorldSector* sector, GCNObjectData* objectData)
{
    objectData->globalMask = 0;
    objectData->localMask = 0;
    objectData->ambient.red = 0.0f;
    objectData->ambient.green = 0.0f;
    objectData->ambient.blue = 0.0f;
    objectData->ambient.alpha = 1.0f;
    objectData->ambientUsed = FALSE;
    objectData->numLights = 0;

    if (((RpWorld*)RWSRCGLOBAL(curWorld))->flags & rpWORLDLIGHT)
    {
        RwLLLink* cur;
        RwLLLink* end;

        _rwGCLightsGlobalEnable(rpLIGHTLIGHTWORLD, objectData);
        RWSRCGLOBAL(lightFrame)++;

        cur = rwLinkListGetFirstLLLink(&sector->lightsInWorldSector);
        end = rwLinkListGetTerminator(&sector->lightsInWorldSector);

        while (cur != end)
        {
            RpLight* light = rwLLLinkGetData(cur, LightTie, sectorLink)->light;

            if (light && rwObjectTestFlags(light, rpLIGHTLIGHTWORLD))
            {
                _rwGCLightsLocalEnable(light, objectData);
            }

            cur = rwLLLinkGetNext(cur);
        }
    }

    if (objectData->ambientUsed)
    {
        if (objectData->ambient.red > 1.0f)
        {
            objectData->ambient.red = 1.0f;
        }

        if (objectData->ambient.green > 1.0f)
        {
            objectData->ambient.green = 1.0f;
        }

        if (objectData->ambient.blue > 1.0f)
        {
            objectData->ambient.blue = 1.0f;
        }
    }

    return sector;
}

// The default instance callback: allocates the sector's entry, builds the display lists and
// instances the vertex arrays.
void* _rxGCWorldSectorDefaultInstanceCallback(void* object, GCNObjectData* objectData)
{
    S32 headerSize;
    RpWorldSector* sector = (RpWorldSector*)object;
    RpWorld* world;
    RpMeshHeader* meshHeader;
    S32 size;
    U8* mem;
    GCNVertexArrays* arrays;
    U32 numVertices;
    U32 dlSize;
    U32 worldFlags;
    U32 flags;

    world = (RpWorld*)RWSRCGLOBAL(curWorld);
    worldFlags = world->flags | rpGEOMETRYPOSITIONS;
    flags = worldFlags & ~objectData->privateData->clearFlags;
    numVertices = sector->numVertices;
    meshHeader = sector->mesh;
    headerSize = meshHeader->numMeshes * sizeof(GCNDisplayList);
    dlSize = GCNDisplayListsGetSize(meshHeader, world->numTexCoordSets,
                                    numVertices < 0xFF ? 1 : 2, worldFlags);

    size = ALLINONE_ALIGN32(sizeof(GCNVertexArrays) + headerSize) + dlSize;
    size = ALLINONE_ALIGN32(size) +
           rwGCNVertexGetSize(RpGameCubeWorldGetVtxFmt(world), flags, numVertices, world->numTexCoordSets);

    if (world->flags & ALLINONE_WORLDOWNENTRY)
    {
        objectData->repEntry = (RwResEntry*)RwMalloc(size + sizeof(RwResEntry) + 0x20);
        objectData->repEntry->link.next = NULL;
        objectData->repEntry->link.prev = NULL;
        objectData->repEntry->owner = sector;
        objectData->repEntry->size = size + 0x20;
        objectData->repEntry->ownerRef = &sector->repEntry;
        objectData->repEntry->destroyNotify = _rxGCResEntryWaitDone;
        sector->repEntry = objectData->repEntry;
    }
    else
    {
        objectData->repEntry = RwResourcesAllocateResEntry(sector, &sector->repEntry, size + 0x20,
                                                           _rxGCResEntryWaitDone);
    }

    arrays = (GCNVertexArrays*)(objectData->repEntry + 1);
    memset(arrays, 0, sizeof(GCNVertexArrays));
    arrays->serialNum = meshHeader->serialNum;
    arrays->indexType = numVertices < 0xFF ? 2 : 3;
    mem = (U8*)ALLINONE_ALIGN32((U32)arrays + sizeof(GCNVertexArrays) + headerSize);
    arrays->syncToken = _RwDlTokenLastSeen;
    arrays->flags = objectData->privateData->clearFlags;
    GCNDisplayListsBuild((GCNDisplayList*)(arrays + 1), mem, meshHeader,
                         world->numTexCoordSets, numVertices < 0xFF ? 1 : 2, worldFlags);
    mem = (U8*)ALLINONE_ALIGN32((U32)mem + dlSize);
    _rwDlWorldSectorInstanceFast(sector, arrays, RpGameCubeWorldGetVtxFmt(world), flags,
                                 world->numTexCoordSets, mem);
    DCFlushRange(objectData->repEntry + 1, objectData->repEntry->size);

    return object;
}

// The node body: instances the sector when its entry is missing or its meshes changed, then runs
// the lighting and render callbacks.
S32 _rxGCWorldSectorAllInOneNode(RxPipelineNode* self, register RxPipelineNodeParam* params)
{
    register RpWorldSector* sector;
    GCNAllInOnePrivateData* privateData;
    GCNObjectData objectData;
    U32 worldFlags;
    register RwGlobals* globals;

    // FAKEMATCH: the engine-globals load and the sector load are inline asm, so they stay on
    // either side of the private-data store as in the target; in C the scheduler issues the
    // current-world load before the sector load. Real form believed to be
    // sector = params->dataParam at the declaration and RWSRCGLOBAL(curWorld) inline.
    privateData = (GCNAllInOnePrivateData*)self->privateData;
    asm volatile { lwz globals, RwEngineInstance }
    objectData.privateData = privateData;
    asm volatile { lwz sector, 0(params) }
    worldFlags = ((RpWorld*)globals->curWorld)->flags;

    if (!(worldFlags & rpWORLDNATIVE))
    {
        RpMeshHeader* meshHeader;
        RwResEntry* repEntry;

        if (!sector->numVertices)
        {
            return TRUE;
        }

        meshHeader = sector->mesh;

        if (!meshHeader->numMeshes)
        {
            return TRUE;
        }

        repEntry = sector->repEntry;
        objectData.repEntry = repEntry;
        objectData.meshHeader = meshHeader;
        objectData.flags = worldFlags;

        if (repEntry && ((GCNVertexArrays*)(repEntry + 1))->serialNum != meshHeader->serialNum)
        {
            RwResourcesFreeResEntry(repEntry);
            objectData.repEntry = NULL;
        }

        if (objectData.repEntry)
        {
            RwResourcesUseResEntry(objectData.repEntry);
        }
        else if (privateData->instanceCB &&
                 ((AllInOneSectorCB)privateData->instanceCB)(sector, &objectData) != sector)
        {
            return FALSE;
        }
    }
    else
    {
        objectData.repEntry = sector->repEntry;
        objectData.meshHeader = sector->mesh;
        objectData.flags = worldFlags;
    }

    if (privateData->lightingCB &&
        ((AllInOneSectorCB)privateData->lightingCB)(sector, &objectData) != sector)
    {
        return FALSE;
    }

    if (privateData->renderCB &&
        ((AllInOneSectorCB)privateData->renderCB)(sector, &objectData) != sector)
    {
        return FALSE;
    }

    return TRUE;
}

S32 _rxGCWorldSectorAllInOnePipelineInit(RxPipelineNode* node)
{
    GCNAllInOnePrivateData* privateData = (GCNAllInOnePrivateData*)node->privateData;

    privateData->instanceCB = _rxGCWorldSectorDefaultInstanceCallback;
    privateData->reinstanceCB = NULL;
    privateData->lightingCB = _rxGCWorldSectorDefaultLightingCallback;
    privateData->renderCB = _rxGCDefaultRenderCallback;
    privateData->clearFlags = 0;

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetGameCubeWorldSectorAllInOne(void)
{
    // The node definition: the node body and the pipeline-node init that fills the private data's
    // callbacks, no clusters or outputs, and private data for the callbacks.
    static RxNodeDefinition nodeGameCubeWorldSectorAllInOneCSL = {
        "GamerCubeWorldSectorAllInOne.csl",
        { _rxGCWorldSectorAllInOneNode, NULL, NULL, _rxGCWorldSectorAllInOnePipelineInit, NULL,
          NULL, NULL },
        { 0, NULL, NULL, 0, NULL },
        sizeof(GCNAllInOnePrivateData),
        rxNODEDEFCONST,
        0,
    };

    return &nodeGameCubeWorldSectorAllInOneCSL;
}
