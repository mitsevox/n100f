#include "xserializer.h"
#include "xordarray.h"
#include "xsavegame.h"
#include "xutil.h"

#include <string.h>

struct st_SERIAL_CLIENTINFO
{
    U32 idtag;
    S32* membuf;
    S32 trueoff;
    S32 actsize;
};

struct st_XSERIAL_DATA_PRIV
{
    S32 flg_info;
    S32* bitbuf;
    S32 buf_bytcnt;
    st_SERIAL_CLIENTINFO* cltbuf;
    st_SERIAL_CLIENTINFO* cltnext;
    st_XORDEREDARRAY cltlist;
};

static S32 g_serinit;
static st_XSERIAL_DATA_PRIV g_xserdata = { 0, NULL, 0, NULL, NULL, NULL };

static S32 g_tbl_onbit[32] = { 0 };

static S32 g_tbl_clear[32] = { 0 };

static void xSER_init_tables();
static void xSER_init_buffers(S32 count, st_SERIAL_PERCID_SIZE* sizeinfo);

S32 xSerialStartup(S32 count, st_SERIAL_PERCID_SIZE* sizeinfo)
{
    if (g_serinit++ == 0)
    {
        memset(&g_xserdata, 0, sizeof(st_XSERIAL_DATA_PRIV));
        xSER_init_tables();
        xSER_init_buffers(count, sizeinfo);
    }

    return g_serinit;
}

xSerial::~xSerial()
{
    if (ctxtdata != NULL)
    {
        xUtil_idtag2string(idtag, 0);
    }
}

void xSerialWipeMainBuffer()
{
    memset(g_xserdata.bitbuf, 0, g_xserdata.buf_bytcnt);
}

static inline void xSER_init_tables()
{
    for (S32 i = 0; i < 32; i++)
    {
        g_tbl_onbit[i] = 1 << i;
        g_tbl_clear[i] = ~(1 << i);
    }
}

static S32 xSER_xsgclt_svproc_ver(void*, st_XSAVEGAME_DATA* xsg, st_XSAVEGAME_WRITECONTEXT* wctxt)
{
    S32 ver = 1;
    xSGWriteData(xsg, wctxt, &ver, 1);
    return 1;
}

static S32 xSER_xsgclt_ldproc_ver(void*, st_XSAVEGAME_DATA* xsg, st_XSAVEGAME_READCONTEXT* rctxt,
                                  U32, S32)
{
    st_XSERIAL_DATA_PRIV* xsd = &g_xserdata;
    S32 ver = 0;
    xSGReadData(xsg, rctxt, &ver, 1);
    if (ver != 1)
    {
        xsd->flg_info |= 1;
    }
    return 1;
}

static S32 xSER_xsgclt_svinfo_clt(void* cltdata, st_XSAVEGAME_DATA*, S32* cur_space,
                                  S32* max_fullgame)
{
    st_SERIAL_CLIENTINFO* clt = (st_SERIAL_CLIENTINFO*)cltdata;
    *cur_space = clt->actsize;
    *max_fullgame = clt->actsize;
    return 1;
}

static S32 xSER_xsgclt_svproc_fill(void*, st_XSAVEGAME_DATA* xsg, st_XSAVEGAME_WRITECONTEXT* wctxt)
{
    char filbuf[9] = { 'R', 'y', 'a', 'n', 'N', 'e', 'i', 'l', '\x00' };
    xSGWriteData(xsg, wctxt, (char*)&filbuf, 8);
    return 1;
}
