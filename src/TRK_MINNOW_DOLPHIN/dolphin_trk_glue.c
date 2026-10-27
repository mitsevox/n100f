#include "TRK_MINNOW_DOLPHIN/trk.h"
#include "dolphin/os.h"
#include "dolphin/OdemuExi2/DebuggerDriver.h"
#include "dolphin/amcstubs/AmcExi2Stubs.h"

#define BUFF_LEN 4362

u8 gWriteBuf[BUFF_LEN];
u8 gReadBuf[BUFF_LEN];
s32 _MetroTRK_Has_Framing;
s32 gReadCount;
s32 gReadPos;
s32 gWritePos;

DBCommTable gDBCommTable = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };

asm void TRKLoadContext(OSContext* ctx, u32 exception) {
    nofralloc
    lwz r0, OSContext.gpr[0](r3)
    lwz r1, OSContext.gpr[1](r3)
    lwz r2, OSContext.gpr[2](r3)
    lhz r5, OSContext.state(r3)
    rlwinm. r6, r5, 0, 30, 30
    beq restore_nonvolatile
    rlwinm r5, r5, 0, 31, 29
    sth r5, OSContext.state(r3)
    lmw r5, OSContext.gpr[5](r3)
    b restore_special
restore_nonvolatile:
    lmw r13, OSContext.gpr[13](r3)
restore_special:
    mr r31, r3
    mr r3, r4
    lwz r4, OSContext.cr(r31)
    mtcrf 0xff, r4
    lwz r4, OSContext.lr(r31)
    mtlr r4
    lwz r4, OSContext.ctr(r31)
    mtctr r4
    lwz r4, OSContext.xer(r31)
    mtxer r4
    mfmsr r4
    rlwinm r4, r4, 0, 17, 15
    rlwinm r4, r4, 0, 31, 29
    mtmsr r4
    mtsprg 1, r2
    lwz r4, OSContext.gpr[3](r31)
    mtsprg 2, r4
    lwz r4, OSContext.gpr[4](r31)
    mtsprg 3, r4
    lwz r2, OSContext.srr0(r31)
    lwz r4, OSContext.srr1(r31)
    lwz r31, OSContext.gpr[31](r31)
    b TRKInterruptHandler
}

void TRKEXICallBack(__OSInterrupt interrupt, OSContext* ctx) {
    OSEnableScheduler();
    TRKLoadContext(ctx, 0x500);
}

int InitMetroTRKCommTable(int hwId) {
    int result;

    if (hwId == HARDWARE_GDEV) {
        OSReport("MetroTRK : Set to GDEV hardware\n");
        result = Hu_IsStub();

        gDBCommTable.initialize_func = (DBCommInitFunc)DBInitComm;
        gDBCommTable.init_interrupts_func = (DBCommFunc)DBInitInterrupts;
        gDBCommTable.peek_func = (DBCommFunc)DBQueryData;
        gDBCommTable.read_func = (DBCommReadFunc)DBRead;
        gDBCommTable.write_func = (DBCommWriteFunc)DBWrite;
        gDBCommTable.open_func = (DBCommFunc)DBOpen;
        gDBCommTable.close_func = (DBCommFunc)DBClose;
    } else {
        OSReport("MetroTRK : Set to AMC DDH hardware\n");
        result = AMC_IsStub();

        gDBCommTable.initialize_func = (DBCommInitFunc)EXI2_Init;
        gDBCommTable.init_interrupts_func = (DBCommFunc)EXI2_EnableInterrupts;
        gDBCommTable.peek_func = EXI2_Poll;
        gDBCommTable.read_func = EXI2_ReadN;
        gDBCommTable.write_func = EXI2_WriteN;
        gDBCommTable.open_func = (DBCommFunc)EXI2_Reserve;
        gDBCommTable.close_func = (DBCommFunc)EXI2_Unreserve;
    }

    return result;
}

DSError TRKInitializeIntDrivenUART(u32 baudRate, u32 unused1, u32 unused2, u8** inputPendingPtrRef) {
    gDBCommTable.initialize_func(inputPendingPtrRef, TRKEXICallBack);
    return DS_NoError;
}

void EnableEXI2Interrupts(void) {
    gDBCommTable.init_interrupts_func();
}

static inline int TRKPollUART(void) {
    return gDBCommTable.peek_func();
}

static inline UARTError TRKReadUARTN(void* bytes, u32 length) {
    int readErr = gDBCommTable.read_func(bytes, length);
    return readErr == 0 ? 0 : -1;
}

static inline UARTError TRKWriteUARTN(const void* bytes, u32 length) {
    int writeErr = gDBCommTable.write_func(bytes, length);
    return writeErr == 0 ? 0 : -1;
}

UARTError WriteUARTFlush(void) {
    UARTError writeErr = 0;

    while (gWritePos < 0x800) {
        gWriteBuf[gWritePos] = 0;
        gWritePos++;
    }
    if (gWritePos != 0) {
        writeErr = TRKWriteUARTN(gWriteBuf, gWritePos);
        gWritePos = 0;
    }
    return writeErr;
}

UARTError WriteUART1(s8 byte) {
    gWriteBuf[gWritePos++] = byte;
    return 0;
}

int TRKReadUARTPoll(s8* byte) {
    UARTError readErr = 4;
    s32 count;

    if (gReadPos >= gReadCount) {
        gReadPos = 0;
        count = gReadCount = TRKPollUART();
        if (count > 0) {
            if (count > BUFF_LEN) {
                gReadCount = BUFF_LEN;
            }
            readErr = TRKReadUARTN(gReadBuf, gReadCount);
            if (readErr != 0) {
                gReadCount = 0;
            }
        }
    }
    if (gReadPos < gReadCount) {
        *byte = gReadBuf[gReadPos++];
        readErr = 0;
    }
    return readErr;
}

void ReserveEXI2Port(void) {
    gDBCommTable.open_func();
}

void UnreserveEXI2Port(void) {
    gDBCommTable.close_func();
}

void TRK_board_display(char* str) {
    OSReport(str);
}

void TRKUARTInterruptHandler(void) {
}
