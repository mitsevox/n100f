#include "zVar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iSnd.h"
#include "iSystem.h"
#include "xAnim.h"
#include "xString.h"
#include "xSnd.h"
#include "xserializer.h"
#include "xsavegame.h"
#include "xutil.h"
#include "zCutsceneMgr.h"
#include "zEntPickup.h"
#include "zGlobals.h"
#include "zSaveLoad.h"
#include "zScene.h"

extern "C" S32 fn_800C9698(char* dst);
extern "C" S32 fn_800C96C0(char* dst);
extern "C" S32 fn_800C96E8(char* dst);
extern "C" S32 fn_800C9710(char* dst);

// Callbacks the variable tables below refer to.
void zVarMagicProcessText(char* src, char* dst);
void zVarInit(zVarEntry* table);
void zVarNewGame();
void zVarSave(xSerial* s);
void zVarLoad(xSerial* s);
U32 zVarEntryCB_PCChances();
U32 zVarEntryCB_PCScoobySnacks();
U32 zVarEntryCB_SndMode();
U32 zVarEntryCB_SndMasterVol();
U32 zVarEntryCB_SndMusicVol();
U32 zVarEntryCB_SndFXVol();
U32 zVarEntryCB_PCSpecialBubbleGum();
U32 zVarEntryCB_PCSpecialSoapBar();
U32 zVarEntryCB_PCSpecialStickyBoots();
U32 zVarEntryCB_PCSpecialPlungers();
U32 zVarEntryCB_PCSpecialSlippers();
U32 zVarEntryCB_PCSpecialLampShade();
U32 zVarEntryCB_PCSpecialBlackKnight();
U32 zVarEntryCB_PCSpecialFlowerPot();
U32 zVarEntryCB_PCSpecialDivingHelmet();
U32 zVarEntryCB_PCSpecialSpring();
U32 zVarEntryCB_PCSpecialLightningBolt();
U32 zVarEntryCB_PCSpecialFootballHelmet();
U32 zVarEntryCB_PCSpecialUmbrella();
U32 zVarEntryCB_PCSpecialShovel();
U32 zVarEntryCB_PCSpecialLightningBoltStun();
U32 zVarEntryCB_MCAvailable();
U32 zVarEntryCB_VibrationOn();
void zVarGiveMap();
void zVarUnlockR001();
void zVarUnlockS001();
void zVarUnlockE001();
void zVarUnlockF001();
void zVarSetArea(U32 sceneID);
void zVarSetSnackGateValue(U32 amount);
void zVarDefeatMasterMind();
char* zVMCB_PlayerScoobySnackCount_Int(char* dst);
char* zVMCB_PlayerChancesCount(char* dst);
char* zVMCB_SoundMusicVolume(char* dst);
char* zVMCB_SoundFXVolume(char* dst);
char* zVMCB_Scene(char* dst);
char* zVMCB_GameSlot0(char* dst);
char* zVMCB_GameSlot1(char* dst);
char* zVMCB_GameSlot2(char* dst);
char* zVMCB_GameSlot3(char* dst);
char* zVMCB_GameSlot4(char* dst);
char* zVMCB_GameSlot5(char* dst);
char* zVMCB_GameSlot6(char* dst);
char* zVMCB_GameSlot7(char* dst);
char* zVMCB_MCSelectedGame(char* dst);
char* zVMCB_MCSelectedCard(char* dst);
char* zVMCB_CurrentPosition(char* dst);
char* zVMCB_SnackRoomStats(char* dst);
char* zVMCB_TokenRoomStats(char* dst);
char* zVMCB_SnackAreaStats(char* dst);
char* zVMCB_TokenAreaStats(char* dst);
char* zVMCB_SnackWorldStats(char* dst);
char* zVMCB_TokenWorldStats(char* dst);
char* zVMCB_CurrentArea(char* dst);
char* zVMCB_AreaStatsSnacks(char* dst);
char* zVMCB_AreaStatsTokens(char* dst);
char* zVMCB_SelectedArea(char* dst);
char* zVMCB_SnackGateValue(char* dst);
char* zVMCB_CurrentDate(char* dst);
char* zVMCB_CurrentTime(char* dst);
char* zVMCB_NoCardMessage(char* dst);
char* zVMCB_SpaceAvailable(char* dst);
char* zVMCB_SpaceNeeded(char* dst);
char* zVMCB_FormatPromptMessage(char* dst);
char* zVMCB_MCActionMessage(char* dst);
char* zVMCB_MCACtionFailMessage(char* dst);
char* zVMCB_CancelButtonLabel(char* dst);
char* zVMCB_ActionButtonLabel(char* dst);
char* zVMCB_AltButtonLabel(char* dst);
U32 zVarEntryCB_CurrentSceneLetter();
U32 zVarEntryCB_IsSneaking();
U32 zVarEntryCB_MapItem();
U32 zVarEntryCB_UnlockedR001();
U32 zVarEntryCB_UnlockedS001();
U32 zVarEntryCB_UnlockedE001();
U32 zVarEntryCB_UnlockedF001();
U32 finishedGame();
void zVarUnlockEndGame();
void zVarUnlockAllMovies();
U32 zVarEntryCB_PuzzlePieceF();
U32 zVarEntryCB_PuzzlePieceL();
U32 zVarEntryCB_PuzzlePieceW();
U32 zVarEntryCB_PuzzlePieceE();
U32 zVarEntryCB_PuzzlePieceC();
U32 zVarEntryCB_PuzzlePieceG();
U32 zVarEntryCB_PuzzlePieceP();
U32 zVarEntryCB_PuzzlePieceB();
U32 zVarEntryCB_PuzzlePieceS();
U32 zVarEntryCB_PuzzlePieceI();
U32 zVarEntryCB_PuzzlePieceR();
U32 zVarEntryCB_PuzzlePieceO();
U32 zVarEntryCB_PuzzlePieceH();
U32 zVarEntryCB_CurrentRoom();
U32 zVarEntryCB_eVarMToken1();
U32 zVarEntryCB_eVarMToken2();
U32 zVarEntryCB_eVarMToken3();
U32 zVarEntryCB_eVarMToken4();
U32 zVarEntryCB_eVarMToken5();
U32 zVarEntryCB_eVarMToken6();
U32 zVarEntryCB_eVarMToken7();
U32 zVarEntryCB_eVarMToken8();
U32 zVarEntryCB_eVarMToken9();
U32 zVarEntryCB_eVarMToken10();
U32 zVarEntryCB_eVarMToken11();
U32 zVarEntryCB_eVarMToken12();
U32 zVarEntryCB_eVarMToken13();
U32 zVarEntryCB_eVarMToken14();
U32 zVarEntryCB_eVarMToken15();
U32 zVarEntryCB_eVarMToken16();
U32 zVarEntryCB_eVarMToken17();
U32 zVarEntryCB_eVarMToken18();
U32 zVarEntryCB_eVarMToken19();
U32 zVarEntryCB_eVarMToken20();
U32 zVarEntryCB_eVarMToken21();
U32 zVarEntryCB_eVarWarpB004();
U32 zVarEntryCB_eVarWarpC004();
U32 zVarEntryCB_eVarWarpE004();
U32 zVarEntryCB_eVarWarpE006();
U32 zVarEntryCB_eVarWarpE009();
U32 zVarEntryCB_eVarWarpF003();
U32 zVarEntryCB_eVarWarpF007();
U32 zVarEntryCB_eVarWarpF010();
U32 zVarEntryCB_eVarWarpG001();
U32 zVarEntryCB_eVarWarpG005();
U32 zVarEntryCB_eVarWarpG008();
U32 zVarEntryCB_eVarWarpH001();
U32 zVarEntryCB_eVarWarpI003();
U32 zVarEntryCB_eVarWarpI006();
U32 zVarEntryCB_eVarWarpL014();
U32 zVarEntryCB_eVarWarpL015();
U32 zVarEntryCB_eVarWarpL018();
U32 zVarEntryCB_eVarWarpO004();
U32 zVarEntryCB_eVarWarpO006();
U32 zVarEntryCB_eVarWarpP003();
U32 zVarEntryCB_eVarWarpP005();
U32 zVarEntryCB_eVarWarpR003();
U32 zVarEntryCB_eVarWarpS003();
U32 zVarEntryCB_eVarWarpW022();
U32 zVarEntryCB_eVarWarpW026();
U32 zVarEntryCB_eVarWarpH003();
U32 zVarEntryCB_eVareVarDefeatedMM();
U32 zVarEntryCB_eVarAllSnacksFound();
U32 zVarEntryCB_eVarCin1();
U32 zVarEntryCB_eVarCin2();
U32 zVarEntryCB_eVarCin3();
U32 zVarEntryCB_eVarCin4();
U32 zVarEntryCB_eVarCin5();
U32 zVarEntryCB_eVarCin6();
U32 zVarEntryCB_eVarCin7();
U32 zVarEntryCB_eVarCin8();
U32 zVarEntryCB_eVarCin9();
U32 zVarEntryCB_eVarCin10();
U32 zVarEntryCB_eVarCin11();
U32 zVarEntryCB_eVarCin12();
U32 zVarEntryCB_eVarCin13();
U32 zVarEntryCB_eVarCin14();
inline char* zVarGameSlotInfo(S32 i);

char txt[64];
S32 sVarMagicInit;

zVarEntry zVarEntryTable[108] = {
    { 0, 0, "Chances", zVarEntryCB_PCChances },
    { 1, 0, "Scooby Snacks", zVarEntryCB_PCScoobySnacks },
    { 2, 0, "Sound mode", zVarEntryCB_SndMode },
    { 3, 0, "Master Volume", zVarEntryCB_SndMasterVol },
    { 19, 0, "Music Volume", zVarEntryCB_SndMusicVol },
    { 4, 0, "SFX Volume", zVarEntryCB_SndFXVol },
    { 5, 0, "Bubble gum", zVarEntryCB_PCSpecialBubbleGum },
    { 6, 0, "Soap bar", zVarEntryCB_PCSpecialSoapBar },
    { 7, 0, "Sticky boots", zVarEntryCB_PCSpecialStickyBoots },
    { 8, 0, "Plungers", zVarEntryCB_PCSpecialPlungers },
    { 9, 0, "Slippers", zVarEntryCB_PCSpecialSlippers },
    { 10, 0, "Lamp shade", zVarEntryCB_PCSpecialLampShade },
    { 11, 0, "Black knight", zVarEntryCB_PCSpecialBlackKnight },
    { 12, 0, "Flower pos", zVarEntryCB_PCSpecialFlowerPot },
    { 13, 0, "Diving helmet", zVarEntryCB_PCSpecialDivingHelmet },
    { 14, 0, "Spring", zVarEntryCB_PCSpecialSpring },
    { 15, 0, "Lightning bolt", zVarEntryCB_PCSpecialLightningBolt },
    { 16, 0, "Football helmet", zVarEntryCB_PCSpecialFootballHelmet },
    { 17, 0, "Umbrella", zVarEntryCB_PCSpecialUmbrella },
    { 18, 0, "Shovel", zVarEntryCB_PCSpecialShovel },
    { 20, 0, "Memory Card Available", zVarEntryCB_MCAvailable },
    { 21, 0, "Vibration is on", zVarEntryCB_VibrationOn },
    { 22, 0, "Letter of scene", zVarEntryCB_CurrentSceneLetter },
    { 23, 0, "Room", zVarEntryCB_CurrentRoom },
    { 24, 0, "IsSneaking", zVarEntryCB_IsSneaking },
    { 25, 0, "Map Item Recieved", zVarEntryCB_MapItem },
    { 26, 0, "R001 Unlocked", zVarEntryCB_UnlockedR001 },
    { 27, 0, "S001 Unlocked", zVarEntryCB_UnlockedS001 },
    { 28, 0, "F Piece Collected", zVarEntryCB_PuzzlePieceF },
    { 29, 0, "L Piece Collected", zVarEntryCB_PuzzlePieceL },
    { 30, 0, "W Piece Collected", zVarEntryCB_PuzzlePieceW },
    { 31, 0, "E Piece Collected", zVarEntryCB_PuzzlePieceE },
    { 32, 0, "C Piece Collected", zVarEntryCB_PuzzlePieceC },
    { 33, 0, "G Piece Collected", zVarEntryCB_PuzzlePieceG },
    { 34, 0, "P Piece Collected", zVarEntryCB_PuzzlePieceP },
    { 35, 0, "B Piece Collected", zVarEntryCB_PuzzlePieceB },
    { 36, 0, "S Piece Collected", zVarEntryCB_PuzzlePieceS },
    { 37, 0, "I Piece Collected", zVarEntryCB_PuzzlePieceI },
    { 38, 0, "R Piece Collected", zVarEntryCB_PuzzlePieceR },
    { 39, 0, "O Piece Collected", zVarEntryCB_PuzzlePieceO },
    { 40, 0, "H Piece Collected", zVarEntryCB_PuzzlePieceH },
    { 41, 0, "F001 Unlocked", zVarEntryCB_UnlockedF001 },
    { 42, 0, "E001 Unlocked", zVarEntryCB_UnlockedE001 },
    { 43, 0, "Monster Token 1", zVarEntryCB_eVarMToken1 },
    { 44, 0, "Monster Token 2", zVarEntryCB_eVarMToken2 },
    { 45, 0, "Monster Token 3", zVarEntryCB_eVarMToken3 },
    { 46, 0, "Monster Token 4", zVarEntryCB_eVarMToken4 },
    { 47, 0, "Monster Token 5", zVarEntryCB_eVarMToken5 },
    { 48, 0, "Monster Token 6", zVarEntryCB_eVarMToken6 },
    { 49, 0, "Monster Token 7", zVarEntryCB_eVarMToken7 },
    { 50, 0, "Monster Token 8", zVarEntryCB_eVarMToken8 },
    { 51, 0, "Monster Token 9", zVarEntryCB_eVarMToken9 },
    { 52, 0, "Monster Token 10", zVarEntryCB_eVarMToken10 },
    { 53, 0, "Monster Token 11", zVarEntryCB_eVarMToken11 },
    { 54, 0, "Monster Token 12", zVarEntryCB_eVarMToken12 },
    { 55, 0, "Monster Token 13", zVarEntryCB_eVarMToken13 },
    { 56, 0, "Monster Token 14", zVarEntryCB_eVarMToken14 },
    { 57, 0, "Monster Token 15", zVarEntryCB_eVarMToken15 },
    { 58, 0, "Monster Token 16", zVarEntryCB_eVarMToken16 },
    { 59, 0, "Monster Token 17", zVarEntryCB_eVarMToken17 },
    { 60, 0, "Monster Token 18", zVarEntryCB_eVarMToken18 },
    { 61, 0, "Monster Token 19", zVarEntryCB_eVarMToken19 },
    { 62, 0, "Monster Token 20", zVarEntryCB_eVarMToken20 },
    { 63, 0, "Monster Token 21", zVarEntryCB_eVarMToken21 },
    { 64, 0, "Warp Point B4", zVarEntryCB_eVarWarpB004 },
    { 65, 0, "Warp Point C4", zVarEntryCB_eVarWarpC004 },
    { 66, 0, "Warp Point E4", zVarEntryCB_eVarWarpE004 },
    { 67, 0, "Warp Point E6", zVarEntryCB_eVarWarpE006 },
    { 68, 0, "Warp Point E9", zVarEntryCB_eVarWarpE009 },
    { 69, 0, "Warp Point F3", zVarEntryCB_eVarWarpF003 },
    { 70, 0, "Warp Point F7", zVarEntryCB_eVarWarpF007 },
    { 71, 0, "Warp Point F10", zVarEntryCB_eVarWarpF010 },
    { 88, 0, "Warp Point G1", zVarEntryCB_eVarWarpG001 },
    { 72, 0, "Warp Point G5", zVarEntryCB_eVarWarpG005 },
    { 73, 0, "Warp Point G8", zVarEntryCB_eVarWarpG008 },
    { 74, 0, "Warp Point H1", zVarEntryCB_eVarWarpH001 },
    { 75, 0, "Warp Point I3", zVarEntryCB_eVarWarpI003 },
    { 76, 0, "Warp Point I6", zVarEntryCB_eVarWarpI006 },
    { 77, 0, "Warp Point L14", zVarEntryCB_eVarWarpL014 },
    { 87, 0, "Warp Point L15", zVarEntryCB_eVarWarpL015 },
    { 78, 0, "Warp Point L18", zVarEntryCB_eVarWarpL018 },
    { 79, 0, "Warp Point O4", zVarEntryCB_eVarWarpO004 },
    { 80, 0, "Warp Point O6", zVarEntryCB_eVarWarpO006 },
    { 81, 0, "Warp Point P3", zVarEntryCB_eVarWarpP003 },
    { 82, 0, "Warp Point P5", zVarEntryCB_eVarWarpP005 },
    { 83, 0, "Warp Point R3", zVarEntryCB_eVarWarpR003 },
    { 84, 0, "Warp Point S3", zVarEntryCB_eVarWarpS003 },
    { 85, 0, "Warp Point W22", zVarEntryCB_eVarWarpW022 },
    { 86, 0, "Warp Point W26", zVarEntryCB_eVarWarpW026 },
    { 89, 0, "Lightning bolt stun", zVarEntryCB_PCSpecialLightningBoltStun },
    { 90, 0, "Defeated Mastermind", zVarEntryCB_eVareVarDefeatedMM },
    { 91, 0, "Warp Point H3", zVarEntryCB_eVarWarpH003 },
    { 92, 0, "AllSnacksBonus open", zVarEntryCB_eVarAllSnacksFound },
    { 93, 0, "Cinematic 1 Unlocked", zVarEntryCB_eVarCin1 },
    { 94, 0, "Cinematic 2 Unlocked", zVarEntryCB_eVarCin2 },
    { 95, 0, "Cinematic 3 Unlocked", zVarEntryCB_eVarCin3 },
    { 96, 0, "Cinematic 4 Unlocked", zVarEntryCB_eVarCin4 },
    { 97, 0, "Cinematic 5 Unlocked", zVarEntryCB_eVarCin5 },
    { 98, 0, "Cinematic 6 Unlocked", zVarEntryCB_eVarCin6 },
    { 99, 0, "Cinematic 7 Unlocked", zVarEntryCB_eVarCin7 },
    { 100, 0, "Cinematic 8 Unlocked", zVarEntryCB_eVarCin8 },
    { 101, 0, "Cinematic 9 Unlocked", zVarEntryCB_eVarCin9 },
    { 102, 0, "Cinematic 10 Unlocked", zVarEntryCB_eVarCin10 },
    { 103, 0, "Cinematic 11 Unlocked", zVarEntryCB_eVarCin11 },
    { 104, 0, "Cinematic 12 Unlocked", zVarEntryCB_eVarCin12 },
    { 105, 0, "Cinematic 13 Unlocked", zVarEntryCB_eVarCin13 },
    { 106, 0, "Cinematic 14 Unlocked", zVarEntryCB_eVarCin14 },
    { 0, 0, NULL, NULL },
};

zVarMagicEntry sVarMagicEntryTrable[39] = {
    { 0, "PlayerChancesCount", zVMCB_PlayerChancesCount },
    { 0, "PlayerScoobySnackCount_Int", zVMCB_PlayerScoobySnackCount_Int },
    { 0, "SoundMusicVolume", zVMCB_SoundMusicVolume },
    { 0, "SoundFXVolume", zVMCB_SoundFXVolume },
    { 0, "CurrentScene", zVMCB_Scene },
    { 0, "GameSlot0", zVMCB_GameSlot0 },
    { 0, "GameSlot1", zVMCB_GameSlot1 },
    { 0, "GameSlot2", zVMCB_GameSlot2 },
    { 0, "GameSlot3", zVMCB_GameSlot3 },
    { 0, "GameSlot4", zVMCB_GameSlot4 },
    { 0, "GameSlot5", zVMCB_GameSlot5 },
    { 0, "GameSlot6", zVMCB_GameSlot6 },
    { 0, "GameSlot7", zVMCB_GameSlot7 },
    { 0, "MCSelectedGame", zVMCB_MCSelectedGame },
    { 0, "MCSelectedCard", zVMCB_MCSelectedCard },
    { 0, "PlayerPosition", zVMCB_CurrentPosition },
    { 0, "SnackRoomStats", zVMCB_SnackRoomStats },
    { 0, "TokenRoomStats", zVMCB_TokenRoomStats },
    { 0, "SnackAreaStats", zVMCB_SnackAreaStats },
    { 0, "TokenAreaStats", zVMCB_TokenAreaStats },
    { 0, "SnackWorldStats", zVMCB_SnackWorldStats },
    { 0, "TokenWorldStats", zVMCB_TokenWorldStats },
    { 0, "CurrentArea", zVMCB_CurrentArea },
    { 0, "AreaStatsSnacks", zVMCB_AreaStatsSnacks },
    { 0, "AreaStatsTokens", zVMCB_AreaStatsTokens },
    { 0, "SelectedArea", zVMCB_SelectedArea },
    { 0, "SnackGateValue", zVMCB_SnackGateValue },
    { 0, "CurrentDate", zVMCB_CurrentDate },
    { 0, "CurrentTime", zVMCB_CurrentTime },
    { 0, "NoCardMessage", zVMCB_NoCardMessage },
    { 0, "SpaceAvailable", zVMCB_SpaceAvailable },
    { 0, "SpaceNeeded", zVMCB_SpaceNeeded },
    { 0, "FormatPromptMessage", zVMCB_FormatPromptMessage },
    { 0, "MCActionMessage", zVMCB_MCActionMessage },
    { 0, "MCACtionFailMessage", zVMCB_MCACtionFailMessage },
    { 0, "CancelButtonLabel", zVMCB_CancelButtonLabel },
    { 0, "ActionButtonLabel", zVMCB_ActionButtonLabel },
    { 0, "AltButtonLabel", zVMCB_AltButtonLabel },
    { 0, NULL, NULL },
};

static U32 sSnackGateValue;
static U32 selSceneID;
static U32 allmovies;
static U32 endgame;
static U32 MMDefeated;
static U32 F001Unlocked;
static U32 E001Unlocked;
static U32 S001Unlocked;
static U32 R001Unlocked;
static U32 hasMap;

// Upper-cases a scene letter.
static inline S32 zVarToUpper(S32 c)
{
    bool lower = false;

    if (c >= 'a' && c <= 'z')
    {
        lower = true;
    }
    if (lower)
    {
        c -= 'a' - 'A';
    }

    return c;
}

// Copies src to dst, replacing each "$V<name>$" with the text its magic-variable callback writes.
void zVarMagicProcessText(char* src, char* dst)
{
    zVarMagicEntry* e;
    U32 mvID;
    char* mv;
    char magicVar[256];

    if (!sVarMagicInit)
    {
        for (e = sVarMagicEntryTrable; e->varName; e++)
        {
            e->varNameID = xStrHash(e->varName);
        }
        sVarMagicInit = 1;
    }

    while (*src)
    {
        if (src[0] == '$' && src[1] == 'V')
        {
            mv = magicVar;
            src += 2;
            while (*src && *src != '$')
            {
                *mv++ = *src++;
            }
            *mv = '\0';
            if (*src == '$')
            {
                src++;
            }

            mvID = xStrHash(magicVar);
            for (e = sVarMagicEntryTrable; e->varName; e++)
            {
                if (e->varNameID == mvID)
                {
                    dst = e->varCB(dst);
                    break;
                }
            }
        }
        else
        {
            *dst++ = *src++;
        }
    }

    *dst = '\0';
}

void zVarInit(zVarEntry* table)
{
    for (S32 i = 0; i < 107; i++)
    {
        table[i].varNameID = xStrHash(table[i].varName);
    }
}

void zVarNewGame()
{
    hasMap = 0;
    R001Unlocked = 0;
    S001Unlocked = 0;
    E001Unlocked = 0;
    F001Unlocked = 0;
    MMDefeated = 0;
    allmovies = 0;
}

void zVarSave(xSerial* s)
{
    s->Write_b1(hasMap != 0);
    s->Write_b1(R001Unlocked != 0);
    s->Write_b1(S001Unlocked != 0);
    s->Write_b1(E001Unlocked != 0);
    s->Write_b1(F001Unlocked != 0);
    s->Write_b1(MMDefeated != 0);
}

void zVarLoad(xSerial* s)
{
    s->Read_b1(&hasMap);
    s->Read_b1(&R001Unlocked);
    s->Read_b1(&S001Unlocked);
    s->Read_b1(&E001Unlocked);
    s->Read_b1(&F001Unlocked);
    s->Read_b1(&MMDefeated);
}

U32 zVarEntryCB_PCChances()
{
    return globals.player.Inv_Chances;
}

U32 zVarEntryCB_PCScoobySnacks()
{
    return globals.player.Inv_ScoobySnacks;
}

U32 zVarEntryCB_SndMode()
{
    return iSndGetStereo();
}

// A volume as the 0-10 menu setting: the average of the left and right levels, rounded.
U32 zVarEntryCB_SndMasterVol()
{
    F64 vol = (gSnd.mastervol.volL + gSnd.mastervol.volR) * 0.5f * 10.0;

    return (U32)(0.5 + vol);
}

U32 zVarEntryCB_SndMusicVol()
{
    F64 vol = (gSnd.streamvol.volL + gSnd.streamvol.volR) * 0.5f * 10.0;

    return (U32)(0.5 + vol);
}

U32 zVarEntryCB_SndFXVol()
{
    F64 vol = (gSnd.fxvol.volL + gSnd.fxvol.volR) * 0.5f * 10.0;

    return (U32)(0.5 + vol);
}

// One bit per special power-up collected.
U32 zVarEntryCB_PCSpecialBubbleGum()
{
    return (globals.player.Inv_Specials >> 0) & 1;
}

U32 zVarEntryCB_PCSpecialSoapBar()
{
    return (globals.player.Inv_Specials >> 1) & 1;
}

U32 zVarEntryCB_PCSpecialStickyBoots()
{
    return (globals.player.Inv_Specials >> 2) & 1;
}

U32 zVarEntryCB_PCSpecialPlungers()
{
    return (globals.player.Inv_Specials >> 3) & 1;
}

U32 zVarEntryCB_PCSpecialSlippers()
{
    return (globals.player.Inv_Specials >> 4) & 1;
}

U32 zVarEntryCB_PCSpecialLampShade()
{
    return (globals.player.Inv_Specials >> 5) & 1;
}

U32 zVarEntryCB_PCSpecialBlackKnight()
{
    return (globals.player.Inv_Specials >> 6) & 1;
}

U32 zVarEntryCB_PCSpecialFlowerPot()
{
    return (globals.player.Inv_Specials >> 7) & 1;
}

U32 zVarEntryCB_PCSpecialDivingHelmet()
{
    return (globals.player.Inv_Specials >> 8) & 1;
}

U32 zVarEntryCB_PCSpecialSpring()
{
    return (globals.player.Inv_Specials >> 9) & 1;
}

U32 zVarEntryCB_PCSpecialLightningBolt()
{
    return (globals.player.Inv_Specials >> 10) & 1;
}

U32 zVarEntryCB_PCSpecialFootballHelmet()
{
    return (globals.player.Inv_Specials >> 11) & 1;
}

U32 zVarEntryCB_PCSpecialUmbrella()
{
    return (globals.player.Inv_Specials >> 12) & 1;
}

U32 zVarEntryCB_PCSpecialShovel()
{
    return (globals.player.Inv_Specials >> 13) & 1;
}

U32 zVarEntryCB_PCSpecialLightningBoltStun()
{
    return (globals.player.Inv_Specials >> 14) & 1;
}

// Which of the first two save slots hold a game: bit 0 for slot 0, bit 1 for slot 1.
U32 zVarEntryCB_MCAvailable()
{
    if (xSGGameIsEmpty(NULL, 0) == 1)
    {
        if (xSGGameIsEmpty(NULL, 1) == 1)
        {
            return 0;
        }
        if (xSGGameIsEmpty(NULL, 1) == 0)
        {
            return 2;
        }
    }
    else if (xSGGameIsEmpty(NULL, 0) == 0)
    {
        if (xSGGameIsEmpty(NULL, 1) == 1)
        {
            return 1;
        }
        if (xSGGameIsEmpty(NULL, 1) == 0)
        {
            return 3;
        }
    }

    return 0;
}

U32 zVarEntryCB_VibrationOn()
{
    return globals.option_vibration & 1;
}

// The save slot's display name: "Empty", or the name zSaveLoad_BuildName builds. The date and
// 12-hour time it formats first go unused.
inline char* zVarGameSlotInfo(S32 i)
{
    char date[32] = {};
    char time[32] = {};
    char ampm[32] = {};
    char buf[256] = {};
    S32 hour;

    strncpy(date, zSaveLoadGameTable[i].date, 5);
    date[2] = '/';
    sprintf(buf, "%s/%c%c", date, zSaveLoadGameTable[i].date[8], zSaveLoadGameTable[i].date[9]);
    strncpy(date, buf, 32);
    date[31] = '\0';

    sprintf(time, "%c%c", zSaveLoadGameTable[i].date[11], zSaveLoadGameTable[i].date[12]);
    hour = atoi(time);
    if (hour >= 12)
    {
        strncpy(ampm, "PM", 3);
        if (hour != 12)
        {
            hour -= 12;
        }
    }
    else
    {
        strncpy(ampm, "AM", 3);
        if (hour == 0)
        {
            hour = 12;
        }
    }

    sprintf(time, "%d:%c%c %s", hour, zSaveLoadGameTable[i].date[14], zSaveLoadGameTable[i].date[15], ampm);
    sprintf(date, "%s %s", date, time);
    time[31] = '\0';
    date[31] = '\0';

    if (xStricmp(zSaveLoadGameTable[i].label, "Empty") == 0)
    {
        sprintf(txt, "Empty");
    }
    else
    {
        zSaveLoad_BuildName(txt, i);
    }

    return txt;
}

void zVarGiveMap()
{
    hasMap = 1;
}

void zVarUnlockR001()
{
    R001Unlocked = 1;
}

void zVarUnlockS001()
{
    S001Unlocked = 1;
}

void zVarUnlockE001()
{
    E001Unlocked = 1;
}

void zVarUnlockF001()
{
    F001Unlocked = 1;
}

void zVarSetArea(U32 sceneID)
{
    selSceneID = sceneID;
}

void zVarSetSnackGateValue(U32 amount)
{
    sSnackGateValue = amount;
}

void zVarDefeatMasterMind()
{
    MMDefeated = 1;
    endgame = 1;
}

char* zVMCB_PlayerScoobySnackCount_Int(char* dst)
{
    return dst + sprintf(dst, "%d", globals.player.Inv_ScoobySnacks);
}

char* zVMCB_PlayerChancesCount(char* dst)
{
    return dst + sprintf(dst, "%d", globals.player.Inv_Chances);
}

char* zVMCB_SoundMusicVolume(char* dst)
{
    F64 vol = (gSnd.streamvol.volL + gSnd.streamvol.volR) * 0.5f * 10.0;

    return dst + sprintf(dst, "%d", (U32)(0.5 + vol));
}

char* zVMCB_SoundFXVolume(char* dst)
{
    F64 vol = (gSnd.fxvol.volL + gSnd.fxvol.volR) * 0.5f * 10.0;

    return dst + sprintf(dst, "%d", (U32)(0.5 + vol));
}

char* zVMCB_Scene(char* dst)
{
    return dst + sprintf(dst, "%s", xUtil_idtag2string(globals.sceneCur->sceneID, 0));
}

// The text for each save slot; only slots 0 to 2 have entries in zSaveLoadGameTable.
char* zVMCB_GameSlot0(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(0));
}

char* zVMCB_GameSlot1(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(1));
}

char* zVMCB_GameSlot2(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(2));
}

char* zVMCB_GameSlot3(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(3));
}

char* zVMCB_GameSlot4(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(4));
}

char* zVMCB_GameSlot5(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(5));
}

char* zVMCB_GameSlot6(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(6));
}

char* zVMCB_GameSlot7(char* dst)
{
    return dst + sprintf(dst, "%s", zVarGameSlotInfo(7));
}

char* zVMCB_MCSelectedGame(char* dst)
{
    return dst + sprintf(dst, "%d", zSaveLoad_getgame() + 1);
}

char* zVMCB_MCSelectedCard(char* dst)
{
    S32 count = 0;

    switch (zSaveLoad_getcard())
    {
    case 0:
        count = sprintf(dst, "%s", "A");
        break;
    case 1:
        count = sprintf(dst, "%s", "B");
        break;
    }

    return dst + count;
}

char* zVMCB_CurrentPosition(char* dst)
{
    xVec3* pos = &globals.player.ent.frame->mat.pos;

    return dst + sprintf(dst, "%d,%d,%d", (S32)(0.5f + pos->x), (S32)(0.5f + pos->y), (S32)(0.5f + pos->z));
}

// Stats lines that print nothing.
char* zVMCB_SnackRoomStats(char* dst)
{
    return dst;
}

char* zVMCB_TokenRoomStats(char* dst)
{
    return dst;
}

char* zVMCB_SnackAreaStats(char* dst)
{
    return dst;
}

char* zVMCB_TokenAreaStats(char* dst)
{
    return dst;
}

char* zVMCB_SnackWorldStats(char* dst)
{
    return dst;
}

char* zVMCB_TokenWorldStats(char* dst)
{
    return dst;
}

char* zVMCB_CurrentArea(char* dst)
{
    return dst + sprintf(dst, "%s", zSceneGetAreaname(globals.sceneCur->sceneID));
}

char* zVMCB_AreaStatsSnacks(char* dst)
{
    // The hub has no area totals.
    if (selSceneID == 'H003')
    {
        return dst;
    }

    return dst + sprintf(dst, "%d/%d", zSceneGetAreaSnacksCollected(selSceneID), zSceneGetAreaTotalSnacks(selSceneID));
}

char* zVMCB_AreaStatsTokens(char* dst)
{
    // The hub has no area totals.
    if (selSceneID == 'H003')
    {
        return dst;
    }

    return dst + sprintf(dst, "%d/%d", zSceneGetAreaTokensCollected(selSceneID), zSceneGetAreaTotalTokens(selSceneID));
}

char* zVMCB_SelectedArea(char* dst)
{
    return dst + sprintf(dst, "%s", zSceneGetAreaname(selSceneID));
}

char* zVMCB_SnackGateValue(char* dst)
{
    return dst + sprintf(dst, "%d", sSnackGateValue);
}

char* zVMCB_CurrentDate(char* dst)
{
    return dst + iGetCurrFormattedDate(dst);
}

char* zVMCB_CurrentTime(char* dst)
{
    return dst + iGetCurrFormattedTime(dst);
}

// Memory-card messages and button labels, filled in by the save/load code.
char* zVMCB_NoCardMessage(char* dst)
{
    return dst + fn_800C9710(dst);
}

char* zVMCB_SpaceAvailable(char* dst)
{
    return dst + sprintf(dst, " ");
}

char* zVMCB_SpaceNeeded(char* dst)
{
    return dst + fn_800C9710(dst);
}

char* zVMCB_FormatPromptMessage(char* dst)
{
    return dst + fn_800C9710(dst);
}

char* zVMCB_MCActionMessage(char* dst)
{
    return dst + fn_800C9710(dst);
}

char* zVMCB_MCACtionFailMessage(char* dst)
{
    return dst + fn_800C9710(dst);
}

char* zVMCB_CancelButtonLabel(char* dst)
{
    return dst + fn_800C96E8(dst);
}

char* zVMCB_ActionButtonLabel(char* dst)
{
    return dst + fn_800C96C0(dst);
}

char* zVMCB_AltButtonLabel(char* dst)
{
    return dst + fn_800C9698(dst);
}

// The current scene letter's place in the alphabet (A is 1).
U32 zVarEntryCB_CurrentSceneLetter()
{
    char buf[16];

    sprintf(buf, "%s", xUtil_idtag2string(globals.sceneCur->sceneID, 0));

    return zVarToUpper(buf[0]) - '@';
}

U32 zVarEntryCB_IsSneaking()
{
    bool sneaking = false;

    if (globals.player.Sneak &&
        (globals.player.ent.model->Anim->Single->State->UserFlags & 0x1E) == 2)
    {
        sneaking = true;
    }

    return sneaking;
}

U32 zVarEntryCB_MapItem()
{
    return hasMap;
}

U32 zVarEntryCB_UnlockedR001()
{
    return R001Unlocked;
}

U32 zVarEntryCB_UnlockedS001()
{
    return S001Unlocked;
}

U32 zVarEntryCB_UnlockedE001()
{
    return E001Unlocked;
}

U32 zVarEntryCB_UnlockedF001()
{
    return F001Unlocked;
}

U32 finishedGame()
{
    return endgame;
}

void zVarUnlockEndGame()
{
    endgame = 1;
}

void zVarUnlockAllMovies()
{
    allmovies = 1;
}

// A level's puzzle piece is earned once all of its clues are found.
U32 zVarEntryCB_PuzzlePieceF()
{
    if (globals.levelStats[0].clues >= globals.levelStats[0].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceL()
{
    if (globals.levelStats[1].clues >= globals.levelStats[1].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceW()
{
    if (globals.levelStats[2].clues >= globals.levelStats[2].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceE()
{
    if (globals.levelStats[3].clues >= globals.levelStats[3].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceC()
{
    if (globals.levelStats[4].clues >= globals.levelStats[4].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceG()
{
    if (globals.levelStats[5].clues >= globals.levelStats[5].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceP()
{
    if (globals.levelStats[6].clues >= globals.levelStats[6].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceB()
{
    if (globals.levelStats[7].clues >= globals.levelStats[7].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceS()
{
    if (globals.levelStats[8].clues >= globals.levelStats[8].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceI()
{
    if (globals.levelStats[9].clues >= globals.levelStats[9].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceR()
{
    if (globals.levelStats[10].clues >= globals.levelStats[10].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceO()
{
    if (globals.levelStats[11].clues >= globals.levelStats[11].clues_tot)
    {
        return 1;
    }

    return 0;
}

U32 zVarEntryCB_PuzzlePieceH()
{
    if (globals.levelStats[12].clues >= globals.levelStats[12].clues_tot)
    {
        return 1;
    }

    return 0;
}

// The current room as a number: the scene letter's place in the alphabet times 100, plus the
// scene's two-digit number (H003 is 803).
U32 zVarEntryCB_CurrentRoom()
{
    char buf[16];

    sprintf(buf, "%s", xUtil_idtag2string(globals.sceneCur->sceneID, 0));
    S32 room = (buf[2] - '0') * 10 + (buf[3] - '0');

    return room + (zVarToUpper(buf[0]) - '@') * 100;
}

// One bit per monster token collected.
U32 zVarEntryCB_eVarMToken1()
{
    return (globals.player.Inv_Mtokens[0] >> 0) & 1;
}

U32 zVarEntryCB_eVarMToken2()
{
    return (globals.player.Inv_Mtokens[0] >> 1) & 1;
}

U32 zVarEntryCB_eVarMToken3()
{
    return (globals.player.Inv_Mtokens[0] >> 2) & 1;
}

U32 zVarEntryCB_eVarMToken4()
{
    return (globals.player.Inv_Mtokens[0] >> 3) & 1;
}

U32 zVarEntryCB_eVarMToken5()
{
    return (globals.player.Inv_Mtokens[0] >> 4) & 1;
}

U32 zVarEntryCB_eVarMToken6()
{
    return (globals.player.Inv_Mtokens[0] >> 5) & 1;
}

U32 zVarEntryCB_eVarMToken7()
{
    return (globals.player.Inv_Mtokens[0] >> 6) & 1;
}

U32 zVarEntryCB_eVarMToken8()
{
    return (globals.player.Inv_Mtokens[0] >> 7) & 1;
}

U32 zVarEntryCB_eVarMToken9()
{
    return (globals.player.Inv_Mtokens[0] >> 8) & 1;
}

U32 zVarEntryCB_eVarMToken10()
{
    return (globals.player.Inv_Mtokens[0] >> 9) & 1;
}

U32 zVarEntryCB_eVarMToken11()
{
    return (globals.player.Inv_Mtokens[0] >> 10) & 1;
}

U32 zVarEntryCB_eVarMToken12()
{
    return (globals.player.Inv_Mtokens[0] >> 11) & 1;
}

U32 zVarEntryCB_eVarMToken13()
{
    return (globals.player.Inv_Mtokens[0] >> 12) & 1;
}

U32 zVarEntryCB_eVarMToken14()
{
    return (globals.player.Inv_Mtokens[0] >> 13) & 1;
}

U32 zVarEntryCB_eVarMToken15()
{
    return (globals.player.Inv_Mtokens[0] >> 14) & 1;
}

U32 zVarEntryCB_eVarMToken16()
{
    return (globals.player.Inv_Mtokens[0] >> 15) & 1;
}

U32 zVarEntryCB_eVarMToken17()
{
    return (globals.player.Inv_Mtokens[0] >> 16) & 1;
}

U32 zVarEntryCB_eVarMToken18()
{
    return (globals.player.Inv_Mtokens[0] >> 17) & 1;
}

U32 zVarEntryCB_eVarMToken19()
{
    return (globals.player.Inv_Mtokens[0] >> 18) & 1;
}

U32 zVarEntryCB_eVarMToken20()
{
    return (globals.player.Inv_Mtokens[0] >> 19) & 1;
}

U32 zVarEntryCB_eVarMToken21()
{
    return (globals.player.Inv_Mtokens[0] >> 20) & 1;
}

// Whether each room's warp point is open.
U32 zVarEntryCB_eVarWarpB004()
{
    return zEntPickup_WarpIsOpen(0);
}

U32 zVarEntryCB_eVarWarpC004()
{
    return zEntPickup_WarpIsOpen(1);
}

U32 zVarEntryCB_eVarWarpE004()
{
    return zEntPickup_WarpIsOpen(2);
}

U32 zVarEntryCB_eVarWarpE006()
{
    return zEntPickup_WarpIsOpen(3);
}

U32 zVarEntryCB_eVarWarpE009()
{
    return zEntPickup_WarpIsOpen(4);
}

U32 zVarEntryCB_eVarWarpF003()
{
    return zEntPickup_WarpIsOpen(5);
}

U32 zVarEntryCB_eVarWarpF007()
{
    return zEntPickup_WarpIsOpen(6);
}

U32 zVarEntryCB_eVarWarpF010()
{
    return zEntPickup_WarpIsOpen(7);
}

U32 zVarEntryCB_eVarWarpG001()
{
    return zEntPickup_WarpIsOpen(24);
}

U32 zVarEntryCB_eVarWarpG005()
{
    return zEntPickup_WarpIsOpen(8);
}

U32 zVarEntryCB_eVarWarpG008()
{
    return zEntPickup_WarpIsOpen(9);
}

U32 zVarEntryCB_eVarWarpH001()
{
    return zEntPickup_WarpIsOpen(10);
}

U32 zVarEntryCB_eVarWarpI003()
{
    return zEntPickup_WarpIsOpen(11);
}

U32 zVarEntryCB_eVarWarpI006()
{
    return zEntPickup_WarpIsOpen(12);
}

U32 zVarEntryCB_eVarWarpL014()
{
    return zEntPickup_WarpIsOpen(13);
}

U32 zVarEntryCB_eVarWarpL015()
{
    return zEntPickup_WarpIsOpen(23);
}

U32 zVarEntryCB_eVarWarpL018()
{
    return zEntPickup_WarpIsOpen(14);
}

U32 zVarEntryCB_eVarWarpO004()
{
    return zEntPickup_WarpIsOpen(15);
}

U32 zVarEntryCB_eVarWarpO006()
{
    return zEntPickup_WarpIsOpen(16);
}

U32 zVarEntryCB_eVarWarpP003()
{
    return zEntPickup_WarpIsOpen(17);
}

U32 zVarEntryCB_eVarWarpP005()
{
    return zEntPickup_WarpIsOpen(18);
}

U32 zVarEntryCB_eVarWarpR003()
{
    return zEntPickup_WarpIsOpen(19);
}

U32 zVarEntryCB_eVarWarpS003()
{
    return zEntPickup_WarpIsOpen(20);
}

U32 zVarEntryCB_eVarWarpW022()
{
    return zEntPickup_WarpIsOpen(21);
}

U32 zVarEntryCB_eVarWarpW026()
{
    return zEntPickup_WarpIsOpen(22);
}

U32 zVarEntryCB_eVarWarpH003()
{
    return zEntPickup_WarpIsOpen(25);
}

U32 zVarEntryCB_eVareVarDefeatedMM()
{
    return MMDefeated;
}

// The number of areas whose snack total has been reached.
U32 zVarEntryCB_eVarAllSnacksFound()
{
    U32 count = 0;

    for (S32 i = 0; i < 30; i++)
    {
        if (globals.areaStats[i].snacks_tot >= zSceneGetAreaTotalSnacks((_tagSceneAreas)i))
        {
            count++;
        }
    }

    return count;
}

// A cutscene counts as unlocked once all movies are unlocked or once the cutscene table marks it seen.
U32 zVarEntryCB_eVarCin1()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(1);
}

U32 zVarEntryCB_eVarCin2()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(2);
}

U32 zVarEntryCB_eVarCin3()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(3);
}

U32 zVarEntryCB_eVarCin4()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(4);
}

U32 zVarEntryCB_eVarCin5()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(5);
}

U32 zVarEntryCB_eVarCin6()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(6);
}

U32 zVarEntryCB_eVarCin7()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(7);
}

U32 zVarEntryCB_eVarCin8()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(8);
}

U32 zVarEntryCB_eVarCin9()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(9);
}

U32 zVarEntryCB_eVarCin10()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(10);
}

// Cutscene 11 is the ending, unlocked by beating the Mastermind.
U32 zVarEntryCB_eVarCin11()
{
    return MMDefeated;
}

U32 zVarEntryCB_eVarCin12()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(12);
}

U32 zVarEntryCB_eVarCin13()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(13);
}

U32 zVarEntryCB_eVarCin14()
{
    if (allmovies)
    {
        return 1;
    }

    return zCutSceneNamesTable_getStatus(14);
}
