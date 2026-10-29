#ifndef XSTRANSVC_H
#define XSTRANSVC_H

#include "types.h"

void* xSTFindAsset(U32 aid, U32* size);
char* xSTAssetName(U32 aid);
void* xSTFindAssetByType(U32 type, S32 idx, U32* size);
S32 xSTAssetCountByType(U32 type);

#endif
