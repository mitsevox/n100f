#ifndef XSAVEGAME_H
#define XSAVEGAME_H

#include "types.h"
#include "iTime.h"

struct st_ISGSESSION;
struct st_XSAVEGAME_DATA;
struct st_XSAVEGAME_READCONTEXT;
struct st_XSAVEGAME_WRITECONTEXT;

enum en_SAVEGAME_MODE
{
    XSG_MODE_LOAD = 10,
    XSG_MODE_SAVE = 11
};

struct st_XSAVEGAME_CLIENT
{
    U32 idtag;
    S32 (*cltinfo)(void*, st_XSAVEGAME_DATA*, S32*, S32*);
    S32 (*cltproc)(void*, st_XSAVEGAME_DATA*, st_XSAVEGAME_WRITECONTEXT*);
    S32 (*cltload)(void*, st_XSAVEGAME_DATA*, st_XSAVEGAME_READCONTEXT*, U32, S32);
    void* cltdata;
    S32 needamt;
    S32 maxamt;
    S32 realamt;
    char* buf_sizepos;
    char* buf_maxpos;
    S32 blokact;
    S32 blokmax;
    char* blokpos;
    char* readpos;
    S32 readamt;
    S32 readremain;
};

struct st_XSAVEGAME_DATA
{
    S32 gfile_idx;
    en_SAVEGAME_MODE mode;
    U32 stage;
    S32 gslot;
    char label[64];
    S32 thumIndex;
    iTime playtime;
    char* membuf;
    S32 memsize;
    char* buf_curpos;
    char* buf_sizespot;
    char* buf_cksmspot;
    S32 totamt;
    U32 chksum;
    U32 upd_tally;
    S32 cltneed;
    S32 cltmax;
    S32 chdrneed;
    S32 stkcnt;
    st_XSAVEGAME_CLIENT cltstk[128];
    st_XSAVEGAME_CLIENT dfltloadclt;
    U32 file_chksum;
    U32 read_chksum;
    S32 readsize;
    char* loadbuf;
    S32 loadsize;
    char* walkpos;
    S32 walkremain;
    st_ISGSESSION* isgsess;
};

// The leader block that opens every saved game: what the load screen shows for it.
struct st_XSAVEGAME_LEADER
{
    char gameLabel[64];
    S32 thumIconIndex;
    iTime gametime;
};

enum en_XSGASYNC_STATUS
{
    XSG_ASTAT_NOOP = 0,
    XSG_ASTAT_INPROG = 1,
    XSG_ASTAT_SUCCESS = 2,
    XSG_ASTAT_FAILED = 3
};

enum en_XSG_WHYFAIL
{
    XSG_WHYERR_NONE,
    XSG_WHYERR_NOCARD,
    XSG_WHYERR_NOROOM,
    XSG_WHYERR_DAMAGE,
    XSG_WHYERR_CARDYANKED,
    XSG_WHYERR_OTHER,
    XSG_WHYERR_NOMORE
};

S32 xSGShutdown();
st_XSAVEGAME_DATA* xSGInit(en_SAVEGAME_MODE mode);
S32 xSGDone(st_XSAVEGAME_DATA* xsgdata);
S32 xSGTgtCount(st_XSAVEGAME_DATA* xsgdata, S32* max);
S32 xSGTgtPhysSlotIdx(st_XSAVEGAME_DATA* xsgdata, S32 tidx);
S32 xSGTgtIsFormat(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32* badEncode);
S32 xSGTgtSelect(st_XSAVEGAME_DATA* xsgdata, S32 tidx);
void xSGGameSet(st_XSAVEGAME_DATA* xsgdata, S32 gidx);
char* xSGGameModDate(st_XSAVEGAME_DATA* xsgdata, S32 gidx);
char* xSGGameLabel(st_XSAVEGAME_DATA* xsgdata, S32 gidx);
S32 xSGGameThumIndex(st_XSAVEGAME_DATA* xsgdata, S32 gidx);
S32 xSGSetup(st_XSAVEGAME_DATA* xsgdata);
S32 xSGSetup(st_XSAVEGAME_DATA* xsgdata, S32 gidx, char* label, S32 thumIdx, iTime playtime);
S32 xSGProcess(st_XSAVEGAME_DATA* xsgdata);
S32 xSGWrapup(st_XSAVEGAME_DATA* xsgdata);
en_XSGASYNC_STATUS xSGAsyncStatus(st_XSAVEGAME_DATA* xsgdata, S32 block, en_XSG_WHYFAIL* whyFail,
                                  char* errmsg);
S32 xSGTgtFormatTgt(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32* canRecover);
S32 xSGTgtHasGameDir(st_XSAVEGAME_DATA* xsgdata, S32 tidx);
S32 xSGTgtHaveRoom(st_XSAVEGAME_DATA* xsgdata, S32 tidx, S32 fsize, S32 slotidx, S32* bytesNeeded,
                   S32* availOnDisk, S32* needFile);
S32 xSGGameIsEmpty(st_XSAVEGAME_DATA* xsgdata, S32 gidx);
S32 xSGGameSize(st_XSAVEGAME_DATA* xsgdata, S32 gidx);
S32 xSGAddSaveClient(st_XSAVEGAME_DATA* xsgdata, U32 clttag, void* cltdata,
                     S32 (*infofunc)(void*, st_XSAVEGAME_DATA*, S32*, S32*),
                     S32 (*procfunc)(void*, st_XSAVEGAME_DATA*, st_XSAVEGAME_WRITECONTEXT*));
S32 xSGAddLoadClient(st_XSAVEGAME_DATA* xsgdata, U32 clttag, void* cltdata,
                     S32 (*loadfunc)(void*, st_XSAVEGAME_DATA*, st_XSAVEGAME_READCONTEXT*, U32,
                                     S32));
S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, char* data,
                 S32 elesiz, S32 n);
S32 xSGWriteStrLen(const char* str);
S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, char* data, S32 n);
S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, S32* data, S32 n);
S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, U32* data, S32 n);
S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, F32* data, S32 n);
S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, char* buff, S32 elesiz,
                S32 n);
S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, char* buff, S32 n);
S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, S32* buff, S32 n);
S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, U32* buff, S32 n);
S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, F32* buff, S32 n);
S32 xSG_chdir_gamedir(st_XSAVEGAME_DATA* xsgdata);
char* xSG_cm_slotname(st_XSAVEGAME_DATA* xsgdata, S32 gidx);
S32 xSG_sv_flipinfo(st_XSAVEGAME_DATA* xsgdata);

#endif
