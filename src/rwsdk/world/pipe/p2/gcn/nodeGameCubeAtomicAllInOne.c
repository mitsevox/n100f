#include <rpworld.h>
#include <string.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>

// The GameCube atomic all-in-one node: instances an atomic's geometry into one resource entry
// (display lists and vertex arrays in the GX formats), brings locked or morphing data up to date,
// collects the lights that reach the atomic and draws it, each step through a callback kept in
// the node's private data.

// The draw sync token to issue next, and the last token the GPU has passed.
extern U16 lbl_8025E800;
extern U16 lbl_8025F890;

// The node definition.
extern RxNodeDefinition lbl_801DDA94;

S32 _rwDlTokenQueryDone(U16 token);
void _rxGCResEntryWaitDone(RwResEntry* resEntry);
RpGameCubeVtxFmt* fn_8015D818(const RpGeometry* geometry);
S32 fn_80159380(const RpGameCubeVtxFmt* vtxFmt, U32 flags, S32 numVerts, U8 numTexCoords);
U32 GCNDisplayListsGetSize(const RpMeshHeader* header, S32 numTexCoords, S32 indexSize, U32 flags);
void GCNDisplayListsBuild(GCNDisplayList* lists, U8* dlMem, const RpMeshHeader* header,
                          S32 numTexCoords, S32 indexSize, U32 flags);
void _rwDlGeometryInstanceFast(RpGeometry* geom, GCNVertexArrays* arrays,
                               const RpGameCubeVtxFmt* vtxFmt, U32 flags, U8 numTexCoords,
                               U8* dst);
RpAtomic* fn_80158FB0(RpAtomic* atomic, GCNObjectData* objectData);

// Draw sync tokens wrap at this value; the geometry flag that keeps the entry out of the resource
// arena; the lock bits of the attributes. Written for this project.
#define ALLINONE_TOKENWRAP 0xE000
#define ALLINONE_GEOMETRYOWNENTRY 0x02000000
#define ALLINONE_LOCKVERTICES 0x02
#define ALLINONE_LOCKNORMALS 0x04
#define ALLINONE_LOCKPRELIGHT 0x08
#define ALLINONE_LOCKTEXCOORDS 0xFF0

#define ALLINONE_ALIGN32(_size) (((_size) + 0x1F) & ~0x1F)

// dst = a + (b - a) * t, in that order.
#define ALLINONE_LERP(_dst, _a, _b, _t)                                                            \
    do                                                                                             \
    {                                                                                              \
        (_dst)->x = (_b)->x - (_a)->x;                                                             \
        (_dst)->y = (_b)->y - (_a)->y;                                                             \
        (_dst)->z = (_b)->z - (_a)->z;                                                             \
        (_dst)->x *= (_t);                                                                         \
        (_dst)->y *= (_t);                                                                         \
        (_dst)->z *= (_t);                                                                         \
        (_dst)->x += (_a)->x;                                                                      \
        (_dst)->y += (_a)->y;                                                                      \
        (_dst)->z += (_a)->z;                                                                      \
    } while (0)

typedef RpAtomic* (*AllInOneAtomicCB)(RpAtomic* atomic, GCNObjectData* objectData);

// Issues a draw sync token for the arrays if the GPU may still be drawing them, then waits until
// the GPU has passed their token.
#define ALLINONE_WAITDONE(_arrays)                                                                 \
    do                                                                                             \
    {                                                                                              \
        if ((_arrays)->syncToken == lbl_8025E800)                                                  \
        {                                                                                          \
            GXSetDrawSync(lbl_8025E800);                                                           \
            lbl_8025E800 = (lbl_8025E800 + 1) % ALLINONE_TOKENWRAP;                                \
        }                                                                                          \
                                                                                                   \
        while (!_rwDlTokenQueryDone((_arrays)->syncToken))                                         \
        {                                                                                          \
        }                                                                                          \
    } while (0)

// The default lighting callback: clears the lighting data, then loads the global lights and the
// local lights of every sector the atomic is in, once each, and clamps the summed ambient color.
RpAtomic* fn_8015DA50(RpAtomic* atomic, GCNObjectData* objectData)
{
    objectData->globalMask = 0;
    objectData->localMask = 0;
    objectData->ambient.red = 0.0f;
    objectData->ambient.green = 0.0f;
    objectData->ambient.blue = 0.0f;
    objectData->ambient.alpha = 1.0f;
    objectData->ambientUsed = FALSE;
    objectData->numLights = 0;

    if ((atomic->geometry->flags & rpGEOMETRYLIGHT) && RWSRCGLOBAL(curWorld))
    {
        RwLLLink* cur;
        RwLLLink* end;

        _rwGCLightsGlobalEnable(rpLIGHTLIGHTATOMICS, objectData);
        RWSRCGLOBAL(lightFrame)++;

        cur = rwLinkListGetFirstLLLink(&atomic->llWorldSectorsInAtomic);
        end = rwLinkListGetTerminator(&atomic->llWorldSectorsInAtomic);

        while (cur != end)
        {
            RpWorldSector* sector = rwLLLinkGetData(cur, WorldTie, atomicLink)->sector;
            RwLLLink* lightCur = rwLinkListGetFirstLLLink(&sector->lightsInWorldSector);
            RwLLLink* lightEnd = rwLinkListGetTerminator(&sector->lightsInWorldSector);

            while (lightCur != lightEnd)
            {
                RpLight* light = rwLLLinkGetData(lightCur, LightTie, sectorLink)->light;

                if (light && light->lightFrame != RWSRCGLOBAL(lightFrame) &&
                    rwObjectTestFlags(light, rpLIGHTLIGHTATOMICS))
                {
                    light->lightFrame = RWSRCGLOBAL(lightFrame);
                    _rwGCLightsLocalEnable(light, objectData);
                }

                lightCur = rwLLLinkGetNext(lightCur);
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

    return atomic;
}

// Writes the interpolated positions (and normals) of the interpolator's two morph targets into
// the instanced arrays, or copies one morph target when both are the same.
void fn_8015DBA4(RpGeometry* geometry, GCNVertexArrays* arrays, RpInterpolator* interpolator)
{
    S32 start = interpolator->startMorphTarget;
    S32 end = interpolator->endMorphTarget;
    S32 numVertices = geometry->numVertices;
    RwV3d* dstPos = (RwV3d*)arrays->pos;
    U8* dstNrm = (U8*)arrays->nrm;

    if (start >= geometry->numMorphTargets || end >= geometry->numMorphTargets)
    {
        end = 0;
        start = 0;
    }

    if (start == end)
    {
        RpMorphTarget* morphTarget = &geometry->morphTarget[start];
        RwV3d* srcPos = morphTarget->verts;

        if (geometry->flags & rpGEOMETRYNORMALS)
        {
            RwV3d* srcNrm = morphTarget->normals;

            while (numVertices--)
            {
                *dstPos++ = *srcPos++;
                *(RwV3d*)dstNrm = *srcNrm++;
                dstNrm += arrays->nrmStride;
            }

            DCFlushRange(arrays->pos, geometry->numVertices * sizeof(RwV3d));
            DCFlushRange(arrays->nrm, arrays->nrmStride * geometry->numVertices);
        }
        else
        {
            while (numVertices--)
            {
                *dstPos++ = *srcPos++;
            }

            DCFlushRange(arrays->pos, geometry->numVertices * sizeof(RwV3d));
        }
    }
    else
    {
        RpMorphTarget* morphTargets = geometry->morphTarget;
        F32 scale = interpolator->recipTime * interpolator->position;
        RwV3d* startPos = morphTargets[start].verts;
        RwV3d* endPos = morphTargets[end].verts;

        if (geometry->flags & rpGEOMETRYNORMALS)
        {
            RwV3d* startNrm = morphTargets[start].normals;
            RwV3d* endNrm = morphTargets[end].normals;

            while (numVertices--)
            {
                ALLINONE_LERP(dstPos, startPos, endPos, scale);
                dstPos++;
                endPos++;
                startPos++;

                ALLINONE_LERP((RwV3d*)dstNrm, startNrm, endNrm, scale);
                endNrm++;
                startNrm++;
                dstNrm += arrays->nrmStride;
            }

            DCFlushRange(arrays->pos, geometry->numVertices * sizeof(RwV3d));
            DCFlushRange(arrays->nrm, arrays->nrmStride * geometry->numVertices);
        }
        else
        {
            while (numVertices--)
            {
                ALLINONE_LERP(dstPos, startPos, endPos, scale);
                dstPos++;
                endPos++;
                startPos++;
            }

            DCFlushRange(arrays->pos, geometry->numVertices * sizeof(RwV3d));
        }
    }

    GXInvalidateVtxCache();
}

// The default reinstance callback: re-instances the attributes locked since the last instance,
// and the morph targets when the interpolator changed, once the GPU is done with the arrays.
RpAtomic* fn_8015E054(RpAtomic* atomic, GCNObjectData* objectData)
{
    RpGeometry* geometry = atomic->geometry;
    GCNVertexArrays* arrays = (GCNVertexArrays*)(objectData->repEntry + 1);

    if (geometry->lockedSinceLastInst)
    {
        U32 locked;
        S32 numVertices;
        U32 flags;

        ALLINONE_WAITDONE(arrays);

        locked = geometry->lockedSinceLastInst;
        numVertices = geometry->numVertices;
        flags = (geometry->flags | rpGEOMETRYPOSITIONS) & ~objectData->privateData->clearFlags;

        if ((locked & ALLINONE_LOCKVERTICES) && (flags & rpGEOMETRYPOSITIONS))
        {
            DCFlushRange(arrays->pos,
                         _rwGCNVtxFmtInstPos3D(fn_8015D818(geometry), arrays->pos,
                                               geometry->morphTarget->verts, numVertices,
                                               arrays->posStride));
        }

        if ((locked & ALLINONE_LOCKNORMALS) && (flags & rpGEOMETRYNORMALS))
        {
            DCFlushRange(arrays->nrm,
                         _rwGCNVtxFmtInstNrm(fn_8015D818(geometry), arrays->nrm,
                                             geometry->morphTarget->normals,
                                             geometry->numVertices, arrays->nrmStride));
        }

        if ((locked & ALLINONE_LOCKPRELIGHT) && (flags & rpGEOMETRYPRELIT))
        {
            DCFlushRange(arrays->clr,
                         _rwGCNVtxFmtInstClr(fn_8015D818(geometry), arrays->clr,
                                             geometry->preLitLum, geometry->numVertices,
                                             arrays->clrStride));
        }

        if ((locked & ALLINONE_LOCKTEXCOORDS) && (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2)))
        {
            S32 i;

            for (i = 0; i < geometry->numTexCoordSets; i++)
            {
                if ((0x10 << i) & locked)
                {
                    DCFlushRange(arrays->tex[i],
                                 _rwGCNVtxFmtInstTex(fn_8015D818(geometry), i, arrays->tex[i],
                                                     geometry->texCoords[i],
                                                     geometry->numVertices,
                                                     arrays->texStride[i]));
                }
            }
        }

        GXInvalidateVtxCache();
        geometry->lockedSinceLastInst = 0;
    }

    if (geometry->numMorphTargets != 1 &&
        (atomic->interpolator.flags & rpINTERPOLATORDIRTYINSTANCE))
    {
        ALLINONE_WAITDONE(arrays);
        fn_8015DBA4(geometry, arrays, &atomic->interpolator);
        atomic->interpolator.flags &= ~rpINTERPOLATORDIRTYINSTANCE;
    }

    return atomic;
}

// The default instance callback: allocates the entry (owned by the atomic when the geometry
// morphs, else by the geometry), builds the display lists and instances the vertex arrays.
RpAtomic* fn_8015E2E8(RpAtomic* atomic, GCNObjectData* objectData)
{
    RpGeometry* geometry = atomic->geometry;
    RpMeshHeader* meshHeader = geometry->mesh;
    U32 numVertices = geometry->numVertices;
    void* owner;
    RwResEntry** ownerRef;
    U32 geometryFlags;
    U32 flags;
    S32 headerSize;
    U32 dlSize;
    S32 size;
    RwResEntry* entry;
    GCNVertexArrays* arrays;
    U8* dlMem;

    if (geometry->numMorphTargets != 1)
    {
        owner = atomic;
        ownerRef = &atomic->repEntry;
    }
    else
    {
        owner = geometry;
        ownerRef = &geometry->repEntry;
    }

    geometryFlags = geometry->flags | rpGEOMETRYPOSITIONS;
    flags = geometryFlags & ~objectData->privateData->clearFlags;
    headerSize = meshHeader->numMeshes * sizeof(GCNDisplayList);
    dlSize = GCNDisplayListsGetSize(meshHeader, geometry->numTexCoordSets,
                                    numVertices >= 0xFF ? 2 : 1, geometryFlags);
    size = ALLINONE_ALIGN32(sizeof(GCNVertexArrays) + headerSize) + dlSize;
    size = ALLINONE_ALIGN32(size) + fn_80159380(fn_8015D818(geometry), flags, numVertices,
                                                geometry->numTexCoordSets);

    if (geometry->flags & ALLINONE_GEOMETRYOWNENTRY)
    {
        objectData->repEntry = (RwResEntry*)RwMalloc(size + sizeof(RwResEntry) + 0x20);
        objectData->repEntry->link.next = NULL;
        objectData->repEntry->link.prev = NULL;
        objectData->repEntry->owner = owner;
        objectData->repEntry->size = size + 0x20;
        objectData->repEntry->ownerRef = ownerRef;
        objectData->repEntry->destroyNotify = _rxGCResEntryWaitDone;
        *ownerRef = objectData->repEntry;
    }
    else
    {
        objectData->repEntry =
            RwResourcesAllocateResEntry(owner, ownerRef, size + 0x20, _rxGCResEntryWaitDone);
    }

    entry = objectData->repEntry;
    arrays = (GCNVertexArrays*)(entry + 1);
    memset(arrays, 0, sizeof(GCNVertexArrays));
    arrays->serialNum = meshHeader->serialNum;
    arrays->indexType = numVertices >= 0xFF ? 3 : 2;
    dlMem = (U8*)ALLINONE_ALIGN32((U32)arrays + sizeof(GCNVertexArrays) + headerSize);
    arrays->syncToken = lbl_8025F890;
    arrays->flags = objectData->privateData->clearFlags;
    GCNDisplayListsBuild((GCNDisplayList*)(arrays + 1), dlMem, meshHeader,
                         geometry->numTexCoordSets, numVertices >= 0xFF ? 2 : 1, geometryFlags);
    _rwDlGeometryInstanceFast(geometry, arrays, fn_8015D818(geometry), flags,
                              geometry->numTexCoordSets,
                              (U8*)ALLINONE_ALIGN32((U32)dlMem + dlSize));
    DCFlushRange(objectData->repEntry + 1, objectData->repEntry->size);
    geometry->lockedSinceLastInst = 0;

    return atomic;
}

// The node body: instances the atomic when its entry is missing or its meshes changed (else
// reinstances it), then runs the lighting and render callbacks.
S32 _rxGCAtomicAllInOneNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RpAtomic* atomic = (RpAtomic*)params->dataParam;
    GCNAllInOnePrivateData* privateData = (GCNAllInOnePrivateData*)self->privateData;
    RpGeometry* geometry = atomic->geometry;
    GCNObjectData objectData;

    objectData.privateData = privateData;

    if (!(geometry->flags & rpGEOMETRYNATIVE))
    {
        RwResEntry* repEntry;
        RpMeshHeader* meshHeader;

        if (!geometry->numVertices)
        {
            return TRUE;
        }

        meshHeader = geometry->mesh;

        if (!meshHeader->numMeshes)
        {
            return TRUE;
        }

        if (geometry->numMorphTargets != 1)
        {
            repEntry = atomic->repEntry;
        }
        else
        {
            repEntry = geometry->repEntry;
        }

        objectData.repEntry = repEntry;
        objectData.meshHeader = geometry->mesh;
        objectData.flags = geometry->flags;

        if (repEntry && ((GCNVertexArrays*)(repEntry + 1))->serialNum != meshHeader->serialNum)
        {
            RwResourcesFreeResEntry(repEntry);
            objectData.repEntry = NULL;
        }

        if (objectData.repEntry)
        {
            if (privateData->reinstanceCB &&
                ((AllInOneAtomicCB)privateData->reinstanceCB)(atomic, &objectData) != atomic)
            {
                return FALSE;
            }

            RwResourcesUseResEntry(objectData.repEntry);
        }
        else
        {
            if (privateData->instanceCB &&
                ((AllInOneAtomicCB)privateData->instanceCB)(atomic, &objectData) != atomic)
            {
                return FALSE;
            }

            if (privateData->reinstanceCB &&
                ((AllInOneAtomicCB)privateData->reinstanceCB)(atomic, &objectData) != atomic)
            {
                return FALSE;
            }
        }
    }
    else
    {
        objectData.repEntry = geometry->repEntry;
        objectData.meshHeader = geometry->mesh;
        objectData.flags = geometry->flags;

        if (privateData->reinstanceCB &&
            ((AllInOneAtomicCB)privateData->reinstanceCB)(atomic, &objectData) != atomic)
        {
            return FALSE;
        }
    }

    if (privateData->lightingCB &&
        ((AllInOneAtomicCB)privateData->lightingCB)(atomic, &objectData) != atomic)
    {
        return FALSE;
    }

    if (privateData->renderCB &&
        ((AllInOneAtomicCB)privateData->renderCB)(atomic, &objectData) != atomic)
    {
        return FALSE;
    }

    return TRUE;
}

S32 _rxGCAtomicAllInOnePipelineInit(RxPipelineNode* node)
{
    GCNAllInOnePrivateData* privateData = (GCNAllInOnePrivateData*)node->privateData;

    privateData->instanceCB = fn_8015E2E8;
    privateData->reinstanceCB = fn_8015E054;
    privateData->lightingCB = fn_8015DA50;
    privateData->renderCB = fn_80158FB0;
    privateData->clearFlags = 0;

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetGameCubeAtomicAllInOne(void)
{
    return &lbl_801DDA94;
}
