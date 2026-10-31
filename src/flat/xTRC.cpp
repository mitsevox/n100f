#include "xTRC.h"
#include "iCamera.h"
#include "iSystem.h"
#include "isavegame.h"
#include "xColor.h"
#include "xFont.h"
#include "xPad.h"
#include "zCutsceneMgr.h"
#include "zGameState.h"
#include "zGlobals.h"
#include "zMenu.h"

#include <stdio.h>
#include <string.h>

// The GameCube system messages: the memory-card dialogs and the pad and disc messages.

// One memory-card message: what it says (a case of fn_80041548), the card and slot it names
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

void zGamePauseIfPossible();
extern F32 gFontWiggleTime;

extern "C" {
// The save-game layer's card state queries.
S32 fn_800132C4(st_ISGSESSION* session, S32 repair, S32* files, S32* repaired);
S32 fn_800138EC(st_ISGSESSION* session, const char* dname, const char* fname);
void fn_80013C64(st_ISGSESSION* session, S32 tidx, S32* blockSize, S32* freeFiles);
U32 fn_80013D3C(st_ISGSESSION* session, S32 tidx, S32 refresh);
S32 fn_80014310(st_ISGSESSION* session, S32 card);
void fn_80016600();
void fn_80016698(S32 reset);
// The localized device, slot and card names.
char* fn_800389FC(S32 lang, S32 card, S32 which);
U32 fn_800AC0BC();

// The two message records, the start-up check's busy flag (zScene reads it), the font box of
// the pad and disc message (sTrcFontBox in the SLUS-20349 DWARF) and the flag that makes that
// message draw a frame of its own.
TRCCardMessage lbl_801B6610[2] = {};
U32 lbl_8025EBE0;
xFontBox* lbl_8025EBE4;
U32 lbl_8025EBE8;
}

extern "C" {

S32 fn_80044478(_tagTRCState state);
void fn_80044458(S32 pad_id, S32 state);
S32 fn_800443F8();
void fn_800443AC();
void fn_8004436C();
void fn_80043E78();
S32 fn_80043E20(TRCCardWatch* watch);
S32 fn_80043E18(S32 card, st_ISGSESSION* session);
S32 fn_80043BA4(S32 card);
S32 fn_80043940();
S32 fn_80043730(S32 card);
S32 fn_80043050(S32 card, S32* formatted);
S32 fn_80042A48(S32 card);
S32 fn_80042774(S32 card);
S32 fn_800423EC(S32 card);
S32 fn_80041D90(char* text, const char** options, TRCCardCheck check, TRCCardWatch* watch);
void fn_80041D58(S32 mode);
void fn_80041CE8(S32 mode, S32 card, S32 slot);
void fn_80041CC4(S32 mode, S32 message);
void fn_80041C78(S32 mode, S32 available, S32 needed, S32 blockSize, S32 freeFiles, S32 needFile);
void fn_80041C4C(S32 mode, S32 value);
S32 fn_80041BC8(S32 mode, char* dst);
S32 fn_80041B10(S32 mode, char* dst);
S32 fn_80041ADC(S32 mode, char* dst);
S32 fn_80041548(S32 mode, char* dst);
S32 fn_80041310(S32 card, S32 mode);
S32 fn_800411A8(S32 card, S32 repair, S32* files, S32* repaired);
S32 fn_80040FCC(S32 card, S32 game);
}

#define TRC_TITLE "$A170066176Scooby-Doo: Night of 100 Frights$C"
#define TRC_RED "200040046"
#define TRC_GREEN "000210068"

// xTRCInit: the pad records number their pads, and the disc state starts unknown.
void xTRCInit()
{
    memset(lbl_802289B8, 0, sizeof(lbl_802289B8));
    for (S32 i = 0; i < 4; i++)
    {
        lbl_802289B8[i].id = i;
    }
    memset(gTrcDisk, 0, sizeof(gTrcDisk));
}

// Draws the message for a pad or disc state over the screen and pauses the game. A missing
// controller counts only in play. Returns 0 for a state outside the switch (and for a missing
// controller outside play), else 1, including TRC_Unknown, which draws no text.
extern "C" S32 fn_80044478(_tagTRCState state)
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
        else if (fn_800AC0BC())
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
    if (!lbl_8025EBE4)
    {
        if (lbl_8025EBE8)
        {
            camera = iCameraCreate(640, 480);
            RwRGBA clear = { 0, 0, 0, 0 };
            RwCameraClear(camera, &clear, 3);
            RwCameraBeginUpdate(camera);
        }
        lbl_8025EBE4 = xFontBoxCreate(0.0f, 0.0f, 640.0f, 480.0f, 0, xColorFromRGBA(255, 230, 0, 255));
        xFontBoxSetText(lbl_8025EBE4, text);
        xFontBoxSetFont(lbl_8025EBE4, 1);
        xFontBoxSetDrawSize(lbl_8025EBE4, 15.0f, 20.0f);
        xFontBoxSetDrawSpacing(lbl_8025EBE4, 15.0f, 20.0f);
        xFontBoxSetBackdrop(lbl_8025EBE4, xColorFromRGBA(0, 0, 0, 200));
        xFontBoxSetInset(lbl_8025EBE4, 50.0f, 50.0f, 230.0f, 0.0f);
        xFontBoxSetJustification(lbl_8025EBE4, 0x40);
        xFontBoxRender(lbl_8025EBE4);
        xFontBoxDestroy(lbl_8025EBE4);
        lbl_8025EBE4 = NULL;
        if (lbl_8025EBE8)
        {
            RwCameraEndUpdate(camera);
            RwCameraShowRaster(camera, NULL, 1);
            iCameraDestroy(camera);
        }
    }
    return 1;
}

// iPad reports pad 0's state here.
extern "C" void fn_80044458(S32 pad_id, S32 state)
{
    if (pad_id == 0)
    {
        lbl_802289B8[pad_id].state = state;
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
extern "C" S32 fn_800443F8()
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
extern "C" void fn_800443AC()
{
    lbl_8025EBE8 = 1;
    fn_80044478((_tagTRCState)lbl_802289B8[0].state) || fn_80044478(gTrcDisk[0]);
    lbl_8025EBE8 = 0;
}

// Draws the pad message, or else the disc message, into the current frame.
extern "C" void fn_8004436C()
{
    fn_80044478((_tagTRCState)lbl_802289B8[0].state) || fn_80044478(gTrcDisk[0]);
}

// The start-up memory-card check that main runs: the cards in both slots are checked for
// damage, the wrong format, a corrupt save file and room for one, and the checks start over
// when the player retries.
extern "C" void fn_80043E78()
{
    static S32 warned = 0;
    S32 formatted;
    S32 retry;
    S32 pass;
    S32 result;

    lbl_8025EBE0 = 1;
    formatted = 0;
    pass = 0;
    for (;;)
    {
        if (pass++ != 0)
        {
            if (warned)
            {
                RwRGBA clear = { 0, 0, 0, 0 };
                char text[256] = { 0 };
                sprintf(text, "$A%sWARNING!$C Accessing the Nintendo GameCube%s Memory Card.\nPlease do not remove the Memory Card or turn off the power to the Nintendo GameCube%s.\n", TRC_RED, "^", "^");
                xFontBox* box = xFontBoxCreate(0.0f, 0.0f, 640.0f, 480.0f, 1, xColorFromRGBA(255, 230, 0, 255));
                xFontBoxSetText(box, text);
                xFontBoxSetDrawSize(box, 18.0f, 24.0f);
                xFontBoxSetDrawSpacing(box, 17.0f, 24.0f);
                xFontBoxSetBackdrop(box, xColorFromRGBA(0, 0, 0, 255));
                xFontBoxSetInset(box, 100.0f, 100.0f, 100.0f, 0.0f);
                xFontBoxSetJustification(box, 0x40);
                RwCamera* camera = iCameraCreate(640, 480);
                RwCameraClear(camera, &clear, 3);
                RwCameraBeginUpdate(camera);
                xFontBoxRender(box);
                RwCameraEndUpdate(camera);
                RwCameraShowRaster(camera, NULL, 1);
                iCameraDestroy(camera);
                xFontBoxDestroy(box);
            }
            else
            {
                RwRGBA clear = { 0, 0, 0, 0 };
                RwCamera* camera = iCameraCreate(640, 480);
                RwCameraClear(camera, &clear, 3);
                RwCameraBeginUpdate(camera);
                RwCameraEndUpdate(camera);
                RwCameraShowRaster(camera, NULL, 1);
                iCameraDestroy(camera);
            }
            for (S32 i = 14; i >= 0; i--)
            {
                fn_80016600();
                iVSync();
            }
        }

        retry = 0;
        formatted = 0;
        result = fn_80043940();
        if (result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
            goto next;
        }

        result = fn_80043BA4(0);
        if (result == 3)
        {
            break;
        }
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        result = fn_80043730(0);
        if (result == 3)
        {
            break;
        }
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        result = fn_80043050(0, &formatted);
        if (result == 3)
        {
            break;
        }
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        if (result == 1 && !formatted)
        {
            goto slot1;
        }
        result = fn_80042A48(0);
        if (result == 3)
        {
            break;
        }
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        result = fn_80042774(0);
        if (result == 3)
        {
            break;
        }
        if (result == 2)
        {
            goto slot1;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        result = fn_800423EC(0);
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
            goto next;
        }
        if (result == 1)
        {
            break;
        }

    slot1:
        do
        {
            result = fn_80043BA4(1);
        } while (result == 2);
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
            goto next;
        }
        result = fn_80043730(1);
        if (result == 2 || result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        result = fn_80043050(1, &formatted);
        if (result == 2 || result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        if (result == 1 && !formatted)
        {
            break;
        }
        result = fn_80042A48(1);
        if (result == 2 || result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        result = fn_80042774(1);
        if (result == 2 || result == 3)
        {
            break;
        }
        if (result == 4)
        {
            retry = 1;
        }
        if (retry)
        {
            goto next;
        }
        result = fn_800423EC(1);
        if (result == 4)
        {
            retry = 1;
        }

    next:
        if (!retry)
        {
            break;
        }
    }

    RwRGBA clear = { 0, 0, 0, 0 };
    RwCamera* camera = iCameraCreate(640, 480);
    RwCameraClear(camera, &clear, 3);
    RwCameraBeginUpdate(camera);
    RwCameraEndUpdate(camera);
    RwCameraShowRaster(camera, NULL, 1);
    iCameraDestroy(camera);
    lbl_8025EBE0 = 0;
}

// A message box's card watch: true once the card in the watched slot is not the one the
// check started with.
extern "C" S32 fn_80043E20(TRCCardWatch* watch)
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

// The start-up check's hook for a card it cannot use; 2 lets the check ask the player.
extern "C" S32 fn_80043E18(S32 card, st_ISGSESSION* session)
{
    return 2;
}

// Checks that a card is in the slot; a card that is not ready says why.
extern "C" S32 fn_80043BA4(S32 card)
{
    S32 result = 1;
    char text[512] = { 0 };
    st_ISGSESSION* session = iSGSessionBegin(NULL, NULL);
    U32 state = fn_80014310(session, card);

    if (card != 0 && !(state & 1))
    {
        fn_80041CE8(0, card, card);
        fn_80041CC4(0, 24);
        fn_80041548(0, text);
        const char* options[4] = {};
        options[0] = "Continue Without Saving";
        options[1] = "Retry";
        S32 choice = fn_80041D90(text, options, NULL, NULL);
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
        S32 hook = fn_80043E18(card, session);
        if (hook != 2)
        {
            result = hook;
        }
        else
        {
            fn_80041CE8(0, card, card);
            fn_80041CC4(0, 27);
            fn_80041548(0, text);
            const char* options[4] = {};
            options[0] = "Continue Without Saving";
            options[1] = "Retry";
            S32 choice = fn_80041D90(text, options, NULL, NULL);
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
extern "C" S32 fn_80043940()
{
    S32 result;
    char text[512] = { 0 };
    st_ISGSESSION* session = iSGSessionBegin(NULL, NULL);
    S32 stateA = fn_80014310(session, 0);
    S32 stateB = fn_80014310(session, 1);

    if ((stateA & 1) || (stateB & 1))
    {
        result = 1;
    }
    else if (!(stateA & 1) && !(stateB & 1))
    {
        fn_80041CE8(0, 0, 0);
        fn_80041CC4(0, 25);
        fn_80041548(0, text);
        const char* options[4] = {};
        options[0] = "Continue Without Saving";
        options[1] = "Retry";
        S32 choice = fn_80041D90(text, options, NULL, NULL);
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
        fn_80041CE8(0, 0, 0);
        fn_80041CC4(0, 25);
        fn_80041548(0, text);
        const char* options[4] = {};
        options[0] = "Continue Without Saving";
        options[1] = "Retry";
        S32 choice = fn_80041D90(text, options, NULL, NULL);
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
extern "C" S32 fn_80043730(S32 card)
{
    S32 result = 1;
    char text[512] = { 0 };
    st_ISGSESSION* session = iSGSessionBegin(NULL, NULL);
    S32 slot = -1;
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
        U32 state = fn_80013D3C(session, slot, 0);
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
            S32 hook = fn_80043E18(card, session);
            if (hook != 2)
            {
                result = hook;
            }
            else
            {
                fn_80041CE8(0, slot, card);
                fn_80041CC4(0, message);
                fn_80041548(0, text);
                const char* options[4] = {};
                options[0] = "Continue Without Saving";
                options[1] = "Retry";
                S32 choice = fn_80041D90(text, options, NULL, NULL);
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

// Draws one memory-card message as a single frame, with no buttons.
static inline void TRCShowCardMessage(char* text)
{
    RwRGBA clear = { 0, 0, 0, 0 };
    char unused[256] = { 0 };
    xFontBox* box = xFontBoxCreate(0.0f, 0.0f, 640.0f, 480.0f, 1, xColorFromRGBA(255, 230, 0, 255));
    xFontBoxSetText(box, text);
    xFontBoxSetDrawSize(box, 18.0f, 24.0f);
    xFontBoxSetDrawSpacing(box, 17.0f, 24.0f);
    xFontBoxSetBackdrop(box, xColorFromRGBA(0, 0, 0, 255));
    xFontBoxSetInset(box, 100.0f, 100.0f, 100.0f, 0.0f);
    xFontBoxSetJustification(box, 0x40);
    RwCamera* camera = iCameraCreate(640, 480);
    RwCameraClear(camera, &clear, 3);
    RwCameraBeginUpdate(camera);
    xFontBoxRender(box);
    RwCameraEndUpdate(camera);
    RwCameraShowRaster(camera, NULL, 1);
    iCameraDestroy(camera);
    xFontBoxDestroy(box);
}

// Checks that the card is formatted for this market, and offers to format it.
extern "C" S32 fn_80043050(S32 card, S32* formatted)
{
    S32 result = 1;
    TRCCardWatch watch = { 0 };
    watch.card = card;
    char text[512] = { 0 };
    S32 slot = -1;

    if (formatted)
    {
        *formatted = 0;
    }
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
        iSGTgtSetActive(session, slot);
        U32 state = fn_80013D3C(session, slot, 0);
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
        U32 state = fn_80013D3C(session, slot, 0);
        U32 market = state & 0x4000000;
        S32 hook = 2;
        watch.session = session;
        watch.card = card;
        watch.slot = slot;
        if (market || !(state & 2))
        {
            hook = fn_80043E18(card, session);
            if (hook != 2)
            {
                result = hook;
            }
        }
        if (hook == 2 && (market || !(state & 2)))
        {
            fn_80041CE8(0, slot, card);
            if (market)
            {
                fn_80041CC4(0, 17);
            }
            else
            {
                fn_80041CC4(0, 16);
            }
            fn_80041548(0, text);
            const char* options[4] = { "Continue Without Saving", "Retry", "Format Memory Card" };
            switch (fn_80041D90(text, options, fn_80043E20, &watch))
            {
            case -1:
                result = 4;
                break;
            case 1:
                result = 4;
                break;
            case 0:
                result = 1;
                break;
            case 2:
            {
                fn_80041CE8(0, slot, card);
                fn_80041CC4(0, 18);
                fn_80041548(0, text);
                const char* confirm[4] = {};
                confirm[0] = "No";
                confirm[1] = "Yes";
                S32 choice = fn_80041D90(text, confirm, fn_80043E20, &watch);
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
                    fn_80041CE8(0, slot, card);
                    fn_80041CC4(0, 19);
                    fn_80041548(0, text);
                    TRCShowCardMessage(text);
                    S32 ok = iSGTgtFormat(session, slot, 0, &canRecover);
                    for (S32 i = 59; i >= 0; i--)
                    {
                        fn_80016600();
                        iVSync();
                    }
                    fn_80041CE8(0, slot, card);
                    if (ok)
                    {
                        result = 1;
                        fn_80041CC4(0, 20);
                        if (formatted)
                        {
                            *formatted = 1;
                        }
                    }
                    else if (canRecover)
                    {
                        result = 4;
                        fn_80041CC4(0, 21);
                    }
                    else
                    {
                        result = 4;
                        fn_80041CC4(0, 22);
                    }
                    fn_80041548(0, text);
                    if (ok)
                    {
                        const char* done[4] = {};
                        done[0] = NULL;
                        done[1] = "Continue";
                        choice = fn_80041D90(text, done, fn_80043E20, &watch);
                    }
                    else
                    {
                        const char* done[4] = {};
                        done[0] = NULL;
                        done[1] = "Continue";
                        choice = fn_80041D90(text, done, NULL, NULL);
                    }
                    if (choice == -1)
                    {
                        result = 4;
                    }
                }
                break;
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
extern "C" S32 fn_80042A48(S32 card)
{
    S32 result = 4;
    TRCCardWatch watch = { 0 };
    watch.card = card;
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
        iSGTgtSetActive(session, slot);
        watch.session = session;
        watch.card = card;
        S32 hook = 2;
        watch.slot = slot;
        S32 corrupt = -1;
        S32 repaired = -1;
        S32 ok = fn_800132C4(session, 0, &corrupt, NULL);
        if (!ok)
        {
            hook = fn_80043E18(card, session);
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
            fn_80041CE8(0, slot, card);
            fn_80041CC4(0, 9);
            fn_80041C4C(0, corrupt);
            fn_80041548(0, text);
            switch (fn_80041D90(text, options, fn_80043E20, &watch))
            {
            case -1:
                result = 4;
                break;
            case 1:
                result = 4;
                break;
            case 0:
                result = 3;
                break;
            case 2:
            {
                fn_80041CE8(0, slot, card);
                fn_80041CC4(0, 10);
                fn_80041548(0, text);
                const char* confirm[4] = {};
                confirm[0] = "No";
                confirm[1] = "Yes";
                switch (fn_80041D90(text, confirm, fn_80043E20, &watch))
                {
                case -1:
                    result = 4;
                    break;
                case 0:
                    result = 4;
                    break;
                case 1:
                {
                    fn_80041CE8(0, slot, card);
                    fn_80041CC4(0, 13);
                    fn_80041548(0, text);
                    TRCShowCardMessage(text);
                    fn_80041CE8(0, slot, card);
                    if (fn_800132C4(session, 1, NULL, &repaired) == 0)
                    {
                        result = 1;
                        fn_80041CC4(0, 11);
                    }
                    else if (repaired >= 0)
                    {
                        result = 4;
                        fn_80041CC4(0, 11);
                    }
                    else
                    {
                        result = 4;
                        fn_80041CC4(0, 12);
                    }
                    fn_80041548(0, text);
                    const char* done[4] = {};
                    done[0] = NULL;
                    done[1] = "Continue";
                    if (fn_80041D90(text, done, fn_80043E20, &watch) == -1)
                    {
                        result = 4;
                    }
                    break;
                }
                default:
                    result = 3;
                    break;
                }
                break;
            }
            default:
                result = 3;
                break;
            }
        }
    }
    iSGSessionEnd(session);
    return result;
}

// Checks that the card has room for a save file, and offers to manage the card.
extern "C" S32 fn_80042774(S32 card)
{
    S32 result = 1;
    S32 reset = 0;
    TRCCardWatch watch = { 0 };
    watch.card = card;
    S32 needed = 0;
    S32 available = 0;
    S32 blockSize = 0;
    S32 needFile = 0;
    S32 freeFiles = 0;
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
        iSGTgtSetActive(session, slot);
        watch.session = session;
        watch.card = card;
        watch.slot = slot;
        fn_80013C64(session, slot, &blockSize, &freeFiles);
        S32 hook = 2;
        S32 room = iSGTgtHaveRoom(session, slot, 0x6000, NULL, NULL, &needed, &available, &needFile);
        if (!room)
        {
            hook = fn_80043E18(card, session);
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
            fn_80041CE8(0, slot, card);
            fn_80041CC4(0, 32);
            fn_80041C78(0, available, needed, blockSize, freeFiles, needFile);
            fn_80041548(0, text);
            switch (fn_80041D90(text, options, fn_80043E20, &watch))
            {
            case -1:
                result = 4;
                break;
            case 1:
                result = 4;
                break;
            case 0:
                result = 3;
                break;
            case 2:
                reset = 1;
                result = 4;
                break;
            default:
                result = 3;
                break;
            }
        }
    }
    iSGSessionEnd(session);
    if (reset)
    {
        fn_80016698(1);
    }
    return result;
}

// Checks that one of the three save files exists or can be created.
extern "C" S32 fn_800423EC(S32 card)
{
    S32 result = 1;
    S32 reset = 0;
    TRCCardWatch watch = { 0 };
    watch.card = card;
    S32 needed = 0;
    S32 available = 0;
    S32 blockSize = 0;
    S32 needFile = 0;
    S32 freeFiles = 0;
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
        iSGTgtSetActive(session, slot);
        watch.session = session;
        watch.card = card;
        watch.slot = slot;
        fn_80013C64(session, slot, &blockSize, &freeFiles);
        S32 missing = 0;
        S32 first = -1;
        S32 hook = 2;
        S32 sizes[3] = { 0 };
        S32 found = fn_800138EC(session, NULL, NULL);
        if (found >= 1 && found != 3)
        {
            S32 i;
            for (i = 0; i < 3; i++)
            {
                sizes[i] = iSGFileSize(session, iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, i));
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
                    hook = fn_80043E18(card, session);
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
                    fn_80041CE8(0, slot, card);
                    fn_80041CC4(0, 33);
                    fn_80041C78(0, available, needed, blockSize, freeFiles, needFile);
                    fn_80041548(0, text);
                    switch (fn_80041D90(text, options, fn_80043E20, &watch))
                    {
                    case -1:
                        result = 4;
                        break;
                    case 1:
                        result = 4;
                        break;
                    case 0:
                        result = 3;
                        break;
                    case 2:
                        reset = 1;
                        result = 4;
                        break;
                    default:
                        result = 3;
                        break;
                    }
                }
            }
        }
    }
    iSGSessionEnd(session);
    if (reset)
    {
        fn_80016698(1);
    }
    return result;
}

// Shows a message box with up to four buttons (A, B, X, Y, in options order: B, A, X, Y) and
// waits for one, or for the check callback to report a change (-1). Returns the button's
// option index. With no text it only clears the screen.
extern "C" S32 fn_80041D90(char* text, const char** options, TRCCardCheck check, TRCCardWatch* watch)
{
    RwRGBA clear = { 0, 0, 0, 0 };
    S32 choice = 0;
    xFontBox* buttons2 = NULL;
    char buttons[256] = { 0 };
    char buttonsLow[256] = { 0 };
    S32 done = 0;

    if (text == NULL)
    {
        RwRGBA black = { 0, 0, 0, 0 };
        RwCamera* camera = iCameraCreate(640, 480);
        RwCameraClear(camera, &black, 3);
        RwCameraBeginUpdate(camera);
        RwCameraEndUpdate(camera);
        RwCameraShowRaster(camera, NULL, 1);
        iCameraDestroy(camera);
        return 0;
    }

    S32 count;
    U32 mask;
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

    xFontBox* box = xFontBoxCreate(0.0f, 0.0f, 640.0f, 360.0f, 1, xColorFromRGBA(255, 230, 0, 255));
    xFontBox* buttons1 = xFontBoxCreate(0.0f, 360.0f, 640.0f, 100.0f, 1, xColorFromRGBA(255, 230, 0, 255));
    if (count > 2)
    {
        buttons2 = xFontBoxCreate(0.0f, 400.0f, 640.0f, 100.0f, 1, xColorFromRGBA(255, 230, 0, 255));
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

    RwCamera* camera = iCameraCreate(640, 480);
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
        fn_80016600();
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
extern "C" void fn_80041D58(S32 mode)
{
    memset(&lbl_801B6610[mode], 0, sizeof(TRCCardMessage));
}

extern "C" void fn_80041CE8(S32 mode, S32 card, S32 slot)
{
    TRCCardMessage* msg = &lbl_801B6610[mode];
    memset(msg, 0, sizeof(TRCCardMessage));
    msg->card = card;
    msg->slot = slot;
    if (card == 0 || card == 1)
    {
        msg->flags |= 1;
    }
}

extern "C" void fn_80041CC4(S32 mode, S32 message)
{
    TRCCardMessage* msg = &lbl_801B6610[mode];
    msg->flags |= 2;
    msg->message = message;
}

extern "C" void fn_80041C78(S32 mode, S32 available, S32 needed, S32 blockSize, S32 freeFiles, S32 needFile)
{
    TRCCardMessage* msg = &lbl_801B6610[mode];
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

extern "C" void fn_80041C4C(S32 mode, S32 value)
{
    TRCCardMessage* msg = &lbl_801B6610[mode];
    if (msg->flags)
    {
        msg->flags |= 8;
        msg->fileIndex = value;
    }
}

// The label of the message box's B button.
extern "C" S32 fn_80041BC8(S32 mode, char* dst)
{
    switch (lbl_801B6610[mode].message)
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
extern "C" S32 fn_80041B10(S32 mode, char* dst)
{
    switch (lbl_801B6610[mode].message)
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

extern "C" S32 fn_80041ADC(S32 mode, char* dst)
{
    return sprintf(dst, "%s", "--error--");
}

// The text of the current memory-card message.
extern "C" S32 fn_80041548(S32 mode, char* dst)
{
    TRCCardMessage* msg = &lbl_801B6610[mode];
    char device[64] = { 0 };
    char cardName[64] = { 0 };
    char slotName[32] = { 0 };
    S32 blocks;

    sprintf(device, fn_800389FC(0, msg->slot, 2));
    sprintf(slotName, fn_800389FC(0, msg->slot, 3));
    sprintf(cardName, fn_800389FC(0, msg->slot, 4));

    switch (msg->message)
    {
    case 1:
        return sprintf(dst, "$A%sWARNING!$C\nSaving. Do not touch the %s or the %s.\n", TRC_RED, cardName, "POWER Button");
    case 2:
        return sprintf(dst, "$A%sSave Successful.$C\n", TRC_GREEN);
    case 3:
        return sprintf(dst, "$A%sSave Failed!$C\nCheck the %s and try again.", TRC_RED, cardName);
    case 4:
        blocks = (msg->needed + (msg->blockSize - 1)) / msg->blockSize;
        return sprintf(dst, "$A%sSave Failed!$C\nNot enough space on the %s to create a new %s file.\nAt least $A%s%d file%s and %d block%s required.\n", TRC_RED, cardName, TRC_TITLE, TRC_GREEN, msg->needFile, msg->needFile == 1 ? "" : "s", blocks, blocks == 1 ? "$C is" : "s$C are");
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
        return sprintf(dst, "Wrong device in %s.\n\nPlease insert a\n%s.\n", slotName, device);
    case 28:
        return sprintf(dst, "The %s is not compatible with the %s save file.", cardName, TRC_TITLE);
    case 29:
        return sprintf(dst, "The %s is formatted for a market not compatible with the save file format of %s.\nTo avoid accidental data loss in this situation, reformatting is not supported.\nPlease review the %s Nintendo GameCube%s Instruction Booklet for more information.\n", cardName, TRC_TITLE, cardName, "^");
    case 30:
        return sprintf(dst, "The %s appears to be damaged and cannot be used.\n", cardName);
    case 31:
        return sprintf(dst, "The %s is not compatible with the save file format of\n%s.\n", cardName, TRC_TITLE);
    case 32:
        blocks = (msg->needed + (msg->blockSize - 1)) / msg->blockSize;
        return sprintf(dst, "Not enough space on the %s.\nAt least $A%s%d file%s and %d block%s required.\n", cardName, TRC_GREEN, msg->needFile, msg->needFile == 1 ? "" : "s", blocks, blocks == 1 ? "$C is" : "s$C are");
    case 33:
        blocks = (msg->needed + (msg->blockSize - 1)) / msg->blockSize;
        return sprintf(dst, "Not enough space on the %s to create a new %s file.\nAt least $A%s%d file%s and %d block%s required.\nExisting files may be loaded and overwritten.\n", cardName, TRC_TITLE, TRC_GREEN, msg->needFile, msg->needFile == 1 ? "" : "s", blocks, blocks == 1 ? "$C is" : "s$C are");
    default:
        return sprintf(dst, "$A255050080<<internal error>>$C");
    }
}

// Checks the card in zSaveLoad's slot; a missing, damaged or incompatible card sets the
// message. Returns 0 when it did, else 1 (with mode set, before reading the card).
extern "C" S32 fn_80041310(S32 card, S32 mode)
{
    S32 result = 1;
    S32 slot = -1;
    st_ISGSESSION* session;
    U32 state;
    S32 message = 0;
    S32 count;

    session = iSGSessionBegin(NULL, NULL);
    state = fn_80014310(session, card);
    if ((state & 1) && !(state & 2))
    {
        fn_80041CE8(1, card, card);
        fn_80041CC4(1, 27);
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
        fn_80041CE8(1, card, card);
        fn_80041CC4(1, 24);
        return 0;
    }
    iSGTgtSetActive(session, slot);
    state = fn_80013D3C(session, slot, 0);
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
        fn_80041CE8(1, card, card);
        fn_80041CC4(1, message);
    }
    iSGSessionEnd(session);
    return result;
}

// Checks the card for corrupt files, or deletes them (repair). Returns 0 on success, -1
// when the card is missing or cannot be selected, else 1.
extern "C" S32 fn_800411A8(S32 card, S32 repair, S32* files, S32* repaired)
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
        S32 err = fn_800132C4(session, 1, NULL, &left);
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
    else if (fn_800132C4(session, 0, files, NULL) == 0)
    {
        result = 0;
    }
    iSGSessionEnd(session);
    return result;
}

// Checks that zSaveLoad's card is usable and has room for the game's save file: -1 when it
// is missing or not usable, 0 when it is full (the message names the space needed), else 1.
extern "C" S32 fn_80040FCC(S32 card, S32 game)
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
        U32 state = fn_80013D3C(session, slot, 0);
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
            fn_80013C64(session, slot, &blockSize, &freeFiles);
            char* fname = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, game);
            if (iSGTgtHaveRoom(session, slot, 0x6000, NULL, fname, &needed, &available, &needFile))
            {
                result = 1;
            }
            else
            {
                result = 0;
                fn_80041CC4(1, 4);
                fn_80041C78(1, available, needed, blockSize, freeFiles, needFile);
            }
        }
    }
    iSGSessionEnd(session);
    return result;
}
