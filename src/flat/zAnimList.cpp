#include "zAnimList.h"

#include "xMemMgr.h"
#include "xstransvc.h"


static S32 nals;
static U32* aids;
static xAnimTable** atbls;
static S32* anused;

static char* astnames[20] = {
    "stop0", "stop1", "stop2", "stop3", "stop4", "stop5", "stop6", "stop7", "stop8", "stop9",
    "loop0", "loop1", "loop2", "loop3", "loop4", "loop5", "loop6", "loop7", "loop8", "loop9",
};

// A transition that is always taken.
U32 AlwaysConditional(xAnimTransition*, xAnimSingle*, void*)
{
    return 1;
}

// Build an animation table for each animation list ('ALST') asset: slot 0 is the "idle" state,
// and each other slot j gives a one-shot "stopj" state, which returns to idle when one exists,
// and a looping "loopj" state.
void zAnimListInit()
{
    nals = xSTAssetCountByType('ALST');
    if (nals == 0)
    {
        return;
    }

    aids = (U32*)xMemAlloc(gActiveHeap, nals * sizeof(U32));
    atbls = (xAnimTable**)xMemAlloc(gActiveHeap, nals * sizeof(xAnimTable*));
    anused = (S32*)xMemAlloc(gActiveHeap, nals * sizeof(S32));

    for (S32 i = 0; i < nals; i++)
    {
        U32 size;
        zAnimListAsset* zala = (zAnimListAsset*)xSTFindAssetByType('ALST', i, &size);
        st_xST_XASSETINFO ainfo;
        xSTGetAssetInfoByType('ALST', i, &ainfo);
        xAnimTable* atbl = xAnimTableNew("", NULL);

        aids[i] = ainfo.aid;
        atbls[i] = atbl;
        anused[i] = 0;

        void* buf = xSTFindAsset(zala->ids[0], &size);
        S32 idle_exists = (S32)buf;
        if (buf != NULL)
        {
            xAnimFile* afile = xAnimFileNew(buf, "", 0, NULL);
            xAnimTableNewState(atbl, "idle", 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
            xAnimTableAddFile(atbl, afile, "idle");
            anused[i]++;
        }

        for (S32 j = 1; j < 10; j++)
        {
            if (zala->ids[j] == 0)
            {
                continue;
            }

            buf = xSTFindAsset(zala->ids[j], &size);
            if (buf == NULL)
            {
                continue;
            }

            xAnimFile* afile = xAnimFileNew(buf, "", 0, NULL);
            if (idle_exists)
            {
                xAnimTableNewState(atbl, astnames[j], 0x20, 0, 1.0f, NULL, NULL, 0.0f, NULL);
                xAnimTableNewTransition(atbl, astnames[j], "idle", AlwaysConditional, NULL, 0x10,
                                        0, 0.0f, 0.0f, 0, 0, 0.0f, NULL);
            }
            else
            {
                xAnimTableNewState(atbl, astnames[j], 0, 0, 1.0f, NULL, NULL, 0.0f, NULL);
            }

            xAnimTableAddFile(atbl, afile, astnames[j]);
            xAnimTableNewState(atbl, astnames[j + 10], 0x10, 0, 1.0f, NULL, NULL, 0.0f, NULL);
            xAnimTableAddFile(atbl, afile, astnames[j + 10]);
            anused[i]++;
        }
    }
}

// Forget the loaded lists.
void zAnimListExit()
{
    nals = 0;
    aids = NULL;
    atbls = NULL;
    anused = NULL;
}

// The animation table built from the animation list asset with this id.
xAnimTable* zAnimListGetTable(U32 id)
{
    for (S32 i = 0; i < nals; i++)
    {
        if (id == aids[i])
        {
            return atbls[i];
        }
    }
    return NULL;
}

// The number of states the animation list asset with this id fills in.
S32 zAnimListGetNumUsed(U32 id)
{
    for (S32 i = 0; i < nals; i++)
    {
        if (id == aids[i])
        {
            return anused[i];
        }
    }
    return 0;
}
