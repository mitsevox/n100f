.include "macros.inc"

# The MetroTRK exception vector table. __TRK_reset copies it over the
# exception vectors at 0x80000000, so each handler sits at its vector's
# offset from the start of the table.

# Saves r2-r4 in SPRG1-3, enables address translation for the return, and
# enters TRKInterruptHandler with the exception's vector in r3.
.macro TRK_ENTER vector
    mtsprg 1, r2
    mtsprg 2, r3
    mtsprg 3, r4
    mfsrr0 r2
    mfsrr1 r4
    mfmsr r3
    ori r3, r3, 0x30
    mtsrr1 r3
    lis r3, TRKInterruptHandler@h
    ori r3, r3, TRKInterruptHandler@l
    mtsrr0 r3
    li r3, \vector
    rfi
.endm

# For the 603e TLB miss vectors: leaves TGPR mode (MSR bit 0x20000) if it
# is set, keeping CR in SPRG2 and r2 in SPRG1 meanwhile, then enters as above.
.macro TRK_ENTER_FP vector
    mtsprg 1, r2
    mfcr r2
    mtsprg 2, r2
    mfmsr r2
    andis. r2, r2, 0x2
    beq 1f
    mfmsr r2
    xoris r2, r2, 0x2
    sync
    mtmsr r2
    sync
    mtsprg 1, r2
1:
    mfsprg r2, 2
    mtcrf 0xff, r2
    mfsprg r2, 1
    TRK_ENTER \vector
.endm

.section .init, "ax"

.fn gTRKInterruptVectorTable, global
    .asciz "Metrowerks Target Resident Kernel for PowerPC"

    .org 0x100
    b __TRK_reset

    # Machine check: also invalidates the instruction and data cache
    # blocks at SRR0 and DAR.
    .org 0x200
    mtsprg 1, r2
    mfsrr0 r2
    icbi 0, r2
    mfdar r2
    dcbi 0, r2
    mfsprg r2, 1
    TRK_ENTER 0x200

    .org 0x300
    TRK_ENTER 0x300

    .org 0x400
    TRK_ENTER 0x400

    .org 0x500
    TRK_ENTER 0x500

    .org 0x600
    TRK_ENTER 0x600

    .org 0x700
    TRK_ENTER 0x700

    .org 0x800
    TRK_ENTER 0x800

    .org 0x900
    TRK_ENTER 0x900

    .org 0xC00
    TRK_ENTER 0xC00

    .org 0xD00
    TRK_ENTER 0xD00

    .org 0xE00
    TRK_ENTER 0xE00

    # 0xF00 branches past the 0xF20 handler to its own.
    .org 0xF00
    b 2f
    .org 0xF20
    TRK_ENTER 0xF20
2:
    TRK_ENTER 0xF00

    .org 0x1000
    TRK_ENTER_FP 0x1000

    .org 0x1100
    TRK_ENTER_FP 0x1100

    .org 0x1200
    TRK_ENTER_FP 0x1200

    .org 0x1300
    TRK_ENTER 0x1300

    .org 0x1400
    TRK_ENTER 0x1400

    .org 0x1600
    TRK_ENTER 0x1600

    .org 0x1700
    TRK_ENTER 0x1700

    .org 0x1C00
    TRK_ENTER 0x1C00

    .org 0x1D00
    TRK_ENTER 0x1D00

    .org 0x1E00
    TRK_ENTER 0x1E00

    .org 0x1F00
    TRK_ENTER 0x1F00

.endfn gTRKInterruptVectorTable

.global gTRKInterruptVectorTableEnd
gTRKInterruptVectorTableEnd:
