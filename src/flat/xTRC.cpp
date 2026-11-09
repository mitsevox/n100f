#include "xTRC.h"
#include "iCamera.h"
#include "iSystem.h"
#include "isavegame.h"
#include "xColor.h"
#include "xFont.h"
#include "xPad.h"
#include "xsavegame.h"
#include "zCutsceneMgr.h"
#include "zGameState.h"
#include "zGlobals.h"
#include "zMenu.h"

#include <stdio.h>
#include <string.h>

// The GameCube system messages: the memory-card dialogs and the pad and disc messages.

// One memory-card message: what it says (a case of xTRCCardMessageText), the card and slot it names
// and the counts the space messages print. Index 0 is the start-up check's, 1 zSaveLoad's.
struct TRCCardMessage
{
    S32 flags; // 1 names a card, 2 message set, 4 space counts set, 8 file index set
    S32 message;
    S32 card;
    S32 slot;
    S32 available;
    S32 needed;
    S32 blockSize;
    S32 freeFiles;
    S32 needFile;
    S32 fileIndex;
};

// The card a message box's callback watches: the box closes when the card in that slot changes.
struct TRCCardWatch
{
    S32 card;
    S32 slot;
    st_ISGSESSION* session;
};

typedef S32 (*TRCCardCheck)(TRCCardWatch* watch);

// The yellow of every message's text. Revision 1 keeps it as one constant.
#if VERSION_GIHE78_REV0
#define TRC_TEXT_COLOR xColorFromRGBA(255, 230, 0, 255)
#elif VERSION_GIHE78_REV1
static const iColor_tag sTrcTextColor = { 255, 230, 0, 255 };
#define TRC_TEXT_COLOR sTrcTextColor
#endif

void zGamePauseIfPossible();
extern F32 gFontWiggleTime;

void iSystemPollResetAndDisc();
void iSystemReset(S32 toMenu);
U32 zGameNickCamActive();

// The localized device, slot and card names.
char* xSGTgtCardName(st_XSAVEGAME_DATA* xsgdata, S32 tidx, en_XSG_WHYFAIL whyFail);

extern "C" {
// The two message records.
TRCCardMessage sTrcCardMessage[2] = {};
}

// The disc state and whether the disc was identified (iSnd and iTime read it).
_tagTRCState gTrcDisk[2];

// The start-up check's busy flag (zScene reads it), the font box of the pad and disc message
// (sTrcFontBox in the SLUS-20349 DWARF) and the flag that makes that message draw a frame of its
// own.
extern "C" {
U32 sTrcCardCheckActive;
xFontBox* sTrcFontBox;
U32 sTrcOwnFrame;
}

// The four pad records.
_tagTRCPadInfo gTrcPad[4];

S32 xTRCRenderState(_tagTRCState state);
void xTRCPad(S32 pad_id, _tagTRCState state);
void xTRCCardCheckStartup();
S32 xTRCCardWatchChanged(TRCCardWatch* watch);
S32 xTRCCardDialogHook(S32 card, st_ISGSESSION* session);
S32 xTRCCardCheckInserted(S32 card);
S32 xTRCCardCheckAny();
S32 xTRCCardCheckUsable(S32 card);
S32 xTRCCardCheckFormat(S32 card, S32* formatted);
S32 xTRCCardCheckSaveFile(S32 card);
S32 xTRCCardCheckSpace(S32 card);
S32 xTRCCardCheckFiles(S32 card);
S32 xTRCMessageBox(char* text, const char** options, TRCCardCheck check, TRCCardWatch* watch);
void xTRCCardMessageClear(S32 mode);
void xTRCCardMessageSetCard(S32 mode, S32 card, S32 slot);
void xTRCCardMessageSet(S32 mode, S32 message);
#if VERSION_GIHE78_REV0
void xTRCCardMessageSetSpace(S32 mode, S32 available, S32 needed, S32 blockSize, S32 freeFiles, S32 needFile);
#elif VERSION_GIHE78_REV1
static void xTRCCardMessageSetSpace(S32 mode, S32 available, S32 needed, S32 blockSize, S32 freeFiles, S32 needFile);
#endif
void xTRCCardMessageSetFile(S32 mode, S32 value);
S32 xTRCCardMessageButtonB(S32 mode, char* dst);
S32 xTRCCardMessageButtonX(S32 mode, char* dst);
S32 xTRCCardMessageButtonY(S32 mode, char* dst);
S32 xTRCCardMessageText(S32 mode, char* dst);
S32 xTRCCardCheckSlot(S32 card, S32 mode);
S32 xTRCCardCheckCorrupt(S32 card, S32 repair, S32* files, S32* repaired);
S32 xTRCCardCheckGame(S32 card, S32 game);

static void TRCClearScreen();
static void TRCWaitFrames(S32 frames);
static void TRCShowCardMessage(char* text);
static S32 TRCAskCard(char* text, const char* first, const char* second, TRCCardCheck check, TRCCardWatch* watch);

#define TRC_TITLE "$A170066176Scooby-Doo: Night of 100 Frights$C"
#define TRC_RED "200040046"
#define TRC_GREEN "000210068"

// xTRCInit: the pad records number their pads, and the disc state starts unknown.
void xTRCInit()
{
    memset(gTrcPad, 0, sizeof(gTrcPad));
    for (S32 i = 0; i < 4; i++)
    {
        gTrcPad[i].id = i;
    }
    memset(gTrcDisk, 0, sizeof(gTrcDisk));
}

// Draws the message for a pad or disc state over the screen and pauses the game. A missing
// controller counts only in play. Returns 0 for a state outside the switch (and for a missing
// controller outside play), else 1, including TRC_Unknown, which draws no text.
S32 xTRCRenderState(_tagTRCState state)
{
    RwCamera* camera;
    char* text;

    switch (state)
    {
    case TRC_Unknown:
        text = NULL;
        break;
    case TRC_PadMissing:
    {
        U32 show;
        if (gGameMode == 0 || gGameMode == 1)
        {
            show = 0;
        }
        else if (zGameGetOstrich() != 2)
        {
            show = 0;
        }
        else if (zMenuRunning())
        {
            show = 0;
        }
        else if (zGameModeGet() == 6 || zGameModeGet() == 4)
        {
            show = 0;
        }
        else if (zGameNickCamActive())
        {
            show = 0;
        }
        else
        {
            show = 1;
        }
        if (!show)
        {
            return 0;
        }
        text = "The Controller in Controller Socket 1 has been removed. Reconnect the Controller to continue with the game.";
        break;
    }
    case TRC_PadInvalidType:
        text = "The Controller in Controller Socket 1 is an unknown type. Connect a standard Controller to continue with the game.";
        break;
    case TRC_DiskNotIdentified:
        text = "Please wait. Checking Disc.";
        break;
    case TRC_DiskTrayOpen:
        text = "The Disc Cover is open. If you want to continue the game, please close the Disc Cover.";
        break;
    case TRC_DiskNoDisk:
        text = "Please insert the Scooby-Doo Game Disc.";
        break;
    case TRC_DiskInvalid:
        text = "This is not the Scooby-Doo Game Disc. Please insert the Scooby-Doo Game Disc.";
        break;
    case TRC_DiskRetry:
        text = "The Game Disc could not be read. Please read the Nintendo GameCube^ Instruction Booklet for more information.";
        break;
    case TRC_DiskFatal:
        text = "An error has occurred. Turn the power off and refer to the Nintendo GameCube^ Instruction Booklet for further instructions.";
        break;
    default:
        return 0;
    }

    if (globals.cmgr)
    {
        globals.cmgr->stop = 1;
    }
    zGamePauseIfPossible();
    if (!sTrcFontBox)
    {
        if (sTrcOwnFrame)
        {
            camera = iCameraCreate(640, 480);
            RwRGBA clear = { 0, 0, 0, 0 };
            RwCameraClear(camera, &clear, 3);
            RwCameraBeginUpdate(camera);
        }
        sTrcFontBox = xFontBoxCreate(0.0f, 0.0f, 640.0f, 480.0f, 0, TRC_TEXT_COLOR);
        xFontBoxSetText(sTrcFontBox, text);
        xFontBoxSetFont(sTrcFontBox, 1);
        xFontBoxSetDrawSize(sTrcFontBox, 15.0f, 20.0f);
        xFontBoxSetDrawSpacing(sTrcFontBox, 15.0f, 20.0f);
        xFontBoxSetBackdrop(sTrcFontBox, xColorFromRGBA(0, 0, 0, 200));
        xFontBoxSetInset(sTrcFontBox, 50.0f, 50.0f, 230.0f, 0.0f);
        xFontBoxSetJustification(sTrcFontBox, 0x40);
        xFontBoxRender(sTrcFontBox);
        xFontBoxDestroy(sTrcFontBox);
        sTrcFontBox = NULL;
        if (sTrcOwnFrame)
        {
            RwCameraEndUpdate(camera);
            RwCameraShowRaster(camera, NULL, 1);
            iCameraDestroy(camera);
        }
    }
    return 1;
}

// iPad reports pad 0's state here.
void xTRCPad(S32 pad_id, _tagTRCState state)
{
    if (pad_id == 0)
    {
        gTrcPad[pad_id].state = state;
    }
}

// The disc layer reports here: while the disc is being identified only the second word
// changes, so the last real state survives the check.
void xTRCDisk(_tagTRCState state)
{
    if (state != TRC_DiskNotIdentified)
    {
        gTrcDisk[0] = state;
        gTrcDisk[1] = TRC_DiskIdentified;
    }
    else
    {
        gTrcDisk[1] = TRC_DiskNotIdentified;
    }
}

// Whether the disc is in an error state that stops the game (cover open, no disc, wrong
// disc, read error, fatal error).
U32 xTRCDiskHalted()
{
    switch (gTrcDisk[0])
    {
    case TRC_DiskTrayOpen:
    case TRC_DiskNoDisk:
    case TRC_DiskInvalid:
    case TRC_DiskRetry:
    case TRC_DiskFatal:
        return 1;
    case TRC_DiskTrayClosed:
    default:
        return 0;
    }
}

// Draws the pad or disc message in a frame of its own, for loops outside the game's render.
void xTRCRenderFrame()
{
    sTrcOwnFrame = 1;
    xTRCRenderState((_tagTRCState)gTrcPad[0].state) || xTRCRenderState(gTrcDisk[0]);
    sTrcOwnFrame = 0;
}

// Draws the pad message, or else the disc message, into the current frame.
void xTRCRenderMessage()
{
    if (xTRCRenderState((_tagTRCState)gTrcPad[0].state))
    {
        return;
    }
    if (xTRCRenderState(gTrcDisk[0]))
    {
        return;
    }
}

// The start-up memory-card check that main runs: the cards in both slots are checked for
// damage, the wrong format, a corrupt save file and room for one, and the checks start over
// when the player retries.
void xTRCCardCheckStartup()
{
    S32 formatted;
    S32 retry;
    S32 pass;
    S32 result;

    sTrcCardCheckActive = 1;
    formatted = 0;
    static S32 warned = 0;
    pass = 0;
    do
    {
        if (pass++ != 0)
        {
            if (warned)
            {
                TRCShowCardMessage(NULL);
            }
            else
            {
                TRCClearScreen();
            }
            TRCWaitFrames(15);
        }

        formatted = 0;
        retry = 0;
        result = xTRCCardCheckAny();
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
            continue;
        }
        result = xTRCCardCheckInserted(0);
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckUsable(0);
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckFormat(0, &formatted);
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (result == 1 && !formatted)
        {
            goto slot1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckSaveFile(0);
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckSpace(0);
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckFiles(0);
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        if (result == 1)
        {
            break;
        }

    slot1:
        result = xTRCCardCheckInserted(1);
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckUsable(1);
        if (result == 2)
        {
            break;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckFormat(1, &formatted);
        if (result == 2)
        {
            break;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (result == 1 && !formatted)
        {
            break;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckSaveFile(1);
        if (result == 2)
        {
            break;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckSpace(1);
        if (result == 2)
        {
            break;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            continue;
        }
        result = xTRCCardCheckFiles(1);
        if (result == 2)
        {
            break;
        }
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
    } while (retry);

    TRCClearScreen();
    sTrcCardCheckActive = 0;
}

// A message box's card watch: true once the card in the watched slot is not the one the
// check started with.
S32 xTRCCardWatchChanged(TRCCardWatch* watch)
{
    S32 changed = 0;
    if (watch == NULL)
    {
        return 0;
    }
    S32 phys = iSGTgtPhysSlotIdx(watch->session, watch->slot);
    if (phys != watch->card)
    {
        changed = 1;
    }
    return changed;
}

// Hook the card dialogs call before they show a problem message. On GameCube it returns 2 at
// once, so the dialog goes ahead; the card checks after the return never run.
S32 xTRCCardDialogHook(S32 card, st_ISGSESSION* session)
{
    S32 result = 2;
    char text[512] = { 0 };
    return result;

    U32 state = iSGCheckMemoryCard(session, card);
    if (!(state & 1))
    {
        xTRCCardMessageSetCard(0, card, card);
        xTRCCardMessageSet(0, 24);
        xTRCCardMessageText(0, text);
        S32 choice = TRCAskCard(text, "Continue Without Saving", "Retry", NULL, NULL);
        if (choice == -1)
        {
            result = 4;
        }
        else if (choice == 0)
        {
            result = 3;
        }
        else if (choice == 1)
        {
            result = 4;
        }
        else
        {
            result = 3;
        }
    }
    else if (!(state & 2))
    {
        xTRCCardMessageSetCard(0, card, card);
        xTRCCardMessageSet(0, 27);
        xTRCCardMessageText(0, text);
        S32 choice = TRCAskCard(text, "Continue Without Saving", "Retry", NULL, NULL);
        if (choice == -1)
        {
            result = 4;
        }
        else if (choice == 0)
        {
            result = 3;
        }
        else if (choice == 1)
        {
            result = 4;
        }
        else
        {
            result = 3;
        }
    }
    return result;
}

// Checks that a card is in the slot; a card that is not ready says why.
S32 xTRCCardCheckInserted(S32 card)
{
    S32 result = 1;
    char text[512] = { 0 };
    st_ISGSESSION* session = iSGSessionBegin(NULL, NULL);
    U32 state = iSGCheckMemoryCard(session, card);

    if (card != 0 && !(state & 1))
    {
        xTRCCardMessageSetCard(0, card, card);
        xTRCCardMessageSet(0, 24);
        xTRCCardMessageText(0, text);
        S32 choice = TRCAskCard(text, "Continue Without Saving", "Retry", NULL, NULL);
        if (choice == -1)
        {
            result = 4;
        }
        else if (choice == 0)
        {
            result = 3;
        }
        else if (choice == 1)
        {
            result = 4;
        }
        else
        {
            result = 3;
        }
    }
    else if ((state & 1) && !(state & 2))
    {
        S32 hook = xTRCCardDialogHook(card, session);
        if (hook != 2)
        {
            result = hook;
        }
        else
        {
            xTRCCardMessageSetCard(0, card, card);
            xTRCCardMessageSet(0, 27);
            xTRCCardMessageText(0, text);
            S32 choice = TRCAskCard(text, "Continue Without Saving", "Retry", NULL, NULL);
            if (choice == -1)
            {
                result = 4;
            }
            else if (choice == 0)
            {
                result = 3;
            }
            else if (choice == 1)
            {
                result = 4;
            }
            else
            {
                result = 3;
            }
        }
    }
    iSGSessionEnd(session);
    return result;
}

// Checks that a memory card is in either slot.
S32 xTRCCardCheckAny()
{
    S32 result;
    char text[512] = { 0 };
    st_ISGSESSION* session = iSGSessionBegin(NULL, NULL);
    S32 stateA = iSGCheckMemoryCard(session, 0);
    S32 stateB = iSGCheckMemoryCard(session, 1);

    if ((stateA & 1) || (stateB & 1))
    {
        result = 1;
    }
    else if (!(stateA & 1) && !(stateB & 1))
    {
        xTRCCardMessageSetCard(0, 0, 0);
        xTRCCardMessageSet(0, 25);
        xTRCCardMessageText(0, text);
        S32 choice = TRCAskCard(text, "Continue Without Saving", "Retry", NULL, NULL);
        if (choice == -1)
        {
            result = 4;
        }
        else if (choice == 0)
        {
            result = 3;
        }
        else if (choice == 1)
        {
            result = 4;
        }
        else
        {
            result = 3;
        }
    }
    else if (iSGTgtCount(session, NULL) > 0)
    {
        result = 1;
    }
    else
    {
        xTRCCardMessageSetCard(0, 0, 0);
        xTRCCardMessageSet(0, 25);
        xTRCCardMessageText(0, text);
        S32 choice = TRCAskCard(text, "Continue Without Saving", "Retry", NULL, NULL);
        if (choice == -1)
        {
            result = 4;
        }
        else if (choice == 0)
        {
            result = 3;
        }
        else if (choice == 1)
        {
            result = 4;
        }
        else
        {
            result = 3;
        }
    }
    iSGSessionEnd(session);
    return result;
}

// Checks that the card is usable: damaged, wrong-device and incompatible cards say so.
S32 xTRCCardCheckUsable(S32 card)
{
    S32 result = 1;
    char text[512] = { 0 };
    S32 slot = -1;
    st_ISGSESSION* session = iSGSessionBegin(NULL, NULL);
    S32 count = iSGTgtCount(session, NULL);
    if (count < 1)
    {
        result = 4;
    }
    else if (count == 2)
    {
        slot = card;
    }
    else
    {
        S32 phys = iSGTgtPhysSlotIdx(session, 0);
        if (phys == card)
        {
            slot = 0;
        }
    }

    if (slot < 0)
    {
        result = 2;
    }
    else
    {
        S32 message = 0;
        iSGTgtSetActive(session, slot);
        U32 state = iSGTgtState(session, slot, NULL);
        if (state & 0x1000000)
        {
            message = 26;
        }
        else if (state & 0x8000000)
        {
            message = 28;
        }
        else if (state & 0x2000000)
        {
            message = 30;
        }
        else if (!(state & 4))
        {
            message = 31;
        }
        if (message != 0)
        {
            S32 hook = xTRCCardDialogHook(card, session);
            if (hook != 2)
            {
                result = hook;
            }
            else
            {
                xTRCCardMessageSetCard(0, slot, card);
                xTRCCardMessageSet(0, message);
                xTRCCardMessageText(0, text);
                S32 choice = TRCAskCard(text, "Continue Without Saving", "Retry", NULL, NULL);
                if (choice == 1)
                {
                    result = 4;
                }
                else if (choice == 0)
                {
                    result = 3;
                }
                else
                {
                    result = 3;
                }
            }
        }
    }
    iSGSessionEnd(session);
    return result;
}

// Checks that the card is formatted for this market, and offers to format it.
S32 xTRCCardCheckFormat(S32 card, S32* formatted)
{
    S32 result = 1;
    st_ISGSESSION* session;
    TRCCardWatch watch = { 0 };
    watch.card = card;
    char text[512] = { 0 };
    S32 slot = -1;

    if (formatted)
    {
        *formatted = 0;
    }
    session = iSGSessionBegin(NULL, NULL);
    S32 count = iSGTgtCount(session, NULL);
    if (count < 1)
    {
        result = 4;
    }
    else if (count == 2)
    {
        slot = card;
    }
    else
    {
        S32 phys = iSGTgtPhysSlotIdx(session, 0);
        if (phys == card)
        {
            slot = 0;
        }
    }

    if (slot < 0)
    {
        result = 2;
    }
    else
    {
        iSGTgtSetActive(session, slot);
        U32 state = iSGTgtState(session, slot, NULL);
        if (!(state & 4) || (state & 0x8000000))
        {
            slot = -1;
        }
    }

    if (slot < 0)
    {
        result = 2;
    }
    else
    {
        iSGTgtSetActive(session, slot);
        U32 state = iSGTgtState(session, slot, NULL);
        U32 market = state & 0x4000000;
        S32 hook = 2;
        watch.session = session;
        watch.card = card;
        watch.slot = slot;
        if (market || !(state & 2))
        {
            hook = xTRCCardDialogHook(card, session);
            if (hook != 2)
            {
                result = hook;
            }
        }
        if (hook == 2 && (market || !(state & 2)))
        {
            xTRCCardMessageSetCard(0, slot, card);
            if (market)
            {
                xTRCCardMessageSet(0, 17);
            }
            else
            {
                xTRCCardMessageSet(0, 16);
            }
            xTRCCardMessageText(0, text);
            const char* options[4] = { "Continue Without Saving", "Retry", "Format Memory Card" };
            S32 choice = xTRCMessageBox(text, options, xTRCCardWatchChanged, &watch);
            if (choice == -1)
            {
                result = 4;
            }
            else if (choice == 1)
            {
                result = 4;
            }
            else if (choice == 0)
            {
                result = 1;
            }
            else if (choice == 2)
            {
                xTRCCardMessageSetCard(0, slot, card);
                xTRCCardMessageSet(0, 18);
                xTRCCardMessageText(0, text);
                S32 choice = TRCAskCard(text, "No", "Yes", xTRCCardWatchChanged, &watch);
                if (choice == -1)
                {
                    result = 4;
                }
                else if (choice == 0)
                {
                    result = 4;
                }
                else
                {
                    S32 canRecover = 0;
                    xTRCCardMessageSetCard(0, slot, card);
                    xTRCCardMessageSet(0, 19);
                    xTRCCardMessageText(0, text);
                    TRCShowCardMessage(text);
                    S32 ok = iSGTgtFormat(session, slot, 0, &canRecover);
                    TRCWaitFrames(60);
                    xTRCCardMessageSetCard(0, slot, card);
                    if (ok)
                    {
                        result = 1;
                        xTRCCardMessageSet(0, 20);
                        if (formatted)
                        {
                            *formatted = 1;
                        }
                    }
                    else if (canRecover)
                    {
                        result = 4;
                        xTRCCardMessageSet(0, 21);
                    }
                    else
                    {
                        result = 4;
                        xTRCCardMessageSet(0, 22);
                    }
                    xTRCCardMessageText(0, text);
                    if (ok)
                    {
                        choice = TRCAskCard(text, NULL, "Continue", xTRCCardWatchChanged, &watch);
                    }
                    else
                    {
                        choice = TRCAskCard(text, NULL, "Continue", NULL, NULL);
                    }
                    if (choice == -1)
                    {
                        result = 4;
                    }
                }
            }
        }
        else if ((state & 2) && formatted)
        {
            *formatted = 1;
        }
    }
    iSGSessionEnd(session);
    return result;
}

// Checks the save file on the card, and offers to delete a corrupt one.
S32 xTRCCardCheckSaveFile(S32 card)
{
    S32 result = 4;
    st_ISGSESSION* session;
    S32 slot = -1;
    TRCCardWatch watch = { 0 };
    watch.card = card;
    char text[512] = { 0 };
    session = iSGSessionBegin(NULL, NULL);
    S32 count = iSGTgtCount(session, NULL);

    if (count < 1)
    {
        result = 4;
    }
    else if (count == 2)
    {
        slot = card;
    }
    else
    {
        S32 phys = iSGTgtPhysSlotIdx(session, 0);
        if (phys == card)
        {
            slot = 0;
        }
    }

    if (slot < 0)
    {
        result = 2;
    }
    else
    {
        iSGTgtSetActive(session, slot);
        watch.session = session;
        watch.card = card;
        S32 hook = 2;
        watch.slot = slot;
        S32 corrupt = -1;
        S32 repaired = -1;
        S32 ok = iSGCheckForCorruptFiles(session, 0, &corrupt, NULL);
        if (!ok)
        {
            hook = xTRCCardDialogHook(card, session);
            if (hook != 2)
            {
                result = hook;
            }
        }
        if (hook == 2 && ok)
        {
            result = 1;
        }
        else if (hook == 2)
        {
            const char* options[4] = { "Continue Without Saving", "Retry", "Delete Corrupt File" };
            xTRCCardMessageSetCard(0, slot, card);
            xTRCCardMessageSet(0, 9);
            xTRCCardMessageSetFile(0, corrupt);
            xTRCCardMessageText(0, text);
            S32 choice = xTRCMessageBox(text, options, xTRCCardWatchChanged, &watch);
            if (choice == -1)
            {
                result = 4;
            }
            else if (choice == 1)
            {
                result = 4;
            }
            else if (choice == 0)
            {
                result = 3;
            }
            else if (choice == 2)
            {
                xTRCCardMessageSetCard(0, slot, card);
                xTRCCardMessageSet(0, 10);
                xTRCCardMessageText(0, text);
                choice = TRCAskCard(text, "No", "Yes", xTRCCardWatchChanged, &watch);
                if (choice == -1)
                {
                    result = 4;
                }
                else if (choice == 0)
                {
                    result = 4;
                }
                else if (choice == 1)
                {
                    xTRCCardMessageSetCard(0, slot, card);
                    xTRCCardMessageSet(0, 13);
                    xTRCCardMessageText(0, text);
                    TRCShowCardMessage(text);
                    xTRCCardMessageSetCard(0, slot, card);
                    S32 ok = iSGCheckForCorruptFiles(session, 1, NULL, &repaired);
                    if (ok)
                    {
                        result = 1;
                        xTRCCardMessageSet(0, 11);
                    }
                    else if (!ok && repaired >= 0)
                    {
                        result = 4;
                        xTRCCardMessageSet(0, 11);
                    }
                    else
                    {
                        result = 4;
                        xTRCCardMessageSet(0, 12);
                    }
                    xTRCCardMessageText(0, text);
                    if (TRCAskCard(text, NULL, "Continue", xTRCCardWatchChanged, &watch) == -1)
                    {
                        result = 4;
                    }
                }
                else
                {
                    result = 3;
                }
            }
            else
            {
                result = 3;
            }
        }
    }
    iSGSessionEnd(session);
    return result;
}

// Checks that the card has room for a save file, and offers to manage the card.
S32 xTRCCardCheckSpace(S32 card)
{
    st_ISGSESSION* session;
    S32 result = 1;
    S32 slot = -1;
    TRCCardWatch watch = { 0 };
    watch.card = card;
    S32 needed = 0;
    S32 available = 0;
    S32 blockSize = 0;
    S32 needFile = 0;
    S32 freeFiles = 0;
    S32 reset = 0;
    char text[512] = { 0 };
    session = iSGSessionBegin(NULL, NULL);
    S32 count = iSGTgtCount(session, NULL);

    if (count < 1)
    {
        result = 4;
    }
    else if (count == 2)
    {
        slot = card;
    }
    else
    {
        S32 phys = iSGTgtPhysSlotIdx(session, 0);
        if (phys == card)
        {
            slot = 0;
        }
    }

    if (slot < 0)
    {
        result = 2;
    }
    else
    {
        iSGTgtSetActive(session, slot);
        watch.session = session;
        watch.card = card;
        watch.slot = slot;
        iSGTgtFreeSpace(session, slot, &blockSize, &freeFiles);
        S32 hook = 2;
        S32 room = iSGTgtHaveRoom(session, slot, 0x6000, NULL, NULL, &needed, &available, &needFile);
        if (!room)
        {
            hook = xTRCCardDialogHook(card, session);
            if (hook != 2)
            {
                result = hook;
            }
        }
        if (hook == 2 && room)
        {
            result = 1;
        }
        else if (hook == 2)
        {
            const char* options[4] = { "Continue Without Saving", "Retry", "Manage Memory Card" };
            xTRCCardMessageSetCard(0, slot, card);
            xTRCCardMessageSet(0, 32);
            xTRCCardMessageSetSpace(0, available, needed, blockSize, freeFiles, needFile);
            xTRCCardMessageText(0, text);
            S32 choice = xTRCMessageBox(text, options, xTRCCardWatchChanged, &watch);
            if (choice == -1)
            {
                result = 4;
            }
            else if (choice == 1)
            {
                result = 4;
            }
            else if (choice == 0)
            {
                result = 3;
            }
            else if (choice == 2)
            {
                reset = 1;
                result = 4;
            }
            else
            {
                result = 3;
            }
        }
    }
    iSGSessionEnd(session);
    if (reset)
    {
        iSystemReset(1);
    }
    return result;
}

// When some but not all of the three save files exist, checks there is room to create the
// missing ones; with none or all three present it passes.
S32 xTRCCardCheckFiles(S32 card)
{
    S32 result = 1;
    st_ISGSESSION* session;
    S32 slot = -1;
    TRCCardWatch watch = { 0 };
    watch.card = card;
    S32 needed = 0;
    S32 available = 0;
    S32 blockSize = 0;
    S32 needFile = 0;
    S32 freeFiles = 0;
    S32 reset = 0;
    char text[512] = { 0 };
    session = iSGSessionBegin(NULL, NULL);
    S32 count = iSGTgtCount(session, NULL);

    if (count < 1)
    {
        result = 4;
    }
    else if (count == 2)
    {
        slot = card;
    }
    else
    {
        S32 phys = iSGTgtPhysSlotIdx(session, 0);
        if (phys == card)
        {
            slot = 0;
        }
    }

    if (slot < 0)
    {
        result = 2;
    }
    else
    {
        iSGTgtSetActive(session, slot);
        watch.session = session;
        watch.card = card;
        watch.slot = slot;
        iSGTgtFreeSpace(session, slot, &blockSize, &freeFiles);
        S32 missing = 0;
        S32 first = -1;
        S32 hook = 2;
        S32 sizes[3] = { 0 };
        S32 found = iSGGameFileCount(session);
        if (found >= 1 && found != 3)
        {
            S32 i;
            for (i = 0; i < 3; i++)
            {
                char* name = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, i);
                sizes[i] = iSGFileSize(session, name);
                if (sizes[i] < 1)
                {
                    sizes[i] = -1;
                    missing++;
                    if (first < 0)
                    {
                        first = i;
                    }
                }
            }
            if (missing != 0)
            {
                S32 room = iSGTgtHaveRoom(session, slot, 0x6000, NULL, iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, i), &needed, &available, &needFile);
                if (!room)
                {
                    hook = xTRCCardDialogHook(card, session);
                    if (hook != 2)
                    {
                        result = hook;
                    }
                }
                if (hook == 2 && room)
                {
                    result = 1;
                }
                else if (hook == 2)
                {
                    const char* options[4] = { "Continue", "Retry", "Manage Memory Card" };
                    xTRCCardMessageSetCard(0, slot, card);
                    xTRCCardMessageSet(0, 33);
                    xTRCCardMessageSetSpace(0, available, needed, blockSize, freeFiles, needFile);
                    xTRCCardMessageText(0, text);
                    S32 choice = xTRCMessageBox(text, options, xTRCCardWatchChanged, &watch);
                    if (choice == -1)
                    {
                        result = 4;
                    }
                    else if (choice == 1)
                    {
                        result = 4;
                    }
                    else if (choice == 0)
                    {
                        result = 3;
                    }
                    else if (choice == 2)
                    {
                        reset = 1;
                        result = 4;
                    }
                    else
                    {
                        result = 3;
                    }
                }
            }
        }
    }
    iSGSessionEnd(session);
    if (reset)
    {
        iSystemReset(1);
    }
    return result;
}

// Waits the given number of frames, polling the reset button and the disc.
static void TRCWaitFrames(S32 frames)
{
    while (frames-- > 0)
    {
        iSystemPollResetAndDisc();
        iVSync();
    }
}

// Clears the screen to black for one frame.
static void TRCClearScreen()
{
    RwRGBA clear = { 0, 0, 0, 0 };
    RwCamera* camera = iCameraCreate(640, 480);
    RwCameraClear(camera, &clear, 3);
    RwCameraBeginUpdate(camera);
    RwCameraEndUpdate(camera);
    RwCameraShowRaster(camera, NULL, 1);
    iCameraDestroy(camera);
}

// Draws one memory-card message as a single frame, with no buttons. With no text it draws the
// warning not to remove the memory card.
static void TRCShowCardMessage(char* text)
{
    RwRGBA clear = { 0, 0, 0, 0 };
    RwCamera* camera;
    char buffer[256] = { 0 };
    if (!text)
    {
        sprintf(buffer, "$A%sWARNING!$C Accessing the Nintendo GameCube%s Memory Card.\nPlease do not remove the Memory Card or turn off the power to the Nintendo GameCube%s.\n", TRC_RED, "^", "^");
        text = buffer;
    }
    xFontBox* box = xFontBoxCreate(0.0f, 0.0f, 640.0f, 480.0f, 1, TRC_TEXT_COLOR);
    xFontBoxSetText(box, text);
    xFontBoxSetDrawSize(box, 18.0f, 24.0f);
    xFontBoxSetDrawSpacing(box, 17.0f, 24.0f);
    xFontBoxSetBackdrop(box, xColorFromRGBA(0, 0, 0, 255));
    xFontBoxSetInset(box, 100.0f, 100.0f, 100.0f, 0.0f);
    xFontBoxSetJustification(box, 0x40);
    camera = iCameraCreate(640, 480);
    RwCameraClear(camera, &clear, 3);
    RwCameraBeginUpdate(camera);
    xFontBoxRender(box);
    RwCameraEndUpdate(camera);
    RwCameraShowRaster(camera, NULL, 1);
    iCameraDestroy(camera);
    xFontBoxDestroy(box);
}

// Shows a message box with two buttons, the first option on B and the second on A.
static S32 TRCAskCard(char* text, const char* first, const char* second, TRCCardCheck check, TRCCardWatch* watch)
{
    const char* options[4] = {};
    options[0] = first;
    options[1] = second;
    return xTRCMessageBox(text, options, check, watch);
}

// Shows a message box with up to four buttons (A, B, X, Y, in options order: B, A, X, Y) and
// waits for one, or for the check callback to report a change (-1). Returns the button's
// option index. With no text it only clears the screen.
S32 xTRCMessageBox(char* text, const char** options, TRCCardCheck check, TRCCardWatch* watch)
{
    S32 choice = 0;
    RwRGBA clear = { 0, 0, 0, 0 };
    RwCamera* camera;
    xFontBox* box;
    xFontBox* buttons1;
    xFontBox* buttons2 = NULL;
    char buttons[256] = { 0 };
    char buttonsLow[256] = { 0 };
    S32 count;
    U32 mask;
    S32 done = 0;

    if (text == NULL)
    {
        TRCClearScreen();
        return 0;
    }

    if (options[1] && options[0] && options[2] && options[3])
    {
        count = 4;
        mask = 0xF0000;
        sprintf(buttons, "%s %s\t%s %s", "#", options[1], "}", options[0]);
        sprintf(buttonsLow, "%s %s\t%s %s", "~", options[2], "{", options[3]);
    }
    else if (options[1] && options[0] && options[2])
    {
        count = 3;
        mask = 0x70000;
        sprintf(buttons, "%s %s\t%s %s", "#", options[1], "}", options[0]);
        sprintf(buttonsLow, "%s %s", "~", options[2]);
    }
    else if (options[1] && options[0])
    {
        count = 2;
        mask = 0x50000;
        sprintf(buttons, "%s %s\t%s %s", "#", options[1], "}", options[0]);
    }
    else
    {
        count = 1;
        mask = 0x10000;
        sprintf(buttons, "%s %s", "#", options[1]);
    }

#if VERSION_GIHE78_REV0
    box = xFontBoxCreate(0.0f, 0.0f, 640.0f, 360.0f, 1, TRC_TEXT_COLOR);
    buttons1 = xFontBoxCreate(0.0f, 360.0f, 640.0f, 100.0f, 1, TRC_TEXT_COLOR);
    if (count > 2)
    {
        buttons2 = xFontBoxCreate(0.0f, 400.0f, 640.0f, 100.0f, 1, TRC_TEXT_COLOR);
    }
    xFontBoxSetText(box, text);
    xFontBoxSetDrawSize(box, 18.0f, 24.0f);
    xFontBoxSetDrawSpacing(box, 17.0f, 24.0f);
    xFontBoxSetBackdrop(box, xColorFromRGBA(0, 0, 0, 255));
    xFontBoxSetInset(box, 100.0f, 100.0f, 100.0f, 0.0f);
    xFontBoxSetJustification(box, 0x40);
    xFontBoxSetText(buttons1, buttons);
    xFontBoxSetDrawSize(buttons1, 20.0f, 26.0f);
    xFontBoxSetDrawSpacing(buttons1, 19.0f, 26.0f);
    xFontBoxSetBackdrop(buttons1, xColorFromRGBA(0, 0, 0, 255));
    xFontBoxSetInset(buttons1, 40.0f, 10.0f, 0.0f, 0.0f);
    xFontBoxSetJustification(buttons1, 0x40);
    if (count > 2)
    {
        xFontBoxSetText(buttons2, buttonsLow);
        xFontBoxSetDrawSize(buttons2, 20.0f, 26.0f);
        xFontBoxSetDrawSpacing(buttons2, 19.0f, 26.0f);
        xFontBoxSetBackdrop(buttons2, xColorFromRGBA(0, 0, 0, 255));
        xFontBoxSetInset(buttons2, 40.0f, 10.0f, 0.0f, 0.0f);
        xFontBoxSetJustification(buttons2, 0x40);
    }
#elif VERSION_GIHE78_REV1
    // Revision 1 draws a smaller box with no black backdrop and does not set the glyph size.
    box = xFontBoxCreate(0.0f, 0.0f, 640.0f, 320.0f, 1, TRC_TEXT_COLOR);
    buttons1 = xFontBoxCreate(0.0f, 320.0f, 640.0f, 30.0f, 1, TRC_TEXT_COLOR);
    if (count > 2)
    {
        buttons2 = xFontBoxCreate(0.0f, 350.0f, 640.0f, 30.0f, 1, TRC_TEXT_COLOR);
    }
    xFontBoxSetText(box, text);
    xFontBoxSetDrawSpacing(box, 16.0f, 24.0f);
    xFontBoxSetInset(box, 32.0f, 32.0f, 96.0f, 0.0f);
    xFontBoxSetJustification(box, 0x40);
    xFontBoxSetText(buttons1, buttons);
    xFontBoxSetDrawSpacing(buttons1, 16.0f, 24.0f);
    xFontBoxSetInset(buttons1, 40.0f, 10.0f, 0.0f, 0.0f);
    xFontBoxSetJustification(buttons1, 0x40);
    if (count > 2)
    {
        xFontBoxSetText(buttons2, buttonsLow);
        xFontBoxSetDrawSpacing(buttons2, 16.0f, 24.0f);
        xFontBoxSetInset(buttons2, 40.0f, 10.0f, 0.0f, 0.0f);
        xFontBoxSetJustification(buttons2, 0x40);
    }
#endif

    camera = iCameraCreate(640, 480);
    while (!done)
    {
        gFontWiggleTime += 1.0f / 60.0f;
        RwCameraClear(camera, &clear, 3);
        RwCameraBeginUpdate(camera);
        xFontBoxRender(box);
        xFontBoxRender(buttons1);
        if (count > 2)
        {
            xFontBoxRender(buttons2);
        }
        RwCameraEndUpdate(camera);
        RwCameraShowRaster(camera, NULL, 1);
        if (check && check(watch))
        {
            choice = -1;
            break;
        }
        iSystemPollResetAndDisc();
        xPadUpdate(0, 1.0f / 60.0f);
        U32 pressed = mask & globals.pad0->pressed;
        switch (count)
        {
        case 4:
            done = 1;
            if (pressed & 0x40000)
            {
                choice = 0;
            }
            else if (pressed & 0x10000)
            {
                choice = 1;
            }
            else if (pressed & 0x20000)
            {
                choice = 2;
            }
            else if (pressed & 0x80000)
            {
                choice = 3;
            }
            else
            {
                done = 0;
            }
            break;
        case 3:
            done = 1;
            if (pressed & 0x40000)
            {
                choice = 0;
            }
            else if (pressed & 0x10000)
            {
                choice = 1;
            }
            else if (pressed & 0x20000)
            {
                choice = 2;
            }
            else
            {
                done = 0;
            }
            break;
        case 2:
            done = 1;
            if (pressed & 0x40000)
            {
                choice = 0;
            }
            else if (pressed & 0x10000)
            {
                choice = 1;
            }
            else
            {
                done = 0;
            }
            break;
        case 1:
            done = 1;
            if (pressed & 0x10000)
            {
                choice = 1;
            }
            else
            {
                done = 0;
            }
            break;
        }
    }

    if (count > 2)
    {
        xFontBoxDestroy(buttons2);
    }
    xFontBoxDestroy(buttons1);
    xFontBoxDestroy(box);
    iCameraDestroy(camera);
    return choice;
}

// The memory-card message setters zSaveLoad calls; mode picks the message record.
void xTRCCardMessageClear(S32 mode)
{
    memset(&sTrcCardMessage[mode], 0, sizeof(TRCCardMessage));
}

void xTRCCardMessageSetCard(S32 mode, S32 card, S32 slot)
{
    TRCCardMessage* msg = &sTrcCardMessage[mode];
    memset(msg, 0, sizeof(TRCCardMessage));
    msg->card = card;
    msg->slot = slot;
    if (card == 0 || card == 1)
    {
        msg->flags |= 1;
    }
}

void xTRCCardMessageSet(S32 mode, S32 message)
{
    TRCCardMessage* msg = &sTrcCardMessage[mode];
    msg->flags |= 2;
    msg->message = message;
}

// Stores the space counts a "not enough space" message prints. zSaveLoad calls it too on
// revision 0; revision 1 keeps it inside this file.
#if VERSION_GIHE78_REV0
void xTRCCardMessageSetSpace(S32 mode, S32 available, S32 needed, S32 blockSize, S32 freeFiles, S32 needFile)
#elif VERSION_GIHE78_REV1
static void xTRCCardMessageSetSpace(S32 mode, S32 available, S32 needed, S32 blockSize, S32 freeFiles, S32 needFile)
#endif
{
    TRCCardMessage* msg = &sTrcCardMessage[mode];
    if (msg->flags)
    {
        msg->flags |= 4;
        msg->available = available;
        msg->needed = needed;
        msg->blockSize = blockSize < 1 ? 1 : blockSize;
        msg->freeFiles = freeFiles;
        msg->needFile = needFile;
    }
}

void xTRCCardMessageSetFile(S32 mode, S32 value)
{
    TRCCardMessage* msg = &sTrcCardMessage[mode];
    if (msg->flags)
    {
        msg->flags |= 8;
        msg->fileIndex = value;
    }
}

// The label of the message box's B button.
S32 xTRCCardMessageButtonB(S32 mode, char* dst)
{
    TRCCardMessage* msg = &sTrcCardMessage[mode];

    switch (msg->message)
    {
    case 33:
        return sprintf(dst, "%s", "Continue");
    case 4:
    case 32:
    default:
        return sprintf(dst, "%s", "Continue Without Saving");
    }
}

// The label of the message box's X button.
S32 xTRCCardMessageButtonX(S32 mode, char* dst)
{
    TRCCardMessage* msg = &sTrcCardMessage[mode];

    switch (msg->message)
    {
    case 4:
    case 32:
    case 33:
        return sprintf(dst, "%s", "Manage Memory Card");
    case 9:
        return sprintf(dst, "%s", "Delete Corrupt File");
    case 14:
    case 16:
        return sprintf(dst, "%s", "Format Memory Card");
    default:
        return sprintf(dst, "%s", " ");
    }
}

S32 xTRCCardMessageButtonY(S32 mode, char* dst)
{
    return sprintf(dst, "%s", "--error--");
}

// The text of the current memory-card message.
S32 xTRCCardMessageText(S32 mode, char* dst)
{
    TRCCardMessage* msg = &sTrcCardMessage[mode];
    char device[64] = { 0 };
    char cardName[64] = { 0 };
    char slotName[32] = { 0 };
    S32 blocks;
    char* name;

    name = xSGTgtCardName(NULL, msg->slot, (en_XSG_WHYFAIL)2);
    sprintf(device, name);
    name = xSGTgtCardName(NULL, msg->slot, (en_XSG_WHYFAIL)3);
    sprintf(slotName, name);
    name = xSGTgtCardName(NULL, msg->slot, (en_XSG_WHYFAIL)4);
    sprintf(cardName, name);

    switch (msg->message)
    {
    case 1:
        return sprintf(dst, "$A%sWARNING!$C\nSaving. Do not touch the %s or the %s.\n", TRC_RED, cardName, "POWER Button");
    case 2:
        return sprintf(dst, "$A%sSave Successful.$C\n", TRC_GREEN);
    case 3:
        return sprintf(dst, "$A%sSave Failed!$C\nCheck the %s and try again.", TRC_RED, cardName);
    case 4:
        blocks = (msg->needed + msg->blockSize - 1) / msg->blockSize;
#if VERSION_GIHE78_REV0
        return sprintf(dst, "$A%sSave Failed!$C\nNot enough space on the %s to create a new %s file.\nAt least $A%s%d file%s and %d block%s required.\n", TRC_RED, cardName, TRC_TITLE, TRC_GREEN, msg->needFile, msg->needFile == 1 ? "" : "s", blocks, blocks == 1 ? "$C is" : "s$C are");
#elif VERSION_GIHE78_REV1
        return sprintf(dst, "$A%sSave Failed!$C\nNot enough space on the %s to create a new %s file.\nAt least $A%s%d file%s and %d block%s required.\nExisting files may be loaded and overwritten.\n", TRC_RED, cardName, TRC_TITLE, TRC_GREEN, msg->needFile, msg->needFile == 1 ? "" : "s", blocks, blocks == 1 ? "$C is" : "s$C are");
#endif
    case 5:
        return sprintf(dst, "$A%sWARNING!$C\nLoading. Do not touch the %s or the %s.\n", TRC_RED, cardName, "POWER Button");
    case 6:
        return sprintf(dst, "$A%sLoad Successful.$C\n", TRC_GREEN);
    case 7:
        return sprintf(dst, "$A%sLoad Failed!$C\nCheck the %s and try again.\n", TRC_RED, cardName);
    case 8:
        return sprintf(dst, "$A%sLoad Failed!$C\nCheck the %s and try again.\n", TRC_RED, cardName);
    case 9:
        if (msg->fileIndex >= 0)
        {
            return sprintf(dst, "The %s file (%d) on the %s is corrupt and needs to be deleted.\n\nDo you wish to delete this file?", TRC_TITLE, msg->fileIndex + 1, cardName);
        }
        else
        {
            return sprintf(dst, "The %s file on the %s is corrupt and needs to be deleted.\n\nDo you wish to delete this file?", TRC_TITLE, cardName);
        }
        break;
    case 10:
        return sprintf(dst, "$A%sCAUTION$C\nDeleting this corrupt %s file will erase it permanently.\n\nAre you sure you wish to delete this file?\n", TRC_RED, TRC_TITLE);
    case 11:
        return sprintf(dst, "Corrupted %s file deleted.", TRC_TITLE);
    case 12:
        return sprintf(dst, "$A%sDelete Failed!$C\nUnabled to delete corrupt %s save file.\n", TRC_RED, TRC_TITLE);
    case 13:
        return sprintf(dst, "$A%sWARNING!$C\nDeleting. Do not touch the %s or the %s.\n", TRC_RED, cardName, "POWER Button");
    case 14:
    case 16:
        return sprintf(dst, "The %s contains corrupted data and needs to be formatted. Do you wish to format this Memory Card?", cardName);
    case 15:
    case 17:
        return sprintf(dst, "The %s is formatted for another market.\n\nTo save a %s file, this Memory Card needs to be formatted. Do you wish to format this Memory Card?", cardName, TRC_TITLE);
    case 18:
        return sprintf(dst, "$A%sCAUTION$C\nFormatting the %s will erase all previously saved data.\n\nAre you sure you wish to format?\n", TRC_RED, cardName);
    case 19:
        return sprintf(dst, "$A%sWARNING!$C\nFormatting. Do not touch the %s or the %s.\n", TRC_RED, cardName, "POWER Button");
    case 20:
        return sprintf(dst, "Format Successful.\n");
    case 21:
        return sprintf(dst, "$A%sFormat Failed!$C\nCheck the %s and try again.\n", TRC_RED, cardName);
    case 22:
        return sprintf(dst, "$A%sFormat Failed!$C\nThe %s is damaged and cannot be used.\n", TRC_RED, cardName, "^");
    case 23:
        return sprintf(dst, "$A%sCAUTION$C\nThis will restart the Nintendo GameCube%s.\n\nYour current game data will be lost.\nAre you sure you wish to restart?\n", TRC_RED, "^");
    case 24:
        return sprintf(dst, "No %s\nfound in %s.\n\nPlease insert a\n%s.\n", device, slotName, device);
    case 25:
        return sprintf(dst, "No %s found in Slot A or B.\n\nPlease insert a\n%s.\n", device, device);
    case 26:
        return sprintf(dst, "The %s is damaged and cannot be used.\n", cardName);
    case 27:
#if VERSION_GIHE78_REV0
        return sprintf(dst, "Wrong device in %s.\n\nPlease insert a\n%s.\n", slotName, device);
#elif VERSION_GIHE78_REV1
        return sprintf(dst, "Wrong device in %s.\n\nPlease insert a\n%s in %s.\n", slotName, device, slotName);
#endif
    case 28:
        return sprintf(dst, "The %s is not compatible with the %s save file.", cardName, TRC_TITLE);
    case 29:
        return sprintf(dst, "The %s is formatted for a market not compatible with the save file format of %s.\nTo avoid accidental data loss in this situation, reformatting is not supported.\nPlease review the %s using the Nintendo GameCube%s IPL Memory Card Browser.\n", cardName, TRC_TITLE, cardName, "^");
    case 30:
        return sprintf(dst, "The %s appears to be damaged and cannot be used.\n", cardName);
    case 31:
        return sprintf(dst, "The %s is not compatible with the save file format of\n%s.\n", cardName, TRC_TITLE);
#if VERSION_GIHE78_REV0
    case 32:
        blocks = (msg->needed + msg->blockSize - 1) / msg->blockSize;
        return sprintf(dst, "Not enough space on the %s.\nAt least $A%s%d file%s and %d block%s required.\n", cardName, TRC_GREEN, msg->needFile, msg->needFile == 1 ? "" : "s", blocks, blocks == 1 ? "$C is" : "s$C are");
#elif VERSION_GIHE78_REV1
    case 32:
#endif
    case 33:
        blocks = (msg->needed + msg->blockSize - 1) / msg->blockSize;
        return sprintf(dst, "Not enough space on the %s to create a new %s file.\nAt least $A%s%d file%s and %d block%s required.\nExisting files may be loaded and overwritten.\n", cardName, TRC_TITLE, TRC_GREEN, msg->needFile, msg->needFile == 1 ? "" : "s", blocks, blocks == 1 ? "$C is" : "s$C are");
    default:
        return sprintf(dst, "$A255050080<<internal error>>$C");
    }
}

// Checks the card in zSaveLoad's slot; a missing, damaged or incompatible card sets the
// message. Returns 0 when it did, else 1 (with mode set, before reading the card).
S32 xTRCCardCheckSlot(S32 card, S32 mode)
{
    S32 result = 1;
    S32 slot = -1;
    st_ISGSESSION* session;
    U32 state;
    S32 message = 0;
    S32 count;

    session = iSGSessionBegin(NULL, NULL);
    state = iSGCheckMemoryCard(session, card);
    if ((state & 1) && !(state & 2))
    {
        xTRCCardMessageSetCard(1, card, card);
        xTRCCardMessageSet(1, 27);
        return 0;
    }
    if (mode)
    {
        return 1;
    }
    count = iSGTgtCount(session, NULL);
    if (count < 1)
    {
        result = 0;
        slot = -1;
    }
    else if (count == 2)
    {
        slot = card;
    }
    else
    {
        S32 phys = iSGTgtPhysSlotIdx(session, 0);
        if (phys == card)
        {
            slot = 0;
        }
    }
    if (slot < 0)
    {
        xTRCCardMessageSetCard(1, card, card);
        xTRCCardMessageSet(1, 24);
        return 0;
    }
    iSGTgtSetActive(session, slot);
    state = iSGTgtState(session, slot, NULL);
    if (state & 0x1000000)
    {
        message = 26;
    }
    else if (state & 0x8000000)
    {
        message = 28;
    }
    else if (state & 0x2000000)
    {
        message = 30;
    }
    else if (!(state & 4))
    {
        message = 31;
    }
    if (message != 0)
    {
        result = 0;
        xTRCCardMessageSetCard(1, card, card);
        xTRCCardMessageSet(1, message);
    }
    iSGSessionEnd(session);
    return result;
}

// Checks the card for corrupt files, or deletes them (repair). Returns 1 when the card has no
// corrupt file (or the repair cleared them), 0 when one is found or remains, -1 when the card is
// missing or cannot be selected.
S32 xTRCCardCheckCorrupt(S32 card, S32 repair, S32* files, S32* repaired)
{
    S32 count;
    S32 result = 1;
    st_ISGSESSION* session;
    S32 slot = -1;

    session = iSGSessionBegin(NULL, NULL);
    if (files)
    {
        *files = 0;
    }
    if (repaired)
    {
        *repaired = 0;
    }
    count = iSGTgtCount(session, NULL);
    if (count < 1)
    {
        slot = -1;
    }
    else if (count == 2)
    {
        slot = card;
    }
    else
    {
        S32 phys = iSGTgtPhysSlotIdx(session, 0);
        if (phys == card)
        {
            slot = 0;
        }
    }
    if (slot < 0)
    {
        result = -1;
    }
    else if (!iSGTgtSetActive(session, slot))
    {
        result = -1;
    }
    else if (repair)
    {
        S32 left = 0;
        S32 err = iSGCheckForCorruptFiles(session, 1, NULL, &left);
        if (err == 0 && left >= 0)
        {
            if (repaired)
            {
                *repaired = 1;
            }
            result = 0;
        }
        else if (err == 0)
        {
            result = 0;
        }
    }
    else if (iSGCheckForCorruptFiles(session, 0, files, NULL) == 0)
    {
        result = 0;
    }
    iSGSessionEnd(session);
    return result;
}

// Checks that zSaveLoad's card is usable and has room for the game's save file: -1 when it
// is missing or not usable, 0 when it is full (the message names the space needed), else 1.
S32 xTRCCardCheckGame(S32 card, S32 game)
{
    S32 result;
    S32 slot = -1;
    S32 needed = 0;
    S32 available = 0;
    S32 blockSize = 0;
    S32 needFile = 0;
    S32 freeFiles = 0;
    st_ISGSESSION* session = iSGSessionBegin(NULL, NULL);
    S32 count = iSGTgtCount(session, NULL);

    if (count >= 1)
    {
        if (count == 2)
        {
            slot = card;
        }
        else
        {
            S32 phys = iSGTgtPhysSlotIdx(session, 0);
            if (phys == card)
            {
                slot = 0;
            }
        }
    }
    if (slot < 0)
    {
        result = -1;
    }
    else
    {
        iSGTgtSetActive(session, slot);
        U32 state = iSGTgtState(session, slot, NULL);
        if (state & 0x1000000)
        {
            result = -1;
        }
        else if (state & 0x8000000)
        {
            result = -1;
        }
        else if (state & 0x2000000)
        {
            result = -1;
        }
        else if (!(state & 4))
        {
            result = -1;
        }
        else if (!(state & 2))
        {
            result = -1;
        }
        else
        {
            iSGTgtFreeSpace(session, slot, &blockSize, &freeFiles);
            char* fname = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, game);
            if (iSGTgtHaveRoom(session, slot, 0x6000, NULL, fname, &needed, &available, &needFile))
            {
                result = 1;
            }
            else
            {
                result = 0;
                xTRCCardMessageSet(1, 4);
                xTRCCardMessageSetSpace(1, available, needed, blockSize, freeFiles, needFile);
            }
        }
    }
    iSGSessionEnd(session);
    return result;
}
