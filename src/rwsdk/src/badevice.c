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
// The compiler lays out uninitialized variables in reverse order of definition.
static RwGlobals staticGlobals;
void* RwEngineInstance;
static S32 engineInstancesOpened;

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

S32 RwImageSetGamma(F32 gammaValue);
void _rwMemoryClose(void);

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
            RwImageSetGamma(RWSRCGLOBAL(dOpenDevice).gammaCorrection);
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
static S32 EngineOpen(RwDevice* device, RwEngineOpenParams* initParams)
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
                result = EngineOpen(device, initParams);

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

S32 _rwStringOpen(void);
void _rwStringClose(void);
S32 _rwMemoryOpen(const RwMemoryFunctions* memFuncs);
void _rwMemoryClose(void);
S32 _rwFileSystemOpen(void);
S32 _rwPluginRegistryOpen(void);
S32 _rwPipeAttach(void);
void* _rwErrorOpen(void* instance, S32 offset, S32 size);
void* _rwErrorClose(void* instance);
void* _rwVectorOpen(void* instance, S32 offset, S32 size);
void* _rwVectorClose(void* instance);
void* _rwColorOpen(void* instance, S32 offset, S32 size);
void* _rwColorClose(void* instance, S32 offset, S32 size);
void* _rwMatrixOpen(void* instance, S32 offset, S32 size);
void* _rwMatrixClose(void* instance, S32 offset, S32 size);
void* _rwFrameOpen(void* instance, S32 offset, S32 size);
void* _rwFrameClose(void* instance, S32 offset, S32 size);
void* _rwStreamModuleOpen(void* instance, S32 offset, S32 size);
void* _rwStreamModuleClose(void* instance, S32 offset, S32 size);
void* _rwCameraOpen(void* instance, S32 offset, S32 size);
void* _rwCameraClose(void* instance, S32 offset, S32 size);
void* _rwImageOpen(void* instance, S32 offset, S32 size);
void* _rwImageClose(void* instance, S32 offset, S32 size);
void* _rwRasterOpen(void* instance, S32 offset, S32 size);
void* _rwRasterClose(void* instance, S32 offset, S32 size);
void* _rwTextureOpen(void* instance, S32 offset, S32 size);
void* _rwTextureClose(void* instance, S32 offset, S32 size);
void* _rwRenderPipelineOpen(void* instance, S32 offset, S32 size);
void* _rwRenderPipelineClose(void* instance, S32 offset, S32 size);
void* _rwIm3DOpen(void* instance, S32 offset, S32 size);
void* _rwIm3DClose(void* instance, S32 offset, S32 size);
void* _rwResourcesOpen(void* instance, S32 offset, S32 size);
void* _rwResourcesClose(void* instance, S32 offset, S32 size);

// Closes the engine's systems once no instance is open.
S32 RwEngineTerm(void)
{
    S32 result = (engineInstancesOpened == 0);

    if (result)
    {
        _rwPluginRegistryClose();
        _rwFileSystemClose();
        _rwMemoryClose();
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSIDLE;
    }

    return result;
}

// Sets up the engine's static globals (allocating from the free lists, or straight from the
// memory functions with rwENGINEINITNOFREELISTS) and, the first time, opens the string, memory,
// file system and plugin registry systems and registers the core modules as engine plugins.
// Whatever opened is closed again if a later step fails.
S32 RwEngineInit(const RwMemoryFunctions* memFuncs, U32 initFlags, U32 resArenaSize)
{
    S32 result = FALSE;

    RwEngineInstance = &staticGlobals;

    if (initFlags & 1)
    {
        RWSRCGLOBAL(memoryAlloc) = MallocWrapper;
        RWSRCGLOBAL(memoryFree) = FreeWrapper;
    }
    else
    {
        RWSRCGLOBAL(memoryAlloc) = _rwFreeListAllocReal;
        RWSRCGLOBAL(memoryFree) = _rwFreeListFreeReal;
    }

    RWSRCGLOBAL(resArenaInitSize) = resArenaSize;

    if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSIDLE)
    {
        if ((result = _rwStringOpen()))
        {
            if ((result = _rwMemoryOpen(memFuncs)))
            {
                if ((result = _rwFileSystemOpen()))
                {
                    if ((result = _rwPluginRegistryOpen()))
                    {
                        S32 status;

                        status = _rwPluginRegistryAddPlugin(
                            &engineTKList, 8, 0x40F, _rwErrorOpen,
                            (RwPluginObjectDestructor)_rwErrorClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(
                            &engineTKList, 16, 0x401, _rwVectorOpen,
                            (RwPluginObjectDestructor)_rwVectorClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 0, 0x40D, _rwColorOpen,
                                                             _rwColorClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 24, 0x402,
                                                             _rwMatrixOpen, _rwMatrixClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 4, 0x403, _rwFrameOpen,
                                                             _rwFrameClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 4, 0x404,
                                                             _rwStreamModuleOpen,
                                                             _rwStreamModuleClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 4, 0x405,
                                                             _rwCameraOpen, _rwCameraClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 0x220, 0x406,
                                                             _rwImageOpen, _rwImageClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 0x64, 0x407,
                                                             _rwRasterOpen, _rwRasterClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 0x30, 0x408,
                                                             _rwTextureOpen, _rwTextureClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 0x64, 0x409,
                                                             _rwRenderPipelineOpen,
                                                             _rwRenderPipelineClose, NULL);
                        status |= _rwPipeAttach();
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 0x90, 0x40A,
                                                             _rwIm3DOpen, _rwIm3DClose, NULL);
                        status |= _rwPluginRegistryAddPlugin(&engineTKList, 0x28, 0x40B,
                                                             _rwResourcesOpen, _rwResourcesClose,
                                                             NULL);

                        status = (status >= 0);
                        result = status;
                        if (status)
                        {
                            RWSRCGLOBAL(engineStatus) = rwENGINESTATUSINITED;
                            return status;
                        }

                        _rwPluginRegistryClose();
                    }

                    _rwFileSystemClose();
                }

                _rwMemoryClose();
            }

            _rwStringClose();
        }
    }

    return result;
}
