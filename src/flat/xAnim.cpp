#include "xAnim.h"
#include "iAnim.h"
#include "xString.h"

#include <string.h>

static xMemPool sxAnimTempTranPool;

void xAnimInit()
{
    iAnimInit();
    memset(&sxAnimTempTranPool, 0, sizeof(xMemPool));
}

xAnimState* xAnimTableGetStateID(xAnimTable* table, U32 ID)
{
    xAnimState* curr;

    curr = table->StateList;

    while (curr)
    {
        if (curr->ID == ID)
        {
            return curr;
        }

        curr = curr->Next;
    }

    return NULL;
}

xAnimState* xAnimTableGetState(xAnimTable* table, char* name)
{
    return xAnimTableGetStateID(table, xStrHash(name));
}

xAnimPlay* xAnimPoolAlloc(xMemPool* pool, void* object, xAnimTable* table,
                          xModelInstance* modelInst)
{
    xAnimPlay* play;

    play = (xAnimPlay*)xMemPoolAlloc(pool);

    xAnimPlaySetup(play, object, table, modelInst);

    return play;
}
