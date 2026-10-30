#ifndef ISCRFX_H
#define ISCRFX_H

#include "types.h"

#include <rwcore.h>

struct iColor_tag;

void iScrFxSetBlackoutFrames(U32 count);
void iScrFxCameraEndScene(RwCamera* pCamera);
void iScrFxBegin();
void iScrFxEnd();
void iScrFxDrawFade(RwCamera* pCamera, iColor_tag* col);
void iScrFxDrawLetterbox(RwCamera* pCamera, S32 y_offset);

#endif
