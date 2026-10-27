#ifndef TRK_MINNOW_DOLPHIN_TRK_H
#define TRK_MINNOW_DOLPHIN_TRK_H

// MetroTRK declarations shared by the TRK_MINNOW_DOLPHIN units, limited to what
// reconstructed units use (docs/libraries.md).

#include "dolphin/types.h"
#include "dolphin/os/OSInterrupt.h"
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

typedef struct Default_PPC {
    u32 GPR[32];   // 0x0
    u32 PC;        // 0x80
    u32 LR;        // 0x84
    u32 CR;        // 0x88
    u32 CTR;       // 0x8C
    u32 XER;       // 0x90
} Default_PPC;

typedef struct Float_PPC {
    u64 FPR[32];   // 0x0
    u64 FPSCR;     // 0x100
    u64 FPECR;     // 0x108
} Float_PPC;

// Special registers saved by TRKSaveExtended1Block, in its store order.
typedef struct Extended1_PPC_6xx_7xx {
    u32 SR[16];      // 0x0
    u32 TBL;         // 0x40
    u32 TBU;         // 0x44
    u32 HID0;        // 0x48
    u32 HID1;        // 0x4C
    u32 MSR;         // 0x50
    u32 PVR;         // 0x54
    u32 IBAT0U;      // 0x58
    u32 IBAT0L;
    u32 IBAT1U;
    u32 IBAT1L;
    u32 IBAT2U;
    u32 IBAT2L;
    u32 IBAT3U;
    u32 IBAT3L;
    u32 DBAT0U;      // 0x78
    u32 DBAT0L;
    u32 DBAT1U;
    u32 DBAT1L;
    u32 DBAT2U;
    u32 DBAT2L;
    u32 DBAT3U;
    u32 DBAT3L;
    u32 DMISS;       // 0x98
    u32 DCMP;
    u32 HASH1;
    u32 HASH2;
    u32 IMISS;
    u32 ICMP;
    u32 RPA;
    u32 SDR1;        // 0xB4
    u32 DAR;
    u32 DSISR;
    u32 SPRG0;
    u32 SPRG1;
    u32 SPRG2;
    u32 SPRG3;
    u32 DEC;         // 0xD0
    u32 IABR;
    u32 EAR;
    u32 DABR;        // 0xDC
    u32 PMC1;
    u32 PMC2;
    u32 PMC3;
    u32 PMC4;
    u32 SIA;
    u32 MMCR0;
    u32 MMCR1;
    u32 THRM1;
    u32 THRM2;
    u32 THRM3;
    u32 ICTC;
    u32 L2CR;
    u32 UMMCR2;      // 0x110
    u32 UBAMR;
    u32 UMMCR0;
    u32 UPMC1;
    u32 UPMC2;
    u32 USIA;
    u32 UMMCR1;
    u32 UPMC3;
    u32 UPMC4;
    u32 USDA;
    u32 MMCR2;       // 0x138
    u32 BAMR;
    u32 SDA;
    u32 MSSCR0;
    u32 MSSCR1;
    u32 PIR;
    u32 exceptionID; // 0x150
    u32 GQR[8];      // 0x154
    u32 HID2;        // 0x174
    u32 WPAR;
    u32 DMA_U;
    u32 DMA_L;
} Extended1_PPC_6xx_7xx;

typedef struct Extended2_PPC_6xx_7xx {
    u32 PSR[32][2];
} Extended2_PPC_6xx_7xx;

typedef struct ProcessorState_PPC {
    Default_PPC Default;              // 0x0
    Float_PPC Float;                  // 0x98
    Extended1_PPC_6xx_7xx Extended1;  // 0x1A8
    Extended2_PPC_6xx_7xx Extended2;  // 0x32C
    u32 transport_handler_saved_ra;   // 0x42C
} ProcessorState_PPC;

// Set by the debugger to have TRKRestoreExtended1Block write back the time base and decrementer.
typedef struct ProcessorRestoreFlags_PPC {
    u8 TBR;
    u8 DEC;
    u8 linker_padding[0x9 - 0x2];
} ProcessorRestoreFlags_PPC;

typedef struct TRKState_PPC {
    u32 GPR[32];         // 0x0
    u32 LR;              // 0x80
    u32 CTR;             // 0x84
    u32 XER;             // 0x88
    u32 MSR;             // 0x8C
    u32 DAR;             // 0x90
    u32 DSISR;           // 0x94
    BOOL stopped;        // 0x98
    BOOL inputActivated; // 0x9C
    u8* inputPendingPtr; // 0xA0
} TRKState_PPC;

// PowerPC exception vector offsets.
enum {
    PPC_SystemReset = 0x100,
    PPC_MachineCheck = 0x200,
    PPC_DataStorage = 0x300,
    PPC_InstructionStorage = 0x400,
    PPC_ExternalInterrupt = 0x500,
    PPC_Alignment = 0x600,
    PPC_Program = 0x700,
    PPC_FloatingPointUnavailable = 0x800,
    PPC_Decrementer = 0x900,
    PPC_SystemCall = 0xC00,
    PPC_Trace = 0xD00,
    PPC_PerformanceMonitor = 0xF00,
    PPC_InstructionAddressBreakpoint = 0x1300,
    PPC_SystemManagementInterrupt = 0x1400,
    PPC_ThermalManagementInterrupt = 0x1700,
};

extern ProcessorState_PPC gTRKCPUState;
extern TRKState_PPC gTRKState;
extern ProcessorRestoreFlags_PPC gTRKRestoreFlags;

// mainloop.c
void TRKNubMainLoop(void);

// nubevent.c
DSError TRKInitializeEventQueue(void);
BOOL TRKGetNextEvent(TRKEvent* event);
DSError TRKPostEvent(TRKEvent* event);
void TRKConstructEvent(TRKEvent* event, int eventType);
void TRKDestructEvent(TRKEvent* event);

// nubinit.c
extern BOOL gTRKBigEndian;
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
DSError TRKAppendBuffer1_ui64(TRKBuffer* buffer, const u64 data);
DSError TRKAppendBuffer_ui32(TRKBuffer* buffer, const u32* data, int count);
DSError TRKAppendBuffer_ui8(TRKBuffer* buffer, const u8* data, int count);
DSError TRKReadBuffer(TRKBuffer* msg, void* data, size_t length);
DSError TRKReadBuffer1_ui8(TRKBuffer* buffer, u8* data);
DSError TRKReadBuffer1_ui16(TRKBuffer* buffer, u16* data);
DSError TRKReadBuffer1_ui32(TRKBuffer* buffer, u32* data);
DSError TRKReadBuffer1_ui64(TRKBuffer* buffer, u64* data);
DSError TRKReadBuffer_ui8(TRKBuffer* buffer, u8* data, int count);
DSError TRKReadBuffer_ui32(TRKBuffer* buffer, u32* data, int count);

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
DSError TRKSuppAccessFile(u32 file_handle, u8* data, size_t* count, u8* io_result, BOOL need_reply,
                          BOOL read);
DSError HandleOpenFileSupportRequest(const char* path, u8 replyError, u32* handle, u8* ioResult);
DSError HandleCloseFileSupportRequest(int handle, u8* ioResult);
DSError HandlePositionFileSupportRequest(u32 handle, u32* position, u8 mode, u8* ioResult);
DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 minReplyLength, u32 retries, BOOL timeout);

// notify.c
DSError TRKDoNotifyStopped(MessageCommandID cmd);

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
u32 __TRK_get_MSR(void);
void TRKInterruptHandler(void);
void TRKPostInterruptEvent(void);

// flush_cache.c
void TRK_flush_cache(u32 start, int length);

// __exception.s
extern u8 gTRKInterruptVectorTable[];

// dolphin_trk.c
void __TRK_reset(void);
void InitMetroTRK(void);
void EnableMetroTRKInterrupts(void);
u32 TRKTargetTranslate(u32 addr);
DSError TRKInitializeTarget(void);

// main_TRK.c
DSError TRK_main(void);

// mpc_7xx_603e.c
void TRKSaveExtended1Block(void);
void TRKRestoreExtended1Block(void);
u8 TRKTargetCPUMinorType(void);

// dolphin_trk_glue.c
// The hardware ID InitMetroTRKCommTable takes for a GDEV board.
#define HARDWARE_GDEV 1

typedef void (*DBCommInitFunc)(u8** inputPendingPtrRef, __OSInterruptHandler handler);
typedef int (*DBCommFunc)(void);
typedef int (*DBCommReadFunc)(void* bytes, u32 length);
typedef int (*DBCommWriteFunc)(const void* bytes, u32 length);

typedef struct DBCommTable {
    DBCommInitFunc initialize_func;
    DBCommFunc init_interrupts_func;
    DBCommFunc peek_func;
    DBCommReadFunc read_func;
    DBCommWriteFunc write_func;
    DBCommFunc open_func;
    DBCommFunc close_func;
} DBCommTable;

void TRKLoadContext(OSContext* ctx, u32 exception);
void TRKEXICallBack(__OSInterrupt interrupt, OSContext* ctx);
int InitMetroTRKCommTable(int hwId);
void EnableEXI2Interrupts(void);
void TRKUARTInterruptHandler(void);
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
