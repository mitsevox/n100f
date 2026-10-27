#include <dolphin/exi.h>
#include <dolphin/os.h>

#define OS_SOUND_MODE_MONO 0
#define OS_SOUND_MODE_STEREO 1

#define RTC_CMD_READ 0x20000000
#define RTC_CMD_WRITE 0xA0000000

#define RTC_SRAM_ADDR 0x00000100
#define RTC_SRAM_SIZE 64

#define RTC_CHAN 0
#define RTC_DEVICE 1
#define RTC_FREQUENCY 3

#define GET_SOUNDMODE(flags) ((flags) & (1 << 2))
#define CLR_SOUNDMODE(flags) ((flags) & (~(1 << 2)))
#define SET_SOUNDMODE(mode) (((mode) & 1) << 2)

typedef struct SramControlBlock {
    u8 sram[RTC_SRAM_SIZE];
    u32 offset;
    BOOL enabled;
    BOOL locked;
    BOOL sync;
    void (*callback)(void);
} SramControlBlock;

static SramControlBlock Scb __attribute__((aligned(32)));

static BOOL WriteSram(void* buffer, u32 offset, u32 size);

static inline BOOL ReadSram(void* buffer)
{
    u32 cmd;
    BOOL err;

    DCInvalidateRange(buffer, RTC_SRAM_SIZE);

    if (!EXILock(RTC_CHAN, RTC_DEVICE, NULL)) {
        return FALSE;
    }

    if (!EXISelect(RTC_CHAN, RTC_DEVICE, RTC_FREQUENCY)) {
        EXIUnlock(RTC_CHAN);
        return FALSE;
    }

    cmd = RTC_CMD_READ | RTC_SRAM_ADDR;
    err = !EXIImm(RTC_CHAN, &cmd, sizeof(u32), EXI_WRITE, NULL);
    err |= !EXISync(RTC_CHAN);
    err |= !EXIDma(RTC_CHAN, buffer, RTC_SRAM_SIZE, EXI_READ, NULL);
    err |= !EXISync(RTC_CHAN);
    err |= !EXIDeselect(RTC_CHAN);
    EXIUnlock(RTC_CHAN);

    return !err;
}

static void WriteSramCallback(s32 chan, OSContext* context)
{
    ASSERTLINE(258, !Scb.locked);
    Scb.sync = WriteSram(Scb.sram + Scb.offset, Scb.offset, RTC_SRAM_SIZE - Scb.offset);
    ASSERTLINE(264, Scb.sync);

    if (Scb.sync) {
        Scb.offset = RTC_SRAM_SIZE;
    }
}

static BOOL WriteSram(void* buffer, u32 offset, u32 size)
{
    u32 cmd;
    BOOL err;

    if (EXILock(RTC_CHAN, 1, WriteSramCallback) == FALSE) {
        return FALSE;
    }

    if (EXISelect(RTC_CHAN, RTC_DEVICE, RTC_FREQUENCY) == FALSE) {
        EXIUnlock(RTC_CHAN);
        return FALSE;
    }

    offset *= RTC_SRAM_SIZE;
    cmd = offset + RTC_SRAM_ADDR | RTC_CMD_WRITE;
    err = !EXIImm(RTC_CHAN, &cmd, sizeof(u32), EXI_WRITE, NULL);
    err |= !EXISync(RTC_CHAN);
    err |= !EXIImmEx(RTC_CHAN, buffer, size, EXI_WRITE);
    err |= !EXIDeselect(RTC_CHAN);
    EXIUnlock(RTC_CHAN);
    return !err;
}

void __OSInitSram(void)
{
    Scb.enabled = FALSE;
    Scb.locked = FALSE;
    Scb.sync = ReadSram(Scb.sram);
    ASSERTLINE(318, Scb.sync);
    Scb.offset = RTC_SRAM_SIZE;
}

static inline void* LockSram(u32 offset)
{
    BOOL enabled = OSDisableInterrupts();

    ASSERTLINE(341, !Scb.locked);
    if (Scb.locked != FALSE) {
        OSRestoreInterrupts(enabled);
        return NULL;
    }

    Scb.enabled = enabled;
    Scb.locked = TRUE;
    return Scb.sram + offset;
}

OSSram* __OSLockSram(void)
{
    return (OSSram*)LockSram(0);
}

OSSramEx* __OSLockSramEx(void)
{
    return (OSSramEx*)LockSram(sizeof(OSSram));
}

static BOOL UnlockSram(BOOL commit, u32 offset)
{
    u16* p;

    ASSERTLINE(375, Scb.locked);
    if (commit) {
        if (offset == 0) {
            OSSram* sram = (OSSram*)Scb.sram;
            if ((sram->flags & 3u) > 2u) {
                sram->flags &= ~3u;
            }

            sram->checkSum = sram->checkSumInv = 0;
            for (p = (u16*)&sram->counterBias; p < (u16*)(Scb.sram + sizeof(OSSram)); p++) {
                sram->checkSum += *p;
                sram->checkSumInv += ~*p;
            }
        }

        if (offset < Scb.offset) {
            Scb.offset = offset;
        }

        Scb.sync = WriteSram(Scb.sram + Scb.offset, Scb.offset, RTC_SRAM_SIZE - Scb.offset);
        if (Scb.sync) {
            Scb.offset = RTC_SRAM_SIZE;
        }
    }

    Scb.locked = FALSE;
    OSRestoreInterrupts(Scb.enabled);
    return Scb.sync;
}

BOOL __OSUnlockSram(BOOL commit)
{
    return UnlockSram(commit, 0);
}

BOOL __OSUnlockSramEx(BOOL commit)
{
    return UnlockSram(commit, sizeof(OSSram));
}

BOOL __OSSyncSram(void)
{
    return Scb.sync;
}

u32 OSGetSoundMode(void)
{
    OSSram* sram = __OSLockSram();
    u32 mode = GET_SOUNDMODE(sram->flags) ? OS_SOUND_MODE_STEREO : OS_SOUND_MODE_MONO;
    __OSUnlockSram(FALSE);
    return mode;
}

void OSSetSoundMode(u32 mode)
{
    u32 flag;
    OSSram* sram;

    ASSERTLINE(617, mode == OS_SOUND_MODE_MONO || mode == OS_SOUND_MODE_STEREO);
    flag = SET_SOUNDMODE(mode);
    sram = __OSLockSram();
    if (flag == GET_SOUNDMODE(sram->flags)) {
        __OSUnlockSram(FALSE);
    } else {
        sram->flags = CLR_SOUNDMODE(sram->flags);
        sram->flags |= flag;
        __OSUnlockSram(TRUE);
    }
}

u16 OSGetWirelessID(s32 chan)
{
    OSSramEx* sramEx = __OSLockSramEx();
    u16 id = sramEx->wirelessPadID[chan];
    __OSUnlockSramEx(FALSE);
    return id;
}

void OSSetWirelessID(s32 chan, u16 id)
{
    OSSramEx* sramEx = __OSLockSramEx();
    if (sramEx->wirelessPadID[chan] != id) {
        sramEx->wirelessPadID[chan] = id;
        __OSUnlockSramEx(TRUE);
    } else {
        __OSUnlockSramEx(FALSE);
    }
}
