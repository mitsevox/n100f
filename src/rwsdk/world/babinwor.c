#include <rpworld.h>

S32 _rpSizeWorldRights(const void* object, S32 offset, S32 size)
{
    const RpWorld* world = (const RpWorld*)object;

    if (world->pipeline && world->pipeline->pluginId)
    {
        return 8;
    }

    return 0;
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
