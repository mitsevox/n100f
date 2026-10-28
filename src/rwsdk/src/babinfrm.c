#include <rwcore.h>

typedef struct rwFrameList rwFrameList;
struct rwFrameList
{
    RwFrame** frames;
    S32 numFrames;
};

extern RwPluginRegistry frameTKList;

rwFrameList* _rwFrameListDeinitialize(rwFrameList* frameList)
{
    if (frameList->numFrames)
    {
        RWSRCGLOBAL(memoryFuncs).rwfree(frameList->frames);
    }

    return frameList;
}

S32 RwFrameRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&frameTKList, pluginID, readCB, writeCB, getSizeCB);
}
