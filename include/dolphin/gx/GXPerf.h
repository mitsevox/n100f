#ifndef _DOLPHIN_GX_GXPERF_H_
#define _DOLPHIN_GX_GXPERF_H_

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1);
void GXClearGPMetric(void);

#ifdef __cplusplus
}
#endif

#endif
