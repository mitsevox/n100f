#ifndef XSND_H
#define XSND_H

#include "types.h"

#include "xVec3.h"

U32 xSndPlay3D(U32 id, F32 vol, F32 pitch, U32 priority, U32 flags, xVec3* pos, F32 radius);
U32 xSndPlay(U32 id, F32 volL, F32 volR, F32 pitch, U32 priority, U32 flags, U32 parentID);

#endif
