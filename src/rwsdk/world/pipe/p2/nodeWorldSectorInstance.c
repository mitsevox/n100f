#include <rpworld.h>

// The generic world sector instance node: copies a sector's vertices and meshes into the packet's
// clusters. The GameCube build uses its own all-in-one node instead; this one stays linked from
// the shared pipeline code.

// The helpers instanceVerts and instanceMeshes take their names from the PS2 prototype symbol
// tables, which list them in this order before WorldSectorInstanceNodeFn.

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
static void instanceVerts(RxObjSpace3DVertex* objVerts, RpWorld* world, RpWorldSector* sector,
                          S32 stride)
{
    static const RwRGBA defaultColor = { 0, 0, 0, 255 };
    RxObjSpace3DVertex* objVert;
    U32 flags;
    const RwTexCoords* texCoord;
    U32 numVerts;
    const RwRGBA* color;
    const RpVertexNormal* normal;
    const RwV3d* pos;

    flags = ((RpWorld*)RWSRCGLOBAL(curWorld))->flags;

    if (flags & (rpWORLDTEXTURED | rpWORLDTEXTURED2))
    {
        objVert = objVerts;
        texCoord = sector->texCoords[0];
        numVerts = sector->numVertices;

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
        objVert = objVerts;
        color = sector->preLitLum;
        numVerts = sector->numVertices;

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
        objVert = objVerts;
        numVerts = sector->numVertices;

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
        objVert = objVerts;
        normal = sector->normals;
        numVerts = sector->numVertices;

        while (numVerts--)
        {
            F32 x = SECTOR_NORMALSCALE * normal->x;
            F32 y = SECTOR_NORMALSCALE * normal->y;
            F32 z = SECTOR_NORMALSCALE * normal->z;

            objVert->nx = x;
            objVert->ny = y;
            objVert->nz = z;
            normal++;
            objVert = OBJVERTEXNEXT(objVert, stride);
        }
    }

    objVert = objVerts;
    pos = sector->vertices;
    numVerts = sector->numVertices;

    while (numVerts--)
    {
        objVert->x = pos->x;
        objVert->y = pos->y;
        objVert->z = pos->z;
        pos++;
        objVert = OBJVERTEXNEXT(objVert, stride);
    }
}

// Builds a record per mesh: its indices copied (rebased to the lowest vertex the mesh uses, with
// strips unrolled to triangles and degenerate triangles dropped) or generated for unindexed
// meshes, and the range of vertices they use.
static void instanceMeshes(SectorInstanceMesh* instMesh, RpMeshHeader* meshHeader, U16* indices,
                           RxObjSpace3DVertex* verts, S32 stride)
{
    U32 numIndices;
    U32 numMeshes;
    RpMesh* mesh;
    S32 triStrip;
    U32 vertIndex;
    U32 maxIndex;
    U16* meshIndices;
    U32 count;

    mesh = (RpMesh*)(meshHeader + 1);
    numMeshes = meshHeader->numMeshes;
    vertIndex = 0;
    triStrip = meshHeader->flags & rpMESHHEADERTRISTRIP;

    while (numMeshes--)
    {
        meshIndices = mesh->indices;

        instMesh->indices = indices;
        instMesh->stride = stride;
        instMesh->material = mesh->material;
        instMesh->mesh = mesh;

        count = numIndices = mesh->numIndices;

        if (meshHeader->flags & rpMESHHEADERUNINDEXED)
        {
            instMesh->verts = (RxObjSpace3DVertex*)((U8*)verts + vertIndex * stride);
            instMesh->numVerts = mesh->numIndices;

            if (triStrip)
            {
                U16 i;

                instMesh->numIndices = (numIndices - 2) * 3;

                for (i = 0; i < numIndices - 2; i++)
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

                    for (i = 0; i < count; i++)
                    {
                        *indices++ = i;
                    }
                }
            }

            vertIndex += count;
        }
        else
        {
            maxIndex = 0;
            vertIndex = 0xFFFF;

            while (count--)
            {
                vertIndex = (vertIndex < meshIndices[count]) ? vertIndex : meshIndices[count];
                maxIndex = (maxIndex > meshIndices[count]) ? maxIndex : meshIndices[count];
            }

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
                U32 i;

                instMesh->numIndices = numIndices;

                for (i = 2; i < numIndices; i += 3)
                {
                    indices[0] = meshIndices[0] - vertIndex;
                    indices[1] = meshIndices[1] - vertIndex;
                    indices[2] = meshIndices[2] - vertIndex;
                    indices += 3;
                    meshIndices += 3;
                }
            }

            instMesh->verts = (RxObjSpace3DVertex*)((U8*)verts + vertIndex * stride);
            instMesh->numVerts = maxIndex - vertIndex + 1;
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
    RpWorldSector* sector;
    RpWorld* world;
    SectorInstanceHeader* header;
    RpMeshHeader* meshHeader;
    U32 textured;
    U32 numMeshes;
    U32 numVerts;
    SectorInstanceMesh* instMesh;
    U32 flags;
    S32 stride;
    RxObjSpace3DVertex* verts;
    S32 numTris;
    RwResEntry* repEntry;
    RxPacket* packet;
    RxCluster* objVertsCl;
    RxCluster* indicesCl;
    RxCluster* meshStateCl;
    RxCluster* renderStateCl;
    RxMeshStateVector* meshState;
    RxRenderStateVector* renderState;
    RpMaterial* material;
    RwTexture* texture;

    sector = (RpWorldSector*)params->dataParam;
    world = (RpWorld*)RWSRCGLOBAL(curWorld);
    numVerts = sector->numVertices;

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

    stride = sizeof(RxObjSpace3DVertex);

    if (flags & rpWORLDNORMALS)
    {
        stride = sizeof(RxObjSpace3DVertex);
    }

    textured = flags & (rpWORLDTEXTURED | rpWORLDTEXTURED2);

    if (textured)
    {
        stride = sizeof(RxObjSpace3DVertex);
    }

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
        repEntry = RwResourcesAllocateResEntry(sector, &sector->repEntry,
                                               sizeof(SectorInstanceHeader) +
                                                   (numMeshes - 1) * sizeof(SectorInstanceMesh) +
                                                   numTris * 3 * sizeof(U16) + numVerts * stride,
                                               NULL);
        header = (SectorInstanceHeader*)(repEntry + 1);
        header->serialNum = meshHeader->serialNum;
        verts = (RxObjSpace3DVertex*)&header->meshes[numMeshes];

        instanceVerts(verts, world, sector, stride);
        instanceMeshes(header->meshes, meshHeader, (U16*)((U8*)verts + numVerts * stride), verts,
                       stride);
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
            packet = RxPacketCreate(self);
            objVertsCl = RxClusterLockWrite(packet, 0, self);
            indicesCl = RxClusterLockWrite(packet, 1, self);
            meshStateCl = RxClusterLockWrite(packet, 2, self);
            renderStateCl = RxClusterLockWrite(packet, 3, self);

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
                texture = meshState->Texture;

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
