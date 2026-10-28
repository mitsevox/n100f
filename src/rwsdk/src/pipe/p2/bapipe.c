#include <rwcore.h>

S32 _rxPipelineGlobalsOffset;

void* _rwRenderPipelineOpen(void* instance, S32 offset, S32 size)
{
    _rxPipelineGlobalsOffset = offset;

    if (_rxPipelineOpen())
    {
        return instance;
    }

    return NULL;
}

void* _rwRenderPipelineClose(void* instance, S32 offset, S32 size)
{
    _rxPipelineClose();

    return instance;
}

S32 _rwPipeAttach(void)
{
    return TRUE;
}

void _rwPipeInitForCamera(const RwCamera* camera)
{
}
