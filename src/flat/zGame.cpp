#include "types.h"
#include "xDraw.h"
#include "iTime.h"
#include "iSystem.h"
#include "zEnv.h"
#include "zCutsceneMgr.h"
#include "iCamera.h"
#include "iScrFX.h"
#include "rpworld.h"
#include "zGame.h"
#include "zGameExtras.h"
#include "zGameState.h"
#include "zGlobals.h"
#include "zMenu.h"
#include "zMusic.h"
#include "xString.h"
#include "xstransvc.h"
#include "xDebug.h"
#include "xPad.h"
#include "zScene.h"
#include "iMath.h"
#include "zLaugh.h"
#include "iPad.h"
#include "xTRC.h"
#include "xEnv.h"
#include "xModel.h"
#include "zEntShaggy.h"
#include "zHud.h"
#include "zPortal.h"
#include "zVar.h"
#include "zEntPickup.h"
#include "xSnd.h"
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <stdio.h>
#include <string.h>

void zEntEvent(char*, U32);
void xPadDestroyRumbleChain(S32 idx);
void xSndStopAll();
void xSndUpdate(F32 time_elapsed);
void zEntPlayer_Init(_xEnt*, p2EntAsset*);
void zEntEventAllOfType(U32 toEvent, U32 type);
extern "C" void fn_80016600();
void xSndSetFrame(xVec3* right, xVec3* up, xVec3* at, xVec3* pos, F32 radius);
void xSndSetFrameFar(xVec3* pos);
void xCameraBegin(_tagxCamFrame* cam, S32 clear);
void zVolume_OccludePrecalc(xVec3* camPos);
void xCameraEnd(_tagxCamFrame* cam, F32 seconds, S32 update_scrn_fx);
U32 zSaveLoad_SaveLoop();
void xCameraPickInitial(_tagxCamFrame* cam, _tagxCam* c);
void zEntPlayer_StoreCheckPoint(F32*);
void zGameInit(U32);
void zGameSetup();
void zGameExit();
static void zGameUpdateMode();
void zFX_SceneEnter(RpWorld*);
void xCameraSetTarget(_tagxCamFrame*, xVec3*);
void xCameraSetTarget(_tagxCamFrame*, xMat4x3*);
void zEntEventAll(xBase*, U32, U32, F32*);
extern "C" RpWorld* RpWorldCreate(const RwBBox*);
extern "C" F32 RwIm2DGetNearScreenZ();
extern "C" void fn_8004436C();
void xFontRender();
extern "C" volatile U32 lbl_8025E968;
extern "C" U32 lbl_8025E9F8;
extern "C" S32 lbl_8025E528;
void xScrFxFade(iColor_tag*, iColor_tag*, F32, void (*)(), S32, S32);
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba);

extern "C" S32 fn_800AC0BC();
void zGameScreenTransitionEnd();
void zGameScreenTransition_UIRender(F32 x1, F32 y1, F32 x2, F32 y2, S32 tex);
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba);
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg);
void zGameScreenTransitionBegin();
void zGamePauseIfPossible();
S32 zGameIsPaused();
void zGameLoop();
static F32 zGameMaxElapsed();
static void zGameCheats(F32 dt);
static S32 TestCheat(U32* cheat_info);
void setupPlayer();
void zGameSetup();
void zGameExit();
void zGameInit(U32 theSceneID);
static void zGameUpdateMode();

static RwImVertexIndex Idx[6] = { 0, 1, 2, 0, 2, 3 };

// The cheat codes as pad bits 0x20000 and 0x40000, oldest press first; leading zeros pad the
// shorter codes. sCheatPressed holds the last fourteen presses. sCheatUnchecked (fourteen presses
// of 0x40000) is a code nothing tests.
U32 sCheatUnlockCredits[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0x40000, 0x20000, 0x20000, 0x40000, 0x20000, 0x40000 };

U32 sCheatUnlockAllMovies[14] = { 0,       0,       0,       0,       0,       0x40000, 0x40000,
                                  0x40000, 0x20000, 0x20000, 0x20000, 0x40000, 0x20000, 0x40000 };

U32 sCheatWarpGate[14] = { 0,       0,       0,       0,       0x40000, 0x40000, 0x20000,
                           0x40000, 0x40000, 0x20000, 0x40000, 0x20000, 0x20000, 0x20000 };

U32 sCheatAllPowerups[14] = { 0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000, 0x40000,
                              0x40000, 0x20000, 0x20000, 0x40000, 0x20000, 0x20000, 0x20000 };

U32 sCheatUnchecked[14] = { 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000,
                            0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000, 0x40000 };

U32 sCheatPressed[14] = {};

U32 startPressed = 0xFFFFFFFF;
iColor_tag black = { 0x00, 0x00, 0x00, 0xFF };
iColor_tag clear = { 0x00, 0x00, 0x00, 0x00 };

static RwIm2DVertex Vert[4];

static U32 sPlayerMarkerStartID;
static U32 sPlayerMarkerStartCamID;
static F32 sPlayerStartAngle;
static S32 sPortalling;
static F32 sGameOverTimer;
F32 sTimeElapsed;
static iTime sTimeLast;
static iTime sTimeCurrent;
iTime t0;
iTime t1;
U32 gLastVsyncFlip;
static RwCamera* sGameScreenTransCam;
static RpWorld* World;
static RpLight* DirectionalLight;
static U32 loadMeter;
static F32 sCheatTimer;

static S32 TestCheat(U32* cheat_info);

struct p2MarkerAsset
{
    xVec3 pos;
};

static void zGameUpdateMode();
void zGameInit(U32 theSceneID);
void zGameExit();
void zGameSetup();
void setupPlayer();
static S32 TestCheat(U32* cheat_info);
static void zGameCheats(F32 dt);
void zGameLoop();
S32 zGameIsPaused();
void zGamePauseIfPossible();
void zGameScreenTransitionBegin();
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg);
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba);
void zGameScreenTransition_UIRender(F32 x1, F32 y1, F32 x2, F32 y2, S32 tex);
void zGameScreenTransitionEnd();
extern "C" S32 fn_800AC0BC();

// Initialize the scene, camera, music and counters for a new game.
void zGameInit(U32 theSceneID)
{
    isrand((U32)iTimeGet());
    iTimeGet();
    irand();
    zSceneInit(theSceneID);
    xCameraCreate(&globals.camera, 640, 480);
    xCameraAssignEnv(&globals.camera, globals.sceneCur->env);
    zLaughInit();
    zMusicInit();
    globals.stats.gameTime = 0.0f;
    globals.stats.tokens = 0;
    globals.stats.tokens_tot = 0;
    globals.stats.snacks = 0;
    globals.stats.snacks_tot = 0;
    zHud_Init();
    zHud_Setup(globals.sceneCur);
}

// Tear down the current game and its scene services.
void zGameExit()
{
    zGameExtrasReset();
    xPadDestroyRumbleChain(0);
    zMusicKill();
    xSndStopAll();
    xSndUpdate(0.0f);
    xCameraRemoveEnv(&globals.camera);
    xCameraDestroy(&globals.camera);
    zSceneExit();
}

// Prepare the player and scene after loading, then start their game services.
void zGameSetup()
{
    zSceneSetup();
    RpWorld* world = globals.sceneCur->env->geom->world;
    xModel_SceneEnter(world);
    zFX_SceneEnter(world);
    setupPlayer();
    zEntShaggy_SceneStart(globals.sceneCur);
    zEnvStartingCamera(gCurEnv);
    xCameraUpdate(&globals.camera, -1.0f);
    zSceneLoad(globals.sceneCur, NULL);
    zMusicNotify(0);
    zEntEventAll(NULL, 0, 0x57, NULL);
    zEntEventAll(NULL, 0, 0x59, NULL);
    zGameExtrasInit();
}

// Whether the game loop runs another frame; the save mode runs the save screen first.
static S32 zGameLoopContinue()
{
    if (gGameMode == eGameMode_Game)
    {
        return lbl_8025E528 == 1 || lbl_8025E528 == 3 || lbl_8025E528 == 4;
    }
    else
    {
        if (gGameMode == eGameMode_Save)
        {
            U32 saved = zSaveLoad_SaveLoop();
            sTimeLast = iTimeGet();
            t1 = iTimeGet();
            if (saved)
            {
                zHudShowSaveSuccess();
            }
        }
    }
    return 1;
}

// Advance input, simulation, rendering and game mode until gameplay exits.
void zGameLoop()
{
    S32 ostrich_delay = 10;
    zGameStateSwitch(1);
    sTimeLast = iTimeGet() - (U64)((F32)OS_TIMER_CLOCK / 60.0f);
    if (globals.cmgr)
        zCutsceneMgrFinishLoad(globals.cmgr);
    iScrFxSetBlackoutFrames(0);
    do
    {
        sTimeCurrent = iTimeGet();
        sTimeElapsed = iTimeDiffSec(sTimeLast, sTimeCurrent);
        if (sTimeElapsed < 0.0f)
            sTimeElapsed = 1.0f / 60.0f;
        else if (sTimeElapsed > zGameMaxElapsed())
            sTimeElapsed = zGameMaxElapsed();
        sTimeLast = sTimeCurrent;
        t0 = t1;
        t1 = iTimeGet();
        xPadUpdate(0, sTimeElapsed);
        fn_80016600();
        globals.update_dt = sTimeElapsed;
        zGameCheats(sTimeElapsed);
        zGameExtras(sTimeElapsed);
        xDrawBegin();
        S32 paused = zGameIsPaused();
        zSceneUpdate(sTimeElapsed);
        zEntShaggy_UpdateDep();
        if (!paused)
            globals.player.ent.update(&globals.player.ent, globals.sceneCur, sTimeElapsed);
        xSndSetFrame(&globals.camera.hi_cam.mat.right, &globals.camera.hi_cam.mat.up, &globals.camera.hi_cam.mat.at, &globals.camera.hi_cam.mat.pos, 25.0f);
        xSndSetFrameFar(xEntGetPos(&globals.player.ent));
        xSndUpdate(sTimeElapsed);
        zHud_Update(globals.sceneCur, sTimeElapsed);
        xCameraUpdate(&globals.camera, sTimeElapsed);
        zScene_UpdateFlyToInterface(sTimeElapsed);
        xCameraBegin(&globals.camera, 1);
        zVolume_OccludePrecalc(&globals.camera.hi_cam.mat.pos);
        zSceneRender();
        fn_8004436C();
        xDebugUpdate();
        xDrawEnd();
        xCameraEnd(&globals.camera, sTimeElapsed, 1);
        iEnvEndRenderFX(NULL);
        while ((lbl_8025E968 = VIGetRetraceCount()) - gLastVsyncFlip < 2)
            iVSync();
        gLastVsyncFlip = lbl_8025E968;
        xCameraShowRaster(&globals.camera);
        zLaughUpdate(sTimeElapsed);
        zMusicUpdate(sTimeElapsed);
        zGameUpdateMode();
        ++lbl_8025E9F8;
        if (ostrich_delay > 0)
            --ostrich_delay;
        else
            zGameSetOstrich(eGameOstrich_InScene);
    } while (zGameLoopContinue());
    iScrFxSetBlackoutFrames(3);
}

// Pause and save modes pause the game; the menu and controller state also matter.
S32 zGameIsPaused()
{
    if (gGameMode == eGameMode_Pause)
        return 1;
    if (gGameMode == eGameMode_Save)
        return 1;
    if (!zMenuRunning() && lbl_802289B8[0].state != 2)
        return 1;
    return 0;
}

// Enter the pause menu when the current state and camera allow it.
void zGamePauseIfPossible()
{
    if (zGameGetOstrich() != 2 || zMenuRunning() || zGameModeGet() == 6 ||
        zGameModeGet() == 4)
        return;
    if (fn_800AC0BC())
        return;
    if (gGameMode != eGameMode_Pause)
    {
        zGameModeSwitch(eGameMode_Pause);
        zGameStateSwitch(0);
        iPadStopRumble(globals.pad0);
        zEntEvent("techbutton6_click", 0x18);
    }
}

// Whether the start button may pause: the player has control and no cutscene is running.
static S32 zGameOkToPause()
{
    if (!globals.player.ControlOn)
    {
        return 0;
    }
    if (globals.cmgr)
    {
        return 0;
    }
    return 1;
}

// Process gameplay state changes, including the transition through a portal.
static void zGameUpdateMode()
{
    if (gGameMode != eGameMode_Game)
        return;

    if (lbl_8025E528 == 1)
    {
        iTimeGameAdvance(sTimeElapsed);
        if (globals.pad0->pressed & 1)
        {
            if (zGameOkToPause())
            {
                if (!startPressed)
                {
                    if (gGameMode != eGameMode_Pause)
                    {
                        zGameModeSwitch(eGameMode_Pause);
                        zGameStateSwitch(0);
                        iPadStopRumble(globals.pad0);
                        zEntEvent("techbutton6_click", 0x18);
                    }
                }
                else
                    startPressed = 1;
            }
        }
        else
            startPressed = 0;
    }
    else if (lbl_8025E528 == 4)
    {
        return;
    }
    else if (lbl_8025E528 == 5)
    {
        zPortalAsset* portal = globals.sceneCur->pendingPortal->passet;
        S8* sceneName = (S8*)&portal->sceneID;
        U32 sceneID = ((U32)sceneName[0] << 24) | ((U32)sceneName[1] << 16) |
                      ((U32)sceneName[2] << 8) | (U32)sceneName[3];
        if (portal->sceneID != globals.sceneCur->sceneID)
        {
            sPlayerMarkerStartID = portal->assetMarkerID;
            sPlayerMarkerStartCamID = portal->assetCameraID;
            sPlayerStartAngle = portal->ang;
            sPortalling = 1;
            zGameExit();
            S32 delay = 30;
            if (sceneID == 'S006')
                delay = 120;
            while (delay-- > 0)
                iVSync();
            zGameInit(sceneID);
            zGameSetup();
            if (globals.cmgr)
                zCutsceneMgrFinishLoad(globals.cmgr);
        }
        else if (sPlayerMarkerStartID)
        {
            U32 size;
            xVec3* pos = (xVec3*)xSTFindAsset(sPlayerMarkerStartID, &size);
            if (pos)
                globals.player.ent.frame->mat.pos = *pos;
            sPlayerMarkerStartID = 0;
        }
        xBase* camera = zSceneFindObject(sPlayerMarkerStartCamID);
        if (!camera)
        {
            xBase* fallback = zSceneGetObject(7, 0);
            sPlayerMarkerStartCamID = fallback->id;
            for (S32 i = 0; i < 50; ++i) {}
            for (S32 i = 0; i < 50; ++i) {}
            xSTAssetName(fallback->id);
        }
        if (camera)
        {
            xCameraPickInitial(&globals.camera, (_tagxCam*)camera);
            globals.player.cp.initCamID = sPlayerMarkerStartCamID;
            zEntPlayer_StoreCheckPoint(NULL);
        }
        sPlayerMarkerStartCamID = 0;
        sTimeLast = iTimeGet() - (U64)((F32)OS_TIMER_CLOCK / 60.0f);
        zGameStateSwitch(1);
    }
    else if (lbl_8025E528 == 2)
    {
        zSceneReset();
        zGameStateSwitch(1);
    }
    else if (lbl_8025E528 == 3)
    {
        if (sGameOverTimer == 0.0f)
        {
            sGameOverTimer = 5.0f;
            xScrFxFade(&clear, &black, 4.5f, NULL, 1, 1);
        }
        else
        {
            sGameOverTimer -= sTimeElapsed;
            xprintf("GAME OVER (%f secs)\n", sGameOverTimer);
            if (sGameOverTimer <= 0.0f)
            {
                sGameOverTimer = 0.0f;
                zGameStateSwitch(7);
            }
        }
    }
    else if (lbl_8025E528 == 7)
        sGameOverTimer = 0.0f;
}

// Create the temporary scene used to display loading progress.
void zGameScreenTransitionBegin()
{
    zGameSetOstrich(eGameOstrich_Loading);
    sGameScreenTransCam = iCameraCreate(640, 480);
    if (sGameScreenTransCam)
    {
        DirectionalLight = RpLightCreate(rpLIGHTDIRECTIONAL);
        if (DirectionalLight)
        {
            RwRGBAReal color;
            color.red = color.green = color.blue = 1.0f;
            color.alpha = 0.0f;
            RpLightSetColor(DirectionalLight, &color);
            RwFrame* frame = RwFrameCreate();
            _rwObjectHasFrameSetFrame(DirectionalLight, frame);
            RwBBox box;
            box.sup.x = 10000.0f;
            box.sup.y = 10000.0f;
            box.sup.z = 10000.0f;
            box.inf.x = -10000.0f;
            box.inf.y = -10000.0f;
            box.inf.z = -10000.0f;
            World = RpWorldCreate(&box);
            RpWorldAddCamera(World, sGameScreenTransCam);
        }
    }
}

// Forward a loading-screen update when the menu flag permits it.
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg)
{
    if (!zMenuIsFirstBoot())
    {
        zGameScreenTransitionUpdate(percentComplete, msg, NULL);
    }
}

// Update and draw the loading screen, including the current scene's title.
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba)
{
    if (zMenuIsFirstBoot())
        return;

    RwRGBA back_col = { 0, 0, 0, 255 };
    if (rgba)
    {
        back_col.red = rgba[0];
        back_col.green = rgba[1];
        back_col.blue = rgba[2];
        back_col.alpha = rgba[3];
    }
    xPadUpdate(0, 1.0f);
    xDrawBegin();
    if (sGameScreenTransCam)
    {
        RwCameraClear(sGameScreenTransCam, &back_col, 3);
        RwCameraBeginUpdate(sGameScreenTransCam);
    }
    xprintf("Loading... %3.2f\n", 100.0f * percentComplete);
    if (msg)
        xprintf(msg);

    char meter[256] = "...";
    switch ((loadMeter / 25) % 5)
    {
    case 0: strcpy(meter, "   "); break;
    case 1: strcpy(meter, ".  "); break;
    case 2: strcpy(meter, ".. "); break;
    case 3: strcpy(meter, "..."); break;
    case 4: loadMeter = 0; break;
    }
    zGameScreenTransition_UIRender(0.0f, 0.0f, 640.0f, 480.0f, 0);
    zGameScreenTransition_UIRender(275.0f, 32.0f, 640.0f, 448.0f, 1);
    if (globals.sceneCur)
    {
        char str[256];
        if (!zSceneGetArea(globals.sceneCur->sceneID))
        {
            sprintf(str, "Loading%s", meter);
            zHud_DrawText(210.0f, 410.0f, 400.0f, 200.0f, str, 16, 1.5f, 200, DROPSHADOW_NORMAL);
        }
        else
        {
            sprintf(str, "Entering%s", meter);
            zHud_DrawText(210.0f, 410.0f, 300.0f, 60.0f, str, 16, 1.0f, 200, DROPSHADOW_NORMAL);
            if (globals.sceneCur->sceneID == 0x48303033)
            {
                sprintf(str, "%s", zSceneGetAreaname(globals.sceneCur->sceneID));
                zHud_DrawText(32.0f, 32.0f, 385.0f, 400.0f, str, 64, 1.2f, 200, DROPSHADOW_NORMAL);
            }
            else
            {
                S32 part = zSceneGetRoominArea(globals.sceneCur->sceneID);
                sprintf(str, "%s\n\nPart %d", zSceneGetAreaname(globals.sceneCur->sceneID), part + 1);
                zHud_DrawText(32.0f, 80.0f, 385.0f, 400.0f, str, 64, 1.2f, 200, DROPSHADOW_NORMAL);
            }
        }
    }
    ++loadMeter;
    fn_8004436C();
    xFontRender();
    xDebugUpdate();
    xDrawEnd();
    if (sGameScreenTransCam)
    {
        RwCameraEndUpdate(sGameScreenTransCam);
        RwCameraShowRaster(sGameScreenTransCam, NULL, 1);
    }
}

// Draw the loading background or an untextured black rectangle.
void zGameScreenTransition_UIRender(F32 x1, F32 y1, F32 x2, F32 y2, S32 tex)
{
    if (tex)
    {
        RwTexture* texture = (RwTexture*)xSTFindAsset(xStrHash("loading screen bg"), NULL);
        if (texture && texture->raster)
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, texture->raster);
    }
    else
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);

    U8 r, g, b;
    if (tex)
    {
        r = g = b = 255;
    }
    else
    {
        r = g = b = 0;
    }
    F32 z = RwIm2DGetNearScreenZ();
    Vert[0].x = x1; Vert[0].y = y1; Vert[0].z = z;
    Vert[0].u = 0.0f; Vert[0].v = 0.0f;
    Vert[1].x = x1; Vert[1].y = y2; Vert[1].z = z;
    Vert[1].u = 0.0f; Vert[1].v = 1.0f;
    Vert[2].x = x2; Vert[2].y = y2; Vert[2].z = z;
    Vert[2].u = 1.0f; Vert[2].v = 1.0f;
    Vert[3].x = x2; Vert[3].y = y1; Vert[3].z = z;
    Vert[3].u = 1.0f; Vert[3].v = 0.0f;
    RwIm2DVertexSetIntRGBA(&Vert[0], r, g, b, 255);
    RwIm2DVertexSetIntRGBA(&Vert[1], r, g, b, 255);
    RwIm2DVertexSetIntRGBA(&Vert[2], r, g, b, 255);
    RwIm2DVertexSetIntRGBA(&Vert[3], r, g, b, 255);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)0);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)2);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)1);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)1);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)0);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
    RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, Vert, 4, Idx, 6);
}

// Release the temporary camera, light and world used by the loading screen.
void zGameScreenTransitionEnd()
{
    _rwFrameSyncDirty();
    if (DirectionalLight)
    {
        RwFrame* frame = (RwFrame*)DirectionalLight->object.object.parent;
        if (frame)
            RwFrameDestroy(frame);
        RpLightDestroy(DirectionalLight);
        DirectionalLight = NULL;
    }
    if (World)
    {
        if (sGameScreenTransCam)
        {
            RpWorldRemoveCamera(World, sGameScreenTransCam);
            iCameraDestroy(sGameScreenTransCam);
            sGameScreenTransCam = NULL;
        }
        RpWorldDestroy(World);
        World = NULL;
    }
}

// Sets up the player from the scene's last player asset: the yaw a portal left (when it set one),
// then the start position from the portal's marker (moving the camera target with it), and
// finally sends event 0xC2 to every entity of type 30.
void setupPlayer()
{
    p2MarkerAsset* m;
    U32 size;
    S32 playerCount;
    p2EntAsset* asset;

    playerCount = xSTAssetCountByType('PLYR');
    asset = (p2EntAsset*)xSTFindAssetByType('PLYR', playerCount - 1, NULL);
    asset->baseType = 3;
    if (sPortalling)
    {
        asset->ang.x = 3.1415927f * sPlayerStartAngle / 180.0f;
        sPortalling = 0;
    }
    asset->ang.y = 0.0f;
    asset->ang.z = 0.0f;
    zEntPlayer_Init(&globals.player.ent, asset);
    if (sPlayerMarkerStartID)
    {
        m = (p2MarkerAsset*)xSTFindAsset(sPlayerMarkerStartID, &size);
        if (m)
        {
            globals.player.ent.frame->mat.pos = m->pos;
            xVec3Copy((xVec3*)&globals.player.ent.model->Mat->pos, &m->pos);

            xCameraSetTarget(&globals.camera, &globals.player.ent.frame->mat.pos);
            xCameraSetTarget(&globals.camera, xEntGetFrame(&globals.player.ent));
        }
        sPlayerMarkerStartID = 0;
    }
    zEntEventAllOfType(0xC2, 30);
}

// The longest frame time the game loop simulates, in seconds.
static F32 zGameMaxElapsed()
{
    return 0.1f;
}

// Whether the last presses end with the code (compared back to its first nonzero entry); a match
// plays one of four excited Scooby sounds.
static S32 TestCheat(U32* cheat_info)
{
    S32 i;

    for (i = 13; cheat_info[i] && i >= 0; i--)
    {
        if (cheat_info[i] != sCheatPressed[i])
        {
            return 0;
        }
    }

    switch (irand() & 3)
    {
    case 0:
        xSndPlay(xStrHash("scooby_excite_01"), 0.5f, 0.5f, 0.0f, 0x80, 0x202, 0xDEADBEEF);
        break;
    case 1:
        xSndPlay(xStrHash("scooby_excite_04"), 0.5f, 0.5f, 0.0f, 0x80, 0x202, 0xDEADBEEF);
        break;
    case 2:
        xSndPlay(xStrHash("scooby_excite_09"), 0.5f, 0.5f, 0.0f, 0x80, 0x202, 0xDEADBEEF);
        break;
    case 3:
    default:
        xSndPlay(xStrHash("scooby_excite_10"), 0.5f, 0.5f, 0.0f, 0x80, 0x202, 0xDEADBEEF);
        break;
    }

    return 1;
}

// While the game is paused with the two shoulder buttons (0x1100) held, each new face-button
// press is shifted into sCheatPressed and every code is tested. Releasing the shoulders, or 0.35
// seconds without a press, clears the presses.
static void zGameCheats(F32 dt)
{
    if (zGameModeGet() != eGameMode_Pause)
    {
        return;
    }

    if ((globals.pad0->on & 0x1100) == 0x1100)
    {
        if (globals.pad0->pressed & 0xF0000)
        {
            U32 pressed = globals.pad0->pressed & 0xF0000;
            for (S32 i = 1; i < 14; i++)
            {
                sCheatPressed[i - 1] = sCheatPressed[i];
            }

            sCheatPressed[13] = pressed;
            sCheatTimer = 0.35f;
            if (TestCheat(sCheatWarpGate))
            {
                zEntPickupCheat_allWarps();
            }

            if (TestCheat(sCheatAllPowerups))
            {
                globals.player.Inv_Specials |= 1 << 0;
                globals.player.Inv_Specials |= 1 << 1;
                globals.player.Inv_Specials |= 1 << 2;
                globals.player.Inv_Specials |= 1 << 4;
                globals.player.Inv_Specials |= 1 << 3;
                globals.player.Inv_Specials |= 1 << 5;
                globals.player.Inv_Specials |= 1 << 9;
                globals.player.Inv_Specials |= 1 << 10;
                globals.player.Inv_Specials |= 1 << 11;
                globals.player.Inv_Specials |= 1 << 12;
                globals.player.Inv_Specials |= 1 << 13;
                globals.player.Inv_Specials |= 1 << 14;
                globals.player.Inv_Specials |= 1 << 6;
                globals.player.Inv_Specials |= 1 << 7;
                globals.player.Inv_Specials |= 1 << 8;
            }

            if (TestCheat(sCheatUnlockCredits))
            {
                zVarUnlockEndGame();
            }

            if (TestCheat(sCheatUnlockAllMovies))
            {
                zVarUnlockAllMovies();
            }

            sCheatTimer = 0.35f;
        }
        else
        {
            sCheatTimer -= dt;
            if (sCheatTimer <= 0.0f)
            {
                sCheatTimer = 0.35f;
                memset(sCheatPressed, 0, sizeof(sCheatPressed));
            }
        }
    }
    else
    {
        sCheatTimer = 0.35f;
        memset(sCheatPressed, 0, sizeof(sCheatPressed));
    }
}

// Test whether the active camera uses the special NICK CAM 2 asset.
extern "C" S32 fn_800AC0BC()
{
    return xStrHash("NICK CAM 2") == globals.camera.hi_cam.asset->id ? TRUE : FALSE;
}
