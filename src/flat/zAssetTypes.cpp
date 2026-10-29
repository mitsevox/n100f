#include "iModel.h"
#include "iSnd.h"
#include "xAnim.h"
#include "xEnv.h"
#include "xMorph.h"
#include "xMovePoint.h"
#include "xSnd.h"
#include "xpkrsvc.h"
#include "xstransvc.h"
#include "zEntVillain.h"
#include "zGlobals.h"
#include "zScene.h"
#include "version.h"

#include <rpworld.h>
#include <rwcore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if VERSION_GIHE78_REV0
extern "C" S32 lbl_8025E984;
#define BSP_LOADING_H003 lbl_8025E984
#elif VERSION_GIHE78_REV1
extern "C" S32 lbl_8027081C;
#define BSP_LOADING_H003 lbl_8027081C
#endif

struct xAnimAssetTable
{
    U32 Magic;
    U32 NumRaw;
    U32 NumFiles;
    U32 NumStates;
    U32 ConstructFunc;
};

struct xAnimAssetFile
{
    U32 FileFlags;
    F32 Duration;
    F32 TimeOffset;
    U16 NumAnims[2];
    void** RawData;
    S32 Physics;
    S32 StartPose;
    S32 EndPose;
};

struct xAnimAssetState
{
    U32 StateID;
    U32 FileIndex;
    U32 EffectCount;
    U32 EffectOffset;
    F32 Speed;
};

struct xAnimAssetEffect
{
    U32 StateID;
    F32 StartTime;
    F32 EndTime;
    U32 Flags;
    U32 EffectType;
    U32 UserDataSize;
};

struct zAnimFxSound
{
    U32 ID;
    F32 vol;
    F32 pitch;
    U32 priority;
    U32 flags;
    F32 radius;
};

S32 zEntVillain_PlayAFXSound(st_zEntVillain* ent, zAnimFxSound* snd, U32* sid_played);

xAnimTable* zEntPlayer_AnimTable();
xAnimTable* zEntVillain_AnimTable_Standard();
xAnimTable* zEntPlayer_UmbrellaAnimTable();
xAnimTable* zEntPlayer_HeadAnimTable();
xAnimTable* zEntShaggy_Anim1();
xAnimTable* zEntPlayer_LampshadeAnimTable();
xAnimTable* zEntShaggy_Anim3();
xAnimTable* zEntShaggy_Anim2();
xAnimTable* zEntShaggy_Chimney();
xAnimTable* zEntVillain_AnimTable_BlackKnight();
xAnimTable* zEntVillain_AnimTable_GreenGhost();
xAnimTable* zEntVillain_AnimTable_RedBeard();
xAnimTable* zEntVillain_AnimTable_MasterMind();
xAnimTable* zEntVillain_AnimTable_BossSham();
xAnimTable* zEntVillain_AnimTable_Simple();
xAnimTable* zEntVillain_AnimTable_Gang();
xAnimTable* zEntVillain_AnimTable_Shark();
xAnimTable* zEntShaggy_Anim0();
xAnimTable* zEntShaggy_Anim8();
xAnimTable* zEntShaggy_Anim5();
xAnimTable* zEntVillain_AnimTable_Witch();
xAnimTable* zEntPlayer_TongueAnimTable();

static void* SndInfoRead(void*, U32, void* indata, U32 insize, U32* outsize);
static void MovePoint_Unload(void* userdata, U32);
static void Anim_Unload(void*, U32);
static void* ATBL_Read(void*, U32 aid, void* indata, U32, U32* outsize);
static void* FindAssetCB(U32 ID, char*);
static U32 soundEffectCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                         void* object);
static U32 dummyEffectCB(U32, xAnimActiveEffect*, xAnimSingle*, void*);
static void TextureRW3_Unload(void* userdata, U32);
static void* RWTX_Read(void*, U32, void* indata, U32 insize, U32* outsize);
static RwTexture* TexCB(RwTexture* texture, void* data);
static void BSP_Unload(void*, U32);
static void* BSP_Read(void*, U32 aid, void* indata, U32 insize, U32* outsize);
static void Model_Unload(void* userdata, U32);
static void* Model_Read(void*, U32, void* indata, U32 insize, U32* outsize);

static st_PACKER_ASSETTYPE assetTypeHandlers[] = {
    { 'BSP ', 0, 0, BSP_Read, NULL, NULL, NULL, NULL, BSP_Unload, NULL },
    { 'TXD ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'MODL', 0, 0, Model_Read, NULL, NULL, NULL, NULL, Model_Unload, NULL },
    { 'ANIM', 0, 0, NULL, NULL, NULL, NULL, NULL, Anim_Unload, NULL },
    { 'RWTX', 0, 0, RWTX_Read, NULL, NULL, NULL, NULL, TextureRW3_Unload, NULL },
    { 'CAM ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PLYR', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'NPC ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'ITEM', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PKUP', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'TRIG', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'SDF ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'TEX ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'TXT ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'ENV ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'ATBL', 0, 0, ATBL_Read, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'MINF', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PICK', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PLAT', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PEND', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'MRKR', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'MVPT', 0, 0, NULL, NULL, NULL, NULL, NULL, MovePoint_Unload, NULL },
    { 'TIMR', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'CNTR', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PORT', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'SND ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'SNDS', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'GRUP', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'MPHT', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'SFX ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'SNDI', 0, 0, SndInfoRead, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'HANG', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'SIMP', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'BUTN', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'SURF', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'DSTR', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'MAPR', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'GUST', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'VOLU', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'UI  ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'UIFT', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'TEXT', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'COND', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'DPAT', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PRJT', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'LOBM', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'FOG ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'LITE', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PARE', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'PARS', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'CSN ', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'CTOC', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'CSNM', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'EGEN', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 'ALST', 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { 0 },
};

static xAnimTable* (*tableFuncList[])() = {
    zEntPlayer_AnimTable,
    zEntVillain_AnimTable_Standard,
    zEntPlayer_UmbrellaAnimTable,
    zEntPlayer_HeadAnimTable,
    zEntShaggy_Anim1,
    zEntPlayer_LampshadeAnimTable,
    zEntShaggy_Anim3,
    zEntShaggy_Anim2,
    zEntShaggy_Chimney,
    zEntVillain_AnimTable_BlackKnight,
    zEntVillain_AnimTable_GreenGhost,
    zEntVillain_AnimTable_RedBeard,
    zEntVillain_AnimTable_MasterMind,
    zEntVillain_AnimTable_BossSham,
    zEntVillain_AnimTable_Simple,
    zEntVillain_AnimTable_Gang,
    zEntVillain_AnimTable_Shark,
    zEntShaggy_Anim0,
    zEntShaggy_Anim8,
    zEntShaggy_Anim5,
    zEntVillain_AnimTable_Witch,
    zEntPlayer_TongueAnimTable,
};

static U32 (*effectFuncList[])(U32, xAnimActiveEffect*, xAnimSingle*, void*) = {
    dummyEffectCB,
    soundEffectCB,
};

// Register the game's asset type handlers with the asset streaming system.
void zAssetStartup()
{
    xSTStartup(assetTypeHandlers);
}

// Shut the asset streamer down.
void zAssetShutdown()
{
    xSTShutdown();
}

// Read a model asset into an atomic.
static void* Model_Read(void*, U32, void* indata, U32 insize, U32* outsize)
{
    RpAtomic* model = iModelFileNew(indata, insize);
    *outsize = 0x70;
    return model;
}

// Unload an atomic read by Model_Read.
static void Model_Unload(void* userdata, U32)
{
    if (userdata != NULL)
    {
        iModelUnload((RpAtomic*)userdata);
    }
}

// Stream the scene's world. The world for scene H003 is read with a flag set that the
// platform layer checks.
static void* BSP_Read(void*, U32 aid, void* indata, U32 insize, U32* outsize)
{
    RwMemory rwmem = { (U8*)indata, insize };

    RwStream* stream = RwStreamOpen(rwSTREAMMEMORY, rwSTREAMREAD, &rwmem);
    if (stream == NULL)
    {
        printf("BSP_Read RwStreamOpen failed\n");
    }

    if (!RwStreamFindChunk(stream, rwID_WORLD, NULL, NULL))
    {
        RwChunkHeaderInfo chunkHeaderInfo;
        RwStreamReadChunkHeaderInfo(stream, &chunkHeaderInfo);
        *outsize = 0;
        return NULL;
    }

    if (globals.sceneCur->sceneID == zSceneIDFromName("H003"))
    {
        BSP_LOADING_H003 = 1;
    }

    RpWorld* bsp = RpWorldStreamRead(stream);

    if (BSP_LOADING_H003)
    {
        BSP_LOADING_H003 = 0;
    }

    if (bsp == NULL)
    {
        printf("BSP_Read RpWorldStreamRead failed\n");
    }

    RwStreamClose(stream, NULL);
    printf("BSP_Read(BSP AID:0x%08X):%s\n", aid, bsp != NULL ? "Ok" : "BAD!");
    *outsize = 4;
    return bsp;
}

// Free the scene environment's world.
static void BSP_Unload(void*, U32)
{
    xEnvFree(globals.sceneCur->env);
}

// Keep the first texture of a dictionary.
static RwTexture* TexCB(RwTexture* texture, void* data)
{
    if (*(RwTexture**)data == NULL)
    {
        *(RwTexture**)data = texture;
    }
    return texture;
}

// A texture asset is a texture dictionary holding one texture, which is taken out and kept.
static void* RWTX_Read(void*, U32, void* indata, U32 insize, U32* outsize)
{
    RwTexDictionary* txd;
    RwMemory rwmem;
    RwStream* stream;
    RwTexture* tex = NULL;

    if (insize != 0)
    {
        rwmem.start = (U8*)indata;
        rwmem.length = insize;
        stream = RwStreamOpen(rwSTREAMMEMORY, rwSTREAMREAD, &rwmem);
        if (stream != NULL)
        {
            if (!RwStreamFindChunk(stream, rwID_TEXDICTIONARY, NULL, NULL))
            {
                RwStreamClose(stream, NULL);
            }
            else
            {
                txd = RwTexDictionaryStreamRead(stream);
                RwStreamClose(stream, NULL);
                if (txd != NULL)
                {
                    RwTexDictionaryForAllTextures(txd, TexCB, &tex);
                    if (tex == NULL)
                    {
                        RwTexDictionaryDestroy(txd);
                    }
                    else
                    {
                        RwTexDictionaryRemoveTexture(tex);
                        RwTexDictionaryDestroy(txd);
                        RwTextureAddRef(tex);
                        RwTextureSetFilterMode(tex, rwFILTERLINEAR);
                        *outsize = sizeof(RwTexture);
                        return tex;
                    }
                }
            }
        }
    }

    *outsize = insize;
    return NULL;
}

// Destroy a texture read by RWTX_Read, whatever its reference count.
static void TextureRW3_Unload(void* userdata, U32)
{
    if (userdata != NULL)
    {
        ((RwTexture*)userdata)->refCount = 1;
        RwTextureDestroy((RwTexture*)userdata);
    }
}

// Placeholder animation effect; does nothing.
static U32 dummyEffectCB(U32, xAnimActiveEffect*, xAnimSingle*, void*)
{
    return 0;
}

// Animation sound effect: a villain plays it through its own sound set when it can, anything
// else plays it at the entity.
static U32 soundEffectCB(U32 cbenum, xAnimActiveEffect* acteffect, xAnimSingle* single,
                         void* object)
{
    U32 sndhandle = 0;
    U32 vil_SID = 0;
    S32 vil_result;

    if (cbenum == 1)
    {
        _xEnt* ent_tmp = (_xEnt*)object;
        zAnimFxSound* snd = (zAnimFxSound*)(acteffect->Effect + 1);

        if (ent_tmp == NULL)
        {
            vil_result = 0;
        }
        else if (ent_tmp->baseType == 2)
        {
            vil_result = zEntVillain_PlayAFXSound((st_zEntVillain*)ent_tmp, snd, &vil_SID);
        }
        else
        {
            vil_result = 0;
        }

        if (vil_result > 0)
        {
            sndhandle = vil_SID;
        }
        else
        {
            sndhandle = xSndPlay3D(snd->ID, snd->vol * 0.5f, snd->pitch, snd->priority,
                                   snd->flags, ent_tmp, snd->radius);
        }
    }

    if (cbenum == 3)
    {
        xSndStop(acteffect->Handle);
    }

    return sndhandle;
}

// Look an asset up by id for the anim-table builder.
static void* FindAssetCB(U32 ID, char*)
{
    U32 size;
    return xSTFindAsset(ID, &size);
}

// Build an animation table from an 'ATBL' asset: resolve its raw animations, make its
// animation files, construct the table its constructor index names, and attach the asset's
// states, speeds and effects.
static void* ATBL_Read(void*, U32 aid, void* indata, U32, U32* outsize)
{
    U32 i;
    U32 j;
    U32 debugNum = 0;
    U32 tmpsize;
    xAnimTable* table;
    xAnimState* astate;
    xAnimTransition* atran;

    xAnimAssetTable* zaTbl = (xAnimAssetTable*)indata;
    void** zaRaw = (void**)(zaTbl + 1);
    xAnimAssetFile* zaFile = (xAnimAssetFile*)(zaRaw + zaTbl->NumRaw);
    xAnimAssetState* zaState = (xAnimAssetState*)(zaFile + zaTbl->NumFiles);

    for (i = 0; i < zaTbl->NumRaw; i++)
    {
        zaRaw[i] = xSTFindAsset((U32)zaRaw[i], &tmpsize);
    }

    for (i = 0; i < zaTbl->NumRaw; i++)
    {
        if (zaRaw[i] == NULL)
        {
            printf("WARNING:  Animation file not found on animtable id 0x%x\n", aid);
            for (j = 0; j < zaTbl->NumRaw; j++)
            {
                if (zaRaw[j] != NULL)
                {
                    zaRaw[i] = zaRaw[j];
                    break;
                }
            }
        }
    }

    for (i = 0; i < zaTbl->NumRaw; i++)
    {
        if (*(U32*)zaRaw[i] == 'QSPM')
        {
            xMorphSeqSetup(zaRaw[i], FindAssetCB);
        }
    }

    for (i = 0; i < zaTbl->NumFiles; i++)
    {
        zaFile[i].RawData = (void**)((U32)zaFile[i].RawData + (U32)zaTbl);
        for (j = 0; j < (U32)(zaFile[i].NumAnims[0] * zaFile[i].NumAnims[1]); j++)
        {
            zaFile[i].RawData[j] = zaRaw[(U32)zaFile[i].RawData[j]];
        }
    }

    xAnimFile** fList = (xAnimFile**)zaFile;
    for (i = 0; i < zaTbl->NumFiles; i++)
    {
        fList[i] = xAnimFileNewBilinear(zaFile[i].RawData, "", zaFile[i].FileFlags, NULL,
                                        zaFile[i].NumAnims[0], zaFile[i].NumAnims[1]);
        if (zaFile[i].TimeOffset >= 0.0f)
        {
            xAnimFileSetTime(fList[i], zaFile[i].Duration, zaFile[i].TimeOffset);
        }

        if (zaFile[i].Physics >= 0)
        {
            xAnimFileSetPhysics(fList[i], zaRaw[zaFile[i].Physics]);
        }
        else if (zaFile[i].StartPose >= 0)
        {
            xAnimFileSetPoses(fList[i], zaRaw[zaFile[i].StartPose], 0.0f,
                              zaRaw[zaFile[i].EndPose], 0.0f);
        }
    }

    if (zaTbl->ConstructFunc >= sizeof(tableFuncList) / sizeof(tableFuncList[0]))
    {
        zaTbl->ConstructFunc = 3;
    }

    gxAnimUseGrowAlloc = 1;

    table = tableFuncList[zaTbl->ConstructFunc]();

    char tmpstr[32];
    for (i = 0; i < zaTbl->NumStates; i++)
    {
        astate = xAnimTableAddFileID(table, fList[zaState[i].FileIndex], zaState[i].StateID);
        if (astate == NULL)
        {
            sprintf(tmpstr, "Debug%02d", debugNum++);
            astate = xAnimTableNewState(table, tmpstr, 0x20, 0x80000000, 1.0f, NULL, NULL, 0.0f,
                                        NULL);
            atran = xAnimTableNewTransition(table, tmpstr, "", NULL, NULL, 0x10, 0, 0.0f, 0.0f, 0,
                                            0, 0.2f, NULL);
            atran->Dest = table->StateList;
            xAnimTableAddFileID(table, fList[zaState[i].FileIndex], astate->ID);
        }
        astate->Speed = zaState[i].Speed;
    }

    xAnimFile* foundFile = NULL;
    for (astate = table->StateList; astate != NULL; astate = astate->Next)
    {
        if (foundFile == NULL && astate->Data != NULL)
        {
            foundFile = astate->Data;
        }
    }
    for (astate = table->StateList; astate != NULL; astate = astate->Next)
    {
        if (astate->Data == NULL)
        {
            astate->Data = foundFile;
            astate->UserFlags |= 0x40000000;
        }
    }

    for (i = 0; i < zaTbl->NumStates; i++)
    {
        if (zaState[i].EffectCount != 0)
        {
            xAnimState* state = xAnimTableGetStateID(table, zaState[i].StateID);
            xAnimAssetEffect* zaEffect = (xAnimAssetEffect*)((U32)zaTbl + zaState[i].EffectOffset);

            if (state != NULL)
            {
                for (j = 0; j < zaState[i].EffectCount; j++)
                {
                    xAnimEffect* effect =
                        xAnimStateNewEffect(state, zaEffect->Flags, zaEffect->StartTime,
                                            zaEffect->EndTime, effectFuncList[zaEffect->EffectType],
                                            zaEffect->UserDataSize);
                    memcpy(effect + 1, zaEffect + 1, zaEffect->UserDataSize);
                    zaEffect = (xAnimAssetEffect*)((U32)(zaEffect + 1) + zaEffect->UserDataSize);
                }
            }
        }
    }

    gxAnimUseGrowAlloc = 0;
    *outsize = sizeof(xAnimTable);
    return table;
}

// Animations need no unload work.
static void Anim_Unload(void*, U32)
{
}

// Destroy a move point's spline.
static void MovePoint_Unload(void* userdata, U32)
{
    xMovePointSplineDestroy((xMovePoint*)userdata);
}

// Copy the sound bank info out of the stream and hand it to the sound layer.
static void* SndInfoRead(void*, U32, void* indata, U32 insize, U32* outsize)
{
    void* dest = malloc(insize);
    if (dest == NULL)
    {
        return dest;
    }

    memcpy(dest, indata, insize);
    iSndLoadSounds(dest);
    *outsize = insize;
    return dest;
}
