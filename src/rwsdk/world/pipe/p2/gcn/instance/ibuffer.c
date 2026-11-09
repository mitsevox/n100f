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

// Write one 8-bit or 16-bit vertex index to the write gather pipe. Names written for this
// project.
static inline void GCNDisplayListIndex8(const U8 index)
{
    GXWGFifo.u8 = index;
}

static inline void GCNDisplayListIndex16(const U16 index)
{
    GXWGFifo.u16 = index;
}

// Builds one indexed display list per mesh of header in dlMem, each vertex sending its index
// once for every attribute in flags, and records each list in lists.
//
// FAKEMATCH: the six arguments are copied into register locals with inline asm `or` (encoded as
// mr) and the locals are declared in a chosen order; the attribute count, its count-8 unroll
// limit, the textured test and the per-mesh index count are named locals; and both per-vertex
// index loops are inline asm, written as the compiler's own 8-way unrolled ctr loop. In the
// 16-bit loop the compiler folds the redundant clrlwi of the halfword and then drops what it no
// longer sees used, so those instructions (the loop counter in r6, the masked copy in r5, its
// stores and the pipe address before them) are opword encodings with the target's registers;
// they hold only because the allocation around them is the target's, which the exact match
// checks. Behavior is unchanged. Real form believed to be the plain function: arguments used
// directly, count += mesh->numIndices, and
// `for (n = 0; n < numAttrs; n++) GCNDisplayListIndex8(vertex);` (Index16 for 2-byte indices)
// in each while (numIndices--) loop.
void GCNDisplayListsBuild(register GCNDisplayList* listsArg, register U8* dlMemArg,
                          register const RpMeshHeader* headerArg, register S32 numTexCoordsArg,
                          register S32 indexSizeArg, register U32 flagsArg)
{
    GXPrimitive primType = GX_TRIANGLES;
    U32 numIdx;
    U32 size;
    GCNDisplayList* list;
    S32 i;
    U32 textured;
    U32 count;
    register const RpMeshHeader* header;
    register S32 attrLimit;
    U32 normals;
    register GCNDisplayList* lists;
    U32 prelit;
    U32 dlSize;
    GXAttrType attrType;
    register U32 attrCount;
    U32 numMeshes;
    const RpMesh* mesh;
    U8 numAttrs;
    register U32 numIndices;
    register const U16* index;
    register U32 flags;
    register S32 indexSize;
    register S32 numTexCoords;
    register U8* dlMem;

    // FAKEMATCH: the six arguments are copied into register locals with asm `or` (mr); real form
    // believed to use the parameters directly.
    asm
    {
        or header, headerArg, headerArg
        or lists, listsArg, listsArg
        or dlMem, dlMemArg, dlMemArg
        or numTexCoords, numTexCoordsArg, numTexCoordsArg
        or indexSize, indexSizeArg, indexSizeArg
        or flags, flagsArg, flagsArg
    }

    if (headerArg->flags & rpMESHHEADERTRISTRIP)
    {
        primType = GX_TRIANGLESTRIP;
    }
    attrType = GX_INDEX16;
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

    // FAKEMATCH: the textured test and the attribute count are named locals; real form believed
    // to test flags and use numAttrs directly.
    textured = flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2);
    attrCount = numAttrs;
    list = lists;
    numMeshes = header->numMeshes;
    mesh = (const RpMesh*)(header + 1);
    // FAKEMATCH: the count-8 unroll limit is a named local; real form believed to leave it to
    // the compiler's unroller.
    attrLimit = attrCount - 8;
    while (numMeshes--)
    {
        index = mesh->indices;
        // FAKEMATCH: the per-mesh index count is a named copy; real form believed to read
        // mesh->numIndices.
        numIdx = mesh->numIndices;
        numIndices = numIdx;
        count = numIdx;

        if (normals)
        {
            count += numIdx;
        }
        if (prelit)
        {
            count += numIdx;
        }
        if (textured)
        {
            count += numIdx * numTexCoords;
        }

        count = indexSize * count + 3;
        if (indexSize == 1 && ALIGN32(count) == count)
        {
            count += 0x20;
        }
        size = ALIGN32(count) + 0x20;

        DCInvalidateRange(dlMem, size);
        GXBeginDisplayList(dlMem, size);
        GXResetWriteGatherPipe();
        GXBegin(primType, GX_VTXFMT0, numIndices);

        if (indexSize == 1)
        {
            register U32 t0, fifo, vtx, n;

            // FAKEMATCH: the 8-bit index loop is inline asm in the compiler's 8-way unrolled ctr
            // form, with one opword (the dead per-vertex counter step, addi r5, r5, 1) that the
            // compiler would drop; real form believed to be a C loop writing each index once per
            // attribute.
            asm
            {
                b test8
            loop8:
                lhz t0, 0(index)
                cmpwi attrCount, 0
                li n, 0
                clrlwi vtx, t0, 24
                ble next8
                cmpwi attrCount, 8
                ble rest8
                addi t0, attrLimit, 7
                lis fifo, 0xcc01
                srwi t0, t0, 3
                mtctr t0
                cmpwi attrLimit, 0
                ble rest8
            unroll8:
                stb vtx, -0x8000(fifo)
                addi n, n, 8
                stb vtx, -0x8000(fifo)
                stb vtx, -0x8000(fifo)
                stb vtx, -0x8000(fifo)
                stb vtx, -0x8000(fifo)
                stb vtx, -0x8000(fifo)
                stb vtx, -0x8000(fifo)
                stb vtx, -0x8000(fifo)
                bdnz unroll8
            rest8:
                subf t0, n, attrCount
                lis fifo, 0xcc01
                mtctr t0
                cmpw n, attrCount
                bge next8
            one8:
                stb vtx, -0x8000(fifo)
                opword 0x38A50001 // addi r5, r5, 1
                bdnz one8
            next8:
                addi index, index, 2
            test8:
                cmplwi numIndices, 0
                subi numIndices, numIndices, 1
                bne loop8
            }
        }
        else
        {
            register U32 t0, fifo, vtx;

            // FAKEMATCH: the 16-bit index loop is inline asm in the compiler's 8-way unrolled ctr
            // form, with opword encodings for the instructions the compiler would fold away; real
            // form believed to be a C loop writing each index once per attribute.
            asm
            {
                b test16
            loop16:
                cmpwi attrCount, 0
                lhz vtx, 0(index)
                opword 0x38C00000 // li r6, 0
                ble next16
                cmpwi attrCount, 8
                ble rest16
                addi t0, attrLimit, 7
                opword 0x5485043E // clrlwi r5, r4, 16
                srwi t0, t0, 3
                opword 0x3C60CC01 // lis r3, 0xcc01
                mtctr t0
                cmpwi attrLimit, 0
                ble rest16
            unroll16:
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                opword 0x38C60008 // addi r6, r6, 8
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                opword 0xB0A38000 // sth r5, -0x8000(r3)
                bdnz unroll16
            rest16:
                opword 0x7C06C850 // subf r0, r6, r25
                opword 0x5484043E // clrlwi r4, r4, 16
                lis fifo, 0xcc01
                opword 0x7C0903A6 // mtctr r0
                opword 0x7C06C800 // cmpw r6, r25
                bge next16
            one16:
                sth vtx, -0x8000(fifo)
                opword 0x38C60001 // addi r6, r6, 1
                bdnz one16
            next16:
                addi index, index, 2
            test16:
                cmplwi numIndices, 0
                subi numIndices, numIndices, 1
                bne loop16
            }
        }

        dlSize = GXEndDisplayList();
        list->list = dlMem;
        dlMem += dlSize;
        list->size = dlSize;
        list++;
        mesh++;
    }
}
