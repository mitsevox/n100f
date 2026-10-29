#ifndef XSTRANSVC_H
#define XSTRANSVC_H

#include "types.h"

void* xSTFindAsset(U32 aid, U32* size);
char* xSTAssetName(U32 aid);
void* xSTFindAssetByType(U32 type, S32 idx, U32* size);
S32 xSTAssetCountByType(U32 type);

struct st_xST_XASSETINFO
{
    U32 aid;
    U32 sector;
    U32 plus_offset;
    U32 size;
    void* mempos;
};

S32 xSTGetAssetInfoByType(U32 type, S32 idx, st_xST_XASSETINFO* ainfo);

struct st_PACKER_ASSETTYPE;
S32 xSTStartup(st_PACKER_ASSETTYPE* handlers);
S32 xSTShutdown();

#endif
