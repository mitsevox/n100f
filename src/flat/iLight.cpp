#include "iLight.h"

RpWorld* gLightWorld;

void iLightInit(RpWorld* world)
{
    gLightWorld = world;
}

iLight* iLightCreate(iLight* light, U32 type)
{
    RwFrame* frame;

    switch (type)
    {
    case ILIGHT_TYPE_POINT:
        light->hw = RpLightCreate(rpLIGHTPOINT);
        break;
    case ILIGHT_TYPE_SPOT:
        light->hw = RpLightCreate(rpLIGHTSPOT);
        break;
    case ILIGHT_TYPE_SPOTSOFT:
        light->hw = RpLightCreate(rpLIGHTSPOTSOFT);
        break;
    }

    frame = RwFrameCreate();

    RpLightSetFlags(light->hw, rpLIGHTLIGHTATOMICS);
    RpLightSetFrame(light->hw, frame);

    RwFrameUpdateObjects(frame);

    light->type = type;
    light->sph.center.x = 0.0f;
    light->sph.center.y = 0.0f;
    light->sph.center.z = 0.0f;
    light->sph.r = 0.0f;
    light->color.r = 1.0f;
    light->color.g = 1.0f;
    light->color.b = 1.0f;
    light->color.a = 1.0f;
    light->dir.x = 0.0f;
    light->dir.y = 0.0f;
    light->dir.z = 1.0f;
    light->coneangle = 0.0f;

    return light;
}

void iLightModify(iLight* light, U32 flags)
{
    if (flags & 0x5)
    {
        RwFrame* frame = RpLightGetFrame(light->hw);
        RwMatrix temp;

        *(xVec3*)&temp.right = g_O3;
        *(xVec3*)&temp.up = g_O3;
        *(xVec3*)&temp.at = light->dir;
        *(xVec3*)&temp.pos = light->sph.center;

        RwFrameTransform(frame, &temp, rwCOMBINEREPLACE);
    }

    if (flags & 0x2)
    {
        RpLightSetRadius(light->hw, light->sph.r);
    }

    if (flags & 0x8)
    {
        RpLightSetColor(light->hw, (RwRGBAReal*)&light->color);
    }

    if (flags & 0x10)
    {
        if (light->type == ILIGHT_TYPE_SPOT || light->type == ILIGHT_TYPE_SPOTSOFT)
        {
            RpLightSetConeAngle(light->hw, light->coneangle);
        }
    }
}

void iLightSetColor(iLight* light, _xFColor* col)
{
    RpLightSetColor(light->hw, (RwRGBAReal*)col);
}

void iLightSetPos(iLight* light, xVec3* pos)
{
    RwFrame* f = RpLightGetFrame(light->hw);
    RwMatrix* m = RwFrameGetMatrix(f);

    m->pos.x = pos->x;
    m->pos.y = pos->y;
    m->pos.z = pos->z;

    RwMatrixUpdate(m);

    RwFrameUpdateObjects(f);
}

void iLightDestroy(iLight* light)
{
    RwFrame* frame;

    light->type = ILIGHT_TYPE_NONE;

    frame = RpLightGetFrame(light->hw);

    if (frame)
    {
        RwFrameDestroy(frame);
    }

    RpLightDestroy(light->hw);
}

void iLightAdd(iLight* light)
{
    RpWorldAddLight(gLightWorld, light->hw);
}

void iLightRemove(iLight* light)
{
    RpWorldRemoveLight(gLightWorld, light->hw);
}

void iLightEnv(iLight* light, S32 env)
{
    U32 flags = 0;

    switch (env)
    {
    case ILIGHT_ENV_NONE:
        flags = 0;
        break;
    case ILIGHT_ENV_ATOMIC:
        flags = rpLIGHTLIGHTWORLD;
        break;
    case ILIGHT_ENV_WORLD:
        flags = rpLIGHTLIGHTATOMICS;
        break;
    case ILIGHT_ENV_ATOMICWORLD:
        flags = rpLIGHTLIGHTATOMICS | rpLIGHTLIGHTWORLD;
        break;
    }

    RpLightSetFlags(light->hw, flags);
}
