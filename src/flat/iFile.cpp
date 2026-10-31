#include "iFile.h"

#include <dolphin/os.h>
#include <string.h>

extern "C" {
void fn_80016600();
void fn_800443AC();
}

struct file_queue_entry
{
    tag_xFile* file;
    void* buf;
    U32 size;
    U32 offset;
    IFILE_READSECTOR_STATUS stat;
    void (*callback)(tag_xFile* file);
    U32 asynckey;
};

static U32 tbuffer[1024 + 8];
file_queue_entry file_queue[4];
static U32* buffer32;
static volatile U32 iFileSyncAsyncReadActive;

static inline void iFileFullPath(const char* relname, char* fullname)
{
    strcpy(fullname, relname);
}

// A 32-byte aligned scratch buffer for the platform layer.
void iFileInit()
{
    buffer32 = (U32*)OSRoundUp32B(tbuffer);
}

void iFileExit()
{
}

// Reads a whole file in one blocking call, into a new 32-byte aligned block when no buffer is given.
// The size returned is rounded up to 32 bytes.
U32* iFileLoad(char* name, U32* buffer, U32* size)
{
    char path[128];
    DVDFileInfo fileInfo;
    U32 alignedSize;

    iFileFullPath(name, path);

    S32 entrynum = DVDConvertPathToEntrynum(path);
    if (entrynum == -1 || !DVDFastOpen(entrynum, &fileInfo))
    {
        return NULL;
    }

    U32 fileSize = fileInfo.length;
    if (buffer == NULL)
    {
        buffer = (U32*)OSAllocFromHeap(__OSCurrHeap, OSRoundUp32B(fileSize));
    }

    alignedSize = OSRoundUp32B(fileSize);
    DVDReadPrio(&fileInfo, buffer, alignedSize, 0, 2);
    DVDClose(&fileInfo);

    if (size)
    {
        *size = alignedSize;
    }

    return buffer;
}

U32 iFileOpen(const char* name, S32 flags, tag_xFile* file)
{
    tag_iFile* ps = &file->ps;

    if (flags & IFILE_OPEN_ABSPATH)
    {
        strcpy(ps->path, name);
    }
    else
    {
        iFileFullPath(name, ps->path);
    }

    ps->entrynum = DVDConvertPathToEntrynum(ps->path);

    if (ps->entrynum == -1)
    {
        return 1;
    }

    if (!DVDFastOpen(ps->entrynum, &ps->fileInfo))
    {
        ps->entrynum = -1;
        return 1;
    }

    ps->unkC4 = 0;
    ps->flags = 0x1;

    iFileSeek(file, 0, IFILE_SEEK_SET);

    return 0;
}

S32 iFileSeek(tag_xFile* file, S32 offset, S32 whence)
{
    tag_iFile* ps = &file->ps;
    S32 new_pos;

    switch (whence)
    {
    case IFILE_SEEK_SET:
        new_pos = offset;
        break;
    case IFILE_SEEK_CUR:
        if (DVDGetCommandBlockStatus(&ps->fileInfo.cb) == DVD_STATE_BUSY)
        {
            return -1;
        }
        new_pos = offset + ps->offset;
        break;
    case IFILE_SEEK_END:
        new_pos = ps->fileInfo.length - offset;
        if (new_pos < 0)
        {
            new_pos = 0;
        }
        break;
    default:
        new_pos = offset;
        break;
    }

    ps->offset = new_pos;

    return ps->offset;
}

static void ifilereadCB(tag_xFile* file)
{
    iFileSyncAsyncReadActive = 0;
}

// Blocking read: queues an asynchronous read and services the disc until it finishes.
U32 iFileRead(tag_xFile* file, void* buf, U32 size)
{
    iFileSyncAsyncReadActive = 1;

    iFileReadAsync(file, buf, size, ifilereadCB);

    while (iFileSyncAsyncReadActive)
    {
        fn_80016600();
        fn_800443AC();
    }

    return size;
}

// Reads the rest of a queued request in 32 KB steps; the first step was only the first 32 bytes.
static void async_cb(s32 result, DVDFileInfo* fileInfo)
{
    file_queue_entry* entry = &file_queue[(S32)fileInfo->cb.userData];
    s32 amount = -1;

    switch (result)
    {
    case -1:
    case -3:
        break;
    case -2:
    default:
        if (result >= 0)
        {
            amount = result;
        }
        break;
    }

    if (amount < 0)
    {
        entry->stat = IFILE_RDSTAT_FAIL;

        if (entry->callback)
        {
            entry->callback(entry->file);
        }
    }
    else if (amount + entry->offset >= entry->size)
    {
        entry->stat = IFILE_RDSTAT_DONE;

        if (entry->callback)
        {
            entry->callback(entry->file);
        }
    }
    else
    {
        entry->offset += amount;
        entry->stat = IFILE_RDSTAT_INPROG;

        s32 length;
        if (entry->size - entry->offset < 0x8000)
        {
            length = (entry->size - entry->offset + 3) & ~3;
        }
        else
        {
            length = 0x8000;
        }

        DVDReadAsyncPrio(&entry->file->ps.fileInfo, (void*)((U32)entry->buf + entry->offset),
                         length, entry->file->ps.offset + entry->offset, async_cb, 2);
    }
}

// Returns a key for iFileReadAsyncStatus: the request count times four plus the queue slot, or -1
// when all four slots are busy.
S32 iFileReadAsync(tag_xFile* file, void* buf, U32 aSize, void (*callback)(tag_xFile*))
{
    static S32 fopcount = 1;
    tag_iFile* ps = &file->ps;
    S32 i;

    for (i = 0; i < 4; i++)
    {
        if (file_queue[i].stat != IFILE_RDSTAT_QUEUED && file_queue[i].stat != IFILE_RDSTAT_INPROG)
        {
            S32 id = fopcount++ << 2;

            file_queue[i].file = file;
            file_queue[i].buf = buf;
            file_queue[i].size = aSize;
            file_queue[i].offset = 0;
            file_queue[i].stat = IFILE_RDSTAT_QUEUED;
            file_queue[i].callback = callback;
            file_queue[i].asynckey = id + i;

            aSize = (aSize < 32) ? (aSize + 3) & ~3 : 32;

            ps->fileInfo.cb.userData = (void*)i;

            DVDReadAsyncPrio(&ps->fileInfo, buf, aSize, ps->offset, async_cb, 2);

            return id + i;
        }
    }

    return -1;
}

IFILE_READSECTOR_STATUS iFileReadAsyncStatus(S32 key, S32* amtToFar)
{
    if (key != file_queue[key & 0x3].asynckey)
    {
        return IFILE_RDSTAT_EXPIRED;
    }

    if (amtToFar)
    {
        *amtToFar = file_queue[key & 0x3].offset;
    }

    return file_queue[key & 0x3].stat;
}

// The disc is read-only, so nothing is ever written.
U32 iFileWrite(tag_xFile* file, void* buf, U32 size)
{
    return 0;
}

// The file is closed twice; only the second result counts.
U32 iFileClose(tag_xFile* file)
{
    tag_iFile* ps = &file->ps;
    S32 ret;

    ret = DVDClose(&file->ps.fileInfo);
    ret = DVDClose(&file->ps.fileInfo);

    if (!ret)
    {
        return 1;
    }

    ps->flags = 0;
    return 0;
}

U32 iFileGetSize(tag_xFile* file)
{
    return file->ps.fileInfo.length;
}

U32 iFileFind(const char* name, tag_xFile* file)
{
    return iFileOpen(name, 0, file);
}

void iFileGetInfo(tag_xFile* file, U32* addr, U32* length)
{
    if (addr)
    {
        *addr = file->ps.fileInfo.startAddr;
    }

    if (length)
    {
        *length = file->ps.fileInfo.length;
    }
}
