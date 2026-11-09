#include <rpskin.h>

// The GameCube skin pipelines: one pipeline, created when the generic skin type is asked for and
// attached to every skinned atomic.

S32 _rpSkinPipelinesCreate(U32 pipes)
{
    SkinGlobals* globals = &_rpSkinGlobals;

    if (pipes & (1 << (rpSKINTYPEGENERIC - 1)))
    {
        globals->pipeline = _rpSkinPipelineCreate(rpSKINTYPEGENERIC, NULL, _rpSkinAtomicReinstanceCallBack, NULL);
    }

    return TRUE;
}

S32 _rpSkinPipelinesDestroy(void)
{
    if (_rpSkinGlobals.pipeline)
    {
        _rxPipelineDestroy(_rpSkinGlobals.pipeline);
        _rpSkinGlobals.pipeline = NULL;
    }

    return TRUE;
}

RpAtomic* _rpSkinPipelinesAttach(RpAtomic* atomic, RpSkinType type)
{
    atomic->pipeline = _rpSkinGlobals.pipeline;

    return atomic;
}
