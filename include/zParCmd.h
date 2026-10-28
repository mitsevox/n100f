#ifndef ZPARCMD_H
#define ZPARCMD_H

#include "types.h"

struct xParGroup;

struct xParCmdAsset
{
    U32 type;
    U8 enabled;
    U8 mode;
    U8 pad[2];
};

struct xParCmdTex : xParCmdAsset
{
    F32 x1;
    F32 y1;
    F32 x2;
    F32 y2;
    U8 birthMode;
    U8 rows;
    U8 cols;
    U8 unit_count;
    F32 unit_width;
    F32 unit_height;
};

struct xParCmd
{
    U32 flag;
};

struct zParCmd : xParCmd
{
    xParCmdAsset* tasset;
};

#define XPARCMD_TYPE_TEX 0xC

void (*zParCmdGetUpdateFunc(U32 parType))(zParCmd*, xParGroup*, F32);
U32 zParCmdGetSize(U32 parType);

#endif
