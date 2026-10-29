#include <rwcore.h>

#define IM3D_PLUGIN_ID 1

S32 _rwIm3DCreatePlatformTransformPipeline(RxPipeline** globalPipe)
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
                lpipe, NULL, RxNodeDefinitionGetGameCubeImmInstance(), NULL));

            if (pipeline)
            {
                *globalPipe = pipeline;
                RwIm3DSetTransformPipeline(pipeline);

                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}

void _rwIm3DDestroyPlatformTransformPipeline(RxPipeline** globalPipe)
{
    RwIm3DSetTransformPipeline(NULL);

    if (*globalPipe)
    {
        _rxPipelineDestroy(*globalPipe);
        *globalPipe = NULL;
    }
}

void _rwIm3DDestroyPlatformRenderPipelines(rwIm3DRenderPipelines* globalPipes)
{
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPETRILIST);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPETRIFAN);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPETRISTRIP);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPELINELIST);
    RwIm3DSetRenderPipeline(NULL, rwPRIMTYPEPOLYLINE);

    if (globalPipes->triList)
    {
        _rxPipelineDestroy(globalPipes->triList);
    }

    globalPipes->triList = NULL;
    globalPipes->triFan = NULL;
    globalPipes->triStrip = NULL;
    globalPipes->lineList = NULL;
    globalPipes->polyLine = NULL;
}

S32 _rwIm3DCreatePlatformRenderPipelines(rwIm3DRenderPipelines* globalPipes)
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
                lpipe, NULL, RxNodeDefinitionGetGameCubeSubmitNoLight(), NULL));

            if (pipeline)
            {
                globalPipes->triList = pipeline;
                globalPipes->triFan = pipeline;
                globalPipes->triStrip = pipeline;
                globalPipes->lineList = pipeline;
                globalPipes->polyLine = pipeline;

                RwIm3DSetRenderPipeline(pipeline, rwPRIMTYPETRILIST);
                RwIm3DSetRenderPipeline(pipeline, rwPRIMTYPETRIFAN);
                RwIm3DSetRenderPipeline(pipeline, rwPRIMTYPETRISTRIP);
                RwIm3DSetRenderPipeline(pipeline, rwPRIMTYPELINELIST);
                RwIm3DSetRenderPipeline(pipeline, rwPRIMTYPEPOLYLINE);

                return TRUE;
            }
        }

        _rxPipelineDestroy(pipeline);
    }

    return FALSE;
}
