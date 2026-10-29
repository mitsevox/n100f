#ifndef ZFX_H
#define ZFX_H

#include "types.h"

#include <rpworld.h>

// Defined outside the reconstructed units: its .sbss slot between zFMV and zGame stays
// unassigned (see evidence.tsv).
extern U32 gFXSurfaceFlags;

void zFX_SceneEnter(RpWorld* world);
void zFX_SceneExit(RpWorld* world);

#endif
