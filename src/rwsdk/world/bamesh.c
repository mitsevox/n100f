#include <rpworld.h>

#define rpWORLDNATIVE 0x01000000

// The object types _rpMeshSize distinguishes: a world and a geometry.
#define MESHOWNER_WORLD 7
#define MESHOWNER_GEOMETRY 8

// The stream sizes of the mesh header and of each mesh and index in it.
#define MESHHEADER_STREAMSIZE 12
#define MESH_STREAMSIZE 8
#define MESHINDEX_STREAMSIZE 4

RpMeshHeader* _rpMeshHeaderCreate(U32 size)
{
    return (RpMeshHeader*)RWSRCGLOBAL(memoryFuncs).rwmalloc(size);
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

S32 _rpMeshSize(const RpMeshHeader* meshHeader, const void* object)
{
    const RwObject* owner = (const RwObject*)object;

    if ((owner->type == MESHOWNER_GEOMETRY &&
         (((const RpGeometry*)object)->flags & rpGEOMETRYNATIVE)) ||
        (owner->type == MESHOWNER_WORLD && (((const RpWorld*)object)->flags & rpWORLDNATIVE)))
    {
        return MESHHEADER_STREAMSIZE + meshHeader->numMeshes * MESH_STREAMSIZE;
    }

    return MESHHEADER_STREAMSIZE + meshHeader->numMeshes * MESH_STREAMSIZE +
           meshHeader->totalIndicesInMesh * MESHINDEX_STREAMSIZE;
}
