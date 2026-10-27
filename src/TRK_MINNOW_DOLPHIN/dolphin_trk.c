#include "TRK_MINNOW_DOLPHIN/trk.h"

#define EXCEPTIONMASK_ADDR 0x44

u32 lc_base;
extern u32 _db_stack_addr;

u32 TRK_ISR_OFFSETS[15] = {
    PPC_SystemReset,
    PPC_MachineCheck,
    PPC_DataStorage,
    PPC_InstructionStorage,
    PPC_ExternalInterrupt,
    PPC_Alignment,
    PPC_Program,
    PPC_FloatingPointUnavailable,
    PPC_Decrementer,
    PPC_SystemCall,
    PPC_Trace,
    PPC_PerformanceMonitor,
    PPC_InstructionAddressBreakpoint,
    PPC_SystemManagementInterrupt,
    PPC_ThermalManagementInterrupt,
};

static void __TRK_copy_vectors(void);

__declspec(section ".init") void __TRK_reset(void) {
    __TRK_copy_vectors();
}

asm void InitMetroTRK(void) {
    nofralloc
    addi r1, r1, -4
    stw r3, 0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, ProcessorState_PPC.Default.GPR(r3)
    lwz r4, 0(r1)
    addi r1, r1, 4
    stw r1, ProcessorState_PPC.Default.GPR[1](r3)
    stw r4, ProcessorState_PPC.Default.GPR[3](r3)
    mflr r4
    stw r4, ProcessorState_PPC.Default.LR(r3)
    stw r4, ProcessorState_PPC.Default.PC(r3)
    mfcr r4
    stw r4, ProcessorState_PPC.Default.CR(r3)
    mfmsr r4
    ori r3, r4, 0x8000
    xori r3, r3, 0x8000
    mtmsr r3
    mtsrr1 r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    lmw r0, ProcessorState_PPC.Default.GPR(r3)
    li r0, 0
    mtspr 0x3f2, r0
    mtspr 0x3f5, r0
    lis r1, _db_stack_addr@h
    ori r1, r1, _db_stack_addr@l
    mr r3, r5
    bl InitMetroTRKCommTable
    cmpwi r3, 1
    bne comm_table_ready
    lwz r4, ProcessorState_PPC.Default.LR(r3)
    mtlr r4
    lmw r0, ProcessorState_PPC.Default.GPR(r3)
    blr
comm_table_ready:
    b TRK_main
}

void EnableMetroTRKInterrupts(void) {
    EnableEXI2Interrupts();
}

u32 TRKTargetTranslate(u32 addr) {
    if (addr >= lc_base) {
        if (addr < lc_base + 0x4000 && (gTRKCPUState.Extended1.DBAT3U & 3) != 0) {
            return addr;
        }
    }

    return addr & 0x3FFFFFFF | 0x80000000;
}

static inline void TRK_copy_vector(u32 offset) {
    void* dest = (void*)TRKTargetTranslate(offset);
    TRK_memcpy(dest, gTRKInterruptVectorTable + offset, 0x100);
    TRK_flush_cache((u32)dest, 0x100);
}

static void __TRK_copy_vectors(void) {
    int i;
    u32 mask;

    mask = *(u32*)TRKTargetTranslate(EXCEPTIONMASK_ADDR);

    for (i = 0; i <= 14; ++i) {
        if (mask & (1 << i)) {
            TRK_copy_vector(TRK_ISR_OFFSETS[i]);
        }
    }
}

DSError TRKInitializeTarget(void) {
    gTRKState.stopped = TRUE;
    gTRKState.MSR = __TRK_get_MSR();
    lc_base = 0xE0000000;
    return DS_NoError;
}
