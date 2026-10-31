#include "xBase.h"

void xBaseInit(xBase* xb, p2BaseAsset* asset)
{
    xb->id = asset->id;
    xb->baseType = asset->baseType;
    xb->baseFlags = asset->baseFlags;
    xb->linkCount = asset->linkCount;
    xb->link = NULL;
    xb->baseFlags |= 0x4;
}

void xBaseSetup(xBase* xb)
{
}

void xBaseSave(xBase* ent, xSerial* s)
{
    if (ent->baseFlags & 0x1)
    {
        s->Write_b1(1);
    }
    else
    {
        s->Write_b1(0);
    }
}

void xBaseLoad(xBase* ent, xSerial* s)
{
    S32 b = 0;
    s->Read_b1(&b);

    if (b)
    {
        ent->baseFlags |= 0x1;
    }
    else
    {
        ent->baseFlags &= ~0x1;
    }
}

// Restores the asset's base flags; bit 4 stays set.
void xBaseReset(xBase* xb, p2BaseAsset* asset)
{
    xb->baseFlags = asset->baseFlags;
    xb->baseFlags |= 0x4;
}
