#include <rpworld.h>

extern S32 lbl_801DDA48[5];
extern S32 lbl_801DDA5C[7];

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
        arrays->posStride = lbl_801DDA48[vtxFmt->compType[0]] * 3;
        stride = arrays->posStride;
    }

    if (flags & rpGEOMETRYNORMALS)
    {
        S32 count = 3;

        if (vtxFmt->frac[1])
        {
            count = 9;
        }
        arrays->nrmStride = lbl_801DDA48[vtxFmt->compType[1]] * count;
        stride += arrays->nrmStride;
    }

    if (flags & rpGEOMETRYPRELIT)
    {
        arrays->clrStride = lbl_801DDA5C[vtxFmt->compType[10]];
        stride += arrays->clrStride;
    }

    if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
    {
        U32 i;

        for (i = 0; i < numTexCoords; i++)
        {
            arrays->texStride[i] = (lbl_801DDA48[vtxFmt->compType[i + 2]] * 2) & 0xFE;
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

// Instances a geometry's first morph target into dst in the format's types, one array per
// attribute in flags (or one interleaved array), and records where each starts.
void _rwDlGeometryInstanceFast(RpGeometry* geom, GCNVertexArrays* arrays,
                               const RpGameCubeVtxFmt* vtxFmt, U32 flags, U8 numTexCoords,
                               U8* dst)
{
    S32 numVerts = geom->numVertices;
    S32 i;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    VtxDescInitFast(arrays, vtxFmt, flags, numTexCoords);

    if (flags & rpGEOMETRYPOSITIONS)
    {
        S32 size = _rwGCNVtxFmtInstPos3D(vtxFmt, dst, geom->morphTarget->verts, numVerts,
                                         arrays->posStride);
        arrays->pos = dst;
        dst += size;
    }

    if (flags & rpGEOMETRYNORMALS)
    {
        S32 size = _rwGCNVtxFmtInstNrm(vtxFmt, dst, geom->morphTarget->normals, numVerts,
                                       arrays->nrmStride);
        arrays->nrm = dst;
        dst += size;
    }

    if (flags & rpGEOMETRYPRELIT)
    {
        S32 size = _rwGCNVtxFmtInstClr(vtxFmt, dst, geom->preLitLum, numVerts, arrays->clrStride);
        arrays->clr = dst;
        dst += size;
    }

    if (flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
    {
        for (i = 0; i < numTexCoords; i++)
        {
            S32 size = _rwGCNVtxFmtInstTex(vtxFmt, i, dst, geom->texCoords[i], numVerts,
                                           arrays->texStride[i]);
            arrays->tex[i] = dst;
            dst += size;
        }
    }
}
