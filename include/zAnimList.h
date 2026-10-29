#ifndef ZANIMLIST_H
#define ZANIMLIST_H

#include "types.h"
#include "xAnim.h"

struct zAnimListAsset
{
    U32 ids[10];
};

S32 zAnimListGetNumUsed(U32 id);
xAnimTable* zAnimListGetTable(U32 id);
void zAnimListExit();
void zAnimListInit();

#endif
