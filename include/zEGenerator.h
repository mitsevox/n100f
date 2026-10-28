#ifndef ZEGENERATOR_H
#define ZEGENERATOR_H

#include "zEnt.h"

struct xAnimFile;
struct _tagLightning;

struct zEGenerator : _zEnt
{
    U16 flags;
    U16 num_dsts;
    F32 tmr;
    xAnimFile* afile;
    xVec3 src_pos;
    xVec3 dst_pos;
    xBase* dst;
    _tagLightning* lfx[2];
};

#endif
