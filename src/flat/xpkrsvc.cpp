#include "xpkrsvc.h"
#include "xhipio.h"
#include "xMemMgr.h"

#include <rwcore.h>
#include <stdlib.h>
#include <string.h>

// Where a layer's data is read to: nowhere (the layer is skipped), a block of its own that stays
// loaded, or the RenderWare resource arena, which is handed back once the assets are converted.
enum en_PKR_LAYER_LOAD_DEST
{
    PKR_LDDEST_SKIP,
    PKR_LDDEST_KEEPSTATIC,
    PKR_LDDEST_RWHANDOFF
};

extern "C" void fn_800167F0();

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
static S32 OrdTest_R_Asset(const void* vkey, void* vitem);
static inline S32 PKR_typeHdlr_idx(st_PACKER_READ_DATA* pr, U32 type);
static S32 PKR_parse_TOC(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static S32 PKR_findNextLayerToLoad(st_PACKER_READ_DATA** work_on_pkg,
                                   st_PACKER_LTOC_NODE** next_layer);
static void PKR_updateLayerAssets(st_PACKER_LTOC_NODE* laynode);
static inline void PKR_xformLayerAssets(st_PACKER_LTOC_NODE* laynode);
static void PKR_xform_asset(st_PACKER_ATOC_NODE* assnode);
static inline char* PKR_LayerMemReserve(st_PACKER_READ_DATA* pr, st_PACKER_LTOC_NODE* layer);
static inline void PKR_LayerMemRelease(st_PACKER_READ_DATA* pr, st_PACKER_LTOC_NODE* layer);
static inline en_PKR_LAYER_LOAD_DEST PKR_layerLoadDest(en_LAYER_TYPE layer);
static inline S32 PKR_layerTypeNeedsXForm(en_LAYER_TYPE layer);
static inline void* PKR_getmem(S32 amount);
static inline void* PKR_gettempmem(S32 amount, char** memtrue);
static inline char* PKR_getRWarena(st_PACKER_READ_DATA* pr, S32 amount);
static inline void PKR_relRWarena();
static S32 PKR_FRIEND_assetIsGameDup(U32 aid, const st_PACKER_READ_DATA* skippr, S32 oursize,
                                     U32 ourtype, U32 chksum, char*);
static inline void PKR_makepool_anode(st_PACKER_READ_DATA* pr, S32 cnt);
static inline void PKR_kiilpool_anode(st_PACKER_READ_DATA* pr);
static inline st_PACKER_ATOC_NODE* PKR_newassnode(st_PACKER_READ_DATA* pr, U32 aid);
static inline st_PACKER_LTOC_NODE* PKR_newlaynode(en_LAYER_TYPE layer, S32 refcnt);
static inline void PKR_oldlaynode(st_PACKER_LTOC_NODE* laynode);
static inline S32 LOD_r_HIPA(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_PACK(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_PVER(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_PFLG(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_PCNT(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_PCRT(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_PMOD(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_DICT(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_ATOC(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_AINF(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static S32 LOD_r_AHDR(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_ADBG(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr, st_PACKER_ATOC_NODE* assnode);
static inline S32 LOD_r_LTOC(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_LINF(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static S32 LOD_r_LHDR(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_LDBG(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr, st_PACKER_LTOC_NODE* laynode);
static inline S32 LOD_r_STRM(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_DHDR(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline S32 LOD_r_DPAK(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr);
static inline st_PACKER_ASSETTYPE* PKR_type2typeref(U32 asstype, st_PACKER_ASSETTYPE* types);
static void PKR_bld_typecnt(st_PACKER_READ_DATA* pr);
static void PKR_special_loadbuf_killed(RwResEntry*);

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
static U32 g_loadlock;
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

// Opens a package: takes a free reader slot (one bit of g_loadlock each), opens the file
// through the HIP reader with a temporary 32 KB table-of-contents buffer, and parses the
// table of contents. Returns NULL with *cltver = -1 when no slot is free or the file won't open.
static st_PACKER_READ_DATA* PKR_ReadInit(void* userdata, char* pkgfile, U32 opts, S32* cltver,
                                         st_PACKER_ASSETTYPE* typelist)
{
    st_PACKER_READ_DATA* pr = NULL;
    S32 uselock = -1;

    char* tocbuf_RAW;
    char* tocbuf_aligned = (char*)PKR_gettempmem(0x8000, &tocbuf_RAW);

    for (S32 i = 0; i < 8; i++)
    {
        if (!(g_loadlock & 1 << i))
        {
            g_loadlock |= 1 << i;
            pr = &g_readdatainst[i];
            uselock = i;
            break;
        }
    }

    if (pr != NULL)
    {
        memset(pr, 0, sizeof(st_PACKER_READ_DATA));
        pr->lockid = uselock;
        pr->userdata = userdata;
        pr->opts = opts;
        pr->types = typelist;
        pr->cltver = -1;
        strncpy(pr->packfile, pkgfile, 0x80);

        if (tocbuf_aligned == NULL)
        {
            pr->pkg = g_hiprf->create(pkgfile, NULL, 0);
        }
        else
        {
            pr->pkg = g_hiprf->create(pkgfile, tocbuf_aligned, 0x8000);
        }

        if (pr->pkg != NULL)
        {
            pr->base_sector = g_hiprf->basesector(pr->pkg);
            PKR_parse_TOC(pr->pkg, pr);
            *cltver = pr->cltver;
            g_hiprf->setBypass(pr->pkg, 1, 1);
        }
        else
        {
            PKR_ReadDone(pr);
            pr = NULL;
            *cltver = -1;
        }
    }
    else
    {
        pr = NULL;
        *cltver = -1;
    }

    if (tocbuf_RAW != NULL)
    {
        xMemPopTemp(tocbuf_RAW);
    }
    return pr;
}

// Closes a package: tells each asset's type handler the asset is going away (last layer and
// asset first), forgets each layer's memory pointer, frees the table-of-contents lists, closes the file and
// gives the reader slot back.
static void PKR_ReadDone(st_PACKER_READ_DATA* pr)
{
    S32 j;
    S32 i;
    S32 lockid;
    st_PACKER_ATOC_NODE* assnode;
    st_PACKER_LTOC_NODE* laynode;
    st_XORDEREDARRAY* tmplist;

    if (pr == NULL)
    {
        return;
    }

    for (i = pr->laytoc.cnt - 1; i >= 0; i--)
    {
        st_PACKER_LTOC_NODE* laynode = (st_PACKER_LTOC_NODE*)pr->laytoc.list[i];
        for (j = laynode->assref.cnt - 1; j >= 0; j--)
        {
            assnode = (st_PACKER_ATOC_NODE*)laynode->assref.list[j];
            if (assnode->typeref != NULL && assnode->typeref->assetUnloaded != NULL &&
                !(assnode->loadflag & 0x100000))
            {
                assnode->typeref->assetUnloaded(assnode->memloc, assnode->aid);
            }
        }
    }

    for (i = 0; i < pr->laytoc.cnt; i++)
    {
        laynode = (st_PACKER_LTOC_NODE*)pr->laytoc.list[i];
        if (laynode->laymem != NULL)
        {
            laynode->laymem = NULL;
        }
    }

    PKR_kiilpool_anode(pr);

    for (i = 0; i < pr->laytoc.cnt; i++)
    {
        laynode = (st_PACKER_LTOC_NODE*)pr->laytoc.list[i];
        PKR_oldlaynode(laynode);
    }

    XOrdDone(&pr->asstoc);
    XOrdDone(&pr->laytoc);

    for (i = 0; i < 129; i++)
    {
        tmplist = &pr->typelist[i];
        if (tmplist->max != 0)
        {
            XOrdDone(tmplist);
        }
    }

    g_hiprf->destroy(pr->pkg);

    lockid = pr->lockid;
    memset(pr, 0, sizeof(st_PACKER_READ_DATA));
    g_loadlock &= ~(1 << lockid);
}

// Hands each loaded asset of the layer (all layers when layer is PKR_LTYPE_DEFAULT or less)
// to its type handler's assetLoaded, once. Returns 0 if any handler refused.
static S32 PKR_SetActive(st_PACKER_READ_DATA* pr, en_LAYER_TYPE layer)
{
    S32 result;
    S32 rc = 1;
    S32 i;
    S32 j;
    st_PACKER_ATOC_NODE* assnode;
    st_PACKER_LTOC_NODE* laynode;

    if (pr == NULL)
    {
        return 0;
    }

    for (i = 0; i < pr->laytoc.cnt; i++)
    {
        laynode = (st_PACKER_LTOC_NODE*)pr->laytoc.list[i];
        if (layer > PKR_LTYPE_DEFAULT && laynode->laytyp != layer)
        {
            continue;
        }

        for (j = 0; j < laynode->assref.cnt; j++)
        {
            assnode = (st_PACKER_ATOC_NODE*)laynode->assref.list[j];
            if (assnode->loadflag & 0x10000 || !(assnode->loadflag & 0x80000))
            {
                continue;
            }

            if (assnode->typeref != NULL && assnode->typeref->assetLoaded != NULL)
            {
                result = assnode->typeref->assetLoaded(pr->userdata, assnode->aid,
                                                       assnode->memloc, assnode->d_size);
                if (!result)
                {
                    rc = 0;
                }
                else
                {
                    assnode->loadflag |= 0x10000;
                }
            }
        }
    }

    return rc;
}

// Reads the package's table of contents: the HIPA tag, the PACK header (versions, counts,
// times), the DICT asset and layer tables, then builds the per-type lists. Stops after DICT.
static S32 PKR_parse_TOC(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 done = 0;
    S32 is_ok = 0;
    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'HIPA':
            done = LOD_r_HIPA(pkg, pr);
            break;
        case 'PACK':
            LOD_r_PACK(pkg, pr);
            if (done)
            {
                if (pr->asscnt > 0)
                {
                    XOrdInit(&pr->asstoc, pr->asscnt);
                    PKR_makepool_anode(pr, pr->asscnt);
                }
                if (pr->laycnt > 0)
                {
                    XOrdInit(&pr->laytoc, pr->laycnt);
                }
            }
            break;
        case 'DICT':
            LOD_r_DICT(pkg, pr);
            PKR_bld_typecnt(pr);
            is_ok = 1;
            break;
        case 'STRM':
            LOD_r_STRM(pkg, pr);
            break;
        }

        if (!done)
        {
            break;
        }

        g_hiprf->exit(pkg);

        if (is_ok)
        {
            break;
        }

        cid = g_hiprf->enter(pkg);
    }
    return done;
}

static RwResEntry* g_RWarena_resEntry;
static RwResEntry* g_RWarena_resOwner;

// One step of the background loader. With no layer in flight, picks the next unloaded layer,
// gets its memory and starts reading the whole layer in one go; otherwise polls that read and,
// once it is done, points the layer's assets into the buffer and runs their conversions.
// Returns 0 only when there is nothing left to load.
static S32 PKR_LoadStep_Async()
{
    S32 rc;
    static st_PACKER_READ_DATA* curpr = NULL;
    static st_PACKER_LTOC_NODE* asynlay = NULL;

    if (asynlay == NULL)
    {
        PKR_findNextLayerToLoad(&curpr, &asynlay);
        if (asynlay != NULL)
        {
            if (PKR_layerLoadDest(asynlay->laytyp) != PKR_LDDEST_SKIP && asynlay->laysize > 1 &&
                asynlay->assref.cnt > 0)
            {
                if (asynlay->laymem == NULL)
                {
                    asynlay->laymem = PKR_LayerMemReserve(curpr, asynlay);
                }

                st_PACKER_ATOC_NODE* tmpass = (st_PACKER_ATOC_NODE*)asynlay->assref.list[0];
                g_hiprf->setSpot(tmpass->ownpkg, tmpass->d_off);

                if (g_hiprf->readBytes(tmpass->ownpkg, asynlay->laymem, asynlay->laysize))
                {
                    asynlay->flg_ldstat |= 0x1000000;
                }
                else
                {
                    PKR_LayerMemRelease(curpr, asynlay);
                    asynlay->laymem = NULL;
                    asynlay->flg_ldstat &= 0xfcffffff;
                    curpr = NULL;
                    asynlay = NULL;
                }

                rc = 1;
            }
            else
            {
                rc = 1;
                asynlay->flg_ldstat |= 0x2000000;
                asynlay = NULL;
            }
        }
        else
        {
            rc = 0;
            curpr = NULL;
            asynlay = NULL;
        }
    }
    else
    {
        S32 moretodo = g_hiprf->pollRead(curpr->pkg);
        if (moretodo == HIP_RDSTAT_INPROG)
        {
            // Keeps the arena entry at the head of the in-use list while the read runs.
            if (PKR_layerLoadDest(asynlay->laytyp) == PKR_LDDEST_RWHANDOFF)
            {
                RwResourcesUseResEntry(g_RWarena_resEntry);
            }
            rc = 1;
        }
        else if (moretodo == HIP_RDSTAT_SUCCESS)
        {
            PKR_updateLayerAssets(asynlay);

            if (PKR_layerTypeNeedsXForm(asynlay->laytyp))
            {
                PKR_xformLayerAssets(asynlay);
            }

            PKR_LayerMemRelease(curpr, asynlay);

            rc = 1;
            asynlay->flg_ldstat |= 0x2000000;
            asynlay = NULL;
        }
        else
        {
            if (asynlay->laymem != NULL)
            {
                PKR_LayerMemRelease(curpr, asynlay);
            }

            rc = 1;
            asynlay->flg_ldstat &= 0xfcffffff;
            asynlay = NULL;
            curpr = NULL;
        }
    }

    return rc;
}

// The layer's read buffer: the RenderWare arena for layers converted into RenderWare objects,
// otherwise a 64-byte-aligned block from the active heap.
static inline char* PKR_LayerMemReserve(st_PACKER_READ_DATA* pr, st_PACKER_LTOC_NODE* layer)
{
    char* mem;
    if (PKR_layerLoadDest(layer->laytyp) == PKR_LDDEST_RWHANDOFF)
    {
        mem = PKR_getRWarena(pr, layer->laysize);
    }
    else
    {
        mem = (char*)PKR_getmem(layer->laysize);
    }
    return mem;
}

// Gives the arena buffer back once a RenderWare layer's assets are converted. Heap blocks stay.
static inline void PKR_LayerMemRelease(st_PACKER_READ_DATA* pr, st_PACKER_LTOC_NODE* layer)
{
    switch (PKR_layerLoadDest(layer->laytyp))
    {
    case PKR_LDDEST_RWHANDOFF:
        PKR_relRWarena();
        layer->laymem = NULL;
        break;
    }
}

// Takes the layer buffer from the RenderWare resource arena: an entry 0x80 bytes larger than
// the layer, owned by the reader, with the buffer 64-byte aligned after the entry header.
static inline char* PKR_getRWarena(st_PACKER_READ_DATA* pr, S32 amount)
{
    char* memptr;

    fn_800167F0();
    RwResourcesGetArenaSize();

    g_RWarena_resEntry = RwResourcesAllocateResEntry(pr, &g_RWarena_resOwner, amount + 0x80,
                                                     PKR_special_loadbuf_killed);
    if (g_RWarena_resEntry == NULL)
    {
        exit(-1);
        memptr = NULL;
    }
    else
    {
        memptr = (char*)(((U32)(g_RWarena_resEntry + 1) + 0x3F) & ~0x3F);
        memset(memptr, 0, amount);
    }
    return memptr;
}

static inline void PKR_relRWarena()
{
    RwResEntry* entry = g_RWarena_resEntry;
    g_RWarena_resOwner = NULL;
    g_RWarena_resEntry = NULL;
    RwResourcesFreeResEntry(entry);
}

// Sound and cutscene data layers are not read here; texture, BSP and model layers go through
// the RenderWare arena; the rest are kept.
static inline en_PKR_LAYER_LOAD_DEST PKR_layerLoadDest(en_LAYER_TYPE layer)
{
    switch (layer)
    {
    case PKR_LTYPE_SRAM:
    case PKR_LTYPE_CUTSCENE:
        return PKR_LDDEST_SKIP;
    case PKR_LTYPE_TEXTURE:
    case PKR_LTYPE_BSP:
    case PKR_LTYPE_MODEL:
        return PKR_LDDEST_RWHANDOFF;
    case PKR_LTYPE_DEFAULT:
    case PKR_LTYPE_ANIMATION:
    case PKR_LTYPE_VRAM:
    case PKR_LTYPE_SNDTOC:
    case PKR_LTYPE_CUTSCENETOC:
        return PKR_LDDEST_KEEPSTATIC;
    default:
        return PKR_LDDEST_KEEPSTATIC;
    }
}

static inline S32 PKR_layerTypeNeedsXForm(en_LAYER_TYPE layer)
{
    switch (layer)
    {
    case PKR_LTYPE_TEXTURE:
    case PKR_LTYPE_BSP:
    case PKR_LTYPE_MODEL:
        return 1;
    case PKR_LTYPE_DEFAULT:
    case PKR_LTYPE_ANIMATION:
    case PKR_LTYPE_VRAM:
    case PKR_LTYPE_SNDTOC:
    case PKR_LTYPE_CUTSCENETOC:
        return 1;
    case PKR_LTYPE_SRAM:
    case PKR_LTYPE_CUTSCENE:
        return 0;
    default:
        return 0;
    }
}

// Finds the first layer not yet loaded: in the package being worked on first, then in the
// other open packages.
static S32 PKR_findNextLayerToLoad(st_PACKER_READ_DATA** work_on_pkg,
                                   st_PACKER_LTOC_NODE** next_layer)
{
    st_PACKER_READ_DATA* tmppr;
    st_PACKER_LTOC_NODE* tmplay;
    S32 i;
    S32 j;

    *next_layer = NULL;
    if (*work_on_pkg != NULL)
    {
        tmppr = *work_on_pkg;
        for (i = 0; i < tmppr->laytoc.cnt; i++)
        {
            tmplay = (st_PACKER_LTOC_NODE*)tmppr->laytoc.list[i];
            if (!(tmplay->flg_ldstat & 0x2000000))
            {
                *next_layer = tmplay;
                *work_on_pkg = tmppr;
                break;
            }
        }
    }

    if (*next_layer == NULL)
    {
        for (i = 0; i < 8; i++)
        {
            if (!(g_loadlock & 1 << i) || &g_readdatainst[i] == *work_on_pkg)
            {
                continue;
            }

            tmppr = &g_readdatainst[i];
            for (j = 0; j < tmppr->laytoc.cnt; j++)
            {
                tmplay = (st_PACKER_LTOC_NODE*)tmppr->laytoc.list[j];
                if (!(tmplay->flg_ldstat & 0x2000000))
                {
                    *next_layer = tmplay;
                    *work_on_pkg = tmppr;
                    break;
                }
            }

            if (*next_layer != NULL)
            {
                break;
            }
        }
    }

    return *next_layer ? 1 : 0;
}

// After a layer is read: points each of its assets at its place in the layer buffer (offsets
// are relative to the first asset with data) and marks it loaded. Empty assets get no memory.
static void PKR_updateLayerAssets(st_PACKER_LTOC_NODE* laynode)
{
    S32 i;
    st_PACKER_ATOC_NODE* tmpass = NULL;

    for (i = 0; i < laynode->assref.cnt; i++)
    {
        tmpass = (st_PACKER_ATOC_NODE*)laynode->assref.list[i];
        if (tmpass->d_off > 0 && tmpass->d_size > 0)
        {
            break;
        }
        tmpass = NULL;
    }

    if (tmpass != NULL)
    {
        S32 lay_hip_pos = tmpass->d_off;
        for (i = 0; i < laynode->assref.cnt; i++)
        {
            st_PACKER_ATOC_NODE* tmpass = (st_PACKER_ATOC_NODE*)laynode->assref.list[i];
            if (!(tmpass->loadflag & 0x100000))
            {
                if (tmpass->loadflag & 0x200000)
                {
                    tmpass->memloc = NULL;
                }
                else
                {
                    tmpass->memloc = laynode->laymem + (tmpass->d_off - lay_hip_pos);
                    tmpass->loadflag |= 0x80000;
                }
            }
        }
    }
}

// Converts each asset of a freshly read layer. In a RenderWare layer the buffer is given back
// afterwards, so an asset whose conversion kept no data of its own loses its pointer.
static inline void PKR_xformLayerAssets(st_PACKER_LTOC_NODE* laynode)
{
    S32 i;
    S32 will_be_dumped = 0;

    if (PKR_layerLoadDest(laynode->laytyp) == PKR_LDDEST_RWHANDOFF)
    {
        will_be_dumped = 1;
    }

    for (i = 0; i < laynode->assref.cnt; i++)
    {
        st_PACKER_ATOC_NODE* tmpass = (st_PACKER_ATOC_NODE*)laynode->assref.list[i];
        if (!(tmpass->loadflag & 0x100000))
        {
            PKR_xform_asset(tmpass);
            if (will_be_dumped && tmpass->x_size < 1)
            {
                tmpass->memloc = NULL;
            }
        }
    }
}

// Runs the asset's readXForm conversion and points the asset at the result. An asset whose
// conversion produced nothing is marked empty (0x200000).
static void PKR_xform_asset(st_PACKER_ATOC_NODE* assnode)
{
    if (!(assnode->infoflag & 4))
    {
        if (assnode->typeref == NULL)
        {
            return;
        }
        if (assnode->typeref->readXForm != NULL)
        {
        }
        else
        {
            return;
        }
    }

    st_PACKER_ASSETTYPE* atype = assnode->typeref;
    if (atype != NULL && atype->readXForm != NULL)
    {
        if (assnode->d_size == 0)
        {
            assnode->memloc = NULL;
        }

        char* xformloc =
            (char*)atype->readXForm(assnode->ownpr->userdata, assnode->aid, assnode->memloc,
                                    assnode->d_size, (U32*)&assnode->x_size);
        if (assnode->memloc == xformloc && assnode->x_size != 0)
        {
        }
        else if (assnode->d_size == 0 || assnode->x_size == 0)
        {
            assnode->memloc = NULL;
            assnode->loadflag |= 0x200000;
        }
        else
        {
            assnode->memloc = xformloc;
        }
    }
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

// Whether another open package already has this asset loaded (sounds count even before they
// load), in which case this package's copy is skipped. The asset 0x7AB6743A is never a duplicate.
static S32 PKR_FRIEND_assetIsGameDup(U32 aid, const st_PACKER_READ_DATA* skippr, S32 oursize,
                                     U32 ourtype, U32 chksum, char*)
{
    S32 is_dup = 0;
    if (aid == 0x7ab6743a)
    {
        return 0;
    }

    for (S32 i = 0; i < 8; i++)
    {
        if (!(g_loadlock & 1 << i) || &g_readdatainst[i] == skippr)
        {
            continue;
        }

        S32 idx = XOrdLookup(&g_readdatainst[i].asstoc, (void*)aid, OrdTest_R_AssetID);
        if (idx < 0)
        {
            continue;
        }

        st_PACKER_ATOC_NODE* tmp_ass = (st_PACKER_ATOC_NODE*)g_readdatainst[i].asstoc.list[idx];
        if (!(tmp_ass->loadflag & 0x80000) && tmp_ass->asstype != 'SND ' &&
            tmp_ass->asstype != 'SNDS')
        {
            continue;
        }

        is_dup = 1;
        break;
    }

    return is_dup;
}

// A zeroed block from the temporary stack, 0x80 bytes larger than asked so the 64-byte-aligned
// pointer it returns has room; *memtrue receives the block itself, for xMemPopTemp.
static inline void* PKR_gettempmem(S32 amount, char** memtrue)
{
    void* memptr = xMemPushTemp(amount + 0x80);
    if (memptr != NULL)
    {
        memset(memptr, 0, amount + 0x80);
    }
    *memtrue = (char*)memptr;
    return (void*)(((U32)memptr + 0x3F) & ~0x3F);
}

// A 64-byte-aligned, zeroed block from the active heap.
static inline void* PKR_getmem(S32 amount)
{
    void* memptr = NULL;
    if (amount != 0)
    {
        memptr = (void*)(((U32)xMemAlloc(gActiveHeap, amount + 0x3F) + 0x3F) & ~0x3F);
        if (memptr != NULL)
        {
            memset(memptr, 0, amount);
        }
    }
    return memptr;
}

static inline void PKR_makepool_anode(st_PACKER_READ_DATA* pr, S32 cnt)
{
    if (cnt == 0)
    {
        return;
    }

    st_PACKER_ATOC_NODE* assnode =
        (st_PACKER_ATOC_NODE*)PKR_getmem(cnt * sizeof(st_PACKER_ATOC_NODE));
    if (assnode != NULL)
    {
        pr->pool_anode = assnode;
        pr->pool_nextaidx = 0;
    }
}

static inline void PKR_kiilpool_anode(st_PACKER_READ_DATA* pr)
{
    if (pr->asscnt == 0)
    {
        return;
    }

    pr->pool_anode = NULL;
    pr->pool_nextaidx = 0;
}

static inline st_PACKER_ATOC_NODE* PKR_newassnode(st_PACKER_READ_DATA* pr, U32 aid)
{
    S32 idx = pr->pool_nextaidx;
    st_PACKER_ATOC_NODE* newnode = &pr->pool_anode[idx];
    pr->pool_nextaidx++;

    memset(newnode, 0, sizeof(st_PACKER_ATOC_NODE));
    newnode->aid = aid;
    return newnode;
}

static inline st_PACKER_LTOC_NODE* PKR_newlaynode(en_LAYER_TYPE layer, S32 refcnt)
{
    st_PACKER_LTOC_NODE* newnode =
        (st_PACKER_LTOC_NODE*)PKR_getmem(sizeof(st_PACKER_LTOC_NODE));
    memset(newnode, 0, sizeof(st_PACKER_LTOC_NODE));

    newnode->laytyp = layer;
    XOrdInit(&newnode->assref, refcnt <= 1 ? 2 : refcnt);
    return newnode;
}

static inline void PKR_oldlaynode(st_PACKER_LTOC_NODE* laynode)
{
    XOrdDone(&laynode->assref);
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

static inline S32 LOD_r_HIPA(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    pr->pkgver = 'HIPA';
    return 1;
}

static inline S32 LOD_r_PACK(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'PVER':
            LOD_r_PVER(pkg, pr);
            break;
        case 'PFLG':
            LOD_r_PFLG(pkg, pr);
            break;
        case 'PCNT':
            LOD_r_PCNT(pkg, pr);
            break;
        case 'PCRT':
            LOD_r_PCRT(pkg, pr);
            break;
        case 'PMOD':
            LOD_r_PMOD(pkg, pr);
            break;
        }
        g_hiprf->exit(pkg);
        cid = g_hiprf->enter(pkg);
    }
    return 1;
}

static inline S32 LOD_r_PVER(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 ver = 0;
    g_hiprf->readLongs(pkg, &ver, 1);
    pr->subver = ver;

    ver = -1;
    g_hiprf->readLongs(pkg, &ver, 1);
    pr->cltver = ver;

    ver = -1;
    S32 amt = g_hiprf->readLongs(pkg, &ver, 1);
    if (amt != 1)
    {
        pr->compatver = 1;
    }
    else
    {
        pr->compatver = ver;
    }
    return 1;
}

static inline S32 LOD_r_PFLG(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 flg = 0;
    g_hiprf->readLongs(pkg, &flg, 1);
    return 1;
}

static inline S32 LOD_r_PCNT(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 cnt = 0;
    g_hiprf->readLongs(pkg, &cnt, 1);
    pr->asscnt = cnt;
    g_hiprf->readLongs(pkg, &cnt, 1);
    pr->laycnt = cnt;
    g_hiprf->readLongs(pkg, &cnt, 1);
    g_hiprf->readLongs(pkg, &cnt, 1);
    g_hiprf->readLongs(pkg, &cnt, 1);
    return 1;
}

static inline S32 LOD_r_PCRT(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 time = 0;
    char arr[256] = {};

    g_hiprf->readLongs(pkg, &time, 1);
    pr->time_made = time;

    if (pr->subver > 1)
    {
        g_hiprf->readString(pkg, arr);
    }
    return 1;
}

static inline S32 LOD_r_PMOD(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 time_mod = 0;
    g_hiprf->readLongs(pkg, &time_mod, 1);
    pr->time_mod = time_mod;
    return 1;
}

static inline S32 LOD_r_DICT(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'ATOC':
            LOD_r_ATOC(pkg, pr);
            XOrdSort(&pr->asstoc, OrdTest_R_Asset);
            break;
        case 'LTOC':
            LOD_r_LTOC(pkg, pr);
            break;
        }

        g_hiprf->exit(pkg);
        cid = g_hiprf->enter(pkg);
    }
    return 1;
}

static inline S32 LOD_r_ATOC(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'AINF':
            LOD_r_AINF(pkg, pr);
            break;
        case 'AHDR':
            LOD_r_AHDR(pkg, pr);
            break;
        }
        g_hiprf->exit(pkg);
        cid = g_hiprf->enter(pkg);
    }
    return 1;
}

static inline S32 LOD_r_AINF(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 ival = 0;
    g_hiprf->readLongs(pkg, &ival, 1);
    return 1;
}

// Reads one asset's table-of-contents entry (id, type, offset, size, padding, info flags and
// the optional ADBG block) into the next node of the pool, and marks it a duplicate (0x100000)
// when another open package already holds it.
static S32 LOD_r_AHDR(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 ival = 0;

    g_hiprf->readLongs(pkg, &ival, 1);
    st_PACKER_ATOC_NODE* assnode = PKR_newassnode(pr, ival);
    assnode->ownpr = pr;
    assnode->ownpkg = pkg;
    XOrdAppend(&pr->asstoc, assnode);

    g_hiprf->readLongs(pkg, &ival, 1);
    assnode->asstype = ival;
    assnode->typeref = PKR_type2typeref(assnode->asstype, pr->types);

    g_hiprf->readLongs(pkg, &ival, 1);
    assnode->d_off = ival;

    g_hiprf->readLongs(pkg, &ival, 1);
    assnode->d_size = ival;
    assnode->readrem = ival;
    assnode->readcnt = 0;
    if (assnode->d_size < 1)
    {
        assnode->loadflag |= 0x200000;
    }

    g_hiprf->readLongs(pkg, &ival, 1);
    assnode->d_pad = ival;

    g_hiprf->readLongs(pkg, &ival, 1);
    assnode->infoflag = ival;

    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'ADBG':
            LOD_r_ADBG(pkg, pr, assnode);
            break;
        }
        g_hiprf->exit(pkg);
        cid = g_hiprf->enter(pkg);
    }

    S32 isdup = PKR_FRIEND_assetIsGameDup(assnode->aid, pr, assnode->d_size, assnode->asstype,
                                          assnode->d_chksum, NULL);
    if (isdup)
    {
        assnode->loadflag |= 0x100000;
    }
    return 1;
}

// The asset's debug block: alignment, name (kept, cut to 31 characters), source file name, and
// a checksum in packages newer than subversion 1.
static inline S32 LOD_r_ADBG(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr, st_PACKER_ATOC_NODE* assnode)
{
    S32 ival = 0;
    char tmpbuf[256] = {};

    g_hiprf->readLongs(pkg, &ival, 1);
    assnode->assalign = ival;

    g_hiprf->readString(pkg, tmpbuf);
    strncpy(assnode->name, tmpbuf, sizeof(assnode->name) - 1);
    assnode->name[sizeof(assnode->name) - 1] = 0;

    tmpbuf[0] = 0;
    g_hiprf->readString(pkg, tmpbuf);

    if (pr->subver > 1)
    {
        g_hiprf->readLongs(pkg, &ival, 1);
        assnode->d_chksum = ival;
    }

    return 1;
}

static inline S32 LOD_r_LTOC(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'LINF':
            LOD_r_LINF(pkg, pr);
            break;
        case 'LHDR':
            LOD_r_LHDR(pkg, pr);
            break;
        }
        g_hiprf->exit(pkg);
        cid = g_hiprf->enter(pkg);
    }
    return 1;
}

static inline S32 LOD_r_LINF(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 ival = 0;
    g_hiprf->readLongs(pkg, &ival, 1);
    return 1;
}

// Reads one layer's entry: its type and the ids of its assets, which it looks up in the asset
// table. The layer's size is the sum of its assets with padding (none after the last one),
// rounded up to the sector size.
static S32 LOD_r_LHDR(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 i;
    S32 ival = 0;
    S32 refcnt = 0;

    g_hiprf->readLongs(pkg, &ival, 1);
    en_LAYER_TYPE laytyp = (en_LAYER_TYPE)ival;
    g_hiprf->readLongs(pkg, &refcnt, 1);
    st_PACKER_LTOC_NODE* laynode = PKR_newlaynode(laytyp, refcnt);
    XOrdAppend(&pr->laytoc, laynode);

    for (i = 0; i < refcnt; i++)
    {
        g_hiprf->readLongs(pkg, &ival, 1);
        S32 idx = XOrdLookup(&pr->asstoc, (void*)ival, OrdTest_R_AssetID);
        st_PACKER_ATOC_NODE* assnode = (st_PACKER_ATOC_NODE*)pr->asstoc.list[idx];
        XOrdAppend(&laynode->assref, assnode);

        if (i != refcnt - 1)
        {
            laynode->laysize += assnode->d_size + assnode->d_pad;
        }
        else
        {
            laynode->laysize += assnode->d_size;
        }
    }

    if (laynode->laysize > 0)
    {
        laynode->laysize = (laynode->laysize + pkr_sector_size - 1) & -pkr_sector_size;
    }

    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'LDBG':
            LOD_r_LDBG(pkg, pr, laynode);
            break;
        }
        g_hiprf->exit(pkg);
        cid = g_hiprf->enter(pkg);
    }
    return 1;
}

static inline S32 LOD_r_LDBG(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr, st_PACKER_LTOC_NODE* laynode)
{
    S32 ivar = 0;
    if (pr->subver > 1)
    {
        g_hiprf->readLongs(pkg, &ivar, 1);
        laynode->chksum = ivar;
    }
    return 1;
}

static inline S32 LOD_r_STRM(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    U32 cid = g_hiprf->enter(pkg);
    while (cid != 0)
    {
        switch (cid)
        {
        case 'DHDR':
            LOD_r_DHDR(pkg, pr);
            break;
        case 'DPAK':
            LOD_r_DPAK(pkg, pr);
            break;
        }
        g_hiprf->exit(pkg);
        cid = g_hiprf->enter(pkg);
    }
    return 1;
}

static inline S32 LOD_r_DHDR(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    S32 ivar = 0;
    g_hiprf->readLongs(pkg, &ivar, 1);
    return 1;
}

static inline S32 LOD_r_DPAK(st_HIPLOADDATA* pkg, st_PACKER_READ_DATA* pr)
{
    return 1;
}

static inline st_PACKER_ASSETTYPE* PKR_type2typeref(U32 asstype, st_PACKER_ASSETTYPE* types)
{
    st_PACKER_ASSETTYPE* da_type = NULL;
    if (types != NULL)
    {
        for (st_PACKER_ASSETTYPE* tmptype = types; tmptype->typetag != 0; tmptype++)
        {
            if (tmptype->typetag == asstype)
            {
                da_type = tmptype;
                break;
            }
        }
    }
    return da_type;
}

// Builds the per-type asset lists that PKR_AssetCount and PKR_AssetByType read: counts the
// loadable assets of each handler type (unknown types go to the last list), sizes the lists,
// then fills them in layer order.
static void PKR_bld_typecnt(st_PACKER_READ_DATA* pr)
{
    st_PACKER_LTOC_NODE* laynode;
    st_PACKER_ATOC_NODE* assnode;
    S32 i;
    S32 j;
    S32 idx;
    S32 typcnt[129] = {};
    st_XORDEREDARRAY* tmplist;
    S32 lasttidx = 0;
    U32 lasttype = 0;

    for (i = 0; i < pr->laytoc.cnt; i++)
    {
        laynode = (st_PACKER_LTOC_NODE*)pr->laytoc.list[i];
        for (j = 0; j < laynode->assref.cnt; j++)
        {
            assnode = (st_PACKER_ATOC_NODE*)laynode->assref.list[j];
            if (!(assnode->loadflag & 0x100000) && !(assnode->loadflag & 0x200000))
            {
                if (lasttype != 0 && assnode->asstype == lasttype)
                {
                    idx = lasttidx;
                }
                else
                {
                    idx = PKR_typeHdlr_idx(pr, assnode->asstype);
                    lasttidx = idx;
                    lasttype = assnode->asstype;
                }

                if (idx < 0)
                {
                    typcnt[128]++;
                }
                else
                {
                    typcnt[idx]++;
                }
            }
        }
    }

    for (i = 0; i < 129; i++)
    {
        if (typcnt[i] >= 1)
        {
            XOrdInit(&pr->typelist[i], typcnt[i] > 1 ? typcnt[i] : 2);
        }
    }

    for (i = 0; i < pr->laytoc.cnt; i++)
    {
        st_PACKER_LTOC_NODE* laynode = (st_PACKER_LTOC_NODE*)pr->laytoc.list[i];
        for (j = 0; j < laynode->assref.cnt; j++)
        {
            st_PACKER_ATOC_NODE* assnode = (st_PACKER_ATOC_NODE*)laynode->assref.list[j];
            if (!(assnode->loadflag & 0x100000) && !(assnode->loadflag & 0x200000))
            {
                if (lasttype != 0 && assnode->asstype == lasttype)
                {
                    idx = lasttidx;
                }
                else
                {
                    idx = PKR_typeHdlr_idx(pr, assnode->asstype);
                    lasttidx = idx;
                    lasttype = assnode->asstype;
                }

                st_XORDEREDARRAY* tmplist;
                if (idx < 0)
                {
                    tmplist = &pr->typelist[128];
                }
                else
                {
                    tmplist = &pr->typelist[idx];
                }
                XOrdAppend(tmplist, assnode);
            }
        }
    }
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
