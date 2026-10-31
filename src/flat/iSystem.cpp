#include "iSystem.h"
#include "iFile.h"
#include "iSnd.h"
#include "iTime.h"
#include "xMath.h"
#include "xDebug.h"
#include "xMemMgr.h"
#include "xPad.h"
#include "xSnd.h"
#include "xFX.h"
#include "xShadow.h"
#include "xString.h"
#include "xTRC.h"
#include "xstransvc.h"

#include <dolphin/dvd.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/pad.h>
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

// GC-only helpers with no name in any reference build: the disc check and the reset path the
// TRC code calls, the reset button poll, the retrace callback, the arena and stack fill, and the
// DOL entry point.
void fn_80016600();
void fn_80016698(S32 reset);
void fn_80016730();
void fn_80016768();
void fn_800167A8(u32 retraceCount);
void fn_800167F0();
void fn_80016898();

void __start();
void mem_null(U32 addr, U32 size);

S32 RwEngineInit(RwMemoryFunctions* fns, U32 initFlags, U32 resArenaSize);
RwFileFunctions* RwOsGetFileInterface();
RwTextureCallBackRead RwTextureSetReadCallBack(RwTextureCallBackRead fpCallBack);

struct iSystemDeviceConfig
{
    GXRenderModeObj* renderMode;
    S32 unk4;
    S32 unk8;
};

// The globals keep their dtk labels; bfbb's names are given beside them.
extern iSystemDeviceConfig lbl_801FE3B8;          // deviceConfig
extern RwVideoMode lbl_801FE3C4;                  // sVideoMode
extern RwMemoryFunctions lbl_801FE3D4;            // MemoryFunctions
extern S32 lbl_8025E3F8;                          // rwID_DOLPHINDEVICEMODULE
extern S32 lbl_8025E96C;                          // last reset button state
extern S32 lbl_8025E970;                          // reset button released
extern VIRetraceCallback lbl_8025E974;            // the retrace callback this one replaced
extern U32 lbl_8025E978;                          // last retrace count
extern U32 lbl_8025E97C;                          // add
extern U32 lbl_8025E980;                          // size
extern S32 lbl_8025E984;                          // RenderWare allocates from the xMem heap
extern S32 lbl_8025E988;                          // FSOpenFiles
extern S32 lbl_8025F870;
extern U8 _stack_addr[];
extern U8 _stack_end[];
}

// FSModuleInfo: the file system plugin's offset in the RenderWare globals and its open count.
extern "C" RwModuleInfo lbl_8025E98C;

// An open disc file: a 0x2800-byte read buffer, the DVD handle, and the read position, the file
// length, the position in the buffer and the access mode.
#define DLFILE_BUF_SIZE 0x2800

struct dlFile
{
    char readBuffer[DLFILE_BUF_SIZE];
    DVDFileInfo fileInfo;
    U32 pos;
    U32 length;
    U32 bufpos;
    S32 mode;
};

// The file system plugin's globals: the file functions it replaced and its free list of files.
struct dlFSGlobals
{
    RwFileFunctions oldFuncs;
    RwFreeList* fileFreeList;
};

#define FSGLOBALS() ((dlFSGlobals*)((U8*)RwEngineInstance + lbl_8025E98C.globalsOffset))

extern char* months[];
extern char* dotw[];

extern U32 mem_top_alloc;
extern U32 mem_base_alloc;
extern OSHeapHandle the_heap;

static U32 RenderWareInit();
static S32 RenderWareExit();
static RwTexture* TextureRead(const char* name, const char* maskName);
static S32 DolphinInstallFileSystem();
S32 DolphinInitMemorySystem(RwMemoryFunctions* memoryFuncs);
void null_func();
_tagTRCState iTRCGetDiskState();
U8 iTRCDiskIdentified();

// RenderWare's memory functions (see DolphinInitMemorySystem).
RwMemoryFunctions* psGetMemoryFunctions()
{
    return &lbl_801FE3D4;
}

// GC only: the DOL's entry point fills the stack with 0xDEADBEEF, so its high-water mark can be
// read later, and then starts the runtime.
extern "C" void fn_80016898()
{
    for (U32* p = (U32*)_stack_end; p < (U32*)_stack_addr; p++)
    {
        *p = 0xDEADBEEF;
    }

    __start();
}

// GC only: fills the free part of RenderWare's resource arena with 0xDEADBEEF. Takes the largest
// entry the arena still gives, in 1 KB steps, and frees it again.
extern "C" void fn_800167F0()
{
    S32 size = RwResourcesGetArenaSize();
    RwResEntry* entry = RwResourcesAllocateResEntry(NULL, NULL, size, NULL);

    while (!entry && size > 0)
    {
        size -= 0x400;
        entry = RwResourcesAllocateResEntry(NULL, NULL, size, NULL);
    }

    if (entry)
    {
        U32* p = (U32*)(entry + 1);

        for (U32 i = 0; i < (U32)size / 4; i++)
        {
            p[i] = 0xDEADBEEF;
        }

        RwResourcesFreeResEntry(entry);
    }
}

// GC only: the retrace callback. Counts retraces, polls the reset button and then runs the
// callback it replaced.
extern "C" void fn_800167A8(u32 retraceCount)
{
    lbl_8025E978 = retraceCount;
    fn_80016768();

    if (lbl_8025E974)
    {
        lbl_8025E974(retraceCount);
    }
}

// GC only: notes when the reset button is released after being held.
extern "C" void fn_80016768()
{
    S32 state = OSGetResetButtonState();

    if (lbl_8025E96C == 1 && state == 0)
    {
        lbl_8025E970 = 1;
    }

    lbl_8025E96C = state;
}

// GC only: resets the console once the reset button has been released.
extern "C" void fn_80016730()
{
    if (lbl_8025E970)
    {
        lbl_8025E970 = 0;
        fn_80016698(0);
    }
}

// GC only: stops sound, pads and graphics, blacks out the screen and resets. Goes back to the
// system menu when asked to, or when there is no usable game disc.
extern "C" void fn_80016698(S32 reset)
{
    iSndDIEDIEDIE();
    PADRecalibrate(0xF0000000);
    GXAbortFrame();
    GXDrawDone();
    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();

    _tagTRCState state = iTRCGetDiskState();

    if (reset || state == TRC_DiskInvalid || state == TRC_DiskTrayOpen || !iTRCDiskIdentified())
    {
        OSResetSystem(1, 0, TRUE);
    }
    else
    {
        OSResetSystem(0, 0, FALSE);
    }
}

// GC only: polls the reset button, then reports the drive status to the TRC disc state.
extern "C" void fn_80016600()
{
    fn_80016768();
    fn_80016730();

    _tagTRCState state;

    switch (DVDGetDriveStatus())
    {
    case DVD_STATE_NO_DISK:
        state = TRC_DiskNoDisk;
        break;
    case DVD_STATE_WRONG_DISK:
        state = TRC_DiskInvalid;
        break;
    case DVD_STATE_COVER_OPEN:
        state = TRC_DiskTrayOpen;
        break;
    case DVD_STATE_RETRY:
        state = TRC_DiskRetry;
        break;
    case DVD_STATE_FATAL_ERROR:
        state = TRC_DiskFatal;
        break;
    case DVD_STATE_END:
        state = TRC_DiskTrayClosed;
        break;
    default:
        if (DVDCheckDisk())
        {
            state = TRC_DiskTrayClosed;
        }
        else
        {
            state = TRC_DiskNotIdentified;
        }
        break;
    }

    xTRCDisk(state);
}

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

    if (!DolphinInitMemorySystem(&lbl_801FE3D4))
    {
        OSPanic("iSystem.cpp", 0x216, "Unable to initialize memory system.\n");
    }

    xDebugInit();
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
    xDebugExit();
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
static U32 RenderWareInit()
{
    RwEngineOpenParams params;

    if (!RwEngineInit(psGetMemoryFunctions(), 1, 0x428000))
    {
        return 1;
    }

    RwResourcesSetArenaSize(0x428000);

    if (!DolphinInstallFileSystem())
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
// A raster under 8 bits per pixel is refused unless the word at +0xC of its driver extension
// is 14.
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

// Called where a debugger breakpoint would go: a bad free, or a free below the top allocation.
void null_func()
{
    mem_base_alloc += 4;
}

// The retail build prints nothing.
extern "C" void OSReport(const char* msg, ...)
{
}

extern "C" int printf(const char* format, ...)
{
    return 0;
}

// Records a block RenderWare tried to free or grow that belongs to the xMem heap.
extern "C" void mem_null(U32 addr, U32 size)
{
    lbl_8025E97C = addr;
    lbl_8025E980 = size;
}

extern "C" void* malloc(U32 size)
{
    return OSAllocFromHeap(the_heap, size);
}

extern "C" void free(void* ptr)
{
    if (ptr)
    {
        OSFreeToHeap(the_heap, ptr);
    }
}

// RenderWare's blocks carry a 0x20-byte header: the size, then seven guard words, 0x1EE7D00D
// when the block came from the xMem heap and 0xDEADBEEF when it came from the OS heap. xMem
// blocks are not freed here.
static void _rwDolphinHeapFree(void* ptr)
{
    if (ptr)
    {
        if ((U32)ptr < mem_top_alloc)
        {
            null_func();
        }

        U32 guard = ((U32*)ptr)[-7];

        if (guard != 0x1EE7D00D)
        {
            OSFreeToHeap(the_heap, (U8*)ptr - 0x20);
        }
        else
        {
            mem_null((U32)ptr, guard);
        }
    }
}

static void* _rwDolphinHeapAlloc(U32 size)
{
    U32* block;
    void* result;

    if (lbl_8025E984)
    {
        block = (U32*)xMemAlloc(gActiveHeap, size + 0x20);
    }
    else
    {
        block = (U32*)OSAllocFromHeap(the_heap, size + 0x20);
    }

    result = block;

    if (block)
    {
        block[0] = size;

        if (lbl_8025E984)
        {
            result = block + 8;
            block[1] = 0x1EE7D00D;
            block[2] = 0x1EE7D00D;
            block[3] = 0x1EE7D00D;
            block[4] = 0x1EE7D00D;
            block[5] = 0x1EE7D00D;
            block[6] = 0x1EE7D00D;
            block[7] = 0x1EE7D00D;
        }
        else
        {
            result = block + 8;
            block[1] = 0xDEADBEEF;
            block[2] = 0xDEADBEEF;
            block[3] = 0xDEADBEEF;
            block[4] = 0xDEADBEEF;
            block[5] = 0xDEADBEEF;
            block[6] = 0xDEADBEEF;
            block[7] = 0xDEADBEEF;
        }
    }

    return result;
}

static void* _rwDolphinHeapCalloc(U32 numObj, U32 sizeObj)
{
    U32 size = numObj * sizeObj;
    void* mem = _rwDolphinHeapAlloc(size);

    if (mem)
    {
        memset(mem, 0, size);
    }

    return mem;
}

// An xMem block cannot grow here: it is recorded and the request fails.
static void* _rwDolphinHeapRealloc(void* ptr, U32 newSize)
{
    U32 oldSize;
    void* mem;

    if (ptr)
    {
        oldSize = ((U32*)ptr)[-8];
    }
    else
    {
        oldSize = 0;
    }

    if (oldSize && ((U32*)ptr)[-7] == 0x1EE7D00D)
    {
        mem_null((U32)ptr, oldSize);
        return NULL;
    }

    mem = _rwDolphinHeapAlloc(newSize);

    if (mem)
    {
        if (oldSize < newSize)
        {
            memcpy(mem, ptr, oldSize);
        }
        else
        {
            memcpy(mem, ptr, newSize);
        }

        _rwDolphinHeapFree(ptr);
    }

    return mem;
}

S32 DolphinInitMemorySystem(RwMemoryFunctions* memoryFuncs)
{
    memoryFuncs->rwmalloc = _rwDolphinHeapAlloc;
    memoryFuncs->rwcalloc = _rwDolphinHeapCalloc;
    memoryFuncs->rwrealloc = _rwDolphinHeapRealloc;
    memoryFuncs->rwfree = _rwDolphinHeapFree;

    return 1;
}

// Only the read modes are supported by dlFopen: "r" and "rb" give 1.
static S32 dlAccessToMode(const char* mode)
{
    char c;
    S32 ret = 0;

    if (mode == NULL)
    {
        return 0;
    }

    c = mode[0];

    if (c == 'r' && (mode[1] == '\0' || (mode[1] == 'b' && mode[2] == '\0')))
    {
        ret = 1;
    }
    else if (c == 'w' && (mode[1] == '\0' || (mode[1] == 'b' && mode[2] == '\0')))
    {
        ret = 0xe;
    }
    else if (c == 'a' && (mode[1] == '\0' || (mode[1] == 'b' && mode[2] == '\0')))
    {
        ret = 6;
    }
    else if (c == 'r' && mode[1] == '+' && (mode[2] == '\0' || (mode[2] == 'b' && mode[3] == '\0')))
    {
        ret = 3;
    }
    else if (c == 'w' && mode[1] == '+' && (mode[2] == '\0' || (mode[2] == 'b' && mode[3] == '\0')))
    {
        ret = 0xf;
    }
    else if (c == 'a' && mode[1] == '+' && (mode[2] == '\0' || (mode[2] == 'b' && mode[3] == '\0')))
    {
        ret = 7;
    }

    return ret;
}

static void* dlFopen(const char* name, const char* access)
{
    U32 mode = dlAccessToMode(access);

    if (mode == 0)
    {
        return NULL;
    }

    dlFile* fp = (dlFile*)RWSRCGLOBAL(memoryAlloc)(FSGLOBALS()->fileFreeList);

    if (fp == NULL)
    {
        return NULL;
    }

    fp->mode = 0;

    if (mode == 1)
    {
        fp->mode = 1;
    }

    if (fp->mode == 0)
    {
        RWSRCGLOBAL(memoryFree)(FSGLOBALS()->fileFreeList, fp);
        return NULL;
    }

    if (DVDOpen((char*)name, &fp->fileInfo) == 0)
    {
        RWSRCGLOBAL(memoryFree)(FSGLOBALS()->fileFreeList, fp);
        return NULL;
    }

    fp->length = fp->fileInfo.length;
    fp->pos = 0;
    fp->bufpos = DLFILE_BUF_SIZE;
    lbl_8025E988++;
    return fp;
}

static S32 dlFclose(void* fptr)
{
    dlFile* fp = (dlFile*)fptr;

    if (fp != NULL && lbl_8025E988 != 0 && DVDClose(&fp->fileInfo) != 0)
    {
        RWSRCGLOBAL(memoryFree)(FSGLOBALS()->fileFreeList, fp);
        lbl_8025E988--;
        return 0;
    }

    return -1;
}

static S32 dlFexist(const char* name)
{
    void* fp = RwOsGetFileInterface()->rwfopen(name, "r");

    if (fp != NULL)
    {
        RwOsGetFileInterface()->rwfclose(fp);
        return TRUE;
    }

    return FALSE;
}

// Serves what it can from the read buffer, then reads straight into the caller's memory when the
// rest is large and 32-byte aligned, and otherwise through the buffer in 0x2800-byte reads.
static U32 dlFread(void* addr, U32 size, U32 count, void* fptr)
{
    S32 pos2;
    S32 posTmp;
    S32 bytesRead = 0;
    S32 readSize;
    U32 buffered;
    U32 uVar1;
    dlFile* fp = (dlFile*)fptr;
    U32 numBytesToRead = size * count;

    if ((U32)(fp->pos + numBytesToRead) > fp->length)
    {
        numBytesToRead = fp->length - fp->pos;
    }

    if (fp->bufpos < DLFILE_BUF_SIZE)
    {
        if (numBytesToRead > bytesRead)
        {
            buffered = DLFILE_BUF_SIZE - fp->bufpos;

            if (numBytesToRead < buffered)
            {
                buffered = numBytesToRead;
            }

            bytesRead = buffered;
            memcpy(addr, fp->readBuffer + fp->bufpos, buffered);
            addr = (char*)addr + buffered;
            fp->bufpos += buffered;
            fp->pos += buffered;
        }
    }

    uVar1 = numBytesToRead - bytesRead;

    if (uVar1 != 0)
    {
        if (uVar1 >= DLFILE_BUF_SIZE)
        {
            if (!((U32)addr & 0x1f) && !(uVar1 & 0x1f))
            {
                uVar1 = DVDReadPrio(&fp->fileInfo, addr, uVar1, fp->pos, 2);

                if ((S32)uVar1 < 0)
                {
                    uVar1 = 0;
                }
            }
            else
            {
                numBytesToRead -= bytesRead;
                uVar1 = 0;
                posTmp = fp->pos;
                S32 loopCount = (S32)numBytesToRead / DLFILE_BUF_SIZE + 1;

                while (loopCount-- != 0)
                {
                    pos2 = fp->pos;
                    readSize = DLFILE_BUF_SIZE;
                    pos2 += uVar1;

                    if ((S32)(fp->length - pos2) <= DLFILE_BUF_SIZE)
                    {
                        readSize = fp->length - pos2;
                    }

                    if (DVDReadPrio(&fp->fileInfo, fp->readBuffer, ((U32)readSize + 0x1f) & ~0x1f,
                                    posTmp, 2) == -1)
                    {
                        return bytesRead + uVar1;
                    }

                    if ((S32)numBytesToRead >= readSize)
                    {
                        memcpy(addr, fp->readBuffer, readSize);
                        numBytesToRead -= readSize;
                        uVar1 += readSize;
                        posTmp += readSize;
                        addr = (char*)addr + readSize;
                    }
                    else
                    {
                        memcpy(addr, fp->readBuffer, numBytesToRead);
                        fp->bufpos = numBytesToRead;
                        uVar1 += numBytesToRead;
                        posTmp += numBytesToRead;
                    }
                }
            }
        }
        else
        {
            buffered = ((U32)(fp->length - fp->pos) + 0x1f) & ~0x1f;

            if ((S32)buffered > DLFILE_BUF_SIZE)
            {
                buffered = DLFILE_BUF_SIZE;
            }

            if (DVDReadPrio(&fp->fileInfo, fp->readBuffer, buffered, fp->pos, 2) == -1)
            {
                return bytesRead;
            }

            memcpy(addr, fp->readBuffer, uVar1);
            fp->bufpos = uVar1;
        }

        bytesRead += uVar1;
        fp->pos += uVar1;
    }

    return bytesRead / size;
}

static U32 dlFwrite(const void* addr, U32 size, U32 count, void* fptr)
{
    return 0;
}

// Moves within the read buffer when it can; otherwise rereads the 0x2800-byte block that holds
// the new position.
static S32 dlFseek(void* fptr, S32 offset, S32 origin)
{
    dlFile* fp = (dlFile*)fptr;
    U32 oldFPos = fp->pos;
    U32 bufStart;

    switch (origin)
    {
    case 1:
        fp->pos = oldFPos + offset;
        if ((S32)(fp->bufpos + offset) >= 0 && fp->bufpos + offset <= DLFILE_BUF_SIZE &&
            fp->pos <= fp->length)
        {
            fp->bufpos += offset;
            return 0;
        }
        break;
    case 2:
        fp->pos = fp->length + offset;
        offset = fp->pos - oldFPos;
        if ((S32)(fp->bufpos + offset) >= 0 && fp->bufpos + offset <= DLFILE_BUF_SIZE &&
            fp->pos <= fp->length)
        {
            fp->bufpos += offset;
            return 0;
        }
        break;
    case 0:
        fp->pos = offset;
        offset = offset - oldFPos;
        if ((S32)(fp->bufpos + offset) >= 0 && fp->bufpos + offset <= DLFILE_BUF_SIZE &&
            fp->pos <= fp->length)
        {
            fp->bufpos += offset;
            return 0;
        }
        break;
    default:
        return -1;
    }

    if (fp->pos > fp->length)
    {
        fp->pos = oldFPos;
        return -1;
    }

    bufStart = (fp->pos / DLFILE_BUF_SIZE) * DLFILE_BUF_SIZE;

    if (DVDReadPrio(&fp->fileInfo, fp->readBuffer,
                    (fp->length - bufStart <= DLFILE_BUF_SIZE) ?
                        (fp->length - bufStart + 31) & ~31 :
                        DLFILE_BUF_SIZE,
                    bufStart, 2) == -1)
    {
        fp->pos = oldFPos;
        return -1;
    }

    fp->bufpos = fp->pos - bufStart;
    return 0;
}

// Reads a line, folding "\r\n" to "\n", and seeks back over what it read past the line.
static char* dlFgets(char* buffer, S32 maxLen, void* fptr)
{
    dlFile* fp = (dlFile*)fptr;
    S32 i = 0;
    S32 numBytesRead = dlFread(buffer, 1, maxLen - 1, fp);

    if (numBytesRead == 0)
    {
        return NULL;
    }

    while (i < numBytesRead)
    {
        if (buffer[i] == '\n')
        {
            i++;
            buffer[i] = '\0';
            i -= numBytesRead;
            dlFseek(fp, i, 1);
            return buffer;
        }

        if (buffer[i] == '\r')
        {
            if (i < numBytesRead - 1 && buffer[i + 1] == '\n')
            {
                memcpy(&buffer[i], &buffer[i + 1], numBytesRead - i - 1);
                numBytesRead--;
            }
            else
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }

    if (numBytesRead < maxLen && fp->pos == fp->length)
    {
        buffer[numBytesRead] = '\0';
    }

    return buffer;
}

static S32 dlFputs(const char* buffer, void* fptr)
{
    return -1;
}

static S32 dlFeof(void* fptr)
{
    dlFile* fp = (dlFile*)fptr;
    return fp->pos >= fp->length;
}

static S32 dlFflush(void* fptr)
{
    return 0;
}

static S32 dlFtell(void* fptr)
{
    dlFile* fp = (dlFile*)fptr;
    return fp->pos;
}

// The file system plugin's constructor: makes the free list of open files, keeps RenderWare's
// own file functions and installs the disc ones.
static void* _rwDolphinFSOpen(void* instance, S32 offset, S32 size)
{
    lbl_8025E98C.globalsOffset = offset;
    FSGLOBALS()->fileFreeList = RwFreeListCreate(sizeof(dlFile), 5, 0x20);

    if (FSGLOBALS()->fileFreeList == NULL)
    {
        return NULL;
    }

    RwFileFunctions* funcs = RwOsGetFileInterface();
    FSGLOBALS()->oldFuncs = *funcs;

    funcs->rwfexist = dlFexist;
    funcs->rwfopen = dlFopen;
    funcs->rwfclose = dlFclose;
    funcs->rwfread = dlFread;
    funcs->rwfwrite = dlFwrite;
    funcs->rwfgets = dlFgets;
    funcs->rwfputs = dlFputs;
    funcs->rwfeof = dlFeof;
    funcs->rwfseek = dlFseek;
    funcs->rwfflush = dlFflush;
    funcs->rwftell = dlFtell;

    lbl_8025E98C.numInstances++;
    return instance;
}

static void* _rwDolphinFSClose(void* instance, S32 offset, S32 size)
{
    *RwOsGetFileInterface() = FSGLOBALS()->oldFuncs;
    RwFreeListDestroy(FSGLOBALS()->fileFreeList);
    lbl_8025E98C.numInstances--;
    return instance;
}

static S32 DolphinInstallFileSystem()
{
    DVDInit();
    U32 plugin = RwEngineRegisterPlugin(sizeof(dlFSGlobals), lbl_8025E3F8, _rwDolphinFSOpen,
                                        _rwDolphinFSClose);
    return plugin >> 31 ^ 1;
}
