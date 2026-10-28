#include <rwcore.h>

S32 _rwPluginRegistryAddPluginStream(RwPluginRegistry* reg, U32 pluginID,
                                     RwPluginDataChunkReadCallBack readCB,
                                     RwPluginDataChunkWriteCallBack writeCB,
                                     RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwPluginRegEntry* entry;

    for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
    {
        if (entry->pluginID == pluginID)
        {
            break;
        }
    }

    if (entry)
    {
        entry->readCB = readCB;
        entry->writeCB = writeCB;
        entry->getSizeCB = getSizeCB;

        return entry->offset;
    }

    return -1;
}

S32 _rwPluginRegistryAddPlgnStrmlwysCB(RwPluginRegistry* reg, U32 pluginID,
                                       RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwPluginRegEntry* entry;

    for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
    {
        if (entry->pluginID == pluginID)
        {
            break;
        }
    }

    if (entry)
    {
        entry->alwaysCB = alwaysCB;

        return entry->offset;
    }

    return -1;
}

S32 _rwPluginRegistryAddPlgnStrmRightsCB(RwPluginRegistry* reg, U32 pluginID,
                                         RwPluginDataChunkRightsCallBack rightsCB)
{
    RwPluginRegEntry* entry;

    for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
    {
        if (entry->pluginID == pluginID)
        {
            break;
        }
    }

    if (entry)
    {
        entry->rightsCB = rightsCB;

        return entry->offset;
    }

    return -1;
}
