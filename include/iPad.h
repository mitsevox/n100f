#ifndef IPAD_H
#define IPAD_H

#include "types.h"

struct _tagxPad;

void iPadKill();
void iPadStopRumble(_tagxPad* pad);
_tagxPad* iPadEnable(_tagxPad* pad, S16 port);
S32 iPadInit();

#endif
