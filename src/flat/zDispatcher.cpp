#include "version.h"

#if VERSION_GIHE78_REV0
#include "xBase.h"
#include "xMemMgr.h"
#include "iSnd.h"
#include "zLaugh.h"
#include "zMusic.h"
#include "zGlobals.h"
#include "zScene.h"
#include "zHud.h"
#include "zGameState.h"
#include "zRumble.h"
#include "zFMV.h"
#include "xScene.h"
#include "xScrFx.h"

#include <string.h>

struct st_ZDISPATCH_DATA : xBase
{
    p2BaseAsset* rawass;
    S32 placeholder;
};

struct st_ZDISPATCH_DEPOT
{
    st_ZDISPATCH_DATA* raw_pool;
    S32 raw_cnt;
};

struct st_ZDISPATCH_CONTEXT
{
    S32 cmd;
    void* indata;
    void* inxtra;
    void* result;
};

void zDispatcher_Init(st_ZDISPATCH_DATA*, p2BaseAsset*);
extern "C" S32 fn_80046E18(st_ZDISPATCH_DATA*, st_ZDISPATCH_CONTEXT*);
extern "C" void fn_80032254(S32, S32);
extern "C" void fn_8003D408(F32, F32);
extern "C" void fn_8003D3F4(F32, F32);
extern "C" void fn_8003D3E0(F32, F32);
extern "C" S32 fn_800D7AEC();
extern "C" S32 fn_800D7810();
extern "C" S32 fn_800D7AA0();
extern "C" S32 fn_800D7A54();
extern "C" S32 fn_800D7A08();
extern "C" void fn_800AF8D8(S32);
extern "C" void fn_800D7804();
extern "C" void fn_800D77F8();
extern "C" void fn_800D77EC();
extern "C" void fn_800D77E0();
extern "C" void fn_800D77D4();
extern "C" void fn_800D77B4();
extern "C" void fn_800CD268(S32);
extern "C" void fn_800CD254();
extern "C" void fn_800C9B14(U32, F32*);
extern "C" void fn_800BF64C(char*, U32, F32);
extern "C" U32 fn_800D5D74();
void zGameStateSwitchEvent(S32);
void zEntPlayer_StoreCheckPoint(F32*);

extern "C" S32 fn_8004651C(xBase*, xBase* to, U32 event, F32* param, xBase* widget)
{
    switch (event)
    {
    case 0xa:
    {
        st_ZDISPATCH_DATA* data = (st_ZDISPATCH_DATA*)to;
        p2BaseAsset* asset = data->rawass;
        xBaseReset(data, asset);
        memset(&data->rawass, 0, sizeof(*data) - sizeof(xBase));
        zDispatcher_Init(data, asset);
        xSceneID2Name(globals.sceneCur, data->id);
        break;
    }
    case 0x60:
    {
        S32 value = 0;
        st_ZDISPATCH_CONTEXT context = { 1, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x61:
    {
        S32 value = 1;
        st_ZDISPATCH_CONTEXT context = { 1, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x62:
    {
        S32 value = 2;
        st_ZDISPATCH_CONTEXT context = { 1, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x63:
    {
        S32 value = 3;
        st_ZDISPATCH_CONTEXT context = { 1, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x64:
    {
        S32 value = 1;
        st_ZDISPATCH_CONTEXT context = { 3, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x65:
    {
        S32 value = 0;
        st_ZDISPATCH_CONTEXT context = { 3, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x66:
    {
        S32 value = 0;
        st_ZDISPATCH_CONTEXT context = { 5, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x67:
    {
        S32 value = 1;
        st_ZDISPATCH_CONTEXT context = { 5, &value, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x68:
    {
        st_ZDISPATCH_CONTEXT context = { 8, NULL, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x69:
    {
        st_ZDISPATCH_CONTEXT context = { 9, NULL, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x6a:
    {
        st_ZDISPATCH_CONTEXT context = { 12, NULL, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x6b:
    {
        st_ZDISPATCH_CONTEXT context = { 13, NULL, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x6c:
    {
        st_ZDISPATCH_CONTEXT context = { 16, NULL, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x6d:
    {
        st_ZDISPATCH_CONTEXT context = { 17, NULL, NULL, NULL };
        fn_80046E18((st_ZDISPATCH_DATA*)to, &context);
        break;
    }
    case 0x89:
    case 0x8a:
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
    case 0x8f:
    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
    case 0x9c:
    case 0x9d:
    case 0x9e:
    case 0x9f:
    case 0xa0:
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa5:
    case 0xa6:
        zGameStateSwitchEvent(event);
        break;
    case 0xa8:
    case 0xa9:
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
        fn_800C9B14(event, param);
        break;
    case 0xc2:
        zEntPlayer_StoreCheckPoint(param);
        break;
    case 0x101:
        fn_800D7804();
        break;
    case 0x102:
        fn_800D77F8();
        break;
    case 0x103:
        fn_800D77EC();
        break;
    case 0x104:
        fn_800D77E0();
        break;
    case 0x105:
        fn_800D77D4();
        break;
    case 0x10c:
        zLaughNotify(13);
        break;
    case 0x10d:
    case 0x10e:
    case 0x10f:
    case 0x110:
        zMusicNotify(3);
        break;
    case 0x11e:
        zHudShow();
        break;
    case 0x11f:
        zHudHide();
        break;
    case 0x120:
    {
        iColor_tag black = { 0, 0, 0, 0 };
        iColor_tag clear = { 0, 0, 0, 255 };
        xScrFxFade(&black, &clear, param[0], NULL, 1, 1);
        break;
    }
    case 0x126:
        switch ((U32)param[0])
        {
        case 0:
            fn_800BF64C(zFMVFileGetName(eFMVFile_WBExtra0), 1, 0.1f);
            break;
        case 1:
            fn_800BF64C(zFMVFileGetName(eFMVFile_WBExtra1), 1, 0.1f);
            break;
        case 2:
            fn_800BF64C(zFMVFileGetName(eFMVFile_Cin11), 0, 0.0f);
            break;
        case 3:
            fn_800BF64C(zFMVFileGetName(eFMVFile_EndCredits), 0, 0.0f);
            zMusicKill();
            break;
        case 4:
            zFMVPlay(zFMVFileGetName(eFMVFile_EndCredits), 1, 0.1f);
            break;
        case 5:
            if (fn_800D5D74())
            {
                fn_800BF64C(zFMVFileGetName(eFMVFile_EndCredits), 1, 0.1f);
            }
            else
            {
                fn_800BF64C(zFMVFileGetName(eFMVFile_Credits), 0x10001, 0.1f);
            }
            break;
        case 6:
            zFMVPlay(zFMVFileGetName((eFMVFile)((param[1] + 27.0f) - 1.0f)), 1, 0.1f);
            break;
        }
        break;
    case 0x127:
        fn_800D77B4();
        break;
    case 0x129:
        zMusicNotifyEvent(param, widget);
        break;
    case 0x130:
        fn_800CD268(1);
        fn_800CD254();
        break;
    case 0x131:
        fn_800CD268(0);
        break;
    }
    return 1;
}

// Dispatch control, sound, and game-state commands.
extern "C" S32 fn_80046E18(st_ZDISPATCH_DATA*, st_ZDISPATCH_CONTEXT* context)
{
    S32* input = (S32*)context->indata;
    S32* result = (S32*)context->result;
    static S32 warned = 0;
    if (!warned)
    {
        warned = 1;
    }
    switch (context->cmd)
    {
    case 0:
        *result = 0;
        break;
    case 1:
        if (result)
        {
            *result = 0;
        }
        break;
    case 2:
        *result = fn_800D7810();
        break;
    case 3:
        globals.option_vibration = *input;
        fn_80032254(0, *input);
        zRumbleStart(0, SDR_Test);
        break;
    case 4:
        *result = fn_800D7AEC();
        break;
    case 5:
        if (*input == 0)
        {
            iSndStereo(0);
        }
        else if (*input == 1)
        {
            iSndStereo(1);
        }
        if (result)
        {
            *result = fn_800D7AEC();
        }
        break;
    case 6:
        *result = fn_800D7AA0();
        break;
    case 7:
    {
        F32 volume = 0.1f * (*input);
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D408(volume, volume);
        if (result)
        {
            *result = fn_800D7AA0();
        }
        break;
    }
    case 8:
    {
        F32 volume = 0.1f * (fn_800D7AA0() + (input ? *input : 1));
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D408(volume, volume);
        if (result)
        {
            *result = fn_800D7AA0();
        }
        break;
    }
    case 9:
    {
        F32 volume = 0.1f * (fn_800D7AA0() - (input ? *input : 1));
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D408(volume, volume);
        if (result)
        {
            *result = fn_800D7AA0();
        }
        break;
    }
    case 10:
        *result = fn_800D7A54();
        break;
    case 11:
    {
        F32 volume = 0.1f * (*input);
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D3F4(volume, volume);
        if (result)
        {
            *result = fn_800D7A54();
        }
        break;
    }
    case 12:
    {
        F32 volume = 0.1f * (fn_800D7A54() + (input ? *input : 1));
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D3F4(volume, volume);
        if (result)
        {
            *result = fn_800D7A54();
        }
        break;
    }
    case 13:
    {
        F32 volume = 0.1f * (fn_800D7A54() - (input ? *input : 1));
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D3F4(volume, volume);
        if (result)
        {
            *result = fn_800D7A54();
        }
        break;
    }
    case 14:
        *result = fn_800D7A08();
        break;
    case 15:
    {
        F32 volume = 0.1f * (*input);
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D3E0(volume, volume);
        if (result)
        {
            *result = fn_800D7A08();
        }
        break;
    }
    case 16:
    {
        F32 volume = 0.1f * (fn_800D7A08() + (input ? *input : 1));
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D3E0(volume, volume);
        if (result)
        {
            *result = fn_800D7A08();
        }
        break;
    }
    case 17:
    {
        F32 volume = 0.1f * (fn_800D7A08() - (input ? *input : 1));
        if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        else if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        fn_8003D3E0(volume, volume);
        if (result)
        {
            *result = fn_800D7A08();
        }
        break;
    }
    case 18:
        *result = zGameStateGet();
        break;
    case 19:
        zGameStateSwitch(*input);
        if (result)
        {
            *result = zGameStateGet();
        }
        break;
    case 20:
        *result = zGameModeGet();
        break;
    case 21:
        fn_800AF8D8(*input);
        if (result)
        {
            *result = zGameModeGet();
        }
        break;
    }
    return 1;
}

static st_ZDISPATCH_DEPOT g_zdsp_depot = {};
static S32 g_zdsp_init;

// Look up the dispatcher name in the current scene.
void zDispatcher_InitDep(st_ZDISPATCH_DATA* data, zScene*)
{
    xSceneID2Name(globals.sceneCur, data->id);
}

// Initialize the base and attach the event callback and asset links.
void zDispatcher_Init(st_ZDISPATCH_DATA* data, p2BaseAsset* asset)
{
    xBaseInit(data, asset);
    data->rawass = asset;
    data->eventFunc = fn_8004651C;
    if (data->linkCount)
    {
        data->link = (p2LinkAsset*)(data->rawass + 1);
    }
    else
    {
        data->link = NULL;
    }
    xSceneID2Name(globals.sceneCur, data->id);
}

// Restore the base state from the save stream.
void zDispatcher_Load(st_ZDISPATCH_DATA* data, xSerial* serial)
{
    xBaseLoad(data, serial);
}

// Save the base state to the stream.
void zDispatcher_Save(st_ZDISPATCH_DATA* data, xSerial* serial)
{
    xBaseSave(data, serial);
}

// Select one dispatcher from the allocated pool.
st_ZDISPATCH_DATA* zDispatcher_getInst(st_ZDISPATCH_DATA* pool, S32 idx)
{
    return &pool[idx];
}

st_ZDISPATCH_DATA* zDispatcher_memPool(S32 cnt)
{
    st_ZDISPATCH_DATA* pool;
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;

    if (cnt < 1)
    {
        return NULL;
    }

    pool = (st_ZDISPATCH_DATA*)xMemAlloc(gActiveHeap, cnt * sizeof(st_ZDISPATCH_DATA));
    memset(pool, 0, cnt * sizeof(st_ZDISPATCH_DATA));
    depot->raw_pool = pool;
    depot->raw_cnt = cnt;
    return pool;
}

// Clear the depot after the last scene exits.
void zDispatcher_sceneFinish()
{
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;

    if (--g_zdsp_init == 0)
    {
        memset(depot, 0, sizeof(st_ZDISPATCH_DEPOT));
    }
}

// Reset the shared depot when the first scene enters.
void zDispatcher_scenePrepare()
{
    st_ZDISPATCH_DEPOT* depot = &g_zdsp_depot;

    if (g_zdsp_init++ == 0)
    {
        memset(depot, 0, sizeof(st_ZDISPATCH_DEPOT));
    }
}

#endif
