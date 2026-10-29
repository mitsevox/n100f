#ifndef ZFMV_H
#define ZFMV_H

#include "types.h"

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

char* zFMVFileGetName(eFMVFile fileEnum);
U32 zFMVPlay(char* filename, U32 buttons, F32 time);

#endif
