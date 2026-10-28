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

void xAnimTableAddFile(xAnimTable* table, xAnimFile* file, char* states)
{
    U8* buffer = giAnimScratch;
    char* stateName = xStrTokBuffer(states, " ,\t\n\r", buffer);

    while (stateName != NULL)
    {
        xAnimTableAddFileID(table, file, xStrHash(stateName));
        stateName = xStrTokBuffer(NULL, " ,\t\n\r", buffer);
    }
}

xAnimState* xAnimTableAddFileID(xAnimTable* table, xAnimFile* file, U32 stateID)
{
    xAnimState* state = xAnimTableGetStateID(table, stateID);

    if (state)
    {
        state->Data = file;

        if (file->FileFlags & 0x8000)
        {
            table->MorphIndex |= 1 << (state->Flags & 0xF);
        }
        else
        {
            table->AnimIndex |= 1 << (state->Flags & 0xF);
        }
    }

    return state;
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
