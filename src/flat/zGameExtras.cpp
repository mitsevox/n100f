#include "zGameExtras.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zEnv.h"
#include "zFog.h"
#include "zGame.h"
#include "zParEmitter.h"
#include "zRumble.h"
#include "xClimate.h"
#include "xSnd.h"
#include "xString.h"
#include "iCamera.h"
#include "iMath.h"
#include "iSnd.h"
#include "iSystem.h"

#include <stdio.h>
#include <string.h>

struct _tagFirework
{
    S32 state;
    F32 timer;
    xVec3 vel;
    xVec3 pos;
    F32 fuel;
};

static S32 sGameExtrasOn;
static S32 sDay;
static S32 sMonth;
static S32 sFireworksOn;
static S32 sColoredFountain;
static _zParEmitter* sFireworkTrailEmit;
static _zParEmitter* sFirework1Emit;
static _zParEmitter* sFirework2Emit;
static _zFog* sFog;
static F32 sFogIntensity;
static F32 sFogPrevIntensity = 1.0f;
static S32 sThunderOn;
static U32 sThunderFar1;
static U32 sThunderFar2;
static U32 sThunderNear1;
static U32 sThunderNear2;
static U32 sThunderNear3;
static U32 sThunderNear4;
static _tagFirework sFirework[10];

static inline F32 ClampColor(F32 c)
{
    if (c > 255.0f)
    {
        c = 255.0f;
    }
    else if (c < 0.0f)
    {
        c = 0.0f;
    }
    return c;
}

static void InitThunder();
static void InitFireworks();
static void InitHalloween();
static void InitSnow();
static void InitStPatricksDay();
static void Init4thJuly();
static void UpdateColoredFountain(F32 dt);
static void UpdateThunder(F32 dt);
static void UpdateFireworks(F32 dt);

void zGameExtrasInit()
{
    sFireworkTrailEmit = NULL;
    sFirework1Emit = NULL;
    sFirework2Emit = NULL;
    sFireworksOn = 0;
    sColoredFountain = 0;
    sGameExtrasOn = 0;
    sDay = 0;
    sMonth = 0;
    sFog = NULL;
    sFogIntensity = 0.0f;
    sFogPrevIntensity = 1.0f;
    sThunderOn = 0;
    InitThunder();

    sFog = (_zFog*)zSceneGetObject(0x24, 0);
    sThunderOn = sFog ? 1 : 0;
    if (!sGameExtrasOn)
    {
        sGameExtrasOn = sThunderOn;
    }

    sDay = iGetDay();
    sMonth = iGetMonth();

    if (globals.sceneCur->sceneID != zSceneIDFromName("H001") &&
        globals.sceneCur->sceneID != zSceneIDFromName("H002"))
    {
        return;
    }

    switch (sMonth)
    {
    case 1:
        if (sDay == 1)
        {
            InitFireworks();
        }
        break;
    case 2:
        if (sDay == 2)
        {
            InitFireworks();
        }
        break;
    case 3:
        if (sDay == 17)
        {
            InitStPatricksDay();
        }
        if (sDay == 22)
        {
            InitFireworks();
        }
        break;
    case 5:
        if (sDay == 30)
        {
            InitFireworks();
        }
        break;
    case 7:
        if (sDay == 4)
        {
            Init4thJuly();
            InitFireworks();
        }
        break;
    case 8:
        if (sDay == 28)
        {
            InitFireworks();
            sColoredFountain = 1;
            sGameExtrasOn = 1;
        }
        break;
    case 9:
        if (sDay == 8)
        {
            InitFireworks();
        }
        break;
    case 10:
        if (sDay == 31)
        {
            InitHalloween();
        }
        break;
    case 11:
        if (sDay == 5)
        {
            InitFireworks();
        }
        break;
    case 12:
        if (sDay == 31)
        {
            InitFireworks();
        }
        else if (sDay == 20)
        {
            InitFireworks();
        }
        else
        {
            InitSnow();
        }
        break;
    }
}

void zGameExtras(F32 dt)
{
    if (!sGameExtrasOn)
    {
        return;
    }
    if (globals.cmgr)
    {
        return;
    }
    if (zGameIsPaused())
    {
        return;
    }

    UpdateColoredFountain(dt);
    UpdateFireworks(dt);
    UpdateThunder(dt);
}

void zGameExtrasReset()
{
    sGameExtrasOn = 0;
    sDay = 0;
    sMonth = 0;
    sFireworksOn = 0;
    sColoredFountain = 0;
}

static void InitStPatricksDay()
{
    _zParEmitter* fe;
    zParEmitterAsset* a;
    S32 i;
    char emitter_txt[64];

    fe = zParEmitterFind("FOUNTAIN EMITTER");
    if (!fe)
    {
        return;
    }

    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 9;
    a->color_birth[1] = a->color_death[1] = 110;
    a->color_birth[2] = a->color_death[2] = 21;

    fe = zParEmitterFind("FOUNTAIN EMITTER TOP");
    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 9;
    a->color_birth[1] = a->color_death[1] = 110;
    a->color_birth[2] = a->color_death[2] = 21;

    fe = zParEmitterFind("FOUNTAIN EMITTER MID");
    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 9;
    a->color_birth[1] = a->color_death[1] = 110;
    a->color_birth[2] = a->color_death[2] = 21;

    fe = zParEmitterFind("FOUNTAIN DOG ON TOP");
    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 9;
    a->color_birth[1] = a->color_death[1] = 110;
    a->color_birth[2] = a->color_death[2] = 21;

    for (i = 1; i <= 7; i++)
    {
        sprintf(emitter_txt, "CHIMNEY EMITTER %d", i);
        fe = zParEmitterFind(emitter_txt);
        if (fe)
        {
            a = fe->tasset;
            if (a)
            {
                a->color_birth[0] = a->color_death[0] = 9;
                a->color_birth[1] = a->color_death[1] = 110;
                a->color_birth[2] = a->color_death[2] = 21;
            }
        }
    }
}

static void Init4thJuly()
{
    _zParEmitter* fe;
    zParEmitterAsset* a;
    S32 idx;
    S32 i;
    char emitter_txt[64];

    fe = zParEmitterFind("FOUNTAIN EMITTER");
    if (!fe)
    {
        return;
    }

    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 32;
    a->color_birth[1] = a->color_death[1] = 172;
    a->color_birth[2] = a->color_death[2] = 252;

    fe = zParEmitterFind("FOUNTAIN EMITTER MID");
    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 255;
    a->color_birth[1] = a->color_death[1] = 255;
    a->color_birth[2] = a->color_death[2] = 255;

    fe = zParEmitterFind("FOUNTAIN DOG ON TOP");
    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 212;
    a->color_birth[1] = a->color_death[1] = 0;
    a->color_birth[2] = a->color_death[2] = 25;

    fe = zParEmitterFind("FOUNTAIN EMITTER TOP");
    a = fe->tasset;
    a->color_birth[0] = a->color_death[0] = 212;
    a->color_birth[1] = a->color_death[1] = 0;
    a->color_birth[2] = a->color_death[2] = 25;

    idx = 0;
    for (i = 1; i <= 7; i++)
    {
        sprintf(emitter_txt, "CHIMNEY EMITTER %d", i);
        fe = zParEmitterFind(emitter_txt);
        if (fe)
        {
            a = fe->tasset;
            if (a)
            {
                if (idx == 0)
                {
                    a->color_birth[0] = a->color_death[0] = 255;
                    a->color_birth[1] = a->color_death[1] = 255;
                    a->color_birth[2] = a->color_death[2] = 255;
                }
                else if (idx == 1)
                {
                    a->color_birth[0] = a->color_death[0] = 212;
                    a->color_birth[1] = a->color_death[1] = 0;
                    a->color_birth[2] = a->color_death[2] = 25;
                }
                else
                {
                    a->color_birth[0] = a->color_death[0] = 32;
                    a->color_birth[1] = a->color_death[1] = 172;
                    a->color_birth[2] = a->color_death[2] = 252;
                }
            }
            idx++;
            if (idx > 2)
            {
                idx = 0;
            }
        }
    }
}

static void InitHalloween()
{
    char bat_txt[64];
    S32 i;
    _xEnt* bat;

    for (i = 0; i < 20; i++)
    {
        sprintf(bat_txt, "BAT HANG %d", i);
        bat = (_xEnt*)zSceneFindObject(xStrHash(bat_txt));
        if (bat)
        {
            xEntShow(bat);
            bat->baseFlags |= 0x8;
        }
    }
}

static void InitSnow()
{
    if (gCurEnv)
    {
        gCurEnv->easset->climateFlags = 2;
        gCurEnv->easset->climateStrengthMin = 0.45f;
        gCurEnv->easset->climateStrengthMax = 0.55f;
        if (sDay == 25)
        {
            gCurEnv->easset->climateStrengthMin = 0.75f;
            gCurEnv->easset->climateStrengthMax = 1.0f;
            xClimateInitAsset(&gClimate, gCurEnv->easset);
        }
        else if (sDay == 14)
        {
            gCurEnv->easset->climateStrengthMin = 0.99f;
            gCurEnv->easset->climateStrengthMax = 1.0f;
            xClimateInitAsset(&gClimate, gCurEnv->easset);
        }
    }
}

static void InitFireworks()
{
    S32 i;

    sThunderOn = 0;
    sFireworkTrailEmit = zParEmitterFind("PAREMIT_FIREWORKS_TRAIL");
    sFirework1Emit = zParEmitterFind("PAREMIT_FIREWORKS1");
    sFirework2Emit = zParEmitterFind("PAREMIT_FIREWORKS2");
    memset(sFirework, 0, sizeof(sFirework));
    for (i = 0; i < 10; i++)
    {
        sFirework[i].state = 0;
        sFirework[i].timer = 5.0f * iurand();
    }
    sFireworksOn = 1;
    sGameExtrasOn = 1;
    sFog = (_zFog*)zSceneFindObject(xStrHash("LIGHT_FOG"));
}

static void UpdateColoredFountain(F32 dt)
{
    static F32 ftimer = 0.0f;
    _zParEmitter* fe;
    zParEmitterAsset* a;

    if (!sColoredFountain)
    {
        return;
    }

    ftimer -= dt;
    if (ftimer <= 0.0f)
    {
        ftimer = 3.0f;

        fe = zParEmitterFind("FOUNTAIN EMITTER");
        if (!fe)
        {
            return;
        }

        a = fe->tasset;
        a->color_birth[0] = a->color_death[0] = 127.0f * iurand() + 128.0f;
        a->color_birth[1] = a->color_death[1] = 127.0f * iurand() + 128.0f;
        a->color_birth[2] = a->color_death[2] = 127.0f * iurand() + 128.0f;

        fe = zParEmitterFind("FOUNTAIN EMITTER TOP");
        a = fe->tasset;
        a->color_birth[0] = a->color_death[0] = 127.0f * iurand() + 128.0f;
        a->color_birth[1] = a->color_death[1] = 127.0f * iurand() + 128.0f;
        a->color_birth[2] = a->color_death[2] = 127.0f * iurand() + 128.0f;

        fe = zParEmitterFind("FOUNTAIN EMITTER MID");
        a = fe->tasset;
        a->color_birth[0] = a->color_death[0] = 127.0f * iurand() + 128.0f;
        a->color_birth[1] = a->color_death[1] = 127.0f * iurand() + 128.0f;
        a->color_birth[2] = a->color_death[2] = 127.0f * iurand() + 128.0f;

        fe = zParEmitterFind("FOUNTAIN DOG ON TOP");
        a = fe->tasset;
        a->color_birth[0] = a->color_death[0] = 127.0f * iurand() + 128.0f;
        a->color_birth[1] = a->color_death[1] = 127.0f * iurand() + 128.0f;
        a->color_birth[2] = a->color_death[2] = 127.0f * iurand() + 128.0f;
    }
}

static void InitThunder()
{
    sThunderFar1 = xStrHash("thunder_distant1");
    sThunderFar2 = xStrHash("thunder_distant4");
    sThunderNear1 = xStrHash("thunder1");
    sThunderNear2 = xStrHash("thunder2");
    sThunderNear3 = xStrHash("thunder3");
    sThunderNear4 = xStrHash("thunder4");
}

static void UpdateThunder(F32 dt)
{
    iFogParams fog;

    if (!sThunderOn)
    {
        return;
    }

    if (sFog && sFogIntensity != sFogPrevIntensity)
    {
        F32 intensity = sFogIntensity;

        if (1.0f != intensity && irand() % 10 == 8)
        {
            sFogIntensity += iurand();
        }

        fog.type = rwFOGTYPELINEAR;
        fog.start = sFog->tasset->fogStart;
        fog.stop = sFog->tasset->fogStop;
        fog.density = sFog->tasset->fogDensity;
        fog.fogcolor.red = ClampColor(sFog->tasset->fogColor[0] * intensity);
        fog.fogcolor.green = ClampColor(sFog->tasset->fogColor[1] * intensity);
        fog.fogcolor.blue = ClampColor(sFog->tasset->fogColor[2] * intensity);
        fog.fogcolor.alpha = ClampColor(sFog->tasset->fogColor[3] * intensity);
        fog.bgcolor.red = ClampColor(sFog->tasset->bkgndColor[0] * intensity);
        fog.bgcolor.green = ClampColor(sFog->tasset->bkgndColor[1] * intensity);
        fog.bgcolor.blue = ClampColor(sFog->tasset->bkgndColor[2] * intensity);
        fog.bgcolor.alpha = ClampColor(sFog->tasset->bkgndColor[3] * intensity);
        fog.table = NULL;
        iCameraSetFogParams(&fog, 0.0f);
        sFogPrevIntensity = sFogIntensity;
    }

    sFogIntensity *= 0.9f;
    if (sFogIntensity < 1.0f)
    {
        sFogIntensity = 1.0f;
    }

    if (iSndIsPlaying(sThunderFar1) || iSndIsPlaying(sThunderFar2))
    {
        sFogIntensity += 0.1f;
    }

    if (iSndIsPlaying(sThunderNear1) || iSndIsPlaying(sThunderNear2) ||
        iSndIsPlaying(sThunderNear3) || iSndIsPlaying(sThunderNear4))
    {
        sFogIntensity += 0.3f;
    }

    if (sFogIntensity > 8.0f)
    {
        sFogIntensity = 8.0f;
    }
}

static void UpdateFireworks(F32 dt)
{
    static S32 july_4_rgb = 0;
    static xVec3 h001_launch[5] = {
        { -29.14f, 2.30486f, 47.4601f },  { -27.533f, 2.30486f, 64.1456f },
        { -11.4041f, 2.30486f, 64.6424f }, { -40.0325f, 2.30486f, 46.0661f },
        { -40.0091f, 2.30486f, 48.8285f },
    };
    static xVec3 h002_launch[5] = {
        { -51.8615f, 2.0f, -7.29103f }, { -48.333f, 2.0f, -18.33f },
        { -51.533f, 2.0f, -5.72145f },  { -57.5795f, 2.0f, 9.16543f },
        { -53.623f, 2.0f, 21.6936f },
    };
    iFogParams fog;
    S32 i;
    _tagFirework* fw;
    _zParEmitterCustomSettings trail_info;
    _zParEmitter* femit;
    _zParEmitterCustomSettings xplo_info;

    if (!sFireworksOn)
    {
        return;
    }

    if (sFog && sFogIntensity != sFogPrevIntensity)
    {
        F32 t = (4.5f - sFogIntensity) / 4.5f;

        fog.type = rwFOGTYPELINEAR;
        fog.start = sFog->tasset->fogStart;
        fog.stop = sFog->tasset->fogStop - 0.4f * sFog->tasset->fogStop * t;
        fog.density = sFog->tasset->fogDensity;
        fog.fogcolor.red = ClampColor(sFog->tasset->fogColor[0] * sFogIntensity);
        fog.fogcolor.green = ClampColor(sFog->tasset->fogColor[1] * sFogIntensity);
        fog.fogcolor.blue = ClampColor(sFog->tasset->fogColor[2] * sFogIntensity);
        fog.fogcolor.alpha = ClampColor(sFog->tasset->fogColor[3] * sFogIntensity);
        fog.bgcolor.red = ClampColor(sFog->tasset->bkgndColor[0] * sFogIntensity);
        fog.bgcolor.green = ClampColor(sFog->tasset->bkgndColor[1] * sFogIntensity);
        fog.bgcolor.blue = ClampColor(sFog->tasset->bkgndColor[2] * sFogIntensity);
        fog.bgcolor.alpha = ClampColor(sFog->tasset->bkgndColor[3] * sFogIntensity);
        fog.table = NULL;
        iCameraSetFogParams(&fog, 0.0f);
        sFogPrevIntensity = sFogIntensity;
    }

    sFogIntensity *= 0.97f;
    if (sFogIntensity < 0.5f)
    {
        sFogIntensity = 0.5f;
    }

    for (i = 0; i < 10; i++)
    {
        fw = &sFirework[i];

        if (fw->state == 0)
        {
            fw->timer -= dt;
            if (fw->timer <= 0.0f)
            {
                S32 idx = 5.0f * iurand();
                xVec3* launch;

                if (idx < 0)
                {
                    idx = 0;
                }
                else if (idx >= 5)
                {
                    idx = 4;
                }

                if (globals.sceneCur->sceneID == zSceneIDFromName("H001"))
                {
                    launch = &h001_launch[idx];
                }
                else
                {
                    launch = &h002_launch[idx];
                }

                if (launch)
                {
                    fw->pos.x = launch->x;
                    fw->pos.y = launch->y;
                    fw->pos.z = launch->z;
                }
                else
                {
                    fw->pos.x = -29.14f;
                    fw->pos.y = 2.30486f;
                    fw->pos.z = 47.4601f;
                }

                fw->vel.x = 13.0f * iurand() + -6.5f;
                fw->vel.y = 0.0f;
                fw->vel.z = 13.0f * iurand() + -6.5f;
                fw->fuel = 0.5f * iurand() + 1.5f;
                fw->state = 1;
            }
        }
        else
        {
            fw->fuel -= dt;
            if (fw->fuel > 0.0f)
            {
                fw->vel.y += 15.0f * dt;
            }

            fw->pos.x += fw->vel.x * dt;
            fw->pos.y += fw->vel.y * dt;
            fw->pos.z += fw->vel.z * dt;

            trail_info.custom_flags = 0x20;
            trail_info.pos.x = fw->pos.x;
            trail_info.pos.y = fw->pos.y;
            trail_info.pos.z = fw->pos.z;
            zParEmitterEmitCustom(sFireworkTrailEmit, dt, &trail_info);

            if (fw->fuel <= 0.0f)
            {
                fw->state = 0;
                fw->timer = 5.0f * iurand() + 2.5f;

                femit = sFirework1Emit;
                if (iurand() < 0.75f)
                {
                    femit = sFirework2Emit;
                }

                xplo_info.custom_flags = 0x1A0;
                xplo_info.pos.x = fw->pos.x;
                xplo_info.pos.y = fw->pos.y;
                xplo_info.pos.z = fw->pos.z;

                if (sDay == 4 && sMonth == 7)
                {
                    if (july_4_rgb == 0)
                    {
                        xplo_info.color_birth[0] = xplo_info.color_death[0] = 212;
                        xplo_info.color_birth[1] = xplo_info.color_death[1] = 0;
                        xplo_info.color_birth[2] = xplo_info.color_death[2] = 25;
                    }
                    else if (july_4_rgb == 1)
                    {
                        xplo_info.color_birth[0] = xplo_info.color_death[0] = 255;
                        xplo_info.color_birth[1] = xplo_info.color_death[1] = 255;
                        xplo_info.color_birth[2] = xplo_info.color_death[2] = 255;
                    }
                    else
                    {
                        xplo_info.color_birth[0] = xplo_info.color_death[0] = 32;
                        xplo_info.color_birth[1] = xplo_info.color_death[1] = 172;
                        xplo_info.color_birth[2] = xplo_info.color_death[2] = 252;
                    }

                    july_4_rgb++;
                    if (july_4_rgb > 2)
                    {
                        july_4_rgb = 0;
                    }
                }
                else
                {
                    xplo_info.color_birth[0] = xplo_info.color_death[0] = (U8)(S32)(127.0f * iurand()) + 128;
                    xplo_info.color_birth[1] = xplo_info.color_death[1] = (U8)(S32)(127.0f * iurand()) + 128;
                    xplo_info.color_birth[2] = xplo_info.color_death[2] = (U8)(S32)(127.0f * iurand()) + 128;
                    fw->timer += 5.0f * iurand();
                }

                xplo_info.color_birth[3] = 255;
                xplo_info.color_death[3] = 0;
                zParEmitterEmitCustom(femit, dt, &xplo_info);

                F32 dx = xEntGetPos(&globals.player.ent)->x - fw->pos.x;
                F32 dz = xEntGetPos(&globals.player.ent)->z - fw->pos.z;
                zRumbleStartDistance(0, dx * dx + dz * dz, 2300.0f);

                fw->pos.y = xEntGetPos(&globals.player.ent)->y;

                if (irand() & 1)
                {
                    xSndPlay(xStrHash("cannon_fire1"), 0.5f, 0.5f, 0.0f, 0x80, 2, 0);
                }
                else
                {
                    xSndPlay(xStrHash("cannon_fire2"), 0.5f, 0.5f, 0.0f, 0x80, 2, 0);
                }

                sFogIntensity += fw->timer;
                if (sFogIntensity > 4.5f)
                {
                    sFogIntensity = 4.5f;
                }
            }
        }
    }
}

S32 zGameExtraIsValentines()
{
    if (sDay == 14 && sMonth == 2)
    {
        return 1;
    }
    return 0;
}

S32 zGameExtraIsShirazBday()
{
    if (sDay == 2 && sMonth == 2)
    {
        return 1;
    }
    return 0;
}

S32 zGameExtraIsAnnaBday()
{
    if (sDay == 9 && sMonth == 1)
    {
        return 1;
    }
    return 0;
}
