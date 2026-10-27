#include "TRK_MINNOW_DOLPHIN/trk.h"

typedef enum {
    VALIDMEM_Readable = 0,
    VALIDMEM_Writeable = 1,
} ValidMemoryOptions;

typedef struct memRange {
    u8* start;
    u8* end;
    BOOL readable;
    BOOL writeable;
} memRange;

typedef struct StopInfo_PPC {
    u32 PC;
    u32 PCInstruction;
    u16 exceptionID;
} StopInfo_PPC;

typedef struct TRKExceptionStatus {
    StopInfo_PPC exceptionInfo; // 0x0
    u8 inTRK;                   // 0xC
    u8 exceptionDetected;       // 0xD
} TRKExceptionStatus;

typedef struct TRKStepStatus {
    BOOL active;     // 0x0
    u8 type;         // 0x4
    u32 count;       // 0x8
    u32 rangeStart;  // 0xC
    u32 rangeEnd;    // 0x10
} TRKStepStatus;

#define SPR_GQR0 912
#define SPR_HID2 920
#define SPR_FPECR 1022

#define INSTR_NOP 0x60000000
#define INSTR_BLR 0x4E800020
#define INSTR_PSQ_ST(psr, offset, rDest, w, gqr) \
    (0xF0000000 | (psr << 21) | (rDest << 16) | (w << 15) | (gqr << 12) | offset)
#define INSTR_PSQ_L(psr, offset, rSrc, w, gqr) \
    (0xE0000000 | (psr << 21) | (rSrc << 16) | (w << 15) | (gqr << 12) | offset)
#define INSTR_STW(rSrc, offset, rDest) (0x90000000 | (rSrc << 21) | (rDest << 16) | offset)
#define INSTR_LWZ(rDest, offset, rSrc) (0x80000000 | (rDest << 21) | (rSrc << 16) | offset)
#define INSTR_STFD(fprSrc, offset, rDest) (0xD8000000 | (fprSrc << 21) | (rDest << 16) | offset)
#define INSTR_LFD(fprDest, offset, rSrc) (0xC8000000 | (fprDest << 21) | (rSrc << 16) | offset)
#define INSTR_MFSPR(rDest, spr) \
    (0x7C000000 | (rDest << 21) | ((spr & 0xFE0) << 6) | ((spr & 0x1F) << 16) | 0x2A6)
#define INSTR_MTSPR(spr, rSrc) \
    (0x7C000000 | (rSrc << 21) | ((spr & 0xFE0) << 6) | ((spr & 0x1F) << 16) | 0x3A6)

const memRange gTRKMemMap[1] = { { (u8*)0, (u8*)-1, TRUE, TRUE } };

ProcessorRestoreFlags_PPC gTRKRestoreFlags = { FALSE, FALSE };

TRKExceptionStatus gTRKExceptionStatus = { { 0, 0, 0 }, TRUE, 0 };

TRKStepStatus gTRKStepStatus = { FALSE, DSSTEP_IntoCount, 0, 0 };

u16 TRK_saved_exceptionID = 0;
ProcessorState_PPC gTRKCPUState;
TRKState_PPC gTRKState;

typedef u8 u128[16];
u128 TRKvalue128_temp;

Default_PPC gTRKSaveState;

static void TRKExceptionHandler(u16 exceptionID);
void TRKInterruptHandlerEnableInterrupts(void);
void __TRK_set_MSR(u32 msr);
static void TRK_ppc_memcpy(register void* dest, register const void* src, register int n,
                           register u32 dest_msr, register u32 src_msr);
static inline DSError TRKPPCAccessSPR(void* value, u32 spr, BOOL read);
static inline DSError TRKPPCAccessPairedSingleRegister(void* srcDest, u32 psr, BOOL read);
DSError TRKPPCAccessFPRegister(void* srcDest, u32 fpr, BOOL read);
static inline DSError TRKPPCAccessSpecialReg(void* value, u32* access_func, BOOL read);
static inline BOOL TRKTargetCheckStep(void);

asm u32 __TRK_get_MSR(void) {
    nofralloc
    mfmsr r3
    blr
}

asm void __TRK_set_MSR(register u32 msr) {
    nofralloc
    mtmsr msr
    blr
}

DSError TRKValidMemory32(const void* addr, size_t length, ValidMemoryOptions readWriteable) {
    DSError err = DS_InvalidMemory;
    const u8* start;
    const u8* end;
    s32 i;

    start = (const u8*)addr;
    end = (const u8*)addr + (length - 1);

    if (end < start) {
        return DS_InvalidMemory;
    }

    for (i = 0; i < (s32)(sizeof(gTRKMemMap) / sizeof(memRange)); i++) {
        // The range overlaps this entry: check its access, then the parts outside it.
        if (start <= (const u8*)gTRKMemMap[i].end && end >= (const u8*)gTRKMemMap[i].start) {
            if (((u8)readWriteable == VALIDMEM_Readable && !gTRKMemMap[i].readable) ||
                ((u8)readWriteable == VALIDMEM_Writeable && !gTRKMemMap[i].writeable)) {
                err = DS_InvalidMemory;
            } else {
                err = DS_NoError;

                if (start < (const u8*)gTRKMemMap[i].start) {
                    err = TRKValidMemory32(start, (u32)((const u8*)gTRKMemMap[i].start - start),
                                           readWriteable);
                }

                if (err == DS_NoError && end > (const u8*)gTRKMemMap[i].end) {
                    err = TRKValidMemory32((const u8*)gTRKMemMap[i].end,
                                           (u32)(end - (const u8*)gTRKMemMap[i].end), readWriteable);
                }
            }

            break;
        }
    }

    return err;
}

// Copies n bytes, reading under src_msr and writing under dest_msr.
static asm void TRK_ppc_memcpy(register void* dest, register const void* src, register int n,
                               register u32 dest_msr, register u32 src_msr) {
#define saved_msr r8
#define byte r9
#define count r10
    nofralloc
    mfmsr saved_msr
    li count, 0
copy_loop:
    cmpw count, n
    beq copy_done
    mtmsr src_msr
    sync
    lbzx byte, count, src
    mtmsr dest_msr
    sync
    stbx byte, count, dest
    addi count, count, 1
    b copy_loop
copy_done:
    mtmsr saved_msr
    sync
    blr
#undef count
#undef byte
#undef saved_msr
}

DSError TRKTargetAccessMemory(void* data, u32 start, size_t* length, MemoryAccessOptions accessOptions,
                              BOOL read) {
    DSError error;
    u32 target_msr;
    void* addr;
    u32 trk_msr;
    TRKExceptionStatus tempExceptionStatus = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    addr = (void*)TRKTargetTranslate(start);
    error = TRKValidMemory32(addr, *length, read ? VALIDMEM_Readable : VALIDMEM_Writeable);

    if (error != DS_NoError) {
        *length = 0;
    } else {
        target_msr = __TRK_get_MSR();
        trk_msr = target_msr | gTRKCPUState.Extended1.MSR & 0x10;

        if (read) {
            TRK_ppc_memcpy(data, addr, *length, target_msr, trk_msr);
        } else {
            TRK_ppc_memcpy(addr, data, *length, trk_msr, target_msr);
            TRK_flush_cache((u32)addr, *length);
            if ((void*)start != addr) {
                TRK_flush_cache(start, *length);
            }
        }
    }

    if (gTRKExceptionStatus.exceptionDetected) {
        *length = 0;
        error = DS_CWDSException;
    }

    gTRKExceptionStatus = tempExceptionStatus;
    return error;
}

static inline DSError TRKTargetReadInstruction(void* data, u32 start) {
    DSError error;
    size_t length = 4;

    error = TRKTargetAccessMemory(data, start, &length, MEMACCESS_UserMemory, TRUE);

    if (error == DS_NoError && length != 4) {
        error = DS_InvalidMemory;
    }

    return error;
}

DSError TRKTargetAccessDefault(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                               BOOL read) {
    DSError error;
    u32 count;
    u32* data;
    TRKExceptionStatus tempExceptionStatus;

    if (lastRegister > 0x24) {
        return DS_InvalidRegister;
    }

    tempExceptionStatus = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    data = gTRKCPUState.Default.GPR + firstRegister;
    count = (lastRegister - firstRegister) + 1;
    *registersLength = count * sizeof(u32);

    if (read) {
        error = TRKAppendBuffer_ui32(b, data, count);
    } else {
        error = TRKReadBuffer_ui32(b, data, count);
    }

    if (gTRKExceptionStatus.exceptionDetected) {
        *registersLength = 0;
        error = DS_CWDSException;
    }

    gTRKExceptionStatus = tempExceptionStatus;
    return error;
}

DSError TRKTargetAccessFP(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                          BOOL read) {
    u64 temp;
    DSError error;
    TRKExceptionStatus tempExceptionStatus;
    u32 current;

    if (lastRegister > 0x21) {
        return DS_InvalidRegister;
    }

    tempExceptionStatus = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    __TRK_set_MSR(__TRK_get_MSR() | 0x2000);

    *registersLength = 0;
    error = DS_NoError;

    for (current = firstRegister; current <= lastRegister && error == DS_NoError;
         current++, *registersLength += sizeof(f64)) {
        if (read) {
            TRKPPCAccessFPRegister(&temp, current, read);
            error = TRKAppendBuffer1_ui64(b, temp);
        } else {
            TRKReadBuffer1_ui64(b, &temp);
            error = TRKPPCAccessFPRegister(&temp, current, read);
        }
    }

    if (gTRKExceptionStatus.exceptionDetected) {
        *registersLength = 0;
        error = DS_CWDSException;
    }

    gTRKExceptionStatus = tempExceptionStatus;
    return error;
}

DSError TRKTargetAccessExtended1(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                                 BOOL read) {
    TRKExceptionStatus tempExceptionStatus;
    int error;
    u32* data;
    int count;

    if (lastRegister > 0x60) {
        return DS_InvalidRegister;
    }

    tempExceptionStatus = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    *registersLength = 0;

    if (firstRegister <= lastRegister) {
        data = (u32*)&gTRKCPUState.Extended1 + firstRegister;
        count = lastRegister - firstRegister + 1;
        *registersLength += count * sizeof(u32);

        if (read) {
            error = TRKAppendBuffer_ui32(b, data, count);
        } else {
            if (data <= &gTRKCPUState.Extended1.TBU && data + count - 1 >= &gTRKCPUState.Extended1.TBL) {
                gTRKRestoreFlags.TBR = 1;
            }

            if (data <= &gTRKCPUState.Extended1.DEC && data + count - 1 >= &gTRKCPUState.Extended1.DEC) {
                gTRKRestoreFlags.DEC = 1;
            }
            error = TRKReadBuffer_ui32(b, data, count);
        }
    }

    if (gTRKExceptionStatus.exceptionDetected) {
        *registersLength = 0;
        error = DS_CWDSException;
    }

    gTRKExceptionStatus = tempExceptionStatus;
    return error;
}

DSError TRKTargetAccessExtended2(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                                 BOOL read) {
    TRKExceptionStatus savedException;
    u32 i;
    u32 hid2[1];
    u32 value[2];
    DSError err;

    if (lastRegister > 0x1F) {
        return DS_InvalidRegister;
    }

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = FALSE;

    // Enable paired singles and locked-cache DMA in HID2, and clear GQR0.
    TRKPPCAccessSPR(hid2, SPR_HID2, TRUE);
    hid2[0] |= 0xA0000000;
    TRKPPCAccessSPR(hid2, SPR_HID2, FALSE);
    hid2[0] = 0;
    TRKPPCAccessSPR(hid2, SPR_GQR0, FALSE);

    *registersLength = 0;
    err = DS_NoError;

    for (i = firstRegister; i <= lastRegister && err == DS_NoError; i++) {
        if (read) {
            err = TRKPPCAccessPairedSingleRegister((u64*)value, i, read);
            err = TRKAppendBuffer1_ui64(b, *(u64*)value);
        } else {
            err = TRKReadBuffer1_ui64(b, (u64*)value);
            err = TRKPPCAccessPairedSingleRegister((u64*)value, i, read);
        }

        *registersLength += sizeof(u64);
    }

    if (gTRKExceptionStatus.exceptionDetected) {
        *registersLength = 0;
        err = DS_CWDSException;
    }

    gTRKExceptionStatus = savedException;
    return err;
}

DSError TRKTargetVersions(DSVersions* versions) {
    versions->kernelMajor = 0;
    versions->kernelMinor = 10;
    versions->protocolMajor = 1;
    versions->protocolMinor = 10;
    return DS_NoError;
}

DSError TRKTargetSupportMask(u8* mask) {
    mask[0] = 0x7A;
    mask[1] = 0;
    mask[2] = 0x4F;
    mask[3] = 7;
    mask[4] = 0;
    mask[5] = 0;
    mask[6] = 0;
    mask[7] = 0;
    mask[8] = 0;
    mask[9] = 0;
    mask[10] = 0;
    mask[11] = 0;
    mask[12] = 0;
    mask[13] = 0;
    mask[14] = 0;
    mask[15] = 0;
    mask[16] = 1;
    mask[17] = 0;
    mask[18] = 3;
    mask[19] = 0;
    mask[20] = 0;
    mask[21] = 0;
    mask[22] = 0;
    mask[23] = 0;
    mask[24] = 0;
    mask[25] = 0;
    mask[26] = 3;
    mask[27] = 0;
    mask[28] = 0;
    mask[29] = 0;
    mask[30] = 0;
    mask[31] = 0x80;
    return DS_NoError;
}

DSError TRKTargetCPUType(DSCPUType* cpuType) {
    cpuType->cpuMajor = 0;
    cpuType->cpuMinor = TRKTargetCPUMinorType();
    cpuType->bigEndian = gTRKBigEndian;
    cpuType->defaultTypeSize = 4;
    cpuType->fpTypeSize = 8;
    cpuType->extended1TypeSize = 4;
    cpuType->extended2TypeSize = 8;
    return DS_NoError;
}

// Common entry for exceptions routed to the debugger. The vector stub has put SRR0 in r2,
// SRR1 in r4 and the exception ID in r3, and saved r2-r4 and CR in SPRG1-3.
asm void TRKInterruptHandler(void) {
    nofralloc
    mtsrr0 r2
    mtsrr1 r4
    mfsprg r4, 3
    mfcr r2
    mtsprg 3, r2
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r2, TRKState_PPC.MSR(r2)
    ori r2, r2, 0x8002
    xori r2, r2, 0x8002
    sync
    mtmsr r2
    sync
    lis r2, TRK_saved_exceptionID@h
    ori r2, r2, TRK_saved_exceptionID@l
    sth r3, 0(r2)
    cmpwi r3, 0x500
    bne handle_exception
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    mflr r3
    stw r3, ProcessorState_PPC.transport_handler_saved_ra(r2)
    bl TRKUARTInterruptHandler
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    lwz r3, ProcessorState_PPC.transport_handler_saved_ra(r2)
    mtlr r3
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r2, TRKState_PPC.inputPendingPtr(r2)
    lbz r2, 0(r2)
    cmpwi r2, 0
    beq resume_target
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    lbz r2, TRKExceptionStatus.inTRK(r2)
    cmpwi r2, 1
    beq resume_target
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    li r3, 1
    stb r3, TRKState_PPC.inputActivated(r2)
    b handle_exception
resume_target:
    lis r2, gTRKSaveState@h
    ori r2, r2, gTRKSaveState@l
    lwz r3, Default_PPC.CR(r2)
    mtcrf 0xff, r3
    lwz r3, Default_PPC.GPR[3](r2)
    lwz r2, Default_PPC.GPR[2](r2)
    rfi
handle_exception:
    lis r2, TRK_saved_exceptionID@h
    ori r2, r2, TRK_saved_exceptionID@l
    lhz r3, 0(r2)
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    lbz r2, TRKExceptionStatus.inTRK(r2)
    cmpwi r2, 0
    bne TRKExceptionHandler
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    stw r0, ProcessorState_PPC.Default.GPR[0](r2)
    stw r1, ProcessorState_PPC.Default.GPR[1](r2)
    mfsprg r0, 1
    stw r0, ProcessorState_PPC.Default.GPR[2](r2)
    sth r3, ProcessorState_PPC.Extended1.exceptionID(r2)
    sth r3, (ProcessorState_PPC.Extended1.exceptionID + 2)(r2)
    mfsprg r0, 2
    stw r0, ProcessorState_PPC.Default.GPR[3](r2)
    stmw r4, ProcessorState_PPC.Default.GPR[4](r2)
    mfsrr0 r27
    mflr r28
    mfsprg r29, 3
    mfctr r30
    mfxer r31
    stmw r27, ProcessorState_PPC.Default.PC(r2)
    bl TRKSaveExtended1Block
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    li r3, 1
    stb r3, TRKExceptionStatus.inTRK(r2)
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r0, TRKState_PPC.MSR(r2)
    sync
    mtmsr r0
    sync
    lwz r0, TRKState_PPC.LR(r2)
    mtlr r0
    lwz r0, TRKState_PPC.CTR(r2)
    mtctr r0
    lwz r0, TRKState_PPC.XER(r2)
    mtxer r0
    lwz r0, TRKState_PPC.DSISR(r2)
    mtdsisr r0
    lwz r0, TRKState_PPC.DAR(r2)
    mtdar r0
    lmw r3, TRKState_PPC.GPR[3](r2)
    lwz r0, TRKState_PPC.GPR[0](r2)
    lwz r1, TRKState_PPC.GPR[1](r2)
    lwz r2, TRKState_PPC.GPR[2](r2)
    b TRKPostInterruptEvent
}

// An exception inside TRK itself: record it, skip the faulting instruction where that is safe,
// and return.
static asm void TRKExceptionHandler(u16 exceptionID) {
    nofralloc
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    sth r3, TRKExceptionStatus.exceptionInfo.exceptionID(r2)
    mfsrr0 r3
    stw r3, TRKExceptionStatus.exceptionInfo.PC(r2)
    lhz r3, TRKExceptionStatus.exceptionInfo.exceptionID(r2)
    cmpwi r3, 0x200
    beq skip_instruction
    cmpwi r3, 0x300
    beq skip_instruction
    cmpwi r3, 0x400
    beq skip_instruction
    cmpwi r3, 0x600
    beq skip_instruction
    cmpwi r3, 0x700
    beq skip_instruction
    cmpwi r3, 0x800
    beq skip_instruction
    cmpwi r3, 0x1000
    beq skip_instruction
    cmpwi r3, 0x1100
    beq skip_instruction
    cmpwi r3, 0x1200
    beq skip_instruction
    cmpwi r3, 0x1300
    beq skip_instruction
    b mark_detected
skip_instruction:
    mfsrr0 r3
    addi r3, r3, 4
    mtsrr0 r3
mark_detected:
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    li r3, 1
    stb r3, TRKExceptionStatus.exceptionDetected(r2)
    mfsprg r3, 3
    mtcrf 0xff, r3
    mfsprg r2, 1
    mfsprg r3, 2
    rfi
}

void TRKPostInterruptEvent(void) {
    NubEventType eventType;
    u32 inst;
    TRKEvent event;

    if (gTRKState.inputActivated) {
        gTRKState.inputActivated = FALSE;
    } else {
        switch (gTRKCPUState.Extended1.exceptionID & 0xFFFF) {
        case PPC_Trace:
        case PPC_Program:
            TRKTargetReadInstruction(&inst, gTRKCPUState.Default.PC);

            // twui r0,0 is the support-request trap.
            if (inst == 0x0FE00000) {
                eventType = NUBEVENT_Support;
            } else {
                eventType = NUBEVENT_Breakpoint;
            }
            break;
        default:
            eventType = NUBEVENT_Exception;
            break;
        }

        TRKConstructEvent(&event, eventType);
        TRKPostEvent(&event);
    }
}

asm void TRKSwapAndGo(void) {
    nofralloc
    lis r3, gTRKState@h
    ori r3, r3, gTRKState@l
    stmw r0, TRKState_PPC.GPR[0](r3)
    mfmsr r0
    stw r0, TRKState_PPC.MSR(r3)
    mflr r0
    stw r0, TRKState_PPC.LR(r3)
    mfctr r0
    stw r0, TRKState_PPC.CTR(r3)
    mfxer r0
    stw r0, TRKState_PPC.XER(r3)
    mfdsisr r0
    stw r0, TRKState_PPC.DSISR(r3)
    mfdar r0
    stw r0, TRKState_PPC.DAR(r3)
    li r1, -0x7FFE
    nor r1, r1, r1
    mfmsr r3
    and r3, r3, r1
    mtmsr r3
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r2, TRKState_PPC.inputPendingPtr(r2)
    lbz r2, 0(r2)
    cmpwi r2, 0
    beq run_target
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    li r3, 1
    stb r3, TRKState_PPC.inputActivated(r2)
    b TRKInterruptHandlerEnableInterrupts
run_target:
    lis r2, gTRKExceptionStatus@h
    ori r2, r2, gTRKExceptionStatus@l
    li r3, 0
    stb r3, TRKExceptionStatus.inTRK(r2)
    bl TRKRestoreExtended1Block
    lis r2, gTRKCPUState@h
    ori r2, r2, gTRKCPUState@l
    lmw r27, ProcessorState_PPC.Default.PC(r2)
    mtsrr0 r27
    mtlr r28
    mtcrf 0xff, r29
    mtctr r30
    mtxer r31
    lmw r3, ProcessorState_PPC.Default.GPR[3](r2)
    lwz r0, ProcessorState_PPC.Default.GPR[0](r2)
    lwz r1, ProcessorState_PPC.Default.GPR[1](r2)
    lwz r2, ProcessorState_PPC.Default.GPR[2](r2)
    rfi
}

asm void TRKInterruptHandlerEnableInterrupts(void) {
    nofralloc
    lis r2, gTRKState@h
    ori r2, r2, gTRKState@l
    lwz r0, TRKState_PPC.MSR(r2)
    sync
    mtmsr r0
    sync
    lwz r0, TRKState_PPC.LR(r2)
    mtlr r0
    lwz r0, TRKState_PPC.CTR(r2)
    mtctr r0
    lwz r0, TRKState_PPC.XER(r2)
    mtxer r0
    lwz r0, TRKState_PPC.DSISR(r2)
    mtdsisr r0
    lwz r0, TRKState_PPC.DAR(r2)
    mtdar r0
    lmw r3, TRKState_PPC.GPR[3](r2)
    lwz r0, TRKState_PPC.GPR[0](r2)
    lwz r1, TRKState_PPC.GPR[1](r2)
    lwz r2, TRKState_PPC.GPR[2](r2)
    b TRKPostInterruptEvent
}

DSError TRKTargetInterrupt(TRKEvent* event) {
    DSError error = DS_NoError;

    switch (event->eventType) {
    case NUBEVENT_Breakpoint:
    case NUBEVENT_Exception:
        if (TRKTargetCheckStep() == FALSE) {
            TRKTargetSetStopped(TRUE);
            error = TRKDoNotifyStopped(DSMSG_NotifyStopped);
        }
        break;
    default:
        break;
    }

    return error;
}

DSError TRKTargetAddStopInfo(TRKBuffer* buffer) {
    DSError error;
    u32 instruction;
    s32 i;

    error = TRKAppendBuffer1_ui32(buffer, gTRKCPUState.Default.PC);
    if (error == DS_NoError) {
        error = TRKTargetReadInstruction(&instruction, gTRKCPUState.Default.PC);
    }
    if (error == DS_NoError) {
        error = TRKAppendBuffer1_ui32(buffer, instruction);
    }
    if (error == DS_NoError) {
        error = TRKAppendBuffer1_ui16(buffer, gTRKCPUState.Extended1.exceptionID);
    }

    if (error == DS_NoError) {
        for (i = 0; i < 32; i++) {
            error = TRKAppendBuffer1_ui32(buffer, (u16)gTRKCPUState.Default.GPR[i]);
        }
        for (i = 0; i < 32; i++) {
            error = TRKAppendBuffer1_ui64(buffer, (u16)gTRKCPUState.Float.FPR[i]);
        }
    }

    return error;
}

DSError TRKTargetAddExceptionInfo(TRKBuffer* buffer) {
    DSError error;
    u32 instruction;

    error = TRKAppendBuffer1_ui32(buffer, gTRKExceptionStatus.exceptionInfo.PC);
    if (error == DS_NoError) {
        error = TRKTargetReadInstruction(&instruction, gTRKExceptionStatus.exceptionInfo.PC);
    }
    if (error == DS_NoError) {
        error = TRKAppendBuffer1_ui32(buffer, instruction);
    }
    if (error == DS_NoError) {
        error = TRKAppendBuffer1_ui16(buffer, gTRKExceptionStatus.exceptionInfo.exceptionID);
    }

    return error;
}

static inline DSError TRKTargetEnableTrace(BOOL enable) {
    if (enable) {
        gTRKCPUState.Extended1.MSR = gTRKCPUState.Extended1.MSR | 0x400;
    } else {
        gTRKCPUState.Extended1.MSR = gTRKCPUState.Extended1.MSR & ~0x400;
    }
    return DS_NoError;
}

static inline BOOL TRKTargetStepDone(void) {
    BOOL result = TRUE;

    if (gTRKStepStatus.active && (u16)gTRKCPUState.Extended1.exceptionID == PPC_Trace) {
        switch (gTRKStepStatus.type) {
        case DSSTEP_IntoCount:
            if (gTRKStepStatus.count > 0) {
                result = FALSE;
            }
            break;
        case DSSTEP_IntoRange:
            if (gTRKCPUState.Default.PC >= gTRKStepStatus.rangeStart &&
                gTRKCPUState.Default.PC <= gTRKStepStatus.rangeEnd) {
                result = FALSE;
            }
            break;
        default:
            break;
        }
    }

    return result;
}

static inline DSError TRKTargetDoStep(void) {
    gTRKStepStatus.active = TRUE;
    TRKTargetEnableTrace(TRUE);

    if (gTRKStepStatus.type == DSSTEP_IntoCount || gTRKStepStatus.type == DSSTEP_OverCount) {
        gTRKStepStatus.count--;
    }

    TRKTargetSetStopped(FALSE);
    return DS_NoError;
}

static inline BOOL TRKTargetCheckStep(void) {
    if (gTRKStepStatus.active) {
        TRKTargetEnableTrace(FALSE);

        if (TRKTargetStepDone()) {
            gTRKStepStatus.active = FALSE;
        } else {
            TRKTargetDoStep();
        }
    }

    return gTRKStepStatus.active;
}

DSError TRKTargetSingleStep(u32 count, BOOL stepOver) {
    DSError error = DS_NoError;

    if (stepOver) {
        error = DS_UnsupportedError;
    } else {
        gTRKStepStatus.type = DSSTEP_IntoCount;
        gTRKStepStatus.count = count;
        error = TRKTargetDoStep();
    }

    return error;
}

DSError TRKTargetStepOutOfRange(u32 rangeStart, u32 rangeEnd, BOOL stepOver) {
    DSError error = DS_NoError;

    if (stepOver) {
        error = DS_UnsupportedError;
    } else {
        gTRKStepStatus.type = DSSTEP_IntoRange;
        gTRKStepStatus.rangeStart = rangeStart;
        gTRKStepStatus.rangeEnd = rangeEnd;
        error = TRKTargetDoStep();
    }

    return error;
}

u32 TRKTargetGetPC(void) {
    return gTRKCPUState.Default.PC;
}

// Services a file request the target makes through the support trap: the command is in r3
// and its arguments in r4-r6, and the I/O result goes back in r3.
DSError TRKTargetSupportRequest(void) {
    DSError error;
    u32 position;
    size_t* length;
    MessageCommandID commandId;
    TRKEvent event;
    u8 ioResult;

    commandId = (u8)gTRKCPUState.Default.GPR[3];
    if ((u32)commandId != DSMSG_ReadFile && (u32)commandId != DSMSG_WriteFile &&
        (u32)commandId != DSMSG_OpenFile && (u32)commandId != DSMSG_CloseFile &&
        (u32)commandId != DSMSG_PositionFile) {
        TRKConstructEvent(&event, NUBEVENT_Exception);
        TRKPostEvent(&event);
        return DS_NoError;
    }

    if ((u32)commandId == DSMSG_OpenFile) {
        error = HandleOpenFileSupportRequest((char*)gTRKCPUState.Default.GPR[4], (u8)gTRKCPUState.Default.GPR[5],
                                             (u32*)gTRKCPUState.Default.GPR[6], &ioResult);
        if (ioResult == DS_IONoError && error != DS_NoError) {
            ioResult = DS_IOError;
        }
        gTRKCPUState.Default.GPR[3] = ioResult;
    } else if ((u32)commandId == DSMSG_CloseFile) {
        error = HandleCloseFileSupportRequest(gTRKCPUState.Default.GPR[4], &ioResult);
        if (ioResult == DS_IONoError && error != DS_NoError) {
            ioResult = DS_IOError;
        }
        gTRKCPUState.Default.GPR[3] = ioResult;
    } else if ((u32)commandId == DSMSG_PositionFile) {
        position = *(u32*)gTRKCPUState.Default.GPR[5];
        error = HandlePositionFileSupportRequest(gTRKCPUState.Default.GPR[4], &position,
                                                 (u8)gTRKCPUState.Default.GPR[6], &ioResult);
        if (ioResult == DS_IONoError && error != DS_NoError) {
            ioResult = DS_IOError;
        }
        gTRKCPUState.Default.GPR[3] = ioResult;
        *(u32*)gTRKCPUState.Default.GPR[5] = position;
    } else {
        length = (size_t*)gTRKCPUState.Default.GPR[5];
        error = TRKSuppAccessFile((u8)gTRKCPUState.Default.GPR[4], (u8*)gTRKCPUState.Default.GPR[6], length,
                                  &ioResult, TRUE, commandId == DSMSG_ReadFile);
        if (ioResult == DS_IONoError && error != DS_NoError) {
            ioResult = DS_IOError;
        }
        gTRKCPUState.Default.GPR[3] = ioResult;
        if ((u32)commandId == DSMSG_ReadFile) {
            TRK_flush_cache(gTRKCPUState.Default.GPR[6], *length);
        }
    }

    gTRKCPUState.Default.PC += 4;
    return error;
}

DSError TRKTargetFlushCache(u8 options, void* start, void* end) {
    if (start < end) {
        TRK_flush_cache((u32)start, (u8*)end - (u8*)start);
        return DS_NoError;
    }

    return DS_InvalidMemory;
}

BOOL TRKTargetStopped(void) {
    return gTRKState.stopped;
}

void TRKTargetSetStopped(unsigned int stopped) {
    gTRKState.stopped = stopped;
}

DSError TRKTargetStop(void) {
    TRKTargetSetStopped(TRUE);
    return DS_NoError;
}

// Builds a mfspr or mtspr between the register and *value, and runs it.
static inline DSError TRKPPCAccessSPR(void* value, u32 spr, BOOL read) {
    u32 access_func[10] = { INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP,
                            INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP };

    if (read) {
        access_func[0] = INSTR_MFSPR(4, spr);
        access_func[1] = (u32)INSTR_STW(4, 0, 3);
    } else {
        access_func[0] = (u32)INSTR_LWZ(4, 0, 3);
        access_func[1] = INSTR_MTSPR(spr, 4);
    }

    return TRKPPCAccessSpecialReg(value, access_func, read);
}

// Builds a psq_st or psq_l between the paired-single register and *srcDest, and runs it.
static inline DSError TRKPPCAccessPairedSingleRegister(void* srcDest, u32 psr, BOOL read) {
    u32 access_func[10] = { INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP,
                            INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP };

    if (read) {
        access_func[0] = INSTR_PSQ_ST(psr, 0, 3, 0, 0);
    } else {
        access_func[0] = INSTR_PSQ_L(psr, 0, 3, 0, 0);
    }

    return TRKPPCAccessSpecialReg(srcDest, access_func, read);
}

// Registers 0-31 are the FPRs, 32 is FPSCR and 33 is FPECR.
DSError TRKPPCAccessFPRegister(void* srcDest, u32 fpr, BOOL read) {
    DSError error = DS_NoError;
    u32 access_func[10] = { INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP,
                            INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP, INSTR_NOP };

    if (fpr < 0x20) {
        if (read) {
            access_func[0] = INSTR_STFD(fpr, 0, 3);
        } else {
            access_func[0] = INSTR_LFD(fpr, 0, 3);
        }

        error = TRKPPCAccessSpecialReg(srcDest, access_func, read);
    } else if (fpr == 0x20) {
        *(u64*)srcDest &= 0xFFFFFFFF;
    } else if (fpr == 0x21) {
        if (!read) {
            *(u32*)srcDest = *((u32*)srcDest + 1);
        }
        error = TRKPPCAccessSPR(srcDest, SPR_FPECR, read);
        if (read) {
            *(u64*)srcDest = *(u32*)srcDest & 0xFFFFFFFFLL;
        }
    }

    return error;
}

// Ends the instruction array with blr, flushes it and calls it with value in r3.
static inline DSError TRKPPCAccessSpecialReg(void* value, u32* access_func, BOOL read) {
    typedef void (*asm_access_type)(void*, void*);
    asm_access_type asm_access;

    access_func[9] = INSTR_BLR;
    asm_access = (asm_access_type)access_func;

    TRK_flush_cache((u32)access_func, sizeof(access_func) * 10);
    asm_access(value, &TRKvalue128_temp);

    return DS_NoError;
}

void TRKTargetSetInputPendingPtr(void* ptr) {
    gTRKState.inputPendingPtr = ptr;
}
