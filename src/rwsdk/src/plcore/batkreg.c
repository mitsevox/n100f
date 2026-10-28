#include <rwcore.h>

static void* PluginDefaultConstructor(void* object, S32 offset, S32 size)
{
    return object;
}

static void* PluginDefaultDestructor(void* object, S32 offset, S32 size)
{
    return object;
}

static void* PluginDefaultCopy(void* dstObject, const void* srcObject, S32 offset, S32 size)
{
    return dstObject;
}

S32 _rwPluginRegistryGetPluginOffset(const RwPluginRegistry* reg, U32 pluginID)
{
    RwPluginRegEntry* entry = reg->firstRegEntry;

    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            return entry->offset;
        }

        entry = entry->nextRegEntry;
    }

    return -1;
}
