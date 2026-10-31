#include "zMenu.h"
#include "iDebug.h"
#include "iSystem.h"
#include "zEnv.h"
#include "zGameState.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zMusic.h"
#include "zHud.h"
#include "iColor.h"
#include "iScrFX.h"
#include "iMath.h"
#include "iTime.h"
#include "xEnt.h"
#include "xPad.h"
#include "xScrFx.h"
#include "xString.h"
#include <dolphin/os.h>
#include <dolphin/vi.h>

U32 zFMVPlay(char* filename, U32 buttons, F32 time);
char* zFMVFileGetName(S32 fileEnum);
void setupPlayer();
void zEntEvent(xBase* to, U32 toEvent);
void zEntEventAll(xBase* from, U32 fromEvent, U32 toEvent, F32* toParam);
void xFontRender();
U32 fn_800443F8();
extern "C" void fn_80016600();
extern "C" void fn_80021E50();
extern "C" void fn_80021B20();
extern "C" void fn_80021E30();
extern "C" void fn_8004436C();
void xSndUpdate(F32 dt);
void xSndStopAll();
void xCameraBegin(_tagxCamFrame* cam, S32 clear);
void xCameraEnd(_tagxCamFrame* cam, F32 seconds, S32 update_scrn_fx);
U32 zSaveLoad_LoadLoop();
extern "C" U32 lbl_8025E968;
extern "C" U32 lbl_8025EF10;
extern "C" U32 lbl_8025E9F8;
extern "C" _zEnv* gCurEnv;

static S32 sFirstBoot = 1;
static F32 time_elapsed = 0.01f;
static F32 holdTmr = 10.0f;
static F32 time_last;
static F32 time_current;
static F32 sAttractMode_timer;
static S32 card;
static S32 sInMenu;

S32 zMenuIsFirstBoot()
{
    return sFirstBoot;
}

void zMenuFMVPlay(char* filename, U32 buttons, F32 time)
{
    zFMVPlay(filename, buttons, time);
    time_last = iTimeGet();
    time_current = iTimeGet();
    sAttractMode_timer = 30.0f;
}

U32 zMenuGetBadCard()
{
    return card + 1;
}

U32 zMenuLoop()
{
    U32 s;

    if (sFirstBoot)
    {
        zGameModeSwitch(eGameMode_Intro);
        zGameStateSwitch(0);
        zMusicNotify(0);
    }
    else
    {
        zGameModeSwitch(eGameMode_Title);
        zGameStateSwitch(0);
        zMusicNotify(0);
    }

    time_last = 1.0f / (F32)(OS_BUS_CLOCK >> 2) * iTimeGet() - 1.0f / 60.0f;

    iColor_tag black = { 0, 0, 0, 255 };
    iColor_tag clear = { 0 };
    xScrFxFade(&black, &clear, 0.0f, NULL, 1, 0);

    S32 ostrich_delay = 10;

    do
    {
        if (zGameModeGet() == eGameMode_Intro)
        {
            switch (zGameStateGet())
            {
            case 0:
                iScrFxSetBlackoutFrames(1);
                zGameStateSwitch(3);
                xScrFxFade(&clear, &black, 0.0f, NULL, 1, 0);
                break;
            case 3:
                zFMVPlay(zFMVFileGetName(0x12), 0, 0.0f);
                zFMVPlay(zFMVFileGetName(0x11), 0, 0.0f);
                zFMVPlay(zFMVFileGetName(0), 0, 0.0f);
                zMenuFMVPlay(zFMVFileGetName(0x1A), 0, 0.0f);
                holdTmr = 10.0f;
                zGameModeSwitch(eGameMode_Title);
                zGameStateSwitch(1);
                xScrFxFade(&black, &clear, 0.1f, NULL, 1, 0);
                break;
            }
        }

        time_current = 1.0f / (F32)(OS_BUS_CLOCK >> 2) * iTimeGet();
        time_elapsed = time_current - time_last;
        if (time_elapsed < 0.0f)
        {
            time_elapsed = 1.0f / 60.0f;
        }

        if (zGameModeGet() == eGameMode_Title)
        {
            if (!fn_800443F8())
            {
                sAttractMode_timer -= time_elapsed;
            }

            if (sAttractMode_timer < 0.0f)
            {
                if (fn_800443F8())
                {
                    sAttractMode_timer = 30.0f;
                }
                else
                {
                    xBase* title1 = zSceneFindObject(xStrHash("MNU3 PRESS START BUTTON MGR"));
                    xBase* title2 = zSceneFindObject(xStrHash("MNU3 START NEWGAME"));
                    if ((title1->baseFlags & 1) || xEntIsVisible((_xEnt*)title2))
                    {
                        zGameStateSwitch(1);
                    }
                }
            }

            if (zGameStateGet() == 1)
            {
                if (sFirstBoot)
                {
                    zMenuFMVPlay(zFMVFileGetName(1), 0x10000, 0.1f);
                    sFirstBoot = 0;
                }
                else
                {
                    zMenuFMVPlay(zFMVFileGetName(irand() % 17), -1, 0.1f);
                }
                zGameStateSwitch(0);
                sAttractMode_timer = 30.0f;
            }
            else
            {
                zMusicUpdate(time_elapsed);
            }

            if (globals.pad0->pressed)
            {
                sAttractMode_timer = 30.0f;
            }
        }

        time_last = time_current;
        xPadUpdate(0, time_elapsed);
        if (globals.pad1)
        {
            xPadUpdate(1, time_elapsed);
        }

        fn_80016600();
        fn_80021E50();
        zSceneUpdate(time_elapsed);
        globals.player.ent.update(&globals.player.ent, globals.sceneCur, time_elapsed);
        xSndUpdate(time_elapsed);
        xCameraUpdate(&globals.camera, time_elapsed);
        xCameraBegin(&globals.camera, 1);
        zSceneRender();
        xFontRender();
        fn_80021B20();
        fn_8004436C();
        fn_80021E30();

        if (gScreenDump)
        {
            iDebugScreenDump();
        }

        xCameraEnd(&globals.camera, time_elapsed, 1);

        while ((lbl_8025E968 = VIGetRetraceCount()) - lbl_8025EF10 < 2)
        {
            iVSync();
        }
        lbl_8025EF10 = lbl_8025E968;

        xCameraShowRaster(&globals.camera);

        s = 0;
        if (gGameMode == eGameMode_Load)
        {
            F32 elapsed = (F32)iTimeGet() - time_last;
            s = zSaveLoad_LoadLoop();
            if (s == '0000')
            {
                s = 0;
            }
            time_last = elapsed + (F32)iTimeGet();
        }

        lbl_8025E9F8++;

        if (ostrich_delay > 0)
        {
            ostrich_delay--;
        }
        else
        {
            zGameSetOstrich(eGameOstrich_InScene);
        }
    } while (gGameMode != eGameMode_Game);

    return s;
}

void zMenuSetup()
{
    zSceneSetup();
    zHud_Setup(globals.sceneCur);
    sAttractMode_timer = 30.0f;
    setupPlayer();
    zEnvStartingCamera(gCurEnv);
    xCameraUpdate(&globals.camera, -1.0f);
    zEntEventAll(NULL, 0, 0x57, NULL);
    zEntEventAll(NULL, 0, 0x59, NULL);
    zEntEvent(&globals.player.ent, 8);
}

void zMenuExit()
{
    zMusicKill();
    xCameraRemoveEnv(&globals.camera);
    xCameraDestroy(&globals.camera);
    zSceneExit();
    xSndStopAll();
    sInMenu = 0;
}

void zMenuInit(U32 theSceneID)
{
    sInMenu = 1;
    isrand(iTimeGet());
    iTimeGet();
    irand();
    zSceneInit(theSceneID);
    zHud_Init();
    xCameraCreate(&globals.camera, 640, 480);
    xCameraAssignEnv(&globals.camera, globals.sceneCur->env);
    zMusicInit();
}

S32 zMenuRunning()
{
    return sInMenu;
}
