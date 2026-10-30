#include <rwcore.h>

// The size of a chunk header in the stream, and the build number chunk headers are written with.
#define CHUNKHEADER_SIZE 12
#define LIBRARY_BUILDNUM 0xFFFF

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

const RwPluginRegistry* _rwPluginRegistryReadDataChunks(const RwPluginRegistry* reg,
                                                        RwStream* stream, void* object)
{
    U32 length;
    U32 version;

    if (!RwStreamFindChunk(stream, rwID_EXTENSION, &length, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        RwPluginRegEntry* entry;

        while (length > 0)
        {
            U32 type;
            U32 chunkLength;

            if (!_rwStreamReadChunkHeader(stream, &type, &chunkLength, NULL, NULL))
            {
                return NULL;
            }

            for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
            {
                if (entry->pluginID == type)
                {
                    break;
                }
            }

            if (entry && entry->readCB)
            {
                if (!entry->readCB(stream, chunkLength, object, entry->offset, entry->size))
                {
                    return NULL;
                }
            }
            else
            {
                if (!RwStreamSkip(stream, chunkLength))
                {
                    return NULL;
                }
            }

            length -= chunkLength + CHUNKHEADER_SIZE;
        }

        for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
        {
            if (entry->alwaysCB)
            {
                if (!entry->alwaysCB(object, entry->offset, entry->size))
                {
                    return NULL;
                }
            }
        }

        return reg;
    }
    else
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);

        return NULL;
    }
}

const RwPluginRegistry* _rwPluginRegistryInvokeRights(const RwPluginRegistry* reg, U32 id,
                                                      void* obj, U32 extraData)
{
    RwPluginRegEntry* entry;

    for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
    {
        if (entry->pluginID == id)
        {
            break;
        }
    }

    if (entry && entry->rightsCB)
    {
        if (entry->rightsCB(obj, entry->offset, entry->size, extraData))
        {
            return reg;
        }

        return NULL;
    }

    return NULL;
}

S32 _rwPluginRegistryGetSize(const RwPluginRegistry* reg, const void* object)
{
    S32 size = 0;
    RwPluginRegEntry* entry;

    for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
    {
        if (entry->getSizeCB)
        {
            S32 chunkSize = entry->getSizeCB(object, entry->offset, entry->size);

            if (chunkSize > 0)
            {
                size += chunkSize + CHUNKHEADER_SIZE;
            }
        }
    }

    return size;
}

const RwPluginRegistry* _rwPluginRegistryWriteDataChunks(const RwPluginRegistry* reg,
                                                         RwStream* stream, const void* object)
{
    RwPluginRegEntry* entry;
    S32 size = _rwPluginRegistryGetSize(reg, object);

    if (!_rwStreamWriteVersionedChunkHeader(stream, rwID_EXTENSION, size, rwLIBRARYCURRENTVERSION,
                                            LIBRARY_BUILDNUM))
    {
        return NULL;
    }

    for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
    {
        if (entry->getSizeCB && entry->writeCB)
        {
            S32 chunkSize = entry->getSizeCB(object, entry->offset, entry->size);

            if (chunkSize > 0)
            {
                if (!_rwStreamWriteVersionedChunkHeader(stream, entry->pluginID, chunkSize,
                                                        rwLIBRARYCURRENTVERSION, LIBRARY_BUILDNUM))
                {
                    return NULL;
                }

                if (!entry->writeCB(stream, chunkSize, object, entry->offset, entry->size))
                {
                    return NULL;
                }
            }
        }
    }

    return reg;
}

const RwPluginRegistry* _rwPluginRegistrySkipDataChunks(const RwPluginRegistry* reg,
                                                        RwStream* stream)
{
    U32 length;

    if (!RwStreamFindChunk(stream, rwID_EXTENSION, &length, NULL))
    {
        return NULL;
    }

    while (length > 0)
    {
        U32 chunkLength;

        if (!_rwStreamReadChunkHeader(stream, NULL, &chunkLength, NULL, NULL))
        {
            return NULL;
        }

        if (!RwStreamSkip(stream, chunkLength))
        {
            return NULL;
        }

        length -= chunkLength + CHUNKHEADER_SIZE;
    }

    return reg;
}
