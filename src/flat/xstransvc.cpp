#include "xstransvc.h"
#include "xpkrsvc.h"
#include "xutil.h"

#include <stdio.h>
#include <string.h>

void iSystemPollResetAndDisc();

struct st_STRAN_SCENE
{
    U32 scnid;
    S32 lockid;
    st_PACKER_READ_DATA* spkg;
    void* userdata;
    char fnam[256];
};

struct st_STRAN_DATA
{
    st_STRAN_SCENE hipscn[8];
    U32 loadlock;
};

// The eight scene slots; bit N of loadlock marks slot N in use.
static st_STRAN_DATA g_xstdata = {};
static S32 g_straninit;
static st_PACKER_READ_FUNCS* g_pkrf;
static st_PACKER_ASSETTYPE* g_typeHandlers;

// Clears every slot and the lock mask.
static inline void XST_reset_raw()
{
    memset(&g_xstdata, 0, sizeof(st_STRAN_DATA));
}

// Claims the first free slot, clears it and records its lock bit; NULL when all eight are taken.
static inline st_STRAN_SCENE* XST_lock_next()
{
    st_STRAN_SCENE* sdata = NULL;
    S32 uselock = -1;
    for (S32 i = 0; i < 8; i++)
    {
        if (!(g_xstdata.loadlock & 1 << i))
        {
            g_xstdata.loadlock |= 1 << i;
            sdata = &g_xstdata.hipscn[i];
            memset(sdata, 0, sizeof(st_STRAN_SCENE));
            uselock = i;
            break;
        }
    }

    if (sdata != NULL)
    {
        sdata->lockid = uselock;
    }

    return sdata;
}

// Releases a slot that is still locked and clears it.
static inline void XST_unlock(st_STRAN_SCENE* sdata)
{
    if (sdata != NULL)
    {
        if (g_xstdata.loadlock & 1 << sdata->lockid)
        {
            g_xstdata.loadlock &= ~(U32)(1 << sdata->lockid);
            memset(sdata, 0, sizeof(st_STRAN_SCENE));
        }
    }
}

static inline st_STRAN_SCENE* XST_get_rawinst(S32 index)
{
    return &g_xstdata.hipscn[index];
}

// Releases every locked slot.
static inline void XST_unlock_all()
{
    if (g_xstdata.loadlock)
    {
        for (S32 i = 0; i < 8; i++)
        {
            if (g_xstdata.loadlock & 1 << i)
            {
                XST_unlock(XST_get_rawinst(i));
            }
        }
    }
}

// Number of slots in use.
static inline S32 XST_cnt_locked()
{
    S32 sum = 0;
    for (S32 i = 0; i < 8; i++)
    {
        if (g_xstdata.loadlock & 1 << i)
        {
            sum++;
        }
    }
    return sum;
}

// The index-th slot in use, counting in slot order.
static inline st_STRAN_SCENE* XST_nth_locked(S32 index)
{
    st_STRAN_SCENE* sdata = NULL;
    S32 cnt = 0;
    for (S32 i = 0; i < 8; i++)
    {
        if (g_xstdata.loadlock & 1 << i)
        {
            if (cnt == index)
            {
                sdata = &g_xstdata.hipscn[i];
                break;
            }
            cnt++;
        }
    }

    return sdata;
}

// The slot holding scene sid, or NULL.
static inline st_STRAN_SCENE* XST_find_bySID(U32 sid)
{
    st_STRAN_SCENE* da_sdata = NULL;

    for (S32 i = 0; i < 8; i++)
    {
        if (g_xstdata.loadlock & 1 << i)
        {
            st_STRAN_SCENE* sc = &g_xstdata.hipscn[i];
            if (sc->scnid == sid)
            {
                da_sdata = sc;
                break;
            }
        }
    }
    return da_sdata;
}

// Opens the scene's package file; returns its client version, or 0 when it does not open.
static inline S32 XST_PreLoadScene(st_STRAN_SCENE* sdata, const char* name)
{
    S32 cltver = 0;
    sdata->spkg = g_pkrf->Init(sdata->userdata, (char*)name, 0x2e, &cltver, g_typeHandlers);
    if (sdata->spkg != NULL)
    {
        return cltver;
    }
    return 0;
}

static char* XST_translate_sid(U32 sid);
static char* XST_translate_sid_path(U32 sid);

// Reference-counted startup: the first call stores the asset type handlers, clears the slots and starts the packer.
S32 xSTStartup(st_PACKER_ASSETTYPE* handlers)
{
    if (g_straninit++ == 0)
    {
        g_typeHandlers = handlers;
        XST_reset_raw();
        PKRStartup();
        g_pkrf = PKRGetReadFuncs(1);
    }
    return g_straninit;
}

// Reference-counted shutdown: the last call releases every scene and stops the packer.
S32 xSTShutdown()
{
    g_straninit--;
    if (g_straninit == 0)
    {
        g_typeHandlers = NULL;
        XST_unlock_all();
        PKRShutdown();
    }
    return g_straninit;
}

// Takes a slot for scene sid and opens its .HIP, first as XX/XXXX.HIP in the scene's folder (except
// BOOT and FONT), then as XXXX.HIP at the root; retries until a file opens. Returns the client version.
S32 xSTPreLoadScene(U32 sid, void* userdata)
{
    S32 result;
    S32 cltver = 0;
    S32 cnt_retry = 0;
    st_STRAN_SCENE* sdata;

    do
    {
        cnt_retry++;
        printf("PreLoading 0x%08X (attempt #%d)\n", sid, cnt_retry);

        sdata = XST_lock_next();
        sdata->scnid = sid;
        sdata->userdata = userdata;

        if (sid != 'BOOT' && sid != 'FONT')
        {
            char* const path = XST_translate_sid_path(sid);
            if (path != NULL)
            {
                strcpy(sdata->fnam, path);
                cltver = XST_PreLoadScene(sdata, path);
            }
        }
        if (cltver == 0)
        {
            char* const path = XST_translate_sid(sid);
            if (path != NULL)
            {
                strcpy(sdata->fnam, path);
                cltver = XST_PreLoadScene(sdata, path);
            }
        }
        if (cltver == 0)
        {
            XST_unlock(sdata);
            result = 0;
        }
        else
        {
            result = cltver;
        }
    } while (cltver == 0);

    return result;
}

// Queues every layer of a loaded scene for reading; 0 when the scene is not loaded.
S32 xSTQueueSceneAssets(U32 sid)
{
    S32 result = 1;
    st_STRAN_SCENE* sdata = XST_find_bySID(sid);
    if (sdata == NULL)
    {
        result = 0;
    }
    else if (sdata->spkg != NULL)
    {
        g_pkrf->LoadLayer(sdata->spkg, PKR_LTYPE_ALL);
    }
    return result;
}

// Closes scene sid's package and frees its slot; sid 0 closes every loaded scene.
void xSTUnLoadScene(U32 sid)
{
    st_STRAN_SCENE* sdata;
    if (sid == 0)
    {
        S32 cnt = XST_cnt_locked();
        for (S32 i = 0; i < cnt; i++)
        {
            sdata = XST_nth_locked(i);
            if (sdata->spkg != NULL)
            {
                g_pkrf->Done(sdata->spkg);
            }
            sdata->spkg = NULL;
        }
        XST_unlock_all();
    }
    else
    {
        sdata = XST_find_bySID(sid);
        if (sdata != NULL)
        {
            if (sdata->spkg != NULL)
            {
                g_pkrf->Done(sdata->spkg);
            }
            sdata->spkg = NULL;
        }
        XST_unlock(sdata);
    }
}

// Advances background loading by one step and polls the reset button and disc; returns 1.00001
// once nothing is left to read, else 0.
F32 xSTLoadStep(U32)
{
    F32 pct = PKRLoadStep(0) != 0 ? 0.0f : 1.00001f;
    iSystemPollResetAndDisc();
    return pct;
}

// Makes scene sid's assets the active set, reporting 0 and 1 to the progress callback.
S32 xSTSwitchScene(U32 sid, void* userdata, S32 (*progmon)(void*, F32))
{
    S32 result = 1;
    st_STRAN_SCENE* sdata = NULL;

    if (sid != 0)
    {
        sdata = XST_find_bySID(sid);
    }
    if (sdata == NULL)
    {
        result = 0;
    }
    if (result != 0)
    {
        if (progmon != NULL)
        {
            progmon(userdata, 0.0f);
        }
        if (g_pkrf->SetActive(sdata->spkg, PKR_LTYPE_ALL) == 0)
        {
            result = 0;
        }
        if (progmon != NULL)
        {
            progmon(userdata, 1.0f);
        }
    }
    return result;
}

// The name of asset aid from the first loaded scene that has it.
char* xSTAssetName(U32 aid)
{
    char* aname = NULL;
    S32 cnt = XST_cnt_locked();
    for (S32 i = 0; i < cnt; i++)
    {
        st_STRAN_SCENE* sdata = XST_nth_locked(i);
        if (sdata->spkg != NULL)
        {
            aname = g_pkrf->AssetName(sdata->spkg, aid);
            if (aname != NULL)
            {
                break;
            }
        }
    }
    return aname;
}

// Asset aid's data and size from the first loaded scene that has it; NULL while it is still loading.
void* xSTFindAsset(U32 aid, U32* size)
{
    void* memloc = NULL;
    S32 cnt = XST_cnt_locked();
    for (S32 i = 0; i < cnt; i++)
    {
        st_STRAN_SCENE* sdata = XST_nth_locked(i);
        if (g_pkrf->PkgHasAsset(sdata->spkg, aid))
        {
            memloc = g_pkrf->LoadAsset(sdata->spkg, aid, NULL, NULL);
            if (!g_pkrf->IsAssetReady(sdata->spkg, aid))
            {
                memloc = NULL;
                if (size != NULL)
                {
                    *size = 0;
                }
            }
            else if (size != NULL)
            {
                *size = g_pkrf->GetAssetSize(sdata->spkg, aid);
            }
            break;
        }
    }
    return memloc;
}

// Number of assets of a type across all loaded scenes.
S32 xSTAssetCountByType(U32 type)
{
    S32 sum = 0;
    S32 cnt = XST_cnt_locked();
    for (S32 i = 0; i < cnt; i++)
    {
        st_STRAN_SCENE* sdata = XST_nth_locked(i);
        sum += g_pkrf->AssetCount(sdata->spkg, type);
    }
    return sum;
}

// The idx-th asset of a type, counting across the loaded scenes in slot order.
void* xSTFindAssetByType(U32 type, S32 idx, U32* size)
{
    void* memptr = NULL;
    S32 i;
    S32 sum = 0;
    S32 cnt = XST_cnt_locked();
    for (i = 0; i < cnt; i++)
    {
        st_STRAN_SCENE* sdata = XST_nth_locked(i);
        S32 scncnt = g_pkrf->AssetCount(sdata->spkg, type);
        if (idx >= sum && idx < sum + scncnt)
        {
            memptr = g_pkrf->AssetByType(sdata->spkg, type, idx - sum, size);
            break;
        }
        sum += scncnt;
    }
    return memptr;
}

// Copies asset aid's table-of-contents entry (disc sector, offset, size, memory) into ainfo.
S32 xSTGetAssetInfo(U32 aid, st_xST_XASSETINFO* ainfo)
{
    S32 found = 0;
    st_PKR_ASSET_TOCINFO tocinfo = { 0, 0, 0, 0, NULL };
    memset(ainfo, 0, sizeof(st_xST_XASSETINFO));

    S32 cnt = XST_cnt_locked();
    for (S32 i = 0; i < cnt; i++)
    {
        st_STRAN_SCENE* sdata = XST_nth_locked(i);
        if (g_pkrf->PkgHasAsset(sdata->spkg, aid))
        {
            g_pkrf->GetBaseSector(sdata->spkg);
            if (g_pkrf->GetAssetInfo(sdata->spkg, aid, &tocinfo))
            {
                found = 1;
                ainfo->aid = tocinfo.aid;
                ainfo->sector = tocinfo.sector;
                ainfo->plus_offset = tocinfo.plus_offset;
                ainfo->size = tocinfo.size;
                ainfo->mempos = tocinfo.mempos;
                break;
            }
        }
    }
    return found;
}

// As xSTGetAssetInfo for the idx-th asset of a type, counting across the loaded scenes.
S32 xSTGetAssetInfoByType(U32 type, S32 idx, st_xST_XASSETINFO* ainfo)
{
    S32 found = 0;
    st_PKR_ASSET_TOCINFO tocinfo = { 0, 0, 0, 0, NULL };
    S32 rc;
    S32 i;
    S32 sum = 0;
    S32 cnt;

    memset(ainfo, 0, sizeof(st_xST_XASSETINFO));
    cnt = XST_cnt_locked();
    for (i = 0; i < cnt; i++)
    {
        st_STRAN_SCENE* sdata = XST_nth_locked(i);
        S32 scncnt = g_pkrf->AssetCount(sdata->spkg, type);
        if (idx >= sum && idx < sum + scncnt)
        {
            g_pkrf->GetBaseSector(sdata->spkg);
            rc = g_pkrf->GetAssetInfoByType(sdata->spkg, type, idx - sum, &tocinfo);
            if (rc)
            {
                found = 1;
                ainfo->aid = tocinfo.aid;
                ainfo->sector = tocinfo.sector;
                ainfo->plus_offset = tocinfo.plus_offset;
                ainfo->size = tocinfo.size;
                ainfo->mempos = tocinfo.mempos;
                break;
            }
        }
        sum += scncnt;
    }
    return found;
}

// The .HIP path of the first loaded scene holding asset aid, and that scene's id.
static inline char* xST_xAssetID_HIPFullPath(U32 aid, U32* sceneID)
{
    char* da_hipname = NULL;
    S32 cnt = XST_cnt_locked();
    for (S32 i = 0; i < cnt; i++)
    {
        st_STRAN_SCENE* sdata = XST_nth_locked(i);
        if (g_pkrf->PkgHasAsset(sdata->spkg, aid))
        {
            da_hipname = sdata->fnam;
            if (sceneID != NULL)
            {
                *sceneID = sdata->scnid;
            }
            break;
        }
    }
    return da_hipname;
}

// The .HIP path of the first loaded scene holding asset aid.
char* xST_xAssetID_HIPFullPath(U32 aid)
{
    return xST_xAssetID_HIPFullPath(aid, NULL);
}

// "XXXX.HIP" for scene id XXXX.
static char* XST_translate_sid(U32 sid)
{
    static char fname[0x40] = {};
    sprintf(fname, "%s%s", xUtil_idtag2string(sid, 0), ".HIP");
    return fname;
}

// "XX/XXXX.HIP": the scene's file in the folder named by its first two letters.
static char* XST_translate_sid_path(U32 sid)
{
    static char fname[0x40] = {};
    char pathSeparator[2] = "/";
    sprintf(fname, "%c%c%s%s%s", *xUtil_idtag2string(sid, 0), *(xUtil_idtag2string(sid, 0) + 1),
            pathSeparator, xUtil_idtag2string(sid, 0), ".HIP");
    return fname;
}
