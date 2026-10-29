#include <rwcore.h>

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
