#include <rpworld.h>

#define WORLD_PLUGIN_ID 2

RxPipeline* RpWorldSetDefaultSectorPipeline(RxPipeline* pipeline)
{
    if (!pipeline)
    {
        if (RXPIPELINEGLOBAL(platformSectorPipeline))
        {
            pipeline = RXPIPELINEGLOBAL(platformSectorPipeline);
        }
        else
        {
            pipeline = RXPIPELINEGLOBAL(genericSectorPipeline);
        }
    }

    RXPIPELINEGLOBAL(defaultSectorPipeline) = pipeline;

    return pipeline;
}

RxPipeline* RpAtomicSetDefaultPipeline(RxPipeline* pipeline)
{
    if (!pipeline)
    {
        if (RXPIPELINEGLOBAL(platformAtomicPipeline))
        {
            pipeline = RXPIPELINEGLOBAL(platformAtomicPipeline);
        }
        else
        {
            pipeline = RXPIPELINEGLOBAL(genericAtomicPipeline);
        }
    }

    RXPIPELINEGLOBAL(defaultAtomicPipeline) = pipeline;

    return pipeline;
}

static S32 _RpMaterialCreateGenericRenderPipeline(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = WORLD_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            pipeline = RxLockedPipeUnlock(RxLockedPipeAddFragment(
                lpipe, NULL, RxNodeDefinitionGetTransform(), RxNodeDefinitionGetCullTriangle(),
                RxNodeDefinitionGetLight(), RxNodeDefinitionGetPostLight(),
                RxNodeDefinitionGetClipTriangle(), RxNodeDefinitionGetSubmitTriangle(), NULL));

            if (pipeline)
            {
                RXPIPELINEGLOBAL(genericMaterialPipeline) = pipeline;
                RXPIPELINEGLOBAL(defaultMaterialPipeline) = pipeline;
                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}

static void _RpMaterialDestroyGenericRenderPipeline(void)
{
    RXPIPELINEGLOBAL(defaultMaterialPipeline) = NULL;

    if (RXPIPELINEGLOBAL(genericMaterialPipeline))
    {
        _rxPipelineDestroy(RXPIPELINEGLOBAL(genericMaterialPipeline));
        RXPIPELINEGLOBAL(genericMaterialPipeline) = NULL;
    }
}

static S32 _RpAtomicCreateGenericInstancePipeline(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = WORLD_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            pipeline = RxLockedPipeUnlock(RxLockedPipeAddFragment(
                lpipe, NULL, RxNodeDefinitionGetAtomicInstance(),
                RxNodeDefinitionGetAtomicEnumerateLights(), RxNodeDefinitionGetMaterialScatter(),
                NULL));

            if (pipeline)
            {
                RXPIPELINEGLOBAL(genericAtomicPipeline) = pipeline;
                RXPIPELINEGLOBAL(defaultAtomicPipeline) = pipeline;
                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}

static void _RpAtomicDestroyGenericInstancePipeline(void)
{
    RXPIPELINEGLOBAL(defaultAtomicPipeline) = NULL;

    if (RXPIPELINEGLOBAL(genericAtomicPipeline))
    {
        _rxPipelineDestroy(RXPIPELINEGLOBAL(genericAtomicPipeline));
        RXPIPELINEGLOBAL(genericAtomicPipeline) = NULL;
    }
}

static S32 _RpWorldSectorCreateGenericInstancePipeline(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = WORLD_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            pipeline = RxLockedPipeUnlock(RxLockedPipeAddFragment(
                lpipe, NULL, RxNodeDefinitionGetWorldSectorInstance(),
                RxNodeDefinitionGetWorldSectorEnumerateLights(),
                RxNodeDefinitionGetMaterialScatter(), NULL));

            if (pipeline)
            {
                RXPIPELINEGLOBAL(genericSectorPipeline) = pipeline;
                RXPIPELINEGLOBAL(defaultSectorPipeline) = pipeline;
                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}

static void _RpWorldSectorDestroyGenericInstancePipeline(void)
{
    RXPIPELINEGLOBAL(defaultSectorPipeline) = NULL;

    if (RXPIPELINEGLOBAL(genericSectorPipeline))
    {
        _rxPipelineDestroy(RXPIPELINEGLOBAL(genericSectorPipeline));
        RXPIPELINEGLOBAL(genericSectorPipeline) = NULL;
    }
}

void _rpWorldPipelineClose(void)
{
    _rpDestroyPlatformWorldSectorPipelines();
    _rpDestroyPlatformAtomicPipelines();
    _rpDestroyPlatformMaterialPipelines();

    _RpWorldSectorDestroyGenericInstancePipeline();
    _RpAtomicDestroyGenericInstancePipeline();
    _RpMaterialDestroyGenericRenderPipeline();
}

S32 _rpWorldPipelineOpen(void)
{
    S32 result = _RpMaterialCreateGenericRenderPipeline();

    if (result)
    {
        result = _RpAtomicCreateGenericInstancePipeline();
    }

    if (result)
    {
        result = _RpWorldSectorCreateGenericInstancePipeline();
    }

    if (result)
    {
        result = _rpCreatePlatformMaterialPipelines();
    }

    if (result)
    {
        result = _rpCreatePlatformAtomicPipelines();
    }

    if (result)
    {
        result = _rpCreatePlatformWorldSectorPipelines();
    }

    if (result)
    {
        return TRUE;
    }

    _rpWorldPipelineClose();

    return FALSE;
}

S32 _rpWorldPipeAttach(void)
{
    return TRUE;
}
