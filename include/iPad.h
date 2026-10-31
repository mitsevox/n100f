#ifndef IPAD_H
#define IPAD_H

#include "types.h"

struct _tagxPad;
struct _tagxRumble;

void iPadKill();
void iPadStartRumble(_tagxPad* pad, _tagxRumble* rumble);
void iPadStopRumble(_tagxPad* pad);
void iPadRumbleFx(_tagxPad* p, _tagxRumble* r, F32 time_passed);
S32 iPadUpdate(_tagxPad* pad, U32* on);
_tagxPad* iPadEnable(_tagxPad* pad, S16 port);
S32 iPadInit();

#endif
