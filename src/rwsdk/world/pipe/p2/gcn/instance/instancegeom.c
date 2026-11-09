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
        arrays->nrmStride = lbl_801DDA48[vtxFmt->compType[1]] * (vtxFmt->frac[1] ? 9 : 3);
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
            arrays->texStride[i] = (lbl_801DDA48[(vtxFmt->compType + 2)[i]] * 2) & 0xFE;
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

static inline RwTexCoords* RpGeometryGetVertexTexCoords(const RpGeometry* geom, S32 texCoordSet)
{
    return geom->texCoords[texCoordSet - 1];
}

// FAKEMATCH: the body sits in an inline that copies its arguments with inline asm `or` (mr)
// into register locals, and the default format is stored the same way; the texture loop keeps
// its own count and array cursor. That numbers the saved values in the target's order (the
// flag tests below the arguments, the loop index below its temporaries). Real form believed
// to be one plain function: numVerts = RpGeometryGetNumVertices(geom), vtxFmt defaulted in C,
// and arrays->tex[i] = dst in a loop to numTexCoords.
#pragma push
// FAKEMATCH: the inline size limit is raised so the large GeometryInstanceFast inline is expanded;
// real form believed to be one plain function with no pragma.
#pragma inline_max_size(10000)

// Instances a geometry's first morph target into dst in the format's types, one array per
// attribute in flags (or one interleaved array), and records where each starts.
static inline void GeometryInstanceFast(register RpGeometry* geomArg,
                                        register GCNVertexArrays* arraysArg,
                                        register const RpGameCubeVtxFmt* vtxFmtArg, U32 flags,
                                        register U8 numTexCoordsArg, register U8* dstArg)
{
    register RpGeometry* geom;
    register GCNVertexArrays* arrays;
    register const RpGameCubeVtxFmt* vtxFmt;
    register U8 numTexCoords;
    register U8* dst;
    S32 numVerts;
    S32 i;
    S32 count;
    GCNVertexArrays* texp;

    // FAKEMATCH: the arguments are copied into register locals with asm `or` (mr); real form
    // believed to use the parameters directly.
    asm
    {
        or geom, geomArg, geomArg
        or arrays, arraysArg, arraysArg
        or vtxFmt, vtxFmtArg, vtxFmtArg
        or numTexCoords, numTexCoordsArg, numTexCoordsArg
        or dst, dstArg, dstArg
    }
    numVerts = geomArg->numVertices;

    if (!vtxFmt)
    {
        register const RpGameCubeVtxFmt* def = &_RpDlVtxFmtDefault;

        // FAKEMATCH: the default format is copied with asm `or` (mr); real form believed to be
        // vtxFmt = &_RpDlVtxFmtDefault.
        asm { or vtxFmt, def, def }
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
        // FAKEMATCH: the loop keeps its own count and array cursor; real form believed to be
        // arrays->tex[i] = dst in a loop to numTexCoords.
        texp = arrays;
        count = numTexCoords;
        for (i = 0; i < count; i++)
        {
            S32 size = _rwGCNVtxFmtInstTex(vtxFmt, i, dst, RpGeometryGetVertexTexCoords(geom, i + 1), numVerts,
                                           arrays->texStride[i]);
            texp->tex[0] = dst;
            dst += size;
            texp = (GCNVertexArrays*)((U8*)texp + 4);
        }
    }
}

void _rwDlGeometryInstanceFast(RpGeometry* geom, GCNVertexArrays* arrays,
                               const RpGameCubeVtxFmt* vtxFmt, U32 flags, U8 numTexCoords,
                               U8* dst)
{
    GeometryInstanceFast(geom, arrays, vtxFmt, flags, numTexCoords, dst);
}

#pragma pop
