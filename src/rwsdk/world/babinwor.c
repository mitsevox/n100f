#include <rpworld.h>

static S32 lastSeenWorldRightsPluginId;
static S32 lastSeenWorldExtraData;
static S32 lastSeenSectRightsPluginId;
static S32 lastSeenSectExtraData;

RwStream* _rpReadWorldRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size)
{
    if (!RwStreamReadInt32(stream, &lastSeenWorldRightsPluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (length == 8)
    {
        if (!RwStreamReadInt32(stream, &lastSeenWorldExtraData, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

RwStream* _rpWriteWorldRights(RwStream* stream, S32 length, const void* object, S32 offset,
                              S32 size)
{
    const RpWorld* world = (const RpWorld*)object;

    if (!RwStreamWriteInt32(stream, (const S32*)&world->pipeline->pluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, (const S32*)&world->pipeline->pluginData, sizeof(S32)))
    {
        return NULL;
    }

    return stream;
}

S32 _rpSizeWorldRights(const void* object, S32 offset, S32 size)
{
    const RpWorld* world = (const RpWorld*)object;

    if (world->pipeline && world->pipeline->pluginId)
    {
        return 8;
    }

    return 0;
}

RwStream* _rpReadSectRights(RwStream* stream, S32 length, void* object, S32 offset, S32 size)
{
    if (!RwStreamReadInt32(stream, &lastSeenSectRightsPluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (length == 8)
    {
        if (!RwStreamReadInt32(stream, &lastSeenSectExtraData, sizeof(S32)))
        {
            return NULL;
        }
    }

    return stream;
}

RwStream* _rpWriteSectRights(RwStream* stream, S32 length, const void* object, S32 offset,
                             S32 size)
{
    const RpWorldSector* sector = (const RpWorldSector*)object;

    if (!RwStreamWriteInt32(stream, (const S32*)&sector->pipeline->pluginId, sizeof(S32)))
    {
        return NULL;
    }

    if (!RwStreamWriteInt32(stream, (const S32*)&sector->pipeline->pluginData, sizeof(S32)))
    {
        return NULL;
    }

    return stream;
}

S32 _rpSizeSectRights(const void* object, S32 offset, S32 size)
{
    const RpWorldSector* sector = (const RpWorldSector*)object;

    if (sector->pipeline && sector->pipeline->pluginId)
    {
        return 8;
    }

    return 0;
}
