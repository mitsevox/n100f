#include "TRK_MINNOW_DOLPHIN/trk.h"

// Sends a message as one frame: 0x7E, the escaped data and checksum, then 0x7E.
DSError TRKMessageSend(TRKBuffer* msg) {
    u8 checksum;
    u8 byte;
    u8 csByte;
    s32 err;
    s32 i;

    checksum = 0;
    for (i = 0; i < msg->length; i++) {
        checksum = checksum + msg->data[i];
    }
    checksum = checksum ^ 0xFF;

    err = WriteUART1(0x7E);
    if (err == 0) {
        for (i = 0; i < msg->length; i++) {
            byte = msg->data[i];
            if (byte == 0x7E || byte == 0x7D) {
                err = WriteUART1(0x7D);
                byte ^= 0x20;
                if (err != 0) {
                    break;
                }
            }
            err = WriteUART1(byte);
            if (err != 0) {
                break;
            }
        }
    }

    if (err == 0) {
        csByte = checksum;
        for (i = 0; i < 1; i++) {
            if (csByte == 0x7E || csByte == 0x7D) {
                err = WriteUART1(0x7D);
                csByte ^= 0x20;
                if (err != 0) {
                    break;
                }
            }
            err = WriteUART1(csByte);
            if (err != 0) {
                break;
            }
        }
    }

    if (err == 0) {
        err = WriteUART1(0x7E);
    }
    if (err == 0) {
        err = WriteUARTFlush();
    }
    return err;
}
