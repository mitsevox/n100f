#include <rwcore.h>

void* _rwResourcesClose(void* instance, S32 offset, S32 size)
{
    RwResourcesEmptyArena();

    _rwResHeapClose(RWRESOURCESGLOBAL(heap));
    RWSRCGLOBAL(memoryFuncs).rwfree(RWRESOURCESGLOBAL(heap));
    RWRESOURCESGLOBAL(heap) = NULL;

    resourcesModule.numInstances--;

    return instance;
}

S32 RwResourcesFreeResEntry(RwResEntry* entry)
{
    if (entry->destroyNotify)
    {
        entry->destroyNotify(entry);
    }

    if (entry->ownerRef)
    {
        *entry->ownerRef = NULL;
    }

    if (entry->link.next)
    {
        rwLinkListRemoveLLLink(&entry->link);
        RWRESOURCESGLOBAL(usedSize) -= entry->size;
        _rwResHeapFree(entry);
    }
    else
    {
        RWSRCGLOBAL(memoryFuncs).rwfree(entry);
    }

    return TRUE;
}

void _rwResourcesPurge(void)
{
    RwLinkList* usedList = RWRESOURCESGLOBAL(usedEntryList);
    RwLinkList* unusedList = RWRESOURCESGLOBAL(unusedEntryList);

    if (!rwLinkListEmpty(unusedList))
    {
        if (rwLinkListEmpty(usedList))
        {
            usedList->link.next = unusedList->link.next;
            usedList->link.next->prev = &usedList->link;
            usedList->link.prev = unusedList->link.prev;
            usedList->link.prev->next = &usedList->link;
            rwLinkListInitialize(unusedList);
        }
        else
        {
            RwLLLink* tail = usedList->link.prev;
            RwLLLink* first = unusedList->link.next;

            tail->next = first;
            first->prev = tail;
            tail = unusedList->link.prev;
            tail->next = &usedList->link;
            usedList->link.prev = tail;
            rwLinkListInitialize(unusedList);
        }
    }

    RWRESOURCESGLOBAL(usedEntryList) = unusedList;
    RWRESOURCESGLOBAL(unusedEntryList) = usedList;
    RWRESOURCESGLOBAL(reclaimedSize) = 0;
}

RwResEntry* RwResourcesAllocateResEntry(void* owner, RwResEntry** ownerRef, S32 size,
                                        RwResEntryDestroyNotify destroyNotify)
{
    RwResEntry* oldest;
    S32 arenaFull = FALSE;
    RwError err;

    while (!arenaFull)
    {
        RwResEntry* entry = (RwResEntry*)_rwResHeapAlloc(RWRESOURCESGLOBAL(heap), size + sizeof(RwResEntry));

        if (entry)
        {
            rwLinkListAddLLLink(RWRESOURCESGLOBAL(usedEntryList), &entry->link);
            entry->owner = owner;
            entry->size = size;
            entry->ownerRef = ownerRef;
            entry->destroyNotify = destroyNotify;
            RWRESOURCESGLOBAL(usedSize) += size;

            if (ownerRef)
            {
                *ownerRef = entry;
            }

            return entry;
        }
        else
        {
            RwLinkList* list = RWRESOURCESGLOBAL(unusedEntryList);

            if (list->link.prev != &list->link)
            {
                RwResourcesFreeResEntry((RwResEntry*)list->link.prev);
            }
            else
            {
                list = RWRESOURCESGLOBAL(usedEntryList);
                oldest = (RwResEntry*)list->link.prev;

                if ((RwLLLink*)oldest != &list->link)
                {
                    RWRESOURCESGLOBAL(reclaimedSize) += oldest->size;
                    RwResourcesFreeResEntry(oldest);
                }
                else
                {
                    arenaFull = TRUE;
                }
            }
        }
    }

    if (ownerRef)
    {
        *ownerRef = NULL;
    }

    err.pluginID = 1;
    err.errorCode = _rwerror(0xC, size);
    RwErrorSet(&err);

    return NULL;
}

S32 RwResourcesSetArenaSize(U32 size)
{
    RwResourcesGlobals* globals;

    if (!resourcesModule.numInstances)
    {
        RWSRCGLOBAL(resArenaInitSize) = size;
        return TRUE;
    }

    globals = (RwResourcesGlobals*)((U8*)RwEngineInstance + resourcesModule.globalsOffset);
    globals->arenaSize = size;

    RwResourcesEmptyArena();
    _rwResHeapClose(globals->heap);
    RWSRCGLOBAL(memoryFuncs).rwfree(globals->heap);

    globals->heap = RWSRCGLOBAL(memoryFuncs).rwmalloc(size);

    if (!globals->heap)
    {
        RwError err;

        globals->arenaSize = 0;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, size);
        RwErrorSet(&err);
        return FALSE;
    }

    if (!_rwResHeapInit(globals->heap, size))
    {
        RwError err;

        RWSRCGLOBAL(memoryFuncs).rwfree(globals->heap);
        err.pluginID = 1;
        err.errorCode = _rwerror(0xC, 0);
        RwErrorSet(&err);
        return FALSE;
    }

    return TRUE;
}

S32 RwResourcesGetArenaSize(void)
{
    return RWRESOURCESGLOBAL(arenaSize);
}

S32 RwResourcesEmptyArena(void)
{
    RwLLLink* cur;
    RwLLLink* end;

    RWRESOURCESGLOBAL(entryLists[0]).link.prev->next = RWRESOURCESGLOBAL(entryLists[1]).link.next;

    cur = rwLinkListGetFirstLLLink(&RWRESOURCESGLOBAL(entryLists[0]));
    end = rwLinkListGetTerminator(&RWRESOURCESGLOBAL(entryLists[1]));

    while (cur != end)
    {
        RwResEntry* entry = (RwResEntry*)cur;

        cur = rwLLLinkGetNext(cur);
        RwResourcesFreeResEntry(entry);
    }

    rwLinkListInitialize(&RWRESOURCESGLOBAL(entryLists[0]));
    rwLinkListInitialize(&RWRESOURCESGLOBAL(entryLists[1]));
    RWRESOURCESGLOBAL(reclaimedSize) = 0;

    return TRUE;
}
