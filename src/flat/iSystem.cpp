#include "iSystem.h"
#include "iFile.h"
#include "iTime.h"
#include "xMath.h"
#include "xMemMgr.h"
#include "xPad.h"
#include "xSnd.h"
#include "xFX.h"
#include "xShadow.h"
#include "xString.h"
#include "xstransvc.h"

#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <rwcore.h>
#include <stdio.h>
#include <string.h>

extern "C" {
S32 RpWorldPluginAttach();
S32 RpCollisionPluginAttach();
S32 RpSkinPluginAttach();
S32 RpHAnimPluginAttach();
S32 fn_8013EDB0();
S32 RpUserDataPluginAttach();
S32 RwEngineStop();
S32 RwEngineClose();
S32 RwEngineTerm();

void fn_80021B1C();

// Not yet identified: the debug, memory-system, file-system and retrace helpers this file
// starts, and the blocks it hands to them.
void fn_80021B24();
S32 fn_80015BFC(RwMemoryFunctions* fns);
RwMemoryFunctions* fn_800168F4();
S32 fn_80015168();
void fn_800167A8(u32 retraceCount);

S32 RwEngineInit(RwMemoryFunctions* fns, U32 initFlags, U32 resArenaSize);
RwTextureCallBackRead RwTextureSetReadCallBack(RwTextureCallBackRead fpCallBack);

struct iSystemDeviceConfig
{
    GXRenderModeObj* renderMode;
    S32 unk4;
    S32 unk8;
};

extern iSystemDeviceConfig lbl_801FE3B8;
extern RwVideoMode lbl_801FE3C4;
extern RwMemoryFunctions lbl_801FE3D4;
extern VIRetraceCallback lbl_8025E974;
extern S32 lbl_8025F870;
}

extern char* months[];
extern char* dotw[];

static S32 RenderWareInit();
static S32 RenderWareExit();
static RwTexture* TextureRead(const char* name, const char* maskName);

void iVSync()
{
    VIWaitForRetrace();
}

S32 iGetDay()
{
    OSTime ticks = OSGetTime();
    OSCalendarTime td;
    OSTicksToCalendarTime(ticks, &td);
    return td.mday;
}

S32 iGetMonth()
{
    OSTime ticks = OSGetTime();
    OSCalendarTime td;
    OSTicksToCalendarTime(ticks, &td);
    return td.mon + 1;
}

// Writes the date as "Weekday Month D, YYYY" and returns the length including the terminator.
U32 iGetCurrFormattedDate(char* str)
{
    char* start = str;
    OSTime ticks = OSGetTime();
    OSCalendarTime td;

    OSTicksToCalendarTime(ticks, &td);

    strcpy(str, dotw[td.wday]);
    strcat(str, months[td.mon]);
    str += strlen(str);

    if (td.mday >= 10)
    {
        *str++ = (td.mday / 10) + '0';
    }

    *str++ = (td.mday % 10) + '0';
    *str++ = ',';
    *str++ = ' ';
    *str++ = (td.year / 1000) + '0';
    *str++ = ((td.year / 100) % 10) + '0';
    *str++ = ((td.year / 10) % 100) + '0';
    *str++ = (td.year % 10) + '0';
    *str++ = '\0';

    return str - start;
}

// Writes the time as "H:MM:SS A.M." on a 12-hour clock and returns the length including the
// terminator.
U32 iGetCurrFormattedTime(char* str)
{
    char* start = str;
    S32 am = 0;

    OSTime ticks = OSGetTime();
    OSCalendarTime td;
    OSTicksToCalendarTime(ticks, &td);

    if (td.hour < 12)
    {
        am = 1;
    }
    else
    {
        td.hour -= 12;
    }

    if (td.hour == 0)
        td.hour = 12;

    if (td.hour >= 10)
    {
        *str++ = (td.hour / 10) + '0';
    }

    *str++ = (td.hour % 10) + '0';
    *str++ = ':';
    *str++ = (td.min / 10) + '0';
    *str++ = (td.min % 10) + '0';
    *str++ = ':';
    *str++ = (td.sec / 10) + '0';
    *str++ = (td.sec % 10) + '0';
    *str++ = ' ';

    if (am)
    {
        *str++ = 'A';
        *str++ = '.';
        *str++ = 'M';
        *str++ = '.';
    }
    else
    {
        *str++ = 'P';
        *str++ = '.';
        *str++ = 'M';
        *str++ = '.';
    }

    *str++ = '\0';

    return str - start;
}

// The function profiler and the file path setting do nothing on this platform.
void iFuncProfileDump()
{
}

void iFileSetPath(char* path)
{
}

void iFuncProfileParse(char* elfPath, S32 profile)
{
}

// Starts the console libraries, the engine's memory, file, timer, pad and sound layers, then
// RenderWare and the math tables, and hooks the retrace counter.
void iSystemInit()
{
    lbl_801FE3B8.renderMode = &GXNtsc480IntDf;
    lbl_801FE3B8.unk4 = 0;
    lbl_801FE3B8.unk8 = 0x40000;

    GXSetMisc(GX_MT_XF_FLUSH, 8);
    OSInit();
    DVDInit();
    VIInit();

    if (!fn_80015BFC(&lbl_801FE3D4))
    {
        OSPanic("iSystem.cpp", 0x216, "Unable to initialize memory system.\n");
    }

    fn_80021B24();
    xMemInit();
    iFileInit();
    iTimeInit();
    xPadInit();
    xSndInit();
    RenderWareInit();
    xMathInit();
    lbl_8025E974 = VISetPreRetraceCallback(fn_800167A8);
}

void iSystemExit()
{
    fn_80021B1C();
    xMathExit();
    RenderWareExit();
    xSndExit();
    xPadKill();
    iFileExit();
    iTimeExit();
    xMemExit();
    OSPanic("iSystem.cpp", 0x25d,
            "(With apologies to Jim Morrison) This the end, my only friend, The End.");
}

// Returns true when a plugin fails to attach.
static U32 RWAttachPlugins()
{
    if (!RpWorldPluginAttach())
    {
        return 1;
    }
    if (!RpCollisionPluginAttach())
    {
        return 1;
    }
    if (!RpSkinPluginAttach())
    {
        return 1;
    }
    if (!RpHAnimPluginAttach())
    {
        return 1;
    }
    if (!fn_8013EDB0())
    {
        return 1;
    }
    if (!RpUserDataPluginAttach())
    {
        return 1;
    }
    return 0;
}

// Returns true on failure. Textures are then read from the loaded assets (see TextureRead).
static S32 RenderWareInit()
{
    RwEngineOpenParams params;

    if (!RwEngineInit(fn_800168F4(), 1, 0x428000))
    {
        return 1;
    }

    RwResourcesSetArenaSize(0x428000);

    if (!fn_80015168())
    {
        return 1;
    }

    if (RWAttachPlugins())
    {
        return 1;
    }

    params.displayID = &lbl_801FE3B8;

    if (!RwEngineOpen(&params))
    {
        RwEngineTerm();
        return 1;
    }

    RwEngineGetVideoModeInfo(&lbl_801FE3C4, RwEngineGetCurrentVideoMode());

    if (!RwEngineStart())
    {
        RwEngineClose();
        RwEngineTerm();
        return 1;
    }

    RwTextureSetReadCallBack(TextureRead);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
    xShadowCameraCreate();
    xFXInit();
    return 0;
}

static S32 RenderWareExit()
{
    RwEngineStop();
    RwEngineClose();
    return RwEngineTerm();
}

// RenderWare's texture reader: textures come from the scene's loaded .rw3 assets, not files.
// A paletted texture (depth below 8) is refused unless the driver converted it (format 14).
static RwTexture* TextureRead(const char* name, const char* maskName)
{
    char buf[256];
    U32 size;
    RwTexture* tex;
    U8* ext;

    sprintf(buf, "%s.rw3", name);
    tex = (RwTexture*)xSTFindAsset(xStrHash(buf), &size);

    if (tex && tex->raster && tex->raster->depth < 8)
    {
        ext = (U8*)tex->raster + lbl_8025F870;

        if (!ext || *(U32*)(ext + 0xC) != 14)
        {
            return NULL;
        }
    }

    if (tex)
    {
        strcpy(tex->name, name);
        strcpy(tex->mask, maskName);
        return tex;
    }

    return NULL;
}
