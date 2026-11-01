#include "iDebug.h"

#include "iFile.h"
#include "iTime.h"
#include "xMemMgr.h"

#include <rwcore.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" RwImage* RwImageSetFromRaster(RwImage* image, RwRaster* raster);

struct tagBITMAPFILEHEADER
{
    S32 bfSize;
    S16 bfReserved1;
    S16 bfReserved2;
    S32 bfOffBits;
};

struct tagBITMAPINFOHEADER
{
    S32 biSize;
    S32 biWidth;
    S32 biHeight;
    S16 biPlanes;
    S16 biBitCount;
    S32 biCompression;
    S32 biSizeImage;
    S32 biXPelsPerMeter;
    S32 biYPelsPerMeter;
    S32 biClrUsed;
    S32 biClrImportant;
};

// The two pad bytes put the "BM" tag right before the file header, so the 54 bytes from bfType
// are the file's header with every field aligned in memory.
struct BmpMegaHeader
{
    char prepad[2];
    char bfType[2];
    tagBITMAPFILEHEADER bmFile;
    tagBITMAPINFOHEADER bmInfo;
};

char gScreenDumpPath[256];
U32 gScreenDump;
static S32 sDumpFileNumber;

// Saves the camera's frame buffer as a numbered 640x480 24-bit BMP, on the host PC when a dump
// path is set. Rows are written bottom-up as the format wants, in 30 KB chunks.
void iDebugScreenDump()
{
    RwRaster* camRas;
    S32 width;
    S32 height;
    RwImage* image;
    U8* oldCamPixels;
    U8* pixels;
    tag_xFile file;
    U32 x;
    U32 y;
    char fname[64];
    U8* bp;
    U8* buf;
    U8* imp;
    iTime t0;
    iTime t1;
    BmpMegaHeader mhdr;
    U8* p;

    camRas = RwCameraGetRaster(RwCameraGetCurrentCamera());

    if (camRas)
    {
        width = camRas->width;
        height = camRas->height;
        oldCamPixels = camRas->cpPixels;
        pixels = (U8*)malloc((camRas->depth >> 3) * (width * height));

        if (pixels)
        {
            camRas->cpPixels = pixels;
        }
        else
        {
            return;
        }

        if (!RwRasterLock(camRas, 0, rwRASTERLOCKREAD))
        {
            camRas->cpPixels = oldCamPixels;
            free(pixels);
        }

        image = RwImageCreate(width, height, 32);

        if (image)
        {
            RwImageAllocatePixels(image);
            RwImageSetFromRaster(image, camRas);
            RwRasterUnlock(camRas);
            camRas->cpPixels = oldCamPixels;
            free(pixels);
        }
        else
        {
            RwRasterUnlock(camRas);
            camRas->cpPixels = oldCamPixels;
            free(pixels);
            return;
        }
    }
    else
    {
        return;
    }

    t0 = iTimeGet();

    if (gScreenDumpPath[0])
    {
        sprintf(fname, "host:%sdump%04d.bmp", gScreenDumpPath, sDumpFileNumber++);
        iFileOpen(fname, 0xE, &file);
    }
    else
    {
        sprintf(fname, "dump%04d.bmp", sDumpFileNumber++);
        iFileOpen(fname, 0xE, &file);
    }

    mhdr.bfType[0] = 'B';
    mhdr.bfType[1] = 'M';
    mhdr.bmFile.bfSize = 640 * 480 * 3 + 54;
    mhdr.bmFile.bfReserved1 = 0;
    mhdr.bmFile.bfReserved2 = 0;
    mhdr.bmFile.bfOffBits = 54;
    mhdr.bmInfo.biSize = sizeof(tagBITMAPINFOHEADER);
    mhdr.bmInfo.biWidth = 640;
    mhdr.bmInfo.biHeight = 480;
    mhdr.bmInfo.biPlanes = 1;
    mhdr.bmInfo.biBitCount = 24;
    mhdr.bmInfo.biCompression = 0;
    mhdr.bmInfo.biSizeImage = 0;
    mhdr.bmInfo.biXPelsPerMeter = 0;
    mhdr.bmInfo.biYPelsPerMeter = 0;
    mhdr.bmInfo.biClrUsed = 0;
    mhdr.bmInfo.biClrImportant = 0;
    iFileWrite(&file, mhdr.bfType, 54);

    buf = (U8*)xMemPushTemp(0x7800);
    p = image->cpPixels;
    bp = buf;

    for (y = 0; y < 480; y++)
    {
        imp = p + (480 - y) * (640 * 4);

        for (x = 0; x < 640; x++)
        {
            bp[0] = imp[2];
            bp[1] = imp[1];
            bp[2] = imp[0];
            imp += 4;
            bp += 3;
        }

        if ((U32)(bp - buf) >= 0x7800)
        {
            iFileWrite(&file, buf, 0x7800);
            bp = buf;
        }
    }

    xMemPopTemp(buf);
    RwImageDestroy(image);
    iFileClose(&file);

    t1 = iTimeGet();
    iTimeDiffSec(t0, t1);

    gScreenDump = 0;
}
