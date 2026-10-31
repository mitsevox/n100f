#include <rwcore.h>

static RwModuleInfo colorModule;

void* _rwColorOpen(void* instance, S32 offset, S32 size)
{
    colorModule.numInstances++;

    return instance;
}

void* _rwColorClose(void* instance, S32 offset, S32 size)
{
    colorModule.numInstances--;

    return instance;
}
