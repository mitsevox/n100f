#include "types.h"
#include "zGlobals.h"
#include "xutil.h"
#include "xserializer.h"
#include "zMenu.h"
#include "zGameState.h"
#include "zScene.h"
#include "zCutsceneMgr.h"
#include "zEntPickup.h"
#include "zVar.h"
#include "xIni.h"
#include "xString.h"
#include "xFont.h"
#include "iCamera.h"
#include "iFile.h"
#include "xPad.h"
#include <string.h>
#include <stdio.h>

extern void CalcJumpImpulse(zJumpParam* param);
extern "C" void fn_80016218();
extern "C" void fn_8002FB54(void (*)());
extern "C" void fn_80044714();
extern xIniFile* xIniParse(char*, S32);
extern void xIniDestroy(xIniFile*);
extern "C" void fn_800162D4(char*);
extern "C" void fn_800162D0(char*, S32);
extern "C" st_SERIAL_PERCID_SIZE g_xser_sizeinfo[];
extern "C" void fn_8004766C();
extern void zAssetStartup();
extern void xFontInit();
extern void xScrFxInit();
extern void xParMgrInit();
extern "C" _tagxPad* fn_800322F0(S32);
extern void xPadRumbleEnable(S32, S32);
extern "C" void fn_80038CF8();
extern void zEntProjectile_Startup();
extern void zLobMaster_Startup();
extern S32 zEntVillain_Startup();
extern "C" void fn_80043E78();
extern "C" RwRGBA lbl_80262CC8;
extern void xFontBoxSetInset(xFontBox*, F32, F32, F32, F32);
extern "C" void fn_800165E0();
extern S32 zEntVillain_Shutdown();
extern void zLobMaster_Shutdown();
extern void zEntProjectile_Shutdown();
extern void zAssetShutdown();
extern "C" void fn_80047668();
extern S32 xSGShutdown();
extern "C" void fn_8003AC90();
extern "C" void fn_800161C0();

S32 xMemPushBase();
S32 xMemGetBase();
S32 xSTPreLoadScene(U32, void*);
S32 xSTQueueSceneAssets(U32);
extern "C" void fn_80016600();
extern "C" void fn_800443AC();
void zEntPlayer_InitCutsceneModelHack();
extern "C" void fn_80016924(F32);
extern "C" void fn_80016920(U32);
extern "C" S32 sShowMenuOnBoot;

F32 xSTLoadStep(U32);
void iShadowInit();
void zGameInit(U32);
void zGameSetup();
void zGameLoop();
void zGameExit();

// Load the boot and menu packages, then alternate between the menu and game scenes.
void zMainLoop()
{
    xMemPushBase();
    xUtil_idtag2string('BOOT', 0);
    xSTPreLoadScene('BOOT', NULL);
    xUtil_idtag2string('BOOT', 0);
    xSTQueueSceneAssets('BOOT');
    xUtil_idtag2string('BOOT', 0);
    while (xSTLoadStep('BOOT') < 1.0f)
    {
        fn_80016600();
        fn_800443AC();
    }
    xUtil_idtag2string('BOOT', 0);
    iShadowInit();
    zEntPlayer_InitCutsceneModelHack();
    xMemPushBase();
    xUtil_idtag2string('MNU4', 0);
    xSTPreLoadScene('MNU4', NULL);
    xUtil_idtag2string('MNU4', 0);
    xSTQueueSceneAssets('MNU4');
    xUtil_idtag2string('MNU4', 0);
    while (xSTLoadStep('MNU4') < 1.0f)
    {
        fn_80016600();
        fn_800443AC();
    }
    xUtil_idtag2string('MNU4', 0);

    static U32 menuModeID = 'MNU3';
    static U32 gameSceneID = (globals.sceneStart[0] << 24) | (globals.sceneStart[1] << 16) |
                             (globals.sceneStart[2] << 8) | globals.sceneStart[3];
    U32 newGameSceneID = gameSceneID;
    xUtil_idtag2string(gameSceneID, 0);
    xMemPushBase();
    for (;;)
    {
        xSerialWipeMainBuffer();
        zSceneStats_Clear();
        zCutSceneNamesTable_clearAll();
        zEntPickupResetWarps();
        if (sShowMenuOnBoot)
        {
            zMenuInit(menuModeID);
            zMenuSetup();
            newGameSceneID = zMenuLoop();
            zMenuExit();
        }
        else
        {
            sShowMenuOnBoot = 1;
            zGameModeSwitch(eGameMode_Game);
            zGameStateSwitch(0);
        }
        if (!newGameSceneID)
        {
            zVarNewGame();
            fn_80016924(0.0f);
            gameSceneID = (globals.sceneStart[0] << 24) | (globals.sceneStart[1] << 16) |
                          (globals.sceneStart[2] << 8) | globals.sceneStart[3];
        }
        else
        {
            gameSceneID = newGameSceneID;
            fn_80016924(0.0f);
        }
        zGameInit(gameSceneID);
        zGameSetup();
        fn_80016920(gameSceneID);
        zGameLoop();
        zGameExit();
    }
}

// Check the memory base and scene memory level.
void zMainMemLvlChkCB()
{
    xMemGetBase();
    zSceneMemLvlChkCB();
}

// Load the player tuning values and initial equipment from the INI file.
void zMainParseINIGlobals(xIniFile* ini)
{
    static F32 fbuf_SpeedSneak[16] = { 0.5f, 1.5f, 0.0f, 1.0f };
    static F32 fbuf_SpeedWalk[16] = { 2.0f, 4.0f, 0.0f, 1.0f };
    static F32 fbuf_SpeedRun[16] = { 4.0f, 7.0f, 0.0f, 1.0f };
    static F32 fbuf_SpeedAir[16] = { 3.0f, 6.0f, 0.0f, 1.0f };
    static F32 fbuf_SpeedHotsauce[16] = { 8.0f, 8.0f, 0.2f, 1.0f };
    static F32 fbuf_AnimSneak[12] = { 1.5f, 0.5f, 1.5f };
    static F32 fbuf_AnimWalk[12] = { 3.0f, 0.5f, 2.5f };
    static F32 fbuf_Jump[16] = { 2.0f, 0.7f, 0.35f, 0.0f };
    static F32 fbuf_Double[16] = { 1.0f, 0.7f, 0.35f, 0.0f };
    static F32 fbuf_Bounce[16] = { 1.5f, 0.2f, 0.2f, 0.0f };
    static F32 fbuf_Spring[16] = { 3.0f, 0.2f, 0.2f, 0.0f };
    static F32 fbuf_Toss[16] = { 4.0f, 0.5f, 0.0f, 0.0f };
    char* value;

    globals.player.s.AnalogMin = xIniGetInt(ini, "AnalogMin", 32);
    globals.player.s.AnalogMax = xIniGetInt(ini, "AnalogMax", 110);
    value = xIniGetString(ini, "SpeedSneak", NULL);
    xStrParseFloatList((F32*)memcpy(globals.player.s.SpeedSneak, fbuf_SpeedSneak, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "SpeedWalk", NULL);
    xStrParseFloatList((F32*)memcpy(globals.player.s.SpeedWalk, fbuf_SpeedWalk, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "SpeedRun", NULL);
    xStrParseFloatList((F32*)memcpy(globals.player.s.SpeedRun, fbuf_SpeedRun, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "SpeedAir", NULL);
    xStrParseFloatList((F32*)memcpy(globals.player.s.SpeedAir, fbuf_SpeedAir, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "SpeedHotsauce", NULL);
    xStrParseFloatList((F32*)memcpy(globals.player.s.SpeedHotsauce, fbuf_SpeedHotsauce, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "AnimSneak", NULL);
    xStrParseFloatList((F32*)memcpy(globals.player.s.AnimSneak, fbuf_AnimSneak, 3 * sizeof(F32)), value, 3);
    value = xIniGetString(ini, "AnimWalk", NULL);
    xStrParseFloatList((F32*)memcpy(globals.player.s.AnimWalk, fbuf_AnimWalk, 3 * sizeof(F32)), value, 3);
    globals.player.s.Gravity = xIniGetFloat(ini, "Gravity", 20.0f);
    globals.player.s.JumpGravity = xIniGetFloat(ini, "JumpGravity", 7.0f);
    globals.player.s.GravSmooth = xIniGetFloat(ini, "GravSmooth", 0.25f);
    globals.player.s.FloatSpeed = xIniGetFloat(ini, "FloatSpeed", 1.0f);
    globals.player.s.ButtsmashSpeed = xIniGetFloat(ini, "ButtsmashSpeed", 5.0f);
    value = xIniGetString(ini, "Jump", NULL);
    xStrParseFloatList((F32*)memcpy(&globals.player.s.Jump, fbuf_Jump, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "Double", NULL);
    xStrParseFloatList((F32*)memcpy(&globals.player.s.Double, fbuf_Double, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "Bounce", NULL);
    xStrParseFloatList((F32*)memcpy(&globals.player.s.Bounce, fbuf_Bounce, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "Spring", NULL);
    xStrParseFloatList((F32*)memcpy(&globals.player.s.Spring, fbuf_Spring, 4 * sizeof(F32)), value, 4);
    value = xIniGetString(ini, "Toss", NULL);
    xStrParseFloatList((F32*)memcpy(&globals.player.s.Toss, fbuf_Toss, 4 * sizeof(F32)), value, 4);
    globals.player.s.HeadbuttSpeedMin = xIniGetFloat(ini, "HeadbuttSpeedMin", 3.0f);
    globals.player.s.HeadbuttSpeedMax = xIniGetFloat(ini, "HeadbuttSpeedMax", 10.0f);
    globals.player.s.HeadbuttAccel = xIniGetFloat(ini, "HeadbuttAccel", 5.0f);
    globals.player.s.HeadbuttTimeMin = xIniGetFloat(ini, "HeadbuttTimeMin", 0.75f);
    globals.player.s.HeadbuttTimeMax = xIniGetFloat(ini, "HeadbuttTimeMax", 2.0f);
    globals.player.s.HeadbuttTurn = xIniGetFloat(ini, "HeadbuttTurn", 2.5f);
    globals.player.s.FireDelay = xIniGetFloat(ini, "FireDelay", 0.3f);
    globals.player.s.FireRepeat = xIniGetFloat(ini, "FireRepeat", 0.6f);
    globals.player.s.FirePress = xIniGetFloat(ini, "FirePress", 0.3f);
    globals.player.s.FireTargetYaw = xIniGetFloat(ini, "FireTargetYaw", 15.0f);
    globals.player.s.FireTargetPitch = xIniGetFloat(ini, "FireTargetPitch", 30.0f);
    globals.player.s.FireTargetDist = xIniGetFloat(ini, "FireTargetDist", 0.7f);
    globals.player.s.FireTargetRadius = xIniGetFloat(ini, "FireTargetRadius", 0.25f);
    globals.player.s.FireTrack = xIniGetFloat(ini, "FireTrack", 0.0f);
    globals.player.s.FireGumSpeed = xIniGetFloat(ini, "FireGumSpeed", 10.0f);
    globals.player.s.FireSoapSpeed = xIniGetFloat(ini, "FireSoapSpeed", 10.0f);
    globals.player.s.FireGumTimer = xIniGetFloat(ini, "FireGumTimer", 3.0f);
    globals.player.s.FireSoapTimer = xIniGetFloat(ini, "FireSoapTimer", 3.0f);
    globals.player.s.AmmoCapacityGum = xIniGetInt(ini, "AmmoCapacityGum", 50);
    globals.player.s.AmmoCapacitySoap = xIniGetInt(ini, "AmmoCapacitySoap", 50);
    globals.player.s.AmmoSceneGum = xIniGetInt(ini, "AmmoSceneGum", 20);
    globals.player.s.AmmoSceneSoap = xIniGetInt(ini, "AmmoSceneSoap", 20);
    globals.player.Inv_AmmoMax[0] = globals.player.s.AmmoCapacityGum;
    globals.player.Inv_AmmoMax[1] = globals.player.s.AmmoCapacitySoap;
    globals.player.s.TakeDamage = xIniGetInt(ini, "TakeDamage", 0);
    globals.player.s.StartSlideAngle = xIniGetFloat(ini, "StartSlideAngle", 30.0f);
    globals.player.s.StopSlideAngle = xIniGetFloat(ini, "StopSlideAngle", 10.0f);
    globals.player.s.RotMatchMaxAngle = xIniGetFloat(ini, "RotMatchMaxAngle", 30.0f);
    globals.player.s.RotMatchMatchTime = xIniGetFloat(ini, "RotMatchMatchTime", 0.1f);
    globals.player.s.RotMatchRelaxTime = xIniGetFloat(ini, "RotMatchRelaxTime", 0.3f);
    globals.player.s.StartSlideAngle = 3.1415927f * globals.player.s.StartSlideAngle / 180.0f;
    globals.player.s.StopSlideAngle = 3.1415927f * globals.player.s.StopSlideAngle / 180.0f;
    globals.player.s.RotMatchMaxAngle = 3.1415927f * globals.player.s.RotMatchMaxAngle / 180.0f;
    CalcJumpImpulse(&globals.player.s.Jump);
    CalcJumpImpulse(&globals.player.s.Double);
    CalcJumpImpulse(&globals.player.s.Bounce);
    CalcJumpImpulse(&globals.player.s.Spring);
    CalcJumpImpulse(&globals.player.s.Toss);
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_BubbleGum", 1) << 0;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_SoapBar", 1) << 1;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_StickyBoots", 0) << 2;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_Plungers", 0) << 3;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_Slippers", 0) << 4;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_LampShade", 0) << 5;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_BlackKnight", 0) << 6;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_FlowerPot", 0) << 7;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_DivingHelmet", 0) << 8;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_Spring", 1) << 9;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_LightningBolt", 1) << 10;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_FootballHelmet", 1) << 11;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_Umbrella", 0) << 12;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_Shovel", 0) << 13;
    globals.player.s.Initial_Specials |= xIniGetInt(ini, "eSPECIAL_LightningBoltStun", 0) << 14;
    sShowMenuOnBoot = xIniGetInt(ini, "ShowMenuOnBoot", 1);
}

extern "C" void main()
{
    RwCamera* camera;
    xFontBox* box;

    fn_80016218();
    fn_8002FB54(zMainMemLvlChkCB);
    memset(&globals, 0, sizeof(globals));
    globals.sceneFirst = 1;
    fn_80044714();

    U32 size;
    U32* data = iFileLoad("SD2.INI", 0, &size);
    if (data)
    {
        xIniFile* ini = xIniParse((char*)data, size);
        char* path = xIniGetString(ini, "PATH", 0);
        if (path)
            fn_800162D4(path);
        char* boot = xIniGetString(ini, "BOOT", 0);
        if (boot)
        {
            strcpy(globals.sceneStart, "BOOT");
            if (strlen(boot) == 4)
                strcpy(globals.sceneStart, boot);
        }
        globals.profile = xIniGetInt(ini, "Profile", 0);
        strncpy(globals.profFunc[0], xIniGetString(ini, "ProfFuncTriangle", ""), 128);
        globals.profFunc[0][127] = 0;
        strncpy(globals.profFunc[1], xIniGetString(ini, "ProfFuncSquare", ""), 128);
        globals.profFunc[1][127] = 0;
        strncpy(globals.profFunc[2], xIniGetString(ini, "ProfFuncLeft", ""), 128);
        globals.profFunc[2][127] = 0;
        strncpy(globals.profFunc[3], xIniGetString(ini, "ProfFuncRight", ""), 128);
        globals.profFunc[3][127] = 0;
        strncpy(globals.profFunc[4], xIniGetString(ini, "ProfFuncUp", ""), 128);
        globals.profFunc[4][127] = 0;
        strncpy(globals.profFunc[5], xIniGetString(ini, "ProfFuncDown", ""), 128);
        globals.profFunc[5][127] = 0;
        zMainParseINIGlobals(ini);
        xIniDestroy(ini);
    }

    fn_800162D0("scooby.elf", globals.profile);
    xUtilStartup();
    xSerialStartup(128, g_xser_sizeinfo);
    fn_8004766C();
    zAssetStartup();
    xFontInit();
    xScrFxInit();
    xParMgrInit();
    globals.option_vibration = 1;
    printf("---PAD ENABLE\n");
    globals.pad0 = fn_800322F0(0);
    globals.pad1 = gDebugPad = fn_800322F0(1);
    xPadRumbleEnable(0, globals.option_vibration);
    fn_80038CF8();
    zEntProjectile_Startup();
    zLobMaster_Startup();
    zEntVillain_Startup();
    fn_80043E78();

    iColor_tag color = {255, 230, 0, 200};
    camera = iCameraCreate(640, 480);
    RwRGBA clear = lbl_80262CC8;
    RwCameraClear(camera, &clear, 3);
    RwCameraBeginUpdate(camera);
    char text[] = "Game and Software, excluding Scooby-Doo materials, (C) 2002 THQ Inc. Used under exclusive license by THQ Inc.\n\n\nDeveloped by Heavy Iron Studios_. Heavy Iron Studios, THQ and their respective logos are trademarks and/or registered trademarks of THQ Inc. All Rights Reserved.\n\n\nLICENSED BY NINTENDO";
    box = xFontBoxCreate(0.0f, 0.0f, 640.0f, 480.0f, 1, color);
    xFontBoxSetJustification(box, 0x40);
    xFontBoxSetDrawSize(box, 17.0f, 22.6f);
    xFontBoxSetDrawSpacing(box, 15.5f, 22.6f);
    xFontBoxSetColor(box, color);
    xFontBoxSetInset(box, 30.0f, 30.0f, 120.0f, 20.0f);
    xFontBoxSetText(box, text);
    xFontBoxRender(box);
    xFontBoxDestroy(box);
    RwCameraEndUpdate(camera);
    RwCameraShowRaster(camera, 0, 1);
    iCameraDestroy(camera);
    S32 count = 120;
    do
    {
        fn_80016600();
        fn_800165E0();
    } while (count-- > 0);
    printf("----ENTERING MAIN LOOP\n");
    zMainLoop();
    zEntVillain_Shutdown();
    zLobMaster_Shutdown();
    zEntProjectile_Shutdown();
    zAssetShutdown();
    fn_80047668();
    xSGShutdown();
    fn_8003AC90();
    xUtilShutdown();
    fn_800161C0();
}
