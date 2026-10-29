#include <rwcore.h>
#include <string.h>

#define IM3D_PLUGIN_ID 1

RwModuleInfo _rwIm3DModule;
rwImmediGlobals* _rwIm3DGlobals;

#define RWIMMEDIGLOBAL(var)                                                                        \
    (((rwImmediGlobals*)((U8*)RwEngineInstance + _rwIm3DModule.globalsOffset))->var)

void* RwIm3DTransform(RwIm3DVertex* pVerts, U32 numVerts, RwMatrix* ltm, U32 flags)
{
    if (numVerts > 0x10000)
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x32);
        RwErrorSet(&err);
    }
    else
    {
        RWIMMEDIGLOBAL(im3DPool).numElements = numVerts;
        RWIMMEDIGLOBAL(im3DPool).elements = pVerts;
        RWIMMEDIGLOBAL(im3DPool).stride = sizeof(RwIm3DVertex);
        RWIMMEDIGLOBAL(im3DPool).stash.ltm = ltm;
        RWIMMEDIGLOBAL(im3DPool).stash.flags = flags | (rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA);

        if (RxPipelineExecute(RWIMMEDIGLOBAL(curIm3DTransformPipeline), &RWIMMEDIGLOBAL(im3DPool),
                              TRUE))
        {
            return pVerts;
        }
    }

    return NULL;
}

S32 RwIm3DEnd(void)
{
    if (!RWIMMEDIGLOBAL(im3DPool).elements)
    {
        return FALSE;
    }

    memset(&RWIMMEDIGLOBAL(im3DPool), 0, sizeof(rwIm3DPool));

    return TRUE;
}

S32 RwIm3DRenderIndexedPrimitive(RwPrimitiveType primType, RwImVertexIndex* indices,
                                 S32 numIndices)
{
    if (RWIMMEDIGLOBAL(im3DPool).elements)
    {
        _rwIm3DPoolStash* stash = &RWIMMEDIGLOBAL(im3DPool).stash;

        stash->pipeline = NULL;
        stash->primType = primType;
        stash->indices = indices;
        stash->numIndices = numIndices;

        switch (primType)
        {
        case rwPRIMTYPETRILIST:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).triList;
            stash->numIndices = numIndices - numIndices % 3;
            break;
        case rwPRIMTYPETRIFAN:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).triFan;
            break;
        case rwPRIMTYPETRISTRIP:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).triStrip;
            break;
        case rwPRIMTYPELINELIST:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).lineList;
            stash->numIndices = numIndices - numIndices % 2;
            break;
        case rwPRIMTYPEPOLYLINE:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).polyLine;
            break;
        default:
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x25, primType);
            RwErrorSet(&err);
            break;
        }
        }

        if (RxPipelineExecute(stash->pipeline, stash, FALSE))
        {
            return TRUE;
        }
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x23);
        RwErrorSet(&err);
    }

    return FALSE;
}

S32 RwIm3DRenderPrimitive(RwPrimitiveType primType)
{
    S32 transformed = RWIMMEDIGLOBAL(im3DPool).elements != NULL;

    RxHeapGetGlobalHeap();

    if (transformed)
    {
        _rwIm3DPoolStash* stash = &RWIMMEDIGLOBAL(im3DPool).stash;

        stash->pipeline = NULL;
        stash->primType = primType;
        stash->indices = NULL;
        stash->numIndices = RWIMMEDIGLOBAL(im3DPool).numElements;

        switch (primType)
        {
        case rwPRIMTYPETRILIST:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).triList;
            break;
        case rwPRIMTYPETRIFAN:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).triFan;
            break;
        case rwPRIMTYPETRISTRIP:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).triStrip;
            break;
        case rwPRIMTYPELINELIST:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).lineList;
            break;
        case rwPRIMTYPEPOLYLINE:
            stash->pipeline = RWIMMEDIGLOBAL(curIm3DRenderPipelines).polyLine;
            break;
        default:
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x25, primType);
            RwErrorSet(&err);
            break;
        }
        }

        if (RxPipelineExecute(stash->pipeline, stash, FALSE))
        {
            return TRUE;
        }
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x23);
        RwErrorSet(&err);
    }

    return FALSE;
}

RxPipeline* RwIm3DSetTransformPipeline(RxPipeline* pipeline)
{
    if (pipeline)
    {
        RWIMMEDIGLOBAL(curIm3DTransformPipeline) = pipeline;
    }
    else if (RWIMMEDIGLOBAL(platformIm3DTransformPipeline))
    {
        RWIMMEDIGLOBAL(curIm3DTransformPipeline) = RWIMMEDIGLOBAL(platformIm3DTransformPipeline);
    }
    else
    {
        RWIMMEDIGLOBAL(curIm3DTransformPipeline) = RWIMMEDIGLOBAL(genericIm3DTransformPipeline);
    }

    return RWIMMEDIGLOBAL(curIm3DTransformPipeline);
}

// Restores the platform pipeline for a primitive type, or the generic one where the platform has
// none.
#define IM3DRESTORERENDER(_member)                                                                 \
    if (RWIMMEDIGLOBAL(platformIm3DRenderPipelines)._member)                                       \
    {                                                                                              \
        RWIMMEDIGLOBAL(curIm3DRenderPipelines)._member =                                           \
            RWIMMEDIGLOBAL(platformIm3DRenderPipelines)._member;                                   \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
        RWIMMEDIGLOBAL(curIm3DRenderPipelines)._member =                                           \
            RWIMMEDIGLOBAL(genericIm3DRenderPipelines)._member;                                    \
    }

RxPipeline* RwIm3DSetRenderPipeline(RxPipeline* pipeline, RwPrimitiveType primType)
{
    if (pipeline)
    {
        switch (primType)
        {
        case rwPRIMTYPETRILIST:
            RWIMMEDIGLOBAL(curIm3DRenderPipelines).triList = pipeline;
            return pipeline;
        case rwPRIMTYPETRIFAN:
            RWIMMEDIGLOBAL(curIm3DRenderPipelines).triFan = pipeline;
            return pipeline;
        case rwPRIMTYPETRISTRIP:
            RWIMMEDIGLOBAL(curIm3DRenderPipelines).triStrip = pipeline;
            return pipeline;
        case rwPRIMTYPELINELIST:
            RWIMMEDIGLOBAL(curIm3DRenderPipelines).lineList = pipeline;
            return pipeline;
        case rwPRIMTYPEPOLYLINE:
            RWIMMEDIGLOBAL(curIm3DRenderPipelines).polyLine = pipeline;
            return pipeline;
        case rwPRIMTYPEPOINTLIST:
            RWIMMEDIGLOBAL(curIm3DRenderPipelines).pointList = pipeline;
            return pipeline;
        default:
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x25, primType);
            RwErrorSet(&err);
            break;
        }
        }
    }
    else
    {
        switch (primType)
        {
        case rwPRIMTYPETRILIST:
            IM3DRESTORERENDER(triList);
            return RWIMMEDIGLOBAL(curIm3DRenderPipelines).triList;
        case rwPRIMTYPETRIFAN:
            IM3DRESTORERENDER(triFan);
            return RWIMMEDIGLOBAL(curIm3DRenderPipelines).triFan;
        case rwPRIMTYPETRISTRIP:
            IM3DRESTORERENDER(triStrip);
            return RWIMMEDIGLOBAL(platformIm3DRenderPipelines).triStrip;
        case rwPRIMTYPELINELIST:
            IM3DRESTORERENDER(lineList);
            return RWIMMEDIGLOBAL(platformIm3DRenderPipelines).lineList;
        case rwPRIMTYPEPOLYLINE:
            IM3DRESTORERENDER(polyLine);
            return RWIMMEDIGLOBAL(platformIm3DRenderPipelines).polyLine;
        case rwPRIMTYPEPOINTLIST:
            IM3DRESTORERENDER(pointList);
            return RWIMMEDIGLOBAL(platformIm3DRenderPipelines).pointList;
        default:
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x25, primType);
            RwErrorSet(&err);
        }
        }
    }

    return NULL;
}

static S32 _rwIm3DCreateGenericTransformPipeline(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = IM3D_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            pipeline = RxLockedPipeUnlock(RxLockedPipeAddFragment(
                lpipe, NULL, RxNodeDefinitionGetImmInstance(), RxNodeDefinitionGetTransform(),
                RxNodeDefinitionGetImmStash(), NULL));

            if (pipeline)
            {
                RWIMMEDIGLOBAL(genericIm3DTransformPipeline) = pipeline;
                RWIMMEDIGLOBAL(curIm3DTransformPipeline) = pipeline;

                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}

static void _rwIm3DDestroyGenericTransformPipeline(void)
{
    RWIMMEDIGLOBAL(curIm3DTransformPipeline) = NULL;

    if (RWIMMEDIGLOBAL(genericIm3DTransformPipeline))
    {
        _rxPipelineDestroy(RWIMMEDIGLOBAL(genericIm3DTransformPipeline));
        RWIMMEDIGLOBAL(genericIm3DTransformPipeline) = NULL;
    }
}

static RxPipeline* _rwIm3DCreateGenericTriRenderPipeline(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = IM3D_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            RxPipelineNode* node;
            RxNodeInput input;
            RxNodeOutput output;

            lpipe = RxLockedPipeAddFragment(
                lpipe, NULL, RxNodeDefinitionGetImmRenderSetup(),
                RxNodeDefinitionGetImmMangleTriangleIndices(), RxNodeDefinitionGetCullTriangle(),
                RxNodeDefinitionGetClipTriangle(), RxNodeDefinitionGetSubmitTriangle(), NULL);

            node = RxPipelineFindNodeByName(lpipe, "ImmRenderSetup.csl", NULL, NULL);
            input = RxPipelineNodeFindInput(
                RxPipelineFindNodeByName(lpipe, "ImmMangleTriangleIndices.csl", NULL, NULL));
            output = RxPipelineNodeFindOutputByName(node, "ImmRenderSetupOutUnindexed");

            pipeline = RxLockedPipeUnlock(RxLockedPipeAddPath(lpipe, output, input));

            if (pipeline)
            {
                return pipeline;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return NULL;
}

static RxPipeline* _rwIm3DCreateGenericLineRenderPipeline(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = IM3D_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            RxPipelineNode* node;

            pipeline = RxLockedPipeAddFragment(lpipe, NULL, RxNodeDefinitionGetImmRenderSetup(),
                                               RxNodeDefinitionGetImmMangleLineIndices(),
                                               RxNodeDefinitionGetClipLine(),
                                               RxNodeDefinitionGetSubmitLine(), NULL);

            node = RxPipelineFindNodeByName(pipeline, "ImmRenderSetup.csl", NULL, NULL);

            pipeline = RxLockedPipeUnlock(RxLockedPipeAddPath(
                pipeline, RxPipelineNodeFindOutputByName(node, "ImmRenderSetupOutUnindexed"),
                RxPipelineNodeFindInput(
                    RxPipelineFindNodeByName(pipeline, "ImmMangleLineIndices.csl", NULL, NULL))));

            if (pipeline)
            {
                return pipeline;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return NULL;
}

static void _rwIm3DDestroyGenericRenderPipelines(void)
{
    RWIMMEDIGLOBAL(curIm3DRenderPipelines).triList = NULL;
    RWIMMEDIGLOBAL(curIm3DRenderPipelines).triFan = NULL;
    RWIMMEDIGLOBAL(curIm3DRenderPipelines).triStrip = NULL;

    if (RWIMMEDIGLOBAL(genericIm3DRenderPipelines).triList)
    {
        _rxPipelineDestroy(RWIMMEDIGLOBAL(genericIm3DRenderPipelines).triList);
    }

    RWIMMEDIGLOBAL(genericIm3DRenderPipelines).triList = NULL;
    RWIMMEDIGLOBAL(genericIm3DRenderPipelines).triFan = NULL;
    RWIMMEDIGLOBAL(genericIm3DRenderPipelines).triStrip = NULL;

    RWIMMEDIGLOBAL(curIm3DRenderPipelines).lineList = NULL;
    RWIMMEDIGLOBAL(curIm3DRenderPipelines).polyLine = NULL;

    if (RWIMMEDIGLOBAL(genericIm3DRenderPipelines).lineList)
    {
        _rxPipelineDestroy(RWIMMEDIGLOBAL(genericIm3DRenderPipelines).lineList);
    }

    RWIMMEDIGLOBAL(genericIm3DRenderPipelines).lineList = NULL;
    RWIMMEDIGLOBAL(genericIm3DRenderPipelines).polyLine = NULL;
}

static S32 _rwIm3DCreateGenericRenderPipelines(void)
{
    rwIm3DRenderPipelines* pipes = &RWIMMEDIGLOBAL(genericIm3DRenderPipelines);
    RxPipeline* triPipeline;
    RxPipeline* linePipeline;

    pipes->triList = NULL;
    pipes->triFan = NULL;
    pipes->triStrip = NULL;
    pipes->lineList = NULL;
    pipes->polyLine = NULL;
    pipes->pointList = NULL;

    triPipeline = _rwIm3DCreateGenericTriRenderPipeline();
    linePipeline = _rwIm3DCreateGenericLineRenderPipeline();

    if (triPipeline && linePipeline)
    {
        rwIm3DRenderPipelines* cur;

        pipes->triList = triPipeline;
        pipes->triFan = triPipeline;
        pipes->triStrip = triPipeline;
        pipes->lineList = linePipeline;
        pipes->polyLine = linePipeline;

        cur = &RWIMMEDIGLOBAL(curIm3DRenderPipelines);
        cur->triList = triPipeline;
        cur->triFan = triPipeline;
        cur->triStrip = triPipeline;
        cur->lineList = linePipeline;
        cur->polyLine = linePipeline;
        cur->pointList = NULL;

        return TRUE;
    }

    _rwIm3DDestroyGenericRenderPipelines();

    return FALSE;
}

void* _rwIm3DClose(void* instance, S32 offset, S32 size)
{
    _rwIm3DDestroyPlatformRenderPipelines(&RWIMMEDIGLOBAL(platformIm3DRenderPipelines));
    _rwIm3DDestroyPlatformTransformPipeline(&RWIMMEDIGLOBAL(platformIm3DTransformPipeline));
    _rwIm3DDestroyGenericRenderPipelines();
    _rwIm3DDestroyGenericTransformPipeline();

    _rwIm3DModule.numInstances--;

    return instance;
}

void* _rwIm3DOpen(void* instance, S32 offset, S32 size)
{
    S32 ok;

    _rwIm3DModule.globalsOffset = offset;
    _rwIm3DGlobals = (rwImmediGlobals*)((U8*)RwEngineInstance + offset);
    _rwIm3DModule.numInstances++;

    memset(_rwIm3DGlobals, 0, sizeof(rwImmediGlobals));

    ok = _rwIm3DCreateGenericTransformPipeline();

    if (ok)
    {
        ok = _rwIm3DCreateGenericRenderPipelines();
    }

    if (ok)
    {
        ok = _rwIm3DCreatePlatformTransformPipeline(
            &RWIMMEDIGLOBAL(platformIm3DTransformPipeline));
    }

    if (ok)
    {
        ok = _rwIm3DCreatePlatformRenderPipelines(&RWIMMEDIGLOBAL(platformIm3DRenderPipelines));
    }

    if (ok)
    {
        return instance;
    }

    _rwIm3DDestroyPlatformRenderPipelines(&RWIMMEDIGLOBAL(platformIm3DRenderPipelines));
    _rwIm3DDestroyPlatformTransformPipeline(&RWIMMEDIGLOBAL(platformIm3DTransformPipeline));
    _rwIm3DDestroyGenericRenderPipelines();
    _rwIm3DDestroyGenericTransformPipeline();

    _rwIm3DModule.numInstances--;

    return NULL;
}
