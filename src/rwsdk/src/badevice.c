#include <rwcore.h>
#include <string.h>

// The device system requests the engine makes.
#define DEVICESYSTEMCLOSE 1
#define DEVICESYSTEMGETMODEINFO 6
#define DEVICESYSTEMGETMODE 10

// The error a failed device system request raises.
#define DEVICEERROR 0x18

static RwPluginRegistry engineTKList = { sizeof(RwGlobals), sizeof(RwGlobals) };
static S32 engineInstancesOpened;
static RwGlobals staticGlobals;

// Passes a system request to the device and raises an error naming the request if it fails.
static S32 _rwDeviceSystemRequest(RwDevice* device, S32 requestID, void* pOut, void* pInOut,
                                  S32 nIn)
{
    S32 result = device->fpSystem(requestID, pOut, pInOut, nIn);

    if (!result)
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(DEVICEERROR, requestID);
        RwErrorSet(&err);
    }

    return result;
}

static void* MallocWrapper(RwFreeList* fl)
{
    return RWSRCGLOBAL(memoryFuncs).rwmalloc(fl->entrySize);
}

static RwFreeList* FreeWrapper(RwFreeList* fl, void* pData)
{
    RWSRCGLOBAL(memoryFuncs).rwfree(pData);

    return fl;
}

U32 _rwGetNumEngineInstances(void)
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

RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* modeinfo, S32 modeIndex)
{
    if (!_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice), DEVICESYSTEMGETMODEINFO, modeinfo,
                                NULL, modeIndex))
    {
        modeinfo = NULL;
    }

    return modeinfo;
}

S32 RwEngineGetCurrentVideoMode(void)
{
    S32 currentMode;

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice), DEVICESYSTEMGETMODE, &currentMode, NULL,
                               0))
    {
        return currentMode;
    }

    return -1;
}

S32 RwEngineClose(void)
{
    S32 result =
        _rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice), DEVICESYSTEMCLOSE, NULL, NULL, 0);

    if (result)
    {
        RwGlobals* globals = (RwGlobals*)RwEngineInstance;

        RwEngineInstance = &staticGlobals;
        memcpy(&staticGlobals, globals, sizeof(RwGlobals));
        RWSRCGLOBAL(memoryFuncs).rwfree(globals);

        engineInstancesOpened--;
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSINITED;
    }

    return result;
}
