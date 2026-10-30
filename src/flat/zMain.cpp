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

S32 xMemPushBase();
S32 xMemGetBase();
S32 xSTPreLoadScene(U32, void*);
S32 xSTQueueSceneAssets(U32);
extern "C" void fn_80016600();
extern "C" void fn_800443AC();
void zEntPlayer_InitCutsceneModelHack();
extern "C" void fn_80016924(F32);
extern "C" void fn_80016920(U32);
extern "C" S32 lbl_8025E570;

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
        if (lbl_8025E570)
        {
            zMenuInit(menuModeID);
            zMenuSetup();
            newGameSceneID = zMenuLoop();
            zMenuExit();
        }
        else
        {
            lbl_8025E570 = 1;
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
