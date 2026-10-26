#include "xutil.h"

#include "iMath.h"

static S32 g_xutilinit;
static S32 g_crc_needinit = 1;
static U32 g_crc32_table[256] = {};

S32 xUtilStartup()
{
    if (!g_xutilinit++)
    {
        xUtil_crc_init();
    }

    return g_xutilinit;
}

S32 xUtilShutdown()
{
    g_xutilinit--;
    return g_xutilinit;
}

U32 xUtil_crc_init()
{
    S32 i, j;
    U32 crc_accum;

    if (g_crc_needinit)
    {
        for (i = 0; i < 256; i++)
        {
            crc_accum = (U32)i << 24;

            for (j = 0; j < 8; j++)
            {
                if (crc_accum & (1 << 31))
                {
                    crc_accum = (crc_accum << 1) ^ 0x04C11DB7;
                }
                else
                {
                    crc_accum = (crc_accum << 1);
                }
            }

            g_crc32_table[i] = crc_accum;
        }

        g_crc_needinit = 0;
    }

    return 0xFFFFFFFF;
}

U32 xUtil_crc_update(U32 crc_accum, char* data, S32 datasize)
{
    S32 i, j;

    if (g_crc_needinit)
    {
        xUtil_crc_init();
    }

    for (i = 0; i < datasize; i++)
    {
        j = ((crc_accum >> 24) ^ *data++) & 0xff;
        crc_accum = (crc_accum << 8) ^ g_crc32_table[j];
    }

    return crc_accum;
}

S32 xUtil_yesno()
{
    return iurand() <= 0.5f ? 1 : 0;
}

S32 xUtil_yesno(F32 wt_yes)
{
    if (0.0f == wt_yes)
    {
        return 0;
    }

    if (1.0f == wt_yes)
    {
        return 1;
    }

    return iurand() <= wt_yes;
}
