#include "xsavegame.h"

#include "isavegame.h"
#include "xMemMgr.h"
#include "xutil.h"

#include <stdio.h>
#include <string.h>

static st_XSAVEGAME_DATA g_xsgdata = {};
static st_XSAVEGAME_LEADER g_leaders[8] = {};
static S32 g_xsginit;

void xSG_cb_ISGChange(void*, en_CHGCODE what);
static S32 xSG_grab_leaders(st_XSAVEGAME_DATA* xsgdata);
static S32 xSG_cb_leader_svinfo(void*, st_XSAVEGAME_DATA*, S32* cur_space, S32* max_fullgame);
static S32 xSG_cb_leader_svproc(void* cltdata, st_XSAVEGAME_DATA* original_xsgdata,
                                st_XSAVEGAME_WRITECONTEXT* wctxt);
static S32 xSG_cb_leader_load(void*, st_XSAVEGAME_DATA* original_xsgdata,
                              st_XSAVEGAME_READCONTEXT* rctxt, U32, S32);

S32 xSGStartup()
{
    if (g_xsginit++ == 0)
    {
        xUtilStartup();
        iSGStartup();
        memset(&g_xsgdata, 0, sizeof(st_XSAVEGAME_DATA));
    }
    return g_xsginit;
}

S32 xSGShutdown()
{
    g_xsginit--;
    if (g_xsginit == 0)
    {
        iSGShutdown();
        xUtilShutdown();
    }
    return g_xsginit;
}

st_XSAVEGAME_DATA* xSGInit(en_SAVEGAME_MODE mode)
{
    st_XSAVEGAME_DATA* xsgdata = &g_xsgdata;
    if (g_xsgdata.stage != 0)
    {
        return NULL;
    }

    xsgdata->mode = mode;
    xsgdata->chksum = xUtil_crc_init();
    xsgdata->upd_tally = 1;
    xsgdata->file_chksum = 0;
    xsgdata->read_chksum = xUtil_crc_init();
    xsgdata->stage |= 1;
    xsgdata->isgsess = iSGSessionBegin(xsgdata, xSG_cb_ISGChange);

    if (mode == XSG_MODE_LOAD)
    {
        xSGAddLoadClient(xsgdata, 'LEDR', xsgdata, xSG_cb_leader_load);
    }
    else
    {
        xSGAddSaveClient(xsgdata, 'LEDR', xsgdata, xSG_cb_leader_svinfo, xSG_cb_leader_svproc);
    }

    memset(&g_leaders, 0, sizeof(g_leaders));
    return xsgdata;
}

// Ends the card session and frees the save or load buffer.
S32 xSGDone(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    if (!(xsgdata->stage & 1))
    {
        result = 0;
    }

    if (xsgdata->isgsess != NULL)
    {
        iSGSessionEnd(xsgdata->isgsess);
    }

    if (xsgdata->mode == XSG_MODE_SAVE)
    {
        if (xsgdata->membuf != NULL)
        {
            xUtil_idtag2string('XSGS', 0);
            xMemPopTemp(xsgdata->membuf);
            xsgdata->membuf = NULL;
            xsgdata->memsize = 0;
            xsgdata->buf_curpos = NULL;
        }
    }
    else if (xsgdata->mode == XSG_MODE_LOAD)
    {
        if (xsgdata->membuf != NULL)
        {
            xUtil_idtag2string('XSGL', 0);
            xMemPopTemp(xsgdata->loadbuf);
            xsgdata->loadbuf = NULL;
            xsgdata->loadsize = 0;
            xsgdata->walkpos = NULL;
            xsgdata->walkremain = 0;
        }
    }

    memset(&g_xsgdata, 0, sizeof(st_XSAVEGAME_DATA));
    memset(&g_leaders, 0, sizeof(g_leaders));
    return result;
}

S32 xSGTgtCount(st_XSAVEGAME_DATA* xsgdata, S32* max)
{
    return iSGTgtCount(xsgdata->isgsess, max);
}

S32 xSGTgtPhysSlotIdx(st_XSAVEGAME_DATA* xsgdata, S32 tidx)
{
    return iSGTgtPhysSlotIdx(xsgdata->isgsess, tidx);
}

// Picks the message text for a memory card failure: the card in slot tidx is named in it.
extern "C" char* fn_800389FC(st_XSAVEGAME_DATA* xsgdata, S32 tidx, en_XSG_WHYFAIL whyFail)
{
    switch (whyFail)
    {
    case XSG_WHYERR_NOCARD:
        return fn_80014180(NULL, tidx, 3);
    case XSG_WHYERR_NOROOM:
        return fn_80014180(NULL, tidx, 7);
    case XSG_WHYERR_DAMAGE:
        return fn_80014180(NULL, tidx, 6);
    case XSG_WHYERR_CARDYANKED:
        return fn_80014180(NULL, tidx, 5);
    default:
        return fn_80014180(xsgdata->isgsess, tidx, 2);
    }
}

// Whether the card is formatted for this console; a card in the wrong encoding sets badEncode.
S32 xSGTgtIsFormat(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32* badEncode)
{
    S32 result = 0;
    U32 state = iSGTgtState(xsgdata->isgsess, tidx, NULL);
    if (state & 0x4000000)
    {
        if (badEncode != NULL)
        {
            *badEncode = 1;
        }
    }
    else if (state & 2)
    {
        result = 1;
    }
    return result;
}

S32 xSGTgtFormatTgt(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32* canRecover)
{
    return iSGTgtFormat(xsgdata->isgsess, tidx, 0, canRecover);
}

// Passes the card's sector size and free file entries through from the card layer.
extern "C" void fn_80038948(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32* sectorSize, S32* filesFree)
{
    fn_80013C64(xsgdata->isgsess, tidx, sectorSize, filesFree);
}

S32 xSGTgtSelect(st_XSAVEGAME_DATA* xsgdata, S32 tidx)
{
    S32 result = iSGTgtSetActive(xsgdata->isgsess, tidx);
    if (result != 0 && xSG_chdir_gamedir(xsgdata) != 0)
    {
        xSG_grab_leaders(xsgdata);
    }
    return result;
}

// Reads the leader block of every saved game in the game directory into g_leaders.
static S32 xSG_grab_leaders(st_XSAVEGAME_DATA* xsgdata)
{
    S32 num_found = 0;
    char readbuf[256] = {};

    memset(&g_leaders, 0, sizeof(g_leaders));

    if (!xSG_chdir_gamedir(xsgdata))
    {
        return 0;
    }

    for (S32 i = 0; i < 8; i++)
    {
        if (!xSGGameIsEmpty(xsgdata, i))
        {
            char* slotname = xSG_cm_slotname(xsgdata, i);
            S32 rc = iSGReadLeader(xsgdata->isgsess, slotname, readbuf, sizeof(readbuf), 0);
            if (rc != 0)
            {
                num_found++;
                memcpy(&g_leaders[i], readbuf + 0x1c, sizeof(st_XSAVEGAME_LEADER));
            }
        }
    }

    return num_found;
}

S32 xSG_chdir_gamedir(st_XSAVEGAME_DATA* xsgdata)
{
    S32 rc = 1;
    char* name = iSGMakeName(ISG_NGTYP_GAMEDIR, NULL, 0);
    if (!iSGSelectGameDir(xsgdata->isgsess, name))
    {
        rc = 0;
    }
    return rc;
}

char* xSG_cm_slotname(st_XSAVEGAME_DATA* xsgdata, S32 gidx)
{
    static char slotname[32] = {};
    slotname[0] = '\0';

    // Slot 0x9A5 names the settings file rather than a game.
    char* tmp;
    if (gidx == 0x9A5)
    {
        tmp = iSGMakeName(ISG_NGTYP_CONFIG, NULL, 0);
    }
    else
    {
        tmp = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, gidx);
    }
    strcpy(slotname, tmp);
    return slotname;
}

S32 xSGGameSize(st_XSAVEGAME_DATA* xsgdata, S32 gidx)
{
    S32 size = 0;
    if (xSG_chdir_gamedir(xsgdata))
    {
        char* fname = xSG_cm_slotname(xsgdata, gidx);
        size = iSGFileSize(xsgdata->isgsess, fname);
    }
    return size;
}

S32 xSGTgtHasGameDir(st_XSAVEGAME_DATA* xsgdata, S32 tidx)
{
    S32 rc = iSGTgtSetActive(xsgdata->isgsess, tidx);
    if (rc != 0)
    {
        return xSG_chdir_gamedir(xsgdata);
    }
    return rc;
}

S32 xSGTgtHaveRoom(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32 fsize, S32 slotidx, S32* bytesNeeded,
                   S32* availOnDisk, S32* needFile)
{
    char fname[256] = {};

    // A negative slot asks only whether a new game directory would fit.
    if (slotidx < 0)
    {
        return iSGTgtHaveRoom(xsgdata->isgsess, tidx, fsize, NULL, NULL, bytesNeeded, availOnDisk,
                              needFile);
    }

    strcpy(fname, iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, slotidx));
    return iSGTgtHaveRoom(xsgdata->isgsess, tidx, fsize, NULL, fname, bytesNeeded, availOnDisk,
                          needFile);
}

void xSGGameSet(st_XSAVEGAME_DATA* xsgdata, S32 gidx)
{
    xsgdata->gslot = gidx;
}

S32 xSGGameIsEmpty(st_XSAVEGAME_DATA* xsgdata, S32 gidx)
{
    return xSGGameSize(xsgdata, gidx) <= 0;
}

// The save slot's last write time as text.
char* xSGGameModDate(st_XSAVEGAME_DATA* xsgdata, S32 gidx)
{
    static char da_date[32] = {};
    da_date[0] = '\0';
    if (xSG_chdir_gamedir(xsgdata))
    {
        strcpy(da_date, iSGFileModDate(xsgdata->isgsess, xSG_cm_slotname(xsgdata, gidx)));
    }
    return da_date;
}

// The save slot's label, from the leader blocks read when the card was selected.
char* xSGGameLabel(st_XSAVEGAME_DATA* xsgdata, S32 gidx)
{
    char* da_name = NULL;
    if (xSG_chdir_gamedir(xsgdata) && g_leaders[gidx].gameLabel != NULL)
    {
        da_name = g_leaders[gidx].gameLabel;
    }
    return da_name;
}

S32 xSGGameThumIndex(st_XSAVEGAME_DATA* xsgdata, S32 gidx)
{
    S32 idx_thum = -1;
    if (xSG_chdir_gamedir(xsgdata))
    {
        idx_thum = g_leaders[gidx].thumIconIndex;
    }
    return idx_thum;
}

S32 xSGAddSaveClient(st_XSAVEGAME_DATA* xsgdata, U32 clttag, void* cltdata,
                     S32 (*infofunc)(void*, st_XSAVEGAME_DATA*, S32*, S32*),
                     S32 (*procfunc)(void*, st_XSAVEGAME_DATA*, st_XSAVEGAME_WRITECONTEXT*))
{
    S32 result = 1;
    if (xsgdata->stage & 0x40000000)
    {
        result = 0;
    }
    else if (!(xsgdata->stage & 1))
    {
        result = 0;
    }

    if (xsgdata->stkcnt + 1 >= 128)
    {
        xUtil_idtag2string(clttag, 0);
        result = 0;
    }

    st_XSAVEGAME_CLIENT* clt = &xsgdata->cltstk[xsgdata->stkcnt++];
    clt->idtag = clttag;
    clt->cltinfo = infofunc;
    clt->cltproc = procfunc;
    clt->cltdata = cltdata;
    clt->needamt = 0;
    clt->realamt = 0;
    clt->buf_sizepos = NULL;

    xsgdata->stage |= 4;
    if (result == 0)
    {
        xsgdata->stage |= 0x40000000;
    }
    return result;
}

S32 xSGAddLoadClient(st_XSAVEGAME_DATA* xsgdata, U32 clttag, void* cltdata,
                     S32 (*loadfunc)(void*, st_XSAVEGAME_DATA*, st_XSAVEGAME_READCONTEXT*, U32,
                                     S32))
{
    S32 result = 1;
    if (xsgdata->stage & 0x40000000)
    {
        result = 0;
    }
    else if (!(xsgdata->stage & 1))
    {
        result = 0;
    }
    else if (xsgdata->stkcnt + 1 >= 128)
    {
        xUtil_idtag2string(clttag, 0);
        result = 0;
    }

    // Tag 0 registers the one default loader, used for blocks no other client claims.
    if (clttag != 0 || xsgdata->dfltloadclt.idtag == 0)
    {
        if (clttag == 0)
        {
            xsgdata->dfltloadclt.idtag = 1;
            xsgdata->dfltloadclt.cltload = loadfunc;
            xsgdata->dfltloadclt.cltdata = cltdata;
            xsgdata->dfltloadclt.blokact = 0;
            xsgdata->dfltloadclt.blokmax = 0;
            xsgdata->dfltloadclt.blokpos = NULL;
            xsgdata->dfltloadclt.readpos = NULL;
            xsgdata->dfltloadclt.readremain = 0;
            xsgdata->dfltloadclt.readamt = 0;
        }
        else
        {
            st_XSAVEGAME_CLIENT* clt = &xsgdata->cltstk[xsgdata->stkcnt++];
            clt->idtag = clttag;
            clt->cltload = loadfunc;
            clt->cltdata = cltdata;
            clt->blokact = 0;
            clt->blokmax = 0;
            clt->blokpos = NULL;
            clt->readpos = NULL;
            clt->readremain = 0;
            clt->readamt = 0;
        }
    }

    xsgdata->stage |= 2;
    if (result == 0)
    {
        xsgdata->stage |= 0x40000000;
    }
    return result;
}

S32 xSGSetup(st_XSAVEGAME_DATA* xsgdata)
{
    return xSGSetup(xsgdata, 0, "nothing", -1, 0);
}

static S32 xSG_smem_blkopen(st_XSAVEGAME_DATA* xsgdata);
S32 xSG_sv_flipproc(st_XSAVEGAME_DATA* xsgdata);
static S32 xSG_sv_bldchksum(st_XSAVEGAME_DATA* xsgdata);
static S32 xSG_smem_blkclose(st_XSAVEGAME_DATA* xsgdata);
static S32 xSG_sv_commit(st_XSAVEGAME_DATA* xsgdata);
static S32 xSG_ld_readgame(st_XSAVEGAME_DATA* xsgdata);

S32 xSGProcess(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    if (xsgdata->stage & 0x40000000)
    {
        result = 0;
    }
    else if (!(xsgdata->stage & 1))
    {
        result = 0;
    }
    else if (!(xsgdata->stage & 8))
    {
        result = 0;
    }

    if (result != 0 && xsgdata->mode == XSG_MODE_SAVE)
    {
        if (xSG_smem_blkopen(xsgdata) == 0)
        {
            result = 0;
        }
        if (result != 0 && xSG_sv_flipproc(xsgdata) == 0)
        {
            result = 0;
        }
        if (result != 0 && xSG_sv_bldchksum(xsgdata) == 0)
        {
            result = 0;
        }
        if (result != 0 && xSG_smem_blkclose(xsgdata) == 0)
        {
            result = 0;
        }
        if (result != 0)
        {
            xSG_sv_commit(xsgdata);
        }
    }
    else if (result != 0 && xsgdata->mode == XSG_MODE_LOAD)
    {
        if (xSG_ld_readgame(xsgdata) == 0)
        {
            result = 0;
        }
    }

    xsgdata->stage |= 0x10;
    if (result == 0)
    {
        xsgdata->stage |= 0x40000000;
    }
    return result;
}

S32 xSG_ld_readhead(st_XSAVEGAME_DATA* xsgdata);
static S32 xSG_ld_validate(st_XSAVEGAME_DATA* xsgdata);
S32 xSG_ld_findcltblk(st_XSAVEGAME_DATA* xsgdata);
static S32 xSG_ld_flipload(st_XSAVEGAME_DATA* xsgdata);

// Finishes a save or load; after a load, checks the file and hands each client its block.
S32 xSGWrapup(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    S32 rc = 0;

    if (xsgdata->stage & 0x40000000)
    {
        result = 0;
    }
    else if (!(xsgdata->stage & 1))
    {
        result = 0;
    }
    else if (!(xsgdata->stage & 0x10))
    {
        result = 0;
        xsgdata->stage |= 0x40000000;
    }

    if (xsgdata->mode != XSG_MODE_LOAD)
    {
        return result;
    }
    if (result == 0)
    {
        return result;
    }

    if (result != 0)
    {
        rc = xSG_ld_readhead(xsgdata);
    }
    if (rc == 0)
    {
        result = 0;
    }
    if (result != 0)
    {
        rc = xSG_ld_validate(xsgdata);
    }
    if (rc == 0)
    {
        result = 0;
    }
    if (result != 0)
    {
        rc = xSG_ld_findcltblk(xsgdata);
    }
    if (rc == 0)
    {
        result = 0;
    }
    if (result != 0)
    {
        rc = xSG_ld_flipload(xsgdata);
    }
    if (rc == 0)
    {
        result = 0;
    }

    if (result == 0)
    {
        xsgdata->stage |= 0x40000000;
    }
    return result;
}

en_XSGASYNC_STATUS xSGAsyncStatus(st_XSAVEGAME_DATA* xsgdata, S32 block, en_XSG_WHYFAIL* whyFail,
                                  char* errmsg)
{
    en_XSGASYNC_STATUS xstat = XSG_ASTAT_NOOP;
    en_ASYNC_OPERR whyerr = ISG_OPERR_NONE;

    if (errmsg != NULL)
    {
        errmsg[0] = '\0';
    }

    en_ASYNC_OPSTAT istat = iSGPollStatus(xsgdata->isgsess, NULL, block);
    switch (istat)
    {
    case ISG_OPSTAT_FAILURE:
        xstat = XSG_ASTAT_FAILED;
        whyerr = iSGOpError(xsgdata->isgsess, errmsg);
        break;
    case ISG_OPSTAT_INPROG:
        xstat = XSG_ASTAT_INPROG;
        break;
    case ISG_OPSTAT_SUCCESS:
        xstat = XSG_ASTAT_SUCCESS;
        break;
    }

    if (whyFail != NULL && whyerr != ISG_OPERR_NONE)
    {
        en_XSG_WHYFAIL con = XSG_WHYERR_NONE;

        switch (whyerr)
        {
        case ISG_OPERR_NONE:
            break;
        case ISG_OPERR_NOCARD:
            con = XSG_WHYERR_NOCARD;
            break;
        case ISG_OPERR_NOROOM:
        case ISG_OPERR_SVNOSPACE:
            con = XSG_WHYERR_NOROOM;
            break;
        case ISG_OPERR_DAMAGE:
            con = XSG_WHYERR_NOROOM;
            break;
        default:
            con = XSG_WHYERR_CARDYANKED;
            break;
        }
        *whyFail = con;
    }
    return xstat;
}

// The leader block always reserves 256 bytes.
static S32 xSG_cb_leader_svinfo(void*, st_XSAVEGAME_DATA*, S32* cur_space, S32* max_fullgame)
{
    *cur_space = 0x100;
    *max_fullgame = 0x100;
    return 1;
}

// Writes the leader block: the save's label, thumbnail and play time, then a marker string.
static S32 xSG_cb_leader_svproc(void* cltdata, st_XSAVEGAME_DATA* original_xsgdata,
                                st_XSAVEGAME_WRITECONTEXT* wctxt)
{
    st_XSAVEGAME_DATA* xsg = (st_XSAVEGAME_DATA*)cltdata;
    st_XSAVEGAME_LEADER leader = {};
    char fundata[] = "--TakeMeToYourLeader--";

    strncpy(leader.gameLabel, xsg->label, 0x40);
    leader.gameLabel[63] = '\0';
    leader.gametime = xsg->playtime;
    leader.thumIconIndex = xsg->thumIndex;

    xSGWriteData(original_xsgdata, wctxt, leader.gameLabel, 1, sizeof(st_XSAVEGAME_LEADER));
    xSGWriteData(original_xsgdata, wctxt, fundata, 1, 0x16);
    return 1;
}

// Skips the leader block when a game loads; the load screen reads it separately.
static S32 xSG_cb_leader_load(void*, st_XSAVEGAME_DATA* original_xsgdata,
                              st_XSAVEGAME_READCONTEXT* rctxt, U32, S32)
{
    char fundata[32] = {};
    st_XSAVEGAME_LEADER discard = {};
    xSGReadData(original_xsgdata, rctxt, discard.gameLabel, sizeof(st_XSAVEGAME_LEADER));
    xSGReadData(original_xsgdata, rctxt, fundata, 0x16);
    return 1;
}

S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, char* data,
                 S32 elesiz, S32 n)
{
    S32 cnt;
    S32 is_ok = 1;

    if (xsgdata->stage & 0x40000000)
    {
        return 0;
    }
    if (elesiz == 0)
    {
        return 0;
    }

    cnt = n * elesiz;
    if (cnt == 0)
    {
        return cnt;
    }

    if (xsgdata->upd_tally != 0 && xsgdata->totamt + cnt > xsgdata->memsize)
    {
        xsgdata->stage |= 0x40000000;
        return 0;
    }

    void* mcprc;
    mcprc = memcpy(xsgdata->buf_curpos, data, cnt);
    if (mcprc == NULL)
    {
        is_ok = 0;
    }
    else
    {
        xsgdata->buf_curpos += cnt;
    }

    // The write context is the client being written, which counts its own bytes.
    if (is_ok & xsgdata->upd_tally)
    {
        xsgdata->totamt += cnt;
        if (wctxt != NULL)
        {
            st_XSAVEGAME_CLIENT* clt = (st_XSAVEGAME_CLIENT*)wctxt;
            clt->realamt += cnt;
        }
    }

    if (is_ok == 0)
    {
        xsgdata->stage |= 0x40000000;
        return 0;
    }
    else
    {
        return cnt / elesiz;
    }

    return cnt;
}

S32 xSGWriteStrLen(const char* str)
{
    // Room for the terminator, rounded up to an even length.
    S32 len = strlen(str);
    len += (len + 1 & 1) + 1;
    return len;
}

S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, char* data, S32 n)
{
    return xSGWriteData(xsgdata, wctxt, data, sizeof(char), n);
}

S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, S32* data, S32 n)
{
    return xSGWriteData(xsgdata, wctxt, (char*)data, sizeof(S32), n);
}

S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, U32* data, S32 n)
{
    return xSGWriteData(xsgdata, wctxt, (char*)data, sizeof(U32), n);
}

S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, F32* data, S32 n)
{
    return xSGWriteData(xsgdata, wctxt, (char*)data, sizeof(F32), n);
}

S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, char* buff, S32 elesiz,
                S32 n)
{
    S32 cnt;
    if (elesiz == 0)
    {
        return 0;
    }

    cnt = n * elesiz;
    if (cnt == 0)
    {
        return cnt;
    }

    // The read context is the client whose block is being read, or NULL for the raw file.
    st_XSAVEGAME_CLIENT* clt = (st_XSAVEGAME_CLIENT*)rctxt;
    if (clt != NULL && cnt > clt->readremain)
    {
        xUtil_idtag2string(clt->idtag, 0);
        return 0;
    }

    if (clt != NULL)
    {
        memcpy(buff, clt->readpos, cnt);
    }
    else
    {
        memcpy(buff, xsgdata->walkpos, cnt);
    }

    xsgdata->walkpos += cnt;
    xsgdata->walkremain -= cnt;

    if (clt != NULL)
    {
        clt->readpos += cnt;
        clt->readamt += cnt;
        clt->readremain -= cnt;
    }

    return cnt / elesiz;
}

S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, char* buff, S32 n)
{
    return xSGReadData(xsgdata, rctxt, buff, sizeof(char), n);
}

S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, S32* buff, S32 n)
{
    return xSGReadData(xsgdata, rctxt, (char*)buff, sizeof(S32), n);
}

S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32* buff, S32 n)
{
    return xSGReadData(xsgdata, rctxt, (char*)buff, sizeof(U32), n);
}

S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, F32* buff, S32 n)
{
    return xSGReadData(xsgdata, rctxt, (char*)buff, sizeof(F32), n);
}

S32 xSG_sv_flipinfo(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    S32 cltamt = 0;
    S32 cltmax = 0;

    // Ask each save client for the space it needs; every client block carries a 12-byte header.
    for (S32 i = 0; i < xsgdata->stkcnt; i++)
    {
        st_XSAVEGAME_CLIENT* clt = &xsgdata->cltstk[i];
        cltamt = 0;
        cltmax = 0;

        S32 rc = clt->cltinfo(clt->cltdata, xsgdata, &cltamt, &cltmax);
        if (rc == 0)
        {
            xUtil_idtag2string(clt->idtag, 0);
            result = 0;
        }
        else
        {
            clt->needamt = cltamt;
            clt->maxamt = cltmax;
            xsgdata->cltneed += cltamt;
            xsgdata->cltmax += cltmax;
            xsgdata->chdrneed += 0xc;
        }

        if (result == 0)
        {
            xsgdata->stage |= 0x40000000;
            break;
        }
    }
    return result;
}

// Writes the client's block header: its tag, then room for its reserved and written sizes.
static S32 xSG_smem_cltopen(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_CLIENT* clt)
{
    S32 i = '3333';
    S32 j = 0;

    xUtil_idtag2string(clt->idtag, 0);
    j = clt->idtag;
    xSGWriteData(xsgdata, NULL, (char*)&j, 4, 1);
    clt->buf_maxpos = xsgdata->buf_curpos;
    xSGWriteData(xsgdata, NULL, (char*)&i, 4, 1);
    clt->buf_sizepos = xsgdata->buf_curpos;
    xSGWriteData(xsgdata, NULL, (char*)&i, 4, 1);
    xUtil_idtag2string(clt->idtag, 0);
    return 1;
}

static S32 xSG_smem_cltclose(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_CLIENT* clt);

S32 xSG_sv_flipproc(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    S32 rc;
    char bfill = 0xbf;

    // Each client writes its block, padded with 0xBF up to the size it reserved.
    for (S32 i = 0; i < xsgdata->stkcnt; i++)
    {
        st_XSAVEGAME_CLIENT* clt = &xsgdata->cltstk[i];

        rc = xSG_smem_cltopen(xsgdata, clt);
        if (rc != 0)
        {
            rc = clt->cltproc(clt->cltdata, xsgdata, (st_XSAVEGAME_WRITECONTEXT*)clt);
            if (rc == 0)
            {
                xUtil_idtag2string(clt->idtag, 0);
                result = 0;
            }

            S32 needfill = clt->maxamt - clt->realamt;
            xUtil_idtag2string(clt->idtag, 0);
            while (needfill-- > 0)
            {
                xSGWriteData(xsgdata, NULL, &bfill, 1, 1);
            }
        }

        if (rc != 0 && xSG_smem_cltclose(xsgdata, clt) == 0)
        {
            result = 0;
            xUtil_idtag2string(clt->idtag, 0);
        }

        if (result == 0)
        {
            break;
        }
    }
    return result;
}

static S32 xSG_sv_bldchksum(st_XSAVEGAME_DATA* xsgdata)
{
    xsgdata->chksum =
        xUtil_crc_update(xsgdata->chksum, xsgdata->membuf + 16, xsgdata->memsize - 16);
    return 1;
}

// Writes the file header: the 'GDAT' tag and version 1, then room for the size and checksum.
static S32 xSG_smem_blkopen(st_XSAVEGAME_DATA* xsgdata)
{
    S32 i = '3333';
    S32 j = 'GDAT';
    S32 k = 1;

    xSGWriteData(xsgdata, NULL, (char*)&j, 4, 1);
    xSGWriteData(xsgdata, NULL, (char*)&k, 4, 1);

    xsgdata->buf_sizespot = xsgdata->buf_curpos;
    xSGWriteData(xsgdata, NULL, (char*)&i, 4, 1);
    xsgdata->buf_cksmspot = xsgdata->buf_curpos;
    xSGWriteData(xsgdata, NULL, (char*)&i, 4, 1);
    return 1;
}

static S32 xSG_smem_blkclose(st_XSAVEGAME_DATA* xsgdata)
{
    xsgdata->upd_tally = 0;

    char* last_bufpos = xsgdata->buf_curpos;
    xsgdata->buf_curpos = xsgdata->buf_sizespot;
    xSGWriteData(xsgdata, NULL, (char*)&xsgdata->totamt, 4, 1);
    xsgdata->buf_curpos = last_bufpos;

    last_bufpos = xsgdata->buf_curpos;
    xsgdata->buf_curpos = xsgdata->buf_cksmspot;
    xSGWriteData(xsgdata, NULL, (char*)&xsgdata->chksum, 4, 1);
    xsgdata->buf_curpos = last_bufpos;
    xsgdata->upd_tally = 1;
    return 1;
}

static S32 xSG_smem_cltclose(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_CLIENT* clt)
{
    xUtil_idtag2string(clt->idtag, 0);
    xsgdata->upd_tally = 0;
    if (clt->needamt < clt->realamt)
    {
        xUtil_idtag2string(clt->idtag, 0);
    }

    char* last_bufpos = xsgdata->buf_curpos;
    xsgdata->buf_curpos = clt->buf_maxpos;
    xSGWriteData(xsgdata, NULL, (char*)&clt->maxamt, 4, 1);
    xsgdata->buf_curpos = last_bufpos;

    xsgdata->buf_curpos = clt->buf_sizepos;
    xSGWriteData(xsgdata, NULL, (char*)&clt->realamt, 4, 1);
    xsgdata->buf_curpos = last_bufpos;
    xsgdata->upd_tally = 1;

    xUtil_idtag2string(clt->idtag, 0);
    return 1;
}

static void xSG_areaComposeLabel(char* label, char*)
{
    char buf[64] = {};
    iSGMakeTimeStamp(buf);
    sprintf(label, "%s", buf);
}

static S32 xSG_sv_commit(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    char browselabel[64] = {};

    char* name = iSGMakeName(ISG_NGTYP_GAMEDIR, NULL, 0);
    if (!iSGSetupGameDir(xsgdata->isgsess, name, 1))
    {
        result = 0;
    }
    else
    {
        xSG_areaComposeLabel(browselabel, xsgdata->label);

        if (!iSGSaveFile(xsgdata->isgsess, iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, xsgdata->gslot),
                         xsgdata->membuf, xsgdata->memsize, 1, browselabel))
        {
            result = 0;
        }
    }

    return result;
}

void xSG_cb_ISGChange(void*, en_CHGCODE what)
{
}

static S32 xSG_ld_readgame(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    if (xsgdata->stage & 0x40000000)
    {
        result = 0;
    }

    char* name = iSGMakeName(ISG_NGTYP_GAMEFILE, NULL, xsgdata->gslot);
    if (!iSGLoadFile(xsgdata->isgsess, name, xsgdata->loadbuf, 1))
    {
        result = 0;
    }

    if (result == 0)
    {
        xsgdata->stage |= 0x40000000;
    }

    return result;
}

// Checks the file header (the GDAT tag and version 1) and reads its size and checksum.
S32 xSG_ld_readhead(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    S32 ival = 0;

    xsgdata->walkpos = xsgdata->loadbuf;
    xsgdata->walkremain = xsgdata->loadsize;
    xSGReadData(xsgdata, NULL, &ival, 1);
    if (ival != 'GDAT')
    {
        result = 0;
    }

    xSGReadData(xsgdata, NULL, &ival, 1);
    if (ival != 1)
    {
        result = 0;
    }

    xSGReadData(xsgdata, NULL, &ival, 1);
    xsgdata->readsize = ival;

    xSGReadData(xsgdata, NULL, &ival, 1);
    xsgdata->file_chksum = ival;
    return result;
}

static S32 xSG_ld_validate(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;

    xsgdata->loadsize = xsgdata->readsize;
    xsgdata->walkpos = xsgdata->loadbuf + 0x10;
    xsgdata->walkremain = xsgdata->loadsize - 0x10;
    xsgdata->read_chksum =
        xUtil_crc_update(xsgdata->read_chksum, xsgdata->walkpos, xsgdata->walkremain);

    if (xsgdata->read_chksum != xsgdata->file_chksum)
    {
        result = 0;
    }

    return result;
}

// Walks the client blocks of a loaded game and points each registered client at its block.
// Blocks no client claims go to the default loader, or are skipped.
S32 xSG_ld_findcltblk(st_XSAVEGAME_DATA* xsgdata)
{
    S32 ival = 0;
    U32 tag = 0;
    S32 maxamt = 0;
    S32 actamt = 0;
    st_XSAVEGAME_CLIENT* dfltclt = &xsgdata->dfltloadclt;

    xsgdata->walkpos = xsgdata->loadbuf;
    xsgdata->walkremain = xsgdata->loadsize;
    xSGReadData(xsgdata, NULL, &ival, 1);
    xSGReadData(xsgdata, NULL, &ival, 1);
    xSGReadData(xsgdata, NULL, &ival, 1);
    xSGReadData(xsgdata, NULL, &ival, 1);

    while (xsgdata->walkremain > 0)
    {
        tag = 0;
        S32 found = 0;
        maxamt = 0;
        actamt = 0;

        xSGReadData(xsgdata, NULL, &tag, 1);
        xSGReadData(xsgdata, NULL, &maxamt, 1);
        xSGReadData(xsgdata, NULL, &actamt, 1);
        xUtil_idtag2string(tag, 0);

        for (S32 i = 0; i < xsgdata->stkcnt; i++)
        {
            st_XSAVEGAME_CLIENT* clt = &xsgdata->cltstk[i];

            if (clt->idtag == tag)
            {
                found = 1;
                clt->blokpos = xsgdata->walkpos;
                clt->blokact = actamt;
                clt->blokmax = maxamt;
                clt->readpos = xsgdata->walkpos;
                clt->readamt = 0;
                clt->readremain = actamt;
                break;
            }
        }

        if (!found)
        {
            if (dfltclt->idtag != 0)
            {
                xUtil_idtag2string(tag, 0);
                dfltclt->blokpos = xsgdata->walkpos;
                dfltclt->blokact = actamt;
                dfltclt->blokmax = maxamt;
                dfltclt->readpos = xsgdata->walkpos;
                dfltclt->readamt = 0;
                dfltclt->readremain = actamt;

                S32 rc =
                    dfltclt->cltload(dfltclt->cltdata, xsgdata, (st_XSAVEGAME_READCONTEXT*)dfltclt,
                                     tag, dfltclt->blokact);
                if (rc == 0)
                {
                    xUtil_idtag2string(tag, 0);
                }

                rc = dfltclt->readremain;
                if (rc > 0)
                {
                    xsgdata->walkpos += rc + (maxamt - actamt);
                    xsgdata->walkremain -= dfltclt->readremain + (maxamt - actamt);
                }
            }
            else
            {
                xUtil_idtag2string(tag, 0);
                xsgdata->walkpos += maxamt;
                xsgdata->walkremain -= maxamt;
            }
        }
        else
        {
            xsgdata->walkpos += maxamt;
            xsgdata->walkremain -= maxamt;
        }
    }
    return 1;
}

static S32 xSG_ld_flipload(st_XSAVEGAME_DATA* xsgdata)
{
    S32 result = 1;
    S32 rc;

    for (S32 i = 0; i < xsgdata->stkcnt; i++)
    {
        st_XSAVEGAME_CLIENT* clt = &xsgdata->cltstk[i];
        if (clt->cltload != NULL && clt->blokact != 0 && clt->blokpos != NULL)
        {
            xUtil_idtag2string(clt->idtag, 0);

            rc = clt->cltload(clt->cltdata, xsgdata, (st_XSAVEGAME_READCONTEXT*)clt, clt->idtag,
                              clt->blokact);
            if (rc == 0)
            {
                xUtil_idtag2string(clt->idtag, 0);
                result = 0;
            }
        }
        else
        {
            xUtil_idtag2string(clt->idtag, 0);
        }

        if (result == 0)
        {
            break;
        }
    }

    return result;
}

