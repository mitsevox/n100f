#include <rpworld.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXLighting.h>
#include <dolphin/gx/GXStruct.h>

// GameCube lights: turns RenderWare lights into the eight GX hardware lights. Directional lights
// become very distant point lights, ambient lights are summed for the caller, and point and spot
// lights are loaded with their position, falloff and cone.

// The camera's view matrix, kept by the device driver.
extern RwMatrix lbl_8025E304;

// The eight hardware light objects.
static GXLightObj _RwGCLightObjs[8];

// The GX light id for each hardware light slot. Name written for this project; its scope (global
// or file-local) is not established.
GXLightID GCLightIDTable[8] = {
    GX_LIGHT0, GX_LIGHT1, GX_LIGHT2, GX_LIGHT3, GX_LIGHT4, GX_LIGHT5, GX_LIGHT6, GX_LIGHT7,
};

// How far away a directional light is placed, in view space. Written for this project.
#define GCLIGHTS_DIRECTIONALDIST -1048576.0f

void _rwGCLightsGlobalEnable(U8 flags, GCNObjectData* data)
{
    RwLLLink* cur = rwLinkListGetFirstLLLink(&((RpWorld*)RWSRCGLOBAL(curWorld))->directionalLightList);
    RwLLLink* end = rwLinkListGetTerminator(&((RpWorld*)RWSRCGLOBAL(curWorld))->directionalLightList);

    while (cur != end)
    {
        RpLight* light = rwLLLinkGetData(cur, RpLight, inWorld);

        if (light && (rwObjectTestFlags(light, flags)))
        {
            if (RpLightGetType(light) == rpLIGHTDIRECTIONAL)
            {
                S32 index = data->numLights;
                RwV3d dir;
                GXLightObj* obj;
                GXColor color;

                RwV3dTransformVectors(&dir, &RwFrameGetLTM(RpLightGetFrame(light))->at, 1,
                                      &lbl_8025E304);

                obj = &_RwGCLightObjs[index];
                GXInitLightPos(obj, GCLIGHTS_DIRECTIONALDIST * -dir.x,
                               GCLIGHTS_DIRECTIONALDIST * dir.y,
                               GCLIGHTS_DIRECTIONALDIST * -dir.z);

                color.r = (U8)(255.0f * light->color.red);
                color.g = (U8)(255.0f * light->color.green);
                color.b = (U8)(255.0f * light->color.blue);
                color.a = 0;
                GXInitLightColor(obj, color);
                GXLoadLightObjImm(obj, GCLightIDTable[index]);

                data->globalMask |= GCLightIDTable[data->numLights];
                data->numLights++;
            }
            else
            {
                data->ambient.red += light->color.red;
                data->ambient.green += light->color.green;
                data->ambient.blue += light->color.blue;
                data->ambientUsed = TRUE;
            }
        }

        cur = rwLLLinkGetNext(cur);
    }
}

void _rwGCLightsLocalEnable(RpLight* light, GCNObjectData* data)
{
    RwMatrix* ltm;
    RwV3d dir;
    RwV3d pos;
    GXColor color;

    color.r = (U8)(255.0f * light->color.red);
    color.g = (U8)(255.0f * light->color.green);
    color.b = (U8)(255.0f * light->color.blue);
    color.a = 0;
    GXInitLightColor(&_RwGCLightObjs[data->numLights], color);

    ltm = RwFrameGetLTM(RpLightGetFrame(light));
    RwV3dTransformPoints(&pos, &ltm->pos, 1, &lbl_8025E304);
    GXInitLightPos(&_RwGCLightObjs[data->numLights], -pos.x, pos.y, -pos.z);
    GXInitLightDistAttn(&_RwGCLightObjs[data->numLights], 0.5f * light->radius, 0.5f,
                        GX_DA_MEDIUM);

    switch (RpLightGetType(light))
    {
    case rpLIGHTPOINT:
        GXInitLightAttnA(&_RwGCLightObjs[data->numLights], 1.0f, 0.0f, 0.0f);
        break;
    case rpLIGHTSPOT:
        RwV3dTransformVectors(&dir, &ltm->at, 1, &lbl_8025E304);
        GXInitLightDir(&_RwGCLightObjs[data->numLights], -dir.x, dir.y, -dir.z);
        GXInitLightSpot(&_RwGCLightObjs[data->numLights], 57.29578f * RpLightGetConeAngle(light),
                        GX_SP_FLAT);
        break;
    case rpLIGHTSPOTSOFT:
        RwV3dTransformVectors(&dir, &ltm->at, 1, &lbl_8025E304);
        GXInitLightDir(&_RwGCLightObjs[data->numLights], -dir.x, dir.y, -dir.z);
        GXInitLightSpot(&_RwGCLightObjs[data->numLights], 57.29578f * RpLightGetConeAngle(light),
                        GX_SP_COS);
        break;
    case rpLIGHTDIRECTIONAL:
    case rpLIGHTAMBIENT:
        break;
    }

    GXLoadLightObjImm(&_RwGCLightObjs[data->numLights], GCLightIDTable[data->numLights]);
    data->localMask |= GCLightIDTable[data->numLights];
    data->numLights++;
}
