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
S32 xSTGetAssetInfo(U32 aid, st_xST_XASSETINFO* ainfo);
char* xST_xAssetID_HIPFullPath(U32 aid);
S32 xSTPreLoadScene(U32 sid, void* userdata);
S32 xSTQueueSceneAssets(U32 sid);
void xSTUnLoadScene(U32 sid);
F32 xSTLoadStep(U32);
S32 xSTSwitchScene(U32 sid, void* userdata, S32 (*progmon)(void*, F32));

struct st_PACKER_ASSETTYPE;
S32 xSTStartup(st_PACKER_ASSETTYPE* handlers);
S32 xSTShutdown();

#endif
