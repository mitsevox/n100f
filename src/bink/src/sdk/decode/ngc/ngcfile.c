#include <dolphin/os.h>
#include <dolphin/dvd.h>
#include "bink/binkio.h"
#include "bink/binkngc.h"

void* memmove(void* dest, const void* src, U32 n);

typedef struct NGCFILE {
    DVDFileInfo fi;
    volatile U32 filepos;
    U32 skip;
    U32 bufpos;
    U8* bufhead;
    volatile U32 bufempty;
    U8* buf;
    U8* bufend;
    U8* buftail;
    U32 dontclose;
    U32 startofs;
    U32 filesize;
    U32 simulate;
    S32 simtime;
    volatile U32 cancelling;
    U32 wasworking;
    U32 readstart;
    BinkIO* io;
} NGCFILE;

#define N ((NGCFILE*)io->priv)

static void intelendian(void* p, U32 bytes)
{
    U32* d = (U32*)p;
    U32 n = (bytes + 3) >> 2;

    while (n--)
    {
        U32 v;
        __asm__("lwbrx %0,%1,%2" : "=r"(v) : "b"(0), "r"(d));
        *d++ = v;
    }
}

static U32 radreadngc(DVDFileInfo* fi, U32 offset, void* dest, U32 size)
{
    U8 tmp[1024 + 31];
    U8* buf = (U8*)(((U32)tmp + 31) & ~31);
    U32 avail = ((U32)tmp + sizeof(tmp) - (U32)buf) & ~31;
    U8* d = (U8*)dest;
    U32 total = 0;

    while (size)
    {
        U32 amt;

        if (size >= avail)
            amt = avail;
        else
            amt = (size + 31) & ~31;

        DVDReadAsyncPrio(fi, buf, amt, offset, 0, 0);

        for (;;)
        {
            switch (DVDGetCommandBlockStatus(&fi->cb))
            {
            case 0:
                goto done;
            case -1:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                return 0;
            }
        }
    done:
        if (amt > size)
            amt = size;
        memcpy(d, buf, amt);
        d += amt;
        total += amt;
        offset += amt;
        size -= amt;
    }

    return total;
}

static U32 BinkFileReadHeader(BinkIO* io, S32 offset, void* dest, U32 size)
{
    U32 amt;

    if (offset != -1)
    {
        if (N->filepos != offset)
            N->filepos = offset;
    }

    amt = radreadngc(&N->fi, N->filepos + N->startofs, dest, size);
    N->filepos += amt;
    N->bufpos = N->filepos;

    io->cur_size = (N->filesize - N->bufpos < io->buf_size) ? N->filesize - N->bufpos : io->buf_size;

    intelendian(dest, amt);
    return amt;
}

static void dosimulate(BinkIO* io, U32 amt, U32 start)
{
    U32 t = mult64anddiv(amt, 1000, N->simulate);
    U32 now = RADTimerRead();

    N->simtime += t - (now - start);
    while (N->simtime > 0)
    {
        do
        {
            t = RADTimerRead();
        } while ((S32)(t - now) < N->simtime);
        N->simtime -= t - now;
        now = t;
    }
}

static void ReadKickoff(BinkIO* io);

static void DVDReadCallback(s32 result, DVDFileInfo* fi)
{
    BinkIO* io = ((NGCFILE*)fi)->io;

    if (N->cancelling)
        return;

    if (result != 4096 && (S32)(N->filesize - N->filepos + N->skip) != result)
    {
        io->reading = 0;
        io->error = 1;
        return;
    }

    if (N->skip)
    {
        memmove(N->buftail, N->buftail + N->skip, N->filesize - N->filepos);
        result -= N->skip;
        N->skip = 0;
    }

    io->bytes_read += result;
    N->buftail += result;
    N->filepos += result;
    if (N->buftail >= N->bufend)
        N->buftail = N->buf;
    N->bufempty -= result;
    io->cur_used += result;
    if (io->cur_used > io->buf_peak)
        io->buf_peak = io->cur_used;

    if (N->simulate)
        dosimulate(io, result, N->readstart);

    N->readstart = RADTimerRead() - N->readstart;
    io->total_time += N->readstart;
    if (N->wasworking || io->busy)
        io->bg_time += N->readstart;
    else
        io->idle_time += N->readstart;

    io->reading = 0;
    ReadKickoff(io);
}

static void ReadKickoff(BinkIO* io)
{
    s32 status = DVDGetCommandBlockStatus(&N->fi.cb);
    U32 left = N->filesize - N->filepos;

    switch (status)
    {
    case -1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 11:
        io->error = 1;
        return;
    }

    if (N->cancelling)
        return;

    if (status != 0 && status != 10)
        return;

    if (N->bufempty >= 4096 && left)
    {
        N->wasworking = io->busy;
        N->readstart = RADTimerRead();
        io->reading = 1;

        if (left > 4096)
            left = 4096;
        else if (left & 31)
        {
            N->skip = -left & 31;
            left += N->skip;
        }

        DVDReadAsyncPrio(&N->fi, N->buftail, left, N->filepos - N->skip + N->startofs, DVDReadCallback, 0);
    }
    else
    {
        io->cur_size = io->cur_used;
    }
}

static U32 BinkFileIdle(BinkIO* io)
{
    if (io->error)
        return 0;

    switch (DVDGetCommandBlockStatus(&N->fi.cb))
    {
    case 0:
        ReadKickoff(io);
        break;
    case -1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 11:
        io->error = 1;
        break;
    case 1:
    case 2:
    case 10:
        break;
    }

    return io->reading;
}

static void CancelReadRequests(BinkIO* io)
{
    s32 status;

    N->cancelling = 1;
    DVDCancel(&N->fi.cb);
    do
    {
        status = DVDGetCommandBlockStatus(&N->fi.cb);
    } while (status == 1 || status == 2);
    N->cancelling = 0;
    io->reading = 0;
}

static U32 BinkFileReadFrame(BinkIO* io, U32 frame, S32 offset, void* dest, U32 size)
{
    void* orig = dest;
    U32 amt = 0;
    U32 start, t, avail, chunk, x;
    BOOL enabled;

    if (io->error)
        return 0;

    start = RADTimerRead();

    if (offset != -1 && N->bufpos != offset)
    {
        if ((U32)offset > N->bufpos && (U32)offset <= N->filepos)
        {
            U8* h;

            enabled = OSDisableInterrupts();
            x = offset - N->bufpos;
            N->bufempty += x;
            N->bufpos = offset;
            h = N->bufhead + x;
            N->bufhead = h;
            io->cur_used -= x;
            if (h > N->bufend)
                N->bufhead = h - io->buf_size;
            OSRestoreInterrupts(enabled);
        }
        else
        {
            CancelReadRequests(io);
            enabled = OSDisableInterrupts();
            N->filepos = offset;
            N->bufpos = offset;
            N->bufempty = io->buf_size;
            io->cur_used = 0;
            N->bufhead = N->buf;
            N->buftail = N->buf;
            OSRestoreInterrupts(enabled);
        }
    }

    if (N->buf == 0)
    {
        t = RADTimerRead();
        x = radreadngc(&N->fi, N->filepos, dest, size);
        if (x < size)
            io->error = 1;
        N->filepos += x;
        N->bufpos += x;
        io->bytes_read += x;
        amt = x;
        if (N->simulate)
            dosimulate(io, amt, t);
        x = RADTimerRead();
        io->total_time += x - t;
        io->fg_time += x - start;
    }
    else
    {
        while (size && !io->error)
        {
            ReadKickoff(io);
            avail = io->cur_used;
            if (avail == 0)
                continue;

            if (avail > size)
                avail = size;
            N->bufpos += avail;
            size -= avail;
            amt += avail;

            chunk = N->bufend - N->bufhead;
            if (chunk <= avail)
            {
                memcpy(dest, N->bufhead, chunk);
                dest = (U8*)dest + chunk;
                avail -= chunk;
                N->bufhead = N->buf;
                enabled = OSDisableInterrupts();
                io->cur_used -= chunk;
                N->bufempty += chunk;
                OSRestoreInterrupts(enabled);
            }

            if (avail)
            {
                memcpy(dest, N->bufhead, avail);
                dest = (U8*)dest + avail;
                N->bufhead += avail;
                enabled = OSDisableInterrupts();
                io->cur_used -= avail;
                N->bufempty += avail;
                OSRestoreInterrupts(enabled);
            }
        }
        io->fg_time += RADTimerRead() - start;
    }

    x = N->filesize - N->bufpos;
    io->cur_size = (x < io->buf_size) ? x : io->buf_size;
    if (io->cur_used + 4096 > io->cur_size)
        io->cur_size = io->cur_used;

    intelendian(orig, amt);
    return amt;
}

static U32 BinkFileGetBufferSize(BinkIO* io, U32 size)
{
    return (size + 4095) & ~4095;
}

static void BinkFileSetInfo(BinkIO* io, void* buf, U32 size, U32 filesize, U32 simulate)
{
    BOOL enabled = OSDisableInterrupts();

    N->bufempty = size & ~4095;
    io->buf_size = size & ~4095;
    N->filesize = filesize;
    N->simulate = simulate;
    N->io = io;
    N->buf = (U8*)buf;
    N->bufhead = (U8*)buf;
    N->buftail = (U8*)buf;
    io->cur_used = 0;
    N->bufend = (U8*)buf + (size & ~4095);
    OSRestoreInterrupts(enabled);
}

static void BinkFileClose(BinkIO* io)
{
    CancelReadRequests(io);
    if (!N->dontclose)
        DVDClose(&N->fi);
}

S32 BinkFileOpen(BinkIO* io, const char* name, U32 flags)
{
    memset(io, 0, sizeof(BinkIO));

    if (flags & 0x800000)
    {
        N->fi = *(DVDFileInfo*)name;
        N->dontclose = 1;
        N->startofs = (U32)N->fi.cb.userData;
    }
    else
    {
        s32 entry = DVDConvertPathToEntrynum((char*)name);
        if (entry == -1 || !DVDFastOpen(entry, &N->fi))
            return 0;
    }

    io->read_header = BinkFileReadHeader;
    io->read_frame = BinkFileReadFrame;
    io->get_buffer_size = BinkFileGetBufferSize;
    io->set_info = BinkFileSetInfo;
    io->idle = BinkFileIdle;
    io->close = BinkFileClose;
    return 1;
}
