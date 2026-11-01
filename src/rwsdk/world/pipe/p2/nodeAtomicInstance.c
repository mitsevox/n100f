#include <rpworld.h>

// The generic atomic instance node: copies an atomic's vertices (blending morph targets when the
// atomic is between two) and its meshes into the packet's clusters. The GameCube build uses its
// own all-in-one node instead; this one stays linked from the shared pipeline code.

// fn_80154994, fn_80154E8C and fn_801551A8 are the node's helpers (the June 2001 prototype names
// instanceVertPos and instanceMeshes among them); which label is which is not established, so they
// keep their labels.

// The per-mesh record the instance node builds for each mesh: its indices, the vertices they use
// and the mesh it came from (project-own names).
typedef struct AtomicInstanceMesh AtomicInstanceMesh;
struct AtomicInstanceMesh
{
    U16* indices;
    U32 numIndices;
    RxObjSpace3DVertex* verts;
    U32 numVerts;
    S32 stride;
    RpMaterial* material;
    RpMesh* mesh;
};

#define OBJVERTEXNEXT(_vert, _stride) ((RxObjSpace3DVertex*)((U8*)(_vert) + (_stride)))

// Writes each vertex's position and normal (zero without normals) from the atomic's current
// morph target, or blended between its start and end targets at the interpolator's position.
static void fn_80154994(RxObjSpace3DVertex* objVert, RpAtomic* atomic, RpGeometry* geometry,
                        S32 stride)
{
    static const RwV3d zeroNormal = { 0.0f, 0.0f, 0.0f };
    S16 start = atomic->interpolator.startMorphTarget;
    S16 end = atomic->interpolator.endMorphTarget;
    S32 numVerts = geometry->numVertices;

    if (start >= geometry->numMorphTargets || end >= geometry->numMorphTargets)
    {
        start = end = 0;
    }

    if (start == end)
    {
        RpMorphTarget* morphTarget = &geometry->morphTarget[start];
        const RwV3d* pos = morphTarget->verts;

        if (geometry->flags & rpGEOMETRYNORMALS)
        {
            const RwV3d* normal = morphTarget->normals;

            while (numVerts--)
            {
                objVert->x = pos->x;
                objVert->y = pos->y;
                objVert->z = pos->z;
                pos++;
                objVert->nx = normal->x;
                objVert->ny = normal->y;
                objVert->nz = normal->z;
                normal++;
                objVert = OBJVERTEXNEXT(objVert, stride);
            }
        }
        else
        {
            while (numVerts--)
            {
                objVert->x = pos->x;
                objVert->y = pos->y;
                objVert->z = pos->z;
                pos++;
                objVert->nx = zeroNormal.x;
                objVert->ny = zeroNormal.y;
                objVert->nz = zeroNormal.z;
                objVert = OBJVERTEXNEXT(objVert, stride);
            }
        }
    }
    else
    {
        RpMorphTarget* startTarget = &geometry->morphTarget[start];
        RpMorphTarget* endTarget = &geometry->morphTarget[end];
        F32 scale = atomic->interpolator.recipTime * atomic->interpolator.position;
        const RwV3d* startPos = startTarget->verts;
        const RwV3d* endPos = endTarget->verts;

        if (geometry->flags & rpGEOMETRYNORMALS)
        {
            const RwV3d* startNormal = startTarget->normals;
            const RwV3d* endNormal = endTarget->normals;

            while (numVerts--)
            {
                objVert->x = (endPos->x - startPos->x) * scale + startPos->x;
                objVert->y = (endPos->y - startPos->y) * scale + startPos->y;
                objVert->z = (endPos->z - startPos->z) * scale + startPos->z;
                startPos++;
                endPos++;
                objVert->nx = (endNormal->x - startNormal->x) * scale + startNormal->x;
                objVert->ny = (endNormal->y - startNormal->y) * scale + startNormal->y;
                objVert->nz = (endNormal->z - startNormal->z) * scale + startNormal->z;
                startNormal++;
                endNormal++;
                objVert = OBJVERTEXNEXT(objVert, stride);
            }
        }
        else
        {
            while (numVerts--)
            {
                objVert->x = (endPos->x - startPos->x) * scale + startPos->x;
                objVert->y = (endPos->y - startPos->y) * scale + startPos->y;
                objVert->z = (endPos->z - startPos->z) * scale + startPos->z;
                startPos++;
                endPos++;
                objVert->nx = zeroNormal.x;
                objVert->ny = zeroNormal.y;
                objVert->nz = zeroNormal.z;
                objVert = OBJVERTEXNEXT(objVert, stride);
            }
        }
    }
}

// Writes each vertex's texture coordinates (when textured) and prelit color (opaque black when not
// prelit), then its position and normal.
static void fn_80154E8C(RxObjSpace3DVertex* objVerts, RpAtomic* atomic, RpGeometry* geometry,
                        S32 stride)
{
    static const RwRGBA defaultColor = { 0, 0, 0, 255 };
    U32 flags = geometry->flags;

    if (flags & rpGEOMETRYTEXTURED)
    {
        RxObjSpace3DVertex* objVert = objVerts;
        const RwTexCoords* texCoord = geometry->texCoords[0];
        S32 numVerts = geometry->numVertices;

        while (numVerts--)
        {
            objVert->u = texCoord->u;
            objVert->v = texCoord->v;
            texCoord++;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }

    if (flags & rpGEOMETRYPRELIT)
    {
        RxObjSpace3DVertex* objVert = objVerts;
        const RwRGBA* color = geometry->preLitLum;
        S32 numVerts = geometry->numVertices;

        while (numVerts--)
        {
            objVert->r = color->red;
            objVert->g = color->green;
            objVert->b = color->blue;
            objVert->a = color->alpha;
            color++;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }
    else
    {
        RxObjSpace3DVertex* objVert = objVerts;
        S32 numVerts = geometry->numVertices;

        while (numVerts--)
        {
            objVert->r = defaultColor.red;
            objVert->g = defaultColor.green;
            objVert->b = defaultColor.blue;
            objVert->a = defaultColor.alpha;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }

    fn_80154994(objVerts, atomic, geometry, stride);
}

// Builds a record per mesh: its indices copied (rebased to the lowest vertex the mesh uses, with
// strips unrolled to triangles and degenerate triangles dropped) or generated for unindexed
// meshes, and the range of vertices they use.
static void fn_801551A8(AtomicInstanceMesh* instMesh, RpMeshHeader* meshHeader, U16* indices,
                        RxObjSpace3DVertex* verts, S32 stride)
{
    RpMesh* mesh = (RpMesh*)(meshHeader + 1);
    U16 numMeshes = meshHeader->numMeshes;
    U32 vertIndex = 0;
    U32 triStrip = meshHeader->flags & rpMESHHEADERTRISTRIP;

    while (numMeshes--)
    {
        U16* meshIndices = mesh->indices;
        U32 numIndices;

        instMesh->indices = indices;
        instMesh->stride = stride;
        instMesh->material = mesh->material;
        instMesh->mesh = mesh;

        numIndices = mesh->numIndices;

        if (meshHeader->flags & rpMESHHEADERUNINDEXED)
        {
            instMesh->verts = (RxObjSpace3DVertex*)((U8*)verts + vertIndex * stride);
            instMesh->numVerts = mesh->numIndices;

            if (triStrip)
            {
                U32 numTris = numIndices - 2;
                U16 i;

                instMesh->numIndices = numTris * 3;

                for (i = 0; i < numTris; i++)
                {
                    U32 odd = i & 1;

                    indices[0] = i + odd;
                    indices[1] = i + (odd ^ 1);
                    indices[2] = i + 2;
                    indices += 3;
                }
            }
            else
            {
                instMesh->numIndices = numIndices;

                if (meshHeader->flags & rpMESHHEADERUNINDEXED)
                {
                    U16 i;

                    for (i = 0; i < numIndices; i++)
                    {
                        *indices++ = i;
                    }
                }
            }

            vertIndex += numIndices;
        }
        else
        {
            U32 maxIndex = 0;
            U16* index = meshIndices + numIndices;
            U32 count = numIndices;

            vertIndex = 0xFFFF;

            while (count--)
            {
                U32 value = *--index;

                vertIndex = (vertIndex < value) ? vertIndex : value;
                maxIndex = (maxIndex > value) ? maxIndex : value;
            }

            instMesh->verts = (RxObjSpace3DVertex*)((U8*)verts + vertIndex * stride);
            instMesh->numVerts = maxIndex - vertIndex + 1;

            if (triStrip)
            {
                U32 swap = 0;

                instMesh->numIndices = (numIndices - 2) * 3;

                for (; numIndices > 2; numIndices--)
                {
                    indices[0] = meshIndices[swap] - vertIndex;
                    indices[1] = meshIndices[swap ^ 1] - vertIndex;
                    indices[2] = meshIndices[2] - vertIndex;

                    if (indices[0] == indices[1] || indices[0] == indices[2] ||
                        indices[1] == indices[2])
                    {
                        instMesh->numIndices -= 3;
                    }
                    else
                    {
                        indices += 3;
                    }

                    swap ^= 1;
                    meshIndices++;
                }
            }
            else
            {
                U32 numTris = numIndices / 3;

                instMesh->numIndices = numIndices;

                if (numIndices > 2)
                {
                    while (numTris--)
                    {
                        indices[0] = meshIndices[0] - vertIndex;
                        indices[1] = meshIndices[1] - vertIndex;
                        indices[2] = meshIndices[2] - vertIndex;
                        indices += 3;
                        meshIndices += 3;
                    }
                }
            }
        }

        instMesh++;
        mesh++;
    }
}

// The instance data behind an atomic's (or a single-target geometry's) resource entry: the mesh
// serial number it was built for, a record per mesh, then the vertices and the triangle indices
// (project-own names).
typedef struct AtomicInstanceHeader AtomicInstanceHeader;
struct AtomicInstanceHeader
{
    U32 serialNum;
    AtomicInstanceMesh meshes[1];
};

// Instances the atomic into its resource entry (rebuilding it when the mesh changed, or updating
// what the geometry's lock flags and the interpolator say is stale), then sends a packet per mesh
// with its vertices, indices, mesh state and render state.
static S32 AtomicInstanceNode(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RpAtomic* atomic = (RpAtomic*)params->dataParam;
    RpGeometry* geometry = atomic->geometry;
    S32 numVerts = geometry->numVertices;
    RpMeshHeader* meshHeader;
    U16 numMeshes;
    U32 flags;
    U32 textured;
    RwResEntry* repEntry;
    void* owner;
    RwResEntry** ownerRef;
    S32 numTris;
    AtomicInstanceHeader* header;
    RxObjSpace3DVertex* verts;
    U16* indices;
    AtomicInstanceMesh* instMesh;

    if (!numVerts)
    {
        return TRUE;
    }

    meshHeader = geometry->mesh;
    numMeshes = meshHeader->numMeshes;

    if (!numMeshes)
    {
        return TRUE;
    }

    flags = geometry->flags;

    if (geometry->numMorphTargets != 1)
    {
        repEntry = atomic->repEntry;
        owner = atomic;
        ownerRef = &atomic->repEntry;
    }
    else
    {
        repEntry = geometry->repEntry;
        owner = geometry;
        ownerRef = &geometry->repEntry;
    }

    textured = flags & rpGEOMETRYTEXTURED;

    if (meshHeader->flags & rpMESHHEADERTRISTRIP)
    {
        numTris = meshHeader->totalIndicesInMesh - numMeshes * 2;
    }
    else
    {
        numTris = meshHeader->totalIndicesInMesh / 3;
    }

    if (repEntry)
    {
        header = (AtomicInstanceHeader*)(repEntry + 1);
        verts = (RxObjSpace3DVertex*)&header->meshes[numMeshes];
        indices = (U16*)(verts + numVerts);

        if (header->serialNum != meshHeader->serialNum)
        {
            RwResourcesFreeResEntry(repEntry);
            repEntry = NULL;
        }
    }

    if (!repEntry)
    {
        S32 vertSize = numVerts * sizeof(RxObjSpace3DVertex);

        repEntry = RwResourcesAllocateResEntry(owner, ownerRef,
                                               sizeof(AtomicInstanceHeader) +
                                                   (numMeshes - 1) * sizeof(AtomicInstanceMesh) +
                                                   vertSize + numTris * 3 * sizeof(U16),
                                               NULL);
        header = (AtomicInstanceHeader*)(repEntry + 1);
        header->serialNum = meshHeader->serialNum;
        verts = (RxObjSpace3DVertex*)&header->meshes[numMeshes];

        fn_80154E8C(verts, atomic, geometry, sizeof(RxObjSpace3DVertex));
        fn_801551A8(header->meshes, meshHeader, (U16*)((U8*)verts + vertSize), verts,
                    sizeof(RxObjSpace3DVertex));

        geometry->lockedSinceLastInst = 0;
    }
    else
    {
        U32 dirty;

        RwResourcesUseResEntry(repEntry);

        dirty = geometry->lockedSinceLastInst;

        if (atomic->interpolator.flags & rpINTERPOLATORDIRTYINSTANCE)
        {
            dirty |= GEOMETRY_LOCKVERTICES | GEOMETRY_LOCKNORMALS;
            atomic->interpolator.flags &= ~rpINTERPOLATORDIRTYINSTANCE;
        }

        switch (dirty)
        {
        case 0:
            break;
        case GEOMETRY_LOCKVERTICES:
        case GEOMETRY_LOCKNORMALS:
        case GEOMETRY_LOCKVERTICES | GEOMETRY_LOCKNORMALS:
            fn_80154994(verts, atomic, geometry, sizeof(RxObjSpace3DVertex));
            break;
        default:
            fn_80154E8C(verts, atomic, geometry, sizeof(RxObjSpace3DVertex));
            fn_801551A8(header->meshes, meshHeader, indices, verts, sizeof(RxObjSpace3DVertex));
            break;
        }

        geometry->lockedSinceLastInst = 0;
    }

    instMesh = header->meshes;

    while (numMeshes--)
    {
        if (instMesh->numVerts && instMesh->numIndices)
        {
            RxPacket* packet = RxPacketCreate(self);
            RxCluster* objVertsCl = RxClusterLockWrite(packet, 0, self);
            RxCluster* indicesCl = RxClusterLockWrite(packet, 1, self);
            RxCluster* meshStateCl = RxClusterLockWrite(packet, 2, self);
            RxCluster* renderStateCl = RxClusterLockWrite(packet, 3, self);
            RxMeshStateVector* meshState;
            RxRenderStateVector* renderState;
            RpMaterial* material;

            meshStateCl = RxClusterInitializeData(meshStateCl, 1, sizeof(RxMeshStateVector));
            renderStateCl = RxClusterInitializeData(renderStateCl, 1, sizeof(RxRenderStateVector));

            RxClusterSetExternalData(objVertsCl, instMesh->verts, instMesh->stride,
                                     instMesh->numVerts);
            RxClusterSetExternalData(indicesCl, instMesh->indices, sizeof(U16),
                                     instMesh->numIndices);

            material = instMesh->material;
            meshState = (RxMeshStateVector*)meshStateCl->currentData;
            meshState->SourceObject = material;
            meshState->DataObject = params->dataParam;
            meshState->PrimType = rwPRIMTYPETRILIST;
            meshState->NumVertices = instMesh->numVerts;
            meshState->NumElements = instMesh->numIndices / 3;

            meshState->Obj2Cam = *RwFrameGetLTM(RpAtomicGetFrame(atomic));
            RwMatrixTransform(&meshState->Obj2Cam, &RwCameraGetCurrentCamera()->viewMatrix,
                              rwCOMBINEPOSTCONCAT);
            meshState->Obj2World = *RwFrameGetLTM(RpAtomicGetFrame(atomic));

            meshState->SurfaceProperties = material->surfaceProps;
            meshState->Flags = flags;
            meshState->Texture = material->texture;
            meshState->MatCol = material->color;
            meshState->Pipeline = material->pipeline;
            meshState->ClipFlagsAnd = 0;
            meshState->ClipFlagsOr = 0;
            meshState->SourceMesh = instMesh->mesh;
            meshStateCl->numUsed++;

            renderState = (RxRenderStateVector*)renderStateCl->currentData;
            *renderState = RXPIPELINEGLOBAL(defaultRenderState);

            if (textured)
            {
                RwTexture* texture = meshState->Texture;

                if (texture)
                {
                    renderState->TextureRaster = texture->raster;
                    renderState->AddressModeU =
                        (RwTextureAddressMode)((texture->filterAddressing >> 8) & 0xF);
                    renderState->AddressModeV =
                        (RwTextureAddressMode)((texture->filterAddressing >> 12) & 0xF);
                    renderState->FilterMode =
                        (RwTextureFilterMode)(texture->filterAddressing & 0xFF);
                }
            }

            renderStateCl->numUsed++;

            if (meshState->MatCol.alpha != 0xFF)
            {
                renderState->Flags |= RSV_FLAG_VERTEXALPHA;
            }

            RxPacketDispatch(packet, 0, self);
        }

        instMesh++;
    }

    return TRUE;
}

RxNodeDefinition* RxNodeDefinitionGetAtomicInstance(void)
{
    static RxClusterRef N1clofinterest[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq N1inputreqs[] = {
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
    };

    static char _AtomicInstanceOut[] = "AtomicInstanceOut";

    static RxClusterValid N1outcl1[] = {
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static RxOutputSpec N1outputs[] = {
        { _AtomicInstanceOut, N1outcl1, rxCLVALID_NOCHANGE },
    };

    static char _AtomicInstance_csl[] = "AtomicInstance.csl";

    static RxNodeDefinition nodeAtomicInstanceCSL = {
        _AtomicInstance_csl,
        { AtomicInstanceNode, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, N1clofinterest, N1inputreqs, 1, N1outputs },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeAtomicInstanceCSL;
}
