#include <rpworld.h>

typedef RwCamera* (*RwCameraCallBack)(RwCamera* camera, void* data);
typedef RpLight* (*RpLightCallBack)(RpLight* light, void* data);

// The extension the clump plugin adds to cameras and lights: the owning clump and the link in
// that clump's camera or light list.
typedef struct ClumpObjectExt ClumpObjectExt;
struct ClumpObjectExt
{
    RpClump* clump;
    RwLLLink inClumpLink;
};

#define CLUMPEXTFROMOBJECT(object, offset) ((ClumpObjectExt*)((U8*)(object) + (offset)))
#define OBJECTFROMCLUMPLINK(link, offset) \
    ((void*)((U8*)(link) - (S32)&((ClumpObjectExt*)0)->inClumpLink - (offset)))

static RwPluginRegistry atomicTKList = { sizeof(RpAtomic), sizeof(RpAtomic) };
static RwPluginRegistry clumpTKList = { sizeof(RpClump), sizeof(RpClump) };

static S32 _rpClumpCameraExtOffset;
static S32 _rpClumpLightExtOffset;

void _rpAtomicResyncInterpolatedSphere(RpAtomic* atomic);

static void ClumpTidyDestroyClump(void* clump, void* data)
{
    RpClumpDestroy((RpClump*)clump);
}

static void ClumpTidyDestroyAtomic(void* atomic, void* data)
{
    RpAtomicDestroy((RpAtomic*)atomic);
}

S32 _rpSizeAtomicRights(const void* object, S32 offset, S32 size)
{
    const RpAtomic* atomic = (const RpAtomic*)object;

    if (atomic->pipeline && atomic->pipeline->pluginId)
    {
        return 8;
    }

    return 0;
}

static RwObjectHasFrame* AtomicSync(RwObjectHasFrame* object)
{
    RpAtomic* atomic = (RpAtomic*)object;

    if (atomic->interpolator.flags & rpINTERPOLATORDIRTYSPHERE)
    {
        _rpAtomicResyncInterpolatedSphere(atomic);
    }

    atomic->object.object.privateFlags |= 1;

    return object;
}

static void* ClumpInitCameraExt(void* object, S32 offset, S32 size)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(object, _rpClumpCameraExtOffset);

    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return object;
}

static void* ClumpDeInitCameraExt(void* object, S32 offset, S32 size)
{
    return object;
}

static void* ClumpInitLightExt(void* object, S32 offset, S32 size)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(object, _rpClumpLightExtOffset);

    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return object;
}

static void* ClumpDeInitLightExt(void* object, S32 offset, S32 size)
{
    return object;
}

static RpClump* ClumpCallBack(RpClump* clump, void* data)
{
    return clump;
}

S32 _rpClumpRegisterExtensions(void)
{
    _rpClumpCameraExtOffset = RwCameraRegisterPlugin(sizeof(ClumpObjectExt), rwID_CLUMP,
                                                     ClumpInitCameraExt, ClumpDeInitCameraExt,
                                                     NULL);
    if (_rpClumpCameraExtOffset < 0)
    {
        return 0;
    }

    return (_rpClumpLightExtOffset =
                RpLightRegisterPlugin(sizeof(ClumpObjectExt), rwID_CLUMP, ClumpInitLightExt,
                                      ClumpDeInitLightExt, NULL)) >= 0;
}

RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* pData)
{
    RwLLLink* cur = clump->atomicList.link.next;
    RwLLLink* end = &clump->atomicList.link;

    while (cur != end)
    {
        RpAtomic* atomic = (RpAtomic*)((U8*)cur - (S32)&((RpAtomic*)0)->inClumpLink);
        RwLLLink* next = cur->next;

        if (!callback(atomic, pData))
        {
            return clump;
        }

        cur = next;
    }

    return clump;
}

RpClump* RpClumpForAllCameras(RpClump* clump, RwCameraCallBack callback, void* pData)
{
    RwLLLink* cur = clump->cameraList.link.next;
    RwLLLink* end = &clump->cameraList.link;

    while (cur != end)
    {
        RwCamera* camera = (RwCamera*)OBJECTFROMCLUMPLINK(cur, _rpClumpCameraExtOffset);
        RwLLLink* next = cur->next;

        if (!callback(camera, pData))
        {
            return clump;
        }

        cur = next;
    }

    return clump;
}

RpClump* RpClumpForAllLights(RpClump* clump, RpLightCallBack callback, void* pData)
{
    RwLLLink* cur = clump->lightList.link.next;
    RwLLLink* end = &clump->lightList.link;

    while (cur != end)
    {
        RpLight* light = (RpLight*)OBJECTFROMCLUMPLINK(cur, _rpClumpLightExtOffset);
        RwLLLink* next = cur->next;

        if (!callback(light, pData))
        {
            return clump;
        }

        cur = next;
    }

    return clump;
}

RpClump* RpClumpAddAtomic(RpClump* clump, RpAtomic* atomic)
{
    atomic->inClumpLink.next = clump->atomicList.link.next;
    atomic->inClumpLink.prev = &clump->atomicList.link;
    clump->atomicList.link.next->prev = &atomic->inClumpLink;
    clump->atomicList.link.next = &atomic->inClumpLink;
    atomic->clump = clump;

    return clump;
}

RpClump* RpClumpRemoveAtomic(RpClump* clump, RpAtomic* atomic)
{
    atomic->inClumpLink.prev->next = atomic->inClumpLink.next;
    atomic->inClumpLink.next->prev = atomic->inClumpLink.prev;
    atomic->clump = NULL;

    return clump;
}

RpClump* RpClumpRemoveLight(RpClump* clump, RpLight* light)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(light, _rpClumpLightExtOffset);

    ext->inClumpLink.prev->next = ext->inClumpLink.next;
    ext->inClumpLink.next->prev = ext->inClumpLink.prev;
    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return clump;
}

RpClump* RpClumpRemoveCamera(RpClump* clump, RwCamera* camera)
{
    ClumpObjectExt* ext = CLUMPEXTFROMOBJECT(camera, _rpClumpCameraExtOffset);

    ext->inClumpLink.prev->next = ext->inClumpLink.next;
    ext->inClumpLink.next->prev = ext->inClumpLink.prev;
    ext->inClumpLink.prev = NULL;
    ext->inClumpLink.next = NULL;
    ext->clump = NULL;

    return clump;
}

S32 RpAtomicRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&atomicTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpClumpRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&clumpTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpAtomicRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                 RwPluginDataChunkWriteCallBack writeCB,
                                 RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&atomicTKList, pluginID, readCB, writeCB, getSizeCB);
}

S32 RpAtomicSetStreamAlwaysCallBack(U32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    return _rwPluginRegistryAddPlgnStrmlwysCB(&atomicTKList, pluginID, alwaysCB);
}

S32 RpAtomicSetStreamRightsCallBack(U32 pluginID, RwPluginDataChunkRightsCallBack rightsCB)
{
    return _rwPluginRegistryAddPlgnStrmRightsCB(&atomicTKList, pluginID, rightsCB);
}

S32 RpAtomicGetPluginOffset(U32 pluginID)
{
    return _rwPluginRegistryGetPluginOffset(&atomicTKList, pluginID);
}

RpAtomic* RpAtomicSetFrame(RpAtomic* atomic, RwFrame* frame)
{
    _rwObjectHasFrameSetFrame(atomic, frame);
    atomic->object.object.privateFlags |= 1;

    return atomic;
}
