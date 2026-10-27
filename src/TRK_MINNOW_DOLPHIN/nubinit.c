#include "TRK_MINNOW_DOLPHIN/trk.h"

BOOL gTRKBigEndian;

// Returns TRUE when the byte order is neither big- nor little-endian.
static inline BOOL TRK_InitializeEndian(void) {
    BOOL res = FALSE;
    u8 bendian[4];
    u32 load;

    gTRKBigEndian = TRUE;

    bendian[0] = 0x12;
    bendian[1] = 0x34;
    bendian[2] = 0x56;
    bendian[3] = 0x78;

    load = *(u32*)bendian;
    if (load == 0x12345678) {
        gTRKBigEndian = TRUE;
    } else if (load == 0x78563412) {
        gTRKBigEndian = FALSE;
    } else {
        res = TRUE;
    }
    return res;
}

DSError TRKInitializeNub(void) {
    int res;

    res = TRK_InitializeEndian();

    if (res == DS_NoError) {
        usr_put_initialize();
    }

    if (res == DS_NoError) {
        res = TRKInitializeEventQueue();
    }

    if (res == DS_NoError) {
        res = TRKInitializeMessageBuffers();
    }

    if (res == DS_NoError) {
        res = TRKInitializeDispatcher();
    }

    if (res == DS_NoError) {
        DSError uartErr = TRKInitializeIntDrivenUART(0xE100, 1, 0, (u8**)&gTRKInputPendingPtr);
        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
        if (uartErr != DS_NoError) {
            res = uartErr;
        }
    }

    if (res == DS_NoError) {
        res = TRKInitializeSerialHandler();
    }

    if (res == DS_NoError) {
        res = TRKInitializeTarget();
    }

    return res;
}

DSError TRKTerminateNub(void) {
    TRKTerminateSerialHandler();
    return DS_NoError;
}

void TRKNubWelcome(void) {
    TRK_board_display("MetroTRK for GAMECUBE v0.10");
}
