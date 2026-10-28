#include <rwcore.h>

// The stream module's data in the engine instance.
typedef struct StreamModuleData StreamModuleData;
struct StreamModuleData
{
    RwFreeList* freeList;
};

#define STREAM_DATA(var) \
    (((StreamModuleData*)((U8*)RwEngineInstance + streamModule.globalsOffset))->var)

static RwModuleInfo streamModule;

void* _rwStreamModuleClose(void* instance, S32 offset, S32 size)
{
    if (STREAM_DATA(freeList))
    {
        RwFreeListDestroy(STREAM_DATA(freeList));
    }

    streamModule.numInstances--;

    return instance;
}
