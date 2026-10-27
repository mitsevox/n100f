#include "TRK_MINNOW_DOLPHIN/trk.h"

static TRKFramingState gTRKFramingState;

void* gTRKInputPendingPtr;

// Strips the checksum from a completed frame, or rejects a frame too short to hold one.
static inline BOOL TRKCompleteFrame(TRKBuffer* buffer) {
    if (buffer->length < 2) {
        TRKStandardACK(buffer, DSMSG_ReplyNAK, DSREPLY_PacketSizeError);
        if (gTRKFramingState.msgBufID != -1) {
            TRKReleaseBuffer(gTRKFramingState.msgBufID);
            gTRKFramingState.msgBufID = -1;
        }
        gTRKFramingState.buffer = NULL;
        gTRKFramingState.receiveState = DSRECV_Wait;
        return FALSE;
    }
    buffer->position = 0;
    buffer->length--;
    return TRUE;
}

// Reads bytes from the UART until a frame completes; returns its buffer ID, or -1.
int TRKTestForPacket(void) {
    DSError err;
    int readErr;
    s8 byte;
    int bufID;

    err = DS_NoError;
    readErr = TRKReadUARTPoll(&byte);
    while (readErr == 0 && err == DS_NoError) {
        if (gTRKFramingState.receiveState != DSRECV_InFrame) {
            gTRKFramingState.isEscape = FALSE;
        }
        switch (gTRKFramingState.receiveState) {
        case DSRECV_Wait:
            if (byte == 0x7E) {
                err = TRKGetFreeBuffer(&gTRKFramingState.msgBufID, &gTRKFramingState.buffer);
                gTRKFramingState.fcs = 0;
                gTRKFramingState.receiveState = DSRECV_Found;
            }
            break;
        case DSRECV_Found:
            if (byte == 0x7E) {
                break;
            }
            gTRKFramingState.receiveState = DSRECV_InFrame;
            // fallthrough
        case DSRECV_InFrame:
            if (byte == 0x7E) {
                if (gTRKFramingState.isEscape) {
                    TRKStandardACK(gTRKFramingState.buffer, DSMSG_ReplyNAK, DSREPLY_EscapeError);
                    if (gTRKFramingState.msgBufID != -1) {
                        TRKReleaseBuffer(gTRKFramingState.msgBufID);
                        gTRKFramingState.msgBufID = -1;
                    }
                    gTRKFramingState.buffer = NULL;
                    gTRKFramingState.receiveState = DSRECV_Wait;
                    break;
                }
                if (TRKCompleteFrame(gTRKFramingState.buffer)) {
                    bufID = gTRKFramingState.msgBufID;
                    gTRKFramingState.msgBufID = -1;
                    gTRKFramingState.buffer = NULL;
                    gTRKFramingState.receiveState = DSRECV_Wait;
                    return bufID;
                }
                gTRKFramingState.receiveState = DSRECV_Wait;
            } else {
                if (gTRKFramingState.isEscape) {
                    byte ^= 0x20;
                    gTRKFramingState.isEscape = FALSE;
                } else if (byte == 0x7D) {
                    gTRKFramingState.isEscape = TRUE;
                    break;
                }
                err = TRKAppendBuffer1_ui8(gTRKFramingState.buffer, byte);
                gTRKFramingState.fcs += byte;
            }
            break;
        case DSRECV_FrameOverflow:
            if (byte == 0x7E) {
                if (gTRKFramingState.msgBufID != -1) {
                    TRKReleaseBuffer(gTRKFramingState.msgBufID);
                    gTRKFramingState.msgBufID = -1;
                }
                gTRKFramingState.buffer = NULL;
                gTRKFramingState.receiveState = DSRECV_Wait;
            }
            break;
        }
        readErr = TRKReadUARTPoll(&byte);
    }
    return -1;
}

void TRKGetInput(void) {
    TRKBuffer* msgBuffer;
    int id;
    u8 command;

    id = TRKTestForPacket();
    if (id == -1) {
        return;
    }

    msgBuffer = TRKGetBuffer(id);
    TRKSetBufferPosition(msgBuffer, 0);
    TRKReadBuffer1_ui8(msgBuffer, &command);
    if (command < DSMSG_ReplyACK) {
        TRKProcessInput(id);
    } else {
        TRKReleaseBuffer(id);
    }
}

void TRKProcessInput(int bufferIdx) {
    TRKEvent event;

    TRKConstructEvent(&event, NUBEVENT_Request);
    gTRKFramingState.msgBufID = -1;
    event.msgBufID = bufferIdx;
    TRKPostEvent(&event);
}

DSError TRKInitializeSerialHandler(void) {
    gTRKFramingState.msgBufID = -1;
    gTRKFramingState.receiveState = DSRECV_Wait;
    gTRKFramingState.isEscape = FALSE;

    return DS_NoError;
}

DSError TRKTerminateSerialHandler(void) {
    return DS_NoError;
}
