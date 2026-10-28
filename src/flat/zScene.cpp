#include "zScene.h"

#include "xString.h"
#include "xstransvc.h"
#include "xutil.h"

#include <stdio.h>
#include <string.h>

static IDBasePair* scobj_idbps;
static S32 scobj_size = -1;

// Open addressing on the low bits of the id; scobj_size is a power of two and an
// id of 0 marks an empty slot.
static xBase* zSceneObjHashtableGet(U32 id)
{
    S32 k, chkd;

    chkd = id & (scobj_size - 1);

    for (k = 0; k < scobj_size; k++)
    {
        IDBasePair* idbp = &scobj_idbps[chkd];

        if (idbp->id == id)
        {
            return idbp->base;
        }

        if (idbp->id == 0)
        {
            return NULL;
        }

        chkd++;

        if (chkd == scobj_size)
        {
            chkd = 0;
        }
    }

    return NULL;
}

const char* zSceneGetName(U32 gameID)
{
    xBase* b = zSceneFindObject(gameID);

    if (b)
    {
        return zSceneGetName(b);
    }

    return "";
}

const char* zSceneGetName(xBase* b)
{
    if (b)
    {
        const char* n = xSTAssetName(b->id);

        if (n)
        {
            return n;
        }
    }

    return "";
}

xBase* zSceneFindObject(U32 gameID)
{
    return zSceneObjHashtableGet(gameID);
}

// A set bit in the save sets base flag 0x1 on the object named after the client's id
// tag, its second character replaced by a space, followed by " VISITED".
S32 zSceneSetup_serialTraverseCB(U32 clientID, xSerial* xser)
{
    char uiName[16];
    S32 val = 0;
    xBase* b;

    xser->Read_b1(&val);

    if (val == 0)
    {
        return 1;
    }

    sprintf(uiName, "%s", xUtil_idtag2string(clientID, 0));

    uiName[1] = ' ';

    strcat(uiName, " VISITED");

    b = zSceneFindObject(xStrHash(uiName));

    if (b)
    {
        b->baseFlags |= 0x1;
    }

    return 1;
}
