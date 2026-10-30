#ifndef ISAVEGAME_H
#define ISAVEGAME_H

#include "types.h"

#include <dolphin/card.h>

enum en_ASYNC_OPERR
{
    ISG_OPERR_NONE,
    ISG_OPERR_NOOPER,
    ISG_OPERR_MULTIOPER,
    ISG_OPERR_INITFAIL,
    ISG_OPERR_GAMEDIR,
    ISG_OPERR_NOCARD,
    ISG_OPERR_NOROOM,
    ISG_OPERR_DAMAGE,
    ISG_OPERR_CORRUPT,
    ISG_OPERR_OTHER,
    ISG_OPERR_SVNOSPACE,
    ISG_OPERR_SVINIT,
    ISG_OPERR_SVWRITE,
    ISG_OPERR_SVOPEN,
    ISG_OPERR_LDINIT,
    ISG_OPERR_LDREAD,
    ISG_OPERR_LDOPEN,
    ISG_OPERR_TGTERR,
    ISG_OPERR_TGTREM,
    ISG_OPERR_TGTPREP,
    ISG_OPERR_UNKNOWN,
    ISG_OPERR_NOMORE
};

enum en_CHGCODE
{
    ISG_CHG_NONE,
    ISG_CHG_TARGET,
    ISG_CHG_GAMELIST
};

enum en_NAMEGEN_TYPE
{
    ISG_NGTYP_GAMEDIR,
    ISG_NGTYP_GAMEFILE,
    ISG_NGTYP_CONFIG,
    ISG_NGTYP_ICONTHUM
};

enum en_ASYNC_OPCODE
{
    ISG_OPER_NOOP,
    ISG_OPER_INIT,
    ISG_OPER_SAVE,
    ISG_OPER_LOAD
};

enum en_ASYNC_OPSTAT
{
    ISG_OPSTAT_FAILURE = -1,
    ISG_OPSTAT_INPROG,
    ISG_OPSTAT_SUCCESS
};

// One memory card slot. The GameCube layout differs from the PS2 class of this name.
struct st_ISG_MEMCARD_DATA
{
    S32 unk_0;
    S32 chan;
    S32 sectorSize;
    CARDFileInfo finfo;
    CARDStat fstat;
    S32 unk_8C[3];
    S32 unk_98;
    CARDFileInfo unk_9C;
    CARDStat unk_B0;
    S32 unk_11C[4];
    S32 unk_12C;
};

#define ISG_NUM_SLOTS 2

struct st_ISGSESSION
{
    st_ISG_MEMCARD_DATA mcdata[ISG_NUM_SLOTS];
    S32 slot;
    S32 unk_264;
    en_ASYNC_OPERR as_operr;
    en_ASYNC_OPSTAT as_opstat;
    S32 unk_270;
    S32 unk_274;
};

S32 iSGStartup();
S32 iSGShutdown();
st_ISGSESSION* iSGSessionBegin(void* cltdata, void (*chgfunc)(void*, en_CHGCODE));
void iSGSessionEnd(st_ISGSESSION* isgdata);
en_ASYNC_OPSTAT iSGPollStatus(st_ISGSESSION* isgdata, en_ASYNC_OPCODE* curop, S32 block);
S32 iSGSetupGameDir(st_ISGSESSION* isgdata, const char* dname, S32 force_iconfix);
char* iSGMakeName(en_NAMEGEN_TYPE type, const char* base, S32 idx);
void iSGMakeTimeStamp(char* str);
en_ASYNC_OPERR iSGOpError(st_ISGSESSION* isgdata, char* errmsg);
S32 iSGTgtCount(st_ISGSESSION* isgdata, S32* max);
S32 iSGTgtPhysSlotIdx(st_ISGSESSION* isgdata, S32 tidx);
S32 iSGTgtSetActive(st_ISGSESSION* isgdata, S32 tgtidx);
S32 iSGTgtFormat(st_ISGSESSION* isgdata, S32 tgtidx, S32 async, S32* canRecover);
S32 iSGTgtHaveRoom(st_ISGSESSION* isgdata, S32 tidx, S32 fsize, const char* dpath,
                   const char* fname, S32* bytesNeeded, S32* availOnDisk, S32* needFile);
S32 iSGSelectGameDir(st_ISGSESSION* isgdata, const char* dname);
S32 iSGFileSize(st_ISGSESSION* isgdata, const char* fname);

#endif
