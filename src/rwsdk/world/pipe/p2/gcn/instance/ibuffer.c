#include <rpworld.h>
#include <dolphin/gx/GXDispList.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/gx/GXVert.h>
#include <dolphin/os.h>

#define ALIGN32(_size) (((_size) + 0x1F) & ~0x1F)

// The display list bytes for every mesh of header: one index per attribute in flags for each
// vertex, indexSize bytes each, plus the GXBegin command, each list 32-byte aligned.
U32 GCNDisplayListsGetSize(const RpMeshHeader* header, S32 numTexCoords, S32 indexSize, U32 flags)
{
    const RpMesh* mesh = (const RpMesh*)(header + 1);
    U32 numMeshes = header->numMeshes;
    U32 total = 0;

    while (numMeshes--)
    {
        U32 numIndices = mesh->numIndices;
        U32 count = numIndices;
        U32 size;

        if (flags & rpGEOMETRYNORMALS)
        {
            count += numIndices;
        }
        if (flags & rpGEOMETRYPRELIT)
        {
            count += numIndices;
        }
        if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
        {
            count += numIndices * numTexCoords;
        }

        size = indexSize * count + 3;
        if (indexSize == 1 && ALIGN32(size) == size)
        {
            size += 0x20;
        }

        total += ALIGN32(size);
        mesh++;
    }

    return total;
}

// Builds one indexed display list per mesh of header in dlMem, each vertex sending its index
// once for every attribute in flags, and records each list in lists.
void GCNDisplayListsBuild(GCNDisplayList* lists, U8* dlMem, const RpMeshHeader* header, S32 numTexCoords,
                 S32 indexSize, U32 flags)
{
    GXPrimitive primType = GX_TRIANGLES;
    GXAttrType attrType = GX_INDEX16;
    U8 numAttrs;
    S32 i;
    const RpMesh* mesh;
    U32 numMeshes;
    U32 normals;
    U32 prelit;

    if (header->flags & rpMESHHEADERTRISTRIP)
    {
        primType = GX_TRIANGLESTRIP;
    }
    if (indexSize == 1)
    {
        attrType = GX_INDEX8;
    }

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, attrType);
    numAttrs = 1;

    normals = flags & rpGEOMETRYNORMALS;
    if (normals)
    {
        GXSetVtxDesc(GX_VA_NRM, attrType);
        numAttrs = 2;
    }

    prelit = flags & rpGEOMETRYPRELIT;
    if (prelit)
    {
        GXSetVtxDesc(GX_VA_CLR0, attrType);
        numAttrs++;
    }

    for (i = 0; i < numTexCoords; i++)
    {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), attrType);
        numAttrs++;
    }

    numMeshes = header->numMeshes;
    mesh = (const RpMesh*)(header + 1);
    while (numMeshes--)
    {
        const U16* index = mesh->indices;
        U32 numIndices = mesh->numIndices;
        U32 count = numIndices;
        U32 size;
        U32 dlSize;

        if (normals)
        {
            count += numIndices;
        }
        if (prelit)
        {
            count += numIndices;
        }
        if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
        {
            count += numIndices * numTexCoords;
        }

        size = indexSize * count + 3;
        if (indexSize == 1 && ALIGN32(size) == size)
        {
            size += 0x20;
        }
        size = ALIGN32(size) + 0x20;

        DCInvalidateRange(dlMem, size);
        GXBeginDisplayList(dlMem, size);
        GXResetWriteGatherPipe();
        GXBegin(primType, GX_VTXFMT0, numIndices);

        if (indexSize == 1)
        {
            while (numIndices--)
            {
                U16 vertex = *index;
                S32 n;

                for (n = 0; n < numAttrs; n++)
                {
                    GXWGFifo.u8 = vertex;
                }
                index++;
            }
        }
        else
        {
            while (numIndices--)
            {
                U16 vertex = *index;
                S32 n;

                for (n = 0; n < numAttrs; n++)
                {
                    GXWGFifo.u16 = vertex;
                }
                index++;
            }
        }

        dlSize = GXEndDisplayList();
        lists->list = dlMem;
        dlMem += dlSize;
        lists->size = dlSize;
        lists++;
        mesh++;
    }
}
