#include "isavegame.h"
#include "iFile.h"

#include <dolphin/card.h>
#include <dolphin/os.h>

#include <stdio.h>
#include <string.h>

// The texture palette file layout of the Dolphin SDK's TPL format. The icon and banner
// files are loaded whole, and their offsets are turned into pointers in place.
struct TPLHeader
{
    U16 height;
    U16 width;
    U32 format;
    void* data;
    U32 wrapS;
    U32 wrapT;
    U32 minFilter;
    U32 magFilter;
    F32 LODBias;
    U8 edgeLODEnable;
    U8 minLOD;
    U8 maxLOD;
    U8 unpacked;
};

struct TPLClutHeader
{
    U16 numEntries;
    U8 unpacked;
    U8 pad8;
    U32 format;
    void* data;
};

struct TPLDescriptor
{
    TPLHeader* textureHeader;
    TPLClutHeader* CLUTHeader;
};

struct TPLPalette
{
    U32 versionNumber;
    U32 numDescriptors;
    TPLDescriptor* descriptorArray;
};

extern "C" void fn_80016768();

static S32 g_isginit;
static TPLPalette* g_rawicon;
static TPLPalette* g_rawbanr;
static U32 g_iconsize;
static U32 g_banrsize;

static st_ISGSESSION g_isgdata_MAIN = { 0 };

S32 iSG_mcidx2slot(S32 mcidx, S32* slot, S32* present);

// The card in this slot was removed: it is no longer the active target and its state is
// cleared.
static void iSG_cb_unmount(s32 chan, s32 result)
{
    st_ISGSESSION* session = &g_isgdata_MAIN;
    if (chan == session->slot)
    {
        session->slot = -1;
    }
    memset(&session->mcdata[chan], 0, sizeof(st_ISG_MEMCARD_DATA));
}

static S32 iSG_mc_unmount(S32 slot)
{
    S32 rc = 0;
    S32 result;

    do
    {
        result = CARDUnmount(slot);
    } while (result == CARD_RESULT_BUSY);

    if (result == CARD_RESULT_READY)
    {
        rc = 1;
    }
    else if (result == CARD_RESULT_NOCARD)
    {
        rc = 1;
    }

    return rc;
}

static S32 iSG_tpl_unpack(TPLPalette* tpl)
{
    if (tpl->versionNumber != 0x20AF30)
    {
        return 0;
    }

    tpl->descriptorArray = (TPLDescriptor*)((U32)tpl + (U32)tpl->descriptorArray);
    for (S32 i = 0; i < tpl->numDescriptors; ++i)
    {
        TPLDescriptor* desc = &tpl->descriptorArray[i];
        if (desc->textureHeader != NULL)
        {
            desc->textureHeader = (TPLHeader*)((U32)tpl + (U32)desc->textureHeader);
            if (desc->textureHeader->unpacked == 0)
            {
                desc->textureHeader->data = (void*)((U32)tpl + (U32)desc->textureHeader->data);
                desc->textureHeader->unpacked = 1;
            }
        }
        if (desc->CLUTHeader != NULL)
        {
            desc->CLUTHeader = (TPLClutHeader*)((U32)tpl + (U32)desc->CLUTHeader);
            if (desc->CLUTHeader->unpacked == 0)
            {
                desc->CLUTHeader->data = tpl;
                desc->CLUTHeader->unpacked = 1;
            }
        }
    }
    return 1;
}

// Returns 1 for a usable card, 2 for a broken one and 3 for one in the wrong encoding;
// *badEncode and *ioError report why a card is unusable.
static S32 iSG_mc_isGCcard(st_ISG_MEMCARD_DATA* mcdata, S32* badEncode, S32* ioError)
{
    S32 result = 0;
    S32 rc = 0;

    s32 xferBytes = 0;
    u16 encoding = 0;
    s32 memSize = 0;
    s32 sectorSize = 0;

    if (badEncode)
    {
        *badEncode = 0;
    }

    if (ioError)
    {
        *ioError = 0;
    }

    if (mcdata->unk_0 == 0)
    {
        return 0;
    }
    if (mcdata->unk_12C)
    {
        return 0;
    }

    do
    {
        result = CARDProbeEx(mcdata->chan, &memSize, &sectorSize);
    } while (result == CARD_RESULT_BUSY);

    if (result == CARD_RESULT_READY)
    {
        rc = 1;
    }

    if (rc != 0)
    {
        do
        {
            result = CARDCheckEx(mcdata->chan, &xferBytes);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY)
        {
            rc = 1;
        }
        else if (result == CARD_RESULT_BROKEN)
        {
            rc = 2;
        }
        else
        {
            if (result == CARD_RESULT_ENCODING)
            {
                rc = 3;
                if (badEncode)
                {
                    *badEncode = 1;
                }
            }
            else
            {
                rc = 0;
                if (result == CARD_RESULT_ENCODING && badEncode)
                {
                    *badEncode = 1;
                }
                if (result == CARD_RESULT_IOERROR && ioError)
                {
                    *ioError = 1;
                }
            }
        }
    }

    if (rc == 1)
    {
        do
        {
            result = CARDGetEncoding(mcdata->chan, &encoding);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY && encoding && badEncode)
        {
            *badEncode = 1;
        }
    }

    return rc;
}

// Maps the mcidx'th card present to its physical slot. present, when given, receives one
// flag per slot.
S32 iSG_mcidx2slot(S32 mcidx, S32* slot, S32* present)
{
    s32 cardReady[ISG_NUM_SLOTS] = {};
    S32 ret = 0;
    S32 found = 0;
    s32 memSize = 0;
    s32 sectorSize = 0;
    *slot = -1;

    for (S32 i = 0; i < ISG_NUM_SLOTS; i++)
    {
        s32 result;
        do
        {
            result = CARDProbeEx(i, &memSize, &sectorSize);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY)
        {
            cardReady[i] = TRUE;
        }
    }

    for (S32 i = 0; i < ISG_NUM_SLOTS; i++)
    {
        if (cardReady[i])
        {
            if (found == mcidx)
            {
                *slot = i;
                ret = 1;
                break;
            }
            found++;
        }
    }

    if (present)
    {
        for (S32 i = 0; i < ISG_NUM_SLOTS; i++)
        {
            present[i] = cardReady[i];
        }
    }

    return ret;
}

en_ASYNC_OPSTAT iSGPollStatus(st_ISGSESSION* isgdata, en_ASYNC_OPCODE* curop, S32 block)
{
    return isgdata->as_opstat;
}

en_ASYNC_OPERR iSGOpError(st_ISGSESSION* isgdata, char* errmsg)
{
    static char* errmsgs[ISG_OPERR_NOMORE + 1] = {
        "No current error",
        "No operation in async queue",
        "Too many async ops queued simultaneously",
        "Init Failed",
        "Unable to access Save Game Directory",
        "Access Error - no card ?!? (eg yanked out)",
        "Access Error - no room on card (file handles free bytes, etc)",
        "Access Error - card is damaged or something bad",
        "Access Error - file being loaded appears to be corrupt (I-Level)",
        "Access Error - general problem",
        "Save Error - Not enough free space to save file",
        "Save Error - during initalization (async queue)",
        "Save Error - during write",
        "Save Error - opening file",
        "Load Error - during initalization (async queue)",
        "Load Error - during read",
        "Load Error - opening file",
        "Target problem (general error)",
        "Target Error - media removed or changed",
        "Target Error - Not ready for I/O (unformatted?)",
        "Operation encountered unknown error",
        NULL
    };

    if (errmsg == NULL)
    {
        return isgdata->as_operr;
    }

    if (isgdata->as_operr < ISG_OPERR_NOMORE)
    {
        strncpy(errmsg, errmsgs[isgdata->as_operr], 0x80);
    }
    else
    {
        strncpy(errmsg, errmsgs[ISG_OPERR_UNKNOWN], 0x80);
    }
    errmsg[0x7F] = NULL;
    return isgdata->as_operr;
}

// The GameCube needs no game directory setup.
S32 iSGSetupGameDir(st_ISGSESSION* isgdata, const char* dname, S32 force_iconfix)
{
    return 1;
}

void iSGMakeTimeStamp(char* str)
{
    OSCalendarTime calendar_time = { 0 };
    OSTime time = OSGetTime();
    OSTicksToCalendarTime(time, &calendar_time);
    sprintf(str, "%02d/%02d/%04d %02d:%02d:%02d", calendar_time.mon + 1, calendar_time.mday,
            calendar_time.year, calendar_time.hour, calendar_time.min, calendar_time.sec);
}

S32 iSGTgtPhysSlotIdx(st_ISGSESSION* isgdata, S32 tidx)
{
    S32 idx = -1;
    if (iSG_mcidx2slot(tidx, &idx, NULL))
    {
        return idx;
    }
    return -1;
}

S32 iSGTgtCount(st_ISGSESSION* isgdata, S32* max)
{
    s32 memSize = 0;
    s32 sectorSize = 0;
    S32 ret = 0;

    if (max != NULL)
    {
        *max = ISG_NUM_SLOTS;
    }

    for (S32 i = 0; i < ISG_NUM_SLOTS; ++i)
    {
        fn_80016768();
        s32 result;
        do
        {
            result = CARDProbeEx(i, &memSize, &sectorSize);
        } while (result == CARD_RESULT_BUSY);
        if (result == CARD_RESULT_READY)
        {
            ret++;
        }
    }

    return ret;
}

// Unmounts every mounted card and clears the session.
void iSGSessionEnd(st_ISGSESSION* isgdata)
{
    fn_80016768();
    for (S32 i = 0; i < ISG_NUM_SLOTS; i++)
    {
        if (isgdata->mcdata[i].unk_0)
        {
            iSG_mc_unmount(i);
            isgdata->mcdata[i].unk_0 = 0;
        }
    }

    memset(isgdata, 0, sizeof(st_ISGSESSION));
}

// Both arguments are ignored on this platform.
st_ISGSESSION* iSGSessionBegin(void* cltdata, void (*chgfunc)(void*, en_CHGCODE))
{
    fn_80016768();
    memset(&g_isgdata_MAIN, 0, sizeof(st_ISGSESSION));
    g_isgdata_MAIN.slot = -1;
    return &g_isgdata_MAIN;
}

static void iSG_discard_icondata()
{
    OSFreeToHeap(__OSCurrHeap, g_rawicon);
    OSFreeToHeap(__OSCurrHeap, g_rawbanr);
    g_rawicon = NULL;
    g_iconsize = 0;
    g_rawbanr = NULL;
    g_banrsize = 0;
}

S32 iSGShutdown()
{
    iSG_discard_icondata();
    return 1;
}

static S32 iSG_start_your_engines()
{
    CARDInit();
    return 1;
}

// Loads the save file's icon and banner textures.
static S32 iSG_load_icondata()
{
    g_rawicon = (TPLPalette*)iFileLoad("/SD2GCIcon.tpl", NULL, &g_iconsize);
    g_rawbanr = (TPLPalette*)iFileLoad("/SD2GCBanner.tpl", NULL, &g_banrsize);
    iSG_tpl_unpack(g_rawicon);
    iSG_tpl_unpack(g_rawbanr);

    return g_rawicon && (S32)g_iconsize && g_rawbanr && (S32)g_banrsize ? 1 : 0;
}

// Only the first call starts the card library; every call returns the call count.
S32 iSGStartup()
{
    if (g_isginit++ != 0)
    {
        return g_isginit;
    }

    iSG_start_your_engines();
    iSG_load_icondata();
    return g_isginit;
}
