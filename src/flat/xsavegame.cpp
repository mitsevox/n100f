#include "xsavegame.h"

#include "isavegame.h"
#include "xutil.h"

#include <string.h>

static st_XSAVEGAME_DATA g_xsgdata = {};
static S32 g_xsginit;

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

S32 xSGTgtCount(st_XSAVEGAME_DATA* xsgdata, S32* max)
{
    return iSGTgtCount(xsgdata->isgsess, max);
}

S32 xSGTgtPhysSlotIdx(st_XSAVEGAME_DATA* xsgdata, S32 tidx)
{
    return iSGTgtPhysSlotIdx(xsgdata->isgsess, tidx);
}

S32 xSGTgtFormatTgt(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32* canRecover)
{
    return iSGTgtFormat(xsgdata->isgsess, tidx, 0, canRecover);
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

// The leader block always reserves 256 bytes.
static S32 xSG_cb_leader_svinfo(void*, st_XSAVEGAME_DATA*, S32* cur_space, S32* max_fullgame)
{
    *cur_space = 0x100;
    *max_fullgame = 0x100;
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

void xSG_cb_ISGChange(void*, en_CHGCODE what)
{
}
