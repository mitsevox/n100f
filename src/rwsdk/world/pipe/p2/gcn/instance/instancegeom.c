#include <rpworld.h>

extern S32 rwGCNCompTypeSize[5];
extern S32 rwGCNClrFormatSize[7];

// Sets the stride of each attribute in flags: its own size, or, when the format interleaves
// the attributes, the size of one whole vertex for all of them.
// FAKEMATCH: the normal, prelit and texture tests are also stored through pointer outputs so the
// caller can reuse them; real form believed to be VtxDescInitFast(arrays, vtxFmt, flags,
// numTexCoords) with the caller testing flags again.
static inline void VtxDescInitFast(GCNVertexArrays* arrays, const RpGameCubeVtxFmt* vtxFmt, U32 flags,
                            U8 numTexCoords, U32* hasNrm, U32* hasPrelit, U32* hasTex)
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

    // FAKEMATCH: the normal test is stored through its pointer output; real form believed to be
    // the plain test.
    if ((*hasNrm = flags & rpGEOMETRYNORMALS) != 0)
    {
        arrays->nrmStride = rwGCNCompTypeSize[vtxFmt->compType[1]] * (vtxFmt->frac[1] ? 9 : 3);
        stride += arrays->nrmStride;
    }

    // FAKEMATCH: the prelit test is stored through its pointer output; real form believed to be
    // the plain test.
    if ((*hasPrelit = flags & rpGEOMETRYPRELIT) != 0)
    {
        arrays->clrStride = rwGCNClrFormatSize[vtxFmt->compType[10]];
        stride += arrays->clrStride;
    }

    // FAKEMATCH: the texture test is stored through its pointer output; real form believed to be
    // the plain test.
    if ((*hasTex = flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2)) != 0)
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

static inline RwTexCoords* RpGeometryGetVertexTexCoords(const RpGeometry* geom, S32 texCoordSet)
{
    return geom->texCoords[texCoordSet - 1];
}

// Instances a geometry's first morph target into dst in the format's types, one array per
// attribute in flags (or one interleaved array), and records where each starts.
// FAKEMATCH: the pointer arguments and numTexCoords are register-qualified so the asm below can
// copy them; real form believed to be plain parameters.
void _rwDlGeometryInstanceFast(register RpGeometry* geomArg, register GCNVertexArrays* arraysArg,
                               register const RpGameCubeVtxFmt* vtxFmtArg, U32 flags,
                               register U8 numTexCoordsArg, register U8* dstArg)
{
    S32 numVerts;
    register U8* dst;
    register U8 numTexCoords;
    register const RpGameCubeVtxFmt* vtxFmt;
    register GCNVertexArrays* arrays;
    register RpGeometry* geom;
    S32 i;
    // FAKEMATCH: the normal, prelit and texture tests are named locals that VtxDescInitFast fills;
    // real form believed to test flags directly.
    U32 hasNrm;
    U32 hasPrelit;
    U32 hasTex;

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
    // FAKEMATCH: the vertex count is read from the argument, not its register copy; real form
    // believed to be numVerts = RpGeometryGetNumVertices(geom).
    numVerts = geomArg->numVertices;

    if (!vtxFmt)
    {
        vtxFmt = &_RpDlVtxFmtDefault;
    }

    // FAKEMATCH: the tests come back through pointer outputs; real form believed to be
    // VtxDescInitFast(arrays, vtxFmt, flags, numTexCoords).
    VtxDescInitFast(arrays, vtxFmt, flags, numTexCoords, &hasNrm, &hasPrelit, &hasTex);

    if (flags & rpGEOMETRYPOSITIONS)
    {
        S32 size = _rwGCNVtxFmtInstPos3D(vtxFmt, dst, geom->morphTarget->verts, numVerts,
                                         arrays->posStride);
        arrays->pos = dst;
        dst += size;
    }

    // FAKEMATCH: tests the named local; real form believed to be flags & rpGEOMETRYNORMALS.
    if (hasNrm)
    {
        S32 size = _rwGCNVtxFmtInstNrm(vtxFmt, dst, geom->morphTarget->normals, numVerts,
                                       arrays->nrmStride);
        arrays->nrm = dst;
        dst += size;
    }

    // FAKEMATCH: tests the named local; real form believed to be flags & rpGEOMETRYPRELIT.
    if (hasPrelit)
    {
        S32 size = _rwGCNVtxFmtInstClr(vtxFmt, dst, geom->preLitLum, numVerts, arrays->clrStride);
        arrays->clr = dst;
        dst += size;
    }

    // FAKEMATCH: tests the named local; real form believed to be
    // flags & (rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2).
    if (hasTex)
    {
        for (i = 0; i < numTexCoords; i++)
        {
            S32 size = _rwGCNVtxFmtInstTex(vtxFmt, i, dst, RpGeometryGetVertexTexCoords(geom, i + 1), numVerts,
                                           arrays->texStride[i]);
            arrays->tex[i] = dst;
            dst += size;
        }
    }
}
