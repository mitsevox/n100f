#include "types.h"
#include "iCamera.h"
#include "rpworld.h"
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
#include "xEnv.h"
#include "zEntShaggy.h"
#include "zPortal.h"
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <stdio.h>
#include <string.h>

extern "C" S32 fn_800BF644();
extern "C" S32 fn_800AFD54();
extern "C" S32 fn_800AFD5C();
extern "C" void fn_800AF8D8(S32);
extern "C" void fn_800AF978(S32);
void zEntEvent(char*, U32);
extern "C" void fn_80031C34(S32);
extern "C" void fn_8003D2FC();
extern "C" void fn_8003D240(F32);
extern "C" void fn_8001E798(void*);
extern "C" void fn_800207E8(void*);
extern "C" void fn_800D0120();
extern "C" void fn_800D02D8(U32);
extern "C" void fn_800207EC(void*, S32, S32);
extern "C" void fn_8001E7C4(void*, _xEnv*);
extern "C" void fn_800B96F8();
extern "C" void fn_800B969C(zScene*);
extern "C" void fn_800CF0A4();
extern "C" void fn_800300CC(RpWorld*);
extern "C" U32 fn_8003FE80(U32);
extern "C" p2EntAsset* fn_8003FD50(U32, U32, U32*);
void zEntPlayer_Init(_xEnt*, p2EntAsset*);
extern "C" void fn_80048E54(S32, S32);
extern "C" void fn_800AA118(_zEnv*);
extern "C" void fn_8001E8A0(_tagxCamFrame*, F32);
extern "C" void fn_800CF6C4(zScene*, S32);
extern "C" void fn_8004623C(_zCutsceneMgr*);
extern "C" void fn_80014A34(S32);
extern "C" void fn_80016600();
extern "C" void fn_800165E0();
extern "C" void fn_800AC100(F32);
extern "C" void fn_800CEB6C(F32);
extern "C" void fn_8003CE34(xVec3*, xVec3*, xVec3*, xVec3*, F32);
extern "C" void fn_8003CE10(xVec3*);
extern "C" void fn_800B94C0(zScene*, F32);
extern "C" void fn_800CD2A4(F32);
extern "C" void fn_8002079C(_tagxCamFrame*, S32);
extern "C" void fn_800056F8(xVec3*);
extern "C" void fn_800CDDC0();
extern "C" void fn_8001E860(_tagxCamFrame*, S32, F32);
extern "C" void fn_8001E83C(_tagxCamFrame*);
extern "C" S32 fn_800C9BBC();
extern "C" void fn_800B74D8();
extern "C" void fn_8001692C(F32);
extern "C" void fn_800CF3A4();
extern "C" void fn_8001D9B0(_tagxCamFrame*, xBase*);
void zEntPlayer_StoreCheckPoint(F32*);
void zGameInit(U32);
void zGameSetup();
void zGameExit();
void zGameUpdateMode();
void zFX_SceneEnter(RpWorld*);
void xCameraSetTarget(_tagxCamFrame*, xVec3*);
void xCameraSetTarget(_tagxCamFrame*, xMat4x3*);
void zEntEventAll(xBase*, U32, U32, F32*);
extern "C" void fn_800AFD4C(S32);
extern "C" RpLight* RpLightCreate(RpLightType);
extern "C" RpLight* RpLightSetColor(RpLight*, const RwRGBAReal*);
extern "C" RpWorld* RpWorldCreate(const RwBBox*);
extern "C" F32 RwIm2DGetNearScreenZ();
extern "C" void fn_80021E50();
extern "C" void fn_80021B20();
extern "C" void fn_80021E30();
extern "C" void fn_8004436C();
extern "C" S32 fn_800CDB04(U32);
extern "C" char* fn_800CDA0C(U32);
extern "C" S32 fn_800CDAA4(U32);
extern "C" void fn_800B0F3C(char*, S32, S32, S32, F32, F32, F32, F32, F32);
void xFontRender();
struct _tagTRCPadInfo
{
    S32 pad_init;
    S32 id;
    S32 state;
};

extern "C" _tagTRCPadInfo lbl_802289B8[4];
extern "C" RwCamera* lbl_8025EF14;
extern "C" RpWorld* lbl_8025EF18;
extern "C" RpLight* lbl_8025EF1C;
extern "C" U32 lbl_8025EF20;
extern "C" U32 lbl_8025EED8;
extern "C" F32 lbl_8025EEE0;
extern "C" S32 lbl_8025EEE4;
extern "C" F32 lbl_8025EEEC;
extern "C" iTime lbl_8025EEF0;
extern "C" iTime lbl_8025EEF8;
extern "C" iTime lbl_8025EF00;
extern "C" iTime lbl_8025EF08;
extern "C" U32 lbl_8025EF10;
extern "C" U32 lbl_8025E968;
extern "C" U32 lbl_8025E9F8;
extern "C" S32 lbl_8025E528;
extern "C" S32 lbl_8025E510;
extern "C" U32 lbl_8025EEDC;
extern "C" F32 lbl_8025EEE8;
struct iColor_tag { U8 r, g, b, a; };
extern "C" iColor_tag lbl_8025E514;
extern "C" iColor_tag lbl_8025E518;
void xScrFxFade(iColor_tag*, iColor_tag*, F32, void (*)(), S32, S32);
extern "C" RwIm2DVertex lbl_8022EAD8[4];
extern "C" RwImVertexIndex lbl_801C5BF8[];
// Only the asset identifier is needed here.
struct xCamAsset : p2BaseAsset {};
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba);

// Test whether the active camera uses the special NICK CAM 2 asset.
extern "C" S32 fn_800AC0BC()
{
    return xStrHash("NICK CAM 2") == globals.camera.hi_cam.asset->id;
}

// Release the temporary camera, light and world used by the loading screen.
void zGameScreenTransitionEnd()
{
    _rwFrameSyncDirty();
    if (lbl_8025EF1C)
    {
        RwFrame* frame = (RwFrame*)lbl_8025EF1C->object.object.parent;
        if (frame)
            RwFrameDestroy(frame);
        RpLightDestroy(lbl_8025EF1C);
        lbl_8025EF1C = NULL;
    }
    if (lbl_8025EF18)
    {
        if (lbl_8025EF14)
        {
            RpWorldRemoveCamera(lbl_8025EF18, lbl_8025EF14);
            iCameraDestroy(lbl_8025EF14);
            lbl_8025EF14 = NULL;
        }
        RpWorldDestroy(lbl_8025EF18);
        lbl_8025EF18 = NULL;
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

    U8 r = tex ? 255 : 0;
    U8 g = tex ? 255 : 0;
    U8 b = tex ? 255 : 0;
    F32 z = RwIm2DGetNearScreenZ();
    lbl_8022EAD8[0].x = x1; lbl_8022EAD8[0].y = y1; lbl_8022EAD8[0].z = z;
    lbl_8022EAD8[0].u = 0.0f; lbl_8022EAD8[0].v = 0.0f;
    lbl_8022EAD8[1].x = x1; lbl_8022EAD8[1].y = y2; lbl_8022EAD8[1].z = z;
    lbl_8022EAD8[1].u = 0.0f; lbl_8022EAD8[1].v = 1.0f;
    lbl_8022EAD8[2].x = x2; lbl_8022EAD8[2].y = y2; lbl_8022EAD8[2].z = z;
    lbl_8022EAD8[2].u = 1.0f; lbl_8022EAD8[2].v = 1.0f;
    lbl_8022EAD8[3].x = x2; lbl_8022EAD8[3].y = y1; lbl_8022EAD8[3].z = z;
    lbl_8022EAD8[3].u = 1.0f; lbl_8022EAD8[3].v = 0.0f;
    RwIm2DVertexSetIntRGBA(&lbl_8022EAD8[0], r, g, b, 255);
    RwIm2DVertexSetIntRGBA(&lbl_8022EAD8[1], r, g, b, 255);
    RwIm2DVertexSetIntRGBA(&lbl_8022EAD8[2], r, g, b, 255);
    RwIm2DVertexSetIntRGBA(&lbl_8022EAD8[3], r, g, b, 255);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)0);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)2);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)1);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)1);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)0);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)0);
    RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, lbl_8022EAD8, 4, lbl_801C5BF8, 6);
}

// Update and draw the loading screen, including the current scene's title.
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg, U8* rgba)
{
    if (fn_800BF644())
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
    fn_80021E50();
    if (lbl_8025EF14)
    {
        RwCameraClear(lbl_8025EF14, &back_col, 3);
        RwCameraBeginUpdate(lbl_8025EF14);
    }
    xprintf("Loading... %3.2f\n", 100.0f * percentComplete);
    if (msg)
        xprintf(msg);

    char meter[256] = "...";
    switch ((lbl_8025EF20 / 25) % 5)
    {
    case 0: strcpy(meter, "   "); break;
    case 1: strcpy(meter, ".  "); break;
    case 2: strcpy(meter, ".. "); break;
    case 3: strcpy(meter, "..."); break;
    case 4: lbl_8025EF20 = 0; break;
    }
    zGameScreenTransition_UIRender(0.0f, 0.0f, 640.0f, 480.0f, 0);
    zGameScreenTransition_UIRender(275.0f, 32.0f, 640.0f, 448.0f, 1);
    if (globals.sceneCur)
    {
        char str[256];
        U32 sceneID = globals.sceneCur->sceneID;
        if (!fn_800CDB04(sceneID))
        {
            sprintf(str, "Loading%s", meter);
            fn_800B0F3C(str, 16, 200, 1, 210.0f, 410.0f, 400.0f, 200.0f, 1.5f);
        }
        else
        {
            sprintf(str, "Entering%s", meter);
            fn_800B0F3C(str, 16, 200, 1, 210.0f, 410.0f, 300.0f, 60.0f, 1.0f);
            if (sceneID == 0x48303033)
            {
                sprintf(str, "%s", fn_800CDA0C(sceneID));
                fn_800B0F3C(str, 64, 200, 1, 32.0f, 32.0f, 385.0f, 400.0f, 1.2f);
            }
            else
            {
                S32 part = fn_800CDAA4(sceneID);
                sprintf(str, "%s\n\nPart %d", fn_800CDA0C(sceneID), part + 1);
                fn_800B0F3C(str, 64, 200, 1, 32.0f, 80.0f, 385.0f, 400.0f, 1.2f);
            }
        }
    }
    ++lbl_8025EF20;
    fn_8004436C();
    xFontRender();
    fn_80021B20();
    fn_80021E30();
    if (lbl_8025EF14)
    {
        RwCameraEndUpdate(lbl_8025EF14);
        RwCameraShowRaster(lbl_8025EF14, NULL, 1);
    }
}

// Forward a loading-screen update when the menu flag permits it.
void zGameScreenTransitionUpdate(F32 percentComplete, char* msg)
{
    if (!fn_800BF644())
    {
        zGameScreenTransitionUpdate(percentComplete, msg, NULL);
    }
}

// Create the temporary scene used to display loading progress.
void zGameScreenTransitionBegin()
{
    fn_800AFD4C(0);
    lbl_8025EF14 = iCameraCreate(640, 480);
    if (lbl_8025EF14)
    {
        lbl_8025EF1C = RpLightCreate(rpLIGHTDIRECTIONAL);
        if (lbl_8025EF1C)
        {
            RwRGBAReal color;
            color.red = color.green = color.blue = 1.0f;
            color.alpha = 0.0f;
            RpLightSetColor(lbl_8025EF1C, &color);
            RwFrame* frame = RwFrameCreate();
            _rwObjectHasFrameSetFrame(lbl_8025EF1C, frame);
            RwBBox box;
            box.sup.x = 10000.0f;
            box.sup.y = 10000.0f;
            box.sup.z = 10000.0f;
            box.inf.x = -10000.0f;
            box.inf.y = -10000.0f;
            box.inf.z = -10000.0f;
            lbl_8025EF18 = RpWorldCreate(&box);
            RpWorldAddCamera(lbl_8025EF18, lbl_8025EF14);
        }
    }
}

// Enter the pause menu when the current state and camera allow it.
void zGamePauseIfPossible()
{
    if (fn_800AFD54() != 2 || zMenuRunning() || fn_800AFD5C() == 6 ||
        fn_800AFD5C() == 4)
        return;
    if (xStrHash("NICK CAM 2") == globals.camera.hi_cam.asset->id)
        return;
    if (gGameMode != eGameMode_Pause)
    {
        fn_800AF8D8(7);
        fn_800AF978(0);
        iPadStopRumble(globals.pad0);
        zEntEvent("techbutton6_click", 0x18);
    }
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

// Advance input, simulation, rendering and game mode until gameplay exits.
void zGameLoop()
{
    S32 ostrich_delay = 10;
    fn_800AF978(1);
    lbl_8025EEF0 = iTimeGet() - (U64)((F32)OS_TIMER_CLOCK / 60.0f);
    if (globals.cmgr)
        fn_8004623C(globals.cmgr);
    fn_80014A34(0);
    do
    {
        lbl_8025EEF8 = iTimeGet();
        lbl_8025EEEC = iTimeDiffSec(lbl_8025EEF0, lbl_8025EEF8);
        if (lbl_8025EEEC < 0.0f)
            lbl_8025EEEC = 1.0f / 60.0f;
        else if (lbl_8025EEEC > 0.1f)
            lbl_8025EEEC = 0.1f;
        lbl_8025EEF0 = lbl_8025EEF8;
        lbl_8025EF00 = lbl_8025EF08;
        lbl_8025EF08 = iTimeGet();
        xPadUpdate(0, lbl_8025EEEC);
        fn_80016600();
        globals.update_dt = lbl_8025EEEC;
        fn_800AC100(lbl_8025EEEC);
        zGameExtras(lbl_8025EEEC);
        fn_80021E50();
        S32 paused = zGameIsPaused();
        fn_800CEB6C(lbl_8025EEEC);
        zEntShaggy_UpdateDep();
        if (!paused)
            globals.player.ent.update(&globals.player.ent, globals.sceneCur, lbl_8025EEEC);
        xMat4x3* mat = &globals.camera.hi_cam.mat;
        fn_8003CE34(&mat->pos, &mat->at, &mat->up, &mat->right, 25.0f);
        fn_8003CE10((xVec3*)&globals.player.ent.model->Mat->pos);
        fn_8003D240(lbl_8025EEEC);
        fn_800B94C0(globals.sceneCur, lbl_8025EEEC);
        fn_8001E8A0(&globals.camera, lbl_8025EEEC);
        fn_800CD2A4(lbl_8025EEEC);
        fn_8002079C(&globals.camera, 1);
        fn_800056F8(&mat->pos);
        fn_800CDDC0();
        fn_8004436C();
        fn_80021B20();
        fn_80021E30();
        fn_8001E860(&globals.camera, 1, lbl_8025EEEC);
        iEnvEndRenderFX(NULL);
        while ((lbl_8025E968 = VIGetRetraceCount()) - lbl_8025EF10 < 2)
            fn_800165E0();
        lbl_8025EF10 = lbl_8025E968;
        fn_8001E83C(&globals.camera);
        zLaughUpdate(lbl_8025EEEC);
        zMusicUpdate(lbl_8025EEEC);
        zGameUpdateMode();
        ++lbl_8025E9F8;
        if (ostrich_delay > 0)
            --ostrich_delay;
        else
            fn_800AFD4C(2);
        if (gGameMode == eGameMode_Game)
        {
            if (lbl_8025E528 != 1 && lbl_8025E528 != 3 && lbl_8025E528 != 4)
                break;
        }
        else if (gGameMode == eGameMode_Save)
        {
            S32 saved = fn_800C9BBC();
            lbl_8025EEF0 = iTimeGet();
            lbl_8025EF08 = iTimeGet();
            if (saved)
                fn_800B74D8();
        }
    } while (1);
    fn_80014A34(3);
}

// Prepare the player and scene after loading, then start their game services.
void zGameSetup()
{
    fn_800CF0A4();
    RpWorld* world = globals.sceneCur->env->geom->world;
    fn_800300CC(world);
    zFX_SceneEnter(world);
    p2EntAsset* asset = fn_8003FD50('PLYR', fn_8003FE80('PLYR') - 1, NULL);
    asset->baseType = 3;
    if (lbl_8025EEE4)
    {
        asset->ang.x = 3.1415927f * lbl_8025EEE0 / 180.0f;
        lbl_8025EEE4 = 0;
    }
    asset->ang.y = 0.0f;
    asset->ang.z = 0.0f;
    zEntPlayer_Init(&globals.player.ent, asset);
    if (lbl_8025EED8)
    {
        U32 size;
        xVec3* pos = (xVec3*)xSTFindAsset(lbl_8025EED8, &size);
        if (pos)
        {
            globals.player.ent.frame->mat.pos = *pos;
            globals.player.ent.model->Mat->pos.x = pos->x;
            globals.player.ent.model->Mat->pos.y = pos->y;
            globals.player.ent.model->Mat->pos.z = pos->z;
            xCameraSetTarget(&globals.camera, &globals.player.ent.frame->mat.pos);
            xCameraSetTarget(&globals.camera, (xMat4x3*)globals.player.ent.model->Mat);
        }
        lbl_8025EED8 = 0;
    }
    fn_80048E54(0xC2, 30);
    zEntShaggy_SceneStart(globals.sceneCur);
    fn_800AA118(gCurEnv);
    fn_8001E8A0(&globals.camera, -1.0f);
    fn_800CF6C4(globals.sceneCur, 0);
    zMusicNotify(0);
    zEntEventAll(NULL, 0, 0x57, NULL);
    zEntEventAll(NULL, 0, 0x59, NULL);
    zGameExtrasInit();
}

// Tear down the current game and its scene services.
void zGameExit()
{
    zGameExtrasReset();
    fn_80031C34(0);
    zMusicKill();
    fn_8003D2FC();
    fn_8003D240(0.0f);
    fn_8001E798(&globals);
    fn_800207E8(&globals);
    fn_800D0120();
}

// Initialize the scene, camera, music and counters for a new game.
void zGameInit(U32 theSceneID)
{
    isrand((U32)iTimeGet());
    iTimeGet();
    irand();
    fn_800D02D8(theSceneID);
    fn_800207EC(&globals.camera, 640, 480);
    fn_8001E7C4(&globals.camera, globals.sceneCur->env);
    zLaughInit();
    zMusicInit();
    globals.stats.gameTime = 0.0f;
    globals.stats.tokens = 0;
    globals.stats.tokens_tot = 0;
    globals.stats.snacks = 0;
    globals.stats.snacks_tot = 0;
    fn_800B96F8();
    fn_800B969C(globals.sceneCur);
}

// Process gameplay state changes, including the transition through a portal.
void zGameUpdateMode()
{
    if (gGameMode != eGameMode_Game)
        return;

    if (lbl_8025E528 == 1)
    {
        fn_8001692C(lbl_8025EEEC);
        if (globals.pad0->pressed & 1)
        {
            if (globals.player.ControlOn && !globals.cmgr)
            {
                if (!lbl_8025E510)
                {
                    if (gGameMode != eGameMode_Pause)
                    {
                        fn_800AF8D8(7);
                        fn_800AF978(0);
                        iPadStopRumble(globals.pad0);
                        zEntEvent("techbutton6_click", 0x18);
                    }
                }
                else
                    lbl_8025E510 = 1;
            }
        }
        else
            lbl_8025E510 = 0;
    }
    else if (lbl_8025E528 == 5)
    {
        zPortalAsset* portal = globals.sceneCur->pendingPortal->passet;
        S8* sceneName = (S8*)&portal->sceneID;
        U32 sceneID = ((U32)sceneName[0] << 24) | ((U32)sceneName[1] << 16) |
                      ((U32)sceneName[2] << 8) | (U32)sceneName[3];
        if (portal->sceneID != globals.sceneCur->sceneID)
        {
            lbl_8025EED8 = portal->assetMarkerID;
            lbl_8025EEDC = portal->assetCameraID;
            lbl_8025EEE0 = portal->ang;
            lbl_8025EEE4 = 1;
            zGameExit();
            S32 delay = sceneID == 'S006' ? 120 : 30;
            while (delay-- > 0)
                fn_800165E0();
            zGameInit(sceneID);
            zGameSetup();
            if (globals.cmgr)
                fn_8004623C(globals.cmgr);
        }
        else if (lbl_8025EED8)
        {
            U32 size;
            xVec3* pos = (xVec3*)xSTFindAsset(lbl_8025EED8, &size);
            if (pos)
                globals.player.ent.frame->mat.pos = *pos;
            lbl_8025EED8 = 0;
        }
        xBase* camera = zSceneFindObject(lbl_8025EEDC);
        if (!camera)
        {
            xBase* fallback = zSceneGetObject(7, 0);
            lbl_8025EEDC = fallback->id;
            for (S32 i = 0; i < 50; ++i) {}
            for (S32 i = 0; i < 50; ++i) {}
            xSTAssetName(fallback->id);
        }
        if (camera)
        {
            fn_8001D9B0(&globals.camera, camera);
            globals.player.cp.initCamID = lbl_8025EEDC;
            zEntPlayer_StoreCheckPoint(NULL);
        }
        lbl_8025EEDC = 0;
        lbl_8025EEF0 = iTimeGet() - (U64)((F32)OS_TIMER_CLOCK / 60.0f);
        fn_800AF978(1);
    }
    else if (lbl_8025E528 == 2)
    {
        fn_800CF3A4();
        fn_800AF978(1);
    }
    else if (lbl_8025E528 == 3)
    {
        if (lbl_8025EEE8 == 0.0f)
        {
            lbl_8025EEE8 = 5.0f;
            xScrFxFade(&lbl_8025E518, &lbl_8025E514, 4.5f, NULL, 1, 1);
        }
        else
        {
            lbl_8025EEE8 -= lbl_8025EEEC;
            xprintf("GAME OVER (%f secs)\n", lbl_8025EEE8);
            if (lbl_8025EEE8 <= 0.0f)
            {
                lbl_8025EEE8 = 0.0f;
                fn_800AF978(7);
            }
        }
    }
    else if (lbl_8025E528 == 7)
        lbl_8025EEE8 = 0.0f;
}
