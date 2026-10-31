#include <rpworld.h>

#define WORLD_PLUGIN_ID 2

// Attaches the GameCube world device plugins: the vertex format plugin.
S32 _rxWorldDevicePluginAttach(void)
{
    return _rpDlVtxFmtPluginAttach() != 0;
}

S32 _rpCreatePlatformMaterialPipelines(void)
{
    return TRUE;
}

void _rpDestroyPlatformMaterialPipelines(void)
{
}

S32 _rpCreatePlatformWorldSectorPipelines(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = WORLD_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            if (RxLockedPipeUnlock(RxLockedPipeAddFragment(
                    lpipe, NULL, RxNodeDefinitionGetGameCubeWorldSectorAllInOne(), NULL)))
            {
                RXPIPELINEGLOBAL(platformSectorPipeline) = pipeline;
                RpWorldSetDefaultSectorPipeline(pipeline);
                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}

void _rpDestroyPlatformWorldSectorPipelines(void)
{
    RpWorldSetDefaultSectorPipeline(NULL);

    if (RXPIPELINEGLOBAL(platformSectorPipeline))
    {
        _rxPipelineDestroy(RXPIPELINEGLOBAL(platformSectorPipeline));
        RXPIPELINEGLOBAL(platformSectorPipeline) = NULL;
    }
}

S32 _rpCreatePlatformAtomicPipelines(void)
{
    RxPipeline* pipeline = RxPipelineCreate();

    if (pipeline)
    {
        RxLockedPipe* lpipe;

        pipeline->pluginId = WORLD_PLUGIN_ID;

        lpipe = RxPipelineLock(pipeline);

        if (lpipe)
        {
            if (RxLockedPipeUnlock(RxLockedPipeAddFragment(
                    lpipe, NULL, RxNodeDefinitionGetGameCubeAtomicAllInOne(), NULL)))
            {
                RXPIPELINEGLOBAL(platformAtomicPipeline) = pipeline;
                RpAtomicSetDefaultPipeline(pipeline);
                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}

void _rpDestroyPlatformAtomicPipelines(void)
{
    RpAtomicSetDefaultPipeline(NULL);

    if (RXPIPELINEGLOBAL(platformAtomicPipeline))
    {
        _rxPipelineDestroy(RXPIPELINEGLOBAL(platformAtomicPipeline));
        RXPIPELINEGLOBAL(platformAtomicPipeline) = NULL;
    }
}
