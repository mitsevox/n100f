#include <rpworld.h>

// The stream sizes of the mesh header and of each mesh and index in it.
#define MESHHEADER_STREAMSIZE 12
#define MESH_STREAMSIZE 8
#define MESHINDEX_STREAMSIZE 4

// The number of indices _rpMeshWrite and _rpMeshRead convert per stream call.
#define MESH_INDEXBUFFERSIZE 256

// A mesh header and a world or geometry that keeps its indices in the stream (not native).
#define MESHINDICESSTREAMED(object)                                                         \
    ((((const RwObject*)(object))->type == GEOMETRY_OBJECTTYPE &&                           \
      !(((const RpGeometry*)(object))->flags & rpGEOMETRYNATIVE)) ||                        \
     (((const RwObject*)(object))->type == rpWORLD &&                                       \
      !(((const RpWorld*)(object))->flags & rpWORLDNATIVE)))

typedef struct MeshStaticData MeshStaticData;
struct MeshStaticData
{
    RwFreeList* buildMeshFreeList;
};

RwModuleInfo meshModule;
static MeshStaticData MeshStatic = { NULL };

RpMeshHeader* _rpMeshHeaderCreate(U32 size)
{
    return (RpMeshHeader*)RWSRCGLOBAL(memoryFuncs).rwmalloc(size);
}

void* _rpMeshClose(void* instance, S32 offset, S32 size)
{
    if (--meshModule.numInstances == 0)
    {
        if (MeshStatic.buildMeshFreeList)
        {
            RwFreeListDestroy(MeshStatic.buildMeshFreeList);
            MeshStatic.buildMeshFreeList = NULL;
        }
    }

    return instance;
}

void* _rpMeshOpen(void* instance, S32 offset, S32 size)
{
    meshModule.globalsOffset = offset;

    if (meshModule.numInstances == 0)
    {
        MeshStatic.buildMeshFreeList = RwFreeListCreate(sizeof(RpBuildMesh), 50, 0);

        if (!MeshStatic.buildMeshFreeList)
        {
            if (MeshStatic.buildMeshFreeList)
            {
                RwFreeListDestroy(MeshStatic.buildMeshFreeList);
                MeshStatic.buildMeshFreeList = NULL;
            }

            return NULL;
        }
    }

    MESHGLOBAL(nextSerialNum) = 1;
    meshModule.numInstances++;

    // The primitive type of each mesh header flag value (0 is a triangle list) and the
    // mesh header flags of each primitive type from the line list on.
    MESHGLOBAL(meshFlagsToPrimType)[0] = rwPRIMTYPETRILIST;
    MESHGLOBAL(meshFlagsToPrimType)[1] = rwPRIMTYPETRISTRIP;
    MESHGLOBAL(meshFlagsToPrimType)[2] = rwPRIMTYPETRIFAN;
    MESHGLOBAL(meshFlagsToPrimType)[4] = rwPRIMTYPELINELIST;
    MESHGLOBAL(meshFlagsToPrimType)[8] = rwPRIMTYPEPOLYLINE;
    MESHGLOBAL(meshFlagsToPrimType)[16] = rwPRIMTYPEPOINTLIST;

    MESHGLOBAL(primTypeToMeshFlags)[rwPRIMTYPELINELIST - 1] = 4;
    MESHGLOBAL(primTypeToMeshFlags)[rwPRIMTYPEPOLYLINE - 1] = 8;
    MESHGLOBAL(primTypeToMeshFlags)[rwPRIMTYPETRILIST - 1] = 0;
    MESHGLOBAL(primTypeToMeshFlags)[rwPRIMTYPETRISTRIP - 1] = 1;
    MESHGLOBAL(primTypeToMeshFlags)[rwPRIMTYPETRIFAN - 1] = 2;
    MESHGLOBAL(primTypeToMeshFlags)[rwPRIMTYPEPOINTLIST - 1] = 16;

    return instance;
}

RpBuildMesh* _rpBuildMeshCreate(U32 bufferSize)
{
    RpBuildMesh* mesh;
    U32 size;

    mesh = (RpBuildMesh*)RWSRCGLOBAL(memoryAlloc)(MeshStatic.buildMeshFreeList);

    if (mesh)
    {
        mesh->numTriangles = 0;

        if (bufferSize)
        {
            size = bufferSize * sizeof(RpBuildMeshTriangle);
            mesh->meshTriangles = (RpBuildMeshTriangle*)RwMalloc(size);

            if (!mesh->meshTriangles)
            {
                RwError err;

                RWSRCGLOBAL(memoryFree)(MeshStatic.buildMeshFreeList, mesh);

                err.pluginID = 2;
                err.errorCode = _rwerror(0x80000013, size);
                RwErrorSet(&err);

                return NULL;
            }

            mesh->triangleBufferSize = bufferSize;
        }
        else
        {
            mesh->meshTriangles = NULL;
            mesh->triangleBufferSize = 0;
        }

        return mesh;
    }
    else
    {
        RwError err;

        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000013, sizeof(RpBuildMesh));
        RwErrorSet(&err);

        return NULL;
    }
}

S32 _rpBuildMeshDestroy(RpBuildMesh* mesh)
{
    if (mesh->meshTriangles)
    {
        RwFree(mesh->meshTriangles);
        mesh->meshTriangles = NULL;
    }

    RWSRCGLOBAL(memoryFree)(MeshStatic.buildMeshFreeList, mesh);

    return 1;
}

S32 _rpMeshDestroy(RpMeshHeader* mesh)
{
    if (mesh->flags || mesh->numMeshes || mesh->serialNum || mesh->totalIndicesInMesh ||
        mesh->firstMeshOffset)
    {
        RwFree(mesh);
    }

    return 1;
}

RpBuildMesh* _rpBuildMeshAddTriangle(RpBuildMesh* mesh, RpMaterial* material, S32 vert1,
                                     S32 vert2, S32 vert3)
{
    if (mesh->numTriangles >= mesh->triangleBufferSize)
    {
        RpBuildMeshTriangle* newTriangles;
        U32 size = (mesh->numTriangles + 1) * sizeof(RpBuildMeshTriangle);

        if (mesh->numTriangles)
        {
            newTriangles = (RpBuildMeshTriangle*)RwRealloc(mesh->meshTriangles, size);
        }
        else
        {
            newTriangles = (RpBuildMeshTriangle*)RwMalloc(size);
        }

        if (!newTriangles)
        {
            RwError err;

            err.pluginID = 2;
            err.errorCode = _rwerror(0x80000013, size);
            RwErrorSet(&err);

            return NULL;
        }

        mesh->meshTriangles = newTriangles;
        mesh->triangleBufferSize = mesh->numTriangles + 1;
    }

    mesh->meshTriangles[mesh->numTriangles].material = material;
    mesh->meshTriangles[mesh->numTriangles].vertIndex[0] = (U16)vert1;
    mesh->meshTriangles[mesh->numTriangles].vertIndex[1] = (U16)vert2;
    mesh->meshTriangles[mesh->numTriangles].vertIndex[2] = (U16)vert3;
    mesh->numTriangles++;

    return mesh;
}

RpMeshHeader* _rpMeshHeaderForAllMeshes(RpMeshHeader* meshHeader, RpMeshCallBack fpCallBack,
                                        void* pData)
{
    S32 numMeshes;
    RpMesh* mesh;

    mesh = (RpMesh*)((U8*)(meshHeader + 1) + meshHeader->firstMeshOffset);
    numMeshes = meshHeader->numMeshes;

    while (numMeshes--)
    {
        if (!fpCallBack(mesh, meshHeader, pData))
        {
            return meshHeader;
        }

        mesh++;
    }

    return meshHeader;
}

RwStream* _rpMeshWrite(const RpMeshHeader* meshHeader, const void* object, RwStream* stream,
                       const RpMaterialList* matList)
{
    S32 header[3];
    S32 meshInfo[2];
    S32 buffer[MESH_INDEXBUFFERSIZE];
    U32 numMeshes;
    const RpMesh* mesh;

    header[0] = meshHeader->flags;
    header[1] = meshHeader->numMeshes;
    header[2] = meshHeader->totalIndicesInMesh;

    if (!RwStreamWriteInt32(stream, header, sizeof(header)))
    {
        return NULL;
    }

    numMeshes = meshHeader->numMeshes;
    mesh = (const RpMesh*)(meshHeader + 1);

    while (numMeshes--)
    {
        meshInfo[0] = mesh->numIndices;
        meshInfo[1] = _rpMaterialListFindMaterialIndex(matList, mesh->material);

        if (meshInfo[1] < 0)
        {
            meshInfo[1] = 0;
        }

        if (!RwStreamWriteInt32(stream, meshInfo, sizeof(meshInfo)))
        {
            return NULL;
        }

        if (MESHINDICESSTREAMED(object))
        {
            U32 remaining = mesh->numIndices;
            const U16* indices = mesh->indices;

            while (remaining)
            {
                U32 count = (remaining < MESH_INDEXBUFFERSIZE) ? remaining : MESH_INDEXBUFFERSIZE;
                U32 i;

                for (i = 0; i < count; i++)
                {
                    buffer[i] = *indices++;
                }

                if (!RwStreamWriteInt32(stream, buffer, count * sizeof(S32)))
                {
                    return NULL;
                }

                remaining -= count;
            }
        }

        mesh++;
    }

    return stream;
}

RpMeshHeader* _rpMeshRead(RwStream* stream, const void* object, const RpMaterialList* matList)
{
    S32 header[3];
    S32 meshInfo[2];
    S32 buffer[MESH_INDEXBUFFERSIZE];
    U32 size;
    RpMeshHeader* meshHeader;
    RpMesh* mesh;
    U16* indices;
    U32 numMeshes;

    if (!RwStreamReadInt32(stream, header, sizeof(header)))
    {
        return NULL;
    }

    // The block holds the header, then 16 bytes for each mesh, then the indices.
    size = sizeof(RpMeshHeader) + header[1] * 16;

    if (MESHINDICESSTREAMED(object))
    {
        size += header[2] * sizeof(U16);
    }

    meshHeader = _rpMeshHeaderCreate(size);

    if (meshHeader)
    {
        mesh = (RpMesh*)(meshHeader + 1);
        indices = (U16*)(mesh + header[1]);

        meshHeader->flags = header[0];
        meshHeader->numMeshes = (U16)header[1];
        meshHeader->serialNum = MESHGLOBAL(nextSerialNum);
        meshHeader->totalIndicesInMesh = header[2];
        meshHeader->firstMeshOffset = 0;
        MESHGLOBAL(nextSerialNum)++;

        numMeshes = meshHeader->numMeshes;

        while (numMeshes--)
        {
            if (!RwStreamReadInt32(stream, meshInfo, sizeof(meshInfo)))
            {
                return NULL;
            }

            mesh->numIndices = meshInfo[0];
            mesh->material = _rpMaterialListGetMaterial(matList, meshInfo[1]);
            mesh->indices = indices;

            if (MESHINDICESSTREAMED(object))
            {
                U32 remaining = mesh->numIndices;

                while (remaining)
                {
                    S32* src = buffer;
                    U32 count = (remaining < MESH_INDEXBUFFERSIZE) ? remaining : MESH_INDEXBUFFERSIZE;

                    if (!RwStreamReadInt32(stream, src, count * sizeof(S32)))
                    {
                        return NULL;
                    }

                    remaining -= count;

                    while (count--)
                    {
                        *indices++ = (U16)*src++;
                    }
                }
            }

            mesh++;
        }
    }

    return meshHeader;
}

S32 _rpMeshSize(const RpMeshHeader* meshHeader, const void* object)
{
    const RwObject* owner = (const RwObject*)object;

    if ((owner->type == GEOMETRY_OBJECTTYPE &&
         (((const RpGeometry*)object)->flags & rpGEOMETRYNATIVE)) ||
        (owner->type == rpWORLD && (((const RpWorld*)object)->flags & rpWORLDNATIVE)))
    {
        return MESHHEADER_STREAMSIZE + meshHeader->numMeshes * MESH_STREAMSIZE;
    }

    return MESHHEADER_STREAMSIZE + meshHeader->numMeshes * MESH_STREAMSIZE +
           meshHeader->totalIndicesInMesh * MESHINDEX_STREAMSIZE;
}
