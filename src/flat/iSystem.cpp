#include "iSystem.h"
#include "iFile.h"
#include "iTime.h"
#include "xMath.h"
#include "xMemMgr.h"
#include "xPad.h"
#include "xSnd.h"

#include <dolphin/os.h>
#include <dolphin/vi.h>
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
}

extern char* months[];
extern char* dotw[];

static S32 RenderWareExit();

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

static S32 RenderWareExit()
{
    RwEngineStop();
    RwEngineClose();
    return RwEngineTerm();
}
