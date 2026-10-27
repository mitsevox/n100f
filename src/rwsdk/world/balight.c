#include <rpworld.h>
#include <math.h>

typedef struct LightModuleData LightModuleData;
struct LightModuleData
{
    RwFreeList* freeList;
};

#define LIGHT_DATA(var) \
    (((LightModuleData*)((U8*)RwEngineInstance + lightModule.globalsOffset))->var)

static RwPluginRegistry lightTKList = { sizeof(RpLight), sizeof(RpLight) };
static RwModuleInfo lightModule;

static void LightTidyDestroyLight(void* light, void* data)
{
    RpLightDestroy((RpLight*)light);
}

static RwObjectHasFrame* LightSync(RwObjectHasFrame* object)
{
    return object;
}

RpLight* RpLightSetRadius(RpLight* light, F32 radius)
{
    RwFrame* frame = (RwFrame*)light->object.object.parent;

    light->radius = radius;
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    return light;
}

RpLight* RpLightSetColor(RpLight* light, const RwRGBAReal* color)
{
    light->color = *color;

    if (light->color.red == light->color.green && light->color.red == light->color.blue)
    {
        light->object.object.privateFlags = 1;
    }
    else
    {
        light->object.object.privateFlags = 0;
    }

    return light;
}

RpLight* RpLightSetConeAngle(RpLight* light, F32 angle)
{
    if (angle < 0.0f || angle > 89.9999f)
    {
        return NULL;
    }

    light->minusCosAngle = -(F32)cos(angle);

    return light;
}

S32 RpLightRegisterPlugin(S32 size, U32 pluginID, RwPluginObjectConstructor constructCB,
                          RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB)
{
    return _rwPluginRegistryAddPlugin(&lightTKList, size, pluginID, constructCB, destructCB,
                                      copyCB);
}

S32 RpLightRegisterPluginStream(U32 pluginID, RwPluginDataChunkReadCallBack readCB,
                                RwPluginDataChunkWriteCallBack writeCB,
                                RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    return _rwPluginRegistryAddPluginStream(&lightTKList, pluginID, readCB, writeCB, getSizeCB);
}

S32 RpLightDestroy(RpLight* light)
{
    _rwPluginRegistryDeInitObject(&lightTKList, light);
    _rwObjectHasFrameReleaseFrame(light);
    RWSRCGLOBAL(memoryFree)(LIGHT_DATA(freeList), light);

    return 1;
}

RpLight* RpLightCreate(S32 type)
{
    RpLight* light = (RpLight*)RWSRCGLOBAL(memoryAlloc)(LIGHT_DATA(freeList));

    if (!light)
    {
        return NULL;
    }

    light->object.object.type = 3;
    light->object.object.subType = type;
    light->object.object.flags = 0;
    light->object.object.privateFlags = 0;
    light->object.object.parent = NULL;
    light->object.sync = LightSync;

    light->radius = 0.0f;
    light->minusCosAngle = 1.0f;
    light->color.red = 1.0f;
    light->color.green = 1.0f;
    light->color.blue = 1.0f;
    light->color.alpha = 1.0f;
    light->object.object.privateFlags = 1;

    light->WorldSectorsInLight.link.next = &light->WorldSectorsInLight.link;
    light->WorldSectorsInLight.link.prev = &light->WorldSectorsInLight.link;
    light->inWorld.prev = NULL;
    light->inWorld.next = NULL;
    light->lightFrame = RWSRCGLOBAL(lightFrame) - 1;
    light->object.object.flags = 3;

    _rwPluginRegistryInitObject(&lightTKList, light);

    return light;
}

void* _rpLightClose(void* instance, S32 offset, S32 size)
{
    RwFreeListForAllUsed(LIGHT_DATA(freeList), LightTidyDestroyLight, NULL);
    RwFreeListDestroy(LIGHT_DATA(freeList));
    LIGHT_DATA(freeList) = NULL;

    lightModule.numInstances--;

    return instance;
}
