#include "TRK_MINNOW_DOLPHIN/trk.h"

static inline void TRKHandleRequestEvent(TRKEvent* event) {
    TRKBuffer* buffer = TRKGetBuffer(event->msgBufID);
    TRKDispatchMessage(buffer);
}

static inline void TRKHandleSupportEvent(TRKEvent* event) {
    TRKTargetSupportRequest();
}

static inline void TRKIdle(void) {
    if (TRKTargetStopped() == FALSE) {
        TRKTargetContinue();
    }
}

void TRKNubMainLoop(void) {
    TRKEvent event;
    BOOL isShutdownRequested;
    BOOL isNewInput;

    isShutdownRequested = FALSE;
    isNewInput = FALSE;
    while (isShutdownRequested == FALSE) {
        if (TRKGetNextEvent(&event) != FALSE) {
            isNewInput = FALSE;

            switch (event.eventType) {
            case NUBEVENT_Null:
                break;

            case NUBEVENT_Request:
                TRKHandleRequestEvent(&event);
                break;

            case NUBEVENT_Shutdown:
                isShutdownRequested = TRUE;
                break;

            case NUBEVENT_Breakpoint:
            case NUBEVENT_Exception:
                TRKTargetInterrupt(&event);
                break;

            case NUBEVENT_Support:
                TRKHandleSupportEvent(&event);
                break;
            }

            TRKDestructEvent(&event);
            continue;
        }

        // After an event or an idle pass, poll for input once; keep polling while input is pending.
        if (isNewInput == FALSE || *(u8*)gTRKInputPendingPtr != '\0') {
            isNewInput = TRUE;
            TRKGetInput();
            continue;
        }

        TRKIdle();
        isNewInput = FALSE;
    }
}
