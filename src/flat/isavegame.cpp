#include "isavegame.h"
#include "iFile.h"
#include "xMemMgr.h"
#include "xSnd.h"
#include "zGlobals.h"

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

// The save file's header block: the game title, the save's label, the banner and four icon
// frames, then the signature that marks the file as this game's.
struct st_ISG_ICONDATA
{
    char game_name[0x20];
    char label[0x20];
    char banner[0x1800];
    char icons[4][0x800];
    char pad[0x200];
    char footer[0x200];
};

static char cardwork[ISG_NUM_SLOTS][0xA000];

static void iSG_cb_asyndone(s32, s32);

// Writes len bytes at the open file's write position, keeping the sound system running while
// the card works.
static S32 iSG_mc_fwrite(st_ISG_MEMCARD_DATA* mcdata, char* buf, S32 len)
{
    S32 ret = 0;
    S32 x = 1000;
    CARDStat* fstat = &mcdata->fstat;
    s32 result;
    do
    {
        result = CARDGetResultCode(mcdata->chan);
    } while (result == CARD_RESULT_BUSY);

    if (result != CARD_RESULT_READY)
    {
        return 0;
    }

    CARDGetXferredBytes(mcdata->chan);
    s32 asynresult;
    do
    {
        asynresult = CARDWriteAsync(&mcdata->finfo, buf, len, mcdata->unk_98, iSG_cb_asyndone);
    } while (asynresult == CARD_RESULT_BUSY);

    if (asynresult != CARD_RESULT_READY)
    {
        return 0;
    }

    result = CARD_RESULT_BUSY;
    do
    {
        if (x++ > 500)
        {
            result = CARDGetResultCode(mcdata->chan);
            xSndUpdate(globals.update_dt);
            CARDGetXferredBytes(mcdata->chan);
            x = 0;
        }
        fn_80016768();
    } while (result == CARD_RESULT_BUSY);

    if (asynresult == CARD_RESULT_READY)
    {
        mcdata->unk_98 += len;

        do
        {
            result = CARDGetStatus(mcdata->chan, mcdata->finfo.fileNo, fstat);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY)
        {
            ret = 1;
        }
    }
    return ret;
}

// Reads len bytes at offset, keeping the sound system running while the card works.
static S32 iSG_mc_fread(st_ISG_MEMCARD_DATA* mcdata, char* buf, S32 len, S32 offset)
{
    S32 x = 1000;
    s32 result = CARDGetResultCode(mcdata->chan);
    do
    {
        result = CARDGetResultCode(mcdata->chan);
    } while (result == CARD_RESULT_BUSY);

    if (result != CARD_RESULT_READY)
    {
        return 0;
    }

    CARDGetXferredBytes(mcdata->chan);
    do
    {
        result = CARDReadAsync(&mcdata->finfo, buf, len, offset, iSG_cb_asyndone);
    } while (result == CARD_RESULT_BUSY);

    if (result != CARD_RESULT_READY)
    {
        return 0;
    }

    result = CARD_RESULT_BUSY;
    do
    {
        if (x++ > 500)
        {
            xSndUpdate(globals.update_dt);
            CARDGetXferredBytes(mcdata->chan);
            result = CARDGetResultCode(mcdata->chan);
            x = 0;
        }
        fn_80016768();
    } while (result == CARD_RESULT_BUSY);
    return result != 0 ? 0 : 1;
}

// The completion callback for card reads and writes; the callers poll for the result instead.
static void iSG_cb_asyndone(s32, s32)
{
}

static S32 iSG_mc_fdel(st_ISG_MEMCARD_DATA* mcdata, const char* fname)
{
    s32 result;
    do
    {
        result = CARDDelete(mcdata->chan, fname);
    } while (result == CARD_RESULT_BUSY);

    S32 ret;
    if (result == CARD_RESULT_NOFILE)
    {
        ret = 1;
    }
    else if (result == CARD_RESULT_READY)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
        switch (result)
        {
        case CARD_RESULT_IOERROR:
            mcdata->unk_12C = 1;
            break;
        case CARD_RESULT_NOPERM:
        case CARD_RESULT_FATAL_ERROR:
        case CARD_RESULT_WRONGDEVICE:
            break;
        }
    }
    return ret;
}

static S32 iSG_mc_fclose(st_ISG_MEMCARD_DATA* mcdata, CARDStat* stat)
{
    S32 ret = 0;
    s32 result;
    do
    {
        result = CARDClose(&mcdata->finfo);
    } while (result == CARD_RESULT_BUSY);

    if (result == CARD_RESULT_READY)
    {
        ret = 1;
    }

    if (stat != NULL)
    {
        memcpy(stat, &mcdata->fstat, sizeof(CARDStat));
    }

    memset(&mcdata->finfo, 0, sizeof(CARDFileInfo));
    memset(&mcdata->fstat, 0, sizeof(CARDStat));

    return ret;
}

static S32 iSG_mc_fclose(st_ISG_MEMCARD_DATA* mcdata)
{
    return iSG_mc_fclose(mcdata, NULL);
}

// Opens fname for reading, or recreates it at fsize bytes for writing. Only this game's files
// open: another game's file of the same name is closed again.
static S32 iSG_mc_fopen(st_ISG_MEMCARD_DATA* mcdata, const char* fname, S32 fsize,
                        en_ISG_IOMODE mode, en_ASYNC_OPERR* operr)
{
    S32 ret = 0;
    CARDFileInfo* finfo = &mcdata->finfo;
    CARDStat* stat = &mcdata->fstat;
    if (operr != NULL)
    {
        *operr = ISG_OPERR_NONE;
    }

    if (mode == ISG_IOMODE_READ)
    {
        s32 result;
        do
        {
            result = CARDOpen(mcdata->chan, (char*)fname, finfo);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY)
        {
            ret = 1;
        }
    }
    else
    {
        iSG_mc_fdel(mcdata, fname);
        if (mcdata->unk_12C == 0)
        {
            s32 result;
            do
            {
                result = CARDCreate(mcdata->chan, (char*)fname, fsize, finfo);
            } while (result == CARD_RESULT_BUSY);

            if (result == CARD_RESULT_READY)
            {
                ret = 1;
                mcdata->unk_98 = 0;
            }
            else if (operr != NULL)
            {
                switch (result)
                {
                case CARD_RESULT_WRONGDEVICE:
                case CARD_RESULT_NOCARD:
                    *operr = ISG_OPERR_NOCARD;
                    break;
                case CARD_RESULT_IOERROR:
                case CARD_RESULT_BROKEN:
                    mcdata->unk_12C = 1;
                    *operr = ISG_OPERR_DAMAGE;
                    break;
                case CARD_RESULT_NOENT:
                case CARD_RESULT_INSSPACE:
                    *operr = ISG_OPERR_NOROOM;
                    break;
                default:
                    *operr = ISG_OPERR_OTHER;
                    break;
                }
            }
        }
    }

    if (ret != 0)
    {
        s32 result;
        do
        {
            result = CARDGetStatus(mcdata->chan, finfo->fileNo, stat);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY)
        {
            ret = 1;
        }
    }

    if (ret != 0)
    {
        if (memcmp(stat->gameName, "GIHE", 4) != 0 || memcmp(stat->company, "78", 2) != 0)
        {
            iSG_mc_fclose(mcdata);
            ret = 0;
        }
    }

    return ret;
}

static S32 iSG_mc_format(st_ISG_MEMCARD_DATA* mcdata, S32, S32* canRecover)
{
    if (mcdata->unk_12C != 0)
    {
        if (canRecover != NULL)
        {
            *canRecover = 0;
        }
        return 0;
    }

    if (canRecover != NULL)
    {
        *canRecover = 1;
    }

    s32 result;
    do
    {
        result = CARDFormat(mcdata->chan);
    } while (result == CARD_RESULT_BUSY);

    S32 ret;
    if (result == CARD_RESULT_READY)
    {
        ret = 1;
    }
    else
    {
        ret = 0;

        switch (result)
        {
        case CARD_RESULT_IOERROR:
            mcdata->unk_12C = 1;
            if (canRecover != NULL)
            {
                *canRecover = 0;
            }
            break;
        case CARD_RESULT_FATAL_ERROR:
        case CARD_RESULT_WRONGDEVICE:
        case CARD_RESULT_NOFILE:
            break;
        }
    }
    return ret;
}

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

// The card space the save file's header block takes, rounded up to whole sectors.
static S32 iSG_cubeicon_size(S32 slot, S32 sectorSize)
{
    if ((U32)slot > 1)
    {
        return -1;
    }

    S32 t = sectorSize + 0x1FF & -sectorSize;
    return -t & sizeof(st_ISG_ICONDATA) + t - 1;
}

// Builds the header block in front of the save data and returns where the data goes.
static char* iSG_bfr_icondata(char* buf, CARDStat* stat, char* label, S32 sectorSize)
{
    st_ISG_ICONDATA data = { 0 };
    static TPLPalette* ico_pal;
    static TPLDescriptor* ico_desc;
    static U32 i = 0;

    sprintf(data.footer, "SCOOBY-DOO:NIGHTOF100FRIGHTS::RyanNeilDan");
    strncpy(data.game_name, "Night of 100 Frights", sizeof(data.game_name));
    data.game_name[0x1F] = '\0';

    if (label != NULL)
    {
        strncpy(data.label, label, sizeof(data.label));
    }
    data.label[0x1F] = '\0';

    ico_pal = g_rawbanr;
    ico_desc = &ico_pal->descriptorArray[0];
    memcpy(data.banner, ico_desc->textureHeader->data, sizeof(data.banner));

    ico_pal = g_rawicon;
    for (i = 0; i < 4; i++)
    {
        ico_desc = &ico_pal->descriptorArray[i];
        memcpy(data.icons[i], ico_desc->textureHeader->data, sizeof(data.icons[i]));
    }

    memcpy(buf, &data, sizeof(data));

    S32 t = sectorSize + 0x1FF & -sectorSize;
    return buf + (t + (sizeof(st_ISG_ICONDATA) - 1) & -t);
}

// A file is this game's when its header block ends with the game's signature.
static S32 iSG_curKosher(CARDStat* stat, CARDFileInfo* info)
{
    S32 rc = 1;

    if ((stat->iconAddr < 1 || stat->iconAddr > 0x7FFFFFFF) &&
        (stat->commentAddr < 1 || stat->commentAddr > 0x7FFFFFFF))
    {
        rc = 0;
    }
    else
    {
        char stuff[0x200] = { 0 };
        sprintf(stuff, "SCOOBY-DOO:NIGHTOF100FRIGHTS::RyanNeilDan");
        void* alloc = xMemPushTemp(0x3E00 + 0x1F);
        char* buf = (char*)((U32)alloc + 0x1F & ~0x1F);

        S32 result;
        do
        {
            result = CARDRead(info, buf, 0x3E00, 0);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY)
        {
            if (memcmp(buf + 0x3A40, stuff, strlen(stuff) - 1))
            {
                rc = 0;
            }
        }

        xMemPopTemp(alloc);
    }

    return rc;
}

static S32 iSG_get_finfo(st_ISG_MEMCARD_DATA* mcdata, const char* dpath)
{
    S32 rc = 0;
    en_ASYNC_OPERR operr = ISG_OPERR_NONE;

    if (iSG_mc_fopen(mcdata, dpath, -1, ISG_IOMODE_READ, &operr))
    {
        rc = 1;
        memcpy(&mcdata->unk_B0, &mcdata->fstat, sizeof(CARDStat));
        memcpy(&mcdata->unk_9C, &mcdata->finfo, sizeof(CARDFileInfo));
        iSG_mc_fclose(mcdata);
    }

    return rc;
}

// Counts the game's save files on the card. With a name buffer, also returns the name of the
// file at position which.
extern "C" S32 fn_80012414(st_ISG_MEMCARD_DATA* mcdata, S32 which, char* name, S32* count)
{
    S32 rc = 0;
    S32 found = 0;

    if (mcdata->unk_0 == 0)
    {
        return 0;
    }
    if (name != NULL)
    {
        *name = '\0';
    }

    for (S32 i = 0; i < 8; i++)
    {
        char* fname = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, i);
        if (iSG_get_finfo(mcdata, fname))
        {
            found++;
            if (name != NULL && found - 1 == which)
            {
                strcpy(name, fname);
                rc = 1;
                if (count == NULL)
                {
                    break;
                }
            }
        }
    }

    if (count != NULL)
    {
        *count = found;
        rc++;
    }
    return rc;
}

static S32 iSG_isSpaceForFile(st_ISG_MEMCARD_DATA* mcdata, S32 fsize, const char* fname,
                              S32* bytesNeeded, S32* availOnDisk, S32* needFile)
{
    S32 rc = 0;
    S32 result = 0;
    s32 byteNotUsed = 0;
    s32 filesNotUsed = 0;
    U32 len;

    if (mcdata->unk_0 == 0)
    {
        return 0;
    }
    len = iSG_cubeicon_size(mcdata->chan, mcdata->sectorSize) + fsize + 0x1FF & ~0x1FF;

    do
    {
        result = CARDFreeBlocks(mcdata->chan, &byteNotUsed, &filesNotUsed);
    } while (result == CARD_RESULT_BUSY);

    if (result != CARD_RESULT_READY)
    {
        return 0;
    }

    if (availOnDisk != NULL)
    {
        *availOnDisk = byteNotUsed;
    }
    if (bytesNeeded != NULL)
    {
        *bytesNeeded = len;
    }
    if (needFile != NULL)
    {
        *needFile = 1;
    }

    if (iSG_get_finfo(mcdata, fname))
    {
        if (needFile != NULL)
        {
            *needFile = *needFile - 1 & ~(*needFile - 1 >> 0x1F);
        }
        if (len <= mcdata->unk_B0.length + byteNotUsed)
        {
            rc = 1;
        }
    }
    else if ((S32)len <= byteNotUsed && filesNotUsed > 0)
    {
        rc = 1;
    }

    return rc;
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

// Reads numbytes of the save data that follows fname's header block. A buffer the card cannot
// read into directly (unaligned, or not a whole number of sectors) is read through a temporary
// one.
S32 iSGReadLeader(st_ISGSESSION* isgdata, const char* fname, char* databuf, S32 numbytes,
                  S32 async)
{
    S32 bufsize;
    S32 iconsize;
    S32 allocsize;
    char* readbuf;

    S32 readret = 0;
    st_ISG_MEMCARD_DATA* data;
    void* alloc = NULL;

    en_ASYNC_OPERR operr = ISG_OPERR_NONE;
    if (isgdata->slot < 0)
    {
        isgdata->as_opstat = ISG_OPSTAT_FAILURE;
        isgdata->as_operr = ISG_OPERR_NOCARD;
        return 0;
    }
    data = &isgdata->mcdata[isgdata->slot];

    if (data->unk_12C != 0)
    {
        isgdata->as_opstat = ISG_OPSTAT_FAILURE;
        isgdata->as_operr = ISG_OPERR_DAMAGE;
        return 0;
    }

    fn_80016768();
    iconsize = iSG_cubeicon_size(data->chan, data->sectorSize);
    S32 sectorsize200 = data->sectorSize + 0x1FF & -data->sectorSize;
    if ((S32)databuf % 32 != 0 || numbytes % sectorsize200 != 0)
    {
        S32 tmpsize = numbytes + 0x1FF & ~0x1FF;
        allocsize = tmpsize + 0x1F;
        alloc = xMemPushTemp(allocsize);
        memset(alloc, 0, allocsize);
        bufsize = tmpsize;
        readbuf = (char*)((U32)alloc + 0x1F & ~0x1F);
    }
    else
    {
        readbuf = databuf;
        bufsize = numbytes;
    }

    fn_80016768();

    if (iSG_mc_fopen(data, fname, -1, ISG_IOMODE_READ, &operr) != 0)
    {
        if (iSG_mc_fread(data, readbuf, bufsize, iconsize) != 0)
        {
            readret = 1;
        }
        else
        {
            readret = 0;
        }
        iSG_mc_fclose(data);
    }

    if (readret != 0 && alloc != NULL)
    {
        memcpy(databuf, readbuf, numbytes);
    }
    if (alloc != NULL)
    {
        xMemPopTemp(alloc);
    }

    isgdata->as_operr = ISG_OPERR_NONE;
    if (readret != 0)
    {
        isgdata->as_opstat = ISG_OPSTAT_SUCCESS;
    }
    else
    {
        isgdata->as_opstat = ISG_OPSTAT_FAILURE;
        if (data->unk_12C != 0)
        {
            isgdata->as_operr = ISG_OPERR_DAMAGE;
        }
        else
        {
            isgdata->as_operr = ISG_OPERR_OTHER;
        }
    }
    return readret;
}

// Reads all of fname's save data.
S32 iSGLoadFile(st_ISGSESSION* isgdata, const char* fname, char* databuf, S32 async)
{
    S32 numBytes = iSGFileSize(isgdata, fname);
    return iSGReadLeader(isgdata, fname, databuf, numBytes, async);
}

// Sets the card file's banner and its four looping icon frames, which follow the 0x40 bytes of
// title and label.
static void iSG_upd_icostat(CARDStat*, CARDStat* stat)
{
    CARDSetCommentAddress(stat, 0);
    CARDSetBannerFormat(stat, CARD_STAT_BANNER_RGB5A3);
    CARDSetIconAddress(stat, 0x40);
    CARDSetIconAnim(stat, CARD_STAT_ANIM_LOOP);

    S32 i;
    for (i = 0; i < 4; ++i)
    {
        CARDSetIconFormat(stat, i, CARD_STAT_BANNER_RGB5A3);
        CARDSetIconSpeed(stat, i, CARD_STAT_SPEED_MIDDLE);
    }

    CARDSetIconFormat(stat, i, CARD_STAT_ICON_NONE);
    CARDSetIconSpeed(stat, i, CARD_STAT_SPEED_END);
}

// Writes the header block and n bytes of data as fname. A failed write deletes the partial
// file unless the card is damaged.
S32 iSGSaveFile(st_ISGSESSION* isgdata, const char* fname, char* data, S32 n, S32 async,
                char* label)
{
    void* alloc;
    S32 allocsize;
    char* icondata;
    S32 iconsize;

    S32 writeret = 0;
    en_ASYNC_OPERR operr = ISG_OPERR_NONE;
    CARDStat statA = { 0 };
    CARDStat statB = { 0 };

    if (isgdata->slot < 0)
    {
        isgdata->as_opstat = ISG_OPSTAT_FAILURE;
        isgdata->as_operr = ISG_OPERR_NOCARD;
        return 0;
    }

    st_ISG_MEMCARD_DATA* mcdata = &isgdata->mcdata[isgdata->slot];
    if (mcdata->unk_12C != 0)
    {
        isgdata->as_opstat = ISG_OPSTAT_FAILURE;
        isgdata->as_operr = ISG_OPERR_DAMAGE;
        return 0;
    }

    fn_80016768();
    iconsize = iSG_cubeicon_size(isgdata->slot, mcdata->sectorSize);
    S32 sectorsize200 = mcdata->sectorSize + 0x1FF & -mcdata->sectorSize;
    iconsize += n + sectorsize200 - 1 & -sectorsize200;

    allocsize = iconsize + 0x1F;
    alloc = xMemPushTemp(allocsize);
    memset(alloc, 0, allocsize);
    icondata = (char*)((U32)alloc + 0x1F & 0xFFFFFFE0);

    memcpy(iSG_bfr_icondata(icondata, &statA, label, mcdata->sectorSize), data, n);
    fn_80016768();

    if (iSG_mc_fopen(mcdata, fname, iconsize, ISG_IOMODE_WRITE, &operr) != 0)
    {
        if (iSG_mc_fwrite(mcdata, icondata, iconsize) != 0)
        {
            writeret = 1;
        }
        else
        {
            writeret = 0;
            operr = ISG_OPERR_SVWRITE;
        }

        if (writeret != 0)
        {
            iSG_upd_icostat(&statA, &mcdata->fstat);
            s32 result;
            do
            {
                result = CARDSetStatus(mcdata->chan, mcdata->finfo.fileNo, &mcdata->fstat);
            } while (result == CARD_RESULT_BUSY);

            if (result != CARD_RESULT_READY)
            {
                writeret = 0;
            }
        }

        if (writeret != 0)
        {
            s32 result;
            do
            {
                result = CARDSetAttributes(mcdata->chan, mcdata->finfo.fileNo, CARD_ATTR_PUBLIC);
            } while (result == CARD_RESULT_BUSY);
        }

        iSG_mc_fclose(mcdata, &statB);
    }
    xMemPopTemp(alloc);

    if (writeret == 0 && mcdata->unk_12C == 0)
    {
        iSG_mc_fdel(mcdata, fname);
    }

    fn_80016768();

    isgdata->as_operr = ISG_OPERR_NONE;
    if (writeret != 0)
    {
        isgdata->as_opstat = ISG_OPSTAT_SUCCESS;
    }
    else
    {
        isgdata->as_opstat = ISG_OPSTAT_FAILURE;
        if (operr != ISG_OPERR_NONE)
        {
            isgdata->as_operr = operr;
        }
        else
        {
            isgdata->as_operr = ISG_OPERR_UNKNOWN;
        }
    }
    return writeret;
}

// The GameCube needs no game directory setup.
S32 iSGSetupGameDir(st_ISGSESSION* isgdata, const char* dname, S32 force_iconfix)
{
    return 1;
}

static S32 iSG_mc_isformatted(st_ISG_MEMCARD_DATA* mcdata)
{
    S32 result = 0;
    S32 rc = 0;
    s32 xferBytes = 0;

    if (mcdata->unk_0 == 0)
    {
        rc = 0;
    }
    else if (mcdata->unk_12C)
    {
        rc = 0;
    }
    else
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
            rc = 0;
        }
        else
        {
            rc = 0;
        }
    }

    return rc;
}

// The active card is usable when it is mounted, readable and formatted.
S32 iSGSelectGameDir(st_ISGSESSION* isgdata, const char* dname)
{
    if (isgdata->slot < 0)
    {
        return 0;
    }
    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];

    if (data->unk_0 == 0)
    {
        return 0;
    }

    if (iSG_mc_isGCcard(data, NULL, NULL) == 0)
    {
        return 0;
    }

    return iSG_mc_isformatted(data) != 0;
}

// Returns 1 for this game's file, 0 for a file without its signature (deleted when
// delbad is set) and -1 when fname does not open.
static S32 iSG_fileKosher(st_ISG_MEMCARD_DATA* mcdata, const char* fname, S32 delbad,
                          S32* deleted)
{
    S32 rc = 0;
    en_ASYNC_OPERR operr = ISG_OPERR_NONE;

    if (deleted)
    {
        *deleted = 0;
    }

    if (iSG_mc_fopen(mcdata, fname, -1, ISG_IOMODE_READ, &operr) == 0)
    {
        return -1;
    }

    S32 ret = iSG_curKosher(&mcdata->fstat, &mcdata->finfo);
    iSG_mc_fclose(mcdata);

    if (ret == 0)
    {
        rc = 0;
        if (delbad && iSG_mc_fdel(mcdata, fname) && deleted)
        {
            *deleted = 1;
        }
    }
    else
    {
        rc = 1;
    }
    return rc;
}

// Looks for the first save file without the game's signature. first receives its index and
// deleted the index of the first one removed when delbad is set; a pass with delbad set
// checks again and returns whether a bad file is still there.
extern "C" S32 fn_800132C4(st_ISGSESSION* isgdata, S32 delbad, S32* first, S32* deleted)
{
    S32 rc = 1;

    if (first != NULL)
    {
        *first = -1;
    }
    if (isgdata->slot < 0)
    {
        return 1;
    }
    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];

    for (S32 i = 0; i < 8; i++)
    {
        S32 wasdel = 0;
        char* fname = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, i);
        S32 kosher = iSG_fileKosher(data, fname, delbad, &wasdel);
        if (kosher == 0)
        {
            if (first != NULL && *first < 0)
            {
                *first = i;
            }
            if (wasdel && deleted != NULL && *deleted < 0)
            {
                *deleted = i;
            }
            rc = 0;
            break;
        }
    }

    if (delbad)
    {
        rc = fn_800132C4(isgdata, 0, NULL, NULL) > 0 ? 1 : 0;
    }
    return rc;
}

void iSGMakeTimeStamp(char* str)
{
    OSCalendarTime calendar_time = { 0 };
    OSTime time = OSGetTime();
    OSTicksToCalendarTime(time, &calendar_time);
    sprintf(str, "%02d/%02d/%04d %02d:%02d:%02d", calendar_time.mon + 1, calendar_time.mday,
            calendar_time.year, calendar_time.hour, calendar_time.min, calendar_time.sec);
}

static S32 iSG_get_fmoddate(st_ISG_MEMCARD_DATA* mcdata, const char* fname, int* sec, int* min,
                            int* hr, int* mon, int* day, int* yr)
{
    S32 rc = 1;
    OSCalendarTime time = { 0 };

    if (iSG_get_finfo(mcdata, fname) == 0)
    {
        rc = 0;
    }
    else
    {
        OSTime t = mcdata->unk_B0.time;
        t = OSSecondsToTicks(t);
        OSTicksToCalendarTime(t, &time);

        if (sec)
        {
            *sec = time.sec;
        }
        if (min)
        {
            *min = time.min;
        }
        if (hr)
        {
            *hr = time.hour;
        }
        if (mon)
        {
            *mon = time.mon + 1;
        }
        if (day)
        {
            *day = time.mday;
        }
        if (yr)
        {
            *yr = time.year;
        }
    }

    return rc;
}

// The save file's last write time as text, for the load screen.
char* iSGFileModDate(st_ISGSESSION* isgdata, const char* fname)
{
    static char datestr[0x40];
    int sec = 0;
    int min = 0;
    int hr = 0;
    int mon = 0;
    int day = 0;
    int yr = 0;
    if (iSG_get_fmoddate(&isgdata->mcdata[isgdata->slot], fname, &sec, &min, &hr, &mon, &day,
                         &yr) != 0)
    {
        sprintf(datestr, "%02d/%02d/%04d %02d:%02d:%02d", mon, day, yr, hr, min, sec);
    }
    else
    {
        sprintf(datestr, "<Unknown Modification>");
    }
    return datestr;
}

static S32 iSG_get_fsize(st_ISG_MEMCARD_DATA* mcdata, const char* fname)
{
    S32 rc = -1;

    if (iSG_get_finfo(mcdata, fname))
    {
        rc = mcdata->unk_B0.length;
    }

    if (rc < 0)
    {
        rc = -1;
    }

    return rc;
}

// The size of fname's save data, without its header block; -1 when it cannot be read.
S32 iSGFileSize(st_ISGSESSION* isgdata, const char* fname)
{
    S32 ret = 0;
    if (isgdata->slot < 0)
    {
        return -1;
    }
    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];
    fn_80016768();
    ret = iSG_get_fsize(data, fname);
    if (ret >= 0)
    {
        ret -= iSG_cubeicon_size(data->chan, data->sectorSize);
        if (ret < 0)
        {
            ret = -1;
        }
    }
    return ret;
}

// The number of the game's save files on the active card.
extern "C" S32 fn_800138EC(st_ISGSESSION* isgdata, const char* dname, const char* fname)
{
    if (isgdata->slot < 0)
    {
        return 0;
    }
    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];
    if (data->unk_0 == 0)
    {
        return 0;
    }
    fn_80016768();
    S32 count = 0;
    fn_80012414(data, -1, NULL, &count);
    return count;
}

// Whether a save of fsize bytes fits on the active card. Without a file name, the first
// existing save file is the one that would be replaced.
S32 iSGTgtHaveRoom(st_ISGSESSION* isgdata, S32 tidx, S32 fsize, const char* dpath,
                   const char* fname, S32* bytesNeeded, S32* availOnDisk, S32* needFile)
{
    if (isgdata->slot < 0)
    {
        return 0;
    }
    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];
    if (data->unk_0 == 0)
    {
        return 0;
    }
    if (fname == NULL)
    {
        for (S32 i = 0; i < 8; ++i)
        {
            fn_80016768();
            fname = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, i);
            if (iSG_get_finfo(data, fname) != 0)
            {
                break;
            }
        }
    }

    return iSG_isSpaceForFile(data, fsize, fname, bytesNeeded, availOnDisk, needFile);
}

static S32 iSG_mc_mount(S32 slot)
{
    S32 ret = 0;
    s32 result;
    do
    {
        result = CARDMount(slot, cardwork[slot], iSG_cb_unmount);
    } while (result == CARD_RESULT_BUSY);

    switch (result)
    {
    case CARD_RESULT_READY:
        ret = 1;
        break;
    case CARD_RESULT_BROKEN:
    case CARD_RESULT_ENCODING:
        ret = 1;
        break;
    }
    return ret;
}

// Marks the card damaged when a check fails for any reason a format cannot fix.
static S32 iSG_mc_tryRepair(st_ISG_MEMCARD_DATA* mcdata)
{
    S32 result = 0;
    S32 rc = 0;
    s32 xferBytes = 0;

    if (mcdata->unk_0 == 0)
    {
        rc = 0;
    }
    else if (mcdata->unk_12C)
    {
        rc = 0;
    }
    else
    {
        do
        {
            result = CARDCheckEx(mcdata->chan, &xferBytes);
        } while (result == CARD_RESULT_BUSY);

        if (result == CARD_RESULT_READY)
        {
            rc = 1;
        }
        else if (result == CARD_RESULT_ENCODING)
        {
            rc = 1;
        }
        else
        {
            if (result != CARD_RESULT_BROKEN)
            {
                mcdata->unk_12C = 1;
            }
            rc = 0;
        }
    }
    return rc;
}

static S32 iSG_mc_settgt(st_ISG_MEMCARD_DATA* mcdata, S32 slot)
{
    if (iSG_mc_mount(slot))
    {
        mcdata->unk_0 = 1;
        mcdata->chan = slot;

        iSG_mc_tryRepair(mcdata);
        CARDGetSectorSize(slot, (u32*)&mcdata->sectorSize);
    }
    else
    {
        mcdata->unk_0 = 0;
    }

    return mcdata->unk_0 != 0 ? 1 : 0;
}

// Mounts the tgtidx'th card present and makes it the active one.
S32 iSGTgtSetActive(st_ISGSESSION* isgdata, S32 tgtidx)
{
    S32 ret = 0;
    S32 slot = 0;
    iSG_mcidx2slot(tgtidx, &slot, NULL);
    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[slot];
    fn_80016768();
    if (data->unk_0)
    {
        ret = 1;
    }
    else
    {
        if (iSG_mc_settgt(data, slot) != 0)
        {
            ret = 1;
            isgdata->slot = slot;
        }
    }
    return ret;
}

// The free bytes on the tidx'th card. sectorSize and filesFree, when given, receive the
// card's sector size and its free file entries.
extern "C" S32 fn_80013C64(st_ISGSESSION* isgdata, S32 tidx, S32* sectorSize, S32* filesFree)
{
    s32 bytesFree = 0;
    s32 files = 0;

    if (sectorSize != NULL)
    {
        *sectorSize = 0;
    }
    if (filesFree != NULL)
    {
        *filesFree = 0;
    }
    if (isgdata->slot < 0)
    {
        return 0;
    }
    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];
    fn_80016768();
    if (iSGTgtState(isgdata, tidx, NULL) == 0)
    {
        return 0;
    }
    CARDFreeBlocks(isgdata->slot, &bytesFree, &files);
    if (filesFree != NULL)
    {
        *filesFree = files;
    }
    if (sectorSize != NULL)
    {
        *sectorSize = data->sectorSize + 0x1FF & -data->sectorSize;
    }
    return bytesFree;
}

static S32 iSG_mc_exists(S32 slot)
{
    S32 ret = 0;
    s32 memSize = 0;
    s32 sectorSize = 0;

    if (slot < -1)
    {
        return 0;
    }

    S32 result;
    do
    {
        result = CARDProbeEx(slot, &memSize, &sectorSize);
    } while (result == CARD_RESULT_BUSY);

    if (result == CARD_RESULT_READY)
    {
        ret = 1;
    }

    return ret;
}

static S32 g_legalSectSize[] = { 0x2000, 0, -1 };

// The tgtidx'th card's state as flags: 1 present, 2 and 8 formatted and usable, 4 needs a
// format, 0x1000000 damaged or absent, 0x2000000 I/O error, 0x4000000 wrong encoding and
// 0x8000000 an unsupported sector size.
U32 iSGTgtState(st_ISGSESSION* isgdata, S32 tgtidx, const char* dpath)
{
    S32 isSectSizeValid = 0;
    S32 state = 0;

    S32 rc = 0;

    S32 slot = 0;
    S32 x = 0;
    S32 y = 0;
    fn_80016768();

    iSG_mcidx2slot(tgtidx, &slot, NULL);
    if (slot != isgdata->slot)
    {
        iSGTgtSetActive(isgdata, tgtidx);
    }

    if (isgdata->slot < 0)
    {
        return 0x1000000;
    }

    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];
    if (data->unk_0 == 0)
    {
        return 0;
    }

    if (iSG_mc_exists(isgdata->slot))
    {
        state |= 1;
    }
    else
    {
        return 0;
    }

    if (data->unk_12C != 0)
    {
        return state | 0x1000000;
    }

    rc = iSG_mc_isGCcard(data, &x, &y);
    if (x != 0)
    {
        state |= 0x4000000;
    }

    if (y != 0)
    {
        state |= 0x2000000;
    }

    if (rc == 0)
    {
        return state;
    }

    if (g_legalSectSize[0] > 0)
    {
        for (S32 i = 0; g_legalSectSize[i] > 0; i++)
        {
            if (data->sectorSize == g_legalSectSize[i])
            {
                isSectSizeValid = 1;
                break;
            }
        }
        if (isSectSizeValid == 0)
        {
            state |= 0x8000004;
            return state & ~4;
        }
    }

    if (iSG_mc_isformatted(data) == 0)
    {
        return state | 4;
    }
    else
    {
        return state | 0xE;
    }
}

S32 iSGTgtFormat(st_ISGSESSION* isgdata, S32 tgtidx, S32 async, S32* canRecover)
{
    S32 slot = 0;
    fn_80016768();
    if (iSG_mc_exists(isgdata->slot) == 0)
    {
        return 0;
    }

    iSG_mcidx2slot(tgtidx, &slot, NULL);
    if (slot != isgdata->slot)
    {
        return 0;
    }

    st_ISG_MEMCARD_DATA* data = &isgdata->mcdata[isgdata->slot];
    if (data->unk_0 == 0)
    {
        return 0;
    }

    if (iSG_mc_isGCcard(data, NULL, NULL) == 0)
    {
        return 0;
    }

    return iSG_mc_format(data, 0, canRecover) != 0;
}

// Names a card slot in the wording the message needs. Types 5 to 7 take a physical slot,
// the others the index of a card present; "^" stands for the trademark sign in the font.
extern "C" char* fn_80014180(st_ISGSESSION* isgdata, S32 tidx, S32 msgtype)
{
    static char slotname[0x40];
    S32 slot = 0;
    char letters[] = "AB---";

    if (msgtype >= 5 && msgtype < 8)
    {
        slot = tidx;
    }
    else if (iSG_mcidx2slot(tidx, &slot, NULL) == 0)
    {
        slot = 3;
    }

    switch (msgtype)
    {
    case 0:
    case 6:
        sprintf(slotname, "Slot %c", letters[slot]);
        break;
    default:
    case 1:
        sprintf(slotname, "Memory Card Slot %c", letters[slot]);
        break;
    case 2:
        sprintf(slotname, "Nintendo GameCube Memory Card in Slot %c", letters[slot]);
        break;
    case 3:
    case 5:
        sprintf(slotname, "Nintendo GameCube%s Memory Card in Slot %c", "^", letters[slot]);
        break;
    case 4:
    case 7:
        sprintf(slotname, "Nintendo GameCube%s Memory Card", "^");
        break;
    }
    return slotname;
}

// What is in a slot: 0 nothing, 1 a device that is not a memory card, 3 a card and 4 a
// fatal error.
extern "C" S32 fn_80014310(st_ISGSESSION* isgdata, S32 slot)
{
    S32 rc;
    s32 memSize = 0;
    s32 sectorSize = 0;
    fn_80016768();

    s32 result;
    do
    {
        result = CARDProbeEx(slot, &memSize, &sectorSize);
    } while (result == CARD_RESULT_BUSY);

    switch (result)
    {
    case CARD_RESULT_READY:
        rc = 3;
        break;
    case CARD_RESULT_NOCARD:
        rc = 0;
        break;
    case CARD_RESULT_WRONGDEVICE:
        rc = 1;
        break;
    case CARD_RESULT_FATAL_ERROR:
        rc = 4;
        break;
    default:
        rc = 0;
        break;
    }
    return rc;
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

// Save file names come from a ring of eight buffers, so a few can be held at once.
char* iSGMakeName(en_NAMEGEN_TYPE type, const char* base, S32 idx)
{
    static S32 rotate = 0;
    static char rotatebuf[8][32] = { 0 };

    const char* fmt_sd = "%s%02d";
    char* use_buf = rotatebuf[rotate++];
    if (rotate == 8)
    {
        rotate = 0;
    }

    *use_buf = NULL;
    switch (type)
    {
    case ISG_NGTYP_GAMEFILE:
        if (base != NULL)
        {
            sprintf(use_buf, fmt_sd, base, idx);
        }
        else
        {
            sprintf(use_buf, fmt_sd, "ScoobyDoo", idx);
        }
        break;
    case ISG_NGTYP_GAMEDIR:
    case ISG_NGTYP_CONFIG:
    case ISG_NGTYP_ICONTHUM:
        break;
    }

    return use_buf;
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
