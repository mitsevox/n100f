#ifndef TRK_MINNOW_DOLPHIN_TRK_H
#define TRK_MINNOW_DOLPHIN_TRK_H

// MetroTRK declarations shared by the TRK_MINNOW_DOLPHIN units, limited to what
// reconstructed units use (docs/libraries.md).

#include "dolphin/types.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DS_NoError = 0x0,
    DS_ParameterError = 0x2,

    DS_EventQueueFull = 0x100,

    DS_MessageBufferOverflow = 0x301,
    DS_MessageBufferReadError = 0x302,

    DS_DispatchError = 0x500,

    DS_InvalidMemory = 0x700,
    DS_InvalidRegister = 0x701,
    DS_CWDSException = 0x702,
    DS_UnsupportedError = 0x703,
    DS_InvalidProcessID = 0x704,
    DS_InvalidThreadID = 0x705,
    DS_OSError = 0x706,

    DS_Error800 = 0x800,
} DSError;

// Message command bytes.
typedef enum {
    DSMSG_ReplyACK = 0x80,
    DSMSG_NotifyStopped = 0x90,
    DSMSG_WriteFile = 0xD0,
    DSMSG_ReadFile = 0xD1,
    DSMSG_OpenFile = 0xD2,
    DSMSG_CloseFile = 0xD3,
    DSMSG_PositionFile = 0xD4,
    DSMSG_ReplyNAK = 0xFF,
} MessageCommandID;

// Error bytes sent in a reply.
typedef enum {
    DSREPLY_NoError = 0x0,
    DSREPLY_Error = 0x1,
    DSREPLY_PacketSizeError = 0x2,
    DSREPLY_CWDSError = 0x3,
    DSREPLY_EscapeError = 0x4,

    DSREPLY_UnsupportedCommandError = 0x10,
    DSREPLY_ParameterError = 0x11,
    DSREPLY_UnsupportedOptionError = 0x12,
    DSREPLY_InvalidMemoryRange = 0x13,
    DSREPLY_InvalidRegisterRange = 0x14,
    DSREPLY_CWDSException = 0x15,
    DSREPLY_NotStopped = 0x16,

    DSREPLY_OSError = 0x20,
    DSREPLY_InvalidProcessID = 0x21,
    DSREPLY_InvalidThreadID = 0x22,
} DSReplyError;

// Register sets named by the read and write register requests.
typedef enum {
    DSREG_Default = 0,
    DSREG_FP = 1,
    DSREG_Extended1 = 2,
    DSREG_Extended2 = 3,
} DSMessageRegisterOptions;

// Step request kinds.
typedef enum {
    DSSTEP_IntoCount = 0x0,
    DSSTEP_IntoRange = 0x1,
    DSSTEP_OverCount = 0x10,
    DSSTEP_OverRange = 0x11,
} DSMessageStepOptions;

typedef enum {
    DS_IONoError = 0,
    DS_IOError = 1,
} DSIOResult;

// Serial framing receiver states.
typedef enum {
    DSRECV_Wait = 0,
    DSRECV_Found = 1,
    DSRECV_InFrame = 2,
    DSRECV_FrameOverflow = 3,
} ReceiverState;

typedef enum {
    NUBEVENT_Null = 0,
    NUBEVENT_Shutdown = 1,
    NUBEVENT_Request = 2,
    NUBEVENT_Breakpoint = 3,
    NUBEVENT_Exception = 4,
    NUBEVENT_Support = 5,
} NubEventType;

typedef enum {
    MEMACCESS_UserMemory = 0,
    MEMACCESS_DebuggerMemory = 1,
} MemoryAccessOptions;

typedef int UARTError;

#define TRKMSGBUF_SIZE 0x880

typedef struct TRKBuffer {
    u32 mutex;                 // 0x0
    BOOL isInUse;              // 0x4
    u32 length;                // 0x8
    u32 position;              // 0xC
    u8 data[TRKMSGBUF_SIZE];   // 0x10
} TRKBuffer;

typedef struct TRKEvent {
    u8 eventType;   // 0x0
    u32 eventID;    // 0x4
    int msgBufID;   // 0x8
} TRKEvent;

typedef struct TRKEventQueue {
    u8 mutex[4];          // 0x0
    int count;            // 0x4
    int next;             // 0x8
    TRKEvent events[2];   // 0xC
    u32 eventID;          // 0x24
} TRKEventQueue;

typedef struct TRKFramingState {
    int msgBufID;       // 0x0
    TRKBuffer* buffer;  // 0x4
    u8 receiveState;    // 0x8
    BOOL isEscape;      // 0xC
    u8 fcs;             // 0x10
} TRKFramingState;

typedef struct DSVersions {
    u8 kernelMajor;
    u8 kernelMinor;
    u8 protocolMajor;
    u8 protocolMinor;
} DSVersions;

typedef struct DSCPUType {
    u8 cpuMajor;
    u8 cpuMinor;
    u8 bigEndian;
    u8 defaultTypeSize;
    u8 fpTypeSize;
    u8 extended1TypeSize;
    u8 extended2TypeSize;
} DSCPUType;

// mainloop.c
void TRKNubMainLoop(void);

// nubevent.c
DSError TRKInitializeEventQueue(void);
BOOL TRKGetNextEvent(TRKEvent* event);
DSError TRKPostEvent(TRKEvent* event);
void TRKConstructEvent(TRKEvent* event, int eventType);
void TRKDestructEvent(TRKEvent* event);

// nubinit.c
DSError TRKInitializeNub(void);
DSError TRKTerminateNub(void);
void TRKNubWelcome(void);

// msg.c
DSError TRKMessageSend(TRKBuffer* msg);

// msgbuf.c
DSError TRKInitializeMessageBuffers(void);
DSError TRKGetFreeBuffer(int* msgID, TRKBuffer** outMsg);
void* TRKGetBuffer(int idx);
void TRKReleaseBuffer(int idx);
void TRKResetBuffer(TRKBuffer* msg, u8 keepData);
DSError TRKSetBufferPosition(TRKBuffer* msg, u32 pos);
DSError TRKAppendBuffer(TRKBuffer* msg, const void* data, size_t length);
DSError TRKAppendBuffer1_ui16(TRKBuffer* buffer, const u16 data);
DSError TRKAppendBuffer1_ui32(TRKBuffer* buffer, const u32 data);
DSError TRKAppendBuffer_ui8(TRKBuffer* buffer, const u8* data, int count);
DSError TRKReadBuffer(TRKBuffer* msg, void* data, size_t length);
DSError TRKReadBuffer1_ui8(TRKBuffer* buffer, u8* data);
DSError TRKReadBuffer1_ui16(TRKBuffer* buffer, u16* data);
DSError TRKReadBuffer1_ui32(TRKBuffer* buffer, u32* data);
DSError TRKReadBuffer_ui8(TRKBuffer* buffer, u8* data, int count);

// serpoll.c
extern void* gTRKInputPendingPtr;
int TRKTestForPacket(void);
void TRKGetInput(void);
void TRKProcessInput(int bufferIdx);
DSError TRKInitializeSerialHandler(void);
DSError TRKTerminateSerialHandler(void);

// usr_put.c
void usr_put_initialize(void);

// dispatch.c
DSError TRKInitializeDispatcher(void);
DSError TRKDispatchMessage(TRKBuffer* buffer);

// msghndlr.c
BOOL GetTRKConnected(void);
void SetTRKConnected(BOOL connected);
DSError TRKStandardACK(TRKBuffer* buffer, MessageCommandID commandID, DSReplyError replyError);
DSError TRKDoUnsupported(TRKBuffer* buffer);
DSError TRKDoConnect(TRKBuffer* buffer);
DSError TRKDoDisconnect(TRKBuffer* buffer);
DSError TRKDoReset(TRKBuffer* buffer);
DSError TRKDoVersions(TRKBuffer* buffer);
DSError TRKDoSupportMask(TRKBuffer* buffer);
DSError TRKDoCPUType(TRKBuffer* buffer);
DSError TRKDoReadMemory(TRKBuffer* buffer);
DSError TRKDoWriteMemory(TRKBuffer* buffer);
DSError TRKDoReadRegisters(TRKBuffer* buffer);
DSError TRKDoWriteRegisters(TRKBuffer* buffer);
DSError TRKDoFlushCache(TRKBuffer* buffer);
DSError TRKDoContinue(TRKBuffer* buffer);
DSError TRKDoStep(TRKBuffer* buffer);
DSError TRKDoStop(TRKBuffer* buffer);
DSError TRKDoSetOption(TRKBuffer* buffer);

// support.c
DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 minReplyLength, u32 retries, BOOL timeout);

// mutex_TRK.c
DSError TRKInitializeMutex(void* mutex);
DSError TRKAcquireMutex(void* mutex);
DSError TRKReleaseMutex(void* mutex);

// mem_TRK.c
void* TRK_memcpy(void* dest, const void* src, size_t count);
void* TRK_memset(void* dest, int val, size_t count);

// targimpl.c
DSError TRKTargetAccessMemory(void* data, u32 start, size_t* length, MemoryAccessOptions accessOptions,
                              BOOL read);
DSError TRKTargetAccessDefault(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                               BOOL read);
DSError TRKTargetAccessFP(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                          BOOL read);
DSError TRKTargetAccessExtended1(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                                 BOOL read);
DSError TRKTargetAccessExtended2(u32 firstRegister, u32 lastRegister, TRKBuffer* b, size_t* registersLength,
                                 BOOL read);
DSError TRKTargetVersions(DSVersions* versions);
DSError TRKTargetSupportMask(u8* mask);
DSError TRKTargetCPUType(DSCPUType* cpuType);
DSError TRKTargetFlushCache(u8 options, void* start, void* end);
DSError TRKTargetInterrupt(TRKEvent* event);
DSError TRKTargetSupportRequest(void);
DSError TRKTargetAddStopInfo(TRKBuffer* buffer);
DSError TRKTargetAddExceptionInfo(TRKBuffer* buffer);
DSError TRKTargetSingleStep(u32 count, BOOL stepOver);
DSError TRKTargetStepOutOfRange(u32 rangeStart, u32 rangeEnd, BOOL stepOver);
u32 TRKTargetGetPC(void);
DSError TRKTargetStop(void);
BOOL TRKTargetStopped(void);
void TRKTargetSetStopped(unsigned int stopped);
void TRKTargetSetInputPendingPtr(void* ptr);
void TRKSwapAndGo(void);

// dolphin_trk.c
DSError TRKInitializeTarget(void);
void __TRK_reset(void);

// dolphin_trk_glue.c
DSError TRKInitializeIntDrivenUART(u32, u32, u32, u8**);
int TRKReadUARTPoll(s8* byte);
UARTError WriteUART1(s8 byte);
UARTError WriteUARTFlush(void);
void TRK_board_display(char* str);
void UnreserveEXI2Port(void);
void ReserveEXI2Port(void);

// targcont.c
unsigned int TRKTargetContinue(void);

// target_options.c
void SetUseSerialIO(u8 sio);
u8 GetUseSerialIO(void);

// Appends one byte to a message buffer.
static inline DSError TRKAppendBuffer1_ui8(TRKBuffer* buffer, const u8 data) {
    if (buffer->position >= TRKMSGBUF_SIZE) {
        return DS_MessageBufferOverflow;
    }

    buffer->data[buffer->position++] = data;
    buffer->length++;
    return DS_NoError;
}

#ifdef __cplusplus
}
#endif

#endif
