#ifndef _DOLPHIN_AR_H_
#define _DOLPHIN_AR_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ARQRequest ARQRequest;

typedef void (*ARCallback)(void);
typedef void (*ARQCallback)(u32 pointerToARQRequest);

struct ARQRequest {
    ARQRequest* next;
    u32 owner;
    u32 type;
    u32 priority;
    u32 source;
    u32 dest;
    u32 length;
    ARQCallback callback;
};

#define ARQ_PRIORITY_LOW 0
#define ARQ_PRIORITY_HIGH 1

ARCallback ARRegisterDMACallback(ARCallback callback);
void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length);
u32 ARAlloc(u32 length);
u32 ARFree(u32* length);
u32 ARInit(u32* stack_index_addr, u32 num_entries);

void ARQInit(void);
void ARQPostRequest(ARQRequest* request, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, ARQCallback callback);
void ARQRemoveRequest(ARQRequest* request);
void ARQFlushQueue(void);

#ifdef __cplusplus
}
#endif

#endif
