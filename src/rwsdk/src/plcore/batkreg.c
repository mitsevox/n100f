#include <rwcore.h>

static RwFreeList* toolkitRegEntries;
static U32 numRegToolkits;
static RwPluginRegistry** toolkitNonFLRegList;

// Creates the free list the plugin registry entries come from (twenty per block).
S32 _rwPluginRegistryOpen(void)
{
    toolkitRegEntries = RwFreeListCreate(sizeof(RwPluginRegEntry), 20, 0);
    if (!toolkitRegEntries)
    {
        return FALSE;
    }

    numRegToolkits = 0;

    return TRUE;
}

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

S32 _rwPluginRegistryClose(void)
{
    if (toolkitRegEntries)
    {
        RwFreeListForAllUsed(toolkitRegEntries, rwDestroyEntry, toolkitRegEntries);

        if (RWSRCGLOBAL(memoryAlloc) != _rwFreeListAllocReal)
        {
            U32 i;

            for (i = 0; i < numRegToolkits; i++)
            {
                RwPluginRegEntry* entry = toolkitNonFLRegList[i]->firstRegEntry;
                RwPluginRegistry* reg = NULL;

                if (entry)
                {
                    reg = entry->parentRegistry;
                }

                while (entry)
                {
                    RwPluginRegEntry* next = entry->nextRegEntry;

                    RWSRCGLOBAL(memoryFree)(NULL, entry);
                    entry = next;
                }

                if (reg && reg->firstRegEntry)
                {
                    reg->sizeOfStruct = reg->origSizeOfStruct;
                    reg->firstRegEntry = NULL;
                    reg->lastRegEntry = NULL;
                }
            }

            if (toolkitNonFLRegList)
            {
                RwFree(toolkitNonFLRegList);
                toolkitNonFLRegList = NULL;
            }
        }

        RwFreeListDestroy(toolkitRegEntries);
        toolkitRegEntries = NULL;
    }

    return TRUE;
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

S32 _rwPluginRegistryAddPlugin(RwPluginRegistry* reg, S32 size, U32 pluginID,
                               RwPluginObjectConstructor constructCB,
                               RwPluginObjectDestructor destructCB,
                               RwPluginObjectCopy copyCB)
{
    RwPluginRegEntry* entry;
    S32 newSize;

    if (!toolkitRegEntries)
    {
        return -1;
    }

    if (_rwGetNumEngineInstances())
    {
        RwError err;

        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000017);
        RwErrorSet(&err);

        return -1;
    }

    if (RWSRCGLOBAL(memoryAlloc) != _rwFreeListAllocReal)
    {
        U32 i;

        for (i = 0; i < numRegToolkits; i++)
        {
            if (reg == toolkitNonFLRegList[i])
            {
                break;
            }
        }

        if (numRegToolkits == i)
        {
            RwPluginRegistry** list =
                (RwPluginRegistry**)RwMalloc(sizeof(RwPluginRegistry*) * (numRegToolkits + 1));
            U32 j = 0;

            if (toolkitNonFLRegList)
            {
                while (j < numRegToolkits)
                {
                    list[j] = toolkitNonFLRegList[j];
                    j++;
                }

                RwFree(toolkitNonFLRegList);
                toolkitNonFLRegList = NULL;
            }

            list[j] = reg;
            toolkitNonFLRegList = list;
            numRegToolkits++;
        }
    }

    for (entry = reg->firstRegEntry; entry; entry = entry->nextRegEntry)
    {
        if (entry->pluginID == pluginID)
        {
            RwError err;

            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000017);
            RwErrorSet(&err);

            return entry->offset;
        }
    }

    newSize = reg->sizeOfStruct + ((size + 3) & ~3);

    if (reg->maxSizeOfStruct && newSize > reg->maxSizeOfStruct)
    {
        return -1;
    }

    entry = (RwPluginRegEntry*)RWSRCGLOBAL(memoryAlloc)(toolkitRegEntries);

    if (entry)
    {
        entry->offset = reg->sizeOfStruct;
        reg->sizeOfStruct = newSize;
        entry->size = size;
        entry->pluginID = pluginID;
        entry->readCB = NULL;
        entry->writeCB = NULL;
        entry->getSizeCB = NULL;
        entry->alwaysCB = NULL;
        entry->rightsCB = NULL;
        entry->constructCB = constructCB ? constructCB : PluginDefaultConstructor;
        entry->destructCB = destructCB ? destructCB : PluginDefaultDestructor;
        entry->copyCB = copyCB ? copyCB : PluginDefaultCopy;
        entry->errStrCB = NULL;
        entry->nextRegEntry = NULL;
        entry->prevRegEntry = NULL;
        entry->parentRegistry = reg;

        if (!reg->firstRegEntry)
        {
            reg->firstRegEntry = entry;
            reg->lastRegEntry = entry;
        }
        else
        {
            reg->lastRegEntry->nextRegEntry = entry;
            entry->prevRegEntry = reg->lastRegEntry;
            reg->lastRegEntry = entry;
        }

        return entry->offset;
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
