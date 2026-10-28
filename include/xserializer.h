#ifndef XSERIALIZER_H
#define XSERIALIZER_H

#include "types.h"

struct st_SERIAL_CLIENTINFO;
struct st_SERIAL_PERCID_SIZE;

struct xSerial
{
    U32 idtag;
    S32 baseoff;
    st_SERIAL_CLIENTINFO* ctxtdata;
    S32 warned;
    S32 curele;
    S32 bitidx;
    S32 bittally;

    ~xSerial();
    void operator delete(void*)
    {
    }

    S32 Read(U8* buf);
    S32 Read(S16* buf);
    S32 Read(U32* buf);
    S32 Read(F32* buf);
    S32 Read_b1(S32* bits);
    S32 Read_b2(S32* bits);
    S32 Write(U8 data);
    S32 Write(S16 data);
    S32 Write(U32 data);
    S32 Write(F32 data);
    S32 Write_b1(S32 bits);
    S32 Write_b2(S32 bits);
};

S32 xSerialStartup(S32 count, st_SERIAL_PERCID_SIZE* sizeinfo);
void xSerialWipeMainBuffer();

#endif
