#include "types.h"
#include "zGlobals.h"
#include "zScene.h"
#include "xString.h"
#include <stdio.h>

enum eFMVFile
{
    eFMVFile_HeavyIronLOGO = 0,
    eFMVFile_SDTitleSequence = 1,
    eFMVFile_Demo1 = 2,
    eFMVFile_Demo2 = 3,
    eFMVFile_Demo3 = 4,
    eFMVFile_Demo4 = 5,
    eFMVFile_Demo5 = 6,
    eFMVFile_Demo6 = 7,
    eFMVFile_Demo7 = 8,
    eFMVFile_Demo8 = 9,
    eFMVFile_Demo9 = 10,
    eFMVFile_Demo10 = 11,
    eFMVFile_Demo11 = 12,
    eFMVFile_Demo12 = 13,
    eFMVFile_Demo13 = 14,
    eFMVFile_Demo14 = 15,
    eFMVFile_Demo15 = 16,
    eFMVFile_THQLOGO = 17,
    eFMVFile_WarnerBrosLOGO = 18,
    eFMVFile_WBExtra0 = 19,
    eFMVFile_WBExtra1 = 20,
    eFMVFile_WBExtra2 = 21,
    eFMVFile_WBExtra3 = 22,
    eFMVFile_WBExtra4 = 23,
    eFMVFile_Credits = 24,
    eFMVFile_EndCredits = 25,
    eFMVFile_RenderWareLOGO = 26,
    eFMVFile_Cin1 = 27,
    eFMVFile_Cin2 = 28,
    eFMVFile_Cin3 = 29,
    eFMVFile_Cin4 = 30,
    eFMVFile_Cin5 = 31,
    eFMVFile_Cin6 = 32,
    eFMVFile_Cin7 = 33,
    eFMVFile_Cin8 = 34,
    eFMVFile_Cin9 = 35,
    eFMVFile_Cin10 = 36,
    eFMVFile_Cin11 = 37,
    eFMVFile_Cin12 = 38,
    eFMVFile_Cin13 = 39,
    eFMVFile_Cin14 = 40,
    eFMVFileCount = 41,
};

struct zFMVFile
{
    eFMVFile type;
    char fileName[32];
    U32 fileNameID;
};

static zFMVFile zFMVFileTable[eFMVFileCount] =
{
    { eFMVFile_HeavyIronLOGO, "HILogo", 0 },
    { eFMVFile_SDTitleSequence, "Intro", 0 },
    { eFMVFile_Demo1, "demo/demo1", 0 },
    { eFMVFile_Demo2, "demo/demo2", 0 },
    { eFMVFile_Demo3, "Intro", 0 },
    { eFMVFile_Demo4, "demo/demo1", 0 },
    { eFMVFile_Demo5, "demo/demo2", 0 },
    { eFMVFile_Demo6, "Intro", 0 },
    { eFMVFile_Demo7, "demo/demo1", 0 },
    { eFMVFile_Demo8, "demo/demo2", 0 },
    { eFMVFile_Demo9, "Intro", 0 },
    { eFMVFile_Demo10, "demo/demo1", 0 },
    { eFMVFile_Demo11, "demo/demo2", 0 },
    { eFMVFile_Demo12, "Intro", 0 },
    { eFMVFile_Demo13, "demo/demo1", 0 },
    { eFMVFile_Demo14, "demo/demo2", 0 },
    { eFMVFile_Demo15, "Intro", 0 },
    { eFMVFile_THQLOGO, "THQLogo", 0 },
    { eFMVFile_WarnerBrosLOGO, "WBLogo", 0 },
    { eFMVFile_WBExtra0, "SDTrailr", 0 },
    { eFMVFile_WBExtra1, "HWTrailr", 0 },
    { eFMVFile_WBExtra2, "HILogo", 0 },
    { eFMVFile_WBExtra3, "HILogo", 0 },
    { eFMVFile_WBExtra4, "HILogo", 0 },
    { eFMVFile_Credits, "Credits1", 0 },
    { eFMVFile_EndCredits, "Credits2", 0 },
    { eFMVFile_RenderWareLOGO, "RW4", 0 },
    { eFMVFile_Cin1, "cin1", 0 },
    { eFMVFile_Cin2, "cin2", 0 },
    { eFMVFile_Cin3, "cin3", 0 },
    { eFMVFile_Cin4, "cin4", 0 },
    { eFMVFile_Cin5, "cin5", 0 },
    { eFMVFile_Cin6, "cin6", 0 },
    { eFMVFile_Cin7, "cin7", 0 },
    { eFMVFile_Cin8, "cin8", 0 },
    { eFMVFile_Cin9, "cin9", 0 },
    { eFMVFile_Cin10, "cin10", 0 },
    { eFMVFile_Cin11, "cin11", 0 },
    { eFMVFile_Cin12, "cin_mmpd", 0 },
    { eFMVFile_Cin13, "cin_mmsc", 0 },
    { eFMVFile_Cin14, "cin_shbr", 0 },
};
extern "C" {
U32 lbl_8025EEC8;
}
extern "C" void fn_8003D49C();
extern "C" void fn_8003D41C();
U32 iFMVPlay(char* filename, U32 buttons, F32 time);

// Play a movie and set a flag after Credits2 in scene s005.
U32 zFMVPlay(char* filename, U32 buttons, F32 time)
{
    U32 ret;
    char c[64];
    sprintf(c, "%s/%s%s", "FMV", filename, ".bik");
    fn_8003D49C();
    ret = iFMVPlay(c, buttons, time);
    fn_8003D41C();
    if (xStricmp(filename, "Credits2") == 0 &&
        globals.sceneCur->sceneID == zSceneIDFromName("s005"))
    {
        lbl_8025EEC8 = 1;
    }
    return ret;
}

// Return the name stored in the movie table for the requested entry.
char* zFMVFileGetName(eFMVFile fileEnum)
{
    return zFMVFileTable[fileEnum].fileName;
}
