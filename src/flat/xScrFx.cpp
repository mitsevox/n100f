#include "xScrFx.h"
#include "zMenu.h"

static F32 mLetterboxTO;

void xScrFxLetterbox(S32 enable)
{
    if (zMenuRunning())
    {
        enable = 0;
    }

    if (enable)
    {
        mLetterboxTO = 16.0f;
    }
    else
    {
        mLetterboxTO = 0.0f;
    }
}

S32 xScrFxIsLetterbox()
{
    return mLetterboxTO > 0.0f;
}
