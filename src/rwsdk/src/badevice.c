#include <rwcore.h>

static RwPluginRegistry engineTKList = { sizeof(RwGlobals), sizeof(RwGlobals) };
static S32 engineInstancesOpened;

static void* MallocWrapper(RwFreeList* fl)
{
    return RWSRCGLOBAL(memoryFuncs).rwmalloc(fl->entrySize);
}

static RwFreeList* FreeWrapper(RwFreeList* fl, void* pData)
{
    RWSRCGLOBAL(memoryFuncs).rwfree(pData);

    return fl;
}

S32 _rwGetNumEngineInstances(void)
{
    return engineInstancesOpened;
}

U32 RwEngineGetVersion(void)
{
    return 0x33002;
}

S32 RwEngineRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor initCB,
                           RwPluginObjectDestructor termCB)
{
    return _rwPluginRegistryAddPlugin(&engineTKList, size, pluginID, initCB, termCB, NULL);
}

S32 RwEngineGetPluginOffset(U32 pluginID)
{
    return _rwPluginRegistryGetPluginOffset(&engineTKList, pluginID);
}
