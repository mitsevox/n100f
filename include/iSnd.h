#ifndef ISND_H
#define ISND_H

#include "types.h"

struct iSndFileInfo;

iSndFileInfo* iSndLookup(U32 id);
U32 iSndIsPlaying(U32 assetID);

#endif
