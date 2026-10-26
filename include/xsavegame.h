#ifndef XSAVEGAME_H
#define XSAVEGAME_H

#include "types.h"

struct st_XSAVEGAME_DATA;
struct st_XSAVEGAME_READCONTEXT;
struct st_XSAVEGAME_WRITECONTEXT;

S32 xSGReadData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_READCONTEXT* rctxt, S32* buff, S32 n);
S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, S32* data, S32 n);
S32 xSGWriteData(st_XSAVEGAME_DATA* xsgdata, st_XSAVEGAME_WRITECONTEXT* wctxt, char* data, S32 n);

#endif
