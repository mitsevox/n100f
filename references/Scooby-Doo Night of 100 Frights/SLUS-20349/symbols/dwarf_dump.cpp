// Per-source-file DWARF 1 dump: globals, statics and functions
// with parameters, locals (register or stack slot) and scopes.

// ==========================================================================
// E:\SD2\Game\flat\fakeprintf.c
// ==========================================================================
static char *sbend; // 0x00357484
static char *sb; // 0x00357480
static char sneaky_buf[12288]; // 0x00358700
// 0x001000D0 - 0x001001C8 (0xF8)
signed int printf(char *fmt) {
    { // 0x001000D0 - 0x001001C8
        signed int rc; // r16
        signed int nc; // r2
        signed int space; // r4
        char *vargs; // r2
        static signed int nw; // 0x00357C00
        static signed char init; // 0x00357C04
    }
}


// ==========================================================================
// C:\usr\local\SCE\EE\lib\crt0.s
// ==========================================================================

// ==========================================================================
// E:\SD2\Game\flat\iMath.cpp
// ==========================================================================
float cosSinPolynomial[16]; // 0x0028A180
static unsigned int rndseed; // 0x00357C08
// 0x00110A50 - 0x00110AA8 (0x58) iurand__Fv
float iurand() {
    { // 0x00110A50 - 0x00110AA8
    }
}

// 0x00110AB0 - 0x00110AD0 (0x20) irand__Fv
unsigned int irand() {
    { // 0x00110AB0 - 0x00110AD0
    }
}

// 0x00110AD0 - 0x00110AD8 (0x8) isrand__FUi
void isrand(unsigned int seed) {
    { // 0x00110AD0 - 0x00110AD8
    }
}

// 0x00110AE0 - 0x00110AFC (0x1C) p2sqrt__Ff
float p2sqrt(float x) {
    { // 0x00110AE0 - 0x00110AFC
        float r; // r29+0x0
    }
}

// 0x00110B00 - 0x00110B08 (0x8) iMathExit__Fv
void iMathExit() {
    { // 0x00110B00 - 0x00110B08
    }
}

// 0x00110B10 - 0x00110B54 (0x44) iMathInit__Fv
void iMathInit() {
    { // 0x00110B10 - 0x00110B54
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iFile.cpp
// ==========================================================================
void (iFileCallbackHandler)(signed int); // 0x00111490
static signed int num_open_files; // 0x00357C10
void (*sCallback)(signed int); // 0x00357C0C
class sceCdRMode sCDReadMode; // 0x00357488
// 0x00110B60 - 0x00110B78 (0x18) iFileGetSectorSize__F14xFileDriveType
unsigned int iFileGetSectorSize(enum xFileDriveType drive) {
    { // 0x00110B60 - 0x00110B78
    }
}

// 0x00110B80 - 0x00110C04 (0x84) iFileReadStop__Fv
unsigned int iFileReadStop() {
    { // 0x00110B80 - 0x00110C04
        signed int ret; // r16
    }
}

// 0x00110C10 - 0x00110CBC (0xAC) iFileReadSectorStatus__Fv
enum IFILE_READSECTOR_STATUS iFileReadSectorStatus() {
    { // 0x00110C10 - 0x00110CBC
        enum IFILE_READSECTOR_STATUS status; // r2
        signed int err; // r2
        signed int inprog; // r2
    }
}

// 0x00110CC0 - 0x00110D98 (0xD8) iFileReadSectors__FUiUiPvPFi_v
unsigned int iFileReadSectors(unsigned int start, unsigned int nsectors, void *buf, void (*callback)(signed int)) {
    { // 0x00110CC0 - 0x00110D98
        unsigned int num; // r16
    }
}

// 0x00110DA0 - 0x00110DC8 (0x28) iFileGetInfo__FP9tag_xFilePUiPUi
void iFileGetInfo(class tag_xFile *file, unsigned int *starting_sector, unsigned int *size_in_bytes) {
    { // 0x00110DA0 - 0x00110DC8
        class tag_iFile *ps; // r4
    }
}

// 0x00110DD0 - 0x00110ED0 (0x100) iFileFind__FPCcP9tag_xFile
unsigned int iFileFind(char *name, class tag_xFile *file) {
    { // 0x00110DD0 - 0x00110ED0
        class tag_iFile *ps; // r16
        signed int ret; // r2
        char fullname[128]; // r29+0x30
    }
}

// 0x00110ED0 - 0x00110F98 (0xC8) iFileGetSize__FP9tag_xFile
unsigned int iFileGetSize(class tag_xFile *file) {
    { // 0x00110ED0 - 0x00110F98
        signed int rc; // r2
        class tag_iFile *ps; // r18
        signed int pos; // r17
        signed int size; // r16
    }
}

// 0x00110FA0 - 0x00111000 (0x60) iFileClose__FP9tag_xFile
unsigned int iFileClose(class tag_xFile *file) {
    { // 0x00110FA0 - 0x00111000
        signed int ret; // r2
        class tag_iFile *ps; // r16
    }
}

// 0x00111000 - 0x0011102C (0x2C) iFileWrite__FP9tag_xFilePvUi
unsigned int iFileWrite(class tag_xFile *file, void *buf, unsigned int size) {
    { // 0x00111000 - 0x0011102C
        signed int num; // r2
    }
}

// 0x00111030 - 0x001110C8 (0x98) iFileRead__FP9tag_xFilePvUi
unsigned int iFileRead(class tag_xFile *file, void *buf, unsigned int size) {
    { // 0x00111030 - 0x001110C8
        signed int num; // r2
        class tag_iFile *ps; // r16
    }
}

// 0x001110D0 - 0x00111144 (0x74) iFileSeek__FP9tag_xFileii
signed int iFileSeek(class tag_xFile *file, signed int offset, signed int whence) {
    { // 0x001110D0 - 0x00111144
        signed int new_pos; // r2
        class tag_iFile *ps; // r3
    }
}

// 0x00111150 - 0x001112B8 (0x168) iFileOpen__FPCciP9tag_xFile
unsigned int iFileOpen(char *name, signed int flags, class tag_xFile *file) {
    { // 0x00111150 - 0x001112B8
        class tag_iFile *ps; // r16
    }
}

// 0x001112C0 - 0x00111488 (0x1C8) iFileLoad__FPcPUiPUi
unsigned int *iFileLoad(char *name, unsigned int *buffer, unsigned int *size) {
    { // 0x001112C0 - 0x00111488
        signed int ret; // r2
        signed int locsize; // r19
        signed int n; // r2
        signed int fd; // r18
        char fullpath[128]; // r29+0x50
    }
}

// 0x00111490 - 0x001114BC (0x2C) iFileCallbackHandler__Fi
static void iFileCallbackHandler(signed int cause) {
    { // 0x00111490 - 0x001114BC
    }
}

// 0x001114C0 - 0x001114E4 (0x24) iFileExit__Fv
void iFileExit() {
    { // 0x001114C0 - 0x001114E4
    }
}

// 0x001114F0 - 0x001114FC (0xC) iFileInit__Fv
void iFileInit() {
    { // 0x001114F0 - 0x001114FC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iPad.cpp
// ==========================================================================
class _tagTRCPadInfo gTrcPad[4]; // 0x003B1A10
static float mLargeMotor[4]; // 0x00362B00
static unsigned char mActAlign[6]; // 0x00357C20
static unsigned char mActDirect[6]; // 0x00357C18
static u_long128 mPadDmaBuffer[4][16]; // 0x00362700
static unsigned char mReadData[32]; // 0x003626C0
// 0x00111500 - 0x00111508 (0x8) iPadKill__Fv
void iPadKill() {
    { // 0x00111500 - 0x00111508
    }
}

// 0x00111510 - 0x00111604 (0xF4) iPadStartRumble__FP8_tagxPadP11_tagxRumble
void iPadStartRumble(class _tagxPad *pad, class _tagxRumble *rumble) {
    { // 0x00111510 - 0x00111604
    }
}

// 0x00111610 - 0x0011162C (0x1C) iPadStopRumble__FP8_tagxPad
void iPadStopRumble(class _tagxPad *pad) {
    { // 0x00111610 - 0x0011162C
    }
}

// 0x00111630 - 0x001116AC (0x7C) iPadRumbleFx__FP8_tagxPadP11_tagxRumblef
void iPadRumbleFx(class _tagxPad *p, class _tagxRumble *r, float time_passed) {
    { // 0x00111630 - 0x001116AC
        float scale; // r29+0x20
        float act; // 
    }
}

// 0x001116B0 - 0x001119D4 (0x324) iPadUpdate__FP8_tagxPadPUi
signed int iPadUpdate(class _tagxPad *pad, unsigned int *on) {
    { // 0x001116B0 - 0x001119D4
        unsigned int temp_on; // r2
        signed int temp; // r3
    }
}

// 0x001119E0 - 0x00111AF4 (0x114) iPadTRCUpdate__FP8_tagxPad
static void iPadTRCUpdate(class _tagxPad *pad) {
    { // 0x001119E0 - 0x00111AF4
    }
}

// 0x00111B00 - 0x00111B64 (0x64) iPadEnable__FP8_tagxPads
class _tagxPad *iPadEnable(class _tagxPad *pad, signed short port) {
    { // 0x00111B00 - 0x00111B64
        enum _tagPadInit pad_init; // r29+0x2c
    }
}

// 0x00111B70 - 0x00111E48 (0x2D8) iPadEnableGuts__FP8_tagxPadP11_tagPadInit
class _tagxPad *iPadEnableGuts(class _tagxPad *pad, enum _tagPadInit *pad_init) {
    { // 0x00111B70 - 0x00111E48
        signed int state5; // r2
        signed int state3; // r2
        signed int exid; // r2
        signed int id; // r18
        signed int state; // r2
    }
}

// 0x00111E50 - 0x00111F04 (0xB4) iPadInit__Fv
signed int iPadInit() {
    { // 0x00111E50 - 0x00111F04
        signed int errorCode; // r2
        char module[2][256]; // r29+0x20
        static char empty[1]; // 0x0035748C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xScrFx.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
signed int gGameState; // 0x003574F4
class iColor_tag g_BLACK; // 0x0034E420
class _xMat4x3 g_I3; // 0x00342E70
static float mLetterboxTO; // 0x00357C2C
static float mLetterboxO; // 0x00357C28
static class _xFadeData mFade; // 0x00362B10
// 0x00111F10 - 0x00111F1C (0xC) xScrFxIsFading__Fv
signed int xScrFxIsFading() {
    { // 0x00111F10 - 0x00111F1C
    }
}

// 0x00111F20 - 0x00111F48 (0x28) xScrFxIsLetterbox__Fv
signed int xScrFxIsLetterbox() {
    { // 0x00111F20 - 0x00111F48
    }
}

// 0x00111F50 - 0x00111F94 (0x44) xScrFxLetterbox__Fi
void xScrFxLetterbox(signed int enable) {
    { // 0x00111F50 - 0x00111F94
    }
}

// 0x00111FA0 - 0x00112014 (0x74) xScrFxFade__FP10iColor_tagP10iColor_tagfPFv_vii
void xScrFxFade(class iColor_tag *base, class iColor_tag *dest, float seconds, void (*callback)(), signed int hold) {
    { // 0x00111FA0 - 0x00112014
    }
}

// 0x00112020 - 0x001126DC (0x6BC) xScrFxUpdate__FP8RwCameraf
void xScrFxUpdate(class RwCamera *cam, float seconds) {
    { // 0x00112020 - 0x001126DC
        class iColor_tag col; // r29+0xec
        class _xMat4x3 old_head; // r29+0xa0
        class _xMat4x3 old; // r29+0x60
        float sc; // 
        float sc; // r29+0xf0
        float dog_z; // 
        float dog_y; // 
        signed int entering_scene; // r17
        float t; // r29+0xf0
    }
}

// 0x001126E0 - 0x001127A8 (0xC8) DrawScreenSizeRectangle__Fv
void DrawScreenSizeRectangle() {
    { // 0x001126E0 - 0x001127A8
        class RwVideoMode video_mode; // r29+0x10
        static unsigned short indices[4]; // 0x00357490
        static class RwSky2DVertex v[4]; // 0x00362B30
    }
}

// 0x001127B0 - 0x001127E0 (0x30) xScrFxInit__Fv
void xScrFxInit() {
    { // 0x001127B0 - 0x001127E0
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xPad.cpp
// ==========================================================================
class _tagxPad *gPlayerPad; // 0x00357C34
class _tagxPad *gDebugPad; // 0x00357C30
class _tagxPad mPad[4]; // 0x00362E30
static class _tagxRumble mRumbleList[32]; // 0x00362C30
// 0x001127E0 - 0x0011298C (0x1AC) xPadAddRumble__Fi14_tagRumbleTypefiUi
signed int xPadAddRumble(signed int idx, enum _tagRumbleType type, float time, signed int replace, unsigned int fxflags) {
    { // 0x001127E0 - 0x0011298C
        class _tagxRumble *last_r; // r18
        class _tagxRumble *r; // r3
        class _tagxPad *pad; // r17
        signed int appended; // r16
    }
}

// 0x00112990 - 0x00112A10 (0x80) xPadDestroyRumbleChain__Fi
void xPadDestroyRumbleChain(signed int idx) {
    { // 0x00112990 - 0x00112A10
    }
}

// 0x00112A10 - 0x00112A68 (0x58) xPadDestroyRumbleChain__FP8_tagxPad
void xPadDestroyRumbleChain(class _tagxPad *pad) {
    { // 0x00112A10 - 0x00112A68
        class _tagxRumble *r; // r16
    }
}

// 0x00112A70 - 0x00112A78 (0x8) xPadKill__Fv
void xPadKill() {
    { // 0x00112A70 - 0x00112A78
    }
}

// 0x00112A80 - 0x00112F9C (0x51C) xPadUpdate__Fif
signed int xPadUpdate(signed int idx, float time_passed) {
    { // 0x00112A80 - 0x00112F9C
        unsigned int fake_dpad; // r4
        class _tagxRumble *r; // r17
        class _tagxPad *p; // r16
        unsigned int new_on; // r29+0x4c
    }
}

// 0x00112FA0 - 0x00113068 (0xC8) xPadRumbleEnable__Fii
void xPadRumbleEnable(signed int idx, signed int enable) {
    { // 0x00112FA0 - 0x00113068
        class _tagxPad *p; // r16
    }
}

// 0x00113070 - 0x00113140 (0xD0) xPadEnable__Fi
class _tagxPad *xPadEnable(signed int idx) {
    { // 0x00113070 - 0x00113140
        class _tagxPad *p; // r2
    }
}

// 0x00113140 - 0x001131A0 (0x60) xPadInit__Fv
signed int xPadInit() {
    { // 0x00113140 - 0x001131A0
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xMath3.cpp
// ==========================================================================
class _xQuat g_IQ; // 0x00342EB0
class _xMat4x3 g_I3; // 0x00342E70
class _xVec3 g_NZ3; // 0x00000000
class _xVec3 g_NY3; // 0x00342E58
class _xVec3 g_NX3; // 0x00000000
class _xVec3 g_Z3; // 0x00342E48
class _xVec3 g_Y3; // 0x00342E38
class _xVec3 g_X3; // 0x00000000
class _xVec3 g_O3; // 0x00342E28
// 0x001131A0 - 0x00113224 (0x84) xQuatMul__FP6_xQuatPC6_xQuatPC6_xQuat
void xQuatMul(class _xQuat *o, class _xQuat *a, class _xQuat *b) {
    { // 0x001131A0 - 0x00113224
        float _s; // 
    }
}

// 0x00113230 - 0x001133CC (0x19C) xQuatSlerp__FP6_xQuatPC6_xQuatPC6_xQuatf
void xQuatSlerp(class _xQuat *o, class _xQuat *a, class _xQuat *b, float t) {
    { // 0x00113230 - 0x001133CC
        class _xQuat b2; // r29+0x50
        float abdot; // r29+0x60
        float theta; // 
        float one_sintheta; // 
        float bsph; // r29+0x60
        float asph; // 
    }
}

// 0x001133D0 - 0x001134F8 (0x128) xQuatNormalize__FP6_xQuatPC6_xQuat
float xQuatNormalize(class _xQuat *o, class _xQuat *q) {
    { // 0x001133D0 - 0x001134F8
        float len; // r29+0x30
        float one_len; // 
    }
}

// 0x00113500 - 0x001135AC (0xAC) xQuatToMat__FPC6_xQuatP8_xMat3x3
void xQuatToMat(class _xQuat *q, class _xMat3x3 *m) {
    { // 0x00113500 - 0x001135AC
        float tzz; // 
        float tyz; // 
        float tyy; // 
        float txz; // 
        float txy; // 
        float txx; // 
        float tsz; // 
        float tsy; // 
        float tsx; // 
        float tz; // 
        float ty; // 
        float tx; // r29+0x0
    }
}

// 0x001135B0 - 0x00113844 (0x294) xQuatFromMat__FP6_xQuatPC8_xMat3x3
void xQuatFromMat(class _xQuat *q, class _xMat3x3 *m) {
    { // 0x001135B0 - 0x00113844
        signed int k; // r20
        signed int j; // r19
        signed int i; // r18
        float root; // r29+0x90
        float tr; // 
        float *qvp; // r2
        float *mp; // r2
        static signed int nxt[3]; // 0x0028A1C0
    }
}

// 0x00113850 - 0x001138A0 (0x50) xMat4x3RMul3x3__FP8_xMat4x3PC8_xMat3x3PC8_xMat4x3
void xMat4x3RMul3x3(class _xMat4x3 *o, class _xMat3x3 *m3x3, class _xMat4x3 *m) {
    { // 0x00113850 - 0x001138A0
    }
}

// 0x001138A0 - 0x00113948 (0xA8) xMat4x3LMul3x3__FP8_xMat4x3PC8_xMat4x3PC8_xMat3x3
void xMat4x3LMul3x3(class _xMat4x3 *o, class _xMat4x3 *m, class _xMat3x3 *m3x3) {
    { // 0x001138A0 - 0x00113948
    }
}

// 0x00113950 - 0x00113A10 (0xC0) xMat4x3Mul__FP8_xMat4x3PC8_xMat4x3PC8_xMat4x3
void xMat4x3Mul(class _xMat4x3 *o, class _xMat4x3 *a, class _xMat4x3 *b) {
    { // 0x00113950 - 0x00113A10
    }
}

// 0x00113A20 - 0x00113B94 (0x174) xMat4x3Rot__FP8_xMat4x3PC6_xVec3fPC6_xVec3
void xMat4x3Rot(class _xMat4x3 *m, class _xVec3 *a, float t, class _xVec3 *p) {
    { // 0x00113A20 - 0x00113B94
        class _xMat4x3 temp; // r29+0x40
    }
}

// 0x00113BA0 - 0x00113E20 (0x280) xMat4x3Orthonormalize__FP8_xMat4x3PC8_xMat4x3
void xMat4x3Orthonormalize(class _xMat4x3 *o, class _xMat4x3 *m) {
    { // 0x00113BA0 - 0x00113E20
    }
}

// 0x00113E20 - 0x00113EF8 (0xD8) xMat3x3Tolocal__FP6_xVec3PC8_xMat3x3PC6_xVec3
void xMat3x3Tolocal(class _xVec3 *o, class _xMat3x3 *m, class _xVec3 *v) {
    { // 0x00113E20 - 0x00113EF8
    }
}

// 0x00113F00 - 0x00113F70 (0x70) xMat3x3RMulVec__FP6_xVec3PC8_xMat3x3PC6_xVec3
void xMat3x3RMulVec(class _xVec3 *o, class _xMat3x3 *m, class _xVec3 *v) {
    { // 0x00113F00 - 0x00113F70
        float z; // 
        float y; // 
    }
}

// 0x00113F70 - 0x00113FE0 (0x70) xMat3x3LMulVec__FP6_xVec3PC8_xMat3x3PC6_xVec3
void xMat3x3LMulVec(class _xVec3 *o, class _xMat3x3 *m, class _xVec3 *v) {
    { // 0x00113F70 - 0x00113FE0
    }
}

// 0x00113FE0 - 0x001141C0 (0x1E0) xMat3x3Mul__FP8_xMat3x3PC8_xMat3x3PC8_xMat3x3
void xMat3x3Mul(class _xMat3x3 *o, class _xMat3x3 *a, class _xMat3x3 *b) {
    { // 0x00113FE0 - 0x001141C0
        unsigned int usetemp; // r7
        class _xMat3x3 *tp; // r3
        class _xMat3x3 temp; // r29+0x10
    }
}

// 0x001141C0 - 0x00114250 (0x90) xMat3x3Transpose__FP8_xMat3x3PC8_xMat3x3
void xMat3x3Transpose(class _xMat3x3 *o, class _xMat3x3 *m) {
    { // 0x001141C0 - 0x00114250
        float temp; // 
        float temp; // 
        float temp; // 
    }
}

// 0x00114250 - 0x00114398 (0x148) xMat3x3RMulRotY__FP8_xMat3x3PC8_xMat3x3f
void xMat3x3RMulRotY(class _xMat3x3 *o, class _xMat3x3 *m, float t) {
    { // 0x00114250 - 0x00114398
        float temp; // 
        float b; // r29+0x40
        float a; // 
    }
}

// 0x001143A0 - 0x001144E8 (0x148) xMat3x3LMulRotY__FP8_xMat3x3PC8_xMat3x3f
void xMat3x3LMulRotY(class _xMat3x3 *o, class _xMat3x3 *m, float t) {
    { // 0x001143A0 - 0x001144E8
        float temp; // 
        float b; // r29+0x40
        float a; // 
    }
}

// 0x001144F0 - 0x0011456C (0x7C) xMat3x3RotY__FP8_xMat3x3f
void xMat3x3RotY(class _xMat3x3 *m, float t) {
    { // 0x001144F0 - 0x0011456C
        float b; // r29+0x30
        float a; // 
    }
}

// 0x00114570 - 0x00114694 (0x124) xMat3x3RotC__FP8_xMat3x3ffff
void xMat3x3RotC(class _xMat3x3 *m, float _x, float _y, float _z, float t) {
    { // 0x00114570 - 0x00114694
        float c; // 
        float b; // r29+0x30
        float a; // 
    }
}

// 0x001146A0 - 0x001147A0 (0x100) xMat3x3Euler__FP8_xMat3x3fff
void xMat3x3Euler(class _xMat3x3 *m, float yaw, float pitch, float roll) {
    { // 0x001146A0 - 0x001147A0
        float cr; // r29+0x40
        float sr; // 
        float cp; // 
        float sp; // 
        float cy; // 
        float sy; // 
    }
}

// 0x001147A0 - 0x001147B0 (0x10) xMat3x3Euler__FP8_xMat3x3P6_xVec3
void xMat3x3Euler(class _xMat3x3 *m, class _xVec3 *ypr) {
    { // 0x001147A0 - 0x001147B0
    }
}

// 0x001147B0 - 0x00114B54 (0x3A4) xMat3x3LookVec__FP8_xMat3x3PC6_xVec3
float xMat3x3LookVec(class _xMat3x3 *m, class _xVec3 *at) {
    { // 0x001147B0 - 0x00114B54
    }
}

// 0x00114B60 - 0x00114BA0 (0x40) xMat4x3MoveLocalAt__FP8_xMat4x3f
void xMat4x3MoveLocalAt(class _xMat4x3 *m, float mag) {
    { // 0x00114B60 - 0x00114BA0
    }
}

// 0x00114BA0 - 0x00114BE0 (0x40) xMat4x3MoveLocalUp__FP8_xMat4x3f
void xMat4x3MoveLocalUp(class _xMat4x3 *m, float mag) {
    { // 0x00114BA0 - 0x00114BE0
    }
}

// 0x00114BE0 - 0x00114C20 (0x40) xMat4x3MoveLocalRight__FP8_xMat4x3f
void xMat4x3MoveLocalRight(class _xMat4x3 *m, float mag) {
    { // 0x00114BE0 - 0x00114C20
    }
}

// 0x00114C20 - 0x00114D2C (0x10C) xMat3x3GetEuler__FPC8_xMat3x3P6_xVec3
void xMat3x3GetEuler(class _xMat3x3 *m, class _xVec3 *a) {
    { // 0x00114C20 - 0x00114D2C
        float roll; // r29+0x40
        float yaw; // 
        float pitch; // 
    }
}

// 0x00114D30 - 0x00114DC8 (0x98) xMat3x3GetScale__FPC8_xMat3x3P6_xVec3
void xMat3x3GetScale(class _xMat3x3 *m, class _xVec3 *s) {
    { // 0x00114D30 - 0x00114DC8
    }
}

// 0x00114DD0 - 0x0011506C (0x29C) xMat3x3Normalize__FP8_xMat3x3PC8_xMat3x3
void xMat3x3Normalize(class _xMat3x3 *o, class _xMat3x3 *m) {
    { // 0x00114DD0 - 0x0011506C
    }
}

// 0x00115070 - 0x001151A0 (0x130) xLine3VecDist2__FPC6_xVec3PC6_xVec3PC6_xVec3P7_xIsect
void xLine3VecDist2(class _xVec3 *p1, class _xVec3 *p2, class _xVec3 *v, class _xIsect *isx) {
    { // 0x00115070 - 0x001151A0
    }
}

// 0x001151A0 - 0x001152A8 (0x108) xVec3Normalize__FP6_xVec3PC6_xVec3
float xVec3Normalize(class _xVec3 *o, class _xVec3 *v) {
    { // 0x001151A0 - 0x001152A8
        float len; // r29+0x30
        float one_len; // 
    }
}

// 0x001152B0 - 0x00115364 (0xB4) xVec3LengthFast__Ffff
float xVec3LengthFast(float x, float y, float z) {
    { // 0x001152B0 - 0x00115364
        float temp; // 
        float temp; // 
        float temp; // 
        float min; // r29+0x20
        float med; // 
        float max; // 
    }
}

// 0x00115370 - 0x00115378 (0x8) xMath3Exit__Fv
void xMath3Exit() {
    { // 0x00115370 - 0x00115378
    }
}

// 0x00115380 - 0x00115388 (0x8) xMath3Init__Fv
void xMath3Init() {
    { // 0x00115380 - 0x00115388
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xMath3.h
// ==========================================================================
// 0x00113A10 - 0x00113A20 (0x10) xVec3Init__FP6_xVec3fff
void xVec3Init(class _xVec3 *v, float _x, float _y, float _z) {
    { // 0x00113A10 - 0x00113A20
    }
}

// 0x0013DA60 - 0x0013DA7C (0x1C) xVec3Copy__FP6_xVec3PC6_xVec3
void xVec3Copy(class _xVec3 *o, class _xVec3 *v) {
    { // 0x0013DA60 - 0x0013DA7C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xMath.cpp
// ==========================================================================
// 0x00115390 - 0x00115418 (0x88) xDangleClamp__Ff
float xDangleClamp(float a) {
    { // 0x00115390 - 0x00115418
        float b; // r29+0x10
    }
}

// 0x00115420 - 0x00115470 (0x50) xAngleClamp__Ff
float xAngleClamp(float a) {
    { // 0x00115420 - 0x00115470
        float b; // r29+0x10
    }
}

// 0x00115470 - 0x00115824 (0x3B4) xMathSolveCubic__FffffPfPfPf
unsigned int xMathSolveCubic(float a, float b, float c, float d, float *x1, float *x2, float *x3) {
    { // 0x00115470 - 0x00115824
        float fSin; // r29+0x50
        float fCos; // 
        float fAngle; // 
        float fDist; // 
        float fTemp; // r29+0x50
        float fHalfB; // 
        float fDiscr; // 
        float fOffset; // 
        float fB; // 
        float fA; // 
        float arecip; // r29+0x50
    }
}

// 0x00115830 - 0x0011596C (0x13C) xMathSolveQuadratic__FfffPfPf
unsigned int xMathSolveQuadratic(float a, float b, float c, float *x1, float *x2) {
    { // 0x00115830 - 0x0011596C
        float p; // 
        float dx; // 
        float d; // r29+0x40
    }
}

// 0x00115970 - 0x00115994 (0x24) xMathExit__Fv
void xMathExit() {
    { // 0x00115970 - 0x00115994
    }
}

// 0x001159A0 - 0x001159C4 (0x24) xMathInit__Fv
void xMathInit() {
    { // 0x001159A0 - 0x001159C4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zMain.cpp
// ==========================================================================
void (zMainMemLvlChkCB)(); // 0x00116440
class _tagxPad *gDebugPad; // 0x00357C30
static class st_SERIAL_PERCID_SIZE g_xser_sizeinfo[2]; // 0x0028A1D0
static signed int sShowMenuOnBoot; // 0x00357498
unsigned int gEE2BuildNumber; // 0x00000000
signed int gBuildNumber; // 0x00000000
class zGlobals globals; // 0x00362FD0
static class _zHipDepend sHipDependTable[1]; // 0x00000000
// 0x001159D0 - 0x00115D00 (0x330) zMainMemCardQueryPost__Fiii
static void zMainMemCardQueryPost(signed int bytesNeeded, signed int availOnDisk, signed int showMsg) {
    { // 0x001159D0 - 0x00115D00
        char strn[256]; // r29+0x170
        char str[256]; // r29+0x70
        signed int available; // r2
        class xFontBox *sTrcFullCardFontBox2; // r21
        class xFontBox *sTrcFullCardFontBox; // r20
        class RwRGBA bg; // r29+0x278
        class RwCamera *cam; // r19
    }
}

// 0x00115D00 - 0x00115DD8 (0xD8) zMainMemCardSpaceQuery__Fv
static void zMainMemCardSpaceQuery() {
    { // 0x00115D00 - 0x00115DD8
        signed int fullCard; // r17
        signed int do_chk; // r16
        signed int availOnDisk; // r29+0x4c
        signed int bytesNeeded; // r29+0x48
    }
}

// 0x00115DE0 - 0x00115F78 (0x198) zMainFirstScreen__Fi
void zMainFirstScreen(signed int mode) {
    { // 0x00115DE0 - 0x00115F78
        class xFontBox *b; // r17
        char str[271]; // r29+0x30
        class RwRGBA bg; // r29+0x14c
        class RwCamera *cam; // r16
        class iColor_tag yellow; // r29+0x148
    }
}

// 0x00115F80 - 0x00116164 (0x1E4) zMainCheckINI__Fv
static void zMainCheckINI() {
    { // 0x00115F80 - 0x00116164
        class xIniFile *ini; // r16
        unsigned int size; // r29+0x4c
        void *buf; // r2
        char *str; // r17
    }
}

// 0x00116170 - 0x00116438 (0x2C8) zMainLoop__Fv
static void zMainLoop() {
    { // 0x00116170 - 0x00116438
        unsigned int newGameSceneID; // r16
        float pdone; // r29+0x60
        static unsigned int menuModeID; // 0x00357C38
        static unsigned int gameSceneID; // 0x00357C40
        static signed char init; // 0x00357C44
        static signed char init; // 0x00357C3C
    }
}

// 0x00116440 - 0x00116464 (0x24) zMainMemLvlChkCB__Fv
static void zMainMemLvlChkCB() {
    { // 0x00116440 - 0x00116464
    }
}

// 0x00116470 - 0x00116DF8 (0x988) zMainParseINIGlobals__FP8xIniFile
static void zMainParseINIGlobals(class xIniFile *ini) {
    { // 0x00116470 - 0x00116DF8
        static float fbuf_Toss[16]; // 0x0028A480
        static float fbuf_Spring[16]; // 0x0028A440
        static float fbuf_Bounce[16]; // 0x0028A400
        static float fbuf_Double[16]; // 0x0028A3C0
        static float fbuf_Jump[16]; // 0x0028A380
        static float fbuf_AnimWalk[12]; // 0x0028A350
        static float fbuf_AnimSneak[12]; // 0x0028A320
        static float fbuf_SpeedHotsauce[16]; // 0x0028A2E0
        static float fbuf_SpeedAir[16]; // 0x0028A2A0
        static float fbuf_SpeedRun[16]; // 0x0028A260
        static float fbuf_SpeedWalk[16]; // 0x0028A220
        static float fbuf_SpeedSneak[16]; // 0x0028A1E0
    }
}

// 0x00116E00 - 0x00116F7C (0x17C)
void main() {
    { // 0x00116E00 - 0x00116F7C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xAnim.cpp
// ==========================================================================
class _reent *_impure_ptr; // 0x002FCC00
unsigned char *giAnimScratch; // 0x003574A0
unsigned int gActiveHeap; // 0x00357CAC
void (xAnimPoolCB)(class xMemPool *, void *); // 0x001172D0
unsigned int gxAnimUseGrowAlloc; // 0x00357C50
static class xMemPool sxAnimTempTranPool; // 0x00366500
// 0x00116F80 - 0x0011700C (0x8C) xAnimPoolFree__FP9xAnimPlay
void xAnimPoolFree(class xAnimPlay *play) {
    { // 0x00116F80 - 0x0011700C
        unsigned int i; // r16
    }
}

// 0x00117010 - 0x00117070 (0x60) xAnimPoolAlloc__FP8xMemPoolPvP10xAnimTableP14xModelInstance
class xAnimPlay *xAnimPoolAlloc(class xMemPool *pool, void *object, class xAnimTable *table, class xModelInstance *modelInst) {
    { // 0x00117010 - 0x00117070
        class xAnimPlay *play; // r16
    }
}

// 0x00117070 - 0x001172CC (0x25C) xAnimPoolInit__FP8xMemPoolUiUiUiUi
void xAnimPoolInit(class xMemPool *pool, unsigned int count, unsigned int singles, unsigned int blendFlags, unsigned int effectMax) {
    { // 0x00117070 - 0x001172CC
        class xAnimActiveEffect *curract; // r5
        class xAnimSingle *currsingle; // r2
        class xAnimPlay *play; // r2
        void *buffer; // r2
        unsigned int i; // r4
        unsigned int size; // r22
    }
}

// 0x001172D0 - 0x001173E8 (0x118) xAnimPoolCB__FP8xMemPoolPv
void xAnimPoolCB(class xMemPool *pool, void *data) {
    { // 0x001172D0 - 0x001173E8
        class xAnimActiveEffect *curract; // r6
        class xAnimSingle *currsingle; // r2
        class xAnimSingle *clonesingle; // r8
        class xAnimPlay *play; // r2
        class xAnimPlay *clone; // r3
        signed int i; // r9
    }
}

// 0x001173F0 - 0x00117860 (0x470) xAnimPlayEval__FP9xAnimPlay
void xAnimPlayEval(class xAnimPlay *play) {
    { // 0x001173F0 - 0x00117860
        unsigned short *blendO; // r18
        float blendR; // 
        float blendF; // r29+0x100
        class xAnimSingle *si; // r17
        class _xVec3 *tranblend; // r23
        class _xQuat *quatblend; // r22
        class _xVec3 *tranresult; // r29+0xd0
        class _xQuat *quatresult; // r2
        unsigned int bone; // r17
        unsigned int i; // r29+0xc0
    }
}

// 0x00117860 - 0x00117964 (0x104) xAnimPlayUpdate__FP9xAnimPlayf
void xAnimPlayUpdate(class xAnimPlay *play, float timeDelta) {
    { // 0x00117860 - 0x00117964
        float tempYaw[2]; // r29+0x98
        class _xQuat tempQuat[2]; // r29+0x70
        class _xVec3 tempTranslate[2]; // r29+0x50
        unsigned int i; // r16
    }
}

// 0x00117970 - 0x00117C60 (0x2F0) xAnimPlayStartTransition__FP9xAnimPlayP15xAnimTransition
void xAnimPlayStartTransition(class xAnimPlay *play, class xAnimTransition *transition) {
    { // 0x00117970 - 0x00117C60
        class xAnimSingle *bl; // r17
        class xAnimSingle *single; // r16
    }
}

// 0x00117C60 - 0x00117DEC (0x18C) xAnimPlayChooseTransition__FP9xAnimPlay
void xAnimPlayChooseTransition(class xAnimPlay *play) {
    { // 0x00117C60 - 0x00117DEC
        unsigned int index; // r3
        class xAnimTransitionList *curr; // r19
        class xAnimTransition **found; // r2
        void *object; // r18
        unsigned int i; // r17
    }
}

// 0x00117DF0 - 0x00117F10 (0x120) xAnimPlaySetup__FP9xAnimPlayPvP10xAnimTableP14xModelInstance
void xAnimPlaySetup(class xAnimPlay *play, void *object, class xAnimTable *table, class xModelInstance *modelInst) {
    { // 0x00117DF0 - 0x00117F10
        signed int i; // r7
    }
}

// 0x00117F10 - 0x00118B6C (0xC5C) SingleUpdate__FP11xAnimSinglefP6_xVec3P6_xQuatPfUi
static void SingleUpdate(class xAnimSingle *single, float timeDelta, class _xVec3 *tran, class _xQuat *quat, float *yaw) {
    { // 0x00117F10 - 0x00118B6C
        float lerp; // r29+0x170
        float recip; // 
        class xAnimTransitionList *curr; // r2
        float timeCmp; // 
        float timeCmp; // r29+0x170
        float duration; // 
        void *object; // r29+0xd0
        float singleTime; // 
        float blendDelta; // 
        float tranDelta; // r29+0x170
        class xAnimSingle *bl; // r20
        float partialYaw[2]; // r29+0x168
        float blendYaw[2]; // r29+0x160
        class _xQuat partialQuat[2]; // r29+0x140
        class _xQuat blendQuat[2]; // r29+0x120
        class _xVec3 partialTran[2]; // r29+0x100
        class _xVec3 blendTran[2]; // r29+0xe0
        class xAnimTransition *foundTransition; // r19
        unsigned int firstStep; // r23
        unsigned int foundBlendstop; // r30
    }
}

// 0x00118B70 - 0x00118CA0 (0x130) xAnimPlaySetState__FP11xAnimSingleP10xAnimStatef
void xAnimPlaySetState(class xAnimSingle *single, class xAnimState *state, float startTime) {
    { // 0x00118B70 - 0x00118CA0
    }
}

// 0x00118CA0 - 0x00118DC8 (0x128) LoopUpdate__FP11xAnimSingleP6_xVec3P6_xQuatPf
static void LoopUpdate(class xAnimSingle *single, class _xVec3 *tran, class _xQuat *quat, float *yaw) {
    { // 0x00118CA0 - 0x00118DC8
        float partialYaw[2]; // r29+0xa8
        class _xQuat partialQuat[2]; // r29+0x80
        class _xVec3 partialTran[2]; // r29+0x60
        float duration; // 
    }
}

// 0x00118DD0 - 0x00118FDC (0x20C) CalcLastAndDeltas__FP11xAnimSingleP6_xVec3P6_xQuatPfUi
static void CalcLastAndDeltas(class xAnimSingle *single, class _xVec3 *tran, class _xQuat *quat, float *yaw, unsigned int lastOnly) {
    { // 0x00118DD0 - 0x00118FDC
        class _xQuat lastQ; // r29+0xa0
        class _xQuat tempQ[2]; // r29+0x80
        class _xVec3 tempV[2]; // r29+0x60
        float tempYaw[2]; // r29+0xb8
    }
}

// 0x00118FE0 - 0x00119134 (0x154) EffectSingleStop__FP11xAnimSingle
static void EffectSingleStop(class xAnimSingle *single) {
    { // 0x00118FE0 - 0x00119134
        class xAnimActiveEffect tempActive; // r29+0x48
        class xAnimEffect *effect; // r16
        unsigned int i; // r16
    }
}

// 0x00119140 - 0x00119288 (0x148) EffectSingleLoop__FP11xAnimSingle
static void EffectSingleLoop(class xAnimSingle *single) {
    { // 0x00119140 - 0x00119288
        class xAnimEffect *effect; // r3
        unsigned int count; // r18
        unsigned int index; // r17
        class xAnimActiveEffect *alist; // r16
    }
}

// 0x00119290 - 0x00119494 (0x204) EffectSingleRun__FP11xAnimSingle
static void EffectSingleRun(class xAnimSingle *single) {
    { // 0x00119290 - 0x00119494
        unsigned int flags; // r17
        float time; // 
        class xAnimActiveEffect tempActive; // r29+0x68
        class xAnimEffect *effect; // r16
    }
}

// 0x001194A0 - 0x001195E8 (0x148) EffectSingleDuration__FP11xAnimSingle
static void EffectSingleDuration(class xAnimSingle *single) {
    { // 0x001194A0 - 0x001195E8
        unsigned int count; // r18
        unsigned int index; // r17
        class xAnimActiveEffect *alist; // r16
        float time; // 
    }
}

// 0x001195F0 - 0x0011983C (0x24C) EffectSingleStart__FP11xAnimSingle
static void EffectSingleStart(class xAnimSingle *single) {
    { // 0x001195F0 - 0x0011983C
        unsigned int flags; // r4
        float time; // 
        class xAnimActiveEffect tempActive; // r29+0x48
        class xAnimEffect *effect; // r16
    }
}

// 0x00119840 - 0x001198A0 (0x60) xAnimTableGetState__FP10xAnimTablePc
class xAnimState *xAnimTableGetState(class xAnimTable *table, char *name) {
    { // 0x00119840 - 0x001198A0
    }
}

// 0x001198A0 - 0x00119920 (0x80) xAnimTableRemoveEffectByCB__FP10xAnimTablePFUiP17xAnimActiveEffectP11xAnimSinglePv_Ui
void xAnimTableRemoveEffectByCB(class xAnimTable *table, unsigned int (*callback)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *)) {
    { // 0x001198A0 - 0x00119920
        class xAnimState *curr; // r4
    }
}

// 0x00119920 - 0x0011995C (0x3C) xAnimTableGetStateID__FP10xAnimTableUi
class xAnimState *xAnimTableGetStateID(class xAnimTable *table, unsigned int ID) {
    { // 0x00119920 - 0x0011995C
        class xAnimState *curr; // r2
    }
}

// 0x00119960 - 0x001199F4 (0x94) xAnimTableAddFileID__FP10xAnimTableP9xAnimFileUi
class xAnimState *xAnimTableAddFileID(class xAnimTable *table, class xAnimFile *file, unsigned int stateID) {
    { // 0x00119960 - 0x001199F4
    }
}

// 0x00119A00 - 0x00119B10 (0x110) xAnimTableAddFile__FP10xAnimTableP9xAnimFilePc
void xAnimTableAddFile(class xAnimTable *table, class xAnimFile *file, char *states) {
    { // 0x00119A00 - 0x00119B10
        char *stateName; // r2
        unsigned char *buffer; // r2
    }
}

// 0x00119B10 - 0x00119D70 (0x260) xAnimTableNewTransition__FP10xAnimTablePcPcPFP15xAnimTransitionP11xAnimSinglePv_UiPFP15xAnimTransitionP11xAnimSinglePv_UiUiUiffUsUsfPUs
class xAnimTransition *xAnimTableNewTransition(class xAnimTable *table, char *source, char *dest, unsigned int (*conditional)(class xAnimTransition *, class xAnimSingle *, void *), unsigned int (*callback)(class xAnimTransition *, class xAnimSingle *, void *), unsigned int flags, unsigned int userFlags, float srcTime, float destTime, unsigned short priority, unsigned short queuePriority, float blendRecip, unsigned short *blendOffset) {
    { // 0x00119B10 - 0x00119D70
        class xAnimTransition *tran; // r18
    }
}

// 0x00119D70 - 0x0011A1C0 (0x450) xAnimTableAddTransition__FP10xAnimTableP15xAnimTransitionPc
void xAnimTableAddTransition(class xAnimTable *table, class xAnimTransition *tran, char *source) {
    { // 0x00119D70 - 0x0011A1C0
        class xAnimTransitionList *curr; // r2
        class xAnimTransitionList *tlist; // r2
        char *stateName; // r2
        unsigned int flags; // r30
        unsigned int allocCount; // r21
        unsigned int stateCount; // r20
        unsigned int i; // r5
        class xAnimState **stateList; // r2
        unsigned char *buffer; // r2
    }
}

// 0x0011A1C0 - 0x0011A3E8 (0x228) xAnimTableNewState__FP10xAnimTablePcUiUifPfPffPUs
class xAnimState *xAnimTableNewState(class xAnimTable *table, char *name, unsigned int flags, unsigned int userFlags, float speed, float *boneBlend, float *timeSnap, float fadeRecip, unsigned short *fadeOffset) {
    { // 0x0011A1C0 - 0x0011A3E8
        class xAnimState *state; // r22
    }
}

// 0x0011A3F0 - 0x0011A458 (0x68) xAnimTableNew__FPcPP10xAnimTable
class xAnimTable *xAnimTableNew(char *name, class xAnimTable **linkedList) {
    { // 0x0011A3F0 - 0x0011A458
        class xAnimTable *table; // r2
    }
}

// 0x0011A460 - 0x0011A528 (0xC8) xAnimStateNewEffect__FP10xAnimStateUiffPFUiP17xAnimActiveEffectP11xAnimSinglePv_UiUi
class xAnimEffect *xAnimStateNewEffect(class xAnimState *state, unsigned int flags, float startTime, float endTime, unsigned int (*callback)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *), unsigned int userDataSize) {
    { // 0x0011A460 - 0x0011A528
        class xAnimEffect *effect; // r2
        class xAnimEffect **prev; // r4
        class xAnimEffect *curr; // r3
    }
}

// 0x0011A530 - 0x0011AF0C (0x9DC) xAnimFileEval__FP9xAnimFilefPfUiP6_xVec3P6_xQuatPf
void xAnimFileEval(class xAnimFile *data, float time, float *bilinear, unsigned int flags, class _xVec3 *tran, class _xQuat *quat, float *yaw) {
    { // 0x0011A530 - 0x0011AF0C
        float *fptr; // r3
        class _xVec3 *t1; // r23
        class _xQuat *q1; // r22
        class _xVec3 *t0; // r17
        class _xQuat *q0; // r16
        unsigned int biplus[2]; // r29+0x188
        unsigned int biindex[2]; // r29+0x180
        float tmpf; // r29+0x190
        float bilerp[2]; // r29+0x178
        class _xQuat tempQ; // r29+0x100
        float tmpyaw; // 
        unsigned int fileflags; // r2
        unsigned int numBones; // r29+0xc0
        signed int i; // r16
    }
}

// 0x0011AF10 - 0x0011B30C (0x3FC) xAnimFileSetPoses__FP9xAnimFilePvfPvf
void xAnimFileSetPoses(class xAnimFile *data, void *startPose, float startTime, void *endPose, float endTime) {
    { // 0x0011AF10 - 0x0011B30C
        class _xQuat tempQ; // r29+0xe0
        float physEnd[4]; // r29+0xd0
        float physStart[4]; // r29+0xc0
        float poseEnd[4]; // r29+0xb0
        float poseStart[4]; // r29+0xa0
        float rootEnd[4]; // r29+0x90
        float rootStart[4]; // r29+0x80
        void *root; // r19
        signed int i; // r5
    }
}

// 0x0011B310 - 0x0011B3DC (0xCC) xAnimFileSetTime__FP9xAnimFileff
void xAnimFileSetTime(class xAnimFile *data, float duration, float timeOffset) {
    { // 0x0011B310 - 0x0011B3DC
        float rawDuration; // r29+0x30
    }
}

// 0x0011B3E0 - 0x0011B3E8 (0x8) xAnimFileSetPhysics__FP9xAnimFilePv
void xAnimFileSetPhysics(class xAnimFile *data, void *rawData) {
    { // 0x0011B3E0 - 0x0011B3E8
    }
}

// 0x0011B3F0 - 0x0011B418 (0x28) xAnimFileNew__FPvPcUiPP9xAnimFile
class xAnimFile *xAnimFileNew(void *rawData, char *name, unsigned int flags, class xAnimFile **linkedList) {
    { // 0x0011B3F0 - 0x0011B418
    }
}

// 0x0011B420 - 0x0011B6CC (0x2AC) xAnimFileNewBilinear__FPPvPcUiPP9xAnimFileUiUi
class xAnimFile *xAnimFileNewBilinear(void **rawData, char *name, unsigned int flags, class xAnimFile **linkedList, unsigned int numX, unsigned int numY) {
    { // 0x0011B420 - 0x0011B6CC
        class xAnimFile *afile; // r22
        signed int i; // r2
    }
}

// 0x0011B6D0 - 0x0011BA80 (0x3B0) CalcPhys__FP9xAnimFile
static void CalcPhys(class xAnimFile *af) {
    { // 0x0011B6D0 - 0x0011BA80
        class _xQuat tempQ; // r29+0xb0
        float physEnd[4]; // r29+0xa0
        float physStart[4]; // r29+0x90
        float rootZero[4]; // r29+0x80
        float rootEnd[4]; // r29+0x70
        float rootStart[4]; // r29+0x60
        void *root; // r16
        signed int i; // r4
    }
}

// 0x0011BA80 - 0x0011BD30 (0x2B0) CalcPhysFromKeys__FP9xAnimFilePfPfPfPf
static void CalcPhysFromKeys(class xAnimFile *af, float *rootStart, float *rootEnd, float *physStart, float *physEnd) {
    { // 0x0011BA80 - 0x0011BD30
        float *temp; // r3
        float *temp; // r3
        float delta; // 
        signed int i; // r3
    }
}

// 0x0011BD30 - 0x0011BD58 (0x28) xAnimTempTransitionAlloc__Fv
class xAnimTransition *xAnimTempTransitionAlloc() {
    { // 0x0011BD30 - 0x0011BD58
        class xAnimTransition *tran; // r2
    }
}

// 0x0011BD60 - 0x0011BDC4 (0x64) xAnimTempTransitionInit__FUi
void xAnimTempTransitionInit(unsigned int count) {
    { // 0x0011BD60 - 0x0011BDC4
    }
}

// 0x0011BDD0 - 0x0011BE00 (0x30) xAnimInit__Fv
void xAnimInit() {
    { // 0x0011BDD0 - 0x0011BE00
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iAnimSKB.cpp
// ==========================================================================
unsigned char *giAnimScratch; // 0x003574A0
signed int (CmpTime)(void *, void *); // 0x00000000
unsigned int gActiveHeap; // 0x00357CAC
signed int (CmpBoneTime)(void *, void *); // 0x00000000
float slerpPolynomial[24]; // 0x0028A5E0
// 0x0011BE00 - 0x0011C120 (0x320) _iAnimSKBFlip180__FP14iAnimSKBHeaderUi
void _iAnimSKBFlip180(class iAnimSKBHeader *data, unsigned int bone) {
    { // 0x0011BE00 - 0x0011C120
        float _z; // 
        float _y; // 
        float _x; // r29+0x10
        float _s; // 
        float quat[4]; // r29+0x0
        unsigned short *offsets; // r11
        class iAnimSKBKey *keys; // r2
        unsigned int tcount; // r2
        unsigned int kcount; // r6
        unsigned int keylast; // r5
        unsigned int keyfirst; // r4
        unsigned int i; // r3
    }
}

// 0x0011C120 - 0x0011C5BC (0x49C) _iAnimSKBAdjustTranslate__FP14iAnimSKBHeaderUiPf
void _iAnimSKBAdjustTranslate(class iAnimSKBHeader *data, unsigned int bone, float *tran) {
    { // 0x0011C120 - 0x0011C5BC
        unsigned short *offsets; // r9
        class iAnimSKBKey *keys; // r2
        float newmax[3]; // r29+0x130
        float oldmax[3]; // r29+0x120
        float factor[3]; // r29+0x110
        float pos; // 
        float outScale[3]; // r29+0x100
        unsigned int tcount; // r2
        unsigned int kcount; // r29+0xb0
        unsigned int keylast; // r23
        unsigned int keyfirst; // r30
        unsigned int idx; // r16
        unsigned int i; // r22
        signed int ipos; // r5
    }
}

// 0x0011C5C0 - 0x0011C868 (0x2A8) _iAnimSKBSwapTranslate__FP14iAnimSKBHeaderUiUi
void _iAnimSKBSwapTranslate(class iAnimSKBHeader *data, unsigned int bonelo, unsigned int bonehi) {
    { // 0x0011C5C0 - 0x0011C868
        unsigned short keyIndex; // r6
        unsigned short *offsets; // r19
        class iAnimSKBKey *keys; // r18
        class iAnimSKBKey *bufmid; // r29+0xa0
        class iAnimSKBKey *bufhi; // r22
        class iAnimSKBKey *buflo; // r23
        unsigned int tcount; // r17
        unsigned int bcount; // r16
        unsigned int j; // r4
        unsigned int i; // r5
        signed int out; // r2
    }
}

// 0x0011C870 - 0x0011C894 (0x24) iAnimDurationSKB__FP14iAnimSKBHeader
float iAnimDurationSKB(class iAnimSKBHeader *data) {
    { // 0x0011C870 - 0x0011C894
    }
}

// 0x0011C8A0 - 0x0011CB88 (0x2E8) _iAnimSKBFixTransPop__FPvPvUi
void _iAnimSKBFixTransPop(void *data, void *pose, unsigned int maxbone) {
    { // 0x0011C8A0 - 0x0011CB88
        class iAnimSKBHeader *hb; // r23
        float *ta; // r19
        class iAnimSKBKey *ka; // r2
        class iAnimSKBHeader *ha; // r18
        class _xVec3 *t_b; // r2
        class _xQuat *q_b; // r30
        class _xVec3 *t_a; // r2
        class _xQuat *q_a; // r2
        float invDuration; // 
        unsigned int boneIndex; // r17
        unsigned int i; // r4
    }
}

// 0x0011CB90 - 0x0011D03C (0x4AC) iAnimEvalSKB__FP14iAnimSKBHeaderfUiP6_xVec3P6_xQuat
void iAnimEvalSKB(class iAnimSKBHeader *data, float time, unsigned int flags, class _xVec3 *tran, class _xQuat *quat) {
    { // 0x0011CB90 - 0x0011D03C
        unsigned int theta; // r2
        unsigned int costheta; // r2
        class iAnimSKBKey *k; // r14
        float lerp; // r29+0x0
        float time2; // 
        float time1; // r29+0x0
        unsigned short *offsets; // r10
        float *times; // r9
        class iAnimSKBKey *keys; // r8
        unsigned int tcount; // r13
        unsigned int bcount; // r3
        unsigned int tidx; // r7
        unsigned int i; // r13
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iAnim.cpp
// ==========================================================================
unsigned char *giAnimScratch; // 0x003574A0
static unsigned char scratchBuffer[9120]; // 0x00366540
// 0x0011D040 - 0x0011D884 (0x844) iAnimBlend__FffPUsPfUiP6_xVec3P6_xQuatP6_xVec3P6_xQuatP6_xVec3P6_xQuat
void iAnimBlend(float BlendFactor, float BlendRecip, unsigned short *BlendTimeOffset, float *BoneTable, unsigned int BoneCount, class _xVec3 *Tran1, class _xQuat *Quat1, class _xVec3 *Tran2, class _xQuat *Quat2, class _xVec3 *TranDest, class _xQuat *QuatDest) {
    { // 0x0011D040 - 0x0011D884
        float z; // 
        float z; // 
        float rSclTo; // 
        float rSclFrom; // r29+0x160
        class RtQuatSlerpCache qcache; // r29+0x130
        float lerp; // 
        float baselerp; // 
        float z; // 
        float z; // 
        float rSclTo; // 
        float rSclFrom; // r29+0x160
        class RtQuatSlerpCache qcache; // r29+0x100
        float lerp; // 
        class _xVec3 *t2; // r17
        class RtQuat ident; // r29+0xf0
        class RtQuat *q2; // r16
        unsigned int invert; // r29+0xb0
        unsigned int i; // r23
    }
}

// 0x0011D890 - 0x0011D898 (0x8) iAnimBoneCount__FPv
unsigned int iAnimBoneCount(void *RawData) {
    { // 0x0011D890 - 0x0011D898
    }
}

// 0x0011D8A0 - 0x0011D8A8 (0x8) iAnimDuration__FPv
float iAnimDuration(void *RawData) {
    { // 0x0011D8A0 - 0x0011D8A8
    }
}

// 0x0011D8B0 - 0x0011D8B8 (0x8) iAnimEval__FPvfUiP6_xVec3P6_xQuat
void iAnimEval(void *RawData, float time, unsigned int flags, class _xVec3 *tran, class _xQuat *quat) {
    { // 0x0011D8B0 - 0x0011D8B8
    }
}

// 0x0011D8C0 - 0x0011D8C8 (0x8) iAnimInit__Fv
void iAnimInit() {
    { // 0x0011D8C0 - 0x0011D8C8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xpkrsvc.cpp
// ==========================================================================
signed int (OrdTest_R_Asset)(void *, void *); // 0x0011E7C0
signed int (OrdTest_R_AssetID)(void *, void *); // 0x0011E790
unsigned int gActiveHeap; // 0x00357CAC
void (PKR_special_loadbuf_killed)(class RwResEntry *); // 0x0011D8D0
static signed int g_RWarena_bufsize; // 0x00000000
static class RwResEntry *g_RWarena_resOwner; // 0x00357C74
static class RwResEntry *g_RWarena_resEntry; // 0x00357C70
static signed int g_memalloc_runtot; // 0x00000000
static signed int g_memalloc_pair; // 0x00000000
static signed int g_packinit; // 0x00357C5C
static unsigned int g_loadlock; // 0x00357C58
static class st_PACKER_READ_DATA g_readdatainst[8]; // 0x0028A6A0
static class st_HIPLOADFUNCS *g_hiprf; // 0x00357C54
static class st_PACKER_READ_FUNCS g_pkr_read_funcmap; // 0x0028A660
// 0x0011D8D0 - 0x0011D8D8 (0x8) PKR_special_loadbuf_killed__FP10RwResEntry
static void PKR_special_loadbuf_killed() {
    { // 0x0011D8D0 - 0x0011D8D8
    }
}

// 0x0011D8E0 - 0x0011D908 (0x28) PKR_relmem__FUiiPvUii
static void PKR_relmem(void *memptr, signed int isTemp) {
    { // 0x0011D8E0 - 0x0011D908
    }
}

// 0x0011D910 - 0x0011D98C (0x7C) PKR_getmem__FUiiUii
static void *PKR_getmem(signed int amount, signed int isTemp) {
    { // 0x0011D910 - 0x0011D98C
        void *memptr; // r16
    }
}

// 0x0011D990 - 0x0011DC84 (0x2F4) PKR_bld_typecnt__FP19st_PACKER_READ_DATA
static void PKR_bld_typecnt(class st_PACKER_READ_DATA *pr) {
    { // 0x0011D990 - 0x0011DC84
        signed int lasttidx; // r2
        unsigned int lasttype; // r2
        class st_XORDEREDARRAY *tmplist; // r2
        signed int typcnt[129]; // r29+0x90
        signed int idx; // r2
        signed int j; // r5
        signed int i; // r4
        class st_PACKER_ATOC_NODE *assnode; // r11
        class st_PACKER_LTOC_NODE *laynode; // r3
    }
}

// 0x0011DC90 - 0x0011DCC8 (0x38) PKR_type2typeref__FUiP19st_PACKER_ASSETTYPE
static class st_PACKER_ASSETTYPE *PKR_type2typeref(unsigned int type, class st_PACKER_ASSETTYPE *typelist) {
    { // 0x0011DC90 - 0x0011DCC8
        class st_PACKER_ASSETTYPE *tmptype; // r2
        class st_PACKER_ASSETTYPE *da_type; // r2
    }
}

// 0x0011DCD0 - 0x0011DF08 (0x238) LOD_r_LHDR__FP14st_HIPLOADDATAP19st_PACKER_READ_DATA
static signed int LOD_r_LHDR(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr) {
    { // 0x0011DCD0 - 0x0011DF08
        class st_PACKER_ATOC_NODE *assnode; // r21
        signed int i; // r20
        signed int idx; // r2
        signed int refcnt; // r29+0x78
        enum en_LAYER_TYPE laytyp; // r20
        signed int ival; // r29+0x74
        unsigned int cid; // r2
    }
}

// 0x0011DF10 - 0x0011E01C (0x10C) LOD_r_ADBG__FP14st_HIPLOADDATAP19st_PACKER_READ_DATAP19st_PACKER_ATOC_NODE
static signed int LOD_r_ADBG(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr, class st_PACKER_ATOC_NODE *assnode) {
    { // 0x0011DF10 - 0x0011E01C
        char tmpbuf[256]; // r29+0x50
        signed int ival; // r29+0x15c
    }
}

// 0x0011E020 - 0x0011E248 (0x228) LOD_r_AHDR__FP14st_HIPLOADDATAP19st_PACKER_READ_DATA
static signed int LOD_r_AHDR(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr) {
    { // 0x0011E020 - 0x0011E248
        signed int isdup; // r2
        signed int ival; // r29+0x6c
        unsigned int cid; // r2
    }
}

// 0x0011E250 - 0x0011E324 (0xD4) LOD_r_ATOC__FP14st_HIPLOADDATAP19st_PACKER_READ_DATA
static signed int LOD_r_ATOC(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr) {
    { // 0x0011E250 - 0x0011E324
        unsigned int cid; // r2
    }
}

// 0x0011E330 - 0x0011E48C (0x15C) LOD_r_DICT__FP14st_HIPLOADDATAP19st_PACKER_READ_DATA
static signed int LOD_r_DICT(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr) {
    { // 0x0011E330 - 0x0011E48C
        unsigned int cid; // r2
    }
}

// 0x0011E490 - 0x0011E550 (0xC0) LOD_r_PVER__FP14st_HIPLOADDATAP19st_PACKER_READ_DATA
static signed int LOD_r_PVER(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr) {
    { // 0x0011E490 - 0x0011E550
        signed int amt; // r2
        signed int ver; // r29+0x4c
    }
}

// 0x0011E550 - 0x0011E788 (0x238) LOD_r_PACK__FP14st_HIPLOADDATAP19st_PACKER_READ_DATA
static signed int LOD_r_PACK(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr) {
    { // 0x0011E550 - 0x0011E788
        unsigned int cid; // r2
    }
}

// 0x0011E790 - 0x0011E7BC (0x2C) OrdTest_R_AssetID__FPCvPv
static signed int OrdTest_R_AssetID(void *vkey, void *vitem) {
    { // 0x0011E790 - 0x0011E7BC
        unsigned int key; // r2
        signed int rc; // r2
    }
}

// 0x0011E7C0 - 0x0011E7F0 (0x30) OrdTest_R_Asset__FPCvPv
static signed int OrdTest_R_Asset(void *vkey, void *vitem) {
    { // 0x0011E7C0 - 0x0011E7F0
        signed int rc; // r2
    }
}

// 0x0011E7F0 - 0x0011E908 (0x118) PKR_FRIEND_assetIsGameDup__FUiPC19st_PACKER_READ_DATAiUiUiPc
static signed int PKR_FRIEND_assetIsGameDup(unsigned int aid, class st_PACKER_READ_DATA *skippr) {
    { // 0x0011E7F0 - 0x0011E908
        signed int idx; // r2
        signed int i; // r17
        class st_PACKER_ATOC_NODE *tmp_ass; // r4
        signed int is_dup; // r16
    }
}

// 0x0011E910 - 0x0011E990 (0x80) PKR_PkgHasAsset__FP19st_PACKER_READ_DATAUi
static signed int PKR_PkgHasAsset(class st_PACKER_READ_DATA *pr, unsigned int aid) {
    { // 0x0011E910 - 0x0011E990
        signed int idx; // r2
        signed int rc; // r2
    }
}

// 0x0011E990 - 0x0011EABC (0x12C) PKR_GetAssetInfoByType__FP19st_PACKER_READ_DATAUiiP20st_PKR_ASSET_TOCINFO
static signed int PKR_GetAssetInfoByType(class st_PACKER_READ_DATA *pr, unsigned int type, signed int idx, class st_PKR_ASSET_TOCINFO *tocinfo) {
    { // 0x0011E990 - 0x0011EABC
        class st_XORDEREDARRAY *typlist; // r4
        class st_PACKER_ATOC_NODE *assnode; // r4
    }
}

// 0x0011EAC0 - 0x0011EB90 (0xD0) PKR_GetAssetInfo__FP19st_PACKER_READ_DATAUiP20st_PKR_ASSET_TOCINFO
static signed int PKR_GetAssetInfo(class st_PACKER_READ_DATA *pr, unsigned int aid, class st_PKR_ASSET_TOCINFO *tocinfo) {
    { // 0x0011EAC0 - 0x0011EB90
        class st_PACKER_ATOC_NODE *assnode; // r5
        signed int idx; // r2
    }
}

// 0x0011EB90 - 0x0011EB98 (0x8) PKR_GetBaseSector__FP19st_PACKER_READ_DATA
static unsigned int PKR_GetBaseSector(class st_PACKER_READ_DATA *pr) {
    { // 0x0011EB90 - 0x0011EB98
    }
}

// 0x0011EBA0 - 0x0011EBFC (0x5C) PKR_AssetName__FP19st_PACKER_READ_DATAUi
static char *PKR_AssetName(class st_PACKER_READ_DATA *pr, unsigned int aid) {
    { // 0x0011EBA0 - 0x0011EBFC
        signed int idx; // r2
        char *da_name; // r16
    }
}

// 0x0011EC00 - 0x0011EC08 (0x8) PKR_getPackTimestamp__FP19st_PACKER_READ_DATA
static unsigned int PKR_getPackTimestamp(class st_PACKER_READ_DATA *pr) {
    { // 0x0011EC00 - 0x0011EC08
    }
}

// 0x0011EC10 - 0x0011EC80 (0x70) PKR_IsAssetReady__FP19st_PACKER_READ_DATAUi
static signed int PKR_IsAssetReady(class st_PACKER_READ_DATA *pr, unsigned int aid) {
    { // 0x0011EC10 - 0x0011EC80
        signed int idx; // r2
        signed int is_ok; // r16
    }
}

// 0x0011EC80 - 0x0011ED2C (0xAC) PKR_AssetByType__FP19st_PACKER_READ_DATAUiiPUi
static void *PKR_AssetByType(class st_PACKER_READ_DATA *pr, unsigned int type, signed int idx, unsigned int *size) {
    { // 0x0011EC80 - 0x0011ED2C
        class st_PACKER_ATOC_NODE *assnode; // r3
        class st_XORDEREDARRAY *typlist; // r4
    }
}

// 0x0011ED30 - 0x0011ED98 (0x68) PKR_AssetCount__FP19st_PACKER_READ_DATAUi
static signed int PKR_AssetCount(class st_PACKER_READ_DATA *pr, unsigned int type) {
    { // 0x0011ED30 - 0x0011ED98
        signed int cnt; // r2
    }
}

// 0x0011EDA0 - 0x0011EE1C (0x7C) PKR_GetAssetSize__FP19st_PACKER_READ_DATAUi
static unsigned int PKR_GetAssetSize(class st_PACKER_READ_DATA *pr, unsigned int aid) {
    { // 0x0011EDA0 - 0x0011EE1C
        class st_PACKER_ATOC_NODE *assnode; // r16
        signed int idx; // r2
    }
}

// 0x0011EE20 - 0x0011EE84 (0x64) PKR_LoadAsset__FP19st_PACKER_READ_DATAUiPCcPv
static void *PKR_LoadAsset(class st_PACKER_READ_DATA *pr, unsigned int aid) {
    { // 0x0011EE20 - 0x0011EE84
    }
}

// 0x0011EE90 - 0x0011EE98 (0x8) PKR_LoadLayer__FP19st_PACKER_READ_DATA13en_LAYER_TYPE
static signed int PKR_LoadLayer() {
    { // 0x0011EE90 - 0x0011EE98
    }
}

// 0x0011EEA0 - 0x0011EF80 (0xE0) PKR_xform_asset__FP19st_PACKER_ATOC_NODE
static void PKR_xform_asset(class st_PACKER_ATOC_NODE *assnode) {
    { // 0x0011EEA0 - 0x0011EF80
        class st_PACKER_ASSETTYPE *atype; // r4
        char *xformloc; // r2
    }
}

// 0x0011EF80 - 0x0011F078 (0xF8) PKR_xformLayerAssets__FP19st_PACKER_LTOC_NODE
static void PKR_xformLayerAssets(class st_PACKER_LTOC_NODE *laynode) {
    { // 0x0011EF80 - 0x0011F078
        class st_PACKER_ATOC_NODE *tmpass; // r18
        signed int will_be_dumped; // r17
        signed int i; // r16
    }
}

// 0x0011F080 - 0x0011F170 (0xF0) PKR_updateLayerAssets__FP19st_PACKER_LTOC_NODE
static void PKR_updateLayerAssets(class st_PACKER_LTOC_NODE *laynode) {
    { // 0x0011F080 - 0x0011F170
        signed int lay_hip_pos; // r7
        class st_PACKER_ATOC_NODE *tmpass; // r6
        signed int i; // r5
    }
}

// 0x0011F170 - 0x0011F290 (0x120) PKR_findNextLayerToLoad__FPP19st_PACKER_READ_DATAPP19st_PACKER_LTOC_NODE
static signed int PKR_findNextLayerToLoad(class st_PACKER_READ_DATA **work_on_pkg, class st_PACKER_LTOC_NODE **next_layer) {
    { // 0x0011F170 - 0x0011F290
        signed int j; // r6
        signed int i; // r8
        class st_PACKER_LTOC_NODE *tmplay; // r7
        class st_PACKER_READ_DATA *tmppr; // r6
    }
}

// 0x0011F290 - 0x0011F7BC (0x52C) PKR_LoadStep_Async__Fv
static signed int PKR_LoadStep_Async() {
    { // 0x0011F290 - 0x0011F7BC
        class st_PACKER_ATOC_NODE *tmpass; // r19
        enum en_READ_ASYNC_STATUS readstat; // r2
        signed int rc; // r2
        signed int moretodo; // r16
        static class st_PACKER_READ_DATA *curpr; // 0x00357C60
        static class st_PACKER_LTOC_NODE *asynlay; // 0x00357C68
        static signed char init; // 0x00357C6C
        static signed char init; // 0x00357C64
    }
}

// 0x0011F7C0 - 0x0011F9D4 (0x214) PKR_parse_TOC__FP14st_HIPLOADDATAP19st_PACKER_READ_DATA
static signed int PKR_parse_TOC(class st_HIPLOADDATA *pkg, class st_PACKER_READ_DATA *pr) {
    { // 0x0011F7C0 - 0x0011F9D4
        signed int done; // r17
        unsigned int cid; // r2
        signed int is_ok; // r16
    }
}

// 0x0011F9E0 - 0x0011FB38 (0x158) PKR_SetActive__FP19st_PACKER_READ_DATA13en_LAYER_TYPE
static signed int PKR_SetActive(class st_PACKER_READ_DATA *pr, enum en_LAYER_TYPE layer) {
    { // 0x0011F9E0 - 0x0011FB38
        class st_PACKER_LTOC_NODE *laynode; // r19
        class st_PACKER_ATOC_NODE *assnode; // r18
        signed int j; // r17
        signed int i; // r23
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0011FB40 - 0x0011FD78 (0x238) PKR_ReadDone__FP19st_PACKER_READ_DATA
static void PKR_ReadDone(class st_PACKER_READ_DATA *pr) {
    { // 0x0011FB40 - 0x0011FD78
        class st_XORDEREDARRAY *tmplist; // r2
        class st_PACKER_LTOC_NODE *laynode; // r19
        class st_PACKER_ATOC_NODE *assnode; // r5
        signed int lockid; // r17
        signed int j; // r18
        signed int i; // r17
    }
}

// 0x0011FD80 - 0x0011FF98 (0x218) PKR_ReadInit__FPvPcUiPiP19st_PACKER_ASSETTYPE
static class st_PACKER_READ_DATA *PKR_ReadInit(void *userdata, char *pkgfile, unsigned int opts, signed int *cltver, class st_PACKER_ASSETTYPE *typelist) {
    { // 0x0011FD80 - 0x0011FF98
        char *tocbuf_aligned; // r18
        char *tocbuf_RAW; // r2
        signed int uselock; // r17
        signed int i; // r5
        class st_PACKER_READ_DATA *pr; // r16
    }
}

// 0x0011FFA0 - 0x0011FFA8 (0x8) PKRLoadStep__Fi
signed int PKRLoadStep() {
    { // 0x0011FFA0 - 0x0011FFA8
        signed int more_todo; // r2
    }
}

// 0x0011FFB0 - 0x0011FFC8 (0x18) PKRShutdown__Fv
signed int PKRShutdown() {
    { // 0x0011FFB0 - 0x0011FFC8
    }
}

// 0x0011FFD0 - 0x00120010 (0x40) PKRStartup__Fv
signed int PKRStartup() {
    { // 0x0011FFD0 - 0x00120010
    }
}

// 0x00120010 - 0x00120030 (0x20) PKRGetReadFuncs__Fi
class st_PACKER_READ_FUNCS *PKRGetReadFuncs(signed int apiver) {
    { // 0x00120010 - 0x00120030
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xDebug.cpp
// ==========================================================================
unsigned int gDBfont; // 0x003580DC
char *sDebugModeName[45]; // 0x00368DB0
void (*sDebugModeCallbackFunc[45])(); // 0x00368CF0
unsigned int gFrameCount; // 0x00357C78
static char dm_names[13][16]; // 0x0028EE60
// 0x00120030 - 0x00120038 (0x8) xDebugTimestampScreen__Fv
void xDebugTimestampScreen() {
    { // 0x00120030 - 0x00120038
    }
}

// 0x00120040 - 0x00120048 (0x8) xDebugExit__Fv
void xDebugExit() {
    { // 0x00120040 - 0x00120048
    }
}

// 0x00120050 - 0x00120058 (0x8) xDebugUpdate__Fv
void xDebugUpdate() {
    { // 0x00120050 - 0x00120058
    }
}

// 0x00120060 - 0x001200B8 (0x58) xDebugInit__Fv
void xDebugInit() {
    { // 0x00120060 - 0x001200B8
        signed int i; // r4
    }
}

// 0x001200C0 - 0x001200D8 (0x18) xDebugModeRegisterCallback__FiPFv_v
void xDebugModeRegisterCallback(signed int mode, void (*func)()) {
    { // 0x001200C0 - 0x001200D8
    }
}

// 0x001200E0 - 0x0012011C (0x3C) xDebugModeAdd__FPCcPFv_v
signed int xDebugModeAdd(char *debugModeName, void (*func)()) {
    { // 0x001200E0 - 0x0012011C
        signed int modeid; // r2
    }
}

// 0x00120120 - 0x001201D0 (0xB0) xDebugModeAdd__FPCc
signed int xDebugModeAdd(char *debugModeName) {
    { // 0x00120120 - 0x001201D0
        signed int i; // r18
    }
}

// 0x001201D0 - 0x001201F8 (0x28) xprintf__FPCce
void xprintf() {
    { // 0x001201D0 - 0x001201F8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xstransvc.cpp
// ==========================================================================
static class st_PACKER_ASSETTYPE *g_typeHandlers; // 0x00357C84
static class st_PACKER_READ_FUNCS *g_pkrf; // 0x00357C80
static class st_STRAN_DATA g_xstdata; // 0x0028EF30
static signed int g_straninit; // 0x00357C7C
// 0x00120200 - 0x00120258 (0x58) XST_find_bySID__FUi
static class st_STRAN_SCENE *XST_find_bySID(unsigned int sid) {
    { // 0x00120200 - 0x00120258
        signed int i; // r6
        class st_STRAN_SCENE *da_sdata; // r2
    }
}

// 0x00120260 - 0x00120308 (0xA8) XST_lock_next__Fv
static class st_STRAN_SCENE *XST_lock_next() {
    { // 0x00120260 - 0x00120308
        signed int uselock; // r4
        signed int i; // r16
        class st_STRAN_SCENE *sdata; // r17
    }
}

// 0x00120310 - 0x00120450 (0x140) xST_xAssetID_HIPFullPath__FUiPUi
char *xST_xAssetID_HIPFullPath(unsigned int aid, unsigned int *sceneID) {
    { // 0x00120310 - 0x00120450
        signed int i; // r17
        signed int rc; // r2
        char *da_hipname; // r16
    }
}

// 0x00120450 - 0x00120458 (0x8) xST_xAssetID_HIPFullPath__FUi
char *xST_xAssetID_HIPFullPath(unsigned int aid) {
    { // 0x00120450 - 0x00120458
    }
}

// 0x00120460 - 0x00120654 (0x1F4) xSTGetAssetInfoByType__FUiiP17st_xST_XASSETINFO
signed int xSTGetAssetInfoByType(unsigned int type, signed int idx, class st_xST_XASSETINFO *ainfo) {
    { // 0x00120460 - 0x00120654
        signed int cnt; // r21
        signed int sum; // r17
        signed int i; // r16
        signed int rc; // r2
        class st_PKR_ASSET_TOCINFO tocinfo; // r29+0xa0
        signed int found; // r30
    }
}

// 0x00120660 - 0x00120818 (0x1B8) xSTGetAssetInfo__FUiP17st_xST_XASSETINFO
signed int xSTGetAssetInfo(unsigned int aid, class st_xST_XASSETINFO *ainfo) {
    { // 0x00120660 - 0x00120818
        signed int i; // r16
        signed int rc; // r2
        class st_PKR_ASSET_TOCINFO tocinfo; // r29+0x70
        signed int found; // r21
    }
}

// 0x00120820 - 0x001209A0 (0x180) xSTFindAssetByType__FUiiPUi
void *xSTFindAssetByType(unsigned int type, signed int idx, unsigned int *size) {
    { // 0x00120820 - 0x001209A0
        signed int cnt; // r2
        signed int sum; // r17
        signed int i; // r16
        void *memptr; // r22
    }
}

// 0x001209A0 - 0x00120AB0 (0x110) xSTAssetCountByType__FUi
signed int xSTAssetCountByType(unsigned int type) {
    { // 0x001209A0 - 0x00120AB0
        signed int i; // r17
        signed int cnt; // r2
        signed int sum; // r16
    }
}

// 0x00120AB0 - 0x00120C48 (0x198) xSTFindAsset__FUiPUi
void *xSTFindAsset(unsigned int aid, unsigned int *size) {
    { // 0x00120AB0 - 0x00120C48
        signed int rc; // r2
        signed int i; // r17
        signed int ready; // r2
        void *memloc; // r16
    }
}

// 0x00120C50 - 0x00120D68 (0x118) xSTAssetName__FUi
char *xSTAssetName(unsigned int aid) {
    { // 0x00120C50 - 0x00120D68
        signed int i; // r16
        char *aname; // r2
    }
}

// 0x00120D70 - 0x00120E24 (0xB4) xSTSwitchScene__FUiPvPFPvf_i
signed int xSTSwitchScene(unsigned int sid, void *userdata, signed int (*progmon)(void *, float)) {
    { // 0x00120D70 - 0x00120E24
        signed int rc; // r2
        class st_STRAN_SCENE *sdata; // r17
        signed int result; // r16
    }
}

// 0x00120E30 - 0x00120E68 (0x38) xSTLoadStep__FUi
float xSTLoadStep() {
    { // 0x00120E30 - 0x00120E68
        signed int rc; // r2
    }
}

// 0x00120E70 - 0x00121078 (0x208) xSTUnLoadScene__FUi
void xSTUnLoadScene(unsigned int sid) {
    { // 0x00120E70 - 0x00121078
        signed int i; // r16
        class st_STRAN_SCENE *sdata; // r16
    }
}

// 0x00121080 - 0x001210D4 (0x54) xSTQueueSceneAssets__FUi
signed int xSTQueueSceneAssets(unsigned int sid) {
    { // 0x00121080 - 0x001210D4
        class st_STRAN_SCENE *sdata; // r2
        signed int result; // r16
    }
}

// 0x001210E0 - 0x00121340 (0x260) xSTPreLoadScene__FUiPv
signed int xSTPreLoadScene(unsigned int sid, void *userdata) {
    { // 0x001210E0 - 0x00121340
        signed int cnt_retry; // r19
        signed int cltver; // r18
        class st_STRAN_SCENE *sdata; // r17
        signed int result; // r2
    }
}

// 0x00121340 - 0x00121400 (0xC0) xSTShutdown__Fv
signed int xSTShutdown() {
    { // 0x00121340 - 0x00121400
    }
}

// 0x00121400 - 0x00121454 (0x54) xSTStartup__FP19st_PACKER_ASSETTYPE
signed int xSTStartup(class st_PACKER_ASSETTYPE *handlers) {
    { // 0x00121400 - 0x00121454
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iModel.cpp
// ==========================================================================
class RpAtomic * (FindAtomicCallback)(class RpAtomic *, void *); // 0x00122310
class RpAtomic * (NextAtomicCallback)(class RpAtomic *, void *); // 0x001221A0
class RpHAnimAtomicGlobalVars RpHAnimAtomicGlobals; // 0x002CAA88
class zGlobals globals; // 0x00362FD0
unsigned int ourGlobals[4096]; // 0x002B5AD0
unsigned char *giAnimScratch; // 0x003574A0
static class RpAtomic *gLastAtomicList[256]; // 0x00368EB0
static unsigned int gLastAtomicCount; // 0x00357C88
// 0x00121460 - 0x00121744 (0x2E4) iModelTagEval__FP8RpAtomicP9iModelTagP11RwMatrixTagP6_xVec3
void iModelTagEval(class RpAtomic *model, class iModelTag *tag, class RwMatrixTag *mat, class _xVec3 *dest) {
    { // 0x00121460 - 0x00121744
    }
}

// 0x00121750 - 0x00121758 (0x8) iModelTagSetup__FP9iModelTagP8RpAtomicfff
unsigned int iModelTagSetup(class iModelTag *tag, class RpAtomic *model, float x, float y, float z) {
    { // 0x00121750 - 0x00121758
    }
}

// 0x00121760 - 0x00121774 (0x14) iModelTagIndex__FP9iModelTagP8RpAtomici
unsigned int iModelTagIndex(class iModelTag *tag, class RpAtomic *model, signed int v) {
    { // 0x00121760 - 0x00121774
    }
}

// 0x00121780 - 0x00121904 (0x184) iModelTagInternal__FP9iModelTagP8RpAtomicfffi
static unsigned int iModelTagInternal(class iModelTag *tag, class RpAtomic *model, float x, float y, float z, signed int closeV) {
    { // 0x00121780 - 0x00121904
        class RwMatrixWeights *wt; // r18
        class RpSkin *skin; // r18
        float closeDistSqr; // 
        float distSqr; // r29+0x40
        signed int numV; // r4
        signed int v; // r3
        class RwV3d *vert; // r2
        class RpGeometry *geom; // r3
    }
}

// 0x00121910 - 0x00121C88 (0x378) iModelVertEval__FP8RpAtomicUiUiP11RwMatrixTagP6_xVec3P6_xVec3
unsigned int iModelVertEval(class RpAtomic *model, unsigned int index, unsigned int count, class RwMatrixTag *mat, class _xVec3 *vert, class _xVec3 *dest) {
    { // 0x00121910 - 0x00121C88
        class RpSkin *skin; // r16
        unsigned int numV; // r3
        class RpGeometry *geom; // r2
    }
}

// 0x00121C90 - 0x00121C9C (0xC) iModelVertCount__FP8RpAtomic
unsigned int iModelVertCount(class RpAtomic *model) {
    { // 0x00121C90 - 0x00121C9C
    }
}

// 0x00121CA0 - 0x00121D9C (0xFC) iModelCull__FP8RpAtomicP11RwMatrixTag
signed int iModelCull(class RpAtomic *model, class RwMatrixTag *mat) {
    { // 0x00121CA0 - 0x00121D9C
    }
}

// 0x00121DA0 - 0x00121EC4 (0x124) iModelRender__FP8RpAtomicP11RwMatrixTag
void iModelRender(class RpAtomic *model, class RwMatrixTag *mat) {
    { // 0x00121DA0 - 0x00121EC4
        class RwFrame *frame; // r3
        class RwMatrixTag *pAnimOldMatrix; // r16
    }
}

// 0x00121ED0 - 0x0012208C (0x1BC) iModelAnimMatrices__FP8RpAtomicP6_xQuatP6_xVec3P11RwMatrixTag
void iModelAnimMatrices(class RpAtomic *model, class _xQuat *quat, class _xVec3 *tran, class RwMatrixTag *mat) {
    { // 0x00121ED0 - 0x0012208C
        class RwMatrixTag *pMatrixArray; // r2
        signed int numFrames; // r13
        signed int i; // r12
        signed int pCurrentFrameFlags; // r11
        class RpHAnimNodeInfo *pCurrentFrame; // r4
        class RwMatrixTag *pMatrixStackTop; // r2
        class RwMatrixTag matrixStack[32]; // r29+0x0
        class RpHAnimHierarchy *pHierarchy; // r2
    }
}

// 0x00122090 - 0x00122100 (0x70) iModelQuatToMat__FP6_xQuatP6_xVec3P11RwMatrixTag
void iModelQuatToMat(class _xQuat *quat, class _xVec3 *tran, class RwMatrixTag *mat) {
    { // 0x00122090 - 0x00122100
    }
}

// 0x00122100 - 0x00122144 (0x44) iModelNumBones__FP8RpAtomic
unsigned int iModelNumBones(class RpAtomic *model) {
    { // 0x00122100 - 0x00122144
    }
}

// 0x00122150 - 0x00122194 (0x44) iModelFile_RWMultiAtomic__FP8RpAtomic
class RpAtomic *iModelFile_RWMultiAtomic(class RpAtomic *model) {
    { // 0x00122150 - 0x00122194
        class RpAtomic *nextModel; // r29+0x1c
        class RpClump *clump; // r3
    }
}

// 0x001221A0 - 0x001221CC (0x2C) NextAtomicCallback__FP8RpAtomicPv
static class RpAtomic *NextAtomicCallback(class RpAtomic *atomic, void *data) {
    { // 0x001221A0 - 0x001221CC
        class RpAtomic **nextModel; // r2
    }
}

// 0x001221D0 - 0x00122240 (0x70) iModelUnload__FP8RpAtomic
void iModelUnload(class RpAtomic *userdata) {
    { // 0x001221D0 - 0x00122240
        class RwFrame *root; // r2
        class RwFrame *frame; // r17
        class RpClump *clump; // r16
    }
}

// 0x00122240 - 0x00122304 (0xC4) iModelFileNew__FPvUi
class RpAtomic *iModelFileNew(void *buffer, unsigned int size) {
    { // 0x00122240 - 0x00122304
        class RwMemory rwmem; // r29+0x38
    }
}

// 0x00122310 - 0x00122424 (0x114) FindAtomicCallback__FP8RpAtomicPv
static class RpAtomic *FindAtomicCallback(class RpAtomic *atomic) {
    { // 0x00122310 - 0x00122424
        class RpSkin *pSkin; // r18
        class RpHAnimHierarchy *pHier; // r2
    }
}

// 0x00122430 - 0x00122438 (0x8) iModelInit__Fv
void iModelInit() {
    { // 0x00122430 - 0x00122438
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xModel.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
class _xMat4x3 g_I3; // 0x00342E70
unsigned int ourGlobals[4096]; // 0x002B5AD0
class RwRaster *ShadowRenderRaster; // 0x00000000
static class RwCamera *subcamera; // 0x00357CA0
static class xModelPool *sxModelPoolList; // 0x00357C98
// 0x00122440 - 0x00122448 (0x8) xModel_SceneExit__FP7RpWorld
void xModel_SceneExit(class RpWorld *world) {
    { // 0x00122440 - 0x00122448
    }
}

// 0x00122450 - 0x00122458 (0x8) xModel_SceneEnter__FP7RpWorld
void xModel_SceneEnter(class RpWorld *world) {
    { // 0x00122450 - 0x00122458
    }
}

// 0x00122460 - 0x00122468 (0x8) xModelResetMaterial__FP14xModelInstance
void xModelResetMaterial(class xModelInstance *modelInst) {
    { // 0x00122460 - 0x00122468
    }
}

// 0x00122470 - 0x00122478 (0x8) xModelMaterialMul__FP14xModelInstancefff
void xModelMaterialMul(class xModelInstance *modelInst, float rm, float gm, float bm) {
    { // 0x00122470 - 0x00122478
    }
}

// 0x00122480 - 0x00122488 (0x8) xModelSetMaterialAlpha__FP14xModelInstancei
void xModelSetMaterialAlpha(class xModelInstance *modelInst, signed int alpha) {
    { // 0x00122480 - 0x00122488
    }
}

// 0x00122490 - 0x00122498 (0x8) xModelSetMaterialTextureRestore__FP14xModelInstance
void xModelSetMaterialTextureRestore(class xModelInstance *modelInst) {
    { // 0x00122490 - 0x00122498
    }
}

// 0x001224A0 - 0x001224A8 (0x8) xModelSetMaterialTexture__FP14xModelInstancePv
void xModelSetMaterialTexture(class xModelInstance *modelInst, void *texture) {
    { // 0x001224A0 - 0x001224A8
    }
}

// 0x001224B0 - 0x0012253C (0x8C) CameraDestroy__FP8RwCamera
static void CameraDestroy(class RwCamera *camera) {
    { // 0x001224B0 - 0x0012253C
        class RwFrame *frame; // r16
        class RwRaster *raster; // r2
    }
}

// 0x00122540 - 0x0012296C (0x42C) xModelRender2D__FP14xModelInstanceP6_xRectP6_xVec3P6_xVec3f
void xModelRender2D(class xModelInstance *modelInst, class _xRect *sr, class _xVec3 *from, class _xVec3 *to) {
    { // 0x00122540 - 0x0012296C
        float shearY; // 
        float shearX; // 
        float viewscale; // 
        class RwV2d *camvw; // r2
        class RwMatrixTag shearmat; // r29+0x110
        class RwMatrixTag objmat; // r29+0xd0
        class RwMatrixTag *cammat; // r21
        class RwCamera *camera; // r20
    }
}

// 0x00122970 - 0x001229B8 (0x48) xModelRender__FP14xModelInstance
void xModelRender(class xModelInstance *modelInst) {
    { // 0x00122970 - 0x001229B8
    }
}

// 0x001229C0 - 0x00122CAC (0x2EC) xModelRenderSingle__FP14xModelInstance
void xModelRenderSingle(class xModelInstance *modelInst) {
    { // 0x001229C0 - 0x00122CAC
        unsigned short i; // r5
        class xAnimPlay *a; // r4
    }
}

// 0x00122CB0 - 0x00122CF8 (0x48) xModelEval__FP14xModelInstance
void xModelEval(class xModelInstance *modelInst) {
    { // 0x00122CB0 - 0x00122CF8
    }
}

// 0x00122D00 - 0x00122E78 (0x178) xModelEvalSingle__FP14xModelInstance
void xModelEvalSingle(class xModelInstance *modelInst) {
    { // 0x00122D00 - 0x00122E78
        unsigned char *remap; // r17
        class xModelInstance *dad; // r2
        signed int i; // r16
    }
}

// 0x00122E80 - 0x00122F04 (0x84) xModelUpdate__FP14xModelInstancef
void xModelUpdate(class xModelInstance *modelInst, float timeDelta) {
    { // 0x00122E80 - 0x00122F04
    }
}

// 0x00122F10 - 0x00122F44 (0x34) xModelInstanceAttach__FP14xModelInstanceP14xModelInstance
void xModelInstanceAttach(class xModelInstance *inst, class xModelInstance *parent) {
    { // 0x00122F10 - 0x00122F44
        class xModelInstance *curr; // r3
    }
}

// 0x00122F50 - 0x00123024 (0xD4) xModelInstanceFree__FP14xModelInstance
void xModelInstanceFree(class xModelInstance *modelInst) {
    { // 0x00122F50 - 0x00123024
        class xModelInstance **prev; // r2
        class xModelInstance *curr; // r3
        class xModelInstance *child; // r2
    }
}

// 0x00123030 - 0x0012317C (0x14C) xModelInstanceAlloc__FP8RpAtomicPvUsUcPUc
class xModelInstance *xModelInstanceAlloc(class RpAtomic *data, void *object, unsigned short flags, unsigned char boneIndex, unsigned char *boneRemap) {
    { // 0x00123030 - 0x0012317C
        class xModelInstance *dude; // r19
        class xModelPool *found; // r18
        class xModelPool *curr; // r2
        unsigned int matCount; // r17
        unsigned int boneCount; // r2
        signed int i; // r16
    }
}

// 0x00123180 - 0x001233EC (0x26C) xModelPoolInit__FUiUi
void xModelPoolInit(unsigned int count, unsigned int numMatrices) {
    { // 0x00123180 - 0x001233EC
        class xModelInstance *inst; // r6
        class xModelPool **prev; // r2
        class xModelPool *curr; // r4
        class xModelPool *pool; // r5
        class RwMatrixTag *mat; // r4
        unsigned char *buffer; // r2
        signed int i; // r3
    }
}

// 0x001233F0 - 0x001234C4 (0xD4) xModelInit__Fv
void xModelInit() {
    { // 0x001233F0 - 0x001234C4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iSystem.cpp
// ==========================================================================
signed int sweMaxFlips; // 0x003576E0
class RwTexture * (TextureRead)(char *, char *); // 0x001237A0
signed int (vsyncCounterHandler)(signed int); // 0x001239F0
unsigned int gVsyncCount; // 0x00357CA8
signed int gFB_Flags; // 0x003574A8
class RwVideoMode sVideoMode; // 0x003692B0
signed int DVD; // 0x00357CA4
void *hackResourceArenaPointer; // 0x00000000
// 0x001234D0 - 0x00123628 (0x158) iGetCurrFormattedTime__FPc
unsigned int iGetCurrFormattedTime(char *str) {
    { // 0x001234D0 - 0x00123628
        class sceCdCLOCK clock; // r29+0x28
    }
}

// 0x00123630 - 0x00123724 (0xF4) iGetCurrFormattedDate__FPc
unsigned int iGetCurrFormattedDate(char *str) {
    { // 0x00123630 - 0x00123724
        class sceCdCLOCK clock; // r29+0x28
    }
}

// 0x00123730 - 0x0012375C (0x2C) iGetMonth__Fv
unsigned char iGetMonth() {
    { // 0x00123730 - 0x0012375C
        class sceCdCLOCK clock; // r29+0x18
    }
}

// 0x00123760 - 0x0012378C (0x2C) iGetDay__Fv
unsigned char iGetDay() {
    { // 0x00123760 - 0x0012378C
        class sceCdCLOCK clock; // r29+0x18
    }
}

// 0x00123790 - 0x00123798 (0x8) iVSync__Fv
void iVSync() {
    { // 0x00123790 - 0x00123798
    }
}

// 0x001237A0 - 0x00123850 (0xB0) TextureRead__FPCcPCc
static class RwTexture *TextureRead(char *name, char *maskName) {
    { // 0x001237A0 - 0x00123850
        unsigned int tmpsize; // r29+0x14c
        unsigned int assetid; // r2
        class RwTexture *result; // r16
        char tmpname[256]; // r29+0x40
    }
}

// 0x00123850 - 0x001239E8 (0x198) RenderWareInit__Fv
static unsigned int RenderWareInit() {
    { // 0x00123850 - 0x001239E8
        class RwEngineOpenParams openParams; // r29+0x1c
    }
}

// 0x001239F0 - 0x00123A04 (0x14) vsyncCounterHandler__Fi
static signed int vsyncCounterHandler() {
    { // 0x001239F0 - 0x00123A04
    }
}

// 0x00123A10 - 0x00123A84 (0x74) iSystemExit__Fv
void iSystemExit() {
    { // 0x00123A10 - 0x00123A84
    }
}

// 0x00123A90 - 0x00123BE8 (0x158) iSystemInit__Fv
void iSystemInit() {
    { // 0x00123A90 - 0x00123BE8
        signed int disk_type; // r2
        char *iopImageFile; // r16
    }
}

// 0x00123BF0 - 0x00123CDC (0xEC) psSelectDevice__Fv
unsigned int psSelectDevice() {
    { // 0x00123BF0 - 0x00123CDC
        signed int num; // r17
        signed int i; // r16
        class RwVideoMode videoMode; // r29+0x40
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iMath3.cpp
// ==========================================================================
// 0x00123CE0 - 0x00123D8C (0xAC) iBoxBoundVec__FP5_xBoxPC5_xBoxPC6_xVec3
void iBoxBoundVec(class _xBox *o, class _xBox *b, class _xVec3 *v) {
    { // 0x00123CE0 - 0x00123D8C
    }
}

// 0x00123D90 - 0x00123DC4 (0x34) iBoxInitBoundVec__FP5_xBoxPC6_xVec3
void iBoxInitBoundVec(class _xBox *b, class _xVec3 *v) {
    { // 0x00123D90 - 0x00123DC4
    }
}

// 0x00123DD0 - 0x0012416C (0x39C) iBoxIsectSphere__FPC5_xBoxPC8_xSphereP7_xIsect
void iBoxIsectSphere(class _xBox *box, class _xSphere *p, class _xIsect *isx) {
    { // 0x00123DD0 - 0x0012416C
        unsigned int zcode; // r2
        unsigned int ycode; // r2
        unsigned int xcode; // r2
    }
}

// 0x00124170 - 0x00124444 (0x2D4) iBoxIsectRay__FPC5_xBoxPC6_xRay3P7_xIsect
void iBoxIsectRay(class _xBox *b, class _xRay3 *r, class _xIsect *isx) {
    { // 0x00124170 - 0x00124444
        float t_out; // r29+0x9c
        float t_in; // r29+0x98
    }
}

// 0x00124450 - 0x00124540 (0xF0) ClipPlane__FffPfPf
static unsigned int ClipPlane(float denom, float numer, float *t_in, float *t_out) {
    { // 0x00124450 - 0x00124540
    }
}

// 0x00124540 - 0x001245DC (0x9C) iBoxIsectVec__FPC5_xBoxPC6_xVec3P7_xIsect
void iBoxIsectVec(class _xBox *b, class _xVec3 *v, class _xIsect *isx) {
    { // 0x00124540 - 0x001245DC
    }
}

// 0x001245E0 - 0x00125030 (0xA50) iBoxVecDist__FPC5_xBoxPC6_xVec3P7_xIsect
void iBoxVecDist(class _xBox *box, class _xVec3 *v, class _xIsect *isx) {
    { // 0x001245E0 - 0x00125030
    }
}

// 0x00125030 - 0x00125334 (0x304) iCylinderIsectRay__FPC10_xCylinderPC6_xRay3P7_xIsect
void iCylinderIsectRay(class _xCylinder *c, class _xRay3 *r, class _xIsect *isx) {
    { // 0x00125030 - 0x00125334
        unsigned int num; // r2
        float t_out; // r29+0x4c
        float t_in; // r29+0x48
        float b; // 
        float a; // r29+0x50
    }
}

// 0x00125340 - 0x001253E4 (0xA4) iCylinderIsectVec__FPC10_xCylinderPC6_xVec3P7_xIsect
void iCylinderIsectVec(class _xCylinder *c, class _xVec3 *v, class _xIsect *isx) {
    { // 0x00125340 - 0x001253E4
        float b; // 
    }
}

// 0x001253F0 - 0x00125530 (0x140) iSphereInitBoundOBB__FP8_xSpherePC5_xBoxPC8_xMat4x3
void iSphereInitBoundOBB(class _xSphere *s, class _xBox *b, class _xMat4x3 *m) {
    { // 0x001253F0 - 0x00125530
    }
}

// 0x00125530 - 0x001255E0 (0xB0) iSphereIsectSphere__FPC8_xSpherePC8_xSphereP7_xIsect
void iSphereIsectSphere(class _xSphere *s, class _xSphere *p, class _xIsect *isx) {
    { // 0x00125530 - 0x001255E0
    }
}

// 0x001255E0 - 0x001257D0 (0x1F0) iSphereIsectRay__FPC8_xSpherePC6_xRay3P7_xIsect
void iSphereIsectRay(class _xSphere *s, class _xRay3 *r, class _xIsect *isx) {
    { // 0x001255E0 - 0x001257D0
        unsigned int num; // r2
        float t_out; // r29+0x3c
        float t_in; // r29+0x38
    }
}

// 0x001257D0 - 0x0012585C (0x8C) iSphereIsectVec__FPC8_xSpherePC6_xVec3P7_xIsect
void iSphereIsectVec(class _xSphere *s, class _xVec3 *v, class _xIsect *isx) {
    { // 0x001257D0 - 0x0012585C
    }
}

// 0x00125860 - 0x00125868 (0x8) iMath3Exit__Fv
void iMath3Exit() {
    { // 0x00125860 - 0x00125868
    }
}

// 0x00125870 - 0x00125878 (0x8) iMath3Init__Fv
void iMath3Init() {
    { // 0x00125870 - 0x00125878
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xMemMgr.cpp
// ==========================================================================
unsigned int gMemReportingThreshold; // 0x00000000
signed int gMemReporting; // 0x00000000
void (*sMemBaseNotifyFunc)(); // 0x00357CB0
class xMemHeap_tag gxHeap[3]; // 0x003692F0
unsigned int gActiveHeap; // 0x00357CAC
class xMemInfo_tag gMemInfo; // 0x003692C0
// 0x00125880 - 0x001258E8 (0x68) xMemPoolFree__FP8xMemPoolPv
void xMemPoolFree(class xMemPool *pool, void *data) {
    { // 0x00125880 - 0x001258E8
        void *curr; // r3
        void **prev; // r2
        unsigned int next; // r7
        void *freeList; // r6
    }
}

// 0x001258F0 - 0x00125984 (0x94) xMemPoolAlloc__FP8xMemPool
void *xMemPoolAlloc(class xMemPool *pool) {
    { // 0x001258F0 - 0x00125984
        unsigned int flags; // r18
        unsigned int next; // r17
        void *retval; // r2
    }
}

// 0x00125990 - 0x001259BC (0x2C) xMemPoolSetup__FP8xMemPoolPvUiUiPFP8xMemPoolPv_vUiUiUi
void xMemPoolSetup(class xMemPool *pool, void *buffer, unsigned int nextOffset, unsigned int flags, void (*initCB)(class xMemPool *, void *), unsigned int size, unsigned int count, unsigned int numRealloc) {
    { // 0x00125990 - 0x001259BC
    }
}

// 0x001259C0 - 0x00125A8C (0xCC) xMemPoolAddElements__FP8xMemPoolPvUi
static void xMemPoolAddElements(class xMemPool *pool, void *buffer, unsigned int count) {
    { // 0x001259C0 - 0x00125A8C
        unsigned int size; // r19
        unsigned int next; // r18
        void (*initCB)(class xMemPool *, void *); // r17
        void *curr; // r2
        signed int i; // r16
    }
}

// 0x00125A90 - 0x00125ABC (0x2C) xMemGetBase__Fv
signed int xMemGetBase() {
    { // 0x00125A90 - 0x00125ABC
    }
}

// 0x00125AC0 - 0x00125AC8 (0x8) xMemRegisterBaseNotifyFunc__FPFv_v
void xMemRegisterBaseNotifyFunc(void (*func)()) {
    { // 0x00125AC0 - 0x00125AC8
    }
}

// 0x00125AD0 - 0x00125B38 (0x68) xMemPopBase__Fi
signed int xMemPopBase(signed int depth) {
    { // 0x00125AD0 - 0x00125B38
    }
}

// 0x00125B40 - 0x00125BDC (0x9C) xMemPushBase__Fv
signed int xMemPushBase() {
    { // 0x00125B40 - 0x00125BDC
    }
}

// 0x00125BE0 - 0x00125BE8 (0x8) xMemPopTemp__FPv
void xMemPopTemp(void *addr) {
    { // 0x00125BE0 - 0x00125BE8
    }
}

// 0x00125BF0 - 0x00125BF8 (0x8) xMemPushTemp__FUi
void *xMemPushTemp(unsigned int size) {
    { // 0x00125BF0 - 0x00125BF8
        void *addr; // r2
    }
}

// 0x00125C00 - 0x00125D18 (0x118) xMemAlloc__FUiUi
void *xMemAlloc(unsigned int heapID, unsigned int size) {
    { // 0x00125C00 - 0x00125D18
        unsigned int total; // r2
        class xMemBlkInfo_tag info; // r29+0x50
        class xHeapState_tag *sp; // r18
        class xMemBlock_tag *hdr; // r17
        class xMemHeap_tag *heap; // r16
    }
}

// 0x00125D20 - 0x00125E34 (0x114) xMemGrowAlloc__FUiUi
void *xMemGrowAlloc(unsigned int heapID, unsigned int size) {
    { // 0x00125D20 - 0x00125E34
        void *memptr; // r16
        class xHeapState_tag *sp; // r3
        class xMemBlock_tag *hdr; // r2
        class xMemHeap_tag *heap; // r8
        unsigned int oldalignsize; // r9
        unsigned int align; // r9
    }
}

// 0x00125E40 - 0x00125F4C (0x10C) xMemGetBlockInfo__FP12xMemHeap_tagUiP15xMemBlkInfo_tag
static unsigned int xMemGetBlockInfo(class xMemHeap_tag *heap, unsigned int size, class xMemBlkInfo_tag *info) {
    { // 0x00125E40 - 0x00125F4C
        class xHeapState_tag *sp; // r10
        signed int post; // r9
        signed int block; // r8
        signed int pre; // r7
        signed int hdr; // r3
        unsigned int total; // r2
        unsigned int align; // r14
    }
}

// 0x00125F50 - 0x00126044 (0xF4) xMemInitHeap__FP12xMemHeap_tagUiUiUi
void xMemInitHeap(class xMemHeap_tag *heap, unsigned int base, unsigned int size, unsigned int flags) {
    { // 0x00125F50 - 0x00126044
        class xHeapState_tag *sp; // r2
        unsigned int align; // r9
        unsigned int old_base; // r8
    }
}

// 0x00126050 - 0x00126058 (0x8) xMemExit__Fv
void xMemExit() {
    { // 0x00126050 - 0x00126058
    }
}

// 0x00126060 - 0x00126100 (0xA0) xMemInit__Fv
void xMemInit() {
    { // 0x00126060 - 0x00126100
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iMemMgr.cpp
// ==========================================================================
char __data_start; // 0x00286EB0
char _memtop; // 0x02000000
char _end; // 0x003B3500
char _stack_size; // 0x00010000
char _heap_size; // 0x01C3CB00
char _stack; // 0x01FF0000
class xMemInfo_tag gMemInfo; // 0x003692C0
static void *sMemPiggyBank; // 0x00000000
unsigned int gPiggyBankSize; // 0x00000000
static unsigned int StackSize; // 0x00357CB8
static unsigned int StackBase; // 0x00357CB4
// 0x00126100 - 0x00126128 (0x28) iMemExit__Fv
void iMemExit() {
    { // 0x00126100 - 0x00126128
    }
}

// 0x00126130 - 0x0012635C (0x22C) iMemInit__Fv
void iMemInit() {
    { // 0x00126130 - 0x0012635C
        void *p; // r2
        unsigned int malloc_max; // r16
        unsigned int HeapBase; // r18
        unsigned int MemTop; // r17
        unsigned int MemBase; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iTime.cpp
// ==========================================================================
signed int (TimerHandler)(signed int); // 0x00126510
static float sGameTime; // 0x00357CEC
signed int initted; // 0x00000000
static unsigned int profSampleSize; // 0x00357CE8
static unsigned int *profSampleStatic; // 0x00357CE4
static unsigned int *profSampleBuffer; // 0x00357CE0
unsigned int prof_enable; // 0x00357CDC
unsigned int scene_id; // 0x003574AC
unsigned int vsync_hits; // 0x00357CD8
unsigned int kernel_hits; // 0x00357CD4
unsigned int app_hits; // 0x00357CD0
unsigned int app_end; // 0x00000000
unsigned int app_top; // 0x00000000
unsigned int pil; // 0x00357CCC
class prof_info *pip; // 0x00357CC8
unsigned int pvsyncaddr; // 0x00000000
static unsigned int sProfCount; // 0x00000000
static unsigned int *sProfBin; // 0x00000000
static char **sProfFile; // 0x00000000
static char **sProfFunc; // 0x00000000
static char *sProfMap; // 0x00000000
static signed int sProfLevel; // 0x00000000
signed long tim0_high; // 0x00357CC0
static signed int tim0_handler_id; // 0x00357CBC
// 0x00126360 - 0x00126368 (0x8) iFuncProfileFuncs__Fii
void iFuncProfileFuncs() {
    { // 0x00126360 - 0x00126368
    }
}

// 0x00126370 - 0x00126378 (0x8) iFuncProfileDump__Fv
void iFuncProfileDump() {
    { // 0x00126370 - 0x00126378
    }
}

// 0x00126380 - 0x00126388 (0x8) iInstrProfileDump__FPc
void iInstrProfileDump() {
    { // 0x00126380 - 0x00126388
    }
}

// 0x00126390 - 0x00126398 (0x8) iFuncProfileParse__FPci
void iFuncProfileParse() {
    { // 0x00126390 - 0x00126398
    }
}

// 0x001263A0 - 0x00126440 (0xA0) iProfileClear__FUi
void iProfileClear(unsigned int sceneID) {
    { // 0x001263A0 - 0x00126440
        signed int i; // r4
    }
}

// 0x00126440 - 0x00126448 (0x8) iTimeSetGame__Ff
void iTimeSetGame(float time) {
    { // 0x00126440 - 0x00126448
    }
}

// 0x00126450 - 0x00126460 (0x10) iTimeGameAdvance__Ff
void iTimeGameAdvance(float elapsed) {
    { // 0x00126450 - 0x00126460
    }
}

// 0x00126460 - 0x0012648C (0x2C) iTimeDiffSec__Fll
float iTimeDiffSec(signed long t0, signed long t1) {
    { // 0x00126460 - 0x0012648C
    }
}

// 0x00126490 - 0x001264C4 (0x34) iTimeDiffSec__Fl
float iTimeDiffSec(signed long time) {
    { // 0x00126490 - 0x001264C4
    }
}

// 0x001264D0 - 0x00126508 (0x38) iTimeGet__Fv
signed long iTimeGet() {
    { // 0x001264D0 - 0x00126508
        signed long low1; // r2
        signed long high1; // r5
        signed long low0; // r2
        signed long high0; // r4
    }
}

// 0x00126510 - 0x0012655C (0x4C) TimerHandler__Fi
static signed int TimerHandler(signed int ca) {
    { // 0x00126510 - 0x0012655C
    }
}

// 0x00126560 - 0x00126588 (0x28) iTimeExit__Fv
void iTimeExit() {
    { // 0x00126560 - 0x00126588
    }
}

// 0x00126590 - 0x001265EC (0x5C) iTimeInit__Fv
void iTimeInit() {
    { // 0x00126590 - 0x001265EC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iMorph.cpp
// ==========================================================================
static signed int s_numV; // 0x00357D04
static float *s_nTemp; // 0x00357D00
static float *s_vTemp; // 0x00357CFC
static float *s_alloc; // 0x00357CF8
static class RpMorphTarget *s_tgt; // 0x00357CF4
static class RpGeometry *s_geom; // 0x00357CF0
// 0x001265F0 - 0x00126730 (0x140) FastS16weight4__FPfPPsPsif
void FastS16weight4(float *dest, signed short **v_array, signed short *weight, signed int count, float scale) {
    { // 0x001265F0 - 0x00126730
    }
}

// 0x00126730 - 0x0012682C (0xFC) FastS16weight2__FPfPPsPsif
void FastS16weight2(float *dest, signed short **v_array, signed short *weight, signed int count, float scale) {
    { // 0x00126730 - 0x0012682C
    }
}

// 0x00126830 - 0x0012690C (0xDC) FastS16unpack__FPfPsif
void FastS16unpack(float *dest, signed short *v, signed int count, float scale) {
    { // 0x00126830 - 0x0012690C
    }
}

// 0x00126910 - 0x00126950 (0x40) iMorphRender__FP8RpAtomicP11RwMatrixTagPPsPsUif
void iMorphRender(class RpAtomic *model, class RwMatrixTag *mat, signed short **v_array, signed short *weight, unsigned int normals, float scale) {
    { // 0x00126910 - 0x00126950
    }
}

// 0x00126950 - 0x001269DC (0x8C) iMorphOptimize__FP8RpAtomici
void iMorphOptimize(class RpAtomic *model, signed int normals) {
    { // 0x00126950 - 0x001269DC
        class RpUserDataArray *usr; // r2
        signed int usridx; // r2
        class RpGeometry *geom; // r16
    }
}

// 0x001269E0 - 0x00127230 (0x850) MorphCommon__FP8RpAtomicP11RwMatrixTagPPsPsUifi
static void MorphCommon(class RpAtomic *model, class RwMatrixTag *mat, signed short **v_array, signed short *weight, unsigned int normals, float scale, signed int dorender) {
    { // 0x001269E0 - 0x00127230
        class DirtyMorph *dm; // r3
        class RpUserDataArray *usr; // r29+0xec
        signed int lockMode; // r29+0xd0
        class RwV3d *nold; // r29+0xc0
        class RwV3d *vold; // r29+0xb0
        signed int wsum; // r17
        signed short *va[4]; // r29+0x120
        signed short wa[4]; // r29+0x138
        unsigned int a; // r16
        unsigned int i; // r3
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xMorph.cpp
// ==========================================================================
// 0x00127230 - 0x00127248 (0x18) xMorphSeqDuration__FP13xMorphSeqFile
float xMorphSeqDuration(class xMorphSeqFile *seq) {
    { // 0x00127230 - 0x00127248
    }
}

// 0x00127250 - 0x0012743C (0x1EC) xMorphRender__FP13xMorphSeqFileP11RwMatrixTagf
void xMorphRender(class xMorphSeqFile *seq, class RwMatrixTag *mat, float time) {
    { // 0x00127250 - 0x0012743C
        class xMorphFrame *frame; // r2
        float *times; // r6
        signed short *wptr; // r2
        signed short weight[4]; // r29+0x18
        unsigned int tcount; // r2
        unsigned int tidx; // r4
        float lerp; // 
    }
}

// 0x00127440 - 0x00127668 (0x228) xMorphSeqSetup__FPvPFUiPc_Pv
class xMorphSeqFile *xMorphSeqSetup(void *data, void * (*FindAssetCB)(unsigned int, char *)) {
    { // 0x00127440 - 0x00127668
        unsigned int skipsize; // r6
        void *assetPtr; // r2
        char *namelist; // r20
        unsigned int *assetlist; // r19
        class xMorphFrame *framelist; // r2
        class xMorphSeqFile *header; // r18
        class xMorphTargetFile *mphtgt; // r8
        class RpAtomic *model; // r4
        signed int j; // r7
        signed int i; // r17
    }
}


// ==========================================================================
// E:\SD2\Game\flat\p2skyfs.c
// ==========================================================================
signed int (skyFexist)(char *); // 0x00127F90
void * (skyFopen)(char *, char *); // 0x00128090
signed int (skyFclose)(void *); // 0x00127FF0
unsigned int (skyFread)(void *, unsigned int, unsigned int, void *); // 0x00127E50
unsigned int (skyFwrite)(void *, unsigned int, unsigned int, void *); // 0x00127D70
char * (skyFgets)(char *, signed int, void *); // 0x00127A60
signed int (skyFputs)(char *, void *); // 0x00127990
signed int (skyFeof)(void *); // 0x00127940
signed int (skyFseek)(void *, signed long, signed int); // 0x00127C00
signed int (skyFflush)(void *); // 0x00127930
signed int (skyFtell)(void *); // 0x00127BB0
unsigned int ourGlobals[4096]; // 0x002B5AD0
void * (_rwSkyFSOpen)(void *, signed int, signed int); // 0x001277B0
void * (_rwSkyFSClose)(void *, signed int, signed int); // 0x001276F0
static class RwSkyFSState SkyFSState; // 0x003694D0
// 0x00127670 - 0x001276F0 (0x80)
signed int SkyInstallFileSystem(char *subdir) {
    { // 0x00127670 - 0x001276F0
    }
}

// 0x001276F0 - 0x001277AC (0xBC) _rwSkyFSClose__FPvii
static void *_rwSkyFSClose(void *instance) {
    { // 0x001276F0 - 0x001277AC
        class RwFileFunctions *FS; // r2
    }
}

// 0x001277B0 - 0x00127928 (0x178) _rwSkyFSOpen__FPvii
static void *_rwSkyFSOpen(void *instance, signed int offset) {
    { // 0x001277B0 - 0x00127928
        class RwFileFunctions *FS; // r2
    }
}

// 0x00127930 - 0x00127938 (0x8) skyFflush__FPv
static signed int skyFflush() {
    { // 0x00127930 - 0x00127938
    }
}

// 0x00127940 - 0x0012798C (0x4C) skyFeof__FPv
static signed int skyFeof(void *fptr) {
    { // 0x00127940 - 0x0012798C
        class skyFile *fp; // r16
    }
}

// 0x00127990 - 0x00127A60 (0xD0) skyFputs__FPCcPv
static signed int skyFputs(char *buffer, void *fptr) {
    { // 0x00127990 - 0x00127A60
        signed int j; // r2
        signed int i; // r17
        class skyFile *fp; // r2
    }
}

// 0x00127A60 - 0x00127BB0 (0x150) skyFgets__FPciPv
static char *skyFgets(char *buffer, signed int maxLen, void *fptr) {
    { // 0x00127A60 - 0x00127BB0
        signed int numBytesRead; // r17
        signed int i; // r16
        class skyFile *fp; // r2
    }
}

// 0x00127BB0 - 0x00127BFC (0x4C) skyFtell__FPv
static signed int skyFtell(void *fptr) {
    { // 0x00127BB0 - 0x00127BFC
        class skyFile *fp; // r16
    }
}

// 0x00127C00 - 0x00127D70 (0x170) skyFseek__FPvli
static signed int skyFseek(void *fptr, signed long offset, signed int origin) {
    { // 0x00127C00 - 0x00127D70
        signed int noBuffer; // r18
        signed int bufStart; // r3
        signed int oldFPos; // r17
        class skyFile *fp; // r16
    }
}

// 0x00127D70 - 0x00127E44 (0xD4) skyFwrite__FPCvUiUiPv
static unsigned int skyFwrite(void *addr, unsigned int size, unsigned int count, void *fptr) {
    { // 0x00127D70 - 0x00127E44
        signed int numBytesToWrite; // r16
        class skyFile *fp; // r2
        signed int bytesWritten; // r2
    }
}

// 0x00127E50 - 0x00127F90 (0x140) skyFread__FPvUiUiPv
static unsigned int skyFread(void *addr, unsigned int size, unsigned int count, void *fptr) {
    { // 0x00127E50 - 0x00127F90
        signed int bytesRead2; // r2
        signed int bytesRead; // r2
        unsigned int numBytesToRead; // r2
        class skyFile *fp; // r16
    }
}

// 0x00127F90 - 0x00127FEC (0x5C) skyFexist__FPCc
static signed int skyFexist(char *name) {
    { // 0x00127F90 - 0x00127FEC
        void *res; // r16
    }
}

// 0x00127FF0 - 0x00128090 (0xA0) skyFclose__FPv
static signed int skyFclose(void *fptr) {
    { // 0x00127FF0 - 0x00128090
        class skyFile *fp; // r17
    }
}

// 0x00128090 - 0x001280B4 (0x24) skyFopen__FPCcPCc
static void *skyFopen(char *name, char *access) {
    { // 0x00128090 - 0x001280B4
        void *res; // r2
    }
}

// 0x001280C0 - 0x001282E8 (0x228) trySkyFopen__FPCcPCc
static void *trySkyFopen(char *fname, char *access) {
    { // 0x001280C0 - 0x001282E8
        char *nameptr; // r5
        char name[256]; // r29+0x50
        signed int mode; // r19
        class skyFile *fp; // r16
    }
}

// 0x001282F0 - 0x00128408 (0x118) skyTransMode__FPCc
static signed int skyTransMode(char *access) {
    { // 0x001282F0 - 0x00128408
        char *d; // r2
        char *n; // r20
        char *plus; // r19
        char *a; // r18
        char *w; // r17
        char *r; // r16
        signed int mode; // r3
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iDebug.cpp
// ==========================================================================
unsigned int ourGlobals[4096]; // 0x002B5AD0
char gScreenDumpPath[256]; // 0x003695E0
static signed int sDumpFileNumber; // 0x00357D0C
unsigned int gScreenDump; // 0x00357D08
static class RwRGBA mBackgroundColor; // 0x00000000
static class RwRGBA mForegroundColor; // 0x00000000
static class RwRaster *mCharset; // 0x00000000
// 0x00128410 - 0x00128788 (0x378) iDebugScreenDump__Fv
void iDebugScreenDump() {
    { // 0x00128410 - 0x00128788
        unsigned char *p; // r4
        class BmpMegaHeader mhdr; // r29+0x160
        signed long t1; // r2
        signed long t0; // r20
        unsigned char *imp; // r19
        unsigned char *buf; // r18
        unsigned char *bp; // r2
        char fname[64]; // r29+0x120
        unsigned int y; // r17
        unsigned int x; // r3
        class tag_xFile file; // r29+0x60
        unsigned char *pixels; // r18
        unsigned char *oldCamPixels; // r17
        class RwImage *image; // r16
        signed int height; // r20
        signed int width; // r16
        class RwRaster *camRas; // r19
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xEnt.cpp
// ==========================================================================
void (xEntDefaultBoundUpdate)(class _xEnt *, class _xVec3 *); // 0x0012C260
void (xEntDefaultTranslate)(class _xEnt *, class _xVec3 *); // 0x0012C180
void (xEntUpdate)(class _xEnt *, class _xScene *, float); // 0x0012C470
void (xEntRender)(class _xEnt *); // 0x00129BE0
void (xEntUpdateTimer)(class _xEnt *); // 0x0012D140
class _xVec3 g_O3; // 0x00342E28
class _xEnt * (xEntCollCheckOneEntNoDepen)(class _xEnt *, class _xScene *, void *); // 0x0012AE00
class _xVec3 g_NY3; // 0x00342E58
class _xEnt * (xEntCollCheckOneTrigNoDepen)(class _xEnt *, class _xScene *, void *); // 0x0012ADA0
float ShadowThreshold; // 0x00357600
void (xEntDebugCB)(); // 0x00129B40
void (xModCollCB)(); // 0x001287F0
unsigned int gActiveHeap; // 0x00357CAC
signed int (CmpModCollCtime)(void *, void *); // 0x00128B10
signed int (CmpModCollLtime)(void *, void *); // 0x00128AB0
class _xScene *g_xSceneCur; // 0x00357D6C
class _tagxPad *gDebugPad; // 0x00357C30
class zGlobals globals; // 0x00362FD0
static char scoll_name[6][6]; // 0x0028F840
static signed int mcmode; // 0x003574B8
static class xModCollRec smcrecs[101]; // 0x00369700
static signed long mctime; // 0x00357D40
static signed int num_mcrecs; // 0x00357D38
static class xModCollRec *mcrecs; // 0x00357D34
static float dbg_ltm; // 0x00357D30
static float dbg_tmr; // 0x00357D2C
static signed int dbg_holding_dec; // 0x00357D28
static signed int dbg_holding_inc; // 0x00357D24
static signed int dbg_mode; // 0x00357D20
static signed short dbg_idx; // 0x003574B4
static unsigned short dbg_num; // 0x00357D1C
static class _xEnt **dbg_xents; // 0x00357D18
static signed int all_ents_box_init; // 0x00357D14
static class _xBox all_ents_box; // 0x003696E0
static float sEntityTimePassed; // 0x00357D10
static float nsn_angle; // 0x003574B0
char *g_xEnt_FMT; // 0x00000000
// 0x00128790 - 0x001287E8 (0x58) xEntIsVisible__FP5_xEnt
unsigned int xEntIsVisible(class _xEnt *ent) {
    { // 0x00128790 - 0x001287E8
    }
}

// 0x001287F0 - 0x00128AA4 (0x2B4) xModCollCB__Fv
static void xModCollCB() {
    { // 0x001287F0 - 0x00128AA4
        float scale; // 
        signed int i; // r18
        class xModCollRec *mc; // r17
    }
}

// 0x00128AB0 - 0x00128B10 (0x60) CmpModCollLtime__FPCvPCv
static signed int CmpModCollLtime(void *a, void *b) {
    { // 0x00128AB0 - 0x00128B10
        class xModCollRec *mcb; // r2
        class xModCollRec *mca; // r2
    }
}

// 0x00128B10 - 0x00128B44 (0x34) CmpModCollCtime__FPCvPCv
static signed int CmpModCollCtime(void *a, void *b) {
    { // 0x00128B10 - 0x00128B44
    }
}

// 0x00128B50 - 0x00128BEC (0x9C) xModCollAddSample__FP5_xEntl
void xModCollAddSample(class _xEnt *xent, signed long elapsed) {
    { // 0x00128B50 - 0x00128BEC
        class xModCollRec *mc; // r2
        class xModCollRec *end; // r7
    }
}

// 0x00128BF0 - 0x00128C9C (0xAC) xModCollAdd__FP5_xEnt
static void xModCollAdd(class _xEnt *xent) {
    { // 0x00128BF0 - 0x00128C9C
        class xModCollRec *mc; // r2
        class xModCollRec *end; // r5
    }
}

// 0x00128CA0 - 0x00128D54 (0xB4) xModCollInit__Fv
static void xModCollInit() {
    { // 0x00128CA0 - 0x00128D54
        signed int i; // r4
    }
}

// 0x00128D60 - 0x00128FC0 (0x260) xEntDebugIPad__FP5_xEnt
static void xEntDebugIPad() {
    { // 0x00128D60 - 0x00128FC0
        float now; // r29+0x10
    }
}

// 0x00128FC0 - 0x00129A4C (0xA8C) xEntDebugWrite__FPC5_xEnt
static void xEntDebugWrite(class _xEnt *xent) {
    { // 0x00128FC0 - 0x00129A4C
        class _xVec3 lhdng; // r29+0x130
        signed int i; // r17
        class _xCollis *scoll; // r16
        class _xVec3 ang; // r29+0x120
        class _xMat3x3 mat_norm; // r29+0xe0
        class _xVec3 scale; // r29+0x110
        class _xVec3 *ovel; // r2
        class _xVec3 *vel; // r2
        char str[40]; // r29+0xb0
    }
}

// 0x00129A50 - 0x00129B40 (0xF0) xEntCollTypeStrAll__FPcUc
static char *xEntCollTypeStrAll(char *str, unsigned char type) {
    { // 0x00129A50 - 0x00129B40
    }
}

// 0x00129B40 - 0x00129B98 (0x58) xEntDebugCB__Fv
static void xEntDebugCB() {
    { // 0x00129B40 - 0x00129B98
        class _xEnt *xent; // r16
    }
}

// 0x00129BA0 - 0x00129BAC (0xC) xEntGetAllEntsBox__Fv
class _xBox *xEntGetAllEntsBox() {
    { // 0x00129BA0 - 0x00129BAC
    }
}

// 0x00129BB0 - 0x00129BB8 (0x8) xEntSetNostepNormAngle__Ff
void xEntSetNostepNormAngle(float angle) {
    { // 0x00129BB0 - 0x00129BB8
    }
}

// 0x00129BC0 - 0x00129BDC (0x1C) xEntSetMaterialMul__FP5_xEntfff
void xEntSetMaterialMul(class _xEnt *ent, float r, float g, float b) {
    { // 0x00129BC0 - 0x00129BDC
    }
}

// 0x00129BE0 - 0x0012A2B0 (0x6D0) xEntRender__FP5_xEnt
void xEntRender(class _xEnt *ent) {
    { // 0x00129BE0 - 0x0012A2B0
        float rad; // r29+0x70
        class _xVec3 *campos; // r2
        class _xVec3 *pos; // r2
        signed int reset_material_colors; // r21
        class RwTexture *texptr; // r2
        class RwTexture *texptr; // r2
        unsigned int texid; // r2
        class _zGroup *g; // r2
        class RwTexture *texptr; // r2
        unsigned int texid; // r2
        class _zGroup *g; // r2
        class zSurfaceProps *pp; // r2
        class RxPipeline *oldPipe; // r20
        signed int setPipeline; // r19
        signed int setMaterialTextureRestore; // r18
        signed int we_altered_the_alpha_on_the_material; // r17
        static signed int fadeout_type; // 0x00357D48
        static signed char init; // 0x00357D4C
    }
}

// 0x0012A2B0 - 0x0012A6F4 (0x444) xEntCollideWalls__FP5_xEntP7_xScenef
void xEntCollideWalls(class _xEnt *p, class _xScene *sc, float dt) {
    { // 0x0012A2B0 - 0x0012A6F4
        unsigned char idx; // r19
        unsigned char sidx; // r2
        class _xEnt *cent; // r2
        class _xCollis *coll; // r18
    }
}

// 0x0012A700 - 0x0012A960 (0x260) xEntCollideCeiling__FP5_xEntP7_xScenef
void xEntCollideCeiling(class _xEnt *p, class _xScene *sc, float dt) {
    { // 0x0012A700 - 0x0012A960
        float ceil_dist; // 
        class _xEnt *fent; // r2
        class _xCollis *mf; // r19
        class _xCollis *ml; // r18
        unsigned char idx; // r17
        class _xCollis *coll; // r16
    }
}

// 0x0012A960 - 0x0012AD9C (0x43C) xEntCollideFloor__FP5_xEntP7_xScenef
void xEntCollideFloor(class _xEnt *p, class _xScene *sc, float dt) {
    { // 0x0012A960 - 0x0012AD9C
        float flr_dist; // r29+0xb0
        class _xEnt *fent; // r2
        class _xCollis *mf; // r20
        signed int stepping; // r30
        class _xCollis *ml; // r19
        unsigned char idx; // r18
        class _xCollis *coll; // r17
    }
}

// 0x0012ADA0 - 0x0012AE00 (0x60) xEntCollCheckOneTrigNoDepen__FP5_xEntP7_xScenePv
class _xEnt *xEntCollCheckOneTrigNoDepen(class _xEnt *ent, void *data) {
    { // 0x0012ADA0 - 0x0012AE00
        class _xEnt *p; // r2
    }
}

// 0x0012AE00 - 0x0012B180 (0x380) xEntCollCheckOneEntNoDepen__FP5_xEntP7_xScenePv
class _xEnt *xEntCollCheckOneEntNoDepen(class _xEnt *ent, void *data) {
    { // 0x0012AE00 - 0x0012B180
        float hsqr; // r29+0x70
        float dz; // r29+0x70
        float dy; // 
        float dx; // 
        float rsum; // 
        unsigned char idx; // r2
        signed long t1; // r2
        unsigned char ncolls; // r20
        signed long t0; // r19
        unsigned int modl_coll; // r19
        class _xCollis *coll; // r18
        class _xEnt *p; // r17
    }
}

// 0x0012B180 - 0x0012B304 (0x184) xEntEndCollCheckTrigs__FP5_xEnt
void xEntEndCollCheckTrigs(class _xEnt *ent) {
    { // 0x0012B180 - 0x0012B304
    }
}

// 0x0012B310 - 0x0012B37C (0x6C) xEntBeginCollCheckTrigs__FP5_xEnt
void xEntBeginCollCheckTrigs(class _xEnt *ent) {
    { // 0x0012B310 - 0x0012B37C
    }
}

// 0x0012B380 - 0x0012B3C8 (0x48) xEntCollCheckNPCs__FP5_xEntP7_xScenePFP5_xEntP7_xScenePv_P5_xEnt
void xEntCollCheckNPCs(class _xEnt *p, class _xScene *sc, class _xEnt * (*hitIt)(class _xEnt *, class _xScene *, void *)) {
    { // 0x0012B380 - 0x0012B3C8
    }
}

// 0x0012B3D0 - 0x0012B418 (0x48) xEntCollCheckDyns__FP5_xEntP7_xScenePFP5_xEntP7_xScenePv_P5_xEnt
void xEntCollCheckDyns(class _xEnt *p, class _xScene *sc, class _xEnt * (*hitIt)(class _xEnt *, class _xScene *, void *)) {
    { // 0x0012B3D0 - 0x0012B418
    }
}

// 0x0012B420 - 0x0012B468 (0x48) xEntCollCheckStats__FP5_xEntP7_xScenePFP5_xEntP7_xScenePv_P5_xEnt
void xEntCollCheckStats(class _xEnt *p, class _xScene *sc, class _xEnt * (*hitIt)(class _xEnt *, class _xScene *, void *)) {
    { // 0x0012B420 - 0x0012B468
    }
}

// 0x0012B470 - 0x0012B49C (0x2C) xEntEndCollide__FP5_xEntP7_xScenef
void xEntEndCollide(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x0012B470 - 0x0012B49C
    }
}

// 0x0012B4A0 - 0x0012B634 (0x194) xEntBeginCollide__FP5_xEntP7_xScenef
void xEntBeginCollide(class _xEnt *ent) {
    { // 0x0012B4A0 - 0x0012B634
        class _xCollis *coll; // r2
        unsigned char idx; // r3
    }
}

// 0x0012B640 - 0x0012BC48 (0x608) xEntCollide__FP5_xEntP7_xScenef
void xEntCollide(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x0012B640 - 0x0012BC48
        class _xRay3 flray; // r29+0x50
        class _xCollis *coll; // r16
        float depen_len; // 
        float h_dot_n; // r29+0x80
        class _xCollis *coll; // r3
    }
}

// 0x0012BC50 - 0x0012BE60 (0x210) xEntApplyPhysics__FP5_xEntP7_xScenef
void xEntApplyPhysics(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x0012BC50 - 0x0012BE60
        float scale; // 
        float tdrag; // 
        float tfric; // 
    }
}

// 0x0012BE60 - 0x0012BEE8 (0x88) xEntMove__FP5_xEntP7_xScenef
void xEntMove(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x0012BE60 - 0x0012BEE8
        class _xEntFrame *dframe; // r2
        class _xVec3 dpos; // r29+0x20
    }
}

// 0x0012BEF0 - 0x0012C178 (0x288) xEntMotionToMatrix__FP5_xEntP10_xEntFrame
void xEntMotionToMatrix(class _xEnt *ent, class _xEntFrame *frame) {
    { // 0x0012BEF0 - 0x0012C178
    }
}

// 0x0012C180 - 0x0012C254 (0xD4) xEntDefaultTranslate__FP5_xEntP6_xVec3
void xEntDefaultTranslate(class _xEnt *ent, class _xVec3 *dpos) {
    { // 0x0012C180 - 0x0012C254
    }
}

// 0x0012C260 - 0x0012C2B4 (0x54) xEntDefaultBoundUpdate__FP5_xEntP6_xVec3
void xEntDefaultBoundUpdate(class _xEnt *ent, class _xVec3 *pos) {
    { // 0x0012C260 - 0x0012C2B4
    }
}

// 0x0012C2C0 - 0x0012C384 (0xC4) xEntEndUpdate__FP5_xEntP7_xScenef
void xEntEndUpdate(class _xEnt *ent) {
    { // 0x0012C2C0 - 0x0012C384
        class _xVec3 *upos; // r2
        class _xVec3 *mpos; // r2
    }
}

// 0x0012C390 - 0x0012C46C (0xDC) xEntBeginUpdate__FP5_xEntP7_xScenef
void xEntBeginUpdate(class _xEnt *ent, float dt) {
    { // 0x0012C390 - 0x0012C46C
    }
}

// 0x0012C470 - 0x0012C6C4 (0x254) xEntUpdate__FP5_xEntP7_xScenef
void xEntUpdate(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x0012C470 - 0x0012C6C4
    }
}

// 0x0012C6D0 - 0x0012C774 (0xA4) xEntLoadModel__FP5_xEntP8RpAtomic
class xModelInstance *xEntLoadModel(class _xEnt *ent, class RpAtomic *imodel) {
    { // 0x0012C6D0 - 0x0012C774
        class xModelInstance *model; // r16
    }
}

// 0x0012C780 - 0x0012CB5C (0x3DC) xEntReset__FP5_xEnt
void xEntReset(class _xEnt *ent) {
    { // 0x0012C780 - 0x0012CB5C
        class _xMat4x3 frame; // r29+0x40
    }
}

// 0x0012CB60 - 0x0012CBC8 (0x68) xEntLoad__FP5_xEntP7xSerial
void xEntLoad(class _xEnt *ent, class xSerial *s) {
    { // 0x0012CB60 - 0x0012CBC8
        signed int b; // r29+0x3c
    }
}

// 0x0012CBD0 - 0x0012CC74 (0xA4) xEntSave__FP5_xEntP7xSerial
void xEntSave(class _xEnt *ent, class xSerial *s) {
    { // 0x0012CBD0 - 0x0012CC74
    }
}

// 0x0012CC80 - 0x0012CD54 (0xD4) xEntSetup__FP5_xEnt
void xEntSetup(class _xEnt *ent) {
    { // 0x0012CC80 - 0x0012CD54
        class _xEnt *dent; // r2
        class p2LinkAsset *la; // r5
        signed int i; // r16
    }
}

// 0x0012CD60 - 0x0012CFAC (0x24C) xEntInitForType__FP5_xEnt
void xEntInitForType(class _xEnt *ent) {
    { // 0x0012CD60 - 0x0012CFAC
    }
}

// 0x0012CFB0 - 0x0012D074 (0xC4) xEntInit__FP5_xEntP10p2EntAsset
void xEntInit(class _xEnt *ent, class p2EntAsset *asset) {
    { // 0x0012CFB0 - 0x0012D074
    }
}

// 0x0012D080 - 0x0012D0D0 (0x50) xEntSceneExit__Fv
void xEntSceneExit() {
    { // 0x0012D080 - 0x0012D0D0
    }
}

// 0x0012D0D0 - 0x0012D128 (0x58) xEntSceneInit__Fv
void xEntSceneInit() {
    { // 0x0012D0D0 - 0x0012D128
    }
}

// 0x0012D130 - 0x0012D138 (0x8) xEntSetTimePassed__Ff
void xEntSetTimePassed(float sec) {
    { // 0x0012D130 - 0x0012D138
    }
}

// 0x0012D140 - 0x0012D14C (0xC) xEntUpdateTimer__FP5_xEnt
static void xEntUpdateTimer(class _xEnt *ent) {
    { // 0x0012D140 - 0x0012D14C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iCollide.cpp
// ==========================================================================
class RpCollisionTriangle * (sphereHitsEnvCB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *); // 0x00000000
class RpCollisionTriangle * (sphereHitsEnv3CB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *); // 0x0012E680
class RpCollisionTriangle * (sphereHitsEnv4CB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *); // 0x0012E4F0
class RpCollisionTriangle * (sphereHitsModelCB)(class RpIntersection *, class RpCollisionTriangle *, float, void *); // 0x00000000
class RpCollisionTriangle * (sphereHitsModel3CB)(class RpIntersection *, class RpCollisionTriangle *, float, void *); // 0x0012E4D0
class RpCollisionTriangle * (rayHitsEnvCB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *); // 0x0012E430
class RpCollisionTriangle * (rayHitsEnvBackwardCB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *); // 0x0012E3B0
class RpCollisionTriangle * (rayHitsModelCB)(class RpIntersection *, class RpCollisionTriangle *, float, void *); // 0x0012E330
class RpCollisionTriangle * (rayHitsModelBackwardCB)(class RpIntersection *, class RpCollisionTriangle *, float, void *); // 0x0012E2D0
static unsigned char OTHER; // 0x00357D68
static unsigned char NEXT2; // 0x00357D64
static unsigned char FLOOR; // 0x00357D60
static unsigned char cbmaxcs; // 0x00357D5C
static unsigned char cbnumcs; // 0x00357D58
static class RpIntersection cbisx_local; // 0x0036A3D0
static class _xMat3x3 *cbmat; // 0x00357D54
static class _xRay3 cbray; // 0x0036A3A0
static float cbath; // 0x00357D50
// 0x0012D150 - 0x0012D1FC (0xAC) iBoxForModelLocal__FP5_xBoxPC14xModelInstance
void iBoxForModelLocal(class _xBox *o, class xModelInstance *m) {
    { // 0x0012D150 - 0x0012D1FC
        unsigned short idx; // r18
        class RpMorphTarget *mtgt; // r17
        class RpGeometry *geom; // r16
    }
}

// 0x0012D200 - 0x0012D324 (0x124) iBoxForModel__FP5_xBoxPC14xModelInstance
void iBoxForModel(class _xBox *o, class xModelInstance *m) {
    { // 0x0012D200 - 0x0012D324
    }
}

// 0x0012D330 - 0x0012D628 (0x2F8) iRayHitsModel__FPC6_xRay3PC14xModelInstanceP8_xCollis
unsigned int iRayHitsModel(class _xRay3 *r, class xModelInstance *m, class _xCollis *coll) {
    { // 0x0012D330 - 0x0012D628
        class RwV3d temp; // r29+0x80
        class _xMat4x3 *mat; // r20
        float len; // 
        class RpIntersection isx; // r29+0x60
    }
}

// 0x0012D630 - 0x0012D930 (0x300) iRayHitsCamEnv__FPC6_xRay3PC5_xEnvP8_xCollis
unsigned int iRayHitsCamEnv(class _xRay3 *r, class _xEnv *env, class _xCollis *coll) {
    { // 0x0012D630 - 0x0012D930
        class RwV3d temp; // r29+0x70
        float len; // 
        class RpIntersection isx; // r29+0x50
    }
}

// 0x0012D930 - 0x0012DB98 (0x268) iRayHitsEnv__FPC6_xRay3PC5_xEnvP8_xCollis
unsigned int iRayHitsEnv(class _xRay3 *r, class _xEnv *env, class _xCollis *coll) {
    { // 0x0012D930 - 0x0012DB98
        float len; // 
        class RpIntersection isx; // r29+0x50
    }
}

// 0x0012DBA0 - 0x0012DEB0 (0x310) iSphereHitsModel3__FPC8_xSpherePC14xModelInstanceP8_xCollisUcf
signed int iSphereHitsModel3(class _xSphere *b, class xModelInstance *m, class _xCollis *colls, unsigned char ncolls, float sth) {
    { // 0x0012DBA0 - 0x0012DEB0
        class _xMat3x3 mnormal; // r29+0xa0
        class _xMat4x3 *mat; // r16
        unsigned char idx; // r3
        class RpIntersection isx; // r29+0x80
    }
}

// 0x0012DEB0 - 0x0012E0EC (0x23C) iSphereHitsEnv4__FPC8_xSpherePC5_xEnvPC8_xMat3x3P8_xCollis
signed int iSphereHitsEnv4(class _xSphere *b, class _xEnv *env, class _xMat3x3 *mat, class _xCollis *colls) {
    { // 0x0012DEB0 - 0x0012E0EC
        float s; // 
        signed int numcs; // r2
        class _xCollis *cend; // r16
        class _xCollis *c; // r4
        class RpIntersection isx; // r29+0x50
    }
}

// 0x0012E0F0 - 0x0012E2C8 (0x1D8) iSphereHitsEnv3__FPC8_xSpherePC5_xEnvP8_xCollisUcf
signed int iSphereHitsEnv3(class _xSphere *b, class _xEnv *env, class _xCollis *colls, unsigned char ncolls, float sth) {
    { // 0x0012E0F0 - 0x0012E2C8
        unsigned char idx; // r3
        class RpIntersection isx; // r29+0x60
    }
}

// 0x0012E2D0 - 0x0012E328 (0x58) rayHitsModelBackwardCB__FP14RpIntersectionP19RpCollisionTrianglefPv
static class RpCollisionTriangle *rayHitsModelBackwardCB(class RpCollisionTriangle *tri, float dist, void *data) {
    { // 0x0012E2D0 - 0x0012E328
    }
}

// 0x0012E330 - 0x0012E3A4 (0x74) rayHitsModelCB__FP14RpIntersectionP19RpCollisionTrianglefPv
static class RpCollisionTriangle *rayHitsModelCB(class RpCollisionTriangle *tri, float dist, void *data) {
    { // 0x0012E330 - 0x0012E3A4
    }
}

// 0x0012E3B0 - 0x0012E42C (0x7C) rayHitsEnvBackwardCB__FP14RpIntersectionP13RpWorldSectorP19RpCollisionTrianglefPv
static class RpCollisionTriangle *rayHitsEnvBackwardCB(class RpWorldSector *sector, class RpCollisionTriangle *tri, float dist, void *data) {
    { // 0x0012E3B0 - 0x0012E42C
        class _xCollis *coll; // r2
    }
}

// 0x0012E430 - 0x0012E4C4 (0x94) rayHitsEnvCB__FP14RpIntersectionP13RpWorldSectorP19RpCollisionTrianglefPv
static class RpCollisionTriangle *rayHitsEnvCB(class RpWorldSector *sector, class RpCollisionTriangle *tri, float dist, void *data) {
    { // 0x0012E430 - 0x0012E4C4
        class _xCollis *coll; // r2
    }
}

// 0x0012E4D0 - 0x0012E4E8 (0x18) sphereHitsModel3CB__FP14RpIntersectionP19RpCollisionTrianglefPv
static class RpCollisionTriangle *sphereHitsModel3CB(class RpCollisionTriangle *tri, float dist, void *data) {
    { // 0x0012E4D0 - 0x0012E4E8
    }
}

// 0x0012E4F0 - 0x0012E680 (0x190) sphereHitsEnv4CB__FP14RpIntersectionP13RpWorldSectorP19RpCollisionTrianglefPv
static class RpCollisionTriangle *sphereHitsEnv4CB(class RpIntersection *isx, class RpWorldSector *sector, class RpCollisionTriangle *tri, float dist, void *data) {
    { // 0x0012E4F0 - 0x0012E680
        float ddist; // 
        class _xCollis *c; // r4
        unsigned char idx; // r2
        class _xVec3 tohit; // r29+0x60
        float radius; // 
        class _xCollis *colls; // r16
    }
}

// 0x0012E680 - 0x0012ED58 (0x6D8) sphereHitsEnv3CB__FP14RpIntersectionP13RpWorldSectorP19RpCollisionTrianglefPv
static class RpCollisionTriangle *sphereHitsEnv3CB(class RpIntersection *isx, class RpWorldSector *sector, class RpCollisionTriangle *tri, float dist, void *data) {
    { // 0x0012E680 - 0x0012ED58
        float scale; // 
        unsigned char idx; // r2
        class _xVec3 tohit; // r29+0xa0
        float radius; // 
        class _xCollis *colls; // r18
    }
}

// 0x0012ED60 - 0x0012EF6C (0x20C) properSphereIsectTri__FPC6_xVec3fP6_xVec3PfP19RpCollisionTriangle
static void properSphereIsectTri(class _xVec3 *center, float radius, class _xVec3 *tohit, float *dist_ptr, class RpCollisionTriangle *tri) {
    { // 0x0012ED60 - 0x0012EF6C
        class _xVec3 vertClosestPoint; // r29+0x90
        unsigned int i; // r16
        float radius2; // 
        float dist2; // 
        float dist; // 
        float dist2plane; // 
        class _xVec3 projPoint; // r29+0x80
    }
}

// 0x0012EF70 - 0x0012F03C (0xCC) FindNearestPointOnLine__FP5RwV3dP5RwV3dP5RwV3dP5RwV3d
static void FindNearestPointOnLine(class RwV3d *result, class RwV3d *point, class RwV3d *start, class RwV3d *end) {
    { // 0x0012EF70 - 0x0012F03C
        float lineLength2; // r29+0x0
        float mu; // 
    }
}

// 0x0012F040 - 0x0012F3C0 (0x380) PointWithinTriangle__FP5RwV3dPP5RwV3dP5RwV3d
static signed int PointWithinTriangle(class RwV3d *pt, class RwV3d **tri, class RwV3d *normal) {
    { // 0x0012F040 - 0x0012F3C0
        signed int j; // r2
        signed int i; // r4
        signed int inside; // r16
        float absZ; // r29+0x60
        float absY; // 
        float absX; // 
        enum Dimension dimension; // r4
    }
}

// 0x0012F3C0 - 0x0012F3C8 (0x8) iCollideInit__FP7_xScene
void iCollideInit() {
    { // 0x0012F3C0 - 0x0012F3C8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xCollide.cpp
// ==========================================================================
class _xVec3 g_O3; // 0x00342E28
char *g_xCollis_FMT; // 0x00000000
// 0x0012F3D0 - 0x0012F540 (0x170) xSphereHitsOBB__FPC8_xSpherePC5_xBoxPC8_xMat4x3P8_xCollis
unsigned int xSphereHitsOBB(class _xSphere *a, class _xBox *b, class _xMat4x3 *m, class _xCollis *coll) {
    { // 0x0012F3D0 - 0x0012F540
        class _xMat3x3 mnormal; // r29+0x70
        class _xSphere mxa; // r29+0x60
    }
}

// 0x0012F540 - 0x0012F6DC (0x19C) xSphereHitsBox__FPC8_xSpherePC5_xBoxP8_xCollis
unsigned int xSphereHitsBox(class _xSphere *a, class _xBox *b, class _xCollis *coll) {
    { // 0x0012F540 - 0x0012F6DC
        float scale; // 
        class _xIsect isx; // r29+0x20
    }
}

// 0x0012F6E0 - 0x0012F8E4 (0x204) xSphereHitsSphere__FPC8_xSpherePC8_xSphereP8_xCollis
unsigned int xSphereHitsSphere(class _xSphere *a, class _xSphere *b, class _xCollis *coll) {
    { // 0x0012F6E0 - 0x0012F8E4
        float scale; // 
        float scale; // 
        class _xIsect isx; // r29+0x30
    }
}

// 0x0012F8F0 - 0x0012F8F8 (0x8) xCollideInit__FP7_xScene
void xCollideInit(class _xScene *sc) {
    { // 0x0012F8F0 - 0x0012F8F8
    }
}

// 0x0012F900 - 0x0012FA10 (0x110) xCollideGetCollsIdx__FPC6_xVec3PC8_xMat3x3
enum _xCollsIdx xCollideGetCollsIdx(class _xVec3 *tohit, class _xMat3x3 *mat) {
    { // 0x0012F900 - 0x0012FA10
        float local_z; // r29+0x0
        float local_x; // 
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xScene.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
class _xScene *g_xSceneCur; // 0x00357D6C
char *g_xScene_FMT; // 0x00000000
// 0x0012FA10 - 0x0012FB58 (0x148) xRayHitsCameraScene__FP7_xSceneP6_xRay3PFP5_xEnt_v
void xRayHitsCameraScene(class _xScene *sc, class _xRay3 *r, void (*camera_func)(class _xEnt *)) {
    { // 0x0012FA10 - 0x0012FB58
        class _xEnt *ent; // r19
        class _xEnt *ent; // r20
        unsigned short i; // r19
        class xQCData qcr; // r29+0x70
    }
}

// 0x0012FB60 - 0x0012FDDC (0x27C) xRayHitsScene__FP7_xSceneP6_xRay3P8_xCollis
void xRayHitsScene(class _xScene *sc, class _xRay3 *r, class _xCollis *coll) {
    { // 0x0012FB60 - 0x0012FDDC
        class _xEnt *ent; // r19
        class _xEnt *ent; // r19
        class _xEnt *ent; // r20
        unsigned short i; // r19
        class _xCollis c; // r29+0x70
        class xQCData q; // r29+0xc0
    }
}

// 0x0012FDE0 - 0x0012FE78 (0x98) xSceneForAllNPCs__FP7_xScenePFP5_xEntP7_xScenePv_P5_xEntPv
void xSceneForAllNPCs(class _xScene *sc, class _xEnt * (*func)(class _xEnt *, class _xScene *, void *), void *data) {
    { // 0x0012FDE0 - 0x0012FE78
        unsigned short i; // r16
    }
}

// 0x0012FE80 - 0x0012FF18 (0x98) xSceneForAllDynamics__FP7_xScenePFP5_xEntP7_xScenePv_P5_xEntPv
void xSceneForAllDynamics(class _xScene *sc, class _xEnt * (*func)(class _xEnt *, class _xScene *, void *), void *data) {
    { // 0x0012FE80 - 0x0012FF18
        unsigned short i; // r16
    }
}

// 0x0012FF20 - 0x0012FFB8 (0x98) xSceneForAllStatics__FP7_xScenePFP5_xEntP7_xScenePv_P5_xEntPv
void xSceneForAllStatics(class _xScene *sc, class _xEnt * (*func)(class _xEnt *, class _xScene *, void *), void *data) {
    { // 0x0012FF20 - 0x0012FFB8
        unsigned short i; // r16
    }
}

// 0x0012FFC0 - 0x00130058 (0x98) xSceneForAllTriggers__FP7_xScenePFP5_xEntP7_xScenePv_P5_xEntPv
void xSceneForAllTriggers(class _xScene *sc, class _xEnt * (*func)(class _xEnt *, class _xScene *, void *), void *data) {
    { // 0x0012FFC0 - 0x00130058
        unsigned short i; // r16
    }
}

// 0x00130060 - 0x00130088 (0x28) xSceneID2Name__FP7_xSceneUi
char *xSceneID2Name(class _xScene *sc, unsigned int id) {
    { // 0x00130060 - 0x00130088
    }
}

// 0x00130090 - 0x001300B8 (0x28) xSceneResolvID__FP7_xSceneUi
class xBase *xSceneResolvID(class _xScene *sc, unsigned int id) {
    { // 0x00130090 - 0x001300B8
    }
}

// 0x001300C0 - 0x00130184 (0xC4) xSceneAddEnt__FP7_xSceneP5_xEnt
void xSceneAddEnt(class _xScene *sc, class _xEnt *ent) {
    { // 0x001300C0 - 0x00130184
    }
}

// 0x00130190 - 0x00130198 (0x8) xSceneSetup__FP7_xScene
void xSceneSetup(class _xScene *sc) {
    { // 0x00130190 - 0x00130198
    }
}

// 0x001301A0 - 0x001301A8 (0x8) xSceneLoad__FP7_xSceneP7xSerial
void xSceneLoad() {
    { // 0x001301A0 - 0x001301A8
    }
}

// 0x001301B0 - 0x001301B8 (0x8) xSceneSave__FP7_xSceneP7xSerial
void xSceneSave() {
    { // 0x001301B0 - 0x001301B8
    }
}

// 0x001301C0 - 0x00130350 (0x190) xSceneInit__FP7_xSceneUsUsUsUs
void xSceneInit(class _xScene *sc, unsigned short num_trigs, unsigned short num_stats, unsigned short num_dyns, unsigned short num_npcs) {
    { // 0x001301C0 - 0x00130350
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zGame.cpp
// ==========================================================================
unsigned int gScreenDump; // 0x00357D08
unsigned int gFrameCount; // 0x00357C78
unsigned int gVsyncCount; // 0x00357CA8
class _tagTRCPadInfo gTrcPad[4]; // 0x003B1A10
enum eGameMode gGameMode; // 0x00357E20
signed int gGameState; // 0x003574F4
class _zEnv *gCurEnv; // 0x00357DF8
class zGlobals globals; // 0x00362FD0
unsigned int sCheatPressed[14]; // 0x0028FA80
unsigned int sCheatAllPowerups[14]; // 0x0028FA40
unsigned int sCheatWarpGate[14]; // 0x0028FA00
unsigned int sCheatUnlockAllMovies[14]; // 0x0028F9C0
unsigned int sCheatUnlockCredits[14]; // 0x0028F980
static float sCheatTimer; // 0x00357DD0
static unsigned int loadCDMeter; // 0x00000000
static unsigned int loadMeter; // 0x00357DCC
static class RpLight *DirectionalLight; // 0x00357DC8
static class RpWorld *World; // 0x00357DC4
static class RwCamera *sGameScreenTransCam; // 0x00357DC0
unsigned int gLastVsyncFlip; // 0x00357DA8
float snd_radius; // 0x003574C8
signed long t1; // 0x00357DA0
signed long t0; // 0x00357D98
class iColor_tag clear; // 0x003574C4
class iColor_tag black; // 0x003574C0
static class iColor_tag sFadeColor; // 0x00000000
unsigned int startPressed; // 0x003574BC
static unsigned short Idx[6]; // 0x0028F868
static class RwSky2DVertex Vert[4]; // 0x0036A3F0
static signed long sTimeCurrent; // 0x00357D90
static signed long sTimeLast; // 0x00357D88
float sTimeElapsed; // 0x00357D84
static float sGameOverTimer; // 0x00357D80
static signed int sPortalling; // 0x00357D7C
static float sPlayerStartAngle; // 0x00357D78
static unsigned int sPlayerMarkerStartCamID; // 0x00357D74
static unsigned int sPlayerMarkerStartID; // 0x00357D70
// 0x00130350 - 0x001305F4 (0x2A4) zGameCheats__Ff
static void zGameCheats(float dt) {
    { // 0x00130350 - 0x001305F4
    }
}

// 0x00130600 - 0x0013077C (0x17C) TestCheat__FPUi
static signed int TestCheat(unsigned int *cheat_info) {
    { // 0x00130600 - 0x0013077C
        signed int i; // r3
    }
}

// 0x00130780 - 0x001308B0 (0x130) setupPlayer__Fv
void setupPlayer() {
    { // 0x00130780 - 0x001308B0
        class p2MarkerAsset *m; // r2
        unsigned int size; // r29+0x3c
        signed int playerCount; // r2
        class p2EntAsset *asset; // r2
    }
}

// 0x001308B0 - 0x00130944 (0x94) zGameScreenTransitionEnd__Fv
void zGameScreenTransitionEnd() {
    { // 0x001308B0 - 0x00130944
        class RwFrame *frame; // r2
    }
}

// 0x00130950 - 0x00130D7C (0x42C) zGameScreenTransition_UIRender__Fffffi
void zGameScreenTransition_UIRender(float x1, float y1, float x2, float y2, signed int tex) {
    { // 0x00130950 - 0x00130D7C
        float cooz; // 
        float cz; // 
        float z; // r29+0x60
        class RwCamera *camera; // r19
        unsigned char b; // r18
        unsigned char g; // r17
        unsigned char r; // r16
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
    }
}

// 0x00130D80 - 0x001311A8 (0x428) zGameScreenTransitionUpdate__FfPcPUc
void zGameScreenTransitionUpdate(float percentComplete, char *msg, unsigned char *rgba) {
    { // 0x00130D80 - 0x001311A8
        char str[256]; // r29+0x150
        char meter[256]; // r29+0x50
        class RwRGBA back_col; // r29+0x25c
    }
}

// 0x001311B0 - 0x001311F4 (0x44) zGameScreenTransitionUpdate__FfPc
void zGameScreenTransitionUpdate(float percentComplete, char *msg) {
    { // 0x001311B0 - 0x001311F4
    }
}

// 0x00131200 - 0x001312DC (0xDC) zGameScreenTransitionBegin__Fv
void zGameScreenTransitionBegin() {
    { // 0x00131200 - 0x001312DC
        class RwBBox box; // r29+0x40
        class RwRGBAReal col; // r29+0x30
        class RwFrame *frame; // r2
    }
}

// 0x001312E0 - 0x001317C0 (0x4E0) zGameUpdateMode__Fv
static void zGameUpdateMode() {
    { // 0x001312E0 - 0x001317C0
        signed int qwerty; // r4
        class _tagxCam *cam; // r2
        class _tagxCam *cam; // r17
        class p2MarkerAsset *m; // r2
        unsigned int size; // r29+0x4c
        signed int vbl; // r17
        unsigned int nextSceneID; // r16
        char *id; // r2
        class zPortalAsset *passet; // r9
    }
}

// 0x001317C0 - 0x001318A4 (0xE4) zGamePauseIfPossible__Fv
void zGamePauseIfPossible() {
    { // 0x001317C0 - 0x001318A4
    }
}

// 0x001318B0 - 0x0013191C (0x6C) zGameIsPaused__Fv
signed int zGameIsPaused() {
    { // 0x001318B0 - 0x0013191C
    }
}

// 0x00131920 - 0x00131EF8 (0x5D8) zGameLoop__Fv
void zGameLoop() {
    { // 0x00131920 - 0x00131EF8
        unsigned int vcount; // r3
        signed long tprof1; // r2
        signed long tprof0; // r29+0xa8
        signed int ostrich_delay; // r30
        static float paccum; // 0x00357DAC
        static float pcount; // 0x00357DB0
    }
}

// 0x00131F00 - 0x00131FBC (0xBC) zGameSetup__Fv
void zGameSetup() {
    { // 0x00131F00 - 0x00131FBC
        class RpWorld *world; // r16
    }
}

// 0x00131FC0 - 0x00132024 (0x64) zGameExit__Fv
void zGameExit() {
    { // 0x00131FC0 - 0x00132024
    }
}

// 0x00132030 - 0x001320D0 (0xA0) zGameInit__FUi
void zGameInit(unsigned int theSceneID) {
    { // 0x00132030 - 0x001320D0
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zMenu.cpp
// ==========================================================================
class _zEnv *gCurEnv; // 0x00357DF8
class zGlobals globals; // 0x00362FD0
unsigned int gScreenDump; // 0x00357D08
unsigned int gFrameCount; // 0x00357C78
enum eGameMode gGameMode; // 0x00357E20
static signed long t1; // 0x00000000
static float holdTmr; // 0x003574D8
static signed int sInMenu; // 0x00357DE4
static signed int fullCard; // 0x00000000
static signed int var; // 0x00000000
static signed int card; // 0x00357DE0
static signed int promptSel; // 0x00000000
static float sAttractMode_timer; // 0x00357DDC
static float s_dbloop_fps; // 0x00000000
static float s_dbloop_elapsec; // 0x00000000
static unsigned int s_dbloop_elaptick; // 0x00000000
static unsigned int sPlayerMarkerStartCamID; // 0x00000000
static unsigned int sPlayerMarkerStartID; // 0x00000000
static float time_current; // 0x00357DD8
static float time_last; // 0x00357DD4
static float time_elapsed; // 0x003574D4
static signed int logoTmr; // 0x00000000
static signed int sFirstBoot; // 0x003574D0
// 0x001320D0 - 0x001320D8 (0x8) zMenuIsFirstBoot__Fv
signed int zMenuIsFirstBoot() {
    { // 0x001320D0 - 0x001320D8
    }
}

// 0x001320E0 - 0x00132128 (0x48) zMenuFMVPlay__FPcUif
void zMenuFMVPlay(char *filename, unsigned int buttons, float time) {
    { // 0x001320E0 - 0x00132128
    }
}

// 0x00132130 - 0x0013213C (0xC) zMenuGetBadCard__Fv
unsigned int zMenuGetBadCard() {
    { // 0x00132130 - 0x0013213C
    }
}

// 0x00132140 - 0x001322C8 (0x188) zMenuCardCheckStartup__FPiPi
signed int zMenuCardCheckStartup(signed int *bytesNeeded, signed int *availOnDisk) {
    { // 0x00132140 - 0x001322C8
        signed int tgtslot; // r2
        signed int rc; // r2
        signed int tgtmax; // r29+0x5c
        signed int tgtcnt; // r2
        class st_XSAVEGAME_DATA *ldinst; // r16
    }
}

// 0x001322D0 - 0x00132904 (0x634) zMenuLoop__Fv
unsigned int zMenuLoop() {
    { // 0x001322D0 - 0x00132904
        class _xEnt *menu; // r2
        class xBase *title; // r29+0xbc
        signed int ss; // r2
        signed int draw_black; // r21
        signed int ostrich_delay; // r22
        class iColor_tag clear; // r29+0xfc
        class iColor_tag black; // r29+0xf8
    }
}

// 0x00132910 - 0x00132998 (0x88) zMenuSetup__Fv
void zMenuSetup() {
    { // 0x00132910 - 0x00132998
    }
}

// 0x001329A0 - 0x001329EC (0x4C) zMenuExit__Fv
void zMenuExit() {
    { // 0x001329A0 - 0x001329EC
    }
}

// 0x001329F0 - 0x00132A78 (0x88) zMenuInit__FUi
void zMenuInit(unsigned int theSceneID) {
    { // 0x001329F0 - 0x00132A78
    }
}

// 0x00132A80 - 0x00132A88 (0x8) zMenuRunning__Fv
signed int zMenuRunning() {
    { // 0x00132A80 - 0x00132A88
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zScene.cpp
// ==========================================================================
class xBase * (zSceneFindObject)(unsigned int); // 0x001337D0
char * (zSceneGetName)(unsigned int); // 0x001336F0
char * (zSceneGetName)(class xBase *); // 0x001336B0
class _xScene *g_xSceneCur; // 0x00357D6C
signed int gOccludeCount; // 0x003581C4
signed int (zSceneSetup_serialTraverseCB)(unsigned int, class xSerial *); // 0x00134760
float gFontWiggleTime; // 0x003580EC
class _tagFeetSkidMark sPlayerSkidMark[5]; // 0x0036E3F0
unsigned int gActiveHeap; // 0x00357CAC
signed int (zSceneCalcProgress_serialTraverseCB)(unsigned int, class xSerial *); // 0x00132B70
class zGlobals globals; // 0x00362FD0
static signed int nidbps; // 0x003574F0
static signed int scobj_size; // 0x003574EC
static class IDBasePair *scobj_idbps; // 0x00357E18
float gSceneUpdateTime; // 0x00357E0C
unsigned int gTransitionSceneID; // 0x00357DFC
class _zEnv *gCurEnv; // 0x00357DF8
class _tagClimate gClimate; // 0x0036A4F0
static class zSceneObjectInstanceDesc sInitTable[34]; // 0x0028FFD0
static class zSceneArea sAreaTable[30]; // 0x0028FB90
static class zSceneLevel sLevelTable[13]; // 0x0028FAC0
static signed int sMemDepthSceneStart; // 0x003574E0
static signed int availOnDisk; // 0x00357DF4
static signed int bytesNeeded; // 0x00357DF0
static signed int roomCnt; // 0x00357DEC
// 0x00132A90 - 0x00132B38 (0xA8) zSceneStats_Clear__Fv
void zSceneStats_Clear() {
    { // 0x00132A90 - 0x00132B38
        unsigned int i; // r4
    }
}

// 0x00132B40 - 0x00132B4C (0xC) zSceneCardCheckStartup_set__Fii
void zSceneCardCheckStartup_set(signed int needed, signed int available) {
    { // 0x00132B40 - 0x00132B4C
    }
}

// 0x00132B50 - 0x00132B64 (0x14) zScene_UpdateFlyToInterface__Ff
void zScene_UpdateFlyToInterface(float dt) {
    { // 0x00132B50 - 0x00132B64
        class zScene *s; // r2
    }
}

// 0x00132B70 - 0x00132BBC (0x4C) zSceneCalcProgress_serialTraverseCB__FUiP7xSerial
signed int zSceneCalcProgress_serialTraverseCB(unsigned int clientID, class xSerial *xser) {
    { // 0x00132B70 - 0x00132BBC
        signed int val; // r29+0x1c
    }
}

// 0x00132BC0 - 0x00132E40 (0x280) zSceneCalcProgress__Fv
unsigned int zSceneCalcProgress() {
    { // 0x00132BC0 - 0x00132E40
        unsigned int totals; // r2
        signed int i; // r5
        signed int i; // r4
        unsigned int killedHolly; // r19
        float mtokPercent; // 
        unsigned int mtokCnt; // r18
        float snackPercent; // 
        unsigned int snackCnt; // r17
        float roomPercent; // 
        unsigned int pupCnt; // r16
        float pupPercent; // 
    }
}

// 0x00132E40 - 0x00132E74 (0x34) zRayHitsPlayer__FP6_xRay3
signed int zRayHitsPlayer(class _xRay3 *r) {
    { // 0x00132E40 - 0x00132E74
        class _xCollis c; // r29+0x10
    }
}

// 0x00132E80 - 0x00132F78 (0xF8) zSceneGameStats_Init__Fv
void zSceneGameStats_Init() {
    { // 0x00132E80 - 0x00132F78
        signed int i; // r4
    }
}

// 0x00132F80 - 0x00133048 (0xC8) zSceneGameStatsTally_clue__Fv
void zSceneGameStatsTally_clue() {
    { // 0x00132F80 - 0x00133048
    }
}

// 0x00133050 - 0x00133074 (0x24) zSceneGameStatsTally_death__Fv
void zSceneGameStatsTally_death() {
    { // 0x00133050 - 0x00133074
    }
}

// 0x00133080 - 0x001330A4 (0x24) zSceneGameStatsTally_scare__Fv
void zSceneGameStatsTally_scare() {
    { // 0x00133080 - 0x001330A4
    }
}

// 0x001330B0 - 0x00133140 (0x90) zSceneGameStatsTally_token__Fv
void zSceneGameStatsTally_token() {
    { // 0x001330B0 - 0x00133140
    }
}

// 0x00133140 - 0x001331D0 (0x90) zSceneGameStatsTally_snack__FUi
void zSceneGameStatsTally_snack(unsigned int amount) {
    { // 0x00133140 - 0x001331D0
    }
}

// 0x001331D0 - 0x00133268 (0x98) zSceneGetAreaTokensCollected__FUi
unsigned int zSceneGetAreaTokensCollected(unsigned int sceneID) {
    { // 0x001331D0 - 0x00133268
    }
}

// 0x00133270 - 0x00133310 (0xA0) zSceneGetAreaSnacksCollected__FUi
unsigned int zSceneGetAreaSnacksCollected(unsigned int sceneID) {
    { // 0x00133270 - 0x00133310
    }
}

// 0x00133310 - 0x00133374 (0x64) zSceneGetAreaTotalTokens__FUi
unsigned int zSceneGetAreaTotalTokens(unsigned int sceneID) {
    { // 0x00133310 - 0x00133374
    }
}

// 0x00133380 - 0x001333A0 (0x20) zSceneGetAreaTotalSnacks__F14_tagSceneAreas
unsigned int zSceneGetAreaTotalSnacks(enum _tagSceneAreas scene) {
    { // 0x00133380 - 0x001333A0
    }
}

// 0x001333A0 - 0x00133404 (0x64) zSceneGetAreaTotalSnacks__FUi
unsigned int zSceneGetAreaTotalSnacks(unsigned int sceneID) {
    { // 0x001333A0 - 0x00133404
    }
}

// 0x00133410 - 0x00133474 (0x64) zSceneGetAreaConstTotalSnacks__FUi
unsigned int zSceneGetAreaConstTotalSnacks(unsigned int sceneID) {
    { // 0x00133410 - 0x00133474
    }
}

// 0x00133480 - 0x0013351C (0x9C) zSceneGetAreaname__FUi
char *zSceneGetAreaname(unsigned int sceneID) {
    { // 0x00133480 - 0x0013351C
    }
}

// 0x00133520 - 0x00133574 (0x54) zSceneGetRoominArea__FUi
unsigned int zSceneGetRoominArea(unsigned int sceneID) {
    { // 0x00133520 - 0x00133574
        unsigned int j; // r2
        unsigned int i; // r5
    }
}

// 0x00133580 - 0x001335D4 (0x54) zSceneGetArea__FUi
unsigned int zSceneGetArea(unsigned int sceneID) {
    { // 0x00133580 - 0x001335D4
        unsigned int j; // r5
        unsigned int i; // r2
    }
}

// 0x001335E0 - 0x001335E8 (0x8) zSceneMemLvlChkCB__Fv
void zSceneMemLvlChkCB() {
    { // 0x001335E0 - 0x001335E8
    }
}

// 0x001335F0 - 0x001336AC (0xBC) zSceneForAllBase__FPFP5xBaseP6zScenePv_P5xBaseiPv
void zSceneForAllBase(class xBase * (*func)(class xBase *, class zScene *, void *), signed int baseType, void *data) {
    { // 0x001335F0 - 0x001336AC
        unsigned short i; // r17
        class zScene *s; // r16
    }
}

// 0x001336B0 - 0x001336E8 (0x38) zSceneGetName__FP5xBase
char *zSceneGetName(class xBase *b) {
    { // 0x001336B0 - 0x001336E8
        char *n; // r2
    }
}

// 0x001336F0 - 0x00133748 (0x58) zSceneGetName__FUi
char *zSceneGetName(unsigned int gameID) {
    { // 0x001336F0 - 0x00133748
    }
}

// 0x00133750 - 0x001337CC (0x7C) zSceneGetObject__Fii
class xBase *zSceneGetObject(signed int type, signed int idx) {
    { // 0x00133750 - 0x001337CC
        signed int i; // r6
        class zScene *s; // r3
    }
}

// 0x001337D0 - 0x00133840 (0x70) zSceneFindObject__FUi
class xBase *zSceneFindObject(unsigned int gameID) {
    { // 0x001337D0 - 0x00133840
    }
}

// 0x00133840 - 0x001338B0 (0x70) zSceneObjHashtableGet__FUi
static class xBase *zSceneObjHashtableGet(unsigned int id) {
    { // 0x00133840 - 0x001338B0
        class IDBasePair *idbp; // r2
        signed int chkd; // r5
        signed int k; // r3
    }
}

// 0x001338B0 - 0x0013391C (0x6C) zSceneObjHashtableAdd__FUiP5xBase
static void zSceneObjHashtableAdd(unsigned int id, class xBase *base) {
    { // 0x001338B0 - 0x0013391C
        class IDBasePair *idbp; // r8
        signed int chkd; // r7
        signed int k; // r6
    }
}

// 0x00133920 - 0x00134184 (0x864) zSceneRender__Fv
void zSceneRender() {
    { // 0x00133920 - 0x00134184
        char strn[256]; // r29+0x3a0
        char str[256]; // r29+0x2a0
        signed int available; // r18
        class xFontBox *sTrcFullCardFontBox2; // r17
        class xFontBox *sTrcFullCardFontBox; // r16
        enum eGameMode mode; // r2
        class _xEnt *ent; // r18
        signed int i; // r2
        class zParSys *psys; // r18
        class _xEnt *swap_e; // r5
        signed int index2; // r4
        signed int i; // r17
        class _xVec3 centre; // r29+0x4a8
        class _xEnt *ent; // r19
        unsigned int tide_hash; // r22
        class _xEnt *tide_ent; // r23
        class _xEnt **entlast; // r21
        class _xEnt **entptr; // r18
        signed int index; // r3
        unsigned int shadowHackCase; // r30
        class _xVec3 *camera_pos; // r17
        signed int sorted_entities; // r29+0x4b8
        float distance[64]; // r29+0x1a0
        class _xEnt *entities[64]; // r29+0xa0
        class _xScene *sc; // r16
        class zScene *s; // r2
    }
}

// 0x00134190 - 0x001342C4 (0x134) zShadowReceivers__Fv
static void zShadowReceivers() {
    { // 0x00134190 - 0x001342C4
        class _xScene *sc; // r18
        class _xEnt *ep; // r17
        signed int i; // r16
    }
}

// 0x001342D0 - 0x0013475C (0x48C) zSceneUpdate__Ff
void zSceneUpdate(float elapsedSec) {
    { // 0x001342D0 - 0x0013475C
        signed int timer; // r6
        char str[32]; // r29+0xc0
        signed int timer; // r18
        class xBase **b; // r2
        class zScene *s; // r17
        signed int isPaused; // r16
        signed int i; // r18
        static unsigned int sUITimers[10]; // 0x0036A520
        static unsigned int init; // 0x00357E10
        static signed char init; // 0x00357E14
    }
}

// 0x00134760 - 0x00134800 (0xA0) zSceneSetup_serialTraverseCB__FUiP7xSerial
signed int zSceneSetup_serialTraverseCB(unsigned int clientID, class xSerial *xser) {
    { // 0x00134760 - 0x00134800
        signed int val; // r29+0x3c
        char uiName[16]; // r29+0x20
    }
}

// 0x00134800 - 0x00134BC4 (0x3C4) zSceneSetup__Fv
void zSceneSetup() {
    { // 0x00134800 - 0x00134BC4
        class iColor_tag clear; // r29+0xac
        class iColor_tag black; // r29+0xa8
        signed int i; // r17
        unsigned int dontcaresize; // r29+0xa4
        signed int i; // r17
        class zScene *s; // r16
    }
}

// 0x00134BD0 - 0x00134F48 (0x378) zSceneReset__Fv
void zSceneReset() {
    { // 0x00134BD0 - 0x00134F48
        class _tagxCam *cam; // r2
        class _zEnv *env; // r19
        signed int i; // r18
        class zScene *s; // r17
    }
}

// 0x00134F50 - 0x00135404 (0x4B4) zSceneLoad__FP6zSceneP7xSerial
void zSceneLoad(class zScene *ent, class xSerial *s) {
    { // 0x00134F50 - 0x00135404
        class xBase *b; // r2
        unsigned short i; // r16
        signed int wpset; // r29+0x8c
        signed int i; // r18
        signed int sceneExist; // r29+0x88
        class xSerial xser; // r29+0x60
    }
}

// 0x00135410 - 0x00135844 (0x434) zSceneSave__FP6zSceneP7xSerial
void zSceneSave(class zScene *ent, class xSerial *s) {
    { // 0x00135410 - 0x00135844
        class xBase *b; // r2
        class xSerial xser; // r29+0x60
        signed int i; // r18
    }
}

// 0x00135850 - 0x0013596C (0x11C) zSceneSwitch__FP8_zPortal
void zSceneSwitch(class _zPortal *p) {
    { // 0x00135850 - 0x0013596C
        class p2MarkerAsset *m; // r2
        unsigned int size; // r29+0x4c
        class _tagxCam *cam; // r2
        unsigned int PlayerMarkerStartCamID; // r18
        unsigned int PlayerMarkerStartID; // r17
        char *id; // r2
        class zPortalAsset *passet; // r8
    }
}

// 0x00135970 - 0x00135B8C (0x21C) zSceneExit__Fv
void zSceneExit() {
    { // 0x00135970 - 0x00135B8C
        signed int i; // r16
        class RpWorld *world; // r16
        class zScene *s; // r2
    }
}

// 0x00135B90 - 0x001360C0 (0x530) zSceneInit__FUi
void zSceneInit(unsigned int theSceneID) {
    { // 0x00135B90 - 0x001360C0
        signed int idx; // r16
        class zProjectileTypeAsset *tmp_ptass; // r2
        signed int projectile_count; // r17
        unsigned int base_idx; // r2
        unsigned int typeCount; // r2
        char b[5]; // r29+0x70
        unsigned char rgba_bkgrd[4]; // r29+0x7c
        signed int i; // r3
        class zScene *s; // r16
        float pdone; // r29+0x80
    }
}

// 0x001360C0 - 0x001361C4 (0x104) zSceneInitFunc_Dispatcher__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Dispatcher(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x001360C0 - 0x001361C4
        class p2EntAsset *asset; // r2
        class xBase *b; // r19
        signed int idx; // r18
        class st_ZDISPATCH_DATA *dpat_pool; // r17
        signed int count; // r16
    }
}

// 0x001361D0 - 0x001362D4 (0x104) zSceneInitFunc_LobMaster__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_LobMaster(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x001361D0 - 0x001362D4
        class p2EntAsset *asset; // r2
        class xBase *b; // r19
        signed int idx; // r18
        class st_LOBMASTER_DATA *lm_pool; // r17
        signed int count; // r16
    }
}

// 0x001362E0 - 0x001363C8 (0xE8) zSceneInitFunc_Volume__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Volume(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x001362E0 - 0x001363C8
        class xBase *b; // r18
        signed int idx; // r17
        signed int count; // r16
    }
}

// 0x001363D0 - 0x001364A4 (0xD4) zSceneInitFunc_Gust__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Gust(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x001363D0 - 0x001364A4
        class xBase *b; // r2
        signed int idx; // r17
        signed int count; // r16
    }
}

// 0x001364B0 - 0x00136584 (0xD4) zSceneInitFunc_Surface__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Surface(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x001364B0 - 0x00136584
        class xBase *b; // r2
        signed int idx; // r17
        signed int count; // r16
    }
}

// 0x00136590 - 0x001366A4 (0x114) zSceneInitFunc_Camera__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Camera(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x00136590 - 0x001366A4
        class xCamAsset *assetCam; // r18
        class xBase *b; // r2
        signed int idx; // r17
        class _tagxCam *camBlock; // r2
        signed int count; // r16
    }
}

// 0x001366B0 - 0x001367D4 (0x124) zSceneInitFunc_Player__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Player(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x001366B0 - 0x001367D4
        class p2EntAsset *asset; // r2
        class xBase *b; // r20
        signed int idx; // r19
        class _zEnt *entBlock; // r2
        signed int count; // r18
    }
}

// 0x001367E0 - 0x001368F4 (0x114) zSceneInitFunc_Villain__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Villain(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x001367E0 - 0x001368F4
        class p2EntAsset *asset; // r2
        class _zEnt *b; // r17
        unsigned int idx; // r16
        class st_zEntVillain *villainBlock; // r2
        signed int count; // r2
    }
}

// 0x00136900 - 0x00136A14 (0x114) zSceneInitFunc_MovePoint__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_MovePoint(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x00136900 - 0x00136A14
        class p2BaseAsset *basset; // r20
        class xBase *b; // r19
        signed int idx; // r18
        class _zMovePoint *movpBlock; // r2
        unsigned int assetSize; // r29+0x8c
        signed int count; // r17
    }
}

// 0x00136A20 - 0x00136B44 (0x124) zSceneInitFunc_Default__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_Default(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x00136A20 - 0x00136B44
        void *asset; // r2
        signed int i; // r20
        class xBase *b; // r19
        unsigned int offset; // r23
        unsigned int assetSize; // r29+0xac
        signed int count; // r18
        unsigned char *block; // r22
    }
}

// 0x00136B50 - 0x00136C94 (0x144) zSceneInitFunc_DefaultEnt__FP6zSceneP24zSceneObjectInstanceDescUi
static unsigned int zSceneInitFunc_DefaultEnt(class zScene *s, class zSceneObjectInstanceDesc *desc, unsigned int base_idx) {
    { // 0x00136B50 - 0x00136C94
        void *asset; // r2
        signed int i; // r20
        class xBase *b; // r19
        unsigned int offset; // r30
        unsigned int assetSize; // r29+0xbc
        signed int count; // r22
        unsigned char *block; // r23
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zGameState.cpp
// ==========================================================================
unsigned int startPressed; // 0x003574BC
static unsigned int sIntroState_DoDispatchTable[4]; // 0x002903C0
static unsigned int sTitleState_DoDispatchTable[2]; // 0x00357520
static unsigned int sLoadState_DoDispatchTable[3]; // 0x002903B0
static unsigned int sOptionsState_DoDispatchTable[1]; // 0x00357518
static unsigned int sSaveState_DoDispatchTable[3]; // 0x002903A0
static unsigned int sPauseState_DoDispatchTable[2]; // 0x00357510
static unsigned int sGameState_DoDispatchTable[8]; // 0x00290380
static unsigned int sIntroState_DispatchTable[4]; // 0x00290370
static unsigned int sTitleState_DispatchTable[2]; // 0x00357508
static unsigned int sLoadState_DispatchTable[3]; // 0x00290360
static unsigned int sOptionsState_DispatchTable[1]; // 0x00357500
static unsigned int sSaveState_DispatchTable[3]; // 0x00290350
static unsigned int sPauseState_DispatchTable[2]; // 0x003574F8
static unsigned int sGameState_DispatchTable[8]; // 0x00290330
static unsigned int sGameMode_DispatchTable[12]; // 0x00290300
enum _GameOstrich gGameOstrich; // 0x00357E24
enum eGameMode gGameMode; // 0x00357E20
signed int gGameState; // 0x003574F4
// 0x00136CA0 - 0x00136D64 (0xC4) zGameModeSwitch__F9eGameMode
void zGameModeSwitch(enum eGameMode modeNew) {
    { // 0x00136CA0 - 0x00136D64
        enum eGameMode modeOld; // r3
    }
}

// 0x00136D70 - 0x00136E7C (0x10C) zGameStateSwitch__Fi
void zGameStateSwitch(signed int theNewState) {
    { // 0x00136D70 - 0x00136E7C
        unsigned int theEvent; // r6
        signed int stateOld; // r5
    }
}

// 0x00136E80 - 0x00137074 (0x1F4) zGameStateSwitchEvent__Fi
void zGameStateSwitchEvent(signed int event) {
    { // 0x00136E80 - 0x00137074
        signed int new_state; // r17
        signed int new_mode; // r16
    }
}

// 0x00137080 - 0x00137088 (0x8) zGameSetOstrich__F12_GameOstrich
void zGameSetOstrich(enum _GameOstrich o) {
    { // 0x00137080 - 0x00137088
    }
}

// 0x00137090 - 0x00137098 (0x8) zGameGetOstrich__Fv
enum _GameOstrich zGameGetOstrich() {
    { // 0x00137090 - 0x00137098
    }
}

// 0x001370A0 - 0x001370A8 (0x8) zGameModeGet__Fv
signed int zGameModeGet() {
    { // 0x001370A0 - 0x001370A8
    }
}

// 0x001370B0 - 0x001370B8 (0x8) zGameStateGet__Fv
signed int zGameStateGet() {
    { // 0x001370B0 - 0x001370B8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEnt.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
class _ShadowParams gShadowParams[26]; // 0x002903D0
// 0x001370C0 - 0x0013719C (0xDC) zEntGetShadowParams__FP5_xEntP6_xVec3Pf
void zEntGetShadowParams(class _xEnt *ent, class _xVec3 *center, float *radius) {
    { // 0x001370C0 - 0x0013719C
        class _ShadowParams *sp; // r9
        class st_zEntVillain *zp; // r2
        class _xVec3 *at; // r8
        unsigned int i; // r7
        float rad; // r29+0x0
    }
}

// 0x001371A0 - 0x00137264 (0xC4) zParamGetVector__FP16zAssetModelParamUiPCc6_xVec3P6_xVec3
signed int zParamGetVector(class zAssetModelParam *param, unsigned int size, char *tok, class _xVec3 *result) {
    { // 0x001371A0 - 0x00137264
    }
}

// 0x00137270 - 0x001372A8 (0x38) zParamGetFloat__FP16zAssetModelParamUiPCcf
float zParamGetFloat(class zAssetModelParam *param, unsigned int size, char *tok, float def) {
    { // 0x00137270 - 0x001372A8
    }
}

// 0x001372B0 - 0x001372E8 (0x38) zParamGetInt__FP16zAssetModelParamUiPCci
signed int zParamGetInt(class zAssetModelParam *param, unsigned int size, char *tok, signed int def) {
    { // 0x001372B0 - 0x001372E8
    }
}

// 0x001372F0 - 0x0013737C (0x8C) zParamGetString__FP16zAssetModelParamUiPcPc
char *zParamGetString(class zAssetModelParam *param, unsigned int size, char *tok, char *def) {
    { // 0x001372F0 - 0x0013737C
        unsigned int testhash; // r2
    }
}

// 0x00137380 - 0x0013740C (0x8C) zEntGetModelParams__FUiPUi
class zAssetModelParam *zEntGetModelParams(unsigned int assetID, unsigned int *size) {
    { // 0x00137380 - 0x0013740C
        class zAssetModelInfo *minf; // r2
        void *info; // r2
        unsigned int bufsize; // r29+0x2c
    }
}

// 0x00137410 - 0x001376C4 (0x2B4) zEntAnimEvent__FP5_zEntUiPf
void zEntAnimEvent(class _zEnt *ent, unsigned int animEvent, float *animParam) {
    { // 0x00137410 - 0x001376C4
        class xAnimState *ast; // r2
        char name[12]; // r29+0x90
        float prob; // 
        signed int anum; // r17
        class xAnimState *ast; // r2
        char name[12]; // r29+0x80
        signed int anum; // r2
        signed int anum2; // r19
        signed int anum1; // r17
        class xAnimState *ast; // r2
        char name[12]; // r29+0x70
        class xAnimState *ast; // r2
        char name[12]; // r29+0x60
        signed int anum; // r2
        class xAnimSingle *single; // r16
    }
}

// 0x001376D0 - 0x001378B0 (0x1E0) zEntRecurseModelInfo__FPvP5_xEnt
class xModelInstance *zEntRecurseModelInfo(void *info, class _xEnt *ent) {
    { // 0x001376D0 - 0x001378B0
        class xAnimTable *table; // r2
        class zAssetModelInfo *zinfo; // r18
        class xModelInstance *tempInst[64]; // r29+0x70
        class RpAtomic *imodel; // r17
        unsigned int bufsize; // r29+0x17c
        unsigned int i; // r16
    }
}

// 0x001378B0 - 0x00137A1C (0x16C) zEntEvent__FP5xBaseUiP5xBaseUiPfP5xBasei
void zEntEvent(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget, signed int forceEvent) {
    { // 0x001378B0 - 0x00137A1C
        class xBase *sendTo; // r21
        signed int i; // r17
        class p2LinkAsset *idx; // r16
    }
}

// 0x00137A20 - 0x00137A3C (0x1C) zEntEvent__FP5xBaseP5xBaseUi
void zEntEvent(class xBase *from, class xBase *to, unsigned int toEvent) {
    { // 0x00137A20 - 0x00137A3C
    }
}

// 0x00137A40 - 0x00137A60 (0x20) zEntEvent__FP5xBaseUiPfP5xBase
void zEntEvent(class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x00137A40 - 0x00137A60
    }
}

// 0x00137A60 - 0x00137A80 (0x20) zEntEvent__FP5xBaseUiPf
void zEntEvent(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x00137A60 - 0x00137A80
    }
}

// 0x00137A80 - 0x00137AA0 (0x20) zEntEvent__FP5xBaseUi
void zEntEvent(class xBase *to, unsigned int toEvent) {
    { // 0x00137A80 - 0x00137AA0
    }
}

// 0x00137AA0 - 0x00137AF0 (0x50) zEntEvent__FPcUi
void zEntEvent(char *to, unsigned int toEvent) {
    { // 0x00137AA0 - 0x00137AF0
        class xBase *sendTo; // r2
        unsigned int id; // r2
    }
}

// 0x00137AF0 - 0x00137BA8 (0xB8) zEntEventAllOfType__FUiUi
void zEntEventAllOfType(unsigned int toEvent, unsigned int type) {
    { // 0x00137AF0 - 0x00137BA8
    }
}

// 0x00137BB0 - 0x00137C6C (0xBC) zEntEventAll__FP5xBaseUiUiPf
void zEntEventAll(class xBase *from, unsigned int fromEvent, unsigned int toEvent, float *toParam) {
    { // 0x00137BB0 - 0x00137C6C
        unsigned short i; // r17
        class zScene *s; // r16
    }
}

// 0x00137C70 - 0x00137C78 (0x8) zEntReset__FP5_zEnt
void zEntReset(class _zEnt *ent) {
    { // 0x00137C70 - 0x00137C78
    }
}

// 0x00137C80 - 0x00137C88 (0x8) zEntLoad__FP5_zEntP7xSerial
void zEntLoad(class _zEnt *ent, class xSerial *s) {
    { // 0x00137C80 - 0x00137C88
    }
}

// 0x00137C90 - 0x00137C98 (0x8) zEntSave__FP5_zEntP7xSerial
void zEntSave(class _zEnt *ent, class xSerial *s) {
    { // 0x00137C90 - 0x00137C98
    }
}

// 0x00137CA0 - 0x00137CA8 (0x8) zEntSetup__FP5_zEnt
void zEntSetup(class _zEnt *ent) {
    { // 0x00137CA0 - 0x00137CA8
    }
}

// 0x00137CB0 - 0x001382C8 (0x618) zEntInit__FP5_zEntP10p2EntAssetUi
void zEntInit(class _zEnt *ent, class p2EntAsset *asset, unsigned int type) {
    { // 0x00137CB0 - 0x001382C8
        class xAnimSingle *single; // r4
        class xAnimState *ast; // r2
        signed int num_used; // r2
        unsigned int size; // r29+0x44
        void *buf; // r2
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iCamera.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
class RwCamera *globalCamera; // 0x00357E30
float xCameraLastFov; // 0x00000000
float sCameraFarClip; // 0x0035752C
float sCameraNearClip; // 0x00357528
// 0x001382D0 - 0x00138838 (0x568) iCameraUpdateFog__FP8RwCameral
void iCameraUpdateFog(class RwCamera *cam, signed long t) {
    { // 0x001382D0 - 0x00138838
        unsigned int bite_me; // r16
        float a; // r29+0xc0
        class iFogParams *fb; // r19
        class iFogParams *fa; // r18
        class iFogParams *fp; // r17
    }
}

// 0x00138840 - 0x00138A94 (0x254) iCameraSetFogParams__FP10iFogParamsf
void iCameraSetFogParams(class iFogParams *fp, float time) {
    { // 0x00138840 - 0x00138A94
    }
}

// 0x00138AA0 - 0x00138AE8 (0x48) iCameraSetNearFarClip__Fff
void iCameraSetNearFarClip(float near, float far) {
    { // 0x00138AA0 - 0x00138AE8
    }
}

// 0x00138AF0 - 0x00138B00 (0x10) iCameraRemoveEnv__FP8RwCameraP4iEnv
void iCameraRemoveEnv(class RwCamera *camera, class iEnv *env_geom) {
    { // 0x00138AF0 - 0x00138B00
    }
}

// 0x00138B00 - 0x00138B3C (0x3C) iCameraAssignEnv__FP8RwCameraP4iEnv
void iCameraAssignEnv(class RwCamera *camera, class iEnv *env_geom) {
    { // 0x00138B00 - 0x00138B3C
    }
}

// 0x00138B40 - 0x00138BD8 (0x98) iCameraSetFOV__FP8RwCameraf
void iCameraSetFOV(class RwCamera *cam, float fov) {
    { // 0x00138B40 - 0x00138BD8
        class RwV2d vw; // r29+0x28
        float view_window; // r29+0x30
    }
}

// 0x00138BE0 - 0x00138C30 (0x50) iCameraUpdatePos__FP8RwCameraP8_xMat4x3
void iCameraUpdatePos(class RwCamera *cam, class _xMat4x3 *pos) {
    { // 0x00138BE0 - 0x00138C30
        class RwMatrixTag *m; // r17
        class RwFrame *f; // r16
    }
}

// 0x00138C30 - 0x00138D34 (0x104) iCameraFrustumPlanes__FP8RwCameraP6_xVec4
void iCameraFrustumPlanes(class RwCamera *cam, class _xVec4 *frustplane) {
    { // 0x00138C30 - 0x00138D34
        class RwFrustumPlane *rwPlane; // r2
    }
}

// 0x00138D40 - 0x00138D4C (0xC) iCameraShowRaster__FP8RwCamera
void iCameraShowRaster(class RwCamera *cam) {
    { // 0x00138D40 - 0x00138D4C
    }
}

// 0x00138D50 - 0x00138D58 (0x8) iCameraEnd__FP8RwCamera
void iCameraEnd(class RwCamera *cam) {
    { // 0x00138D50 - 0x00138D58
    }
}

// 0x00138D60 - 0x00138DC8 (0x68) iCameraBegin__FP8RwCamerai
void iCameraBegin(class RwCamera *cam, signed int clear) {
    { // 0x00138D60 - 0x00138DC8
    }
}

// 0x00138DD0 - 0x00138E5C (0x8C) iCameraDestroy__FP8RwCamera
void iCameraDestroy(class RwCamera *camera) {
    { // 0x00138DD0 - 0x00138E5C
        class RwFrame *frame; // r16
        class RwRaster *raster; // r2
    }
}

// 0x00138E60 - 0x00138F10 (0xB0) iCameraCreate__Fii
class RwCamera *iCameraCreate(signed int width, signed int height) {
    { // 0x00138E60 - 0x00138F10
        class RwCamera *camera; // r16
        class RwV2d vw; // r29+0x48
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xCamera.cpp
// ==========================================================================
class _xMat4x3 g_I3; // 0x00342E70
signed int gDeadScooby; // 0x00357F2C
class _xVec3 gDeadScoobyPos; // 0x0036E290
signed int gPlayerHitWater; // 0x00357F28
class _xVec3 gPlayerWaterPos; // 0x0036E280
class _xScene *g_xSceneCur; // 0x00357D6C
void (xCameraSeeThruCallback)(class _xEnt *); // 0x0013AF90
unsigned int gActiveHeap; // 0x00357CAC
class zGlobals globals; // 0x00362FD0
static float sShoulderTimer; // 0x00357E64
signed int sDYDown; // 0x00357E60
signed int sDYUp; // 0x00357E5C
signed int sDXRight; // 0x00357E58
signed int sDXLeft; // 0x00357E54
class _xMat4x3 sDummyTargetFrame; // 0x0036A760
class _xVec3 sDummyTargetPos; // 0x00290538
static unsigned int sCamDebugFixInterp; // 0x00357530
static signed int sCamDebugInfoLevel; // 0x00000000
float gCameraLastFov; // 0x00357E48
static signed int sControllerStill; // 0x00357E44
static float sCurYOffset; // 0x00357E40
static float sCurXOffset; // 0x00357E3C
static float sGroundHeight; // 0x00357E38
static signed int sOnGround; // 0x00357E34
static class _xVec3 sLastCamPos; // 0x00290528
static class _xVec3 sLastTargetPos; // 0x00290518
static class _xVec3 sTargetPos; // 0x00290508
// 0x00138F10 - 0x00138F18 (0x8) xCameraGetCurMatrix__FP13_tagxCamFrame
class _xMat4x3 *xCameraGetCurMatrix(class _tagxCamFrame *cam) {
    { // 0x00138F10 - 0x00138F18
    }
}

// 0x00138F20 - 0x00138F28 (0x8) xCameraTargetOnGround__Fif
void xCameraTargetOnGround(signed int on_ground) {
    { // 0x00138F20 - 0x00138F28
    }
}

// 0x00138F30 - 0x0013902C (0xFC) xCameraPickInitial__FP13_tagxCamFrameP8_tagxCam
void xCameraPickInitial(class _tagxCamFrame *cam, class _tagxCam *c) {
    { // 0x00138F30 - 0x0013902C
    }
}

// 0x00139030 - 0x00139178 (0x148) xCameraPickInitial__FP13_tagxCamFrameUi
void xCameraPickInitial(class _tagxCamFrame *cam, unsigned int init_id) {
    { // 0x00139030 - 0x00139178
    }
}

// 0x00139180 - 0x00139420 (0x2A0) xCameraNext__FP13_tagxCamFrameP8_tagxCam
void xCameraNext(class _tagxCamFrame *cam, class _tagxCam *trans_cam) {
    { // 0x00139180 - 0x00139420
        class _xMat3x3 from_new_src; // r29+0x50
        class _xQuat *dst_rot; // r17
        class _xQuat *src_rot; // r16
    }
}

// 0x00139420 - 0x00139C58 (0x838) xCameraInterpValid__FP13_tagxCamFrame13_tagTransTypef
void xCameraInterpValid(class _tagxCamFrame *cam, enum _tagTransType trans_type, float seconds) {
    { // 0x00139420 - 0x00139C58
        class _xQuat dst_fm; // r29+0xc0
        class _xQuat cur_fm; // r29+0xb0
        float t; // r29+0x100
        class _tagxCam *orig; // r18
        class _tagxCam *src; // r17
        class _tagxCam *dest; // r16
    }
}

// 0x00139C60 - 0x00139EE0 (0x280) xCameraSetAsset__FP13_tagxCamFrameP9xCamAsseti
class _tagxCam *xCameraSetAsset(class _tagxCamFrame *cam, class xCamAsset *src, signed int idx) {
    { // 0x00139C60 - 0x00139EE0
        class _tagxCam *dst; // r16
    }
}

// 0x00139EE0 - 0x0013A0A0 (0x1C0) xCameraInit__FP13_tagxCamFrameUi
class _tagxCam *xCameraInit(class _tagxCamFrame *cam, unsigned int total) {
    { // 0x00139EE0 - 0x0013A0A0
        signed int i; // r3
        class _tagxCam *array; // r2
    }
}

// 0x0013A0A0 - 0x0013A0B0 (0x10) xCameraRemoveEnv__FP13_tagxCamFrame
void xCameraRemoveEnv(class _tagxCamFrame *camera) {
    { // 0x0013A0A0 - 0x0013A0B0
    }
}

// 0x0013A0B0 - 0x0013A0C4 (0x14) xCameraAssignEnv__FP13_tagxCamFrameP5_xEnv
void xCameraAssignEnv(class _tagxCamFrame *camera, class _xEnv *env) {
    { // 0x0013A0B0 - 0x0013A0C4
    }
}

// 0x0013A0D0 - 0x0013A0D8 (0x8) xCameraSetTarget__FP13_tagxCamFrameP8_xMat4x3
void xCameraSetTarget(class _tagxCamFrame *cam, class _xMat4x3 *frame_target) {
    { // 0x0013A0D0 - 0x0013A0D8
    }
}

// 0x0013A0E0 - 0x0013A128 (0x48) xCameraSetTarget__FP13_tagxCamFrameP6_xVec3
void xCameraSetTarget(class _tagxCamFrame *cam, class _xVec3 *new_target) {
    { // 0x0013A0E0 - 0x0013A128
    }
}

// 0x0013A130 - 0x0013A138 (0x8) xCameraShowRaster__FP13_tagxCamFrame
void xCameraShowRaster(class _tagxCamFrame *cam) {
    { // 0x0013A130 - 0x0013A138
    }
}

// 0x0013A140 - 0x0013A174 (0x34) xCameraEnd__FP13_tagxCamFramefi
void xCameraEnd(class _tagxCamFrame *cam, float seconds, signed int update_scrn_fx) {
    { // 0x0013A140 - 0x0013A174
    }
}

// 0x0013A180 - 0x0013A224 (0xA4) xCameraUpdate__FP13_tagxCamFramef
void xCameraUpdate(class _tagxCamFrame *cam, float seconds) {
    { // 0x0013A180 - 0x0013A224
    }
}

// 0x0013A230 - 0x0013A538 (0x308) xCameraRunState__FP13_tagxCamFramef
void xCameraRunState(class _tagxCamFrame *cam, float seconds) {
    { // 0x0013A230 - 0x0013A538
        class _tagxCam *c; // r2
    }
}

// 0x0013A540 - 0x0013A72C (0x1EC) xCameraAdjustTargetPos__FP13_tagxCamFramef
static void xCameraAdjustTargetPos(class _tagxCamFrame *cam) {
    { // 0x0013A540 - 0x0013A72C
        class _xVec3 cur_pos; // r29+0x10
        class _xVec3 desired_target_pos; // r29+0x0
        class _tagxCam *c; // r3
    }
}

// 0x0013A730 - 0x0013AEA8 (0x778) xCameraShoulderState__FP13_tagxCamFramef
class _tagxCam *xCameraShoulderState(class _tagxCamFrame *cam, float seconds) {
    { // 0x0013A730 - 0x0013AEA8
        class _xCollis collis_env; // r29+0x140
        class _xRay3 ray; // r29+0x110
        class _xMat4x3 desired_pos_matrix; // r29+0xd0
        class _xMat4x3 target_pos_matrix; // r29+0x90
        class _tagxCam *c; // r2
        static class _xMat4x3 old_matrix; // 0x00290550
    }
}

// 0x0013AF90 - 0x0013AFFC (0x6C) xCameraSeeThruCallback__FP5_xEnt
static void xCameraSeeThruCallback(class _xEnt *ent) {
    { // 0x0013AF90 - 0x0013AFFC
    }
}

// 0x0013B000 - 0x0013B3EC (0x3EC) ShoulderStateRealign__FP13_tagxCamFrameP8_tagxCamf
static void ShoulderStateRealign(class _tagxCamFrame *cam, class _tagxCam *c) {
    { // 0x0013B000 - 0x0013B3EC
        class _xVec3 target_with_y; // r29+0xb0
        float dp2; // 
        class _xVec3 target_with_y; // r29+0xa0
        class _xVec3 desired_pos; // r29+0x90
        class _xVec3 vec; // r29+0x80
        float dp1; // 
        float rad_rot; // r29+0xe0
        class _xMat3x3 rot_matrix; // r29+0x50
    }
}

// 0x0013B3F0 - 0x0013B7F4 (0x404) ShoulderStatePause__FP13_tagxCamFrameP8_tagxCamf
static void ShoulderStatePause(class _tagxCamFrame *cam, class _tagxCam *c, float seconds) {
    { // 0x0013B3F0 - 0x0013B7F4
        class _xVec3 desired_pos; // r29+0x40
    }
}

// 0x0013B800 - 0x0013B864 (0x64) ShoulderStateSwitch__FP13_tagxCamFrameP8_tagxCami
static void ShoulderStateSwitch(class _tagxCamFrame *cam, class _tagxCam *c, signed int new_state) {
    { // 0x0013B800 - 0x0013B864
    }
}

// 0x0013B870 - 0x0013B8D0 (0x60) ShoulderGetDesiredPos__FP13_tagxCamFrameP8_tagxCamP6_xVec3ff
static signed int ShoulderGetDesiredPos(class _tagxCam *c, class _xVec3 *pos, float x, float z) {
    { // 0x0013B870 - 0x0013B8D0
    }
}

// 0x0013B8D0 - 0x0013BC6C (0x39C) xCameraFollowState__FP13_tagxCamFramef
class _tagxCam *xCameraFollowState(class _tagxCamFrame *cam, float seconds) {
    { // 0x0013B8D0 - 0x0013BC6C
        class _xVec3 new_pos; // r29+0xf0
        class _xVec3 preline_test_pos; // r29+0xe0
        float cpb; // 
        float cpa; // 
        signed int trans; // r3
        class _xVec3 tmp; // r29+0xd0
        class _xMat3x3 mrot; // r29+0x80
        class _xVec3 dirvec; // r29+0xc0
        class _xVec3 to_vec; // r29+0xb0
        class _tagxCam *c; // r2
    }
}

// 0x0013BC70 - 0x0013BE10 (0x1A0) GetPointOnLine__FP6_xVec3UiUiP6_xVec3
static void GetPointOnLine(class _xVec3 *C, unsigned int marker1id, unsigned int marker2id, class _xVec3 *result) {
    { // 0x0013BC70 - 0x0013BE10
        class _xVec3 normAB; // r29+0x70
        class _xVec3 AB; // r29+0x60
        class p2MarkerAsset *m2; // r19
        class p2MarkerAsset *m1; // r18
        float len; // r29+0x80
        float r; // 
    }
}

// 0x0013BE10 - 0x0013BED4 (0xC4) xCameraSetController__FP13_tagxCamFramefff
void xCameraSetController(class _tagxCamFrame *cam, float x, float y, float seconds) {
    { // 0x0013BE10 - 0x0013BED4
    }
}

// 0x0013BEE0 - 0x0013C11C (0x23C) xCameraSetTargetDY__FP13_tagxCamFrameff
void xCameraSetTargetDY(class _tagxCamFrame *cam, float dy, float seconds) {
    { // 0x0013BEE0 - 0x0013C11C
        class _tagxCam *c; // r3
        float end; // 
        float start; // 
        float desired; // 
    }
}

// 0x0013C120 - 0x0013C35C (0x23C) xCameraSetTargetDX__FP13_tagxCamFrameff
void xCameraSetTargetDX(class _tagxCamFrame *cam, float dx, float seconds) {
    { // 0x0013C120 - 0x0013C35C
        class _tagxCam *c; // r3
        float end; // 
        float start; // 
        float desired; // 
    }
}

// 0x0013C360 - 0x0013C6E8 (0x388) xCameraFollowWithOffsetState__FP13_tagxCamFramef
class _tagxCam *xCameraFollowWithOffsetState(class _tagxCamFrame *cam, float seconds) {
    { // 0x0013C360 - 0x0013C6E8
        class _xVec3 preline_test_pos; // r29+0xe0
        float cpb; // 
        float cpa; // 
        signed int trans; // r3
        class _xVec3 tmp; // r29+0xd0
        class _xMat3x3 mrot; // r29+0x80
        class _xVec3 dirvec; // r29+0xc0
        class _xVec3 to_vec; // r29+0xb0
        class _tagxCam *c; // r2
    }
}

// 0x0013C6F0 - 0x0013C730 (0x40) xCameraBegin__FP13_tagxCamFramei
void xCameraBegin(class _tagxCamFrame *cam, signed int clear) {
    { // 0x0013C6F0 - 0x0013C730
    }
}

// 0x0013C730 - 0x0013C738 (0x8) xCameraDestroy__FP13_tagxCamFrame
void xCameraDestroy() {
    { // 0x0013C730 - 0x0013C738
    }
}

// 0x0013C740 - 0x0013C7B8 (0x78) xCameraCreate__FP13_tagxCamFrameUiUi
void xCameraCreate(class _tagxCamFrame *cam, unsigned int width, unsigned int height) {
    { // 0x0013C740 - 0x0013C7B8
        static signed int first_time; // 0x00357E4C
        static signed char init; // 0x00357E50
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xMath2.h
// ==========================================================================
// 0x0013AEB0 - 0x0013AF84 (0xD4) xVec2Normalize__FP6_xVec2PC6_xVec2
float xVec2Normalize(class _xVec2 *o, class _xVec2 *v) {
    { // 0x0013AEB0 - 0x0013AF84
        float len; // r29+0x30
        float one_len; // 
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iEnv.cpp
// ==========================================================================
class RwCamera *globalCamera; // 0x00357E30
static class iEnv *lastEnv; // 0x00357E6C
static signed int sBeginDrawFX; // 0x00357E68
// 0x0013C7C0 - 0x0013C82C (0x6C) iEnvEndRenderFX__FP4iEnv
void iEnvEndRenderFX(class iEnv *env) {
    { // 0x0013C7C0 - 0x0013C82C
    }
}

// 0x0013C830 - 0x0013C8A4 (0x74) iEnvRenderFX__FP4iEnv
void iEnvRenderFX(class iEnv *env) {
    { // 0x0013C830 - 0x0013C8A4
        signed int varRENDERSTATEALPHAPRIMITIVEBUFFER; // r29+0x2c
    }
}

// 0x0013C8B0 - 0x0013C924 (0x74) iEnvBeginRenderFX__FP4iEnv
signed int iEnvBeginRenderFX(class iEnv *env) {
    { // 0x0013C8B0 - 0x0013C924
    }
}

// 0x0013C930 - 0x0013C990 (0x60) iEnvRender__FP4iEnv
void iEnvRender(class iEnv *env) {
    { // 0x0013C930 - 0x0013C990
        signed int varRENDERSTATEALPHAPRIMITIVEBUFFER; // r29+0x2c
    }
}

// 0x0013C990 - 0x0013C998 (0x8) iEnvLightingBasics__FP4iEnvP10p2EnvAsset
void iEnvLightingBasics() {
    { // 0x0013C990 - 0x0013C998
    }
}

// 0x0013C9A0 - 0x0013C9A8 (0x8) iEnvDefaultLighting__FP4iEnv
void iEnvDefaultLighting() {
    { // 0x0013C9A0 - 0x0013C9A8
    }
}

// 0x0013C9B0 - 0x0013CA10 (0x60) iEnvFree__FP4iEnv
void iEnvFree(class iEnv *env) {
    { // 0x0013C9B0 - 0x0013CA10
    }
}

// 0x0013CA10 - 0x0013CA5C (0x4C) iEnvLoad__FP4iEnvPCvUii
void iEnvLoad(class iEnv *env, void *data, signed int dataType) {
    { // 0x0013CA10 - 0x0013CA5C
    }
}

// 0x0013CA60 - 0x0013CACC (0x6C) iEnvSetBSP__FP4iEnviP7RpWorld
static void iEnvSetBSP(class iEnv *env, signed int envDataType, class RpWorld *bsp) {
    { // 0x0013CA60 - 0x0013CACC
        class RwSurfaceProperties surf_props; // r29+0x10
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xEnv.cpp
// ==========================================================================
class _xEnv *gCurXEnv; // 0x00357E70
// 0x0013CAD0 - 0x0013CAF8 (0x28) xEnvRender__FP5_xEnv
void xEnvRender(class _xEnv *env) {
    { // 0x0013CAD0 - 0x0013CAF8
    }
}

// 0x0013CB00 - 0x0013CB38 (0x38) xEnvFree__FP5_xEnv
void xEnvFree(class _xEnv *env) {
    { // 0x0013CB00 - 0x0013CB38
    }
}

// 0x0013CB40 - 0x0013CB6C (0x2C) xEnvSetup__FP5_xEnv
void xEnvSetup(class _xEnv *env) {
    { // 0x0013CB40 - 0x0013CB6C
    }
}

// 0x0013CB70 - 0x0013CBA8 (0x38) xEnvLoadBsp__FP5_xEnvPCvUii
void xEnvLoadBsp(class _xEnv *env, void *data, unsigned int datasize, signed int dataType) {
    { // 0x0013CB70 - 0x0013CBA8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xDraw.cpp
// ==========================================================================
unsigned int ourGlobals[4096]; // 0x002B5AD0
class _xVec3 g_O3; // 0x00342E28
static float sph2_yaw; // 0x00000000
static class iColor_tag sgDrawColor; // 0x00357534
// 0x0013CBB0 - 0x0013CC1C (0x6C) xDrawLine__FPC6_xVec3PC6_xVec3
void xDrawLine(class _xVec3 *a, class _xVec3 *b) {
    { // 0x0013CBB0 - 0x0013CC1C
        class RwV3d verts[2]; // r29+0x10
        unsigned int indices[2]; // r29+0x28
    }
}

// 0x0013CC20 - 0x0013CC70 (0x50) xDrawSetColor__FUcUcUcUc
void xDrawSetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    { // 0x0013CC20 - 0x0013CC70
    }
}

// 0x0013CC70 - 0x0013CC9C (0x2C) xDrawSetColor__F10iColor_tag
void xDrawSetColor(class iColor_tag color) {
    { // 0x0013CC70 - 0x0013CC9C
    }
}

// 0x0013CCA0 - 0x0013CCA8 (0x8) xDrawEnd__Fv
void xDrawEnd() {
    { // 0x0013CCA0 - 0x0013CCA8
    }
}

// 0x0013CCB0 - 0x0013CCB8 (0x8) xDrawBegin__Fv
void xDrawBegin() {
    { // 0x0013CCB0 - 0x0013CCB8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iDraw.cpp
// ==========================================================================
u_long128 *sweLocalPacket; // 0x00357754
signed long skyFrame_1; // 0x003577C0
// 0x0013CCC0 - 0x0013CCC8 (0x8) iDrawEnd__Fv
void iDrawEnd() {
    { // 0x0013CCC0 - 0x0013CCC8
    }
}

// 0x0013CCD0 - 0x0013CCD8 (0x8) iQueueLineList__F10iColor_tagP5RwV3dPUiP11RwMatrixTagUiUi
void iQueueLineList() {
    { // 0x0013CCD0 - 0x0013CCD8
    }
}

// 0x0013CCE0 - 0x0013CCE8 (0x8) iDrawBegin__Fv
void iDrawBegin() {
    { // 0x0013CCE0 - 0x0013CCE8
    }
}

// 0x0013CCF0 - 0x0013CD7C (0x8C) iDrawSetFBMSK__FUi
void iDrawSetFBMSK(unsigned int FBMSK) {
    { // 0x0013CCF0 - 0x0013CD7C
        u_long128 ___ltmp; // r4
        u_long128 ___ltmp; // r6
        unsigned long long hi; // r5
        unsigned long long lo; // r3
        unsigned long long tmp; // r3
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xString.cpp
// ==========================================================================
// 0x0013CD80 - 0x0013CF68 (0x1E8) xStrParseFloatList__FPfPCci
signed int xStrParseFloatList(float *dest, char *strbuf, signed int max) {
    { // 0x0013CD80 - 0x0013CF68
        char *numstart; // r2
        signed int negate; // r18
        signed int digits; // r3
        signed int index; // r17
        char *str; // r16
    }
}

// 0x0013CF70 - 0x0013CFB4 (0x44) xStrupr__FPc
char *xStrupr(char *string) {
    { // 0x0013CF70 - 0x0013CFB4
        char *p; // r3
    }
}

// 0x0013CFC0 - 0x0013D0BC (0xFC) xStricmp__FPCcPCc
signed int xStricmp(char *string1, char *string2) {
    { // 0x0013CFC0 - 0x0013D0BC
        signed int result; // r3
    }
}

// 0x0013D0C0 - 0x0013D1F8 (0x138) xStrTokBuffer__FPCcPCcPv
char *xStrTokBuffer(char *string, char *control, void *buffer) {
    { // 0x0013D0C0 - 0x0013D1F8
        signed int count; // r7
        unsigned char map[32]; // r29+0x0
        unsigned char *ctrl; // r2
        unsigned char *str; // r8
        char *dest; // r2
        char **nextoken; // r2
    }
}

// 0x0013D200 - 0x0013D32C (0x12C) xStrTok__FPcPCcPPc
char *xStrTok(char *string, char *control, char **nextoken) {
    { // 0x0013D200 - 0x0013D32C
        signed int count; // r3
        unsigned char map[32]; // r29+0x0
        unsigned char *ctrl; // r2
        unsigned char *str; // r7
    }
}

// 0x0013D330 - 0x0013D37C (0x4C) xStrHash__FPc
unsigned int xStrHash(char *str) {
    { // 0x0013D330 - 0x0013D37C
        char c; // r2
        unsigned int i; // r2
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntVillain.cpp
// ==========================================================================
void (VILN_KookRender)(class _xEnt *); // 0x00160CF0
signed int (VILN_std_flag)(class st_zEntVillain *, enum en_ZVIL_FLAG_GROUPS, enum en_BEHAVIOUR_GOAL); // 0x00169670
signed int (VILN_std_query)(class st_zEntVillain *, enum en_ZVIL_QUERY); // 0x00168C60
signed int (VILN_std_sysevent)(class st_zEntVillain *, class xBase *, unsigned int, float *, class xBase *, class st_STIMULUS_CONTEXT_DATA *); // 0x00168470
signed int (VILN_std_notice)(class st_zEntVillain *, enum en_ZVIL_NOTICE, class st_STIMULUS_CONTEXT_DATA *); // 0x00168350
signed int (VILN_std_aware)(class st_zEntVillain *, class _xScene *, float, class st_STIMULUS_CONTEXT_DATA *); // 0x001656E0
signed int (VILN_std_stimulus)(class st_zEntVillain *, class st_STIMULUS_CONTEXT_DATA *, class st_STIMULUS_CONTEXT_DATA *); // 0x00163F90
signed int (VILN_std_trangoal)(class st_zEntVillain *, class st_STIMULUS_CONTEXT_DATA *, float); // 0x001621B0
signed int (VILN_std_calcmove)(class st_zEntVillain *, class _xScene *, float, class _xEntFrame *); // 0x00161810
signed int (VILN_std_respond)(class st_zEntVillain *, class st_STIMULUS_CONTEXT_DATA *); // 0x00161790
signed int (VILN_std_pickanim)(class st_zEntVillain *, enum en_BEHAVIOUR_GOAL, enum en_ZVIL_GOAL_SPOT); // 0x00160D60
unsigned char gVillainPoofColorBirth[50][4]; // 0x002FE100
unsigned char gVillainPoofColorDeath[50][4]; // 0x002FE1D0
unsigned char gVillainDotsColorBirth[50][4]; // 0x002FE2A0
unsigned char gVillainDotsColorDeath[50][4]; // 0x002FE370
class _xVec3 g_NY3; // 0x00342E58
signed int (VILN_std_xtrarend)(class st_zEntVillain *); // 0x00160D30
signed int (VNAV_chklos_timz)(void *, class _xVec3 *, class _zMovePoint *); // 0x00151A90
signed int (zVIL_elcb_event)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x00153830
void (zVIL_elcb_update)(class _xEnt *, class _xScene *, float); // 0x00153530
void (zVIL_elcb_bupdate)(class _xEnt *, class _xVec3 *); // 0x00153380
void (zVIL_elcb_move)(class _xEnt *, class _xScene *, float, class _xEntFrame *); // 0x00153310
class iColor_tag g_MAROON; // 0x0034E438
class iColor_tag g_LAVENDER; // 0x00000000
class iColor_tag g_MIDNIGHTBLUE; // 0x00000000
class iColor_tag g_NEON_BLUE; // 0x00000000
class iColor_tag g_NEON_GREEN; // 0x0034E430
class iColor_tag g_YELLOW; // 0x00000000
class iColor_tag g_ORANGE; // 0x00000000
class iColor_tag g_NEON_RED; // 0x0034E428
class _xVec3 g_Z3; // 0x00342E48
signed int (VWEP_prjOwnerNotify)(class zEntProjectile *, enum en_PROJECTILE_EVENT, void *); // 0x0014C8F0
void (VPRJ_BBK_bupdate)(class _xEnt *, class _xVec3 *); // 0x0014BA60
void (VPRJ_BMM_bupdate)(class _xEnt *, class _xVec3 *); // 0x00149F10
void (VWEP_rbs_bupdate)(class _xEnt *, class _xVec3 *); // 0x00149020
class _xVec3 g_Y3; // 0x00342E38
unsigned int gActiveHeap; // 0x00357CAC
unsigned int (VANM_dotran)(class xAnimTransition *, class xAnimSingle *, class st_zEntVillain *); // 0x0013FC20
unsigned int (VANM_didtran)(class xAnimTransition *, class xAnimSingle *, class st_zEntVillain *); // 0x0013FB40
unsigned int (VANM_dodflt)(class xAnimTransition *, class xAnimSingle *, class st_zEntVillain *); // 0x0013FB30
unsigned int (VANM_diddflt)(class xAnimTransition *, class xAnimSingle *, class st_zEntVillain *); // 0x0013FA50
class zGlobals globals; // 0x00362FD0
class _xVec3 g_O3; // 0x00342E28
static char *g_datablock; // 0x00357EEC
static class st_ZVMM_FLASK_DATA g_mmflask[10]; // 0x002971A0
static char *g_stz_mmftyp_name[3]; // 0x00000000
static class _zMovePoint *g_mmtp[12]; // 0x00297170
static char *g_strz_mmtp[12]; // 0x00297140
static class _tagLightning *g_mm_lytstrk[5]; // 0x0036C490
static char *g_strz_rbtgt[5]; // 0x00297120
static class st_REDBEARD_SWORD g_rbsword[4]; // 0x0036A7D0
static class st_BGG_QUAKEBALL g_quakepool[4]; // 0x00297070
static class st_BKAXE_DATA g_axfxpool[10]; // 0x00296E10
static class _tagLightning *g_bk_lytstrk[5]; // 0x0036A7B0
static signed int g_scene_is_a_boss_battle; // 0x00357EE8
static signed long g_stat_tymr; // 0x00000000
static char *g_parmname[49]; // 0x00296D40
static char *g_animname[148]; // 0x00296AF0
static unsigned int g_anamhash[147]; // 0x002968A0
static class _zParEmitterCustomSettings g_pecust; // 0x00296840
static class st_zEntVillain_ParEmitInfo g_emitinfo[32]; // 0x002965C0
static signed int g_emitready; // 0x00357EE4
static signed int g_emitnext; // 0x00357EE0
static class st_zEntVillain_ParEmitRecord *g_emitpool; // 0x00357EDC
static class st_XORDEREDARRAY g_emitlist; // 0x002965B0
static signed int g_stattotvil; // 0x00357ED8
static class st_NPC_TALLY_RECORD g_stattally[51]; // 0x00296280
static char *g_strz_npcBonus[11]; // 0x00296250
static class st_NPC_BONUS g_npcBonus[11]; // 0x002961F0
static signed int g_bonusTableReady; // 0x00357ED4
static class st_NPCTYPE_CONFIG g_npcTypeTable[50]; // 0x00000000
static class st_ZVIL_NPC_HANDLERFUNCS g_npcFuncs[51]; // 0x00295790
static class st_ZVIL_NPCTYPE_LOOKUP g_npcTable[51]; // 0x00294470
static class RpAtomic *g_gumModelAsset; // 0x00357ED0
static enum en_NPC_STIMULUS g_pstm; // 0x00357ECC
static enum en_BEHAVIOUR_GOAL g_pgol; // 0x00357EC8
static class st_zEntVillain *g_npcpool; // 0x00357EC4
static signed int g_npccnt; // 0x00357EC0
static signed int g_npcinit; // 0x00357EBC
static signed int g_NPCSoundStreamLock; // 0x00357EB8
static class st_NPC_SOUND_ENTRY g_npcJukeBox[33]; // 0x00294360
static class st_NPC_SOUND_RECORDS g_sndTrax_Zombie[32]; // 0x002941E0
static class st_NPC_SOUND_RECORDS g_sndTrax_Wolfman[39]; // 0x00294000
static class st_NPC_SOUND_RECORDS g_sndTrax_WitchDoctor[53]; // 0x00293D80
static class st_NPC_SOUND_RECORDS g_sndTrax_Witch[74]; // 0x00293A00
static class st_NPC_SOUND_RECORDS g_sndTrax_TarMonster[88]; // 0x002935E0
static class st_NPC_SOUND_RECORDS g_sndTrax_SpaceKook[25]; // 0x002934B0
static class st_NPC_SOUND_RECORDS g_sndTrax_Robot[65]; // 0x002931A0
static class st_NPC_SOUND_RECORDS g_sndTrax_Rat[6]; // 0x00293150
static class st_NPC_SOUND_RECORDS g_sndTrax_Shark[17]; // 0x00293080
static class st_NPC_SOUND_RECORDS g_sndTrax_ScareCrow[78]; // 0x00292CD0
static class st_NPC_SOUND_RECORDS g_sndTrax_SeaCreature[66]; // 0x002929B0
static class st_NPC_SOUND_RECORDS g_sndTrax_KillerPlant[7]; // 0x00292950
static class st_NPC_SOUND_RECORDS g_sndTrax_HeadlessSpecter[35]; // 0x002927A0
static class st_NPC_SOUND_RECORDS g_sndTrax_Ghost[5]; // 0x00292760
static class st_NPC_SOUND_RECORDS g_sndTrax_Gargoyle[61]; // 0x00292480
static class st_NPC_SOUND_RECORDS g_sndTrax_GhostDiver[73]; // 0x00292110
static class st_NPC_SOUND_RECORDS g_sndTrax_Creeper[45]; // 0x00291EF0
static class st_NPC_SOUND_RECORDS g_sndTrax_Caveman[43]; // 0x00291CE0
static class st_NPC_SOUND_RECORDS g_sndTrax_CaptainMoody[27]; // 0x00291B90
static class st_NPC_SOUND_RECORDS g_sndTrax_BuckMaster[54]; // 0x00291900
static class st_NPC_SOUND_RECORDS g_sndTrax_Bat[9]; // 0x00291890
static class st_NPC_SOUND_RECORDS g_sndTrax_TimConway[44]; // 0x00000000
static class st_NPC_SOUND_RECORDS g_sndTrax_Professor[1]; // 0x00291878
static class st_NPC_SOUND_RECORDS g_sndTrax_GroundsKeeper[67]; // 0x00291550
static class st_NPC_SOUND_RECORDS g_sndTrax_Holly[41]; // 0x00291360
static class st_NPC_SOUND_RECORDS g_sndTrax_Shaggy[1]; // 0x00000000
static class st_NPC_SOUND_RECORDS g_sndTrax_Velma[1]; // 0x00291348
static class st_NPC_SOUND_RECORDS g_sndTrax_Daphne[3]; // 0x00291320
static class st_NPC_SOUND_RECORDS g_sndTrax_Fred[1]; // 0x00291310
static class st_NPC_SOUND_RECORDS g_sndTrax_MasterMind[77]; // 0x00290F70
static class st_NPC_SOUND_RECORDS g_sndTrax_RedBeard[23]; // 0x00290E50
static class st_NPC_SOUND_RECORDS g_sndTrax_GreenGhost[35]; // 0x00290CA0
static class st_NPC_SOUND_RECORDS g_sndTrax_BlackKnight[49]; // 0x00290A50
static class st_NPC_SOUND_RECORDS g_sndTrax_General[65]; // 0x00290740
static class st_NPC_SOUND_PROPERTIES g_sndProps[36]; // 0x00290590
static class _xVec3 sMMPreTeleportPos; // 0x0036A7A0
static class _zParEmitter *sEmitMMTeleportShow; // 0x00357EB4
static class _zParEmitter *sEmitMMTeleportHide; // 0x00357EB0
static class _zParEmitter *sEmitMMBlackFlask; // 0x00357EAC
static class _zParEmitter *sEmitMMGreenFlask; // 0x00357EA8
static class _zParEmitter *sEmitMMRedFlask; // 0x00357EA4
static class _zParEmitter *sEmitKillerPlant; // 0x00357EA0
static class _zParEmitter *sEmitWitchDying; // 0x00357E9C
static class _zParEmitter *sEmitGreenBallShockwave; // 0x00357E98
static class _zParEmitter *sEmitGreenBallSmack; // 0x00357E94
static class _zParEmitter *sEmitGreenBall; // 0x00357E90
static class _zParEmitter *sEmitGGVortexHurt; // 0x00357E8C
static class _zParEmitter *sEmitGGVortex3; // 0x00357E88
static class _zParEmitter *sEmitGGVortex2; // 0x00357E84
static class _zParEmitter *sEmitGGVortex1; // 0x00357E80
static class _zParEmitter *sEmitRBFlames; // 0x00357E7C
static class _zParEmitter *sEmitHoloDots; // 0x00357E78
static class _zParEmitter *sEmitHoloPoof; // 0x00357E74
// 0x0013D380 - 0x0013D508 (0x188) VSUP_cheap_reflect__FP6_xVec3P6_xVec3
static void VSUP_cheap_reflect(class _xVec3 *out, class _xVec3 *alter) {
    { // 0x0013D380 - 0x0013D508
    }
}

// 0x0013D510 - 0x0013D838 (0x328) VSUP_accum_collide__FP14st_zEntVillainP6_xVec3
static signed int VSUP_accum_collide(class st_zEntVillain *vil, class _xVec3 *vec_accum) {
    { // 0x0013D510 - 0x0013D838
        signed int looked; // r19
        class _xVec3 dir_trav; // r29+0x80
        signed int hit_something; // r18
        class _xCollis *colrec; // r3
        signed int i; // r3
    }
}

// 0x0013D840 - 0x0013D968 (0x128) VSUP_wtadjust__FPfif
static void VSUP_wtadjust(float *wts, signed int cnt, float arbref) {
    { // 0x0013D840 - 0x0013D968
        float fac; // 
        float sum; // r29+0x0
        signed int i; // r6
    }
}

// 0x0013D970 - 0x0013DA54 (0xE4) VSUP_choose__FPiPfi
static signed int VSUP_choose(signed int *list, float *wts, signed int cnt) {
    { // 0x0013D970 - 0x0013DA54
        float dlta; // 
        float pt; // r29+0x60
        float hi; // 
        float lo; // 
        signed int i; // r3
        signed int idx; // r19
    }
}

// 0x0013DA80 - 0x0013DC4C (0x1CC) VSUP_ent_AOI__FPC14st_zEntVillainPC5_zEntP15en_NPC_QUADRANT
static float VSUP_ent_AOI(class st_zEntVillain *vil, class _zEnt *ent, enum en_NPC_QUADRANT *qdrant) {
    { // 0x0013DA80 - 0x0013DC4C
        float ang_diff; // r29+0x60
        float ang_aoi; // 
    }
}

// 0x0013DC50 - 0x0013DE48 (0x1F8) VSUP_ent_inLOS_timz__FPC14st_zEntVillainPC5_zEntif
static signed int VSUP_ent_inLOS_timz(class st_zEntVillain *vil, class _zEnt *ent, signed int prev, float rand) {
    { // 0x0013DC50 - 0x0013DE48
        class _xVec3 mypos; // r29+0xc0
        class _xRay3 r; // r29+0x90
        signed int result; // r2
    }
}

// 0x0013DE50 - 0x0013DE58 (0x8) VSUP_ent_inLOS__FPC14st_zEntVillainPC5_zEntif
static signed int VSUP_ent_inLOS(class st_zEntVillain *vil, class _zEnt *ent, signed int prev, float rand) {
    { // 0x0013DE50 - 0x0013DE58
    }
}

// 0x0013DE60 - 0x0013DF3C (0xDC) VSUP_ent_inFOV__FPC14st_zEntVillainPC5_zEnt
static signed int VSUP_ent_inFOV(class st_zEntVillain *vil, class _zEnt *ent) {
    { // 0x0013DE60 - 0x0013DF3C
        signed int result; // r16
    }
}

// 0x0013DF40 - 0x0013DF84 (0x44) VSUP_ang_toXZDir__FfP6_xVec3
static void VSUP_ang_toXZDir(float angle, class _xVec3 *dir) {
    { // 0x0013DF40 - 0x0013DF84
    }
}

// 0x0013DFA0 - 0x0013E084 (0xE4) VSUP_dir_toPos__FPC5_zEnt6_xVec3P6_xVec3
static float VSUP_dir_toPos(class _zEnt *ent, class _xVec3 *dir) {
    { // 0x0013DFA0 - 0x0013E084
        class _xVec3 delta; // r29+0x40
        class _xVec3 pos; // r29+0x30
    }
}

// 0x0013E090 - 0x0013E120 (0x90) VSUP_ds2_toPos__FPC5_zEnt6_xVec3
static float VSUP_ds2_toPos(class _zEnt *ent) {
    { // 0x0013E090 - 0x0013E120
        class _xVec3 delta; // r29+0x10
        class _xVec3 pos; // r29+0x0
    }
}

// 0x0013E120 - 0x0013E738 (0x618) VBON_choose_bonusItem__F23en_zEntVillainSubTypeID
static class st_NPC_BONUS *VBON_choose_bonusItem(enum en_zEntVillainSubTypeID vtyp) {
    { // 0x0013E120 - 0x0013E738
        signed int wt_ramp; // r17
        class _xVec3 farfar; // r29+0xe0
        signed int soap_always; // r20
        float food_weight; // 
        float gum_weight; // r29+0xf0
        float soap_weight; // 
        float rat; // 
        signed int choice; // r2
        signed int cnt; // r19
        float wts[11]; // r29+0xb0
        signed int list[11]; // r29+0x80
        signed int i; // r18
    }
}

// 0x0013E740 - 0x0013E910 (0x1D0) VBON_drop_bonusItem__FP14st_zEntVillain
static void VBON_drop_bonusItem(class st_zEntVillain *vil) {
    { // 0x0013E740 - 0x0013E910
        signed int skip_distchk; // r5
        signed int overlap; // r3
        class _xVec3 vec_tmp; // r29+0x30
        signed int i; // r7
        class st_NPC_BONUS *bon; // r16
    }
}

// 0x0013E910 - 0x0013EA24 (0x114) VBON_setup_bonusTable__Fv
static void VBON_setup_bonusTable() {
    { // 0x0013E910 - 0x0013EA24
        unsigned int aid_pick; // r2
        signed int i; // r16
        class _xVec3 farfar; // r29+0x40
    }
}

// 0x0013EA30 - 0x0013EB3C (0x10C) VSND_playFromSFX__FP14st_zEntVillainP4zSFX
static void VSND_playFromSFX(class st_zEntVillain *vil, class zSFX *sfx) {
    { // 0x0013EA30 - 0x0013EB3C
        class st_NPC_SOUND_PROPERTIES *sprop; // r29+0x3c
    }
}

// 0x0013EB40 - 0x0013EC60 (0x120) VSND_playFromSimilar__FP14st_zEntVillainP12zAnimFxSoundPUi
static signed int VSND_playFromSimilar(class st_zEntVillain *vil, class zAnimFxSound *afxsnd, unsigned int *xsid_caller) {
    { // 0x0013EB40 - 0x0013EC60
        unsigned int xsid; // r2
        float radius; // r29+0x40
        enum en_NPC_SOUND sndEvent; // r2
    }
}

// 0x0013EC60 - 0x0013ECAC (0x4C) VSND_findProps__F12en_NPC_SOUNDPP23st_NPC_SOUND_PROPERTIES
static signed int VSND_findProps(enum en_NPC_SOUND sndEvent, class st_NPC_SOUND_PROPERTIES **sprop_caller) {
    { // 0x0013EC60 - 0x0013ECAC
        class st_NPC_SOUND_PROPERTIES *sprop; // r3
    }
}

// 0x0013ECB0 - 0x0013EE5C (0x1AC) VSND_select_sound__FP14st_zEntVillain12en_NPC_SOUND
static unsigned int VSND_select_sound(class st_zEntVillain *vil, enum en_NPC_SOUND sndEvent) {
    { // 0x0013ECB0 - 0x0013EE5C
        float use_wt; // r29+0x120
        signed int cnt; // r9
        float wts[32]; // r29+0xa0
        signed int list[32]; // r29+0x20
        class st_NPC_SOUND_RECORDS *trax; // r2
        class st_NPC_SOUND_ENTRY *entry; // r3
        enum en_zEntVillainSubTypeID viltyp; // r8
        unsigned int da_aid_of_sound; // r16
    }
}

// 0x0013EE60 - 0x0013EEF0 (0x90) VSND_typeFromHash__FP14st_zEntVillainUi
static enum en_NPC_SOUND VSND_typeFromHash(class st_zEntVillain *vil, unsigned int aid_snd) {
    { // 0x0013EE60 - 0x0013EEF0
        class st_NPC_SOUND_RECORDS *trax; // r2
        class st_NPC_SOUND_ENTRY *entry; // r7
        enum en_zEntVillainSubTypeID viltyp; // r6
        enum en_NPC_SOUND typ_sndEvent; // r2
    }
}

// 0x0013EEF0 - 0x0013F0F8 (0x208) VSND_goPlay__FP14st_zEntVillain12en_NPC_SOUNDUiif
static unsigned int VSND_goPlay(class st_zEntVillain *vil, enum en_NPC_SOUND sndEvent, unsigned int sndExplicit, signed int flg_vsnd, float dist) {
    { // 0x0013EEF0 - 0x0013F0F8
        float vol; // r29+0x60
        unsigned int xsid; // r17
        unsigned int owner; // r2
        unsigned int priority; // r2
        unsigned int aid_toplay; // r16
    }
}

// 0x0013F100 - 0x0013F260 (0x160) VSND_procQueue__FP14st_zEntVillainf
static signed int VSND_procQueue(class st_zEntVillain *vil, float dt) {
    { // 0x0013F100 - 0x0013F260
        signed int cnt; // r18
        signed int i; // r17
        class st_NPC_SOUND_QUEUE *que; // r16
    }
}

// 0x0013F260 - 0x0013F354 (0xF4) VSND_addQueueEntry__FP14st_zEntVillain12en_NPC_SOUNDifffiUi
static signed int VSND_addQueueEntry(class st_zEntVillain *vil, enum en_NPC_SOUND sndEvent, signed int flg_vsnd, float radius, float delay, float repeatDelay, signed int repeatCount, unsigned int useThisSound) {
    { // 0x0013F260 - 0x0013F354
        signed int added; // r17
        signed int i; // r5
        class st_NPC_SOUND_QUEUE *que; // r16
    }
}

// 0x0013F360 - 0x0013F3EC (0x8C) VSND_setup_soundTable__Fv
static void VSND_setup_soundTable() {
    { // 0x0013F360 - 0x0013F3EC
        unsigned int hash; // r17
        class st_NPC_SOUND_RECORDS *trax; // r2
        class st_NPC_SOUND_ENTRY *entry; // r16
    }
}

// 0x0013F3F0 - 0x0013F420 (0x30) VANM_entSingle__FPC5_zEnt
static class xAnimSingle *VANM_entSingle(class _zEnt *ent) {
    { // 0x0013F3F0 - 0x0013F420
    }
}

// 0x0013F420 - 0x0013F5C0 (0x1A0) VANM_findtran__FP14st_zEntVillainP10xAnimTableP10xAnimStateP10xAnimStateiPP15xAnimTransition
static class xAnimTransition *VANM_findtran(class st_zEntVillain *ent, class xAnimState *from, class xAnimState *to, signed int autoslamit, class xAnimTransition **newtran) {
    { // 0x0013F420 - 0x0013F5C0
        class xAnimTransition *tmp_tran; // r5
        class xAnimTransitionList *tmp_tranlist; // r4
        class xAnimTransition *smack; // r2
        class xAnimTransition *da_tran; // r16
    }
}

// 0x0013F5C0 - 0x0013F7E0 (0x220) VANM_randset__FP14st_zEntVillainPfP17en_NPC_ANIMSTATESi
static enum en_NPC_ANIMSTATES VANM_randset(class st_zEntVillain *ent, float *wts, enum en_NPC_ANIMSTATES *choice_list, signed int cnt) {
    { // 0x0013F5C0 - 0x0013F7E0
        signed int act_cnt; // r21
        float *act_wts; // r22
        signed int use_anim; // r2
        signed int *act_list; // r29+0xac
        class xAnimState *chk_ast; // r2
        signed int i; // r16
        enum en_NPC_ANIMSTATES fnl_anim; // r16
    }
}

// 0x0013F7E0 - 0x0013FA44 (0x264) VANM_set__FP14st_zEntVillainP17en_NPC_ANIMSTATESii
static enum en_NPC_ANIMSTATES VANM_set(class st_zEntVillain *ent, enum en_NPC_ANIMSTATES *best, signed int cnt, signed int sync) {
    { // 0x0013F7E0 - 0x0013FA44
        class xAnimState *astate; // r2
        signed int i; // r16
    }
}

// 0x0013FA50 - 0x0013FB30 (0xE0) VANM_diddflt__FP15xAnimTransitionP11xAnimSingleP14st_zEntVillain
static unsigned int VANM_diddflt(class xAnimSingle *single, class st_zEntVillain *ent) {
    { // 0x0013FA50 - 0x0013FB30
        float tym; // r29+0x1c
    }
}

// 0x0013FB30 - 0x0013FB38 (0x8) VANM_dodflt__FP15xAnimTransitionP11xAnimSingleP14st_zEntVillain
static unsigned int VANM_dodflt() {
    { // 0x0013FB30 - 0x0013FB38
    }
}

// 0x0013FB40 - 0x0013FC20 (0xE0) VANM_didtran__FP15xAnimTransitionP11xAnimSingleP14st_zEntVillain
static unsigned int VANM_didtran(class xAnimSingle *single, class st_zEntVillain *ent) {
    { // 0x0013FB40 - 0x0013FC20
        float tym; // r29+0x1c
    }
}

// 0x0013FC20 - 0x0013FD3C (0x11C) VANM_dotran__FP15xAnimTransitionP11xAnimSingleP14st_zEntVillain
static unsigned int VANM_dotran(class xAnimTransition *tran, class xAnimSingle *single, class st_zEntVillain *ent) {
    { // 0x0013FC20 - 0x0013FD3C
        unsigned int dotran; // r16
    }
}

// 0x0013FD40 - 0x00142314 (0x25D4) VANM_mktable_STD__Fv
static class xAnimTable *VANM_mktable_STD() {
    { // 0x0013FD40 - 0x00142314
        signed int i; // r17
        class xAnimTransition *deftran[147]; // r29+0x390
        class xAnimTransition *intranz[147]; // r29+0x140
        char *strptrz[32]; // r29+0xc0
        class xAnimTable *table; // r16
    }
}

// 0x00142320 - 0x00143044 (0xD24) VANM_mktable_GANG__Fv
static class xAnimTable *VANM_mktable_GANG() {
    { // 0x00142320 - 0x00143044
        signed int j; // r17
        signed int i; // r17
        class xAnimTransition *deftran[147]; // r29+0x380
        class xAnimTransition *intranz[147]; // r29+0x130
        char *strptrz[32]; // r29+0xb0
        class xAnimTable *table; // r16
    }
}

// 0x00143050 - 0x001431E8 (0x198) VANM_mktable_SIMP__Fv
static class xAnimTable *VANM_mktable_SIMP() {
    { // 0x00143050 - 0x001431E8
        signed int i; // r17
        class xAnimTable *table; // r16
    }
}

// 0x001431F0 - 0x0014382C (0x63C) VANM_mktable_SHARK__Fv
static class xAnimTable *VANM_mktable_SHARK() {
    { // 0x001431F0 - 0x0014382C
        signed int i; // r17
        class xAnimTransition *tmp_tran; // r2
        class xAnimTable *table; // r16
    }
}

// 0x00143830 - 0x00143AE0 (0x2B0) VANM_mktable_SHAM__Fv
static class xAnimTable *VANM_mktable_SHAM() {
    { // 0x00143830 - 0x00143AE0
        signed int i; // r17
        class xAnimTable *table; // r16
    }
}

// 0x00143AE0 - 0x00144F5C (0x147C) VANM_mktable_MM__Fv
static class xAnimTable *VANM_mktable_MM() {
    { // 0x00143AE0 - 0x00144F5C
        signed int i; // r16
        char *strptrz[32]; // r29+0xb0
        class xAnimTable *table; // r19
    }
}

// 0x00144F60 - 0x00145D80 (0xE20) VANM_mktable_RB__Fv
static class xAnimTable *VANM_mktable_RB() {
    { // 0x00144F60 - 0x00145D80
        signed int i; // r16
        char *strptrz[32]; // r29+0xb0
        class xAnimTable *table; // r19
    }
}

// 0x00145D80 - 0x00146714 (0x994) VANM_mktable_GG__Fv
static class xAnimTable *VANM_mktable_GG() {
    { // 0x00145D80 - 0x00146714
        signed int i; // r17
        char *strptrz[32]; // r29+0xb0
        class xAnimTable *table; // r16
    }
}

// 0x00146720 - 0x00147308 (0xBE8) VANM_mktable_BK__Fv
static class xAnimTable *VANM_mktable_BK() {
    { // 0x00146720 - 0x00147308
        signed int i; // r16
        char *strptrz[32]; // r29+0xb0
        class xAnimTransition *tmp_tran; // r2
        class xAnimTable *table; // r19
    }
}

// 0x00147310 - 0x001475C4 (0x2B4) VPAR_emit_extattack__FP14st_zEntVillainP6_xVec3P6_xVec3fff
static void VPAR_emit_extattack(class st_zEntVillain *ent, class _xVec3 *origin, class _xVec3 *aimdir, float length, float slope, float dt) {
    { // 0x00147310 - 0x001475C4
        class st_zEntVillain_ParEmitRecord *vpar; // r18
        class _zParEmitterCustomSettings *pecust; // r17
        float delta; // 
        float fv; // 
        signed int j; // r16
        signed int i; // r23
    }
}

// 0x001475D0 - 0x00147750 (0x180) VPAR_emit_headbutt__FP14st_zEntVillainfP28st_zEntVillain_ParEmitRecordiP6_xVec3P26_zParEmitterCustomSettings
static signed int VPAR_emit_headbutt(float dt, class st_zEntVillain_ParEmitRecord *vpar, class _zParEmitterCustomSettings *pecust) {
    { // 0x001475D0 - 0x00147750
        class _xVec3 angles; // r29+0x50
        class _xEnt *player_ent; // r6
    }
}

// 0x00147750 - 0x00147B94 (0x444) VPAR_emit_partype__FP14st_zEntVillain20en_VPAR_EMITTER_ROLEf
static void VPAR_emit_partype(class st_zEntVillain *ent, enum en_VPAR_EMITTER_ROLE emittype, float dt) {
    { // 0x00147750 - 0x00147B94
        class _xVec3 vilhed; // r29+0x130
        class _xVec3 vilctr; // r29+0x120
        signed int idx; // r22
        signed int do_emit; // r16
        signed int i; // r23
        class st_zEntVillain_ParEmitRecord *vpar; // r21
        class _zParEmitterCustomSettings pecust; // r29+0xc0
    }
}

// 0x00147BA0 - 0x00147DD8 (0x238) VPAR_readyize_emitters__Fv
static void VPAR_readyize_emitters() {
    { // 0x00147BA0 - 0x00147DD8
        enum en_zEntVillainSubTypeID viltype; // r20
        class st_zEntVillain_ParEmitInfo *vinf; // r19
        class xBase *pebase; // r2
        signed int flg_omit; // r18
        signed int flg_need; // r17
        unsigned int emitid; // r16
        enum en_VPAR_EMITTER_ROLE emitrole; // r22
    }
}

// 0x00147DE0 - 0x00147EE4 (0x104) VEFX_zapper_type_B__FP6_xVec3P6_xVec3f
static class _tagLightning *VEFX_zapper_type_B(class _xVec3 *from, class _xVec3 *to, float dur) {
    { // 0x00147DE0 - 0x00147EE4
        class _tagLightningAdd add; // r29+0x20
        class _tagLightning *bolt; // r2
    }
}

// 0x00147EF0 - 0x00148158 (0x268) VEFX_zapTheKnight__FP14st_zEntVillaini
static void VEFX_zapTheKnight(class st_zEntVillain *vil, signed int cleanup) {
    { // 0x00147EF0 - 0x00148158
        class _xVec3 loca; // r29+0xd0
        signed int i; // r17
        class _tagLightningAdd add; // r29+0x90
    }
}

// 0x00148160 - 0x00148370 (0x210) VEFX_mm_zapper__FP14st_zEntVillaini
static void VEFX_mm_zapper(class st_zEntVillain *vil, signed int cleanup) {
    { // 0x00148160 - 0x00148370
        class _tagLightningAdd add; // r29+0x90
        signed int i; // r19
    }
}

// 0x00148370 - 0x001485CC (0x25C) VEFX_plop_gum__FP14st_zEntVillain
static void VEFX_plop_gum(class st_zEntVillain *ent) {
    { // 0x00148370 - 0x001485CC
        class RwTexture *tex; // r2
        class _xMat3x3 dir_shoot; // r29+0x50
        class _xVec3 pos_vert; // r29+0xb0
        class _xVec3 npcpos; // r29+0xa0
        signed int i; // r16
        class _xVec3 to; // r29+0x90
        class _xVec3 from; // r29+0x80
    }
}

// 0x001485D0 - 0x00148900 (0x330) SuperVillainGumDraw__FP6_xVec3P6_xVec3ff
static void SuperVillainGumDraw(class _xVec3 *from, class _xVec3 *to, float top_radius, float bot_radius) {
    { // 0x001485D0 - 0x00148900
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class _xVec3 offset; // r29+0xe0
        class _xMat3x3 mat; // r29+0xa0
        signed int i; // r17
        class RxObjSpace3DVertex *vert; // r16
        class _xVec3 vec_in; // r29+0xd0
        static class RxObjSpace3DVertex sStripVert[32]; // 0x0036CD70
    }
}

// 0x00148900 - 0x00148C1C (0x31C) VWEP_rbs_chkCollide__FP17st_REDBEARD_SWORDf
static void VWEP_rbs_chkCollide(class st_REDBEARD_SWORD *rbs, float dt) {
    { // 0x00148900 - 0x00148C1C
        signed int i; // r17
        class _xEnt *tgt; // r16
        class _xCollis colrec; // r29+0x70
    }
}

// 0x00148C20 - 0x00148DD0 (0x1B0) VWEP_rbs_flight__FP17st_REDBEARD_SWORDf
static void VWEP_rbs_flight(class st_REDBEARD_SWORD *rbs, float dt) {
    { // 0x00148C20 - 0x00148DD0
        class _xVec3 vec_tmp; // r29+0x80
        class _xVec3 vec_rot; // r29+0x70
        class _xMat3x3 mat_rot; // r29+0x40
    }
}

// 0x00148DD0 - 0x00149014 (0x244) VWEP_rbs_throw__FP17st_REDBEARD_SWORD
static void VWEP_rbs_throw(class st_REDBEARD_SWORD *rbs) {
    { // 0x00148DD0 - 0x00149014
        signed int rc; // r2
    }
}

// 0x00149020 - 0x00149050 (0x30) VWEP_rbs_bupdate__FP5_xEntP6_xVec3
static void VWEP_rbs_bupdate(class _xEnt *ent_rbs, class _xVec3 *pos) {
    { // 0x00149020 - 0x00149050
        class st_REDBEARD_SWORD *rbs; // r2
    }
}

// 0x00149050 - 0x001490A0 (0x50) VWEP_rbs_animmodel__FP14st_zEntVillain
static class xModelInstance *VWEP_rbs_animmodel(class st_zEntVillain *vil) {
    { // 0x00149050 - 0x001490A0
        signed int idx; // r5
        class xModelInstance *minst; // r4
        class xModelInstance *da_minst; // r2
    }
}

// 0x001490A0 - 0x0014939C (0x2FC) VWEP_rbs_setup__FP17st_REDBEARD_SWORDP14st_zEntVillain
static void VWEP_rbs_setup(class st_REDBEARD_SWORD *rbs, class st_zEntVillain *rbvil) {
    { // 0x001490A0 - 0x0014939C
        unsigned int aid; // r2
        signed int i; // r17
        class _xVec3 vec_pos; // r29+0x90
    }
}

// 0x001493A0 - 0x001494B4 (0x114) VPRJ_BMM_blackflask__FP14zEntProjectilef
static void VPRJ_BMM_blackflask(class zEntProjectile *prj, float dt) {
    { // 0x001493A0 - 0x001494B4
        class _xCollis colrec; // r29+0x30
        float rad_oldbnd; // 
        class st_ZVMM_FLASK_DATA *flask; // r10
    }
}

// 0x001494C0 - 0x001495D4 (0x114) VPRJ_BMM_greenflask__FP14zEntProjectilef
static void VPRJ_BMM_greenflask(class zEntProjectile *prj, float dt) {
    { // 0x001494C0 - 0x001495D4
        class _xCollis colrec; // r29+0x30
        float rad_oldbnd; // 
        class st_ZVMM_FLASK_DATA *flask; // r10
    }
}

// 0x001495E0 - 0x001496F4 (0x114) VPRJ_BMM_redflask__FP14zEntProjectilef
static void VPRJ_BMM_redflask(class zEntProjectile *prj, float dt) {
    { // 0x001495E0 - 0x001496F4
        class _xCollis colrec; // r29+0x30
        float rad_oldbnd; // 
        class st_ZVMM_FLASK_DATA *flask; // r10
    }
}

// 0x00149700 - 0x00149794 (0x94) VPRJ_BMM_select_flasktype__FP14st_zEntVillain
static enum en_ZVMM_FLASK_TYPE VPRJ_BMM_select_flasktype(class st_zEntVillain *vil) {
    { // 0x00149700 - 0x00149794
        signed int typlist[3]; // r29+0x10
        enum en_ZVMM_FLASK_TYPE ftype; // r2
    }
}

// 0x001497A0 - 0x001499AC (0x20C) VPRJ_BMM_notify__FP14zEntProjectile19en_PROJECTILE_EVENTPv
static signed int VPRJ_BMM_notify(class zEntProjectile *prj, enum en_PROJECTILE_EVENT event) {
    { // 0x001497A0 - 0x001499AC
        unsigned int aid_snd; // r2
        class st_zEntVillain *vil; // r16
    }
}

// 0x001499B0 - 0x00149E2C (0x47C) VPRJ_BMM_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int VPRJ_BMM_logix(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x001499B0 - 0x00149E2C
        class ST_CAT_AIMDATA aiminf; // r29+0x70
        class st_zEntVillain *vil; // r18
        class st_ZVMM_FLASK_DATA *flask; // r17
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r16
    }
}

// 0x00149E30 - 0x00149E68 (0x38) VPRJ_BMM_detach__FP14zEntProjectile
static void VPRJ_BMM_detach(class zEntProjectile *prj) {
    { // 0x00149E30 - 0x00149E68
        class st_ZVMM_FLASK_DATA *flask; // r2
    }
}

// 0x00149E70 - 0x00149F10 (0xA0) VPRJ_BMM_attach__FP14zEntProjectile
static signed int VPRJ_BMM_attach(class zEntProjectile *prj) {
    { // 0x00149E70 - 0x00149F10
        signed int i; // r3
    }
}

// 0x00149F10 - 0x00149F38 (0x28) VPRJ_BMM_bupdate__FP5_xEntP6_xVec3
static void VPRJ_BMM_bupdate(class _xEnt *prjent, class _xVec3 *pos) {
    { // 0x00149F10 - 0x00149F38
        class zEntProjectile *prj; // r2
    }
}

// 0x00149F40 - 0x00149F48 (0x8) VPRJ_BRB_notify__FP14zEntProjectile19en_PROJECTILE_EVENTPv
static signed int VPRJ_BRB_notify() {
    { // 0x00149F40 - 0x00149F48
    }
}

// 0x00149F50 - 0x0014A270 (0x320) VPRJ_BRB_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int VPRJ_BRB_logix(class zEntProjectile *prj, float dt) {
    { // 0x00149F50 - 0x0014A270
        class st_ZPRJ_CAT_ARROW *arrow; // r16
    }
}

// 0x0014A270 - 0x0014A2A8 (0x38) VPRJ_BRB_detach__FP14zEntProjectile
static void VPRJ_BRB_detach(class zEntProjectile *prj) {
    { // 0x0014A270 - 0x0014A2A8
        class st_REDBEARD_SWORD *sword; // r2
    }
}

// 0x0014A2B0 - 0x0014A338 (0x88) VPRJ_BRB_attach__FP14zEntProjectile
static signed int VPRJ_BRB_attach(class zEntProjectile *prj) {
    { // 0x0014A2B0 - 0x0014A338
        signed int i; // r3
    }
}

// 0x0014A340 - 0x0014A5C0 (0x280) VPRJ_BGG_shockwave__FP14zEntProjectilef
static signed int VPRJ_BGG_shockwave(class zEntProjectile *prj, float dt) {
    { // 0x0014A340 - 0x0014A5C0
        float br; // 
        float fb; // 
        class _xVec3 delta; // r29+0xe0
        class _xVec3 pos_plyr; // r29+0xd0
        class _xVec3 pos_prj; // r29+0xc0
        float outer; // 
        float inner; // 
        class st_BGG_QUAKEBALL *quake; // r16
        static class _xVec3 prj_pos; // 0x0036CB10
    }
}

// 0x0014A5C0 - 0x0014A7CC (0x20C) VPRJ_BGG_notify__FP14zEntProjectile19en_PROJECTILE_EVENTPv
static signed int VPRJ_BGG_notify(class zEntProjectile *prj, enum en_PROJECTILE_EVENT event) {
    { // 0x0014A5C0 - 0x0014A7CC
        unsigned int aid_snd; // r2
        class st_zEntVillain *vil; // r16
    }
}

// 0x0014A7D0 - 0x0014AF00 (0x730) VPRJ_BGG_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int VPRJ_BGG_logix(class zEntProjectile *prj, float dt) {
    { // 0x0014A7D0 - 0x0014AF00
        signed int start_fall; // r18
        float dist; // r29+0x2b0
        class _xVec3 dir_delt; // r29+0x2a0
        class _xVec3 vec_delt; // r29+0x290
        class _xVec3 pos_ryht; // r29+0x280
        class _xVec3 pos_left; // r29+0x270
        class _xVec3 vec_tmp; // r29+0x260
        class st_zEntVillain *vil; // r18
        class st_BGG_QUAKEBALL *quake; // r17
    }
}

// 0x0014AF00 - 0x0014AF38 (0x38) VPRJ_BGG_detach__FP14zEntProjectile
static void VPRJ_BGG_detach(class zEntProjectile *prj) {
    { // 0x0014AF00 - 0x0014AF38
        class st_BGG_QUAKEBALL *quake; // r2
    }
}

// 0x0014AF40 - 0x0014AFE8 (0xA8) VPRJ_BGG_attach__FP14zEntProjectile
static signed int VPRJ_BGG_attach(class zEntProjectile *prj) {
    { // 0x0014AF40 - 0x0014AFE8
        signed int i; // r6
    }
}

// 0x0014AFF0 - 0x0014B304 (0x314) VPRJ_zapTheAxe__FP14zEntProjectilei
static void VPRJ_zapTheAxe(class zEntProjectile *prj, signed int cleanup) {
    { // 0x0014AFF0 - 0x0014B304
        class _xVec3 from; // r29+0x110
        class _xVec3 loca; // r29+0x100
        signed int i; // r18
        class st_BKAXE_DATA *axfx; // r16
    }
}

// 0x0014B310 - 0x0014B578 (0x268) VPRJ_BBK_notify__FP14zEntProjectile19en_PROJECTILE_EVENTPv
static signed int VPRJ_BBK_notify(class zEntProjectile *prj, enum en_PROJECTILE_EVENT event) {
    { // 0x0014B310 - 0x0014B578
        unsigned int aid_snd; // r2
        class st_zEntVillain *vil; // r16
    }
}

// 0x0014B580 - 0x0014B690 (0x110) VPRJ_BBK_prjmove__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int VPRJ_BBK_prjmove(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x0014B580 - 0x0014B690
        class st_BKAXE_DATA *axfx; // r16
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r5
    }
}

// 0x0014B690 - 0x0014B924 (0x294) VPRJ_BBK_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int VPRJ_BBK_logix(class zEntProjectile *prj, float dt) {
    { // 0x0014B690 - 0x0014B924
        class _xVec3 pos_prj; // r29+0x60
        class st_BKAXE_DATA *axfx; // r17
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r16
    }
}

// 0x0014B930 - 0x0014B97C (0x4C) VPRJ_BBK_detach__FP14zEntProjectile
static void VPRJ_BBK_detach(class zEntProjectile *prj) {
    { // 0x0014B930 - 0x0014B97C
        class st_BKAXE_DATA *axfx; // r16
    }
}

// 0x0014B980 - 0x0014BA5C (0xDC) VPRJ_BBK_attach__FP14zEntProjectile
static signed int VPRJ_BBK_attach(class zEntProjectile *prj) {
    { // 0x0014B980 - 0x0014BA5C
        signed int i; // r3
    }
}

// 0x0014BA60 - 0x0014BA88 (0x28) VPRJ_BBK_bupdate__FP5_xEntP6_xVec3
static void VPRJ_BBK_bupdate(class _xEnt *prjent, class _xVec3 *pos) {
    { // 0x0014BA60 - 0x0014BA88
        class zEntProjectile *prj; // r2
    }
}

// 0x0014BA90 - 0x0014BC00 (0x170) VPRJ_startup__Fv
static void VPRJ_startup() {
    { // 0x0014BA90 - 0x0014BC00
        class st_ZPRJ_CATEGORY_PROPS prop_flask; // r29+0x100
        class st_ZPRJ_CATEGORY_PROPS prop_red; // r29+0xf0
        class st_ZPRJ_CATEGORY_PROPS prop_green; // r29+0xe0
        class st_ZPRJ_CATEGORY_PROPS prop_black; // r29+0xd0
        class st_ZPRJ_CATEGORY_FUNCS fmap_flask; // r29+0xa0
        class st_ZPRJ_CATEGORY_FUNCS fmap_red; // r29+0x70
        class st_ZPRJ_CATEGORY_FUNCS fmap_green; // r29+0x40
        class st_ZPRJ_CATEGORY_FUNCS fmap_black; // r29+0x10
    }
}

// 0x0014BC00 - 0x0014BDB4 (0x1B4) VWEP_lobInitMMFlask__FP14st_zEntVillainP6_xVec3P6_xVec3P6_xVec3ff
static signed int VWEP_lobInitMMFlask(class st_zEntVillain *vil, class _xVec3 *vec_spin, float arcness, float heavy) {
    { // 0x0014BC00 - 0x0014BDB4
        class _xMat3x3 mat_rot; // r29+0x60
        class st_ZVMM_FLASK_DATA *flask; // r17
        class zEntProjectile *prj; // r16
        static class _xVec3 vec_initwobble; // 0x00297558
    }
}

// 0x0014BDC0 - 0x0014BFE0 (0x220) VWEP_lobInitRBSword__FP14st_zEntVillainP6_xVec3P6_xVec3P6_xVec3ff
static signed int VWEP_lobInitRBSword(class st_zEntVillain *vil) {
    { // 0x0014BDC0 - 0x0014BFE0
        signed int rc; // r2
        class _xVec3 tmp_vec; // r29+0x40
        class st_REDBEARD_SWORD *sword; // r17
        class zEntProjectile *prj; // r16
    }
}

// 0x0014BFE0 - 0x0014C098 (0xB8) VWEP_lobInitGGQuaker__FP14st_zEntVillainP6_xVec3P6_xVec3P6_xVec3ff
static signed int VWEP_lobInitGGQuaker(class st_zEntVillain *vil, class _xVec3 *pos_tgt, class _xVec3 *vec_off, class _xVec3 *vec_spin, float arcness, float heavy) {
    { // 0x0014BFE0 - 0x0014C098
        class st_BGG_QUAKEBALL *quake; // r17
        class zEntProjectile *prj; // r16
    }
}

// 0x0014C0A0 - 0x0014C294 (0x1F4) VWEP_lobInitBKAxe__FP14st_zEntVillainP6_xVec3P6_xVec3P6_xVec3ff
static signed int VWEP_lobInitBKAxe(class st_zEntVillain *vil, class _xVec3 *pos_tgt, class _xVec3 *vec_off, class _xVec3 *vec_spin, float arcness, float heavy) {
    { // 0x0014C0A0 - 0x0014C294
        class _xVec3 ang; // r29+0xb0
        class _xVec3 vec_tmp; // r29+0xa0
        class _xMat3x3 mat_rot; // r29+0x60
        class _xVec3 vec_initrot; // r29+0x90
        class st_BKAXE_DATA *axfx; // r17
        class zEntProjectile *prj; // r16
    }
}

// 0x0014C2A0 - 0x0014C69C (0x3FC) VWEP_lobInitArcing__FP14st_zEntVillainP6_xVec3P6_xVec3P6_xVec3ff
static signed int VWEP_lobInitArcing(class st_zEntVillain *vil, class _xVec3 *pos_tgt, class _xVec3 *vec_off, class _xVec3 *vec_spin, float arcness, float heavy) {
    { // 0x0014C2A0 - 0x0014C69C
        class _xVec3 tmp_vec; // r29+0xd0
        signed int rc; // r2
        class ST_CAT_AIMDATA aiminf; // r29+0x80
        class _xVec3 pos_launch; // r29+0xc0
        class zEntProjectile *prj; // r17
        signed int result; // r16
    }
}

// 0x0014C6A0 - 0x0014C8E8 (0x248) VWEP_lobInitArrow__FP14st_zEntVillainP6_xVec3P6_xVec3P6_xVec3ff
static signed int VWEP_lobInitArrow(class st_zEntVillain *vil, class _xVec3 *pos_tgt, class _xVec3 *vec_off) {
    { // 0x0014C6A0 - 0x0014C8E8
        class _xVec3 tmp_vec; // r29+0xa0
        signed int rc; // r2
        class ST_CAT_AIMDATA aiminf; // r29+0x50
        class _xVec3 pos_launch; // r29+0x90
        class zEntProjectile *prj; // r16
    }
}

// 0x0014C8F0 - 0x0014CB48 (0x258) VWEP_prjOwnerNotify__FP14zEntProjectile19en_PROJECTILE_EVENTPv
static signed int VWEP_prjOwnerNotify(class zEntProjectile *prj, enum en_PROJECTILE_EVENT event) {
    { // 0x0014C8F0 - 0x0014CB48
        signed int do_sounds; // r17
        unsigned int aid_snd; // r2
        class st_zEntVillain *vil; // r16
        signed int handled; // r2
    }
}

// 0x0014CB50 - 0x0014CD08 (0x1B8) VWEP_lobActivatePRJ__FP14zEntProjectileP14st_zEntVillainP6_xVec3P6_xVec3P6_xVec3ff
static signed int VWEP_lobActivatePRJ(class zEntProjectile *prj, class st_zEntVillain *vil, class _xVec3 *pos_tgt, class _xVec3 *vec_off, class _xVec3 *vec_spin, float arcness, float heavy) {
    { // 0x0014CB50 - 0x0014CD08
        signed int result; // r2
    }
}

// 0x0014CD10 - 0x0014CE3C (0x12C) VWEP_lobReservePRJ__FP14st_zEntVillain
static class zEntProjectile *VWEP_lobReservePRJ(class st_zEntVillain *vil) {
    { // 0x0014CD10 - 0x0014CE3C
        enum en_ZVMM_FLASK_TYPE flask_type; // r16
        enum en_ZPRJ_CATEGORY cat; // r18
        class zEntProjectile *prj; // r2
    }
}

// 0x0014CE40 - 0x0014D3B4 (0x574) VWEP_extend_particles__FP14st_zEntVillainfP6_xVec3
static signed int VWEP_extend_particles(class st_zEntVillain *ent, float dt, class _xVec3 *pos_tgt) {
    { // 0x0014CE40 - 0x0014D3B4
        float conrat; // 
        float dist; // 
        float tym_elap; // r29+0x140
        float ang_updwn; // r29+0x140
        class _xVec3 dltvec; // r29+0x120
        class _xMat3x3 mat_rot; // r29+0x50
        class _xVec3 tmpvec; // r29+0x110
        class _xVec3 weppos; // r29+0x100
        class _xVec3 shotpos; // r29+0xf0
        class _xVec3 dirvec; // r29+0xe0
    }
}

// 0x0014D3C0 - 0x0014D618 (0x258) VWEP_swipe__FP14st_zEntVillainf
static signed int VWEP_swipe(class st_zEntVillain *vil) {
    { // 0x0014D3C0 - 0x0014D618
        class _xBound bnd; // r29+0x90
        class _xCollis colrec; // r29+0x40
        signed int hitplyr; // r16
    }
}

// 0x0014D620 - 0x0014D7B8 (0x198) VGOL_set__FP14st_zEntVillain17en_BEHAVIOUR_GOAL
static enum en_BEHAVIOUR_GOAL VGOL_set(class st_zEntVillain *ent, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0014D620 - 0x0014D7B8
    }
}

// 0x0014D7C0 - 0x0014D880 (0xC0) VSAV_load__FP14st_zEntVillainP7xSerial
static signed int VSAV_load(class st_zEntVillain *ent, class xSerial *xser) {
    { // 0x0014D7C0 - 0x0014D880
        signed int iv; // r29+0x3c
    }
}

// 0x0014D880 - 0x0014D924 (0xA4) VSAV_save__FP14st_zEntVillainP7xSerial
static signed int VSAV_save(class st_zEntVillain *ent, class xSerial *xser) {
    { // 0x0014D880 - 0x0014D924
        enum en_BEHAVIOUR_GOAL gol; // r3
        signed int iv; // r2
    }
}

// 0x0014D930 - 0x0014DCEC (0x3BC) VCFG_getParm__FP24st_ZVIL_CONFIG_REFERENCE21en_NPC_INI_PARAMETERSPv
static signed int VCFG_getParm(class st_ZVIL_CONFIG_REFERENCE *cfgdata, enum en_NPC_INI_PARAMETERS pid, void *vv) {
    { // 0x0014D930 - 0x0014DCEC
        unsigned int aid_ptyp; // r2
        signed int found; // r16
    }
}

// 0x0014DCF0 - 0x0014DD74 (0x84) VCFG_allowFlags__FP24st_ZVIL_CONFIG_REFERENCE
static signed int VCFG_allowFlags(class st_ZVIL_CONFIG_REFERENCE *cfgdata) {
    { // 0x0014DCF0 - 0x0014DD74
        signed int allow; // r2
    }
}

// 0x0014DD80 - 0x0014DFC8 (0x248) VCFG_readAsset__FP24st_ZVIL_CONFIG_REFERENCE
static void VCFG_readAsset(class st_ZVIL_CONFIG_REFERENCE *cfgdata) {
    { // 0x0014DD80 - 0x0014DFC8
        class p2EntVillainAsset *rawass; // r3
    }
}

// 0x0014DFD0 - 0x0014E994 (0x9C4) VCFG_readParms__FP24st_ZVIL_CONFIG_REFERENCE
static void VCFG_readParms(class st_ZVIL_CONFIG_REFERENCE *cfgdata) {
    { // 0x0014DFD0 - 0x0014E994
        signed int iv; // r2
        float fv; // r29+0x90
        class _xVec3 vec_res; // r29+0x80
        class _xVec3 scl_dflt; // r29+0x70
        class _xVec3 vec_tmp; // r29+0x60
        class _xVec3 vec_zero; // r29+0x50
        unsigned int pmsize; // r29+0x8c
        class zAssetModelParam *pmdata; // r17
        signed int i; // r16
    }
}

// 0x0014E9A0 - 0x0014EBB8 (0x218) VIRM_initVilDflt__FP14st_zEntVillain
static void VIRM_initVilDflt(class st_zEntVillain *ent) {
    { // 0x0014E9A0 - 0x0014EBB8
    }
}

// 0x0014EBC0 - 0x0014EC80 (0xC0) VIRM_initVil__FP14st_zEntVillain
static void VIRM_initVil(class st_zEntVillain *ent) {
    { // 0x0014EBC0 - 0x0014EC80
    }
}

// 0x0014EC80 - 0x0014F100 (0x480) zVIL_chk_pitfall__FP14st_zEntVillainif
static signed int zVIL_chk_pitfall(class st_zEntVillain *vil, signed int prev, float rand) {
    { // 0x0014EC80 - 0x0014F100
        class _xVec3 xos; // r29+0x100
        class _xVec3 tgt; // r29+0xf0
        class _xVec3 vilpos; // r29+0xe0
        class _xRay3 ray_los; // r29+0xb0
        float sbr; // 
        signed int ispit; // r2
    }
}

// 0x0014F100 - 0x0014FA58 (0x958) zVIL_apply_move__FP14st_zEntVillainP7_xScenefP10_xEntFrame
static void zVIL_apply_move(class st_zEntVillain *ent, float dt, class _xEntFrame *frame) {
    { // 0x0014F100 - 0x0014FA58
        float slope; // r29+0xc0
        float ds2_innersnap; // r29+0xc0
        float ds2_outersnap; // r29+0xc0
        float distSqr; // 
        float sin_ang; // r29+0xc0
        float cos_ang; // 
        signed int slowmove; // r19
        signed int apply_veloc; // r18
        signed int apply_move; // r17
        signed int apply_rot; // r16
        float ang_travel; // 
        float ang_diff; // r29+0xc0
        class _xVec3 npcpos; // r29+0xb0
        float hrate; // 
        float rate; // 
        float dy; // 
    }
}

// 0x0014FA60 - 0x0015047C (0xA1C) zVIL_precalc__FP14st_zEntVillaini
static void zVIL_precalc(class st_zEntVillain *vil, signed int flg_needinfo) {
    { // 0x0014FA60 - 0x0015047C
        float tmp_detect_radiusSqr; // r29+0x190
        float low_detect; // 
        float upr_detect; // 
        class _xVec3 vec_tmp; // r29+0x100
        class _xVec3 ctr_plyr; // r29+0xf0
        class _xVec3 pos_plyr; // r29+0xe0
        class _xVec3 ctr_vil; // r29+0xd0
        class _xVec3 pos_vil; // r29+0xc0
        class st_ZVIL_PRECALC *prec; // r16
    }
}

// 0x00150480 - 0x00150C20 (0x7A0) zVIL_upd_timers__FP14st_zEntVillainf
static void zVIL_upd_timers(class st_zEntVillain *ent, float dt) {
    { // 0x00150480 - 0x00150C20
    }
}

// 0x00150C20 - 0x00150EB4 (0x294) zVIL_apply_vilcfg__FP14st_zEntVillainP24st_ZVIL_CONFIG_REFERENCE
static void zVIL_apply_vilcfg(class st_zEntVillain *ent, class st_ZVIL_CONFIG_REFERENCE *cfg) {
    { // 0x00150C20 - 0x00150EB4
        class st_ZVIL_NPCTYPE_LOOKUP *vilrec; // r2
    }
}

// 0x00150EC0 - 0x00150F9C (0xDC) zVIL_tag_verts__FP14st_zEntVillain
static void zVIL_tag_verts(class st_zEntVillain *ent) {
    { // 0x00150EC0 - 0x00150F9C
        class _xVec3 *tmp_pos; // r2
        signed int i; // r16
    }
}

// 0x00150FA0 - 0x001510B8 (0x118) VNAV_setup_telepoint__FP14st_zEntVillaini
static signed int VNAV_setup_telepoint(class st_zEntVillain *vil, signed int force) {
    { // 0x00150FA0 - 0x001510B8
        signed int do_teleport; // r16
        class _zMovePoint *telepoint; // r2
    }
}

// 0x001510C0 - 0x00151594 (0x4D4) VNAV_find_telepoint__FP14st_zEntVillaini
static class _zMovePoint *VNAV_find_telepoint(class st_zEntVillain *vil, signed int force) {
    { // 0x001510C0 - 0x00151594
        float safe_wt[12]; // r29+0x170
        float dngr_wt[4]; // r29+0x160
        signed int safelist[8]; // r29+0x140
        signed int dngrlist[4]; // r29+0x130
        signed int choice; // r2
        class _xVec3 vec_b; // r29+0x1b0
        class _xVec3 vec_a; // r29+0x1a0
        float ds2_toSelf[12]; // r29+0x100
        float ds2_toScoob[12]; // r29+0xd0
        signed int i; // r30
        signed int idx_best; // r16
        class _zMovePoint *da_tp; // r29+0xa0
        static signed int do_rand_select; // 0x00357F08
        static signed char init; // 0x00357F0C
    }
}

// 0x001515A0 - 0x00151A8C (0x4EC) VNAV_get_movespeed__FP14st_zEntVillainf
static float VNAV_get_movespeed(class st_zEntVillain *ent, float base) {
    { // 0x001515A0 - 0x00151A8C
        float fv; // r29+0x60
        float rate; // 
    }
}

// 0x00151A90 - 0x00151BD8 (0x148) VNAV_chklos_timz__FPvP6_xVec3PC11_zMovePoint
static signed int VNAV_chklos_timz(void *usrdata, class _xVec3 *mypos, class _zMovePoint *mvpt) {
    { // 0x00151A90 - 0x00151BD8
        class _xRay3 r; // r29+0x60
        signed int result; // r2
    }
}

// 0x00151BE0 - 0x00151E04 (0x224) VNAV_get_nav__FP14st_zEntVillain
static signed int VNAV_get_nav(class st_zEntVillain *ent) {
    { // 0x00151BE0 - 0x00151E04
        class _xVec3 vec_trav; // r29+0x30
        class _zMovePoint *nav_tmp; // r29+0x3c
    }
}

// 0x00151E10 - 0x00151E90 (0x80) VTAL_cntKillable__Fv
static void VTAL_cntKillable() {
    { // 0x00151E10 - 0x00151E90
        signed int i; // r5
    }
}

// 0x00151E90 - 0x00151F68 (0xD8) VOUT_spikey_header__FP14st_zEntVillain
static enum en_ZVILLAIN_HEADBUTT_RESPONSE VOUT_spikey_header(class st_zEntVillain *ent) {
    { // 0x00151E90 - 0x00151F68
        enum en_NPC_QUADRANT quad; // r29+0x3c
        enum en_ZVILLAIN_HEADBUTT_RESPONSE result; // r16
    }
}

// 0x00151F70 - 0x001520C4 (0x154) VOUT_apply_anchor__FP14st_zEntVillain
static signed int VOUT_apply_anchor(class st_zEntVillain *vil) {
    { // 0x00151F70 - 0x001520C4
        signed int have_anchor; // r16
    }
}

// 0x001520D0 - 0x00152274 (0x1A4) VOUT_hit_isWall__FP14st_zEntVillainP6_xVec3P6_xVec3
static signed int VOUT_hit_isWall(class st_zEntVillain *vil, class _xVec3 *dir_trav, class _xVec3 *vec_fromwall) {
    { // 0x001520D0 - 0x00152274
        class _xVec3 vec_tmp; // r29+0x60
        signed int flg_look; // r17
        signed int result; // r16
    }
}

// 0x00152280 - 0x0015244C (0x1CC) VOUT_dogoto_widget__FP14st_zEntVillainP5xBasefi
static signed int VOUT_dogoto_widget(class st_zEntVillain *vil, class xBase *widge, float dst_offset, signed int follow) {
    { // 0x00152280 - 0x0015244C
        signed int do_std_nav; // r16
    }
}

// 0x00152450 - 0x00152598 (0x148) VOUT_chase_move__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VOUT_chase_move(class st_zEntVillain *vil) {
    { // 0x00152450 - 0x00152598
    }
}

// 0x001525A0 - 0x001527DC (0x23C) VOUT_chkdo_followArrived__FP14st_zEntVillain
static signed int VOUT_chkdo_followArrived(class st_zEntVillain *vil) {
    { // 0x001525A0 - 0x001527DC
        class _xVec3 entpos; // r29+0x40
        float dy; // r29+0x70
        float dist; // r29+0x70
    }
}

// 0x001527E0 - 0x001529DC (0x1FC) VOUT_follow_move__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VOUT_follow_move(class st_zEntVillain *vil) {
    { // 0x001527E0 - 0x001529DC
    }
}

// 0x001529E0 - 0x00152BA4 (0x1C4) VOUT_chkdo_minipatrolArrived__FP14st_zEntVillain
static signed int VOUT_chkdo_minipatrolArrived(class st_zEntVillain *vil) {
    { // 0x001529E0 - 0x00152BA4
        class _xVec3 entpos; // r29+0x40
        float dy; // r29+0x70
        signed int amthere; // r2
    }
}

// 0x00152BB0 - 0x00152D84 (0x1D4) VOUT_chkdo_patrolArrived__FP14st_zEntVillain
static signed int VOUT_chkdo_patrolArrived(class st_zEntVillain *vil) {
    { // 0x00152BB0 - 0x00152D84
        class _xVec3 entpos; // r29+0x40
        float dy; // r29+0x70
        float dist; // r29+0x70
    }
}

// 0x00152D90 - 0x00152F14 (0x184) VOUT_patrol_move__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VOUT_patrol_move(class st_zEntVillain *vil) {
    { // 0x00152D90 - 0x00152F14
    }
}

// 0x00152F20 - 0x00152FE4 (0xC4) zVIL_cond_spawn_ok__FP14st_zEntVillain
static signed int zVIL_cond_spawn_ok(class st_zEntVillain *vil) {
    { // 0x00152F20 - 0x00152FE4
        signed int vil_minhardness; // r29+0x4c
        signed int msk_condspawn; // r29+0x48
        signed int result; // r16
    }
}

// 0x00152FF0 - 0x00153130 (0x140) zVIL_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int zVIL_trangoal(class st_zEntVillain *ent, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x00152FF0 - 0x00153130
        class st_STIMULUS_CONTEXT_DATA tranctxt; // r29+0x50
        signed int rc; // r2
        signed int result; // r16
        static signed int trangoal_safety_nest; // 0x00357F00
        static signed char init; // 0x00357F04
    }
}

// 0x00153130 - 0x00153278 (0x148) zVIL_input_stim__FP14st_zEntVillain15en_NPC_STIMULUSP24st_STIMULUS_CONTEXT_DATA
static void zVIL_input_stim(class st_zEntVillain *ent, class st_STIMULUS_CONTEXT_DATA *stimctxt) {
    { // 0x00153130 - 0x00153278
        class st_STIMULUS_CONTEXT_DATA stimresp; // r29+0x20
    }
}

// 0x00153310 - 0x0015337C (0x6C) zVIL_elcb_move__FP5_xEntP7_xScenefP10_xEntFrame
static void zVIL_elcb_move(class _xEnt *xe, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x00153310 - 0x0015337C
        class st_zEntVillain *ent; // r16
    }
}

// 0x00153380 - 0x0015352C (0x1AC) zVIL_elcb_bupdate__FP5_xEntP6_xVec3
static void zVIL_elcb_bupdate(class _xEnt *xe, class _xVec3 *pos_bound) {
    { // 0x00153380 - 0x0015352C
        class _xVec3 offset; // r29+0x60
        class st_zEntVillain *ent; // r16
    }
}

// 0x00153530 - 0x0015382C (0x2FC) zVIL_elcb_update__FP5_xEntP7_xScenef
static void zVIL_elcb_update(class _xEnt *xe, class _xScene *scn, float dt) {
    { // 0x00153530 - 0x0015382C
        class st_STIMULUS_CONTEXT_DATA stimresp; // r29+0x40
        class st_zEntVillain *ent; // r16
    }
}

// 0x00153830 - 0x00153938 (0x108) zVIL_elcb_event__FP5xBaseP5xBaseUiPfP5xBase
static signed int zVIL_elcb_event(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x00153830 - 0x00153938
        class st_STIMULUS_CONTEXT_DATA npcresp; // r29+0x20
        class st_zEntVillain *ent; // r16
    }
}

// 0x00153940 - 0x00153FB4 (0x674) VILN_BMM_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_BMM_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x00153940 - 0x00153FB4
        signed int battle_mode; // r2
    }
}

// 0x00153FC0 - 0x00154140 (0x180) VILN_BMM_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BMM_respond(class st_zEntVillain *vil) {
    { // 0x00153FC0 - 0x00154140
        class _xVec3 vec_sum; // r29+0x0
        class _xCollis *colrec; // r5
        signed int i; // r4
    }
}

// 0x00154140 - 0x00154500 (0x3C0) VILN_BMM_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_BMM_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x00154140 - 0x00154500
        float fv; // r29+0x30
    }
}

// 0x00154500 - 0x001554A4 (0xFA4) VILN_BMM_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_BMM_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x00154500 - 0x001554A4
        enum en_BEHAVIOUR_GOAL procgoal; // r19
        enum en_NPC_STIMULUS stim; // r18
        signed int result; // r17
    }
}

// 0x001554B0 - 0x00155CA4 (0x7F4) VILN_BMM_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BMM_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x001554B0 - 0x00155CA4
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r16
    }
}

// 0x00155CB0 - 0x00156B08 (0xE58) VILN_BMM_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BMM_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00155CB0 - 0x00156B08
        signed int rc; // r2
        signed int rc; // r2
        signed int rc; // r2
        class _xVec3 vec_tmp; // r29+0x58
        signed int rc; // r2
        signed int rc; // r2
        signed int rc; // r2
        signed int rc; // r2
        class st_ZVIL_PRECALC *prec; // r16
    }
}

// 0x00156B10 - 0x00156D28 (0x218) VILN_BMM_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BMM_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x00156B10 - 0x00156D28
        signed int i; // r20
        signed int idx; // r19
        class xModelInstance *minst; // r18
    }
}

// 0x00156D30 - 0x00156DBC (0x8C) VILN_BMM_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BMM_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00156D30 - 0x00156DBC
        signed int result; // r2
    }
}

// 0x00156DC0 - 0x00156E4C (0x8C) VILN_BMM_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_BMM_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x00156DC0 - 0x00156E4C
        class st_ZVIL_PRECALC *prec; // r3
        signed int result; // r2
    }
}

// 0x00156E50 - 0x00156E58 (0x8) VILN_BMM_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_BMM_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x00156E50 - 0x00156E58
    }
}

// 0x00156E60 - 0x00156E98 (0x38) VILN_BRB_xtrarend__FP14st_zEntVillain
static signed int VILN_BRB_xtrarend() {
    { // 0x00156E60 - 0x00156E98
        class st_REDBEARD_SWORD *rbs; // r2
    }
}

// 0x00156EA0 - 0x00157280 (0x3E0) VILN_BRB_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_BRB_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x00156EA0 - 0x00157280
    }
}

// 0x00157280 - 0x00157288 (0x8) VILN_BRB_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BRB_respond() {
    { // 0x00157280 - 0x00157288
    }
}

// 0x00157290 - 0x00157448 (0x1B8) VILN_BRB_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_BRB_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x00157290 - 0x00157448
        class _xVec3 pos_hardwired; // r29+0x20
    }
}

// 0x00157450 - 0x001579C4 (0x574) VILN_BRB_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_BRB_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x00157450 - 0x001579C4
        enum en_BEHAVIOUR_GOAL procgoal; // r18
        enum en_NPC_STIMULUS stim; // r17
        signed int result; // r16
    }
}

// 0x001579D0 - 0x00157EE0 (0x510) VILN_BRB_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BRB_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x001579D0 - 0x00157EE0
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r6
    }
}

// 0x00157EE0 - 0x0015869C (0x7BC) VILN_BRB_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BRB_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00157EE0 - 0x0015869C
        class st_ZVIL_PRECALC *prec; // r3
    }
}

// 0x001586A0 - 0x001589D0 (0x330) VILN_BRB_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BRB_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x001586A0 - 0x001589D0
        float vx; // 
        signed int goal; // r3
        signed int idx; // r17
        class xModelInstance *minst; // r16
        static class _xVec3 pos_hardwired; // 0x00297468
        static float flame_timer; // 0x00357EF8
        static signed char init; // 0x00357EFC
    }
}

// 0x001589D0 - 0x001589D8 (0x8) VILN_BRB_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BRB_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x001589D0 - 0x001589D8
    }
}

// 0x001589E0 - 0x001589E8 (0x8) VILN_BRB_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_BRB_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x001589E0 - 0x001589E8
    }
}

// 0x001589F0 - 0x001589F8 (0x8) VILN_BRB_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_BRB_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x001589F0 - 0x001589F8
    }
}

// 0x00158A00 - 0x00158DC8 (0x3C8) VILN_BGG_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_BGG_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x00158A00 - 0x00158DC8
    }
}

// 0x00158DD0 - 0x00158DD8 (0x8) VILN_BGG_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BGG_respond() {
    { // 0x00158DD0 - 0x00158DD8
    }
}

// 0x00158DE0 - 0x0015913C (0x35C) VILN_BGG_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_BGG_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x00158DE0 - 0x0015913C
        class _xVec3 vecoff; // r29+0x60
        float dx; // 
        class _xVec3 entpos; // r29+0x50
    }
}

// 0x00159140 - 0x001596AC (0x56C) VILN_BGG_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_BGG_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x00159140 - 0x001596AC
        class _xVec3 delt; // r29+0x60
        enum en_BEHAVIOUR_GOAL procgoal; // r18
        enum en_NPC_STIMULUS stim; // r17
        signed int result; // r16
    }
}

// 0x001596B0 - 0x00159BB4 (0x504) VILN_BGG_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BGG_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x001596B0 - 0x00159BB4
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r6
    }
}

// 0x00159BC0 - 0x0015A59C (0x9DC) VILN_BGG_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BGG_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00159BC0 - 0x0015A59C
        class _xVec3 vec_tmp; // r29+0x78
        class _xVec3 vec_dir; // r29+0x68
        class st_ZVIL_PRECALC *prec; // r16
    }
}

// 0x0015A5A0 - 0x0015A640 (0xA0) VILN_BGG_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BGG_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x0015A5A0 - 0x0015A640
    }
}

// 0x0015A640 - 0x0015A648 (0x8) VILN_BGG_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BGG_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015A640 - 0x0015A648
    }
}

// 0x0015A650 - 0x0015A658 (0x8) VILN_BGG_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_BGG_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x0015A650 - 0x0015A658
    }
}

// 0x0015A660 - 0x0015A668 (0x8) VILN_BGG_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_BGG_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0015A660 - 0x0015A668
    }
}

// 0x0015A670 - 0x0015A738 (0xC8) VILN_BBK_animtran__FP14st_zEntVillainP15xAnimTransitionP11xAnimSingle
static signed int VILN_BBK_animtran(class st_zEntVillain *vil, class xAnimTransition *tran) {
    { // 0x0015A670 - 0x0015A738
        signed int idx; // r6
        class xModelInstance *minst; // r4
    }
}

// 0x0015A740 - 0x0015AAC0 (0x380) VILN_BBK_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_BBK_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x0015A740 - 0x0015AAC0
    }
}

// 0x0015AAC0 - 0x0015AAC8 (0x8) VILN_BBK_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BBK_respond() {
    { // 0x0015AAC0 - 0x0015AAC8
    }
}

// 0x0015AAD0 - 0x0015AC1C (0x14C) VILN_BBK_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_BBK_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x0015AAD0 - 0x0015AC1C
    }
}

// 0x0015AC20 - 0x0015AFF0 (0x3D0) VILN_BBK_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_BBK_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x0015AC20 - 0x0015AFF0
        enum en_BEHAVIOUR_GOAL procgoal; // r18
        enum en_NPC_STIMULUS stim; // r17
        signed int result; // r16
    }
}

// 0x0015AFF0 - 0x0015B4D4 (0x4E4) VILN_BBK_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BBK_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015AFF0 - 0x0015B4D4
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r6
    }
}

// 0x0015B4E0 - 0x0015BE54 (0x974) VILN_BBK_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BBK_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015B4E0 - 0x0015BE54
        class _xVec3 vec_off; // r29+0x78
        class _xVec3 vec_dir; // r29+0x68
        class st_ZVIL_PRECALC *prec; // r16
    }
}

// 0x0015BE60 - 0x0015BF74 (0x114) VILN_BBK_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BBK_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x0015BE60 - 0x0015BF74
        signed int idx; // r7
        class xModelInstance *minst; // r6
    }
}

// 0x0015BF80 - 0x0015C0C0 (0x140) VILN_BBK_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_BBK_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015BF80 - 0x0015C0C0
    }
}

// 0x0015C0C0 - 0x0015C0C8 (0x8) VILN_BBK_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_BBK_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x0015C0C0 - 0x0015C0C8
    }
}

// 0x0015C0D0 - 0x0015C0D8 (0x8) VILN_BBK_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_BBK_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0015C0D0 - 0x0015C0D8
    }
}

// 0x0015C0E0 - 0x0015C25C (0x17C) VILN_sham_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_sham_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x0015C0E0 - 0x0015C25C
    }
}

// 0x0015C260 - 0x0015C268 (0x8) VILN_sham_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_sham_respond() {
    { // 0x0015C260 - 0x0015C268
    }
}

// 0x0015C270 - 0x0015C354 (0xE4) VILN_sham_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_sham_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x0015C270 - 0x0015C354
    }
}

// 0x0015C360 - 0x0015C5E0 (0x280) VILN_sham_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_sham_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x0015C360 - 0x0015C5E0
        enum en_BEHAVIOUR_GOAL procgoal; // r18
        enum en_NPC_STIMULUS stim; // r17
        signed int result; // r16
    }
}

// 0x0015C5E0 - 0x0015CAD4 (0x4F4) VILN_sham_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_sham_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015C5E0 - 0x0015CAD4
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r6
    }
}

// 0x0015CAE0 - 0x0015CEE8 (0x408) VILN_sham_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_sham_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015CAE0 - 0x0015CEE8
    }
}

// 0x0015CEF0 - 0x0015CF54 (0x64) VILN_sham_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_sham_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x0015CEF0 - 0x0015CF54
    }
}

// 0x0015CF60 - 0x0015CF68 (0x8) VILN_sham_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_sham_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015CF60 - 0x0015CF68
    }
}

// 0x0015CF70 - 0x0015CF78 (0x8) VILN_sham_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_sham_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x0015CF70 - 0x0015CF78
    }
}

// 0x0015CF80 - 0x0015CF88 (0x8) VILN_sham_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_sham_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0015CF80 - 0x0015CF88
    }
}

// 0x0015CF90 - 0x0015D044 (0xB4) VILN_gang_animtran__FP14st_zEntVillainP15xAnimTransitionP11xAnimSingle
static signed int VILN_gang_animtran(class st_zEntVillain *vil, class xAnimTransition *tran) {
    { // 0x0015CF90 - 0x0015D044
        signed int idx; // r4
        class xModelInstance *minst; // r6
    }
}

// 0x0015D050 - 0x0015D58C (0x53C) VILN_gang_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_gang_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x0015D050 - 0x0015D58C
    }
}

// 0x0015D590 - 0x0015D598 (0x8) VILN_gang_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_gang_respond() {
    { // 0x0015D590 - 0x0015D598
    }
}

// 0x0015D5A0 - 0x0015D6CC (0x12C) VILN_gang_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_gang_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x0015D5A0 - 0x0015D6CC
    }
}

// 0x0015D6D0 - 0x0015DC18 (0x548) VILN_gang_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_gang_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x0015D6D0 - 0x0015DC18
        enum en_NPC_SOUND use_dis; // r19
        enum en_BEHAVIOUR_GOAL procgoal; // r20
        enum en_NPC_STIMULUS stim; // r19
        signed int result; // r18
    }
}

// 0x0015DC20 - 0x0015E1A0 (0x580) VILN_gang_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_gang_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015DC20 - 0x0015E1A0
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r6
    }
}

// 0x0015E1A0 - 0x0015EA1C (0x87C) VILN_gang_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_gang_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015E1A0 - 0x0015EA1C
        class st_ZVIL_PRECALC *prec; // r16
    }
}

// 0x0015EA20 - 0x0015EBC8 (0x1A8) VILN_gang_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_gang_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x0015EA20 - 0x0015EBC8
        signed int idx; // r17
        class xModelInstance *minst; // r16
        class xAnimState *astate; // r2
    }
}

// 0x0015EBD0 - 0x0015EBD8 (0x8) VILN_gang_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_gang_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015EBD0 - 0x0015EBD8
    }
}

// 0x0015EBE0 - 0x0015EBE8 (0x8) VILN_gang_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_gang_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x0015EBE0 - 0x0015EBE8
    }
}

// 0x0015EBF0 - 0x0015EBF8 (0x8) VILN_gang_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_gang_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0015EBF0 - 0x0015EBF8
    }
}

// 0x0015EC00 - 0x0015EC1C (0x1C) VILN_shagM_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_shagM_pickanim(class st_zEntVillain *vil) {
    { // 0x0015EC00 - 0x0015EC1C
    }
}

// 0x0015EC20 - 0x0015EC28 (0x8) VILN_shagM_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shagM_respond() {
    { // 0x0015EC20 - 0x0015EC28
    }
}

// 0x0015EC30 - 0x0015ED14 (0xE4) VILN_shagM_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_shagM_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x0015EC30 - 0x0015ED14
    }
}

// 0x0015ED20 - 0x0015EF98 (0x278) VILN_shagM_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_shagM_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x0015ED20 - 0x0015EF98
        enum en_BEHAVIOUR_GOAL procgoal; // r18
        enum en_NPC_STIMULUS stim; // r17
        signed int result; // r16
    }
}

// 0x0015EFA0 - 0x0015F370 (0x3D0) VILN_shagM_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shagM_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015EFA0 - 0x0015F370
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r5
    }
}

// 0x0015F370 - 0x0015F754 (0x3E4) VILN_shagM_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shagM_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015F370 - 0x0015F754
    }
}

// 0x0015F760 - 0x0015F7C4 (0x64) VILN_shagM_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shagM_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x0015F760 - 0x0015F7C4
    }
}

// 0x0015F7D0 - 0x0015F7D8 (0x8) VILN_shagM_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shagM_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015F7D0 - 0x0015F7D8
    }
}

// 0x0015F7E0 - 0x0015F7E8 (0x8) VILN_shagM_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_shagM_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x0015F7E0 - 0x0015F7E8
    }
}

// 0x0015F7F0 - 0x0015F7F8 (0x8) VILN_shagM_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_shagM_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0015F7F0 - 0x0015F7F8
    }
}

// 0x0015F800 - 0x0015F808 (0x8) VILN_shag_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_shag_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x0015F800 - 0x0015F808
    }
}

// 0x0015F810 - 0x0015F884 (0x74) VILN_shag_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shag_respond(class st_zEntVillain *vil) {
    { // 0x0015F810 - 0x0015F884
    }
}

// 0x0015F890 - 0x0015F898 (0x8) VILN_shag_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_shag_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x0015F890 - 0x0015F898
    }
}

// 0x0015F8A0 - 0x0015F8A8 (0x8) VILN_shag_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_shag_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x0015F8A0 - 0x0015F8A8
    }
}

// 0x0015F8B0 - 0x0015F8B8 (0x8) VILN_shag_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shag_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015F8B0 - 0x0015F8B8
    }
}

// 0x0015F8C0 - 0x0015F8C8 (0x8) VILN_shag_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shag_aware(class st_zEntVillain *vil, class _xScene *scn, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015F8C0 - 0x0015F8C8
    }
}

// 0x0015F8D0 - 0x0015F934 (0x64) VILN_shag_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shag_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x0015F8D0 - 0x0015F934
    }
}

// 0x0015F940 - 0x0015F948 (0x8) VILN_shag_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shag_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015F940 - 0x0015F948
    }
}

// 0x0015F950 - 0x0015F958 (0x8) VILN_shag_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_shag_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x0015F950 - 0x0015F958
    }
}

// 0x0015F960 - 0x0015F968 (0x8) VILN_shag_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_shag_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0015F960 - 0x0015F968
    }
}

// 0x0015F970 - 0x0015FAFC (0x18C) VILN_shark_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_shark_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x0015F970 - 0x0015FAFC
    }
}

// 0x0015FB00 - 0x0015FB08 (0x8) VILN_shark_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shark_respond() {
    { // 0x0015FB00 - 0x0015FB08
    }
}

// 0x0015FB10 - 0x0015FB18 (0x8) VILN_shark_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_shark_calcmove() {
    { // 0x0015FB10 - 0x0015FB18
    }
}

// 0x0015FB20 - 0x0015FB28 (0x8) VILN_shark_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_shark_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x0015FB20 - 0x0015FB28
    }
}

// 0x0015FB30 - 0x0015FCFC (0x1CC) VILN_shark_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shark_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015FB30 - 0x0015FCFC
        enum en_NPC_STIMULUS stim; // r8
    }
}

// 0x0015FD00 - 0x0015FE90 (0x190) VILN_shark_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shark_aware(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015FD00 - 0x0015FE90
    }
}

// 0x0015FE90 - 0x0015FF24 (0x94) VILN_shark_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shark_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x0015FE90 - 0x0015FF24
    }
}

// 0x0015FF30 - 0x0015FF38 (0x8) VILN_shark_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_shark_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x0015FF30 - 0x0015FF38
    }
}

// 0x0015FF40 - 0x0015FF48 (0x8) VILN_shark_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_shark_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x0015FF40 - 0x0015FF48
    }
}

// 0x0015FF50 - 0x0015FF58 (0x8) VILN_shark_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_shark_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x0015FF50 - 0x0015FF58
    }
}

// 0x0015FF60 - 0x0015FF68 (0x8) VILN_crit_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_crit_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x0015FF60 - 0x0015FF68
    }
}

// 0x0015FF70 - 0x0015FFE4 (0x74) VILN_crit_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_crit_respond(class st_zEntVillain *vil) {
    { // 0x0015FF70 - 0x0015FFE4
    }
}

// 0x0015FFF0 - 0x0015FFF8 (0x8) VILN_crit_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_crit_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x0015FFF0 - 0x0015FFF8
    }
}

// 0x00160000 - 0x00160008 (0x8) VILN_crit_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_crit_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x00160000 - 0x00160008
    }
}

// 0x00160010 - 0x00160018 (0x8) VILN_crit_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_crit_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00160010 - 0x00160018
    }
}

// 0x00160020 - 0x00160028 (0x8) VILN_crit_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_crit_aware(class st_zEntVillain *vil, class _xScene *scn, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00160020 - 0x00160028
    }
}

// 0x00160030 - 0x00160094 (0x64) VILN_crit_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_crit_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x00160030 - 0x00160094
    }
}

// 0x001600A0 - 0x001600A8 (0x8) VILN_crit_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_crit_sysevent(class st_zEntVillain *vil, class xBase *from, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x001600A0 - 0x001600A8
    }
}

// 0x001600B0 - 0x001600B8 (0x8) VILN_crit_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_crit_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x001600B0 - 0x001600B8
    }
}

// 0x001600C0 - 0x001600C8 (0x8) VILN_crit_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_crit_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup, enum en_BEHAVIOUR_GOAL goal) {
    { // 0x001600C0 - 0x001600C8
    }
}

// 0x001600D0 - 0x00160C80 (0xBB0) VILN_kp_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_kp_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x001600D0 - 0x00160C80
        class _xVec3 vec_off; // r29+0x60
        class st_ZVIL_PRECALC *prec; // r5
    }
}

// 0x00160C80 - 0x00160CE8 (0x68) VILN_kp_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_kp_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x00160C80 - 0x00160CE8
        class st_ZVIL_PRECALC *prec; // r3
        signed int result; // r2
    }
}

// 0x00160CF0 - 0x00160D30 (0x40) VILN_KookRender__FP5_xEnt
static void VILN_KookRender(class _xEnt *ent) {
    { // 0x00160CF0 - 0x00160D30
    }
}

// 0x00160D30 - 0x00160D5C (0x2C) VILN_std_xtrarend__FP14st_zEntVillain
static signed int VILN_std_xtrarend(class st_zEntVillain *vil) {
    { // 0x00160D30 - 0x00160D5C
    }
}

// 0x00160D60 - 0x00161790 (0xA30) VILN_std_pickanim__FP14st_zEntVillain17en_BEHAVIOUR_GOAL17en_ZVIL_GOAL_SPOT
static signed int VILN_std_pickanim(class st_zEntVillain *vil, enum en_BEHAVIOUR_GOAL goal, enum en_ZVIL_GOAL_SPOT gspot) {
    { // 0x00160D60 - 0x00161790
    }
}

// 0x00161790 - 0x00161804 (0x74) VILN_std_respond__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_std_respond(class st_zEntVillain *vil) {
    { // 0x00161790 - 0x00161804
        class _xVec3 vec_accum; // r29+0x20
        signed int yep; // r2
    }
}

// 0x00161810 - 0x001621A8 (0x998) VILN_std_calcmove__FP14st_zEntVillainP7_xScenefPC10_xEntFrame
static signed int VILN_std_calcmove(class st_zEntVillain *vil, class _xScene *scn, float dt, class _xEntFrame *frame) {
    { // 0x00161810 - 0x001621A8
        class _zMovePoint *nav_tmp; // r29+0xfc
        class _zMovePoint *nav_old; // r16
        class st_ZVIL_PRECALC *prec; // r16
        float dy; // r29+0x100
        float dist; // r29+0x100
        class _xVec3 entpos; // r29+0x80
    }
}

// 0x001621B0 - 0x00163F78 (0x1DC8) VILN_std_trangoal__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAf
static signed int VILN_std_trangoal(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, float dt) {
    { // 0x001621B0 - 0x00163F78
        class st_ZVIL_NPC_HANDLERFUNCS *myfun; // r3
        float tmp_fv; // r29+0xb0
        class _xVec3 vec_trav; // r29+0x98
        class st_ZVIL_PRECALC *prec; // r17
        enum en_BEHAVIOUR_GOAL procgoal; // r2
        enum en_NPC_STIMULUS stim; // r4
    }
}

// 0x00163F90 - 0x001656D4 (0x1744) VILN_std_stimulus__FP14st_zEntVillainP24st_STIMULUS_CONTEXT_DATAP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_std_stimulus(class st_zEntVillain *vil, class st_STIMULUS_CONTEXT_DATA *stimctxt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00163F90 - 0x001656D4
        class _xVec3 vec_tmp; // r29+0x80
        class _xVec3 vec_tmp; // r29+0x70
        float tmp_fv; // r29+0x90
        enum en_NPC_QUADRANT quad; // r29+0x8c
        signed int std_mvpt_nav; // r2
        enum en_NPC_STIMULUS stim; // r18
        static float fac; // 0x00357EF0
        static signed char init; // 0x00357EF4
    }
}

// 0x001656E0 - 0x0016817C (0x2A9C) VILN_std_aware__FP14st_zEntVillainP7_xScenefP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_std_aware(class st_zEntVillain *vil, float dt, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x001656E0 - 0x0016817C
        class _xVec3 vec_off; // r29+0xe0
        class st_ZVIL_PRECALC *prec; // r18
        float fv; // r29+0x1c0
        signed int iv; // r2
    }
}

// 0x00168180 - 0x00168350 (0x1D0) VPAR_emit_deathfx__FP14st_zEntVillainf
static void VPAR_emit_deathfx(class st_zEntVillain *vil, float dt) {
    { // 0x00168180 - 0x00168350
        unsigned char *col; // r6
        class _zParEmitterCustomSettings info; // r29+0xb0
    }
}

// 0x00168350 - 0x00168470 (0x120) VILN_std_notice__FP14st_zEntVillain14en_ZVIL_NOTICEP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_std_notice(class st_zEntVillain *vil, enum en_ZVIL_NOTICE note) {
    { // 0x00168350 - 0x00168470
    }
}

// 0x00168470 - 0x00168C5C (0x7EC) VILN_std_sysevent__FP14st_zEntVillainP5xBaseUiPfP5xBaseP24st_STIMULUS_CONTEXT_DATA
static signed int VILN_std_sysevent(class st_zEntVillain *vil, unsigned int toEvent, float *toParam, class xBase *toParamWidget, class st_STIMULUS_CONTEXT_DATA *resp) {
    { // 0x00168470 - 0x00168C5C
        unsigned int aid; // r2
    }
}

// 0x00168C60 - 0x00169670 (0xA10) VILN_std_query__FP14st_zEntVillain13en_ZVIL_QUERY
static signed int VILN_std_query(class st_zEntVillain *vil, enum en_ZVIL_QUERY query) {
    { // 0x00168C60 - 0x00169670
        float fv_b; // 
        class st_ZVIL_PRECALC *prec; // r16
        signed int result; // r2
    }
}

// 0x00169670 - 0x00169858 (0x1E8) VILN_std_flag__FP14st_zEntVillain19en_ZVIL_FLAG_GROUPS17en_BEHAVIOUR_GOAL
static signed int VILN_std_flag(class st_zEntVillain *vil, enum en_ZVIL_FLAG_GROUPS flaggroup) {
    { // 0x00169670 - 0x00169858
        signed int result; // r2
    }
}

// 0x00169860 - 0x0016A1D0 (0x970) zVIL_setup_npcTable__Fv
static void zVIL_setup_npcTable() {
    { // 0x00169860 - 0x0016A1D0
        unsigned char flg_chkby; // r14
        unsigned char flg_penby; // r13
        unsigned char flg_pen; // r12
        unsigned char flg_chk; // r11
        unsigned char flg_physics; // r10
        enum en_NPC_PERSONALITIES typ_person; // r9
        signed int abl_vuln; // r16
        signed int abl_spyk; // r8
        signed int abl_move; // r7
        signed int abl_dtct; // r6
        signed int abl_atak; // r5
        signed int ability; // r4
        signed int vidx; // r3
    }
}

// 0x0016A1D0 - 0x0016A1F8 (0x28) zEntVillain_PlayAFXSound__FP14st_zEntVillainP12zAnimFxSoundPUi
signed int zEntVillain_PlayAFXSound(class st_zEntVillain *ent, class zAnimFxSound *snd, unsigned int *sid_played) {
    { // 0x0016A1D0 - 0x0016A1F8
        signed int rc; // r2
    }
}

// 0x0016A200 - 0x0016A31C (0x11C) zEntVillain_Stun__FP14st_zEntVillainf
void zEntVillain_Stun(class st_zEntVillain *ent, float strength) {
    { // 0x0016A200 - 0x0016A31C
        class st_STIMULUS_CONTEXT_DATA stimctxt; // r29+0x20
    }
}

// 0x0016A320 - 0x0016A328 (0x8) zEntVillain_HeroWasHurt__Fv
void zEntVillain_HeroWasHurt() {
    { // 0x0016A320 - 0x0016A328
    }
}

// 0x0016A330 - 0x0016A434 (0x104) zEntVillain_Bumped__FP14st_zEntVillainPC8_xCollis
enum en_ZVILLAIN_BUMPED_RESPONSE zEntVillain_Bumped(class st_zEntVillain *ent, class _xCollis *colrec) {
    { // 0x0016A330 - 0x0016A434
        class st_STIMULUS_CONTEXT_DATA stimctxt; // r29+0x30
    }
}

// 0x0016A440 - 0x0016A840 (0x400) zEntVillain_Bubble__FP14st_zEntVillainP11_zEntBubble
enum en_ZVILLAIN_BUBBLE_RESPONSE zEntVillain_Bubble(class st_zEntVillain *ent, class _zEntBubble *bub) {
    { // 0x0016A440 - 0x0016A840
        class st_STIMULUS_CONTEXT_DATA stimctxt; // r29+0x40
    }
}

// 0x0016A840 - 0x0016AB28 (0x2E8) zEntVillain_Headbutt__FP14st_zEntVillainP6_xVec3
enum en_ZVILLAIN_HEADBUTT_RESPONSE zEntVillain_Headbutt(class st_zEntVillain *ent, class _xVec3 *vec_hit) {
    { // 0x0016A840 - 0x0016AB28
        class _xVec3 dir_PtoV; // r29+0x90
        float mag; // r29+0xa0
        class st_STIMULUS_CONTEXT_DATA stimctxt; // r29+0x30
    }
}

// 0x0016AB30 - 0x0016AD04 (0x1D4) zEntVillain_Bounce__FP14st_zEntVillain
enum en_ZVILLAIN_BOUNCE_RESPONSE zEntVillain_Bounce(class st_zEntVillain *ent) {
    { // 0x0016AB30 - 0x0016AD04
        class st_STIMULUS_CONTEXT_DATA stimctxt; // r29+0x30
    }
}

// 0x0016AD10 - 0x0016ADF0 (0xE0) zEntVillain_BonusRender__Fv
void zEntVillain_BonusRender() {
    { // 0x0016AD10 - 0x0016ADF0
        signed int i; // r16
    }
}

// 0x0016ADF0 - 0x0016ADF8 (0x8) zEntVillain_Load__FP14st_zEntVillainP7xSerial
void zEntVillain_Load(class st_zEntVillain *ent, class xSerial *xser) {
    { // 0x0016ADF0 - 0x0016ADF8
    }
}

// 0x0016AE00 - 0x0016AE08 (0x8) zEntVillain_Save__FP14st_zEntVillainP7xSerial
void zEntVillain_Save(class st_zEntVillain *ent, class xSerial *xser) {
    { // 0x0016AE00 - 0x0016AE08
    }
}

// 0x0016AE10 - 0x0016B0D0 (0x2C0) zEntVillain_InitDep__FP14st_zEntVillainP6zScene
void zEntVillain_InitDep(class st_zEntVillain *ent) {
    { // 0x0016AE10 - 0x0016B0D0
        class st_STIMULUS_CONTEXT_DATA stimrush; // r29+0x20
        enum en_zEntVillain_StartBehaviour st_start; // r2
    }
}

// 0x0016B0D0 - 0x0016B3C4 (0x2F4) zEntVillain_Init__FP14st_zEntVillainP10p2EntAsset
void zEntVillain_Init(class st_zEntVillain *ent, class p2EntAsset *asset) {
    { // 0x0016B0D0 - 0x0016B3C4
    }
}

// 0x0016B3D0 - 0x0016B3F0 (0x20) zEntVillain_NPCInst__FUi
class st_zEntVillain *zEntVillain_NPCInst(unsigned int npcidx) {
    { // 0x0016B3D0 - 0x0016B3F0
    }
}

// 0x0016B3F0 - 0x0016B494 (0xA4) zEntVillain_MemPool__FUi
class st_zEntVillain *zEntVillain_MemPool(unsigned int npccnt) {
    { // 0x0016B3F0 - 0x0016B494
    }
}

// 0x0016B4A0 - 0x0016B4A8 (0x8) zEntVillain_AnimTable_Standard__Fv
class xAnimTable *zEntVillain_AnimTable_Standard() {
    { // 0x0016B4A0 - 0x0016B4A8
    }
}

// 0x0016B4B0 - 0x0016B4B8 (0x8) zEntVillain_AnimTable_Gang__Fv
class xAnimTable *zEntVillain_AnimTable_Gang() {
    { // 0x0016B4B0 - 0x0016B4B8
    }
}

// 0x0016B4C0 - 0x0016B4C8 (0x8) zEntVillain_AnimTable_Simple__Fv
class xAnimTable *zEntVillain_AnimTable_Simple() {
    { // 0x0016B4C0 - 0x0016B4C8
    }
}

// 0x0016B4D0 - 0x0016B534 (0x64) zEntVillain_AnimTable_Witch__Fv
class xAnimTable *zEntVillain_AnimTable_Witch() {
    { // 0x0016B4D0 - 0x0016B534
    }
}

// 0x0016B540 - 0x0016B548 (0x8) zEntVillain_AnimTable_Shark__Fv
class xAnimTable *zEntVillain_AnimTable_Shark() {
    { // 0x0016B540 - 0x0016B548
    }
}

// 0x0016B550 - 0x0016B558 (0x8) zEntVillain_AnimTable_BossSham__Fv
class xAnimTable *zEntVillain_AnimTable_BossSham() {
    { // 0x0016B550 - 0x0016B558
    }
}

// 0x0016B560 - 0x0016B568 (0x8) zEntVillain_AnimTable_MasterMind__Fv
class xAnimTable *zEntVillain_AnimTable_MasterMind() {
    { // 0x0016B560 - 0x0016B568
    }
}

// 0x0016B570 - 0x0016B578 (0x8) zEntVillain_AnimTable_RedBeard__Fv
class xAnimTable *zEntVillain_AnimTable_RedBeard() {
    { // 0x0016B570 - 0x0016B578
    }
}

// 0x0016B580 - 0x0016B588 (0x8) zEntVillain_AnimTable_GreenGhost__Fv
class xAnimTable *zEntVillain_AnimTable_GreenGhost() {
    { // 0x0016B580 - 0x0016B588
    }
}

// 0x0016B590 - 0x0016B598 (0x8) zEntVillain_AnimTable_BlackKnight__Fv
class xAnimTable *zEntVillain_AnimTable_BlackKnight() {
    { // 0x0016B590 - 0x0016B598
    }
}

// 0x0016B5A0 - 0x0016B5FC (0x5C) zEntVillain_sceneFinish__Fv
signed int zEntVillain_sceneFinish() {
    { // 0x0016B5A0 - 0x0016B5FC
    }
}

// 0x0016B600 - 0x0016B69C (0x9C) zEntVillain_scenePrepare__Fv
signed int zEntVillain_scenePrepare() {
    { // 0x0016B600 - 0x0016B69C
    }
}

// 0x0016B6A0 - 0x0016B6CC (0x2C) zEntVillain_Shutdown__Fv
signed int zEntVillain_Shutdown() {
    { // 0x0016B6A0 - 0x0016B6CC
    }
}

// 0x0016B6D0 - 0x0016B850 (0x180) zEntVillain_Startup__Fv
signed int zEntVillain_Startup() {
    { // 0x0016B6D0 - 0x0016B850
        signed int i; // r4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xModel.h
// ==========================================================================
// 0x0013DF90 - 0x0013DF98 (0x8) xModelGetFrame__FP14xModelInstance
class _xMat4x3 *xModelGetFrame(class xModelInstance *model) {
    { // 0x0013DF90 - 0x0013DF98
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xEnt.h
// ==========================================================================
// 0x00163F80 - 0x00163F90 (0x10) xEntGetPos__FPC5_xEnt
class _xVec3 *xEntGetPos(class _xEnt *ent) {
    { // 0x00163F80 - 0x00163F90
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntPlayer.cpp
// ==========================================================================
class _tagTRCPadInfo gTrcPad[4]; // 0x003B1A10
unsigned int (SD0_TossCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001810F0
unsigned int (Shaggy0_FlipCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C19A0
unsigned int (LandHealthCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182950
unsigned int (IdleCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183120
unsigned int (TeeterCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183750
unsigned int (SlipperyCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001836F0
unsigned int (PlungerCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183820
unsigned int (SkidStopCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001830E0
unsigned int (WalkCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001833E0
unsigned int (WalkPlungeCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001831F0
unsigned int (SlideCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181860
unsigned int (RunCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001835D0
unsigned int (StickySurfWalk)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183380
unsigned int (StickySurfIdle)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001833B0
unsigned int (StickNoBootsWalk)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183240
unsigned int (StickNoBootsIdle)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001832E0
unsigned int (ClearHelmetCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001811E0
unsigned int (NotRunCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001834F0
unsigned int (SneakMoveCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183080
unsigned int (SneakIdleCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183040
unsigned int (NosneakTeeter)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182EF0
unsigned int (NosneakSlippery)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182E70
unsigned int (NosneakPlunger)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182E10
unsigned int (NosneakCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182FE0
unsigned int (HotsauceCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001836D0
unsigned int (CowerCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182D80
unsigned int (UncowerCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182D20
unsigned int (ScareCheckNoHealth)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182CF0
unsigned int (ScareCheckCritter)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182C40
unsigned int (DeathCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001812D0
unsigned int (FallToDeathStart)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001812A0
unsigned int (FallToDeathDrop)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181270
unsigned int (FallToDeathDropCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001811F0
unsigned int (ScareCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182BD0
unsigned int (ScareCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182DB0
unsigned int (ScareKnockbackCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182AD0
unsigned int (NospringCheck)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x00181FC0
unsigned int (JumpCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182480
unsigned int (DblJumpCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182380
unsigned int (DblJumpCheckTime)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001822D0
unsigned int (DblJumpCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182210
unsigned int (HangCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181750
unsigned int (HangCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181460
unsigned int (JumpCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182540
unsigned int (JumpHangCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001823F0
unsigned int (NoHangCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181730
unsigned int (HangSwingCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181400
unsigned int (HangNoSwingCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001813A0
unsigned int (FallingCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182930
unsigned int (FloatCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181780
unsigned int (NotFloatCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181760
unsigned int (ButtSmashCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181940
unsigned int (ButtSmashCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181900
unsigned int (ButtSmashStateCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001818E0
unsigned int (ButtSmashLandCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182010
unsigned int (BounceCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001821F0
unsigned int (SpringCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181FF0
unsigned int (BounceCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001821D0
unsigned int (LandSlowCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182A20
unsigned int (LandWalkCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001829E0
unsigned int (LandFastCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001829A0
unsigned int (LandCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182AB0
unsigned int (ScareLandCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182A60
unsigned int (IdleSitCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182820
unsigned int (IdleSitCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182710
unsigned int (ActiveCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182680
unsigned int (IdleGetupCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001827D0
unsigned int (IdleGetupCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001826E0
unsigned int (IdleMinorCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001828C0
unsigned int (IdleMinorCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182790
unsigned int (IdleMajorCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182850
unsigned int (IdleMajorCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00182740
unsigned int (PowerupCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181710
unsigned int (PowerupCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181700
unsigned int (ProjectileCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181DD0
unsigned int (ProjectileRepeatSync)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181CE0
unsigned int (ProjectileRepeat)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181C00
unsigned int (ProjectileCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181BD0
unsigned int (AttackCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181B50
unsigned int (AttackCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181B20
unsigned int (HeadHitCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001819E0
unsigned int (HeadHitCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001819D0
unsigned int (HealthCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00183860
unsigned int (AttackFinished)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181AC0
unsigned int (NotRunCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001834D0
unsigned int (AttackFinishedRun)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181A40
unsigned int (AttackFinishedHotsauce)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001819F0
unsigned int (DigCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181F30
unsigned int (DigCB)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181EF0
unsigned int (StopDigCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00181E80
unsigned int (FaceScare)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A600
unsigned int (FaceMad)(class xAnimTransition *, class xAnimSingle *); // 0x0017A660
unsigned int (FaceBlow)(class xAnimTransition *, class xAnimSingle *); // 0x0017A680
unsigned int (FaceClearScareCB)(class xAnimTransition *, class xAnimSingle *); // 0x0017A650
unsigned int (FaceEvent)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A560
unsigned int (FaceRun)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A4F0
unsigned int (FacePant)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A450
unsigned int (FaceTeeter)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A3B0
unsigned int (FaceSneak)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A310
unsigned int (FaceSwing)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A270
unsigned int (FaceIdle)(class xAnimTransition *, class xAnimSingle *, void *); // 0x0017A1D0
unsigned int (LamponCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00179120
unsigned int (LampoffCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001790D0
unsigned int (LampbackCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00179080
unsigned int (LampFixbugCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00179030
unsigned int (LampDrawCheck)(class xAnimTransition *, class xAnimSingle *, void *); // 0x00179180
void (zEntPlayer_Update)(class _xEnt *, class _xScene *, float); // 0x00176150
void (zEntPlayer_Move)(class _xEnt *, class _xScene *, float, class _xEntFrame *); // 0x00174F30
void (zEntPlayer_Render)(class _zEnt *); // 0x00175040
signed int (zEntPlayerEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x0016EEB0
void (PlayerRotMatchUpdateEnt)(class _xEnt *, class _xScene *, float, void *); // 0x0016DF80
void (zGustUpdateEnt)(class _xEnt *, class _xScene *, float, void *); // 0x001A6250
signed int (ButtonHBCallback)(class _zEnt *, class _xCollis *); // 0x00180EE0
signed int (DestructObjHBCallback)(class _zEnt *, class _xCollis *); // 0x00180990
signed int (VillHBCallback)(class _zEnt *, class _xCollis *); // 0x00183890
float sTimeElapsed; // 0x00357D84
signed int (Shaggy2_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001BAF60
signed int (Shaggy2Plat_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001BAEA0
unsigned int (PlayerDepenQuery)(class _xEnt *, class _xEnt *, class _xScene *, float, class _xCollis *); // 0x00172EC0
void (PlayerBoundUpdate)(class _xEnt *, class _xVec3 *); // 0x00172C90
class _xEnt * (PlayerCollCheckOneVillain)(class _xEnt *, class _xScene *, void *); // 0x001710C0
class _xEnt * (PlayerCollCheckOneEnt)(class _xEnt *, class _xScene *, void *); // 0x00171680
class _xEnt * (zEntPlayerCollTriggerCB)(class _xEnt *, class _xScene *, void *); // 0x0016F540
class _xVec3 g_O3; // 0x00342E28
class _xVec3 g_Y3; // 0x00342E38
unsigned int (zEntPlayer_FXScratchCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x0016BE60
unsigned int (zEntPlayer_SNDSkidCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x0016BBC0
unsigned int (zEntPlayer_FXDigupCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x0016BE20
unsigned int (zEntPlayer_FXSpringCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x0016BD30
unsigned int (zEntPlayer_FXOpenBrollyCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x0016BD20
class zGlobals globals; // 0x00362FD0
static class RpClump *sPlayerHeadClump; // 0x00358030
static class RpClump *sPlayerBodyClump; // 0x0035802C
static class RpAtomic *sPlayerHeadAtomic; // 0x00358028
static class RpAtomic *sPlayerBodyAtomic; // 0x00358024
static float sVOTimer; // 0x00358010
static class zPlayerSituation *sVONextSituation; // 0x0035800C
static class zPlayerSituation sPlayerInfo[37]; // 0x00297890
static unsigned int sVOPriority; // 0x00358008
static unsigned int sVOAssetID; // 0x00358004
static unsigned int sVO; // 0x00358000
static unsigned int cchkSquish; // 0x00357FFC
static unsigned int cchkButtbounce; // 0x00357FF8
class _zParEmitterCustomSettings g_info; // 0x0036E850
float floor_tmr[4]; // 0x0036E840
float floor_dist[4]; // 0x0036E830
class _xVec3 floor_supp[4]; // 0x0036E800
class _xMat4x3 rendermat; // 0x0036E7C0
class _xVec3 headcoll_os; // 0x00297850
class _xVec3 headcoll_fromos; // 0x00297840
float headcoll_bltime; // 0x00357FAC
float headcoll_tmr; // 0x00357FA8
signed int headcoll_anim; // 0x00357FA4
static float headcoll_bltimes[12]; // 0x00297810
static class _xVec3 headcoll_oses[12]; // 0x00297780
class _xMat4x3 gPlayerAbsMat; // 0x0036E780
class zGustData gust_data; // 0x0036E750
class _xVec3 vHangLast; // 0x0036E740
class _xVec3 vHangFirst; // 0x0036E730
class _xVec3 vHangBefore; // 0x0036E720
static class _xVec3 req_motion; // 0x0036E710
static class _xVec3 update_motion; // 0x0036E700
static float update_dt; // 0x00357FA0
static float surfSlipTimer; // 0x00357F9C
static float surfMaxSpeed; // 0x00357F98
static float surfDecelSkid; // 0x00357578
static float surfDecelIdle; // 0x00357574
static float surfAccelRun; // 0x00357570
static float surfAccelWalk; // 0x0035756C
static float surfPeakRatio; // 0x00357568
static float surfSlickTimer; // 0x00357F94
static float surfSlickRatio; // 0x00357F90
static float surfSlideStop; // 0x00357564
static float surfSlideStart; // 0x00357560
static unsigned int surfSticky; // 0x00357F8C
static class _xEnt *lastFloorEnt; // 0x00357F88
static class _xVec3 lastFloorNorm; // 0x0036E6F0
static class _xVec3 lastDeltaPos; // 0x0036E6E0
static signed int lastSlickness; // 0x0035755C
static float surfDamping; // 0x00357F84
static float surfFriction; // 0x00357558
static signed int surfSlickness; // 0x00357554
static float curVelangle; // 0x00357F80
static float curVelmag; // 0x00357F7C
static float maxVelmag; // 0x00357550
static float minVelmag; // 0x0035754C
float umbrellaFudgeRot; // 0x00357548
float umbrellaFudgeY; // 0x00357544
float umbrellaFudgeX; // 0x00357540
float startBounce; // 0x00357F78
float startDouble; // 0x00357F74
float startJump; // 0x00357F70
static class _tagPlayerSndFX sPlayerSndfx; // 0x0036E4B0
float sDecrementAmount; // 0x00357F6C
float sDecrementDelay; // 0x00357F68
class _zEntPickup *sDecrementSnackGateEnt; // 0x00357F64
unsigned int sSnackDecrement; // 0x00357F60
class _tagFeetSkidMark *sPlayerCurSkidMark; // 0x00357F5C
class _tagFeetSkidMark sPlayerSkidMark[5]; // 0x0036E3F0
class tagzFeet sPlayerFeet[10]; // 0x0036E370
class _tagPlayerFX gPlayerfx; // 0x0036E2A0
static unsigned int sSoundWaterSplash; // 0x00357F58
static class _zParEmitter *sEmitterHeadButt2; // 0x00357F54
static class _zParEmitter *sEmitterHeadButt; // 0x00357F50
static class _zParEmitter *sEmitterBounceOnVillain; // 0x00357F4C
static class _zParEmitter *sEmitterSpringBoard; // 0x00357F48
static class _zParEmitter *sEmitterSprings; // 0x00357F44
static class _zParEmitter *sEmitterBounceMiss; // 0x00357F40
static class _zParEmitter *sEmitterWaterSplash; // 0x00357F3C
static class _zParEmitter *sEmitterShockRight; // 0x00357F38
static class _zParEmitter *sEmitterShockLeft; // 0x00357F34
static class _zParEmitter *sEmitterStars; // 0x00357F30
class _xEnt *sDebugPlayerPtr; // 0x00000000
class _xVec3 gDeadScoobyPos; // 0x0036E290
signed int gDeadScooby; // 0x00357F2C
signed int gPlayerHitWater; // 0x00357F28
class _xVec3 gPlayerWaterPos; // 0x0036E280
static float sPlayerWaterOffset; // 0x00357F24
signed int sPlayerSurfaceType; // 0x00357F20
// 0x0016B850 - 0x0016B928 (0xD8) zEntPlayer_InitCutsceneModelHack__Fv
void zEntPlayer_InitCutsceneModelHack() {
    { // 0x0016B850 - 0x0016B928
    }
}

// 0x0016B930 - 0x0016BAA4 (0x174) zEntPlayer_ButtonHBFX__FP5_xEntP11_zEntButton
static void zEntPlayer_ButtonHBFX() {
    { // 0x0016B930 - 0x0016BAA4
        class _xVec3 angles; // r29+0xa0
        class _xEnt *player_ent; // r4
        class _zParEmitterCustomSettings info; // r29+0x40
    }
}

// 0x0016BAB0 - 0x0016BBB8 (0x108) zEntPlayer_ButtonBBFX__FP5_xEntP11_zEntButton
static void zEntPlayer_ButtonBBFX(class _xEnt *ent) {
    { // 0x0016BAB0 - 0x0016BBB8
        class _zParEmitterCustomSettings info; // r29+0x10
    }
}

// 0x0016BBC0 - 0x0016BD1C (0x15C) zEntPlayer_SNDSkidCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int zEntPlayer_SNDSkidCB() {
    { // 0x0016BBC0 - 0x0016BD1C
        signed int newsnd; // r3
        float pitch; // r29+0x2c
        static signed int snd; // 0x00358014
        static signed short pitch_angle; // 0x0035801C
        static signed char init; // 0x00358020
        static signed char init; // 0x00358018
    }
}

// 0x0016BD20 - 0x0016BD28 (0x8) zEntPlayer_FXOpenBrollyCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int zEntPlayer_FXOpenBrollyCB() {
    { // 0x0016BD20 - 0x0016BD28
    }
}

// 0x0016BD30 - 0x0016BE20 (0xF0) zEntPlayer_FXSpringCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int zEntPlayer_FXSpringCB() {
    { // 0x0016BD30 - 0x0016BE20
        class _zParEmitterCustomSettings info; // r29+0x60
        class _xEnt *ent; // r16
    }
}

// 0x0016BE20 - 0x0016BE54 (0x34) zEntPlayer_FXDigupCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int zEntPlayer_FXDigupCB() {
    { // 0x0016BE20 - 0x0016BE54
    }
}

// 0x0016BE60 - 0x0016BE94 (0x34) zEntPlayer_FXScratchCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int zEntPlayer_FXScratchCB() {
    { // 0x0016BE60 - 0x0016BE94
    }
}

// 0x0016BEA0 - 0x0016C0B8 (0x218) zEntPlayer_FXSetup__FP5_xEntP10xAnimTable
static void zEntPlayer_FXSetup(class xAnimTable *table) {
    { // 0x0016BEA0 - 0x0016C0B8
        signed int i; // r16
        class xAnimState *state; // r2
    }
}

// 0x0016C0C0 - 0x0016C48C (0x3CC) zEntPlayer_FXUpdate__FP5_xEntf
static void zEntPlayer_FXUpdate(class _xEnt *ent, float seconds) {
    { // 0x0016C0C0 - 0x0016C48C
        class zParEmitterAsset *a; // r4
        class _xVec3 paw; // r29+0x60
        class zParEmitterAsset *a; // r16
        signed int i; // r16
        class zParEmitterAsset *a; // r3
        signed int i; // r5
        signed int least_edges; // r4
        signed int i; // r3
        class zSurfAsset *surface_asset; // r2
        class zSurfaceProps *surface; // r2
        class xSurface *xsurf; // r2
    }
}

// 0x0016C490 - 0x0016C79C (0x30C) zEntPlayer_FXLostChance__FP5_xEntf
static void zEntPlayer_FXLostChance(class _xEnt *ent) {
    { // 0x0016C490 - 0x0016C79C
        class _zParEmitterCustomSettings info; // r29+0x120
        class _zParEmitterCustomSettings info; // r29+0xc0
        class _zParEmitterCustomSettings info; // r29+0x60
        float dz; // 
        float dx; // 
        class zSurfaceProps *prop; // r2
        class xSurface *fsurf; // r2
        enum _LostChanceFX fx_to_play; // r18
        signed int sd_surface; // r17
    }
}

// 0x0016C7A0 - 0x0016C870 (0xD0) zEntPlayer_Notify__Fi
void zEntPlayer_Notify(signed int situation) {
    { // 0x0016C7A0 - 0x0016C870
        class zPlayerSituation *s; // r16
    }
}

// 0x0016C870 - 0x0016CB04 (0x294) zEntPlayer_Notify__Fif
void zEntPlayer_Notify(signed int characterSubType) {
    { // 0x0016C870 - 0x0016CB04
    }
}

// 0x0016CB10 - 0x0016CEE8 (0x3D8) zEntPlayer_SituationDo__Fv
void zEntPlayer_SituationDo() {
    { // 0x0016CB10 - 0x0016CEE8
    }
}

// 0x0016CEF0 - 0x0016CFD8 (0xE8) zEntPlayer_SNDPlayRandom__FPUiUi
static void zEntPlayer_SNDPlayRandom(unsigned int *array, unsigned int max) {
    { // 0x0016CEF0 - 0x0016CFD8
        unsigned int idx; // r17
        unsigned int i; // r16
    }
}

// 0x0016CFE0 - 0x0016D008 (0x28) zEntPlayer_SNDVan__FP5_xEnt
void zEntPlayer_SNDVan() {
    { // 0x0016CFE0 - 0x0016D008
    }
}

// 0x0016D010 - 0x0016D038 (0x28) zEntPlayer_SNDSnackCounter__FP5_xEnt
void zEntPlayer_SNDSnackCounter() {
    { // 0x0016D010 - 0x0016D038
    }
}

// 0x0016D040 - 0x0016D0A4 (0x64) zEntPlayer_SNDKeyPickup__FP5_xEnt
void zEntPlayer_SNDKeyPickup() {
    { // 0x0016D040 - 0x0016D0A4
        float pitch; // r29+0x2c
    }
}

// 0x0016D0B0 - 0x0016D114 (0x64) zEntPlayer_SNDErrorPickup__FP5_xEnt
void zEntPlayer_SNDErrorPickup() {
    { // 0x0016D0B0 - 0x0016D114
        float pitch; // r29+0x2c
    }
}

// 0x0016D120 - 0x0016D184 (0x64) zEntPlayer_SNDInfoPickup__FP5_xEnt
void zEntPlayer_SNDInfoPickup() {
    { // 0x0016D120 - 0x0016D184
        float pitch; // r29+0x2c
    }
}

// 0x0016D190 - 0x0016D1F4 (0x64) zEntPlayer_SNDSavePickup__FP5_xEnt
void zEntPlayer_SNDSavePickup() {
    { // 0x0016D190 - 0x0016D1F4
        float pitch; // r29+0x2c
    }
}

// 0x0016D200 - 0x0016D264 (0x64) zEntPlayer_SNDTokenPickup__FP5_xEnt
void zEntPlayer_SNDTokenPickup() {
    { // 0x0016D200 - 0x0016D264
        float pitch; // r29+0x2c
    }
}

// 0x0016D270 - 0x0016D2D4 (0x64) zEntPlayer_SNDSpecialPickup__FP5_xEnt
void zEntPlayer_SNDSpecialPickup() {
    { // 0x0016D270 - 0x0016D2D4
        float pitch; // r29+0x2c
    }
}

// 0x0016D2E0 - 0x0016D344 (0x64) zEntPlayer_SNDAmmoPickup__FP5_xEnt
void zEntPlayer_SNDAmmoPickup() {
    { // 0x0016D2E0 - 0x0016D344
        float pitch; // r29+0x2c
    }
}

// 0x0016D350 - 0x0016D3B4 (0x64) zEntPlayer_SNDBitePickup__FP5_xEnt
void zEntPlayer_SNDBitePickup() {
    { // 0x0016D350 - 0x0016D3B4
        float pitch; // r29+0x2c
    }
}

// 0x0016D3C0 - 0x0016D4AC (0xEC) zEntPlayer_SNDButtBounce__FP5_xEnti
static void zEntPlayer_SNDButtBounce(signed int hit_villain) {
    { // 0x0016D3C0 - 0x0016D4AC
        signed short choice; // r5
        float pitch; // r29+0x3c
    }
}

// 0x0016D4B0 - 0x0016DE40 (0x990) zEntPlayer_SNDInit__Fv
static void zEntPlayer_SNDInit() {
    { // 0x0016D4B0 - 0x0016DE40
        char tmp[256]; // r29+0x40
        signed int i; // r17
    }
}

// 0x0016DE40 - 0x0016DEF0 (0xB0) zEntPlayer_LoadCheckPoint__Fv
void zEntPlayer_LoadCheckPoint() {
    { // 0x0016DE40 - 0x0016DEF0
    }
}

// 0x0016DEF0 - 0x0016DF7C (0x8C) zEntPlayer_StoreCheckPoint__FPf
void zEntPlayer_StoreCheckPoint(float *toParam) {
    { // 0x0016DEF0 - 0x0016DF7C
        float r; // 
        class _xVec3 p; // r29+0x0
    }
}

// 0x0016DF80 - 0x0016E37C (0x3FC) PlayerRotMatchUpdateEnt__FP5_xEntP7_xScenefPv
static void PlayerRotMatchUpdateEnt(class _xEnt *ent, float dt, void *fdata) {
    { // 0x0016DF80 - 0x0016E37C
        class _xMat4x3 rot; // r29+0xe0
        float dang; // r29+0x170
        float rang; // r29+0x170
        class _xVec3 raxis; // r29+0x160
        class _xVec3 neup; // r29+0x150
        float s; // 
        class _xMat4x3 rot; // r29+0xa0
        float dang; // r29+0x170
        float rang; // r29+0x170
        class _xVec3 raxis; // r29+0x140
        float edecl; // 
        float fdecl; // 
        class _xVec3 neup; // r29+0x130
        class _xVec3 nfup; // r29+0x120
        class _xVec3 *eup; // r19
        class _xVec3 *fup; // r18
        signed int grounded; // r17
        class xSurface *surf; // r2
        signed int hit_it; // r17
        class _xCollis *coll; // r2
        class xFFXRotMatchState *rms; // r16
    }
}

// 0x0016E380 - 0x0016E58C (0x20C) PlayerDigUpdate__FP5_xEntf
static void PlayerDigUpdate(float dt) {
    { // 0x0016E380 - 0x0016E58C
        class _xEnt *tgtEnt; // r2
        class xBase *tgt; // r2
        class xBase *tgt; // r2
        class p2LinkAsset *curlink; // r17
        unsigned int i; // r16
        float digDelta; // 
        float digT; // r29+0x50
    }
}

// 0x0016E590 - 0x0016E678 (0xE8) PlayerTeeterCheck__FP5_xEntP7_xScenef
static void PlayerTeeterCheck(float dt) {
    { // 0x0016E590 - 0x0016E678
        signed int i; // r4
    }
}

// 0x0016E680 - 0x0016E9C8 (0x348) PlayerHangCheck__FP5_xEntP7_xScene
static void PlayerHangCheck(class _xEnt *ent, class _xScene *sc) {
    { // 0x0016E680 - 0x0016E9C8
        float d2; // r29+0x60
        float d1; // 
        class _xVec3 oldpos; // r29+0x50
        class _zEntHangable *hangme; // r9
        class _zEntHangable *hangclose; // r17
        float closedist; // 
        unsigned int sphereresult; // r16
        unsigned int i; // r8
    }
}

// 0x0016E9D0 - 0x0016EEB0 (0x4E0) PlayerHangUpdate__FP5_xEntfff
static void PlayerHangUpdate(class _xEnt *ent, float mag, float angle, float dt) {
    { // 0x0016E9D0 - 0x0016EEB0
        float mag; // r29+0x80
        float removedot; // r29+0x80
        class xAnimState *astate; // r4
        class _xVec3 unitHang; // r29+0x70
    }
}

// 0x0016EEB0 - 0x0016F4A0 (0x5F0) zEntPlayerEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntPlayerEventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x0016EEB0 - 0x0016F4A0
        char *astatename; // r16
        class _zEnt *z; // r2
    }
}

// 0x0016F4A0 - 0x0016F534 (0x94) GetPosVec__FP5xBase
static class _xVec3 *GetPosVec(class xBase *target) {
    { // 0x0016F4A0 - 0x0016F534
        class _xVec3 *pos; // r2
    }
}

// 0x0016F540 - 0x0016F990 (0x450) zEntPlayerCollTriggerCB__FP5_xEntP7_xScenePv
class _xEnt *zEntPlayerCollTriggerCB(class _xEnt *ent, void *data) {
    { // 0x0016F540 - 0x0016F990
        class _xIsect isect; // r29+0x120
        class _xSphere sphere; // r29+0x110
        class _xIsect isect; // r29+0xe0
        class _xCylinder cylinder; // r29+0xc0
        class _xIsect isect; // r29+0x90
        class _xSphere sphere; // r29+0x80
        class _xIsect isect; // r29+0x50
        class _xBox box; // r29+0x30
        class p2EntTriggerAsset *tasset; // r7
        class _zEnt *t; // r17
        class _zEnt *p; // r16
    }
}

// 0x0016F990 - 0x0016FC04 (0x274) zEntPlayerCollTrigger__FP5_xEntP7_xScene
void zEntPlayerCollTrigger(class _xEnt *ent, class _xScene *sc) {
    { // 0x0016F990 - 0x0016FC04
        class p2EntTriggerAsset *asset; // r5
        class p2EntTriggerAsset *asset; // r5
        unsigned short i; // r16
    }
}

// 0x0016FC10 - 0x0016FEBC (0x2AC) zEntPlayerCollide__FP5_xEntP7_xScenef
void zEntPlayerCollide(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x0016FC10 - 0x0016FEBC
        class _xCollis *c; // r5
        class _xCollis *cend; // r4
        class _xCollis *colls; // r4
    }
}

// 0x0016FEC0 - 0x00170E44 (0xF84) PlayerCollsSelectDepen__FP5_xEntP7_xScenef
static void PlayerCollsSelectDepen(class _xEnt *p) {
    { // 0x0016FEC0 - 0x00170E44
        float dz; // 
        float dz; // 
        class _xCollis *cright; // r5
        class _xCollis *cleft; // r4
        float dz; // 
        float dx; // 
        float dz; // 
        float dx; // 
        class _xCollis *crear; // r3
        class _xCollis *cfront; // r5
        class _xCollis *inside_wall; // r7
        class _xCollis *first_wall; // r6
        signed int num_walls; // r5
        class _xCollis *cceil; // r18
        class xSurface *sfloor; // r2
        class xSurface *surf; // r18
        class _xEnt *cent; // r2
        float depen_len; // 
        float h_dot_n; // r29+0xa0
        class _xCollis *coll; // r18
        class _xCollis *curr; // r3
        unsigned char idx; // r2
        class _xCollis *cend; // r19
        class _xCollis *c; // r18
        class _xMat4x3 *mat; // r17
        class _xCollis *colls; // r16
    }
}

// 0x00170E50 - 0x00170EF0 (0xA0) PlayerCollsWallsTranslate__FP8_xCollisfff
static void PlayerCollsWallsTranslate(class _xCollis *colls, float x, float y, float z) {
    { // 0x00170E50 - 0x00170EF0
        class _xCollis *c; // r17
        class _xCollis *cend; // r16
    }
}

// 0x00170EF0 - 0x00170F90 (0xA0) PlayerCollsAllTranslate__FP8_xCollisfff
static void PlayerCollsAllTranslate(class _xCollis *colls, float x, float y, float z) {
    { // 0x00170EF0 - 0x00170F90
        class _xCollis *c; // r17
        class _xCollis *cend; // r16
    }
}

// 0x00170F90 - 0x001710B4 (0x124) PlayerCollisTranslate__FP8_xCollisfff
static void PlayerCollisTranslate(class _xCollis *c, float x, float y, float z) {
    { // 0x00170F90 - 0x001710B4
        float s; // 
        float dist; // r29+0x30
        float dz; // 
        float dy; // 
        float dx; // 
    }
}

// 0x001710C0 - 0x00171674 (0x5B4) PlayerCollCheckOneVillain__FP5_xEntP7_xScenePv
static class _xEnt *PlayerCollCheckOneVillain(class _xEnt *ent, void *data) {
    { // 0x001710C0 - 0x00171674
        float hsqr; // r29+0xc0
        float dz; // r29+0xc0
        float dy; // 
        float dx; // 
        float rsum; // 
        class _xCollis coll2; // r29+0x70
        class st_zEntVillain *vill; // r2
        float playerOldRad; // 
        unsigned int modl_coll; // r6
        class _xCollis *coll; // r17
        class _xEnt *p; // r16
    }
}

// 0x00171680 - 0x00171CA0 (0x620) PlayerCollCheckOneEnt__FP5_xEntP7_xScenePv
static class _xEnt *PlayerCollCheckOneEnt(class _xEnt *ent, void *data) {
    { // 0x00171680 - 0x00171CA0
        float hsqr; // r29+0x100
        float dz; // r29+0x100
        float dy; // 
        float dx; // 
        float rsum; // 
        unsigned char idx; // r2
        unsigned char ncolls; // r2
        class _xSphere temp; // r29+0xf0
        class _xCollis coll2; // r29+0xa0
        unsigned int head_hit; // r19
        unsigned int modl_coll; // r18
        class _xCollis *coll; // r17
        class _xEnt *p; // r16
    }
}

// 0x00171CA0 - 0x001724C4 (0x824) zEntPlayerReset__FP5_xEnt
void zEntPlayerReset(class _xEnt *ent) {
    { // 0x00171CA0 - 0x001724C4
        class iColor_tag clear; // r29+0x8c
        class iColor_tag black; // r29+0x88
        unsigned int index; // r4
        class xModelInstance *minst; // r3
        class xAnimState *curr; // r6
        class iModelTag tmpTag; // r29+0x60
        unsigned int numV; // r19
        unsigned int wnum; // r6
        unsigned int widx; // r3
        unsigned int tngV; // r18
        unsigned int tongueIdx; // r3
        class xModelInstance *tongue; // r17
    }
}

// 0x001724D0 - 0x0017257C (0xAC) zEntPlayerExit__FP5_xEnt
void zEntPlayerExit(class _xEnt *ent) {
    { // 0x001724D0 - 0x0017257C
    }
}

// 0x00172580 - 0x00172AE8 (0x568) zEntPlayerDriveUpdate__FP5_xEntP7_xScenef
void zEntPlayerDriveUpdate(class _xEnt *p, float dt) {
    { // 0x00172580 - 0x00172AE8
        class xAnimSingle *asing; // r3
        class xAnimPlay *aplay; // r2
        class _xVec3 *jmpdir; // r16
        float *jmphs; // r2
        class xSurface *surf; // r2
        class _zPlatform *plat; // r2
        class _xCollis *coll; // r2
        class _zPlatform *oplat; // r2
        class _xEntDrive *drv; // r16
    }
}

// 0x00172AF0 - 0x00172C88 (0x198) zEntPlayerSurfDamageUpdate__FP5_xEntP7_xScenef
void zEntPlayerSurfDamageUpdate(class _xEnt *p) {
    { // 0x00172AF0 - 0x00172C88
        class _xEnt *cent; // r2
        class zSurfaceProps *prop; // r3
        signed int damaged; // r6
        class xSurface *surf; // r2
        class _xCollis *cend; // r17
        class _xCollis *coll; // r16
    }
}

// 0x00172C90 - 0x00172EB8 (0x228) PlayerBoundUpdate__FP5_xEntP6_xVec3
static void PlayerBoundUpdate(class _xEnt *ent, class _xVec3 *pos) {
    { // 0x00172C90 - 0x00172EB8
        class _xVec3 dpos; // r29+0x60
    }
}

// 0x00172EC0 - 0x00172F74 (0xB4) PlayerDepenQuery__FP5_xEntP5_xEntP7_xScenefP8_xCollis
static unsigned int PlayerDepenQuery(class _xCollis *coll) {
    { // 0x00172EC0 - 0x00172F74
        class zSurfaceProps *prop; // r2
        class xSurface *surf; // r2
    }
}

// 0x00172F80 - 0x00173B4C (0xBCC) zEntPlayerFloorUpdate__FP5_xEntP7_xScenef
void zEntPlayerFloorUpdate(class _xEnt *p, float dt) {
    { // 0x00172F80 - 0x00173B4C
        float s; // 
        float dz; // 
        float s; // 
        float ddotm; // 
        class _xVec3 D_flat; // r29+0x1d0
        class _xMat3x3 N_to_Up; // r29+0x180
        class _xVec3 D; // r29+0x1c0
        class _xVec3 X; // r29+0x1b0
        class _xVec3 *vel; // r16
        float dmax; // 
        float dmin; // 
        float dave; // 
        signed int i; // r30
        class _xEnt *fent; // r16
        class _xCollis rcoll; // r29+0x130
        class _xRay3 ray; // r29+0x100
        class _xVec3 *fnorm; // r29+0xd0
        float slickLerp; // 
        float decelmag; // 
        float velmag; // 
        class _xVec3 *vel; // r16
        class xSurface *fsurf; // r29+0xcc
        class _xCollis *fcoll; // r29+0xb0
        static class _xVec3 lorigins[4]; // 0x00297860
    }
}

// 0x00173B50 - 0x00173FA0 (0x450) zEntPlayerVelUpdate__FP5_xEntP7_xScenef
void zEntPlayerVelUpdate(class _xEnt *ent, float dt) {
    { // 0x00173B50 - 0x00173FA0
        float s; // 
        float s; // 
        float sft; // 
        signed int i; // r6
        class _xCollis *coll; // r5
        class _xCollis *colls; // r2
        float velen2; // r29+0x50
        float s; // 
        float aft; // 
        float s; // 
        float gft; // 
        float rft; // 
        class _xVec3 *v; // r16
    }
}

// 0x00173FA0 - 0x00174508 (0x568) zEntPlayerJumpUpdate__FP5_xEntP7_xScenef
void zEntPlayerJumpUpdate(class _xEnt *ent, float dt) {
    { // 0x00173FA0 - 0x00174508
        class _xCollis *cfloor; // r2
        float lerp; // 
    }
}

// 0x00174510 - 0x001745BC (0xAC) zEntPlayerJumpStart__FP5_xEntP10zJumpParam
void zEntPlayerJumpStart(class _xEnt *ent, class zJumpParam *jump) {
    { // 0x00174510 - 0x001745BC
    }
}

// 0x001745C0 - 0x00174604 (0x44) CalcJumpImpulse__FP10zJumpParam
void CalcJumpImpulse(class zJumpParam *param) {
    { // 0x001745C0 - 0x00174604
    }
}

// 0x00174610 - 0x001748E4 (0x2D4) CalcJumpImpulse_Smooth__Ffffff
static float CalcJumpImpulse_Smooth(float g, float j, float h, float Tgc, float Tgs) {
    { // 0x00174610 - 0x001748E4
        float Tmfound; // r29+0x60
        float Tf; // 
        float Tm[3]; // r29+0x50
        float Ts; // 
        float Kbf; // r29+0x4c
        float AF; // 
        float D; // r29+0x48
        float Kc; // 
        float B; // 
        float A; // 
        float AS; // r29+0x60
        unsigned int solcnt; // r2
        unsigned int i; // r4
    }
}

// 0x001748F0 - 0x00174940 (0x50) zEntPlayer_GiveHotsauce__FUi
void zEntPlayer_GiveHotsauce(unsigned int value) {
    { // 0x001748F0 - 0x00174940
    }
}

// 0x00174940 - 0x00174970 (0x30) zEntPlayer_GiveHealth__FUi
void zEntPlayer_GiveHealth(unsigned int quantity) {
    { // 0x00174940 - 0x00174970
    }
}

// 0x00174970 - 0x001749A0 (0x30) zEntPlayer_GiveChance__FUi
void zEntPlayer_GiveChance(unsigned int quantity) {
    { // 0x00174970 - 0x001749A0
    }
}

// 0x001749A0 - 0x001749D0 (0x30) zEntPlayer_GiveKey__FUi
void zEntPlayer_GiveKey(unsigned int quantity) {
    { // 0x001749A0 - 0x001749D0
    }
}

// 0x001749D0 - 0x001749E8 (0x18) zEntPlayer_GiveClue__FUi
void zEntPlayer_GiveClue(unsigned int quantity) {
    { // 0x001749D0 - 0x001749E8
    }
}

// 0x001749F0 - 0x00174A18 (0x28) zEntPlayer_TakeScoobySnack__FP5_xEntUi
void zEntPlayer_TakeScoobySnack(class _xEnt *ent, unsigned int quantity) {
    { // 0x001749F0 - 0x00174A18
    }
}

// 0x00174A20 - 0x00174A34 (0x14) zEntPlayer_HasScoobySnack__FUi
signed int zEntPlayer_HasScoobySnack(unsigned int quantity) {
    { // 0x00174A20 - 0x00174A34
    }
}

// 0x00174A40 - 0x00174A84 (0x44) zEntPlayer_GiveScoobySnack__FUi
void zEntPlayer_GiveScoobySnack(unsigned int quantity) {
    { // 0x00174A40 - 0x00174A84
    }
}

// 0x00174A90 - 0x00174C08 (0x178) zEntPlayer_MoveInfo__Fv
unsigned int zEntPlayer_MoveInfo() {
    { // 0x00174A90 - 0x00174C08
        class _xEnt *plyr; // r17
        unsigned int infoflags; // r16
        unsigned int animflags; // r2
    }
}

// 0x00174C10 - 0x00174EBC (0x2AC) zEntPlayer_Damage__FP5xBaseUi
signed int zEntPlayer_Damage(class xBase *src, unsigned int damage) {
    { // 0x00174C10 - 0x00174EBC
    }
}

// 0x00174EC0 - 0x00174F2C (0x6C) zEntPlayer_Damage__FP5xBaseUiP6_xVec3
signed int zEntPlayer_Damage(class xBase *src, unsigned int damage, class _xVec3 *knockback) {
    { // 0x00174EC0 - 0x00174F2C
    }
}

// 0x00174F30 - 0x00175040 (0x110) zEntPlayer_Move__FP5_xEntP7_xScenefP10_xEntFrame
void zEntPlayer_Move(class _xEnt *ent, float dt, class _xEntFrame *frame) {
    { // 0x00174F30 - 0x00175040
        signed int pz; // r5
        signed int px; // r3
    }
}

// 0x00175040 - 0x00176144 (0x1104) zEntPlayer_Render__FP5_zEnt
void zEntPlayer_Render(class _zEnt *ent) {
    { // 0x00175040 - 0x00176144
        class xModelInstance *minst; // r17
        class xModelInstance *minst; // r3
        signed int player_trans; // r16
        class _xVec3 pos1; // r29+0x150
        class _xVec3 pos0; // r29+0x140
        class xAnimSingle *asingle; // r17
        unsigned int index; // r16
        class xModelInstance *minst; // r2
        class xModelInstance *minst; // r3
        class _xMat4x3 tmpMat; // r29+0xf0
        class _xVec3 rootZvec; // r29+0x130
        class xAnimSingle *ablend; // r16
        class xAnimSingle *asingle; // r2
        class RwMatrixTag rootOldMat; // r29+0xb0
        float rootZadjust; // r29+0x160
    }
}

// 0x00176150 - 0x0017864C (0x24FC) zEntPlayer_Update__FP5_xEntP7_xScenef
void zEntPlayer_Update(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x00176150 - 0x0017864C
        float parm[4]; // r29+0x1e0
        class _zEntSimpleObj *simpent; // r2
        class zScene *zsc; // r2
        unsigned int i; // r16
        float lerpspeed; // r29+0x240
        float mag; // r29+0x240
        class _zEntBubble *bub; // r2
        class _xCollis *endcoll; // r17
        class _xCollis *coll; // r16
        signed int took_damage; // r17
        enum en_ZVILLAIN_BUMPED_RESPONSE bumpedResponse; // r2
        signed int bounceResponse; // r17
        class _xCollis *endcoll; // r21
        class _xCollis *coll; // r16
        class _zEnt *shagent; // r6
        class _xCollis *endcoll; // r4
        class _xCollis *coll; // r5
        class _zEnt *shagent; // r17
        class _xCollis *endcoll; // r21
        class _xCollis *coll; // r16
        class xSurface *wsurf; // r2
        float hdotm; // 
        class _xCollis *gcoll; // r21
        class _xCollis *wcoll; // r16
        class _xEnt *cent; // r2
        class _xCollis *cend; // r22
        class _xCollis *c; // r16
        class _xCollis ctc; // r29+0x190
        class _xRay3 ctr; // r29+0x160
        class _xBox bbox; // r29+0x140
        class _xCollis bbc; // r29+0xf0
        float m; // 
        float fg; // r29+0x240
        float sft; // 
        float s; // 
        float dy; // 
        float sett2; // r29+0x240
        float ndotm; // r29+0x240
        class xFFX *gust_fkt; // r16
        class _xVec3 suboldpos; // r29+0x228
        signed int updidx; // r23
        signed int num_updates; // r29+0xc0
        class _xEnt *vill; // r21
        float ydist; // r29+0x240
        float sdistold2; // 
        float sdist2; // 
        float radold2; // 
        float rad2; // 
        unsigned int i; // r16
        class iColor_tag clear; // r29+0x238
        class iColor_tag black; // r29+0x234
        class xAnimState *astate; // r16
    }
}

// 0x00178650 - 0x00178798 (0x148) zEntPlayer_SnackDecrement__Ff
static void zEntPlayer_SnackDecrement(float dt) {
    { // 0x00178650 - 0x00178798
        unsigned int amount; // r4
    }
}

// 0x001787A0 - 0x001787A8 (0x8) zEntPlayer_Load__FP5_xEntP7xSerial
void zEntPlayer_Load() {
    { // 0x001787A0 - 0x001787A8
    }
}

// 0x001787B0 - 0x00178B2C (0x37C) zEntPlayer_Init__FP5_xEntP10p2EntAsset
void zEntPlayer_Init(class _xEnt *ent, class p2EntAsset *asset) {
    { // 0x001787B0 - 0x00178B2C
        class xFFXRotMatchState *rms; // r2
    }
}

// 0x00178B30 - 0x00178D3C (0x20C) zEntPlayerFindEmitters__Fv
static void zEntPlayerFindEmitters() {
    { // 0x00178B30 - 0x00178D3C
        signed int i; // r16
    }
}

// 0x00178D40 - 0x00179028 (0x2E8) zEntPlayer_LampshadeAnimTable__Fv
class xAnimTable *zEntPlayer_LampshadeAnimTable() {
    { // 0x00178D40 - 0x00179028
        class xAnimTable *table; // r16
    }
}

// 0x00179030 - 0x00179074 (0x44) LampFixbugCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LampFixbugCheck() {
    { // 0x00179030 - 0x00179074
    }
}

// 0x00179080 - 0x001790C4 (0x44) LampbackCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LampbackCheck() {
    { // 0x00179080 - 0x001790C4
    }
}

// 0x001790D0 - 0x00179114 (0x44) LampoffCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LampoffCheck() {
    { // 0x001790D0 - 0x00179114
    }
}

// 0x00179120 - 0x00179180 (0x60) LamponCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LamponCheck(class xAnimSingle *anim) {
    { // 0x00179120 - 0x00179180
    }
}

// 0x00179180 - 0x001791A0 (0x20) LampDrawCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LampDrawCheck(class xAnimSingle *anim) {
    { // 0x00179180 - 0x001791A0
    }
}

// 0x001791A0 - 0x00179204 (0x64) zEntPlayer_UmbrellaAnimTable__Fv
class xAnimTable *zEntPlayer_UmbrellaAnimTable() {
    { // 0x001791A0 - 0x00179204
        class xAnimTable *table; // r16
    }
}

// 0x00179210 - 0x00179274 (0x64) zEntPlayer_TongueAnimTable__Fv
class xAnimTable *zEntPlayer_TongueAnimTable() {
    { // 0x00179210 - 0x00179274
        class xAnimTable *table; // r16
    }
}

// 0x00179280 - 0x0017A06C (0xDEC) zEntPlayer_HeadAnimTable__Fv
class xAnimTable *zEntPlayer_HeadAnimTable() {
    { // 0x00179280 - 0x0017A06C
        class xAnimTransition *tran; // r17
        class xAnimTable *table; // r16
    }
}

// 0x0017A070 - 0x0017A1C8 (0x158) PlayerFaceUpdate__Ff
static void PlayerFaceUpdate(float dt) {
    { // 0x0017A070 - 0x0017A1C8
        class xAnimState *astate; // r6
    }
}

// 0x0017A1D0 - 0x0017A26C (0x9C) FaceIdle__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FaceIdle() {
    { // 0x0017A1D0 - 0x0017A26C
    }
}

// 0x0017A270 - 0x0017A30C (0x9C) FaceSwing__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FaceSwing() {
    { // 0x0017A270 - 0x0017A30C
    }
}

// 0x0017A310 - 0x0017A3AC (0x9C) FaceSneak__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FaceSneak() {
    { // 0x0017A310 - 0x0017A3AC
    }
}

// 0x0017A3B0 - 0x0017A44C (0x9C) FaceTeeter__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FaceTeeter() {
    { // 0x0017A3B0 - 0x0017A44C
    }
}

// 0x0017A450 - 0x0017A4E8 (0x98) FacePant__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FacePant() {
    { // 0x0017A450 - 0x0017A4E8
    }
}

// 0x0017A4F0 - 0x0017A560 (0x70) FaceRun__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FaceRun() {
    { // 0x0017A4F0 - 0x0017A560
    }
}

// 0x0017A560 - 0x0017A5F4 (0x94) FaceEvent__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FaceEvent(class xAnimTransition *tran) {
    { // 0x0017A560 - 0x0017A5F4
    }
}

// 0x0017A600 - 0x0017A64C (0x4C) FaceScare__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FaceScare(class xAnimTransition *tran) {
    { // 0x0017A600 - 0x0017A64C
    }
}

// 0x0017A650 - 0x0017A660 (0x10) FaceClearScareCB__FP15xAnimTransitionP11xAnimSingle
static unsigned int FaceClearScareCB() {
    { // 0x0017A650 - 0x0017A660
    }
}

// 0x0017A660 - 0x0017A674 (0x14) FaceMad__FP15xAnimTransitionP11xAnimSingle
static unsigned int FaceMad() {
    { // 0x0017A660 - 0x0017A674
    }
}

// 0x0017A680 - 0x0017A694 (0x14) FaceBlow__FP15xAnimTransitionP11xAnimSingle
static unsigned int FaceBlow() {
    { // 0x0017A680 - 0x0017A694
    }
}

// 0x0017A6A0 - 0x0018098C (0x62EC) zEntPlayer_AnimTable__Fv
class xAnimTable *zEntPlayer_AnimTable() {
    { // 0x0017A6A0 - 0x0018098C
        class xAnimState *currst; // r16
        class xAnimTable *table; // r17
        class xAnimTransition *tran; // r16
    }
}

// 0x00180990 - 0x00180B1C (0x18C) DestructObjHBCallback__FP5_zEntP8_xCollis
static signed int DestructObjHBCallback(class _zEnt *ent, class _xCollis *coll) {
    { // 0x00180990 - 0x00180B1C
        unsigned int dobjState; // r17
        unsigned int headResponse; // r16
    }
}

// 0x00180B20 - 0x00180D60 (0x240) DestructObjBBCheck__FP5_xEnt
void DestructObjBBCheck(class _xEnt *ent) {
    { // 0x00180B20 - 0x00180D60
        unsigned int buttonState; // r2
        unsigned int bounceResponse; // r20
        class _xCollis *endcoll; // r17
        class _xCollis *coll; // r16
    }
}

// 0x00180D60 - 0x00180ED4 (0x174) ButtonBBCheck__FP5_xEnt
void ButtonBBCheck(class _xEnt *ent) {
    { // 0x00180D60 - 0x00180ED4
        unsigned int buttonState; // r2
        unsigned int bounceResponse; // r19
        class _xCollis *endcoll; // r17
        class _xCollis *coll; // r16
    }
}

// 0x00180EE0 - 0x0018103C (0x15C) ButtonHBCallback__FP5_zEntP8_xCollis
static signed int ButtonHBCallback(class _zEnt *ent, class _xCollis *coll) {
    { // 0x00180EE0 - 0x0018103C
        unsigned int buttonState; // r2
        unsigned int headResponse; // r16
    }
}

// 0x00181040 - 0x001810E8 (0xA8) ButtonStandOnCheck__FP5_xEnt
void ButtonStandOnCheck(class _xEnt *ent) {
    { // 0x00181040 - 0x001810E8
        unsigned int buttonState; // r2
        unsigned int response; // r17
        class _zEnt *bn; // r2
        class _xCollis *coll; // r16
    }
}

// 0x001810F0 - 0x001811D8 (0xE8) SD0_TossCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int SD0_TossCB() {
    { // 0x001810F0 - 0x001811D8
    }
}

// 0x001811E0 - 0x001811F0 (0x10) ClearHelmetCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ClearHelmetCB() {
    { // 0x001811E0 - 0x001811F0
    }
}

// 0x001811F0 - 0x00181270 (0x80) FallToDeathDropCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FallToDeathDropCB() {
    { // 0x001811F0 - 0x00181270
    }
}

// 0x00181270 - 0x001812A0 (0x30) FallToDeathDrop__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FallToDeathDrop() {
    { // 0x00181270 - 0x001812A0
    }
}

// 0x001812A0 - 0x001812CC (0x2C) FallToDeathStart__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FallToDeathStart() {
    { // 0x001812A0 - 0x001812CC
    }
}

// 0x001812D0 - 0x00181398 (0xC8) DeathCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int DeathCB() {
    { // 0x001812D0 - 0x00181398
        unsigned int deathSnd; // r2
    }
}

// 0x001813A0 - 0x00181400 (0x60) HangNoSwingCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HangNoSwingCheck() {
    { // 0x001813A0 - 0x00181400
    }
}

// 0x00181400 - 0x0018145C (0x5C) HangSwingCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HangSwingCheck() {
    { // 0x00181400 - 0x0018145C
    }
}

// 0x00181460 - 0x00181700 (0x2A0) HangCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HangCB() {
    { // 0x00181460 - 0x00181700
        float spinAngle; // r29+0x70
        float scoobAngle; // r29+0x70
        float hangAngle; // 
        class _xVec3 targetPos; // r29+0x60
        class _zEnt *ent; // r16
    }
}

// 0x00181700 - 0x00181710 (0x10) PowerupCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int PowerupCB() {
    { // 0x00181700 - 0x00181710
    }
}

// 0x00181710 - 0x00181728 (0x18) PowerupCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int PowerupCheck(class xAnimTransition *tran) {
    { // 0x00181710 - 0x00181728
    }
}

// 0x00181730 - 0x00181744 (0x14) NoHangCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NoHangCheck() {
    { // 0x00181730 - 0x00181744
    }
}

// 0x00181750 - 0x00181760 (0x10) HangCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HangCheck() {
    { // 0x00181750 - 0x00181760
    }
}

// 0x00181760 - 0x00181774 (0x14) NotFloatCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NotFloatCheck() {
    { // 0x00181760 - 0x00181774
    }
}

// 0x00181780 - 0x00181858 (0xD8) FloatCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FloatCheck(class xAnimTransition *tran) {
    { // 0x00181780 - 0x00181858
    }
}

// 0x00181860 - 0x001818E0 (0x80) SlideCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int SlideCheck() {
    { // 0x00181860 - 0x001818E0
    }
}

// 0x001818E0 - 0x001818F4 (0x14) ButtSmashStateCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ButtSmashStateCB() {
    { // 0x001818E0 - 0x001818F4
    }
}

// 0x00181900 - 0x0018193C (0x3C) ButtSmashCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ButtSmashCB() {
    { // 0x00181900 - 0x0018193C
    }
}

// 0x00181940 - 0x001819C8 (0x88) ButtSmashCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ButtSmashCheck() {
    { // 0x00181940 - 0x001819C8
    }
}

// 0x001819D0 - 0x001819E0 (0x10) HeadHitCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HeadHitCB() {
    { // 0x001819D0 - 0x001819E0
    }
}

// 0x001819E0 - 0x001819EC (0xC) HeadHitCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HeadHitCheck() {
    { // 0x001819E0 - 0x001819EC
    }
}

// 0x001819F0 - 0x00181A38 (0x48) AttackFinishedHotsauce__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int AttackFinishedHotsauce() {
    { // 0x001819F0 - 0x00181A38
    }
}

// 0x00181A40 - 0x00181ABC (0x7C) AttackFinishedRun__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int AttackFinishedRun(class xAnimTransition *tran) {
    { // 0x00181A40 - 0x00181ABC
    }
}

// 0x00181AC0 - 0x00181B1C (0x5C) AttackFinished__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int AttackFinished(class xAnimTransition *tran) {
    { // 0x00181AC0 - 0x00181B1C
    }
}

// 0x00181B20 - 0x00181B44 (0x24) AttackCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int AttackCB() {
    { // 0x00181B20 - 0x00181B44
    }
}

// 0x00181B50 - 0x00181BD0 (0x80) AttackCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int AttackCheck() {
    { // 0x00181B50 - 0x00181BD0
    }
}

// 0x00181BD0 - 0x00181BF4 (0x24) ProjectileCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ProjectileCB() {
    { // 0x00181BD0 - 0x00181BF4
    }
}

// 0x00181C00 - 0x00181CD4 (0xD4) ProjectileRepeat__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ProjectileRepeat(class xAnimSingle *anim) {
    { // 0x00181C00 - 0x00181CD4
    }
}

// 0x00181CE0 - 0x00181DC8 (0xE8) ProjectileRepeatSync__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ProjectileRepeatSync(class xAnimSingle *anim) {
    { // 0x00181CE0 - 0x00181DC8
    }
}

// 0x00181DD0 - 0x00181E78 (0xA8) ProjectileCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ProjectileCheck() {
    { // 0x00181DD0 - 0x00181E78
    }
}

// 0x00181E80 - 0x00181EEC (0x6C) StopDigCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int StopDigCheck(class xAnimTransition *tran) {
    { // 0x00181E80 - 0x00181EEC
    }
}

// 0x00181EF0 - 0x00181F28 (0x38) DigCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int DigCB() {
    { // 0x00181EF0 - 0x00181F28
    }
}

// 0x00181F30 - 0x00181FB8 (0x88) DigCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int DigCheck() {
    { // 0x00181F30 - 0x00181FB8
    }
}

// 0x00181FC0 - 0x00181FEC (0x2C) NospringCheck__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int NospringCheck(class _zEnt *ent) {
    { // 0x00181FC0 - 0x00181FEC
    }
}

// 0x00181FF0 - 0x00182004 (0x14) SpringCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int SpringCheck() {
    { // 0x00181FF0 - 0x00182004
    }
}

// 0x00182010 - 0x001821D0 (0x1C0) ButtSmashLandCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ButtSmashLandCB() {
    { // 0x00182010 - 0x001821D0
        signed int i; // r16
        class _xCollis *c; // r2
        class _xEnt *ent; // r2
    }
}

// 0x001821D0 - 0x001821F0 (0x20) BounceCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int BounceCB() {
    { // 0x001821D0 - 0x001821F0
    }
}

// 0x001821F0 - 0x00182204 (0x14) BounceCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int BounceCheck() {
    { // 0x001821F0 - 0x00182204
    }
}

// 0x00182210 - 0x001822CC (0xBC) DblJumpCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int DblJumpCB() {
    { // 0x00182210 - 0x001822CC
    }
}

// 0x001822D0 - 0x00182374 (0xA4) DblJumpCheckTime__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int DblJumpCheckTime(class xAnimSingle *anim) {
    { // 0x001822D0 - 0x00182374
    }
}

// 0x00182380 - 0x001823F0 (0x70) DblJumpCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int DblJumpCheck() {
    { // 0x00182380 - 0x001823F0
    }
}

// 0x001823F0 - 0x00182478 (0x88) JumpHangCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int JumpHangCB() {
    { // 0x001823F0 - 0x00182478
    }
}

// 0x00182480 - 0x0018253C (0xBC) JumpCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int JumpCB() {
    { // 0x00182480 - 0x0018253C
    }
}

// 0x00182540 - 0x0018267C (0x13C) JumpCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int JumpCheck() {
    { // 0x00182540 - 0x0018267C
        class _xCollis *flc; // r7
    }
}

// 0x00182680 - 0x001826DC (0x5C) ActiveCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ActiveCheck(class xAnimTransition *tran) {
    { // 0x00182680 - 0x001826DC
    }
}

// 0x001826E0 - 0x00182708 (0x28) IdleGetupCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleGetupCB() {
    { // 0x001826E0 - 0x00182708
    }
}

// 0x00182710 - 0x00182738 (0x28) IdleSitCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleSitCB() {
    { // 0x00182710 - 0x00182738
    }
}

// 0x00182740 - 0x00182784 (0x44) IdleMajorCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleMajorCB() {
    { // 0x00182740 - 0x00182784
    }
}

// 0x00182790 - 0x001827CC (0x3C) IdleMinorCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleMinorCB() {
    { // 0x00182790 - 0x001827CC
    }
}

// 0x001827D0 - 0x0018281C (0x4C) IdleGetupCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleGetupCheck(class xAnimSingle *anim) {
    { // 0x001827D0 - 0x0018281C
    }
}

// 0x00182820 - 0x00182850 (0x30) IdleSitCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleSitCheck() {
    { // 0x00182820 - 0x00182850
    }
}

// 0x00182850 - 0x001828BC (0x6C) IdleMajorCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleMajorCheck(class xAnimTransition *tran) {
    { // 0x00182850 - 0x001828BC
    }
}

// 0x001828C0 - 0x0018292C (0x6C) IdleMinorCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleMinorCheck(class xAnimTransition *tran) {
    { // 0x001828C0 - 0x0018292C
    }
}

// 0x00182930 - 0x00182944 (0x14) FallingCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int FallingCheck() {
    { // 0x00182930 - 0x00182944
    }
}

// 0x00182950 - 0x0018299C (0x4C) LandHealthCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LandHealthCheck(class xAnimTransition *tran) {
    { // 0x00182950 - 0x0018299C
    }
}

// 0x001829A0 - 0x001829D4 (0x34) LandFastCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LandFastCheck() {
    { // 0x001829A0 - 0x001829D4
    }
}

// 0x001829E0 - 0x00182A14 (0x34) LandWalkCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LandWalkCheck() {
    { // 0x001829E0 - 0x00182A14
    }
}

// 0x00182A20 - 0x00182A54 (0x34) LandSlowCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LandSlowCheck() {
    { // 0x00182A20 - 0x00182A54
    }
}

// 0x00182A60 - 0x00182AAC (0x4C) ScareLandCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ScareLandCheck() {
    { // 0x00182A60 - 0x00182AAC
    }
}

// 0x00182AB0 - 0x00182AC4 (0x14) LandCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int LandCheck() {
    { // 0x00182AB0 - 0x00182AC4
    }
}

// 0x00182AD0 - 0x00182BC4 (0xF4) ScareKnockbackCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ScareKnockbackCB() {
    { // 0x00182AD0 - 0x00182BC4
        class _xVec3 vel; // r29+0x40
    }
}

// 0x00182BD0 - 0x00182C34 (0x64) ScareCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ScareCB() {
    { // 0x00182BD0 - 0x00182C34
    }
}

// 0x00182C40 - 0x00182CF0 (0xB0) ScareCheckCritter__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ScareCheckCritter() {
    { // 0x00182C40 - 0x00182CF0
    }
}

// 0x00182CF0 - 0x00182D1C (0x2C) ScareCheckNoHealth__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ScareCheckNoHealth() {
    { // 0x00182CF0 - 0x00182D1C
    }
}

// 0x00182D20 - 0x00182D7C (0x5C) UncowerCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int UncowerCheck(class xAnimTransition *tran) {
    { // 0x00182D20 - 0x00182D7C
    }
}

// 0x00182D80 - 0x00182DAC (0x2C) CowerCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int CowerCheck() {
    { // 0x00182D80 - 0x00182DAC
    }
}

// 0x00182DB0 - 0x00182E0C (0x5C) ScareCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int ScareCheck(class xAnimTransition *tran) {
    { // 0x00182DB0 - 0x00182E0C
    }
}

// 0x00182E10 - 0x00182E68 (0x58) NosneakPlunger__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NosneakPlunger() {
    { // 0x00182E10 - 0x00182E68
    }
}

// 0x00182E70 - 0x00182EF0 (0x80) NosneakSlippery__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NosneakSlippery() {
    { // 0x00182E70 - 0x00182EF0
    }
}

// 0x00182EF0 - 0x00182FE0 (0xF0) NosneakTeeter__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NosneakTeeter() {
    { // 0x00182EF0 - 0x00182FE0
    }
}

// 0x00182FE0 - 0x00183034 (0x54) NosneakCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NosneakCheck(class xAnimTransition *tran) {
    { // 0x00182FE0 - 0x00183034
    }
}

// 0x00183040 - 0x00183080 (0x40) SneakIdleCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int SneakIdleCheck(class xAnimTransition *tran) {
    { // 0x00183040 - 0x00183080
    }
}

// 0x00183080 - 0x001830D8 (0x58) SneakMoveCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int SneakMoveCheck(class xAnimTransition *tran) {
    { // 0x00183080 - 0x001830D8
    }
}

// 0x001830E0 - 0x0018311C (0x3C) SkidStopCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int SkidStopCheck() {
    { // 0x001830E0 - 0x0018311C
    }
}

// 0x00183120 - 0x001831E8 (0xC8) IdleCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int IdleCheck(class xAnimTransition *tran) {
    { // 0x00183120 - 0x001831E8
    }
}

// 0x001831F0 - 0x0018323C (0x4C) WalkPlungeCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int WalkPlungeCheck() {
    { // 0x001831F0 - 0x0018323C
    }
}

// 0x00183240 - 0x001832D4 (0x94) StickNoBootsWalk__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int StickNoBootsWalk() {
    { // 0x00183240 - 0x001832D4
    }
}

// 0x001832E0 - 0x00183374 (0x94) StickNoBootsIdle__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int StickNoBootsIdle() {
    { // 0x001832E0 - 0x00183374
    }
}

// 0x00183380 - 0x001833AC (0x2C) StickySurfWalk__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int StickySurfWalk() {
    { // 0x00183380 - 0x001833AC
    }
}

// 0x001833B0 - 0x001833DC (0x2C) StickySurfIdle__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int StickySurfIdle() {
    { // 0x001833B0 - 0x001833DC
    }
}

// 0x001833E0 - 0x001834D0 (0xF0) WalkCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int WalkCheck(class xAnimTransition *tran) {
    { // 0x001833E0 - 0x001834D0
    }
}

// 0x001834D0 - 0x001834E4 (0x14) NotRunCB__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NotRunCB() {
    { // 0x001834D0 - 0x001834E4
    }
}

// 0x001834F0 - 0x001835C4 (0xD4) NotRunCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int NotRunCheck(class xAnimTransition *tran) {
    { // 0x001834F0 - 0x001835C4
    }
}

// 0x001835D0 - 0x001836CC (0xFC) RunCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int RunCheck(class xAnimTransition *tran) {
    { // 0x001835D0 - 0x001836CC
    }
}

// 0x001836D0 - 0x001836E4 (0x14) HotsauceCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HotsauceCheck() {
    { // 0x001836D0 - 0x001836E4
    }
}

// 0x001836F0 - 0x0018374C (0x5C) SlipperyCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int SlipperyCheck() {
    { // 0x001836F0 - 0x0018374C
    }
}

// 0x00183750 - 0x0018381C (0xCC) TeeterCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int TeeterCheck() {
    { // 0x00183750 - 0x0018381C
    }
}

// 0x00183820 - 0x00183854 (0x34) PlungerCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int PlungerCheck() {
    { // 0x00183820 - 0x00183854
    }
}

// 0x00183860 - 0x00183888 (0x28) HealthCheck__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int HealthCheck(class xAnimTransition *tran) {
    { // 0x00183860 - 0x00183888
    }
}

// 0x00183890 - 0x001839F4 (0x164) VillHBCallback__FP5_zEntP8_xCollis
static signed int VillHBCallback(class _zEnt *ent, class _xCollis *coll) {
    { // 0x00183890 - 0x001839F4
        signed int headResponse; // r2
    }
}

// 0x00183A00 - 0x00183B7C (0x17C) HeadbuttTest__FP8_xCollisP8_xCollisfPFP5_zEntP8_xCollis_i
static void HeadbuttTest(class _xCollis *coll, class _xCollis *endcoll, float angle_cosine, signed int (*headbuttCB)(class _zEnt *, class _xCollis *)) {
    { // 0x00183A00 - 0x00183B7C
        float dz2; // 
        float dx2; // 
        class _zEnt *villent; // r20
        float mag; // r29+0x80
        float dz; // 
        float dx; // 
        class _zEnt *ent; // r16
    }
}

// 0x00183B80 - 0x00183D18 (0x198) HeadcollUpdate__Ff
static void HeadcollUpdate(float dt) {
    { // 0x00183B80 - 0x00183D18
        float t; // 
        float s; // 
        signed int headcoll_animto; // r2
    }
}

// 0x00183D20 - 0x00184028 (0x308) GetHeadcollAnimState__Fv
static signed int GetHeadcollAnimState() {
    { // 0x00183D20 - 0x00184028
        class xAnimState *state; // r16
        static unsigned int FRUN_ID; // 0x00357FE8
        static unsigned int FWALK_ID; // 0x00357FE0
        static unsigned int FNORMAL_ID; // 0x00357FD8
        static unsigned int JSB_ID; // 0x00357FD0
        static unsigned int JRUN_ID; // 0x00357FC8
        static unsigned int JWALK_ID; // 0x00357FC0
        static unsigned int JDOUBLE_ID; // 0x00357FB8
        static unsigned int JNORMAL_ID; // 0x00357FB0
        static signed char init; // 0x00357FEC
        static signed char init; // 0x00357FE4
        static signed char init; // 0x00357FDC
        static signed char init; // 0x00357FD4
        static signed char init; // 0x00357FCC
        static signed char init; // 0x00357FC4
        static signed char init; // 0x00357FBC
        static signed char init; // 0x00357FB4
    }
}

// 0x00184030 - 0x00185BEC (0x1BBC) PlayerAbsControl__FP5_xEntfff
static void PlayerAbsControl(class _xEnt *ent, float x, float z, float dt) {
    { // 0x00184030 - 0x00185BEC
        float slickLerp; // 
        float peakLerp; // 
        float accelMag; // 
        unsigned int moveFlag; // r16
        float turnang; // 
        float dz; // 
        float dx; // 
        float tmpdrot; // 
        class _xVec3 delta; // r29+0x138
        class _xVec3 delta; // r29+0x128
        float downAngle; // r29+0x150
        class _xVec3 D_flat; // r29+0x118
        class _xMat3x3 N_to_Up; // r29+0xb0
        class _xVec3 D; // r29+0x108
        class _xVec3 X; // r29+0xf8
        class _xVec3 *N; // r17
        class _xVec3 euler; // r29+0xe8
        float camAngle; // r29+0x150
        float autodist2d; // 
        float diffAngle; // 
        float turnfactor; // 
        float dir_dp; // 
        float mag; // r29+0x14c
        float angle; // r29+0x148
        unsigned int blendUserFlag; // r2
        unsigned int animUserFlag; // r2
    }
}

// 0x00185BF0 - 0x00185E24 (0x234) DampenControls__FPfPfff
static void DampenControls(float *angle, float *mag, float x, float y) {
    { // 0x00185BF0 - 0x00185E24
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xordarray.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
// 0x00185E30 - 0x00185FA8 (0x178) XOrdSort__FP16st_XORDEREDARRAYPFPCvPv_i
void XOrdSort(class st_XORDEREDARRAY *array, signed int (*test)(void *, void *)) {
    { // 0x00185E30 - 0x00185FA8
        void *v; // r19
        signed int h; // r18
        signed int j; // r17
        signed int i; // r30
        signed int num; // r29+0xa0
        void **list; // r16
    }
}

// 0x00185FB0 - 0x0018607C (0xCC) XOrdLookup__FP16st_XORDEREDARRAYPCvPFPCvPv_i
signed int XOrdLookup(class st_XORDEREDARRAY *array, void *key, signed int (*test)(void *, void *)) {
    { // 0x00185FB0 - 0x0018607C
        signed int v; // r2
        signed int k; // r19
        signed int k1; // r18
        signed int k0; // r17
        signed int da_idx; // r16
    }
}

// 0x00186080 - 0x00186148 (0xC8) XOrdInsert__FP16st_XORDEREDARRAYPvPFPvPv_i
void XOrdInsert(class st_XORDEREDARRAY *array, void *elt, signed int (*compare)(void *, void *)) {
    { // 0x00186080 - 0x00186148
        signed int i; // r16
    }
}

// 0x00186150 - 0x00186180 (0x30) XOrdAppend__FP16st_XORDEREDARRAYPv
void XOrdAppend(class st_XORDEREDARRAY *array, void *elt) {
    { // 0x00186150 - 0x00186180
    }
}

// 0x00186180 - 0x00186194 (0x14) XOrdDone__FP16st_XORDEREDARRAY
void XOrdDone(class st_XORDEREDARRAY *array) {
    { // 0x00186180 - 0x00186194
    }
}

// 0x001861A0 - 0x00186228 (0x88) XOrdInit__FP16st_XORDEREDARRAYi
void XOrdInit(class st_XORDEREDARRAY *array, signed int max) {
    { // 0x001861A0 - 0x00186228
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xbinio.cpp
// ==========================================================================
void (LoadDestroy)(class st_FILELOADINFO *); // 0x00186C80
signed int (ReadBytes)(class st_FILELOADINFO *, char *, signed int); // 0x001869C0
signed int (ReadMShorts)(class st_FILELOADINFO *, signed short *, signed int); // 0x00186960
signed int (ReadMLongs)(class st_FILELOADINFO *, signed int *, signed int); // 0x001868F0
signed int (ReadMFloats)(class st_FILELOADINFO *, float *, signed int); // 0x00186880
signed int (ReadMDoubles)(class st_FILELOADINFO *, double *, signed int); // 0x00186840
signed int (ReadIShorts)(class st_FILELOADINFO *, signed short *, signed int); // 0x00186830
signed int (ReadILongs)(class st_FILELOADINFO *, signed int *, signed int); // 0x00186820
signed int (ReadIFloats)(class st_FILELOADINFO *, float *, signed int); // 0x00186810
signed int (ReadIDoubles)(class st_FILELOADINFO *, double *, signed int); // 0x00186800
signed int (AsyncIRead)(class st_FILELOADINFO *, signed int, char *, signed int, signed int); // 0x001867E0
signed int (AsyncMRead)(class st_FILELOADINFO *, signed int, char *, signed int, signed int); // 0x001867F0
enum en_BIO_ASYNC_ERRCODES (AsyncReadStatus)(class st_FILELOADINFO *); // 0x001867D0
signed int (SkipBytes)(class st_FILELOADINFO *, signed int); // 0x00186BB0
signed int (ReadSeek)(class st_FILELOADINFO *, signed int); // 0x00186AF0
void (SetBuffer)(class st_FILELOADINFO *, char *, signed int); // 0x00186AC0
void (DiscardBuffer)(class st_FILELOADINFO *); // 0x00186A90
void (BFD_cb_asyncread)(signed int); // 0x001863D0
static class st_BINIO_XTRADATA *g_async_context; // 0x00358034
static class st_BINIO_XTRADATA g_xtraload[8]; // 0x002985E0
static class tag_xFile g_xfload[8]; // 0x00297FE0
static class st_FILELOADINFO g_loadinst[8]; // 0x00297CA0
static unsigned int g_loadlock; // 0x00357584
// 0x00186230 - 0x001862E8 (0xB8) BFD_AsyncReadStatus__FP15st_FILELOADINFO
static enum en_BIO_ASYNC_ERRCODES BFD_AsyncReadStatus(class st_FILELOADINFO *fli) {
    { // 0x00186230 - 0x001862E8
        enum XFILE_READSECTOR_STATUS xrdstat; // r2
        class st_BINIO_XTRADATA *xtra; // r16
        enum en_BIO_ASYNC_ERRCODES status; // r2
    }
}

// 0x001862F0 - 0x001863CC (0xDC) BFD_AsyncRead__FP15st_FILELOADINFOiPviii
static signed int BFD_AsyncRead(class st_FILELOADINFO *fli, signed int pos, void *data, signed int size, signed int n) {
    { // 0x001862F0 - 0x001863CC
        signed int xsectsize; // r2
        unsigned int rdsec; // r2
        class st_BINIO_XTRADATA *xtra; // r3
    }
}

// 0x001863D0 - 0x001863E0 (0x10) BFD_cb_asyncread__Fi
static void BFD_cb_asyncread() {
    { // 0x001863D0 - 0x001863E0
    }
}

// 0x001863E0 - 0x0018661C (0x23C) BFD_read__FPviiP9tag_xFilePv
static signed int BFD_read(void *data, signed int elesize, signed int elecnt, class tag_xFile *bffp, void *xtradata) {
    { // 0x001863E0 - 0x0018661C
        unsigned int numBytes; // r23
        unsigned int safety; // r19
        unsigned int holdpos; // r20
        signed int actual; // r18
        signed int remain; // r17
        signed int refill; // r22
        signed int readbeg; // r3
        char *dest; // r30
        class st_BINIO_XTRADATA *xtra; // r16
    }
}

// 0x00186620 - 0x0018665C (0x3C) BFD_close__FP9tag_xFilePv
static void BFD_close(class tag_xFile *bffp, void *xtradata) {
    { // 0x00186620 - 0x0018665C
        class st_BINIO_XTRADATA *xtra; // r16
    }
}

// 0x00186660 - 0x00186758 (0xF8) BFD_open__FPCcPCcUiiPv
static class tag_xFile *BFD_open(char *filename, char *mode, unsigned int lockid, void *xtradata) {
    { // 0x00186660 - 0x00186758
        signed int xfflg; // r19
        class st_BINIO_XTRADATA *xtra; // r16
        unsigned int orc; // r2
        class tag_xFile *bffp; // r18
    }
}

// 0x00186760 - 0x001867C8 (0x68) Swap8__FPci
static void Swap8(char *d, signed int n) {
    { // 0x00186760 - 0x001867C8
        char t; // r2
    }
}

// 0x001867D0 - 0x001867D8 (0x8) AsyncReadStatus__FP15st_FILELOADINFO
static enum en_BIO_ASYNC_ERRCODES AsyncReadStatus(class st_FILELOADINFO *fli) {
    { // 0x001867D0 - 0x001867D8
    }
}

// 0x001867E0 - 0x001867E8 (0x8) AsyncIRead__FP15st_FILELOADINFOiPcii
static signed int AsyncIRead(class st_FILELOADINFO *fli, signed int offset, char *data, signed int size, signed int n) {
    { // 0x001867E0 - 0x001867E8
    }
}

// 0x001867F0 - 0x001867F8 (0x8) AsyncMRead__FP15st_FILELOADINFOiPcii
static signed int AsyncMRead(class st_FILELOADINFO *fli, signed int offset, char *data, signed int size, signed int n) {
    { // 0x001867F0 - 0x001867F8
    }
}

// 0x00186800 - 0x0018680C (0xC) ReadIDoubles__FP15st_FILELOADINFOPdi
static signed int ReadIDoubles(class st_FILELOADINFO *fli, double *data, signed int count) {
    { // 0x00186800 - 0x0018680C
        signed int act; // r2
    }
}

// 0x00186810 - 0x0018681C (0xC) ReadIFloats__FP15st_FILELOADINFOPfi
static signed int ReadIFloats(class st_FILELOADINFO *fli, float *data, signed int count) {
    { // 0x00186810 - 0x0018681C
        signed int act; // r2
    }
}

// 0x00186820 - 0x0018682C (0xC) ReadILongs__FP15st_FILELOADINFOPii
static signed int ReadILongs(class st_FILELOADINFO *fli, signed int *data, signed int count) {
    { // 0x00186820 - 0x0018682C
        signed int act; // r2
    }
}

// 0x00186830 - 0x0018683C (0xC) ReadIShorts__FP15st_FILELOADINFOPsi
static signed int ReadIShorts(class st_FILELOADINFO *fli, signed short *data, signed int count) {
    { // 0x00186830 - 0x0018683C
        signed int act; // r2
    }
}

// 0x00186840 - 0x00186878 (0x38) ReadMDoubles__FP15st_FILELOADINFOPdi
static signed int ReadMDoubles(class st_FILELOADINFO *fli, double *data, signed int count) {
    { // 0x00186840 - 0x00186878
        signed int act; // r2
    }
}

// 0x00186880 - 0x001868F0 (0x70) ReadMFloats__FP15st_FILELOADINFOPfi
static signed int ReadMFloats(class st_FILELOADINFO *fli, float *data, signed int count) {
    { // 0x00186880 - 0x001868F0
        signed int act; // r2
    }
}

// 0x001868F0 - 0x00186960 (0x70) ReadMLongs__FP15st_FILELOADINFOPii
static signed int ReadMLongs(class st_FILELOADINFO *fli, signed int *data, signed int count) {
    { // 0x001868F0 - 0x00186960
        signed int act; // r2
    }
}

// 0x00186960 - 0x001869C0 (0x60) ReadMShorts__FP15st_FILELOADINFOPsi
static signed int ReadMShorts(class st_FILELOADINFO *fli, signed short *data, signed int count) {
    { // 0x00186960 - 0x001869C0
        signed int act; // r2
    }
}

// 0x001869C0 - 0x001869CC (0xC) ReadBytes__FP15st_FILELOADINFOPci
static signed int ReadBytes(class st_FILELOADINFO *fli, char *data, signed int count) {
    { // 0x001869C0 - 0x001869CC
        signed int act; // r2
    }
}

// 0x001869D0 - 0x00186A8C (0xBC) ReadRaw__FP15st_FILELOADINFOPvii
static signed int ReadRaw(class st_FILELOADINFO *fli, void *data, signed int size, signed int count) {
    { // 0x001869D0 - 0x00186A8C
        signed int n; // r2
        signed int amt; // r16
        class tag_xFile *file; // r3
    }
}

// 0x00186A90 - 0x00186AB8 (0x28) DiscardBuffer__FP15st_FILELOADINFO
static void DiscardBuffer(class st_FILELOADINFO *fli) {
    { // 0x00186A90 - 0x00186AB8
    }
}

// 0x00186AC0 - 0x00186AE8 (0x28) SetBuffer__FP15st_FILELOADINFOPci
static void SetBuffer(class st_FILELOADINFO *fli, char *dblbuffer, signed int bufsize) {
    { // 0x00186AC0 - 0x00186AE8
        class st_BINIO_XTRADATA *xtra; // r3
    }
}

// 0x00186AF0 - 0x00186BA4 (0xB4) ReadSeek__FP15st_FILELOADINFOi
static signed int ReadSeek(class st_FILELOADINFO *fli, signed int pos) {
    { // 0x00186AF0 - 0x00186BA4
        class tag_xFile *file; // r2
    }
}

// 0x00186BB0 - 0x00186C80 (0xD0) SkipBytes__FP15st_FILELOADINFOi
static signed int SkipBytes(class st_FILELOADINFO *fli, signed int fwd) {
    { // 0x00186BB0 - 0x00186C80
        class tag_xFile *file; // r2
    }
}

// 0x00186C80 - 0x00186D00 (0x80) LoadDestroy__FP15st_FILELOADINFO
static void LoadDestroy(class st_FILELOADINFO *fli) {
    { // 0x00186C80 - 0x00186D00
        unsigned int lockid; // r16
        class tag_xFile *fp; // r2
    }
}

// 0x00186D00 - 0x00186FA8 (0x2A8) xBinioLoadCreate__FPCc
class st_FILELOADINFO *xBinioLoadCreate(char *filename) {
    { // 0x00186D00 - 0x00186FA8
        signed int uselock; // r17
        signed int i; // r5
        class tag_xFile *tmp_fp; // r17
        class st_FILELOADINFO *fli; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xhipio.cpp
// ==========================================================================
static class st_HIPSAVEFUNCS g_map_HIPS_funcmap; // 0x00000000
static class st_HIPLOADFUNCS g_map_HIPL_funcmap; // 0x00298CE0
static class st_HIPLOADDATA g_hiploadinst[8]; // 0x002987A0
static unsigned int g_loadlock; // 0x00358038
// 0x00186FB0 - 0x001870B8 (0x108) HIPLReadString__FP14st_HIPLOADDATAPc
static signed int HIPLReadString(class st_HIPLOADDATA *lddata, char *buf) {
    { // 0x00186FB0 - 0x001870B8
        char pad; // r29+0x5f
        signed int n; // r16
    }
}

// 0x001870C0 - 0x00187110 (0x50) HIPLReadFloats__FP14st_HIPLOADDATAPfi
static signed int HIPLReadFloats(class st_HIPLOADDATA *lddata, float *data, signed int cnt) {
    { // 0x001870C0 - 0x00187110
        signed int got; // r2
    }
}

// 0x00187110 - 0x00187160 (0x50) HIPLReadLongs__FP14st_HIPLOADDATAPii
static signed int HIPLReadLongs(class st_HIPLOADDATA *lddata, signed int *data, signed int cnt) {
    { // 0x00187110 - 0x00187160
        signed int got; // r2
    }
}

// 0x00187160 - 0x001871B0 (0x50) HIPLReadShorts__FP14st_HIPLOADDATAPsi
static signed int HIPLReadShorts(class st_HIPLOADDATA *lddata, signed short *data, signed int cnt) {
    { // 0x00187160 - 0x001871B0
        signed int got; // r2
    }
}

// 0x001871B0 - 0x001871E8 (0x38) HIPLReadBytes__FP14st_HIPLOADDATAPci
static signed int HIPLReadBytes(class st_HIPLOADDATA *lddata, char *data, signed int cnt) {
    { // 0x001871B0 - 0x001871E8
    }
}

// 0x001871F0 - 0x0018729C (0xAC) HIPLPollRead__FP14st_HIPLOADDATA
static enum en_READ_ASYNC_STATUS HIPLPollRead(class st_HIPLOADDATA *lddata) {
    { // 0x001871F0 - 0x0018729C
        enum en_BIO_ASYNC_ERRCODES pollstat; // r2
        enum en_READ_ASYNC_STATUS rdstat; // r17
    }
}

// 0x001872A0 - 0x00187394 (0xF4) HIPLBypassRead__FP14st_HIPLOADDATAPvii
static signed int HIPLBypassRead(class st_HIPLOADDATA *lddata, void *data, signed int cnt, signed int size) {
    { // 0x001872A0 - 0x00187394
        signed int got; // r2
    }
}

// 0x001873A0 - 0x001874F0 (0x150) HIPLBlockRead__FP14st_HIPLOADDATAPvii
static signed int HIPLBlockRead(class st_HIPLOADDATA *lddata, void *data, signed int cnt, signed int size) {
    { // 0x001873A0 - 0x001874F0
        signed int head; // r7
        signed int left; // r4
        signed int got; // r2
        class st_HIPLOADBLOCK *top; // r16
    }
}

// 0x001874F0 - 0x00187558 (0x68) HIPLBlockExit__FP14st_HIPLOADDATA
static void HIPLBlockExit(class st_HIPLOADDATA *lddata) {
    { // 0x001874F0 - 0x00187558
        class st_HIPLOADBLOCK *top; // r17
    }
}

// 0x00187560 - 0x001876BC (0x15C) HIPLBlockEnter__FP14st_HIPLOADDATA
static unsigned int HIPLBlockEnter(class st_HIPLOADDATA *lddata) {
    { // 0x00187560 - 0x001876BC
        signed int size; // r29+0x2c
        unsigned int cid; // r29+0x28
        class st_HIPLOADBLOCK *top; // r2
    }
}

// 0x001876C0 - 0x001876D8 (0x18) HIPLSetSpot__FP14st_HIPLOADDATAi
static void HIPLSetSpot(class st_HIPLOADDATA *lddata, signed int spot) {
    { // 0x001876C0 - 0x001876D8
    }
}

// 0x001876E0 - 0x001877A0 (0xC0) HIPLSetBypass__FP14st_HIPLOADDATAii
static signed int HIPLSetBypass(class st_HIPLOADDATA *lddata, signed int enable, signed int use_async) {
    { // 0x001876E0 - 0x001877A0
    }
}

// 0x001877A0 - 0x001877A8 (0x8) HIPLBaseSector__FP14st_HIPLOADDATA
static unsigned int HIPLBaseSector(class st_HIPLOADDATA *lddata) {
    { // 0x001877A0 - 0x001877A8
    }
}

// 0x001877B0 - 0x00187820 (0x70) HIPLDestroy__FP14st_HIPLOADDATA
static void HIPLDestroy(class st_HIPLOADDATA *lddata) {
    { // 0x001877B0 - 0x00187820
        signed int lockid; // r16
    }
}

// 0x00187820 - 0x00187A24 (0x204) HIPLCreate__FPCcPci
static class st_HIPLOADDATA *HIPLCreate(char *filename, char *dblbuf, signed int bufsize) {
    { // 0x00187820 - 0x00187A24
        signed int uselock; // r17
        signed int i; // r5
        class st_HIPLOADBLOCK *tmp_blk; // r2
        class st_FILELOADINFO *fli; // r2
        class st_HIPLOADDATA *lddata; // r16
    }
}

// 0x00187A30 - 0x00187A3C (0xC) get_HIPLFuncs__Fv
class st_HIPLOADFUNCS *get_HIPLFuncs() {
    { // 0x00187A30 - 0x00187A3C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xIni.cpp
// ==========================================================================
// 0x00187A40 - 0x00187AFC (0xBC) xIniGetString__FP8xIniFilePcPc
char *xIniGetString(class xIniFile *ini, char *tok, char *def) {
    { // 0x00187A40 - 0x00187AFC
    }
}

// 0x00187B00 - 0x00187BC4 (0xC4) xIniGetFloat__FP8xIniFilePcf
float xIniGetFloat(class xIniFile *ini, char *tok, float def) {
    { // 0x00187B00 - 0x00187BC4
    }
}

// 0x00187BD0 - 0x00187C8C (0xBC) xIniGetInt__FP8xIniFilePci
signed int xIniGetInt(class xIniFile *ini, char *tok, signed int def) {
    { // 0x00187BD0 - 0x00187C8C
    }
}

// 0x00187C90 - 0x00187CC0 (0x30) xIniDestroy__FP8xIniFile
void xIniDestroy(class xIniFile *ini) {
    { // 0x00187C90 - 0x00187CC0
    }
}

// 0x00187CC0 - 0x001881F0 (0x530) xIniParse__FPci
class xIniFile *xIniParse(char *buf, signed int len) {
    { // 0x00187CC0 - 0x001881F0
        class xIniFile *ini; // r20
        char *ltoken; // r29+0x7c
        char *line; // r2
        char *lastLine; // r17
        char *c; // r2
        signed int lastCRLF; // r19
        signed int copen; // r18
        signed int clf; // r5
        signed int ccr; // r17
        signed int i; // r4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zAssetTypes.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
class RwTexture * (TexCB)(class RwTexture *, void *); // 0x001889F0
void * (FindAssetCB)(unsigned int, char *); // 0x00188780
unsigned int gxAnimUseGrowAlloc; // 0x00357C50
static unsigned int (*effectFuncList[2])(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x00357588
static class xAnimTable * (*tableFuncList[22])(); // 0x002995E0
static class st_PACKER_ASSETTYPE assetTypeHandlers[56]; // 0x00298D20
// 0x001881F0 - 0x001881F8 (0x8) MovePoint_Unload__FPvUi
static void MovePoint_Unload(void *userdata) {
    { // 0x001881F0 - 0x001881F8
    }
}

// 0x00188200 - 0x00188208 (0x8) Anim_Unload__FPvUi
static void Anim_Unload() {
    { // 0x00188200 - 0x00188208
    }
}

// 0x00188210 - 0x00188780 (0x570) ATBL_Read__FPvUiPvUiPUi
static void *ATBL_Read(unsigned int assetid, void *indata, unsigned int *outsize) {
    { // 0x00188210 - 0x00188780
        class xAnimEffect *effect; // r2
        class zAssetAnimEffect *zaEffect; // r18
        class xAnimState *state; // r17
        class xAnimFile *foundFile; // r4
        char tmpstr[32]; // r29+0xe0
        class xAnimFile **fList; // r23
        class zAssetAnimState *zaState; // r2
        class zAssetAnimFile *zaFile; // r2
        void **zaRaw; // r17
        class zAssetAnimTable *zaTbl; // r16
        unsigned char *zaBytes; // r30
        class xAnimTransition *atran; // r2
        class xAnimState *astate; // r17
        unsigned int tmpsize; // r29+0x10c
        unsigned int debugNum; // r29+0xb0
        unsigned int j; // r3
        unsigned int i; // r18
    }
}

// 0x00188780 - 0x0018879C (0x1C) FindAssetCB__FUiPc
static void *FindAssetCB(unsigned int ID) {
    { // 0x00188780 - 0x0018879C
        unsigned int size; // r29+0x1c
    }
}

// 0x001887A0 - 0x0018887C (0xDC) soundEffectCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int soundEffectCB(unsigned int cbenum, class xAnimActiveEffect *acteffect, void *object) {
    { // 0x001887A0 - 0x0018887C
        class zAnimFxSound *snd; // r16
        class _xEnt *ent_tmp; // r2
        signed int vil_result; // r2
        unsigned int vil_SID; // r29+0x5c
        unsigned int sndhandle; // r16
    }
}

// 0x00188880 - 0x00188888 (0x8) dummyEffectCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int dummyEffectCB() {
    { // 0x00188880 - 0x00188888
    }
}

// 0x00188890 - 0x001888B4 (0x24) TextureRW3_Unload__FPvUi
static void TextureRW3_Unload(void *userdata) {
    { // 0x00188890 - 0x001888B4
        class RwTexture *tex; // r2
    }
}

// 0x001888C0 - 0x001889E4 (0x124) RWTX_Read__FPvUiPvUiPUi
static void *RWTX_Read(void *indata, unsigned int insize, unsigned int *outsize) {
    { // 0x001888C0 - 0x001889E4
        class RwTexture *tex; // r29+0x5c
        class RwStream *stream; // r18
        class RwMemory rwmem; // r29+0x50
        class RwTexDictionary *txd; // r17
    }
}

// 0x001889F0 - 0x00188A08 (0x18) TexCB__FP9RwTexturePv
static class RwTexture *TexCB(class RwTexture *texture, void *data) {
    { // 0x001889F0 - 0x00188A08
        class RwTexture **texFound; // r2
    }
}

// 0x00188A10 - 0x00188A20 (0x10) BSP_Unload__FPvUi
static void BSP_Unload() {
    { // 0x00188A10 - 0x00188A20
    }
}

// 0x00188A20 - 0x00188B2C (0x10C) BSP_Read__FPvUiPvUiPUi
static void *BSP_Read(unsigned int assetid, void *indata, unsigned int insize, unsigned int *outsize) {
    { // 0x00188A20 - 0x00188B2C
        class RpWorld *bsp; // r16
        class RwChunkHeaderInfo chunkHeaderInfo; // r29+0x50
        class RwMemory rwmem; // r29+0x68
        class RwStream *stream; // r19
    }
}

// 0x00188B30 - 0x00188B50 (0x20) Model_Unload__FPvUi
static void Model_Unload(void *userdata) {
    { // 0x00188B30 - 0x00188B50
    }
}

// 0x00188B50 - 0x00188B84 (0x34) Model_Read__FPvUiPvUiPUi
static void *Model_Read(void *indata, unsigned int insize, unsigned int *outsize) {
    { // 0x00188B50 - 0x00188B84
        class RpAtomic *model; // r2
    }
}

// 0x00188B90 - 0x00188B98 (0x8) zAssetShutdown__Fv
void zAssetShutdown() {
    { // 0x00188B90 - 0x00188B98
    }
}

// 0x00188BA0 - 0x00188BAC (0xC) zAssetStartup__Fv
void zAssetStartup() {
    { // 0x00188BA0 - 0x00188BAC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\isavegame.cpp
// ==========================================================================
static char *g_strz_egotrip[8]; // 0x002B07B0
static char *g_isg_scemodule[3]; // 0x002B0658
static class st_ISGSESSION g_isgdata; // 0x002B05C0
static class st_ISG_MEMCARD_DATA g_mcdata; // 0x002B04C0
static signed int g_isginit; // 0x00358048
static char *g_scoobydoo_icon_delete; // 0x00357594
static char *g_scoobydoo_icon_copy; // 0x00357590
static char g_scoobydoo_icon_list[93814]; // 0x00299640
// 0x00188BB0 - 0x0018915C (0x5AC) iSG_is_synccode_realerr__FiiP19st_ISG_MEMCARD_DATA
signed int iSG_is_synccode_realerr(signed int mcop, signed int mcopret) {
    { // 0x00188BB0 - 0x0018915C
        signed int is_ok; // r2
    }
}

// 0x00189160 - 0x001892D4 (0x174) iSG_mcasync_chkop__FP19st_ISG_MEMCARD_DATAiPi
enum en_ISGMCA_STATUS iSG_mcasync_chkop(class st_ISG_MEMCARD_DATA *mcdata, signed int block, signed int *sync_resval) {
    { // 0x00189160 - 0x001892D4
        signed int mcf; // r29+0x6c
        signed int ret; // r29+0x68
        signed int rc; // r2
        enum en_ISGMCA_STATUS result; // r16
    }
}

// 0x001892E0 - 0x00189390 (0xB0) iSG_mca_fwrite__FP19st_ISG_MEMCARD_DATAPci
signed int iSG_mca_fwrite(class st_ISG_MEMCARD_DATA *mcdata, char *data, signed int n) {
    { // 0x001892E0 - 0x00189390
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x00189390 - 0x0018943C (0xAC) iSG_mca_fread__FP19st_ISG_MEMCARD_DATAPci
signed int iSG_mca_fread(class st_ISG_MEMCARD_DATA *mcdata, char *buf, signed int bufsize) {
    { // 0x00189390 - 0x0018943C
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x00189440 - 0x001894DC (0x9C) iSG_mca_fclose__FP19st_ISG_MEMCARD_DATA
signed int iSG_mca_fclose(class st_ISG_MEMCARD_DATA *mcdata) {
    { // 0x00189440 - 0x001894DC
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x001894E0 - 0x00189594 (0xB4) iSG_mca_fopen__FP19st_ISG_MEMCARD_DATAPCc13en_ISG_IOMODE
signed int iSG_mca_fopen(class st_ISG_MEMCARD_DATA *mcdata, char *fname, enum en_ISG_IOMODE mode) {
    { // 0x001894E0 - 0x00189594
        signed int ps2mode; // r2
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x001895A0 - 0x0018964C (0xAC) iSG_mca_chdir__FP19st_ISG_MEMCARD_DATAPc
signed int iSG_mca_chdir(class st_ISG_MEMCARD_DATA *mcdata, char *dname) {
    { // 0x001895A0 - 0x0018964C
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x00189650 - 0x001896F8 (0xA8) iSG_mca_mkdir__FP19st_ISG_MEMCARD_DATAPc
signed int iSG_mca_mkdir(class st_ISG_MEMCARD_DATA *mcdata, char *dname) {
    { // 0x00189650 - 0x001896F8
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x00189700 - 0x001897B8 (0xB8) iSG_mca_unfmt__FP19st_ISG_MEMCARD_DATA
signed int iSG_mca_unfmt(class st_ISG_MEMCARD_DATA *mcdata) {
    { // 0x00189700 - 0x001897B8
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x001897C0 - 0x001898BC (0xFC) iSG_mca_fmt__FP19st_ISG_MEMCARD_DATAi
signed int iSG_mca_fmt(class st_ISG_MEMCARD_DATA *mcdata, signed int force) {
    { // 0x001897C0 - 0x001898BC
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x001898C0 - 0x001899B0 (0xF0) iSG_get_fmoddate__FP19st_ISG_MEMCARD_DATAPCcPiPiPiPiPiPi
signed int iSG_get_fmoddate(class st_ISG_MEMCARD_DATA *mcdata, char *fname, signed int *sec, signed int *min, signed int *hr, signed int *mon, signed int *day, signed int *yr) {
    { // 0x001898C0 - 0x001899B0
        class sceMcTblGetDir *finf; // r16
        signed int rc; // r2
        signed int result; // r23
    }
}

// 0x001899B0 - 0x00189BEC (0x23C) iSG_get_finfo__FP19st_ISG_MEMCARD_DATAPCcPCc
signed int iSG_get_finfo(class st_ISG_MEMCARD_DATA *mcdata, char *fname, char *path) {
    { // 0x001899B0 - 0x00189BEC
        signed int numfound; // r29+0x9c
        signed int len; // r2
        char str_buf[64]; // r29+0x50
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x00189BF0 - 0x00189D88 (0x198) iSG_isSpaceForFile__FP19st_ISG_MEMCARD_DATAiiPCcPCcPiPi
signed int iSG_isSpaceForFile(class st_ISG_MEMCARD_DATA *mcdata, signed int mcidx, signed int fsize, char *dpath, char *fname, signed int *bytesNeeded, signed int *availOnDisk) {
    { // 0x00189BF0 - 0x00189D88
        signed int reset_mcpath; // r23
        signed int totclust; // r2
        signed int estclust; // r16
        signed int xtra_fent; // r2
        signed int fEc_need; // r17
        signed int fc_need; // r16
        signed int rc; // r2
    }
}

// 0x00189D90 - 0x00189E80 (0xF0) iSG_mc_availDirEnt__FP19st_ISG_MEMCARD_DATAiPCc
signed int iSG_mc_availDirEnt(class st_ISG_MEMCARD_DATA *mcdata, signed int mcidx, char *dpath) {
    { // 0x00189D90 - 0x00189E80
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x00189E80 - 0x00189F6C (0xEC) iSG_mc_availclust__FP19st_ISG_MEMCARD_DATAi
signed int iSG_mc_availclust(class st_ISG_MEMCARD_DATA *mcdata, signed int mcidx) {
    { // 0x00189E80 - 0x00189F6C
        signed int clust; // r29+0x3c
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x00189F70 - 0x0018A070 (0x100) iSG_mc_isPSIIcard__FP19st_ISG_MEMCARD_DATAi
signed int iSG_mc_isPSIIcard(class st_ISG_MEMCARD_DATA *mcdata, signed int mcidx) {
    { // 0x00189F70 - 0x0018A070
        signed int type; // r29+0x3c
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018A070 - 0x0018A164 (0xF4) iSG_mc_isformatted__FP19st_ISG_MEMCARD_DATAi
signed int iSG_mc_isformatted(class st_ISG_MEMCARD_DATA *mcdata, signed int mcidx) {
    { // 0x0018A070 - 0x0018A164
        signed int is_fmtd; // r29+0x3c
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018A170 - 0x0018A254 (0xE4) iSG_mc_exists__FP19st_ISG_MEMCARD_DATAi
signed int iSG_mc_exists(class st_ISG_MEMCARD_DATA *mcdata, signed int mcidx) {
    { // 0x0018A170 - 0x0018A254
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018A260 - 0x0018A454 (0x1F4) iSG_mcidx_portslot__FiPiPiPi
signed int iSG_mcidx_portslot(signed int mcidx, signed int *port, signed int *slot, signed int *concnt) {
    { // 0x0018A260 - 0x0018A454
        signed int cur_mcop; // r29+0xac
        signed int use_port; // r20
        signed int con_p1; // r19
        signed int con_p0; // r18
        signed int tp; // r17
        signed int type; // r29+0xa8
        signed int i; // r16
        signed int ret; // r29+0xa4
        signed int rc; // r2
        signed int result; // r23
    }
}

// 0x0018A460 - 0x0018A624 (0x1C4) SQUIB_init_st_iconsys__FP12sceMcIconSys
void SQUIB_init_st_iconsys(class sceMcIconSys *icsys) {
    { // 0x0018A460 - 0x0018A624
        unsigned char sjistitle[68]; // r29+0xd0
        char *iconname; // r2
        float ambient[4]; // r29+0xc0
        float lightcol[3][4]; // r29+0x90
        float lightdir[3][4]; // r29+0x60
        signed int bgcolor[4][4]; // r29+0x20
    }
}

// 0x0018A630 - 0x0018A8D4 (0x2A4) iSG_add_sysicons__FP19st_ISG_MEMCARD_DATA
signed int iSG_add_sysicons(class st_ISG_MEMCARD_DATA *mcdata) {
    { // 0x0018A630 - 0x0018A8D4
        char *iconname; // r2
        class sceMcIconSys icsysdata; // r29+0x30
    }
}

// 0x0018A8E0 - 0x0018AA14 (0x134) iSG_add_cfgholder__FP19st_ISG_MEMCARD_DATA
signed int iSG_add_cfgholder(class st_ISG_MEMCARD_DATA *mcdata) {
    { // 0x0018A8E0 - 0x0018AA14
        char *cfgname; // r2
        char *strptr; // r17
        char cfgdata[992]; // r29+0x40
    }
}

// 0x0018AA20 - 0x0018AAD0 (0xB0) iSG_start_your_engines__Fv
signed int iSG_start_your_engines() {
    { // 0x0018AA20 - 0x0018AAD0
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018AAD0 - 0x0018ACCC (0x1FC) iSGOpError__FP13st_ISGSESSIONPc
enum en_ASYNC_OPERR iSGOpError(class st_ISGSESSION *isgdata, char *errmsg) {
    { // 0x0018AAD0 - 0x0018ACCC
    }
}

// 0x0018ACD0 - 0x0018ADD4 (0x104) iSGPollStatus__FP13st_ISGSESSIONP15en_ASYNC_OPCODEi
enum en_ASYNC_OPSTAT iSGPollStatus(class st_ISGSESSION *isgdata, enum en_ASYNC_OPCODE *curop, signed int block) {
    { // 0x0018ACD0 - 0x0018ADD4
        signed int rc; // r2
    }
}

// 0x0018ADE0 - 0x0018AFEC (0x20C) iSGReadLeader__FP13st_ISGSESSIONPCcPcii
signed int iSGReadLeader(class st_ISGSESSION *isgdata, char *fname, char *databuf, signed int numbytes, signed int async) {
    { // 0x0018ADE0 - 0x0018AFEC
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018AFF0 - 0x0018B07C (0x8C) iSGLoadFile__FP13st_ISGSESSIONPCcPci
signed int iSGLoadFile(class st_ISGSESSION *isgdata, char *fname, char *databuf, signed int async) {
    { // 0x0018AFF0 - 0x0018B07C
    }
}

// 0x0018B080 - 0x0018B2D0 (0x250) iSGSaveFile__FP13st_ISGSESSIONPCcPcii
signed int iSGSaveFile(class st_ISGSESSION *isgdata, char *fname, char *data, signed int n, signed int async) {
    { // 0x0018B080 - 0x0018B2D0
        signed int rc; // r2
        signed int result; // r20
    }
}

// 0x0018B2D0 - 0x0018B560 (0x290) iSGSetupGameDir__FP13st_ISGSESSIONPCci
signed int iSGSetupGameDir(class st_ISGSESSION *isgdata, char *dname, signed int force_iconfix) {
    { // 0x0018B2D0 - 0x0018B560
        char *strptr; // r2
        signed int dir_isnew; // r18
        class st_ISG_MEMCARD_DATA *mcdata; // r17
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018B560 - 0x0018B60C (0xAC) iSGSelectGameDir__FP13st_ISGSESSIONPCc
signed int iSGSelectGameDir(class st_ISGSESSION *isgdata, char *dname) {
    { // 0x0018B560 - 0x0018B60C
    }
}

// 0x0018B610 - 0x0018B76C (0x15C) iSGFileModDate__FP13st_ISGSESSIONPCcPiPiPiPiPiPi
char *iSGFileModDate(class st_ISGSESSION *isgdata, char *fname, signed int *sec, signed int *min, signed int *hr, signed int *mon, signed int *day, signed int *yr) {
    { // 0x0018B610 - 0x0018B76C
        class sceCdCLOCK clock; // r29+0x88
        class sceMcTblGetDir *finf; // r16
        signed int rc; // r2
        static char datestr[64]; // 0x002B0770
    }
}

// 0x0018B770 - 0x0018B78C (0x1C) iSGFileModDate__FP13st_ISGSESSIONPCc
char *iSGFileModDate(class st_ISGSESSION *isgdata, char *fname) {
    { // 0x0018B770 - 0x0018B78C
        char *date_str; // r2
    }
}

// 0x0018B790 - 0x0018B7D8 (0x48) iSGFileSize__FP13st_ISGSESSIONPCc
signed int iSGFileSize(class st_ISGSESSION *isgdata, char *fname) {
    { // 0x0018B790 - 0x0018B7D8
        signed int size; // r2
    }
}

// 0x0018B7E0 - 0x0018B7E8 (0x8) iSGTgtHaveRoom__FP13st_ISGSESSIONiiPCcPCcPiPi
signed int iSGTgtHaveRoom(class st_ISGSESSION *isgdata, signed int tidx, signed int fsize, char *dpath, char *fname, signed int *bytesNeeded, signed int *availOnDisk) {
    { // 0x0018B7E0 - 0x0018B7E8
        signed int result; // r2
    }
}

// 0x0018B7F0 - 0x0018B860 (0x70) iSGTgtSetActive__FP13st_ISGSESSIONi
signed int iSGTgtSetActive(class st_ISGSESSION *isgdata, signed int tgtidx) {
    { // 0x0018B7F0 - 0x0018B860
    }
}

// 0x0018B860 - 0x0018B94C (0xEC) iSGTgtState__FP13st_ISGSESSIONiPCc
unsigned int iSGTgtState(class st_ISGSESSION *isgdata, signed int tgtidx, char *dpath) {
    { // 0x0018B860 - 0x0018B94C
        signed int rc; // r2
        unsigned int state; // r16
    }
}

// 0x0018B950 - 0x0018BA4C (0xFC) iSGTgtFormat__FP13st_ISGSESSIONii
signed int iSGTgtFormat(class st_ISGSESSION *isgdata, signed int tgtidx, signed int async) {
    { // 0x0018B950 - 0x0018BA4C
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018BA50 - 0x0018BA84 (0x34) iSGTgtPhysSlotIdx__FP13st_ISGSESSIONi
signed int iSGTgtPhysSlotIdx() {
    { // 0x0018BA50 - 0x0018BA84
        signed int ds; // r29+0x1c
        signed int dp; // r29+0x18
        signed int concnt[2]; // r29+0x10
    }
}

// 0x0018BA90 - 0x0018BB0C (0x7C) iSGTgtCount__FP13st_ISGSESSIONPi
signed int iSGTgtCount(signed int *max) {
    { // 0x0018BA90 - 0x0018BB0C
        signed int ds; // r29+0x3c
        signed int dp; // r29+0x38
        signed int concnt[2]; // r29+0x30
        signed int tgtmax; // r2
        signed int rc; // r16
    }
}

// 0x0018BB10 - 0x0018BB1C (0xC) iSGSessionEnd__FP13st_ISGSESSION
void iSGSessionEnd(class st_ISGSESSION *isgdata) {
    { // 0x0018BB10 - 0x0018BB1C
    }
}

// 0x0018BB20 - 0x0018BBE4 (0xC4) iSGSessionBegin__FPvPFPv10en_CHGCODE_v
class st_ISGSESSION *iSGSessionBegin() {
    { // 0x0018BB20 - 0x0018BBE4
    }
}

// 0x0018BBF0 - 0x0018BDD8 (0x1E8) iSGMakeName__F15en_NAMEGEN_TYPEPCci
char *iSGMakeName(enum en_NAMEGEN_TYPE type, char *base, signed int idx) {
    { // 0x0018BBF0 - 0x0018BDD8
        char *fmt_sbd; // r11
        char *fmt_sd; // r2
        char *fmt_sb; // r10
        char *use_buf; // r16
        static signed int rotate; // 0x0035804C
        static char rotatebuf[8][32]; // 0x002B0670
        static signed char init; // 0x00358050
    }
}

// 0x0018BDE0 - 0x0018BDF4 (0x14) iSGShutdown__Fv
signed int iSGShutdown() {
    { // 0x0018BDE0 - 0x0018BDF4
    }
}

// 0x0018BE00 - 0x0018BE30 (0x30) iSGStartup__Fv
signed int iSGStartup() {
    { // 0x0018BE00 - 0x0018BE30
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xsavegame.cpp
// ==========================================================================
void (xSG_cb_ISGChange)(void *, enum en_CHGCODE); // 0x0018C2C0
signed int (xSG_cb_leader_load)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int); // 0x0018CDB0
signed int (xSG_cb_leader_svinfo)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *); // 0x0018CF50
signed int (xSG_cb_leader_svproc)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *); // 0x0018CE60
static class st_XSAVEGAME_LEADER g_leaders[8]; // 0x002B2970
static class st_XSAVEGAME_DATA g_xsgdata; // 0x002B0880
static signed int g_xsginit; // 0x00358068
// 0x0018BE30 - 0x0018BF1C (0xEC) xSG_ld_flipload__FP17st_XSAVEGAME_DATA
signed int xSG_ld_flipload(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018BE30 - 0x0018BF1C
        class st_XSAVEGAME_CLIENT *clt; // r18
        signed int i; // r17
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018BF20 - 0x0018C1D8 (0x2B8) xSG_ld_findcltblk__FP17st_XSAVEGAME_DATA
signed int xSG_ld_findcltblk(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018BF20 - 0x0018C1D8
        class st_XSAVEGAME_CLIENT *dfltclt; // r17
        signed int rc; // r2
        signed int found; // r16
        signed int i; // r4
        class st_XSAVEGAME_CLIENT *clt; // r3
        signed int actamt; // r29+0x4c
        signed int maxamt; // r29+0x48
        unsigned int tag; // r29+0x44
        signed int ival; // r29+0x40
    }
}

// 0x0018C1E0 - 0x0018C2B4 (0xD4) xSG_ld_readhead__FP17st_XSAVEGAME_DATA
signed int xSG_ld_readhead(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C1E0 - 0x0018C2B4
        signed int ival; // r29+0x3c
        signed int result; // r16
    }
}

// 0x0018C2C0 - 0x0018C2C8 (0x8) xSG_cb_ISGChange__FPv10en_CHGCODE
void xSG_cb_ISGChange() {
    { // 0x0018C2C0 - 0x0018C2C8
    }
}

// 0x0018C2D0 - 0x0018C364 (0x94) xSG_sv_commit__FP17st_XSAVEGAME_DATA
signed int xSG_sv_commit(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C2D0 - 0x0018C364
        char *name; // r2
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018C370 - 0x0018C438 (0xC8) xSG_smem_cltclose__FP17st_XSAVEGAME_DATAP19st_XSAVEGAME_CLIENT
signed int xSG_smem_cltclose(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_CLIENT *clt) {
    { // 0x0018C370 - 0x0018C438
        char *last_bufpos; // r18
    }
}

// 0x0018C440 - 0x0018C4C0 (0x80) xSG_smem_blkclose__FP17st_XSAVEGAME_DATA
signed int xSG_smem_blkclose(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C440 - 0x0018C4C0
        char *last_bufpos; // r16
    }
}

// 0x0018C4C0 - 0x0018C67C (0x1BC) xSG_sv_flipproc__FP17st_XSAVEGAME_DATA
signed int xSG_sv_flipproc(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C4C0 - 0x0018C67C
        signed int needfill; // r19
        char bfill; // r29+0x8f
        class st_XSAVEGAME_CLIENT *clt; // r18
        signed int i; // r17
        signed int rc; // r16
        signed int result; // r22
    }
}

// 0x0018C680 - 0x0018C718 (0x98) xSG_sv_prepdest__FP17st_XSAVEGAME_DATA
signed int xSG_sv_prepdest(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C680 - 0x0018C718
        signed int result; // r16
    }
}

// 0x0018C720 - 0x0018C82C (0x10C) xSG_sv_flipinfo__FP17st_XSAVEGAME_DATA
signed int xSG_sv_flipinfo(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C720 - 0x0018C82C
        signed int cltmax; // r29+0x6c
        signed int cltamt; // r29+0x68
        class st_XSAVEGAME_CLIENT *clt; // r18
        signed int rc; // r2
        signed int i; // r17
        signed int result; // r16
    }
}

// 0x0018C830 - 0x0018C894 (0x64) xSG_cm_slotname__FP17st_XSAVEGAME_DATAi
char *xSG_cm_slotname(signed int slotidx) {
    { // 0x0018C830 - 0x0018C894
        char *sptr; // r2
        static char slotname[32]; // 0x002B2C30
    }
}

// 0x0018C8A0 - 0x0018C8F8 (0x58) xSG_chdir_gamedir__FP17st_XSAVEGAME_DATA
signed int xSG_chdir_gamedir(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C8A0 - 0x0018C8F8
        char *namestr; // r2
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018C900 - 0x0018CAB8 (0x1B8) xSG_grab_leaders__FP17st_XSAVEGAME_DATA
static signed int xSG_grab_leaders(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018C900 - 0x0018CAB8
        signed int rc; // r2
        char readbuf[256]; // r29+0x70
        signed int i; // r17
        signed int num_found; // r16
    }
}

// 0x0018CAC0 - 0x0018CACC (0xC) xSGReadData__FP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTPfi
signed int xSGReadData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt, float *buff, signed int n) {
    { // 0x0018CAC0 - 0x0018CACC
        signed int cnt; // r2
    }
}

// 0x0018CAD0 - 0x0018CADC (0xC) xSGReadData__FP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTPUii
signed int xSGReadData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt, unsigned int *buff, signed int n) {
    { // 0x0018CAD0 - 0x0018CADC
        signed int cnt; // r2
    }
}

// 0x0018CAE0 - 0x0018CAEC (0xC) xSGReadData__FP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTPii
signed int xSGReadData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt, signed int *buff, signed int n) {
    { // 0x0018CAE0 - 0x0018CAEC
        signed int cnt; // r2
    }
}

// 0x0018CAF0 - 0x0018CAFC (0xC) xSGReadData__FP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTPci
signed int xSGReadData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt, char *buff, signed int n) {
    { // 0x0018CAF0 - 0x0018CAFC
        signed int cnt; // r2
    }
}

// 0x0018CB00 - 0x0018CC0C (0x10C) xSGReadData__FP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTPcii
signed int xSGReadData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt, char *buff, signed int elesiz, signed int n) {
    { // 0x0018CB00 - 0x0018CC0C
        class st_XSAVEGAME_CLIENT *clt; // r17
        signed int cnt; // r16
    }
}

// 0x0018CC10 - 0x0018CC1C (0xC) xSGWriteData__FP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXTPfi
signed int xSGWriteData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt, float *data, signed int n) {
    { // 0x0018CC10 - 0x0018CC1C
        signed int cnt; // r2
    }
}

// 0x0018CC20 - 0x0018CC2C (0xC) xSGWriteData__FP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXTPUii
signed int xSGWriteData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt, unsigned int *data, signed int n) {
    { // 0x0018CC20 - 0x0018CC2C
        signed int cnt; // r2
    }
}

// 0x0018CC30 - 0x0018CC3C (0xC) xSGWriteData__FP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXTPii
signed int xSGWriteData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt, signed int *data, signed int n) {
    { // 0x0018CC30 - 0x0018CC3C
        signed int cnt; // r2
    }
}

// 0x0018CC40 - 0x0018CC4C (0xC) xSGWriteData__FP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXTPci
signed int xSGWriteData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt, char *data, signed int n) {
    { // 0x0018CC40 - 0x0018CC4C
        signed int cnt; // r2
    }
}

// 0x0018CC50 - 0x0018CC7C (0x2C) xSGWriteStrLen__FPCc
signed int xSGWriteStrLen(char *str) {
    { // 0x0018CC50 - 0x0018CC7C
        signed int len; // r2
    }
}

// 0x0018CC80 - 0x0018CDB0 (0x130) xSGWriteData__FP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXTPcii
signed int xSGWriteData(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt, char *data, signed int elesiz, signed int n) {
    { // 0x0018CC80 - 0x0018CDB0
        void *mcprc; // r2
        signed int is_ok; // r18
        class st_XSAVEGAME_CLIENT *clt; // r17
        signed int cnt; // r16
    }
}

// 0x0018CDB0 - 0x0018CE5C (0xAC) xSG_cb_leader_load__FPvP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTUii
static signed int xSG_cb_leader_load(class st_XSAVEGAME_DATA *original_xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt) {
    { // 0x0018CDB0 - 0x0018CE5C
        class st_XSAVEGAME_LEADER discard; // r29+0x50
        char fundata[32]; // r29+0x30
    }
}

// 0x0018CE60 - 0x0018CF4C (0xEC) xSG_cb_leader_svproc__FPvP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXT
static signed int xSG_cb_leader_svproc(void *cltdata, class st_XSAVEGAME_DATA *original_xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt) {
    { // 0x0018CE60 - 0x0018CF4C
        char fundata[23]; // r29+0x90
        class st_XSAVEGAME_LEADER leader; // r29+0x40
        class st_XSAVEGAME_DATA *xsg; // r16
    }
}

// 0x0018CF50 - 0x0018CF64 (0x14) xSG_cb_leader_svinfo__FPvP17st_XSAVEGAME_DATAPiPi
static signed int xSG_cb_leader_svinfo(signed int *cur_space, signed int *max_fullgame) {
    { // 0x0018CF50 - 0x0018CF64
    }
}

// 0x0018CF70 - 0x0018D008 (0x98) xSGAsyncStatus__FP17st_XSAVEGAME_DATAiPc
enum en_XSGASYNC_STATUS xSGAsyncStatus(class st_XSAVEGAME_DATA *xsgdata, signed int block, char *errmsg) {
    { // 0x0018CF70 - 0x0018D008
        enum en_ASYNC_OPSTAT istat; // r2
        enum en_XSGASYNC_STATUS xstat; // r16
    }
}

// 0x0018D010 - 0x0018D178 (0x168) xSGWrapup__FP17st_XSAVEGAME_DATA
signed int xSGWrapup(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018D010 - 0x0018D178
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018D180 - 0x0018D39C (0x21C) xSGProcess__FP17st_XSAVEGAME_DATA
signed int xSGProcess(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018D180 - 0x0018D39C
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018D3A0 - 0x0018D4F8 (0x158) xSGSetup__FP17st_XSAVEGAME_DATAiPcil
signed int xSGSetup(class st_XSAVEGAME_DATA *xsgdata, signed int gidx, char *label, signed int thumIdx, signed long playtime) {
    { // 0x0018D3A0 - 0x0018D4F8
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0018D500 - 0x0018D518 (0x18) xSGSetup__FP17st_XSAVEGAME_DATA
signed int xSGSetup(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018D500 - 0x0018D518
    }
}

// 0x0018D520 - 0x0018D650 (0x130) xSGAddLoadClient__FP17st_XSAVEGAME_DATAUiPvPFPvP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTUii_i
signed int xSGAddLoadClient(class st_XSAVEGAME_DATA *xsgdata, unsigned int clttag, void *cltdata, signed int (*loadfunc)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int)) {
    { // 0x0018D520 - 0x0018D650
        class st_XSAVEGAME_CLIENT *clt; // r2
        signed int result; // r2
    }
}

// 0x0018D650 - 0x0018D73C (0xEC) xSGAddSaveClient__FP17st_XSAVEGAME_DATAUiPvPFPvP17st_XSAVEGAME_DATAPiPi_iPFPvP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXT_i
signed int xSGAddSaveClient(class st_XSAVEGAME_DATA *xsgdata, unsigned int clttag, void *cltdata, signed int (*infofunc)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *), signed int (*procfunc)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *)) {
    { // 0x0018D650 - 0x0018D73C
        class st_XSAVEGAME_CLIENT *clt; // r2
        signed int result; // r2
    }
}

// 0x0018D740 - 0x0018D7D8 (0x98) xSGGameThumIndex__FP17st_XSAVEGAME_DATAi
signed int xSGGameThumIndex(class st_XSAVEGAME_DATA *xsgdata, signed int gidx) {
    { // 0x0018D740 - 0x0018D7D8
        signed int idx_thum; // r16
    }
}

// 0x0018D7E0 - 0x0018D87C (0x9C) xSGGameLabel__FP17st_XSAVEGAME_DATAi
char *xSGGameLabel(class st_XSAVEGAME_DATA *xsgdata, signed int gidx) {
    { // 0x0018D7E0 - 0x0018D87C
        class st_XSAVEGAME_LEADER *lead; // r2
        char *da_name; // r16
    }
}

// 0x0018D880 - 0x0018D95C (0xDC) xSGGameModDate__FP17st_XSAVEGAME_DATAi
char *xSGGameModDate(class st_XSAVEGAME_DATA *xsgdata, signed int gidx) {
    { // 0x0018D880 - 0x0018D95C
        static char da_date[32]; // 0x002B2BF0
    }
}

// 0x0018D960 - 0x0018D9FC (0x9C) xSGGameIsEmpty__FP17st_XSAVEGAME_DATAi
signed int xSGGameIsEmpty(class st_XSAVEGAME_DATA *xsgdata, signed int gidx) {
    { // 0x0018D960 - 0x0018D9FC
    }
}

// 0x0018DA00 - 0x0018DA08 (0x8) xSGGameSet__FP17st_XSAVEGAME_DATAi
void xSGGameSet(class st_XSAVEGAME_DATA *xsgdata, signed int gidx) {
    { // 0x0018DA00 - 0x0018DA08
    }
}

// 0x0018DA10 - 0x0018DAF8 (0xE8) xSGTgtHaveRoom__FP17st_XSAVEGAME_DATAiiiPiPi
signed int xSGTgtHaveRoom(class st_XSAVEGAME_DATA *xsgdata, signed int tidx, signed int fsize, signed int slotidx, signed int *bytesNeeded, signed int *availOnDisk) {
    { // 0x0018DA10 - 0x0018DAF8
        char fname[256]; // r29+0x60
        signed int isroom; // r2
    }
}

// 0x0018DB00 - 0x0018DB64 (0x64) xSGTgtHasGameDir__FP17st_XSAVEGAME_DATAi
signed int xSGTgtHasGameDir(class st_XSAVEGAME_DATA *xsgdata, signed int tidx) {
    { // 0x0018DB00 - 0x0018DB64
        signed int rc; // r2
    }
}

// 0x0018DB70 - 0x0018DBEC (0x7C) xSGTgtSelect__FP17st_XSAVEGAME_DATAi
signed int xSGTgtSelect(class st_XSAVEGAME_DATA *xsgdata, signed int tidx) {
    { // 0x0018DB70 - 0x0018DBEC
        signed int result; // r16
    }
}

// 0x0018DBF0 - 0x0018DBFC (0xC) xSGTgtFormatTgt__FP17st_XSAVEGAME_DATAi
signed int xSGTgtFormatTgt(class st_XSAVEGAME_DATA *xsgdata, signed int tidx) {
    { // 0x0018DBF0 - 0x0018DBFC
        signed int result; // r2
    }
}

// 0x0018DC00 - 0x0018DC24 (0x24) xSGTgtIsFormat__FP17st_XSAVEGAME_DATAi
signed int xSGTgtIsFormat(class st_XSAVEGAME_DATA *xsgdata, signed int tidx) {
    { // 0x0018DC00 - 0x0018DC24
        unsigned int rc; // r2
    }
}

// 0x0018DC30 - 0x0018DC38 (0x8) xSGTgtPhysSlotIdx__FP17st_XSAVEGAME_DATAi
signed int xSGTgtPhysSlotIdx(class st_XSAVEGAME_DATA *xsgdata, signed int tidx) {
    { // 0x0018DC30 - 0x0018DC38
    }
}

// 0x0018DC40 - 0x0018DC48 (0x8) xSGTgtCount__FP17st_XSAVEGAME_DATAPi
signed int xSGTgtCount(class st_XSAVEGAME_DATA *xsgdata, signed int *max) {
    { // 0x0018DC40 - 0x0018DC48
        signed int cnt; // r2
    }
}

// 0x0018DC50 - 0x0018DD4C (0xFC) xSGDone__FP17st_XSAVEGAME_DATA
signed int xSGDone(class st_XSAVEGAME_DATA *xsgdata) {
    { // 0x0018DC50 - 0x0018DD4C
        signed int result; // r16
    }
}

// 0x0018DD50 - 0x0018DE3C (0xEC) xSGInit__F16en_SAVEGAME_MODE
class st_XSAVEGAME_DATA *xSGInit(enum en_SAVEGAME_MODE mode) {
    { // 0x0018DD50 - 0x0018DE3C
        class st_XSAVEGAME_DATA *xsgdata; // r16
    }
}

// 0x0018DE40 - 0x0018DE78 (0x38) xSGShutdown__Fv
signed int xSGShutdown() {
    { // 0x0018DE40 - 0x0018DE78
    }
}

// 0x0018DE80 - 0x0018DECC (0x4C) xSGStartup__Fv
signed int xSGStartup() {
    { // 0x0018DE80 - 0x0018DECC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xutil.cpp
// ==========================================================================
char _ctype_[0]; // 0x003554F0
static unsigned short ascii_special_table[33]; // 0x00000000
static unsigned short ascii_k_table[33]; // 0x002B3060
static unsigned short ascii_table[3][2]; // 0x002B3050
static unsigned int g_crc32_table[256]; // 0x002B2C50
static signed int g_crc_needinit; // 0x00357598
static signed int g_xutilinit; // 0x0035806C
// 0x0018DED0 - 0x0018DF48 (0x78) xUtil_yesno__Ff
signed int xUtil_yesno(float wt_yes) {
    { // 0x0018DED0 - 0x0018DF48
    }
}

// 0x0018DF50 - 0x0018DF8C (0x3C) xUtil_yesno__Fv
signed int xUtil_yesno() {
    { // 0x0018DF50 - 0x0018DF8C
    }
}

// 0x0018DF90 - 0x0018DFF0 (0x60) itoBCD__FUs
unsigned char itoBCD(unsigned short dec) {
    { // 0x0018DF90 - 0x0018DFF0
        signed int ones; // r4
    }
}

// 0x0018DFF0 - 0x0018E050 (0x60) itoBCD__FUc
unsigned char itoBCD(unsigned char dec) {
    { // 0x0018DFF0 - 0x0018E050
        signed int ones; // r4
    }
}

// 0x0018E050 - 0x0018E084 (0x34) BCDtoi__FUc
unsigned char BCDtoi(unsigned char hex) {
    { // 0x0018E050 - 0x0018E084
        char c[16]; // r29+0x10
    }
}

// 0x0018E090 - 0x0018E2A4 (0x214) strtosjis__FPUcPUc
void strtosjis(unsigned char *string, unsigned char *dest) {
    { // 0x0018E090 - 0x0018E2A4
        unsigned char *dest2; // r5
        unsigned char stmp2; // r18
        unsigned char stmp; // r17
        signed int ascii_code; // r16
        signed int sjis_code; // r4
        signed int i; // r4
    }
}

// 0x0018E2B0 - 0x0018E4E8 (0x238) xUtil_crc_update__FUiPci
unsigned int xUtil_crc_update(unsigned int crc_accum, char *data, signed int datasize) {
    { // 0x0018E2B0 - 0x0018E4E8
        signed int j; // r8
        signed int i; // r10
    }
}

// 0x0018E4F0 - 0x0018E57C (0x8C) xUtil_crc_init__Fv
unsigned int xUtil_crc_init() {
    { // 0x0018E4F0 - 0x0018E57C
        unsigned int crc_accum; // r7
        signed int j; // r6
        signed int i; // r5
    }
}

// 0x0018E580 - 0x0018E7E8 (0x268) xUtil_idtag2string__FUii
char *xUtil_idtag2string(unsigned int srctag, signed int bufidx) {
    { // 0x0018E580 - 0x0018E7E8
        char t; // r2
        signed int l; // r29+0xc
        char *uc; // r3
        char *strptr; // r2
        unsigned int tag; // r29+0x8
        static char buf[6][10]; // 0x002B30B0
    }
}

// 0x0018E7F0 - 0x0018E804 (0x14) xUtilShutdown__Fv
signed int xUtilShutdown() {
    { // 0x0018E7F0 - 0x0018E804
    }
}

// 0x0018E810 - 0x0018E840 (0x30) xUtilStartup__Fv
signed int xUtilStartup() {
    { // 0x0018E810 - 0x0018E840
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xColor.cpp
// ==========================================================================
char *g_xColor_FMT; // 0x00000000
class iColor_tag g_COLOR_TRYME3; // 0x00000000
class iColor_tag g_COLOR_TRYME2; // 0x00000000
class iColor_tag g_COLOR_TRYME; // 0x00000000
class iColor_tag g_LAVENDER; // 0x00000000
class iColor_tag g_KHAKI; // 0x00000000
class iColor_tag g_ORANGE; // 0x00000000
class iColor_tag g_PIMP_GOLD; // 0x0034E440
class iColor_tag g_FORESTGREEN; // 0x00000000
class iColor_tag g_SEAGREEN; // 0x00000000
class iColor_tag g_MIDNIGHTBLUE; // 0x00000000
class iColor_tag g_MAROON; // 0x0034E438
class iColor_tag g_FUSCHIA; // 0x00000000
class iColor_tag g_PEACH; // 0x00000000
class iColor_tag g_NEON_BLUE; // 0x00000000
class iColor_tag g_NEON_GREEN; // 0x0034E430
class iColor_tag g_NEON_RED; // 0x0034E428
class iColor_tag g_CLEAR; // 0x00000000
class iColor_tag g_GRAY90; // 0x00000000
class iColor_tag g_GRAY80; // 0x00000000
class iColor_tag g_GRAY70; // 0x00000000
class iColor_tag g_GRAY60; // 0x00000000
class iColor_tag g_GRAY50; // 0x00000000
class iColor_tag g_GRAY40; // 0x00000000
class iColor_tag g_GRAY30; // 0x00000000
class iColor_tag g_GRAY20; // 0x00000000
class iColor_tag g_GRAY10; // 0x00000000
class iColor_tag g_WHITE; // 0x00000000
class iColor_tag g_BLACK; // 0x0034E420
class iColor_tag g_YELLOW; // 0x00000000
class iColor_tag g_MAGENTA; // 0x00000000
class iColor_tag g_CYAN; // 0x00000000
class iColor_tag g_BLUE; // 0x00000000
class iColor_tag g_GREEN; // 0x00000000
class iColor_tag g_RED; // 0x00000000

// ==========================================================================
// E:\SD2\Game\flat\zEntTrigger.cpp
// ==========================================================================
void (zEntTriggerUpdate)(class zEntTrigger *, class _xScene *, float); // 0x0018E890
signed int (zEntTriggerEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x0018E860
// 0x0018E840 - 0x0018E848 (0x8) zEntTriggerLoad__FP11zEntTriggerP7xSerial
void zEntTriggerLoad(class zEntTrigger *ent, class xSerial *s) {
    { // 0x0018E840 - 0x0018E848
    }
}

// 0x0018E850 - 0x0018E858 (0x8) zEntTriggerSave__FP11zEntTriggerP7xSerial
void zEntTriggerSave(class zEntTrigger *ent, class xSerial *s) {
    { // 0x0018E850 - 0x0018E858
    }
}

// 0x0018E860 - 0x0018E890 (0x30) zEntTriggerEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntTriggerEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x0018E860 - 0x0018E890
        class zEntTrigger *trig; // r2
    }
}

// 0x0018E890 - 0x0018EBD0 (0x340) zEntTriggerUpdate__FP11zEntTriggerP7_xScenef
void zEntTriggerUpdate(class zEntTrigger *trig) {
    { // 0x0018E890 - 0x0018EBD0
        class _xIsect isect; // r29+0x150
        class _xSphere sphere; // r29+0x140
        class _xIsect isect; // r29+0x110
        class _xCylinder cylinder; // r29+0xf0
        class _xIsect isect; // r29+0xc0
        class _xSphere sphere; // r29+0xb0
        class _xBox box; // r29+0x90
        class _xVec3 *chkPos; // r2
        class _xEnt *chkEnt; // r2
        signed int collide; // r19
        class _xIsect isect; // r29+0x60
        class p2EntTriggerAsset *tasset; // r18
        class p2LinkAsset *link; // r17
        unsigned int i; // r16
    }
}

// 0x0018EBD0 - 0x0018EC38 (0x68) zEntTriggerInit__FPvPv
void zEntTriggerInit(void *ent, void *asset) {
    { // 0x0018EBD0 - 0x0018EC38
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zCamera.cpp
// ==========================================================================
signed int (zCameraEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x0018EC40
class zGlobals globals; // 0x00362FD0
// 0x0018EC40 - 0x0018EC98 (0x58) zCameraEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zCameraEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x0018EC40 - 0x0018EC98
    }
}

// 0x0018ECA0 - 0x0018ECA8 (0x8) zCameraLoad__FP8_tagxCamP7xSerial
void zCameraLoad(class _tagxCam *ent, class xSerial *s) {
    { // 0x0018ECA0 - 0x0018ECA8
    }
}

// 0x0018ECB0 - 0x0018ECB8 (0x8) zCameraSave__FP8_tagxCamP7xSerial
void zCameraSave(class _tagxCam *ent, class xSerial *s) {
    { // 0x0018ECB0 - 0x0018ECB8
    }
}

// 0x0018ECC0 - 0x0018ED10 (0x50) zCameraInit__FP5xBaseP9xCamAsset
void zCameraInit(class xBase *b, class xCamAsset *asset) {
    { // 0x0018ECC0 - 0x0018ED10
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntPickup.cpp
// ==========================================================================
void (zEntPickup_Update)(class _zEntPickup *, class _xScene *, float); // 0x00190380
signed int (zEntPickupEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x00191CA0
class _xEnt *sDecrementSnackGateEnt; // 0x00357F64
unsigned char *giAnimScratch; // 0x003574A0
class zGlobals globals; // 0x00362FD0
class _zParEmitter *sEmitDust; // 0x003581A0
static class zEntPickupWarpPoint zWarpPointTable[27]; // 0x002B3150
signed int zHudShowWarpGateInfo; // 0x003575A0
static class _zCounter *keyCounter; // 0x003580C0
static class _zParEmitter *sEmitMiniVanPuff; // 0x003580BC
static class _zParEmitter *sEmitWarpGate; // 0x003580B8
static class _zParEmitter *sEmitGrabSDLogo; // 0x003580B4
static class _zParEmitter *sEmitGrabGhost; // 0x003580B0
static class _zParEmitter *sEmitAkmalDaisy; // 0x003580AC
static class _zParEmitter *sEmitLoveHart; // 0x003580A8
static class _zParEmitter *sEmitGrabBubbles; // 0x003580A4
static class _zParEmitter *sEmitGrabGum; // 0x003580A0
static class _zParEmitter *sEmitGrabQMark; // 0x0035809C
static class _zParEmitter *sEmitGrabMmm; // 0x00358098
static class _zParEmitter *sEmitGrabSnackBox; // 0x00358094
static class _zParEmitter *sEmitGrabCrumb; // 0x00358090
static class _zParEmitter *sEmitGrabPowerup; // 0x0035808C
static class _zParEmitter *sEmitOdour; // 0x00358088
static class _zParEmitter *sEmitGum; // 0x00358084
static class _zParEmitter *sEmitBubble; // 0x00358080
static class _zParEmitter *sEmitQMark; // 0x0035807C
static class _zParEmitter *sEmitMmm; // 0x00358078
static class _zParEmitter *sEmitGlow; // 0x00358074
static class _xEntCollis gPickupCollis; // 0x0036F1A0
static class _xEntFrame gPickupFrame; // 0x0036F0B0
static class _tagKeyShake sKeyShake[4]; // 0x002B30F0
static signed int sKeyCounterEnabledBeforeGive; // 0x00358070
static signed int zHudShowSnackGateInfo; // 0x00000000
static float sKeysOffTimer; // 0x0035759C
// 0x0018ED10 - 0x0018ED20 (0x10) zEntPickup_SceneUpdate__Ff
void zEntPickup_SceneUpdate(float dt) {
    { // 0x0018ED10 - 0x0018ED20
    }
}

// 0x0018ED20 - 0x0018EE10 (0xF0) zEntPickup_ShowKeyCnt__FUiUi
void zEntPickup_ShowKeyCnt(unsigned int collected, unsigned int total) {
    { // 0x0018ED20 - 0x0018EE10
        signed int i; // r2
        char s[64]; // r29+0x40
    }
}

// 0x0018EE10 - 0x0018EE94 (0x84) zEntPickupResetWarps__Fv
void zEntPickupResetWarps() {
    { // 0x0018EE10 - 0x0018EE94
        unsigned int i; // r4
    }
}

// 0x0018EEA0 - 0x0018EF28 (0x88) zEntPickupCheat_allWarps__Fv
void zEntPickupCheat_allWarps() {
    { // 0x0018EEA0 - 0x0018EF28
        unsigned int i; // r5
    }
}

// 0x0018EF30 - 0x0018F038 (0x108) zEntPickup_UnlockSnackGate__FP5_xEnt
void zEntPickup_UnlockSnackGate(class _xEnt *ent) {
    { // 0x0018EF30 - 0x0018F038
        class _zParEmitterCustomSettings info; // r29+0x30
        class _zEntPickup *pickup; // r2
    }
}

// 0x0018F040 - 0x0018F08C (0x4C) zEntPickup_lastSnackInArea__Fv
unsigned int zEntPickup_lastSnackInArea() {
    { // 0x0018F040 - 0x0018F08C
    }
}

// 0x0018F090 - 0x0018F1D4 (0x144) zEntPickup_UpdateFlyToInterface__FP11_zEntPickupUif
void zEntPickup_UpdateFlyToInterface(class _zEntPickup *ent, unsigned int pcount, float dt) {
    { // 0x0018F090 - 0x0018F1D4
        float ydist; // r29+0x50
        float xdist; // 
        class _xVec3 *ent_pos; // r18
        signed int i; // r16
    }
}

// 0x0018F1E0 - 0x0018F200 (0x20) zEntPickup_WarpIsOpen__FUi
unsigned int zEntPickup_WarpIsOpen(unsigned int index) {
    { // 0x0018F1E0 - 0x0018F200
    }
}

// 0x0018F200 - 0x0018F224 (0x24) enableWarpPoint__FUi
void enableWarpPoint(unsigned int index) {
    { // 0x0018F200 - 0x0018F224
    }
}

// 0x0018F230 - 0x0018F554 (0x324) zEntPickup_GiveFX__FP11_zEntPickup
void zEntPickup_GiveFX(class _zEntPickup *ent) {
    { // 0x0018F230 - 0x0018F554
        class _zParEmitterCustomSettings info; // r29+0xd0
        class _zParEmitterCustomSettings info; // r29+0x70
        enum _tagGiveFXType type; // r19
        signed int pickup_idx; // r18
        signed int pickup_type; // r17
        signed int kill_glow; // r16
    }
}

// 0x0018F560 - 0x0018F7E0 (0x280) zEntPickup_RenderList__FP11_zEntPickupUi
void zEntPickup_RenderList(class _zEntPickup *plist, unsigned int pcount) {
    { // 0x0018F560 - 0x0018F7E0
        float dist2; // r29+0x80
        signed int alpha; // r20
        class RwV3d vec_scale; // r29+0x70
        class RwFrame *frame; // r3
        class RwMatrixTag *imat; // r21
        class RpAtomic *imodel; // r20
        signed int i; // r19
    }
}

// 0x0018F7E0 - 0x0018F8C8 (0xE8) zEntPickup_PlaySound__FP11_zEntPickup
static void zEntPickup_PlaySound(class _zEntPickup *ent) {
    { // 0x0018F7E0 - 0x0018F8C8
    }
}

// 0x0018F8D0 - 0x0018FE64 (0x594) zEntPickup_UpdateFX__FP11_zEntPickupP7_xScenef
static void zEntPickup_UpdateFX(class _zEntPickup *ent, float dt) {
    { // 0x0018F8D0 - 0x0018FE64
        class _zParEmitterCustomSettings info; // r29+0xf0
        class _zParEmitterCustomSettings info; // r29+0x90
        unsigned char pupb; // r19
        unsigned char pupg; // r18
        unsigned char pupr; // r17
        signed int powerup; // r16
    }
}

// 0x0018FE70 - 0x00190018 (0x1A8) zEntPickup_Drop__FP11_zEntPickup
void zEntPickup_Drop(class _zEntPickup *ent) {
    { // 0x0018FE70 - 0x00190018
        unsigned int solcnt; // r2
        float sol[2]; // r29+0x38
        float c; // r29+0x40
        float b; // r29+0x40
    }
}

// 0x00190020 - 0x0019007C (0x5C) zEntPickup_MakeDroppable__FP11_zEntPickupP6_xVec3P5xBase
void zEntPickup_MakeDroppable(class _zEntPickup *ent, class _xVec3 *pos, class xBase *parent) {
    { // 0x00190020 - 0x0019007C
    }
}

// 0x00190080 - 0x001901EC (0x16C) zEntPickup_Load__FP11_zEntPickupP7xSerial
void zEntPickup_Load(class _zEntPickup *ent, class xSerial *s) {
    { // 0x00190080 - 0x001901EC
        signed int is_a_dropper; // r17
    }
}

// 0x001901F0 - 0x0019022C (0x3C) zEntPickup_Save__FP11_zEntPickupP7xSerial
void zEntPickup_Save(class _zEntPickup *ent, class xSerial *s) {
    { // 0x001901F0 - 0x0019022C
    }
}

// 0x00190230 - 0x0019032C (0xFC) zEntPickup_Reset__FP11_zEntPickup
void zEntPickup_Reset(class _zEntPickup *ent) {
    { // 0x00190230 - 0x0019032C
    }
}

// 0x00190330 - 0x00190378 (0x48) zEntPickup_hideSnackGateInfo__FP11_zEntPickup
void zEntPickup_hideSnackGateInfo(class _zEntPickup *ent) {
    { // 0x00190330 - 0x00190378
    }
}

// 0x00190380 - 0x001916A4 (0x1324) zEntPickup_Update__FP11_zEntPickupP7_xScenef
void zEntPickup_Update(class _zEntPickup *ent, class _xScene *sc, float dt) {
    { // 0x00190380 - 0x001916A4
        float spin_time; // r29+0x290
        float rot_sec; // 
        class _zParEmitterCustomSettings info; // r29+0x1e0
        float dist; // r29+0x290
        class _xVec3 *facept; // r2
        class _xVec3 *t0; // r19
        class _xQuat *q0; // r16
        float duration; // r29+0x290
        class _zParEmitterCustomSettings info; // r29+0x180
        class _zParEmitterCustomSettings info; // r29+0x120
        class _zParEmitterCustomSettings info; // r29+0xc0
        class _zParEmitterCustomSettings info; // r29+0x60
        class xBase *b; // r19
        float distsqr2; // 
        float distsqr; // 
    }
}

// 0x001916B0 - 0x0019198C (0x2DC) zEntPickup_FlyToInterface__FP11_zEntPickupf
void zEntPickup_FlyToInterface(class _zEntPickup *ent) {
    { // 0x001916B0 - 0x0019198C
        float scale; // 
        class _xVec3 *src; // r2
        float perc; // r29+0x90
        class _xMat4x3 dest; // r29+0x50
    }
}

// 0x00191990 - 0x00191CA0 (0x310) zPickup_Give__FP11_zEntPickupP12zAssetPickupUs
signed int zPickup_Give(class _zEntPickup *ent, class zAssetPickup *p, unsigned short value) {
    { // 0x00191990 - 0x00191CA0
    }
}

// 0x00191CA0 - 0x00191DC0 (0x120) zEntPickupEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntPickupEventCB(class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x00191CA0 - 0x00191DC0
        class _zEntPickup *p; // r2
    }
}

// 0x00191DC0 - 0x00192114 (0x354) zEntPickup_Setup__Fv
void zEntPickup_Setup() {
    { // 0x00191DC0 - 0x00192114
        class _xVec3 *srcPos; // r5
        class _zEntPickup *p; // r2
        class xBase *base; // r2
        unsigned int j; // r18
        unsigned int i; // r17
        class zScene *zsc; // r16
    }
}

// 0x00192120 - 0x00192154 (0x34) zEntPickup_Setup__FP11_zEntPickup
void zEntPickup_Setup(class _zEntPickup *p) {
    { // 0x00192120 - 0x00192154
    }
}

// 0x00192160 - 0x001923EC (0x28C) zEntPickupInit__FP11_zEntPickupP10p2EntAsset
void zEntPickupInit(class _zEntPickup *ent, class p2EntAsset *asset) {
    { // 0x00192160 - 0x001923EC
        void *animData; // r17
        class RpAtomic *modelData; // r20
        class zAssetPickup *ptbl; // r16
        unsigned int tmpsize; // r29+0x6c
        unsigned int i; // r5
        class p2EntPickupAsset *pickupAsset; // r4
    }
}

// 0x001923F0 - 0x001923F8 (0x8) zEntPickupInit__FPvPv
void zEntPickupInit(void *ent, void *asset) {
    { // 0x001923F0 - 0x001923F8
    }
}

// 0x00192400 - 0x001926AC (0x2AC) PickupFallPhysics__FP11_zEntPickupP7_xScenef
static void PickupFallPhysics(class _zEntPickup *ent, class _xScene *sc, float dt) {
    { // 0x00192400 - 0x001926AC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEnv.cpp
// ==========================================================================
signed int (zEnvEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001926B0
class zGlobals globals; // 0x00362FD0
// 0x001926B0 - 0x00192754 (0xA4) zEnvEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEnvEventCB(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x001926B0 - 0x00192754
    }
}

// 0x00192760 - 0x00192768 (0x8) zEnvLoad__FP5_zEnvP7xSerial
void zEnvLoad(class _zEnv *ent, class xSerial *s) {
    { // 0x00192760 - 0x00192768
    }
}

// 0x00192770 - 0x00192778 (0x8) zEnvSave__FP5_zEnvP7xSerial
void zEnvSave(class _zEnv *ent, class xSerial *s) {
    { // 0x00192770 - 0x00192778
    }
}

// 0x00192780 - 0x00192874 (0xF4) zEnvRender__FP5_xEnv
void zEnvRender(class _xEnv *env) {
    { // 0x00192780 - 0x00192874
        class RwTexture *texptr; // r2
        unsigned int texid; // r2
        class _zGroup *g; // r2
        class RpMaterial *mp; // r21
        class zSurfaceProps *pp; // r3
        class xSurface *sp; // r2
        signed int i; // r18
        signed int num; // r17
        class RpWorld *world; // r16
    }
}

// 0x00192880 - 0x001928C0 (0x40) zEnvStartingCamera__FP5_zEnv
void zEnvStartingCamera(class _zEnv *env) {
    { // 0x00192880 - 0x001928C0
        class _tagxCam *cam; // r2
    }
}

// 0x001928C0 - 0x001928FC (0x3C) zEnvSetup__FP5_zEnv
void zEnvSetup(class _zEnv *env) {
    { // 0x001928C0 - 0x001928FC
    }
}

// 0x00192900 - 0x00192A78 (0x178) zEnvInit__FP5_zEnvP10p2EnvAsset
void zEnvInit(class _zEnv *env, class p2EnvAsset *easset) {
    { // 0x00192900 - 0x00192A78
        void *buf; // r2
        unsigned int bufsize; // r29+0x3c
    }
}

// 0x00192A80 - 0x00192A88 (0x8) zEnvInit__FPvPv
void zEnvInit(void *env, void *easset) {
    { // 0x00192A80 - 0x00192A88
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xEntMotion.cpp
// ==========================================================================
void (xEntMotionDebugCB)(); // 0x001938D0
unsigned int gActiveHeap; // 0x00357CAC
class iColor_tag g_NEON_GREEN; // 0x0034E430
class iColor_tag g_PIMP_GOLD; // 0x0034E440
class iColor_tag g_NEON_RED; // 0x0034E428
class _tagxPad *gDebugPad; // 0x00357C30
class _xScene *g_xSceneCur; // 0x00357D6C
static signed short dbg_idx; // 0x003575A4
static unsigned short dbg_num_allocd; // 0x003580CC
static unsigned short dbg_num; // 0x003580C8
static class _xEntMotion **dbg_xems; // 0x003580C4
// 0x00192A90 - 0x00192BA4 (0x114) xEntMotionDebugIPad__FP11_xEntMotion
static void xEntMotionDebugIPad(class _xEntMotion *xem) {
    { // 0x00192A90 - 0x00192BA4
    }
}

// 0x00192BB0 - 0x00192E28 (0x278) xEntMotionDebugDraw__FPC11_xEntMotion
static void xEntMotionDebugDraw(class _xEntMotion *xem) {
    { // 0x00192BB0 - 0x00192E28
        unsigned short jdx; // r18
        unsigned short idx; // r23
        class _xMovePoint *xmp; // r17
    }
}

// 0x00192E30 - 0x001938C8 (0xA98) xEntMotionDebugWrite__FPC11_xEntMotion
static void xEntMotionDebugWrite(class _xEntMotion *xem) {
    { // 0x00192E30 - 0x001938C8
        char *gps; // r16
    }
}

// 0x001938D0 - 0x00193930 (0x60) xEntMotionDebugCB__Fv
static void xEntMotionDebugCB() {
    { // 0x001938D0 - 0x00193930
        class _xEntMotion *xem; // r16
    }
}

// 0x00193930 - 0x00193948 (0x18) xEntMotionDebugExit__Fv
void xEntMotionDebugExit() {
    { // 0x00193930 - 0x00193948
    }
}

// 0x00193950 - 0x001939AC (0x5C) xEntMotionDebugInit__FUs
void xEntMotionDebugInit(unsigned short num_xems) {
    { // 0x00193950 - 0x001939AC
    }
}

// 0x001939B0 - 0x00193B70 (0x1C0) xEntMotionTranslate__FP11_xEntMotionPC6_xVec3
void xEntMotionTranslate(class _xEntMotion *motion, class _xVec3 *dpos) {
    { // 0x001939B0 - 0x00193B70
    }
}

// 0x00193B70 - 0x00193C7C (0x10C) xEntMechReverse__FP11_xEntMotion
void xEntMechReverse(class _xEntMotion *motion) {
    { // 0x00193B70 - 0x00193C7C
        class p2EntMotionMechData *mkasst; // r8
        class _xEntMechData *mech; // r7
    }
}

// 0x00193C80 - 0x00193D88 (0x108) xEntMechForward__FP11_xEntMotion
void xEntMechForward(class _xEntMotion *motion) {
    { // 0x00193C80 - 0x00193D88
        class p2EntMotionMechData *mkasst; // r9
        class _xEntMechData *mech; // r8
    }
}

// 0x00193D90 - 0x00194010 (0x280) xEntRotMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
static unsigned int xEntRotMove(class _xEntMotion *motion, float dt, class _xEntFrame *frame) {
    { // 0x00193D90 - 0x00194010
        class p2EntAsset *easst; // r2
        class _xEnt *ownr; // r2
        float speed; // r29+0x40
        float rem; // 
        unsigned int last; // r16
        class p2EntMotionMechData *mkasst; // r4
        class _xEntMechData *mech; // r16
    }
}

// 0x00194010 - 0x001941DC (0x1CC) xEntSldMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
static unsigned int xEntSldMove(class _xEntMotion *motion, float dt, class _xEntFrame *frame) {
    { // 0x00194010 - 0x001941DC
        float speed; // r29+0x0
        float rem; // 
        unsigned int last; // r2
        class p2EntMotionMechData *mkasst; // r4
        class _xEntMechData *mech; // r3
    }
}

// 0x001941E0 - 0x001944DC (0x2FC) xEntMechMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
static void xEntMechMove(class _xEntMotion *motion, class _xScene *sc, float dt, class _xEntFrame *frame) {
    { // 0x001941E0 - 0x001944DC
        unsigned int last; // r18
        class p2EntMotionMechData *mkasst; // r17
        class _xEntMechData *mech; // r16
    }
}

// 0x001944E0 - 0x0019460C (0x12C) xEntPenMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
static void xEntPenMove(class _xEntMotion *motion, float dt, class _xEntFrame *frame) {
    { // 0x001944E0 - 0x0019460C
        float rem; // 
        float newt; // r29+0x60
        float dangle; // r29+0x60
        float v; // 
        float th; // 
        class p2EntMotionPenData *aspen; // r18
        class _xEntPenData *pen; // r17
    }
}

// 0x00194610 - 0x00194BD4 (0x5C4) xEntMPMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
static void xEntMPMove(class _xEntMotion *motion, class _xScene *sc, float dt, class _xEntFrame *frame) {
    { // 0x00194610 - 0x00194BD4
        float u; // 
        class _xMat3x3 tmpmat; // r29+0x80
        class _xQuat qold; // r29+0x70
        class _xQuat quat; // r29+0x60
        class _xVec3 dir; // r29+0xc0
        class _xVec3 tgt; // r29+0xb0
        float qdot; // r29+0xd0
        class _xMovePoint *prev; // r2
        float newdist; // r29+0xd0
        class _xEntMPData *mp; // r16
    }
}

// 0x00194BE0 - 0x00194D34 (0x154) xEntMPGetNext__FP11_xEntMotionP11_xMovePointP7_xScene
static void xEntMPGetNext(class _xEntMotion *motion, class _xMovePoint *prev) {
    { // 0x00194BE0 - 0x00194D34
        class _xMat3x3 mat; // r29+0x30
        class _xVec3 tempdir; // r29+0x60
        class _xEntMPData *mp; // r16
    }
}

// 0x00194D40 - 0x00194E84 (0x144) xEntOrbitMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
static void xEntOrbitMove(class _xEntMotion *motion, float dt, class _xEntFrame *frame) {
    { // 0x00194D40 - 0x00194E84
        float rem; // 
        float newt; // r29+0x40
        float s; // 
        float u; // 
        float th; // 
    }
}

// 0x00194E90 - 0x00195138 (0x2A8) xEntERMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
static void xEntERMove(class _xEntMotion *motion, float dt, class _xEntFrame *frame) {
    { // 0x00194E90 - 0x00195138
        float scale; // 
        float rem; // r29+0x0
        float scale; // 
        float rem; // 
        float newt; // 
    }
}

// 0x00195140 - 0x00195204 (0xC4) xEntMotionMove__FP11_xEntMotionP7_xScenefP10_xEntFrame
void xEntMotionMove(class _xEntMotion *motion, class _xScene *sc, float dt, class _xEntFrame *frame) {
    { // 0x00195140 - 0x00195204
    }
}

// 0x00195210 - 0x00195718 (0x508) xEntMotionReset__FP11_xEntMotionP7_xScene
void xEntMotionReset(class _xEntMotion *motion, class _xScene *sc) {
    { // 0x00195210 - 0x00195718
        float drot; // 
        class _xEntMechData *mech; // r17
        class p2EntMotionMechData *mkasst; // r16
        class _xEnt *ownr; // r5
        class _xVec3 *modlpos; // r17
        class _xMat3x3 *modlrot; // r2
        class _xVec3 pshtotop; // r29+0xb0
        class _xVec3 totop; // r29+0xa0
        class _xMat3x3 pshrot; // r29+0x70
        float dangle; // r29+0xc0
        class p2EntMotionPenData *aspen; // r16
    }
}

// 0x00195720 - 0x0019575C (0x3C) xEntMotionSetup__FP11_xEntMotionP7_xScene
void xEntMotionSetup(class _xEntMotion *motion) {
    { // 0x00195720 - 0x0019575C
    }
}

// 0x00195760 - 0x00195A44 (0x2E4) xEntMotionInit__FP11_xEntMotionP5_xEntP16p2EntMotionAsset
void xEntMotionInit(class _xEntMotion *motion, class _xEnt *owner, class p2EntMotionAsset *asset) {
    { // 0x00195760 - 0x00195A44
        class p2EntMotionMechData *mkasst; // r7
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zTimer.cpp
// ==========================================================================
signed int (zTimerEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x00195AA0
// 0x00195A50 - 0x00195AA0 (0x50) zTimerUpdate__FP5xBaseP7_xScenef
void zTimerUpdate(class xBase *to, float dt) {
    { // 0x00195A50 - 0x00195AA0
        class _zTimer *t; // r2
    }
}

// 0x00195AA0 - 0x00195B34 (0x94) zTimerEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zTimerEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x00195AA0 - 0x00195B34
        class _zTimer *t; // r16
    }
}

// 0x00195B40 - 0x00195B88 (0x48) zTimerLoad__FP7_zTimerP7xSerial
void zTimerLoad(class _zTimer *ent, class xSerial *s) {
    { // 0x00195B40 - 0x00195B88
    }
}

// 0x00195B90 - 0x00195BD8 (0x48) zTimerSave__FP7_zTimerP7xSerial
void zTimerSave(class _zTimer *ent, class xSerial *s) {
    { // 0x00195B90 - 0x00195BD8
    }
}

// 0x00195BE0 - 0x00195C4C (0x6C) zTimerInit__FPvPv
void zTimerInit(void *b, void *tasset) {
    { // 0x00195BE0 - 0x00195C4C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xEntDrive.cpp
// ==========================================================================
// 0x00195C50 - 0x001960C4 (0x474) xEntDriveUpdate__FP10_xEntDriveP7_xScenef
void xEntDriveUpdate(class _xEntDrive *drv, float dt) {
    { // 0x00195C50 - 0x001960C4
        class _xVec3 newq; // r29+0x70
        class _xMat3x3 rot; // r29+0x30
        class _xVec3 euler; // r29+0x60
    }
}

// 0x001960D0 - 0x00196180 (0xB0) xEntDriveDismount__FP10_xEntDrivef
void xEntDriveDismount(class _xEntDrive *drv, float dmt) {
    { // 0x001960D0 - 0x00196180
    }
}

// 0x00196180 - 0x001962A0 (0x120) xEntDriveMount__FP10_xEntDriveP5_xEntf
void xEntDriveMount(class _xEntDrive *drv, class _xEnt *driver, float mt) {
    { // 0x00196180 - 0x001962A0
        class _xVec3 euler; // r29+0x20
    }
}

// 0x001962A0 - 0x001962CC (0x2C) xEntDriveInit__FP10_xEntDriveP5_xEnt
void xEntDriveInit(class _xEntDrive *drv, class _xEnt *driven) {
    { // 0x001962A0 - 0x001962CC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zPlatform.cpp
// ==========================================================================
void (zPlatform_Update)(class _xEnt *, class _xScene *, float); // 0x00196920
void (zPlatform_Move)(class _xEnt *, class _xScene *, float, class _xEntFrame *); // 0x00196910
signed int (zPlatformEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001962D0
void (zPlatformTranslate)(class _xEnt *, class _xVec3 *); // 0x00196620
unsigned int gActiveHeap; // 0x00357CAC
class _xVec3 g_O3; // 0x00342E28
void (xFFXShakeUpdateEnt)(class _xEnt *, class _xScene *, float, void *); // 0x001A7A40
class zGlobals globals; // 0x00362FD0
static class _zParEmitter *sEmitBreakaway; // 0x003580D4
static class _zParEmitter *sEmitTremble; // 0x003580D0
// 0x001962D0 - 0x00196614 (0x344) zPlatformEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zPlatformEventCB(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x001962D0 - 0x00196614
        class _zPlatform *plat; // r16
    }
}

// 0x00196620 - 0x0019665C (0x3C) zPlatformTranslate__FP5_xEntP6_xVec3
static void zPlatformTranslate(class _xEnt *xent, class _xVec3 *dpos) {
    { // 0x00196620 - 0x0019665C
        class _zPlatform *plat; // r16
    }
}

// 0x00196660 - 0x00196684 (0x24) zPlatform_MakeDiggable__FP10_zPlatform
void zPlatform_MakeDiggable(class _zPlatform *plat) {
    { // 0x00196660 - 0x00196684
    }
}

// 0x00196690 - 0x00196698 (0x8) zPlatform_Dismount__FP10_zPlatform
void zPlatform_Dismount() {
    { // 0x00196690 - 0x00196698
    }
}

// 0x001966A0 - 0x00196704 (0x64) zPlatform_Mount__FP10_zPlatform
void zPlatform_Mount(class _zPlatform *plat) {
    { // 0x001966A0 - 0x00196704
    }
}

// 0x00196710 - 0x00196844 (0x134) zPlatform_Tremble__FP10_zPlatformfff
static void zPlatform_Tremble(class _zPlatform *plat, float ampl, float freq, float dur) {
    { // 0x00196710 - 0x00196844
        signed int i; // r16
        class _zParEmitterCustomSettings info; // r29+0x40
        class xFFX *sfkt; // r2
        class xFFXShakeState *ss; // r16
    }
}

// 0x00196850 - 0x0019690C (0xBC) zPlatform_Shake__FP10_zPlatformfff
void zPlatform_Shake(class _zPlatform *plat, float ampl, float freq) {
    { // 0x00196850 - 0x0019690C
        class xFFX *sfkt; // r2
        class xFFXShakeState *ss; // r16
    }
}

// 0x00196910 - 0x00196918 (0x8) zPlatform_Move__FP5_xEntP7_xScenefP10_xEntFrame
void zPlatform_Move(class _xEnt *entplat, class _xScene *s, float dt, class _xEntFrame *frame) {
    { // 0x00196910 - 0x00196918
    }
}

// 0x00196920 - 0x00197200 (0x8E0) zPlatform_Update__FP5_xEntP7_xScenef
void zPlatform_Update(class _xEnt *entplat, class _xScene *sc, float dt) {
    { // 0x00196920 - 0x00197200
        class _xVec3 *rot; // r8
        class _xVec3 *orot; // r7
        class _xVec3 *pos; // r6
        class _xVec3 *opos; // r5
        signed int moving; // r17
        class _xMovePoint *src; // r17
        class _xVec3 *scale; // r2
        float tangle; // r29+0xc0
        float mangle; // 
        float dangle; // 
        float cangle; // 
        class _xVec3 lpos; // r29+0xb0
        class _xVec3 *scale; // r2
        float dtilt; // r29+0xc0
        float itilt; // 
        float ctilt; // 
        class _xEntDrive *drv; // r3
        class _xEnt *p; // r6
        class _xEntDrive *drv; // r4
        class p2EntMotionMechData *mkasst; // r4
        unsigned short state; // r17
        unsigned short state; // r2
        class _zPlatform *plat; // r16
    }
}

// 0x00197200 - 0x00197324 (0x124) zPlatform_Reset__FP10_zPlatformP7_xScene
void zPlatform_Reset(class _zPlatform *plat, class _xScene *sc) {
    { // 0x00197200 - 0x00197324
    }
}

// 0x00197330 - 0x00197338 (0x8) zPlatform_Load__FP10_zPlatformP7xSerial
void zPlatform_Load(class _zPlatform *ent, class xSerial *s) {
    { // 0x00197330 - 0x00197338
    }
}

// 0x00197340 - 0x00197348 (0x8) zPlatform_Save__FP10_zPlatformP7xSerial
void zPlatform_Save(class _zPlatform *ent, class xSerial *s) {
    { // 0x00197340 - 0x00197348
    }
}

// 0x00197350 - 0x001973AC (0x5C) zPlatform_Setup__FP10_zPlatformP7_xScene
void zPlatform_Setup(class _zPlatform *plat, class _xScene *sc) {
    { // 0x00197350 - 0x001973AC
    }
}

// 0x001973B0 - 0x00197590 (0x1E0) zPlatform_Init__FP10_zPlatformP10p2EntAsset
void zPlatform_Init(class _zPlatform *plat, class p2EntAsset *asset) {
    { // 0x001973B0 - 0x00197590
        class xAnimFile *afile; // r16
        unsigned int size; // r29+0x4c
        void *buf; // r2
        unsigned int size; // r29+0x48
        void *buf; // r2
        class p2EntMotionAsset *emasset; // r17
        class p2PlatformAsset *passet; // r16
    }
}

// 0x00197590 - 0x00197598 (0x8) zPlatform_Init__FPvPv
void zPlatform_Init(void *plat, void *asset) {
    { // 0x00197590 - 0x00197598
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xserializer.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (xSER_ord_test)(void *, void *); // 0x00197AD0
signed int (xSER_ord_compare)(void *, void *); // 0x00197B00
signed int (xSER_xsgclt_svinfo_ver)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *); // 0x001978C0
signed int (xSER_xsgclt_svproc_ver)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *); // 0x00197890
signed int (xSER_xsgclt_svinfo_clt)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *); // 0x00197810
signed int (xSER_xsgclt_svproc_clt)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *); // 0x001977C0
signed int (xSER_xsgclt_svinfo_fill)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *); // 0x00197650
signed int (xSER_xsgclt_svproc_fill)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *); // 0x00197600
signed int (xSER_xsgclt_ldproc_ver)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int); // 0x00197830
signed int (xSER_xsgclt_ldproc_clt)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int); // 0x00197750
signed int (xSER_xsgclt_ldproc_fill)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int); // 0x001975A0
static signed int g_tbl_clear[32]; // 0x002B3350
static signed int g_tbl_onbit[32]; // 0x002B32D0
static class st_XSERIAL_DATA_PRIV g_xserdata; // 0x002B32A0
static signed int g_serinit; // 0x003580D8
// 0x001975A0 - 0x00197600 (0x60) xSER_xsgclt_ldproc_fill__FPvP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTUii
static signed int xSER_xsgclt_ldproc_fill(class st_XSAVEGAME_DATA *xsg, class st_XSAVEGAME_READCONTEXT *rctxt) {
    { // 0x001975A0 - 0x00197600
        char filbuf[9]; // r29+0x10
    }
}

// 0x00197600 - 0x00197648 (0x48) xSER_xsgclt_svproc_fill__FPvP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXT
static signed int xSER_xsgclt_svproc_fill(class st_XSAVEGAME_DATA *xsg, class st_XSAVEGAME_WRITECONTEXT *wctxt) {
    { // 0x00197600 - 0x00197648
        char filbuf[9]; // r29+0x10
    }
}

// 0x00197650 - 0x00197744 (0xF4) xSER_xsgclt_svinfo_fill__FPvP17st_XSAVEGAME_DATAPiPi
static signed int xSER_xsgclt_svinfo_fill(signed int *cur_space, signed int *max_fullgame) {
    { // 0x00197650 - 0x00197744
        signed int size; // r4
        signed int tally; // r3
        signed int i; // r2
        class st_XSERIAL_DATA_PRIV *xsd; // r2
    }
}

// 0x00197750 - 0x001977B4 (0x64) xSER_xsgclt_ldproc_clt__FPvP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTUii
static signed int xSER_xsgclt_ldproc_clt(class st_XSAVEGAME_DATA *xsg, class st_XSAVEGAME_READCONTEXT *rctxt, unsigned int idtag) {
    { // 0x00197750 - 0x001977B4
        class st_SERIAL_CLIENTINFO *clt; // r2
    }
}

// 0x001977C0 - 0x0019780C (0x4C) xSER_xsgclt_svproc_clt__FPvP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXT
static signed int xSER_xsgclt_svproc_clt(void *cltdata, class st_XSAVEGAME_DATA *xsg, class st_XSAVEGAME_WRITECONTEXT *wctxt) {
    { // 0x001977C0 - 0x0019780C
        class st_SERIAL_CLIENTINFO *clt; // r2
    }
}

// 0x00197810 - 0x00197828 (0x18) xSER_xsgclt_svinfo_clt__FPvP17st_XSAVEGAME_DATAPiPi
static signed int xSER_xsgclt_svinfo_clt(void *cltdata, signed int *cur_space, signed int *max_fullgame) {
    { // 0x00197810 - 0x00197828
        class st_SERIAL_CLIENTINFO *clt; // r2
    }
}

// 0x00197830 - 0x0019788C (0x5C) xSER_xsgclt_ldproc_ver__FPvP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTUii
static signed int xSER_xsgclt_ldproc_ver(class st_XSAVEGAME_DATA *xsg, class st_XSAVEGAME_READCONTEXT *rctxt) {
    { // 0x00197830 - 0x0019788C
        signed int ver; // r29+0x2c
        class st_XSERIAL_DATA_PRIV *xsd; // r16
    }
}

// 0x00197890 - 0x001978C0 (0x30) xSER_xsgclt_svproc_ver__FPvP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXT
static signed int xSER_xsgclt_svproc_ver(class st_XSAVEGAME_DATA *xsg, class st_XSAVEGAME_WRITECONTEXT *wctxt) {
    { // 0x00197890 - 0x001978C0
        signed int ver; // r29+0x1c
    }
}

// 0x001978C0 - 0x001978D4 (0x14) xSER_xsgclt_svinfo_ver__FPvP17st_XSAVEGAME_DATAPiPi
static signed int xSER_xsgclt_svinfo_ver(signed int *cur_space, signed int *max_fullgame) {
    { // 0x001978C0 - 0x001978D4
    }
}

// 0x001978E0 - 0x00197A24 (0x144) xSerial_svgame_register__FP17st_XSAVEGAME_DATA16en_SAVEGAME_MODE
signed int xSerial_svgame_register(class st_XSAVEGAME_DATA *sgctxt, enum en_SAVEGAME_MODE mode) {
    { // 0x001978E0 - 0x00197A24
        signed int i; // r16
        class st_SERIAL_CLIENTINFO *clt; // r2
        class st_XSERIAL_DATA_PRIV *xsd; // r2
    }
}

// 0x00197A30 - 0x00197AC4 (0x94) XSER_get_client__FUi
static class st_SERIAL_CLIENTINFO *XSER_get_client(unsigned int idtag) {
    { // 0x00197A30 - 0x00197AC4
        signed int idx; // r2
        class st_SERIAL_CLIENTINFO *clt; // r16
        class st_XSERIAL_DATA_PRIV *xsd; // r2
    }
}

// 0x00197AD0 - 0x00197AFC (0x2C) xSER_ord_test__FPCvPv
static signed int xSER_ord_test(void *key, void *elt) {
    { // 0x00197AD0 - 0x00197AFC
        unsigned int idtag; // r2
        signed int rc; // r2
    }
}

// 0x00197B00 - 0x00197B30 (0x30) xSER_ord_compare__FPvPv
static signed int xSER_ord_compare(void *e1, void *e2) {
    { // 0x00197B00 - 0x00197B30
        signed int rc; // r2
    }
}

// 0x00197B30 - 0x00197D18 (0x1E8) xSER_init_buffers__FiP21st_SERIAL_PERCID_SIZE
static void xSER_init_buffers(signed int count, class st_SERIAL_PERCID_SIZE *sizeinfo) {
    { // 0x00197B30 - 0x00197D18
        class st_SERIAL_CLIENTINFO *tmp_clt; // r19
        class st_SERIAL_PERCID_SIZE *sitmp; // r4
        signed int sicnt; // r18
        signed int tally; // r2
        signed int i; // r2
        class st_XSERIAL_DATA_PRIV *xsd; // r2
    }
}

// 0x00197D20 - 0x00197D38 (0x18) xSerialWipeMainBuffer__Fv
void xSerialWipeMainBuffer() {
    { // 0x00197D20 - 0x00197D38
    }
}

// 0x00197D40 - 0x00197E14 (0xD4) wrbit__7xSerialFi
void wrbit(class xSerial *this, signed int is_on) {
    { // 0x00197D40 - 0x00197E14
        class st_SERIAL_CLIENTINFO *clt; // r9
    }
}

// 0x00197E20 - 0x00197E2C (0xC) Read__7xSerialFPf
signed int Read(class xSerial *this, float *buf) {
    { // 0x00197E20 - 0x00197E2C
    }
}

// 0x00197E30 - 0x00197E3C (0xC) Read__7xSerialFPUi
signed int Read(class xSerial *this, unsigned int *buf) {
    { // 0x00197E30 - 0x00197E3C
    }
}

// 0x00197E40 - 0x00197E4C (0xC) Read__7xSerialFPi
signed int Read(class xSerial *this, signed int *buf) {
    { // 0x00197E40 - 0x00197E4C
    }
}

// 0x00197E50 - 0x00197E5C (0xC) Read__7xSerialFPs
signed int Read(class xSerial *this, signed short *buf) {
    { // 0x00197E50 - 0x00197E5C
    }
}

// 0x00197E60 - 0x00197E6C (0xC) Read__7xSerialFPUc
signed int Read(class xSerial *this, unsigned char *buf) {
    { // 0x00197E60 - 0x00197E6C
    }
}

// 0x00197E70 - 0x00197E7C (0xC) Read_b7__7xSerialFPUi
signed int Read_b7(class xSerial *this, unsigned int *bits) {
    { // 0x00197E70 - 0x00197E7C
    }
}

// 0x00197E80 - 0x00197E8C (0xC) Read_b2__7xSerialFPi
signed int Read_b2(class xSerial *this, signed int *bits) {
    { // 0x00197E80 - 0x00197E8C
    }
}

// 0x00197E90 - 0x00197E9C (0xC) Read_b1__7xSerialFPUi
signed int Read_b1(class xSerial *this, unsigned int *bits) {
    { // 0x00197E90 - 0x00197E9C
    }
}

// 0x00197EA0 - 0x00197EAC (0xC) Read_b1__7xSerialFPi
signed int Read_b1(class xSerial *this, signed int *bits) {
    { // 0x00197EA0 - 0x00197EAC
    }
}

// 0x00197EB0 - 0x00197FE0 (0x130) Read__7xSerialFPcii
signed int Read(class xSerial *this, char *buf, signed int elesize, signed int n) {
    { // 0x00197EB0 - 0x00197FE0
        signed int i; // r6
        signed int bidx; // r3
        char *cptr; // r2
        signed int nbit; // r2
    }
}

// 0x00197FE0 - 0x00198008 (0x28) Write__7xSerialFf
signed int Write(class xSerial *this, float data) {
    { // 0x00197FE0 - 0x00198008
    }
}

// 0x00198010 - 0x00198038 (0x28) Write__7xSerialFUi
signed int Write(class xSerial *this, unsigned int data) {
    { // 0x00198010 - 0x00198038
    }
}

// 0x00198040 - 0x00198068 (0x28) Write__7xSerialFi
signed int Write(class xSerial *this, signed int data) {
    { // 0x00198040 - 0x00198068
    }
}

// 0x00198070 - 0x00198098 (0x28) Write__7xSerialFs
signed int Write(class xSerial *this, signed short data) {
    { // 0x00198070 - 0x00198098
    }
}

// 0x001980A0 - 0x001980C8 (0x28) Write__7xSerialFUc
signed int Write(class xSerial *this, unsigned char data) {
    { // 0x001980A0 - 0x001980C8
    }
}

// 0x001980D0 - 0x001980F8 (0x28) Write_b7__7xSerialFUi
signed int Write_b7(class xSerial *this, unsigned int bits) {
    { // 0x001980D0 - 0x001980F8
    }
}

// 0x00198100 - 0x00198128 (0x28) Write_b2__7xSerialFi
signed int Write_b2(class xSerial *this, signed int bits) {
    { // 0x00198100 - 0x00198128
    }
}

// 0x00198130 - 0x00198158 (0x28) Write_b1__7xSerialFi
signed int Write_b1(class xSerial *this, signed int bits) {
    { // 0x00198130 - 0x00198158
    }
}

// 0x00198160 - 0x00198228 (0xC8) Write__7xSerialFPcii
signed int Write(class xSerial *this, char *data, signed int elesize, signed int n) {
    { // 0x00198160 - 0x00198228
        signed int i; // r19
        signed int bidx; // r18
        char *cptr; // r17
        signed int nbit; // r16
    }
}

// 0x00198230 - 0x0019827C (0x4C) setClient__7xSerialFUi
void setClient(class xSerial *this, unsigned int idtag) {
    { // 0x00198230 - 0x0019827C
    }
}

// 0x00198280 - 0x001982C4 (0x44) __dt__7xSerialFv
void *__dt(class xSerial *this) {
    { // 0x00198280 - 0x001982C4
    }
}

// 0x001982D0 - 0x001982D8 (0x8) __ct__7xSerialFv
void *__ct(class xSerial *this) {
    { // 0x001982D0 - 0x001982D8
    }
}

// 0x001982E0 - 0x001983B0 (0xD0) xSerialTraverse__FPFUiP7xSerial_i
void xSerialTraverse(signed int (*func)(unsigned int, class xSerial *)) {
    { // 0x001982E0 - 0x001983B0
        signed int rc; // r2
        class xSerial xser; // r29+0x60
        class st_SERIAL_CLIENTINFO *clt; // r18
        class st_XSERIAL_DATA_PRIV *xsd; // r17
        signed int i; // r16
    }
}

// 0x001983B0 - 0x001983C8 (0x18) xSerialShutdown__Fv
signed int xSerialShutdown() {
    { // 0x001983B0 - 0x001983C8
    }
}

// 0x001983D0 - 0x00198504 (0x134) xSerialStartup__FiP21st_SERIAL_PERCID_SIZE
signed int xSerialStartup(signed int count, class st_SERIAL_PERCID_SIZE *sizeinfo) {
    { // 0x001983D0 - 0x00198504
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xFont.cpp
// ==========================================================================
static char *blankText; // 0x003575A8
class xFontGlobals_t gFontGlobals; // 0x002B33E0
unsigned int gDBfont; // 0x003580DC
class xFontBox gBoxList[4]; // 0x0036FD50
class xFontInfo gFontList[4]; // 0x0036F6B0
// 0x00198510 - 0x0019854C (0x3C) xFontBoxSetBackdrop__FP8xFontBox10iColor_tag
void xFontBoxSetBackdrop(class xFontBox *box, class iColor_tag color) {
    { // 0x00198510 - 0x0019854C
    }
}

// 0x00198550 - 0x00198590 (0x40) xFontBoxSetJustification__FP8xFontBoxi
void xFontBoxSetJustification(class xFontBox *box, signed int mode) {
    { // 0x00198550 - 0x00198590
    }
}

// 0x00198590 - 0x001985E0 (0x50) xFontBoxSetInset__FP8xFontBoxffff
void xFontBoxSetInset(class xFontBox *box, float inset_left, float inset_right, float inset_top, float inset_bottom) {
    { // 0x00198590 - 0x001985E0
    }
}

// 0x001985E0 - 0x00198624 (0x44) xFontBoxSetDrawSpacing__FP8xFontBoxff
void xFontBoxSetDrawSpacing(class xFontBox *box, float x_spacing, float y_spacing) {
    { // 0x001985E0 - 0x00198624
    }
}

// 0x00198630 - 0x00198674 (0x44) xFontBoxSetColor__FP8xFontBox10iColor_tag
void xFontBoxSetColor(class xFontBox *box, class iColor_tag color) {
    { // 0x00198630 - 0x00198674
    }
}

// 0x00198680 - 0x001986FC (0x7C) xFontBoxSetFont__FP8xFontBoxUi
void xFontBoxSetFont(class xFontBox *box, unsigned int id) {
    { // 0x00198680 - 0x001986FC
        class xFontInfo *font; // r6
    }
}

// 0x00198700 - 0x00198718 (0x18) xFontBoxSetText__FP8xFontBoxPc
void xFontBoxSetText(class xFontBox *box, char *text) {
    { // 0x00198700 - 0x00198718
    }
}

// 0x00198720 - 0x00198728 (0x8) xFontBoxDestroy__FP8xFontBox
void xFontBoxDestroy(class xFontBox *box) {
    { // 0x00198720 - 0x00198728
    }
}

// 0x00198730 - 0x001988DC (0x1AC) xFontBoxRender__FP8xFontBox
char *xFontBoxRender(class xFontBox *box) {
    { // 0x00198730 - 0x001988DC
        float maxy; // 
        float maxx; // 
        float miny; // 
        float minx; // 
        char *ret; // r16
        char *t; // r2
        char *s; // r18
    }
}

// 0x001988E0 - 0x00198E5C (0x57C) JustifyLine__FP8xFontBoxPcff
static char *JustifyLine(class xFontBox *box, char *text, float maxwidth, float maxheight) {
    { // 0x001988E0 - 0x00198E5C
        float space; // 
        float space; // 
        class xFontInfo *font; // r21
        char *end; // r20
        char *t; // r2
        char *s; // r19
        float testheight; // r29+0x9c
        float testwidth; // r29+0x98
        float whitespace_width; // 
        float height; // r29+0xa0
        float width; // 
    }
}

// 0x00198E60 - 0x00199238 (0x3D8) ProcessWord__FP8xFontBoxPcPfPff
static char *ProcessWord(class xFontBox *box, char *text, float *width, float *height, float maxw) {
    { // 0x00198E60 - 0x00199238
        char c; // r2
        float dx; // 
        class iColor_tag color; // r29+0xb4
        class xFontInfo *font; // r17
        unsigned int fontID; // r2
        char *s; // r16
    }
}

// 0x00199240 - 0x001993CC (0x18C) xFontBoxCreate__FffffUi10iColor_tag
class xFontBox *xFontBoxCreate(float x, float y, float w, float h, unsigned int id, class iColor_tag color) {
    { // 0x00199240 - 0x001993CC
        class xFontInfo *font; // r3
        class xFontBox *box; // r2
        unsigned int i; // r8
    }
}

// 0x001993D0 - 0x00199414 (0x44) xFontBoxSetDrawSize__FP8xFontBoxff
void xFontBoxSetDrawSize(class xFontBox *box, float draw_width, float draw_height) {
    { // 0x001993D0 - 0x00199414
    }
}

// 0x00199420 - 0x0019947C (0x5C) xFontRender__Fv
void xFontRender() {
    { // 0x00199420 - 0x0019947C
        class xFontBox *box; // r17
        unsigned int i; // r16
    }
}

// 0x00199480 - 0x00199558 (0xD8) xFontInit__Fv
void xFontInit() {
    { // 0x00199480 - 0x00199558
        float pdone; // r29+0x10
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iFont.cpp
// ==========================================================================
unsigned int ourGlobals[4096]; // 0x002B5AD0
class xFontGlobals_t gFontGlobals; // 0x002B33E0
class xFontInfo gFontList[4]; // 0x0036F6B0
unsigned int gDBfont; // 0x003580DC
static class RwRaster *sButtonRaster; // 0x003580FC
static class RwRaster *sRaster; // 0x003580F8
static float nsz; // 0x003580F4
static float rcz; // 0x003580F0
static class RxRenderStateVector rsv; // 0x0036FE80
float gFontWiggleTime; // 0x003580EC
float gFontWiggle; // 0x003580E8
static signed int im2d_vert_ct; // 0x003580E4
static class RwSky2DVertex *im2d_vert; // 0x003580E0
// 0x00199560 - 0x00199634 (0xD4) iFontInit__Fv
void iFontInit() {
    { // 0x00199560 - 0x00199634
        class RwTexture *button_texture; // r2
        class RwTexture *font_texture; // r16
        unsigned int id; // r2
    }
}

// 0x00199640 - 0x00199AA4 (0x464) iFontInitFont__FP9RwTexturePCcUiUsUsUsUsUs
static unsigned int iFontInitFont(class RwTexture *texture, char *characters, unsigned int layout, unsigned short u, unsigned short v, unsigned short char_width, unsigned short char_height, unsigned short flags) {
    { // 0x00199640 - 0x00199AA4
        unsigned int ov; // r8
        unsigned int ou; // r7
        unsigned char maxu; // r6
        unsigned char minu; // r5
        unsigned char vv; // r4
        unsigned char uu; // r3
        unsigned char c; // r2
        unsigned char *imp; // r2
        class RwImage *image; // r23
        unsigned char val; // r7
        unsigned int tex_height; // r30
        unsigned int tex_width; // r29+0xa0
        class xFontInfo *font; // r21
        char t; // r6
        char *s; // r5
        unsigned int id; // r20
    }
}

// 0x00199AB0 - 0x0019A6B8 (0xC08) iFontDrawChar__Fcf10iColor_tagP8xFontBox
void iFontDrawChar(char c, class iColor_tag color, class xFontBox *box) {
    { // 0x00199AB0 - 0x0019A6B8
        float pulse_h; // 
        float pulse_w; // 
        float wave; // 
        float pulse_h; // 
        float pulse_w; // 
        float wave; // 
        float _dy; // 
        float _dx; // 
        float _y; // 
        float _x; // 
        float n; // 
        float n; // r29+0xd0
        float n; // 
        float n; // r29+0xd0
        float sy; // 
        float sx; // 
        unsigned int draw_bg; // r23
        class RwSky2DVertex *vert; // r16
        class xFontInfo *font; // r16
        unsigned int i; // r2
        float dv; // 
        float du; // 
        float v; // 
        float u; // 
        float dy; // 
        float dx; // 
        float y; // 
        float x; // 
    }
}

// 0x0019A750 - 0x0019AB3C (0x3EC) iFontDrawBackdrop__FP8xFontBox
void iFontDrawBackdrop(class xFontBox *box) {
    { // 0x0019A750 - 0x0019AB3C
        class RwSky2DVertex vert[4]; // r29+0x10
    }
}

// 0x0019AB40 - 0x0019AC14 (0xD4) iFontRestoreRenderState__Fv
void iFontRestoreRenderState() {
    { // 0x0019AB40 - 0x0019AC14
    }
}

// 0x0019AC20 - 0x0019ACBC (0x9C) iFontSetCharRenderState__FP8xFontBox
void iFontSetCharRenderState(class xFontBox *box) {
    { // 0x0019AC20 - 0x0019ACBC
        class xFontInfo *font; // r16
    }
}

// 0x0019ACC0 - 0x0019AD10 (0x50) iFontSetBackdropRenderState__FP8xFontBox
void iFontSetBackdropRenderState() {
    { // 0x0019ACC0 - 0x0019AD10
    }
}

// 0x0019AD10 - 0x0019AD64 (0x54) iFontSaveRenderState__Fv
void iFontSaveRenderState() {
    { // 0x0019AD10 - 0x0019AD64
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntBubble.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (zEntBubbleEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x0019AEE0
void (zEntBubble_Update)(class _zEntBubble *, class _xScene *, float); // 0x0019B0A0
void (xEntDefaultBoundUpdate)(class _xEnt *, class _xVec3 *); // 0x0012C260
void (zEntBubble_Move)(class _zEntBubble *, class _xScene *, float, class _xEntFrame *); // 0x0019B090
class _xMat4x3 g_I3; // 0x00342E70
class zGlobals globals; // 0x00362FD0
static class _zParEmitter *sEmitSoapFlight; // 0x00358108
static class _zParEmitter *sEmitGumWad; // 0x00358104
static class RpAtomic *gBubbleModel; // 0x00358100
// 0x0019AD70 - 0x0019AEDC (0x16C) zEntBubble_FX__FP11_zEntBubblef
static void zEntBubble_FX(class _zEntBubble *ent, float dt) {
    { // 0x0019AD70 - 0x0019AEDC
        class _zParEmitterCustomSettings info; // r29+0x30
    }
}

// 0x0019AEE0 - 0x0019AEE8 (0x8) zEntBubbleEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntBubbleEventCB() {
    { // 0x0019AEE0 - 0x0019AEE8
    }
}

// 0x0019AEF0 - 0x0019AFDC (0xEC) zEntBubble_Render__Fv
void zEntBubble_Render() {
    { // 0x0019AEF0 - 0x0019AFDC
        class RwRGBA colsoap; // r29+0x5c
        class zScene *sc; // r17
        unsigned int i; // r16
    }
}

// 0x0019AFE0 - 0x0019B078 (0x98) zEntBubble_Cleanup__Fv
void zEntBubble_Cleanup() {
    { // 0x0019AFE0 - 0x0019B078
        class _zEnt *tmp; // r7
        class zScene *sc; // r6
        unsigned int i; // r5
    }
}

// 0x0019B080 - 0x0019B090 (0x10) zEntBubble_Pop__FP11_zEntBubble
void zEntBubble_Pop(class _zEntBubble *ent) {
    { // 0x0019B080 - 0x0019B090
    }
}

// 0x0019B090 - 0x0019B098 (0x8) zEntBubble_Move__FP11_zEntBubbleP7_xScenefP10_xEntFrame
void zEntBubble_Move(class _xEntFrame *frame) {
    { // 0x0019B090 - 0x0019B098
    }
}

// 0x0019B0A0 - 0x0019B770 (0x6D0) zEntBubble_Update__FP11_zEntBubbleP7_xScenef
void zEntBubble_Update(class _zEntBubble *ent, class _xScene *sc, float dt) {
    { // 0x0019B0A0 - 0x0019B770
        enum en_ZVILLAIN_BUBBLE_RESPONSE response; // r2
        class _zEnt *vill; // r19
        class _xCollis *endcoll; // r18
        class _xCollis *coll; // r17
        float lerp; // 
        class _zParEmitterCustomSettings info; // r29+0x570
        float cpitch; // r29+0x5d0
        float spitch; // 
        float mag; // r29+0x5d0
        float dz; // 
        float dy; // 
        float dx; // 
        float flyspeed; // 
        class _xEntCollis coll; // r29+0x70
    }
}

// 0x0019B770 - 0x0019B968 (0x1F8) zEntBubble_Launch__Ff
void zEntBubble_Launch(float wait) {
    { // 0x0019B770 - 0x0019B968
        class _zEntBubble *bub; // r16
    }
}

// 0x0019B970 - 0x0019B9C8 (0x58) zEntBubble_DisableCollision__Fv
void zEntBubble_DisableCollision() {
    { // 0x0019B970 - 0x0019B9C8
        class zScene *sc; // r6
        signed int i; // r5
    }
}

// 0x0019B9D0 - 0x0019BA58 (0x88) zEntBubble_EnableCollision__Fv
void zEntBubble_EnableCollision() {
    { // 0x0019B9D0 - 0x0019BA58
        class zScene *sc; // r7
        signed int i; // r6
    }
}

// 0x0019BA60 - 0x0019BCD0 (0x270) AutoTarget__Fv
static class _zEnt *AutoTarget() {
    { // 0x0019BA60 - 0x0019BCD0
        float spitch; // r29+0x90
        float xzdist; // 
        float dz; // 
        float dy; // 
        float dx; // 
        class _zEnt **vill; // r2
        class _zEnt *near; // r17
        float nearDist; // 
        float pitchSin; // r29+0x8c
        float plusAngle; // 
        float minusAngle; // 
        float dirAngle; // 
        unsigned int i; // r16
    }
}

// 0x0019BCD0 - 0x0019BEB4 (0x1E4) zEntBubble_SceneInit__FP6zScene
void zEntBubble_SceneInit(class zScene *scene) {
    { // 0x0019BCD0 - 0x0019BEB4
        class _xEntFrame *frameBlock; // r2
        class _zEntBubble *bubbleBlock; // r2
        unsigned int bufsize; // r29+0x7c
        unsigned int idx; // r17
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ms4sound.cpp
// ==========================================================================
void (JP)(void *); // 0x0019C9F0
signed int jptest; // 0x00000000
unsigned int iopFrameCount; // 0x00358144
unsigned long SOUND_SPUVol[48]; // 0x00371570
unsigned char SOUND_SPUKeyStatus[48]; // 0x002B33F0
class _sif_client_data gCd; // 0x00371540
unsigned int SOUND_IOP_DATA_ID; // 0x00358140
unsigned int SOUND_IOP_DATA_CHECK; // 0x0035813C
unsigned int SOUND_IOP_DATA_SEEK; // 0x00358138
unsigned int SOUND_IOP_DATA_SIZE; // 0x00358134
unsigned int SOUND_IOP_DATA_ADDRESS; // 0x00358130
signed int SOUND_SFX_TRANSFER_END; // 0x0035812C
signed int SOUND_SFX_TRANSFER_STATUS; // 0x00000000
signed int SOUND_STREAM_ENV[48]; // 0x00371480
signed int SOUND_STREAM_PLAY_HALF[48]; // 0x003713C0
signed int SOUND_STREAM_SPU_ADDR[48]; // 0x00371300
signed int SOUND_STREAM_ID[48]; // 0x00371240
signed int SOUND_STREAM_SPU_CHAN[48]; // 0x00371180
signed int SOUND_STREAM_STATUS[48]; // 0x003710C0
signed int SOUND_IOP_REQUEST_LOAD; // 0x003575B0
signed int SOUND_STREAM_CD_ACCESS_CHAN; // 0x00358128
signed int SOUND_STREAM_CD_ACCESS; // 0x00358124
signed int SOUND_SPU_ADDR; // 0x00358120
signed int SOUND_CD_STATUS; // 0x0035811C
signed int SOUND_STEREO_READY_FLAG; // 0x00358118
unsigned int *sbuff2; // 0x00358114
unsigned int cached_sbuff2[128]; // 0x00370EC0
unsigned short IOPCommandData[1024]; // 0x003706C0
unsigned short senddata[1024]; // 0x0036FEC0
signed int IOP_Cnt; // 0x00358110
static unsigned int StreamIRXVersion; // 0x0035810C
// 0x0019BEC0 - 0x0019BEE0 (0x20) SOUND_DelayThread__FUl
void SOUND_DelayThread(unsigned long microsec) {
    { // 0x0019BEC0 - 0x0019BEE0
    }
}

// 0x0019BEE0 - 0x0019BF20 (0x40) SOUND_LoadBlock__FUlPvUi
void SOUND_LoadBlock(void *cmd, unsigned int size) {
    { // 0x0019BEE0 - 0x0019BF20
    }
}

// 0x0019BF20 - 0x0019BF34 (0x14) SOUND_SetDVDMode__Fi
void SOUND_SetDVDMode(signed int mode) {
    { // 0x0019BF20 - 0x0019BF34
    }
}

// 0x0019BF40 - 0x0019BF54 (0x14) SOUND_SetCutsceneMode__Fi
void SOUND_SetCutsceneMode(signed int mode) {
    { // 0x0019BF40 - 0x0019BF54
    }
}

// 0x0019BF60 - 0x0019BF98 (0x38) SOUND_SetHIPName__FiPc
void SOUND_SetHIPName(signed int idx, char *HIP) {
    { // 0x0019BF60 - 0x0019BF98
    }
}

// 0x0019BFA0 - 0x0019BFD8 (0x38) SOUND_SetFileInfo__FiP12iSndFileInfo
void SOUND_SetFileInfo(signed int slot, class iSndFileInfo *info) {
    { // 0x0019BFA0 - 0x0019BFD8
    }
}

// 0x0019BFE0 - 0x0019BFFC (0x1C) SOUND_PauseChannel__Fii
void SOUND_PauseChannel(signed int channel, signed int pause) {
    { // 0x0019BFE0 - 0x0019BFFC
    }
}

// 0x0019C000 - 0x0019C0AC (0xAC) FlushIOPCommand__Fi
signed int FlushIOPCommand(signed int type) {
    { // 0x0019C000 - 0x0019C0AC
        signed int a; // r2
        signed int oldcnt; // r2
    }
}

// 0x0019C0B0 - 0x0019C10C (0x5C) SOUND_CheckAndStartStereo__FUiUiUi
signed int SOUND_CheckAndStartStereo(unsigned int spuchannel1, unsigned int spuchannel2, unsigned int pitch) {
    { // 0x0019C0B0 - 0x0019C10C
    }
}

// 0x0019C110 - 0x0019C450 (0x340) SOUND_CopyIOPBuffer__Fv
void SOUND_CopyIOPBuffer() {
    { // 0x0019C110 - 0x0019C450
        signed int count; // r6
        signed int streamnum; // r16
        signed int streams; // r5
        signed int l; // r7
        signed int k; // r8
        signed int j; // r15
        signed int i; // r3
    }
}

// 0x0019C450 - 0x0019C6B4 (0x264) SetIOPCommand__Fss
void SetIOPCommand(signed short command, signed short length) {
    { // 0x0019C450 - 0x0019C6B4
        signed int i; // r15
    }
}

// 0x0019C6C0 - 0x0019C704 (0x44) SOUND_AllocateStreamBuffer__FiUiUi
void SOUND_AllocateStreamBuffer(signed int strm, unsigned int spuaddr, unsigned int size) {
    { // 0x0019C6C0 - 0x0019C704
    }
}

// 0x0019C710 - 0x0019C724 (0x14) SOUND_SetMaxStreamLimit__Fi
void SOUND_SetMaxStreamLimit(signed int limit) {
    { // 0x0019C710 - 0x0019C724
    }
}

// 0x0019C730 - 0x0019C744 (0x14) SOUND_CloseStreamBuffer__Fi
void SOUND_CloseStreamBuffer(signed int strm) {
    { // 0x0019C730 - 0x0019C744
    }
}

// 0x0019C750 - 0x0019C764 (0x14) SOUND_InitStreamData__Fi
void SOUND_InitStreamData(signed int loadtype) {
    { // 0x0019C750 - 0x0019C764
    }
}

// 0x0019C770 - 0x0019C77C (0xC) SOUND_InitSPU__Fv
void SOUND_InitSPU() {
    { // 0x0019C770 - 0x0019C77C
    }
}

// 0x0019C780 - 0x0019C79C (0x1C) SOUND_GetStatus__Fii
void SOUND_GetStatus(signed int AllowLoad, signed int reqcheck) {
    { // 0x0019C780 - 0x0019C79C
    }
}

// 0x0019C7A0 - 0x0019C7E0 (0x40) SOUND_StopSound__Fi
signed int SOUND_StopSound(signed int SPUChannel) {
    { // 0x0019C7A0 - 0x0019C7E0
    }
}

// 0x0019C7E0 - 0x0019C818 (0x38) SOUND_StopStream__Fi
signed int SOUND_StopStream(signed int SPUChannel) {
    { // 0x0019C7E0 - 0x0019C818
    }
}

// 0x0019C820 - 0x0019C868 (0x48) SOUND_SetChannelPitch__Fii
signed int SOUND_SetChannelPitch(signed int Chan, signed int Pch) {
    { // 0x0019C820 - 0x0019C868
    }
}

// 0x0019C870 - 0x0019C8C0 (0x50) SOUND_SetChannelVolume__Fiii
signed int SOUND_SetChannelVolume(signed int Chan, signed int VL, signed int VR) {
    { // 0x0019C870 - 0x0019C8C0
    }
}

// 0x0019C8C0 - 0x0019C944 (0x84) SOUND_PlaySFX__Fiiiiiii
signed int SOUND_PlaySFX(signed int ID, signed int Chan, signed int VL, signed int VR, signed int Spd, signed int at, signed int rl) {
    { // 0x0019C8C0 - 0x0019C944
        signed int adsr; // r2
    }
}

// 0x0019C950 - 0x0019C9E8 (0x98) SOUND_PlayStream__Fiiiiiiiii
signed int SOUND_PlayStream(signed int Stream, signed int StrmChan, signed int Channel, signed int VL, signed int VR, signed int Spd, signed int flag, signed int at, signed int rl) {
    { // 0x0019C950 - 0x0019C9E8
        signed int adsr; // r2
    }
}

// 0x0019C9F0 - 0x0019C9F8 (0x8) JP__FPv
void JP() {
    { // 0x0019C9F0 - 0x0019C9F8
    }
}

// 0x0019CA00 - 0x0019CA88 (0x88) SOUND_InitIOP__Fv
signed int SOUND_InitIOP() {
    { // 0x0019CA00 - 0x0019CA88
        signed int i; // r3
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zMovePoint.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (zMovePointEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x0019D150
signed int (MVPT_dflt_chklos)(void *, class _xVec3 *, class _zMovePoint *); // 0x0019CA90
class zGlobals globals; // 0x00362FD0
static signed int g_mvpt_cnt; // 0x0035814C
static class _zMovePoint *g_mvpt_list; // 0x00358148
// 0x0019CA90 - 0x0019CBE0 (0x150) MVPT_dflt_chklos__FPvP6_xVec3PC11_zMovePoint
static signed int MVPT_dflt_chklos(class _xVec3 *mypos, class _zMovePoint *mvpt) {
    { // 0x0019CA90 - 0x0019CBE0
        class _xCollis colrec; // r29+0x50
        class _xVec3 vec_tgt; // r29+0xa0
        class _xRay3 ray_los; // r29+0x20
    }
}

// 0x0019CBE0 - 0x0019D0C8 (0x4E8) zMovePointFindNearest__FiP6_xVec3ffPvPFPvP6_xVec3PC11_zMovePoint_iPiPP11_zMovePoint
class _zMovePoint *zMovePointFindNearest(signed int flg_opts, class _xVec3 *yourpos, float okdist, float nohope, void *yourdata, signed int (*losfunc)(void *, class _xVec3 *, class _zMovePoint *), signed int *idx_resume, class _zMovePoint **prev_best) {
    { // 0x0019CBE0 - 0x0019D0C8
        class _xVec3 delta; // r29+0x150
        float ds2_far; // 
        float ds2_best; // 
        float ds2_nohope; // 
        float ds2_ok; // 
        signed int rc; // r2
        signed int suspend; // r20
        signed int eidx; // r22
        signed int sidx; // r2
        signed int i; // r2
        class _zMovePoint *cur_best; // r19
        class _zMovePoint *da_mvpt; // r29+0xcc
    }
}

// 0x0019D0D0 - 0x0019D0F4 (0x24) zMovePointFindNearest__FiP6_xVec3PvPFPvP6_xVec3PC11_zMovePoint_i
class _zMovePoint *zMovePointFindNearest(signed int flg_opts, class _xVec3 *yourpos, void *yourdata, signed int (*losfunc)(void *, class _xVec3 *, class _zMovePoint *)) {
    { // 0x0019D0D0 - 0x0019D0F4
    }
}

// 0x0019D100 - 0x0019D118 (0x18) zMovePointIsOn__FPC11_zMovePoint
signed int zMovePointIsOn(class _zMovePoint *m) {
    { // 0x0019D100 - 0x0019D118
    }
}

// 0x0019D120 - 0x0019D128 (0x8) zMovePointGetDelay__FPC11_zMovePoint
float zMovePointGetDelay(class _zMovePoint *m) {
    { // 0x0019D120 - 0x0019D128
    }
}

// 0x0019D130 - 0x0019D138 (0x8) zMovePointGetPos__FPC11_zMovePoint
class _xVec3 *zMovePointGetPos(class _zMovePoint *m) {
    { // 0x0019D130 - 0x0019D138
    }
}

// 0x0019D140 - 0x0019D148 (0x8) zMovePointGetNext__FPC11_zMovePointPC11_zMovePointPP11_zMovePointP6_xVec3
float zMovePointGetNext(class _zMovePoint *current, class _zMovePoint *prev, class _zMovePoint **next, class _xVec3 *hdng) {
    { // 0x0019D140 - 0x0019D148
        float dist; // r29+0x0
    }
}

// 0x0019D150 - 0x0019D1B0 (0x60) zMovePointEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zMovePointEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x0019D150 - 0x0019D1B0
        class _zMovePoint *m; // r2
    }
}

// 0x0019D1B0 - 0x0019D1B8 (0x8) zMovePointLoad__FP11_zMovePointP7xSerial
void zMovePointLoad(class _zMovePoint *ent, class xSerial *s) {
    { // 0x0019D1B0 - 0x0019D1B8
    }
}

// 0x0019D1C0 - 0x0019D1C8 (0x8) zMovePointSave__FP11_zMovePointP7xSerial
void zMovePointSave(class _zMovePoint *ent, class xSerial *s) {
    { // 0x0019D1C0 - 0x0019D1C8
    }
}

// 0x0019D1D0 - 0x0019D218 (0x48) zMovePoint_From_xAssetID__FUi
class _zMovePoint *zMovePoint_From_xAssetID(unsigned int aid) {
    { // 0x0019D1D0 - 0x0019D218
        signed int i; // r5
        class _zMovePoint *da_mvpt; // r2
    }
}

// 0x0019D220 - 0x0019D228 (0x8) zMovePointSetup__FP11_zMovePointP6zScene
void zMovePointSetup(class _zMovePoint *mvpt, class zScene *scn) {
    { // 0x0019D220 - 0x0019D228
    }
}

// 0x0019D230 - 0x0019D250 (0x20) zMovePoint_GetInst__Fi
class _zMovePoint *zMovePoint_GetInst(signed int n) {
    { // 0x0019D230 - 0x0019D250
    }
}

// 0x0019D250 - 0x0019D2B4 (0x64) zMovePointInit__FP11_zMovePointP16p2MovePointAsset
void zMovePointInit(class _zMovePoint *m, class p2MovePointAsset *asset) {
    { // 0x0019D250 - 0x0019D2B4
    }
}

// 0x0019D2C0 - 0x0019D318 (0x58) zMovePoint_GetMemPool__Fi
class _zMovePoint *zMovePoint_GetMemPool(signed int cnt) {
    { // 0x0019D2C0 - 0x0019D318
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xBase.cpp
// ==========================================================================
// 0x0019D320 - 0x0019D338 (0x18) xBaseReset__FP5xBaseP11p2BaseAsset
void xBaseReset(class xBase *xb, class p2BaseAsset *asset) {
    { // 0x0019D320 - 0x0019D338
    }
}

// 0x0019D340 - 0x0019D398 (0x58) xBaseLoad__FP5xBaseP7xSerial
void xBaseLoad(class xBase *ent, class xSerial *s) {
    { // 0x0019D340 - 0x0019D398
        signed int b; // r29+0x2c
    }
}

// 0x0019D3A0 - 0x0019D3E0 (0x40) xBaseSave__FP5xBaseP7xSerial
void xBaseSave(class xBase *ent, class xSerial *s) {
    { // 0x0019D3A0 - 0x0019D3E0
    }
}

// 0x0019D3E0 - 0x0019D3E8 (0x8) xBaseSetup__FP5xBase
void xBaseSetup() {
    { // 0x0019D3E0 - 0x0019D3E8
    }
}

// 0x0019D3F0 - 0x0019D424 (0x34) xBaseInit__FP5xBaseP11p2BaseAsset
void xBaseInit(class xBase *xb, class p2BaseAsset *asset) {
    { // 0x0019D3F0 - 0x0019D424
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iScrFX.cpp
// ==========================================================================
// 0x0019D430 - 0x0019D5A0 (0x170) iScrFxDrawLetterbox__FP8RwCamerai
void iScrFxDrawLetterbox(signed int y_offset) {
    { // 0x0019D430 - 0x0019D5A0
        class RwVideoMode video_mode; // r29+0x40
        static unsigned short indices[4]; // 0x003575C0
        static class RwSky2DVertex v[4]; // 0x00371890
    }
}

// 0x0019D5A0 - 0x0019D95C (0x3BC) iScrFxDrawFade__FP8RwCameraP10iColor_tag
void iScrFxDrawFade(class iColor_tag *col) {
    { // 0x0019D5A0 - 0x0019D95C
        class RwVideoMode video_mode; // r29+0x20
        static unsigned short indices[4]; // 0x003575B8
        static class RwSky2DVertex v[4]; // 0x00371790
    }
}

// 0x0019D960 - 0x0019D9C8 (0x68) iScrFxEnd__Fv
void iScrFxEnd() {
    { // 0x0019D960 - 0x0019D9C8
    }
}

// 0x0019D9D0 - 0x0019DA44 (0x74) iScrFxBegin__Fv
void iScrFxBegin() {
    { // 0x0019D9D0 - 0x0019DA44
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iSnd.cpp
// ==========================================================================
signed int SOUND_STREAM_STATUS[48]; // 0x003710C0
signed int SOUND_SFX_TRANSFER_END; // 0x0035812C
signed int SOUND_CD_STATUS; // 0x0035811C
signed int SOUND_STREAM_CD_ACCESS; // 0x00358124
signed int SOUND_STREAM_CD_ACCESS_CHAN; // 0x00358128
signed int DVD; // 0x00357CA4
unsigned int SOUND_IOP_DATA_CHECK; // 0x0035813C
unsigned int SOUND_IOP_DATA_ADDRESS; // 0x00358130
unsigned int SOUND_IOP_DATA_ID; // 0x00358140
unsigned int SOUND_IOP_DATA_SEEK; // 0x00358138
unsigned int SOUND_IOP_DATA_SIZE; // 0x00358134
signed int SOUND_IOP_REQUEST_LOAD; // 0x003575B0
unsigned char SOUND_SPUKeyStatus[48]; // 0x002B33F0
class xSndGlobals gSnd; // 0x00373B80
static void (*sCutsceneCallback)(class iSndCutsceneCallbackInfo *); // 0x00358170
static class SIPLoadInfo LastLoaded[8]; // 0x00000000
signed int previous; // 0x0035816C
signed int did; // 0x00358168
signed int EXTERNAL_LOAD_FLAG; // 0x00358164
unsigned int EXTERNAL_TotalFiles; // 0x00000000
unsigned int FILE_SIZE; // 0x00000000
signed int EXTERNAL_NewRequestCheck; // 0x00358160
static unsigned int first_time; // 0x003575C8
static unsigned int sfx_total; // 0x0035815C
static unsigned int SPUaddr; // 0x00358158
static unsigned int EEinfo_max; // 0x00358154
static class iSndFileInfo EEinfo[351]; // 0x00371A90
static unsigned int HIPct; // 0x00358150
static char HIP[8][32]; // 0x00371990
class ADPCM_FILENAME_INFO ADPCM_FileNames[351]; // 0x00000000
// 0x0019DA50 - 0x0019DA7C (0x2C) iSndCutsceneMode__FPFP24iSndCutsceneCallbackInfo_v
void iSndCutsceneMode(void (*callback)(class iSndCutsceneCallbackInfo *)) {
    { // 0x0019DA50 - 0x0019DA7C
    }
}

// 0x0019DA80 - 0x0019DBFC (0x17C) iSndWaitForDeadSounds__Fv
void iSndWaitForDeadSounds() {
    { // 0x0019DA80 - 0x0019DBFC
        signed int ret; // r2
        class xSndVoiceInfo *vp; // r19
        unsigned long zombies; // r18
        signed int numdelay; // r17
        signed int i; // r16
    }
}

// 0x0019DC00 - 0x0019DD0C (0x10C) iSndSetPitch__FUif
void iSndSetPitch(unsigned int snd, float pitch) {
    { // 0x0019DC00 - 0x0019DD0C
        signed int i; // r16
    }
}

// 0x0019DD10 - 0x0019DE20 (0x110) iSndStop__FUi
void iSndStop(unsigned int snd) {
    { // 0x0019DD10 - 0x0019DE20
        signed int i; // r16
    }
}

// 0x0019DE20 - 0x0019DE80 (0x60) iSndPause__FUiUi
void iSndPause(unsigned int snd, unsigned int pause) {
    { // 0x0019DE20 - 0x0019DE80
        signed int i; // r6
    }
}

// 0x0019DE80 - 0x0019E000 (0x180) iSndFindFreeVoice__FUiUi
unsigned int iSndFindFreeVoice(unsigned int priority, unsigned int flags) {
    { // 0x0019DE80 - 0x0019E000
        signed int R; // r2
        signed int L; // r2
        signed int volR; // r22
        class xSndVoiceInfo *vp; // r21
        signed int bestvol; // r20
        unsigned int bestpri; // r19
        unsigned int best; // r18
        unsigned int vhi; // r17
        unsigned int vlo; // r16
        unsigned int i; // r2
    }
}

// 0x0019E000 - 0x0019E18C (0x18C) iSndStartStereo__FUiUif
void iSndStartStereo(unsigned int id1, unsigned int id2, float pitch) {
    { // 0x0019E000 - 0x0019E18C
        unsigned int v2; // r17
        unsigned int v1; // r16
    }
}

// 0x0019E190 - 0x0019E218 (0x88) iSndIsReady__FUi
unsigned int iSndIsReady(unsigned int id) {
    { // 0x0019E190 - 0x0019E218
        unsigned int v; // r2
    }
}

// 0x0019E220 - 0x0019E440 (0x220) iSndPlay__FP13xSndVoiceInfo
unsigned int iSndPlay(class xSndVoiceInfo *vp) {
    { // 0x0019E220 - 0x0019E440
        unsigned int voice; // r16
        unsigned int i; // r4
    }
}

// 0x0019E440 - 0x0019E4A0 (0x60) iSndIsPlaying__FUiUi
unsigned int iSndIsPlaying(unsigned int assetID, unsigned int parid) {
    { // 0x0019E440 - 0x0019E4A0
        unsigned int i; // r3
    }
}

// 0x0019E4A0 - 0x0019E4EC (0x4C) iSndIsPlaying__FUi
unsigned int iSndIsPlaying(unsigned int assetID) {
    { // 0x0019E4A0 - 0x0019E4EC
        unsigned int i; // r3
    }
}

// 0x0019E4F0 - 0x0019E838 (0x348) iSndUpdate__Ff
void iSndUpdate() {
    { // 0x0019E4F0 - 0x0019E838
        class iSndCutsceneCallbackInfo info; // r29+0x40
        class iSndVol nvol; // r29+0x54
        class iSndVol nvol; // r29+0x50
        class _xEnt *ent; // r3
        class xSndVoiceInfo *vp; // r3
        signed int i; // r2
    }
}

// 0x0019E840 - 0x0019EB6C (0x32C) iSndCalcVol__FP13xSndVoiceInfo
class iSndVol iSndCalcVol(class xSndVoiceInfo *vp) {
    { // 0x0019E840 - 0x0019EB6C
        float volscale; // r29+0x90
        float pan; // 
        class _xVec3 to; // r29+0x78
        float volR; // r29+0x90
        float volL; // r29+0x90
        class iSndVol nvol; // r29+0x84
    }
}

// 0x0019EB70 - 0x0019EB78 (0x8) iSndExit__Fv
void iSndExit() {
    { // 0x0019EB70 - 0x0019EB78
    }
}

// 0x0019EB80 - 0x0019ECF8 (0x178) iSndInit2__Fv
void iSndInit2() {
    { // 0x0019EB80 - 0x0019ECF8
        signed long t1; // r18
        signed long t0; // r17
        signed int i; // r17
    }
}

// 0x0019ED00 - 0x0019F410 (0x710) iSndLoadFaster__Fv
void iSndLoadFaster() {
    { // 0x0019ED00 - 0x0019F410
        char hipname[32]; // r29+0x150
        signed int hip_idx; // r2
        unsigned int assetID; // r21
        unsigned int predicted_sector; // r29+0xe0
        unsigned int first_offset; // r29+0xdc
        unsigned int offset; // r29+0xc0
        class SndBlockEntry *ep; // r20
        class SndBlockHdr *hdr; // r22
        class st_xST_XASSETINFO xinfo; // r29+0x130
        unsigned int data; // r29+0xb0
        char *hipname; // r2
        class iSndFileInfo *ip; // r5
        unsigned short *cmdbuf; // r2
        class VAGheader *vp; // r16
        class SndInfo *p; // r30
        signed int sip; // r29+0xa0
        signed int i; // r17
        unsigned int sz; // r29+0x17c
    }
}

// 0x0019F410 - 0x0019F4EC (0xDC) iSndInit__Fv
void iSndInit() {
    { // 0x0019F410 - 0x0019F4EC
        signed int i; // r16
        char *module[3]; // r29+0x50
        static char empty[1]; // 0x003575CC
    }
}

// 0x0019F4F0 - 0x0019F5E4 (0xF4) iSndSuspendCD__FUi
void iSndSuspendCD(unsigned int suspend) {
    { // 0x0019F4F0 - 0x0019F5E4
    }
}

// 0x0019F5F0 - 0x0019F638 (0x48) iSndLookup__FUi
class iSndFileInfo *iSndLookup(unsigned int id) {
    { // 0x0019F5F0 - 0x0019F638
        class iSndFileInfo *eep; // r2
        unsigned int i; // r5
    }
}

// 0x0019F640 - 0x0019F700 (0xC0) TransferDataToIOP__FUiUiUi
signed int TransferDataToIOP(unsigned int source, unsigned int dest, unsigned int size) {
    { // 0x0019F640 - 0x0019F700
        class sceSifDmaData JDMA; // r29+0x60
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xSnd.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
class xSndGlobals gSnd; // 0x00373B80
// 0x0019F700 - 0x0019F708 (0x8) xSndCutsceneMode__FPFP24iSndCutsceneCallbackInfo_v
void xSndCutsceneMode(void (*callback)(class iSndCutsceneCallbackInfo *)) {
    { // 0x0019F700 - 0x0019F708
    }
}

// 0x0019F710 - 0x0019F718 (0x8) xSndSetPitch__FUif
void xSndSetPitch(unsigned int snd, float pitch) {
    { // 0x0019F710 - 0x0019F718
    }
}

// 0x0019F720 - 0x0019F7A0 (0x80) xSndStopChildren__FUi
void xSndStopChildren(unsigned int pid) {
    { // 0x0019F720 - 0x0019F7A0
        class xSndVoiceInfo *vp; // r17
        unsigned int i; // r16
    }
}

// 0x0019F7A0 - 0x0019F7E0 (0x40) xSndParentDied__FUi
void xSndParentDied(unsigned int pid) {
    { // 0x0019F7A0 - 0x0019F7E0
        class xSndVoiceInfo *vp; // r6
        unsigned int i; // r5
    }
}

// 0x0019F7E0 - 0x0019F7E8 (0x8) xSndStop__FUi
void xSndStop(unsigned int snd) {
    { // 0x0019F7E0 - 0x0019F7E8
    }
}

// 0x0019F7F0 - 0x0019F7F8 (0x8) xSndPause__FUiUi
void xSndPause(unsigned int snd, unsigned int pause) {
    { // 0x0019F7F0 - 0x0019F7F8
    }
}

// 0x0019F800 - 0x0019F808 (0x8) xSndStartStereo__FUiUif
void xSndStartStereo(unsigned int id1, unsigned int id2, float pitch) {
    { // 0x0019F800 - 0x0019F808
    }
}

// 0x0019F810 - 0x0019F818 (0x8) xSndIsReady__FUi
unsigned int xSndIsReady(unsigned int id) {
    { // 0x0019F810 - 0x0019F818
    }
}

// 0x0019F820 - 0x0019F844 (0x24) xSndSetFrameFar__FP6_xVec3
void xSndSetFrameFar(class _xVec3 *pos) {
    { // 0x0019F820 - 0x0019F844
    }
}

// 0x0019F850 - 0x0019F8E8 (0x98) xSndSetFrame__FP6_xVec3P6_xVec3P6_xVec3P6_xVec3f
void xSndSetFrame(class _xVec3 *right, class _xVec3 *up, class _xVec3 *at, class _xVec3 *pos, float radius) {
    { // 0x0019F850 - 0x0019F8E8
    }
}

// 0x0019F8F0 - 0x0019FA8C (0x19C) xSndPlay3D__FUiffUiUiP6_xVec3f
unsigned int xSndPlay3D(unsigned int id, float vol, float pitch, unsigned int priority, unsigned int flags, class _xVec3 *pos, float radius) {
    { // 0x0019F8F0 - 0x0019FA8C
        class xSndVoiceInfo *vp; // r17
        class iSndFileInfo *ip; // r16
        unsigned int voice; // r2
    }
}

// 0x0019FA90 - 0x0019FC34 (0x1A4) xSndPlay3D__FUiffUiUiP5_xEntf
unsigned int xSndPlay3D(unsigned int id, float vol, float pitch, unsigned int priority, unsigned int flags, class _xEnt *parent, float radius) {
    { // 0x0019FA90 - 0x0019FC34
        class xSndVoiceInfo *vp; // r17
        class iSndFileInfo *ip; // r16
        unsigned int voice; // r2
    }
}

// 0x0019FC40 - 0x0019FE28 (0x1E8) xSndPlay__FUifffUiUiUi
unsigned int xSndPlay(unsigned int id, float volL, float volR, float pitch, unsigned int priority, unsigned int flags, unsigned int parentID) {
    { // 0x0019FC40 - 0x0019FE28
        unsigned int sceneID; // r7
        class xSndVoiceInfo *vp; // r17
        class iSndFileInfo *ip; // r16
        unsigned int voice; // r2
    }
}

// 0x0019FE30 - 0x0019FE38 (0x8) xSndExit__Fv
void xSndExit() {
    { // 0x0019FE30 - 0x0019FE38
    }
}

// 0x0019FE40 - 0x0019FF20 (0xE0) xSndUpdate__Ff
void xSndUpdate(float time_elapsed) {
    { // 0x0019FE40 - 0x0019FF20
        float a; // 
    }
}

// 0x0019FF20 - 0x0019FF7C (0x5C) xSndStopAll__Fv
void xSndStopAll() {
    { // 0x0019FF20 - 0x0019FF7C
        unsigned int i; // r16
    }
}

// 0x0019FF80 - 0x001A0024 (0xA4) xSndPauseAll__FUiUi
void xSndPauseAll(unsigned int pause_effects, unsigned int pause_streams) {
    { // 0x0019FF80 - 0x001A0024
        unsigned int i; // r16
    }
}

// 0x001A0030 - 0x001A0044 (0x14) xSndSetFXVol__Fff
void xSndSetFXVol(float left, float right) {
    { // 0x001A0030 - 0x001A0044
    }
}

// 0x001A0050 - 0x001A0064 (0x14) xSndSetStreamVol__Fff
void xSndSetStreamVol(float left, float right) {
    { // 0x001A0050 - 0x001A0064
    }
}

// 0x001A0070 - 0x001A0084 (0x14) xSndSetMasterVol__Fff
void xSndSetMasterVol(float left, float right) {
    { // 0x001A0070 - 0x001A0084
    }
}

// 0x001A0090 - 0x001A011C (0x8C) xSndResume__Fv
void xSndResume() {
    { // 0x001A0090 - 0x001A011C
    }
}

// 0x001A0120 - 0x001A0294 (0x174) xSndSuspend__Fv
void xSndSuspend() {
    { // 0x001A0120 - 0x001A0294
    }
}

// 0x001A02A0 - 0x001A0354 (0xB4) xSndInit__Fv
void xSndInit() {
    { // 0x001A02A0 - 0x001A0354
        class _xVec3 origin; // r29+0x10
        class xSndVoiceInfo *voice; // r4
        unsigned int i; // r3
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zPortal.cpp
// ==========================================================================
signed int (zPortalEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A0360
class zGlobals globals; // 0x00362FD0
// 0x001A0360 - 0x001A03C4 (0x64) zPortalEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zPortalEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001A0360 - 0x001A03C4
    }
}

// 0x001A03D0 - 0x001A03D8 (0x8) zPortalLoad__FP8_zPortalP7xSerial
void zPortalLoad(class _zPortal *ent, class xSerial *s) {
    { // 0x001A03D0 - 0x001A03D8
    }
}

// 0x001A03E0 - 0x001A03E8 (0x8) zPortalSave__FP8_zPortalP7xSerial
void zPortalSave(class _zPortal *ent, class xSerial *s) {
    { // 0x001A03E0 - 0x001A03E8
    }
}

// 0x001A03F0 - 0x001A0448 (0x58) zPortalInit__FPvPv
void zPortalInit(void *portal, void *passet) {
    { // 0x001A03F0 - 0x001A0448
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xMovePoint.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
// 0x001A0450 - 0x001A0458 (0x8) xMovePointGetPos__FPC11_xMovePoint
class _xVec3 *xMovePointGetPos(class _xMovePoint *m) {
    { // 0x001A0450 - 0x001A0458
    }
}

// 0x001A0460 - 0x001A05E4 (0x184) xMovePointGetNext__FPC11_xMovePointPC11_xMovePointPP11_xMovePointP6_xVec3
float xMovePointGetNext(class _xMovePoint *m, class _xMovePoint *prev, class _xMovePoint **next, class _xVec3 *hdng) {
    { // 0x001A0460 - 0x001A05E4
        class _xMovePoint *previousOption; // r16
        unsigned short idx; // r5
        signed int rnd; // r4
    }
}

// 0x001A05F0 - 0x001A0608 (0x18) xMovePointSplineDestroy__FP11_xMovePoint
void xMovePointSplineDestroy(class _xMovePoint *m) {
    { // 0x001A05F0 - 0x001A0608
    }
}

// 0x001A0610 - 0x001A07F4 (0x1E4) xMovePointSplineSetup__FP11_xMovePoint
void xMovePointSplineSetup(class _xMovePoint *m) {
    { // 0x001A0610 - 0x001A07F4
        class _xVec3 p2; // r29+0x50
        class _xVec3 p1; // r29+0x40
        class _xVec3 points[2]; // r29+0x20
        class _xMovePoint *w3; // r2
        class _xMovePoint *w2; // r6
        class _xMovePoint *w0; // r5
    }
}

// 0x001A0800 - 0x001A08D0 (0xD0) xMovePointSetup__FP11_xMovePointP7_xScene
void xMovePointSetup(class _xMovePoint *m, class _xScene *sc) {
    { // 0x001A0800 - 0x001A08D0
        unsigned short idx; // r16
    }
}

// 0x001A08D0 - 0x001A0910 (0x40) xMovePointReset__FP11_xMovePoint
void xMovePointReset(class _xMovePoint *m) {
    { // 0x001A08D0 - 0x001A0910
    }
}

// 0x001A0910 - 0x001A0918 (0x8) xMovePointLoad__FP11_xMovePointP7xSerial
void xMovePointLoad(class _xMovePoint *ent, class xSerial *s) {
    { // 0x001A0910 - 0x001A0918
    }
}

// 0x001A0920 - 0x001A0928 (0x8) xMovePointSave__FP11_xMovePointP7xSerial
void xMovePointSave(class _xMovePoint *ent, class xSerial *s) {
    { // 0x001A0920 - 0x001A0928
    }
}

// 0x001A0930 - 0x001A09B4 (0x84) xMovePointInit__FP11_xMovePointP16p2MovePointAsset
void xMovePointInit(class _xMovePoint *m, class p2MovePointAsset *asset) {
    { // 0x001A0930 - 0x001A09B4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zGroup.cpp
// ==========================================================================
signed int (zGroupEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A0A10
// 0x001A09C0 - 0x001A09D8 (0x18) zGroupGetItem__FP7_zGroupUi
unsigned int zGroupGetItem(class _zGroup *g, unsigned int index) {
    { // 0x001A09C0 - 0x001A09D8
    }
}

// 0x001A09E0 - 0x001A09F8 (0x18) zGroupGetItemPtr__FP7_zGroupUi
class xBase *zGroupGetItemPtr(class _zGroup *g, unsigned int index) {
    { // 0x001A09E0 - 0x001A09F8
    }
}

// 0x001A0A00 - 0x001A0A0C (0xC) zGroupGetCount__FP7_zGroup
unsigned int zGroupGetCount(class _zGroup *g) {
    { // 0x001A0A00 - 0x001A0A0C
    }
}

// 0x001A0A10 - 0x001A0B48 (0x138) zGroupEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zGroupEventCB(class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001A0A10 - 0x001A0B48
        class xBase *b; // r2
        signed int i; // r18
        signed int rand; // r17
        class _zGroup *g; // r16
    }
}

// 0x001A0B50 - 0x001A0B58 (0x8) zGroupLoad__FP7_zGroupP7xSerial
void zGroupLoad(class _zGroup *ent, class xSerial *s) {
    { // 0x001A0B50 - 0x001A0B58
    }
}

// 0x001A0B60 - 0x001A0B68 (0x8) zGroupSave__FP7_zGroupP7xSerial
void zGroupSave(class _zGroup *ent, class xSerial *s) {
    { // 0x001A0B60 - 0x001A0B68
    }
}

// 0x001A0B70 - 0x001A0BDC (0x6C) zGroupInit__FPvPv
void zGroupInit(void *b, void *asset) {
    { // 0x001A0B70 - 0x001A0BDC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zPendulum.cpp
// ==========================================================================
void (zPendulum_Update)(class _zPendulum *, class _xScene *, float); // 0x001A0D80
void (zPendulum_Move)(class _zPendulum *, class _xScene *, float, class _xEntFrame *); // 0x001A0D70
signed int (zPendulumEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A0BE0
void (zPendulumTranslate)(class _xEnt *, class _xVec3 *); // 0x001A0D30
class zGlobals globals; // 0x00362FD0
// 0x001A0BE0 - 0x001A0D2C (0x14C) zPendulumEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zPendulumEventCB(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x001A0BE0 - 0x001A0D2C
        class _zPendulum *pend; // r2
    }
}

// 0x001A0D30 - 0x001A0D6C (0x3C) zPendulumTranslate__FP5_xEntP6_xVec3
static void zPendulumTranslate(class _xEnt *xent, class _xVec3 *dpos) {
    { // 0x001A0D30 - 0x001A0D6C
        class _zPendulum *pend; // r16
    }
}

// 0x001A0D70 - 0x001A0D78 (0x8) zPendulum_Move__FP10_zPendulumP7_xScenefP10_xEntFrame
void zPendulum_Move(class _zPendulum *pend, class _xScene *s, float dt, class _xEntFrame *frame) {
    { // 0x001A0D70 - 0x001A0D78
    }
}

// 0x001A0D80 - 0x001A0E30 (0xB0) zPendulum_Update__FP10_zPendulumP7_xScenef
void zPendulum_Update(class _zPendulum *pend, class _xScene *sc, float dt) {
    { // 0x001A0D80 - 0x001A0E30
        float t; // 
        class p2EntMotionPenData *aspen; // r3
    }
}

// 0x001A0E30 - 0x001A0F70 (0x140) zPendulum_Reset__FP10_zPendulumP7_xScene
void zPendulum_Reset(class _zPendulum *pend, class _xScene *sc) {
    { // 0x001A0E30 - 0x001A0F70
        class p2EntMotionPenData *aspen; // r5
    }
}

// 0x001A0F70 - 0x001A0FAC (0x3C) zPendulum_Setup__FP10_zPendulumP7_xScene
void zPendulum_Setup(class _zPendulum *pend, class _xScene *sc) {
    { // 0x001A0F70 - 0x001A0FAC
    }
}

// 0x001A0FB0 - 0x001A0FB8 (0x8) zPendulum_Load__FP10_zPendulumP7xSerial
void zPendulum_Load(class _zPendulum *pend, class xSerial *s) {
    { // 0x001A0FB0 - 0x001A0FB8
    }
}

// 0x001A0FC0 - 0x001A0FC8 (0x8) zPendulum_Save__FP10_zPendulumP7xSerial
void zPendulum_Save(class _zPendulum *pend, class xSerial *s) {
    { // 0x001A0FC0 - 0x001A0FC8
    }
}

// 0x001A0FD0 - 0x001A106C (0x9C) zPendulum_Init__FPvPv
void zPendulum_Init(void *pend, void *asset) {
    { // 0x001A0FD0 - 0x001A106C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zSFX.cpp
// ==========================================================================
signed int (zSFXEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A1070
// 0x001A1070 - 0x001A15C4 (0x554) zSFXEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zSFXEventCB(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x001A1070 - 0x001A15C4
        class _xEnt *ent; // r2
        class _xEnt *ent; // r2
        unsigned int sndFlags; // r17
        class zSFX *t; // r16
    }
}

// 0x001A15D0 - 0x001A15D8 (0x8) zSFXLoad__FP4zSFXP7xSerial
void zSFXLoad(class zSFX *ent, class xSerial *s) {
    { // 0x001A15D0 - 0x001A15D8
    }
}

// 0x001A15E0 - 0x001A15E8 (0x8) zSFXSave__FP4zSFXP7xSerial
void zSFXSave(class zSFX *ent, class xSerial *s) {
    { // 0x001A15E0 - 0x001A15E8
    }
}

// 0x001A15F0 - 0x001A1654 (0x64) zSFXInit__FPvPv
void zSFXInit(void *t, void *asset) {
    { // 0x001A15F0 - 0x001A1654
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iShadow.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
static unsigned int sShadowVertCount; // 0x00358178
static class RxObjSpace3DVertex sShadowVert[384]; // 0x00374920
static class _xRay3 sShadowRay; // 0x003748F0
static class RxRenderStateVector rsv; // 0x003748C0
static class RwRaster *sShadowRaster; // 0x00358174
// 0x001A1660 - 0x001A176C (0x10C) iShadowRender__Fv
void iShadowRender() {
    { // 0x001A1660 - 0x001A176C
    }
}

// 0x001A1770 - 0x001A1910 (0x1A0) iShadowAdd__FP12iShadowCacheUc
void iShadowAdd(class iShadowCache *cache, unsigned char alpha) {
    { // 0x001A1770 - 0x001A1910
    }
}

// 0x001A1910 - 0x001A1D30 (0x420) iShadowCollide__FP12iShadowCacheP5_xEntff
void iShadowCollide(class iShadowCache *cache, class _xEnt *ent, float radius, float ecc) {
    { // 0x001A1910 - 0x001A1D30
        float centerz; // 
        float centery; // 
        float centerx; // 
        float dydz; // 
        float dydx; // 
        float mag; // 
        class _xVec3 tmppos[4]; // r29+0xb0
        class _xCollis crec; // r29+0x60
        float height; // 
    }
}

// 0x001A1D30 - 0x001A1E0C (0xDC) iShadowInit__Fv
void iShadowInit() {
    { // 0x001A1D30 - 0x001A1E0C
        unsigned int i; // r5
        class RwTexture *tex; // r2
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zCounter.cpp
// ==========================================================================
signed int (zCounterEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A1E10
class xBase * (zCounterDebugModeIteratorCB)(class xBase *, class zScene *, void *); // 0x00000000
// 0x001A1E10 - 0x001A1EDC (0xCC) zCounterEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zCounterEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001A1E10 - 0x001A1EDC
        class _zCounter *t; // r2
    }
}

// 0x001A1EE0 - 0x001A1F28 (0x48) zCounterLoad__FP9_zCounterP7xSerial
void zCounterLoad(class _zCounter *ent, class xSerial *s) {
    { // 0x001A1EE0 - 0x001A1F28
    }
}

// 0x001A1F30 - 0x001A1F78 (0x48) zCounterSave__FP9_zCounterP7xSerial
void zCounterSave(class _zCounter *ent, class xSerial *s) {
    { // 0x001A1F30 - 0x001A1F78
    }
}

// 0x001A1F80 - 0x001A1FD8 (0x58) zCounterReset__FP5xBase
void zCounterReset(class xBase *b) {
    { // 0x001A1F80 - 0x001A1FD8
        class _zCounter *t; // r16
    }
}

// 0x001A1FE0 - 0x001A204C (0x6C) zCounterInit__FPvPv
void zCounterInit(void *b, void *asset) {
    { // 0x001A1FE0 - 0x001A204C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntHangable.cpp
// ==========================================================================
void (zEntHangable_Update)(class _zEntHangable *, class _xScene *, float); // 0x001A2DF0
signed int (zEntHangableEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A2A80
class zGlobals globals; // 0x00362FD0
static class _zParEmitter *sMountEmitter; // 0x00358188
static unsigned int sChandelierHash; // 0x00358184
static class _zParEmitter *sCandleSmokeEmitter; // 0x00358180
static class _zParEmitter *sCandleEmitter; // 0x0035817C
// 0x001A2050 - 0x001A21B0 (0x160) zEntHangable_FollowUpdate__FP13_zEntHangable
void zEntHangable_FollowUpdate(class _zEntHangable *ent) {
    { // 0x001A2050 - 0x001A21B0
        class _xVec3 *center; // r2
    }
}

// 0x001A21B0 - 0x001A2338 (0x188) zEntHangable_SetShaggy__FP13_zEntHangableP5_zEnt
void zEntHangable_SetShaggy(class _zEntHangable *ent, class _zEnt *shaggy) {
    { // 0x001A21B0 - 0x001A2338
    }
}

// 0x001A2340 - 0x001A238C (0x4C) zEntHangable_Reset__FP13_zEntHangable
void zEntHangable_Reset(class _zEntHangable *ent) {
    { // 0x001A2340 - 0x001A238C
    }
}

// 0x001A2390 - 0x001A2398 (0x8) zEntHangable_Load__FP13_zEntHangableP7xSerial
void zEntHangable_Load(class _zEntHangable *ent, class xSerial *s) {
    { // 0x001A2390 - 0x001A2398
    }
}

// 0x001A23A0 - 0x001A23A8 (0x8) zEntHangable_Save__FP13_zEntHangableP7xSerial
void zEntHangable_Save(class _zEntHangable *ent, class xSerial *s) {
    { // 0x001A23A0 - 0x001A23A8
    }
}

// 0x001A23B0 - 0x001A2A78 (0x6C8) zEntHangable_SetMatrix__FP13_zEntHangablef
void zEntHangable_SetMatrix(class _zEntHangable *ent, float dt) {
    { // 0x001A23B0 - 0x001A2A78
        class _xVec3 rot; // r29+0x120
        class _xVec3 *orot; // r19
        class _xVec3 *pos; // r18
        class _xVec3 *opos; // r17
        signed int moving; // r19
        float height; // 
        class _xMat3x3 spinMat; // r29+0xf0
        class _xMat4x3 tmpMat; // r29+0xb0
    }
}

// 0x001A2A80 - 0x001A2DE4 (0x364) zEntHangableEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntHangableEventCB(class xBase *from, class xBase *to, unsigned int toEvent, class xBase *toParamWidget) {
    { // 0x001A2A80 - 0x001A2DE4
        class _zEnt *follow; // r2
        class _zEnt *shagent; // r2
        unsigned int oldvisible; // r17
        class _zEntHangable *ent; // r16
    }
}

// 0x001A2DF0 - 0x001A3198 (0x3A8) zEntHangable_Update__FP13_zEntHangableP7_xScenef
void zEntHangable_Update(class _zEntHangable *ent, float dt) {
    { // 0x001A2DF0 - 0x001A3198
        class _xVec3 unitHang; // r29+0x50
    }
}

// 0x001A31A0 - 0x001A32F4 (0x154) zEntHangable_UpdateFX__FP13_zEntHangable
static void zEntHangable_UpdateFX(class _zEntHangable *ent) {
    { // 0x001A31A0 - 0x001A32F4
        class _xVec3 offset; // r29+0x120
        signed int i; // r17
        class _zParEmitter *emitter; // r16
        class _zParEmitterCustomSettings info; // r29+0xc0
        class _xVec3 *local_offset; // r2
        class _xVec3 offset_rlii0006[8]; // r29+0x60
    }
}

// 0x001A3300 - 0x001A3364 (0x64) zEntHangable_Init__FPvPv
void zEntHangable_Init(void *ent, void *asset) {
    { // 0x001A3300 - 0x001A3364
    }
}

// 0x001A3370 - 0x001A346C (0xFC) HangableSetup__FP13_zEntHangableP10p2EntAsset
static void HangableSetup(class _zEntHangable *ent, class p2EntAsset *asset) {
    { // 0x001A3370 - 0x001A346C
        class _xMat3x3 hackMat; // r29+0x20
        class _xVec3 *center; // r2
        class p2EntHangableAsset *hangAsset; // r3
    }
}

// 0x001A3470 - 0x001A34C4 (0x54) zEntHangable_SetupFX__Fv
void zEntHangable_SetupFX() {
    { // 0x001A3470 - 0x001A34C4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntSimpleObj.cpp
// ==========================================================================
signed int (zEntSimpleObjEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A34D0
// 0x001A34D0 - 0x001A3694 (0x1C4) zEntSimpleObjEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntSimpleObjEventCB(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x001A34D0 - 0x001A3694
        class _zEntSimpleObj *s; // r16
    }
}

// 0x001A36A0 - 0x001A36A8 (0x8) zEntSimpleObj_Load__FP14_zEntSimpleObjP7xSerial
void zEntSimpleObj_Load(class _zEntSimpleObj *ent, class xSerial *s) {
    { // 0x001A36A0 - 0x001A36A8
    }
}

// 0x001A36B0 - 0x001A36B8 (0x8) zEntSimpleObj_Save__FP14_zEntSimpleObjP7xSerial
void zEntSimpleObj_Save(class _zEntSimpleObj *ent, class xSerial *s) {
    { // 0x001A36B0 - 0x001A36B8
    }
}

// 0x001A36C0 - 0x001A36EC (0x2C) zEntSimpleObj_Setup__FP14_zEntSimpleObj
void zEntSimpleObj_Setup(class _zEntSimpleObj *ent) {
    { // 0x001A36C0 - 0x001A36EC
    }
}

// 0x001A36F0 - 0x001A37C4 (0xD4) DiggableSetup__FP14_zEntSimpleObj
static void DiggableSetup(class _zEntSimpleObj *ent) {
    { // 0x001A36F0 - 0x001A37C4
        class xBase *tgt; // r2
        class p2LinkAsset *curlink; // r17
        unsigned int dug; // r5
        unsigned int i; // r16
    }
}

// 0x001A37D0 - 0x001A3888 (0xB8) zEntSimpleObj_Init__FP14_zEntSimpleObjP10p2EntAsset
void zEntSimpleObj_Init(class _zEntSimpleObj *ent, class p2EntAsset *asset) {
    { // 0x001A37D0 - 0x001A3888
    }
}

// 0x001A3890 - 0x001A3898 (0x8) zEntSimpleObj_Init__FPvPv
void zEntSimpleObj_Init(void *ent, void *asset) {
    { // 0x001A3890 - 0x001A3898
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntButton.cpp
// ==========================================================================
void (zEntButton_Update)(class _zEntButton *, class _xScene *, float); // 0x001A3B10
void (zEntButton_Move)(class _zEntButton *, class _xScene *, float, class _xEntFrame *); // 0x001A3D40
signed int (zEntButtonEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A38A0
class zGlobals globals; // 0x00362FD0
// 0x001A38A0 - 0x001A3A8C (0x1EC) zEntButtonEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntButtonEventCB(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x001A38A0 - 0x001A3A8C
        class _zEntButton *s; // r16
    }
}

// 0x001A3A90 - 0x001A3A98 (0x8) zEntButton_CheckState__FP11_zEntButton
unsigned int zEntButton_CheckState(class _zEntButton *ent) {
    { // 0x001A3A90 - 0x001A3A98
    }
}

// 0x001A3AA0 - 0x001A3ACC (0x2C) zEntButton_ButtBounce__FP11_zEntButton
unsigned int zEntButton_ButtBounce(class _zEntButton *ent) {
    { // 0x001A3AA0 - 0x001A3ACC
    }
}

// 0x001A3AD0 - 0x001A3AE4 (0x14) zEntButton_Stand__FP11_zEntButton
unsigned int zEntButton_Stand(class _zEntButton *ent) {
    { // 0x001A3AD0 - 0x001A3AE4
    }
}

// 0x001A3AF0 - 0x001A3B10 (0x20) zEntButton_Headbutt__FP11_zEntButton
unsigned int zEntButton_Headbutt(class _zEntButton *ent) {
    { // 0x001A3AF0 - 0x001A3B10
    }
}

// 0x001A3B10 - 0x001A3C6C (0x15C) zEntButton_Update__FP11_zEntButtonP7_xScenef
void zEntButton_Update(class _zEntButton *ent, class _xScene *sc, float dt) {
    { // 0x001A3B10 - 0x001A3C6C
    }
}

// 0x001A3C70 - 0x001A3D10 (0xA0) zEntButton_Reset__FP11_zEntButtonP7_xScene
void zEntButton_Reset(class _zEntButton *ent, class _xScene *sc) {
    { // 0x001A3C70 - 0x001A3D10
    }
}

// 0x001A3D10 - 0x001A3D18 (0x8) zEntButton_Load__FP11_zEntButtonP7xSerial
void zEntButton_Load(class _zEntButton *ent, class xSerial *s) {
    { // 0x001A3D10 - 0x001A3D18
    }
}

// 0x001A3D20 - 0x001A3D28 (0x8) zEntButton_Save__FP11_zEntButtonP7xSerial
void zEntButton_Save(class _zEntButton *ent, class xSerial *s) {
    { // 0x001A3D20 - 0x001A3D28
    }
}

// 0x001A3D30 - 0x001A3D38 (0x8) zEntButton_Setup__FP11_zEntButtonP7_xScene
void zEntButton_Setup(class _zEntButton *ent, class _xScene *sc) {
    { // 0x001A3D30 - 0x001A3D38
    }
}

// 0x001A3D40 - 0x001A3D48 (0x8) zEntButton_Move__FP11_zEntButtonP7_xScenefP10_xEntFrame
void zEntButton_Move(class _zEntButton *ent, class _xScene *s, float dt, class _xEntFrame *frame) {
    { // 0x001A3D40 - 0x001A3D48
    }
}

// 0x001A3D50 - 0x001A3E40 (0xF0) zEntButton_Init__FP11_zEntButtonP10p2EntAsset
void zEntButton_Init(class _zEntButton *ent, class p2EntAsset *asset) {
    { // 0x001A3D50 - 0x001A3E40
        class p2EntMotionAsset *emasset; // r16
        class zEntButtonAsset *passet; // r2
    }
}

// 0x001A3E40 - 0x001A3E48 (0x8) zEntButton_Init__FPvPv
void zEntButton_Init(void *ent, void *asset) {
    { // 0x001A3E40 - 0x001A3E48
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xSurface.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
static unsigned short nsurfs; // 0x00358190
static class xSurface *surfs; // 0x0035818C
// 0x001A3E50 - 0x001A3E78 (0x28) xSurfaceGetByIdx__FUs
class xSurface *xSurfaceGetByIdx(unsigned short n) {
    { // 0x001A3E50 - 0x001A3E78
    }
}

// 0x001A3E80 - 0x001A3E88 (0x8) xSurfaceGetNumSurfaces__Fv
unsigned short xSurfaceGetNumSurfaces() {
    { // 0x001A3E80 - 0x001A3E88
    }
}

// 0x001A3E90 - 0x001A3E98 (0x8) xSurfaceReset__FP8xSurface
void xSurfaceReset() {
    { // 0x001A3E90 - 0x001A3E98
    }
}

// 0x001A3EA0 - 0x001A3EA8 (0x8) xSurfaceLoad__FP8xSurfaceP7xSerial
void xSurfaceLoad(class xSurface *ent, class xSerial *s) {
    { // 0x001A3EA0 - 0x001A3EA8
    }
}

// 0x001A3EB0 - 0x001A3EB8 (0x8) xSurfaceSave__FP8xSurfaceP7xSerial
void xSurfaceSave(class xSurface *ent, class xSerial *s) {
    { // 0x001A3EB0 - 0x001A3EB8
    }
}

// 0x001A3EC0 - 0x001A3EC8 (0x8) xSurfaceExit__Fv
void xSurfaceExit() {
    { // 0x001A3EC0 - 0x001A3EC8
    }
}

// 0x001A3ED0 - 0x001A3F60 (0x90) xSurfaceInit__FUs
void xSurfaceInit(unsigned short num_surfs) {
    { // 0x001A3ED0 - 0x001A3F60
        unsigned short i; // r6
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zSurface.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (zSurfaceEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A41C0
unsigned int gFrameCount; // 0x00357C78
static class zSurfAsset sDef_surf_asset; // 0x00378010
static class zSurfaceProps sDef_surf_props; // 0x00377F50
static class xSurface sDef_surf; // 0x00377F20
static class zMaterialMapAsset *sMapper[1]; // 0x003575D0
static signed int sMapperCount; // 0x00358198
static class zSurfaceProps *zsps; // 0x00358194
// 0x001A3F60 - 0x001A41BC (0x25C) zSurfaceGetName__FiPc
void zSurfaceGetName(signed int type, char *buffer) {
    { // 0x001A3F60 - 0x001A41BC
    }
}

// 0x001A41C0 - 0x001A44F4 (0x334) zSurfaceEventCB__FP5xBaseP5xBaseUiPfP5xBase
static signed int zSurfaceEventCB(class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001A41C0 - 0x001A44F4
        class zSurfaceProps *p; // r2
        class zSurfaceProps *p; // r2
        class zSurfaceProps *p; // r2
        class zSurfaceProps *p; // r2
        class zSurfaceProps *p; // r2
        class zSurfaceProps *p; // r2
        class xSurface *t; // r16
    }
}

// 0x001A4500 - 0x001A4C10 (0x710) zSurfaceUpdate__FP5xBaseP7_xScenef
void zSurfaceUpdate(class xBase *to, float dt) {
    { // 0x001A4500 - 0x001A4C10
        signed int max; // r16
        class _zGroup *g; // r2
        class zSurfaceProps *moprops; // r17
        signed int j; // r16
    }
}

// 0x001A4C10 - 0x001A4C6C (0x5C) zSurfaceSetup__FP8xSurface
void zSurfaceSetup(class xSurface *s) {
    { // 0x001A4C10 - 0x001A4C6C
        signed int i; // r16
        class zSurfaceProps *pp; // r2
    }
}

// 0x001A4C70 - 0x001A4C78 (0x8) zSurfaceLoad__FP8xSurfaceP7xSerial
void zSurfaceLoad(class xSurface *ent, class xSerial *s) {
    { // 0x001A4C70 - 0x001A4C78
    }
}

// 0x001A4C80 - 0x001A4C88 (0x8) zSurfaceSave__FP8xSurfaceP7xSerial
void zSurfaceSave(class xSurface *ent, class xSerial *s) {
    { // 0x001A4C80 - 0x001A4C88
    }
}

// 0x001A4C90 - 0x001A4C98 (0x8) zSurfaceGetDamping__FPC8xSurfacef
float zSurfaceGetDamping(class xSurface *surf, float min_vel) {
    { // 0x001A4C90 - 0x001A4C98
    }
}

// 0x001A4CA0 - 0x001A4CC8 (0x28) zSurfaceGetSlickness__FPC8xSurface
signed int zSurfaceGetSlickness(class xSurface *surf) {
    { // 0x001A4CA0 - 0x001A4CC8
    }
}

// 0x001A4CD0 - 0x001A4CD8 (0x8) zSurfaceGetFriction__FPC8xSurface
float zSurfaceGetFriction(class xSurface *surf) {
    { // 0x001A4CD0 - 0x001A4CD8
    }
}

// 0x001A4CE0 - 0x001A4D0C (0x2C) zSurfaceGetStandOn__FPC8xSurface
unsigned int zSurfaceGetStandOn(class xSurface *surf) {
    { // 0x001A4CE0 - 0x001A4D0C
    }
}

// 0x001A4D10 - 0x001A4D30 (0x20) zSurfaceGetSticky__FPC8xSurface
unsigned int zSurfaceGetSticky(class xSurface *surf) {
    { // 0x001A4D10 - 0x001A4D30
    }
}

// 0x001A4D30 - 0x001A4D50 (0x20) zSurfaceGetDamagePassthrough__FPC8xSurface
unsigned int zSurfaceGetDamagePassthrough(class xSurface *surf) {
    { // 0x001A4D30 - 0x001A4D50
    }
}

// 0x001A4D50 - 0x001A4D70 (0x20) zSurfaceGetDamageType__FPC8xSurface
signed int zSurfaceGetDamageType(class xSurface *surf) {
    { // 0x001A4D50 - 0x001A4D70
    }
}

// 0x001A4D70 - 0x001A4D90 (0x20) zSurfaceGetMatchOrient__FPC8xSurface
unsigned int zSurfaceGetMatchOrient(class xSurface *surf) {
    { // 0x001A4D70 - 0x001A4D90
    }
}

// 0x001A4D90 - 0x001A4E14 (0x84) zSurfaceGetSlideStopAngle__FPC8xSurface
float zSurfaceGetSlideStopAngle(class xSurface *surf) {
    { // 0x001A4D90 - 0x001A4E14
    }
}

// 0x001A4E20 - 0x001A4EA4 (0x84) zSurfaceGetSlideStartAngle__FPC8xSurface
float zSurfaceGetSlideStartAngle(class xSurface *surf) {
    { // 0x001A4E20 - 0x001A4EA4
    }
}

// 0x001A4EB0 - 0x001A4ED0 (0x20) zSurfaceGetStep__FPC8xSurface
unsigned int zSurfaceGetStep(class xSurface *surf) {
    { // 0x001A4EB0 - 0x001A4ED0
    }
}

// 0x001A4ED0 - 0x001A4EF0 (0x20) zSurfaceGetSlide__FPC8xSurface
unsigned int zSurfaceGetSlide(class xSurface *surf) {
    { // 0x001A4ED0 - 0x001A4EF0
    }
}

// 0x001A4EF0 - 0x001A4F44 (0x54) zSurfaceGetSurface__FPC8_xCollis
class xSurface *zSurfaceGetSurface(class _xCollis *coll) {
    { // 0x001A4EF0 - 0x001A4F44
        class xSurface *surf; // r2
    }
}

// 0x001A4F50 - 0x001A5074 (0x124) zSurfaceGetSurface__FUi
class xSurface *zSurfaceGetSurface(unsigned int mat_id) {
    { // 0x001A4F50 - 0x001A5074
        class xSurface *surf; // r2
        unsigned short j; // r20
        unsigned short nsurfs; // r2
        class zMaterialMapEntry *entry; // r2
        unsigned short i; // r19
        class zMaterialMapAsset *mapper; // r18
        signed int map; // r23
    }
}

// 0x001A5080 - 0x001A50A0 (0x20) zSurfaceExit__Fv
void zSurfaceExit() {
    { // 0x001A5080 - 0x001A50A0
    }
}

// 0x001A50A0 - 0x001A5108 (0x68) zSurfaceRegisterMapper__FUi
void zSurfaceRegisterMapper(unsigned int assetId) {
    { // 0x001A50A0 - 0x001A5108
    }
}

// 0x001A5110 - 0x001A53B4 (0x2A4) zSurfaceInit__Fv
void zSurfaceInit() {
    { // 0x001A5110 - 0x001A53B4
        signed int j; // r6
        class zSurfaceProps *moprops; // r2
        class xSurface *surf; // r17
        class zSurfAsset *asset; // r2
        unsigned short i; // r16
        unsigned short nsurfs; // r16
        unsigned int size; // r29+0x8c
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iLight.cpp
// ==========================================================================
class _xVec3 g_O3; // 0x00342E28
class RpWorld *gLightWorld; // 0x0035819C
// 0x001A53C0 - 0x001A541C (0x5C) iLightEnv__FP6iLighti
void iLightEnv(class iLight *light, signed int env) {
    { // 0x001A53C0 - 0x001A541C
        unsigned int flags; // r7
    }
}

// 0x001A5420 - 0x001A542C (0xC) iLightRemove__FP6iLight
void iLightRemove(class iLight *light) {
    { // 0x001A5420 - 0x001A542C
    }
}

// 0x001A5430 - 0x001A543C (0xC) iLightAdd__FP6iLight
void iLightAdd(class iLight *light) {
    { // 0x001A5430 - 0x001A543C
    }
}

// 0x001A5440 - 0x001A5488 (0x48) iLightDestroy__FP6iLight
void iLightDestroy(class iLight *light) {
    { // 0x001A5440 - 0x001A5488
        class RwFrame *frame; // r2
    }
}

// 0x001A5490 - 0x001A54DC (0x4C) iLightSetPos__FP6iLightP6_xVec3
void iLightSetPos(class iLight *light, class _xVec3 *pos) {
    { // 0x001A5490 - 0x001A54DC
        class RwMatrixTag *m; // r2
        class RwFrame *f; // r16
    }
}

// 0x001A54E0 - 0x001A54E8 (0x8) iLightSetColor__FP6iLightP8_xFColor
void iLightSetColor(class iLight *light, class _xFColor *col) {
    { // 0x001A54E0 - 0x001A54E8
    }
}

// 0x001A54F0 - 0x001A560C (0x11C) iLightModify__FP6iLightUi
void iLightModify(class iLight *light, unsigned int flags) {
    { // 0x001A54F0 - 0x001A560C
        class RwMatrixTag temp; // r29+0x30
        class RwFrame *frame; // r2
    }
}

// 0x001A5610 - 0x001A5738 (0x128) iLightCreate__FP6iLightUi
class iLight *iLightCreate(class iLight *light, unsigned int type) {
    { // 0x001A5610 - 0x001A5738
        class RwFrame *frame; // r18
    }
}

// 0x001A5740 - 0x001A5748 (0x8) iLightInit__FP7RpWorld
void iLightInit(class RpWorld *world) {
    { // 0x001A5740 - 0x001A5748
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntDestructObj.cpp
// ==========================================================================
void (zEntDextructObj_Move)(class _zEntDestructObj *, class _xScene *, float, class _xEntFrame *); // 0x001A5C60
void (zEntDestructObj_Update)(class _zEntDestructObj *, class _xScene *, float); // 0x001A5BB0
signed int (zEntDestructObjEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A5750
class zGlobals globals; // 0x00362FD0
class _zParEmitter *sEmitWeb; // 0x003581A8
class _zParEmitter *sEmitXplo; // 0x003581A4
class _zParEmitter *sEmitDust; // 0x003581A0
// 0x001A5750 - 0x001A59C4 (0x274) zEntDestructObjEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEntDestructObjEventCB(class xBase *to, unsigned int toEvent, float *toParam) {
    { // 0x001A5750 - 0x001A59C4
        class _zEntDestructObj *s; // r16
    }
}

// 0x001A59D0 - 0x001A5AD0 (0x100) zEntDestructObj_DestroyFX__FP16_zEntDestructObj
void zEntDestructObj_DestroyFX(class _zEntDestructObj *o) {
    { // 0x001A59D0 - 0x001A5AD0
        enum _tagSDRumbleType rt; // r2
    }
}

// 0x001A5AD0 - 0x001A5AD8 (0x8) zEntDestructObj_CheckState__FP16_zEntDestructObj
unsigned int zEntDestructObj_CheckState(class _zEntDestructObj *ent) {
    { // 0x001A5AD0 - 0x001A5AD8
    }
}

// 0x001A5AE0 - 0x001A5B1C (0x3C) zEntDestructObj_Load__FP16_zEntDestructObjP7xSerial
void zEntDestructObj_Load(class _zEntDestructObj *ent, class xSerial *s) {
    { // 0x001A5AE0 - 0x001A5B1C
    }
}

// 0x001A5B20 - 0x001A5B5C (0x3C) zEntDestructObj_Save__FP16_zEntDestructObjP7xSerial
void zEntDestructObj_Save(class _zEntDestructObj *ent, class xSerial *s) {
    { // 0x001A5B20 - 0x001A5B5C
    }
}

// 0x001A5B60 - 0x001A5B8C (0x2C) zEntDestructObj_Headbutt__FP16_zEntDestructObj
unsigned int zEntDestructObj_Headbutt(class _zEntDestructObj *ent) {
    { // 0x001A5B60 - 0x001A5B8C
    }
}

// 0x001A5B90 - 0x001A5BA8 (0x18) zEntDestructObj_ButtBounce__FP16_zEntDestructObj
unsigned int zEntDestructObj_ButtBounce(class _zEntDestructObj *ent) {
    { // 0x001A5B90 - 0x001A5BA8
    }
}

// 0x001A5BB0 - 0x001A5C60 (0xB0) zEntDestructObj_Update__FP16_zEntDestructObjP7_xScenef
void zEntDestructObj_Update(class _zEntDestructObj *ent, class _xScene *sc, float dt) {
    { // 0x001A5BB0 - 0x001A5C60
        class _zParEmitterCustomSettings info; // r29+0x30
    }
}

// 0x001A5C60 - 0x001A5C68 (0x8) zEntDextructObj_Move__FP16_zEntDestructObjP7_xScenefP10_xEntFrame
void zEntDextructObj_Move() {
    { // 0x001A5C60 - 0x001A5C68
    }
}

// 0x001A5C70 - 0x001A5D48 (0xD8) zEntDestructObj_Init__FP16_zEntDestructObjP10p2EntAsset
void zEntDestructObj_Init(class _zEntDestructObj *ent, class p2EntAsset *asset) {
    { // 0x001A5C70 - 0x001A5D48
        class zEntDestructObjAsset *dasset; // r2
    }
}

// 0x001A5D50 - 0x001A5D58 (0x8) zEntDestructObj_Init__FPvPv
void zEntDestructObj_Init(void *ent, void *asset) {
    { // 0x001A5D50 - 0x001A5D58
    }
}

// 0x001A5D60 - 0x001A5DA4 (0x44) zEntDestructObj_FindFX__Fv
void zEntDestructObj_FindFX() {
    { // 0x001A5D60 - 0x001A5DA4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zGust.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (zGustEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A6170
static class _zParEmitter *sGustDebrisEmitter; // 0x003581B8
static class _zParEmitter *sGustDustEmitter; // 0x003581B4
static unsigned short ngusts; // 0x003581B0
static class _zGust *gusts; // 0x003581AC
// 0x001A5DB0 - 0x001A5E40 (0x90) zGustUpdateFX__Ff
void zGustUpdateFX(float seconds) {
    { // 0x001A5DB0 - 0x001A5E40
        signed int i; // r16
    }
}

// 0x001A5E40 - 0x001A616C (0x32C) UpdateGustFX__FP6_zGustf
static void UpdateGustFX(class _zGust *g, float seconds) {
    { // 0x001A5E40 - 0x001A616C
        signed int i; // r19
        signed int vol_area; // r3
        signed int total_debris; // r18
        class _zParEmitter *e; // r17
        class _zParEmitterCustomSettings info; // r29+0x90
        class _xBBox *box; // r2
    }
}

// 0x001A6170 - 0x001A6224 (0xB4) zGustEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zGustEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001A6170 - 0x001A6224
        class _zGust *g; // r16
    }
}

// 0x001A6230 - 0x001A6238 (0x8) zGustLoad__FP6_zGustP7xSerial
void zGustLoad(class _zGust *ent, class xSerial *s) {
    { // 0x001A6230 - 0x001A6238
    }
}

// 0x001A6240 - 0x001A6248 (0x8) zGustSave__FP6_zGustP7xSerial
void zGustSave(class _zGust *ent, class xSerial *s) {
    { // 0x001A6240 - 0x001A6248
    }
}

// 0x001A6250 - 0x001A6574 (0x324) zGustUpdateEnt__FP5_xEntP7_xScenefPv
void zGustUpdateEnt(class _xEnt *ent, float dt, void *gdata) {
    { // 0x001A6250 - 0x001A6574
        class _xVec3 *gvel; // r20
        float lerpinc; // 
        class _xCollis coll; // r29+0x90
        class zGustData *data; // r16
        unsigned int minidx; // r21
        unsigned int j; // r4
        unsigned int i; // r20
    }
}

// 0x001A6580 - 0x001A65A8 (0x28) zGustGetGust__FUs
class _zGust *zGustGetGust(unsigned short n) {
    { // 0x001A6580 - 0x001A65A8
    }
}

// 0x001A65B0 - 0x001A6680 (0xD0) zGustSetup__Fv
void zGustSetup() {
    { // 0x001A65B0 - 0x001A6680
        unsigned short i; // r16
    }
}

// 0x001A6680 - 0x001A67A4 (0x124) zGustInit__Fv
void zGustInit() {
    { // 0x001A6680 - 0x001A67A4
        class zGustAsset *asset; // r17
        unsigned int size; // r29+0x7c
        unsigned short i; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zVolume.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (zVolumeEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A67B0
class zGlobals globals; // 0x00362FD0
signed int occludeAlwaysFalse; // 0x00000000
class PreCalcOcclude gOccludeCalc[10]; // 0x00378110
signed int gOccludeCalcCount; // 0x003581C8
class _zVolume *gOccludeList[10]; // 0x003780E0
signed int gOccludeCount; // 0x003581C4
static unsigned short nvols; // 0x003581C0
static class _zVolume *vols; // 0x003581BC
// 0x001A67B0 - 0x001A68D0 (0x120) zVolumeEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zVolumeEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001A67B0 - 0x001A68D0
        signed int i; // r3
        class _zVolume *vol; // r2
    }
}

// 0x001A68D0 - 0x001A6A30 (0x160) zVolume_OccludeTest__FP8RpAtomicP11RwMatrixTag
signed int zVolume_OccludeTest(class RpAtomic *model, class RwMatrixTag *mat) {
    { // 0x001A68D0 - 0x001A6A30
        signed int i; // r5
    }
}

// 0x001A6A30 - 0x001A700C (0x5DC) zVolume_OccludePrecalc__FP6_xVec3
void zVolume_OccludePrecalc(class _xVec3 *camPos) {
    { // 0x001A6A30 - 0x001A700C
        float camdot; // r29+0x160
        float depthdot; // r29+0x160
        class _xVec4 locFrustVec[4]; // r29+0x120
        class PreCalcOcclude *calc; // r17
        float s; // r29+0x160
        float c; // 
        class zVolumeAsset *a; // r18
        class _xVec3 corner[5]; // r29+0xe0
        signed int j; // r22
        signed int i; // r29+0xb0
    }
}

// 0x001A7010 - 0x001A702C (0x1C) zVolumeGetVolume__FUs
class _zVolume *zVolumeGetVolume(unsigned short n) {
    { // 0x001A7010 - 0x001A702C
    }
}

// 0x001A7030 - 0x001A703C (0xC) zVolumeGetBound__FP8_zVolume
class _xBound *zVolumeGetBound(class _zVolume *vol) {
    { // 0x001A7030 - 0x001A703C
    }
}

// 0x001A7040 - 0x001A70A8 (0x68) zVolumeSetup__Fv
void zVolumeSetup() {
    { // 0x001A7040 - 0x001A70A8
        unsigned int i; // r5
    }
}

// 0x001A70B0 - 0x001A71C4 (0x114) zVolumeInit__Fv
void zVolumeInit() {
    { // 0x001A70B0 - 0x001A71C4
        class zVolumeAsset *asset; // r17
        unsigned int size; // r29+0x7c
        unsigned short i; // r16
    }
}

// 0x001A71D0 - 0x001A71D8 (0x8) zVolumeLoad__FP8_zVolumeP7xSerial
void zVolumeLoad(class _zVolume *ent, class xSerial *s) {
    { // 0x001A71D0 - 0x001A71D8
    }
}

// 0x001A71E0 - 0x001A71E8 (0x8) zVolumeSave__FP8_zVolumeP7xSerial
void zVolumeSave(class _zVolume *ent, class xSerial *s) {
    { // 0x001A71E0 - 0x001A71E8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xBound.cpp
// ==========================================================================
// 0x001A71F0 - 0x001A72F8 (0x108) xVecHitsBound__FPC6_xVec3PC7_xBoundP8_xCollis
void xVecHitsBound(class _xVec3 *v, class _xBound *b, class _xCollis *c) {
    { // 0x001A71F0 - 0x001A72F8
        class _xVec3 lv; // r29+0x60
        class _xIsect isect; // r29+0x30
    }
}

// 0x001A7300 - 0x001A736C (0x6C) xRayHitsBound__FPC6_xRay3PC7xQCDataPC7_xBoundP8_xCollis
void xRayHitsBound(class _xRay3 *r, class xQCData *q, class _xBound *b, class _xCollis *c) {
    { // 0x001A7300 - 0x001A736C
    }
}

// 0x001A7370 - 0x001A7558 (0x1E8) xRayHitsBound__FPC6_xRay3PC7_xBoundP8_xCollis
void xRayHitsBound(class _xRay3 *r, class _xBound *b, class _xCollis *c) {
    { // 0x001A7370 - 0x001A7558
        float one_ms; // 
        class _xMat3x3 *dmp; // r19
        class _xMat3x3 mn; // r29+0xc0
        class _xRay3 lr; // r29+0x90
        class _xIsect isect; // r29+0x60
    }
}

// 0x001A7560 - 0x001A75AC (0x4C) xBoundHitsModel__FPC7_xBoundPC14xModelInstanceP8_xCollisUcf
void xBoundHitsModel(class _xBound *b, class xModelInstance *m, class _xCollis *colls, unsigned char ncolls, float sth) {
    { // 0x001A7560 - 0x001A75AC
    }
}

// 0x001A75B0 - 0x001A76A8 (0xF8) xBoundHitsBound__FPC7_xBoundPC7_xBoundP8_xCollis
void xBoundHitsBound(class _xBound *a, class _xBound *b, class _xCollis *c) {
    { // 0x001A75B0 - 0x001A76A8
    }
}

// 0x001A76B0 - 0x001A7804 (0x154) xBoundUpdate__FP7_xBound
void xBoundUpdate(class _xBound *b) {
    { // 0x001A76B0 - 0x001A7804
    }
}

// 0x001A7810 - 0x001A7860 (0x50) xBoundCenter__FPC7_xBound
class _xVec3 *xBoundCenter(class _xBound *b) {
    { // 0x001A7810 - 0x001A7860
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xFFX.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
static class xFFXRotMatchState *rot_match_alist; // 0x003581EC
static class xFFXRotMatchState *rot_match_pool; // 0x003581E8
static unsigned int rot_match_psize; // 0x003581E4
static class xFFXShakeState *shake_alist; // 0x003581E0
static class xFFXShakeState *shake_pool; // 0x003581DC
static unsigned int shake_psize; // 0x003581D8
static class xFFX *alist; // 0x003581D4
static class xFFX *pool; // 0x003581D0
static unsigned int psize; // 0x003581CC
// 0x001A7860 - 0x001A7888 (0x28) xFFXRotMatchAlloc__Fv
class xFFXRotMatchState *xFFXRotMatchAlloc() {
    { // 0x001A7860 - 0x001A7888
    }
}

// 0x001A7890 - 0x001A793C (0xAC) xFFXRotMatchPoolInit__FUi
void xFFXRotMatchPoolInit(unsigned int num) {
    { // 0x001A7890 - 0x001A793C
        unsigned int i; // r5
    }
}

// 0x001A7940 - 0x001A7954 (0x14) xFFXShakeFree__FP14xFFXShakeState
void xFFXShakeFree(class xFFXShakeState *s) {
    { // 0x001A7940 - 0x001A7954
    }
}

// 0x001A7960 - 0x001A7988 (0x28) xFFXShakeAlloc__Fv
class xFFXShakeState *xFFXShakeAlloc() {
    { // 0x001A7960 - 0x001A7988
    }
}

// 0x001A7990 - 0x001A7A3C (0xAC) xFFXShakePoolInit__FUi
void xFFXShakePoolInit(unsigned int num) {
    { // 0x001A7990 - 0x001A7A3C
        unsigned int i; // r5
    }
}

// 0x001A7A40 - 0x001A7BA8 (0x168) xFFXShakeUpdateEnt__FP5_xEntP7_xScenefPv
void xFFXShakeUpdateEnt(class _xEnt *ent, float dt, void *fdata) {
    { // 0x001A7A40 - 0x001A7BA8
        float mag; // 
        float tnext; // 
        class xFFXShakeState *ss; // r2
    }
}

// 0x001A7BB0 - 0x001A7CAC (0xFC) xFFXApply__FP5_xEntP7_xScenef
void xFFXApply(class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x001A7BB0 - 0x001A7CAC
    }
}

// 0x001A7CB0 - 0x001A7DE0 (0x130) xFFXApplyOne__FP4xFFXP5_xEntP7_xScenef
static void xFFXApplyOne(class xFFX *ffx, class _xEnt *ent, class _xScene *sc, float dt) {
    { // 0x001A7CB0 - 0x001A7DE0
    }
}

// 0x001A7DE0 - 0x001A7E44 (0x64) xFFXAddEffect__FP5_xEntPFP5_xEntP7_xScenefPv_vPv
signed short xFFXAddEffect(class _xEnt *ent, void (*dof)(class _xEnt *, class _xScene *, float, void *), void *fd) {
    { // 0x001A7DE0 - 0x001A7E44
    }
}

// 0x001A7E50 - 0x001A7E78 (0x28) xFFXAddEffect__FP5_xEntP4xFFX
signed short xFFXAddEffect(class _xEnt *ent, class xFFX *f) {
    { // 0x001A7E50 - 0x001A7E78
    }
}

// 0x001A7E80 - 0x001A7E94 (0x14) xFFXTurnOff__FP4xFFX
void xFFXTurnOff(class xFFX *f) {
    { // 0x001A7E80 - 0x001A7E94
    }
}

// 0x001A7EA0 - 0x001A7EB0 (0x10) xFFXTurnOn__FP4xFFX
void xFFXTurnOn(class xFFX *f) {
    { // 0x001A7EA0 - 0x001A7EB0
    }
}

// 0x001A7EB0 - 0x001A7ED8 (0x28) xFFXAlloc__Fv
class xFFX *xFFXAlloc() {
    { // 0x001A7EB0 - 0x001A7ED8
    }
}

// 0x001A7EE0 - 0x001A7F70 (0x90) xFFXPoolInit__FUi
void xFFXPoolInit(unsigned int num_ffx) {
    { // 0x001A7EE0 - 0x001A7F70
        unsigned int i; // r5
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xLinkList.cpp
// ==========================================================================
// 0x001A7F70 - 0x001A7FD0 (0x60) MoveList__FP8st_ELISTP8st_ELIST
void MoveList(class st_ELIST *to, class st_ELIST *from) {
    { // 0x001A7F70 - 0x001A7FD0
    }
}

// 0x001A7FD0 - 0x001A8004 (0x34) RemHead__FP8st_ELIST
class st_ENODE *RemHead(class st_ELIST *list) {
    { // 0x001A7FD0 - 0x001A8004
    }
}

// 0x001A8010 - 0x001A803C (0x2C) AddTail__FP8st_ELISTP8st_ENODE
void AddTail(class st_ELIST *list, class st_ENODE *node) {
    { // 0x001A8010 - 0x001A803C
    }
}

// 0x001A8040 - 0x001A805C (0x1C) AddHead__FP8st_ELISTP8st_ENODE
void AddHead(class st_ELIST *list, class st_ENODE *node) {
    { // 0x001A8040 - 0x001A805C
    }
}

// 0x001A8060 - 0x001A8074 (0x14) NewList__FP8st_ELIST
void NewList(class st_ELIST *list) {
    { // 0x001A8060 - 0x001A8074
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zDispatcher.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (ZDSP_elcb_event)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001A8080
class zGlobals globals; // 0x00362FD0
class xSndGlobals gSnd; // 0x00373B80
static class st_ZDISPATCH_DEPOT g_zdsp_depot; // 0x003575D8
static signed int g_zdsp_dblog_init; // 0x00000000
static signed int g_zdsp_init; // 0x003581F0
// 0x001A8080 - 0x001A883C (0x7BC) ZDSP_elcb_event__FP5xBaseP5xBaseUiPfP5xBase
static signed int ZDSP_elcb_event(class xBase *xb, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001A8080 - 0x001A883C
        class iColor_tag clear; // r29+0x3c
        class iColor_tag black; // r29+0x38
        class st_ZDISPATCH_DATA *dspdata; // r16
    }
}

// 0x001A8840 - 0x001A9004 (0x7C4) ZDSP_doCommand__FP17st_ZDISPATCH_DATAP20st_ZDISPATCH_CONTEXT
static signed int ZDSP_doCommand(class st_ZDISPATCH_CONTEXT *cmdCtxt) {
    { // 0x001A8840 - 0x001A9004
        void *result; // r17
        void *indata; // r16
        enum en_DISPATCH_COMMAND cmd; // r3
        signed int iv; // r2
        static signed int warned; // 0x003581F4
        static signed char init; // 0x003581F8
    }
}

// 0x001A9010 - 0x001A9050 (0x40) ZDSP_injectCmd__FP17st_ZDISPATCH_DATA19en_DISPATCH_COMMANDPvPvPv
void ZDSP_injectCmd(class st_ZDISPATCH_DATA *dspdata, enum en_DISPATCH_COMMAND cmd, void *indata, void *inxtra, void *result) {
    { // 0x001A9010 - 0x001A9050
        class st_ZDISPATCH_CONTEXT cmdctxt; // r29+0x10
    }
}

// 0x001A9050 - 0x001A90C0 (0x70) ZDSP_instInit__FP17st_ZDISPATCH_DATAP11p2BaseAsset
static void ZDSP_instInit(class st_ZDISPATCH_DATA *dspdata, class p2BaseAsset *bass) {
    { // 0x001A9050 - 0x001A90C0
    }
}

// 0x001A90C0 - 0x001A90C8 (0x8) zDispatcher_Load__FP17st_ZDISPATCH_DATAP7xSerial
void zDispatcher_Load(class st_ZDISPATCH_DATA *dspdata, class xSerial *s) {
    { // 0x001A90C0 - 0x001A90C8
    }
}

// 0x001A90D0 - 0x001A90D8 (0x8) zDispatcher_Save__FP17st_ZDISPATCH_DATAP7xSerial
void zDispatcher_Save(class st_ZDISPATCH_DATA *dspdata, class xSerial *s) {
    { // 0x001A90D0 - 0x001A90D8
    }
}

// 0x001A90E0 - 0x001A90F0 (0x10) zDispatcher_InitDep__FP17st_ZDISPATCH_DATAP6zScene
void zDispatcher_InitDep(class st_ZDISPATCH_DATA *dspdata) {
    { // 0x001A90E0 - 0x001A90F0
    }
}

// 0x001A90F0 - 0x001A9160 (0x70) zDispatcher_Init__FP17st_ZDISPATCH_DATAP11p2BaseAsset
void zDispatcher_Init(class st_ZDISPATCH_DATA *dspdata, class p2BaseAsset *bass) {
    { // 0x001A90F0 - 0x001A9160
    }
}

// 0x001A9160 - 0x001A9174 (0x14) zDispatcher_getInst__FP17st_ZDISPATCH_DATAi
class st_ZDISPATCH_DATA *zDispatcher_getInst(class st_ZDISPATCH_DATA *pool, signed int idx) {
    { // 0x001A9160 - 0x001A9174
    }
}

// 0x001A9180 - 0x001A9200 (0x80) zDispatcher_memPool__Fi
class st_ZDISPATCH_DATA *zDispatcher_memPool(signed int cnt) {
    { // 0x001A9180 - 0x001A9200
        class st_ZDISPATCH_DEPOT *depot; // r17
        class st_ZDISPATCH_DATA *pool; // r16
    }
}

// 0x001A9200 - 0x001A9234 (0x34) zDispatcher_sceneFinish__Fv
void zDispatcher_sceneFinish() {
    { // 0x001A9200 - 0x001A9234
        class st_ZDISPATCH_DEPOT *depot; // r2
    }
}

// 0x001A9240 - 0x001A9274 (0x34) zDispatcher_scenePrepare__Fv
void zDispatcher_scenePrepare() {
    { // 0x001A9240 - 0x001A9274
        class st_ZDISPATCH_DEPOT *depot; // r2
    }
}

// 0x001A9280 - 0x001A9288 (0x8) zDispatcher_Shutdown__Fv
void zDispatcher_Shutdown() {
    { // 0x001A9280 - 0x001A9288
    }
}

// 0x001A9290 - 0x001A9298 (0x8) zDispatcher_Startup__Fv
void zDispatcher_Startup() {
    { // 0x001A9290 - 0x001A9298
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xSpline.cpp
// ==========================================================================
static float sBasisHermite[4][4]; // 0x002B34D0
static float sBasisBezier[4][4]; // 0x002B3490
static float sBasisUniformBspline[4][4]; // 0x00000000
// 0x001A92A0 - 0x001A93D4 (0x134) xSpline3_Bezier__FP6_xVec3PfUiUiP6_xVec3P6_xVec3
class xSpline3 *xSpline3_Bezier(class _xVec3 *points, float *time, unsigned int numpoints, unsigned int numalloc, class _xVec3 *p1, class _xVec3 *p2) {
    { // 0x001A92A0 - 0x001A93D4
        unsigned int i; // r5
        class xSpline3 *spl; // r16
    }
}

// 0x001A93E0 - 0x001A9510 (0x130) AllocSpline3__FP6_xVec3PfUiUiUiUi
static class xSpline3 *AllocSpline3(class _xVec3 *points, float *time, unsigned int numpoints, unsigned int numalloc, unsigned int flags, unsigned int type) {
    { // 0x001A93E0 - 0x001A9510
        class xSpline3 *spl; // r16
    }
}

// 0x001A9510 - 0x001A9A2C (0x51C) xSpline3_ArcInit__FP8xSpline3Ui
void xSpline3_ArcInit(class xSpline3 *spl, unsigned int sample) {
    { // 0x001A9510 - 0x001A9A2C
        class xCoef3 tempCoef; // r29+0xf0
        float arcsum; // 
        float len; // r29+0x190
        unsigned int seg; // r29+0xb0
        unsigned int i; // r16
    }
}

// 0x001A9A30 - 0x001A9A74 (0x44) xSpline3_EvalArcApprox__FP8xSpline3fUiP6_xVec3
float xSpline3_EvalArcApprox(class xSpline3 *spl, float s, unsigned int deriv, class _xVec3 *o) {
    { // 0x001A9A30 - 0x001A9A74
    }
}

// 0x001A9A80 - 0x001AA11C (0x69C) ArcEvalIterate__FP8xSpline3fUiP6_xVec3Ui
static float ArcEvalIterate(class xSpline3 *spl, float s, unsigned int deriv, class _xVec3 *o, unsigned int iterations) {
    { // 0x001A9A80 - 0x001AA11C
        signed int segmul; // r6
        signed int seg; // r2
        signed int test; // r7
        signed int max; // r2
        signed int min; // r6
        float arctest; // r29+0x130
        float utest; // 
        float smax; // 
        float smin; // r29+0x130
        float umax; // 
        float umin; // r29+0x130
        class xCoef3 tempCoef; // r29+0x90
    }
}

// 0x001AA120 - 0x001AA5A8 (0x488) xSpline3_EvalSeg__FP8xSpline3fUiP6_xVec3
void xSpline3_EvalSeg(class xSpline3 *spl, float u, unsigned int deriv, class _xVec3 *o) {
    { // 0x001AA120 - 0x001AA5A8
        unsigned int seg; // r7
        float flr; // 
        class xCoef3 tempCoef; // r29+0x60
    }
}

// 0x001AA5B0 - 0x001AA610 (0x60) SegBspline__FP8xSpline3f
static unsigned int SegBspline(class xSpline3 *spl, float u) {
    { // 0x001AA5B0 - 0x001AA610
        unsigned int mid; // r7
        unsigned int max; // r6
        unsigned int min; // r5
    }
}

// 0x001AA610 - 0x001AA7CC (0x1BC) BasisBspline__FPA4_fPf
static void BasisBspline(float (*N)[4], float *t) {
    { // 0x001AA610 - 0x001AA7CC
        float Ntemp[4]; // r29+0x0
        float d2; // 
        float d1; // 
        unsigned int c; // r6
        unsigned int k; // r8
        unsigned int i; // r7
    }
}

// 0x001AA7D0 - 0x001AA884 (0xB4) CoefToUnity3__FP6xCoef3P6xCoef3ff
static void CoefToUnity3(class xCoef3 *dest, class xCoef3 *coef, float t1, float t2) {
    { // 0x001AA7D0 - 0x001AA884
        float dt; // 
        float *o; // r2
        float *p; // r2
        float d; // 
        float c; // 
        float b; // 
        unsigned int i; // r6
    }
}

// 0x001AA890 - 0x001AAA64 (0x1D4) EvalCoef3__FP6xCoef3fUiP6_xVec3
static void EvalCoef3(class xCoef3 *coef, float u, unsigned int deriv, class _xVec3 *o) {
    { // 0x001AA890 - 0x001AAA64
    }
}

// 0x001AAA70 - 0x001AACB0 (0x240) ArcLength3__FP6xCoef3ff
static float ArcLength3(class xCoef3 *coef, float ustart, float uend) {
    { // 0x001AAA70 - 0x001AACB0
        float u; // 
        float sum; // 
        float h; // 
        float E; // 
        float D; // 
        float C; // 
        float B; // 
        float A; // 
        unsigned int i; // r16
    }
}

// 0x001AACB0 - 0x001AAEB0 (0x200) Interpolate_Bspline__FP6_xVec3P6_xVec3PfUi
static void Interpolate_Bspline(class _xVec3 *data, class _xVec3 *control, float *knots, unsigned int nodata) {
    { // 0x001AACB0 - 0x001AAEB0
        float t5; // 
        float t4; // 
        float t3; // 
        float t2; // 
        float t1; // 
        float *gamma; // r18
        float *beta; // r17
        float *alpha; // r16
        unsigned int i; // r4
    }
}

// 0x001AAEB0 - 0x001AB13C (0x28C) Tridiag_Solve__FPfPfPfP6_xVec3P6_xVec3i
static void Tridiag_Solve(float *a, float *b, float *c, class _xVec3 *d, class _xVec3 *x, signed int n) {
    { // 0x001AAEB0 - 0x001AB13C
        class _xVec3 *delta; // r17
        float *gamma; // r16
        float beta; // 
        signed int j; // r4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zUI.cpp
// ==========================================================================
void (zUI_Update)(class _zUI *, class _xScene *, float); // 0x001AC040
signed int (zUIEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001AB140
class xBase * (zUIRenderIteratorInsert)(class xBase *, class zScene *, void *); // 0x001AC010
signed int (iRenderQSort_Face)(void *, void *); // 0x001ABFC0
class zGlobals globals; // 0x00362FD0
float gFontWiggleTime; // 0x003580EC
static float ushift; // 0x00358224
static class zSceneLevel sLevelTable2[14]; // 0x002B3520
static unsigned int cKeyUIid4on; // 0x00358220
static unsigned int cKeyUIid3on; // 0x0035821C
static unsigned int cKeyUIid2on; // 0x00358218
static unsigned int cKeyUIid1on; // 0x00358214
static unsigned int cKeyUIid4off; // 0x00358210
static unsigned int cKeyUIid3off; // 0x0035820C
static unsigned int cKeyUIid2off; // 0x00358208
static unsigned int cKeyUIid1off; // 0x00358204
static unsigned short Index[6]; // 0x002B3510
static class RwSky2DVertex Vertex[4]; // 0x00379040
class _zUI *sSorted[768]; // 0x00378440
static unsigned int sSortedCount; // 0x00358200
// 0x001AB140 - 0x001AB474 (0x334) zUIEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zUIEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001AB140 - 0x001AB474
        char uiName[256]; // r29+0x180
        signed int j; // r18
        signed int i; // r16
        char uiName[256]; // r29+0x80
        signed int j; // r18
        signed int i; // r22
        char level; // r2
        class _zUI *s; // r17
    }
}

// 0x001AB480 - 0x001ABE50 (0x9D0) zUI_Render__FP5_xEnt
void zUI_Render(class _xEnt *ent) {
    { // 0x001AB480 - 0x001ABE50
        class _xRect win1; // r29+0x80
        class _xVec3 *pos; // r8
        class _xVec3 relto; // r29+0xc0
        class _xVec3 relfrom; // r29+0xb0
        class _xVec3 to; // r29+0xa0
        class _xVec3 from; // r29+0x90
        float cooz; // 
        float cz; // 
        float z; // r29+0xd0
        float pulse_w; // 
        float wave; // r29+0xd0
        float pulse_h; // r29+0xd0
        float pulse_w; // 
        float wave; // 
        float y2; // 
        float x2; // 
        float y1; // 
        float x1; // 
        float v4; // 
        float u4; // 
        float v3; // 
        float u3; // 
        float v2; // 
        float u2; // 
        float v1; // 
        float u1; // 
        class RwCamera *camera; // r18
        unsigned char a; // r17
        unsigned char b; // r2
        unsigned char g; // r2
        unsigned char r; // r2
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
        class _zUI *ui; // r2
        static unsigned int hack_token2; // 0x00358248
        static unsigned int hack_token1; // 0x00358240
        static signed char init; // 0x0035824C
        static signed char init; // 0x00358244
        static unsigned int hack_pulse3; // 0x00358238
        static unsigned int hack_pulse2; // 0x00358230
        static unsigned int hack_pulse1; // 0x00358228
        static signed char init; // 0x0035823C
        static signed char init; // 0x00358234
        static signed char init; // 0x0035822C
    }
}

// 0x001ABE50 - 0x001ABFB8 (0x168) zUIRenderAll__Fv
void zUIRenderAll() {
    { // 0x001ABE50 - 0x001ABFB8
        signed int i; // r16
    }
}

// 0x001ABFC0 - 0x001AC010 (0x50) iRenderQSort_Face__FPCvPCv
signed int iRenderQSort_Face(void *arg1, void *arg2) {
    { // 0x001ABFC0 - 0x001AC010
    }
}

// 0x001AC010 - 0x001AC040 (0x30) zUIRenderIteratorInsert__FP5xBaseP6zScenePv
static class xBase *zUIRenderIteratorInsert(class xBase *b) {
    { // 0x001AC010 - 0x001AC040
    }
}

// 0x001AC040 - 0x001AC24C (0x20C) zUI_Update__FP4_zUIP7_xScenef
void zUI_Update(class _zUI *ent, class _xScene *sc, float dt) {
    { // 0x001AC040 - 0x001AC24C
    }
}

// 0x001AC250 - 0x001AC468 (0x218) zUI_PreUpdate__FP4_zUIP7_xScenef
void zUI_PreUpdate(class _zUI *ent) {
    { // 0x001AC250 - 0x001AC468
    }
}

// 0x001AC470 - 0x001AC4B8 (0x48) zUI_Reset__FP4_zUI
void zUI_Reset(class _zUI *ent) {
    { // 0x001AC470 - 0x001AC4B8
    }
}

// 0x001AC4C0 - 0x001AC4C8 (0x8) zUI_Load__FP4_zUIP7xSerial
void zUI_Load(class _zUI *ent, class xSerial *s) {
    { // 0x001AC4C0 - 0x001AC4C8
    }
}

// 0x001AC4D0 - 0x001AC4D8 (0x8) zUI_Save__FP4_zUIP7xSerial
void zUI_Save(class _zUI *ent, class xSerial *s) {
    { // 0x001AC4D0 - 0x001AC4D8
    }
}

// 0x001AC4E0 - 0x001AC614 (0x134) zUI_Init__FP4_zUIP10p2EntAsset
void zUI_Init(class _zUI *ent, class p2EntAsset *asset) {
    { // 0x001AC4E0 - 0x001AC614
        class zUIAsset *sasset; // r2
    }
}

// 0x001AC620 - 0x001AC628 (0x8) zUI_Init__FPvPv
void zUI_Init(void *ent, void *asset) {
    { // 0x001AC620 - 0x001AC628
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zUIFont.cpp
// ==========================================================================
void (zUIFont_Render)(class _xEnt *); // 0x001AC630
void (zUIFont_Update)(class _zUIFont *, class _xScene *, float); // 0x001ACC00
signed int (zUIFontEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001ACA90
class zGlobals globals; // 0x00362FD0
float gFontWiggle; // 0x003580E8
// 0x001AC630 - 0x001ACA84 (0x454) zUIFont_Render__FP5_xEnt
void zUIFont_Render(class _xEnt *e) {
    { // 0x001AC630 - 0x001ACA84
        char processedText[512]; // r29+0x60
        char *text; // r2
        unsigned int jflags; // r2
        class xFontBox *b; // r17
        class iColor_tag c; // r29+0x26c
        class _zUIFont *ent; // r16
    }
}

// 0x001ACA90 - 0x001ACC00 (0x170) zUIFontEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zUIFontEventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001ACA90 - 0x001ACC00
        class _zUIFont *s; // r17
        signed int rval; // r16
    }
}

// 0x001ACC00 - 0x001ACEF4 (0x2F4) zUIFont_Update__FP8_zUIFontP7_xScenef
void zUIFont_Update(class _zUIFont *ent, class _xScene *sc, float dt) {
    { // 0x001ACC00 - 0x001ACEF4
        class _zUI *ui; // r2
    }
}

// 0x001ACF00 - 0x001AD118 (0x218) zUIFont_PreUpdate__FP8_zUIFontP7_xScenef
void zUIFont_PreUpdate(class _zUIFont *ent) {
    { // 0x001ACF00 - 0x001AD118
        class _zUI *ui; // r2
    }
}

// 0x001AD120 - 0x001AD128 (0x8) zUIFont_Load__FP8_zUIFontP7xSerial
void zUIFont_Load(class _zUIFont *ent, class xSerial *s) {
    { // 0x001AD120 - 0x001AD128
    }
}

// 0x001AD130 - 0x001AD138 (0x8) zUIFont_Save__FP8_zUIFontP7xSerial
void zUIFont_Save(class _zUIFont *ent, class xSerial *s) {
    { // 0x001AD130 - 0x001AD138
    }
}

// 0x001AD140 - 0x001AD290 (0x150) zUIFont_Init__FP8_zUIFontP10p2EntAsset
void zUIFont_Init(class _zUIFont *ent, class p2EntAsset *asset) {
    { // 0x001AD140 - 0x001AD290
    }
}

// 0x001AD290 - 0x001AD298 (0x8) zUIFont_Init__FPvPv
void zUIFont_Init(void *ent, void *asset) {
    { // 0x001AD290 - 0x001AD298
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zVar.cpp
// ==========================================================================
class zSaveLoadGame zSaveLoadGameTable[0]; // 0x003ABBF0
class xSndGlobals gSnd; // 0x00373B80
class zGlobals globals; // 0x00362FD0
char txt[64]; // 0x00379160
signed int sVarMagicInit; // 0x00358280
class zVarMagicEntry sVarMagicEntryTrable[32]; // 0x002B3CD0
class zVarEntry zVarEntryTable[108]; // 0x002B3610
static unsigned int sSnackGateValue; // 0x0035827C
static unsigned int selSceneID; // 0x00358278
static unsigned int allmovies; // 0x00358274
static unsigned int endgame; // 0x00358270
static unsigned int MMDefeated; // 0x0035826C
static unsigned int F001Unlocked; // 0x00358268
static unsigned int E001Unlocked; // 0x00358264
static unsigned int S001Unlocked; // 0x00358260
static unsigned int R001Unlocked; // 0x0035825C
static unsigned int hasMap; // 0x00358258
// 0x001AD2A0 - 0x001AD2D0 (0x30) zVarEntryCB_eVarCin14__Fv
unsigned int zVarEntryCB_eVarCin14() {
    { // 0x001AD2A0 - 0x001AD2D0
    }
}

// 0x001AD2D0 - 0x001AD300 (0x30) zVarEntryCB_eVarCin13__Fv
unsigned int zVarEntryCB_eVarCin13() {
    { // 0x001AD2D0 - 0x001AD300
    }
}

// 0x001AD300 - 0x001AD330 (0x30) zVarEntryCB_eVarCin12__Fv
unsigned int zVarEntryCB_eVarCin12() {
    { // 0x001AD300 - 0x001AD330
    }
}

// 0x001AD330 - 0x001AD338 (0x8) zVarEntryCB_eVarCin11__Fv
unsigned int zVarEntryCB_eVarCin11() {
    { // 0x001AD330 - 0x001AD338
    }
}

// 0x001AD340 - 0x001AD370 (0x30) zVarEntryCB_eVarCin10__Fv
unsigned int zVarEntryCB_eVarCin10() {
    { // 0x001AD340 - 0x001AD370
    }
}

// 0x001AD370 - 0x001AD3A0 (0x30) zVarEntryCB_eVarCin9__Fv
unsigned int zVarEntryCB_eVarCin9() {
    { // 0x001AD370 - 0x001AD3A0
    }
}

// 0x001AD3A0 - 0x001AD3D0 (0x30) zVarEntryCB_eVarCin8__Fv
unsigned int zVarEntryCB_eVarCin8() {
    { // 0x001AD3A0 - 0x001AD3D0
    }
}

// 0x001AD3D0 - 0x001AD400 (0x30) zVarEntryCB_eVarCin7__Fv
unsigned int zVarEntryCB_eVarCin7() {
    { // 0x001AD3D0 - 0x001AD400
    }
}

// 0x001AD400 - 0x001AD430 (0x30) zVarEntryCB_eVarCin6__Fv
unsigned int zVarEntryCB_eVarCin6() {
    { // 0x001AD400 - 0x001AD430
    }
}

// 0x001AD430 - 0x001AD460 (0x30) zVarEntryCB_eVarCin5__Fv
unsigned int zVarEntryCB_eVarCin5() {
    { // 0x001AD430 - 0x001AD460
    }
}

// 0x001AD460 - 0x001AD490 (0x30) zVarEntryCB_eVarCin4__Fv
unsigned int zVarEntryCB_eVarCin4() {
    { // 0x001AD460 - 0x001AD490
    }
}

// 0x001AD490 - 0x001AD4C0 (0x30) zVarEntryCB_eVarCin3__Fv
unsigned int zVarEntryCB_eVarCin3() {
    { // 0x001AD490 - 0x001AD4C0
    }
}

// 0x001AD4C0 - 0x001AD4F0 (0x30) zVarEntryCB_eVarCin2__Fv
unsigned int zVarEntryCB_eVarCin2() {
    { // 0x001AD4C0 - 0x001AD4F0
    }
}

// 0x001AD4F0 - 0x001AD520 (0x30) zVarEntryCB_eVarCin1__Fv
unsigned int zVarEntryCB_eVarCin1() {
    { // 0x001AD4F0 - 0x001AD520
    }
}

// 0x001AD520 - 0x001AD598 (0x78) zVarEntryCB_eVarAllSnacksFound__Fv
unsigned int zVarEntryCB_eVarAllSnacksFound() {
    { // 0x001AD520 - 0x001AD598
        signed int i; // r18
        unsigned int cnt; // r17
    }
}

// 0x001AD5A0 - 0x001AD5A8 (0x8) zVarEntryCB_eVareVarDefeatedMM__Fv
unsigned int zVarEntryCB_eVareVarDefeatedMM() {
    { // 0x001AD5A0 - 0x001AD5A8
    }
}

// 0x001AD5B0 - 0x001AD5B8 (0x8) zVarEntryCB_eVarWarpH003__Fv
unsigned int zVarEntryCB_eVarWarpH003() {
    { // 0x001AD5B0 - 0x001AD5B8
    }
}

// 0x001AD5C0 - 0x001AD5C8 (0x8) zVarEntryCB_eVarWarpW026__Fv
unsigned int zVarEntryCB_eVarWarpW026() {
    { // 0x001AD5C0 - 0x001AD5C8
    }
}

// 0x001AD5D0 - 0x001AD5D8 (0x8) zVarEntryCB_eVarWarpW022__Fv
unsigned int zVarEntryCB_eVarWarpW022() {
    { // 0x001AD5D0 - 0x001AD5D8
    }
}

// 0x001AD5E0 - 0x001AD5E8 (0x8) zVarEntryCB_eVarWarpS003__Fv
unsigned int zVarEntryCB_eVarWarpS003() {
    { // 0x001AD5E0 - 0x001AD5E8
    }
}

// 0x001AD5F0 - 0x001AD5F8 (0x8) zVarEntryCB_eVarWarpR003__Fv
unsigned int zVarEntryCB_eVarWarpR003() {
    { // 0x001AD5F0 - 0x001AD5F8
    }
}

// 0x001AD600 - 0x001AD608 (0x8) zVarEntryCB_eVarWarpP005__Fv
unsigned int zVarEntryCB_eVarWarpP005() {
    { // 0x001AD600 - 0x001AD608
    }
}

// 0x001AD610 - 0x001AD618 (0x8) zVarEntryCB_eVarWarpP003__Fv
unsigned int zVarEntryCB_eVarWarpP003() {
    { // 0x001AD610 - 0x001AD618
    }
}

// 0x001AD620 - 0x001AD628 (0x8) zVarEntryCB_eVarWarpO006__Fv
unsigned int zVarEntryCB_eVarWarpO006() {
    { // 0x001AD620 - 0x001AD628
    }
}

// 0x001AD630 - 0x001AD638 (0x8) zVarEntryCB_eVarWarpO004__Fv
unsigned int zVarEntryCB_eVarWarpO004() {
    { // 0x001AD630 - 0x001AD638
    }
}

// 0x001AD640 - 0x001AD648 (0x8) zVarEntryCB_eVarWarpL018__Fv
unsigned int zVarEntryCB_eVarWarpL018() {
    { // 0x001AD640 - 0x001AD648
    }
}

// 0x001AD650 - 0x001AD658 (0x8) zVarEntryCB_eVarWarpL015__Fv
unsigned int zVarEntryCB_eVarWarpL015() {
    { // 0x001AD650 - 0x001AD658
    }
}

// 0x001AD660 - 0x001AD668 (0x8) zVarEntryCB_eVarWarpL014__Fv
unsigned int zVarEntryCB_eVarWarpL014() {
    { // 0x001AD660 - 0x001AD668
    }
}

// 0x001AD670 - 0x001AD678 (0x8) zVarEntryCB_eVarWarpI006__Fv
unsigned int zVarEntryCB_eVarWarpI006() {
    { // 0x001AD670 - 0x001AD678
    }
}

// 0x001AD680 - 0x001AD688 (0x8) zVarEntryCB_eVarWarpI003__Fv
unsigned int zVarEntryCB_eVarWarpI003() {
    { // 0x001AD680 - 0x001AD688
    }
}

// 0x001AD690 - 0x001AD698 (0x8) zVarEntryCB_eVarWarpH001__Fv
unsigned int zVarEntryCB_eVarWarpH001() {
    { // 0x001AD690 - 0x001AD698
    }
}

// 0x001AD6A0 - 0x001AD6A8 (0x8) zVarEntryCB_eVarWarpG008__Fv
unsigned int zVarEntryCB_eVarWarpG008() {
    { // 0x001AD6A0 - 0x001AD6A8
    }
}

// 0x001AD6B0 - 0x001AD6B8 (0x8) zVarEntryCB_eVarWarpG005__Fv
unsigned int zVarEntryCB_eVarWarpG005() {
    { // 0x001AD6B0 - 0x001AD6B8
    }
}

// 0x001AD6C0 - 0x001AD6C8 (0x8) zVarEntryCB_eVarWarpG001__Fv
unsigned int zVarEntryCB_eVarWarpG001() {
    { // 0x001AD6C0 - 0x001AD6C8
    }
}

// 0x001AD6D0 - 0x001AD6D8 (0x8) zVarEntryCB_eVarWarpF010__Fv
unsigned int zVarEntryCB_eVarWarpF010() {
    { // 0x001AD6D0 - 0x001AD6D8
    }
}

// 0x001AD6E0 - 0x001AD6E8 (0x8) zVarEntryCB_eVarWarpF007__Fv
unsigned int zVarEntryCB_eVarWarpF007() {
    { // 0x001AD6E0 - 0x001AD6E8
    }
}

// 0x001AD6F0 - 0x001AD6F8 (0x8) zVarEntryCB_eVarWarpF003__Fv
unsigned int zVarEntryCB_eVarWarpF003() {
    { // 0x001AD6F0 - 0x001AD6F8
    }
}

// 0x001AD700 - 0x001AD708 (0x8) zVarEntryCB_eVarWarpE009__Fv
unsigned int zVarEntryCB_eVarWarpE009() {
    { // 0x001AD700 - 0x001AD708
    }
}

// 0x001AD710 - 0x001AD718 (0x8) zVarEntryCB_eVarWarpE006__Fv
unsigned int zVarEntryCB_eVarWarpE006() {
    { // 0x001AD710 - 0x001AD718
    }
}

// 0x001AD720 - 0x001AD728 (0x8) zVarEntryCB_eVarWarpE004__Fv
unsigned int zVarEntryCB_eVarWarpE004() {
    { // 0x001AD720 - 0x001AD728
    }
}

// 0x001AD730 - 0x001AD738 (0x8) zVarEntryCB_eVarWarpC004__Fv
unsigned int zVarEntryCB_eVarWarpC004() {
    { // 0x001AD730 - 0x001AD738
    }
}

// 0x001AD740 - 0x001AD748 (0x8) zVarEntryCB_eVarWarpB004__Fv
unsigned int zVarEntryCB_eVarWarpB004() {
    { // 0x001AD740 - 0x001AD748
    }
}

// 0x001AD750 - 0x001AD76C (0x1C) zVarEntryCB_eVarMToken21__Fv
unsigned int zVarEntryCB_eVarMToken21() {
    { // 0x001AD750 - 0x001AD76C
    }
}

// 0x001AD770 - 0x001AD78C (0x1C) zVarEntryCB_eVarMToken20__Fv
unsigned int zVarEntryCB_eVarMToken20() {
    { // 0x001AD770 - 0x001AD78C
    }
}

// 0x001AD790 - 0x001AD7AC (0x1C) zVarEntryCB_eVarMToken19__Fv
unsigned int zVarEntryCB_eVarMToken19() {
    { // 0x001AD790 - 0x001AD7AC
    }
}

// 0x001AD7B0 - 0x001AD7CC (0x1C) zVarEntryCB_eVarMToken18__Fv
unsigned int zVarEntryCB_eVarMToken18() {
    { // 0x001AD7B0 - 0x001AD7CC
    }
}

// 0x001AD7D0 - 0x001AD7EC (0x1C) zVarEntryCB_eVarMToken17__Fv
unsigned int zVarEntryCB_eVarMToken17() {
    { // 0x001AD7D0 - 0x001AD7EC
    }
}

// 0x001AD7F0 - 0x001AD808 (0x18) zVarEntryCB_eVarMToken16__Fv
unsigned int zVarEntryCB_eVarMToken16() {
    { // 0x001AD7F0 - 0x001AD808
    }
}

// 0x001AD810 - 0x001AD828 (0x18) zVarEntryCB_eVarMToken15__Fv
unsigned int zVarEntryCB_eVarMToken15() {
    { // 0x001AD810 - 0x001AD828
    }
}

// 0x001AD830 - 0x001AD848 (0x18) zVarEntryCB_eVarMToken14__Fv
unsigned int zVarEntryCB_eVarMToken14() {
    { // 0x001AD830 - 0x001AD848
    }
}

// 0x001AD850 - 0x001AD868 (0x18) zVarEntryCB_eVarMToken13__Fv
unsigned int zVarEntryCB_eVarMToken13() {
    { // 0x001AD850 - 0x001AD868
    }
}

// 0x001AD870 - 0x001AD888 (0x18) zVarEntryCB_eVarMToken12__Fv
unsigned int zVarEntryCB_eVarMToken12() {
    { // 0x001AD870 - 0x001AD888
    }
}

// 0x001AD890 - 0x001AD8A8 (0x18) zVarEntryCB_eVarMToken11__Fv
unsigned int zVarEntryCB_eVarMToken11() {
    { // 0x001AD890 - 0x001AD8A8
    }
}

// 0x001AD8B0 - 0x001AD8C8 (0x18) zVarEntryCB_eVarMToken10__Fv
unsigned int zVarEntryCB_eVarMToken10() {
    { // 0x001AD8B0 - 0x001AD8C8
    }
}

// 0x001AD8D0 - 0x001AD8E8 (0x18) zVarEntryCB_eVarMToken9__Fv
unsigned int zVarEntryCB_eVarMToken9() {
    { // 0x001AD8D0 - 0x001AD8E8
    }
}

// 0x001AD8F0 - 0x001AD908 (0x18) zVarEntryCB_eVarMToken8__Fv
unsigned int zVarEntryCB_eVarMToken8() {
    { // 0x001AD8F0 - 0x001AD908
    }
}

// 0x001AD910 - 0x001AD928 (0x18) zVarEntryCB_eVarMToken7__Fv
unsigned int zVarEntryCB_eVarMToken7() {
    { // 0x001AD910 - 0x001AD928
    }
}

// 0x001AD930 - 0x001AD948 (0x18) zVarEntryCB_eVarMToken6__Fv
unsigned int zVarEntryCB_eVarMToken6() {
    { // 0x001AD930 - 0x001AD948
    }
}

// 0x001AD950 - 0x001AD968 (0x18) zVarEntryCB_eVarMToken5__Fv
unsigned int zVarEntryCB_eVarMToken5() {
    { // 0x001AD950 - 0x001AD968
    }
}

// 0x001AD970 - 0x001AD988 (0x18) zVarEntryCB_eVarMToken4__Fv
unsigned int zVarEntryCB_eVarMToken4() {
    { // 0x001AD970 - 0x001AD988
    }
}

// 0x001AD990 - 0x001AD9A8 (0x18) zVarEntryCB_eVarMToken3__Fv
unsigned int zVarEntryCB_eVarMToken3() {
    { // 0x001AD990 - 0x001AD9A8
    }
}

// 0x001AD9B0 - 0x001AD9C8 (0x18) zVarEntryCB_eVarMToken2__Fv
unsigned int zVarEntryCB_eVarMToken2() {
    { // 0x001AD9B0 - 0x001AD9C8
    }
}

// 0x001AD9D0 - 0x001AD9E8 (0x18) zVarEntryCB_eVarMToken1__Fv
unsigned int zVarEntryCB_eVarMToken1() {
    { // 0x001AD9D0 - 0x001AD9E8
    }
}

// 0x001AD9F0 - 0x001ADA8C (0x9C) zVarEntryCB_CurrentRoom__Fv
unsigned int zVarEntryCB_CurrentRoom() {
    { // 0x001AD9F0 - 0x001ADA8C
        char s[16]; // r29+0x10
        signed int room; // r4
    }
}

// 0x001ADA90 - 0x001ADAB4 (0x24) zVarEntryCB_PuzzlePieceH__Fv
unsigned int zVarEntryCB_PuzzlePieceH() {
    { // 0x001ADA90 - 0x001ADAB4
    }
}

// 0x001ADAC0 - 0x001ADAE4 (0x24) zVarEntryCB_PuzzlePieceO__Fv
unsigned int zVarEntryCB_PuzzlePieceO() {
    { // 0x001ADAC0 - 0x001ADAE4
    }
}

// 0x001ADAF0 - 0x001ADB14 (0x24) zVarEntryCB_PuzzlePieceR__Fv
unsigned int zVarEntryCB_PuzzlePieceR() {
    { // 0x001ADAF0 - 0x001ADB14
    }
}

// 0x001ADB20 - 0x001ADB44 (0x24) zVarEntryCB_PuzzlePieceI__Fv
unsigned int zVarEntryCB_PuzzlePieceI() {
    { // 0x001ADB20 - 0x001ADB44
    }
}

// 0x001ADB50 - 0x001ADB74 (0x24) zVarEntryCB_PuzzlePieceS__Fv
unsigned int zVarEntryCB_PuzzlePieceS() {
    { // 0x001ADB50 - 0x001ADB74
    }
}

// 0x001ADB80 - 0x001ADBA4 (0x24) zVarEntryCB_PuzzlePieceB__Fv
unsigned int zVarEntryCB_PuzzlePieceB() {
    { // 0x001ADB80 - 0x001ADBA4
    }
}

// 0x001ADBB0 - 0x001ADBD4 (0x24) zVarEntryCB_PuzzlePieceP__Fv
unsigned int zVarEntryCB_PuzzlePieceP() {
    { // 0x001ADBB0 - 0x001ADBD4
    }
}

// 0x001ADBE0 - 0x001ADC04 (0x24) zVarEntryCB_PuzzlePieceG__Fv
unsigned int zVarEntryCB_PuzzlePieceG() {
    { // 0x001ADBE0 - 0x001ADC04
    }
}

// 0x001ADC10 - 0x001ADC34 (0x24) zVarEntryCB_PuzzlePieceC__Fv
unsigned int zVarEntryCB_PuzzlePieceC() {
    { // 0x001ADC10 - 0x001ADC34
    }
}

// 0x001ADC40 - 0x001ADC64 (0x24) zVarEntryCB_PuzzlePieceE__Fv
unsigned int zVarEntryCB_PuzzlePieceE() {
    { // 0x001ADC40 - 0x001ADC64
    }
}

// 0x001ADC70 - 0x001ADC94 (0x24) zVarEntryCB_PuzzlePieceW__Fv
unsigned int zVarEntryCB_PuzzlePieceW() {
    { // 0x001ADC70 - 0x001ADC94
    }
}

// 0x001ADCA0 - 0x001ADCC4 (0x24) zVarEntryCB_PuzzlePieceL__Fv
unsigned int zVarEntryCB_PuzzlePieceL() {
    { // 0x001ADCA0 - 0x001ADCC4
    }
}

// 0x001ADCD0 - 0x001ADCF4 (0x24) zVarEntryCB_PuzzlePieceF__Fv
unsigned int zVarEntryCB_PuzzlePieceF() {
    { // 0x001ADCD0 - 0x001ADCF4
    }
}

// 0x001ADD00 - 0x001ADD0C (0xC) zVarUnlockAllMovies__Fv
void zVarUnlockAllMovies() {
    { // 0x001ADD00 - 0x001ADD0C
    }
}

// 0x001ADD10 - 0x001ADD1C (0xC) zVarUnlockEndGame__Fv
void zVarUnlockEndGame() {
    { // 0x001ADD10 - 0x001ADD1C
    }
}

// 0x001ADD20 - 0x001ADD28 (0x8) finishedGame__Fv
unsigned int finishedGame() {
    { // 0x001ADD20 - 0x001ADD28
    }
}

// 0x001ADD30 - 0x001ADD38 (0x8) zVarEntryCB_UnlockedF001__Fv
unsigned int zVarEntryCB_UnlockedF001() {
    { // 0x001ADD30 - 0x001ADD38
    }
}

// 0x001ADD40 - 0x001ADD48 (0x8) zVarEntryCB_UnlockedE001__Fv
unsigned int zVarEntryCB_UnlockedE001() {
    { // 0x001ADD40 - 0x001ADD48
    }
}

// 0x001ADD50 - 0x001ADD58 (0x8) zVarEntryCB_UnlockedS001__Fv
unsigned int zVarEntryCB_UnlockedS001() {
    { // 0x001ADD50 - 0x001ADD58
    }
}

// 0x001ADD60 - 0x001ADD68 (0x8) zVarEntryCB_UnlockedR001__Fv
unsigned int zVarEntryCB_UnlockedR001() {
    { // 0x001ADD60 - 0x001ADD68
    }
}

// 0x001ADD70 - 0x001ADD78 (0x8) zVarEntryCB_MapItem__Fv
unsigned int zVarEntryCB_MapItem() {
    { // 0x001ADD70 - 0x001ADD78
    }
}

// 0x001ADD80 - 0x001ADDC4 (0x44) zVarEntryCB_IsSneaking__Fv
unsigned int zVarEntryCB_IsSneaking() {
    { // 0x001ADD80 - 0x001ADDC4
    }
}

// 0x001ADDD0 - 0x001ADE34 (0x64) zVarEntryCB_CurrentSceneLetter__Fv
unsigned int zVarEntryCB_CurrentSceneLetter() {
    { // 0x001ADDD0 - 0x001ADE34
        char s[16]; // r29+0x10
    }
}

// 0x001ADE40 - 0x001ADE7C (0x3C) zVMCB_SpaceNeeded__FPc
char *zVMCB_SpaceNeeded(char *dst) {
    { // 0x001ADE40 - 0x001ADE7C
        signed int count; // r2
    }
}

// 0x001ADE80 - 0x001ADEBC (0x3C) zVMCB_SpaceAvailable__FPc
char *zVMCB_SpaceAvailable(char *dst) {
    { // 0x001ADE80 - 0x001ADEBC
        signed int count; // r2
    }
}

// 0x001ADEC0 - 0x001ADEE8 (0x28) zVMCB_CurrentTime__FPc
char *zVMCB_CurrentTime(char *dst) {
    { // 0x001ADEC0 - 0x001ADEE8
        signed int count; // r2
    }
}

// 0x001ADEF0 - 0x001ADF18 (0x28) zVMCB_CurrentDate__FPc
char *zVMCB_CurrentDate(char *dst) {
    { // 0x001ADEF0 - 0x001ADF18
        signed int count; // r2
    }
}

// 0x001ADF20 - 0x001ADF54 (0x34) zVMCB_SnackGateValue__FPc
char *zVMCB_SnackGateValue(char *dst) {
    { // 0x001ADF20 - 0x001ADF54
        signed int count; // r2
    }
}

// 0x001ADF60 - 0x001ADFA0 (0x40) zVMCB_SelectedArea__FPc
char *zVMCB_SelectedArea(char *dst) {
    { // 0x001ADF60 - 0x001ADFA0
        signed int count; // r2
    }
}

// 0x001ADFA0 - 0x001AE014 (0x74) zVMCB_AreaStatsTokens__FPc
char *zVMCB_AreaStatsTokens(char *dst) {
    { // 0x001ADFA0 - 0x001AE014
        signed int count; // r2
    }
}

// 0x001AE020 - 0x001AE094 (0x74) zVMCB_AreaStatsSnacks__FPc
char *zVMCB_AreaStatsSnacks(char *dst) {
    { // 0x001AE020 - 0x001AE094
        signed int count; // r2
    }
}

// 0x001AE0A0 - 0x001AE0E8 (0x48) zVMCB_CurrentArea__FPc
char *zVMCB_CurrentArea(char *dst) {
    { // 0x001AE0A0 - 0x001AE0E8
        signed int count; // r2
    }
}

// 0x001AE0F0 - 0x001AE0F8 (0x8) zVMCB_TokenWorldStats__FPc
char *zVMCB_TokenWorldStats(char *dst) {
    { // 0x001AE0F0 - 0x001AE0F8
    }
}

// 0x001AE100 - 0x001AE108 (0x8) zVMCB_SnackWorldStats__FPc
char *zVMCB_SnackWorldStats(char *dst) {
    { // 0x001AE100 - 0x001AE108
    }
}

// 0x001AE110 - 0x001AE118 (0x8) zVMCB_TokenAreaStats__FPc
char *zVMCB_TokenAreaStats(char *dst) {
    { // 0x001AE110 - 0x001AE118
    }
}

// 0x001AE120 - 0x001AE128 (0x8) zVMCB_SnackAreaStats__FPc
char *zVMCB_SnackAreaStats(char *dst) {
    { // 0x001AE120 - 0x001AE128
    }
}

// 0x001AE130 - 0x001AE138 (0x8) zVMCB_TokenRoomStats__FPc
char *zVMCB_TokenRoomStats(char *dst) {
    { // 0x001AE130 - 0x001AE138
    }
}

// 0x001AE140 - 0x001AE148 (0x8) zVMCB_SnackRoomStats__FPc
char *zVMCB_SnackRoomStats(char *dst) {
    { // 0x001AE140 - 0x001AE148
    }
}

// 0x001AE150 - 0x001AE1AC (0x5C) zVMCB_CurrentPosition__FPc
char *zVMCB_CurrentPosition(char *dst) {
    { // 0x001AE150 - 0x001AE1AC
        signed int count; // r2
    }
}

// 0x001AE1B0 - 0x001AE1F0 (0x40) zVMCB_MCSelectedCard__FPc
char *zVMCB_MCSelectedCard(char *dst) {
    { // 0x001AE1B0 - 0x001AE1F0
        signed int count; // r2
    }
}

// 0x001AE1F0 - 0x001AE230 (0x40) zVMCB_MCSelectedGame__FPc
char *zVMCB_MCSelectedGame(char *dst) {
    { // 0x001AE1F0 - 0x001AE230
        signed int count; // r2
    }
}

// 0x001AE230 - 0x001AE274 (0x44) zVMCB_GameSlot7__FPc
char *zVMCB_GameSlot7(char *dst) {
    { // 0x001AE230 - 0x001AE274
        signed int count; // r2
    }
}

// 0x001AE280 - 0x001AE2C4 (0x44) zVMCB_GameSlot6__FPc
char *zVMCB_GameSlot6(char *dst) {
    { // 0x001AE280 - 0x001AE2C4
        signed int count; // r2
    }
}

// 0x001AE2D0 - 0x001AE314 (0x44) zVMCB_GameSlot5__FPc
char *zVMCB_GameSlot5(char *dst) {
    { // 0x001AE2D0 - 0x001AE314
        signed int count; // r2
    }
}

// 0x001AE320 - 0x001AE364 (0x44) zVMCB_GameSlot4__FPc
char *zVMCB_GameSlot4(char *dst) {
    { // 0x001AE320 - 0x001AE364
        signed int count; // r2
    }
}

// 0x001AE370 - 0x001AE3B4 (0x44) zVMCB_GameSlot3__FPc
char *zVMCB_GameSlot3(char *dst) {
    { // 0x001AE370 - 0x001AE3B4
        signed int count; // r2
    }
}

// 0x001AE3C0 - 0x001AE404 (0x44) zVMCB_GameSlot2__FPc
char *zVMCB_GameSlot2(char *dst) {
    { // 0x001AE3C0 - 0x001AE404
        signed int count; // r2
    }
}

// 0x001AE410 - 0x001AE454 (0x44) zVMCB_GameSlot1__FPc
char *zVMCB_GameSlot1(char *dst) {
    { // 0x001AE410 - 0x001AE454
        signed int count; // r2
    }
}

// 0x001AE460 - 0x001AE4A4 (0x44) zVMCB_GameSlot0__FPc
char *zVMCB_GameSlot0(char *dst) {
    { // 0x001AE460 - 0x001AE4A4
        signed int count; // r2
    }
}

// 0x001AE4B0 - 0x001AE4FC (0x4C) zVMCB_Scene__FPc
char *zVMCB_Scene(char *dst) {
    { // 0x001AE4B0 - 0x001AE4FC
        signed int count; // r2
    }
}

// 0x001AE500 - 0x001AE594 (0x94) zVMCB_SoundFXVolume__FPc
char *zVMCB_SoundFXVolume(char *dst) {
    { // 0x001AE500 - 0x001AE594
        signed int count; // r2
    }
}

// 0x001AE5A0 - 0x001AE634 (0x94) zVMCB_SoundMusicVolume__FPc
char *zVMCB_SoundMusicVolume(char *dst) {
    { // 0x001AE5A0 - 0x001AE634
        signed int count; // r2
    }
}

// 0x001AE640 - 0x001AE678 (0x38) zVMCB_PlayerChancesCount__FPc
char *zVMCB_PlayerChancesCount(char *dst) {
    { // 0x001AE640 - 0x001AE678
        signed int count; // r2
    }
}

// 0x001AE680 - 0x001AE6B8 (0x38) zVMCB_PlayerScoobySnackCount_Int__FPc
char *zVMCB_PlayerScoobySnackCount_Int(char *dst) {
    { // 0x001AE680 - 0x001AE6B8
        signed int count; // r2
    }
}

// 0x001AE6C0 - 0x001AE6D0 (0x10) zVarDefeatMasterMind__Fv
void zVarDefeatMasterMind() {
    { // 0x001AE6C0 - 0x001AE6D0
    }
}

// 0x001AE6D0 - 0x001AE6D8 (0x8) zVarSetSnackGateValue__FUi
void zVarSetSnackGateValue(unsigned int amount) {
    { // 0x001AE6D0 - 0x001AE6D8
    }
}

// 0x001AE6E0 - 0x001AE6E8 (0x8) zVarSetArea__FUi
void zVarSetArea(unsigned int sceneID) {
    { // 0x001AE6E0 - 0x001AE6E8
    }
}

// 0x001AE6F0 - 0x001AE6FC (0xC) zVarUnlockF001__Fv
void zVarUnlockF001() {
    { // 0x001AE6F0 - 0x001AE6FC
    }
}

// 0x001AE700 - 0x001AE70C (0xC) zVarUnlockE001__Fv
void zVarUnlockE001() {
    { // 0x001AE700 - 0x001AE70C
    }
}

// 0x001AE710 - 0x001AE71C (0xC) zVarUnlockS001__Fv
void zVarUnlockS001() {
    { // 0x001AE710 - 0x001AE71C
    }
}

// 0x001AE720 - 0x001AE72C (0xC) zVarUnlockR001__Fv
void zVarUnlockR001() {
    { // 0x001AE720 - 0x001AE72C
    }
}

// 0x001AE730 - 0x001AE73C (0xC) zVarGiveMap__Fv
void zVarGiveMap() {
    { // 0x001AE730 - 0x001AE73C
    }
}

// 0x001AE740 - 0x001AE93C (0x1FC) zVarGameSlotInfo__Fi
char *zVarGameSlotInfo(signed int i) {
    { // 0x001AE740 - 0x001AE93C
        char ampm[32]; // r29+0xa0
        signed int hour; // r16
        char time[32]; // r29+0x80
        char date[32]; // r29+0x60
    }
}

// 0x001AE940 - 0x001AE950 (0x10) zVarEntryCB_VibrationOn__Fv
unsigned int zVarEntryCB_VibrationOn() {
    { // 0x001AE940 - 0x001AE950
    }
}

// 0x001AE950 - 0x001AE9F8 (0xA8) zVarEntryCB_MCAvailable__Fv
unsigned int zVarEntryCB_MCAvailable() {
    { // 0x001AE950 - 0x001AE9F8
    }
}

// 0x001AEA00 - 0x001AEA18 (0x18) zVarEntryCB_PCSpecialLightningBoltStun__Fv
unsigned int zVarEntryCB_PCSpecialLightningBoltStun() {
    { // 0x001AEA00 - 0x001AEA18
    }
}

// 0x001AEA20 - 0x001AEA38 (0x18) zVarEntryCB_PCSpecialShovel__Fv
unsigned int zVarEntryCB_PCSpecialShovel() {
    { // 0x001AEA20 - 0x001AEA38
    }
}

// 0x001AEA40 - 0x001AEA58 (0x18) zVarEntryCB_PCSpecialUmbrella__Fv
unsigned int zVarEntryCB_PCSpecialUmbrella() {
    { // 0x001AEA40 - 0x001AEA58
    }
}

// 0x001AEA60 - 0x001AEA78 (0x18) zVarEntryCB_PCSpecialFootballHelmet__Fv
unsigned int zVarEntryCB_PCSpecialFootballHelmet() {
    { // 0x001AEA60 - 0x001AEA78
    }
}

// 0x001AEA80 - 0x001AEA98 (0x18) zVarEntryCB_PCSpecialLightningBolt__Fv
unsigned int zVarEntryCB_PCSpecialLightningBolt() {
    { // 0x001AEA80 - 0x001AEA98
    }
}

// 0x001AEAA0 - 0x001AEAB8 (0x18) zVarEntryCB_PCSpecialSpring__Fv
unsigned int zVarEntryCB_PCSpecialSpring() {
    { // 0x001AEAA0 - 0x001AEAB8
    }
}

// 0x001AEAC0 - 0x001AEAD8 (0x18) zVarEntryCB_PCSpecialDivingHelmet__Fv
unsigned int zVarEntryCB_PCSpecialDivingHelmet() {
    { // 0x001AEAC0 - 0x001AEAD8
    }
}

// 0x001AEAE0 - 0x001AEAF8 (0x18) zVarEntryCB_PCSpecialFlowerPot__Fv
unsigned int zVarEntryCB_PCSpecialFlowerPot() {
    { // 0x001AEAE0 - 0x001AEAF8
    }
}

// 0x001AEB00 - 0x001AEB18 (0x18) zVarEntryCB_PCSpecialBlackKnight__Fv
unsigned int zVarEntryCB_PCSpecialBlackKnight() {
    { // 0x001AEB00 - 0x001AEB18
    }
}

// 0x001AEB20 - 0x001AEB38 (0x18) zVarEntryCB_PCSpecialLampShade__Fv
unsigned int zVarEntryCB_PCSpecialLampShade() {
    { // 0x001AEB20 - 0x001AEB38
    }
}

// 0x001AEB40 - 0x001AEB58 (0x18) zVarEntryCB_PCSpecialSlippers__Fv
unsigned int zVarEntryCB_PCSpecialSlippers() {
    { // 0x001AEB40 - 0x001AEB58
    }
}

// 0x001AEB60 - 0x001AEB78 (0x18) zVarEntryCB_PCSpecialPlungers__Fv
unsigned int zVarEntryCB_PCSpecialPlungers() {
    { // 0x001AEB60 - 0x001AEB78
    }
}

// 0x001AEB80 - 0x001AEB98 (0x18) zVarEntryCB_PCSpecialStickyBoots__Fv
unsigned int zVarEntryCB_PCSpecialStickyBoots() {
    { // 0x001AEB80 - 0x001AEB98
    }
}

// 0x001AEBA0 - 0x001AEBB8 (0x18) zVarEntryCB_PCSpecialSoapBar__Fv
unsigned int zVarEntryCB_PCSpecialSoapBar() {
    { // 0x001AEBA0 - 0x001AEBB8
    }
}

// 0x001AEBC0 - 0x001AEBD8 (0x18) zVarEntryCB_PCSpecialBubbleGum__Fv
unsigned int zVarEntryCB_PCSpecialBubbleGum() {
    { // 0x001AEBC0 - 0x001AEBD8
    }
}

// 0x001AEBE0 - 0x001AEC50 (0x70) zVarEntryCB_SndFXVol__Fv
unsigned int zVarEntryCB_SndFXVol() {
    { // 0x001AEBE0 - 0x001AEC50
        double floatVol; // r2
    }
}

// 0x001AEC50 - 0x001AECC0 (0x70) zVarEntryCB_SndMusicVol__Fv
unsigned int zVarEntryCB_SndMusicVol() {
    { // 0x001AEC50 - 0x001AECC0
        double floatVol; // r2
    }
}

// 0x001AECC0 - 0x001AED30 (0x70) zVarEntryCB_SndMasterVol__Fv
unsigned int zVarEntryCB_SndMasterVol() {
    { // 0x001AECC0 - 0x001AED30
        double floatVol; // r2
    }
}

// 0x001AED30 - 0x001AED3C (0xC) zVarEntryCB_SndMode__Fv
unsigned int zVarEntryCB_SndMode() {
    { // 0x001AED30 - 0x001AED3C
    }
}

// 0x001AED40 - 0x001AED4C (0xC) zVarEntryCB_PCScoobySnacks__Fv
unsigned int zVarEntryCB_PCScoobySnacks() {
    { // 0x001AED40 - 0x001AED4C
    }
}

// 0x001AED50 - 0x001AED5C (0xC) zVarEntryCB_PCChances__Fv
unsigned int zVarEntryCB_PCChances() {
    { // 0x001AED50 - 0x001AED5C
    }
}

// 0x001AED60 - 0x001AEDC4 (0x64) zVarLoad__FP7xSerial
void zVarLoad(class xSerial *s) {
    { // 0x001AED60 - 0x001AEDC4
    }
}

// 0x001AEDD0 - 0x001AEE7C (0xAC) zVarSave__FP7xSerial
void zVarSave(class xSerial *s) {
    { // 0x001AEDD0 - 0x001AEE7C
    }
}

// 0x001AEE80 - 0x001AEEA0 (0x20) zVarNewGame__Fv
void zVarNewGame() {
    { // 0x001AEE80 - 0x001AEEA0
    }
}

// 0x001AEEA0 - 0x001AEEE8 (0x48) zVarInit__FP9zVarEntry
void zVarInit(class zVarEntry *table) {
    { // 0x001AEEA0 - 0x001AEEE8
        signed int i; // r16
    }
}

// 0x001AEEF0 - 0x001AF044 (0x154) zVarMagicProcessText__FPcPc
void zVarMagicProcessText(char *src, char *dst) {
    { // 0x001AEEF0 - 0x001AF044
        class zVarMagicEntry *e; // r5
        unsigned int mvID; // r2
        char *mv; // r4
        char magicVar[256]; // r29+0x50
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xParMgr.cpp
// ==========================================================================
static signed int sParMgrInit; // 0x00358288
static unsigned int sParMgrIgnoreFXCounter; // 0x00000000
static signed int sCounter; // 0x00000000
static unsigned int sFrameCount; // 0x00358284
// 0x001AF050 - 0x001AF058 (0x8) xParMgrRender__Fv
void xParMgrRender() {
    { // 0x001AF050 - 0x001AF058
    }
}

// 0x001AF060 - 0x001AF0D4 (0x74) xParMgrUpdate__Ff
void xParMgrUpdate(float elapsedTime) {
    { // 0x001AF060 - 0x001AF0D4
    }
}

// 0x001AF0E0 - 0x001AF130 (0x50) xParMgrInit__Fv
void xParMgrInit() {
    { // 0x001AF0E0 - 0x001AF130
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xPar.cpp
// ==========================================================================
class xPar *gParDead; // 0x00358290
class xPar gParPool[2000]; // 0x00379200
// 0x001AF130 - 0x001AF1A0 (0x70) xParInit__FP4xPar
void xParInit(class xPar *p) {
    { // 0x001AF130 - 0x001AF1A0
    }
}

// 0x001AF1A0 - 0x001AF1F0 (0x50) xParFree__FP4xPar
void xParFree(class xPar *par) {
    { // 0x001AF1A0 - 0x001AF1F0
    }
}

// 0x001AF1F0 - 0x001AF230 (0x40) xParAlloc__Fv
class xPar *xParAlloc() {
    { // 0x001AF1F0 - 0x001AF230
    }
}

// 0x001AF230 - 0x001AF27C (0x4C) xParMemInit__Fv
void xParMemInit() {
    { // 0x001AF230 - 0x001AF27C
        signed int i; // r4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iParMgr.cpp
// ==========================================================================
float cosSinPolynomial[0]; // 0x0028A180
class zGlobals globals; // 0x00362FD0
unsigned int ourGlobals[4096]; // 0x002B5AD0
class tagiRenderInput gRenderBuffer; // 0x003A5980
class tagiRenderArrays gRenderArr; // 0x003A0700
static signed int sParAdded; // 0x00000000
static class RwRGBA sParColors[8]; // 0x00000000
signed int sParEndAlpha; // 0x00000000
signed int sParEndColor; // 0x00000000
signed int sParStartAlpha; // 0x00000000
signed int sParStartColor; // 0x00000000
class RwV3d sParForce; // 0x00000000
float sParDamping; // 0x00000000
float sParMaxSpeed; // 0x00000000
float sParMinSpeed; // 0x00000000
float sParFlightTime; // 0x00000000
float sParAspectRatio; // 0x00000000
float sParGrowth; // 0x00000000
float sParSize; // 0x00000000
float sParEmitterAngle; // 0x00000000
float sParEmitterLength; // 0x00000000
float sParEmitterWidth; // 0x00000000
class RwTexture *sParTexture; // 0x00000000
class RwFrame *sParAtomicFrame; // 0x00000000
class RwFrame *sParClumpFrame; // 0x00000000
class RpMaterial *sParMaterial; // 0x00000000
class RpClump *sParClump; // 0x00000000
class RpAtomic *sParAtomic; // 0x00000000
static float gColorTable[256]; // 0x003A0300
static signed int gColorTableInit; // 0x00358294
// 0x001AF280 - 0x001AF4BC (0x23C) iParMgrRenderParSys_Flat__FPvP9xParGroup
void iParMgrRenderParSys_Flat(void *data, class xParGroup *ps) {
    { // 0x001AF280 - 0x001AF4BC
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
        class xPar *idx; // r16
    }
}

// 0x001AF4C0 - 0x001AFD78 (0x8B8) iParMgrRenderParSys_Ground__FPvP9xParGroup
void iParMgrRenderParSys_Ground(void *data, class xParGroup *ps) {
    { // 0x001AF4C0 - 0x001AFD78
        unsigned short *dst; // r2
        float v2; // r29+0xc0
        float v1; // 
        float u2; // 
        float u1; // 
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        float angz; // r29+0xc0
        float angy; // 
        float angx; // 
        class _xMat3x3 groundmat; // r29+0x90
        unsigned char a; // r5
        unsigned char b; // r4
        unsigned char g; // r3
        unsigned char r; // r2
        float size; // 
        void *vertices; // r2
        class xParCmdTex *tex; // r17
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
        class xPar *idx; // r16
        static unsigned short i3d[6]; // 0x002B3E70
        static class RxObjSpace3DVertex v3d[4]; // 0x003A5B20
    }
}

// 0x001AFD80 - 0x001AFD88 (0x8) iParMgrRenderParSys_Static__FPvP9xParGroup
void iParMgrRenderParSys_Static() {
    { // 0x001AFD80 - 0x001AFD88
    }
}

// 0x001AFD90 - 0x001AFFDC (0x24C) iParMgrRenderParSys_QuadStreak__FPvP9xParGroup
void iParMgrRenderParSys_QuadStreak(void *data, class xParGroup *ps) {
    { // 0x001AFD90 - 0x001AFFDC
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
        class xPar *idx; // r16
    }
}

// 0x001AFFE0 - 0x001B0388 (0x3A8) iParMgrRenderParSys_InvStreak__FPvP9xParGroup
void iParMgrRenderParSys_InvStreak(void *data, class xParGroup *ps) {
    { // 0x001AFFE0 - 0x001B0388
        class RxObjSpace3DVertex *v3d; // r6
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
        class xPar *idx; // r16
    }
}

// 0x001B0390 - 0x001B0738 (0x3A8) iParMgrRenderParSys_Streak__FPvP9xParGroup
void iParMgrRenderParSys_Streak(void *data, class xParGroup *ps) {
    { // 0x001B0390 - 0x001B0738
        class RxObjSpace3DVertex *v3d; // r6
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
        class xPar *idx; // r16
    }
}

// 0x001B0740 - 0x001B07B8 (0x78) iRenderTrianglesImmediate__FiiPviPUsi
void iRenderTrianglesImmediate(void *vertices, signed int vertexCount, unsigned short *indices, signed int indexCount) {
    { // 0x001B0740 - 0x001B07B8
    }
}

// 0x001B07C0 - 0x001B0BB8 (0x3F8) iRenderPushFlat__FP4xParP10xParCmdTex
static void iRenderPushFlat(class xPar *p, class xParCmdTex *tex) {
    { // 0x001B07C0 - 0x001B0BB8
        unsigned short *dst; // r4
        float v2; // r29+0xe0
        float v1; // 
        float u2; // 
        float u1; // 
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        float yaw; // r29+0xe0
        class _xMat3x3 groundmat; // r29+0xb0
        float size; // 
        unsigned char a; // r17
        unsigned char b; // r22
        unsigned char g; // r23
        unsigned char r; // r30
        unsigned short *indices; // r16
        void *vertices; // r2
        static unsigned short i3d[6]; // 0x002B3E60
        static class RxObjSpace3DVertex v3d[4]; // 0x003A5A90
    }
}

// 0x001B0BC0 - 0x001B0F54 (0x394) iRenderPushQuadStreak__FP4xParP10xParCmdTex
void iRenderPushQuadStreak(class xPar *p, class xParCmdTex *tex) {
    { // 0x001B0BC0 - 0x001B0F54
        unsigned short *dst; // r4
        float v2; // r29+0x60
        float v1; // 
        float u2; // 
        float u1; // 
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        float size; // 
        unsigned char a; // r2
        unsigned char b; // r2
        unsigned char g; // r2
        unsigned char r; // r2
        unsigned short *indices; // r16
        void *vertices; // r2
        static unsigned short i3d[6]; // 0x002B3E50
        static class RxObjSpace3DVertex v3d[4]; // 0x003A5A00
    }
}

// 0x001B0F60 - 0x001B1578 (0x618) iParMgrRenderParSys_Sprite__FPvP9xParGroup
void iParMgrRenderParSys_Sprite(void *data, class xParGroup *ps) {
    { // 0x001B0F60 - 0x001B1578
        float v2; // r29+0x50
        float v1; // 
        float u2; // 
        float u1; // 
        class xPar *p; // r2
        class xParCmdTex *tex; // r17
        class RxObjSpace3DVertex *v3d; // r2
        unsigned short *i3d; // r16
        signed int vertexCount; // r7
        signed int indexCount; // r3
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
        class zParSys *s; // r2
        class xPar *idx; // r2
    }
}

// 0x001B1580 - 0x001B1588 (0x8) iParMgrRender__Fv
void iParMgrRender() {
    { // 0x001B1580 - 0x001B1588
    }
}

// 0x001B1590 - 0x001B1598 (0x8) iParMgrUpdate__Ff
void iParMgrUpdate() {
    { // 0x001B1590 - 0x001B1598
    }
}

// 0x001B15A0 - 0x001B16C8 (0x128) iParMgrInit__Fv
void iParMgrInit() {
    { // 0x001B15A0 - 0x001B16C8
        signed int i; // r11
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zConditional.cpp
// ==========================================================================
signed int (zConditionalEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001B16D0
class zVarEntry zVarEntryTable[0]; // 0x002B3610
// 0x001B16D0 - 0x001B1768 (0x98) zConditionalEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zConditionalEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001B16D0 - 0x001B1768
        class _zConditional *t; // r16
    }
}

// 0x001B1770 - 0x001B1878 (0x108) zConditional_Evaluate__FP13_zConditional
unsigned int zConditional_Evaluate(class _zConditional *c) {
    { // 0x001B1770 - 0x001B1878
        signed int i; // r5
        class zVarEntry *v; // r4
        unsigned int temp; // r2
    }
}

// 0x001B1880 - 0x001B1888 (0x8) zConditionalLoad__FP13_zConditionalP7xSerial
void zConditionalLoad(class _zConditional *ent, class xSerial *s) {
    { // 0x001B1880 - 0x001B1888
    }
}

// 0x001B1890 - 0x001B1898 (0x8) zConditionalSave__FP13_zConditionalP7xSerial
void zConditionalSave(class _zConditional *ent, class xSerial *s) {
    { // 0x001B1890 - 0x001B1898
    }
}

// 0x001B18A0 - 0x001B190C (0x6C) zConditionalInit__FPvPv
void zConditionalInit(void *b, void *asset) {
    { // 0x001B18A0 - 0x001B190C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zLobMaster.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
signed int (ZLOB_elcb_event)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001B3B50
signed int (ZLOB_legacy_notify)(class zEntProjectile *, enum en_PROJECTILE_EVENT, void *); // 0x001B3890
class zGlobals globals; // 0x00362FD0
class _xVec3 g_Y3; // 0x00342E38
class st_LOBMASTER_QUEUE g_zlob_queue[5]; // 0x002B3E80
static class st_LOBMASTER_DEPOT g_zlob_depot; // 0x003575E8
static signed int g_zlob_init; // 0x00358298
// 0x001B1910 - 0x001B1A10 (0x100) LPRJ_debris_prjmove__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int LPRJ_debris_prjmove(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x001B1910 - 0x001B1A10
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r5
    }
}

// 0x001B1A10 - 0x001B1DF8 (0x3E8) LPRJ_debris_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int LPRJ_debris_logix(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x001B1A10 - 0x001B1DF8
        float elastic; // 
        class _xVec3 tmp_vec; // r29+0x60
        class _xVec3 vec_accum; // r29+0x50
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r16
    }
}

// 0x001B1E00 - 0x001B1E38 (0x38) LPRJ_debris_attach__FP14zEntProjectile
static signed int LPRJ_debris_attach(class zEntProjectile *prj) {
    { // 0x001B1E00 - 0x001B1E38
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r2
    }
}

// 0x001B1E40 - 0x001B2584 (0x744) ZLOB_randpos_fromBound__FP7_xBoundiiiP6_xVec3
static signed int ZLOB_randpos_fromBound(class _xBound *bnd, signed int flg_ropts, class _xVec3 *pos_launch) {
    { // 0x001B1E40 - 0x001B2584
        signed int result; // r18
        float rad; // 
        float hyt; // 
        class _xVec3 rot_range; // r29+0xe0
        class _xMat3x3 mat_rot; // r29+0x80
        class _xVec3 dirvec; // r29+0xd0
        class _xVec3 vec_tmp; // r29+0xc0
        class _xVec3 delta; // r29+0xb0
    }
}

// 0x001B2590 - 0x001B25F0 (0x60) ZLOB_find_lobber__FUi
static class st_LOBMASTER_DATA *ZLOB_find_lobber(unsigned int aid_lobm) {
    { // 0x001B2590 - 0x001B25F0
        signed int i; // r5
        class st_LOBMASTER_DEPOT *depot; // r2
        class st_LOBMASTER_DATA *lob; // r2
        class st_LOBMASTER_DATA *tmp_lob; // r3
    }
}

// 0x001B25F0 - 0x001B267C (0x8C) ZLOB_grab_nextBull__FP17st_LOBMASTER_DATA
static void ZLOB_grab_nextBull(class st_LOBMASTER_DATA *lob) {
    { // 0x001B25F0 - 0x001B267C
        class _xVec3 hdng; // r29+0x20
        class _zMovePoint *nav_tmp; // r29+0x2c
    }
}

// 0x001B2680 - 0x001B2D58 (0x6D8) ZLOB_shoot_debris__FP17st_LOBMASTER_DATAii
static signed int ZLOB_shoot_debris(class st_LOBMASTER_DATA *lob, signed int nth, signed int total) {
    { // 0x001B2680 - 0x001B2D58
        class _xMat3x3 mat_rot; // r29+0xa0
        class _xVec3 ang; // r29+0x108
        class _xVec3 rot_tumble; // r29+0xf8
        class _xVec3 dir_launch; // r29+0xe8
        class _xVec3 pos_launch; // r29+0xd8
        float spd_launch; // 
        float spd_base; // 
        signed int flg_player; // r17
        float ang_cone; // 
        float fval; // r29+0x120
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r18
        unsigned int prjTypeID; // r29+0x118
        class zEntProjectile *prj; // r16
        signed int result; // r2
    }
}

// 0x001B2D60 - 0x001B30A8 (0x348) ZLOB_shoot_arrow__FP17st_LOBMASTER_DATAii
static signed int ZLOB_shoot_arrow(class st_LOBMASTER_DATA *lob) {
    { // 0x001B2D60 - 0x001B30A8
        class _xVec3 pos_tgt; // r29+0x60
        class _xVec3 dir_launch; // r29+0x50
        class _xVec3 pos_launch; // r29+0x40
        float tmp_rand; // r29+0x80
        float spd_randPct; // r29+0x78
        float spd_launch; // 
        float spd_base; // r29+0x74
        unsigned int prjTypeID; // r29+0x70
        class zEntProjectile *prj; // r16
        signed int result; // r2
    }
}

// 0x001B30B0 - 0x001B3888 (0x7D8) ZLOB_shoot_legacy__FP17st_LOBMASTER_DATAii
static signed int ZLOB_shoot_legacy(class st_LOBMASTER_DATA *lob) {
    { // 0x001B30B0 - 0x001B3888
        class _xMat3x3 mat_rot; // r29+0xf0
        class _xVec3 ang; // r29+0x168
        float spd_base; // 
        class st_CAT_AIMGRAV *grav; // r18
        class ST_CAT_AIMDATA aim; // r29+0xb0
        float fac_arc; // r29+0x188
        class _xVec3 pos_tgt; // r29+0x158
        class _xVec3 dir_launch; // r29+0x148
        class _xVec3 pos_launch; // r29+0x138
        float tmp_rand; // 
        float spd_randPct; // r29+0x184
        float spd_launch; // r29+0x180
        class _xVec3 rot_tumble; // r29+0x128
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r17
        float fval; // r29+0x17c
        unsigned int prjTypeID; // r29+0x178
        class zEntProjectile *prj; // r16
        signed int result; // r2
    }
}

// 0x001B3890 - 0x001B3918 (0x88) ZLOB_legacy_notify__FP14zEntProjectile19en_PROJECTILE_EVENTPv
static signed int ZLOB_legacy_notify(class zEntProjectile *prj, enum en_PROJECTILE_EVENT event) {
    { // 0x001B3890 - 0x001B3918
        float fval; // r29+0x2c
        class st_LOBMASTER_DATA *lob; // r3
    }
}

// 0x001B3920 - 0x001B39EC (0xCC) ZLOB_shoot_one__FP17st_LOBMASTER_DATAii
static signed int ZLOB_shoot_one(class st_LOBMASTER_DATA *lob, signed int nth, signed int total) {
    { // 0x001B3920 - 0x001B39EC
        enum en_LOB_MASTER_TYPE type; // r29+0x4c
    }
}

// 0x001B39F0 - 0x001B3B44 (0x154) ZLOB_doLaunch__FP17st_LOBMASTER_DATA
static signed int ZLOB_doLaunch(class st_LOBMASTER_DATA *lob) {
    { // 0x001B39F0 - 0x001B3B44
        enum en_LOB_MASTER_TYPE type; // r29+0x58
        signed int salvo; // r29+0x54
        signed int cnt; // r17
        signed int rc; // r2
        signed int i; // r16
    }
}

// 0x001B3B50 - 0x001B3DC8 (0x278) ZLOB_elcb_event__FP5xBaseP5xBaseUiPfP5xBase
static signed int ZLOB_elcb_event(class xBase *from, class xBase *xb, unsigned int toEvent) {
    { // 0x001B3B50 - 0x001B3DC8
        signed int do_fromWidge; // r17
        class st_LOBMASTER_DATA *lob; // r16
    }
}

// 0x001B3DD0 - 0x001B40CC (0x2FC) LCFG_getParam__FP19st_LOBMASTER_CONFIG20en_LOBMASTER_PARAMIDPv
static signed int LCFG_getParam(class st_LOBMASTER_CONFIG *cfg, enum en_LOBMASTER_PARAMID pid, void *vv) {
    { // 0x001B3DD0 - 0x001B40CC
        signed int iv; // r4
        signed int found; // r16
        class zLobMasterAsset *lmass; // r2
    }
}

// 0x001B40D0 - 0x001B41BC (0xEC) ZLOB_HEB_to_DIR__FP6_xVec3P6_xVec3
static void ZLOB_HEB_to_DIR(class _xVec3 *dir_result, class _xVec3 *rot_HPB) {
    { // 0x001B40D0 - 0x001B41BC
        class _xMat3x3 mat_rot; // r29+0x30
        class _xVec3 vec_aim; // r29+0x60
    }
}

// 0x001B41C0 - 0x001B42F0 (0x130) zLobMaster_timestep__FP6zScenef
void zLobMaster_timestep(float dt) {
    { // 0x001B41C0 - 0x001B42F0
        signed int i; // r16
        class st_LOBMASTER_DATA *lobber; // r2
    }
}

// 0x001B42F0 - 0x001B43DC (0xEC) zLobMaster_InitDep__FP17st_LOBMASTER_DATAP6zScene
void zLobMaster_InitDep(class st_LOBMASTER_DATA *lob) {
    { // 0x001B42F0 - 0x001B43DC
    }
}

// 0x001B43E0 - 0x001B44A0 (0xC0) zLobMaster_Init__FP17st_LOBMASTER_DATAP11p2BaseAsset
void zLobMaster_Init(class st_LOBMASTER_DATA *lob, class p2BaseAsset *basset) {
    { // 0x001B43E0 - 0x001B44A0
        class zLobMasterAsset *tmp_rawass; // r16
    }
}

// 0x001B44A0 - 0x001B44B4 (0x14) zLobMaster_getInst__FP17st_LOBMASTER_DATAi
class st_LOBMASTER_DATA *zLobMaster_getInst(class st_LOBMASTER_DATA *pool, signed int idx) {
    { // 0x001B44A0 - 0x001B44B4
    }
}

// 0x001B44C0 - 0x001B4540 (0x80) zLobMaster_memPool__Fi
class st_LOBMASTER_DATA *zLobMaster_memPool(signed int cnt) {
    { // 0x001B44C0 - 0x001B4540
        class st_LOBMASTER_DEPOT *depot; // r17
        class st_LOBMASTER_DATA *pool; // r16
    }
}

// 0x001B4540 - 0x001B4574 (0x34) zLobMaster_sceneFinish__Fv
void zLobMaster_sceneFinish() {
    { // 0x001B4540 - 0x001B4574
        class st_LOBMASTER_DEPOT *depot; // r2
    }
}

// 0x001B4580 - 0x001B45D4 (0x54) zLobMaster_scenePrepare__Fv
void zLobMaster_scenePrepare() {
    { // 0x001B4580 - 0x001B45D4
        class st_LOBMASTER_DEPOT *depot; // r16
    }
}

// 0x001B45E0 - 0x001B45E8 (0x8) zLobMaster_Shutdown__Fv
void zLobMaster_Shutdown() {
    { // 0x001B45E0 - 0x001B45E8
    }
}

// 0x001B45F0 - 0x001B468C (0x9C) zLobMaster_Startup__Fv
void zLobMaster_Startup() {
    { // 0x001B45F0 - 0x001B468C
        class st_ZPRJ_CATEGORY_PROPS prop_bang; // r29+0x50
        class st_ZPRJ_CATEGORY_PROPS prop_debris; // r29+0x40
        class st_ZPRJ_CATEGORY_FUNCS fmap_debris; // r29+0x10
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntShaggy.cpp
// ==========================================================================
unsigned int (Shaggy1_ThrowEndCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2E20
unsigned int (Shaggy1_EndSetDownCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2440
unsigned int (Shaggy1_Turn)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C23C0
unsigned int (Shaggy1_TurnCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2260
unsigned int (Shaggy1_Land2)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2D00
unsigned int (Shaggy1_Fall)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2BE0
unsigned int (Shaggy1_FallCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2B50
unsigned int (Shaggy1_CatchCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2C10
unsigned int (Shaggy1_LetGo)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2980
unsigned int (Shaggy1_ScareCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2790
unsigned int (Shaggy1_PlyrSet)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C24F0
unsigned int (Shaggy1_Noturn)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C21E0
unsigned int (Shaggy1_LetGoShark)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2900
unsigned int (Shaggy1_LetGoCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2870
unsigned int (Shaggy1_HangSwing)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2AC0
unsigned int (Shaggy1_HangNoSwing)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2A50
unsigned int (Shaggy1_TalkChoose)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1EE0
unsigned int (Shaggy1_Throw)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2E90
unsigned int (Shaggy1_Land)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2D80
unsigned int (Shaggy1_Catch)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2C90
unsigned int (Shaggy1_Hang)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2B30
unsigned int (Shaggy1_SetDown)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2540
unsigned int (Shaggy1_GrabCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2F50
unsigned int (Shaggy1_ThrowCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2E40
unsigned int (Shaggy0_PlayerLanded)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1A30
unsigned int (Shaggy0_PlayerCaught)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1940
unsigned int (Shaggy1_EventCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2580
unsigned int (Shaggy1_TalkFlag)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1FC0
unsigned int (Shaggy1_TalkChooseCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1DC0
unsigned int (Shaggy1_NotalkCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1C80
unsigned int (Shaggy1_LandCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2CE0
unsigned int (Shaggy1_FallDeath)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2160
unsigned int (Shaggy1_FallDeathCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2140
unsigned int (Shaggy1_Scare)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2800
unsigned int (Shaggy0_CarryHitCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C18C0
unsigned int (Shaggy0_FlipCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C19A0
unsigned int (Shaggy1_Grab)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2F90
unsigned int (Shaggy1_Walk)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2F00
unsigned int (Shaggy1_Stop)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2EB0
unsigned int (Shaggy0_Throw)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1BD0
unsigned int (Shaggy0_Catch)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1BC0
unsigned int (Shaggy0_CatchCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1BB0
unsigned int (Shaggy2_Apex)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001BBA30
unsigned int (Shaggy2_RandCheck)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001BBA10
unsigned int (Shaggy2_StateChange)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001BBAD0
unsigned int (Shaggy3_Move)(class xAnimTransition *, class xAnimSingle *, class _zPlatform *); // 0x001BADF0
unsigned int (Shaggy3_Stop)(class xAnimTransition *, class xAnimSingle *, class _zPlatform *); // 0x001BACB0
unsigned int (Shaggy3_StopScare)(class xAnimTransition *, class xAnimSingle *, class _zPlatform *); // 0x001BAC50
unsigned int (Shaggy3_Left)(class xAnimTransition *, class xAnimSingle *, class _zPlatform *); // 0x001BABE0
unsigned int (Shaggy3_Right)(class xAnimTransition *, class xAnimSingle *, class _zPlatform *); // 0x001BAB70
unsigned int (Shaggy3_Center)(class xAnimTransition *, class xAnimSingle *, class _zPlatform *); // 0x001BAB00
unsigned int (Shaggy1_Talk)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C20C0
unsigned int (Shaggy1_Notalk)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C1D40
unsigned int (Shaggy3_MoveDead)(class xAnimTransition *, class xAnimSingle *, class _zPlatform *); // 0x001BAD60
unsigned int (Shaggy3_MowerStopCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x001BAAD0
unsigned int (Witch_HasShaggy)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B98F0
unsigned int (Witch_GrabShaggyCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B97D0
unsigned int (Witch_DropShaggyIdle)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B97F0
unsigned int (Witch_DropShaggyMove)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9870
unsigned int (Witch_DropShaggyCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B97B0
unsigned int (Shaggy5_WitchAttached)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9DE0
unsigned int (Shaggy5_WitchDropped)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9D80
unsigned int (Shaggy5_Action)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9D10
unsigned int (Shaggy5_EatMoveCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9C40
unsigned int (Shaggy5_EatStop)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9B90
unsigned int (Shaggy5_EatStopCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9B20
unsigned int (Shaggy5_EatTimeUp)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9AA0
unsigned int (Shaggy5_IdleTran)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9E40
unsigned int (Shaggy1_IdleLeave)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C25B0
unsigned int (Shaggy5_GargRunPick)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9940
unsigned int (Shaggy1_ScareReact)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C21B0
unsigned int (Shaggy1_DeathReact)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2190
unsigned int (Shaggy8_FlipOver)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B73B0
unsigned int (Shaggy8_FlipOverCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B7350
unsigned int (Shaggy8_FlipBack)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B72D0
unsigned int (Shaggy8_FlipBackCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B7270
unsigned int (Shaggy8_Crash)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B71E0
unsigned int (Shaggy5_ActionCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9CB0
unsigned int (Shaggy8_Getup)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B70A0
unsigned int (Shaggy8_Bounce)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B7180
unsigned int (Shaggy8_BounceCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B7120
unsigned int (Shaggy5_MoveTran)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B9EE0
unsigned int (Shaggy8_Stopped)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001B7240
unsigned int (Shaggy1_IdlePick)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2610
unsigned int (Shaggy1_IdleResetCB)(class xAnimTransition *, class xAnimSingle *, class _zEnt *); // 0x001C2730
unsigned int (Shaggy8_BarrelDestroy)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x001B7030
void (Shaggy1_Update)(class _zEnt *, class _xScene *, float); // 0x001BDC50
void (Shaggy1_Move)(class _xEnt *, class _xScene *, float, class _xEntFrame *); // 0x001BD1A0
signed int (Shaggy0_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001BBB80
signed int (Shaggy1_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001BBC40
void (Shaggy5_Update)(class _zEnt *, class _xScene *, float); // 0x001B7B10
void (Shaggy5_Move)(class _xEnt *, class _xScene *, float, class _xEntFrame *); // 0x001B7FB0
signed int (Shaggy5_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001B7430
void (Shaggy8_Update)(class _zEnt *, class _xScene *, float); // 0x001B61D0
signed int (Shaggy8_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001B5E60
void (Shaggy3_Update)(class _zEnt *, class _xScene *, float); // 0x001BA220
signed int (Shaggy3_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001B9F80
void (Shaggy2_Update)(class _zEnt *, class _xScene *, float); // 0x001BB050
void (Shaggy2_Render)(class _xEnt *); // 0x001BAE60
signed int (Shaggy2_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001BAF60
signed int (Shaggy2Plat_EventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001BAEA0
class zGlobals globals; // 0x00362FD0
static signed short sSndCallPitch; // 0x003582CC
static class ShaggyMagnet sMagnetList[64]; // 0x003A6950
static signed int sMagnetCount; // 0x003582C8
static class iModelTag ShaggyTag; // 0x003A6920
static unsigned int ShaggyTagIndex; // 0x003575FC
static class _xVec3 ShaggyTagPoint; // 0x00000000
static class iModelTag ChimneyTag; // 0x003A6900
static unsigned int ChimneyTagIndex; // 0x003575F8
static class _xVec3 ChimneyTagPoint; // 0x00000000
float sDefHangTime; // 0x003582C4
float sHangVirtualPeak; // 0x003582C0
float THROW_DEFHEIGHT; // 0x003575F4
float THROW_MINHEIGHT; // 0x003575F0
float gACCEL_END; // 0x003582BC
float gACCEL_START; // 0x003582B8
static void (*sPlatMoveFunc)(class _xEnt *, class _xScene *, float, class _xEntFrame *); // 0x00000000
static void (*sVilMoveFunc)(class _xEnt *, class _xScene *, float, class _xEntFrame *); // 0x003582B4
static void (*sVilUpdateFunc)(class _xEnt *, class _xScene *, float); // 0x003582B0
static signed int (*sVilEventFunc)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x003582AC
static class _zParEmitter *gBurningManEmitter; // 0x003582A8
static class _zParEmitter *gTractorEmitter; // 0x003582A4
static float gShaggy3TurnAngle; // 0x003582A0
class ShaggyGuts gShaggy[16]; // 0x003A5D80
unsigned int gNumShaggy; // 0x0035829C
// 0x001B4690 - 0x001B4A34 (0x3A4) zEntShaggy_SceneReset__Fv
void zEntShaggy_SceneReset() {
    { // 0x001B4690 - 0x001B4A34
        class xModelInstance *minst; // r4
        class xModelInstance *minst; // r3
        unsigned int i; // r20
    }
}

// 0x001B4A40 - 0x001B5800 (0xDC0) zEntShaggy_SceneStart__FP6zScene
void zEntShaggy_SceneStart(class zScene *sc) {
    { // 0x001B4A40 - 0x001B5800
        class xModelInstance *tmpinst; // r3
        class _zPlatform *plat; // r3
        class xModelInstance *minst; // r3
        class xModelInstance *newminst; // r21
        class RpAtomic *data1; // r3
        class xModelInstance *inst2; // r21
        class xModelInstance *inst; // r20
        unsigned int hangHashID; // r30
        unsigned int j; // r20
        unsigned int i; // r22
    }
}

// 0x001B5800 - 0x001B598C (0x18C) zEntShaggy_Init__FP14st_zEntVillain
void zEntShaggy_Init(class st_zEntVillain *ent) {
    { // 0x001B5800 - 0x001B598C
        unsigned int i; // r5
        class _zEnt *shaggy; // r2
    }
}

// 0x001B5990 - 0x001B5A68 (0xD8) zEntShaggy_UpdateDep__Fv
void zEntShaggy_UpdateDep() {
    { // 0x001B5990 - 0x001B5A68
        signed int i; // r5
    }
}

// 0x001B5A70 - 0x001B5E60 (0x3F0) Shaggy8_CallToPlayer__Fv
signed int Shaggy8_CallToPlayer() {
    { // 0x001B5A70 - 0x001B5E60
        float oldZ; // 
        float oldX; // 
        float dummyParam[4]; // r29+0xc0
        float tandist; // r29+0xe0
        float normdist; // 
        float mz; // 
        float mx; // 
        float bestmagdist; // 
        float magrad; // 
        class _xVec3 *magvec; // r19
        signed int bestmag; // r18
        signed int magnum; // r17
        float pitch; // r29+0xdc
        float mag; // r29+0xe0
        float rayz; // 
        float rayx; // 
        class _zEnt *plyr; // r23
        class _zEnt *ent; // r2
    }
}

// 0x001B5E60 - 0x001B61CC (0x36C) Shaggy8_EventCB__FP5xBaseP5xBaseUiPfP5xBase
static signed int Shaggy8_EventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001B5E60 - 0x001B61CC
        signed int magnum; // r6
        signed int magnum; // r3
        class _zEnt *ent; // r2
        signed int retval; // r16
    }
}

// 0x001B61D0 - 0x001B6418 (0x248) Shaggy8_Update__FP5_zEntP7_xScenef
static void Shaggy8_Update(class _zEnt *ent, class _xScene *sc, float dt) {
    { // 0x001B61D0 - 0x001B6418
        signed int cnum; // r16
        class xAnimState *astate; // r3
    }
}

// 0x001B6420 - 0x001B654C (0x12C) Shaggy8_BBCallback__FP5_zEntP8_xCollis
signed int Shaggy8_BBCallback(class _xCollis *coll) {
    { // 0x001B6420 - 0x001B654C
        class _zEnt *ent; // r16
    }
}

// 0x001B6550 - 0x001B669C (0x14C) Shaggy8_HBCallback__FP5_zEntP8_xCollis
signed int Shaggy8_HBCallback(class _zEnt *plyr, class _xCollis *coll) {
    { // 0x001B6550 - 0x001B669C
        class _zEnt *ent; // r16
    }
}

// 0x001B66A0 - 0x001B7028 (0x988) zEntShaggy_Anim8__Fv
class xAnimTable *zEntShaggy_Anim8() {
    { // 0x001B66A0 - 0x001B7028
        class xAnimTable *table; // r16
    }
}

// 0x001B7030 - 0x001B7098 (0x68) Shaggy8_BarrelDestroy__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int Shaggy8_BarrelDestroy(void *object) {
    { // 0x001B7030 - 0x001B7098
        class xModelInstance *inst; // r3
        class _zEnt *ent; // r2
    }
}

// 0x001B70A0 - 0x001B7120 (0x80) Shaggy8_Getup__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_Getup(class _zEnt *ent) {
    { // 0x001B70A0 - 0x001B7120
    }
}

// 0x001B7120 - 0x001B7180 (0x60) Shaggy8_BounceCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_BounceCB(class _zEnt *ent) {
    { // 0x001B7120 - 0x001B7180
    }
}

// 0x001B7180 - 0x001B71E0 (0x60) Shaggy8_Bounce__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_Bounce(class _zEnt *ent) {
    { // 0x001B7180 - 0x001B71E0
    }
}

// 0x001B71E0 - 0x001B7240 (0x60) Shaggy8_Crash__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_Crash(class _zEnt *ent) {
    { // 0x001B71E0 - 0x001B7240
    }
}

// 0x001B7240 - 0x001B7268 (0x28) Shaggy8_Stopped__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_Stopped(class _zEnt *ent) {
    { // 0x001B7240 - 0x001B7268
    }
}

// 0x001B7270 - 0x001B72D0 (0x60) Shaggy8_FlipBackCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_FlipBackCB(class _zEnt *ent) {
    { // 0x001B7270 - 0x001B72D0
    }
}

// 0x001B72D0 - 0x001B7348 (0x78) Shaggy8_FlipBack__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_FlipBack(class _zEnt *ent) {
    { // 0x001B72D0 - 0x001B7348
    }
}

// 0x001B7350 - 0x001B73B0 (0x60) Shaggy8_FlipOverCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_FlipOverCB(class _zEnt *ent) {
    { // 0x001B7350 - 0x001B73B0
    }
}

// 0x001B73B0 - 0x001B7428 (0x78) Shaggy8_FlipOver__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy8_FlipOver(class _zEnt *ent) {
    { // 0x001B73B0 - 0x001B7428
    }
}

// 0x001B7430 - 0x001B7708 (0x2D8) Shaggy5_EventCB__FP5xBaseP5xBaseUiPfP5xBase
static signed int Shaggy5_EventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001B7430 - 0x001B7708
        float tempToParam[4]; // r29+0x70
        class _zEnt *ent; // r2
        signed int retval; // r21
    }
}

// 0x001B7710 - 0x001B7B0C (0x3FC) ShaggySwap__FP5_zEntP5_zEnt
static void ShaggySwap(class _zEnt *src, class _zEnt *dest) {
    { // 0x001B7710 - 0x001B7B0C
        char *dstState; // r16
        char *srcState; // r2
    }
}

// 0x001B7B10 - 0x001B7FA4 (0x494) Shaggy5_Update__FP5_zEntP7_xScenef
static void Shaggy5_Update(class _zEnt *ent, class _xScene *sc, float dt) {
    { // 0x001B7B10 - 0x001B7FA4
        float dz; // 
        float dx; // 
        class _zParEmitterCustomSettings info; // r29+0x50
    }
}

// 0x001B7FB0 - 0x001B7FBC (0xC) Shaggy5_Move__FP5_xEntP7_xScenefP10_xEntFrame
static void Shaggy5_Move(class _xEnt *ent, class _xScene *s, float dt, class _xEntFrame *frame) {
    { // 0x001B7FB0 - 0x001B7FBC
    }
}

// 0x001B7FC0 - 0x001B95E4 (0x1624) zEntShaggy_Anim5__Fv
class xAnimTable *zEntShaggy_Anim5() {
    { // 0x001B7FC0 - 0x001B95E4
        class xAnimTable *table; // r16
        class xAnimTransition *tran; // r2
    }
}

// 0x001B95F0 - 0x001B97A8 (0x1B8) Shaggy5_WitchTransitions__FP10xAnimTable
void Shaggy5_WitchTransitions(class xAnimTable *table) {
    { // 0x001B95F0 - 0x001B97A8
        class xAnimTransition *tran; // r16
    }
}

// 0x001B97B0 - 0x001B97C8 (0x18) Witch_DropShaggyCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Witch_DropShaggyCB(class _zEnt *ent) {
    { // 0x001B97B0 - 0x001B97C8
    }
}

// 0x001B97D0 - 0x001B97E8 (0x18) Witch_GrabShaggyCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Witch_GrabShaggyCB(class _zEnt *ent) {
    { // 0x001B97D0 - 0x001B97E8
    }
}

// 0x001B97F0 - 0x001B986C (0x7C) Witch_DropShaggyIdle__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Witch_DropShaggyIdle(class _zEnt *ent) {
    { // 0x001B97F0 - 0x001B986C
    }
}

// 0x001B9870 - 0x001B98EC (0x7C) Witch_DropShaggyMove__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Witch_DropShaggyMove(class _zEnt *ent) {
    { // 0x001B9870 - 0x001B98EC
    }
}

// 0x001B98F0 - 0x001B9934 (0x44) Witch_HasShaggy__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Witch_HasShaggy(class _zEnt *ent) {
    { // 0x001B98F0 - 0x001B9934
        signed int i; // r3
    }
}

// 0x001B9940 - 0x001B9A94 (0x154) Shaggy5_GargRunPick__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_GargRunPick(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001B9940 - 0x001B9A94
    }
}

// 0x001B9AA0 - 0x001B9B1C (0x7C) Shaggy5_EatTimeUp__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_EatTimeUp(class _zEnt *ent) {
    { // 0x001B9AA0 - 0x001B9B1C
    }
}

// 0x001B9B20 - 0x001B9B88 (0x68) Shaggy5_EatStopCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_EatStopCB(class _zEnt *ent) {
    { // 0x001B9B20 - 0x001B9B88
    }
}

// 0x001B9B90 - 0x001B9C38 (0xA8) Shaggy5_EatStop__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_EatStop(class _zEnt *ent) {
    { // 0x001B9B90 - 0x001B9C38
    }
}

// 0x001B9C40 - 0x001B9CAC (0x6C) Shaggy5_EatMoveCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_EatMoveCB(class _zEnt *ent) {
    { // 0x001B9C40 - 0x001B9CAC
    }
}

// 0x001B9CB0 - 0x001B9D10 (0x60) Shaggy5_ActionCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_ActionCB(class _zEnt *ent) {
    { // 0x001B9CB0 - 0x001B9D10
    }
}

// 0x001B9D10 - 0x001B9D78 (0x68) Shaggy5_Action__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_Action(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001B9D10 - 0x001B9D78
    }
}

// 0x001B9D80 - 0x001B9DE0 (0x60) Shaggy5_WitchDropped__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_WitchDropped(class _zEnt *ent) {
    { // 0x001B9D80 - 0x001B9DE0
    }
}

// 0x001B9DE0 - 0x001B9E40 (0x60) Shaggy5_WitchAttached__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_WitchAttached(class _zEnt *ent) {
    { // 0x001B9DE0 - 0x001B9E40
    }
}

// 0x001B9E40 - 0x001B9EDC (0x9C) Shaggy5_IdleTran__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_IdleTran(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001B9E40 - 0x001B9EDC
    }
}

// 0x001B9EE0 - 0x001B9F7C (0x9C) Shaggy5_MoveTran__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy5_MoveTran(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001B9EE0 - 0x001B9F7C
    }
}

// 0x001B9F80 - 0x001BA218 (0x298) Shaggy3_EventCB__FP5xBaseP5xBaseUiPfP5xBase
static signed int Shaggy3_EventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001B9F80 - 0x001BA218
        class xAnimState *astate; // r21
        class _zPlatform *t; // r2
    }
}

// 0x001BA220 - 0x001BA588 (0x368) Shaggy3_Update__FP5_zEntP7_xScenef
static void Shaggy3_Update(class _zEnt *ent, class _xScene *sc, float dt) {
    { // 0x001BA220 - 0x001BA588
        float dz; // r29+0xd0
        float dx; // 
        class xAnimState *astate; // r20
        class _zParEmitterCustomSettings info; // r29+0x70
    }
}

// 0x001BA590 - 0x001BAAC4 (0x534) zEntShaggy_Anim3__Fv
class xAnimTable *zEntShaggy_Anim3() {
    { // 0x001BA590 - 0x001BAAC4
        class xAnimTable *table; // r16
    }
}

// 0x001BAAD0 - 0x001BAB00 (0x30) Shaggy3_MowerStopCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int Shaggy3_MowerStopCB(void *object) {
    { // 0x001BAAD0 - 0x001BAB00
    }
}

// 0x001BAB00 - 0x001BAB68 (0x68) Shaggy3_Center__FP15xAnimTransitionP11xAnimSingleP10_zPlatform
static unsigned int Shaggy3_Center(class xAnimSingle *anim) {
    { // 0x001BAB00 - 0x001BAB68
    }
}

// 0x001BAB70 - 0x001BABDC (0x6C) Shaggy3_Right__FP15xAnimTransitionP11xAnimSingleP10_zPlatform
static unsigned int Shaggy3_Right(class xAnimSingle *anim) {
    { // 0x001BAB70 - 0x001BABDC
    }
}

// 0x001BABE0 - 0x001BAC4C (0x6C) Shaggy3_Left__FP15xAnimTransitionP11xAnimSingleP10_zPlatform
static unsigned int Shaggy3_Left(class xAnimSingle *anim) {
    { // 0x001BABE0 - 0x001BAC4C
    }
}

// 0x001BAC50 - 0x001BACAC (0x5C) Shaggy3_StopScare__FP15xAnimTransitionP11xAnimSingleP10_zPlatform
static unsigned int Shaggy3_StopScare(class _zPlatform *ent) {
    { // 0x001BAC50 - 0x001BACAC
    }
}

// 0x001BACB0 - 0x001BAD58 (0xA8) Shaggy3_Stop__FP15xAnimTransitionP11xAnimSingleP10_zPlatform
static unsigned int Shaggy3_Stop(class _zPlatform *ent) {
    { // 0x001BACB0 - 0x001BAD58
    }
}

// 0x001BAD60 - 0x001BADE4 (0x84) Shaggy3_MoveDead__FP15xAnimTransitionP11xAnimSingleP10_zPlatform
static unsigned int Shaggy3_MoveDead(class _zPlatform *ent) {
    { // 0x001BAD60 - 0x001BADE4
    }
}

// 0x001BADF0 - 0x001BAE5C (0x6C) Shaggy3_Move__FP15xAnimTransitionP11xAnimSingleP10_zPlatform
static unsigned int Shaggy3_Move(class _zPlatform *ent) {
    { // 0x001BADF0 - 0x001BAE5C
    }
}

// 0x001BAE60 - 0x001BAE94 (0x34) Shaggy2_Render__FP5_xEnt
static void Shaggy2_Render(class _xEnt *ent) {
    { // 0x001BAE60 - 0x001BAE94
    }
}

// 0x001BAEA0 - 0x001BAF5C (0xBC) Shaggy2Plat_EventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int Shaggy2Plat_EventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001BAEA0 - 0x001BAF5C
        unsigned int i; // r10
    }
}

// 0x001BAF60 - 0x001BB048 (0xE8) Shaggy2_EventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int Shaggy2_EventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001BAF60 - 0x001BB048
        signed int rval; // r16
    }
}

// 0x001BB050 - 0x001BB578 (0x528) Shaggy2_Update__FP5_zEntP7_xScenef
static void Shaggy2_Update(class _zEnt *ent, class _xScene *sc, float dt) {
    { // 0x001BB050 - 0x001BB578
        class _xVec3 shagVec; // r29+0xc0
        class _xVec3 chimVec; // r29+0xb0
        float hdiff; // 
        class _xVec3 *shaggyvec; // r20
        class xModelInstance *shagmodel; // r19
        class xModelInstance *chimmodel; // r18
        class xModelInstance *collmodel; // r23
        float oldvel; // r29+0xd0
        unsigned int i; // r3
    }
}

// 0x001BB580 - 0x001BB70C (0x18C) zEntShaggy_Chimney__Fv
class xAnimTable *zEntShaggy_Chimney() {
    { // 0x001BB580 - 0x001BB70C
        class xAnimTable *table; // r16
    }
}

// 0x001BB710 - 0x001BBA08 (0x2F8) zEntShaggy_Anim2__Fv
class xAnimTable *zEntShaggy_Anim2() {
    { // 0x001BB710 - 0x001BBA08
        class xAnimTable *table; // r16
    }
}

// 0x001BBA10 - 0x001BBA30 (0x20) Shaggy2_RandCheck__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy2_RandCheck() {
    { // 0x001BBA10 - 0x001BBA30
    }
}

// 0x001BBA30 - 0x001BBAC4 (0x94) Shaggy2_Apex__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy2_Apex(class _zEnt *ent) {
    { // 0x001BBA30 - 0x001BBAC4
    }
}

// 0x001BBAD0 - 0x001BBB78 (0xA8) Shaggy2_StateChange__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy2_StateChange(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001BBAD0 - 0x001BBB78
    }
}

// 0x001BBB80 - 0x001BBC38 (0xB8) Shaggy0_EventCB__FP5xBaseP5xBaseUiPfP5xBase
static signed int Shaggy0_EventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001BBB80 - 0x001BBC38
        signed int retval; // r16
    }
}

// 0x001BBC40 - 0x001BBF88 (0x348) Shaggy1_EventCB__FP5xBaseP5xBaseUiPfP5xBase
static signed int Shaggy1_EventCB(class xBase *from, class xBase *to, unsigned int toEvent, float *toParam, class xBase *toParamWidget) {
    { // 0x001BBC40 - 0x001BBF88
        signed int retval; // r16
    }
}

// 0x001BBF90 - 0x001BC12C (0x19C) ShaggyVil_HandleVisible__FP5_zEntUi
static void ShaggyVil_HandleVisible(class _zEnt *ent, unsigned int toEvent) {
    { // 0x001BBF90 - 0x001BC12C
    }
}

// 0x001BC130 - 0x001BC744 (0x614) Shaggy1_ChooseThrowTarget__FP5_zEnt
void Shaggy1_ChooseThrowTarget(class _zEnt *ent) {
    { // 0x001BC130 - 0x001BC744
        class _zEntHangable *hang; // r19
        float hangtime; // 
        float dropheight; // 
        float throwdist; // r29+0x90
        float dz; // 
        float dx; // 
        class _xVec3 nearpt; // r29+0x80
        float timepeak; // r29+0x8c
        float mag; // 
        float dot; // 
        float dz; // r29+0x90
        float dx; // 
        unsigned int i; // r17
    }
}

// 0x001BC750 - 0x001BC860 (0x110) Shaggy1_CalcThrowAccel__FfPf
static float Shaggy1_CalcThrowAccel(float peak, float *timepeak) {
    { // 0x001BC750 - 0x001BC860
        float sol[2]; // r29+0x48
        float Vthrow; // 
        float Ta; // 
        unsigned int solcnt; // r2
    }
}

// 0x001BC860 - 0x001BCC7C (0x41C) NearestPointObb2D__FP6_xVec3P6_xVec3P5_xBoxP8_xMat4x3
static void NearestPointObb2D(class _xVec3 *near, class _xVec3 *vec, class _xBox *b, class _xMat4x3 *m) {
    { // 0x001BC860 - 0x001BCC7C
        class _xVec3 *v2; // r18
        float testdist2; // 
        float lerp; // 
        float ddot; // r29+0xe0
        float pdot; // 
        float dz; // 
        float dx; // 
        float planeD; // 
        class _xVec3 corner[4]; // r29+0xb0
        signed int i; // r16
    }
}

// 0x001BCC80 - 0x001BCD88 (0x108) PointInQuadXZ__FP6_xVec3P6_xVec3
static signed int PointInQuadXZ(class _xVec3 *quad, class _xVec3 *vec) {
    { // 0x001BCC80 - 0x001BCD88
        float fDz; // 
        float fDx; // 
        float fNz; // 
        float fNx; // r29+0x0
    }
}

// 0x001BCD90 - 0x001BCE54 (0xC4) NearestPointSphere2D__FP6_xVec3P6_xVec3P6_xVec3f
static void NearestPointSphere2D(class _xVec3 *near, class _xVec3 *vec, class _xVec3 *center, float radius) {
    { // 0x001BCD90 - 0x001BCE54
        float magmul; // 
        float mag2; // r29+0x40
        float dz; // 
        float dx; // 
    }
}

// 0x001BCE60 - 0x001BCEF0 (0x90) NearestPointBox2D__FP6_xVec3P6_xVec3P6_xVec3P6_xVec3
static void NearestPointBox2D(class _xVec3 *near, class _xVec3 *vec, class _xVec3 *lower, class _xVec3 *upper) {
    { // 0x001BCE60 - 0x001BCEF0
    }
}

// 0x001BCEF0 - 0x001BD19C (0x2AC) Shaggy1_PlayerUpdate__FP5_zEntP7_xScenef
void Shaggy1_PlayerUpdate(class _zEnt *shag, class _xScene *sc, float dt) {
    { // 0x001BCEF0 - 0x001BD19C
        class xAnimState *astate; // r5
    }
}

// 0x001BD1A0 - 0x001BDC44 (0xAA4) Shaggy1_Move__FP5_xEntP7_xScenefP10_xEntFrame
static void Shaggy1_Move(class _xEnt *ent, class _xScene *s, float dt, class _xEntFrame *frame) {
    { // 0x001BD1A0 - 0x001BDC44
        float idealz; // 
        float idealx; // 
        unsigned int solcnt; // r2
        float sol[2]; // r29+0xb8
        float b; // r29+0xe0
        float a; // 
        float curheight; // 
        class _zEntHangable *hang; // r2
        float newTransY; // 
        float falltime; // 
        float falldist; // 
        class xAnimState *astate; // r21
        class xAnimPlay *play; // r16
        signed int retnow; // r3
    }
}

// 0x001BDC50 - 0x001BE234 (0x5E4) Shaggy1_Update__FP5_zEntP7_xScenef
static void Shaggy1_Update(class _zEnt *ent, class _xScene *sc, float dt) {
    { // 0x001BDC50 - 0x001BE234
        float dz; // 
        float dx; // 
        class _xMat3x3 *mat; // r17
        float mag; // 
        class _xVec3 *pos; // r2
        class _zEntPickup *pup; // r2
        class zScene *zsc; // r2
        float cz; // 
        float cy; // 
        float cx; // 
        unsigned int i; // r7
    }
}

// 0x001BE240 - 0x001BE44C (0x20C) Shaggy1_MiddleUpdate__FP5_zEntP7_xScenef
static void Shaggy1_MiddleUpdate(class _zEnt *shag, class _xScene *sc, float dt) {
    { // 0x001BE240 - 0x001BE44C
        float dz; // 
        float dx; // 
        float oldgravity; // 
        class xAnimState *plyrstate; // r17
        class xAnimState *astate; // r16
    }
}

// 0x001BE450 - 0x001BECD8 (0x888) Shaggy1_BonusAnims__FP5_zEnt
static void Shaggy1_BonusAnims(class _zEnt *ent) {
    { // 0x001BE450 - 0x001BECD8
        float temptran[3]; // r29+0xf0
        class _xVec3 vb; // r29+0xe0
        class _xVec3 va; // r29+0xd0
        void *hangswing_animdata; // r23
        void *hang_animdata; // r30
        class xAnimState *plyrstate; // r4
        class xAnimTable *plyrtbl; // r20
        class xAnimState *state; // r19
        class _xQuat tempquat; // r29+0xa0
        float shark_tranadjust[3]; // r29+0xc0
        float catch_tranadjust[3]; // r29+0xb0
        unsigned int rootbone; // r18
    }
}

// 0x001BECE0 - 0x001BED3C (0x5C) Shaggy1_SetHangable__FP5_zEntP13_zEntHangable
void Shaggy1_SetHangable(class _zEnt *ent, class _zEntHangable *hang) {
    { // 0x001BECE0 - 0x001BED3C
    }
}

// 0x001BED40 - 0x001BEFBC (0x27C) Shaggy0_PlayerTransitions__FP10xAnimTable
void Shaggy0_PlayerTransitions(class xAnimTable *table) {
    { // 0x001BED40 - 0x001BEFBC
        class xAnimTransition *tran; // r17
    }
}

// 0x001BEFC0 - 0x001BFCB4 (0xCF4) zEntShaggy_Anim0__Fv
class xAnimTable *zEntShaggy_Anim0() {
    { // 0x001BEFC0 - 0x001BFCB4
        class xAnimTable *table; // r16
    }
}

// 0x001BFCC0 - 0x001BFDFC (0x13C) Shaggy0_PlayerStates__FP10xAnimTable
void Shaggy0_PlayerStates(class xAnimTable *table) {
    { // 0x001BFCC0 - 0x001BFDFC
    }
}

// 0x001BFE00 - 0x001C0184 (0x384) Shaggy1_PlayerTransitions__FP10xAnimTable
void Shaggy1_PlayerTransitions(class xAnimTable *table) {
    { // 0x001BFE00 - 0x001C0184
    }
}

// 0x001C0190 - 0x001C16B8 (0x1528) zEntShaggy_Anim1__Fv
class xAnimTable *zEntShaggy_Anim1() {
    { // 0x001C0190 - 0x001C16B8
        class xAnimTable *table; // r16
    }
}

// 0x001C16C0 - 0x001C18BC (0x1FC) Shaggy1_PlayerStates__FP10xAnimTable
void Shaggy1_PlayerStates(class xAnimTable *table) {
    { // 0x001C16C0 - 0x001C18BC
    }
}

// 0x001C18C0 - 0x001C1938 (0x78) Shaggy0_CarryHitCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy0_CarryHitCB(class _zEnt *ent) {
    { // 0x001C18C0 - 0x001C1938
    }
}

// 0x001C1940 - 0x001C199C (0x5C) Shaggy0_PlayerCaught__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy0_PlayerCaught(class _zEnt *ent) {
    { // 0x001C1940 - 0x001C199C
    }
}

// 0x001C19A0 - 0x001C1A24 (0x84) Shaggy0_FlipCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
unsigned int Shaggy0_FlipCB(class _zEnt *ent) {
    { // 0x001C19A0 - 0x001C1A24
    }
}

// 0x001C1A30 - 0x001C1BAC (0x17C) Shaggy0_PlayerLanded__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy0_PlayerLanded(class xAnimSingle *anim, class _zEnt *ent) {
    { // 0x001C1A30 - 0x001C1BAC
    }
}

// 0x001C1BB0 - 0x001C1BC0 (0x10) Shaggy0_CatchCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy0_CatchCB() {
    { // 0x001C1BB0 - 0x001C1BC0
    }
}

// 0x001C1BC0 - 0x001C1BCC (0xC) Shaggy0_Catch__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy0_Catch() {
    { // 0x001C1BC0 - 0x001C1BCC
    }
}

// 0x001C1BD0 - 0x001C1C80 (0xB0) Shaggy0_Throw__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy0_Throw(class _zEnt *ent) {
    { // 0x001C1BD0 - 0x001C1C80
    }
}

// 0x001C1C80 - 0x001C1D3C (0xBC) Shaggy1_NotalkCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_NotalkCB(class _zEnt *ent) {
    { // 0x001C1C80 - 0x001C1D3C
    }
}

// 0x001C1D40 - 0x001C1DBC (0x7C) Shaggy1_Notalk__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Notalk(class _zEnt *ent) {
    { // 0x001C1D40 - 0x001C1DBC
    }
}

// 0x001C1DC0 - 0x001C1ED4 (0x114) Shaggy1_TalkChooseCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_TalkChooseCB(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001C1DC0 - 0x001C1ED4
        unsigned int maxT; // r17
        unsigned int minT; // r16
    }
}

// 0x001C1EE0 - 0x001C1FB8 (0xD8) Shaggy1_TalkChoose__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_TalkChoose(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001C1EE0 - 0x001C1FB8
    }
}

// 0x001C1FC0 - 0x001C20BC (0xFC) Shaggy1_TalkFlag__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_TalkFlag(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001C1FC0 - 0x001C20BC
    }
}

// 0x001C20C0 - 0x001C213C (0x7C) Shaggy1_Talk__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Talk(class _zEnt *ent) {
    { // 0x001C20C0 - 0x001C213C
    }
}

// 0x001C2140 - 0x001C2160 (0x20) Shaggy1_FallDeathCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_FallDeathCB(class _zEnt *ent) {
    { // 0x001C2140 - 0x001C2160
    }
}

// 0x001C2160 - 0x001C218C (0x2C) Shaggy1_FallDeath__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_FallDeath() {
    { // 0x001C2160 - 0x001C218C
    }
}

// 0x001C2190 - 0x001C21A8 (0x18) Shaggy1_DeathReact__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_DeathReact() {
    { // 0x001C2190 - 0x001C21A8
    }
}

// 0x001C21B0 - 0x001C21DC (0x2C) Shaggy1_ScareReact__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_ScareReact() {
    { // 0x001C21B0 - 0x001C21DC
    }
}

// 0x001C21E0 - 0x001C225C (0x7C) Shaggy1_Noturn__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Noturn(class _zEnt *ent) {
    { // 0x001C21E0 - 0x001C225C
    }
}

// 0x001C2260 - 0x001C23C0 (0x160) Shaggy1_TurnCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_TurnCB(class _zEnt *ent) {
    { // 0x001C2260 - 0x001C23C0
        float angdst; // r29+0x40
        float angsrc; // 
    }
}

// 0x001C23C0 - 0x001C243C (0x7C) Shaggy1_Turn__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Turn(class xAnimSingle *anim, class _zEnt *ent) {
    { // 0x001C23C0 - 0x001C243C
    }
}

// 0x001C2440 - 0x001C24EC (0xAC) Shaggy1_EndSetDownCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_EndSetDownCB(class _zEnt *ent) {
    { // 0x001C2440 - 0x001C24EC
    }
}

// 0x001C24F0 - 0x001C2534 (0x44) Shaggy1_PlyrSet__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_PlyrSet() {
    { // 0x001C24F0 - 0x001C2534
    }
}

// 0x001C2540 - 0x001C2580 (0x40) Shaggy1_SetDown__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_SetDown() {
    { // 0x001C2540 - 0x001C2580
    }
}

// 0x001C2580 - 0x001C25A8 (0x28) Shaggy1_EventCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_EventCB(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001C2580 - 0x001C25A8
    }
}

// 0x001C25B0 - 0x001C2610 (0x60) Shaggy1_IdleLeave__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_IdleLeave(class _zEnt *ent) {
    { // 0x001C25B0 - 0x001C2610
    }
}

// 0x001C2610 - 0x001C272C (0x11C) Shaggy1_IdlePick__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_IdlePick(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001C2610 - 0x001C272C
    }
}

// 0x001C2730 - 0x001C2790 (0x60) Shaggy1_IdleResetCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_IdleResetCB(class _zEnt *ent) {
    { // 0x001C2730 - 0x001C2790
    }
}

// 0x001C2790 - 0x001C2800 (0x70) Shaggy1_ScareCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_ScareCB(class _zEnt *ent) {
    { // 0x001C2790 - 0x001C2800
    }
}

// 0x001C2800 - 0x001C286C (0x6C) Shaggy1_Scare__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Scare() {
    { // 0x001C2800 - 0x001C286C
    }
}

// 0x001C2870 - 0x001C2900 (0x90) Shaggy1_LetGoCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_LetGoCB(class _zEnt *ent) {
    { // 0x001C2870 - 0x001C2900
    }
}

// 0x001C2900 - 0x001C2980 (0x80) Shaggy1_LetGoShark__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_LetGoShark(class _zEnt *ent) {
    { // 0x001C2900 - 0x001C2980
    }
}

// 0x001C2980 - 0x001C2A48 (0xC8) Shaggy1_LetGo__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_LetGo(class _zEnt *ent) {
    { // 0x001C2980 - 0x001C2A48
    }
}

// 0x001C2A50 - 0x001C2ABC (0x6C) Shaggy1_HangNoSwing__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_HangNoSwing(class _zEnt *ent) {
    { // 0x001C2A50 - 0x001C2ABC
    }
}

// 0x001C2AC0 - 0x001C2B2C (0x6C) Shaggy1_HangSwing__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_HangSwing(class _zEnt *ent) {
    { // 0x001C2AC0 - 0x001C2B2C
    }
}

// 0x001C2B30 - 0x001C2B48 (0x18) Shaggy1_Hang__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Hang() {
    { // 0x001C2B30 - 0x001C2B48
    }
}

// 0x001C2B50 - 0x001C2BD8 (0x88) Shaggy1_FallCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_FallCB(class _zEnt *ent) {
    { // 0x001C2B50 - 0x001C2BD8
    }
}

// 0x001C2BE0 - 0x001C2C08 (0x28) Shaggy1_Fall__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Fall(class _zEnt *ent) {
    { // 0x001C2BE0 - 0x001C2C08
    }
}

// 0x001C2C10 - 0x001C2C8C (0x7C) Shaggy1_CatchCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_CatchCB(class _zEnt *ent) {
    { // 0x001C2C10 - 0x001C2C8C
    }
}

// 0x001C2C90 - 0x001C2CDC (0x4C) Shaggy1_Catch__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Catch(class _zEnt *ent) {
    { // 0x001C2C90 - 0x001C2CDC
    }
}

// 0x001C2CE0 - 0x001C2D00 (0x20) Shaggy1_LandCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_LandCB(class _zEnt *ent) {
    { // 0x001C2CE0 - 0x001C2D00
    }
}

// 0x001C2D00 - 0x001C2D80 (0x80) Shaggy1_Land2__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Land2(class _zEnt *ent) {
    { // 0x001C2D00 - 0x001C2D80
    }
}

// 0x001C2D80 - 0x001C2E14 (0x94) Shaggy1_Land__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Land() {
    { // 0x001C2D80 - 0x001C2E14
    }
}

// 0x001C2E20 - 0x001C2E3C (0x1C) Shaggy1_ThrowEndCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_ThrowEndCB() {
    { // 0x001C2E20 - 0x001C2E3C
    }
}

// 0x001C2E40 - 0x001C2E88 (0x48) Shaggy1_ThrowCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_ThrowCB(class _zEnt *ent) {
    { // 0x001C2E40 - 0x001C2E88
    }
}

// 0x001C2E90 - 0x001C2EA4 (0x14) Shaggy1_Throw__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Throw() {
    { // 0x001C2E90 - 0x001C2EA4
    }
}

// 0x001C2EB0 - 0x001C2EF8 (0x48) Shaggy1_Stop__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Stop() {
    { // 0x001C2EB0 - 0x001C2EF8
    }
}

// 0x001C2F00 - 0x001C2F48 (0x48) Shaggy1_Walk__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Walk() {
    { // 0x001C2F00 - 0x001C2F48
    }
}

// 0x001C2F50 - 0x001C2F8C (0x3C) Shaggy1_GrabCB__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_GrabCB(class _zEnt *ent) {
    { // 0x001C2F50 - 0x001C2F8C
    }
}

// 0x001C2F90 - 0x001C3038 (0xA8) Shaggy1_Grab__FP15xAnimTransitionP11xAnimSingleP5_zEnt
static unsigned int Shaggy1_Grab(class xAnimTransition *tran, class _zEnt *ent) {
    { // 0x001C2F90 - 0x001C3038
    }
}

// 0x001C3040 - 0x001C30AC (0x6C) zEntShaggy_GetSwap__FP5_zEnt
class _zEnt *zEntShaggy_GetSwap(class _zEnt *ent) {
    { // 0x001C3040 - 0x001C30AC
    }
}

// 0x001C30B0 - 0x001C3118 (0x68) zEntShaggy_SetSpin__FP5_zEntf
void zEntShaggy_SetSpin(class _zEnt *ent, float spin) {
    { // 0x001C30B0 - 0x001C3118
    }
}

// 0x001C3120 - 0x001C3190 (0x70) zEntShaggy_GetSpin__FP5_zEnt
float zEntShaggy_GetSpin(class _zEnt *ent) {
    { // 0x001C3120 - 0x001C3190
    }
}

// 0x001C3190 - 0x001C31D8 (0x48) zEntIsShaggy__FP5_zEnt
unsigned int zEntIsShaggy(class _zEnt *ent) {
    { // 0x001C3190 - 0x001C31D8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xShadow.cpp
// ==========================================================================
void (modelRenderCB)(void *); // 0x001C4720
class zGlobals globals; // 0x00362FD0
unsigned int ourGlobals[4096]; // 0x002B5AD0
class tagiRenderInput gRenderBuffer; // 0x003A5980
class RpCollisionTriangle * (ShadowRenderTriangleCB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *); // 0x001C3460
float shit; // 0x00357614
signed int watch_tri; // 0x00357610
unsigned int tri_no; // 0x00358304
float zamt; // 0x00000000
float zlim; // 0x00000000
static class _xSphere sSphere; // 0x003A6C60
static unsigned int sShadowFlags; // 0x00358300
signed int ShadowFade; // 0x003582FC
float ShadowStrength; // 0x00357608
float ShadowZoneRadius; // 0x00357604
signed int ShadowAA; // 0x00000000
unsigned int ShadowBlur; // 0x00000000
static unsigned int ShadowResolutionIndex; // 0x00000000
static signed int ShadowEnabled; // 0x00000000
static class RwV2d ShadowPreviewVerts[2]; // 0x00000000
static class RwRaster *ShadowRenderRaster; // 0x003582F8
static class RwRaster *ShadowCameraRaster; // 0x003582F4
static class RwCamera *ShadowCamera; // 0x003582F0
float ShadowSubdArea; // 0x00000000
float ShadowThreshold; // 0x00357600
static unsigned int ShadowNumTriangles; // 0x003582E0
static unsigned int Im3DBufferPos; // 0x003582DC
static class RxObjSpace3DVertex *Im3DBuffer; // 0x003582D8
static float SHADOW_BOTH; // 0x003582D4
static float SHADOW_BF_DOT; // 0x003582D0
// 0x001C31E0 - 0x001C345C (0x27C) ShadowRender__FP8RwCameraP8RwRasterP7RpWorldP14RpIntersectionff
static signed int ShadowRender(class RwCamera *shadowCamera, class RpWorld *world, class RpIntersection *shadowZone, float shadowFactor, float fadeDist) {
    { // 0x001C31E0 - 0x001C345C
        class _xVec3 B; // r29+0x110
        class _xVec3 A; // r29+0x100
        class RwV3d tr; // r29+0xf0
        class RwV3d scl; // r29+0xe0
        float radius; // 
        class RwMatrixTag *shadowMatrix; // r16
        class _ProjectionParam param; // r29+0x80
    }
}

// 0x001C3460 - 0x001C3910 (0x4B0) ShadowRenderTriangleCB__FP14RpIntersectionP13RpWorldSectorP19RpCollisionTrianglefPv
static class RpCollisionTriangle *ShadowRenderTriangleCB(class RpCollisionTriangle *collTriangle, void *data) {
    { // 0x001C3460 - 0x001C3910
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        unsigned char val; // r2
        class RxObjSpace3DVertex *imv; // r4
        class RwV3d vShad[3]; // r29+0xd0
        class RwV3d vIn[3]; // r29+0xa0
        class _ProjectionParam *param; // r20
    }
}

// 0x001C3910 - 0x001C39F0 (0xE0) ShadowCameraUpdate__FP8RwCameraPvPFPv_vP6_xVec3f
static class RwCamera *ShadowCameraUpdate(class RwCamera *shadowCamera, void *model, void (*renderCB)(void *)) {
    { // 0x001C3910 - 0x001C39F0
        class RwCamera *camera; // r19
        class RwRGBA bgColor; // r29+0x5c
    }
}

// 0x001C39F0 - 0x001C3A74 (0x84) ShadowCameraDestroy__FP8RwCamera
static void ShadowCameraDestroy(class RwCamera *shadowCamera) {
    { // 0x001C39F0 - 0x001C3A74
        class RwFrame *frame; // r16
        class RwRaster *raster; // r2
    }
}

// 0x001C3A80 - 0x001C3AD0 (0x50) xShadowCameraDestroy__Fv
void xShadowCameraDestroy() {
    { // 0x001C3A80 - 0x001C3AD0
        class RpWorld *world; // r2
    }
}

// 0x001C3AD0 - 0x001C3CD4 (0x204) xShadowRender__FP5_xEnt
void xShadowRender(class _xEnt *ent) {
    { // 0x001C3AD0 - 0x001C3CD4
        float radius; // r29+0x4c
        class _xVec3 center; // r29+0x20
    }
}

// 0x001C3CE0 - 0x001C44D8 (0x7F8) xShadowReceiveShadow__FP5_xEnt
void xShadowReceiveShadow(class _xEnt *ent) {
    { // 0x001C3CE0 - 0x001C44D8
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        float len; // r29+0x220
        class RwV3d c; // r29+0x210
        class _xVec3 *v2; // r20
        class _xVec3 *v1; // r19
        class _xVec3 *v0; // r18
        class RwV3d vShad[3]; // r29+0x1c0
        class RwV3d vIn[3]; // r29+0x190
        class RxObjSpace3DVertex *imv; // r17
        unsigned int max_verts; // r4
        class xModelInstance *model; // r3
        unsigned char val; // r16
        float shadowFactor; // r29+0x220
        class RpGeometry *geom; // r29+0xc0
        class RpTriangle *tri; // r22
        class _xVec3 *xvert; // r23
        unsigned int numverts; // r2
        unsigned int i; // r30
        class RwV3d tr; // r29+0x200
        class RwV3d scl; // r29+0x1f0
        float radius; // 
        class RwMatrixTag invMatrix; // r29+0x150
        class RwMatrixTag *shadowMatrix; // r2
        class RwCamera *shadowCamera; // r16
    }
}

// 0x001C44E0 - 0x001C45E4 (0x104) xShadowReceiveShadowSetup__FP5_xEnt
unsigned int xShadowReceiveShadowSetup(class _xEnt *ent) {
    { // 0x001C44E0 - 0x001C45E4
        unsigned int rsf_valid; // r7
        unsigned int sceneID; // r16
    }
}

// 0x001C45F0 - 0x001C471C (0x12C) xShadowEntIsOutsideFrustum__FP5_xEnt
static unsigned int xShadowEntIsOutsideFrustum(class _xEnt *ent) {
    { // 0x001C45F0 - 0x001C471C
        unsigned int test; // r2
        class RwSphere worldsph; // r29+0x40
        class RwCamera *cam; // r17
        class RwMatrixTag *mat; // r16
        class RwV3d *at; // r2
        class RwV3d *up; // r2
        float atomicScale; // r29+0x50
        float zScale2; // 
        float yScale2; // 
        float xScale2; // r29+0x50
    }
}

// 0x001C4720 - 0x001C4728 (0x8) modelRenderCB__FPv
static void modelRenderCB(void *model) {
    { // 0x001C4720 - 0x001C4728
    }
}

// 0x001C4730 - 0x001C4C70 (0x540) xShadowRenderWorld__FP6_xVec3f
void xShadowRenderWorld(class _xVec3 *center, float radius) {
    { // 0x001C4730 - 0x001C4C70
        class _xCollis ecrec; // r29+0x290
        class _xVec3 hit; // r29+0x2e0
        signed int i; // r30
        float env_dist; // 
        float ent_dist; // r29+0x2f0
        unsigned int hit_ent; // r29+0xc0
        unsigned int hit_env; // r17
        float sf[3][2]; // r29+0x270
        class _xCollis crec[3]; // r29+0x1a0
        class _xRay3 R[3]; // r29+0x130
        class _xVec3 *rt; // r16
        class _xVec3 *up; // r22
        class _xVec3 *at; // r23
        class RwMatrixTag *camMatrix; // r2
        class _xSphere zone; // r29+0x120
        class RpIntersection shadowZone; // r29+0x100
    }
}

// 0x001C4C70 - 0x001C4DB8 (0x148) xShadowCameraUpdate__FPvPFPv_vP6_xVec3f
void xShadowCameraUpdate(void *model, void (*renderCB)(void *), class _xVec3 *center, float radius) {
    { // 0x001C4C70 - 0x001C4DB8
    }
}

// 0x001C4DC0 - 0x001C4DE0 (0x20) xShadowCameraCreate__Fv
signed int xShadowCameraCreate() {
    { // 0x001C4DC0 - 0x001C4DE0
    }
}

// 0x001C4DE0 - 0x001C4E68 (0x88) xShadowSetLight__FP6_xVec3P6_xVec3f
void xShadowSetLight(class _xVec3 *target_pos, class _xVec3 *in_vec) {
    { // 0x001C4DE0 - 0x001C4E68
        class RwMatrixTag *camMatrix; // r17
        class RwFrame *camFrame; // r16
        class _xMat4x3 matrix; // r29+0x30
        class _xVec3 zvec; // r29+0x70
    }
}

// 0x001C4E70 - 0x001C511C (0x2AC) xShadowSetWorld__FP7RpWorld
void xShadowSetWorld(class RpWorld *world) {
    { // 0x001C4E70 - 0x001C511C
        unsigned int sceneID; // r16
    }
}

// 0x001C5120 - 0x001C5240 (0x120) SetupShadow__Fv
static signed int SetupShadow() {
    { // 0x001C5120 - 0x001C5240
        signed int res; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zRumble.cpp
// ==========================================================================
// 0x001C5240 - 0x001C52D0 (0x90) zRumbleStartDistance__Fiff
void zRumbleStartDistance(signed int pad_id, float real_dist, float max_dist) {
    { // 0x001C5240 - 0x001C52D0
        float rumble_it; // r29+0x10
    }
}

// 0x001C52D0 - 0x001C55B8 (0x2E8) zRumbleStart__Fi16_tagSDRumbleType
void zRumbleStart(signed int pad_id, enum _tagSDRumbleType rumble_type) {
    { // 0x001C52D0 - 0x001C55B8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zHud.cpp
// ==========================================================================
unsigned int ourGlobals[4096]; // 0x002B5AD0
class zGlobals globals; // 0x00362FD0
static signed int sStatInit; // 0x00358350
static float dynOffset; // 0x0035834C
static class iColor_tag clrDropShadow; // 0x00357624
static class iColor_tag clr; // 0x00357620
static class xFontBox *fontBox; // 0x00358348
static class _zHudElement sHudElements[10]; // 0x002B42A0
static class zHudItem sHudGameStatsArray[7]; // 0x003AA710
static class zHudItem sHudHealthbarArray[5]; // 0x003A9C00
static class zHudItem sHudPlayerStatusArray[6]; // 0x003A8EC0
static class zHudItem sHudGumpackArray[1]; // 0x003A8C80
static class zHudItem sHudSoapboxArray[1]; // 0x003A8A40
static class zHudItem sHudSoapArray[6]; // 0x003A7D00
static class zHudItem sHudGumArray[5]; // 0x003A71F0
static class zHudItem sHudChancesArray[1]; // 0x003A6FB0
static class zHudItem sHudSnackArray[1]; // 0x003A6D70
static class zHudModel zHudModelTable[42]; // 0x002B40A0
static unsigned short Idx[6]; // 0x002B4088
static class RwSky2DVertex Vert[4]; // 0x003A6C70
static float sHudDisplayTimer_saveDone; // 0x0035761C
static float sHudDisplayTimer_allSnacksArea; // 0x00357618
static signed int oldSoapCnt2; // 0x00358344
static signed int oldSoapCnt; // 0x00358340
static signed int oldGumCnt2; // 0x0035833C
static signed int oldGumCnt; // 0x00358338
static unsigned int oldSnackCnt; // 0x00000000
static unsigned int oldHealthCnt2; // 0x00358334
static unsigned int oldHealthCnt; // 0x00358330
static float sHudDisplayTimer_healthBar; // 0x0035832C
static float sHudDisplayTimer_soap; // 0x00358328
static float sHudDisplayTimer_gum; // 0x00358324
static float sHudDisplayTimer_snacks; // 0x00358320
static float sHudDisplayTimer_plrStatus; // 0x0035831C
static float sHudStatusAngZ; // 0x00000000
static float sHudStatusAngY; // 0x00000000
static float sHudStatusAngX; // 0x00000000
static float tmpRoomTime; // 0x00358318
static signed int sHudGameStatsTimer; // 0x00358314
static float heightAspect; // 0x00358310
static float widthAspect; // 0x0035830C
static signed int sShakeSnackCounter; // 0x00358308
// 0x001C55C0 - 0x001C59A0 (0x3E0) zHudInterp2d__FfPfPffffi
void zHudInterp2d(float dt, float *srcx, float *srcy, float dstx, float dsty, float speed, signed int type) {
    { // 0x001C55C0 - 0x001C59A0
        float d2; // 
        class _xVec2 tNorm; // r29+0x78
        class _xVec2 t; // r29+0x70
        float dy; // r29+0x80
        float dx; // r29+0x80
    }
}

// 0x001C59A0 - 0x001C5AE8 (0x148) zHudInterp1d__FfPfffi
void zHudInterp1d(float dt, float *src, float dst, float speed, signed int type) {
    { // 0x001C59A0 - 0x001C5AE8
        float diff; // r29+0x30
    }
}

// 0x001C5AF0 - 0x001C5B08 (0x18) zHudHide__Fv
void zHudHide() {
    { // 0x001C5AF0 - 0x001C5B08
    }
}

// 0x001C5B10 - 0x001C5B30 (0x20) zHudShow__Fv
void zHudShow() {
    { // 0x001C5B10 - 0x001C5B30
    }
}

// 0x001C5B30 - 0x001C5B7C (0x4C) zHudModelGetID__FUi
unsigned int zHudModelGetID(unsigned int modelEnum) {
    { // 0x001C5B30 - 0x001C5B7C
        signed int i; // r3
    }
}

// 0x001C5B80 - 0x001C5B88 (0x8) zHud_sceneReset__Fv
void zHud_sceneReset() {
    { // 0x001C5B80 - 0x001C5B88
    }
}

// 0x001C5B90 - 0x001C5FCC (0x43C) zHud_UIRender__FPcffffUc
void zHud_UIRender(char *asset, float xPos, float yPos, float w, float h, unsigned char alpha) {
    { // 0x001C5B90 - 0x001C5FCC
        float cooz; // 
        float cz; // 
        float z; // r29+0x50
        float y2; // 
        float x2; // 
        class RwCamera *camera; // r17
        unsigned char b; // r16
        unsigned char g; // r2
        unsigned char r; // r2
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
    }
}

// 0x001C5FD0 - 0x001C6188 (0x1B8) zHud_DrawText__FffffPcifUc18eHudDropShadowMode
void zHud_DrawText(float x, float y, float w, float h, char *s, signed int just, float size, unsigned char alpha, enum eHudDropShadowMode shadow) {
    { // 0x001C5FD0 - 0x001C6188
        float drawSizeY; // r29+0x70
        float drawSizeX; // r29+0x70
    }
}

// 0x001C6190 - 0x001C63BC (0x22C) zHudElementsCB_GameStatsRender__Fv
void zHudElementsCB_GameStatsRender() {
    { // 0x001C6190 - 0x001C63BC
        class _xMat4x3 frame; // r29+0x90
        class _xVec3 to; // r29+0xe0
        class _xVec3 from; // r29+0xd0
        class _xRect r; // r29+0x80
        signed int i; // r16
    }
}

// 0x001C63C0 - 0x001C6A5C (0x69C) zHudElementsCB_GameStatsUpdate__FP7_xScenef
void zHudElementsCB_GameStatsUpdate(float dt) {
    { // 0x001C63C0 - 0x001C6A5C
        float total; // 
        float collected; // 
        signed int i; // r16
        class iColor_tag black; // r29+0x6c
        class iColor_tag clear; // r29+0x68
    }
}

// 0x001C6A60 - 0x001C6D70 (0x310) zHudElementsCB_GameStatsSetup__FP7_xScene
void zHudElementsCB_GameStatsSetup() {
    { // 0x001C6A60 - 0x001C6D70
        signed int i; // r3
    }
}

// 0x001C6D70 - 0x001C70C8 (0x358) zHudElementsCB_GameStatsInit__Fv
void zHudElementsCB_GameStatsInit() {
    { // 0x001C6D70 - 0x001C70C8
        class xAnimFile *afile; // r18
        unsigned int size; // r29+0x60
        void *buff; // r17
        signed int i; // r16
    }
}

// 0x001C70D0 - 0x001C727C (0x1AC) zHudElementsCB_HealthbarRender__Fv
void zHudElementsCB_HealthbarRender() {
    { // 0x001C70D0 - 0x001C727C
        class _xMat4x3 frame; // r29+0x80
        class _xVec3 to; // r29+0xd0
        class _xVec3 from; // r29+0xc0
        class _xRect r; // r29+0x70
        signed int i; // r16
    }
}

// 0x001C7280 - 0x001C784C (0x5CC) zHudElementsCB_HealthbarUpdate__FP7_xScenef
void zHudElementsCB_HealthbarUpdate(float dt) {
    { // 0x001C7280 - 0x001C784C
        signed int i; // r16
        signed int i; // r9
        signed int i; // r9
        signed int i; // r8
        signed int num; // r7
    }
}

// 0x001C7850 - 0x001C79FC (0x1AC) zHudElementsCB_HealthbarSetup__FP7_xScene
void zHudElementsCB_HealthbarSetup() {
    { // 0x001C7850 - 0x001C79FC
        class xAnimSingle *asing; // r2
        class xAnimPlay *aplay; // r3
        signed int i; // r16
    }
}

// 0x001C7A00 - 0x001C7C90 (0x290) zHudElementsCB_HealthbarInit__Fv
void zHudElementsCB_HealthbarInit() {
    { // 0x001C7A00 - 0x001C7C90
        class xAnimFile *afile; // r18
        unsigned int size; // r29+0x68
        void *buff; // r17
        signed int i; // r16
    }
}

// 0x001C7C90 - 0x001C7ED8 (0x248) zHudElementsCB_GumpackRender__Fv
void zHudElementsCB_GumpackRender() {
    { // 0x001C7C90 - 0x001C7ED8
        char str[256]; // r29+0x80
        class RwRGBA colgum; // r29+0x19c
        class _xMat4x3 frame; // r29+0x40
        class _xVec3 to; // r29+0x190
        class _xVec3 from; // r29+0x180
        class _xRect r; // r29+0x30
        class zHudItem *item; // r2
    }
}

// 0x001C7EE0 - 0x001C82AC (0x3CC) zHudElementsCB_GumpackUpdate__FP7_xScenef
void zHudElementsCB_GumpackUpdate(float dt) {
    { // 0x001C7EE0 - 0x001C82AC
        class zHudItem *item; // r2
    }
}

// 0x001C82B0 - 0x001C8434 (0x184) zHudElementsCB_GumpackSetup__FP7_xScene
void zHudElementsCB_GumpackSetup() {
    { // 0x001C82B0 - 0x001C8434
        class xAnimSingle *asing; // r5
        class xAnimPlay *aplay; // r4
        class zHudItem *item; // r2
    }
}

// 0x001C8440 - 0x001C85BC (0x17C) zHudElementsCB_GumpackInit__Fv
void zHudElementsCB_GumpackInit() {
    { // 0x001C8440 - 0x001C85BC
        class xAnimFile *afile; // r17
        unsigned int size; // r29+0x38
        void *buff; // r2
    }
}

// 0x001C85C0 - 0x001C87BC (0x1FC) zHudElementsCB_SoapboxRender__Fv
void zHudElementsCB_SoapboxRender() {
    { // 0x001C85C0 - 0x001C87BC
        char str[256]; // r29+0x80
        class _xMat4x3 frame; // r29+0x40
        class _xVec3 to; // r29+0x190
        class _xVec3 from; // r29+0x180
        class _xRect r; // r29+0x30
        class zHudItem *item; // r2
    }
}

// 0x001C87C0 - 0x001C8B68 (0x3A8) zHudElementsCB_SoapboxUpdate__FP7_xScenef
void zHudElementsCB_SoapboxUpdate(float dt) {
    { // 0x001C87C0 - 0x001C8B68
        class zHudItem *item; // r2
    }
}

// 0x001C8B70 - 0x001C8D0C (0x19C) zHudElementsCB_SoapboxSetup__FP7_xScene
void zHudElementsCB_SoapboxSetup() {
    { // 0x001C8B70 - 0x001C8D0C
        class xAnimSingle *asing; // r5
        class xAnimPlay *aplay; // r4
        class zHudItem *item; // r2
    }
}

// 0x001C8D10 - 0x001C8E8C (0x17C) zHudElementsCB_SoapboxInit__Fv
void zHudElementsCB_SoapboxInit() {
    { // 0x001C8D10 - 0x001C8E8C
        class xAnimFile *afile; // r17
        unsigned int size; // r29+0x38
        void *buff; // r2
    }
}

// 0x001C8E90 - 0x001C8E98 (0x8) zHudElementsCB_MessageRender__Fv
void zHudElementsCB_MessageRender() {
    { // 0x001C8E90 - 0x001C8E98
    }
}

// 0x001C8EA0 - 0x001C8EA8 (0x8) zHudElementsCB_MessageUpdate__FP7_xScenef
void zHudElementsCB_MessageUpdate() {
    { // 0x001C8EA0 - 0x001C8EA8
    }
}

// 0x001C8EB0 - 0x001C8EB8 (0x8) zHudElementsCB_MessageSetup__FP7_xScene
void zHudElementsCB_MessageSetup() {
    { // 0x001C8EB0 - 0x001C8EB8
    }
}

// 0x001C8EC0 - 0x001C8EC8 (0x8) zHudElementsCB_MessageInit__Fv
void zHudElementsCB_MessageInit() {
    { // 0x001C8EC0 - 0x001C8EC8
    }
}

// 0x001C8ED0 - 0x001C91D8 (0x308) zHudElementsCB_SoapRender__Fv
void zHudElementsCB_SoapRender() {
    { // 0x001C8ED0 - 0x001C91D8
        class RwRGBA colsoap; // r29+0x15c
        class _xMat4x3 frame; // r29+0xe0
        class _xVec3 to; // r29+0x150
        class _xVec3 from; // r29+0x140
        class _xRect r; // r29+0xd0
        signed int i; // r16
        class _xMat4x3 frame; // r29+0x90
        class _xVec3 to; // r29+0x130
        class _xVec3 from; // r29+0x120
        class _xRect r; // r29+0x80
    }
}

// 0x001C91E0 - 0x001C97B8 (0x5D8) zHudElementsCB_SoapUpdate__FP7_xScenef
void zHudElementsCB_SoapUpdate(float dt) {
    { // 0x001C91E0 - 0x001C97B8
        signed int i; // r16
        signed int i; // r5
        signed int i; // r6
        signed int num; // r5
    }
}

// 0x001C97C0 - 0x001C9A60 (0x2A0) zHudElementsCB_SoapSetup__FP7_xScene
void zHudElementsCB_SoapSetup() {
    { // 0x001C97C0 - 0x001C9A60
        signed int i; // r3
    }
}

// 0x001C9A60 - 0x001C9BD4 (0x174) zHudElementsCB_SoapInit__Fv
void zHudElementsCB_SoapInit() {
    { // 0x001C9A60 - 0x001C9BD4
    }
}

// 0x001C9BE0 - 0x001C9D9C (0x1BC) zHudElementsCB_GumRender__Fv
void zHudElementsCB_GumRender() {
    { // 0x001C9BE0 - 0x001C9D9C
        class _xMat4x3 frame; // r29+0x80
        class _xVec3 to; // r29+0xd0
        class _xVec3 from; // r29+0xc0
        class _xRect r; // r29+0x70
        signed int i; // r16
    }
}

// 0x001C9DA0 - 0x001CA2AC (0x50C) zHudElementsCB_GumUpdate__FP7_xScenef
void zHudElementsCB_GumUpdate(float dt) {
    { // 0x001C9DA0 - 0x001CA2AC
        signed int i; // r16
        signed int i; // r3
        signed int num; // r4
    }
}

// 0x001CA2B0 - 0x001CA424 (0x174) zHudElementsCB_GumSetup__FP7_xScene
void zHudElementsCB_GumSetup() {
    { // 0x001CA2B0 - 0x001CA424
        class xAnimSingle *asing; // r2
        class xAnimPlay *aplay; // r3
        signed int i; // r16
    }
}

// 0x001CA430 - 0x001CA834 (0x404) zHudElementsCB_GumInit__Fv
void zHudElementsCB_GumInit() {
    { // 0x001CA430 - 0x001CA834
        class xAnimFile *afile; // r17
        unsigned int size; // r29+0x58
        void *buff; // r2
        signed int i; // r16
    }
}

// 0x001CA840 - 0x001CAA1C (0x1DC) zHudElementsCB_SnacksRender__Fv
void zHudElementsCB_SnacksRender() {
    { // 0x001CA840 - 0x001CAA1C
        class _xMat4x3 frame; // r29+0x140
        class _xVec3 to; // r29+0x190
        class _xVec3 from; // r29+0x180
        class _xRect r; // r29+0x130
        char str[256]; // r29+0x30
    }
}

// 0x001CAA20 - 0x001CAA2C (0xC) zHudShowSaveSuccess__Fv
void zHudShowSaveSuccess() {
    { // 0x001CAA20 - 0x001CAA2C
    }
}

// 0x001CAA30 - 0x001CAA40 (0x10) zHudShowAllSnacksArea__Fv
void zHudShowAllSnacksArea() {
    { // 0x001CAA30 - 0x001CAA40
    }
}

// 0x001CAA40 - 0x001CAA4C (0xC) zHud_showSnackSpecial__Fv
void zHud_showSnackSpecial() {
    { // 0x001CAA40 - 0x001CAA4C
    }
}

// 0x001CAA50 - 0x001CAA5C (0xC) zHud_showSnack__Fv
void zHud_showSnack() {
    { // 0x001CAA50 - 0x001CAA5C
    }
}

// 0x001CAA60 - 0x001CAA78 (0x18) zHud_ShakeSnackCounter__Fv
void zHud_ShakeSnackCounter() {
    { // 0x001CAA60 - 0x001CAA78
    }
}

// 0x001CAA80 - 0x001CAE40 (0x3C0) zHudElementsCB_SnacksUpdate__FP7_xScenef
void zHudElementsCB_SnacksUpdate(float dt) {
    { // 0x001CAA80 - 0x001CAE40
        signed int i; // r4
        class zHudItem *item; // r16
    }
}

// 0x001CAE40 - 0x001CAF48 (0x108) zHudElementsCB_SnacksSetup__FP7_xScene
void zHudElementsCB_SnacksSetup() {
    { // 0x001CAE40 - 0x001CAF48
    }
}

// 0x001CAF50 - 0x001CB008 (0xB8) zHudElementsCB_SnacksInit__Fv
void zHudElementsCB_SnacksInit() {
    { // 0x001CAF50 - 0x001CB008
    }
}

// 0x001CB010 - 0x001CB018 (0x8) zHudElementsCB_ChancesRender__Fv
void zHudElementsCB_ChancesRender() {
    { // 0x001CB010 - 0x001CB018
    }
}

// 0x001CB020 - 0x001CB230 (0x210) zHudElementsCB_ChancesUpdate__FP7_xScenef
void zHudElementsCB_ChancesUpdate(float dt) {
    { // 0x001CB020 - 0x001CB230
        class zHudItem *item; // r2
    }
}

// 0x001CB230 - 0x001CB2C8 (0x98) zHudElementsCB_ChancesSetup__FP7_xScene
void zHudElementsCB_ChancesSetup() {
    { // 0x001CB230 - 0x001CB2C8
    }
}

// 0x001CB2D0 - 0x001CB2D8 (0x8) zHudElementsCB_ChancesInit__Fv
void zHudElementsCB_ChancesInit() {
    { // 0x001CB2D0 - 0x001CB2D8
    }
}

// 0x001CB2E0 - 0x001CB4FC (0x21C) zHudElementsCB_PlayerStatusRender__Fv
void zHudElementsCB_PlayerStatusRender() {
    { // 0x001CB2E0 - 0x001CB4FC
        class _xMat4x3 frame; // r29+0x90
        class _xVec3 to; // r29+0xe0
        class _xVec3 from; // r29+0xd0
        class _xRect r; // r29+0x80
        signed int i; // r16
    }
}

// 0x001CB500 - 0x001CB510 (0x10) zHud_showHealthandStatus__Fv
void zHud_showHealthandStatus() {
    { // 0x001CB500 - 0x001CB510
    }
}

// 0x001CB510 - 0x001CB968 (0x458) zHudElementsCB_PlayerStatusUpdate__FP7_xScenef
void zHudElementsCB_PlayerStatusUpdate(float dt) {
    { // 0x001CB510 - 0x001CB968
        signed int i; // r16
        signed int i; // r3
        unsigned int num; // r4
    }
}

// 0x001CB970 - 0x001CBAF4 (0x184) zHudElementsCB_PlayerStatusSetup__FP7_xScene
void zHudElementsCB_PlayerStatusSetup() {
    { // 0x001CB970 - 0x001CBAF4
        class xAnimSingle *asing; // r2
        class xAnimPlay *aplay; // r3
        signed int i; // r16
    }
}

// 0x001CBB00 - 0x001CC078 (0x578) zHudElementsCB_PlayerStatusInit__Fv
void zHudElementsCB_PlayerStatusInit() {
    { // 0x001CBB00 - 0x001CC078
        class xAnimFile *afile; // r18
        unsigned int size; // r29+0x64
        void *buff; // r17
        signed int i; // r16
    }
}

// 0x001CC080 - 0x001CC0E8 (0x68) zHud_Render__Fv
void zHud_Render() {
    { // 0x001CC080 - 0x001CC0E8
        signed int i; // r16
    }
}

// 0x001CC0F0 - 0x001CC330 (0x240) zHud_Update__FP7_xScenef
void zHud_Update(class _xScene *sc, float dt) {
    { // 0x001CC0F0 - 0x001CC330
        signed int i; // r16
    }
}

// 0x001CC330 - 0x001CC388 (0x58) zHud_Setup__FP7_xScene
void zHud_Setup(class _xScene *sc) {
    { // 0x001CC330 - 0x001CC388
        signed int i; // r16
    }
}

// 0x001CC390 - 0x001CC45C (0xCC) zHud_Init__Fv
void zHud_Init() {
    { // 0x001CC390 - 0x001CC45C
        signed int i; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xFilter.cpp
// ==========================================================================
// 0x001CC460 - 0x001CC4A4 (0x44) xFilterCurvedInterp4__Ff
float xFilterCurvedInterp4(float in_t) {
    { // 0x001CC460 - 0x001CC4A4
        float in_t2; // 
    }
}

// 0x001CC4B0 - 0x001CC4F4 (0x44) xFilterCurvedInterp3__Ff
float xFilterCurvedInterp3(float in_t) {
    { // 0x001CC4B0 - 0x001CC4F4
        float in_t2; // 
    }
}

// 0x001CC500 - 0x001CC544 (0x44) xFilterCurvedInterp2__Ff
float xFilterCurvedInterp2(float in_t) {
    { // 0x001CC500 - 0x001CC544
        float in_t2; // 
    }
}

// 0x001CC550 - 0x001CC594 (0x44) xFilterCurvedInterp1__Ff
float xFilterCurvedInterp1(float in_t) {
    { // 0x001CC550 - 0x001CC594
        float in_t2; // 
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zFog.cpp
// ==========================================================================
signed int (zFogEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001CC5B0
// 0x001CC5A0 - 0x001CC5A8 (0x8) zFogUpdate__FP5xBaseP7_xScenef
void zFogUpdate() {
    { // 0x001CC5A0 - 0x001CC5A8
    }
}

// 0x001CC5B0 - 0x001CC6B4 (0x104) zFogEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zFogEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001CC5B0 - 0x001CC6B4
        class iFogParams fog; // r29+0x10
        class _zFog *t; // r2
    }
}

// 0x001CC6C0 - 0x001CC6C8 (0x8) zFogLoad__FP5_zFogP7xSerial
void zFogLoad(class _zFog *ent, class xSerial *s) {
    { // 0x001CC6C0 - 0x001CC6C8
    }
}

// 0x001CC6D0 - 0x001CC6D8 (0x8) zFogSave__FP5_zFogP7xSerial
void zFogSave(class _zFog *ent, class xSerial *s) {
    { // 0x001CC6D0 - 0x001CC6D8
    }
}

// 0x001CC6E0 - 0x001CC740 (0x60) zFogInit__FPvPv
void zFogInit(void *b, void *tasset) {
    { // 0x001CC6E0 - 0x001CC740
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zLight.cpp
// ==========================================================================
signed int (zLightEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001CD220
unsigned int gActiveHeap; // 0x00357CAC
class RpWorld *gLightWorld; // 0x0035819C
static class _xVec3 sDefaultShadowVec; // 0x002B4EB8
void (*sEffectInitFuncs[18])(class _zLight *); // 0x002B4E70
void (*sEffectFuncs[18])(class _zLight *, float); // 0x002B4E20
static class _zLight *gTemporaryLights[32]; // 0x003ABB60
static signed int gNumTemporaryLights; // 0x00358370
static class _zVolume *sPartitionVolume; // 0x0035836C
static class _tagPartition sLightPart; // 0x003ABB20
static signed int sLightTotal; // 0x00358368
static class _zLight *sLight[32]; // 0x003ABAA0
// 0x001CC740 - 0x001CC770 (0x30) zLightOn__FP7_zLighti
void zLightOn(class _zLight *zl, signed int on) {
    { // 0x001CC740 - 0x001CC770
    }
}

// 0x001CC770 - 0x001CC7B8 (0x48) zLightSetVolume__FP8_zVolume
void zLightSetVolume(class _zVolume *vol) {
    { // 0x001CC770 - 0x001CC7B8
        unsigned int lp_id; // r2
    }
}

// 0x001CC7C0 - 0x001CC82C (0x6C) zLightRemoveLocal__Fv
void zLightRemoveLocal() {
    { // 0x001CC7C0 - 0x001CC82C
        signed int i; // r16
    }
}

// 0x001CC830 - 0x001CC880 (0x50) zLightRemoveLocalEnv__Fv
void zLightRemoveLocalEnv() {
    { // 0x001CC830 - 0x001CC880
        class RwLLLink *link; // r6
        signed int i; // r5
    }
}

// 0x001CC880 - 0x001CD000 (0x780) zLightAddLocal__FP5_xEnt
void zLightAddLocal(class _xEnt *ent) {
    { // 0x001CC880 - 0x001CD000
        float w; // 
        signed int i; // r16
        class _xVec3 single_light_pos; // r29+0x448
        class _xVec3 single_light_vec; // r29+0x438
        float dist; // 
        float z; // 
        float y; // 
        float x; // 
        class iLight *light; // r2
        float z; // 
        float y; // 
        float x; // 
        class iLight *light; // r23
        unsigned int test_flags; // r5
        class _zLight *zlight; // r16
        class _tagPartLink *link; // r2
        class _tagPartSpace *space; // r2
        signed int true_idx; // r2
        signed int xi; // r29+0xe0
        signed int zi; // r29+0xd0
        signed int zend; // r29+0x46c
        signed int xend; // r29+0x468
        signed int zstart; // r29+0x464
        signed int xstart; // r29+0x460
        signed int vz; // r29+0x45c
        signed int vx; // r29+0x458
        class _xVec3 default_light_pos; // r29+0x428
        signed int shadow_light_total; // r29+0xc0
        float shadow_dist[32]; // r29+0x3a0
        class _xVec3 shadow_vec[32]; // r29+0x220
        class _zLight *shadow_light[32]; // r29+0x1a0
    }
}

// 0x001CD000 - 0x001CD124 (0x124) zLightAddLocalEnv__Fv
void zLightAddLocalEnv() {
    { // 0x001CD000 - 0x001CD124
        class iLight *light; // r18
        class _zLight *zlight; // r17
        signed int i; // r16
    }
}

// 0x001CD130 - 0x001CD21C (0xEC) zLightUpdate__FP5xBaseP7_xScenef
void zLightUpdate(class xBase *to, float dt) {
    { // 0x001CD130 - 0x001CD21C
        class _xVec3 pos; // r29+0x40
        class _zLight *t; // r16
    }
}

// 0x001CD220 - 0x001CD2F8 (0xD8) zLightEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zLightEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001CD220 - 0x001CD2F8
        class _zLight *t; // r16
    }
}

// 0x001CD300 - 0x001CD308 (0x8) zLightLoad__FP7_zLightP7xSerial
void zLightLoad(class _zLight *ent, class xSerial *s) {
    { // 0x001CD300 - 0x001CD308
    }
}

// 0x001CD310 - 0x001CD318 (0x8) zLightSave__FP7_zLightP7xSerial
void zLightSave(class _zLight *ent, class xSerial *s) {
    { // 0x001CD310 - 0x001CD318
    }
}

// 0x001CD320 - 0x001CD3A0 (0x80) zLightDestroyAll__Fv
void zLightDestroyAll() {
    { // 0x001CD320 - 0x001CD3A0
        signed int i; // r17
        signed int total; // r16
    }
}

// 0x001CD3A0 - 0x001CD42C (0x8C) zLightResolveLinks__Fv
void zLightResolveLinks() {
    { // 0x001CD3A0 - 0x001CD42C
        class _zLight *zl; // r17
        signed int i; // r16
    }
}

// 0x001CD430 - 0x001CD608 (0x1D8) zLightInit__FP5xBaseP11zLightAsset
void zLightInit(class xBase *b, class zLightAsset *tasset) {
    { // 0x001CD430 - 0x001CD608
        unsigned int itype; // r2
        class _zLight *t; // r2
    }
}

// 0x001CD610 - 0x001CD618 (0x8) zLightInit__FPvPv
void zLightInit(void *b, void *tasset) {
    { // 0x001CD610 - 0x001CD618
    }
}

// 0x001CD620 - 0x001CD6B0 (0x90) zLightResetAll__FP5_xEnv
void zLightResetAll(class _xEnv *env) {
    { // 0x001CD620 - 0x001CD6B0
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zSaveLoad.cpp
// ==========================================================================
unsigned int gScreenDump; // 0x00357D08
unsigned int gFrameCount; // 0x00357C78
signed int (xSGT_SaveInfoCB)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *); // 0x001CDA50
signed int (xSGT_SaveProcCB)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *); // 0x001CD9C0
signed int (xSGT_SaveInfoPrefsCB)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *); // 0x001CD9A0
signed int (xSGT_SaveProcPrefsCB)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *); // 0x001CD8C0
signed int (xSGT_LoadLoadCB)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int); // 0x001CD7F0
signed int (xSGT_LoadPrefsCB)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int); // 0x001CD700
class xSndGlobals gSnd; // 0x00373B80
class zGlobals globals; // 0x00362FD0
class zSaveLoadGame zSaveLoadGameTable[3]; // 0x003ABBF0
class zSaveLoadUI zSaveLoadUITable[49]; // 0x002B4F20
static char sceneRead[32]; // 0x002B4F00
static char currSceneStr[32]; // 0x002B4EE0
static float dontPoll; // 0x00357640
static signed int sNeeded; // 0x0035839C
static signed int sAvailable; // 0x00358398
static signed int badCard; // 0x00358394
static signed int promptSel; // 0x00358390
static signed int currentGame; // 0x0035763C
static signed int currentCard; // 0x00357638
static signed long t1; // 0x00358388
static signed long t0; // 0x00358380
static float time_current; // 0x0035837C
static float time_last; // 0x00358378
static float time_elapsed; // 0x00357634
static unsigned int saveSuccess; // 0x00358374
// 0x001CD6B0 - 0x001CD6F8 (0x48) zSaveLoad_slotIsEmpty__FUi
unsigned int zSaveLoad_slotIsEmpty(unsigned int i) {
    { // 0x001CD6B0 - 0x001CD6F8
    }
}

// 0x001CD700 - 0x001CD7E8 (0xE8) xSGT_LoadPrefsCB__FPvP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTUii
signed int xSGT_LoadPrefsCB(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt) {
    { // 0x001CD700 - 0x001CD7E8
    }
}

// 0x001CD7F0 - 0x001CD8B8 (0xC8) xSGT_LoadLoadCB__FPvP17st_XSAVEGAME_DATAP24st_XSAVEGAME_READCONTEXTUii
signed int xSGT_LoadLoadCB(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_READCONTEXT *rctxt) {
    { // 0x001CD7F0 - 0x001CD8B8
        signed int compdiff; // r16
        char bigbuf[32]; // r29+0x50
    }
}

// 0x001CD8C0 - 0x001CD9A0 (0xE0) xSGT_SaveProcPrefsCB__FPvP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXT
signed int xSGT_SaveProcPrefsCB(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt) {
    { // 0x001CD8C0 - 0x001CD9A0
        signed int sum; // r16
        signed int amy; // r2
    }
}

// 0x001CD9A0 - 0x001CD9BC (0x1C) xSGT_SaveInfoPrefsCB__FPvP17st_XSAVEGAME_DATAPiPi
signed int xSGT_SaveInfoPrefsCB(signed int *need, signed int *most) {
    { // 0x001CD9A0 - 0x001CD9BC
    }
}

// 0x001CD9C0 - 0x001CDA50 (0x90) xSGT_SaveProcCB__FPvP17st_XSAVEGAME_DATAP25st_XSAVEGAME_WRITECONTEXT
signed int xSGT_SaveProcCB(class st_XSAVEGAME_DATA *xsgdata, class st_XSAVEGAME_WRITECONTEXT *wctxt) {
    { // 0x001CD9C0 - 0x001CDA50
        signed int sum; // r16
        signed int amy; // r2
    }
}

// 0x001CDA50 - 0x001CDA9C (0x4C) xSGT_SaveInfoCB__FPvP17st_XSAVEGAME_DATAPiPi
signed int xSGT_SaveInfoCB(signed int *need, signed int *most) {
    { // 0x001CDA50 - 0x001CDA9C
        signed int slen; // r2
    }
}

// 0x001CDAA0 - 0x001CDB18 (0x78) zSaveLoad_DispatchCB__FUiPf
void zSaveLoad_DispatchCB(unsigned int dispatchEvent, float *toParam) {
    { // 0x001CDAA0 - 0x001CDB18
    }
}

// 0x001CDB20 - 0x001CDDBC (0x29C) zSaveLoad_SaveLoop__Fv
unsigned int zSaveLoad_SaveLoop() {
    { // 0x001CDB20 - 0x001CDDBC
        signed int tmp; // r2
        signed int state; // r16
    }
}

// 0x001CDDC0 - 0x001CE094 (0x2D4) zSaveLoad_LoadLoop__Fv
unsigned int zSaveLoad_LoadLoop() {
    { // 0x001CDDC0 - 0x001CE094
        signed int tmp; // r2
        signed int state; // r16
    }
}

// 0x001CE0A0 - 0x001CE268 (0x1C8) zSaveLoad_LoadGame__Fv
signed int zSaveLoad_LoadGame() {
    { // 0x001CE0A0 - 0x001CE268
        signed int use_tgt; // r19
        enum en_XSGASYNC_STATUS asstat; // r18
        signed int rc; // r2
        signed int teststat; // r17
        signed int success; // r16
    }
}

// 0x001CE270 - 0x001CE4C8 (0x258) zSaveLoad_SaveGame__Fv
signed int zSaveLoad_SaveGame() {
    { // 0x001CE270 - 0x001CE4C8
        char *area; // r2
        char label[64]; // r29+0x90
        unsigned int progress; // r20
        signed int use_tgt; // r22
        enum en_XSGASYNC_STATUS asstat; // r18
        signed int rc; // r2
        signed int teststat; // r17
        signed int success; // r23
    }
}

// 0x001CE4D0 - 0x001CEA7C (0x5AC) zSaveLoad_GameSelect__Fi
signed int zSaveLoad_GameSelect(signed int mode) {
    { // 0x001CE4D0 - 0x001CEA7C
        signed int i; // r19
        signed int use_tgt; // r29+0xac
        class st_XSAVEGAME_DATA *svinst; // r22
        signed int done; // r2
    }
}

// 0x001CEA80 - 0x001CEC0C (0x18C) zSaveLoad_BuildName__FPci
void zSaveLoad_BuildName(char *name_txt, signed int idx) {
    { // 0x001CEA80 - 0x001CEC0C
        signed int i; // r19
        signed int counter; // r18
        char current_name[64]; // r29+0xa0
        char desired[64]; // r29+0x60
    }
}

// 0x001CEC10 - 0x001CEFF0 (0x3E0) zSaveLoad_CardPick__Fi
signed int zSaveLoad_CardPick(signed int mode) {
    { // 0x001CEC10 - 0x001CEFF0
        signed int done; // r2
    }
}

// 0x001CEFF0 - 0x001CF17C (0x18C) zSaveLoad_CardCheckSlotOverwrite__Fii
signed int zSaveLoad_CardCheckSlotOverwrite(signed int cardNumber, signed int gameNumber) {
    { // 0x001CEFF0 - 0x001CF17C
    }
}

// 0x001CF180 - 0x001CF298 (0x118) zSaveLoad_CardCheckSlotEmpty__Fii
signed int zSaveLoad_CardCheckSlotEmpty(signed int cardNumber, signed int gameNumber) {
    { // 0x001CF180 - 0x001CF298
    }
}

// 0x001CF2A0 - 0x001CF490 (0x1F0) zSaveLoad_CardCheckGames__Fii
signed int zSaveLoad_CardCheckGames(signed int cardNumber, signed int mode) {
    { // 0x001CF2A0 - 0x001CF490
        signed int tmp; // r2
    }
}

// 0x001CF490 - 0x001CF680 (0x1F0) zSaveLoad_CardCheckSpace__Fii
signed int zSaveLoad_CardCheckSpace(signed int cardNumber, signed int mode) {
    { // 0x001CF490 - 0x001CF680
        signed int tmp; // r2
    }
}

// 0x001CF680 - 0x001CF7D4 (0x154) zSaveLoad_CardCheckSlotEmpty_hasGame__Fii
signed int zSaveLoad_CardCheckSlotEmpty_hasGame(signed int num, signed int game) {
    { // 0x001CF680 - 0x001CF7D4
        signed int tgtslot; // r2
        signed int rc; // r17
        signed int tgtmax; // r29+0x5c
        signed int tgtcnt; // r2
        class st_XSAVEGAME_DATA *ldinst; // r16
    }
}

// 0x001CF7E0 - 0x001CF908 (0x128) zSaveLoad_CardCheckGamesSingle__Fi
signed int zSaveLoad_CardCheckGamesSingle(signed int num) {
    { // 0x001CF7E0 - 0x001CF908
        signed int tgtslot; // r2
        signed int rc; // r17
        signed int tgtmax; // r29+0x4c
        signed int tgtcnt; // r2
        class st_XSAVEGAME_DATA *ldinst; // r16
    }
}

// 0x001CF910 - 0x001CF9DC (0xCC) zSaveLoad_CardCheckSpaceSingle__Fi
signed int zSaveLoad_CardCheckSpaceSingle(signed int num) {
    { // 0x001CF910 - 0x001CF9DC
        signed int tgtslot; // r2
        signed int rc; // r17
        signed int tgtmax; // r29+0x4c
        signed int tgtcnt; // r2
        class st_XSAVEGAME_DATA *ldinst; // r16
    }
}

// 0x001CF9E0 - 0x001CFA88 (0xA8) zSaveLoad_CardCheckSpaceSingle_doCheck__FP17st_XSAVEGAME_DATAi
signed int zSaveLoad_CardCheckSpaceSingle_doCheck(class st_XSAVEGAME_DATA *xsgdata, signed int num) {
    { // 0x001CF9E0 - 0x001CFA88
        signed int rc; // r2
    }
}

// 0x001CFA90 - 0x001CFB74 (0xE4) zSaveLoad_CardCheckFormattedSingle__Fi
signed int zSaveLoad_CardCheckFormattedSingle(signed int num) {
    { // 0x001CFA90 - 0x001CFB74
        signed int tgtslot; // r2
        signed int rc; // r2
        signed int tgtmax; // r29+0x4c
        signed int tgtcnt; // r2
        class st_XSAVEGAME_DATA *ldinst; // r16
    }
}

// 0x001CFB80 - 0x001CFC2C (0xAC) zSaveLoad_CardCheckSingle__Fi
signed int zSaveLoad_CardCheckSingle(signed int num) {
    { // 0x001CFB80 - 0x001CFC2C
        signed int tgtslot; // r17
        signed int tgtmax; // r29+0x4c
        signed int tgtcnt; // r2
        class st_XSAVEGAME_DATA *ldinst; // r16
    }
}

// 0x001CFC30 - 0x001CFDC0 (0x190) zSaveLoad_CardPromptFormat__Fi
signed int zSaveLoad_CardPromptFormat(signed int mode) {
    { // 0x001CFC30 - 0x001CFDC0
    }
}

// 0x001CFDC0 - 0x001CFF50 (0x190) zSaveLoad_CardPrompt__Fi
signed int zSaveLoad_CardPrompt(signed int mode) {
    { // 0x001CFDC0 - 0x001CFF50
    }
}

// 0x001CFF50 - 0x001CFFE4 (0x94) CardtoTgt__Fi
signed int CardtoTgt(signed int card) {
    { // 0x001CFF50 - 0x001CFFE4
        signed int tgtmax; // r29+0x3c
        signed int tgtcnt; // r2
        class st_XSAVEGAME_DATA *ldinst; // r16
    }
}

// 0x001CFFF0 - 0x001D02D8 (0x2E8) format__Fii
signed int format(signed int num, signed int mode) {
    { // 0x001CFFF0 - 0x001D02D8
        signed int tgtslot; // r2
        signed int rc; // r16
        signed int tgtmax; // r29+0x6c
        signed int tgtcnt; // r2
    }
}

// 0x001D02E0 - 0x001D02E8 (0x8) zSaveLoad_getMCneeded__Fv
signed int zSaveLoad_getMCneeded() {
    { // 0x001D02E0 - 0x001D02E8
    }
}

// 0x001D02F0 - 0x001D02F8 (0x8) zSaveLoad_getMCavailable__Fv
signed int zSaveLoad_getMCavailable() {
    { // 0x001D02F0 - 0x001D02F8
    }
}

// 0x001D0300 - 0x001D0308 (0x8) zSaveLoad_getcard__Fv
signed int zSaveLoad_getcard() {
    { // 0x001D0300 - 0x001D0308
    }
}

// 0x001D0310 - 0x001D0318 (0x8) zSaveLoad_getgame__Fv
signed int zSaveLoad_getgame() {
    { // 0x001D0310 - 0x001D0318
    }
}

// 0x001D0320 - 0x001D04AC (0x18C) zSaveLoad_Tick__Fv
void zSaveLoad_Tick() {
    { // 0x001D0320 - 0x001D04AC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xCutscene.cpp
// ==========================================================================
void (SoundCutsceneCB)(class iSndCutsceneCallbackInfo *); // 0x001D04C0
unsigned char *giAnimScratch; // 0x003574A0
class zGlobals globals; // 0x00362FD0
float gCameraLastFov; // 0x00357E48
class _xMat4x3 g_I3; // 0x00342E70
unsigned int ourGlobals[4096]; // 0x002B5AD0
void (csDrawFakeEntity)(class _xEnt *); // 0x001D10D0
void (csAsyncReadCB)(signed int); // 0x001D04B0
static class _xEnt sCutsceneFakeEntity[8]; // 0x003ABE80
static class xCutsceneInfo *sCutTocInfo; // 0x003583A4
static unsigned int sCutTocCount; // 0x003583A0
static class xCutscene sActiveCutscene; // 0x003ABD40
// 0x001D04B0 - 0x001D04BC (0xC) csAsyncReadCB__Fi
static void csAsyncReadCB() {
    { // 0x001D04B0 - 0x001D04BC
    }
}

// 0x001D04C0 - 0x001D0558 (0x98) SoundCutsceneCB__FP24iSndCutsceneCallbackInfo
static void SoundCutsceneCB(class iSndCutsceneCallbackInfo *info) {
    { // 0x001D04C0 - 0x001D0558
        void *data; // r2
        unsigned int channel; // r2
    }
}

// 0x001D0560 - 0x001D0630 (0xD0) SoundGetData__FP9xCutsceneUiUi
static void *SoundGetData(class xCutscene *csn, unsigned int channel, unsigned int chunk) {
    { // 0x001D0560 - 0x001D0630
        unsigned int channelIndex; // r11
        unsigned int numData; // r10
        unsigned int dataIndex; // r9
        class xCutsceneData *data; // r8
        void *retdata; // r2
    }
}

// 0x001D0630 - 0x001D0F14 (0x8E4) xCutscene_Render__FP9xCutscenePP5_xEntPiPf
void xCutscene_Render(class xCutscene *csn, class _xEnt **alphaent, signed int *alphacnt, float *alphadist) {
    { // 0x001D0630 - 0x001D0F14
        class FakeEntityInfo *finfo; // r2
        class FakeEntityInfo *finfo; // r2
        unsigned int skipsize; // r4
        signed short weight[4]; // r29+0x11c0
        signed short *v_array[4]; // r29+0x11b0
        class xMorphTargetFile *mphFile; // r21
        class xCutsceneMphFrame *mphFrame; // r29+0x110
        unsigned int numFrame; // r20
        signed int matnum; // r4
        class RwTexture *tex; // r2
        class RpGeometry *geom; // r17
        unsigned int frameIndex; // r5
        unsigned int frameMax; // r9
        unsigned int frameMin; // r8
        unsigned int subIndex; // r7
        unsigned int *currvis; // r17
        unsigned int viscnt; // r2
        unsigned int tempSize; // r29+0x11cc
        class _xVec3 *camVec; // r29+0x100
        class RwMatrixTag animMat[65]; // r29+0x170
        class RpAtomic *model; // r19
        class xCutsceneData *mphdata; // r18
        class xCutsceneData *data; // r29+0xf0
        unsigned int backface; // r29+0xe0
        unsigned int hasAlpha; // r29+0xd0
        unsigned int fakeCount; // r23
        unsigned int visIdx; // r29+0xc0
        unsigned int visFlags; // r2
        unsigned int mphIndex; // r17
        unsigned int animIndex; // r2
        unsigned int dataIndex; // r29+0xb0
        unsigned int i; // r7
    }
}

// 0x001D0F20 - 0x001D10CC (0x1AC) csCalcAnimMatrices__FP11RwMatrixTagP8RpAtomicP16xCutsceneAnimHdrf
static void csCalcAnimMatrices(class RwMatrixTag *animMat, class RpAtomic *model, class xCutsceneAnimHdr *ahdr, float time) {
    { // 0x001D0F20 - 0x001D10CC
        class _xVec3 *ttt; // r19
        class _xQuat *qqq; // r18
        unsigned int boneidx; // r17
        unsigned int numbone; // r16
        class _xVec3 tranresult[65]; // r29+0x490
        class _xQuat quatresult[65]; // r29+0x80
    }
}

// 0x001D10D0 - 0x001D114C (0x7C) csDrawFakeEntity__FP5_xEnt
static void csDrawFakeEntity(class _xEnt *fakeent) {
    { // 0x001D10D0 - 0x001D114C
        class FakeEntityInfo *finfo; // r16
        class RwMatrixTag animMat[65]; // r29+0x20
    }
}

// 0x001D1150 - 0x001D13AC (0x25C) xCutscene_SetCamera__FP9xCutscene
void xCutscene_SetCamera(class xCutscene *csn) {
    { // 0x001D1150 - 0x001D13AC
        float lerp; // 
        float *auxdata; // r7
        void *afile; // r2
        class xCutsceneAnimHdr *ahdr; // r21
        class _xVec3 *tranresult; // r20
        class _xQuat *quatresult; // r19
        class _xMat4x3 camMat; // r29+0xa0
        float camFOV; // r29+0xe0
        unsigned int i; // r6
        unsigned int camAux; // r5
        unsigned int dataIndex; // r18
        class xCutsceneData *data; // r17
    }
}

// 0x001D13B0 - 0x001D15B0 (0x200) xCutscene_Update__FP9xCutscenef
signed int xCutscene_Update(class xCutscene *csn, float dt) {
    { // 0x001D13B0 - 0x001D15B0
    }
}

// 0x001D15B0 - 0x001D1A34 (0x484) xCutscene_LoadStep__FP9xCutscene
signed int xCutscene_LoadStep(class xCutscene *csn) {
    { // 0x001D15B0 - 0x001D1A34
        void *foundModel; // r2
        unsigned int tmpSize; // r29+0x4c
        unsigned int skipAccum; // r16
        enum XFILE_READSECTOR_STATUS cdstat; // r2
        unsigned int i; // r16
    }
}

// 0x001D1A40 - 0x001D1AC0 (0x80) xCutscene_LoadStart__FP9xCutscene
signed int xCutscene_LoadStart(class xCutscene *csn) {
    { // 0x001D1A40 - 0x001D1AC0
    }
}

// 0x001D1AC0 - 0x001D1BDC (0x11C) xCutscene_Destroy__FP9xCutscene
signed int xCutscene_Destroy(class xCutscene *csn) {
    { // 0x001D1AC0 - 0x001D1BDC
        enum XFILE_READSECTOR_STATUS cdstat; // r2
        unsigned int i; // r16
    }
}

// 0x001D1BE0 - 0x001D1D34 (0x154) xCutscene_Create__FUi
class xCutscene *xCutscene_Create(unsigned int id) {
    { // 0x001D1BE0 - 0x001D1D34
        unsigned int maxload; // r2
        class xCutsceneInfo *cnfo; // r16
        unsigned int i; // r3
        class xCutscene *csn; // r2
    }
}

// 0x001D1D40 - 0x001D1D98 (0x58) xCutscene_Init__FPv
void xCutscene_Init(void *toc) {
    { // 0x001D1D40 - 0x001D1D98
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zRenderState.cpp
// ==========================================================================
static signed int sBlendTable[11]; // 0x002B5170
static enum _SDRenderState sRS; // 0x003583B0
// 0x001D1DA0 - 0x001D1DBC (0x1C) zRenderStateSetDstBlendMode__Fi
void zRenderStateSetDstBlendMode(signed int xmode) {
    { // 0x001D1DA0 - 0x001D1DBC
    }
}

// 0x001D1DC0 - 0x001D1DDC (0x1C) zRenderStateSetSrcBlendMode__Fi
void zRenderStateSetSrcBlendMode(signed int xmode) {
    { // 0x001D1DC0 - 0x001D1DDC
    }
}

// 0x001D1DE0 - 0x001D1E30 (0x50) zRenderStateSetTexture__FUi
void zRenderStateSetTexture(unsigned int assetID) {
    { // 0x001D1DE0 - 0x001D1E30
        class RwRaster *raster; // r2
        class RwTexture *texture; // r2
    }
}

// 0x001D1E30 - 0x001D22E4 (0x4B4) zRenderState__F14_SDRenderState
void zRenderState(enum _SDRenderState next) {
    { // 0x001D1E30 - 0x001D22E4
    }
}

// 0x001D22F0 - 0x001D22F8 (0x8) zRenderStateInit__Fv
void zRenderStateInit() {
    { // 0x001D22F0 - 0x001D22F8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xQuickCull.cpp
// ==========================================================================
static float ucl_world_zsz; // 0x00358404
static float ucl_world_ysz; // 0x00358400
static float ucl_world_xsz; // 0x003583FC
static float lcl_world_zsz; // 0x003583F8
static float lcl_world_ysz; // 0x003583F4
static float lcl_world_xsz; // 0x003583F0
static float one_block_zsz; // 0x003583EC
static float one_block_ysz; // 0x003583E8
static float one_block_xsz; // 0x003583E4
static float block_zsz; // 0x003583E0
static float block_ysz; // 0x003583DC
static float block_xsz; // 0x003583D8
static float world_zsz; // 0x003583D4
static float world_ysz; // 0x003583D0
static float world_xsz; // 0x003583CC
static float world_zmax; // 0x003583C8
static float world_ymax; // 0x003583C4
static float world_xmax; // 0x003583C0
static float world_zmin; // 0x003583BC
static float world_ymin; // 0x003583B8
static float world_xmin; // 0x003583B4
// 0x001D2300 - 0x001D2464 (0x164) xQuickCullForRay__FP7xQCDataPC6_xRay3
void xQuickCullForRay(class xQCData *q, class _xRay3 *r) {
    { // 0x001D2300 - 0x001D2464
        class xQCData b; // r29+0x60
        class xQCData a; // r29+0x50
        float len; // 
        class _xLine3 ln; // r29+0x30
    }
}

// 0x001D2470 - 0x001D250C (0x9C) xQuickCullCellMerge__FP7xQCDataPC7xQCDataPC7xQCData
static void xQuickCullCellMerge(class xQCData *o, class xQCData *a, class xQCData *b) {
    { // 0x001D2470 - 0x001D250C
    }
}

// 0x001D2510 - 0x001D25A8 (0x98) xQuickCullCellForVec__FP7xQCDataPC6_xVec3
static void xQuickCullCellForVec(class xQCData *c, class _xVec3 *v) {
    { // 0x001D2510 - 0x001D25A8
    }
}

// 0x001D25B0 - 0x001D2724 (0x174) xQuickCullForBound__FP7xQCDataPC7_xBound
void xQuickCullForBound(class xQCData *q, class _xBound *b) {
    { // 0x001D25B0 - 0x001D2724
    }
}

// 0x001D2730 - 0x001D2760 (0x30) xQuickCullIsects__FPC7xQCDataPC7xQCData
signed int xQuickCullIsects(class xQCData *a, class xQCData *b) {
    { // 0x001D2730 - 0x001D2760
    }
}

// 0x001D2760 - 0x001D277C (0x1C) xQuickCullInit__FPC5_xBox
void xQuickCullInit(class _xBox *box) {
    { // 0x001D2760 - 0x001D277C
    }
}

// 0x001D2780 - 0x001D28B4 (0x134) xQuickCullInit__Fffffff
void xQuickCullInit(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax) {
    { // 0x001D2780 - 0x001D28B4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zParSys.cpp
// ==========================================================================
signed int (zParSysEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001D2C40
unsigned int gActiveHeap; // 0x00357CAC
class zGlobals globals; // 0x00362FD0
static class zParSysInfo sParSysInfo[7]; // 0x002B51A0
// 0x001D28C0 - 0x001D297C (0xBC) zParSysRender__FP5xBase
void zParSysRender(class xBase *b) {
    { // 0x001D28C0 - 0x001D297C
        class zParSys *s; // r17
        class _zParGroup *g; // r16
    }
}

// 0x001D2980 - 0x001D2AC8 (0x148) zParGroupUpdateR__FP7zParSysP10_zParGroupf
static void zParGroupUpdateR(class zParSys *s, class _zParGroup *g, float dt) {
    { // 0x001D2980 - 0x001D2AC8
        void (*func)(class zParCmd *, class xParGroup *, float); // r2
        class zParCmd *cmd; // r18
        signed int i; // r17
    }
}

// 0x001D2AD0 - 0x001D2C40 (0x170) zParSysUpdate__FP5xBaseP7_xScenef
void zParSysUpdate(class xBase *to, float dt) {
    { // 0x001D2AD0 - 0x001D2C40
        class _zParGroup *g; // r18
        class zParSys *parent; // r17
        class zParSys *s; // r16
    }
}

// 0x001D2C40 - 0x001D2D8C (0x14C) zParSysEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zParSysEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001D2C40 - 0x001D2D8C
        class zParSys *t; // r16
    }
}

// 0x001D2D90 - 0x001D2DCC (0x3C) zParSysExit__FP7zParSys
void zParSysExit(class zParSys *t) {
    { // 0x001D2D90 - 0x001D2DCC
    }
}

// 0x001D2DD0 - 0x001D2E1C (0x4C) zParSysSetup__FP7zParSys
void zParSysSetup(class zParSys *t) {
    { // 0x001D2DD0 - 0x001D2E1C
    }
}

// 0x001D2E20 - 0x001D30D4 (0x2B4) zParSysInit__FP5xBaseP12zParSysAsset
void zParSysInit(class xBase *b, class zParSysAsset *tasset) {
    { // 0x001D2E20 - 0x001D30D4
        signed int i; // r5
        signed int i; // r17
        unsigned char *cmdPtr; // r16
        class zParSys *t; // r2
    }
}

// 0x001D30E0 - 0x001D30E8 (0x8) zParSysInit__FPvPv
void zParSysInit(void *b, void *tasset) {
    { // 0x001D30E0 - 0x001D30E8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zParCmd.cpp
// ==========================================================================
float cosSinPolynomial[0]; // 0x0028A180
class _tagClimate gClimate; // 0x0036A4F0
class zGlobals globals; // 0x00362FD0
static signed int sClipVolumeTotal; // 0x00358408
static class _zVolume *sClipVolume[32]; // 0x003AC5D0
static class zCmdInfo sCmdInfo[29]; // 0x002B51E0
// 0x001D30F0 - 0x001D32C0 (0x1D0) xParCmdCustom_Update__FP7zParCmdP9xParGroupf
void xParCmdCustom_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D30F0 - 0x001D32C0
        float oorSqr; // 
        float rSqr; // 
        float mdt; // 
        class xPar *p; // r16
    }
}

// 0x001D32C0 - 0x001D33B8 (0xF8) xParCmdCustom_Grass_Update__FP7zParCmdP9xParGroupf
void xParCmdCustom_Grass_Update(class xParGroup *ps, float dt) {
    { // 0x001D32C0 - 0x001D33B8
        class xPar *p; // r4
    }
}

// 0x001D33C0 - 0x001D37EC (0x42C) xParCmdTexAnim_Update__FP7zParCmdP9xParGroupf
void xParCmdTexAnim_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D33C0 - 0x001D37EC
        class xParCmdTex *tex; // r17
        class xParCmdTexAnim *cmd; // r3
        class xPar *p; // r16
    }
}

// 0x001D37F0 - 0x001D37F8 (0x8) xParCmdTex_Update__FP7zParCmdP9xParGroupf
void xParCmdTex_Update() {
    { // 0x001D37F0 - 0x001D37F8
    }
}

// 0x001D3800 - 0x001D39A4 (0x1A4) xParCmdJet_Update__FP7zParCmdP9xParGroupf
void xParCmdJet_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D3800 - 0x001D39A4
        float oorSqr; // 
        float rSqr; // 
        float mdt; // 
        class xParCmdJet *cmd; // r16
        class xPar *p; // r17
    }
}

// 0x001D39B0 - 0x001D3AF0 (0x140) xParCmdRotateAround_Update__FP7zParCmdP9xParGroupf
void xParCmdRotateAround_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D39B0 - 0x001D3AF0
        class _xVec3 out_offset; // r29+0xd0
        class _xVec3 offset; // r29+0xc0
        class _xMat3x3 rotmat; // r29+0x70
        class _xVec3 angles; // r29+0xb0
        float radius; // 
        class _xMat3x3 lookmat; // r29+0x40
        class _xVec3 at; // r29+0xa0
        float radius_growth; // 
        float yaw; // 
        class xParCmdRotateAround *cmd; // r17
        class xPar *p; // r16
    }
}

// 0x001D3AF0 - 0x001D3C58 (0x168) xParCmdDamagePlayer_Update__FP7zParCmdP9xParGroupf
void xParCmdDamagePlayer_Update(class xParGroup *ps) {
    { // 0x001D3AF0 - 0x001D3C58
        signed int i; // r4
        class _xCollis collis; // r29+0x40
        signed int i; // r4
        class _xBound *pbound; // r17
        class xPar *p; // r16
        static signed int last_idx; // 0x0035840C
        static signed char init; // 0x00358410
    }
}

// 0x001D3C60 - 0x001D3DFC (0x19C) xParCmdAnimalMagentism_Update__FP7zParCmdP9xParGroupf
void xParCmdAnimalMagentism_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D3C60 - 0x001D3DFC
        float dist; // r29+0x70
        class _xVec3 vec; // r29+0x60
        float mul; // 
        class _xVec3 pos; // r29+0x50
        class xPar *p; // r16
    }
}

// 0x001D3E00 - 0x001D3F60 (0x160) xParCmdApplyCamMat_Update__FP7zParCmdP9xParGroupf
void xParCmdApplyCamMat_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D3E00 - 0x001D3F60
        float mul; // 
        class _xMat4x3 *mat; // r6
        class xParCmdApplyCamMat *cmd; // r4
        class xPar *p; // r3
    }
}

// 0x001D3F60 - 0x001D3FB0 (0x50) xParCmdVelocityApply_Update__FP7zParCmdP9xParGroupf
void xParCmdVelocityApply_Update(class xParGroup *ps) {
    { // 0x001D3F60 - 0x001D3FB0
        class xPar *p; // r3
    }
}

// 0x001D3FB0 - 0x001D4174 (0x1C4) xParCmdRotPar_Update__FP7zParCmdP9xParGroupf
void xParCmdRotPar_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D3FB0 - 0x001D4174
        class xParCmdRotPar *cmd; // r4
        class xPar *p; // r16
    }
}

// 0x001D4180 - 0x001D4230 (0xB0) xParCmdApplyWind_Update__FP7zParCmdP9xParGroupf
void xParCmdApplyWind_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4180 - 0x001D4230
        class _xVec3 wind_vec; // r29+0x40
        class xParCmdApplyWind *cmd; // r16
        class xPar *p; // r3
    }
}

// 0x001D4230 - 0x001D4474 (0x244) xParCmdRandomVelocityPar_Update__FP7zParCmdP9xParGroupf
void xParCmdRandomVelocityPar_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4230 - 0x001D4474
        unsigned int uvelz; // r2
        unsigned int uvely; // r2
        unsigned int uvelx; // r2
        class _xVec3 *velvec; // r2
        class _xVec3 rot; // r29+0x40
        class xParCmdRandomVelocityPar *cmd; // r3
        class xPar *p; // r3
    }
}

// 0x001D4480 - 0x001D45D8 (0x158) xParCmdPlayerCollision_Update__FP7zParCmdP9xParGroupf
void xParCmdPlayerCollision_Update(class zParCmd *c, class xParGroup *ps) {
    { // 0x001D4480 - 0x001D45D8
        class _xRay3 ray; // r29+0x40
        float t; // 
        class xParCmdPlayerCollision *cmd; // r17
        class xPar *p; // r16
    }
}

// 0x001D45E0 - 0x001D46E4 (0x104) xParCmdAlpha3rdPolyReg_Update__FP7zParCmdP9xParGroupf
void xParCmdAlpha3rdPolyReg_Update(class zParCmd *c, class xParGroup *ps) {
    { // 0x001D45E0 - 0x001D46E4
        float alpha; // r29+0x30
        float t2; // 
        float t; // 
        class xParCmdAlpha3rdPolyReg *cmd; // r17
        class xPar *p; // r16
    }
}

// 0x001D46F0 - 0x001D47A8 (0xB8) xParCmdClipVolumes_Update__FP7zParCmdP9xParGroupf
void xParCmdClipVolumes_Update(class xParGroup *ps) {
    { // 0x001D46F0 - 0x001D47A8
        class _xCollis collis; // r29+0x60
        signed int i; // r17
        class xPar *p; // r16
    }
}

// 0x001D47B0 - 0x001D48E0 (0x130) xParCmdScale_Update__FP7zParCmdP9xParGroupf
void xParCmdScale_Update(class zParCmd *c, class xParGroup *ps) {
    { // 0x001D47B0 - 0x001D48E0
        float t2; // 
        float t; // 
        float a3; // 
        float a2; // 
        float a; // 
        float constant; // 
        class xPar *p; // r4
    }
}

// 0x001D48E0 - 0x001D4A60 (0x180) xParCmdSmokeAlpha_Update__FP7zParCmdP9xParGroupf
void xParCmdSmokeAlpha_Update(class zParCmd *c, class xParGroup *ps) {
    { // 0x001D48E0 - 0x001D4A60
        float alpha; // r29+0x30
        float t2; // 
        float t; // 
        float a3; // 
        float a2; // 
        float a; // 
        float constant; // 
        class xPar *p; // r16
    }
}

// 0x001D4A60 - 0x001D4B18 (0xB8) xParCmdScale3rdPolyReg_Update__FP7zParCmdP9xParGroupf
void xParCmdScale3rdPolyReg_Update(class zParCmd *c, class xParGroup *ps) {
    { // 0x001D4A60 - 0x001D4B18
        float t2; // 
        float t; // r29+0x0
        class xParCmdScale3rdPolyReg *cmd; // r6
        class xPar *p; // r4
    }
}

// 0x001D4B20 - 0x001D4BC8 (0xA8) xParCmdMoveRandomPar_Update__FP7zParCmdP9xParGroupf
void xParCmdMoveRandomPar_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4B20 - 0x001D4BC8
        class xParCmdMoveRandomPar *cmd; // r4
        class xPar *p; // r16
    }
}

// 0x001D4BD0 - 0x001D4CCC (0xFC) xParCmdMoveRandom_Update__FP7zParCmdP9xParGroupf
void xParCmdMoveRandom_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4BD0 - 0x001D4CCC
        class xParCmdMove *cmd; // r16
        class xPar *p; // r4
    }
}

// 0x001D4CD0 - 0x001D4D30 (0x60) xParCmdMove_Update__FP7zParCmdP9xParGroupf
void xParCmdMove_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4CD0 - 0x001D4D30
        class xParCmdMove *cmd; // r4
        class xPar *p; // r3
    }
}

// 0x001D4D30 - 0x001D4D90 (0x60) xParCmdAccelerate_Update__FP7zParCmdP9xParGroupf
void xParCmdAccelerate_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4D30 - 0x001D4D90
        class xParCmdAccelerate *cmd; // r4
        class xPar *p; // r3
    }
}

// 0x001D4D90 - 0x001D4EE0 (0x150) xParCmdOrbitLine_Update__FP7zParCmdP9xParGroupf
void xParCmdOrbitLine_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4D90 - 0x001D4EE0
        float force; // 
        float rSqr; // r29+0x50
        float mdt; // 
        class xParCmdOrbitLine *cmd; // r17
        class xPar *p; // r16
    }
}

// 0x001D4EE0 - 0x001D4FE8 (0x108) xParCmdOrbitPoint_Update__FP7zParCmdP9xParGroupf
void xParCmdOrbitPoint_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4EE0 - 0x001D4FE8
        float force; // 
        float rSqr; // r29+0x50
        float mdt; // 
        class xParCmdOrbitPoint *cmd; // r17
        class xPar *p; // r16
    }
}

// 0x001D4FF0 - 0x001D50EC (0xFC) xParCmdFollow_Update__FP7zParCmdP9xParGroupf
void xParCmdFollow_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D4FF0 - 0x001D50EC
        float force; // 
        float lenSqr; // r29+0x40
        float mdt; // 
        class xParCmdFollow *cmd; // r17
        class xPar *p; // r16
    }
}

// 0x001D50F0 - 0x001D5128 (0x38) xParCmdAge_Update__FP7zParCmdP9xParGroupf
void xParCmdAge_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D50F0 - 0x001D5128
        float age_rate; // 
        class xPar *p; // r4
    }
}

// 0x001D5130 - 0x001D51E8 (0xB8) xParCmdKillSlow_Update__FP7zParCmdP9xParGroupf
void xParCmdKillSlow_Update(class zParCmd *c, class xParGroup *ps, float dt) {
    { // 0x001D5130 - 0x001D51E8
        float speedLimit; // 
        class xParCmdKillSlow *cmd; // r3
        class xPar *p; // r4
    }
}

// 0x001D51F0 - 0x001D52E0 (0xF0) xParCmdKillDistance_Update__FP7zParCmdP9xParGroupf
void xParCmdKillDistance_Update(class zParCmd *c, class xParGroup *ps) {
    { // 0x001D51F0 - 0x001D52E0
        class _xVec3 *camera_pos; // r5
        class xParCmdKillDistance *cmd; // r6
        class xPar *p; // r4
    }
}

// 0x001D52E0 - 0x001D532C (0x4C) zParCmdGetUpdateFunc__FUi
void (*zParCmdGetUpdateFunc(unsigned int parType))(class zParCmd *, class xParGroup *, float) {
    { // 0x001D52E0 - 0x001D532C
        signed int i; // r3
    }
}

// 0x001D5330 - 0x001D537C (0x4C) zParCmdGetSize__FUi
unsigned int zParCmdGetSize(unsigned int parType) {
    { // 0x001D5330 - 0x001D537C
        signed int i; // r3
    }
}

// 0x001D5380 - 0x001D541C (0x9C) zParCmdFindClipVolumes__Fv
void zParCmdFindClipVolumes() {
    { // 0x001D5380 - 0x001D541C
        class _zVolume *vol; // r2
        char findname[64]; // r29+0x30
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xParGroup.cpp
// ==========================================================================
class xParGroup *sParGroupRegTable[255]; // 0x003AC6A0
static signed int sParGroupRegTableCount; // 0x00358418
static signed int sParGroupRegTableInit; // 0x00358414
// 0x001D5420 - 0x001D54E0 (0xC0) xParGroupKillPar__FP9xParGroupP4xPar
void xParGroupKillPar(class xParGroup *ps, class xPar *p) {
    { // 0x001D5420 - 0x001D54E0
    }
}

// 0x001D54E0 - 0x001D564C (0x16C) xParGroupAddPar__FP9xParGroup
class xPar *xParGroupAddPar(class xParGroup *ps) {
    { // 0x001D54E0 - 0x001D564C
        unsigned char i; // r5
        signed int myPriority; // r4
        class xPar *p; // r2
    }
}

// 0x001D5650 - 0x001D577C (0x12C) xParGroupAnimate__FP9xParGroupf
void xParGroupAnimate(class xParGroup *ps, float dt) {
    { // 0x001D5650 - 0x001D577C
        float age; // 
        class xPar *tmp; // r2
        class xPar *i; // r2
    }
}

// 0x001D5780 - 0x001D57CC (0x4C) xParGroupKillAllParticles__FP9xParGroup
void xParGroupKillAllParticles(class xParGroup *ps) {
    { // 0x001D5780 - 0x001D57CC
        class xPar *tmp; // r2
        class xPar *i; // r16
    }
}

// 0x001D57D0 - 0x001D57D8 (0x8) xParGroupSetActive__FP9xParGroupUi
void xParGroupSetActive(class xParGroup *ps, unsigned int isActive) {
    { // 0x001D57D0 - 0x001D57D8
    }
}

// 0x001D57E0 - 0x001D5838 (0x58) xParGroupUnregister__FP9xParGroup
void xParGroupUnregister(class xParGroup *ps) {
    { // 0x001D57E0 - 0x001D5838
        unsigned char i; // r5
    }
}

// 0x001D5840 - 0x001D5928 (0xE8) xParGroupRegister__FP9xParGroup
void xParGroupRegister(class xParGroup *ps) {
    { // 0x001D5840 - 0x001D5928
        unsigned char i; // r5
    }
}

// 0x001D5930 - 0x001D5938 (0x8) xParGroupSetPriority__FP9xParGroupUc
void xParGroupSetPriority(class xParGroup *ps, unsigned char val) {
    { // 0x001D5930 - 0x001D5938
    }
}

// 0x001D5940 - 0x001D5948 (0x8) xParGroupSetVisibility__FP9xParGroupi
void xParGroupSetVisibility(class xParGroup *ps, signed int vis) {
    { // 0x001D5940 - 0x001D5948
    }
}

// 0x001D5950 - 0x001D5984 (0x34) xParGroupSetStealing__FP9xParGroupi
void xParGroupSetStealing(class xParGroup *ps, signed int stealing) {
    { // 0x001D5950 - 0x001D5984
    }
}

// 0x001D5990 - 0x001D59C4 (0x34) xParGroupSetBack2Life__FP9xParGroupi
void xParGroupSetBack2Life(class xParGroup *ps, signed int b2l) {
    { // 0x001D5990 - 0x001D59C4
    }
}

// 0x001D59D0 - 0x001D5A04 (0x34) xParGroupSetAging__FP9xParGroupi
void xParGroupSetAging(class xParGroup *ps, signed int age) {
    { // 0x001D59D0 - 0x001D5A04
    }
}

// 0x001D5A10 - 0x001D5A5C (0x4C) xParGroupInit__FP9xParGroup
void xParGroupInit(class xParGroup *ps) {
    { // 0x001D5A10 - 0x001D5A5C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zParEmitter.cpp
// ==========================================================================
signed int (zParEmitterEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001D74F0
class zGlobals globals; // 0x00362FD0
static class _zParEmitterCustomSettings sSaveEmmiterSettings; // 0x003ACAA0
// 0x001D5A60 - 0x001D5ACC (0x6C) zParEmitterFind__FPc
class _zParEmitter *zParEmitterFind(char *asset_name) {
    { // 0x001D5A60 - 0x001D5ACC
        unsigned int asset_id; // r2
    }
}

// 0x001D5AD0 - 0x001D5B24 (0x54) zParEmitterFind__FUi
class _zParEmitter *zParEmitterFind(unsigned int asset_id) {
    { // 0x001D5AD0 - 0x001D5B24
        signed int i; // r2
        class _zParEmitter *pe; // r2
        class zScene *s; // r3
    }
}

// 0x001D5B30 - 0x001D5B80 (0x50) zParEmitterUpdate__FP5xBaseP7_xScenef
void zParEmitterUpdate(class xBase *to, float dt) {
    { // 0x001D5B30 - 0x001D5B80
        class _zParEmitter *pe; // r16
    }
}

// 0x001D5B80 - 0x001D6734 (0xBB4) zParEmitterEmit__FP12_zParEmitterf
class xPar *zParEmitterEmit(class _zParEmitter *pe, float dt) {
    { // 0x001D5B80 - 0x001D6734
        signed int i; // r30
        unsigned int get_rnd_group_idx; // r17
        class _zGroup *g; // r29+0x120
        signed int marker; // r18
        class xBase *emitObj; // r2
        signed int emitAgain; // r29+0x110
        signed int attachGroupTotal; // r29+0x100
        signed int attachGroupIndex; // r2
        class xBase *attachObject; // r2
        signed int sanity; // r2
        unsigned int count; // r29+0xf0
        float size_birth; // 
        float life; // 
        class xPar *p; // r17
        class zParSys *ps; // r29+0xe0
        class zParEmitterAsset *a; // r16
        class xPar *last_p; // r29+0xd0
    }
}

// 0x001D6740 - 0x001D6B54 (0x414) zParEmitterEmitCustom__FP12_zParEmitterfP26_zParEmitterCustomSettings
class xPar *zParEmitterEmitCustom(class _zParEmitter *p, float dt, class _zParEmitterCustomSettings *info) {
    { // 0x001D6740 - 0x001D6B54
        class xPar *emitted_par; // r16
        class _zParEmitterCustomSettings *s; // r18
        class zParEmitterAsset *pe; // r17
        unsigned int flags; // r16
    }
}

// 0x001D6B60 - 0x001D6EE4 (0x384) zParEmitterEmitSphereEdge__FP4xParP16zParEmitterAssetfi
static void zParEmitterEmitSphereEdge(class xPar *p, class zParEmitterAsset *a, float dt, signed int subtype) {
    { // 0x001D6B60 - 0x001D6EE4
        class _xMat3x3 mat_rot; // r29+0x40
        class _xVec3 offset; // r29+0xb0
        class _xVec3 dirvec; // r29+0xa0
    }
}

// 0x001D6EF0 - 0x001D7170 (0x280) zParEmitterEmitCircle__FP4xParP16zParEmitterAssetf
static void zParEmitterEmitCircle(class xPar *p, class zParEmitterAsset *a, float dt) {
    { // 0x001D6EF0 - 0x001D7170
        float dt_radius; // 
        class _xVec2 vec2; // r29+0xb8
        class _xMat2x2 rot_mat; // r29+0x70
        float st; // r29+0xd0
        float ct; // 
        float rot_amount; // 
    }
}

// 0x001D7170 - 0x001D73D8 (0x268) zParEmitterEmitCircleEdge__FP4xParP16zParEmitterAssetf
static void zParEmitterEmitCircleEdge(class xPar *p, class zParEmitterAsset *a, float dt) {
    { // 0x001D7170 - 0x001D73D8
        float dt_radius; // 
        class _xVec2 vec2; // r29+0x98
        class _xMat2x2 rot_mat; // r29+0x50
        float st; // r29+0xb0
        float ct; // 
        float rot_amount; // 
    }
}

// 0x001D73E0 - 0x001D74E8 (0x108) zParEmitterAngleVariation__FP4xParP16zParEmitterAsset
static void zParEmitterAngleVariation(class xPar *p, class zParEmitterAsset *a) {
    { // 0x001D73E0 - 0x001D74E8
        float ang[3]; // r29+0x70
        float ang_debrisCone; // 
        class _xMat3x3 mat_rot; // r29+0x40
    }
}

// 0x001D74F0 - 0x001D75B8 (0xC8) zParEmitterEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zParEmitterEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001D74F0 - 0x001D75B8
        class _zParEmitterCustomSettings fake_settings; // r29+0x20
        class _zParEmitter *t; // r16
    }
}

// 0x001D75C0 - 0x001D75DC (0x1C) zParEmitterSetup__FP12_zParEmitter
void zParEmitterSetup(class _zParEmitter *t) {
    { // 0x001D75C0 - 0x001D75DC
    }
}

// 0x001D75E0 - 0x001D76EC (0x10C) zParEmitterInit__FP5xBaseP16zParEmitterAsset
void zParEmitterInit(class xBase *b, class zParEmitterAsset *tasset) {
    { // 0x001D75E0 - 0x001D76EC
        class _xEnt *ent; // r2
        class _zParEmitter *t; // r2
    }
}

// 0x001D76F0 - 0x001D76F8 (0x8) zParEmitterInit__FPvPv
void zParEmitterInit(void *b, void *tasset) {
    { // 0x001D76F0 - 0x001D76F8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zCutsceneMgr.cpp
// ==========================================================================
signed int (zCutsceneMgrEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001D7BD0
class RpAtomic * (HackBoundCB)(class RpAtomic *, void *); // 0x001D7F50
class zGlobals globals; // 0x00362FD0
static signed int gCutsceneSkipOK; // 0x0035841C
static class zCutSceneNames zCutSceneNamesTable[14]; // 0x002B5540
static class zCutsceneBoundHack boundHackTable[12]; // 0x002B54E0
static class zCutsceneZbufferHack zhacktable[5]; // 0x002B5380
// 0x001D7700 - 0x001D7724 (0x24) zCutSceneNamesTable_getStatus__FUi
unsigned int zCutSceneNamesTable_getStatus(unsigned int number) {
    { // 0x001D7700 - 0x001D7724
    }
}

// 0x001D7730 - 0x001D7A6C (0x33C) zCutsceneMgrUpdate__FP5xBaseP7_xScenef
void zCutsceneMgrUpdate(class xBase *to, float dt) {
    { // 0x001D7730 - 0x001D7A6C
        signed int i; // r17
        signed int pauseValue; // r17
        class _zCutsceneMgr *t; // r2
    }
}

// 0x001D7A70 - 0x001D7B40 (0xD0) zCutsceneMgrUpdateFX__FP13_zCutsceneMgrf
static void zCutsceneMgrUpdateFX(class _zCutsceneMgr *t) {
    { // 0x001D7A70 - 0x001D7B40
        class _zParEmitter *e; // r2
        signed int i; // r17
        class xCutscene *csn; // r16
        class zCutsceneMgrAsset *a; // r2
    }
}

// 0x001D7B40 - 0x001D7BC8 (0x88) zCutsceneMgrFinishLoad__FP5xBase
void zCutsceneMgrFinishLoad(class xBase *to) {
    { // 0x001D7B40 - 0x001D7BC8
        class _zCutsceneMgr *t; // r2
    }
}

// 0x001D7BD0 - 0x001D7ED0 (0x300) zCutsceneMgrEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zCutsceneMgrEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001D7BD0 - 0x001D7ED0
        signed int i; // r17
        unsigned int hackid; // r19
        unsigned int j; // r18
        unsigned int i; // r17
        unsigned int i; // r17
        signed int i; // r17
        class _zCutsceneMgr *t; // r16
    }
}

// 0x001D7ED0 - 0x001D7F48 (0x78) zCutSceneNamesTable_clearAll__Fv
void zCutSceneNamesTable_clearAll() {
    { // 0x001D7ED0 - 0x001D7F48
        signed int i; // r4
    }
}

// 0x001D7F50 - 0x001D7F78 (0x28) HackBoundCB__FP8RpAtomicPv
static class RpAtomic *HackBoundCB(class RpAtomic *atomic, void *data) {
    { // 0x001D7F50 - 0x001D7F78
    }
}

// 0x001D7F80 - 0x001D7FDC (0x5C) zCutsceneMgrLoad__FP13_zCutsceneMgrP7xSerial
void zCutsceneMgrLoad(class xSerial *s) {
    { // 0x001D7F80 - 0x001D7FDC
        signed int i; // r16
    }
}

// 0x001D7FE0 - 0x001D8064 (0x84) zCutsceneMgrSave__FP13_zCutsceneMgrP7xSerial
void zCutsceneMgrSave(class xSerial *s) {
    { // 0x001D7FE0 - 0x001D8064
        signed int i; // r16
    }
}

// 0x001D8070 - 0x001D80D4 (0x64) zCutsceneMgrInit__FPvPv
void zCutsceneMgrInit(void *b, void *tasset) {
    { // 0x001D8070 - 0x001D80D4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xPartition.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
// 0x001D80E0 - 0x001D81A4 (0xC4) xPartitionGetLoop__FP13_tagPartitioniPiPiPiiiiPiPiPi
void xPartitionGetLoop(class _tagPartition *p, signed int step, signed int *xi, signed int *yi, signed int *zi, signed int xmid, signed int ymid, signed int zmid, signed int *xend, signed int *yend, signed int *zend) {
    { // 0x001D80E0 - 0x001D81A4
    }
}

// 0x001D81B0 - 0x001D81B8 (0x8) xPartitionDump__FP13_tagPartitionPc
void xPartitionDump() {
    { // 0x001D81B0 - 0x001D81B8
    }
}

// 0x001D81C0 - 0x001D824C (0x8C) xPartitionGetVirtualIdx__FP13_tagPartitionP6_xVec3PiPiPi
void xPartitionGetVirtualIdx(class _tagPartition *part, class _xVec3 *insert_pos, signed int *x, signed int *y, signed int *z) {
    { // 0x001D81C0 - 0x001D824C
    }
}

// 0x001D8250 - 0x001D8268 (0x18) xPartitionGetSpace__FP13_tagPartitioni
class _tagPartSpace *xPartitionGetSpace(class _tagPartition *p, signed int true_idx) {
    { // 0x001D8250 - 0x001D8268
    }
}

// 0x001D8270 - 0x001D82F4 (0x84) xPartitionSpaceMove__FP13_tagPartSpaceP13_tagPartSpaceUi
void xPartitionSpaceMove(class _tagPartSpace *src, class _tagPartSpace *dest, unsigned int data) {
    { // 0x001D8270 - 0x001D82F4
        class _tagPartLink *src_pre; // r9
        class _tagPartLink *src_lnk; // r8
        class _tagPartLink *dest_lnk; // r7
    }
}

// 0x001D8300 - 0x001D8470 (0x170) xPartitionUpdate__FP13_tagPartitionPviP6_xVec3
signed int xPartitionUpdate(class _tagPartition *part, void *data, signed int old_idx, class _xVec3 *current_pos) {
    { // 0x001D8300 - 0x001D8470
        class _tagPartSpace *dest; // r2
        class _tagPartSpace *src; // r6
        signed int cur_idx; // r2
    }
}

// 0x001D8470 - 0x001D863C (0x1CC) xPartitionInsert__FP13_tagPartitionPvP6_xVec3
signed int xPartitionInsert(class _tagPartition *part, void *insert_data, class _xVec3 *insert_pos) {
    { // 0x001D8470 - 0x001D863C
    }
}

// 0x001D8640 - 0x001D8818 (0x1D8) xPartitionWorld__FP13_tagPartitionP5_xEnviii
void xPartitionWorld(class _tagPartition *part, class _xEnv *env, signed int x_spaces, signed int y_spaces, signed int z_spaces) {
    { // 0x001D8640 - 0x001D8818
        signed int x; // r22
        signed int y; // r21
        signed int z; // r20
        float dz; // 
        float dy; // 
        float dx; // 
        class _xBox *bb; // r2
    }
}

// 0x001D8820 - 0x001D89F8 (0x1D8) xPartitionVolume__FP13_tagPartitionP8_zVolumeiii
void xPartitionVolume(class _tagPartition *part, class _zVolume *volume, signed int x_spaces, signed int y_spaces, signed int z_spaces) {
    { // 0x001D8820 - 0x001D89F8
        signed int x; // r22
        signed int y; // r21
        signed int z; // r20
        float dz; // 
        float dy; // 
        float dx; // 
        class _xBox *bb; // r2
        class _xBound *xb; // r2
    }
}

// 0x001D8A00 - 0x001D8A20 (0x20) xPartitionGetTrueIdx__FP13_tagPartitioniii
signed int xPartitionGetTrueIdx(class _tagPartition *part, signed int x, signed int y, signed int z) {
    { // 0x001D8A00 - 0x001D8A20
        signed int idx; // r2
    }
}

// 0x001D8A20 - 0x001D8A28 (0x8) xPartitionReset__Fv
void xPartitionReset() {
    { // 0x001D8A20 - 0x001D8A28
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEGenerator.cpp
// ==========================================================================
void (zEGenerator_Update)(class zEGenerator *, class _xScene *, float); // 0x001D90C0
signed int (zEGeneratorEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x001D8A30
void (zEGenerator_Render)(class zEGenerator *); // 0x001D90B0
class zGlobals globals; // 0x00362FD0
// 0x001D8A30 - 0x001D8BFC (0x1CC) zEGeneratorEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zEGeneratorEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x001D8A30 - 0x001D8BFC
        class zEGenerator *egen; // r2
    }
}

// 0x001D8C00 - 0x001D8C7C (0x7C) zEGenerator_TurnOff__FP11zEGenerator
void zEGenerator_TurnOff(class zEGenerator *egen) {
    { // 0x001D8C00 - 0x001D8C7C
        signed int i; // r16
    }
}

// 0x001D8C80 - 0x001D90A4 (0x424) zEGenerator_TurnOn__FP11zEGenerator
void zEGenerator_TurnOn(class zEGenerator *egen) {
    { // 0x001D8C80 - 0x001D90A4
        class _tagLightningAdd add; // r29+0xa0
        signed int i; // r16
        class xBase *b; // r19
        class p2LinkAsset *link; // r18
        signed int i; // r17
        unsigned short imp; // r16
        unsigned short itgt; // r2
        class zEGenAsset *zasset; // r5
    }
}

// 0x001D90B0 - 0x001D90B8 (0x8) zEGenerator_Render__FP11zEGenerator
void zEGenerator_Render(class zEGenerator *egen) {
    { // 0x001D90B0 - 0x001D90B8
    }
}

// 0x001D90C0 - 0x001D91F8 (0x138) zEGenerator_Update__FP11zEGeneratorP7_xScenef
void zEGenerator_Update(class zEGenerator *egen, class _xScene *sc, float dt) {
    { // 0x001D90C0 - 0x001D91F8
    }
}

// 0x001D9200 - 0x001D92D0 (0xD0) zEGenerator_Reset__FP11zEGeneratorP7_xScene
void zEGenerator_Reset(class zEGenerator *egen) {
    { // 0x001D9200 - 0x001D92D0
        class zEGenAsset *zasset; // r16
    }
}

// 0x001D92D0 - 0x001D92D8 (0x8) zEGenerator_Load__FP11zEGeneratorP7xSerial
void zEGenerator_Load(class zEGenerator *ent, class xSerial *s) {
    { // 0x001D92D0 - 0x001D92D8
    }
}

// 0x001D92E0 - 0x001D92E8 (0x8) zEGenerator_Save__FP11zEGeneratorP7xSerial
void zEGenerator_Save(class zEGenerator *ent, class xSerial *s) {
    { // 0x001D92E0 - 0x001D92E8
    }
}

// 0x001D92F0 - 0x001D93B4 (0xC4) zEGenerator_Setup__FP11zEGeneratorP7_xScene
void zEGenerator_Setup(class zEGenerator *egen) {
    { // 0x001D92F0 - 0x001D93B4
        class xBase *b; // r2
        class p2LinkAsset *la; // r19
        signed int i; // r16
    }
}

// 0x001D93C0 - 0x001D94E8 (0x128) zEGenerator_Init__FP11zEGeneratorP10p2EntAsset
void zEGenerator_Init(class zEGenerator *egen, class p2EntAsset *asset) {
    { // 0x001D93C0 - 0x001D94E8
        unsigned int size; // r29+0x3c
        void *buf; // r2
        class zEGenAsset *zasset; // r2
    }
}

// 0x001D94F0 - 0x001D94F8 (0x8) zEGenerator_Init__FPvPv
void zEGenerator_Init(void *egen, void *asset) {
    { // 0x001D94F0 - 0x001D94F8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zLightEffect.cpp
// ==========================================================================
// 0x001D9500 - 0x001D97C0 (0x2C0) zLightEffectCauldron__FP7_zLightf
void zLightEffectCauldron(class _zLight *zlight, float seconds) {
    { // 0x001D9500 - 0x001D97C0
        float amount; // r29+0x40
        float *reg; // r17
        class iLight *l; // r16
    }
}

// 0x001D97C0 - 0x001D97D0 (0x10) zLightEffectInitCauldron__FP7_zLight
void zLightEffectInitCauldron(class _zLight *zlight) {
    { // 0x001D97C0 - 0x001D97D0
    }
}

// 0x001D97D0 - 0x001D97D8 (0x8) zLightEffectRandomColFast__FP7_zLightf
void zLightEffectRandomColFast() {
    { // 0x001D97D0 - 0x001D97D8
    }
}

// 0x001D97E0 - 0x001D97E8 (0x8) zLightEffectRandomCol__FP7_zLightf
void zLightEffectRandomCol() {
    { // 0x001D97E0 - 0x001D97E8
    }
}

// 0x001D97F0 - 0x001D97F8 (0x8) zLightEffectRandomColSlow__FP7_zLightf
void zLightEffectRandomColSlow() {
    { // 0x001D97F0 - 0x001D97F8
    }
}

// 0x001D9800 - 0x001D9808 (0x8) zLightEffectHalfDimFast__FP7_zLightf
void zLightEffectHalfDimFast() {
    { // 0x001D9800 - 0x001D9808
    }
}

// 0x001D9810 - 0x001D9818 (0x8) zLightEffectHalfDim__FP7_zLightf
void zLightEffectHalfDim() {
    { // 0x001D9810 - 0x001D9818
    }
}

// 0x001D9820 - 0x001D9828 (0x8) zLightEffectHalfDimSlow__FP7_zLightf
void zLightEffectHalfDimSlow() {
    { // 0x001D9820 - 0x001D9828
    }
}

// 0x001D9830 - 0x001D9838 (0x8) zLightEffectDimFast__FP7_zLightf
void zLightEffectDimFast() {
    { // 0x001D9830 - 0x001D9838
    }
}

// 0x001D9840 - 0x001D9848 (0x8) zLightEffectDim__FP7_zLightf
void zLightEffectDim() {
    { // 0x001D9840 - 0x001D9848
    }
}

// 0x001D9850 - 0x001D9858 (0x8) zLightEffectDimSlow__FP7_zLightf
void zLightEffectDimSlow() {
    { // 0x001D9850 - 0x001D9858
    }
}

// 0x001D9860 - 0x001D9868 (0x8) zLightEffectStrobeFast__FP7_zLightf
void zLightEffectStrobeFast() {
    { // 0x001D9860 - 0x001D9868
    }
}

// 0x001D9870 - 0x001D9878 (0x8) zLightEffectStrobe__FP7_zLightf
void zLightEffectStrobe() {
    { // 0x001D9870 - 0x001D9878
    }
}

// 0x001D9880 - 0x001D9888 (0x8) zLightEffectStrobeSlow__FP7_zLightf
void zLightEffectStrobeSlow() {
    { // 0x001D9880 - 0x001D9888
    }
}

// 0x001D9890 - 0x001D98AC (0x1C) zLightEffectFlickerErratic__FP7_zLightf
void zLightEffectFlickerErratic(class _zLight *zlight, float seconds) {
    { // 0x001D9890 - 0x001D98AC
    }
}

// 0x001D98B0 - 0x001D98D4 (0x24) zLightEffectFlickerSlow__FP7_zLightf
void zLightEffectFlickerSlow(class _zLight *zlight, float seconds) {
    { // 0x001D98B0 - 0x001D98D4
    }
}

// 0x001D98E0 - 0x001D9904 (0x24) zLightEffectFlicker__FP7_zLightf
void zLightEffectFlicker(class _zLight *zlight, float seconds) {
    { // 0x001D98E0 - 0x001D9904
    }
}

// 0x001D9910 - 0x001D9C1C (0x30C) EffectFlicker__FP7_zLightfff
static void EffectFlicker(class _zLight *zlight, float seconds, float min, float rnd) {
    { // 0x001D9910 - 0x001D9C1C
        float *reg; // r18
        class iLight *l; // r16
    }
}

// 0x001D9C20 - 0x001D9C30 (0x10) zLightEffectInitFlicker__FP7_zLight
void zLightEffectInitFlicker(class _zLight *zlight) {
    { // 0x001D9C20 - 0x001D9C30
    }
}

// 0x001D9C30 - 0x001D9C38 (0x8) zLightEffectInitRandomCol__FP7_zLight
void zLightEffectInitRandomCol() {
    { // 0x001D9C30 - 0x001D9C38
    }
}

// 0x001D9C40 - 0x001D9C48 (0x8) zLightEffectInitHalfDim__FP7_zLight
void zLightEffectInitHalfDim() {
    { // 0x001D9C40 - 0x001D9C48
    }
}

// 0x001D9C50 - 0x001D9C58 (0x8) zLightEffectInitDim__FP7_zLight
void zLightEffectInitDim() {
    { // 0x001D9C50 - 0x001D9C58
    }
}

// 0x001D9C60 - 0x001D9C68 (0x8) zLightEffectInitStrobe__FP7_zLight
void zLightEffectInitStrobe() {
    { // 0x001D9C60 - 0x001D9C68
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xFX.cpp
// ==========================================================================
class RpWorldSector * (SectorMatFXEnableCB)(class RpWorldSector *, void *); // 0x001DA4F0
unsigned int ourGlobals[4096]; // 0x002B5AD0
class RpMaterial * (MaterialSetEnvMap2)(class RpMaterial *, void *); // 0x001DA330
class RpMaterial * (MaterialSetShininess)(class RpMaterial *, void *); // 0x001DA4A0
class RpMaterial * (MaterialSetBumpiness)(class RpMaterial *, void *); // 0x00000000
class RpMaterial * (MaterialSetBumpMap)(class RpMaterial *, void *); // 0x00000000
unsigned int gFXSurfaceFlags; // 0x00358644
class zGlobals globals; // 0x00362FD0
class RpMaterial * (MaterialResetEffects)(class RpMaterial *, void *); // 0x00000000
class RpAtomic * (AtomicResetMaterialEffects)(class RpAtomic *, void *); // 0x00000000
class RpMaterial * (iModelSetMaterialTextureCallBack)(class RpMaterial *, void *); // 0x001DA210
class RpMaterial * (iModelSetMaterialTextureRestoreCallBack)(class RpMaterial *, void *); // 0x001DA1C0
class RpMaterial * (iModelMaterialCallBack)(class RpMaterial *, void *); // 0x001DA120
class RpMaterial * (iModelMatMultiplyCallBack)(class RpMaterial *, void *); // 0x001D9E90
class RpMaterial * (iModelMatMultiplyResetCallBack)(class RpMaterial *, void *); // 0x001D9E40
class RwTexture *xFXanimUV2PTexture; // 0x00358434
float xFXanimUV2PScale[2]; // 0x00357698
float xFXanimUV2PTrans[2]; // 0x00357690
float xFXanimUV2PRotMat1[2]; // 0x00357688
float xFXanimUV2PRotMat0[2]; // 0x00357680
float xFXanimUVScale[2]; // 0x00357678
float xFXanimUVTrans[2]; // 0x00357670
float xFXanimUVRotMat1[2]; // 0x00357668
float xFXanimUVRotMat0[2]; // 0x00357660
static class RxPipeline *xFXanimUVPipeline; // 0x00358430
static signed int sMaterialTotal; // 0x0035842C
static class RwRGBA sMaterialColor[16]; // 0x003ACC40
static class RwTexture *sSetMaterialTextureBuffer[32]; // 0x003ACBC0
static signed int sSetMaterialTextureBufferCount; // 0x00358428
float follow; // 0x00000000
unsigned int xfx_initted; // 0x00358424
class RpLight *MainLight; // 0x00358420
unsigned int BumpMapActiveMaterials; // 0x00000000
float BumpMapBumpiness; // 0x00000000
unsigned int EnvMapActiveMaterials; // 0x00000000
float EnvMapShininess; // 0x00357658
// 0x001D9C70 - 0x001D9C94 (0x24) xFXanimUVAtomicSetup__FP8RpAtomic
class RpAtomic *xFXanimUVAtomicSetup(class RpAtomic *atomic) {
    { // 0x001D9C70 - 0x001D9C94
    }
}

// 0x001D9CA0 - 0x001D9CA8 (0x8) xFXanimUV2PSetTexture__FP9RwTexture
void xFXanimUV2PSetTexture(class RwTexture *texture) {
    { // 0x001D9CA0 - 0x001D9CA8
    }
}

// 0x001D9CB0 - 0x001D9CFC (0x4C) xFXanimUV2PSetAngle__Ff
void xFXanimUV2PSetAngle(float angle) {
    { // 0x001D9CB0 - 0x001D9CFC
        float c; // r29+0x20
        float s; // 
    }
}

// 0x001D9D00 - 0x001D9D18 (0x18) xFXanimUV2PSetScale__FP6_xVec3
void xFXanimUV2PSetScale(class _xVec3 *scale) {
    { // 0x001D9D00 - 0x001D9D18
    }
}

// 0x001D9D20 - 0x001D9D38 (0x18) xFXanimUV2PSetTranslation__FP6_xVec3
void xFXanimUV2PSetTranslation(class _xVec3 *trans) {
    { // 0x001D9D20 - 0x001D9D38
    }
}

// 0x001D9D40 - 0x001D9D8C (0x4C) xFXanimUVSetAngle__Ff
void xFXanimUVSetAngle(float angle) {
    { // 0x001D9D40 - 0x001D9D8C
        float c; // r29+0x20
        float s; // 
    }
}

// 0x001D9D90 - 0x001D9DA8 (0x18) xFXanimUVSetScale__FP6_xVec3
void xFXanimUVSetScale(class _xVec3 *scale) {
    { // 0x001D9D90 - 0x001D9DA8
    }
}

// 0x001D9DB0 - 0x001D9DC8 (0x18) xFXanimUVSetTranslation__FP6_xVec3
void xFXanimUVSetTranslation(class _xVec3 *trans) {
    { // 0x001D9DB0 - 0x001D9DC8
    }
}

// 0x001D9DD0 - 0x001D9DE8 (0x18) iModelResetMaterial__FP8RpAtomic
void iModelResetMaterial(class RpAtomic *model) {
    { // 0x001D9DD0 - 0x001D9DE8
    }
}

// 0x001D9DF0 - 0x001D9E34 (0x44) iModelMaterialMul__FP8RpAtomicfff
void iModelMaterialMul(class RpAtomic *model, float rm, float gm, float bm) {
    { // 0x001D9DF0 - 0x001D9E34
        float cols[3]; // r29+0x10
        class RpGeometry *geom; // r2
    }
}

// 0x001D9E40 - 0x001D9E88 (0x48) iModelMatMultiplyResetCallBack__FP10RpMaterialPv
static class RpMaterial *iModelMatMultiplyResetCallBack(class RpMaterial *material) {
    { // 0x001D9E40 - 0x001D9E88
    }
}

// 0x001D9E90 - 0x001DA0D8 (0x248) iModelMatMultiplyCallBack__FP10RpMaterialPv
static class RpMaterial *iModelMatMultiplyCallBack(class RpMaterial *material, void *data) {
    { // 0x001D9E90 - 0x001DA0D8
        float tmp; // r29+0x60
        float *mods; // r16
        class RwRGBA col; // r29+0x5c
        class RwRGBA *rw_col; // r2
    }
}

// 0x001DA0E0 - 0x001DA118 (0x38) iModelSetMaterialAlpha__FP8RpAtomici
void iModelSetMaterialAlpha(class RpAtomic *model, signed int alpha) {
    { // 0x001DA0E0 - 0x001DA118
        class RpGeometry *geom; // r2
    }
}

// 0x001DA120 - 0x001DA180 (0x60) iModelMaterialCallBack__FP10RpMaterialPv
static class RpMaterial *iModelMaterialCallBack(class RpMaterial *material, void *data) {
    { // 0x001DA120 - 0x001DA180
        class RwRGBA new_rw_col; // r29+0xc
    }
}

// 0x001DA180 - 0x001DA194 (0x14) iModelSetMaterialTextureRestore__FP8RpAtomic
void iModelSetMaterialTextureRestore(class RpAtomic *model) {
    { // 0x001DA180 - 0x001DA194
    }
}

// 0x001DA1A0 - 0x001DA1B4 (0x14) iModelSetMaterialTexture__FP8RpAtomicPv
void iModelSetMaterialTexture(class RpAtomic *model, void *texture) {
    { // 0x001DA1A0 - 0x001DA1B4
    }
}

// 0x001DA1C0 - 0x001DA208 (0x48) iModelSetMaterialTextureRestoreCallBack__FP10RpMaterialPv
static class RpMaterial *iModelSetMaterialTextureRestoreCallBack(class RpMaterial *material) {
    { // 0x001DA1C0 - 0x001DA208
    }
}

// 0x001DA210 - 0x001DA264 (0x54) iModelSetMaterialTextureCallBack__FP10RpMaterialPv
static class RpMaterial *iModelSetMaterialTextureCallBack(class RpMaterial *material, void *data) {
    { // 0x001DA210 - 0x001DA264
    }
}

// 0x001DA270 - 0x001DA330 (0xC0) xFXAtomicEnvMapSetup__FP8RpAtomicUif
class RpAtomic *xFXAtomicEnvMapSetup(class RpAtomic *atomic, unsigned int envmapID, float shininess) {
    { // 0x001DA270 - 0x001DA330
        class RpSkin *skin; // r2
        float tmp; // 
        class RwTexture *env; // r16
    }
}

// 0x001DA330 - 0x001DA3D0 (0xA0) MaterialSetEnvMap2__FP10RpMaterialPv
class RpMaterial *MaterialSetEnvMap2(class RpMaterial *material, void *data) {
    { // 0x001DA330 - 0x001DA3D0
        class RwFrame *frame; // r18
        class RwTexture *texture; // r16
        char *textureName; // r2
    }
}

// 0x001DA3D0 - 0x001DA49C (0xCC) MaterialSetEnvMap__FP10RpMaterialPv
static class RpMaterial *MaterialSetEnvMap(class RpMaterial *material, void *data) {
    { // 0x001DA3D0 - 0x001DA49C
        class RwFrame *frame; // r18
        class RwTexture *texture; // r17
    }
}

// 0x001DA4A0 - 0x001DA4E8 (0x48) MaterialSetShininess__FP10RpMaterialPv
static class RpMaterial *MaterialSetShininess(class RpMaterial *material) {
    { // 0x001DA4A0 - 0x001DA4E8
        enum RpMatFXMaterialFlags flags; // r2
    }
}

// 0x001DA4F0 - 0x001DA518 (0x28) SectorMatFXEnableCB__FP13RpWorldSectorPv
static class RpWorldSector *SectorMatFXEnableCB(class RpWorldSector *worldSector) {
    { // 0x001DA4F0 - 0x001DA518
    }
}

// 0x001DA520 - 0x001DA55C (0x3C) xFXExit__Fv
void xFXExit() {
    { // 0x001DA520 - 0x001DA55C
    }
}

// 0x001DA560 - 0x001DA568 (0x8) xFXUpdate__Ff
void xFXUpdate(float dt) {
    { // 0x001DA560 - 0x001DA568
    }
}

// 0x001DA570 - 0x001DA578 (0x8) iModelRenderShit__FPv
void iModelRenderShit() {
    { // 0x001DA570 - 0x001DA578
    }
}

// 0x001DA580 - 0x001DA588 (0x8) FindAtomicCallbackShit__FPv
void FindAtomicCallbackShit() {
    { // 0x001DA580 - 0x001DA588
    }
}

// 0x001DA590 - 0x001DA598 (0x8) xFX_SceneExit__FP7RpWorld
void xFX_SceneExit() {
    { // 0x001DA590 - 0x001DA598
    }
}

// 0x001DA5A0 - 0x001DAD18 (0x778) xFX_SceneEnter__FP7RpWorld
void xFX_SceneEnter(class RpWorld *world) {
    { // 0x001DA5A0 - 0x001DAD18
        class _xEnt *ent; // r20
        class zScene *sc; // r19
        class RwTexture *bump; // r20
        class RwTexture *env; // r19
        class RwTexture *bump; // r2
        class RwTexture *env; // r2
        class RpMaterial *mp; // r18
        class zSurfMatFX *fxp; // r17
        class zSurfaceProps *pp; // r3
        class xSurface *sp; // r2
        signed int num; // r29+0xb0
        signed int i; // r16
    }
}

// 0x001DAD20 - 0x001DAE0C (0xEC) xFXInit__Fv
void xFXInit() {
    { // 0x001DAD20 - 0x001DAE0C
        class RwFrame *frame; // r2
        class RpLight *light; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zFeet.cpp
// ==========================================================================
void (zFeetDebugCB)(); // 0x001DB4E0
unsigned int (zFeetStepCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x001DB760
unsigned int (zFeetStepVillainCB)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *); // 0x001DB4F0
class _xEnv *gCurXEnv; // 0x00357E70
class _tagPlayerFX gPlayerfx; // 0x0036E2A0
unsigned int gActiveHeap; // 0x00357CAC
class zGlobals globals; // 0x00362FD0
static float cPlayerSkidWidth[4]; // 0x00351D70
static class RwRaster *sSkidMarkRaster; // 0x00358440
static class xBase *paremit_vil_footprint; // 0x0035843C
static class xBase *paremit_sd_pawprint; // 0x00358438
static unsigned int sSurfaceSoundIDStep[23]; // 0x003ACC80
// 0x001DAE10 - 0x001DAE98 (0x88) zFeetRenderSkidMark__FP16_tagFeetSkidMark
void zFeetRenderSkidMark(class _tagFeetSkidMark *m) {
    { // 0x001DAE10 - 0x001DAE98
        signed int i; // r17
    }
}

// 0x001DAEA0 - 0x001DB1EC (0x34C) DrawSkidMarkStrip__FP16_tagFeetSkidMarkP12_tagSkidEdgeUcUcUc
static void DrawSkidMarkStrip(class _tagFeetSkidMark *m, class _tagSkidEdge *edge_array, unsigned char red, unsigned char green, unsigned char blue) {
    { // 0x001DAEA0 - 0x001DB1EC
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        signed int count; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        unsigned char alphau8; // r2
        float alphaf; // r29+0x60
        class _tagSkidEdge *e; // r16
        unsigned int vert_idx; // r2
        static class RxObjSpace3DVertex strip_vert[256]; // 0x003ACCE0
    }
}

// 0x001DB1F0 - 0x001DB258 (0x68) zFeetUpdateSkidMark__FP16_tagFeetSkidMarkf
void zFeetUpdateSkidMark(class _tagFeetSkidMark *m, float seconds) {
    { // 0x001DB1F0 - 0x001DB258
    }
}

// 0x001DB260 - 0x001DB454 (0x1F4) zFeetAddSkidMark__FP5_xEntP16_tagFeetSkidMarkf
void zFeetAddSkidMark(class _xEnt *ent, class _tagFeetSkidMark *m, float seconds) {
    { // 0x001DB260 - 0x001DB454
        class _tagSkidEdge *e; // r19
        signed int i; // r18
        class _xVec3 feet_dir; // r29+0xa0
    }
}

// 0x001DB460 - 0x001DB4DC (0x7C) zFeetInitSkidMark__FP16_tagFeetSkidMarki
void zFeetInitSkidMark(class _tagFeetSkidMark *m, signed int total_feet) {
    { // 0x001DB460 - 0x001DB4DC
        signed int i; // r16
    }
}

// 0x001DB4E0 - 0x001DB4E8 (0x8) zFeetDebugCB__Fv
static void zFeetDebugCB() {
    { // 0x001DB4E0 - 0x001DB4E8
    }
}

// 0x001DB4F0 - 0x001DB760 (0x270) zFeetStepVillainCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int zFeetStepVillainCB(class xAnimActiveEffect *acteffect) {
    { // 0x001DB4F0 - 0x001DB760
        class _xVec3 angles; // r29+0xc0
        class xBase *b; // r17
        float pitch; // r29+0xcc
        class zSurfaceProps *surface; // r2
        class xSurface *xsurf; // r2
        signed int surface_type; // r16
        class _zParEmitterCustomSettings info; // r29+0x60
        class _xEnt *ent; // r18
        class tagzFeet *feet; // r17
        class _tagFoot *foot; // r16
    }
}

// 0x001DB760 - 0x001DBAAC (0x34C) zFeetStepCB__FUiP17xAnimActiveEffectP11xAnimSinglePv
static unsigned int zFeetStepCB(class xAnimActiveEffect *acteffect) {
    { // 0x001DB760 - 0x001DBAAC
        class _xVec3 angles; // r29+0x110
        class xBase *b; // r16
        float pitch; // r29+0x11c
        class zSurfaceProps *surface; // r2
        class xSurface *xsurf; // r2
        signed int surface_type; // r18
        signed int collided_with_ground; // r21
        class _xRay3 ray; // r29+0xe0
        class _zParEmitterCustomSettings info; // r29+0x80
        class _xEnt *ent; // r17
        class tagzFeet *feet; // r16
        class _tagFoot *foot; // r18
    }
}

// 0x001DBAB0 - 0x001DCB38 (0x1088) zFeetInit__FiP5_xEntP8tagzFeetiP10xAnimTable
void zFeetInit(signed int ent_type, class _xEnt *owner, class tagzFeet *feet, signed int feet_subtype, class xAnimTable *table) {
    { // 0x001DBAB0 - 0x001DCB38
        signed int tag_idx[6]; // r29+0xc0
        float step_time[6]; // r29+0xa0
    }
}

// 0x001DCB40 - 0x001DCB50 (0x10) zFeetExit__FP5_xEntP10xAnimTable
void zFeetExit(class xAnimTable *table) {
    { // 0x001DCB40 - 0x001DCB50
    }
}

// 0x001DCB50 - 0x001DCC30 (0xE0) zFeetGetIDs__Fv
void zFeetGetIDs() {
    { // 0x001DCB50 - 0x001DCC30
        class RwTexture *tex; // r2
        signed int i; // r16
        char name[128]; // r29+0xb0
        char type_name[128]; // r29+0x30
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zAnimList.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
unsigned int (AlwaysConditional)(class xAnimTransition *, class xAnimSingle *, void *); // 0x001DD070
static char *astnames[20]; // 0x002B5950
static signed int *anused; // 0x00358450
static class xAnimTable **atbls; // 0x0035844C
static unsigned int *aids; // 0x00358448
static signed int nals; // 0x00358444
// 0x001DCC30 - 0x001DCC7C (0x4C) zAnimListGetNumUsed__FUi
signed int zAnimListGetNumUsed(unsigned int id) {
    { // 0x001DCC30 - 0x001DCC7C
        signed int i; // r3
    }
}

// 0x001DCC80 - 0x001DCCCC (0x4C) zAnimListGetTable__FUi
class xAnimTable *zAnimListGetTable(unsigned int id) {
    { // 0x001DCC80 - 0x001DCCCC
        signed int i; // r3
    }
}

// 0x001DCCD0 - 0x001DCCE4 (0x14) zAnimListExit__Fv
void zAnimListExit() {
    { // 0x001DCCD0 - 0x001DCCE4
    }
}

// 0x001DCCF0 - 0x001DD070 (0x380) zAnimListInit__Fv
void zAnimListInit() {
    { // 0x001DCCF0 - 0x001DD070
        class xAnimFile *afile; // r18
        signed int j; // r17
        signed int idle_exists; // r22
        class xAnimFile *afile; // r18
        void *buf; // r2
        class xAnimTable *atbl; // r16
        class st_xST_XASSETINFO ainfo; // r29+0xf0
        class zAnimListAsset *zala; // r17
        unsigned int size; // r29+0x10c
        signed int i; // r30
    }
}

// 0x001DD070 - 0x001DD078 (0x8) AlwaysConditional__FP15xAnimTransitionP11xAnimSinglePv
static unsigned int AlwaysConditional() {
    { // 0x001DD070 - 0x001DD078
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zPitchChange.cpp
// ==========================================================================
// 0x001DD080 - 0x001DD19C (0x11C) zPitchChange__FiPfif
signed int zPitchChange(signed int angle, float *offset, float max) {
    { // 0x001DD080 - 0x001DD19C
        signed int old_angle; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zHologram.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
class _tagxPad *gDebugPad; // 0x00357C30
class zGlobals globals; // 0x00362FD0
static class _HoloBurst cHologramColBurst[23]; // 0x00354850
static class _tagHologram *sHologram[16]; // 0x003B06F0
// 0x0024A250 - 0x0024A2C0 (0x70) zHologramUpdate__Ff
void zHologramUpdate(float seconds) {
    { // 0x0024A250 - 0x0024A2C0
        signed int i; // r16
    }
}

// 0x0024A2C0 - 0x0024A3C0 (0x100) UpdateHologram__FP12_tagHologramf
static void UpdateHologram(class _tagHologram *h, float seconds) {
    { // 0x0024A2C0 - 0x0024A3C0
        float bset; // r29+0x10
        float gset; // r29+0x10
        float rset; // r29+0x10
        class _HoloBurst *burst; // r2
    }
}

// 0x0024A3C0 - 0x0024A49C (0xDC) zHologramAdd__FP5_xEnt
void zHologramAdd(class _xEnt *ent) {
    { // 0x0024A3C0 - 0x0024A49C
    }
}

// 0x0024A4A0 - 0x0024A4E4 (0x44) zHologramInit__Fv
void zHologramInit() {
    { // 0x0024A4A0 - 0x0024A4E4
        signed int i; // r4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xClimate.cpp
// ==========================================================================
class _xMat4x3 g_I3; // 0x00342E70
class zGlobals globals; // 0x00362FD0
static class _tagClimate *sClimate; // 0x00358588
// 0x0024A4F0 - 0x0024A518 (0x28) xClimateGetRealWindDir__FP11_tagClimateP6_xVec3
void xClimateGetRealWindDir(class _tagClimate *climate, class _xVec3 *vec) {
    { // 0x0024A4F0 - 0x0024A518
    }
}

// 0x0024A520 - 0x0024A528 (0x8) xClimateUpdate__FP11_tagClimatef
void xClimateUpdate(class _tagClimate *climate, float seconds) {
    { // 0x0024A520 - 0x0024A528
    }
}

// 0x0024A530 - 0x0024AAB0 (0x580) UpdateRain__FP11_tagClimatef
static void UpdateRain(class _tagClimate *climate, float seconds) {
    { // 0x0024A530 - 0x0024AAB0
        float perc; // 
        signed int i; // r17
        signed int total_snow_flakes; // r16
        signed int i; // r16
        signed int i; // r21
        signed int i; // r21
        signed int i; // r21
        signed int total_rain_drops; // r22
        class _zParEmitterCustomSettings info; // r29+0x90
    }
}

// 0x0024AAB0 - 0x0024AACC (0x1C) xClimateSetRain__Ff
void xClimateSetRain(float stre) {
    { // 0x0024AAB0 - 0x0024AACC
    }
}

// 0x0024AAD0 - 0x0024AAE8 (0x18) xClimateSetSnow__Ff
void xClimateSetSnow(float stre) {
    { // 0x0024AAD0 - 0x0024AAE8
    }
}

// 0x0024AAF0 - 0x0024AC40 (0x150) xClimateInitAsset__FP11_tagClimateP10p2EnvAsset
void xClimateInitAsset(class _tagClimate *climate, class p2EnvAsset *easset) {
    { // 0x0024AAF0 - 0x0024AC40
        class _tagWind *w; // r16
    }
}

// 0x0024AC40 - 0x0024ACB4 (0x74) xClimateInit__FP11_tagClimate
void xClimateInit(class _tagClimate *climate) {
    { // 0x0024AC40 - 0x0024ACB4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zLightning.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
class _tagxPad *gDebugPad; // 0x00357C30
class zGlobals globals; // 0x00362FD0
static unsigned char cSparkColors[24]; // 0x003549E0
static class _tagLightning *sLightning[48]; // 0x003B0730
// 0x0024ACC0 - 0x0024ADD8 (0x118) zLightningModifyEndpoints__FP13_tagLightningP6_xVec3P6_xVec3
void zLightningModifyEndpoints(class _tagLightning *l, class _xVec3 *start, class _xVec3 *end) {
    { // 0x0024ACC0 - 0x0024ADD8
        float scalar; // 
        float pos; // 
        signed int i; // r7
    }
}

// 0x0024ADE0 - 0x0024ADF4 (0x14) zLightningKill__FP13_tagLightning
void zLightningKill(class _tagLightning *l) {
    { // 0x0024ADE0 - 0x0024ADF4
    }
}

// 0x0024AE00 - 0x0024AE94 (0x94) zLightningRender__Fv
void zLightningRender() {
    { // 0x0024AE00 - 0x0024AE94
        signed int i; // r16
        class RwTexture *tex; // r2
    }
}

// 0x0024AEA0 - 0x0024B2E0 (0x440) RenderLightning__FP13_tagLightning
static void RenderLightning(class _tagLightning *l) {
    { // 0x0024AEA0 - 0x0024B2E0
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        signed int i; // r2
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        float oldthickness; // 
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        signed int i; // r4
        class RwRGBA *_col; // r2
        class RwRGBA *_col; // r2
        unsigned int verts; // r2
        unsigned char alphau8; // r3
        float alphaf; // r29+0x50
        static class RxObjSpace3DVertex sStripVert[128]; // 0x003B07F0
    }
}

// 0x0024B2E0 - 0x0024B350 (0x70) zLightningUpdate__Ff
void zLightningUpdate(float seconds) {
    { // 0x0024B2E0 - 0x0024B350
        signed int i; // r16
    }
}

// 0x0024B350 - 0x0024B848 (0x4F8) UpdateLightning__FP13_tagLightningf
static void UpdateLightning(class _tagLightning *l, float seconds) {
    { // 0x0024B350 - 0x0024B848
        signed int col; // r9
        signed int i; // r3
        class _zParEmitterCustomSettings info; // r29+0xd0
        class _zParEmitter *e; // r17
        float sc1; // 
        float sc2; // 
        class _xVec3 vec; // r29+0x150
        class _xMat3x3 mat3; // r29+0xa0
        signed int i; // r17
        float full; // 
        class _xVec3 dir_euler; // r29+0x140
        class _xMat3x3 dirmat; // r29+0x70
        class _xVec3 dir; // r29+0x130
        float sc1; // 
        signed int i; // r17
        float full; // 
    }
}

// 0x0024B850 - 0x0024BAD4 (0x284) zLightningAdd__FP16_tagLightningAdd
class _tagLightning *zLightningAdd(class _tagLightningAdd *add) {
    { // 0x0024B850 - 0x0024BAD4
        float scalar; // 
        float pos; // 
        signed int i; // r6
        float currot; // r29+0x40
    }
}

// 0x0024BAE0 - 0x0024BB24 (0x44) zLightningInit__Fv
void zLightningInit() {
    { // 0x0024BAE0 - 0x0024BB24
        signed int i; // r4
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iCollideFast.cpp
// ==========================================================================
class RpCollisionTriangle * (rayHitsModelFastCB)(class RpIntersection *, class RpCollisionTriangle *, float, void *); // 0x0024C180
class RpCollisionTriangle * (rayHitsEnvFastCB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *); // 0x0024C170
static unsigned int hitzhit; // 0x0035858C
// 0x0024BB30 - 0x0024BD88 (0x258) iRayHitsEnvSpecialFast__FPC6_xRay3PC5_xEnv
unsigned int iRayHitsEnvSpecialFast(class _xRay3 *r, class _xEnv *env) {
    { // 0x0024BB30 - 0x0024BD88
        class RwV3d temp; // r29+0x50
        float len; // 
        class RpIntersection isx; // r29+0x30
    }
}

// 0x0024BD90 - 0x0024BF80 (0x1F0) iRayHitsEnvFast__FPC6_xRay3PC5_xEnv
unsigned int iRayHitsEnvFast(class _xRay3 *r, class _xEnv *env) {
    { // 0x0024BD90 - 0x0024BF80
        class RwV3d temp; // r29+0x50
        float len; // 
        class RpIntersection isx; // r29+0x30
    }
}

// 0x0024BF80 - 0x0024C168 (0x1E8) iRayHitsModelFast__FPC6_xRay3PC14xModelInstance
unsigned int iRayHitsModelFast(class _xRay3 *r, class xModelInstance *m) {
    { // 0x0024BF80 - 0x0024C168
        class RwV3d temp; // r29+0x50
        float len; // 
        class RpIntersection isx; // r29+0x30
    }
}

// 0x0024C170 - 0x0024C180 (0x10) rayHitsEnvFastCB__FP14RpIntersectionP13RpWorldSectorP19RpCollisionTrianglefPv
static class RpCollisionTriangle *rayHitsEnvFastCB() {
    { // 0x0024C170 - 0x0024C180
    }
}

// 0x0024C180 - 0x0024C190 (0x10) rayHitsModelFastCB__FP14RpIntersectionP19RpCollisionTrianglefPv
static class RpCollisionTriangle *rayHitsModelFastCB() {
    { // 0x0024C180 - 0x0024C190
    }
}

// 0x0024C190 - 0x0024C198 (0x8) iCollideFastInit__FP7_xScene
void iCollideFastInit() {
    { // 0x0024C190 - 0x0024C198
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xCollideFast.cpp
// ==========================================================================
// 0x0024C1A0 - 0x0024C394 (0x1F4) xRayHitsSceneFast__FPC6_xRay3PC7_xScene
unsigned int xRayHitsSceneFast(class _xRay3 *r, class _xScene *sc) {
    { // 0x0024C1A0 - 0x0024C394
        class _xEnt *ent; // r20
        class _xEnt *ent; // r18
        class _xEnt *ent; // r19
        class xQCData qcr; // r29+0x60
        unsigned int hit; // r2
        signed int i; // r18
    }
}

// 0x0024C3A0 - 0x0024C3A8 (0x8) xRayHitsEnvFast__FPC6_xRay3PC5_xEnv
unsigned int xRayHitsEnvFast(class _xRay3 *r, class _xEnv *env) {
    { // 0x0024C3A0 - 0x0024C3A8
    }
}

// 0x0024C3B0 - 0x0024C6C0 (0x310) xRayHitsBoundFast__FPC6_xRay3PC7_xBound
unsigned int xRayHitsBoundFast(class _xRay3 *r, class _xBound *b) {
    { // 0x0024C3B0 - 0x0024C6C0
        class _xMat3x3 mn; // r29+0x70
        class _xRay3 lr; // r29+0x40
    }
}

// 0x0024C6C0 - 0x0024C6C8 (0x8) xCollideFastInit__FP7_xScene
void xCollideFastInit(class _xScene *sc) {
    { // 0x0024C6C0 - 0x0024C6C8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iFX.cpp
// ==========================================================================
class RwTexture *xFXanimUV2PTexture; // 0x00358434
u_long128 *sweLocalPacket; // 0x00357754
float xFXanimUV2PRotMat0[2]; // 0x00357680
float xFXanimUV2PRotMat1[2]; // 0x00357688
float xFXanimUV2PTrans[2]; // 0x00357690
float xFXanimUV2PScale[2]; // 0x00357698
float xFXanimUVRotMat0[2]; // 0x00357660
float xFXanimUVRotMat1[2]; // 0x00357668
float xFXanimUVTrans[2]; // 0x00357670
float xFXanimUVScale[2]; // 0x00357678
class RxClusterDefinition RxClPS2xyz; // 0x002FC7B0
class RxClusterDefinition RxClPS2uv; // 0x002FC7C0
class RxClusterDefinition RxClPS2rgba; // 0x002FC7E0
class RxClusterDefinition RxClPS2normal; // 0x002FC7F0
signed int (iFXanimUVInstanceCallBack)(void **, unsigned int); // 0x0024D110
signed long FXanimUVPRS; // 0x002839A0
unsigned int gActiveHeap; // 0x00357CAC
static float warbTime; // 0x003585A0
static unsigned int warbMode; // 0x0035859C
static class _xVec3 *nbuf; // 0x00358598
static class _xVec3 *vbuf; // 0x00358594
static float warbc[4]; // 0x002FC870
class _xEnt *warbEnt; // 0x00358590
float xFXWarbleParam[8]; // 0x003B19F0
static float sinRangePoint5[8]; // 0x002FC850
// 0x0024C6D0 - 0x0024CDBC (0x6EC) iFXwarb__FP6_xVec3P6_xVec3P10RpGeometry
static void iFXwarb(class _xVec3 *orig_verts, class RpGeometry *geom) {
    { // 0x0024C6D0 - 0x0024CDBC
        float b; // r29+0xb0
        float a; // r29+0xb0
        float x; // r29+0xb0
        float percent; // r29+0xb0
        float mul; // r29+0xb0
        float b; // r29+0xb0
        float a; // r29+0xb0
        float x; // r29+0xb0
        float mul; // 
        float ooy; // 
        float b; // r29+0xb0
        float a; // r29+0xb0
        float x; // r29+0xb0
        float b; // r29+0xb0
        float a; // r29+0xb0
        float x; // r29+0xb0
        float b; // r29+0xb0
        float a; // r29+0xb0
        float y; // r29+0xb0
        float x; // 
        float b; // 
        float a; // r29+0xb0
        float y; // r29+0xb0
        float x; // 
        class _xVec3 *nv; // r20
        class _xVec3 *ov; // r2
        float t; // 
        signed int i; // r17
        float maxy; // r29+0xb0
        float miny; // r29+0xb0
    }
}

// 0x0024CDC0 - 0x0024CE24 (0x64) iFXWarbUpdate__Ff
void iFXWarbUpdate(float dt) {
    { // 0x0024CDC0 - 0x0024CE24
    }
}

// 0x0024CE30 - 0x0024CE70 (0x40) iFXWarbSetParams__FUiPf
void iFXWarbSetParams(unsigned int mode, float *val) {
    { // 0x0024CE30 - 0x0024CE70
    }
}

// 0x0024CE70 - 0x0024CF6C (0xFC) iFXWarbInit__FPv12iFXWarbModes
void iFXWarbInit(void *vp_ent, enum iFXWarbModes warb_mode) {
    { // 0x0024CE70 - 0x0024CF6C
        float coeffs[4]; // r29+0x50
        class _xVec3 *verts; // r17
        class RpGeometry *geom; // r3
        class _xEnt *ent; // r16
    }
}

// 0x0024CF70 - 0x0024CFC0 (0x50) iFXanimUVCreatePipe__Fv
class RxPipeline *iFXanimUVCreatePipe() {
    { // 0x0024CF70 - 0x0024CFC0
        class RxPipeline *lockedPipe; // r16
        class RxPipeline *newPipe; // r2
    }
}

// 0x0024CFC0 - 0x0024D110 (0x150) iFXanimUVManagerNodeSetup__FP10RxPipeline
static class RxPipeline *iFXanimUVManagerNodeSetup(class RxPipeline *lockedPipe) {
    { // 0x0024CFC0 - 0x0024D110
        class RxPipelineNode *managerNode; // r18
        class RxNodeDefinition *manager; // r17
        static void *VUCodeArray[32]; // 0x002FC880
    }
}

// 0x0024D110 - 0x0024D530 (0x420) iFXanimUVInstanceCallBack__FPPvUi
static signed int iFXanimUVInstanceCallBack(void **clusterData) {
    { // 0x0024D110 - 0x0024D530
        u_long128 ___ltmp; // r6
        u_long128 ___ltmp; // r5
        u_long128 ___ltmp; // r5
        u_long128 ___ltmp; // r5
        u_long128 ___ltmp; // r7
        u_long128 ___ltmp; // r7
        u_long128 ___ltmp; // r4
        u_long128 ___ltmp; // r4
        u_long128 ___ltmp; // r6
        u_long128 ___ltmp; // r6
        u_long128 ___ltmp; // r6
        u_long128 ___ltmp; // r6
        u_long128 ___ltmp; // r7
        u_long128 ltmp; // r4
        unsigned long long tmp1; // r6
        unsigned long long tmp; // r2
        unsigned int fogSwitch; // r18
        unsigned int primSwitch; // r17
        class RxPS2DMASessionRecord *dmaSesRec; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\xTRC.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
static class xFontBox *sTrcFontBox; // 0x003585A8
class _tagTRCDiskInfo gTrcDisk; // 0x003585A4
class _tagTRCPadInfo gTrcPad[4]; // 0x003B1A10
// 0x0025FE00 - 0x0025FFDC (0x1DC) xTRCPad__Fi12_tagTRCState
void xTRCPad(signed int pad_id, enum _tagTRCState state) {
    { // 0x0025FE00 - 0x0025FFDC
    }
}

// 0x0025FFE0 - 0x00260040 (0x60) xTRCInit__Fv
void xTRCInit() {
    { // 0x0025FFE0 - 0x00260040
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\vobuf.c
// ==========================================================================
// 0x00260040 - 0x00260060 (0x20) voBufDecCount__FP5VoBuf
void voBufDecCount(class VoBuf *f) {
    { // 0x00260040 - 0x00260060
    }
}

// 0x00260060 - 0x002600C4 (0x64) voBufGetTag__FP5VoBuf
class VoTag *voBufGetTag(class VoBuf *f) {
    { // 0x00260060 - 0x002600C4
    }
}

// 0x002600D0 - 0x00260120 (0x50) voBufGetData__FP5VoBuf
class VoData *voBufGetData(class VoBuf *f) {
    { // 0x002600D0 - 0x00260120
    }
}

// 0x00260120 - 0x0026019C (0x7C) voBufIncCount__FP5VoBuf
void voBufIncCount(class VoBuf *f) {
    { // 0x00260120 - 0x0026019C
    }
}

// 0x002601A0 - 0x002601B4 (0x14) voBufIsFull__FP5VoBuf
signed int voBufIsFull(class VoBuf *f) {
    { // 0x002601A0 - 0x002601B4
    }
}

// 0x002601C0 - 0x002601CC (0xC) voBufReset__FP5VoBuf
void voBufReset(class VoBuf *f) {
    { // 0x002601C0 - 0x002601CC
    }
}

// 0x002601D0 - 0x002601D8 (0x8) voBufDelete__FP5VoBuf
void voBufDelete() {
    { // 0x002601D0 - 0x002601D8
    }
}

// 0x002601E0 - 0x002602F8 (0x118) voBufCreate__FP5VoBufP6VoDataP5VoTagi
void voBufCreate(class VoBuf *f, class VoData *data, class VoTag *tag, signed int size) {
    { // 0x002601E0 - 0x002602F8
        signed int i; // r3
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\videodec.c
// ==========================================================================
signed int (mpegError)(class sceMpeg *, class sceMpegCbDataError *, void *); // 0x002603E0
signed int (mpegNodata)(class sceMpeg *, class sceMpegCbData *, void *); // 0x002603B0
signed int (mpegStopDMA)(class sceMpeg *, class sceMpegCbData *, void *); // 0x00260380
signed int (mpegRestartDMA)(class sceMpeg *, class sceMpegCbData *, void *); // 0x00260350
signed int (mpegTS)(class sceMpeg *, class sceMpegCbDataTimeStamp *, void *); // 0x00260300
class VoBuf voBuf; // 0x003B1BA0
class VideoDec videoDec; // 0x003B1A80
// 0x00260300 - 0x00260344 (0x44) mpegTS__FP7sceMpegP22sceMpegCbDataTimeStampPv
signed int mpegTS(class sceMpegCbDataTimeStamp *cbts) {
    { // 0x00260300 - 0x00260344
        class TimeStamp ts; // r29+0x20
    }
}

// 0x00260350 - 0x00260374 (0x24) mpegRestartDMA__FP7sceMpegP13sceMpegCbDataPv
signed int mpegRestartDMA() {
    { // 0x00260350 - 0x00260374
    }
}

// 0x00260380 - 0x002603A4 (0x24) mpegStopDMA__FP7sceMpegP13sceMpegCbDataPv
signed int mpegStopDMA() {
    { // 0x00260380 - 0x002603A4
    }
}

// 0x002603B0 - 0x002603DC (0x2C) mpegNodata__FP7sceMpegP13sceMpegCbDataPv
signed int mpegNodata() {
    { // 0x002603B0 - 0x002603DC
    }
}

// 0x002603E0 - 0x00260408 (0x28) mpegError__FP7sceMpegP18sceMpegCbDataErrorPv
signed int mpegError(class sceMpegCbDataError *cberror) {
    { // 0x002603E0 - 0x00260408
    }
}

// 0x00260410 - 0x002605C4 (0x1B4) decBs0__FP8VideoDec
signed int decBs0(class VideoDec *vd) {
    { // 0x00260410 - 0x002605C4
        signed int image_h; // r18
        signed int image_w; // r17
        signed int i; // r16
        signed int status; // r23
        signed int ret; // r2
        class VoData *voData; // r2
    }
}

// 0x002605D0 - 0x0026063C (0x6C) videoDecMain__FP8VideoDec
void videoDecMain(class VideoDec *vd) {
    { // 0x002605D0 - 0x0026063C
    }
}

// 0x00260640 - 0x00260694 (0x54) videoDecIsFlushed__FP8VideoDec
signed int videoDecIsFlushed(class VideoDec *vd) {
    { // 0x00260640 - 0x00260694
    }
}

// 0x002606A0 - 0x00260830 (0x190) videoDecFlush__FP8VideoDec
signed int videoDecFlush(class VideoDec *vd) {
    { // 0x002606A0 - 0x00260830
        signed int d1; // r29+0x6c
        signed int d0; // r29+0x68
        unsigned char seq_end_code[4]; // r29+0x64
        unsigned char *pd1Unc; // r16
        unsigned char *pd0Unc; // r2
        unsigned char *pd1; // r29+0x60
        unsigned char *pd0; // r29+0x5c
    }
}

// 0x00260830 - 0x0026086C (0x3C) videoDecPutTs__FP8VideoDecllPUci
signed int videoDecPutTs(class VideoDec *vd, signed long pts_val, signed long dts_val, unsigned char *start, signed int len) {
    { // 0x00260830 - 0x0026086C
        class TimeStamp ts; // r29+0x10
    }
}

// 0x00260870 - 0x00260878 (0x8) videoDecGetState__FP8VideoDec
unsigned int videoDecGetState(class VideoDec *vd) {
    { // 0x00260870 - 0x00260878
    }
}

// 0x00260880 - 0x0026088C (0xC) videoDecAbort__FP8VideoDec
void videoDecAbort(class VideoDec *vd) {
    { // 0x00260880 - 0x0026088C
    }
}

// 0x00260890 - 0x002608C4 (0x34) videoDecDelete__FP8VideoDec
signed int videoDecDelete(class VideoDec *vd) {
    { // 0x00260890 - 0x002608C4
    }
}

// 0x002608D0 - 0x002608D8 (0x8) videoDecEndPut__FP8VideoDeci
void videoDecEndPut(class VideoDec *vd, signed int size) {
    { // 0x002608D0 - 0x002608D8
    }
}

// 0x002608E0 - 0x002608E8 (0x8) videoDecBeginPut__FP8VideoDecPPUcPiPPUcPi
void videoDecBeginPut(class VideoDec *vd, unsigned char **ptr0, signed int *len0, unsigned char **ptr1, signed int *len1) {
    { // 0x002608E0 - 0x002608E8
    }
}

// 0x002608F0 - 0x00260910 (0x20) videoDecSetStream__FP8VideoDeciiPFP7sceMpegP13sceMpegCbDataPv_iPv
signed int videoDecSetStream(class VideoDec *vd, signed int strType, signed int ch, signed int (*cb)(class sceMpeg *, class sceMpegCbData *, void *), void *data) {
    { // 0x002608F0 - 0x00260910
    }
}

// 0x00260910 - 0x00260A0C (0xFC) videoDecCreate__FP8VideoDecPUciP1P1iP9TimeStampi
signed int videoDecCreate(class VideoDec *vd, unsigned char *mpegWork, signed int mpegWorkSize, u_long128 *data, u_long128 *tag, signed int tagSize, class TimeStamp *pts, signed int n_pts) {
    { // 0x00260910 - 0x00260A0C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\vibuf.c
// ==========================================================================
// 0x00260A10 - 0x00260BC4 (0x1B4) viBufGetTs__FP5ViBufP9TimeStamp
signed int viBufGetTs(class ViBuf *f, class TimeStamp *ts) {
    { // 0x00260A10 - 0x00260BC4
        signed int rd; // r9
        signed int i; // r3
        signed int tscount; // r2
        signed int isEnd; // r20
        signed int datasize; // r2
        unsigned int stop; // r5
        signed int bp; // r7
        unsigned int ipubp; // r19
        unsigned int d4madr; // r18
    }
}

// 0x00260BD0 - 0x00260CFC (0x12C) viBufPutTs__FP5ViBufP9TimeStamp
signed int viBufPutTs(class ViBuf *f, class TimeStamp *ts) {
    { // 0x00260BD0 - 0x00260CFC
        signed int ret; // r18
    }
}

// 0x00260D00 - 0x00260E4C (0x14C) viBufModifyPts__FP5ViBufP9TimeStamp
signed int viBufModifyPts(class ViBuf *f, class TimeStamp *new_ts) {
    { // 0x00260D00 - 0x00260E4C
        signed int loop; // r3
        signed int datasize; // r2
        signed int rd; // r8
        class TimeStamp *ts; // r7
    }
}

// 0x00260E50 - 0x00260EA0 (0x50) viBufFlush__FP5ViBuf
void viBufFlush(class ViBuf *f) {
    { // 0x00260E50 - 0x00260EA0
    }
}

// 0x00260EA0 - 0x00260EE4 (0x44) viBufCount__FP5ViBuf
signed int viBufCount(class ViBuf *f) {
    { // 0x00260EA0 - 0x00260EE4
        signed int ret; // r16
    }
}

// 0x00260EF0 - 0x00260F80 (0x90) viBufDelete__FP5ViBuf
signed int viBufDelete(class ViBuf *f) {
    { // 0x00260EF0 - 0x00260F80
    }
}

// 0x00260F80 - 0x00261330 (0x3B0) viBufRestartDMA__FP5ViBuf
signed int viBufRestartDMA(class ViBuf *f) {
    { // 0x00260F80 - 0x00261330
        signed int index_next; // r2
        signed int index; // r2
        unsigned int d4chcr_next; // r21
        unsigned int d4tadr_next; // r2
        unsigned int d4qwc_next; // r20
        unsigned int d4madr_next; // r19
        signed int bp; // r18
    }
}

// 0x00261330 - 0x00261490 (0x160) viBufStopDMA__FP5ViBuf
signed int viBufStopDMA(class ViBuf *f) {
    { // 0x00261330 - 0x00261490
    }
}

// 0x00261490 - 0x00261784 (0x2F4) viBufAddDMA__FP5ViBuf
signed int viBufAddDMA(class ViBuf *f) {
    { // 0x00261490 - 0x00261784
        signed int read_n; // r3
        signed int read_start; // r2
        signed int consume; // r3
        signed int isNewData; // r17
        unsigned int d4chcr; // r16
        signed int last; // r8
        signed int index; // r2
        signed int i; // r10
    }
}

// 0x00261790 - 0x002617EC (0x5C) viBufEndPut__FP5ViBufi
void viBufEndPut(class ViBuf *f, signed int size) {
    { // 0x00261790 - 0x002617EC
    }
}

// 0x002617F0 - 0x002618E4 (0xF4) viBufBeginPut__FP5ViBufPPUcPiPPUcPi
void viBufBeginPut(class ViBuf *f, unsigned char **ptr0, signed int *len0, unsigned char **ptr1, signed int *len1) {
    { // 0x002617F0 - 0x002618E4
        signed int en; // r4
        signed int es; // r3
    }
}

// 0x002618F0 - 0x00261A84 (0x194) viBufReset__FP5ViBuf
signed int viBufReset(class ViBuf *f) {
    { // 0x002618F0 - 0x00261A84
        signed int i; // r5
    }
}

// 0x00261A90 - 0x00261B04 (0x74) viBufCreate__FP5ViBufP1P1iP9TimeStampi
signed int viBufCreate(class ViBuf *f, u_long128 *data, u_long128 *tag, signed int size, class TimeStamp *ts, signed int n_ts) {
    { // 0x00261A90 - 0x00261B04
        class SemaParam param; // r29+0x20
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\strfile.c
// ==========================================================================
// 0x00261B10 - 0x00261BA4 (0x94) strFileRead__FP7StrFilePvi
signed int strFileRead(class StrFile *file, void *buff, signed int size) {
    { // 0x00261B10 - 0x00261BA4
        unsigned int err; // r29+0x4c
        signed int count; // r16
    }
}

// 0x00261BB0 - 0x00261BFC (0x4C) strFileClose__FP7StrFile
signed int strFileClose(class StrFile *file) {
    { // 0x00261BB0 - 0x00261BFC
    }
}

// 0x00261C00 - 0x00261EF0 (0x2F0) strFileOpen__FP7StrFilePc
signed int strFileOpen(class StrFile *file, char *filename) {
    { // 0x00261C00 - 0x00261EF0
        class sceCdRMode mode; // r29+0x1bc
        signed int len; // r18
        signed int i; // r17
        signed int dlen; // r17
        char devname[64]; // r29+0x170
        char fn[256]; // r29+0x70
        char *body; // r16
        signed int ret; // r2
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\readbuf.c
// ==========================================================================
// 0x00261EF0 - 0x00261F2C (0x3C) readBufEndGet__FP7ReadBufi
signed int readBufEndGet(class ReadBuf *b, signed int size) {
    { // 0x00261EF0 - 0x00261F2C
    }
}

// 0x00261F30 - 0x00261F90 (0x60) readBufBeginGet__FP7ReadBufPPUc
signed int readBufBeginGet(class ReadBuf *b, unsigned char **ptr) {
    { // 0x00261F30 - 0x00261F90
    }
}

// 0x00261F90 - 0x00262000 (0x70) readBufEndPut__FP7ReadBufi
signed int readBufEndPut(class ReadBuf *b, signed int size) {
    { // 0x00261F90 - 0x00262000
    }
}

// 0x00262000 - 0x0026203C (0x3C) readBufBeginPut__FP7ReadBufPPUc
signed int readBufBeginPut(class ReadBuf *b, unsigned char **ptr) {
    { // 0x00262000 - 0x0026203C
        signed int size; // r2
    }
}

// 0x00262040 - 0x00262048 (0x8) readBufDelete__FP7ReadBuf
void readBufDelete() {
    { // 0x00262040 - 0x00262048
    }
}

// 0x00262050 - 0x0026207C (0x2C) readBufCreate__FP7ReadBuf
void readBufCreate(class ReadBuf *b) {
    { // 0x00262050 - 0x0026207C
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\read.c
// ==========================================================================
class VideoDec videoDec; // 0x003B1A80
class AudioDec audioDec; // 0x003B1B40
// 0x00262080 - 0x00262230 (0x1B0) pcmCallback__FP7sceMpegP16sceMpegCbDataStrPv
signed int pcmCallback(class sceMpegCbDataStr *str, void *data) {
    { // 0x00262080 - 0x00262230
        signed int len; // r2
        signed int d1; // r29+0xac
        signed int d0; // r29+0xa8
        unsigned char *pd1; // r29+0xa4
        unsigned char *pd0; // r29+0xa0
        signed int s1; // r18
        unsigned char *ps0; // r17
        class ReadBuf *rb; // r16
    }
}

// 0x00262230 - 0x00262428 (0x1F8) videoCallback__FP7sceMpegP16sceMpegCbDataStrPv
signed int videoCallback(class sceMpegCbDataStr *str, void *data) {
    { // 0x00262230 - 0x00262428
        signed int d1; // r29+0xbc
        signed int d0; // r29+0xb8
        unsigned char *pd1Unc; // r30
        unsigned char *pd0Unc; // r2
        unsigned char *pd1; // r29+0xb4
        unsigned char *pd0; // r29+0xb0
        signed int s1; // r16
        signed int s0; // r2
        unsigned char *ps0; // r2
        class ReadBuf *rb; // r22
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\ps2_mpeg.c
// ==========================================================================
class _tagxPad mPad[4]; // 0x00362E30
signed int (videoCallback)(class sceMpeg *, class sceMpegCbDataStr *, void *); // 0x00262230
signed int (pcmCallback)(class sceMpeg *, class sceMpegCbDataStr *, void *); // 0x00262080
void (defMain)(); // 0x00262440
void *_gp; // 0x0035F370
void (videoDecMain)(class VideoDec *); // 0x002605D0
signed int (vblankHandler)(signed int); // 0x00262EA0
signed int (handler_endimage)(signed int); // 0x00262E60
static unsigned int skip_time; // 0x003585E8
static unsigned int skip_buttons; // 0x003585E4
class sceGsDBuff sony_db; // 0x003B1BC0
signed int isWithAudio; // 0x00357B84
unsigned int controller_val; // 0x00000000
signed int frd; // 0x003585E0
class VoBuf voBuf; // 0x003B1BA0
class AudioDec audioDec; // 0x003B1B40
class VideoDec videoDec; // 0x003B1A80
class StrFile infile; // 0x003B1A40
signed int defaultTh; // 0x003585DC
signed int videoDecTh; // 0x003585D8
static class TimeStamp *timeStamp; // 0x003585D4
static char *videoDecStack; // 0x003585D0
static u_long128 *viBufData; // 0x003585CC
static unsigned char *audioBuff; // 0x003585C8
class ReadBuf *readBufp; // 0x003585C4
static char *defStack; // 0x003585C0
static signed int mpegWorkSz; // 0x003585BC
static unsigned char *mpegWork; // 0x003585B8
static u_long128 *viBufTag; // 0x003585B4
static class VoTag *voBufTag; // 0x003585B0
static class VoData *voBufData; // 0x003585AC
// 0x00262430 - 0x00262440 (0x10) ErrMessage__FPc
void ErrMessage(char *message) {
    { // 0x00262430 - 0x00262440
    }
}

// 0x00262440 - 0x00262458 (0x18) defMain__Fv
static void defMain() {
    { // 0x00262440 - 0x00262458
    }
}

// 0x00262460 - 0x00262558 (0xF8) termAll__Fv
static void termAll() {
    { // 0x00262460 - 0x00262558
    }
}

// 0x00262560 - 0x002627C0 (0x260) initAll__FPCc
static signed int initAll(char *bsfilename) {
    { // 0x00262560 - 0x002627C0
        class ThreadParam th_param; // r29+0xa0
    }
}

// 0x002627C0 - 0x002627C8 (0x8) switchThread__Fv
void switchThread() {
    { // 0x002627C0 - 0x002627C8
    }
}

// 0x002627D0 - 0x00262AC8 (0x2F8) readMpeg__FP8VideoDecP7ReadBufP7StrFile
static signed int readMpeg(class VideoDec *vd, class ReadBuf *rb, class StrFile *file) {
    { // 0x002627D0 - 0x00262AC8
        signed long t1; // r21
        signed long t0; // r23
        signed int isStarted; // r30
        signed int proceed; // r21
        signed int count; // r21
        signed int writerest; // r2
        signed int readrest; // r20
        signed int getsize; // r2
        signed int putsize; // r2
        unsigned char *get_ptr; // r29+0xfc
        unsigned char *put_ptr; // r29+0xf8
    }
}

// 0x00262AD0 - 0x00262E00 (0x330) ps2_mpeg_play__FPCcPciUif
signed int ps2_mpeg_play(char *fname, char *work_area, signed int work_area_size, unsigned int buttons, float time) {
    { // 0x00262AD0 - 0x00262E00
        char *bp; // r3
        char *extra_buf; // r16
        char filename[128]; // r29+0x80
        char *ptr; // r3
        unsigned int i; // r18
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\disp.c
// ==========================================================================
class sceGsDBuff sony_db; // 0x003B1BC0
class VoBuf voBuf; // 0x003B1BA0
signed int frd; // 0x003585E0
signed int handler_error; // 0x00358604
signed int oddeven; // 0x00358600
signed int isFrameEnd; // 0x003585FC
signed int vblankCount; // 0x003585F8
signed int isCountVblank; // 0x003585F4
// 0x00262E00 - 0x00262E0C (0xC) endDisplay__Fv
void endDisplay() {
    { // 0x00262E00 - 0x00262E0C
    }
}

// 0x00262E10 - 0x00262E5C (0x4C) startDisplay__Fi
void startDisplay(signed int waitEven) {
    { // 0x00262E10 - 0x00262E5C
    }
}

// 0x00262E60 - 0x00262E98 (0x38) handler_endimage__Fi
signed int handler_endimage() {
    { // 0x00262E60 - 0x00262E98
    }
}

// 0x00262EA0 - 0x00263058 (0x1B8) vblankHandler__Fi
signed int vblankHandler() {
    { // 0x00262EA0 - 0x00263058
        class VoTag *tag; // r17
        class sceDmaChan *dmaGif_loadimage; // r16
    }
}

// 0x00263060 - 0x00263370 (0x310) setImageTag__FPUiPviii
void setImageTag(unsigned int *tags, void *image, signed int index, signed int image_w, signed int image_h) {
    { // 0x00263060 - 0x00263370
        unsigned long *tag; // r2
        unsigned long giftag_eop[2]; // r29+0xd0
        unsigned long giftag[2]; // r29+0xc0
        class sceGifPacket packet; // r29+0xb0
        signed int j; // r17
        signed int i; // r22
        signed int mby; // r16
        signed int mbx; // r23
    }
}

// 0x00263370 - 0x002635D8 (0x268) clearGsMem__Fiiiii
void clearGsMem(signed int r, signed int g, signed int b, signed int disp_width, signed int disp_height) {
    { // 0x00263370 - 0x002635D8
        class sceDmaChan *dmaGif; // r22
        u_long128 *packetBase; // r19
        class sceGifPacket packet; // r29+0x90
        unsigned long giftag_clear[2]; // r29+0x80
    }
}


// ==========================================================================
// E:\SD2\Game\flat\ps2_mpeg\audiodec.c
// ==========================================================================
class xSndGlobals gSnd; // 0x00373B80
static char _0_buf[2048]; // 0x003B1E00
// 0x002635E0 - 0x00263928 (0x348) sendToIOP2area__FiiiiPUciPUci
static signed int sendToIOP2area(signed int pd0, signed int d0, signed int pd1, signed int d1, unsigned char *ps0, signed int s0, unsigned char *ps1, signed int s1) {
    { // 0x002635E0 - 0x00263928
        signed int diff; // r3
    }
}

// 0x00263930 - 0x00263B0C (0x1DC) audioDecSendToIOP__FP8AudioDec
signed int audioDecSendToIOP(class AudioDec *ad) {
    { // 0x00263930 - 0x00263B0C
        signed int pos; // r2
        signed int countAdj; // r3
        signed int count_sent; // r16
        signed int s1; // r2
        unsigned char *ps0; // r2
        signed int d1; // r2
        signed int d0; // r2
        signed int pd1; // r2
        signed int pd0; // r2
    }
}

// 0x00263B10 - 0x00263B24 (0x14) audioDecIsPreset__FP8AudioDec
signed int audioDecIsPreset(class AudioDec *ad) {
    { // 0x00263B10 - 0x00263B24
    }
}

// 0x00263B30 - 0x00263BB4 (0x84) audioDecEndPut__FP8AudioDeci
void audioDecEndPut(class AudioDec *ad, signed int size) {
    { // 0x00263B30 - 0x00263BB4
        signed int hdr_add; // r2
    }
}

// 0x00263BC0 - 0x00263C7C (0xBC) audioDecBeginPut__FP8AudioDecPPUcPiPPUcPi
void audioDecBeginPut(class AudioDec *ad, unsigned char **ptr0, signed int *len0, unsigned char **ptr1, signed int *len1) {
    { // 0x00263BC0 - 0x00263C7C
        signed int len; // r9
    }
}

// 0x00263C80 - 0x00263D40 (0xC0) audioDecReset__FP8AudioDec
void audioDecReset(class AudioDec *ad) {
    { // 0x00263C80 - 0x00263D40
    }
}

// 0x00263D40 - 0x00263DC8 (0x88) audioDecStart__FP8AudioDec
void audioDecStart(class AudioDec *ad) {
    { // 0x00263D40 - 0x00263DC8
    }
}

// 0x00263DD0 - 0x00263E40 (0x70) audioDecDelete__FP8AudioDec
signed int audioDecDelete(class AudioDec *ad) {
    { // 0x00263DD0 - 0x00263E40
    }
}

// 0x00263E40 - 0x00263FF4 (0x1B4) audioDecCreate__FP8AudioDecPUcii
signed int audioDecCreate(class AudioDec *ad, unsigned char *buff, signed int buffSize, signed int iopBuffSize) {
    { // 0x00263E40 - 0x00263FF4
        unsigned int mvol; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zFMV.cpp
// ==========================================================================
signed int sweLocalPacketState; // 0x00357744
static class zFMVFile zFMVFileTable[41]; // 0x002FD340
// 0x0026C3D0 - 0x0026C3F0 (0x20) zFMVFileGetName__F8eFMVFile
char *zFMVFileGetName(enum eFMVFile fileEnum) {
    { // 0x0026C3D0 - 0x0026C3F0
    }
}

// 0x0026C3F0 - 0x0026C49C (0xAC) zFMVPlay__FPcUif
unsigned int zFMVPlay(char *filename, unsigned int buttons, float time) {
    { // 0x0026C3F0 - 0x0026C49C
        unsigned int ret; // r16
        char c[32]; // r29+0x40
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntProjectile.cpp
// ==========================================================================
signed int (zEntProjectile_gen_render)(class zEntProjectile *); // 0x0026DDE0
unsigned int gActiveHeap; // 0x00357CAC
class _xVec3 g_NY3; // 0x00342E58
void (ZPRJ_wrap_bupdate)(class _xEnt *, class _xVec3 *); // 0x0026E460
void (zGustUpdateEnt)(class _xEnt *, class _xScene *, float, void *); // 0x001A6250
class _xVec3 g_Y3; // 0x00342E38
float (ZPRJ_newt_zfunc_ty)(float, void *); // 0x00000000
float (ZPRJ_newt_dzfunc_tdy)(float, void *); // 0x00000000
class zGlobals globals; // 0x00362FD0
static char *g_sfxnames[1]; // 0x00357B8C
static class zSFX *g_sounds[1]; // 0x00357B88
static char *g_parname[32]; // 0x002FDD10
static class _zParEmitter *g_paremit[32]; // 0x002FDC90
static class st_ZPRJ_CATEGORY_PROPS g_iniProps[13]; // 0x002FDBF0
static class st_ZPRJ_CATEGORY_FUNCS g_catFuncs[13]; // 0x002FD9E0
static class st_PROJECTILE_DEPOT g_prjdepot; // 0x002FD9B0
static signed int g_prj_init; // 0x00358608
// 0x0026C4A0 - 0x0026C5CC (0x12C) PEFX_flight__14zEntProjectileF15en_ZPRJ_FX_TYPEf
void PEFX_flight(class zEntProjectile *this, enum en_ZPRJ_FX_TYPE fxtyp, float dt) {
    { // 0x0026C4A0 - 0x0026C5CC
        class _zParEmitterCustomSettings info; // r29+0x10
        class _zParEmitter *emit; // r6
    }
}

// 0x0026C5D0 - 0x0026C620 (0x50) DoEffects__14zEntProjectileF19en_PROJECTILE_EVENTf
void DoEffects(class zEntProjectile *this, enum en_PROJECTILE_EVENT event, float dt) {
    { // 0x0026C5D0 - 0x0026C620
        enum en_ZPRJ_FX_TYPE fxtyp; // r5
    }
}

// 0x0026C620 - 0x0026C948 (0x328) ZPRJ_solv_velvec__F6_xVec3ffffP6_xVec3
static signed int ZPRJ_solv_velvec(float grav, float arc_factor, float spd_limit, float dst_range, class _xVec3 *vec_vel) {
    { // 0x0026C620 - 0x0026C948
        class _xVec3 vec_tmp; // r29+0xb0
        float tym_limit; // 
        float tym_ariv; // r29+0xc0
        signed int limit_tally; // r16
        unsigned int solcnt; // r2
        float sol[2]; // r29+0xa8
        float c; // r29+0xc0
        float b; // 
        class _xVec3 vec_tgt; // r29+0x98
        float dst_tgt; // r29+0xc0
        class _xVec3 vec_tgtOrig; // r29+0x88
    }
}

// 0x0026C950 - 0x0026CCBC (0x36C) zEntProjectile_gen_aimer__FP14zEntProjectileP14ST_CAT_AIMDATAi
signed int zEntProjectile_gen_aimer(class zEntProjectile *prj, class ST_CAT_AIMDATA *info, signed int apply) {
    { // 0x0026C950 - 0x0026CCBC
        class st_ZPRJ_SOLV_METHOD_A *metha; // r17
        class st_ZPRJ_AUTOCALC_INFO autinf; // r29+0x70
        class st_CAT_AIMGRAV *grav; // r16
        class st_CAT_AIMARROW *arrow; // r16
        signed int didit; // r2
    }
}

// 0x0026CCC0 - 0x0026CEE8 (0x228) zEntProjectile_softReflect__FP6_xVec3P6_xVec3P6_xVec3f
void zEntProjectile_softReflect(class _xVec3 *dir_new, class _xVec3 *dir_cur, class _xVec3 *oppose, float scaleMag) {
    { // 0x0026CCC0 - 0x0026CEE8
    }
}

// 0x0026CEF0 - 0x0026D2E0 (0x3F0) zEntProjectile_colAccum__FP14zEntProjectileP6_xVec3
float zEntProjectile_colAccum(class zEntProjectile *prj, class _xVec3 *dir_away) {
    { // 0x0026CEF0 - 0x0026D2E0
        class _xVec3 vec_accum; // r29+0x50
        signed int hit_something; // r16
        float mag_hit; // r29+0x60
        class _xCollis *colrec; // r3
        signed int i; // r3
    }
}

// 0x0026D2E0 - 0x0026D3E0 (0x100) ZPRJ_bounce_calcmove__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int ZPRJ_bounce_calcmove(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x0026D2E0 - 0x0026D3E0
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r5
    }
}

// 0x0026D3E0 - 0x0026D7C0 (0x3E0) ZPRJ_bounce_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int ZPRJ_bounce_logix(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x0026D3E0 - 0x0026D7C0
        float elastic; // 
        class _xVec3 vec_accum; // r29+0x50
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r16
    }
}

// 0x0026D7C0 - 0x0026D7F8 (0x38) ZPRJ_bounce_attach__FP14zEntProjectile
static signed int ZPRJ_bounce_attach(class zEntProjectile *prj) {
    { // 0x0026D7C0 - 0x0026D7F8
        class st_ZPRJ_CAT_BOUNCEBALL *bounce; // r2
    }
}

// 0x0026D800 - 0x0026D864 (0x64) ZPRJ_grav_calcmove__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int ZPRJ_grav_calcmove(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x0026D800 - 0x0026D864
        class st_ZPRJ_CAT_GRAVBALL *grav; // r3
    }
}

// 0x0026D870 - 0x0026D9C8 (0x158) ZPRJ_grav_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int ZPRJ_grav_logix(class zEntProjectile *prj, float dt) {
    { // 0x0026D870 - 0x0026D9C8
    }
}

// 0x0026D9D0 - 0x0026D9E4 (0x14) ZPRJ_grav_attach__FP14zEntProjectile
static signed int ZPRJ_grav_attach(class zEntProjectile *prj) {
    { // 0x0026D9D0 - 0x0026D9E4
    }
}

// 0x0026D9F0 - 0x0026DDC4 (0x3D4) ZPRJ_arrow_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
static signed int ZPRJ_arrow_logix(class zEntProjectile *prj, float dt) {
    { // 0x0026D9F0 - 0x0026DDC4
        class st_ZPRJ_CAT_ARROW *arrow; // r16
    }
}

// 0x0026DDD0 - 0x0026DDDC (0xC) ZPRJ_arrow_attach__FP14zEntProjectile
static signed int ZPRJ_arrow_attach(class zEntProjectile *prj) {
    { // 0x0026DDD0 - 0x0026DDDC
    }
}

// 0x0026DDE0 - 0x0026DE18 (0x38) zEntProjectile_gen_render__FP14zEntProjectile
signed int zEntProjectile_gen_render(class zEntProjectile *prj) {
    { // 0x0026DDE0 - 0x0026DE18
    }
}

// 0x0026DE20 - 0x0026DF08 (0xE8) zEntProjectile_gen_logix__FP14zEntProjectilefP6zSceneP10_xEntFrame
signed int zEntProjectile_gen_logix(class zEntProjectile *prj, float dt, class _xEntFrame *frame) {
    { // 0x0026DE20 - 0x0026DF08
        class _xVec3 vec_home; // r29+0x20
    }
}

// 0x0026DF10 - 0x0026DF5C (0x4C) zEntProjectile_gen_attach__FP14zEntProjectile
signed int zEntProjectile_gen_attach(class zEntProjectile *prj) {
    { // 0x0026DF10 - 0x0026DF5C
        class st_ZPRJ_CATDATA *catd; // r5
    }
}

// 0x0026DF60 - 0x0026E228 (0x2C8) LookForFloorRoof__14zEntProjectileFP7_xScenef
signed int LookForFloorRoof(class zEntProjectile *this) {
    { // 0x0026DF60 - 0x0026E228
        signed int i; // r4
        class _xCollis *best; // r2
        class _xCollis *cur_rec; // r18
        class _xCollis *ceil; // r17
        class _xCollis *flor; // r16
    }
}

// 0x0026E230 - 0x0026E458 (0x228) ZPRJ_render__14zEntProjectileFv
signed int ZPRJ_render(class zEntProjectile *this) {
    { // 0x0026E230 - 0x0026E458
        class _xVec3 vec_tmp; // r29+0x20
    }
}

// 0x0026E460 - 0x0026E488 (0x28) ZPRJ_wrap_bupdate__FP5_xEntP6_xVec3
void ZPRJ_wrap_bupdate(class _xEnt *prjent, class _xVec3 *pos) {
    { // 0x0026E460 - 0x0026E488
        class zEntProjectile *prj; // r2
    }
}

// 0x0026E490 - 0x0026E678 (0x1E8) DoColCheck__14zEntProjectileFf
signed int DoColCheck(class zEntProjectile *this, float dt) {
    { // 0x0026E490 - 0x0026E678
        class st_ZPRJ_CATEGORY_FUNCS *catf; // r5
        signed int doit; // r2
        static enum en_ZPRJ_DOCOL_HEURISTICS use_heur; // 0x0035861C
        static signed char init; // 0x00358620
    }
}

// 0x0026E680 - 0x0026EAAC (0x42C) ZPRJ_update__14zEntProjectileFP6zScenef
signed int ZPRJ_update(class zEntProjectile *this, class zScene *scn, float dt) {
    { // 0x0026E680 - 0x0026EAAC
        signed int rc; // r2
        class _xCollis lcl_xcollis_plyr; // r29+0x560
        class _xEntCollis lcl_xentcollis; // r29+0x60
        class st_ZPRJ_CATEGORY_FUNCS *catf; // r16
    }
}

// 0x0026EAB0 - 0x0026EC0C (0x15C) OrientToPath__14zEntProjectileFP10_xEntFramei
void OrientToPath(class zEntProjectile *this, class _xEntFrame *frame, signed int flg_orient) {
    { // 0x0026EAB0 - 0x0026EC0C
        class _xVec3 *camera_pos; // r2
        signed int opt_orient; // r2
        class _xVec3 vec_tmp; // r29+0x20
    }
}

// 0x0026EC10 - 0x0026EC34 (0x24) OrientSet__14zEntProjectileFi
void OrientSet(class zEntProjectile *this, signed int orient) {
    { // 0x0026EC10 - 0x0026EC34
    }
}

// 0x0026EC40 - 0x0026EE70 (0x230) SetupFrame__14zEntProjectileFP6_xVec3P6_xVec3
signed int SetupFrame(class zEntProjectile *this, class _xVec3 *pos, class _xVec3 *veloc) {
    { // 0x0026EC40 - 0x0026EE70
        class _xMat4x3 initmat; // r29+0x40
        class _xVec3 vel_dflt; // r29+0x90
        class _xVec3 pos_dflt; // r29+0x80
    }
}

// 0x0026EE70 - 0x0026EFB4 (0x144) Setup_xEnt__14zEntProjectileFv
signed int Setup_xEnt(class zEntProjectile *this) {
    { // 0x0026EE70 - 0x0026EFB4
    }
}

// 0x0026EFC0 - 0x0026F0DC (0x11C) SetupPrelim__14zEntProjectileF16en_ZPRJ_CATEGORYUi
signed int SetupPrelim(class zEntProjectile *this, enum en_ZPRJ_CATEGORY cat, unsigned int ptaid) {
    { // 0x0026EFC0 - 0x0026F0DC
        class st_ZPRJ_CATEGORY_PROPS *prop; // r5
        class st_ZPRJ_TYPERECORD *ptype; // r2
        signed int isok; // r16
    }
}

// 0x0026F0E0 - 0x0026F298 (0x1B8) Init__14zEntProjectileF16en_ZPRJ_CATEGORYUi
signed int Init(class zEntProjectile *this, enum en_ZPRJ_CATEGORY cat, unsigned int ptypeID) {
    { // 0x0026F0E0 - 0x0026F298
        signed int rc; // r2
        signed int status; // r17
        class st_ZPRJ_CATEGORY_FUNCS *catf; // r16
    }
}

// 0x0026F2A0 - 0x0026F364 (0xC4) Recycle__14zEntProjectileFv
void Recycle(class zEntProjectile *this) {
    { // 0x0026F2A0 - 0x0026F364
    }
}

// 0x0026F370 - 0x0026F408 (0x98) ZPRJ_getNextAvail__Fv
class zEntProjectile *ZPRJ_getNextAvail() {
    { // 0x0026F370 - 0x0026F408
        class st_PROJECTILE_DEPOT *depot; // r3
        class st_ZPRJ_LINKNODE *link; // r2
        class zEntProjectile *prj; // r16
        static signed int warned; // 0x0035860C
        static signed char init; // 0x00358610
    }
}

// 0x0026F410 - 0x0026F454 (0x44) AimAtTarget__14zEntProjectileFP14ST_CAT_AIMDATAi
signed int AimAtTarget(class zEntProjectile *this, class ST_CAT_AIMDATA *info, signed int apply) {
    { // 0x0026F410 - 0x0026F454
        signed int result; // r2
    }
}

// 0x0026F460 - 0x0026F540 (0xE0) AssignModel__14zEntProjectileF19en_ZPRJ_MODEL_INDEX
signed int AssignModel(class zEntProjectile *this, enum en_ZPRJ_MODEL_INDEX midx) {
    { // 0x0026F460 - 0x0026F540
        signed int have_copy; // r16
        class _xMat4x3 mat_copy; // r29+0x40
        class st_ZPRJ_TYPERECORD *ptype; // r4
    }
}

// 0x0026F540 - 0x0026F5C8 (0x88) FlexMatSet__14zEntProjectileFP6_xVec3f
void FlexMatSet(class zEntProjectile *this, class _xVec3 *rot_euler, float dt) {
    { // 0x0026F540 - 0x0026F5C8
        class _xVec3 ang; // r29+0x10
    }
}

// 0x0026F5D0 - 0x0026F5EC (0x1C) FlexVecSet__14zEntProjectileFP6_xVec3
void FlexVecSet(class zEntProjectile *this, class _xVec3 *vec_flex) {
    { // 0x0026F5D0 - 0x0026F5EC
    }
}

// 0x0026F5F0 - 0x0026F6C4 (0xD4) SendEvent__14zEntProjectileF19en_PROJECTILE_EVENTfPvPi
signed int SendEvent(class zEntProjectile *this, enum en_PROJECTILE_EVENT event, float dt, void *edata, signed int *res_user) {
    { // 0x0026F5F0 - 0x0026F6C4
        signed int rc; // r2
        signed int result; // r16
    }
}

// 0x0026F6D0 - 0x0026F780 (0xB0) SendEvent__14zEntProjectileF19en_PROJECTILE_EVENTf
signed int SendEvent(class zEntProjectile *this, enum en_PROJECTILE_EVENT event, float dt) {
    { // 0x0026F6D0 - 0x0026F780
    }
}

// 0x0026F780 - 0x0026F834 (0xB4) SendEvent__14zEntProjectileF19en_PROJECTILE_EVENT
signed int SendEvent(class zEntProjectile *this, enum en_PROJECTILE_EVENT event) {
    { // 0x0026F780 - 0x0026F834
    }
}

// 0x0026F840 - 0x0026F918 (0xD8) SyncFrame__14zEntProjectileFi
void SyncFrame(class zEntProjectile *this, signed int applyToModel) {
    { // 0x0026F840 - 0x0026F918
        class _xVec3 *upos; // r2
        class _xVec3 *mpos; // r2
        class _xEnt *ent; // r2
    }
}

// 0x0026F920 - 0x0026F974 (0x54) Abort__14zEntProjectileFv
void Abort(class zEntProjectile *this) {
    { // 0x0026F920 - 0x0026F974
    }
}

// 0x0026F980 - 0x0026F9C0 (0x40) ColDelaySet__14zEntProjectileFfi
void ColDelaySet(class zEntProjectile *this, float tym, signed int addto) {
    { // 0x0026F980 - 0x0026F9C0
    }
}

// 0x0026F9C0 - 0x0026FB20 (0x160) StateSet__14zEntProjectileF14en_ZPRJ_STATES
enum en_ZPRJ_STATES StateSet(class zEntProjectile *this, enum en_ZPRJ_STATES state) {
    { // 0x0026F9C0 - 0x0026FB20
        enum en_ZPRJ_STATES oldstate; // r16
    }
}

// 0x0026FB20 - 0x0026FBD8 (0xB8) zEntProjectile_acquire__F16en_ZPRJ_CATEGORYUi
class zEntProjectile *zEntProjectile_acquire(enum en_ZPRJ_CATEGORY cat, unsigned int ptypeID) {
    { // 0x0026FB20 - 0x0026FBD8
        signed int rc; // r2
        signed int status; // r17
        class zEntProjectile *prj; // r16
    }
}

// 0x0026FBE0 - 0x0026FC20 (0x40) zEntProjectile_catFromPrjType__FUi16en_ZPRJ_CATEGORY
enum en_ZPRJ_CATEGORY zEntProjectile_catFromPrjType(unsigned int ptypeID, enum en_ZPRJ_CATEGORY prefer) {
    { // 0x0026FBE0 - 0x0026FC20
        class st_ZPRJ_TYPERECORD *ptype; // r2
    }
}

// 0x0026FC20 - 0x0026FC70 (0x50) ZPRJ_findType__FUi
static class st_ZPRJ_TYPERECORD *ZPRJ_findType(unsigned int aid) {
    { // 0x0026FC20 - 0x0026FC70
        class st_PROJECTILE_DEPOT *depot; // r2
        signed int i; // r5
        class st_ZPRJ_TYPERECORD *da_type; // r2
    }
}

// 0x0026FC70 - 0x0026FD0C (0x9C) ZPRJ_insertType__FP20zProjectileTypeAsset
static void ZPRJ_insertType(class zProjectileTypeAsset *ptype) {
    { // 0x0026FC70 - 0x0026FD0C
        signed int idx; // r2
        class st_PROJECTILE_DEPOT *depot; // r2
    }
}

// 0x0026FD10 - 0x0026FDA4 (0x94) zEntProjectile_poolRender__F16en_ZPRJ_CATEGORY
void zEntProjectile_poolRender(enum en_ZPRJ_CATEGORY cat) {
    { // 0x0026FD10 - 0x0026FDA4
        class zEntProjectile *tmpprj; // r17
        class st_ZPRJ_LINKNODE *tmplink; // r16
    }
}

// 0x0026FDB0 - 0x0026FE98 (0xE8) zEntProjectile_poolUpdate__F16en_ZPRJ_CATEGORYP6zScenef
signed int zEntProjectile_poolUpdate(class zScene *scn, float dt) {
    { // 0x0026FDB0 - 0x0026FE98
        class st_ELIST holdlist; // r29+0x60
        signed int cnt; // r16
        class zEntProjectile *tmpprj; // r2
        class st_ZPRJ_LINKNODE *tmplink; // r18
        class st_PROJECTILE_DEPOT *depot; // r2
    }
}

// 0x0026FEA0 - 0x00270090 (0x1F0) zEntProjectile_prepSystem__FiP6zScene
void zEntProjectile_prepSystem(signed int num_ptype) {
    { // 0x0026FEA0 - 0x00270090
        signed int i; // r18
        class st_PROJECTILE_DEPOT *depot; // r2
    }
}

// 0x00270090 - 0x002700DC (0x4C) zEntProjectile_sceneFinish__Fv
void zEntProjectile_sceneFinish() {
    { // 0x00270090 - 0x002700DC
        class st_PROJECTILE_DEPOT *depot; // r16
    }
}

// 0x002700E0 - 0x00270128 (0x48) zEntProjectile_sceneReset__Fv
void zEntProjectile_sceneReset() {
    { // 0x002700E0 - 0x00270128
        class st_ZPRJ_LINKNODE *link; // r2
        class st_PROJECTILE_DEPOT *depot; // r16
    }
}

// 0x00270130 - 0x00270190 (0x60) zEntProjectile_scenePrepare__Fv
void zEntProjectile_scenePrepare() {
    { // 0x00270130 - 0x00270190
        class st_PROJECTILE_DEPOT *depot; // r16
    }
}

// 0x00270190 - 0x00270198 (0x8) zEntProjectile_registerType__FP20zProjectileTypeAsset
void zEntProjectile_registerType(class zProjectileTypeAsset *ptype) {
    { // 0x00270190 - 0x00270198
    }
}

// 0x002701A0 - 0x002701F4 (0x54) zEntProjectile_addProps__F16en_ZPRJ_CATEGORYP22st_ZPRJ_CATEGORY_PROPS
signed int zEntProjectile_addProps(enum en_ZPRJ_CATEGORY cat, class st_ZPRJ_CATEGORY_PROPS *props) {
    { // 0x002701A0 - 0x002701F4
        signed int isok; // r16
    }
}

// 0x00270200 - 0x00270288 (0x88) zEntProjectile_addCat__F16en_ZPRJ_CATEGORYP22st_ZPRJ_CATEGORY_FUNCS
signed int zEntProjectile_addCat(enum en_ZPRJ_CATEGORY cat, class st_ZPRJ_CATEGORY_FUNCS *funcs) {
    { // 0x00270200 - 0x00270288
        class st_ZPRJ_CATEGORY_FUNCS *catmap; // r17
        signed int isok; // r16
    }
}

// 0x00270290 - 0x00270298 (0x8) zEntProjectile_Shutdown__Fv
void zEntProjectile_Shutdown() {
    { // 0x00270290 - 0x00270298
    }
}

// 0x002702A0 - 0x00270430 (0x190) zEntProjectile_Startup__Fv
void zEntProjectile_Startup() {
    { // 0x002702A0 - 0x00270430
        class st_ZPRJ_CATEGORY_PROPS prop_bounce; // r29+0x110
        class st_ZPRJ_CATEGORY_PROPS prop_grav; // r29+0x100
        class st_ZPRJ_CATEGORY_PROPS prop_arrow; // r29+0xf0
        class st_ZPRJ_CATEGORY_PROPS prop_gen; // r29+0xe0
        class st_ZPRJ_CATEGORY_FUNCS fmap_bounce; // r29+0xb0
        class st_ZPRJ_CATEGORY_FUNCS fmap_grav; // r29+0x80
        class st_ZPRJ_CATEGORY_FUNCS fmap_arrow; // r29+0x50
        class st_ZPRJ_CATEGORY_FUNCS fmap_gen; // r29+0x20
    }
}


// ==========================================================================
// E:\RW PS2 Libs\RW32-171201\Graphics\shared\cwcommon\gcc_wrapper.c
// ==========================================================================
// 0x00270430 - 0x00270454 (0x24)
signed int _dpfle(double left, double right) {
    { // 0x00270430 - 0x00270454
    }
}


// ==========================================================================
// E:\SD2\Game\flat\iFMV.cpp
// ==========================================================================
// 0x00270460 - 0x00270518 (0xB8) iFMVPlay__FPcUif
unsigned int iFMVPlay(char *filename, unsigned int buttons, float time) {
    { // 0x00270460 - 0x00270518
        class RwResEntry *repEntryOwner; // r29+0x5c
        class RwResEntry *repEntry; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\p2rwcoll\ctbsp_.c
// ==========================================================================
unsigned int ourGlobals[4096]; // 0x002B5AD0
static char rcsid[12]; // 0x00000000
// 0x00270520 - 0x0027065C (0x13C)
class RpCollBSPTree *_rpCollBSPTreeForAllBoxLeafNodeIntersections(class RpCollBSPTree *tree, class RwBBox *box, signed int (*callBack)(signed int, signed int, void *), void *data) {
    { // 0x00270520 - 0x0027065C
        class RpCollBSPBranchNode *branch; // r4
        class RpCollBSPLeafNode *leaf; // r2
        class nodeInfo nodeStack[33]; // r29+0x60
        signed int nStack; // r16
    }
}

// 0x00270660 - 0x00270FCC (0x96C)
class RpCollBSPTree *_rpCollBSPTreeForAllLineLeafNodeIntersections(class RpCollBSPTree *tree, class RwLine *line, class RpV3dGradient *grad, signed int (*callBack)(signed int, signed int, void *), void *data) {
    { // 0x00270660 - 0x00270FCC
        float delta; // 
        float delta; // 
        float delta; // 
        float delta; // 
        class RpCollBSPBranchNode *branch; // r2
        class RwSplitBits rEnd; // r29+0x53c
        class RwSplitBits rStart; // r29+0x538
        class RwSplitBits lEnd; // r29+0x534
        class RwSplitBits lStart; // r29+0x530
        class RpCollBSPLeafNode *leaf; // r2
        class RwLine currLine; // r29+0x510
        class RwLine lineStack[33]; // r29+0x1f0
        class nodeInfo nodeStack[33]; // r29+0xe0
        signed int nStack; // r16
    }
}

// 0x00270FD0 - 0x00270FE8 (0x18)
signed int _rpCollBSPTreeStreamGetSize(class RpCollBSPTree *tree) {
    { // 0x00270FD0 - 0x00270FE8
        signed int size; // r2
    }
}

// 0x00270FF0 - 0x00271178 (0x188)
class RpCollBSPTree *_rpCollBSPTreeStreamRead(class RpCollBSPTree *tree, class RwStream *stream) {
    { // 0x00270FF0 - 0x00271178
        unsigned int data; // r29+0x5c
        unsigned int indices; // r29+0x58
        unsigned int types; // r29+0x54
        class RpCollBSPLeafNode *leaf; // r17
        class RpCollBSPBranchNode *branch; // r17
        unsigned int numBranches; // r16
        unsigned int numLeaves; // r16
    }
}

// 0x00271180 - 0x002712D8 (0x158)
class RpCollBSPTree *_rpCollBSPTreeStreamWrite(class RpCollBSPTree *tree, class RwStream *stream) {
    { // 0x00271180 - 0x002712D8
        unsigned int data; // r29+0x5c
        unsigned int indices; // r29+0x58
        unsigned int types; // r29+0x54
        class RpCollBSPLeafNode *leaf; // r17
        class RpCollBSPBranchNode *branch; // r17
        unsigned int numBranches; // r16
        unsigned int numLeaves; // r16
    }
}

// 0x002712E0 - 0x0027130C (0x2C)
void _rpCollBSPTreeDestroy(class RpCollBSPTree *tree) {
    { // 0x002712E0 - 0x0027130C
    }
}

// 0x00271310 - 0x00271328 (0x18)
signed int _rpCollBSPTreeMemGetSize(signed int numLeafNodes) {
    { // 0x00271310 - 0x00271328
    }
}

// 0x00271330 - 0x0027136C (0x3C)
class RpCollBSPTree *_rpCollBSPTreeInit(class RpCollBSPTree *tree, signed int numLeafNodes) {
    { // 0x00271330 - 0x0027136C
        signed int numBranchNodes; // r2
    }
}


// ==========================================================================
// E:\SD2\Game\flat\p2rwcoll\ctgeom_.c
// ==========================================================================
signed int (GeomLeafNodeForAllLineIntersections)(signed int, signed int, void *); // 0x00272AF0
signed int (GeomLeafNodeForAllSphereIntersections)(signed int, signed int, void *); // 0x00272960
signed int (GeomLeafNodeForAllBBoxIntersections)(signed int, signed int, void *); // 0x00000000
signed int _rpCollisionGeometryDataOffset; // 0x0035863C
static char rcsid[66]; // 0x00000000
// 0x00271370 - 0x00271414 (0xA4)
class RpAtomic *RpAtomicForAllIntersections(class RpAtomic *atomic, class RpIntersection *intersection, class RpCollisionTriangle * (*callBack)(class RpIntersection *, class RpCollisionTriangle *, float, void *), void *data) {
    { // 0x00271370 - 0x00271414
        class RwSphere *boundingSphere; // r2
        class RpAtomic *intersectionAtomic; // r2
        class GeomCallBack cb; // r29+0x20
    }
}

// 0x00271420 - 0x002718F8 (0x4D8) AtomicForAllSphereIntersections__FP8RpAtomicPC8RwSphereP12GeomCallBack
static class RpAtomic *AtomicForAllSphereIntersections(class RpAtomic *atomic, class RwSphere *testSphere, class GeomCallBack *cb) {
    { // 0x00271420 - 0x002718F8
        class RwV3d *endVert; // r7
        class RwV3d *startVert; // r6
        class RpCollisionTriangle collisionTri; // r29+0x110
        float distance; // r29+0x16c
        class RpTriangle *triangle; // r20
        signed int iTri; // r19
        float recipSphereRad; // 
        float blendFactor; // 
        class RwV3d *endFramePoints; // r18
        class RwV3d *startFramePoints; // r17
        class RpInterpolator *interpolator; // r5
        class RwV3d *vp2; // r22
        class RwV3d *vp1; // r23
        class RwV3d *vp0; // r16
        class RwV3d v2; // r29+0x160
        class RwV3d v1; // r29+0x150
        class RwV3d v0; // r29+0x140
        float _result; // 
        float scaleSq; // 
        class RwMatrixTag inverseLTM; // r29+0xd0
        class RwSphere localSphere; // r29+0xc0
        class RpGeometry *geom; // r30
    }
}

// 0x00271900 - 0x002720E8 (0x7E8) AtomicForAllLineIntersections__FP8RpAtomicP6RwLineP12GeomCallBack
static class RpAtomic *AtomicForAllLineIntersections(class RpAtomic *atomic, class RwLine *testLine, class GeomCallBack *cb) {
    { // 0x00271900 - 0x002720E8
        float _result; // r29+0x1b0
        float lengthSq; // 
        class RpCollisionTriangle collisionTri; // r29+0x130
        float v; // 
        float u; // 
        float hi; // 
        float lo; // 
        class RwV3d edgetmp; // r29+0x1a0
        float det; // r29+0x1b0
        class RwV3d edge1; // r29+0x190
        class RwV3d *endVert; // r7
        class RwV3d *startVert; // r6
        signed int result; // r4
        float distance; // 
        class RpTriangle *triangle; // r21
        signed int iTri; // r22
        float blendFactor; // r29+0x1b0
        class RwV3d *endFramePoints; // r20
        class RwV3d *startFramePoints; // r19
        class RpInterpolator *interpolator; // r5
        class RwV3d *vp2; // r18
        class RwV3d *vp1; // r17
        class RwV3d *vp0; // r16
        class RwV3d v2; // r29+0x180
        class RwV3d v1; // r29+0x170
        class RwV3d v0; // r29+0x160
        class RwMatrixTag inverseLTM; // r29+0xf0
        class RwLine localLine; // r29+0xd0
        class RpGeometry *geom; // r30
    }
}

// 0x002720F0 - 0x00272360 (0x270) GeometryForAllSphereIntersections__FP10RpGeometryP8RwSphereP12GeomCallBack
static class RpGeometry *GeometryForAllSphereIntersections(class RpGeometry *geometry, class RwSphere *testSphere, class GeomCallBack *cb) {
    { // 0x002720F0 - 0x00272360
        class RpCollisionTriangle collisionTri; // r29+0xe0
        float distance; // r29+0x10c
        class RwV3d *v2; // r21
        class RwV3d *v1; // r22
        class RwV3d *v0; // r20
        signed int iTri; // r19
        float recipRadius; // 
        class RpTriangle *triangle; // r18
        class RwV3d *vertices; // r17
        class SphereCollParam isData; // r29+0xd0
        class RwBBox bbox; // r29+0xb0
        class RpCollisionData *collData; // r3
    }
}

// 0x00272360 - 0x00272960 (0x600) GeometryForAllLineIntersections__FP10RpGeometryP6RwLineP12GeomCallBack
static class RpGeometry *GeometryForAllLineIntersections(class RpGeometry *geometry, class RwLine *testLine, class GeomCallBack *cb) {
    { // 0x00272360 - 0x00272960
        float _result; // r29+0x110
        float lengthSq; // 
        class RpCollisionTriangle collisionTri; // r29+0xc0
        float v; // 
        float u; // 
        float hi; // 
        float lo; // 
        class RwV3d edgetmp; // r29+0x100
        float det; // 
        class RwV3d edge1; // r29+0xf0
        signed int result; // r7
        float distance; // 
        class RwV3d *v2; // r4
        class RwV3d *v1; // r3
        class RwV3d *v0; // r2
        signed int iTri; // r21
        class RpTriangle *triangle; // r20
        class RwV3d *vertices; // r19
        class LineCollParam isData; // r29+0xa0
        class RpV3dGradient grad; // r29+0x80
        class RpCollisionData *collData; // r3
    }
}

// 0x00272960 - 0x00272AE4 (0x184) GeomLeafNodeForAllSphereIntersections__FiiPv
static signed int GeomLeafNodeForAllSphereIntersections(signed int numTriangles, signed int triOffset, void *data) {
    { // 0x00272960 - 0x00272AE4
        class RpCollisionTriangle collisionTri; // r29+0xa0
        float distance; // r29+0xcc
        class RwV3d *v2; // r21
        class RwV3d *v1; // r22
        class RwV3d *v0; // r20
        class RpTriangle *tri; // r2
        unsigned short *triIndex; // r19
        class GeomCallBack *cb; // r18
        class RpTriangle *triangles; // r30
        class RwV3d *vertices; // r17
        class RpGeometry *geometry; // r7
        class SphereCollParam *isData; // r23
    }
}

// 0x00272AF0 - 0x00272FAC (0x4BC) GeomLeafNodeForAllLineIntersections__FiiPv
static signed int GeomLeafNodeForAllLineIntersections(signed int numTriangles, signed int triOffset, void *data) {
    { // 0x00272AF0 - 0x00272FAC
        float _result; // r29+0xd0
        float lengthSq; // 
        class RpCollisionTriangle collisionTri; // r29+0x80
        float v; // 
        float u; // 
        float hi; // 
        float lo; // 
        class RwV3d edgetmp; // r29+0xc0
        float det; // 
        class RwV3d edge1; // r29+0xb0
        signed int result; // r7
        float distance; // 
        class RwV3d *v2; // r4
        class RwV3d *v1; // r3
        class RwV3d *v0; // r2
        class RpTriangle *tri; // r2
        unsigned short *triIndex; // r21
        class GeomCallBack *cb; // r20
        class RpTriangle *triangles; // r19
        class RwV3d *vertices; // r18
        class RpGeometry *geometry; // r7
        class LineCollParam *isData; // r17
    }
}


// ==========================================================================
// E:\SD2\Game\flat\p2rwcoll\ctworld_.c
// ==========================================================================
signed int (LeafNodeForAllLinePolyIntersections)(signed int, signed int, void *); // 0x002738C0
signed int _rpCollisionAtomicDataOffset; // 0x00358638
class RpWorldSector * (CollisionWorldSectorTranslate)(class RpWorldSector *, void *); // 0x00000000
class RpWorldSector * (WorldSectorIntersectionCallBack)(class RpWorldSector *, void *); // 0x00000000
class RpWorldSector * (WorldSectorForAllLinePolyIntersections)(class RpWorldSector *, void *); // 0x00273460
signed int (LeafNodeForAllBoxPolyIntersections)(signed int, signed int, void *); // 0x002734D0
signed int (OldLeafNodeForAllBoxPolyIntersections)(signed int, signed int, void *); // 0x00274070
signed int (LeafNodeForAllSpherePolyIntersections)(signed int, signed int, void *); // 0x00273730
signed int (OldLeafNodeForAllSpherePolyIntersections)(signed int, signed int, void *); // 0x00273EF0
class RpWorldSector * (WorldSectorForAllBoxedPrimitivePolyIntersections)(class RpWorldSector *, void *); // 0x00273400
signed int _rpCollisionGlobalsOffset; // 0x00358634
class RpAtomic * (AtomicLineTest)(class RpAtomic *, void *); // 0x00000000
class RpAtomic * (AtomicBoxTest)(class RpAtomic *, void *); // 0x00000000
class RpAtomic * (AtomicSphereTest)(class RpAtomic *, void *); // 0x00000000
class RpWorldSector * (WorldSectorForAllPrimitiveAtomicIntersections)(class RpWorldSector *, void *); // 0x00000000
unsigned int ourGlobals[4096]; // 0x002B5AD0
signed int _rpCollisionWorldSectorDataOffset; // 0x00358640
class RpWorldSector * (WorldSectorBuildDataCallBack)(class RpWorldSector *, void *); // 0x00000000
class RpWorldSector * (WorldSectorDestroyDataCallBack)(class RpWorldSector *, void *); // 0x00000000
class RpWorldSector * (WorldSectorQueryDataCallBack)(class RpWorldSector *, void *); // 0x00000000
static char rcsid[67]; // 0x00000000
// 0x00272FB0 - 0x002733FC (0x44C)
class RpWorld *RpCollisionWorldForAllIntersections(class RpWorld *world, class RpIntersection *intersection, class RpCollisionTriangle * (*callBack)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *), void *data) {
    { // 0x00272FB0 - 0x002733FC
        class RpAtomic *atomic; // r17
        class RwSphere sphere; // r29+0x110
        class TestSphere testSphere; // r29+0x138
        class PolyTestParam isData; // r29+0xe0
        class PolyTestParam isData; // r29+0xb0
        class TestSphere testSphere; // r29+0x130
        class PolyTestParam isData; // r29+0x80
        class PolyLineTestParam isData; // r29+0x30
        class CallBackParam cbParam; // r29+0x120
    }
}

// 0x00273400 - 0x0027345C (0x5C) WorldSectorForAllBoxedPrimitivePolyIntersections__FP13RpWorldSectorPv
static class RpWorldSector *WorldSectorForAllBoxedPrimitivePolyIntersections(class RpWorldSector *worldSector, void *data) {
    { // 0x00273400 - 0x0027345C
        class RpCollisionData *collData; // r2
        class PolyTestParam *isData; // r2
    }
}

// 0x00273460 - 0x002734C8 (0x68) WorldSectorForAllLinePolyIntersections__FP13RpWorldSectorPv
static class RpWorldSector *WorldSectorForAllLinePolyIntersections(class RpWorldSector *worldSector, void *data) {
    { // 0x00273460 - 0x002734C8
        class RpCollisionData *collData; // r2
        class PolyLineTestParam *isData; // r2
    }
}

// 0x002734D0 - 0x00273724 (0x254) LeafNodeForAllBoxPolyIntersections__FiiPv
static signed int LeafNodeForAllBoxPolyIntersections(signed int numPolygons, signed int polyOffset, void *data) {
    { // 0x002734D0 - 0x00273724
        float _result; // r29+0xe0
        float lengthSq; // r29+0xe0
        class RpCollisionTriangle collisionTri; // r29+0xb0
        class RwV3d *v2; // r21
        class RwV3d *v1; // r20
        class RwV3d *v0; // r19
        class RpPolygon *poly; // r2
        unsigned short *polyIndex; // r18
        class RpPolygon *polygons; // r30
        class RwV3d *vertices; // r17
        class RpWorldSector *worldSector; // r23
        class CallBackParam *cbParam; // r16
        class PolyTestParam *isData; // r29+0xac
    }
}

// 0x00273730 - 0x002738BC (0x18C) LeafNodeForAllSpherePolyIntersections__FiiPv
static signed int LeafNodeForAllSpherePolyIntersections(signed int numPolygons, signed int polyOffset, void *data) {
    { // 0x00273730 - 0x002738BC
        class RpCollisionTriangle collisionTri; // r29+0xb0
        float distance; // r29+0xdc
        class RwV3d *v2; // r21
        class RwV3d *v1; // r22
        class RwV3d *v0; // r20
        class RpPolygon *poly; // r2
        unsigned short *polyIndex; // r19
        class RpPolygon *polygons; // r29+0xa0
        class RwV3d *vertices; // r18
        class RpWorldSector *worldSector; // r30
        class TestSphere *testSphere; // r23
        class CallBackParam *cbParam; // r17
        class PolyTestParam *isData; // r2
    }
}

// 0x002738C0 - 0x00273D88 (0x4C8) LeafNodeForAllLinePolyIntersections__FiiPv
static signed int LeafNodeForAllLinePolyIntersections(signed int numPolygons, signed int polyOffset, void *data) {
    { // 0x002738C0 - 0x00273D88
        float _result; // r29+0xe0
        float lengthSq; // 
        class RpCollisionTriangle collisionTri; // r29+0x90
        float v; // 
        float u; // 
        float hi; // 
        float lo; // 
        class RwV3d edgetmp; // r29+0xd0
        float det; // 
        class RwV3d edge1; // r29+0xc0
        signed int result; // r7
        float distance; // 
        class RwV3d *v2; // r4
        class RwV3d *v1; // r3
        class RwV3d *v0; // r2
        class RpPolygon *poly; // r2
        unsigned short *polyIndex; // r21
        class RpPolygon *polygons; // r22
        class RwV3d *vertices; // r20
        class RpWorldSector *worldSector; // r19
        class CallBackParam *cbParam; // r18
        class PolyLineTestParam *isData; // r17
    }
}

// 0x00273D90 - 0x00273EF0 (0x160) OldWorldSectorForAllBoxedPrimitivePolyIntersections__FP13RpWorldSectorP13PolyTestParam
static class RpWorldSector *OldWorldSectorForAllBoxedPrimitivePolyIntersections(class RpWorldSector *worldSector, class PolyTestParam *isData) {
    { // 0x00273D90 - 0x00273EF0
        class RwSplitBits right; // r29+0xbc
        class RwSplitBits left; // r29+0xb8
        float value; // 
        signed int plane; // r6
        signed int polyOffset; // r2
        signed int numPolygons; // r2
        signed int cutStack[16]; // r29+0x70
        signed int cut; // r18
        signed int nStack; // r17
        class RwV3d *vertices; // r16
        class RpCollSector *collSector; // r2
    }
}

// 0x00273EF0 - 0x00274064 (0x174) OldLeafNodeForAllSpherePolyIntersections__FiiPv
static signed int OldLeafNodeForAllSpherePolyIntersections(signed int numPolygons, signed int polyOffset, void *data) {
    { // 0x00273EF0 - 0x00274064
        class RpCollisionTriangle collTri; // r29+0xb0
        float distance; // r29+0xdc
        class RwV3d *v2; // r22
        class RwV3d *v1; // r23
        class RwV3d *v0; // r19
        class RpPolygon *poly; // r18
        class RwV3d *vertices; // r17
        class RpWorldSector *worldSector; // r29+0xa0
        class CallBackParam *cbParam; // r16
        class TestSphere *testSphere; // r30
        class PolyTestParam *isData; // r2
    }
}

// 0x00274070 - 0x002742A4 (0x234) OldLeafNodeForAllBoxPolyIntersections__FiiPv
static signed int OldLeafNodeForAllBoxPolyIntersections(signed int numPolygons, signed int polyOffset, void *data) {
    { // 0x00274070 - 0x002742A4
        float _result; // r29+0xe0
        float lengthSq; // r29+0xe0
        class RpCollisionTriangle collTri; // r29+0xb0
        class RwV3d *v2; // r21
        class RwV3d *v1; // r20
        class RwV3d *v0; // r19
        class RpPolygon *poly; // r18
        class RwV3d *vertices; // r17
        class RpWorldSector *worldSector; // r30
        class CallBackParam *cbParam; // r16
        class PolyTestParam *isData; // r29+0xac
    }
}

// 0x002742B0 - 0x00274A54 (0x7A4) OldWorldSectorForAllLinePolyIntersections__FP13RpWorldSectorP17PolyLineTestParam
static class RpWorldSector *OldWorldSectorForAllLinePolyIntersections(class RpWorldSector *worldSector, class PolyLineTestParam *isData) {
    { // 0x002742B0 - 0x00274A54
        float _result; // r29+0x2f0
        float lengthSq; // 
        class RpCollisionTriangle collTri; // r29+0x2a0
        float v; // 
        float u; // 
        float hi; // 
        float lo; // 
        class RwV3d edgetmp; // r29+0x2d8
        float det; // 
        class RwV3d edge1; // r29+0x2c8
        signed int result; // r7
        float distance; // 
        class RwV3d *v2; // r4
        class RwV3d *v1; // r3
        class RwV3d *v0; // r2
        float delta; // 
        class RwSplitBits end; // r29+0x2ec
        class RwSplitBits start; // r29+0x2e8
        float value; // 
        signed int plane; // r2
        class RpPolygon *poly; // r19
        signed int polyOffset; // r18
        signed int numPolygons; // r2
        class RwLine lineStack[16]; // r29+0x120
        class RwLine line; // r29+0x100
        signed int cutStack[16]; // r29+0xc0
        signed int cut; // r2
        signed int nStack; // r23
        class RwV3d *vertices; // r17
        class RpCollSector *collSector; // r2
        class CallBackParam *cbParam; // r16
    }
}

// 0x00274A60 - 0x00274B50 (0xF0) WorldForAllBoxWorldSectorIntersections__FP7RpWorldP6RwBBoxPFP13RpWorldSectorPv_P13RpWorldSectorPv
static class RpWorld *WorldForAllBoxWorldSectorIntersections(class RpWorld *world, class RwBBox *box, class RpWorldSector * (*callBack)(class RpWorldSector *, void *), void *data) {
    { // 0x00274A60 - 0x00274B50
        class RpPlaneSector *plane; // r3
        class RpSector *sectorStack[64]; // r29+0x60
        class RpSector *sector; // r2
        signed int nStack; // r16
    }
}

// 0x00274B50 - 0x00275438 (0x8E8) WorldForAllLineWorldSectorIntersections__FP7RpWorldP6RwLineP13RpV3dGradientPFP13RpWorldSectorPv_P13RpWorldSectorPvP6RwLine
static class RpWorld *WorldForAllLineWorldSectorIntersections(class RpWorld *world, class RwLine *testLine, class RpV3dGradient *grad, class RpWorldSector * (*callBack)(class RpWorldSector *, void *), void *data, class RwLine *clippedLineOut) {
    { // 0x00274B50 - 0x00275438
        float delta; // 
        float delta; // 
        float delta; // 
        float delta; // 
        class RwSplitBits rEnd; // r29+0x80c
        class RwSplitBits rStart; // r29+0x808
        class RwSplitBits lEnd; // r29+0x804
        class RwSplitBits lStart; // r29+0x800
        class RpPlaneSector *plane; // r2
        class RwLine lineStack[64]; // r29+0x200
        class RwLine line; // r29+0x1e0
        class RpSector *sectorStack[64]; // r29+0xe0
        class RpSector *sector; // r2
        signed int nStack; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\p2rwcoll\rpcollis_.c
// ==========================================================================
unsigned int ourGlobals[4096]; // 0x002B5AD0
void * (CollisionOpen)(void *, signed int, signed int); // 0x00275950
void * (CollisionClose)(void *, signed int, signed int); // 0x00275930
void * (CollisionAtomicInit)(void *, signed int, signed int); // 0x00275880
void * (CollisionDataCreate)(void *, signed int, signed int); // 0x002758A0
void * (CollisionDataDestroy)(void *, signed int, signed int); // 0x002758B0
class RwStream * (CollisionDataStreamRead)(class RwStream *, signed int, void *, signed int, signed int); // 0x00275600
class RwStream * (CollisionDataStreamWrite)(class RwStream *, signed int, void *, signed int, signed int); // 0x00275790
signed int (CollisionDataStreamGetSize)(void *, signed int, signed int); // 0x002755A0
signed int _rpCollisionWorldSectorDataOffset; // 0x00358640
signed int _rpCollisionGeometryDataOffset; // 0x0035863C
signed int _rpCollisionAtomicDataOffset; // 0x00358638
signed int _rpCollisionGlobalsOffset; // 0x00358634
signed int _rpCollisionNumInstances; // 0x00358630
static char rcsid[68]; // 0x00000000
// 0x00275440 - 0x00275594 (0x154)
signed int RpCollisionPluginAttach() {
    { // 0x00275440 - 0x00275594
        signed int result; // r2
    }
}

// 0x002755A0 - 0x002755F4 (0x54) CollisionDataStreamGetSize__FPCvii
static signed int CollisionDataStreamGetSize(void *object, signed int offsetInObject) {
    { // 0x002755A0 - 0x002755F4
        unsigned int binarySize; // r17
        class RpCollisionData *data; // r16
    }
}

// 0x00275600 - 0x0027578C (0x18C) CollisionDataStreamRead__FP8RwStreamiPvii
static class RwStream *CollisionDataStreamRead(class RwStream *stream, signed int binaryLength, void *object, signed int offset) {
    { // 0x00275600 - 0x0027578C
        signed int i; // r29+0x8c
        unsigned short *triIndex; // r18
        signed int numTri; // r17
        signed int treeSize; // r17
        signed int numTris; // r29+0x88
        signed int numLeaves; // r29+0x84
        class RpCollisionData *data; // r16
    }
}

// 0x00275790 - 0x00275874 (0xE4) CollisionDataStreamWrite__FP8RwStreamiPCvii
static class RwStream *CollisionDataStreamWrite(class RwStream *stream, void *object, signed int offset) {
    { // 0x00275790 - 0x00275874
        signed int i; // r29+0x4c
        unsigned short *triIndex; // r17
        signed int numTri; // r18
        class RpCollisionData *data; // r17
    }
}

// 0x00275880 - 0x00275894 (0x14) CollisionAtomicInit__FPvii
static void *CollisionAtomicInit(void *object) {
    { // 0x00275880 - 0x00275894
    }
}

// 0x002758A0 - 0x002758B0 (0x10) CollisionDataCreate__FPvii
static void *CollisionDataCreate(void *object, signed int offset) {
    { // 0x002758A0 - 0x002758B0
    }
}

// 0x002758B0 - 0x00275928 (0x78) CollisionDataDestroy__FPvii
static void *CollisionDataDestroy(void *object, signed int offset) {
    { // 0x002758B0 - 0x00275928
        class RpCollisionData **extData; // r17
    }
}

// 0x00275930 - 0x00275944 (0x14) CollisionClose__FPvii
static void *CollisionClose(void *instance) {
    { // 0x00275930 - 0x00275944
    }
}

// 0x00275950 - 0x00275978 (0x28) CollisionOpen__FPvii
static void *CollisionOpen(void *instance) {
    { // 0x00275950 - 0x00275978
    }
}


// ==========================================================================
// E:\SD2\Game\flat\p2rwcoll\rtintsec_.c
// ==========================================================================
static char rcsid[62]; // 0x00000000
// 0x00275980 - 0x00276CF8 (0x1378)
signed int RtIntersectionBBoxTriangle(class RwBBox *bbox, class RwV3d *v0, class RwV3d *v1, class RwV3d *v2) {
    { // 0x00275980 - 0x00276CF8
        float v; // r29+0x20
        float u; // 
        float v; // r29+0x20
        float u; // 
        float dist; // 
        float det; // 
        class RwV3d norm; // r29+0x10
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        float tmp; // 
        signed int v2Test; // r7
        signed int v1Test; // r6
        signed int v0Test; // r3
    }
}

// 0x00276D00 - 0x00277304 (0x604)
signed int RtIntersectionSphereTriangle(class RwSphere *sphere, class RwV3d *v0, class RwV3d *v1, class RwV3d *v2, class RwV3d *normal, float *distance) {
    { // 0x00276D00 - 0x00277304
        float _result; // r29+0x30
        float factor; // 
        float length2; // r29+0x30
        float sphereRadiusSquared; // r29+0x30
        float distToPlane; // 
        float nDotN; // 
        class RwV3d vc[3]; // r29+0x0
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zFX.cpp
// ==========================================================================
unsigned int gFXSurfaceFlags; // 0x00358644
// 0x00277310 - 0x00277338 (0x28) zFX_SceneExit__FP7RpWorld
void zFX_SceneExit(class RpWorld *world) {
    { // 0x00277310 - 0x00277338
    }
}

// 0x00277340 - 0x002773CC (0x8C) zFX_SceneEnter__FP7RpWorld
void zFX_SceneEnter(class RpWorld *world) {
    { // 0x00277340 - 0x002773CC
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zLaugh.cpp
// ==========================================================================
static signed int sLastRefLaugh; // 0x00358654
static class zLaughSituation sLaughInfo[15]; // 0x002FDE90
static class zLaughSituation *sLaughQueueData; // 0x00358650
static float sLaughTimer; // 0x0035864C
static unsigned int sLaughSoundID[17]; // 0x003B30E0
static signed int sLastLaughEnum; // 0x00358648
// 0x002773D0 - 0x002775C4 (0x1F4) zLaughUpdate__Ff
void zLaughUpdate(float dt) {
    { // 0x002773D0 - 0x002775C4
        signed int i; // r4
    }
}

// 0x002775D0 - 0x0027764C (0x7C) zLaughNotify__Fi
void zLaughNotify(signed int situation) {
    { // 0x002775D0 - 0x0027764C
        class zLaughSituation *s; // r4
    }
}

// 0x00277650 - 0x002776F8 (0xA8) zLaughNextLaughEnum__Fv
static signed int zLaughNextLaughEnum() {
    { // 0x00277650 - 0x002776F8
        signed int i; // r16
    }
}

// 0x00277700 - 0x00277814 (0x114) zLaughInit__Fv
void zLaughInit() {
    { // 0x00277700 - 0x00277814
        char str[32]; // r29+0x30
        signed int i; // r16
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zMusic.cpp
// ==========================================================================
class zGlobals globals; // 0x00362FD0
enum eGameMode gGameMode; // 0x00357E20
static class zMusicTrackInfo sMusicTrack[2]; // 0x003B32A0
static class zMusicSituation sMusicInfo[7]; // 0x002FDFC0
static class zMusicSituation *sMusicQueueData[2]; // 0x00358660
static float sMusicTimer[2]; // 0x00357B98
static signed int sMusicLastEnum[2]; // 0x00357B90
static unsigned int sMusicSoundID[45][2]; // 0x003B3130
static unsigned int sMusicPaused; // 0x00358658
// 0x00277820 - 0x002778E8 (0xC8) zMusicUnpause__Fi
void zMusicUnpause(signed int kill) {
    { // 0x00277820 - 0x002778E8
        signed int i; // r16
    }
}

// 0x002778F0 - 0x00277970 (0x80) zMusicPause__Fv
void zMusicPause() {
    { // 0x002778F0 - 0x00277970
        signed int i; // r17
    }
}

// 0x00277970 - 0x002779D4 (0x64) zMusicKill__Fv
void zMusicKill() {
    { // 0x00277970 - 0x002779D4
        signed int i; // r16
    }
}

// 0x002779E0 - 0x00277B54 (0x174) zMusicUpdate__Ff
void zMusicUpdate(float dt) {
    { // 0x002779E0 - 0x00277B54
        signed int i; // r16
    }
}

// 0x00277B60 - 0x00277C68 (0x108) zMusicNotifyEvent__FPfP5xBase
void zMusicNotifyEvent(float *toParam) {
    { // 0x00277B60 - 0x00277C68
        class zMusicSituation *s; // r7
    }
}

// 0x00277C70 - 0x00277D20 (0xB0) zMusicNotify__Fi
void zMusicNotify(signed int situation) {
    { // 0x00277C70 - 0x00277D20
        class zMusicSituation *s; // r5
    }
}

// 0x00277D20 - 0x002799EC (0x1CCC) zMusicDo__Fi
static signed int zMusicDo(signed int track) {
    { // 0x00277D20 - 0x002799EC
        float vol; // 
        signed int snd_enum; // r16
    }
}

// 0x002799F0 - 0x00279ED0 (0x4E0) zMusicInit__Fv
void zMusicInit() {
    { // 0x002799F0 - 0x00279ED0
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zActionLine.cpp
// ==========================================================================
unsigned int gActiveHeap; // 0x00357CAC
static class RwRaster *sActionLineRaster; // 0x00358668
static class _tagActionLine *sActionLine[8]; // 0x003B32D0
// 0x00279ED0 - 0x00279F44 (0x74) zActionLineRender__Fv
void zActionLineRender() {
    { // 0x00279ED0 - 0x00279F44
        signed int i; // r16
    }
}

// 0x00279F50 - 0x00279FFC (0xAC) RenderActionLine__FP14_tagActionLine
static void RenderActionLine(class _tagActionLine *l) {
    { // 0x00279F50 - 0x00279FFC
        class RwRGBA *_col; // r2
        signed int i; // r7
        class RxObjSpace3DVertex *vert; // r6
        static class RxObjSpace3DVertex sStripVert[4]; // 0x003B32F0
    }
}

// 0x0027A000 - 0x0027A070 (0x70) zActionLineUpdate__Ff
void zActionLineUpdate(float seconds) {
    { // 0x0027A000 - 0x0027A070
        class _tagActionLine *l; // r6
        signed int i; // r5
    }
}

// 0x0027A070 - 0x0027A0E8 (0x78) zActionLineInit__Fv
void zActionLineInit() {
    { // 0x0027A070 - 0x0027A0E8
        class RwTexture *tex; // r2
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zEntVillainCol.cpp
// ==========================================================================
unsigned char gVillainDotsColorDeath[50][4]; // 0x002FE370
unsigned char gVillainDotsColorBirth[50][4]; // 0x002FE2A0
unsigned char gVillainPoofColorDeath[50][4]; // 0x002FE1D0
unsigned char gVillainPoofColorBirth[50][4]; // 0x002FE100

// ==========================================================================
// E:\SD2\Game\flat\zScript.cpp
// ==========================================================================
signed int (zScriptEventCB)(class xBase *, class xBase *, unsigned int, float *, class xBase *); // 0x0027E160
class zGlobals globals; // 0x00362FD0
// 0x0027DFA0 - 0x0027E04C (0xAC) zScriptUpdate__FP5xBaseP7_xScenef
void zScriptUpdate(class xBase *to, float dt) {
    { // 0x0027DFA0 - 0x0027E04C
        class _zScript *t; // r16
    }
}

// 0x0027E050 - 0x0027E160 (0x110) zScriptExecuteEvents__FP8_zScriptff
static void zScriptExecuteEvents(class _zScript *t, float start, float end) {
    { // 0x0027E050 - 0x0027E160
        class xBase *b; // r19
        signed int i; // r16
        class zScriptEventAsset *a; // r18
    }
}

// 0x0027E160 - 0x0027E23C (0xDC) zScriptEventCB__FP5xBaseP5xBaseUiPfP5xBase
signed int zScriptEventCB(class xBase *to, unsigned int toEvent) {
    { // 0x0027E160 - 0x0027E23C
        class _zScript *t; // r16
    }
}

// 0x0027E240 - 0x0027E248 (0x8) zScriptLoad__FP8_zScriptP7xSerial
void zScriptLoad(class _zScript *ent, class xSerial *s) {
    { // 0x0027E240 - 0x0027E248
    }
}

// 0x0027E250 - 0x0027E258 (0x8) zScriptSave__FP8_zScriptP7xSerial
void zScriptSave(class _zScript *ent, class xSerial *s) {
    { // 0x0027E250 - 0x0027E258
    }
}

// 0x0027E260 - 0x0027E2E8 (0x88) zScriptInit__FPvPv
void zScriptInit(void *b, void *tasset) {
    { // 0x0027E260 - 0x0027E2E8
    }
}


// ==========================================================================
// E:\SD2\Game\flat\p2im3dhack.cpp
// ==========================================================================
signed int (NewIm3DPS2AllInstance)(class RxPS2AllPipeData *, void **, unsigned int); // 0x0027E2F0
signed int (*oldTriInstanceCB)(class RxPS2AllPipeData *, void **, unsigned int); // 0x0035866C
signed int gRWPS2FastPath; // 0x00000000
// 0x0027E2F0 - 0x0027E8A4 (0x5B4) NewIm3DPS2AllInstance__FP16RxPS2AllPipeDataPPvUi
signed int NewIm3DPS2AllInstance(class RxPS2AllPipeData *ps2AllPipeData) {
    { // 0x0027E2F0 - 0x0027E8A4
        class RxObjSpace3DVertex *verts; // r5
        unsigned int *dataTmp; // r2
        u_long128 *data; // r4
        class RxObjSpace3DVertex *vert; // r20
        class RxObjSpace3DVertex *verts; // r9
        float *dataTmp; // r2
        u_long128 *data; // r8
        class RxObjSpace3DVertex *verts; // r24
        float *dataTmp; // r2
        u_long128 *data; // r15
        signed int *dataUV; // r17
        signed int *dataRGBA; // r24
        signed int *dataXYZ; // r9
        class RxObjSpace3DVertex *verts; // r8
        signed int batch; // r7
        signed int uvskip; // r16
        signed int rgbaskip; // r5
        signed int xyzskip; // r4
        u_long128 *data; // r12
        unsigned short *indexArray; // r14
        unsigned char *swap; // r2
        unsigned char *vertInc; // r13
        unsigned char *shiftMask2; // r12
        unsigned char *shiftMask; // r11
        unsigned int stripReverse; // r10
        unsigned int modCnt; // r9
        unsigned int startIndOffset; // r2
        unsigned int indOffset; // r8
        unsigned int cntGen; // r7
        signed int j; // r2
        signed int vertCounter; // r2
        unsigned int numVerts; // r2
        class rwPS2AllResEntryHeader *ps2ResHeader; // r6
        class _rwIm3DPoolStash *stash; // r5
        static class rwPS2AllIndexLUTs IndexLUT; // 0x00356FF0
    }
}

// 0x0027E8B0 - 0x0027E900 (0x50) p2EnableFastIm3D__Fv
void p2EnableFastIm3D() {
    { // 0x0027E8B0 - 0x0027E900
        class rxNodePS2AllMatPvtData *matPvtData; // r5
        class RxPipeline *objPipe; // r2
    }
}


// ==========================================================================
// E:\SD2\Game\flat\zGameExtras.cpp
// ==========================================================================
class _zEnv *gCurEnv; // 0x00357DF8
class _tagClimate gClimate; // 0x0036A4F0
class zGlobals globals; // 0x00362FD0
static class _tagFirework sFirework[10]; // 0x003B3380
static unsigned int sThunderNear4; // 0x003586B0
static unsigned int sThunderNear3; // 0x003586AC
static unsigned int sThunderNear2; // 0x003586A8
static unsigned int sThunderNear1; // 0x003586A4
static unsigned int sThunderFar2; // 0x003586A0
static unsigned int sThunderFar1; // 0x0035869C
static signed int sThunderOn; // 0x00358698
static float sFogPrevIntensity; // 0x00357BA0
static float sFogIntensity; // 0x00358694
static class _zFog *sFog; // 0x00358690
static class _zParEmitter *sFirework2Emit; // 0x0035868C
static class _zParEmitter *sFirework1Emit; // 0x00358688
static class _zParEmitter *sFireworkTrailEmit; // 0x00358684
static signed int sColoredFountain; // 0x00358680
static signed int sFireworksOn; // 0x0035867C
static signed int sMonth; // 0x00358678
static signed int sDay; // 0x00358674
static signed int sGameExtrasOn; // 0x00358670
// 0x0027F020 - 0x0027F054 (0x34) zGameExtraIsAnnaBday__Fv
signed int zGameExtraIsAnnaBday() {
    { // 0x0027F020 - 0x0027F054
    }
}

// 0x0027F060 - 0x0027F090 (0x30) zGameExtraIsShirazBday__Fv
signed int zGameExtraIsShirazBday() {
    { // 0x0027F060 - 0x0027F090
    }
}

// 0x0027F090 - 0x0027F0C4 (0x34) zGameExtraIsValentines__Fv
signed int zGameExtraIsValentines() {
    { // 0x0027F090 - 0x0027F0C4
    }
}

// 0x0027F0D0 - 0x0027FC30 (0xB60) UpdateFireworks__Ff
static void UpdateFireworks(float dt) {
    { // 0x0027F0D0 - 0x0027FC30
        class _zParEmitterCustomSettings xplo_info; // r29+0x170
        class _zParEmitter *femit; // r18
        class _zParEmitterCustomSettings trail_info; // r29+0x110
        signed int i; // r17
        class iFogParams fog; // r29+0xf0
        static signed int july_4_rgb; // 0x003586BC
        static class _xVec3 h002_launch[5]; // 0x00341B50
        static class _xVec3 h001_launch[5]; // 0x00341B10
        static signed char init; // 0x003586C0
    }
}

// 0x0027FC30 - 0x00280250 (0x620) UpdateThunder__Ff
static void UpdateThunder() {
    { // 0x0027FC30 - 0x00280250
        class iFogParams fog; // r29+0x50
    }
}

// 0x00280250 - 0x00280514 (0x2C4) UpdateColoredFountain__Ff
static void UpdateColoredFountain(float dt) {
    { // 0x00280250 - 0x00280514
        class zParEmitterAsset *a; // r16
        class _zParEmitter *fe; // r2
        static float ftimer; // 0x003586B4
        static signed char init; // 0x003586B8
    }
}

// 0x00280520 - 0x002806EC (0x1CC) Init4thJuly__Fv
static void Init4thJuly() {
    { // 0x00280520 - 0x002806EC
        char emitter_txt[64]; // r29+0x30
        signed int i; // r17
        signed int idx; // r16
        class zParEmitterAsset *a; // r7
        class _zParEmitter *fe; // r2
    }
}

// 0x002806F0 - 0x00280848 (0x158) InitStPatricksDay__Fv
static void InitStPatricksDay() {
    { // 0x002806F0 - 0x00280848
        char emitter_txt[64]; // r29+0x20
        signed int i; // r16
        class zParEmitterAsset *a; // r7
        class _zParEmitter *fe; // r2
    }
}

// 0x00280850 - 0x00280868 (0x18) zGameExtrasReset__Fv
void zGameExtrasReset() {
    { // 0x00280850 - 0x00280868
    }
}

// 0x00280870 - 0x002808D0 (0x60) zGameExtras__Ff
void zGameExtras(float dt) {
    { // 0x00280870 - 0x002808D0
    }
}

// 0x002808D0 - 0x002812A4 (0x9D4) zGameExtrasInit__Fv
void zGameExtrasInit() {
    { // 0x002808D0 - 0x002812A4
    }
}

