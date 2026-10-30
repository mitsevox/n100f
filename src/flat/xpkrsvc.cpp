#include "xpkrsvc.h"
#include "xhipio.h"

#include <string.h>

struct RwResEntry;

static st_PACKER_READ_DATA* PKR_ReadInit(void* userdata, char* pkgfile, U32 opts, S32* cltver,
                                         st_PACKER_ASSETTYPE* typelist);
static void PKR_ReadDone(st_PACKER_READ_DATA* pr);
static S32 PKR_SetActive(st_PACKER_READ_DATA* pr, en_LAYER_TYPE layer);
static S32 PKR_LoadLayer(st_PACKER_READ_DATA* pr, en_LAYER_TYPE layer);
static void* PKR_LoadAsset(st_PACKER_READ_DATA* pr, U32 aid, const char*, void*);
static U32 PKR_GetAssetSize(st_PACKER_READ_DATA* pr, U32 aid);
static S32 PKR_AssetCount(st_PACKER_READ_DATA* pr, U32 type);
static void* PKR_AssetByType(st_PACKER_READ_DATA* pr, U32 type, S32 idx, U32* size);
static S32 PKR_IsAssetReady(st_PACKER_READ_DATA* pr, U32 aid);
static U32 PKR_getPackTimestamp(st_PACKER_READ_DATA* pr);
static char* PKR_AssetName(st_PACKER_READ_DATA* pr, U32 aid);
static U32 PKR_GetBaseSector(st_PACKER_READ_DATA* pr);
static S32 PKR_GetAssetInfo(st_PACKER_READ_DATA* pr, U32 aid, st_PKR_ASSET_TOCINFO* tocainfo);
static S32 PKR_GetAssetInfoByType(st_PACKER_READ_DATA* pr, U32 type, S32 idx,
                                  st_PKR_ASSET_TOCINFO* tocainfo);
static S32 PKR_PkgHasAsset(st_PACKER_READ_DATA* pr, U32 aid);
static S32 OrdTest_R_AssetID(const void* vkey, void* vitem);
static inline S32 PKR_typeHdlr_idx(st_PACKER_READ_DATA* pr, U32 type);

static st_PACKER_READ_FUNCS g_pkr_read_funcmap = { 1,
                                                   PKR_ReadInit,
                                                   PKR_ReadDone,
                                                   PKR_LoadLayer,
                                                   PKR_GetAssetSize,
                                                   PKR_LoadAsset,
                                                   PKR_AssetByType,
                                                   PKR_AssetCount,
                                                   PKR_IsAssetReady,
                                                   PKR_SetActive,
                                                   PKR_AssetName,
                                                   PKR_GetBaseSector,
                                                   PKR_GetAssetInfo,
                                                   PKR_GetAssetInfoByType,
                                                   PKR_PkgHasAsset,
                                                   PKR_getPackTimestamp };
static st_PACKER_READ_DATA g_readdatainst[8] = {};

static st_HIPLOADFUNCS* g_hiprf;
static S32 pkr_sector_size;
static S32 g_packinit;

static S32 PKR_LoadStep_Async();

st_PACKER_READ_FUNCS* PKRGetReadFuncs(S32 apiver)
{
    switch (apiver)
    {
    case 1:
        return &g_pkr_read_funcmap;
    default:
        return NULL;
    }
}

S32 PKRStartup()
{
    if (g_packinit++ == 0)
    {
        g_hiprf = get_HIPLFuncs();
        pkr_sector_size = 32;
    }
    return g_packinit;
}

S32 PKRShutdown()
{
    return --g_packinit;
}

S32 PKRLoadStep(S32)
{
    return PKR_LoadStep_Async();
}

// Layers are not loaded one at a time on this platform.
static S32 PKR_LoadLayer(st_PACKER_READ_DATA* pr, en_LAYER_TYPE layer)
{
    return 0;
}

static void* PKR_LoadAsset(st_PACKER_READ_DATA* pr, U32 aid, const char*, void*)
{
    st_PACKER_ATOC_NODE* assnode = NULL;
    S32 idx = XOrdLookup(&pr->asstoc, (void*)aid, OrdTest_R_AssetID);

    if (!(idx < 0))
    {
        assnode = (st_PACKER_ATOC_NODE*)pr->asstoc.list[idx];
    }

    return assnode != NULL ? assnode->memloc : NULL;
}

static U32 PKR_getPackTimestamp(st_PACKER_READ_DATA* pr)
{
    return pr->time_made;
}

static char* PKR_AssetName(st_PACKER_READ_DATA* pr, U32 aid)
{
    char* name = NULL;
    S32 idx = XOrdLookup(&pr->asstoc, (void*)aid, OrdTest_R_AssetID);

    if (idx >= 0)
    {
        name = ((st_PACKER_ATOC_NODE*)pr->asstoc.list[idx])->name;
    }

    return name;
}

static U32 PKR_GetBaseSector(st_PACKER_READ_DATA* pr)
{
    return pr->base_sector;
}

static S32 PKR_GetAssetInfo(st_PACKER_READ_DATA* pr, U32 aid, st_PKR_ASSET_TOCINFO* tocainfo)
{
    memset(tocainfo, 0, sizeof(st_PKR_ASSET_TOCINFO));
    S32 idx = XOrdLookup(&pr->asstoc, (void*)aid, OrdTest_R_AssetID);
    if (idx >= 0)
    {
        st_PACKER_ATOC_NODE* assnode = (st_PACKER_ATOC_NODE*)pr->asstoc.list[idx];
        tocainfo->aid = aid;
        tocainfo->sector = pr->base_sector + assnode->d_off / pkr_sector_size;
        tocainfo->plus_offset = assnode->d_off % pkr_sector_size;
        tocainfo->size = assnode->d_size;
        tocainfo->mempos = assnode->memloc;
    }
    return idx >= 0 ? 1 : 0;
}

static S32 PKR_GetAssetInfoByType(st_PACKER_READ_DATA* pr, U32 type, S32 idx,
                                  st_PKR_ASSET_TOCINFO* tocainfo)
{
    memset(tocainfo, 0, sizeof(st_PKR_ASSET_TOCINFO));
    if (idx < 0)
    {
        idx = 0;
    }

    S32 typeidx = PKR_typeHdlr_idx(pr, type);
    if (typeidx < 0)
    {
        return 0;
    }

    st_XORDEREDARRAY* tmplist = &pr->typelist[typeidx];
    if (idx >= tmplist->cnt)
    {
        return 0;
    }

    st_PACKER_ATOC_NODE* assnode = (st_PACKER_ATOC_NODE*)tmplist->list[idx];
    tocainfo->aid = assnode->aid;
    tocainfo->sector = pr->base_sector + assnode->d_off / pkr_sector_size;
    tocainfo->plus_offset = assnode->d_off % pkr_sector_size;
    tocainfo->size = assnode->d_size;
    tocainfo->mempos = assnode->memloc;

    return 1;
}

static U32 PKR_GetAssetSize(st_PACKER_READ_DATA* pr, U32 aid)
{
    st_PACKER_ATOC_NODE* assnode = NULL;
    S32 idx = XOrdLookup(&pr->asstoc, (void*)aid, OrdTest_R_AssetID);

    if (idx > -1)
    {
        assnode = (st_PACKER_ATOC_NODE*)pr->asstoc.list[idx];
    }

    if (assnode != NULL)
    {
        if (assnode->x_size > 0)
        {
            return assnode->x_size;
        }
        return assnode->d_size;
    }
    return 0;
}

static S32 PKR_AssetCount(st_PACKER_READ_DATA* pr, U32 type)
{
    S32 count = 0;
    if (type == 0)
    {
        return pr->asstoc.cnt;
    }
    else
    {
        S32 idx = PKR_typeHdlr_idx(pr, type);
        if (idx >= 0)
        {
            st_XORDEREDARRAY* tmplist = &pr->typelist[idx];
            count = tmplist->cnt;
        }
    }
    return count;
}

static void* PKR_AssetByType(st_PACKER_READ_DATA* pr, U32 type, S32 idx, U32* size)
{
    if (size != NULL)
    {
        *size = 0;
    }

    if (idx < 0)
    {
        idx = 0;
    }

    S32 typeidx = PKR_typeHdlr_idx(pr, type);
    if (typeidx < 0)
    {
        return 0;
    }

    st_XORDEREDARRAY* tmplist = &pr->typelist[typeidx];
    if (idx >= tmplist->cnt)
    {
        return 0;
    }

    st_PACKER_ATOC_NODE* assnode = (st_PACKER_ATOC_NODE*)tmplist->list[idx];
    if (size != NULL)
    {
        *size = assnode->d_size;
    }
    return assnode->memloc;
}

static S32 PKR_IsAssetReady(st_PACKER_READ_DATA* pr, U32 aid)
{
    S32 ready = false;
    S32 idx = XOrdLookup(&pr->asstoc, (void*)aid, OrdTest_R_AssetID);
    if (idx >= 0)
    {
        st_PACKER_ATOC_NODE* assnode = (st_PACKER_ATOC_NODE*)pr->asstoc.list[idx];
        if (assnode->loadflag & 0x80000)
        {
            ready = true;
        }
        else
        {
            ready = false;
        }
    }

    return ready;
}

static S32 PKR_PkgHasAsset(st_PACKER_READ_DATA* pr, U32 aid)
{
    S32 idx = XOrdLookup(&pr->asstoc, (void*)aid, OrdTest_R_AssetID);
    if (idx < 0)
    {
        return 0;
    }

    S32 rc = 1;
    st_PACKER_ATOC_NODE* assnode = (st_PACKER_ATOC_NODE*)pr->asstoc.list[idx];
    if (assnode->loadflag & 0x100000)
    {
        rc = 0;
    }
    else if (assnode->loadflag & 0x200000)
    {
        rc = 0;
    }
    return rc;
}

static S32 OrdTest_R_AssetID(const void* vkey, void* vitem)
{
    S32 rc;
    if ((U32)vkey < *(U32*)vitem)
    {
        rc = -1;
    }
    else if ((U32)vkey > *(U32*)vitem)
    {
        rc = 1;
    }
    else
    {
        rc = 0;
    }
    return rc;
}

static S32 OrdTest_R_Asset(const void* vkey, void* vitem)
{
    S32 rc;
    if (*(U32*)vkey < *(U32*)vitem)
    {
        rc = -1;
    }
    else if (*(U32*)vkey > *(U32*)vitem)
    {
        rc = 1;
    }
    else
    {
        rc = 0;
    }
    return rc;
}

static void PKR_special_loadbuf_killed(RwResEntry*)
{
}

static inline S32 PKR_typeHdlr_idx(st_PACKER_READ_DATA* pr, U32 type)
{
    S32 idx = -1;
    int i = 0;
    st_PACKER_ASSETTYPE* tmptype = pr->types;
    while (tmptype->typetag != 0)
    {
        if (tmptype->typetag == type)
        {
            idx = i;
            break;
        }
        i++;
        tmptype++;
    }

    return idx;
}
