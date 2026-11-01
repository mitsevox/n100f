#include <rpworld.h>
#include <math.h>
#include <string.h>

// The first version whose light chunk holds the cone's minus cosine; older chunks hold its tangent.
#define LIGHT_MINUSCOSVERSION 0x30300

// A light's stream chunk.
typedef struct RpLightChunkInfo RpLightChunkInfo;
struct RpLightChunkInfo
{
    F32 radius;
    F32 red;
    F32 green;
    F32 blue;
    F32 minusCosAngle;
    U32 typeAndFlags;
};

typedef struct LightModuleData LightModuleData;
struct LightModuleData
{
    RwFreeList* freeList;
};

#define LIGHT_DATA(var) \
    (((LightModuleData*)((U8*)RwEngineInstance + lightModule.globalsOffset))->var)

// The rational approximation fdlibm's acos uses on z = x * x.
#define ACOSF_RATIO(z)                                                                             \
    (((z) * (_RW_pS0 +                                                                             \
             (z) * (_RW_pS1 + (z) * (_RW_pS2 + (z) * (_RW_pS3 + (z) * (_RW_pS4 + (z) * _RW_pS5)))))) / \
     (_RW_one + (z) * (_RW_qS1 + (z) * (_RW_qS2 + (z) * (_RW_qS3 + (z) * _RW_qS4)))))

// A single-precision acos in the manner of fdlibm, without the special cases for NaN.
#define ACOSF(_result, _x)                                                                         \
    do                                                                                             \
    {                                                                                              \
        RwSplitBits xBits;                                                                         \
        S32 hx;                                                                                    \
        S32 ix;                                                                                    \
                                                                                                   \
        xBits.nReal = (_x);                                                                        \
        hx = xBits.nInt;                                                                           \
        ix = hx & 0x7FFFFFFF;                                                                      \
                                                                                                   \
        if (ix >= 0x3F800000)                                                                      \
        {                                                                                          \
            /* |x| >= 1 */                                                                         \
            if (hx > 0)                                                                            \
            {                                                                                      \
                (_result) = 0.0f;                                                                  \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                (_result) = _RW_pi + 2.0f * _RW_pio2_lo;                                           \
            }                                                                                      \
        }                                                                                          \
        else if (ix < 0x3F000000)                                                                  \
        {                                                                                          \
            /* |x| < 0.5 */                                                                        \
            if (ix <= 0x23000000)                                                                  \
            {                                                                                      \
                (_result) = _RW_pio2_hi + _RW_pio2_lo;                                             \
            }                                                                                      \
            else                                                                                   \
            {                                                                                      \
                F32 z = (_x) * (_x);                                                               \
                                                                                                   \
                (_result) = _RW_pio2_hi - ((_x) - (_RW_pio2_lo - (_x) * ACOSF_RATIO(z)));          \
            }                                                                                      \
        }                                                                                          \
        else if (hx < 0)                                                                           \
        {                                                                                          \
            /* x < -0.5 */                                                                         \
            F32 z = 0.5f * (_RW_one + (_x));                                                       \
            F32 s = _rwSqrt(z);                                                                    \
                                                                                                   \
            F32 r = ACOSF_RATIO(z);                                                                \
            F32 w = r * s - _RW_pio2_lo;                                                           \
            (_result) = _RW_pi - 2.0f * (s + w);                                                   \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            /* x > 0.5 */                                                                          \
            F32 z = 0.5f * (_RW_one - (_x));                                                       \
            F32 s = _rwSqrt(z);                                                                    \
            RwSplitBits sBits;                                                                     \
            RwSplitBits dfBits;                                                                    \
            F32 df;                                                                                \
            F32 c;                                                                                 \
            F32 r;                                                                                 \
            F32 w;                                                                                 \
                                                                                                   \
            sBits.nReal = s;                                                                       \
            dfBits.nInt = sBits.nInt & 0xFFFFF000;                                                 \
            df = dfBits.nReal;                                                                     \
            c = (z - df * df) / (s + df);                                                          \
                                                                                                   \
            r = ACOSF_RATIO(z);                                                                    \
            w = r * s + c;                                                                         \
            (_result) = 2.0f * (df + w);                                                           \
        }                                                                                          \
    } while (0)

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

// The spot cone's half angle, recovered from the stored minus cosine.
F32 RpLightGetConeAngle(const RpLight* light)
{
    F32 cosAngle = -light->minusCosAngle;
    F32 angle;

    ACOSF(angle, cosAngle);

    return angle;
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

RpLight* RpLightStreamRead(RwStream* stream)
{
    U32 size;
    U32 version;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        return NULL;
    }

    if (version >= rwLIBRARYBASEVERSION && version <= rwLIBRARYCURRENTVERSION)
    {
        RpLightChunkInfo info;
        RpLight* light;

        memset(&info, 0, sizeof(info));

        if (RwStreamRead(stream, &info, size) != size)
        {
            return NULL;
        }

        RwMemNative32(&info, sizeof(info));

        light = RpLightCreate((info.typeAndFlags >> 16) & 0xFF);
        if (!light)
        {
            return NULL;
        }

        RwMemFloat32ToReal(&info.radius, sizeof(info.radius));
        RwMemFloat32ToReal(&info.red, sizeof(info.red));
        RwMemFloat32ToReal(&info.green, sizeof(info.green));
        RwMemFloat32ToReal(&info.blue, sizeof(info.blue));
        RwMemFloat32ToReal(&info.minusCosAngle, sizeof(info.minusCosAngle));

        light->radius = info.radius;
        light->color.red = info.red;
        light->color.green = info.green;
        light->color.blue = info.blue;

        if (version < LIGHT_MINUSCOSVERSION)
        {
            light->minusCosAngle =
                -_rwSqrt(1.0f / (1.0f + info.minusCosAngle * info.minusCosAngle));
        }
        else
        {
            light->minusCosAngle = info.minusCosAngle;
        }

        if (light->color.red == light->color.green && light->color.red == light->color.blue)
        {
            light->object.object.privateFlags = LIGHT_PRIVATENOCHROMA;
        }
        else
        {
            light->object.object.privateFlags = 0;
        }

        light->object.object.flags = info.typeAndFlags;

        if (!_rwPluginRegistryReadDataChunks(&lightTKList, stream, light))
        {
            return NULL;
        }

        return light;
    }
    else
    {
        RwError err;
        err.pluginID = 2;
        err.errorCode = _rwerror(0x80000004);
        RwErrorSet(&err);
        return NULL;
    }
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

// Creates the free list the lights come from, sized from the plugin registry's light size.
void* _rpLightOpen(void* instance, S32 offset, S32 size)
{
    lightModule.globalsOffset = offset;

    LIGHT_DATA(freeList) = RwFreeListCreate(lightTKList.sizeOfStruct, 20, 0);
    if (LIGHT_DATA(freeList))
    {
        lightModule.numInstances++;
        return instance;
    }

    return NULL;
}
