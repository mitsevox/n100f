#include <rpworld.h>

// The generic world sector instance node: copies a sector's vertices and meshes into the packet's
// clusters. The GameCube build uses its own all-in-one node instead; this one stays linked from
// the shared pipeline code.

// fn_80157A3C and fn_80157FB8 are the node's helpers (the June 2001 prototype names
// instanceVerts and instanceMeshes among them); they keep their labels.

// The per-mesh record the instance node builds, as in nodeAtomicInstance.c (project-own names).
typedef struct SectorInstanceMesh SectorInstanceMesh;
struct SectorInstanceMesh
{
    U16* indices;
    U32 numIndices;
    RxObjSpace3DVertex* verts;
    U32 numVerts;
    S32 stride;
    RpMaterial* material;
    RpMesh* mesh;
};

// The instance data behind a sector's resource entry (project-own names).
typedef struct SectorInstanceHeader SectorInstanceHeader;
struct SectorInstanceHeader
{
    U32 serialNum;
    SectorInstanceMesh meshes[1];
};

#define OBJVERTEXNEXT(_vert, _stride) ((RxObjSpace3DVertex*)((U8*)(_vert) + (_stride)))

// The scale from a packed normal component to a float.
#define SECTOR_NORMALSCALE (1.0f / 128.0f)

// Writes each vertex's texture coordinates (when the world is textured), prelit color (opaque
// black when not prelit), unpacked normal (when the world has normals) and position.
static void fn_80157A3C(RxObjSpace3DVertex* objVerts, RpWorld* world, RpWorldSector* sector,
                        S32 stride)
{
    static const RwRGBA defaultColor = { 0, 0, 0, 255 };
    U32 flags = ((RpWorld*)RWSRCGLOBAL(curWorld))->flags;

    if (flags & (rpWORLDTEXTURED | rpWORLDTEXTURED2))
    {
        RxObjSpace3DVertex* objVert = objVerts;
        const RwTexCoords* texCoord = sector->texCoords[0];
        U32 numVerts = sector->numVertices;

        while (numVerts--)
        {
            objVert->u = texCoord->u;
            objVert->v = texCoord->v;
            texCoord++;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }

    if (flags & rpWORLDPRELIT)
    {
        RxObjSpace3DVertex* objVert = objVerts;
        const RwRGBA* color = sector->preLitLum;
        U32 numVerts = sector->numVertices;

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
        U32 numVerts = sector->numVertices;

        while (numVerts--)
        {
            objVert->r = defaultColor.red;
            objVert->g = defaultColor.green;
            objVert->b = defaultColor.blue;
            objVert->a = defaultColor.alpha;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }

    if (flags & rpWORLDNORMALS)
    {
        RxObjSpace3DVertex* objVert = objVerts;
        const RpVertexNormal* normal = sector->normals;
        U32 numVerts = sector->numVertices;

        while (numVerts--)
        {
            objVert->nx = SECTOR_NORMALSCALE * normal->x;
            objVert->ny = SECTOR_NORMALSCALE * normal->y;
            objVert->nz = SECTOR_NORMALSCALE * normal->z;
            normal++;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }

    {
        RxObjSpace3DVertex* objVert = objVerts;
        const RwV3d* pos = sector->vertices;
        U32 numVerts = sector->numVertices;

        while (numVerts--)
        {
            objVert->x = pos->x;
            objVert->y = pos->y;
            objVert->z = pos->z;
            pos++;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }
}

// Builds a record per mesh: its indices copied (rebased to the lowest vertex the mesh uses, with
// strips unrolled to triangles and degenerate triangles dropped) or generated for unindexed
// meshes, and the range of vertices they use.
static void fn_80157FB8(SectorInstanceMesh* instMesh, RpMeshHeader* meshHeader, U16* indices,
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

// Instances the sector into its resource entry (rebuilding it when the mesh changed), then sends a
// packet per mesh with its vertices, indices, mesh state (world space, so an identity object
// matrix) and render state.
static S32 WorldSectorInstanceNodeFn(RxPipelineNode* self, RxPipelineNodeParam* params)
{
    RpWorldSector* sector = (RpWorldSector*)params->dataParam;
    RpWorld* world = (RpWorld*)RWSRCGLOBAL(curWorld);
    U32 numVerts = sector->numVertices;
    RpMeshHeader* meshHeader;
    U16 numMeshes;
    U32 flags;
    U32 textured;
    RwResEntry* repEntry;
    S32 numTris;
    SectorInstanceHeader* header;
    SectorInstanceMesh* instMesh;

    if (!numVerts)
    {
        return TRUE;
    }

    meshHeader = sector->mesh;
    numMeshes = meshHeader->numMeshes;

    if (!numMeshes)
    {
        return TRUE;
    }

    flags = world->flags;
    repEntry = sector->repEntry;

    textured = flags & (rpWORLDTEXTURED | rpWORLDTEXTURED2);

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
        header = (SectorInstanceHeader*)(repEntry + 1);

        if (header->serialNum != meshHeader->serialNum)
        {
            RwResourcesFreeResEntry(repEntry);
            repEntry = NULL;
        }
    }

    if (!repEntry)
    {
        S32 vertSize = numVerts * sizeof(RxObjSpace3DVertex);
        RxObjSpace3DVertex* verts;

        repEntry = RwResourcesAllocateResEntry(sector, &sector->repEntry,
                                               sizeof(SectorInstanceHeader) +
                                                   (numMeshes - 1) * sizeof(SectorInstanceMesh) +
                                                   numTris * 3 * sizeof(U16) + vertSize,
                                               NULL);
        header = (SectorInstanceHeader*)(repEntry + 1);
        header->serialNum = meshHeader->serialNum;
        verts = (RxObjSpace3DVertex*)&header->meshes[numMeshes];

        fn_80157A3C(verts, world, sector, sizeof(RxObjSpace3DVertex));
        fn_80157FB8(header->meshes, meshHeader, (U16*)((U8*)verts + vertSize), verts,
                    sizeof(RxObjSpace3DVertex));
    }
    else
    {
        RwResourcesUseResEntry(repEntry);
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

            meshState->Obj2Cam = RwCameraGetCurrentCamera()->viewMatrix;
            RwMatrixSetIdentityMacro(&meshState->Obj2World);

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

RxNodeDefinition* RxNodeDefinitionGetWorldSectorInstance(void)
{
    static RxClusterRef gNodeClusters[] = {
        { &RxClObjSpace3DVertices, rxCLALLOWABSENT, 0 },
        { &RxClIndices, rxCLALLOWABSENT, 0 },
        { &RxClMeshState, rxCLALLOWABSENT, 0 },
        { &RxClRenderState, rxCLALLOWABSENT, 0 },
    };

    static RxClusterValidityReq gNodeReqs[] = {
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
        rxCLREQ_DONTWANT,
    };

    static char _Output[] = "Output";

    static RxClusterValid gNodeOut1[] = {
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
        rxCLVALID_VALID,
    };

    static RxOutputSpec gNodeOuts[] = {
        { _Output, gNodeOut1, rxCLVALID_INVALID },
    };

    static char _WorldSectorInstance_csl[] = "WorldSectorInstance.csl";

    static RxNodeDefinition nodeWorldSectorInstanceCSL = {
        _WorldSectorInstance_csl,
        { WorldSectorInstanceNodeFn, NULL, NULL, NULL, NULL, NULL, NULL },
        { 4, gNodeClusters, gNodeReqs, 1, gNodeOuts },
        0,
        rxNODEDEFCONST,
        0,
    };

    return &nodeWorldSectorInstanceCSL;
}
