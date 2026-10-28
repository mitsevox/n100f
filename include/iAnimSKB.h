#ifndef IANIMSKB_H
#define IANIMSKB_H

#include "types.h"
#include "xMath3.h"

struct iAnimSKBHeader
{
    U32 Magic;
    U32 Flags;
    U16 BoneCount;
    U16 TimeCount;
    U32 KeyCount;
    F32 Scale[3];
};

void _iAnimSKBFlip180(iAnimSKBHeader* data, U32 bone);
void _iAnimSKBAdjustTranslate(iAnimSKBHeader* data, U32 bone, F32* tran);
void _iAnimSKBSwapTranslate(iAnimSKBHeader* data, U32 bonelo, U32 bonehi);
void _iAnimSKBFixTransPop(void* data, void* pose, U32 maxbone);
F32 iAnimDurationSKB(iAnimSKBHeader* data);
void iAnimEvalSKB(iAnimSKBHeader* data, F32 time, U32 flags, xVec3* tran, xQuat* quat);

#endif
