#include <rpworld.h>

extern S32 rwGCNCompTypeSize[5];
extern S32 rwGCNClrFormatSize[7];

// Sets the stride of each attribute in flags: its own size, or, when the format interleaves
// the attributes, the size of one whole vertex for all of them.
static inline void VtxDescInitFast(GCNVertexArrays* arrays, const RpGameCubeVtxFmt* vtxFmt, U32 flags,
                            U8 numTexCoords)
{
    U32 stride = 0;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    if (flags & rpGEOMETRYPOSITIONS)
    {
        arrays->posStride = rwGCNCompTypeSize[vtxFmt->compType[0]] * 3;
        stride = arrays->posStride;
    }

    if (flags & rpGEOMETRYNORMALS)
    {
        arrays->nrmStride = rwGCNCompTypeSize[vtxFmt->compType[1]] * (vtxFmt->frac[1] ? 9 : 3);
        stride += arrays->nrmStride;
    }

    if (flags & rpGEOMETRYPRELIT)
    {
        arrays->clrStride = rwGCNClrFormatSize[vtxFmt->compType[10]];
        stride += arrays->clrStride;
    }

    if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
    {
        U32 i;

        for (i = 0; i < numTexCoords; i++)
        {
            arrays->texStride[i] = (rwGCNCompTypeSize[(vtxFmt->compType + 2)[i]] * 2) & 0xFE;
            stride += arrays->texStride[i];
        }
    }

    if (vtxFmt->flags & VTXFMT_INTERLEAVED)
    {
        U32 i;

        arrays->posStride = stride;
        arrays->nrmStride = stride;
        arrays->clrStride = stride;
        for (i = 0; i < numTexCoords; i++)
        {
            arrays->texStride[i] = stride;
        }
    }
}

// Instances a world sector's vertices into buffer in the format's types, one array per attribute
// in flags (or one interleaved array), and records where each starts.
void _rwDlWorldSectorInstanceFast(RpWorldSector* sector, GCNVertexArrays* arrays,
                                  const RpGameCubeVtxFmt* vtxFmt, U32 flags, U8 numTexCoords,
                                  U8* buffer)
{
    U8* dst;
    S32 numVerts;
    S32 size;
    S32 i;

    numVerts = RpWorldSectorGetNumVertices(sector);
    dst = buffer;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    VtxDescInitFast(arrays, vtxFmt, flags, numTexCoords);

    if (flags & rpGEOMETRYPOSITIONS)
    {
        size = _rwGCNVtxFmtInstPos3D(vtxFmt, dst, sector->vertices, numVerts, arrays->posStride);
        arrays->pos = dst;
        dst += size;
    }

    if (flags & rpGEOMETRYNORMALS)
    {
        size = _rwGCNVtxFmtInstNrmCmp(vtxFmt, dst, sector->normals, numVerts, arrays->nrmStride);
        arrays->nrm = dst;
        dst += size;
    }

    if (flags & rpGEOMETRYPRELIT)
    {
        size = _rwGCNVtxFmtInstClr(vtxFmt, dst, sector->preLitLum, numVerts, arrays->clrStride);
        arrays->clr = dst;
        dst += size;
    }

    if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
    {
        for (i = 0; i < numTexCoords; i++)
        {
            size = _rwGCNVtxFmtInstTex(vtxFmt, i, dst, sector->texCoords[i], numVerts,
                                       arrays->texStride[i]);
            arrays->tex[i] = dst;
            dst += size;
        }
    }
}
