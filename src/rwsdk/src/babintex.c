#include <rwcore.h>

extern RwPluginRegistry textureTKList;

S32 RwTextureRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                  RwPluginDataChunkWriteCallBack writeCB,
                                  RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&textureTKList, pluginID, readCB, writeCB,
                                            getSizeCB);
}

static RwTexture* destroyTexture(RwTexture* texture, void* data)
{
    RwTextureDestroy(texture);

    return texture;
}
