#ifndef XSERIALIZER_H
#define XSERIALIZER_H

#include "types.h"

struct st_SERIAL_CLIENTINFO;

struct xSerial
{
    U32 idtag;
    S32 baseoff;
    st_SERIAL_CLIENTINFO* ctxtdata;
    S32 warned;
    S32 curele;
    S32 bitidx;
    S32 bittally;

    S32 Read_b1(S32* bits);
    S32 Write_b1(S32 bits);
};

#endif
