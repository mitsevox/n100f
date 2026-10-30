#include <rwcore.h>
#include <string.h>

// The device system requests the engine makes.
#define DEVICESYSTEMOPEN 0
#define DEVICESYSTEMCLOSE 1
#define DEVICESYSTEMSTART 2
#define DEVICESYSTEMSTOP 3
#define DEVICESYSTEMREGISTER 4
#define DEVICESYSTEMGETMODEINFO 6
#define DEVICESYSTEMGETMODE 10
#define DEVICESYSTEMSTANDARDS 11
#define DEVICESYSTEMFINALIZESTART 17
#define DEVICESYSTEMINITIATESTOP 18

// The number of standard functions the device fills in when the engine opens.
#define NUMSTANDARDFUNCS 29

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

    // A device need not handle the requests around starting and stopping.
    if (!result &&
        (requestID == DEVICESYSTEMFINALIZESTART || requestID == DEVICESYSTEMINITIATESTOP))
    {
        result = TRUE;
    }

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

void fn_8016D498(F32 gamma);
void fn_80163A8C(void);

// Lets the plugins and the device shut down, leaving the engine open.
S32 RwEngineStop(void)
{
    RwDevice* device = &RWSRCGLOBAL(dOpenDevice);
    S32 result;

    _rwDeviceSystemRequest(device, DEVICESYSTEMINITIATESTOP, NULL, NULL, 0);
    _rwPluginRegistryDeInitObject(&engineTKList, RwEngineInstance);

    result = _rwDeviceSystemRequest(device, DEVICESYSTEMSTOP, NULL, NULL, 0);

    if (result)
    {
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSOPENED;
    }

    return result;
}

// Starts the device, then the plugins attached to the engine.
S32 RwEngineStart(void)
{
    RwDevice* device = &RWSRCGLOBAL(dOpenDevice);
    S32 result = _rwDeviceSystemRequest(device, DEVICESYSTEMSTART, NULL, NULL, 0);

    if (result)
    {
        if (_rwPluginRegistryInitObject(&engineTKList, RwEngineInstance))
        {
            fn_8016D498(RWSRCGLOBAL(dOpenDevice).gammaCorrection);
            _rwDeviceSystemRequest(device, DEVICESYSTEMFINALIZESTART, NULL, NULL, 0);
            RWSRCGLOBAL(engineStatus) = rwENGINESTATUSSTARTED;
            return TRUE;
        }

        _rwDeviceSystemRequest(device, DEVICESYSTEMSTOP, NULL, NULL, 0);
    }

    return FALSE;
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

// Moves the engine globals into their own allocation and opens the device on them.
static S32 EngineOpenDevice(RwDevice* device, RwEngineOpenParams* initParams)
{
    RwGlobals* globals;

    RwEngineInstance = RwMalloc(engineTKList.sizeOfStruct);
    globals = (RwGlobals*)RwEngineInstance;

    if (globals)
    {
        memcpy(RwEngineInstance, &staticGlobals, sizeof(RwGlobals));

        _rwDeviceSystemRequest(device, DEVICESYSTEMREGISTER, &RWSRCGLOBAL(dOpenDevice),
                               &RWSRCGLOBAL(memoryFuncs), 0);

        if (_rwDeviceSystemRequest(device, DEVICESYSTEMOPEN, NULL, initParams, 0))
        {
            _rwDeviceSystemRequest(device, DEVICESYSTEMSTANDARDS, RWSRCGLOBAL(stdFunc), NULL,
                                   NUMSTANDARDFUNCS);
            engineInstancesOpened++;

            return TRUE;
        }

        RwEngineInstance = &staticGlobals;
        memcpy(&staticGlobals, globals, sizeof(RwGlobals));
        RwFree(globals);

        return FALSE;
    }
    else
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000013, engineTKList.sizeOfStruct);
        RwErrorSet(&err);
    }

    return FALSE;
}

// Opens the engine on the platform's device.
S32 RwEngineOpen(RwEngineOpenParams* initParams)
{
    S32 result;

    if (!RwEngineInstance)
    {
        RwEngineInstance = &staticGlobals;
    }

    result = (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSINITED);

    if (result)
    {
        result = (initParams != NULL);

        if (result)
        {
            RwDevice* device = _rwDeviceGetHandle();

            result = (device != NULL);

            if (result)
            {
                result = EngineOpenDevice(device, initParams);

                if (result)
                {
                    RWSRCGLOBAL(engineStatus) = rwENGINESTATUSOPENED;
                }
            }
        }
        else
        {
            RwError err;
            err.pluginID = 1;
            err.errorCode = _rwerror(0x80000016);
            RwErrorSet(&err);
        }
    }
    else
    {
        RwError err;
        err.pluginID = 1;
        err.errorCode = _rwerror(0x80000001);
        RwErrorSet(&err);
    }

    return result;
}

// Closes the engine's systems once no instance is open.
S32 RwEngineTerm(void)
{
    S32 result = (engineInstancesOpened == 0);

    if (result)
    {
        _rwPluginRegistryClose();
        _rwFileSystemClose();
        fn_80163A8C();
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSIDLE;
    }

    return result;
}
