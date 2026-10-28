#include <rwcore.h>

static void rwDestroyEntry(void* mem, void* data)
{
    RwPluginRegEntry* entry = (RwPluginRegEntry*)mem;

    if (entry->parentRegistry->firstRegEntry)
    {
        entry->parentRegistry->sizeOfStruct = entry->parentRegistry->origSizeOfStruct;
        entry->parentRegistry->firstRegEntry = NULL;
        entry->parentRegistry->lastRegEntry = NULL;
    }

    RWSRCGLOBAL(memoryFree)((RwFreeList*)data, entry);
}

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

const RwPluginRegistry* _rwPluginRegistryInitObject(const RwPluginRegistry* reg, void* object)
{
    RwPluginRegEntry* entry = reg->firstRegEntry;

    while (entry)
    {
        if (!entry->constructCB(object, entry->offset, entry->size))
        {
            entry = entry->prevRegEntry;

            while (entry)
            {
                entry->destructCB(object, entry->offset, entry->size);
                entry = entry->prevRegEntry;
            }

            return NULL;
        }

        entry = entry->nextRegEntry;
    }

    return reg;
}

const RwPluginRegistry* _rwPluginRegistryDeInitObject(const RwPluginRegistry* reg, void* object)
{
    RwPluginRegEntry* entry = reg->lastRegEntry;

    while (entry)
    {
        entry->destructCB(object, entry->offset, entry->size);
        entry = entry->prevRegEntry;
    }

    return reg;
}
