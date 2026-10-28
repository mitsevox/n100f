#include <rwcore.h>

F32 RwIm2DGetNearScreenZ(void)
{
    return RWSRCGLOBAL(dOpenDevice).zBufferNear;
}

F32 RwIm2DGetFarScreenZ(void)
{
    return RWSRCGLOBAL(dOpenDevice).zBufferFar;
}

S32 RwRenderStateSet(RwRenderState state, void* value)
{
    if (!RWSRCGLOBAL(curCamera))
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x10);
        RwErrorSet(&err);
        return 0;
    }

    return RWSRCGLOBAL(dOpenDevice).fpRenderStateSet(state, value);
}

S32 RwRenderStateGet(RwRenderState state, void* value)
{
    return RWSRCGLOBAL(dOpenDevice).fpRenderStateGet(state, value);
}

S32 RwIm2DRenderLine(RwIm2DVertex* vertices, S32 numVertices, S32 vert1, S32 vert2)
{
    return RWSRCGLOBAL(dOpenDevice).fpIm2DRenderLine(vertices, numVertices, vert1, vert2);
}

S32 RwIm2DRenderPrimitive(RwPrimitiveType primType, RwIm2DVertex* vertices, S32 numVertices)
{
    return RWSRCGLOBAL(dOpenDevice).fpIm2DRenderPrimitive(primType, vertices, numVertices);
}

S32 RwIm2DRenderIndexedPrimitive(RwPrimitiveType primType, RwIm2DVertex* vertices,
                                 S32 numVertices, RwImVertexIndex* indices, S32 numIndices)
{
    return RWSRCGLOBAL(dOpenDevice).fpIm2DRenderIndexedPrimitive(primType, vertices, numVertices,
                                                                 indices, numIndices);
}
