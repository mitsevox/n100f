// Per-source-file DWARF 1 dump: globals, statics and functions
// with parameters, locals (register or stack slot) and scopes.

// ==========================================================================
// C:\usr\local\SCE\EE\lib\crt0.s
// ==========================================================================

// ==========================================================================
// C:\Projects\RW3SDK-PS2\app\cwcommon\gcc_wrapper.c
// ==========================================================================
// 0x002000C0 - 0x002000E4 (0x24)
signed int _dpfge(double left, double right) {
    { // 0x002000C0 - 0x002000E4
    }
}

// 0x002000F0 - 0x00200110 (0x20)
signed int _dpfgt(double left, double right) {
    { // 0x002000F0 - 0x00200110
    }
}

// 0x00200110 - 0x00200130 (0x20)
signed int _dpflt(double left, double right) {
    { // 0x00200110 - 0x00200130
    }
}

// 0x00200130 - 0x00200154 (0x24)
signed int _dpfeq(double left, double right) {
    { // 0x00200130 - 0x00200154
    }
}

// 0x00200160 - 0x0020019C (0x3C)
float _f_ulltof(unsigned long long ul) {
    { // 0x00200160 - 0x0020019C
        float f; // r29+0x10
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\viewskel.cpp
// ==========================================================================
class _rpAtomic * (WorldPickFunc)(class _RpIntersection *, class _RpWorldSector *, class _rpAtomic *, float, void *); // 0x00201DC0
struct @anon0 __vt__8ViewSkel; // 0x004A2600
// 0x002001A0 - 0x00200210 (0x70) __ct__8ViewSkelFii
void *__ct(class ViewSkel *this, signed int width, signed int height) {
    { // 0x002001A0 - 0x00200210
    }
}

// 0x00200210 - 0x00200294 (0x84) __dt__8ViewSkelFv
void *__dt(class ViewSkel *this) {
    { // 0x00200210 - 0x00200294
    }
}

// 0x002002A0 - 0x002003C8 (0x128) RwInitialize__8ViewSkelFPv
signed int RwInitialize(class ViewSkel *this, void *displayID) {
    { // 0x002002A0 - 0x002003C8
        char windowText[128]; // r29+0x40
    }
}

// 0x002003D0 - 0x00200428 (0x58) RwTerminate__8ViewSkelFv
signed int RwTerminate(class ViewSkel *this) {
    { // 0x002003D0 - 0x00200428
        class ViewApp *ptr; // r2
    }
}

// 0x00200430 - 0x002004D8 (0xA8) KeyDown__8ViewSkelFP12_RsKeyStatus
signed int KeyDown(class ViewSkel *this, class _RsKeyStatus *ks) {
    { // 0x00200430 - 0x002004D8
        signed int result; // r16
    }
}

// 0x002004E0 - 0x00200508 (0x28) KeyUp__8ViewSkelFP12_RsKeyStatus
signed int KeyUp(class _RsKeyStatus *ks) {
    { // 0x002004E0 - 0x00200508
    }
}

// 0x00200510 - 0x0020053C (0x2C) PadButtonRight__8ViewSkelFP6_RwV2d
signed int PadButtonRight(class _RwV2d *delta) {
    { // 0x00200510 - 0x0020053C
    }
}

// 0x00200540 - 0x0020056C (0x2C) PadButtonLeft__8ViewSkelFP6_RwV2d
signed int PadButtonLeft(class _RwV2d *delta) {
    { // 0x00200540 - 0x0020056C
    }
}

// 0x00200570 - 0x0020057C (0xC) PadButtonUp__8ViewSkelFP18_RsPadButtonStatus
signed int PadButtonUp() {
    { // 0x00200570 - 0x0020057C
    }
}

// 0x00200580 - 0x002005D4 (0x54) PadButtonDown__8ViewSkelFP18_RsPadButtonStatus
signed int PadButtonDown(class _RsPadButtonStatus *pbs) {
    { // 0x00200580 - 0x002005D4
    }
}

// 0x002005E0 - 0x00200630 (0x50) CameraSize__8ViewSkelFP7_RwRect
signed int CameraSize(class ViewSkel *this, class _RwRect *rect) {
    { // 0x002005E0 - 0x00200630
    }
}

// 0x00200630 - 0x00200FFC (0x9CC) MouseMove__8ViewSkelFP14_RsMouseStatus
signed int MouseMove(class ViewSkel *this, class _RsMouseStatus *ms) {
    { // 0x00200630 - 0x00200FFC
        class _RwV3d offset; // r29+0xb0
        class _RwV3d zoom; // r29+0xa0
        class _RwV2d *offset; // r16
        class _RwMatrix *worldToLocal; // r19
        class _RwMatrix *tmpMatrix; // r16
        class _RwFrame *parent; // r16
        class _RwFrame *cameraFrame; // r16
        class _RwV2d delta; // r29+0x98
        class _RwV3d right; // r29+0x88
        class _RwV3d at; // r29+0x78
        class _RwV3d up; // r29+0x68
        class _RwV3d v; // r29+0x58
    }
}

// 0x00201000 - 0x00201024 (0x24) RightButtonUp__8ViewSkelFP14_RsMouseStatus
signed int RightButtonUp(class ViewSkel *this) {
    { // 0x00201000 - 0x00201024
    }
}

// 0x00201030 - 0x00201088 (0x58) RightButtonDown__8ViewSkelFP14_RsMouseStatus
signed int RightButtonDown(class ViewSkel *this, class _RsMouseStatus *ms) {
    { // 0x00201030 - 0x00201088
    }
}

// 0x00201090 - 0x002010B4 (0x24) LeftButtonUp__8ViewSkelFP14_RsMouseStatus
signed int LeftButtonUp(class ViewSkel *this) {
    { // 0x00201090 - 0x002010B4
    }
}

// 0x002010C0 - 0x00201300 (0x240) LeftButtonDown__8ViewSkelFP14_RsMouseStatus
signed int LeftButtonDown(class ViewSkel *this, class _RsMouseStatus *ms) {
    { // 0x002010C0 - 0x00201300
        float distance; // r29+0x4c
        class _RpIntersection is; // r29+0x30
    }
}

// 0x00201300 - 0x00201388 (0x88) FileLoad__8ViewSkelFPc
signed int FileLoad(class ViewSkel *this, char *pathname) {
    { // 0x00201300 - 0x00201388
        signed int retVal; // r17
        char *psFilename; // r16
    }
}

// 0x00201390 - 0x00201438 (0xA8) Update__8ViewSkelFf
signed int Update(class ViewSkel *this, float timeDelta) {
    { // 0x00201390 - 0x00201438
    }
}

// 0x00201440 - 0x002014FC (0xBC) PluginAttach__8ViewSkelFv
signed int PluginAttach() {
    { // 0x00201440 - 0x002014FC
        signed int rv; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\view.cpp
// ==========================================================================
class _RpCollisionTriangle * (AtomicPickFunc)(class _RpIntersection *, class _RpCollisionTriangle *, float, void *); // 0x00202070
unsigned int ourGlobals[4096]; // 0x004400D8
class _RwRGBA BGNDColor; // 0x004A2B00
class _RwRGBA FGNDColor; // 0x00000000
static class ViewSkel *App; // 0x004A4B00
// 0x00201500 - 0x002015E0 (0xE0) __ct__7ViewAppFv
void *__ct(class ViewApp *this) {
    { // 0x00201500 - 0x002015E0
    }
}

// 0x002015E0 - 0x002017B8 (0x1D8) __dt__7ViewAppFv
void *__dt(class ViewApp *this) {
    { // 0x002015E0 - 0x002017B8
        class _RwFrame *frame; // r18
    }
}

// 0x002017C0 - 0x00201A7C (0x2BC) fileLoad__7ViewAppFPc
signed int fileLoad(class ViewApp *this, char *filename) {
    { // 0x002017C0 - 0x00201A7C
        signed int dot; // r16
        signed int status; // r2
    }
}

// 0x00201A80 - 0x00201B28 (0xA8) worldDestroy__7ViewAppFv
void worldDestroy(class ViewApp *this) {
    { // 0x00201A80 - 0x00201B28
    }
}

// 0x00201B30 - 0x00201C30 (0x100) worldInit__7ViewAppFv
void worldInit(class ViewApp *this) {
    { // 0x00201B30 - 0x00201C30
        class _RwV3d v; // r29+0x30
    }
}

// 0x00201C30 - 0x00201D08 (0xD8) render__7ViewAppFv
void render(class ViewApp *this) {
    { // 0x00201C30 - 0x00201D08
    }
}

// 0x00201D10 - 0x00201D64 (0x54) RegisterSkelClass__Fv
class RwSkel *RegisterSkelClass() {
    { // 0x00201D10 - 0x00201D64
    }
}

// 0x00201D70 - 0x00201DB8 (0x48) UnRegisterSkelClass__Fv
void UnRegisterSkelClass() {
    { // 0x00201D70 - 0x00201DB8
    }
}

// 0x00201DC0 - 0x00201E3C (0x7C) WorldPickFunc__FP15_RpIntersectionP14_RpWorldSectorP9_rpAtomicfPv
class _rpAtomic *WorldPickFunc(class _RpIntersection *is, class _rpAtomic *a, float distance, void *data) {
    { // 0x00201DC0 - 0x00201E3C
        float oldDistance; // 
    }
}

// 0x00201E40 - 0x00202070 (0x230) RsSetModelTexturePath__FPc
signed int RsSetModelTexturePath(char *pathName) {
    { // 0x00201E40 - 0x00202070
        char separator[2]; // r29+0x44c
        char *extender; // r2
        char *testSeparator; // r2
        char *lastSeparator; // r18
        unsigned int numChars; // r17
        char *temp; // r16
        char pad[1024]; // r29+0x40
    }
}

// 0x00202070 - 0x00202094 (0x24) AtomicPickFunc__FP15_RpIntersectionP20_RpCollisionTrianglefPv
static class _RpCollisionTriangle *AtomicPickFunc(class _RpCollisionTriangle *cp, float distance, void *data) {
    { // 0x00202070 - 0x00202094
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\appcam.cpp
// ==========================================================================
class _RwRGBA BGNDColor; // 0x004A2B00
// 0x002020A0 - 0x002020E4 (0x44) __ct__9AppCameraFiii
void *__ct(class AppCamera *this, signed int width, signed int height, signed int zBuffer) {
    { // 0x002020A0 - 0x002020E4
    }
}

// 0x002020F0 - 0x00202174 (0x84) __dt__9AppCameraFv
void *__dt(class AppCamera *this) {
    { // 0x002020F0 - 0x00202174
    }
}

// 0x00202180 - 0x002021F8 (0x78) CameraReset__9AppCameraFP6_RwV3d
void CameraReset(class AppCamera *this, class _RwV3d *origin) {
    { // 0x00202180 - 0x002021F8
        class _RwV3d v; // r29+0x10
    }
}

// 0x00202200 - 0x002023D8 (0x1D8) CameraClear__9AppCameraFv
void CameraClear(class AppCamera *this) {
    { // 0x00202200 - 0x002023D8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\demoskel\mouse.cpp
// ==========================================================================
unsigned char mousedat[836]; // 0x0041D880
// 0x002023E0 - 0x002024E4 (0x104) __ct__7RsMouseFi
void *__ct(class RsMouse *this, signed int visible) {
    { // 0x002023E0 - 0x002024E4
        class _RwImage *image; // r16
        signed int rasterFlags; // r29+0x4c
        signed int rasterDepth; // r29+0x48
        signed int rasterWidth; // r29+0x44
        signed int rasterHeight; // r29+0x40
    }
}

// 0x002024F0 - 0x00202564 (0x74) __dt__7RsMouseFv
void *__dt(class RsMouse *this) {
    { // 0x002024F0 - 0x00202564
    }
}

// 0x00202570 - 0x00202710 (0x1A0) render__7RsMouseFP9_RwCamera
void render(class RsMouse *this, class _RwCamera *camera) {
    { // 0x00202570 - 0x00202710
    }
}

// 0x00202710 - 0x0020271C (0xC) setVisibility__7RsMouseFi
void setVisibility(class RsMouse *this, signed int visible) {
    { // 0x00202710 - 0x0020271C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\demoskel\skel.cpp
// ==========================================================================
class _RwImage * (RwImageReadBMP)(char *); // 0x002A0F80
class _RwImage * (RwImageReadPNG)(char *); // 0x0027EFB8
struct @anon0 __vt__6RwSkel; // 0x004A2670
// 0x00202720 - 0x00202754 (0x34) __ct__6RwSkelFii
void *__ct(class RwSkel *this, signed int width, signed int height) {
    { // 0x00202720 - 0x00202754
    }
}

// 0x00202760 - 0x002027E0 (0x80) __dt__6RwSkelFv
void *__dt(class RwSkel *this) {
    { // 0x00202760 - 0x002027E0
    }
}

// 0x002027E0 - 0x002027EC (0xC) PadButtonRight__6RwSkelFP6_RwV2d
signed int PadButtonRight() {
    { // 0x002027E0 - 0x002027EC
    }
}

// 0x002027F0 - 0x002027FC (0xC) PadButtonLeft__6RwSkelFP6_RwV2d
signed int PadButtonLeft() {
    { // 0x002027F0 - 0x002027FC
    }
}

// 0x00202800 - 0x0020280C (0xC) PadButtonUp__6RwSkelFP18_RsPadButtonStatus
signed int PadButtonUp() {
    { // 0x00202800 - 0x0020280C
    }
}

// 0x00202810 - 0x0020281C (0xC) PadButtonDown__6RwSkelFP18_RsPadButtonStatus
signed int PadButtonDown() {
    { // 0x00202810 - 0x0020281C
    }
}

// 0x00202820 - 0x0020282C (0xC) KeyUp__6RwSkelFP12_RsKeyStatus
signed int KeyUp() {
    { // 0x00202820 - 0x0020282C
    }
}

// 0x00202830 - 0x0020283C (0xC) KeyDown__6RwSkelFP12_RsKeyStatus
signed int KeyDown() {
    { // 0x00202830 - 0x0020283C
    }
}

// 0x00202840 - 0x0020284C (0xC) Update__6RwSkelFf
signed int Update(float timeDelta) {
    { // 0x00202840 - 0x0020284C
    }
}

// 0x00202850 - 0x00202880 (0x30) SelectDevice__6RwSkelFv
signed int SelectDevice(class RwSkel *this) {
    { // 0x00202850 - 0x00202880
    }
}

// 0x00202880 - 0x002028B4 (0x34) RwTerminate__6RwSkelFv
signed int RwTerminate() {
    { // 0x00202880 - 0x002028B4
    }
}

// 0x002028C0 - 0x00202918 (0x58) CameraShowRaster__6RwSkelFP9_RwCamera
void CameraShowRaster(class RwSkel *this, class _RwCamera *camera) {
    { // 0x002028C0 - 0x00202918
    }
}

// 0x00202920 - 0x00202AB4 (0x194) RwInitialize__6RwSkelFPv
signed int RwInitialize(class RwSkel *this, void *displayID) {
    { // 0x00202920 - 0x00202AB4
        class _RwEngineOpenParams openParams; // r29+0x3c
    }
}

// 0x00202AC0 - 0x00202ACC (0xC) RightButtonUp__6RwSkelFP14_RsMouseStatus
signed int RightButtonUp() {
    { // 0x00202AC0 - 0x00202ACC
    }
}

// 0x00202AD0 - 0x00202ADC (0xC) RightButtonDown__6RwSkelFP14_RsMouseStatus
signed int RightButtonDown() {
    { // 0x00202AD0 - 0x00202ADC
    }
}

// 0x00202AE0 - 0x00202B48 (0x68) RegisterImageLoader__6RwSkelFv
signed int RegisterImageLoader() {
    { // 0x00202AE0 - 0x00202B48
    }
}

// 0x00202B50 - 0x00202BC4 (0x74) PluginAttach__6RwSkelFv
signed int PluginAttach() {
    { // 0x00202B50 - 0x00202BC4
        signed int rv; // r2
    }
}

// 0x00202BD0 - 0x00202BDC (0xC) MouseMove__6RwSkelFP14_RsMouseStatus
signed int MouseMove() {
    { // 0x00202BD0 - 0x00202BDC
    }
}

// 0x00202BE0 - 0x00202BEC (0xC) LeftButtonUp__6RwSkelFP14_RsMouseStatus
signed int LeftButtonUp() {
    { // 0x00202BE0 - 0x00202BEC
    }
}

// 0x00202BF0 - 0x00202BFC (0xC) LeftButtonDown__6RwSkelFP14_RsMouseStatus
signed int LeftButtonDown() {
    { // 0x00202BF0 - 0x00202BFC
    }
}

// 0x00202C00 - 0x00202C6C (0x6C) InitDebug__6RwSkelFv
signed int InitDebug(class RwSkel *this) {
    { // 0x00202C00 - 0x00202C6C
    }
}

// 0x00202C70 - 0x00202C7C (0xC) FileLoad__6RwSkelFPc
signed int FileLoad() {
    { // 0x00202C70 - 0x00202C7C
    }
}

// 0x00202C80 - 0x00202D44 (0xC4) CommandLine__6RwSkelFPc
signed int CommandLine(class RwSkel *this, char *command) {
    { // 0x00202C80 - 0x00202D44
        char fileName[256]; // r29+0x30
        char *s2; // r4
        char *s1; // r16
    }
}

// 0x00202D50 - 0x00202D5C (0xC) CameraSize__6RwSkelFP7_RwRect
signed int CameraSize() {
    { // 0x00202D50 - 0x00202D5C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\demoskel\sky\skyfs.cpp
// ==========================================================================
signed int (skyFexist)(char *); // 0x00203780
void * (skyFopen)(char *, char *); // 0x002038C0
signed int (skyFclose)(void *); // 0x00203800
unsigned int (skyFread)(void *, unsigned int, unsigned int, void *); // 0x002035E0
unsigned int (skyFwrite)(void *, unsigned int, unsigned int, void *); // 0x002034D0
char * (skyFgets)(char *, signed int, void *); // 0x00203150
signed int (skyFputs)(char *, void *); // 0x00203060
signed int (skyFeof)(void *); // 0x00203000
signed int (skyFseek)(void *, signed long, signed int); // 0x002032E0
signed int (skyFflush)(void *); // 0x00202FF0
unsigned int ourGlobals[4096]; // 0x004400D8
void * (_rwSkyFSOpen)(void *, signed int, signed int); // 0x00202E70
void * (_rwSkyFSClose)(void *, signed int, signed int); // 0x00202DA0
static signed int rwID_SKYDEVICEMODULE; // 0x004A2B04
static class _RwModuleInfo gdfsModuleInfo; // 0x004A51E8
static signed int gdfsOpenFiles; // 0x004A4B04
// 0x00202D60 - 0x00202DA0 (0x40)
signed int SkyInstallFileSystem() {
    { // 0x00202D60 - 0x00202DA0
    }
}

// 0x00202DA0 - 0x00202E68 (0xC8) _rwSkyFSClose__FPvii
static void *_rwSkyFSClose(void *instance) {
    { // 0x00202DA0 - 0x00202E68
        class _RwFileFunctions *FS; // r2
    }
}

// 0x00202E70 - 0x00202FF0 (0x180) _rwSkyFSOpen__FPvii
static void *_rwSkyFSOpen(void *instance, signed int offset) {
    { // 0x00202E70 - 0x00202FF0
        class _RwFileFunctions *FS; // r2
    }
}

// 0x00202FF0 - 0x00202FFC (0xC) skyFflush__FPv
static signed int skyFflush() {
    { // 0x00202FF0 - 0x00202FFC
    }
}

// 0x00203000 - 0x00203058 (0x58) skyFeof__FPv
static signed int skyFeof(void *fptr) {
    { // 0x00203000 - 0x00203058
        class skyFile *fp; // r16
    }
}

// 0x00203060 - 0x00203150 (0xF0) skyFputs__FPCcPv
static signed int skyFputs(char *buffer, void *fptr) {
    { // 0x00203060 - 0x00203150
        signed int j; // r2
        signed int i; // r16
        class skyFile *fp; // r2
    }
}

// 0x00203150 - 0x002032D8 (0x188) skyFgets__FPciPv
static char *skyFgets(char *buffer, signed int maxLen, void *fptr) {
    { // 0x00203150 - 0x002032D8
        signed int numBytesRead; // r17
        signed int i; // r16
        class skyFile *fp; // r2
    }
}

// 0x002032E0 - 0x002034C8 (0x1E8) skyFseek__FPvli
static signed int skyFseek(void *fptr, signed long offset, signed int origin) {
    { // 0x002032E0 - 0x002034C8
        signed int noBuffer; // r18
        signed int bufStart; // r4
        signed int oldFPos; // r17
        class skyFile *fp; // r16
    }
}

// 0x002034D0 - 0x002035DC (0x10C) skyFwrite__FPCvUiUiPv
static unsigned int skyFwrite(void *addr, unsigned int size, unsigned int count, void *fptr) {
    { // 0x002034D0 - 0x002035DC
        signed int numBytesToWrite; // r18
        class skyFile *fp; // r2
        signed int bytesWritten; // r2
    }
}

// 0x002035E0 - 0x0020377C (0x19C) skyFread__FPvUiUiPv
static unsigned int skyFread(void *addr, unsigned int size, unsigned int count, void *fptr) {
    { // 0x002035E0 - 0x0020377C
        signed int bytesRead2; // r17
        signed int bytesRead; // r18
        unsigned int numBytesToRead; // r17
        class skyFile *fp; // r16
    }
}

// 0x00203780 - 0x002037F8 (0x78) skyFexist__FPCc
static signed int skyFexist(char *name) {
    { // 0x00203780 - 0x002037F8
        void *res; // r16
    }
}

// 0x00203800 - 0x002038B4 (0xB4) skyFclose__FPv
static signed int skyFclose(void *fptr) {
    { // 0x00203800 - 0x002038B4
        class skyFile *fp; // r16
    }
}

// 0x002038C0 - 0x002038F4 (0x34) skyFopen__FPCcPCc
static void *skyFopen(char *name, char *access) {
    { // 0x002038C0 - 0x002038F4
        void *res; // r2
    }
}

// 0x00203900 - 0x00203BD4 (0x2D4) trySkyFopen__FPCcPCc
static void *trySkyFopen(char *fname, char *access) {
    { // 0x00203900 - 0x00203BD4
        char *nameptr; // r4
        char name[256]; // r29+0x30
        signed int mode; // r17
        class skyFile *fp; // r16
    }
}

// 0x00203BE0 - 0x00203D34 (0x154) skyTransMode__FPCc
static signed int skyTransMode(char *access) {
    { // 0x00203BE0 - 0x00203D34
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
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\demoskel\sky\sky.cpp
// ==========================================================================
void (DebugMessageHandler)(enum _RwDebugType, char *); // 0x002049F0
unsigned int ourGlobals[4096]; // 0x004400D8
void * (private_malloc)(unsigned int); // 0x00204140
void (private_free)(void *); // 0x00204110
void * (private_realloc)(void *, unsigned int); // 0x00204050
void * (private_calloc)(unsigned int, unsigned int); // 0x00203FD0
signed int (TimerHandler)(signed int); // 0x00204920
struct @anon1 __vt__8Platform; // 0x004A2720
class RWInfo gRWInfo; // 0x00B02B80
struct @anon0 __vt__2Ps; // 0x004A26E0
static class _RwMemoryFunctions bugWorkAround; // 0x004A5350
static signed int keybHandle; // 0x00000000
static signed int skyTimerHandlerHid; // 0x004A2B08
static unsigned long sweHighCount; // 0x004A5348
unsigned long gTimerValue; // 0x004A5340
static class RwSkel *Skel; // 0x004A4B10
static class Ps *PsSky; // 0x004A4B0C
u_long128 pad_dma_buf[16]; // 0x004A5240
unsigned char rdata[32]; // 0x004A5200
unsigned int paddata; // 0x004A4B08
// 0x00203D40 - 0x00203EA4 (0x164) p2RenderWareInit__Fv
signed int p2RenderWareInit() {
    { // 0x00203D40 - 0x00203EA4
    }
}

// 0x00203F10 - 0x00203F1C (0xC) NativeTextureSupport__2PsFv
signed int NativeTextureSupport() {
    { // 0x00203F10 - 0x00203F1C
    }
}

// 0x00203F20 - 0x00203F80 (0x60) InstallFileSystem__2PsFv
signed int InstallFileSystem() {
    { // 0x00203F20 - 0x00203F80
    }
}

// 0x00203F80 - 0x00203FCC (0x4C) GetMemoryFunctions__2PsFv
class _RwMemoryFunctions *GetMemoryFunctions() {
    { // 0x00203F80 - 0x00203FCC
    }
}

// 0x00203FD0 - 0x00204050 (0x80) private_calloc__FUiUi
static void *private_calloc(unsigned int a, unsigned int b) {
    { // 0x00203FD0 - 0x00204050
        void *newMem; // r2
        unsigned int realSize; // r16
    }
}

// 0x00204050 - 0x00204110 (0xC0) private_realloc__FPvUi
static void *private_realloc(void *oldMem, unsigned int newSize) {
    { // 0x00204050 - 0x00204110
        unsigned int oldSize; // r2
        void *newMem; // r2
    }
}

// 0x00204110 - 0x00204134 (0x24) private_free__FPv
static void private_free(void *alignedMem) {
    { // 0x00204110 - 0x00204134
    }
}

// 0x00204140 - 0x0020419C (0x5C) private_malloc__FUi
static void *private_malloc(unsigned int s) {
    { // 0x00204140 - 0x0020419C
        void *alignedMem; // r3
        void *newMem; // r2
    }
}

// 0x002041A0 - 0x002041EC (0x4C) SelectDevice__2PsFv
signed int SelectDevice() {
    { // 0x002041A0 - 0x002041EC
        class _RwVideoMode videoMode; // r29+0x10
    }
}

// 0x002041F0 - 0x002044E4 (0x2F4) _handlePad__Fv
void _handlePad() {
    { // 0x002041F0 - 0x002044E4
        class _RwV2d delta; // r29+0x38
        class _RwV2d delta; // r29+0x30
        signed int dRighty; // r3
        signed int dRightx; // r4
        signed int dLefty; // r3
        signed int dLeftx; // r4
        unsigned int buttons; // r16
        class _RsPadButtonStatus padButtonsUp; // r29+0x28
        class _RsPadButtonStatus padButtonsDown; // r29+0x20
        static unsigned int oldButtons; // 0x004A4B14
        static unsigned char rightV; // 0x004A4B34
        static unsigned char rightH; // 0x004A4B2C
        static unsigned char leftV; // 0x004A4B24
        static unsigned char leftH; // 0x004A4B1C
        static signed char init; // 0x004A4B38
        static signed char init; // 0x004A4B30
        static signed char init; // 0x004A4B28
        static signed char init; // 0x004A4B20
        static signed char init; // 0x004A4B18
    }
}

// 0x002044F0 - 0x002045FC (0x10C) remapButtons__Fi
static unsigned int remapButtons(signed int sceButtons) {
    { // 0x002044F0 - 0x002045FC
        unsigned int rsButtons; // r2
    }
}

// 0x00204600 - 0x0020479C (0x19C) SkyPadOpen__Fv
static signed int SkyPadOpen() {
    { // 0x00204600 - 0x0020479C
        char _PADMAN[256]; // r29+0x210
        char _SIO2MAN[256]; // r29+0x110
        char buf[256]; // r29+0x10
        static char _empty[1]; // 0x004A2B0C
    }
}

// 0x002047A0 - 0x002047AC (0xC) PathGetSeparator__2PsFv
char PathGetSeparator() {
    { // 0x002047A0 - 0x002047AC
    }
}

// 0x002047B0 - 0x002047F4 (0x44) PathnameDestroy__2PsFPc
void PathnameDestroy(char *pathname) {
    { // 0x002047B0 - 0x002047F4
    }
}

// 0x00204800 - 0x00204910 (0x110) PathnameCreate__2PsFPCc
char *PathnameCreate(char *pathname) {
    { // 0x00204800 - 0x00204910
        char *charToConvert; // r2
        char *dstBuffer; // r16
    }
}

// 0x00204910 - 0x00204920 (0x10) Timer__2PsFv
unsigned int Timer() {
    { // 0x00204910 - 0x00204920
    }
}

// 0x00204920 - 0x002049DC (0xBC) TimerHandler__Fi
static signed int TimerHandler(signed int ca) {
    { // 0x00204920 - 0x002049DC
    }
}

// 0x002049E0 - 0x002049F0 (0x10) GetDebugMessageHandler__2PsFv
void (*GetDebugMessageHandler())(enum _RwDebugType, char *) {
    { // 0x002049E0 - 0x002049F0
    }
}

// 0x002049F0 - 0x00204A18 (0x28) DebugMessageHandler__2PsF12_RwDebugTypePCc
void DebugMessageHandler(char *str) {
    { // 0x002049F0 - 0x00204A18
    }
}

// 0x00204A20 - 0x00204A4C (0x2C) CameraShowRaster__2PsFP9_RwCamera
void CameraShowRaster(class _RwCamera *camera) {
    { // 0x00204A20 - 0x00204A4C
    }
}

// 0x00204A50 - 0x00204A8C (0x3C) WarningMessage__2PsFPCc
void WarningMessage(char *message) {
    { // 0x00204A50 - 0x00204A8C
    }
}

// 0x00204A90 - 0x00204ACC (0x3C) ErrorMessage__2PsFPCc
void ErrorMessage(char *message) {
    { // 0x00204A90 - 0x00204ACC
    }
}

// 0x00204AD0 - 0x00204AD8 (0x8) WindowSetText__2PsFPCc
void WindowSetText() {
    { // 0x00204AD0 - 0x00204AD8
    }
}

// 0x00204AE0 - 0x00204B4C (0x6C) __dt__2PsFv
void *__dt(class Ps *this) {
    { // 0x00204AE0 - 0x00204B4C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\demoskel\platform.h
// ==========================================================================
// 0x00203EB0 - 0x00203F08 (0x58) __dt__8PlatformFv
void *__dt(class Platform *this) {
    { // 0x00203EB0 - 0x00203F08
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\democom\padmap.cpp
// ==========================================================================
// 0x00204B50 - 0x00204B78 (0x28) __ct__6PadMapFP6RwSkel
void *__ct(class PadMap *this, class RwSkel *skel) {
    { // 0x00204B50 - 0x00204B78
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\democom\menu.cpp
// ==========================================================================
unsigned int ourGlobals[4096]; // 0x004400D8
static char AltHelpFileName[10]; // 0x0041DC48
static char HelpFileName[13]; // 0x0041DC38
static char test[22]; // 0x0041DC20
static char *helpLines[1500]; // 0x004A5360
static char *emptyString; // 0x00000000
static class MenuStateTag MenuState; // 0x0041DBD0
// 0x00204B80 - 0x0020521C (0x69C) MenuRender__FP9_RwCameraP9_RwRasterP7RsMouse
signed int MenuRender(class _RwCamera *camera, class _RwRaster *Charset, class RsMouse *mouse) {
    { // 0x00204B80 - 0x0020521C
        signed int line; // r16
        signed int helpLine; // r3
        float value; // 
        float min; // 
        float max; // r29+0x160
        signed int index; // r3
        signed int levelOn; // r2
        char caption[128]; // r29+0xd0
        class _RtCharsetDesc charsetDesc; // r29+0xb0
        signed int invert; // r20
        signed int line; // r19
        signed int column; // r18
        signed int count; // r17
        class MenuEntryTag *ptr; // r16
    }
}

// 0x00205220 - 0x002054EC (0x2CC) MenuKeyboardHandler__F11RsMenuEventPv
enum RsEventStatus MenuKeyboardHandler(enum RsMenuEvent event, void *param) {
    { // 0x00205220 - 0x002054EC
        signed int key; // r16
        class _RsKeyStatus *keyStatus; // r2
        static signed int exitMode; // 0x004A4B3C
    }
}

// 0x002054F0 - 0x002055E8 (0xF8) MenuHandleKeyUp__FPv
static enum RsEventStatus MenuHandleKeyUp(void *param) {
    { // 0x002054F0 - 0x002055E8
        class _RsKeyStatus *keyStatus; // r2
    }
}

// 0x002055F0 - 0x0020584C (0x25C) MenuHandleKeyDown__FPv
static enum RsEventStatus MenuHandleKeyDown(void *param) {
    { // 0x002055F0 - 0x0020584C
        class _RsKeyStatus *keyStatus; // r2
    }
}

// 0x00205850 - 0x002059A8 (0x158) MenuKeyboardShortcutHandler__F11RsMenuEventPv
enum RsEventStatus MenuKeyboardShortcutHandler(enum RsMenuEvent event, void *param) {
    { // 0x00205850 - 0x002059A8
        class MenuEntryTag *ptr; // r5
        signed int key; // r3
        class _RsKeyStatus *keyStatus; // r2
    }
}

// 0x002059B0 - 0x0020615C (0x7AC) MenuPadHandler__F11RsMenuEventPv
enum RsEventStatus MenuPadHandler(enum RsMenuEvent event, void *param) {
    { // 0x002059B0 - 0x0020615C
        float percentage; // r29+0x20
        float biggestDelta; // 
        class _RwV2d *delta; // r2
        class _RsPadButtonStatus *pb; // r16
        class _RsPadButtonStatus *pb; // r16
    }
}

// 0x00206160 - 0x0020626C (0x10C) AutoRepeatPrepare__F11RsMenuEventPv
static signed int AutoRepeatPrepare(enum RsMenuEvent event, void *param) {
    { // 0x00206160 - 0x0020626C
    }
}

// 0x00206270 - 0x002064A8 (0x238) MenuSelectionAddValue__Fi
static signed int MenuSelectionAddValue(signed int increment) {
    { // 0x00206270 - 0x002064A8
        float maxValue; // 
        float minValue; // 
        float value; // r29+0x20
    }
}

// 0x002064B0 - 0x002066F0 (0x240) MenuSelectionAddPercentage__Ff
static signed int MenuSelectionAddPercentage(float percentage) {
    { // 0x002064B0 - 0x002066F0
        float maxValue; // 
        float minValue; // 
        float value; // 
    }
}

// 0x002066F0 - 0x002068A0 (0x1B0) MenuSelectPrevious__Fv
static signed int MenuSelectPrevious() {
    { // 0x002066F0 - 0x002068A0
    }
}

// 0x002068A0 - 0x00206AA8 (0x208) MenuSelectNext__Fv
static signed int MenuSelectNext() {
    { // 0x002068A0 - 0x00206AA8
    }
}

// 0x00206AB0 - 0x00206BD4 (0x124) MenuClose__Fv
signed int MenuClose() {
    { // 0x00206AB0 - 0x00206BD4
        signed int result; // r16
    }
}

// 0x00206BE0 - 0x00206D18 (0x138) MenuRemoveEntryByNumber__Fi
static signed int MenuRemoveEntryByNumber(signed int entryNum) {
    { // 0x00206BE0 - 0x00206D18
        signed int count; // r3
        class MenuEntryTag *ptr; // r2
    }
}

// 0x00206D20 - 0x00206DA0 (0x80) SelectByNumber__Fi
static signed int SelectByNumber(signed int entryNum) {
    { // 0x00206D20 - 0x00206DA0
        signed int count; // r4
        class MenuEntryTag *ptr; // r3
    }
}

// 0x00206DA0 - 0x00206DE0 (0x40) MenuToggle__Fv
signed int MenuToggle() {
    { // 0x00206DA0 - 0x00206DE0
    }
}

// 0x00206DE0 - 0x00206DF0 (0x10) MenuGetStatus__Fv
signed int MenuGetStatus() {
    { // 0x00206DE0 - 0x00206DF0
    }
}

// 0x00206DF0 - 0x00206F14 (0x124) MenuSetStatus__Fi
signed int MenuSetStatus(signed int newMode) {
    { // 0x00206DF0 - 0x00206F14
    }
}

// 0x00206F20 - 0x002071AC (0x28C) LoadHelpFile__Fv
static signed int LoadHelpFile() {
    { // 0x00206F20 - 0x002071AC
        signed int i; // r5
        signed int length; // r2
        void *buffer; // r17
        char line[79]; // r29+0x30
        class _RwFileFunctions *fileFuncs; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\democom\camera.cpp
// ==========================================================================
class xScene *xScene_current; // 0x004A4E08
static class Camera *g_GlobalCamera; // 0x004A4B40
// 0x002071B0 - 0x002072AC (0xFC) AdjustHeight__6CameraFf
void AdjustHeight(class Camera *this, float adjust) {
    { // 0x002071B0 - 0x002072AC
    }
}

// 0x002072B0 - 0x002073A0 (0xF0) AdjustTether__6CameraFf
void AdjustTether(class Camera *this, float adjust) {
    { // 0x002072B0 - 0x002073A0
    }
}

// 0x002073A0 - 0x002073E0 (0x40) NextPresetPos__6CameraFv
void NextPresetPos(class Camera *this) {
    { // 0x002073A0 - 0x002073E0
    }
}

// 0x002073E0 - 0x00207624 (0x244) __ct__6CameraFiii
void *__ct(class Camera *this, signed int width, signed int height, signed int zBuffer) {
    { // 0x002073E0 - 0x00207624
        class _RwRect rect; // r29+0x60
    }
}

// 0x00207630 - 0x00207790 (0x160) __dt__6CameraFv
void *__dt(class Camera *this) {
    { // 0x00207630 - 0x00207790
        class _RwRaster *parentRaster; // r18
        class _RwRaster *parentRaster; // r18
        class _RwFrame *frame; // r17
        class _RwRaster *raster; // r16
    }
}

// 0x00207790 - 0x00207DE0 (0x650) SetTarget__6CameraFP6_RwV3dP6_RwV3d
void SetTarget(class Camera *this, class _RwV3d *dp, class _RwV3d *target_vec) {
    { // 0x00207790 - 0x00207DE0
        signed int height; // r4
        float presets[10]; // r29+0xb0
        signed int tether; // r8
        float scale; // 
        class tagCollideInfo collision; // r29+0x70
        class _RwMatrix *cameraMatrix; // r16
        class _RwFrame *cameraFrame; // r2
        float dist; // r29+0xf0
        class _RwV3d look_at_pos; // r29+0xe0
        class _RwV3d *orig_pos; // r17
    }
}

// 0x00207DE0 - 0x00207EE0 (0x100) CameraPan__6CameraFPC6_RwV3df
void CameraPan(class Camera *this, class _RwV3d *pos, float angle) {
    { // 0x00207DE0 - 0x00207EE0
        class _RwV3d up; // r29+0x70
        class _RwV3d camPos; // r29+0x60
        class _RwMatrix *cameraMatrix; // r17
        class _RwFrame *cameraFrame; // r16
        class _RwV3d invCamPos; // r29+0x50
    }
}

// 0x00207EE0 - 0x00207FDC (0xFC) CameraTilt__6CameraFPC6_RwV3df
void CameraTilt(class Camera *this, class _RwV3d *pos, float angle) {
    { // 0x00207EE0 - 0x00207FDC
        class _RwV3d camPos; // r29+0x60
        class _RwMatrix *cameraMatrix; // r17
        class _RwFrame *cameraFrame; // r16
        class _RwV3d invCamPos; // r29+0x50
    }
}

// 0x00207FE0 - 0x00208114 (0x134) CameraMove__6CameraFP6_RwV3d
void CameraMove(class Camera *this, class _RwV3d *v) {
    { // 0x00207FE0 - 0x00208114
        class _RwV3d *right; // r2
        class _RwV3d *up; // r18
        class _RwV3d *at; // r17
        class _RwMatrix *cameraMatrix; // r16
        class _RwV3d offset; // r29+0x60
    }
}

// 0x00208120 - 0x0020831C (0x1FC) CameraSize__6CameraFP7_RwRectff
void CameraSize(class Camera *this, class _RwRect *rect, float viewWindow, float aspectRatio) {
    { // 0x00208120 - 0x0020831C
        class _RwRaster *subRaster; // r18
        class _RwRect r; // r29+0x60
        class _RwVideoMode videoMode; // r29+0x50
        class _RwV2d vw; // r29+0x78
    }
}

// 0x00208320 - 0x002085DC (0x2BC) MatrixLookAt__FP9_RwMatrixP6_RwV3d
void MatrixLookAt(class _RwMatrix *view, class _RwV3d *tp) {
    { // 0x00208320 - 0x002085DC
        class _RwV3d *t; // r2
        class _RwV3d *cp; // r2
    }
}


// ==========================================================================
// E:\CodeWarrior-R2.5\2.4_FE_Based\PS2 Support\Msl\MSL_C++\MSL_Common\Src\ios.cpp
// ==========================================================================
static struct @anon6 __RTTI__Q33std6locale5facet; // 0x00000000
struct @anon1 __vt__Q33std6locale5facet; // 0x00000000
static struct @anon4 __RTTI__Q33std8ios_base7failure; // 0x00000000
static struct @anon7 __RTTI__Q23std9exception; // 0x00000000
static struct @anon3 __RTTI__Q23std8ios_base; // 0x00000000
class _RefCountedPtr global_s; // 0x00000000
void * (__dt)(class Init *, signed short); // 0x00000000
class basic_istream cin; // 0x00000000
class basic_ostream cout; // 0x00000000
class basic_ostream clog; // 0x00000000
class basic_ostream cerr; // 0x00000000
struct @anon0 __vt__Q23std8ios_base; // 0x00000000
struct @anon2 __vt__Q33std8ios_base7failure; // 0x00000000
struct @anon5 __vt__Q23std9exception; // 0x004A2760
void * (__dt)(class failure *, signed short); // 0x00000000

// ==========================================================================
// E:\CodeWarrior-R2.5\2.4_FE_Based\PS2 Support\Msl\MSL_C++\MSL_Common\Include\vector
// ==========================================================================
// 0x0020A6D0 - 0x0020A788 (0xB8) __dt__Q23std38__vector_pod<Ui,Q23std13allocator<Ui>>Fv
void *__dt(class __vector_pod *this) {
    { // 0x0020A6D0 - 0x0020A788
    }
}

// 0x0020A7C0 - 0x0020A7E8 (0x28) cap__Q23std38__vector_pod<Ui,Q23std13allocator<Ui>>Fv
unsigned int &cap(class __vector_pod *this) {
    { // 0x0020A7C0 - 0x0020A7E8
    }
}

// 0x0020A840 - 0x0020A868 (0x28) alloc__Q23std38__vector_pod<Ui,Q23std13allocator<Ui>>Fv
class allocator &alloc(class __vector_pod *this) {
    { // 0x0020A840 - 0x0020A868
    }
}

// 0x0020A8C0 - 0x0020A8DC (0x1C) data__Q23std38__vector_pod<Ui,Q23std13allocator<Ui>>Fv
unsigned int *&data(class __vector_pod *this) {
    { // 0x0020A8C0 - 0x0020A8DC
    }
}


// ==========================================================================
// E:\CodeWarrior-R2.5\2.4_FE_Based\PS2 Support\Msl\MSL_C++\MSL_Common\Include\memory
// ==========================================================================
// 0x0020A790 - 0x0020A7B8 (0x28) deallocate__Q23std13allocator<Ui>FPUiUi
void deallocate(unsigned int *p) {
    { // 0x0020A790 - 0x0020A7B8
    }
}


// ==========================================================================
// E:\CodeWarrior-R2.5\2.4_FE_Based\PS2 Support\Msl\MSL_C++\MSL_Common\Include\msl_utility
// ==========================================================================
// 0x0020A7F0 - 0x0020A818 (0x28) second__Q210Metrowerks41compressed_pair<Q23std13allocator<Ui>,Ui>Fv
unsigned int &second(class compressed_pair *this) {
    { // 0x0020A7F0 - 0x0020A818
    }
}

// 0x0020A820 - 0x0020A838 (0x18) second__Q310Metrowerks7details47compressed_pair_imp<Q23std13allocator<Ui>,Ui,1>Fv
unsigned int &second(class compressed_pair_imp *this) {
    { // 0x0020A820 - 0x0020A838
    }
}

// 0x0020A870 - 0x0020A898 (0x28) first__Q210Metrowerks41compressed_pair<Q23std13allocator<Ui>,Ui>Fv
class allocator &first(class compressed_pair *this) {
    { // 0x0020A870 - 0x0020A898
    }
}

// 0x0020A8A0 - 0x0020A8B8 (0x18) first__Q310Metrowerks7details47compressed_pair_imp<Q23std13allocator<Ui>,Ui,1>Fv
class allocator &first(class compressed_pair_imp *this) {
    { // 0x0020A8A0 - 0x0020A8B8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xTrigger.cpp
// ==========================================================================
signed int theGlobalTriggerDisplay; // 0x004A3E90
class xScene *xScene_current; // 0x004A4E08
signed int theGlobalTriggerNumber; // 0x004A4CC4
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int gDebugGameEventCounter; // 0x004A4C84
signed int theGlobalRuleNumber; // 0x004A4C80
class xTrigger *sPlayerTrigger[16]; // 0x004AC9F0
// 0x0032C0D0 - 0x0032C2C4 (0x1F4) xTriggerPlayerExistTrigger__FP8xTriggerPi
signed int xTriggerPlayerExistTrigger(class xTrigger *t, signed int *freeSlot) {
    { // 0x0032C0D0 - 0x0032C2C4
        class xTrigger *val; // r3
        signed int freeidx; // r16
        signed int i; // r4
    }
}

// 0x0032C2D0 - 0x0032C30C (0x3C) xTriggerPlayerInit__Fv
void xTriggerPlayerInit() {
    { // 0x0032C2D0 - 0x0032C30C
        unsigned int i; // r5
    }
}

// 0x0032C310 - 0x0032C544 (0x234) xTriggerIntersectLine__FP8xTriggerP7xLine2DP6_RwV2d
signed int xTriggerIntersectLine(class xTrigger *t, class xLine2D *line, class _RwV2d *hitVector) {
    { // 0x0032C310 - 0x0032C544
        signed int intersect; // r2
    }
}

// 0x0032C550 - 0x0032C814 (0x2C4) xTriggerCircleIntersectLine__FP8xTriggerP7xLine2DP6_RwV2d
signed int xTriggerCircleIntersectLine(class xTrigger *trig, class xLine2D *l, class _RwV2d *hitVector) {
    { // 0x0032C550 - 0x0032C814
        class Vector3D ep; // r29+0x98
        class Vector3D sp; // r29+0x88
        signed int _ein; // r2
        signed int _sin; // r19
        float t2; // r29+0xac
        float t1; // r29+0xa8
        class _RwV2d center; // r29+0x80
        class _RwV2d ray_dir; // r29+0x78
        class _RwV2d ray; // r29+0x70
        signed int intersect; // r2
        float radius; // r29+0xb0
        class iTriggerResource *r; // r19
    }
}

// 0x0032C820 - 0x0032C974 (0x154) xTriggerTriIntersectLine__FP8xTriggerP7xLine2DP6_RwV2d
signed int xTriggerTriIntersectLine(class xTrigger *t, class xLine2D *l, class _RwV2d *hitVector) {
    { // 0x0032C820 - 0x0032C974
        class iTriggerResource *r; // r18
        class xLine2D triLine; // r29+0x60
        signed int intersect; // r17
        signed int i; // r16
    }
}

// 0x0032C980 - 0x0032CAD4 (0x154) xTriggerPolyIntersectLine__FP8xTriggerP7xLine2DP6_RwV2d
signed int xTriggerPolyIntersectLine(class xTrigger *t, class xLine2D *l, class _RwV2d *hitVector) {
    { // 0x0032C980 - 0x0032CAD4
        class iTriggerResource *r; // r18
        class xLine2D triLine; // r29+0x60
        signed int intersect; // r17
        signed int i; // r16
    }
}

// 0x0032CAE0 - 0x0032CEDC (0x3FC) xTriggerRectIntersectLine__FP8xTriggerP7xLine2DP6_RwV2d
signed int xTriggerRectIntersectLine(class xTrigger *t, class xLine2D *l, class _RwV2d *hitVector) {
    { // 0x0032CAE0 - 0x0032CEDC
        class iTriggerResource *r; // r17
        class xLine2D triLine; // r29+0x70
        signed int intersect; // r16
    }
}

// 0x0032CEE0 - 0x0032D14C (0x26C) xTriggerResourceIntersectPoint__FP16iTriggerResourceP8Vector3D
signed int xTriggerResourceIntersectPoint(class iTriggerResource *t, class Vector3D *point) {
    { // 0x0032CEE0 - 0x0032D14C
        class Vector3D tmp; // r29+0x60
        class Vector3D *v; // r16
        signed int intersect; // r17
    }
}

// 0x0032D150 - 0x0032D46C (0x31C) xTriggerIntersectPoint__FP8xTriggerP8Vector3D
signed int xTriggerIntersectPoint(class xTrigger *t, class Vector3D *point) {
    { // 0x0032D150 - 0x0032D46C
        class Vector3D tmp; // r29+0x80
        class Vector3D *v; // r17
        signed int intersect; // r16
    }
}

// 0x0032D470 - 0x0032DD90 (0x920) xTriggerHitPlayer__FiP8Vector3D
void xTriggerHitPlayer(signed int genericTrigIndex, class Vector3D *playerPoint) {
    { // 0x0032D470 - 0x0032DD90
        unsigned int tflags; // r18
        class xTrigger *t; // r17
        unsigned int i; // r16
    }
}

// 0x0032DD90 - 0x0032DFA4 (0x214) xTriggerGameEvent__FP8xTriggerUiUiUiUi
void xTriggerGameEvent(class xTrigger *t, unsigned int eventWho, unsigned int eventType, unsigned int eventCmd, unsigned int eventData) {
    { // 0x0032DD90 - 0x0032DFA4
        unsigned int i; // r16
    }
}

// 0x0032DFB0 - 0x0032E02C (0x7C) xTriggerEvaluate__FP8xTrigger
void xTriggerEvaluate(class xTrigger *t) {
    { // 0x0032DFB0 - 0x0032E02C
        unsigned int i; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xTime.cpp
// ==========================================================================
unsigned long gTimerValue; // 0x004A5340
float gTimeSec; // 0x004A4C8C
float deltaFrame; // 0x004A4C88
unsigned long lastTimerValue; // 0x004ACA30
// 0x0032E030 - 0x0032E10C (0xDC) xTimeIncrement__Fv
void xTimeIncrement() {
    { // 0x0032E030 - 0x0032E10C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xText.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static char *blankText; // 0x004A3E40
static char *badText; // 0x00000000
class tagxText gTextReference[320]; // 0x004ACA40
signed int gTextResetPoint; // 0x004A4C94
signed int gTextCount; // 0x004A4C90
// 0x0032E110 - 0x0032E1C4 (0xB4) xTextFind__FUi
char *xTextFind(unsigned int assetId) {
    { // 0x0032E110 - 0x0032E1C4
        char *ret; // r2
        signed int i; // r5
    }
}

// 0x0032E1D0 - 0x0032E2E4 (0x114) xTextAdd__FUiPc
void xTextAdd(unsigned int assetId, char *string) {
    { // 0x0032E1D0 - 0x0032E2E4
    }
}

// 0x0032E2F0 - 0x0032E300 (0x10) xTextLevelReset__Fv
void xTextLevelReset() {
    { // 0x0032E2F0 - 0x0032E300
    }
}

// 0x0032E300 - 0x0032E310 (0x10) xTextSetLevelReset__Fv
void xTextSetLevelReset() {
    { // 0x0032E300 - 0x0032E310
    }
}

// 0x0032E310 - 0x0032E320 (0x10) xTextInit__Fv
void xTextInit() {
    { // 0x0032E310 - 0x0032E320
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xTex.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _xTex *g_Textures[200]; // 0x004AD440
signed int g_TextureLevelReset; // 0x004A4C9C
signed int g_TextureCount; // 0x004A4C98
// 0x0032E320 - 0x0032E3C4 (0xA4) xTexFind__FUi
class _xTex *xTexFind(unsigned int assetID) {
    { // 0x0032E320 - 0x0032E3C4
        unsigned int testID; // r2
        signed int i; // r5
    }
}

// 0x0032E3D0 - 0x0032E4C0 (0xF0) xTexAdd__FP5_xTex
void xTexAdd(class _xTex *next) {
    { // 0x0032E3D0 - 0x0032E4C0
    }
}

// 0x0032E4C0 - 0x0032E4D0 (0x10) xTexLevelReset__Fv
void xTexLevelReset() {
    { // 0x0032E4C0 - 0x0032E4D0
    }
}

// 0x0032E4D0 - 0x0032E4E0 (0x10) xTexSetLevelReset__Fv
void xTexSetLevelReset() {
    { // 0x0032E4D0 - 0x0032E4E0
    }
}

// 0x0032E4E0 - 0x0032E508 (0x28) xTexInit__Fv
void xTexInit() {
    { // 0x0032E4E0 - 0x0032E508
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSystem.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
float g_DEGTORAD; // 0x00485A58
class xScene *xScene_current; // 0x004A4E08
signed int g_sceneSwapPending; // 0x004A4CCC
unsigned int g_scenePendingScene; // 0x004A4CD0
signed int g_scenePendingCamera; // 0x004A4CD4
signed int g_scenePendingMarker; // 0x004A4CD8
signed int (xFastRandom)(signed int); // 0x0032E6C0
signed int (xTimeGetMSecScript)(); // 0x0032E900
signed int (xFastRandomRange)(signed int, signed int); // 0x0032E5A0
void (arraycopy)(signed int, signed int, signed int, signed int, signed int); // 0x0032EAE0
void (scriptDebugPrintInt)(signed int); // 0x0032ED60
void (scriptDebugPrintString)(char *); // 0x0032ED30
void (scriptDebugPrintStringInt)(signed int, char *); // 0x0032ED00
signed int (scriptPlatform)(); // 0x0032ECF0
void (scriptSystemAbort)(); // 0x0032EC20
signed int (scriptIsPrivateInterpreter)(); // 0x0032ED90
void (scriptControlCamera)(signed int, signed int, signed int, signed int, signed int); // 0x0032EC00
void (scriptSceneSwitch)(signed int, signed int, signed int, signed int); // 0x0032EB90
signed int (scriptSwitchToDisk)(signed int, signed int); // 0x0032EB80
void (scriptReboot)(); // 0x0032EB30
signed int (scriptCheckSoftReset)(); // 0x0032EB20
unsigned long gTimerValue; // 0x004A5340
static signed int sFastRandomIdx; // 0x004A4CA4
static signed int sFastRandom[64]; // 0x004AD760
static signed int sFastRandomInit; // 0x004A4CA0
unsigned int gScriptSoftReset; // 0x00000000
unsigned int xRnd_seed; // 0x004A3E44
// 0x0032E510 - 0x0032E59C (0x8C) xRnd__Fi
signed int xRnd(signed int range) {
    { // 0x0032E510 - 0x0032E59C
        signed int test; // r5
    }
}

// 0x0032E5A0 - 0x0032E6B4 (0x114) xFastRandomRange__Fii
signed int xFastRandomRange(signed int min, signed int max) {
    { // 0x0032E5A0 - 0x0032E6B4
    }
}

// 0x0032E6C0 - 0x0032E7CC (0x10C) xFastRandom__Fi
signed int xFastRandom(signed int max) {
    { // 0x0032E6C0 - 0x0032E7CC
        signed int i; // r6
    }
}

// 0x0032E7D0 - 0x0032E868 (0x98) xRandomRange__Fii
signed int xRandomRange(signed int min, signed int max) {
    { // 0x0032E7D0 - 0x0032E868
        signed int num; // r2
    }
}

// 0x0032E870 - 0x0032E8FC (0x8C) xRandom__Fi
signed int xRandom(signed int max) {
    { // 0x0032E870 - 0x0032E8FC
    }
}

// 0x0032E900 - 0x0032E910 (0x10) xTimeGetMSecScript__Fv
signed int xTimeGetMSecScript() {
    { // 0x0032E900 - 0x0032E910
    }
}

// 0x0032E910 - 0x0032EAD4 (0x1C4) xSystemInit__Fv
void xSystemInit() {
    { // 0x0032E910 - 0x0032EAD4
    }
}

// 0x0032EAE0 - 0x0032EB1C (0x3C) arraycopy__Fiiiii
void arraycopy(signed int src, signed int srcp, signed int dst, signed int dstp, signed int length) {
    { // 0x0032EAE0 - 0x0032EB1C
        signed int unitSize; // r9
    }
}

// 0x0032EB20 - 0x0032EB2C (0xC) scriptCheckSoftReset__Fv
signed int scriptCheckSoftReset() {
    { // 0x0032EB20 - 0x0032EB2C
    }
}

// 0x0032EB30 - 0x0032EB80 (0x50) scriptReboot__Fv
void scriptReboot() {
    { // 0x0032EB30 - 0x0032EB80
    }
}

// 0x0032EB80 - 0x0032EB8C (0xC) scriptSwitchToDisk__Fii
signed int scriptSwitchToDisk() {
    { // 0x0032EB80 - 0x0032EB8C
    }
}

// 0x0032EB90 - 0x0032EBF4 (0x64) scriptSceneSwitch__Fiiii
void scriptSceneSwitch(signed int sceneID, signed int cameraID, signed int markerID, signed int ry) {
    { // 0x0032EB90 - 0x0032EBF4
        class Vector3D ang; // r29+0x10
    }
}

// 0x0032EC00 - 0x0032EC20 (0x20) scriptControlCamera__Fiiiii
void scriptControlCamera(signed int cmd, signed int camera, signed int version, signed int start, signed int end) {
    { // 0x0032EC00 - 0x0032EC20
    }
}

// 0x0032EC20 - 0x0032ECE4 (0xC4) scriptSystemAbort__Fv
void scriptSystemAbort() {
    { // 0x0032EC20 - 0x0032ECE4
    }
}

// 0x0032ECF0 - 0x0032ECFC (0xC) scriptPlatform__Fv
signed int scriptPlatform() {
    { // 0x0032ECF0 - 0x0032ECFC
    }
}

// 0x0032ED00 - 0x0032ED30 (0x30) scriptDebugPrintStringInt__FiPCc
void scriptDebugPrintStringInt(signed int num, char *value) {
    { // 0x0032ED00 - 0x0032ED30
    }
}

// 0x0032ED30 - 0x0032ED5C (0x2C) scriptDebugPrintString__FPCc
void scriptDebugPrintString(char *value) {
    { // 0x0032ED30 - 0x0032ED5C
    }
}

// 0x0032ED60 - 0x0032ED90 (0x30) scriptDebugPrintInt__Fi
void scriptDebugPrintInt(signed int value) {
    { // 0x0032ED60 - 0x0032ED90
    }
}

// 0x0032ED90 - 0x0032ED9C (0xC) scriptIsPrivateInterpreter__Fv
signed int scriptIsPrivateInterpreter() {
    { // 0x0032ED90 - 0x0032ED9C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xStream.cpp
// ==========================================================================
void (xStreamReadNextAsset)(class XStream *); // 0x0032F0C0
char vAssertBuf2[128]; // 0x00AFF460
static void (*initTable[36])(class XStream *); // 0x004463C0
class XStreamHeader currentHeader; // 0x004AD860
static unsigned int currentID; // 0x004A4CB0
static signed int headerUpdated; // 0x004A4CAC
static signed int streamOpen; // 0x004A4CA8
// 0x0032EDA0 - 0x0032EF28 (0x188) xStreamBlock__FP7XStream
void xStreamBlock(class XStream *stream) {
    { // 0x0032EDA0 - 0x0032EF28
    }
}

// 0x0032EF30 - 0x0032F0BC (0x18C) xStreamBeginReadMDS__FP7XStream
void xStreamBeginReadMDS(class XStream *stream) {
    { // 0x0032EF30 - 0x0032F0BC
    }
}

// 0x0032F0C0 - 0x0032F1B8 (0xF8) xStreamReadNextAsset__FP7XStream
void xStreamReadNextAsset(class XStream *stream) {
    { // 0x0032F0C0 - 0x0032F1B8
        signed int type; // r5
    }
}

// 0x0032F1C0 - 0x0032F33C (0x17C) xStreamReadHeader__FP7XStream
void xStreamReadHeader(class XStream *stream) {
    { // 0x0032F1C0 - 0x0032F33C
    }
}

// 0x0032F340 - 0x0032F388 (0x48) xStreamSeek__FP7XStream
void xStreamSeek(class XStream *stream) {
    { // 0x0032F340 - 0x0032F388
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xState.cpp
// ==========================================================================
signed int (xStateTransitionIsPossible)(signed int, signed int, signed int); // 0x003302A0
class tagxActorDBData *gadb; // 0x004A4658
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x0032F390 - 0x0032F40C (0x7C) xCharStateReset__FiP9tagxState
void xCharStateReset(signed int stateMax, class tagxState *s) {
    { // 0x0032F390 - 0x0032F40C
        class tagXAnim *anim; // r3
        signed int i; // r7
    }
}

// 0x0032F410 - 0x0032F70C (0x2FC) xCharStateInit__FiP9tagxState
void xCharStateInit(signed int stateMax, class tagxState *s) {
    { // 0x0032F410 - 0x0032F70C
        class tagXAnim *anim; // r17
        signed int i; // r16
    }
}

// 0x0032F710 - 0x0032F820 (0x110) xStateDoCallout__FP7_xActorP15tagxStateThreadi
void xStateDoCallout(class _xActor *actor, class tagxStateThread *thread, signed int callout) {
    { // 0x0032F710 - 0x0032F820
    }
}

// 0x0032F820 - 0x0032F8C8 (0xA8) xStateIdle__FP7_xActor
void xStateIdle(class _xActor *actor) {
    { // 0x0032F820 - 0x0032F8C8
        class tagxStateMgr *sinfo; // r17
        class tagxStateThread *thread; // r2
        signed int t; // r16
    }
}

// 0x0032F8D0 - 0x0033029C (0x9CC) xStateTransition__FP7_xActorP13tagxPadAction
signed int xStateTransition(class _xActor *actor, class tagxPadAction *action) {
    { // 0x0032F8D0 - 0x0033029C
        char actorString[256]; // r29+0x190
        char actionString[256]; // r29+0x90
        signed int n; // r2
        signed int alreadyActive; // r2
        class tagxStateThread *newThread; // r22
        signed int alreadyActive; // r2
        signed int firstAvail; // r22
        signed int n; // r2
        signed int t2; // r2
        class iActorRuntime *a; // r2
        signed int found; // r23
        class tagxStateMgr *sinfo; // r21
        unsigned int command; // r20
        class tagxStateThread *thread; // r19
        class tagxStateLink *l; // r18
        signed int t; // r17
        signed int i; // r3
    }
}

// 0x003302A0 - 0x003303D0 (0x130) xStateTransitionIsPossible__Fiii
signed int xStateTransitionIsPossible(signed int actorType, signed int actorId, signed int command) {
    { // 0x003302A0 - 0x003303D0
        signed int n; // r4
        class tagxStateMgr *sinfo; // r2
        class tagxStateThread *thread; // r2
        class tagxStateLink *l; // r8
        signed int t; // r7
        signed int i; // r5
    }
}

// 0x003303D0 - 0x00330450 (0x80) xStateLevelRestart__FP7_xActor
void xStateLevelRestart(class _xActor *actor) {
    { // 0x003303D0 - 0x00330450
        class tagxStateThread *thread; // r16
        class tagxStateMgr *sinfo; // r6
    }
}

// 0x00330450 - 0x0033049C (0x4C) xStateLevelReset__FP7_xActor
void xStateLevelReset(class _xActor *actor) {
    { // 0x00330450 - 0x0033049C
        class tagxStateThread *thread; // r2
        class tagxStateMgr *sinfo; // r5
        signed int t; // r4
    }
}

// 0x003304A0 - 0x003305B0 (0x110) xStateInit__FP7_xActorP9tagxStateiPFP7_xActoriP8tagXAnimP15tagXAnimTrigger_v
void xStateInit(class _xActor *actor, class tagxState *states, signed int animationBasedMovement, void (*triggerHandler)(class _xActor *, signed int, class tagXAnim *, class tagXAnimTrigger *)) {
    { // 0x003304A0 - 0x003305B0
        class tagxStateMgr *sinfo; // r2
        class tagxStateThread *thread; // r16
        signed int t; // r6
    }
}

// 0x003305B0 - 0x003305E4 (0x34) xStateInitialize__Fv
void xStateInitialize() {
    { // 0x003305B0 - 0x003305E4
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSpanBuf.cpp
// ==========================================================================
static class xSpan *m_dead; // 0x00000000
// 0x003305F0 - 0x003305F8 (0x8) xSpanBufferInsert__FP11xSpanBufferffffi
void xSpanBufferInsert() {
    { // 0x003305F0 - 0x003305F8
    }
}

// 0x00330600 - 0x00330608 (0x8) xSpanBufferInit__FP11xSpanBuffer
void xSpanBufferInit() {
    { // 0x00330600 - 0x00330608
    }
}

// 0x00330610 - 0x00330618 (0x8) xSpanBufferDispose__FP11xSpanBuffer
void xSpanBufferDispose() {
    { // 0x00330610 - 0x00330618
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSndMgr.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
char scriptSoundAssetName[16]; // 0x00000000
class _xSndMgrData gSndMgr; // 0x004B1F00
// 0x00330620 - 0x00330824 (0x204) xSndMgrSetVoicePosition__Fifff
void xSndMgrSetVoicePosition(signed int voice, float x, float y, float z) {
    { // 0x00330620 - 0x00330824
    }
}

// 0x00330830 - 0x003309E4 (0x1B4) xSndMgrSetVoiceRadius__Fif
void xSndMgrSetVoiceRadius(signed int voice, float radius) {
    { // 0x00330830 - 0x003309E4
    }
}

// 0x003309F0 - 0x00330A68 (0x78) xSndMgrGetUnusedVoice__Fv
signed int xSndMgrGetUnusedVoice() {
    { // 0x003309F0 - 0x00330A68
        signed int i; // r2
    }
}

// 0x00330A70 - 0x00330B48 (0xD8) xSndMgrLoadSoundBuffer__FPUcUi
signed int xSndMgrLoadSoundBuffer(unsigned char *buffer, unsigned int length) {
    { // 0x00330A70 - 0x00330B48
        signed int soundID; // r2
    }
}

// 0x00330B50 - 0x00330C04 (0xB4) xSndMgrGetAssetSND_ID__FUi
signed int xSndMgrGetAssetSND_ID(unsigned int assetID) {
    { // 0x00330B50 - 0x00330C04
        signed int i; // r2
    }
}

// 0x00330C10 - 0x00330C94 (0x84) xSndMgrSetListenerPosition__Ffff
void xSndMgrSetListenerPosition(float posX, float posY, float posZ) {
    { // 0x00330C10 - 0x00330C94
    }
}

// 0x00330CA0 - 0x00330CE4 (0x44) xSndMgrSetListenerRadius__Ff
void xSndMgrSetListenerRadius(float radius) {
    { // 0x00330CA0 - 0x00330CE4
    }
}

// 0x00330CF0 - 0x00330DD0 (0xE0) xSndMgrPlay3DSoundFreq__FiiiP8Vector3D
signed int xSndMgrPlay3DSoundFreq(signed int soundID, signed int soundVol, signed int freq, class Vector3D *p) {
    { // 0x00330CF0 - 0x00330DD0
        signed int v; // r2
    }
}

// 0x00330DD0 - 0x00330E00 (0x30) xSndMgrIdle__FUl
void xSndMgrIdle(unsigned long time) {
    { // 0x00330DD0 - 0x00330E00
    }
}

// 0x00330E00 - 0x003310BC (0x2BC) xSndMgrLevelReset__Fv
void xSndMgrLevelReset() {
    { // 0x00330E00 - 0x003310BC
    }
}

// 0x003310C0 - 0x00331410 (0x350) xSndMgrSetLevelReset__Fv
void xSndMgrSetLevelReset() {
    { // 0x003310C0 - 0x00331410
        signed int i; // r5
    }
}

// 0x00331410 - 0x00331CBC (0x8AC) xSndMgrPrepareNonStaticSoundBanks__Fv
void xSndMgrPrepareNonStaticSoundBanks() {
    { // 0x00331410 - 0x00331CBC
        unsigned int sramGenericSize; // r17
        unsigned int freeSRAM; // r2
        unsigned int usedSRAM; // r16
    }
}

// 0x00331CC0 - 0x0033202C (0x36C) xSndMgrRemoveWavFromBankList__FP5_iWAV
void xSndMgrRemoveWavFromBankList(class _iWAV *wav) {
    { // 0x00331CC0 - 0x0033202C
        class _iWAV *prev; // r16
        class _iWAV *idx; // r3
    }
}

// 0x00332030 - 0x00332320 (0x2F0) xSndMgrAddWavToBankList__FP5_iWAV
void xSndMgrAddWavToBankList(class _iWAV *wav) {
    { // 0x00332030 - 0x00332320
        class _iWAV *prev; // r17
        class _iWAV *idx; // r16
    }
}

// 0x00332320 - 0x003324A4 (0x184) xSndMgrSetActiveSoundBank__Fi
void xSndMgrSetActiveSoundBank(signed int bank) {
    { // 0x00332320 - 0x003324A4
    }
}

// 0x003324B0 - 0x003324D0 (0x20) xSndMgrDispose__Fv
void xSndMgrDispose() {
    { // 0x003324B0 - 0x003324D0
    }
}

// 0x003324D0 - 0x00332544 (0x74) xSndMgrMasterVolumeChange__Fv
void xSndMgrMasterVolumeChange() {
    { // 0x003324D0 - 0x00332544
        class _iSFX *sp; // r6
        signed int i; // r5
    }
}

// 0x00332550 - 0x00332948 (0x3F8) xSndMgrInit__Fv
void xSndMgrInit() {
    { // 0x00332550 - 0x00332948
        signed int i; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSkel.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int g_DNPCSkeletonCount; // 0x004A4CC0
signed int g_SNPCSkeletonCount; // 0x004A4CBC
signed int g_BossSkeletonCount; // 0x004A4CB8
signed int g_AshSkeletonCount; // 0x004A4CB4
class xSkel g_DNPCSkeleton[5]; // 0x004B5340
class xSkel g_SNPCSkeleton[5]; // 0x004B5310
class xSkel g_BossSkeleton[2]; // 0x004B5300
class xSkel g_AshSkeleton[1]; // 0x004B52F8
// 0x00332950 - 0x00332968 (0x18) xSkelInit__Fv
void xSkelInit() {
    { // 0x00332950 - 0x00332968
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSDA.cpp
// ==========================================================================
float g_DEGTORAD; // 0x00485A58
class tagxState g_sdaStates[31]; // 0x00446830
static class tagxStateLink link_sda_DeathRange4[1]; // 0x004A3E8C
static class tagxStateLink link_sda_DeathRange3[1]; // 0x004A3E88
static class tagxStateLink link_sda_DeathRange2[1]; // 0x004A3E84
static class tagxStateLink link_sda_DeathRange1[1]; // 0x004A3E80
static class tagxStateLink link_sda_DeathMelee4[1]; // 0x004A3E7C
static class tagxStateLink link_sda_DeathMelee3[1]; // 0x004A3E78
static class tagxStateLink link_sda_DeathMelee2[1]; // 0x004A3E74
static class tagxStateLink link_sda_DeathMelee1[1]; // 0x004A3E70
static class tagxStateLink link_sda_InjuryUpUpperBodyThread[1]; // 0x004A3E6C
static class tagxStateLink link_sda_InjuryDownUpperBodyThread[1]; // 0x004A3E68
static class tagxStateLink link_sda_InjuryRightUpperBodyThread[1]; // 0x004A3E64
static class tagxStateLink link_sda_InjuryLeftUpperBodyThread[1]; // 0x004A3E60
static class tagxStateLink link_sda_InjuryFrontUpperBodyThread[1]; // 0x004A3E5C
static class tagxStateLink link_sda_InjuryUp[1]; // 0x004A3E58
static class tagxStateLink link_sda_InjuryDown[1]; // 0x004A3E54
static class tagxStateLink link_sda_InjuryRight[1]; // 0x004A3E50
static class tagxStateLink link_sda_InjuryLeft[1]; // 0x004A3E4C
static class tagxStateLink link_sda_InjuryFront[1]; // 0x004A3E48
static class tagxStateLink link_sda_AttackKick[6]; // 0x00446810
static class tagxStateLink link_sda_AttackClaw2[6]; // 0x004467F0
static class tagxStateLink link_sda_AttackClaw1[6]; // 0x004467D0
static class tagxStateLink link_sda_AttackUp[6]; // 0x004467B0
static class tagxStateLink link_sda_AttackRight[6]; // 0x00446790
static class tagxStateLink link_sda_AttackLeft[6]; // 0x00446770
static class tagxStateLink link_sda_DodgeRight[6]; // 0x00446750
static class tagxStateLink link_sda_DodgeLeft[6]; // 0x00446730
static class tagxStateLink link_sda_Run[15]; // 0x004466F0
static class tagxStateLink link_sda_WalkBack[14]; // 0x004466B0
static class tagxStateLink link_sda_Walk[15]; // 0x00446670
static class tagxStateLink link_sda_StandRRun[23]; // 0x00446610
static class tagxStateLink link_sda_Idle[24]; // 0x004465B0
unsigned char g_sdaJointsUpperOnly[25]; // 0x00446590
unsigned char g_sdaJointsAll[25]; // 0x00446570
// 0x00332970 - 0x0033299C (0x2C) xSDAStateReset__Fv
void xSDAStateReset() {
    { // 0x00332970 - 0x0033299C
    }
}

// 0x003329A0 - 0x003329CC (0x2C) xSDAStateInit__Fv
void xSDAStateInit() {
    { // 0x003329A0 - 0x003329CC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xScrFx.cpp
// ==========================================================================
void (xScrFxScriptLetterbox)(signed int); // 0x00332C90
void (xScrFxScriptFadeOut)(); // 0x00332C40
void (xScrFxScriptFadeIn)(); // 0x00332BF0
void (xScrFxScriptShake)(signed int); // 0x00332BD0
signed int (xScrFxScriptBusy)(); // 0x00332B70
void (xScrFxScriptComplete)(); // 0x00332AC0
class __xScrFxMgr gScrFx; // 0x004B5368
// 0x003329D0 - 0x003329F0 (0x20) xScrFxShake__Fi
void xScrFxShake(signed int permilli) {
    { // 0x003329D0 - 0x003329F0
    }
}

// 0x003329F0 - 0x00332A3C (0x4C) xScrFxFadeIn__Fv
void xScrFxFadeIn() {
    { // 0x003329F0 - 0x00332A3C
    }
}

// 0x00332A40 - 0x00332A8C (0x4C) xScrFxFadeOut__Fv
void xScrFxFadeOut() {
    { // 0x00332A40 - 0x00332A8C
    }
}

// 0x00332A90 - 0x00332AB8 (0x28) xScrFxLetterbox__Fi
void xScrFxLetterbox(signed int flag) {
    { // 0x00332A90 - 0x00332AB8
    }
}

// 0x00332AC0 - 0x00332B68 (0xA8) xScrFxScriptComplete__Fv
void xScrFxScriptComplete() {
    { // 0x00332AC0 - 0x00332B68
    }
}

// 0x00332B70 - 0x00332BCC (0x5C) xScrFxScriptBusy__Fv
signed int xScrFxScriptBusy() {
    { // 0x00332B70 - 0x00332BCC
    }
}

// 0x00332BD0 - 0x00332BF0 (0x20) xScrFxScriptShake__Fi
void xScrFxScriptShake(signed int permilli) {
    { // 0x00332BD0 - 0x00332BF0
    }
}

// 0x00332BF0 - 0x00332C3C (0x4C) xScrFxScriptFadeIn__Fv
void xScrFxScriptFadeIn() {
    { // 0x00332BF0 - 0x00332C3C
    }
}

// 0x00332C40 - 0x00332C8C (0x4C) xScrFxScriptFadeOut__Fv
void xScrFxScriptFadeOut() {
    { // 0x00332C40 - 0x00332C8C
    }
}

// 0x00332C90 - 0x00332CB8 (0x28) xScrFxScriptLetterbox__Fi
void xScrFxScriptLetterbox(signed int flag) {
    { // 0x00332C90 - 0x00332CB8
    }
}

// 0x00332CC0 - 0x00332D80 (0xC0) xScrFxRegisterScriptCalls__Fv
void xScrFxRegisterScriptCalls() {
    { // 0x00332CC0 - 0x00332D80
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xScene.cpp
// ==========================================================================
void (xSceneScriptLightSetMode)(signed int, signed int); // 0x00338F60
void (xSceneScriptLightSetSpeed)(signed int, signed int); // 0x00338BE0
void (xSceneScriptLightSetFrameSpeed)(signed int, signed int); // 0x00338A20
void (xSceneScriptLightSetWeight)(signed int, signed int); // 0x00338DA0
void (xSceneScriptLightSetEnable)(signed int, signed int); // 0x00338880
void (xSceneScriptLightSetFalloff)(signed int, signed int, signed int); // 0x003384D0
void (xSceneScriptLightSetRange)(signed int, signed int, signed int); // 0x00338200
void (xSceneScriptLightSetPositionMarker)(signed int, signed int, signed int); // 0x00337E60
void (xSceneScriptLightSetColor)(signed int, signed int, signed int, signed int, signed int); // 0x00337B40
void (xSceneScriptLightSetAmbient)(signed int, signed int, signed int); // 0x003379F0
void (xSceneScriptLightSetSunColor)(signed int, signed int, signed int); // 0x00337460
void (xSceneScriptLightSetSunDirection)(signed int, signed int); // 0x003375B0
void (xSceneScriptTriggerEnable)(signed int, signed int); // 0x00337350
void (xSceneScriptEntryPointEnable)(signed int, signed int); // 0x00337240
signed int (xSceneScriptGetNavPointCount)(); // 0x003398D0
signed int (xSceneScriptGetMarkerCount)(); // 0x00339800
signed int (xSceneScriptIsObjMarkerVisible)(signed int); // 0x00339510
signed int (xSceneScriptIsNPCMarkerVisible)(signed int); // 0x00339100
signed int (xSceneScriptIsNPCInVisArea)(signed int, signed int); // 0x003393F0
signed int gDebugGameEventCounter; // 0x004A4C84
unsigned char gAudioStatus; // 0x004A4E34
signed int g_BossModelCount; // 0x004A4D3C
signed int g_BossSkeletonCount; // 0x004A4CB8
signed int g_BossAnimationsCount; // 0x004A4EA4
signed int g_DNPCModelCount; // 0x004A4D44
signed int g_DNPCSkeletonCount; // 0x004A4CC0
signed int g_DNPCAnimationsCount; // 0x004A4EAC
class tagxObjDBData *godb; // 0x004A3FAC
signed int gPlayerID; // 0x004A4E04
class tagxActorDBData *gadb; // 0x004A4658
signed int gPlayerType; // 0x004A4E00
signed int gDebugDrawDebugInfo; // 0x004A4E1C
signed int gGameState; // 0x004A4640
signed int gBossBattle; // 0x004A4F70
class xScene *xScene_current; // 0x004A4E08
class tag_xCameraControl gCamera; // 0x00AA67C0
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
unsigned char gDisableCameraTriggers; // 0x004A4CFC
static signed int sDefaultCameraEnd; // 0x004A4CF8
static signed int sDefaultCameraStart; // 0x004A4CF4
static signed int sDefaultCameraVersion; // 0x004A4CF0
static signed int sDefaultCameraIndex; // 0x004A4CEC
static signed int sDefaultCameraCommand; // 0x004A4CE8
static signed int sDefaultCameraChangeInProgress; // 0x004A4CE4
static unsigned int interpreterMethod; // 0x004A4CE0
signed int gDebugLight; // 0x004A4CDC
class Vector3D g_scenePendingAng; // 0x004B5378
signed int g_scenePendingMarker; // 0x004A4CD8
signed int g_scenePendingCamera; // 0x004A4CD4
unsigned int g_scenePendingScene; // 0x004A4CD0
signed int g_sceneSwapPending; // 0x004A4CCC
static signed int sLastEvaluateCamera; // 0x004A3E9C
static signed int sLastRejectedCameraEnd; // 0x00000000
static signed int sLastRejectedCameraStart; // 0x00000000
static signed int sLastRejectedCameraVersion; // 0x00000000
static signed int sLastRejectedCameraCommand; // 0x00000000
static signed int sLastRejectedCameraIndex; // 0x00000000
static signed int sDisableCameraChange; // 0x004A4CC8
signed int nextCameraCounter; // 0x004A3E98
signed int gLastCameraIndex; // 0x004A3E94
signed int theGlobalTriggerDisplay; // 0x004A3E90
signed int theGlobalTriggerNumber; // 0x004A4CC4
// 0x00332D80 - 0x00332F48 (0x1C8) xSceneAnimateCamera__Fv
void xSceneAnimateCamera() {
    { // 0x00332D80 - 0x00332F48
        signed int nextFrame; // r16
        class iCameraResource *cam; // r2
    }
}

// 0x00332F50 - 0x0033341C (0x4CC) xSceneControlCamera__Fiiiii
void xSceneControlCamera(signed int cmd, signed int theCamera, signed int version, signed int start, signed int end) {
    { // 0x00332F50 - 0x0033341C
        class tag_xCameraControl next; // r29+0xd0
        signed int nextFrame; // r22
        signed int nextSpeed; // r18
        signed int nextEnd; // r23
        signed int nextStart; // r17
        class iCameraResource *cam; // r16
    }
}

// 0x00333420 - 0x00333500 (0xE0) xSceneScriptControlCamera__Fiiiii
void xSceneScriptControlCamera(signed int cmd, signed int theCamera, signed int version, signed int start, signed int end) {
    { // 0x00333420 - 0x00333500
    }
}

// 0x00333500 - 0x003335C4 (0xC4) xSceneSetCamera__Fii
void xSceneSetCamera(signed int c, signed int v) {
    { // 0x00333500 - 0x003335C4
    }
}

// 0x003335D0 - 0x00333720 (0x150) xSceneVFXVisible__FP6xSceneP8Vector3D
signed int xSceneVFXVisible(class xScene *s, class Vector3D *p) {
    { // 0x003335D0 - 0x00333720
        unsigned int mask; // r3
        class iTriggerResource *t; // r2
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        signed int i; // r16
        signed int vis; // r2
    }
}

// 0x00333720 - 0x00333870 (0x150) xSceneRTObjVisible__FP6xSceneP8Vector3D
signed int xSceneRTObjVisible(class xScene *s, class Vector3D *p) {
    { // 0x00333720 - 0x00333870
        unsigned int mask; // r3
        class iTriggerResource *t; // r2
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        signed int i; // r16
        signed int vis; // r2
    }
}

// 0x00333870 - 0x003339C0 (0x150) xSceneNPCVisible__FP6xSceneP8Vector3D
signed int xSceneNPCVisible(class xScene *s, class Vector3D *p) {
    { // 0x00333870 - 0x003339C0
        unsigned int mask; // r3
        class iTriggerResource *t; // r2
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        signed int i; // r16
        signed int vis; // r2
    }
}

// 0x003339C0 - 0x00333B10 (0x150) xSceneLightVisible__FP6xSceneP8Vector3D
signed int xSceneLightVisible(class xScene *s, class Vector3D *p) {
    { // 0x003339C0 - 0x00333B10
        unsigned int mask; // r3
        class iTriggerResource *t; // r2
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        signed int i; // r16
        signed int vis; // r2
    }
}

// 0x00333B10 - 0x00333C60 (0x150) xScenePlayerVisible__FP6xSceneP8Vector3D
signed int xScenePlayerVisible(class xScene *s, class Vector3D *p) {
    { // 0x00333B10 - 0x00333C60
        unsigned int mask; // r3
        class iTriggerResource *t; // r2
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        signed int i; // r16
        signed int vis; // r2
    }
}

// 0x00333C60 - 0x00333E4C (0x1EC) xSceneEvaluateTriggers__FP6xScene
void xSceneEvaluateTriggers(class xScene *s) {
    { // 0x00333C60 - 0x00333E4C
        unsigned int tflags; // r17
        signed int camerachange; // r2
        class xTrigger *t; // r2
        unsigned int i; // r16
    }
}

// 0x00333E50 - 0x003340A4 (0x254) xSceneCheckTriggerHitPlayer__FP6xScene
void xSceneCheckTriggerHitPlayer(class xScene *s) {
    { // 0x00333E50 - 0x003340A4
        class iActorRuntime *r; // r3
        unsigned int *maskp; // r19
        unsigned int mask; // r4
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        class Vector3D p; // r29+0x80
        unsigned int i; // r16
    }
}

// 0x003340B0 - 0x00334408 (0x358) xSceneGetTriggerMaterials__FP6xSceneP8Vector3DPUcUi
signed int xSceneGetTriggerMaterials(class xScene *s, class Vector3D *p, unsigned char *mat, unsigned int maxMat) {
    { // 0x003340B0 - 0x00334408
        unsigned char val; // r4
        unsigned int flags; // r3
        unsigned int mask; // r3
        unsigned int count; // r16
        unsigned int pmasklo; // r23
        unsigned int pmaskhi; // r30
        unsigned int pmask; // r2
        class xTrigger *t; // r22
        unsigned int i; // r21
    }
}

// 0x00334410 - 0x00334588 (0x178) xSceneDebugLights__FP6xScene
void xSceneDebugLights(class xScene *s) {
    { // 0x00334410 - 0x00334588
        class Vector3D b; // r29+0x40
        class Vector3D a; // r29+0x30
        unsigned int i; // r16
    }
}

// 0x00334590 - 0x00334708 (0x178) xSceneDebugDrawTriggers__FP6xScene
void xSceneDebugDrawTriggers(class xScene *s) {
    { // 0x00334590 - 0x00334708
        signed int b; // r2
        signed int g; // r2
        signed int r; // r2
        class Vector3D *v; // r2
        class xTrigger *t; // r10
        unsigned int i; // r16
    }
}

// 0x00334710 - 0x003347BC (0xAC) xSceneDebugDrawEntryPoints__FP11xEntryPoint
void xSceneDebugDrawEntryPoints(class xEntryPoint *e) {
    { // 0x00334710 - 0x003347BC
        signed int b; // r2
        signed int g; // r2
        signed int r; // r2
    }
}

// 0x003347C0 - 0x00334B84 (0x3C4) xSceneCheckEntryHitPlayer__FP6xScene
void xSceneCheckEntryHitPlayer(class xScene *s) {
    { // 0x003347C0 - 0x00334B84
        class xEntryPoint *e; // r21
        class iActorRuntime *r; // r3
        unsigned int pmask; // r3
        unsigned int maskHi; // r20
        unsigned int maskLo; // r19
        unsigned int mask; // r2
        class Vector3D *ppos; // r18
        unsigned int i; // r17
    }
}

// 0x00334B90 - 0x003350D0 (0x540) xSceneActivate__Fv
void xSceneActivate() {
    { // 0x00334B90 - 0x003350D0
        unsigned char *resetPointer; // r18
        signed int i; // r17
        signed int t; // r16
        class XStream stream; // r29+0x40
    }
}

// 0x003350D0 - 0x0033552C (0x45C) xSceneSwitch__FP6xSceneUiUiUiP8Vector3D
void xSceneSwitch(class xScene *s, unsigned int sceneID, unsigned int cameraID, unsigned int markerID, class Vector3D *ang) {
    { // 0x003350D0 - 0x0033552C
    }
}

// 0x00335530 - 0x0033573C (0x20C) xSceneIdle__FP6xScene
void xSceneIdle(class xScene *s) {
    { // 0x00335530 - 0x0033573C
        class xCondition *c; // r5
        unsigned int i; // r16
        class xCondition *cend; // r4
    }
}

// 0x00335740 - 0x00335ADC (0x39C) xSceneDebugDrawGeom__FiP8Vector3Diii
void xSceneDebugDrawGeom(signed int type, class Vector3D *v, signed int red, signed int green, signed int blue) {
    { // 0x00335740 - 0x00335ADC
        class Vector3D a; // r29+0x110
        class Vector3D c; // r29+0x100
        class Vector3D b; // r29+0xf0
        class Vector3D a; // r29+0xe0
        class Vector3D d; // r29+0xd0
        class Vector3D c; // r29+0xc0
        class Vector3D b; // r29+0xb0
        class Vector3D a; // r29+0xa0
        class Vector3D d; // r29+0x90
        class Vector3D c; // r29+0x80
        class Vector3D b; // r29+0x70
        class Vector3D a; // r29+0x60
    }
}

// 0x00335AE0 - 0x00337240 (0x1760) xSceneInit__FP6xSceneUi
void xSceneInit(class xScene *s, unsigned int sceneId) {
    { // 0x00335AE0 - 0x00337240
        signed int cmd; // r3
        signed int onflags; // r4
        class xTrigger *t; // r20
        class xEntryPoint *idx; // r16
        class xTrigger *trigger; // r20
        class Vector3D d; // r29+0xe0
        class xRule *r; // r18
        class xCondition *c; // r19
        unsigned int idxMaterial; // r30
        unsigned int idxEffect; // r29+0xd0
        unsigned int idxGeneric; // r18
        unsigned int idxWalk; // r29+0xc0
        unsigned int idxNoWalk; // r23
        unsigned int k; // r17
        unsigned int j; // r16
        unsigned int i; // r22
    }
}

// 0x00337240 - 0x00337350 (0x110) xSceneScriptEntryPointEnable__Fii
void xSceneScriptEntryPointEnable(signed int id, signed int state) {
    { // 0x00337240 - 0x00337350
    }
}

// 0x00337350 - 0x00337460 (0x110) xSceneScriptTriggerEnable__Fii
void xSceneScriptTriggerEnable(signed int id, signed int state) {
    { // 0x00337350 - 0x00337460
    }
}

// 0x00337460 - 0x003375AC (0x14C) xSceneScriptLightSetSunColor__Fiii
void xSceneScriptLightSetSunColor(signed int r, signed int g, signed int b) {
    { // 0x00337460 - 0x003375AC
    }
}

// 0x003375B0 - 0x003379E8 (0x438) xSceneScriptLightSetSunDirection__Fii
void xSceneScriptLightSetSunDirection(signed int m1, signed int m2) {
    { // 0x003375B0 - 0x003379E8
    }
}

// 0x003379F0 - 0x00337B3C (0x14C) xSceneScriptLightSetAmbient__Fiii
void xSceneScriptLightSetAmbient(signed int r, signed int g, signed int b) {
    { // 0x003379F0 - 0x00337B3C
    }
}

// 0x00337B40 - 0x00337E54 (0x314) xSceneScriptLightSetColor__Fiiiii
void xSceneScriptLightSetColor(signed int idx, signed int r, signed int g, signed int b, signed int anchorIdx) {
    { // 0x00337B40 - 0x00337E54
        class iLightResource *rsrc; // r4
    }
}

// 0x00337E60 - 0x003381FC (0x39C) xSceneScriptLightSetPositionMarker__Fiii
void xSceneScriptLightSetPositionMarker(signed int idx, signed int midx, signed int anchorIdx) {
    { // 0x00337E60 - 0x003381FC
        class iLightResource *r; // r5
    }
}

// 0x00338200 - 0x003384C4 (0x2C4) xSceneScriptLightSetRange__Fiii
void xSceneScriptLightSetRange(signed int idx, signed int range, signed int anchorIdx) {
    { // 0x00338200 - 0x003384C4
        class iLightResource *r; // r4
    }
}

// 0x003384D0 - 0x0033887C (0x3AC) xSceneScriptLightSetFalloff__Fiii
void xSceneScriptLightSetFalloff(signed int idx, signed int falloff, signed int anchorIdx) {
    { // 0x003384D0 - 0x0033887C
    }
}

// 0x00338880 - 0x00338A20 (0x1A0) xSceneScriptLightSetEnable__Fii
void xSceneScriptLightSetEnable(signed int idx, signed int enabled) {
    { // 0x00338880 - 0x00338A20
    }
}

// 0x00338A20 - 0x00338BD8 (0x1B8) xSceneScriptLightSetFrameSpeed__Fii
void xSceneScriptLightSetFrameSpeed(signed int idx, signed int rate) {
    { // 0x00338A20 - 0x00338BD8
    }
}

// 0x00338BE0 - 0x00338D9C (0x1BC) xSceneScriptLightSetSpeed__Fii
void xSceneScriptLightSetSpeed(signed int idx, signed int speed) {
    { // 0x00338BE0 - 0x00338D9C
    }
}

// 0x00338DA0 - 0x00338F5C (0x1BC) xSceneScriptLightSetWeight__Fii
void xSceneScriptLightSetWeight(signed int idx, signed int weight) {
    { // 0x00338DA0 - 0x00338F5C
    }
}

// 0x00338F60 - 0x00339100 (0x1A0) xSceneScriptLightSetMode__Fii
void xSceneScriptLightSetMode(signed int idx, signed int mode) {
    { // 0x00338F60 - 0x00339100
    }
}

// 0x00339100 - 0x003393E8 (0x2E8) xSceneScriptIsNPCMarkerVisible__Fi
signed int xSceneScriptIsNPCMarkerVisible(signed int markerID) {
    { // 0x00339100 - 0x003393E8
    }
}

// 0x003393F0 - 0x00339510 (0x120) xSceneScriptIsNPCInVisArea__Fii
signed int xSceneScriptIsNPCInVisArea(signed int type, signed int id) {
    { // 0x003393F0 - 0x00339510
        class _xActor *a; // r16
        class Vector3D pos; // r29+0x40
    }
}

// 0x00339510 - 0x003397F8 (0x2E8) xSceneScriptIsObjMarkerVisible__Fi
signed int xSceneScriptIsObjMarkerVisible(signed int markerID) {
    { // 0x00339510 - 0x003397F8
    }
}

// 0x00339800 - 0x003398C4 (0xC4) xSceneScriptGetMarkerCount__Fv
signed int xSceneScriptGetMarkerCount() {
    { // 0x00339800 - 0x003398C4
    }
}

// 0x003398D0 - 0x00339994 (0xC4) xSceneScriptGetNavPointCount__Fv
signed int xSceneScriptGetNavPointCount() {
    { // 0x003398D0 - 0x00339994
    }
}

// 0x003399A0 - 0x003399AC (0xC) xSceneReInitializeInterpreter__Fv
void xSceneReInitializeInterpreter() {
    { // 0x003399A0 - 0x003399AC
    }
}

// 0x003399B0 - 0x00339BDC (0x22C) xSceneRegisterScriptCalls__Fv
void xSceneRegisterScriptCalls() {
    { // 0x003399B0 - 0x00339BDC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSAL.cpp
// ==========================================================================
class tagxState g_salStates[3]; // 0x00446B40
static class tagxStateLink link_sal_LookFoot[1]; // 0x004A3EA4
static class tagxStateLink link_sal_LookBack[1]; // 0x004A3EA0
static class tagxStateLink link_sal_Idle[2]; // 0x00446B38
unsigned char g_salJointsUpperOnly[17]; // 0x00000000
unsigned char g_salJointsAll[17]; // 0x00446B20
// 0x00339BE0 - 0x00339C0C (0x2C) xSALStateReset__Fv
void xSALStateReset() {
    { // 0x00339BE0 - 0x00339C0C
    }
}

// 0x00339C10 - 0x00339C3C (0x2C) xSALStateInit__Fv
void xSALStateInit() {
    { // 0x00339C10 - 0x00339C3C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSAE.cpp
// ==========================================================================
class tagxState g_saeStates[12]; // 0x00446C00
static class tagxStateLink link_sae_LookAround[1]; // 0x004A3EB4
static class tagxStateLink link_sae_WipeHands[1]; // 0x004A3EB0
static class tagxStateLink link_sae_ScratchLeg[1]; // 0x004A3EAC
static class tagxStateLink link_sae_WipeBrow[1]; // 0x004A3EA8
static class tagxStateLink link_sae_RunBook[2]; // 0x00446BF8
static class tagxStateLink link_sae_WalkBook[2]; // 0x00446BF0
static class tagxStateLink link_sae_Run[2]; // 0x00446BE8
static class tagxStateLink link_sae_Walk[2]; // 0x00446BE0
static class tagxStateLink link_sae_IdleReadyBook[4]; // 0x00446BD0
static class tagxStateLink link_sae_IdleReady[4]; // 0x00446BC0
static class tagxStateLink link_sae_IdleBook[4]; // 0x00446BB0
static class tagxStateLink link_sae_Idle[4]; // 0x00446BA0
unsigned char g_saeJointsUpperOnly[16]; // 0x00000000
unsigned char g_saeJointsAll[16]; // 0x00446B90
// 0x00339C40 - 0x00339C6C (0x2C) xSAEStateReset__Fv
void xSAEStateReset() {
    { // 0x00339C40 - 0x00339C6C
    }
}

// 0x00339C70 - 0x00339C9C (0x2C) xSAEStateInit__Fv
void xSAEStateInit() {
    { // 0x00339C70 - 0x00339C9C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xRule.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x00339CA0 - 0x00339FA4 (0x304) xRuleExecute__FP5xRule
void xRuleExecute(class xRule *rule) {
    { // 0x00339CA0 - 0x00339FA4
        unsigned int i; // r17
    }
}

// 0x00339FB0 - 0x0033A2A4 (0x2F4) xRuleEvaluate__FP5xRule
void xRuleEvaluate(class xRule *rule) {
    { // 0x00339FB0 - 0x0033A2A4
    }
}

// 0x0033A2B0 - 0x0033A5CC (0x31C) xRuleGameEvent__FP5xRuleUiUiUiUi
void xRuleGameEvent(class xRule *rule, unsigned int eventWho, unsigned int eventType, unsigned int eventCmd, unsigned int eventData) {
    { // 0x0033A2B0 - 0x0033A5CC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xRouteDB.cpp
// ==========================================================================
signed int gPlayerType; // 0x004A4E00
signed int gPlayerID; // 0x004A4E04
class tagxObjDBData *godb; // 0x004A3FAC
class tagxActorDBData *gadb; // 0x004A4658
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class xRoute gRouteArray[16]; // 0x004B53E0
static class xRouteDB *db; // 0x004A3EB8
class xRouteDB g_routeDB; // 0x004B5390
// 0x0033A5D0 - 0x0033A830 (0x260) xRouteDBSetRouteUseage__Fii
void xRouteDBSetRouteUseage(signed int id, signed int useageType) {
    { // 0x0033A5D0 - 0x0033A830
        class xRoute *r; // r16
    }
}

// 0x0033A830 - 0x0033AA78 (0x248) xRouteDBRouteStatus__Fi
signed int xRouteDBRouteStatus(signed int id) {
    { // 0x0033A830 - 0x0033AA78
    }
}

// 0x0033AA80 - 0x0033AAD0 (0x50) xRouteDBRouteDraw__Fi
void xRouteDBRouteDraw(signed int routeID) {
    { // 0x0033AA80 - 0x0033AAD0
        class xRoute *r; // r2
    }
}

// 0x0033AAD0 - 0x0033AC7C (0x1AC) xRouteDBRouteCreate__Fv
signed int xRouteDBRouteCreate() {
    { // 0x0033AAD0 - 0x0033AC7C
        signed int i; // r16
    }
}

// 0x0033AC80 - 0x0033AD68 (0xE8) xRouteDBRouteDestroy__Fi
void xRouteDBRouteDestroy(signed int routeID) {
    { // 0x0033AC80 - 0x0033AD68
    }
}

// 0x0033AD70 - 0x0033B1E8 (0x478) xRouteDBRouteRebuild__FP6xRoute
signed int xRouteDBRouteRebuild(class xRoute *r) {
    { // 0x0033AD70 - 0x0033B1E8
        class xListItem *anchor; // r17
        class xListItem *idx; // r2
        signed int routeID; // r16
    }
}

// 0x0033B1F0 - 0x0033B4CC (0x2DC) xRouteDBRouteBuild__FP8Vector3DP8Vector3Di
signed int xRouteDBRouteBuild(class Vector3D *pa, class Vector3D *pb, signed int physType) {
    { // 0x0033B1F0 - 0x0033B4CC
        class xRoute *r; // r17
        signed int routeID; // r16
    }
}

// 0x0033B4D0 - 0x0033B950 (0x480) xRouteDBRouteToNavigationPoint__Fiii
signed int xRouteDBRouteToNavigationPoint(signed int objectType, signed int actorID, signed int index) {
    { // 0x0033B4D0 - 0x0033B950
        class xRoute *r; // r2
        class Vector3D posa; // r29+0x70
        class _xActor *actor; // r17
        signed int routeID; // r16
    }
}

// 0x0033B950 - 0x0033BE14 (0x4C4) xRouteDBRouteMarkerToMarker__Fii
signed int xRouteDBRouteMarkerToMarker(signed int ma, signed int mb) {
    { // 0x0033B950 - 0x0033BE14
        class xRoute *r; // r2
        class xMarker *marker2; // r17
        class xMarker *marker1; // r4
        signed int routeID; // r16
    }
}

// 0x0033BE20 - 0x0033C304 (0x4E4) xRouteDBRouteNavToNav__Fii
signed int xRouteDBRouteNavToNav(signed int na, signed int nb) {
    { // 0x0033BE20 - 0x0033C304
        class xRoute *r; // r2
        signed int routeID; // r16
    }
}

// 0x0033C310 - 0x0033C818 (0x508) xRouteDBRouteToMemoryPosition__Fiii
signed int xRouteDBRouteToMemoryPosition(signed int objectType, signed int actorID, signed int index) {
    { // 0x0033C310 - 0x0033C818
        class xRoute *r; // r2
        class Vector3D posa; // r29+0x70
        class _xActor *actor; // r17
        signed int routeID; // r16
    }
}

// 0x0033C820 - 0x0033CD7C (0x55C) xRouteDBRouteToRTObj__Fiii
signed int xRouteDBRouteToRTObj(signed int actorType, signed int actorID, signed int objIndex) {
    { // 0x0033C820 - 0x0033CD7C
        class xRoute *r; // r2
        class Vector3D posa; // r29+0x70
        class _xObj *obj; // r17
        signed int routeID; // r16
    }
}

// 0x0033CD80 - 0x0033D3B4 (0x634) xRouteDBRouteToMarker__Fiii
signed int xRouteDBRouteToMarker(signed int objectType, signed int actorID, signed int index) {
    { // 0x0033CD80 - 0x0033D3B4
        class xRoute *r; // r2
        class Vector3D posa; // r29+0x80
        class xMarker *marker; // r17
        signed int routeID; // r16
    }
}

// 0x0033D3C0 - 0x0033D790 (0x3D0) xRouteDBRouteToActor__Fiiii
signed int xRouteDBRouteToActor(signed int fromObjectType, signed int fromActorID, signed int toObjectType, signed int toActorID) {
    { // 0x0033D3C0 - 0x0033D790
        class xRoute *r; // r2
        class _xActor *to_actor; // r18
        class _xActor *from_actor; // r17
        signed int routeID; // r16
        class Vector3D posa; // r29+0x80
    }
}

// 0x0033D790 - 0x0033D89C (0x10C) xRouteDBRouteToPlayer__Fii
signed int xRouteDBRouteToPlayer(signed int objectID, signed int actorID) {
    { // 0x0033D790 - 0x0033D89C
        signed int routeID; // r16
    }
}

// 0x0033D8A0 - 0x0033DC18 (0x378) xRouteDBRouteAppend__Fii
void xRouteDBRouteAppend(signed int routeID, signed int routeAppendID) {
    { // 0x0033D8A0 - 0x0033DC18
        class xRoute *rb; // r17
        class xRoute *ra; // r16
    }
}

// 0x0033DC20 - 0x0033EB18 (0xEF8) xRouteDBIdle__Fv
void xRouteDBIdle() {
    { // 0x0033DC20 - 0x0033EB18
        class xSearch *s; // r17
        class xRoute *r; // r18
        signed char sid; // r17
        signed int i; // r16
    }
}

// 0x0033EB20 - 0x0033ED7C (0x25C) xRouteDBSetScene__FP6xScene
void xRouteDBSetScene(class xScene *scene) {
    { // 0x0033EB20 - 0x0033ED7C
        class xRoute *r; // r17
        signed int i; // r16
    }
}

// 0x0033ED80 - 0x0033EFA8 (0x228) xRouteDBGetRoutePtr__Fi
class xRoute *xRouteDBGetRoutePtr(signed int id) {
    { // 0x0033ED80 - 0x0033EFA8
    }
}

// 0x0033EFB0 - 0x0033F050 (0xA0) xRouteDBInit__Fv
void xRouteDBInit() {
    { // 0x0033EFB0 - 0x0033F050
        signed int i; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xRoute.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x0033F050 - 0x0033F17C (0x12C) xRouteClearPath__FP6xRoute
void xRouteClearPath(class xRoute *r) {
    { // 0x0033F050 - 0x0033F17C
        class xListItem *i; // r16
    }
}

// 0x0033F180 - 0x0033F29C (0x11C) xRouteOccupyPath__FP6xRoute
void xRouteOccupyPath(class xRoute *r) {
    { // 0x0033F180 - 0x0033F29C
        class xListItem *i; // r16
    }
}

// 0x0033F2A0 - 0x0033F498 (0x1F8) xRouteBuildSearchRoute__FP6xRouteP12xSearchPoint
void xRouteBuildSearchRoute(class xRoute *r, class xSearchPoint *p) {
    { // 0x0033F2A0 - 0x0033F498
    }
}

// 0x0033F4A0 - 0x0033F54C (0xAC) xRouteBuildSearchRouteAB__FP6xRouteP7xSearchP8Vector3DP8Vector3D
void xRouteBuildSearchRouteAB(class xRoute *r, class xSearch *s, class Vector3D *a, class Vector3D *b) {
    { // 0x0033F4A0 - 0x0033F54C
    }
}

// 0x0033F550 - 0x0033F768 (0x218) xRouteDestroyRoute__FP6xRoute
void xRouteDestroyRoute(class xRoute *r) {
    { // 0x0033F550 - 0x0033F768
    }
}

// 0x0033F770 - 0x0033F958 (0x1E8) xRouteDraw__FP6xRoute
void xRouteDraw(class xRoute *r) {
    { // 0x0033F770 - 0x0033F958
        class xNavPoint *b; // r2
        class xNavPoint *a; // r3
        class Vector3D posb; // r29+0x60
        class Vector3D posa; // r29+0x50
        class xListItem *i; // r17
        float shift; // 
        class Vector3D lastPoint; // r29+0x40
    }
}

// 0x0033F960 - 0x0033F9D4 (0x74) xRouteGetNextNavPoint__FP6xRouteP9xNavPoint
class xNavPoint *xRouteGetNextNavPoint(class xRoute *r, class xNavPoint *p) {
    { // 0x0033F960 - 0x0033F9D4
        class xListItem *i; // r3
    }
}

// 0x0033F9E0 - 0x0033FAB4 (0xD4) xRouteSetState__FP6xRoutei
void xRouteSetState(class xRoute *r, signed int newState) {
    { // 0x0033F9E0 - 0x0033FAB4
    }
}

// 0x0033FAC0 - 0x0033FAF8 (0x38) xRouteInit__FP6xRoute
void xRouteInit(class xRoute *r) {
    { // 0x0033FAC0 - 0x0033FAF8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPlayer.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagxActorDBData *gadb; // 0x004A4658
signed int gPlayerType; // 0x004A4E00
static signed int functionIndex; // 0x004A4D00
class tagxPlayer g_Players[1]; // 0x004B5A20
// 0x0033FB00 - 0x0033FC98 (0x198) xPlayerResetAnimation__Fv
void xPlayerResetAnimation() {
    { // 0x0033FB00 - 0x0033FC98
        unsigned int playerDeadFlag; // r29+0x4c
        class xMessage theMessage; // r29+0x20
        signed int i; // r16
    }
}

// 0x0033FCA0 - 0x0033FD4C (0xAC) xPlayerSelectWeapon__Fiii
void xPlayerSelectWeapon(signed int player, signed int left, signed int weapon) {
    { // 0x0033FCA0 - 0x0033FD4C
        class tagxPlayer *p; // r16
    }
}

// 0x0033FD50 - 0x0033FD58 (0x8) xPadActionFunc_Player__FP7tagxPadiP13tagxPadAction
void xPadActionFunc_Player() {
    { // 0x0033FD50 - 0x0033FD58
    }
}

// 0x0033FD60 - 0x003403F4 (0x694) xPadMapperFunc_Player__FP7tagxPadP13tagxPadActioni
signed int xPadMapperFunc_Player(class tagxPad *pad, class tagxPadAction *actions, signed int maxActions) {
    { // 0x0033FD60 - 0x003403F4
        class tagxPadAction *a; // r5
        class _XBUTTON *b; // r4
        signed int i; // r3
        signed int actionCount; // r2
    }
}

// 0x00340400 - 0x003404AC (0xAC) xPlayerInit__FiP7_xActori
void xPlayerInit(signed int player, class _xActor *actor) {
    { // 0x00340400 - 0x003404AC
        signed int o; // r17
        class tagxPlayer *p; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPlate.cpp
// ==========================================================================
// 0x003404B0 - 0x003404B8 (0x8) xPlateReset__Fv
void xPlateReset() {
    { // 0x003404B0 - 0x003404B8
    }
}

// 0x003404C0 - 0x003404C8 (0x8) xPlateAdd__FP7XStream
void xPlateAdd() {
    { // 0x003404C0 - 0x003404C8
    }
}

// 0x003404D0 - 0x003404D8 (0x8) xPlateInit__Fv
void xPlateInit() {
    { // 0x003404D0 - 0x003404D8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPhysObj.cpp
// ==========================================================================
// 0x003404E0 - 0x0034053C (0x5C) xPhysicsObjectSetOrientation__FP17tagxPhysicsObjectfff
void xPhysicsObjectSetOrientation(class tagxPhysicsObject *p, float rx, float ry, float rz) {
    { // 0x003404E0 - 0x0034053C
    }
}

// 0x00340540 - 0x003405C0 (0x80) xPhysicsObjectTranslatePosition__FP17tagxPhysicsObjectfff
void xPhysicsObjectTranslatePosition(class tagxPhysicsObject *p, float x, float y, float z) {
    { // 0x00340540 - 0x003405C0
    }
}

// 0x003405C0 - 0x0034061C (0x5C) xPhysicsObjectSetPosition__FP17tagxPhysicsObjectfff
void xPhysicsObjectSetPosition(class tagxPhysicsObject *p, float x, float y, float z) {
    { // 0x003405C0 - 0x0034061C
    }
}

// 0x00340620 - 0x00340660 (0x40) xPhysicsObjectInitialize__FP17tagxPhysicsObject
void xPhysicsObjectInitialize(class tagxPhysicsObject *p) {
    { // 0x00340620 - 0x00340660
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPhysics.cpp
// ==========================================================================
signed int gDebugDrawDebugInfo; // 0x004A4E1C
// 0x00340660 - 0x003407D0 (0x170) xPhysicsClipVelocity__FP8Vector3DP8Vector3DP8Vector3Df
void xPhysicsClipVelocity(class Vector3D *in, class Vector3D *normal, class Vector3D *out, float overbounce) {
    { // 0x00340660 - 0x003407D0
        float backoff; // 
    }
}

// 0x003407D0 - 0x00340960 (0x190) xPhysicsMovePhysicsObject__FP17tagxPhysicsObject
void xPhysicsMovePhysicsObject(class tagxPhysicsObject *o) {
    { // 0x003407D0 - 0x00340960
        class Vector3D c; // r29+0x30
        float ct; // r29+0x3c
        class Vector3D dst; // r29+0x20
    }
}

// 0x00340960 - 0x00340BD4 (0x274) xPhysicsCalcMotionVector__FP17tagxPhysicsObject
void xPhysicsCalcMotionVector(class tagxPhysicsObject *o) {
    { // 0x00340960 - 0x00340BD4
        signed int collision; // r2
        static class tagMATRIX m2; // 0x004B5B30
        static class Vector3D vd; // 0x004B5B70
        static class tagMATRIX m1; // 0x004B5AF0
        static class tagMATRIX mz; // 0x004B5AB0
        static class tagMATRIX mx; // 0x004B5A30
        static class tagMATRIX my; // 0x004B5A70
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPathfdr.cpp
// ==========================================================================
unsigned int gTime; // 0x004A4E10
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int gDebugLOSCounter; // 0x004A4D04
signed int gDebugLOSFast; // 0x00000000
class xNavPoint *startPt; // 0x00000000
class xSearchPoint gSP[4][150]; // 0x004B5C70
static class xPathfinder *pf; // 0x004A3EBC
class xPathfinder gPathfinder; // 0x004B5B80
// 0x00340BE0 - 0x00341464 (0x884) xPathfinderAStarGenerateSuccessors__FP7xSearchP12xSearchPoint
signed int xPathfinderAStarGenerateSuccessors(class xSearch *s, class xSearchPoint *parent) {
    { // 0x00340BE0 - 0x00341464
        class xSearchPoint *tmp; // r2
        class xSearchPoint *tmp; // r2
        float f; // 
        float heuristic; // 
        float cost; // 
        unsigned int i; // r18
        class xSearchPoint *sp; // r16
        class xNavPoint *node; // r17
        class xNavLink *link; // r16
    }
}

// 0x00341470 - 0x0034175C (0x2EC) xPathfinderSetScene__FP6xScene
void xPathfinderSetScene(class xScene *s) {
    { // 0x00341470 - 0x0034175C
        class xSearchPoint *p; // r6
        unsigned int j; // r3
        unsigned int i; // r16
    }
}

// 0x00341760 - 0x003418A8 (0x148) xPathfinderFindLOSNearestNotInPortal__FP8Vector3Di
class xNavPoint *xPathfinderFindLOSNearestNotInPortal(class Vector3D *p, signed int physObj) {
    { // 0x00341760 - 0x003418A8
        float bestDistance; // 
        class xNavPoint *best; // r17
        float idxDistance; // 
        class xNavPoint *idx; // r20
        unsigned int i; // r16
    }
}

// 0x003418B0 - 0x00341B00 (0x250) xPathfinderNavPointInPortal__FP9xNavPoint
signed int xPathfinderNavPointInPortal(class xNavPoint *nav) {
    { // 0x003418B0 - 0x00341B00
        class xEntryPoint *e; // r2
        unsigned int pmask; // r3
        unsigned int maskHi; // r19
        unsigned int maskLo; // r18
        unsigned int mask; // r2
        class Vector3D *ppos; // r17
        unsigned int i; // r16
    }
}

// 0x00341B00 - 0x00341C18 (0x118) xPathfinderFindLOSNearest__FP8Vector3Di
class xNavPoint *xPathfinderFindLOSNearest(class Vector3D *p, signed int physObj) {
    { // 0x00341B00 - 0x00341C18
        float bestDistance; // 
        class xNavPoint *best; // r17
        float idxDistance; // 
        class xNavPoint *idx; // r20
        unsigned int i; // r16
    }
}

// 0x00341C20 - 0x00341ED8 (0x2B8) xPathfinderLineOfSight__FP8Vector3DP8Vector3Di
signed int xPathfinderLineOfSight(class Vector3D *a, class Vector3D *b, signed int physObjectType) {
    { // 0x00341C20 - 0x00341ED8
        unsigned int him; // r2
        unsigned int lom; // r16
        unsigned int *maskp; // r19
        unsigned int mask; // r3
        unsigned int lineFloorMaskLo; // r18
        unsigned int lineFloorMaskHi; // r17
        unsigned int lineFloorMask; // r2
        class xLine2D line; // r29+0x70
        class _RwV2d hitVector[2]; // r29+0x60
        unsigned int i; // r16
        class xTrigger *idx; // r2
    }
}

// 0x00341EE0 - 0x00341F30 (0x50) xPathfinderLineOfSightPhysObj__FP17tagxPhysicsObjectP8Vector3D
signed int xPathfinderLineOfSightPhysObj(class tagxPhysicsObject *po, class Vector3D *b) {
    { // 0x00341EE0 - 0x00341F30
        class Vector3D pos; // r29+0x10
    }
}

// 0x00341F30 - 0x00341FE0 (0xB0) xPathfinderIdle__Fv
void xPathfinderIdle() {
    { // 0x00341F30 - 0x00341FE0
        signed int i; // r16
    }
}

// 0x00341FE0 - 0x00342554 (0x574) xPathfinderSearchIdle__FP7xSearch
void xPathfinderSearchIdle(class xSearch *s) {
    { // 0x00341FE0 - 0x00342554
        class xListItem *best; // r17
        class xListItem *idx; // r3
        class xSearchPoint *node; // r16
    }
}

// 0x00342560 - 0x00342C44 (0x6E4) xPathfinderSearchStart__FSc
void xPathfinderSearchStart(signed char id) {
    { // 0x00342560 - 0x00342C44
        class xSearchPoint *p; // r17
        class xSearch *s; // r16
    }
}

// 0x00342C50 - 0x00342E64 (0x214) xPathfinderSearchFree__FSc
void xPathfinderSearchFree(signed char id) {
    { // 0x00342C50 - 0x00342E64
    }
}

// 0x00342E70 - 0x00343114 (0x2A4) xPathfinderSearchRestart__FSc
void xPathfinderSearchRestart(signed char id) {
    { // 0x00342E70 - 0x00343114
        signed int i; // r6
    }
}

// 0x00343120 - 0x003432DC (0x1BC) xPathfinderSearchGetPtr__FSc
class xSearch *xPathfinderSearchGetPtr(signed char id) {
    { // 0x00343120 - 0x003432DC
    }
}

// 0x003432E0 - 0x00343480 (0x1A0) xPathfinderSearchNew__Fv
signed char xPathfinderSearchNew() {
    { // 0x003432E0 - 0x00343480
        signed int i; // r2
    }
}

// 0x00343480 - 0x00343798 (0x318) xPathfinderInit__FP6xScene
void xPathfinderInit(class xScene *scene) {
    { // 0x00343480 - 0x00343798
        signed int j; // r3
        signed int i; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPAS.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagxPlayer g_Players[0]; // 0x004B5A20
class tagxObjDBData *godb; // 0x004A3FAC
class tagxActorDBData *gadb; // 0x004A4658
signed int gPlayerType; // 0x004A4E00
class tagxState g_pasStates[163]; // 0x00448190
static class tagxStateLink link_pas_Ash_LowerBody_WalkFromRun[4]; // 0x00448180
static class tagxStateLink link_pas_Ash_LowerBody_Run[4]; // 0x00448170
static class tagxStateLink link_pas_Ash_LowerBody_RunFromWalk[4]; // 0x00448160
static class tagxStateLink link_pas_Ash_LowerBody_Walk[4]; // 0x00448150
static class tagxStateLink link_pas_Ash_LowerBody_WalkBack[1]; // 0x004A3FA8
static class tagxStateLink link_pas_oldAsh_LowerBody_WalkFromRun[2]; // 0x00448140
static class tagxStateLink link_pas_oldAsh_LowerBody_Run[2]; // 0x00448138
static class tagxStateLink link_pas_oldAsh_LowerBody_RunFromWalk[2]; // 0x00448130
static class tagxStateLink link_pas_oldAsh_LowerBody_Walk[2]; // 0x00448128
static class tagxStateLink link_pas_AshWithSaw_PushLow[9]; // 0x00448100
static class tagxStateLink link_pas_AshWithSaw_PushMedium[9]; // 0x004480D0
static class tagxStateLink link_pas_AshWithSaw_ClimbDown[1]; // 0x004A3FA4
static class tagxStateLink link_pas_AshWithSaw_ClimbUp[1]; // 0x004A3FA0
static class tagxStateLink link_pas_AshWithSaw_PickRoof[13]; // 0x00448090
static class tagxStateLink link_pas_AshWithSaw_PickTable[13]; // 0x00448050
static class tagxStateLink link_pas_AshWithSaw_PickFloor[13]; // 0x00448010
static class tagxStateLink link_pas_AshWithSaw_Dodge_Left_WithWeapon[1]; // 0x004A3F9C
static class tagxStateLink link_pas_AshWithSaw_Dodge_Right_WithWeapon[1]; // 0x004A3F98
static class tagxStateLink link_pas_AshWithSaw_Rotate_CounterClockwise_Anim[6]; // 0x00447FF0
static class tagxStateLink link_pas_AshWithSaw_Rotate_Clockwise_Anim[6]; // 0x00447FD0
static class tagxStateLink link_pas_AshWithSaw_Rotate_CounterClockwise[3]; // 0x00447FB8
static class tagxStateLink link_pas_AshWithSaw_Rotate_Clockwise[3]; // 0x00447FA8
static class tagxStateLink link_pas_AshWithSaw_Armed_HitBack_Upper[1]; // 0x004A3F94
static class tagxStateLink link_pas_AshWithSaw_Armed_HitUp_Upper[1]; // 0x004A3F90
static class tagxStateLink link_pas_AshWithSaw_Armed_HitDown_Upper[1]; // 0x004A3F8C
static class tagxStateLink link_pas_AshWithSaw_Armed_HitRight_Upper[1]; // 0x004A3F88
static class tagxStateLink link_pas_AshWithSaw_Armed_HitLeft_Upper[1]; // 0x004A3F84
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBack_Upper[1]; // 0x004A3F80
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitUp_Upper[1]; // 0x004A3F7C
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitDown_Upper[1]; // 0x004A3F78
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitRight_Upper[1]; // 0x004A3F74
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitLeft_Upper[1]; // 0x004A3F70
static class tagxStateLink link_pas_AshWithSaw_LookUp_Upper[1]; // 0x004A3F6C
static class tagxStateLink link_pas_AshWithSaw_LookAround_Upper[1]; // 0x004A3F68
static class tagxStateLink link_pas_AshWithSaw_Taunt3_Upper[1]; // 0x004A3F64
static class tagxStateLink link_pas_AshWithSaw_Taunt2_Upper[1]; // 0x004A3F60
static class tagxStateLink link_pas_AshWithSaw_Taunt1_Upper[1]; // 0x004A3F5C
static class tagxStateLink link_pas_AshWithSaw_Startle2[5]; // 0x00447F90
static class tagxStateLink link_pas_AshWithSaw_Startle1[5]; // 0x00447F70
static class tagxStateLink link_pas_AshWithSaw_Taunt3[9]; // 0x00447F40
static class tagxStateLink link_pas_AshWithSaw_Taunt2[9]; // 0x00447F10
static class tagxStateLink link_pas_AshWithSaw_Taunt1[9]; // 0x00447EE0
static class tagxStateLink link_pas_AshWithSaw_Armed_HitBack[6]; // 0x00447EC0
static class tagxStateLink link_pas_AshWithSaw_Armed_HitUp[6]; // 0x00447EA0
static class tagxStateLink link_pas_AshWithSaw_Armed_HitDown[6]; // 0x00447E80
static class tagxStateLink link_pas_AshWithSaw_Armed_HitRight[6]; // 0x00447E60
static class tagxStateLink link_pas_AshWithSaw_Armed_HitLeft[6]; // 0x00447E40
static class tagxStateLink link_pas_AshWithSaw_AttackRight3_Upper[1]; // 0x004A3F58
static class tagxStateLink link_pas_AshWithSaw_AttackRight2_Upper[1]; // 0x004A3F54
static class tagxStateLink link_pas_AshWithSaw_AttackRight1_Upper[1]; // 0x004A3F50
static class tagxStateLink link_pas_AshWithSaw_AttackRight3_Rifle[6]; // 0x00447E20
static class tagxStateLink link_pas_AshWithSaw_AttackRight2_Rifle[6]; // 0x00447E00
static class tagxStateLink link_pas_AshWithSaw_AttackRight1_Rifle[6]; // 0x00447DE0
static class tagxStateLink link_pas_AshWithSaw_Armed_HitBalls_Rifle[1]; // 0x004A3F4C
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBalls_Rifle[1]; // 0x004A3F48
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitStrong_Rifle[1]; // 0x004A3F44
static class tagxStateLink link_pas_AshWithSaw_Armed_HitStrong_Rifle[1]; // 0x004A3F40
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBack_Rifle[7]; // 0x00447DC0
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitUp_Rifle[7]; // 0x00447DA0
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitDown_Rifle[7]; // 0x00447D80
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitRight_Rifle[7]; // 0x00447D60
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitLeft_Rifle[7]; // 0x00447D40
static class tagxStateLink link_pas_AshWithSaw_AttackComboLeft_Rifle[1]; // 0x004A3F3C
static class tagxStateLink link_pas_AshWithSaw_AttackComboRight_Rifle[3]; // 0x00447D30
static class tagxStateLink link_pas_AshWithSaw_WalkingBack_Relaxed_Rifle[24]; // 0x00447CD0
static class tagxStateLink link_pas_AshWithSaw_Running_Relaxed_Rifle[25]; // 0x00447C60
static class tagxStateLink link_pas_AshWithSaw_Walking_Relaxed_Rifle[25]; // 0x00447BF0
static class tagxStateLink link_pas_AshWithSaw_Reload_Rifle_Upper[1]; // 0x004A3F38
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Rifle_Upper[1]; // 0x004A3F34
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Rifle_Upper[1]; // 0x004A3F30
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Rifle_Upper[1]; // 0x004A3F2C
static class tagxStateLink link_pas_AshWithSaw_Reload_Rifle[4]; // 0x00447BE0
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Rifle[6]; // 0x00447BC0
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Rifle[6]; // 0x00447BA0
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Rifle[6]; // 0x00447B80
static class tagxStateLink link_pas_AshWithSaw_Armed_Rifle_Upper[10]; // 0x00447B50
static class tagxStateLink link_pas_AshWithSaw_Idle_Armed_Rifle[36]; // 0x00447AC0
static class tagxStateLink link_pas_AshWithSaw_Idle_Relaxed_Rifle[41]; // 0x00447A10
static class tagxStateLink link_pas_AshWithSaw_AttackRight3_Pistol[6]; // 0x004479F0
static class tagxStateLink link_pas_AshWithSaw_AttackRight2_Pistol[6]; // 0x004479D0
static class tagxStateLink link_pas_AshWithSaw_AttackRight1_Pistol[6]; // 0x004479B0
static class tagxStateLink link_pas_AshWithSaw_Armed_HitBalls_Pistol[1]; // 0x004A3F28
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBalls_Pistol[1]; // 0x004A3F24
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitStrong_Pistol[1]; // 0x004A3F20
static class tagxStateLink link_pas_AshWithSaw_Armed_HitStrong_Pistol[1]; // 0x004A3F1C
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBack_Pistol[7]; // 0x00447990
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitUp_Pistol[7]; // 0x00447970
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitDown_Pistol[7]; // 0x00447950
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitRight_Pistol[7]; // 0x00447930
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitLeft_Pistol[7]; // 0x00447910
static class tagxStateLink link_pas_AshWithSaw_AttackComboLeft_Pistol[1]; // 0x004A3F18
static class tagxStateLink link_pas_AshWithSaw_AttackComboRight_Pistol[3]; // 0x00447900
static class tagxStateLink link_pas_AshWithSaw_WalkingBack_Relaxed_Pistol[24]; // 0x004478A0
static class tagxStateLink link_pas_AshWithSaw_Running_Relaxed_Pistol[25]; // 0x00447830
static class tagxStateLink link_pas_AshWithSaw_Walking_Relaxed_Pistol[25]; // 0x004477C0
static class tagxStateLink link_pas_AshWithSaw_Reload_Pistol_Upper[1]; // 0x004A3F14
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Pistol_Upper[1]; // 0x004A3F10
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Pistol_Upper[1]; // 0x004A3F0C
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Pistol_Upper[1]; // 0x004A3F08
static class tagxStateLink link_pas_AshWithSaw_Reload_Pistol[4]; // 0x004477B0
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Pistol[6]; // 0x00447790
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Pistol[6]; // 0x00447770
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Pistol[6]; // 0x00447750
static class tagxStateLink link_pas_AshWithSaw_Armed_Pistol_Upper[10]; // 0x00447720
static class tagxStateLink link_pas_AshWithSaw_Idle_Armed_Pistol[36]; // 0x00447690
static class tagxStateLink link_pas_AshWithSaw_Idle_Relaxed_Pistol[41]; // 0x004475E0
static class tagxStateLink link_pas_AshWithSaw_AttackRight3_Shotgun[6]; // 0x004475C0
static class tagxStateLink link_pas_AshWithSaw_AttackRight2_Shotgun[6]; // 0x004475A0
static class tagxStateLink link_pas_AshWithSaw_AttackRight1_Shotgun[6]; // 0x00447580
static class tagxStateLink link_pas_AshWithSaw_Armed_HitBalls_Shotgun[1]; // 0x004A3F04
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBalls_Shotgun[1]; // 0x004A3F00
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitStrong_Shotgun[1]; // 0x004A3EFC
static class tagxStateLink link_pas_AshWithSaw_Armed_HitStrong_Shotgun[1]; // 0x004A3EF8
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBack_Shotgun[7]; // 0x00447560
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitUp_Shotgun[7]; // 0x00447540
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitDown_Shotgun[7]; // 0x00447520
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitRight_Shotgun[7]; // 0x00447500
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitLeft_Shotgun[7]; // 0x004474E0
static class tagxStateLink link_pas_AshWithSaw_AttackComboLeft_Shotgun[1]; // 0x004A3EF4
static class tagxStateLink link_pas_AshWithSaw_AttackComboRight_Shotgun[3]; // 0x004474D0
static class tagxStateLink link_pas_AshWithSaw_WalkingBack_Relaxed_Shotgun[24]; // 0x00447470
static class tagxStateLink link_pas_AshWithSaw_Running_Relaxed_Shotgun[25]; // 0x00447400
static class tagxStateLink link_pas_AshWithSaw_Walking_Relaxed_Shotgun[25]; // 0x00447390
static class tagxStateLink link_pas_AshWithSaw_Reload_Shotgun_Upper[1]; // 0x004A3EF0
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Shotgun_Upper[1]; // 0x004A3EEC
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Shotgun_Upper[1]; // 0x004A3EE8
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Shotgun_Upper[1]; // 0x004A3EE4
static class tagxStateLink link_pas_AshWithSaw_Reload_Shotgun[4]; // 0x00447380
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Shotgun[6]; // 0x00447360
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Shotgun[6]; // 0x00447340
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Shotgun[6]; // 0x00447320
static class tagxStateLink link_pas_AshWithSaw_Armed_Shotgun_Upper[10]; // 0x004472F0
static class tagxStateLink link_pas_AshWithSaw_Idle_Armed_Shotgun[36]; // 0x00447260
static class tagxStateLink link_pas_AshWithSaw_Idle_Relaxed_Shotgun[41]; // 0x004471B0
static class tagxStateLink link_pas_AshWithSaw_AttackRight3_Ax[6]; // 0x00447190
static class tagxStateLink link_pas_AshWithSaw_AttackRight2_Ax[6]; // 0x00447170
static class tagxStateLink link_pas_AshWithSaw_AttackRight1_Ax[6]; // 0x00447150
static class tagxStateLink link_pas_AshWithSaw_Armed_HitBalls_Ax[1]; // 0x004A3EE0
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBalls_Ax[1]; // 0x004A3EDC
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitStrong_Ax[1]; // 0x004A3ED8
static class tagxStateLink link_pas_AshWithSaw_Armed_HitStrong_Ax[1]; // 0x004A3ED4
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitBack_Ax[7]; // 0x00447130
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitUp_Ax[7]; // 0x00447110
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitDown_Ax[7]; // 0x004470F0
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitRight_Ax[7]; // 0x004470D0
static class tagxStateLink link_pas_AshWithSaw_Relaxed_HitLeft_Ax[7]; // 0x004470B0
static class tagxStateLink link_pas_AshWithSaw_AttackComboLeft_Ax[1]; // 0x004A3ED0
static class tagxStateLink link_pas_AshWithSaw_AttackComboRight_Ax[3]; // 0x004470A0
static class tagxStateLink link_pas_AshWithSaw_WalkingBack_Relaxed_Ax[24]; // 0x00447040
static class tagxStateLink link_pas_AshWithSaw_Running_Relaxed_Ax[25]; // 0x00446FD0
static class tagxStateLink link_pas_AshWithSaw_Walking_Relaxed_Ax[25]; // 0x00446F60
static class tagxStateLink link_pas_AshWithSaw_Reload_Ax_Upper[1]; // 0x004A3ECC
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Ax_Upper[1]; // 0x004A3EC8
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Ax_Upper[1]; // 0x004A3EC4
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Ax_Upper[1]; // 0x004A3EC0
static class tagxStateLink link_pas_AshWithSaw_Reload_Ax[4]; // 0x00446F50
static class tagxStateLink link_pas_AshWithSaw_AttackLeft3_Ax[6]; // 0x00446F30
static class tagxStateLink link_pas_AshWithSaw_AttackLeft2_Ax[6]; // 0x00446F10
static class tagxStateLink link_pas_AshWithSaw_AttackLeft1_Ax[6]; // 0x00446EF0
static class tagxStateLink link_pas_AshWithSaw_Armed_Ax_Upper[10]; // 0x00446EC0
static class tagxStateLink link_pas_AshWithSaw_Idle_Armed_Ax[36]; // 0x00446E30
static class tagxStateLink link_pas_AshWithSaw_Idle_Relaxed_Ax[41]; // 0x00446D80
unsigned char g_pasJointsAll[17]; // 0x00446D60
unsigned char g_pasJointsUpperOnly[17]; // 0x00446D40
unsigned char g_pasJointsLowerOnly[17]; // 0x00446D20
// 0x003437A0 - 0x00343AF8 (0x358) xPASAnimTrigger__FP7_xActoriP8tagXAnimP15tagXAnimTrigger
void xPASAnimTrigger(class _xActor *actor, class tagXAnimTrigger *trigger) {
    { // 0x003437A0 - 0x00343AF8
        class xMessage theMessage; // r29+0xa0
        class _xActor *npc; // r17
        signed int i; // r16
        class xMessage theMessage; // r29+0x70
        signed int distance; // r2
        class _xObj *obj; // r4
        signed int i; // r16
        class tagxPlayer *player; // r2
    }
}

// 0x00343B00 - 0x00343B08 (0x8) xPASSetWeapon__FP7_xActoriP9tagxState
void xPASSetWeapon() {
    { // 0x00343B00 - 0x00343B08
    }
}

// 0x00343B10 - 0x00343B3C (0x2C) xPASStateReset__Fv
void xPASStateReset() {
    { // 0x00343B10 - 0x00343B3C
    }
}

// 0x00343B40 - 0x00343B6C (0x2C) xPASStateInit__Fv
void xPASStateInit() {
    { // 0x00343B40 - 0x00343B6C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xParMgr.cpp
// ==========================================================================
class xPar *gParDead; // 0x004A4D2C
unsigned int gTimeDelta; // 0x004A4E18
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagxActorDBData *gadb; // 0x004A4658
static signed int sParMgrInit; // 0x004A4D14
static unsigned int sParMgrIgnoreFXCounter; // 0x004A4D10
static signed int sCounter; // 0x004A4D0C
static signed int sFrameCount; // 0x004A4D08
class tagParCmdList *gParCmdList[37]; // 0x004B9960
class xParCmd *gParEmitter[37]; // 0x004B98C0
class tagxParSys gParSys[37]; // 0x004B94B0
signed int gParSysLimit[37]; // 0x004496C0
signed int gParSysPriority[37]; // 0x00449620
static char sParSysName[37][32]; // 0x00449180
static signed int gParSysNumParticles[37]; // 0x004490E0
// 0x00343B70 - 0x003451E0 (0x1670) xParMgrFXEvent__FPviP8Vector3DP8Vector3D
void xParMgrFXEvent(signed int vfxtype, class Vector3D *v1, class Vector3D *v2) {
    { // 0x00343B70 - 0x003451E0
        signed int amount; // r2
        unsigned int dt; // r18
        class tagxParSys *ps; // r17
        class xParCmd *cmd; // r16
    }
}

// 0x003451E0 - 0x003454F8 (0x318) xParMgrFXEventMode__FPviP8Vector3DP8Vector3Di
void xParMgrFXEventMode(void *a, signed int vfxType, class Vector3D *v1, class Vector3D *v2, signed int emitMode) {
    { // 0x003451E0 - 0x003454F8
        class xParCmd *cmd; // r16
    }
}

// 0x00345500 - 0x00345520 (0x20) xParMgrRender__Fv
void xParMgrRender() {
    { // 0x00345500 - 0x00345520
    }
}

// 0x00345520 - 0x00345528 (0x8) xParMgrAnimate__Fv
void xParMgrAnimate() {
    { // 0x00345520 - 0x00345528
    }
}

// 0x00345530 - 0x003457D0 (0x2A0) xParMgrExecute__Fv
void xParMgrExecute() {
    { // 0x00345530 - 0x003457D0
        unsigned int numFrames; // r16
        signed int i; // r16
    }
}

// 0x003457D0 - 0x00345984 (0x1B4) xParMgrDebug__Fi
void xParMgrDebug() {
    { // 0x003457D0 - 0x00345984
        class tagxParSys *sys; // r2
        signed int sysAliveCount; // r19
        unsigned int i; // r18
        unsigned int totalAlive; // r17
        unsigned int sysDeadCount; // r2
        unsigned int deadCount; // r16
        class xPar *p; // r2
    }
}

// 0x00345990 - 0x00345A08 (0x78) xParMgrLevelReset__Fv
void xParMgrLevelReset() {
    { // 0x00345990 - 0x00345A08
    }
}

// 0x00345A10 - 0x00345A80 (0x70) xParMgrKillAllParticles__Fv
void xParMgrKillAllParticles() {
    { // 0x00345A10 - 0x00345A80
        signed int i; // r16
    }
}

// 0x00345A80 - 0x00345B48 (0xC8) xParMgrIgnoreFXEvents__Fi
void xParMgrIgnoreFXEvents(signed int numFrames) {
    { // 0x00345A80 - 0x00345B48
    }
}

// 0x00345B50 - 0x00346098 (0x548) xParMgrInit__Fv
void xParMgrInit() {
    { // 0x00345B50 - 0x00346098
        signed int i; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xParExe.cpp
// ==========================================================================
float g_2_PI; // 0x00485A48
class tagxActorDBData *gadb; // 0x004A4658
float gFloorQuickHeight[32][32]; // 0x00AF9BA0
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
void (*gParCmdExeTable[38])(class xParCmd *, class tagxParSys *, unsigned int); // 0x00449760
// 0x003460A0 - 0x00346464 (0x3C4) ParCmdExec_SpawnVFX__FP7xParCmdP10tagxParSysUi
void ParCmdExec_SpawnVFX(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x003460A0 - 0x00346464
        class Vector3D a; // r29+0x60
        class xPar *p; // r17
        class tagParCmd_SpawnVFX *d; // r16
    }
}

// 0x00346470 - 0x00346B8C (0x71C) ParCmdExec_Emitter__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Emitter(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x00346470 - 0x00346B8C
        class xPar *tmp; // r19
        float rnd; // 
        signed int emitCount; // r18
        signed int j; // r17
        class tagParCmd_Emitter *d; // r16
    }
}

// 0x00346B90 - 0x00346D50 (0x1C0) ParCmdExec_KillInvis__FP7xParCmdP10tagxParSysUi
void ParCmdExec_KillInvis(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x00346B90 - 0x00346D50
        class xPar *p; // r6
        class tagParCmd_KillInvis *d; // r5
    }
}

// 0x00346D50 - 0x00346F00 (0x1B0) ParCmdExec_KillOld__FP7xParCmdP10tagxParSysUi
void ParCmdExec_KillOld(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x00346D50 - 0x00346F00
        class xPar *p; // r6
        class tagParCmd_KillOld *d; // r5
    }
}

// 0x00346F00 - 0x00347198 (0x298) ParCmdExec_KillSlow__FP7xParCmdP10tagxParSysUi
void ParCmdExec_KillSlow(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x00346F00 - 0x00347198
        class xPar *p; // r6
        class tagParCmd_KillSlow *d; // r5
    }
}

// 0x003471A0 - 0x003475AC (0x40C) ParCmdExec_KillPlane__FP7xParCmdP10tagxParSysUi
void ParCmdExec_KillPlane(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x003471A0 - 0x003475AC
        float zow; // 
        float xow; // 
        unsigned int zbit; // r2
        unsigned int xbit; // r18
        signed short plane; // r7
        class tagiFloor *f; // r17
        class xPar *p; // r16
        class tagParCmd_KillPlane *d; // r6
    }
}

// 0x003475B0 - 0x00347898 (0x2E8) ParCmdExec_KillBox__FP7xParCmdP10tagxParSysUi
void ParCmdExec_KillBox(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x003475B0 - 0x00347898
        class Vector3D *v; // r5
        class xPar *p; // r4
        class tagParCmd_KillBox *d; // r3
    }
}

// 0x003478A0 - 0x0034800C (0x76C) ParCmdExec_Texture__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Texture(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x003478A0 - 0x0034800C
        float col; // 
        float row; // 
        float col; // 
        float row; // 
        float col; // 
        float row; // 
        float col; // 
        float row; // 
        class xPar *p; // r17
        class tagParCmd_Texture *d; // r16
    }
}

// 0x00348010 - 0x003483C8 (0x3B8) ParCmdExecHelper_TextureBirth__FP7xParCmdP4xPar
static void ParCmdExecHelper_TextureBirth(class xParCmd *cmd, class xPar *p) {
    { // 0x00348010 - 0x003483C8
        float col; // 
        float row; // 
        class tagParCmd_Texture *d; // r16
    }
}

// 0x003483D0 - 0x00348590 (0x1C0) ParCmdExec_RotMove__FP7xParCmdP10tagxParSysUi
void ParCmdExec_RotMove(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x003483D0 - 0x00348590
        class xPar *p; // r6
        class tagParCmd_RotMove *d; // r5
    }
}

// 0x00348590 - 0x0034886C (0x2DC) ParCmdExec_Accelerate__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Accelerate(class xParCmd *cmd, class tagxParSys *ps, unsigned int dt) {
    { // 0x00348590 - 0x0034886C
        float daz; // 
        float day; // 
        float dax; // 
        class xPar *p; // r3
        class tagParCmd_Accelerate *d; // r5
    }
}

// 0x00348870 - 0x00348AC4 (0x254) ParCmdExec_Move__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Move(class xParCmd *cmd, class tagxParSys *ps, unsigned int dt) {
    { // 0x00348870 - 0x00348AC4
        float t; // 
        class xPar *p; // r4
        class tagParCmd_Move *d; // r4
    }
}

// 0x00348AD0 - 0x00348DE8 (0x318) ParCmdExec_RandomDisplace__FP7xParCmdP10tagxParSysUi
void ParCmdExec_RandomDisplace(class xParCmd *cmd, class tagxParSys *ps, unsigned int dt) {
    { // 0x00348AD0 - 0x00348DE8
        float t; // r29+0x70
        class xPar *p; // r17
        class tagParCmd_RandomDisplace *d; // r16
    }
}

// 0x00348DF0 - 0x00349140 (0x350) ParCmdExec_OrbitPoint__FP7xParCmdP10tagxParSysUi
void ParCmdExec_OrbitPoint(class xParCmd *cmd, class tagxParSys *ps, unsigned int dt) {
    { // 0x00348DF0 - 0x00349140
        float oorSqr; // 
        float rSqr; // 
        class iActorRuntime *r; // r3
        class _xActor *a; // r4
        signed int i; // r17
        float mdt; // 
        float t; // 
        class xPar *p; // r19
        class tagParCmd_OrbitPoint *d; // r16
    }
}

// 0x00349140 - 0x00349504 (0x3C4) ParCmdExec_Turbulence__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Turbulence(class xParCmd *cmd, class tagxParSys *ps, unsigned int dt) {
    { // 0x00349140 - 0x00349504
        float zcos; // 
        float xsin; // 
        float ysin; // r29+0x70
        float t; // r29+0x70
        class xPar *p; // r4
        class tagParCmd_Turbulence *d; // r16
    }
}

// 0x00349510 - 0x003498D4 (0x3C4) ParCmdExec_Jet__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Jet(class xParCmd *cmd, class tagxParSys *ps, unsigned int dt) {
    { // 0x00349510 - 0x003498D4
        float oorSqr; // 
        float rSqr; // 
        class iActorRuntime *r; // r3
        class _xActor *a; // r4
        signed int i; // r17
        float mdt; // 
        float t; // 
        class xPar *p; // r19
        class tagParCmd_Jet *d; // r16
    }
}

// 0x003498E0 - 0x00349CA4 (0x3C4) ParCmdExec_Friction__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Friction(class xParCmd *cmd, class tagxParSys *ps, unsigned int dt) {
    { // 0x003498E0 - 0x00349CA4
        float t; // r29+0x60
        class xPar *p; // r3
        class tagParCmd_Friction *d; // r5
    }
}

// 0x00349CB0 - 0x0034A190 (0x4E0) ParCmdExec_Bounce__FP7xParCmdP10tagxParSysUi
void ParCmdExec_Bounce(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x00349CB0 - 0x0034A190
        float zow; // 
        float xow; // 
        unsigned int zbit; // r2
        unsigned int xbit; // r19
        signed short plane; // r7
        class tagiFloor *f; // r18
        class xPar *p; // r17
        class tagParCmd_Bounce *d; // r16
    }
}

// 0x0034A190 - 0x0034A378 (0x1E8) ParCmdExec_LimitVelocity__FP7xParCmdP10tagxParSysUi
void ParCmdExec_LimitVelocity(class xParCmd *cmd, class tagxParSys *ps) {
    { // 0x0034A190 - 0x0034A378
        float mz; // 
        float my; // 
        float mx; // 
        class xPar *p; // r4
        class tagParCmd_LimitVelocity *d; // r3
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xParCmd.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
void (*gParCmdExeTable[38])(class xParCmd *, class tagxParSys *, unsigned int); // 0x00449760
static void *sParCmdDataPool[38]; // 0x00449930
static signed int sParCmdDataInfo[38][2]; // 0x00449800
class tagParCmd_ModulateColor sParCmdModulateColor[2]; // 0x004BDA20
class tagParCmd_Turbulence sParCmdTurbulence[2]; // 0x004BD9E0
class tagParCmd_KillInvis sParCmdKillInvis[1]; // 0x004A4D28
class tagParCmd_Texture sParCmdTexture[37]; // 0x004BD250
class tagParCmd_RotMove sParCmdRotMove[2]; // 0x004BD240
class tagParCmd_KillPlane sParCmdKillPlane[5]; // 0x004BD1F0
class tagParCmd_SpawnVFX sParCmdSpawnVFX[37]; // 0x004BD030
class tagParCmd_KillBox sParCmdKillBox[2]; // 0x004BCFF0
class tagParCmd_LimitVelocity sParCmdLimitVelocity[2]; // 0x004BCFD0
class tagParCmd_EnableCmd sParCmdEnableCmd[2]; // 0x004BCFC8
class tagParCmd_DisableCmd sParCmdDisableCmd[2]; // 0x004BCFB0
class tagParCmd_KillCmd sParCmdKillCmd[2]; // 0x004BCF90
class tagParCmd_Emitter sParCmdEmitter[47]; // 0x004BBA40
class tagParCmd_Accelerate sParCmdAccelerate[37]; // 0x004BB6C0
class tagParCmd_KillOld sParCmdKillOld[37]; // 0x004BB590
class tagParCmd_KillSlow sParCmdKillSlow[2]; // 0x004BB570
class tagParCmd_Friction sParCmdFriction[2]; // 0x004BB550
class tagParCmd_Vortex sParCmdVortex[2]; // 0x004BB540
class tagParCmd_TargetSize sParCmdTargetSize[2]; // 0x004BB538
class tagParCmd_TargetColor sParCmdTargetColor[2]; // 0x004BB520
class tagParCmd_Source sParCmdSource[2]; // 0x004BB510
class tagParCmd_SinkVelocity sParCmdSinkVelocity[2]; // 0x004BB508
class tagParCmd_Sink sParCmdSink[2]; // 0x004BB500
class tagParCmd_Restore sParCmdRestore[2]; // 0x004BB4F8
class tagParCmd_RandomVelocity sParCmdRandomVelocity[2]; // 0x004BB4F0
class tagParCmd_RandomDisplace sParCmdRandomDisplace[2]; // 0x004BB4D0
class tagParCmd_RandomAccel sParCmdRandomAccel[2]; // 0x004BB4C0
class tagParCmd_OrbitPoint sParCmdOrbitPoint[1]; // 0x004BB4A0
class tagParCmd_OrbitLine sParCmdOrbitLine[2]; // 0x004BB498
class tagParCmd_Move sParCmdMove[37]; // 0x004BB370
class tagParCmd_Jet sParCmdJet[2]; // 0x004BB320
class tagParCmd_GravitateCmd sParCmdGravitate[2]; // 0x004BB318
class tagParCmd_Follow sParCmdFollow[2]; // 0x004BB310
class tagParCmd_Explosion sParCmdExplosion[2]; // 0x004BB2D0
class tagParCmd_Damping sParCmdDamping[2]; // 0x004BB2C8
class tagParCmd_CopyVertexB sParCmdCopyVertexB[2]; // 0x004BB2C0
class tagParCmd_Bounce sParCmdBounce[10]; // 0x004BB180
class tagParCmd_Header sParCmdHeader[2]; // 0x004BB178
static class xParCmd *sParCmdDead; // 0x004A4D24
static class xParCmd sParCmdPool[250]; // 0x004B9DF0
static unsigned int sParCmdPoolCount; // 0x004A4D20
static class tagParCmdList *sParCmdListDead; // 0x004A4D1C
static class tagParCmdList sParCmdListPool[50]; // 0x004B9A00
static unsigned int sParCmdListPoolCount; // 0x004A4D18
// 0x0034A380 - 0x0034A524 (0x1A4) xParCmdListAddCmd__FP13tagParCmdList10eParCmdTag
class xParCmd *xParCmdListAddCmd(class tagParCmdList *list, enum eParCmdTag cmdtype) {
    { // 0x0034A380 - 0x0034A524
        class xParCmd *cmd; // r2
    }
}

// 0x0034A530 - 0x0034A75C (0x22C) xParCmdListExecute__FP13tagParCmdListP10tagxParSysUi
void xParCmdListExecute(class tagParCmdList *list, class tagxParSys *ps, unsigned int dt) {
    { // 0x0034A530 - 0x0034A75C
        class xParCmd *idx; // r16
    }
}

// 0x0034A760 - 0x0034A774 (0x14) xParCmdListInit__FP13tagParCmdList
void xParCmdListInit(class tagParCmdList *list) {
    { // 0x0034A760 - 0x0034A774
    }
}

// 0x0034A780 - 0x0034A898 (0x118) xParCmdAlloc__Fv
class xParCmd *xParCmdAlloc() {
    { // 0x0034A780 - 0x0034A898
        class xParCmd *l; // r2
    }
}

// 0x0034A8A0 - 0x0034A908 (0x68) xParCmdMemInit__Fv
void xParCmdMemInit() {
    { // 0x0034A8A0 - 0x0034A908
        class xParCmd *cmd; // r4
        signed int i; // r5
    }
}

// 0x0034A910 - 0x0034AAD8 (0x1C8) xParCmdListFree__FP13tagParCmdList
void xParCmdListFree(class tagParCmdList *cmdlist) {
    { // 0x0034A910 - 0x0034AAD8
    }
}

// 0x0034AAE0 - 0x0034ABF8 (0x118) xParCmdListAlloc__Fv
class tagParCmdList *xParCmdListAlloc() {
    { // 0x0034AAE0 - 0x0034ABF8
        class tagParCmdList *l; // r2
    }
}

// 0x0034AC00 - 0x0034AC68 (0x68) xParCmdListMemInit__Fv
void xParCmdListMemInit() {
    { // 0x0034AC00 - 0x0034AC68
        class tagParCmdList *cmd; // r4
        signed int i; // r5
    }
}

// 0x0034AC70 - 0x0034AF24 (0x2B4) xParCmdDataAlloc__Fi
void *xParCmdDataAlloc(signed int cmdDataType) {
    { // 0x0034AC70 - 0x0034AF24
        unsigned char *data; // r18
        signed int size; // r17
        signed int count; // r16
        signed int i; // r4
    }
}

// 0x0034AF30 - 0x0034AFC4 (0x94) xParCmdDataMemInit__Fv
void xParCmdDataMemInit() {
    { // 0x0034AF30 - 0x0034AFC4
        unsigned char *data; // r8
        signed int size; // r7
        signed int count; // r6
        signed int j; // r4
        signed int i; // r5
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPar.cpp
// ==========================================================================
signed int gParSysPriority[37]; // 0x00449620
class tagxParSys gParSys[37]; // 0x004B94B0
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class xPar *gParDead; // 0x004A4D2C
class xPar gParPool[1500]; // 0x004BDA30
// 0x0034AFD0 - 0x0034B02C (0x5C) xParSysInitParCmdList__FP10tagxParSys
class tagParCmdList *xParSysInitParCmdList(class tagxParSys *ps) {
    { // 0x0034AFD0 - 0x0034B02C
    }
}

// 0x0034B030 - 0x0034B458 (0x428) xParSysAllocPars__FP10tagxParSysi
void xParSysAllocPars(class tagxParSys *ps, signed int numPars) {
    { // 0x0034B030 - 0x0034B458
        class xPar *p; // r2
        signed int i; // r16
    }
}

// 0x0034B460 - 0x0034B77C (0x31C) xParSysKillPar__FP10tagxParSysP4xPar
void xParSysKillPar(class tagxParSys *ps, class xPar *p) {
    { // 0x0034B460 - 0x0034B77C
    }
}

// 0x0034B780 - 0x0034BA44 (0x2C4) xParSysAddPar__FP10tagxParSys
class xPar *xParSysAddPar(class tagxParSys *ps) {
    { // 0x0034B780 - 0x0034BA44
        signed int myPriority; // r3
        unsigned int i; // r2
        class xPar *p; // r2
    }
}

// 0x0034BA50 - 0x0034BC08 (0x1B8) xParSysAddParP__FP10tagxParSysP4xPar
void xParSysAddParP(class tagxParSys *ps, class xPar *p) {
    { // 0x0034BA50 - 0x0034BC08
    }
}

// 0x0034BC10 - 0x0034BEE8 (0x2D8) xParSysAnimate__FP10tagxParSysUi
void xParSysAnimate(class tagxParSys *ps, unsigned int dt) {
    { // 0x0034BC10 - 0x0034BEE8
        signed short col; // r4
        unsigned char t; // r17
        class xPar *tmp; // r2
        class xPar *i; // r16
    }
}

// 0x0034BEF0 - 0x0034BFEC (0xFC) xParSysKillAllParticles__FP10tagxParSys
void xParSysKillAllParticles(class tagxParSys *ps) {
    { // 0x0034BEF0 - 0x0034BFEC
        class xPar *tmp; // r2
        class xPar *i; // r17
    }
}

// 0x0034BFF0 - 0x0034C0DC (0xEC) xParSysExecute__FP10tagxParSysUi
void xParSysExecute(class tagxParSys *ps, unsigned int dt) {
    { // 0x0034BFF0 - 0x0034C0DC
        class tagParCmdList *list; // r2
    }
}

// 0x0034C0E0 - 0x0034C0EC (0xC) xParSysSetCulled__FP10tagxParSysi
void xParSysSetCulled(class tagxParSys *ps, signed int isCulled) {
    { // 0x0034C0E0 - 0x0034C0EC
    }
}

// 0x0034C0F0 - 0x0034C0FC (0xC) xParSysSetVisible__FP10tagxParSysi
void xParSysSetVisible(class tagxParSys *ps, signed int isVisible) {
    { // 0x0034C0F0 - 0x0034C0FC
    }
}

// 0x0034C100 - 0x0034C13C (0x3C) xParSysInit__FP10tagxParSys
void xParSysInit(class tagxParSys *ps) {
    { // 0x0034C100 - 0x0034C13C
    }
}

// 0x0034C140 - 0x0034C220 (0xE0) xParInit__FP4xPar
void xParInit(class xPar *p) {
    { // 0x0034C140 - 0x0034C220
        signed int i; // r6
    }
}

// 0x0034C220 - 0x0034C28C (0x6C) xParMemInit__Fv
void xParMemInit() {
    { // 0x0034C220 - 0x0034C28C
        class xPar *p; // r4
        signed int i; // r5
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xPad.cpp
// ==========================================================================
void (iPadInitFunc_Game)(class tagxPad *); // 0x0037FB60
class tagxPlayer g_Players[0]; // 0x004B5A20
class xScene *xScene_current; // 0x004A4E08
class tag_xCameraControl gCamera; // 0x00AA67C0
signed int gDebugDrawDebugInfo; // 0x004A4E1C
signed int gDebugShadowMode; // 0x00000000
signed int gDebugMaskMode; // 0x00000000
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int debugCallFlag; // 0x004A4684
signed int debugOpFlag; // 0x004A4FAC
class tagPadMgrData console; // 0x004DF540
// 0x0034C290 - 0x0034C440 (0x1B0) xPadButtonPressed__Fii
signed int xPadButtonPressed(signed int pad, signed int whichXPAD_BUTTON) {
    { // 0x0034C290 - 0x0034C440
        class _XBUTTON *b; // r2
    }
}

// 0x0034C440 - 0x0034CE8C (0xA4C) xConsoleMgrIdle__FUl
void xConsoleMgrIdle(unsigned long theTime) {
    { // 0x0034C440 - 0x0034CE8C
        signed int canGoOn; // r17
        class tagxPadAction *ca; // r8
        class tagxPadAction *ta; // r5
        signed int k; // r4
        signed int j; // r3
        class _xActor *a; // r3
        class tagxPlayer *p; // r3
        signed int newCount; // r2
        signed int i; // r16
        static signed int actionCount[4]; // 0x004499D0
        static class tagxPadAction actionQueue[4][33]; // 0x004DF7C0
    }
}

// 0x0034CE90 - 0x0034CF78 (0xE8) xPadMgrGetFuncPtrAction__Fi
void (*xPadMgrGetFuncPtrAction(signed int controller))(class tagxPad *, signed int, class tagxPadAction *) {
    { // 0x0034CE90 - 0x0034CF78
    }
}

// 0x0034CF80 - 0x0034D074 (0xF4) xPadMgrAssignFuncPtrAction__FiPFP7tagxPadiP13tagxPadAction_v
void xPadMgrAssignFuncPtrAction(signed int controller, void (*action)(class tagxPad *, signed int, class tagxPadAction *)) {
    { // 0x0034CF80 - 0x0034D074
    }
}

// 0x0034D080 - 0x0034D168 (0xE8) xPadMgrGetFuncPtrMapper__Fi
signed int (*xPadMgrGetFuncPtrMapper(signed int controller))(class tagxPad *, class tagxPadAction *, signed int) {
    { // 0x0034D080 - 0x0034D168
    }
}

// 0x0034D170 - 0x0034D264 (0xF4) xPadMgrAssignFuncPtrMapper__FiPFP7tagxPadP13tagxPadActioni_i
void xPadMgrAssignFuncPtrMapper(signed int controller, signed int (*mapper)(class tagxPad *, class tagxPadAction *, signed int)) {
    { // 0x0034D170 - 0x0034D264
    }
}

// 0x0034D270 - 0x0034DA78 (0x808) xConsoleMgrInit__Fi
signed short xConsoleMgrInit(signed int maxControllers) {
    { // 0x0034D270 - 0x0034DA78
        signed int i; // r16
    }
}

// 0x0034DA80 - 0x0034DAA0 (0x20) xConsoleMgrRegisterScriptCalls__Fv
void xConsoleMgrRegisterScriptCalls() {
    { // 0x0034DA80 - 0x0034DAA0
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xObjDB.cpp
// ==========================================================================
signed int (objFlagsAdd)(signed int, signed int); // 0x00354CD0
signed int (objFlagsSub)(signed int, signed int); // 0x003549E0
signed int (objFlagsSet)(signed int, signed int); // 0x00354700
void (objSetVisible)(signed int, signed int); // 0x003532E0
void (objSetOrientation)(signed int, signed int, signed int, signed int); // 0x00353030
void (objSetPositionActor)(signed int, signed int, signed int, signed int); // 0x00353010
void (objSetPositionMarker)(signed int, signed int, signed int); // 0x00352D40
void (objSetPositionNavigationPoint)(signed int, signed int); // 0x00352B50
void (objSetPosition)(signed int, signed int, signed int, signed int); // 0x00352890
void (objTranslatePosition)(signed int, signed int, signed int, signed int); // 0x00351A50
void (objSetBRadius)(signed int, signed int); // 0x003516A0
signed int (objCreate)(signed int, signed int, signed int, signed int); // 0x003515D0
void (objSetup)(signed int, signed int, signed int, signed int, signed int); // 0x003515A0
void (objSetTexture)(signed int, signed int); // 0x00350E70
void (objSetType)(signed int, signed int); // 0x00351190
void (objDestroy)(signed int); // 0x003513E0
signed int (objDistanceToActor)(signed int, signed int, signed int); // 0x003508D0
void (objFaceActor)(signed int, signed int, signed int); // 0x00350340
signed int (objMoveStrafe)(signed int, signed int); // 0x0034FF70
signed int (objMoveForward)(signed int, signed int); // 0x0034FC30
signed int (objGetAngle)(signed int); // 0x0034F950
signed int (objGetPositionX)(signed int); // 0x00352640
signed int (objGetPositionY)(signed int); // 0x003523F0
signed int (objGetPositionZ)(signed int); // 0x003521A0
signed int (objGetRotY)(signed int); // 0x00351F50
signed int (objGetFlags)(signed int); // 0x00351D10
signed int (objIsFacingActor)(signed int, signed int, signed int, signed int); // 0x0034F290
signed int (objInTrigger)(signed int, signed int); // 0x0034ED10
signed int (objInFOV)(signed int, signed int, signed int); // 0x0034E8D0
signed int (objIsVisible)(signed int, signed int, signed int); // 0x0034E350
void (objTurn)(signed int, signed int); // 0x0034E060
void (objFace)(signed int, signed int); // 0x0034DD70
void (objSetCollisionFlags)(signed int, signed int); // 0x0034DAA0
void (xObjDBEventSendAll)(signed int, signed int, signed int, signed int, signed int); // 0x00353B20
void (objEffectFadeInfo)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x00354330
void (objEffectFadeStart)(signed int, signed int, signed int); // 0x003543B0
signed int (objEffectFadeDone)(signed int); // 0x00354400
void (objEffectReflect)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x003542C0
void (objEffectSetFlag)(signed int, signed int, signed int, signed int); // 0x00354270
signed int g_RTObjModelCount; // 0x004A4D34
float g_PI_DIV_2; // 0x00485A50
float g_RADTODEG; // 0x00485A60
class xScene *xScene_current; // 0x004A4E08
class tagxActorDBData *gadb; // 0x004A4658
float g_DEGTORAD; // 0x00485A58
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static unsigned int interpreterMethod; // 0x004A4D30
class tagxObjDBData *godb; // 0x004A3FAC
class tagxObjDBData gObjDBData; // 0x004DFDF0
// 0x0034DAA0 - 0x0034DD70 (0x2D0) objSetCollisionFlags__Fii
void objSetCollisionFlags(signed int objID, signed int flags) {
    { // 0x0034DAA0 - 0x0034DD70
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x0034DD70 - 0x0034E058 (0x2E8) objFace__Fii
void objFace(signed int objID, signed int degrees) {
    { // 0x0034DD70 - 0x0034E058
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x0034E060 - 0x0034E350 (0x2F0) objTurn__Fii
void objTurn(signed int objID, signed int degDelta) {
    { // 0x0034E060 - 0x0034E350
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x0034E350 - 0x0034E8C8 (0x578) objIsVisible__Fiii
signed int objIsVisible(signed int objID, signed int targetActorID) {
    { // 0x0034E350 - 0x0034E8C8
        class iObjRuntime *br; // r17
        class _xObj *b; // r17
        class iObjRuntime *ar; // r16
        class _xObj *a; // r16
    }
}

// 0x0034E8D0 - 0x0034ED0C (0x43C) objInFOV__Fiii
signed int objInFOV(signed int objID, signed int targetObjectType, signed int targetActorID) {
    { // 0x0034E8D0 - 0x0034ED0C
        class _xObj *b; // r2
        class _xActor *a; // r2
    }
}

// 0x0034ED10 - 0x0034F288 (0x578) objInTrigger__Fii
signed int objInTrigger(signed int objID, signed int triggerID) {
    { // 0x0034ED10 - 0x0034F288
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x0034F290 - 0x0034F944 (0x6B4) objIsFacingActor__Fiiii
signed int objIsFacingActor(signed int objID, signed int targetActorID, signed int delta) {
    { // 0x0034F290 - 0x0034F944
        float ang; // r29+0x80
        signed int anglem; // r16
        signed int angle2; // r3
        signed int angle1; // r2
        class Vector3D v; // r29+0x70
        class iObjRuntime *ar; // r17
        class _xObj *b; // r16
        class _xObj *a; // r16
    }
}

// 0x0034F950 - 0x0034FC28 (0x2D8) objGetAngle__Fi
signed int objGetAngle(signed int objID) {
    { // 0x0034F950 - 0x0034FC28
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x0034FC30 - 0x0034FF6C (0x33C) objMoveForward__Fii
signed int objMoveForward(signed int objID, signed int amount) {
    { // 0x0034FC30 - 0x0034FF6C
        class Vector3D d; // r29+0x50
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x0034FF70 - 0x0035033C (0x3CC) objMoveStrafe__Fii
signed int objMoveStrafe(signed int objID, signed int amount) {
    { // 0x0034FF70 - 0x0035033C
        class Vector3D rot; // r29+0x60
        class Vector3D d; // r29+0x50
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x00350340 - 0x003508CC (0x58C) objFaceActor__Fiii
void objFaceActor(signed int objID, signed int targetActorID) {
    { // 0x00350340 - 0x003508CC
        class Vector3D d; // r29+0x50
        class iObjRuntime *br; // r17
        class _xObj *b; // r17
        class iObjRuntime *ar; // r16
        class _xObj *a; // r16
    }
}

// 0x003508D0 - 0x00350E70 (0x5A0) objDistanceToActor__Fiii
signed int objDistanceToActor(signed int objID, signed int targetActorID) {
    { // 0x003508D0 - 0x00350E70
        signed int mag; // r2
        class Vector3D d; // r29+0x50
        class iObjRuntime *br; // r17
        class _xObj *b; // r17
        class iObjRuntime *ar; // r16
        class _xObj *a; // r16
    }
}

// 0x00350E70 - 0x00351188 (0x318) objSetTexture__Fii
void objSetTexture(signed int objID, signed int textureID) {
    { // 0x00350E70 - 0x00351188
        class _xTex *t; // r2
        class iObjRuntime *r; // r2
        class _xObj *a; // r16
    }
}

// 0x00351190 - 0x003513E0 (0x250) objSetType__Fii
void objSetType(signed int objID, signed int objType) {
    { // 0x00351190 - 0x003513E0
        class _xObj *a; // r16
    }
}

// 0x003513E0 - 0x0035159C (0x1BC) objDestroy__Fi
void objDestroy(signed int objID) {
    { // 0x003513E0 - 0x0035159C
    }
}

// 0x003515A0 - 0x003515CC (0x2C) objSetup__Fiiiii
void objSetup(signed int objID, signed int objType, signed int modelID, signed int skeletonID, signed int textureID) {
    { // 0x003515A0 - 0x003515CC
    }
}

// 0x003515D0 - 0x00351698 (0xC8) objCreate__Fiiii
signed int objCreate(signed int objType, signed int modelID, signed int skeletonID, signed int textureID) {
    { // 0x003515D0 - 0x00351698
    }
}

// 0x003516A0 - 0x00351A44 (0x3A4) objSetBRadius__Fii
void objSetBRadius(signed int objID, signed int bradius) {
    { // 0x003516A0 - 0x00351A44
        class iObjRuntime *r; // r16
        class _xObj *a; // r16
    }
}

// 0x00351A50 - 0x00351D0C (0x2BC) objTranslatePosition__Fiiii
void objTranslatePosition(signed int objID, signed int x, signed int y, signed int z) {
    { // 0x00351A50 - 0x00351D0C
    }
}

// 0x00351D10 - 0x00351F4C (0x23C) objGetFlags__Fi
signed int objGetFlags(signed int id) {
    { // 0x00351D10 - 0x00351F4C
    }
}

// 0x00351F50 - 0x003521A0 (0x250) objGetRotY__Fi
signed int objGetRotY(signed int id) {
    { // 0x00351F50 - 0x003521A0
    }
}

// 0x003521A0 - 0x003523F0 (0x250) objGetPositionZ__Fi
signed int objGetPositionZ(signed int id) {
    { // 0x003521A0 - 0x003523F0
    }
}

// 0x003523F0 - 0x00352640 (0x250) objGetPositionY__Fi
signed int objGetPositionY(signed int id) {
    { // 0x003523F0 - 0x00352640
    }
}

// 0x00352640 - 0x00352890 (0x250) objGetPositionX__Fi
signed int objGetPositionX(signed int id) {
    { // 0x00352640 - 0x00352890
    }
}

// 0x00352890 - 0x00352B44 (0x2B4) objSetPosition__Fiiii
void objSetPosition(signed int objID, signed int x, signed int y, signed int z) {
    { // 0x00352890 - 0x00352B44
    }
}

// 0x00352B50 - 0x00352D3C (0x1EC) objSetPositionNavigationPoint__Fii
void objSetPositionNavigationPoint(signed int objID, signed int navID) {
    { // 0x00352B50 - 0x00352D3C
    }
}

// 0x00352D40 - 0x00353004 (0x2C4) objSetPositionMarker__Fiii
void objSetPositionMarker(signed int objID, signed int markerID, signed int mapToFloor) {
    { // 0x00352D40 - 0x00353004
        class xMarker *marker; // r2
    }
}

// 0x00353010 - 0x00353030 (0x20) objSetPositionActor__Fiiii
void objSetPositionActor(signed int objID, signed int toObjectType, signed int toActorID, signed int smartDrop) {
    { // 0x00353010 - 0x00353030
    }
}

// 0x00353030 - 0x003532D8 (0x2A8) objSetOrientation__Fiiii
void objSetOrientation(signed int objID, signed int x, signed int y, signed int z) {
    { // 0x00353030 - 0x003532D8
    }
}

// 0x003532E0 - 0x003535F4 (0x314) objSetVisible__Fii
void objSetVisible(signed int id, signed int visible) {
    { // 0x003532E0 - 0x003535F4
    }
}

// 0x00353600 - 0x00353A34 (0x434) xObjDBSetPositionActor__Fiiii
void xObjDBSetPositionActor(signed int objID, signed int toActorType, signed int toActorID, signed int smartDrop) {
    { // 0x00353600 - 0x00353A34
        class xNavPoint *nav; // r2
    }
}

// 0x00353A40 - 0x00353B1C (0xDC) xObjDBIdle__Fv
void xObjDBIdle() {
    { // 0x00353A40 - 0x00353B1C
        class iObjRuntime *r; // r17
        class _xObj *a; // r2
        signed int i; // r16
    }
}

// 0x00353B20 - 0x00353B28 (0x8) xObjDBEventSendAll__Fiiiii
void xObjDBEventSendAll() {
    { // 0x00353B20 - 0x00353B28
    }
}

// 0x00353B30 - 0x003540A8 (0x578) xObjDBSetup__FiiUiUiUi
void xObjDBSetup(signed int objectType, signed int id, unsigned int modelID, unsigned int textureID) {
    { // 0x00353B30 - 0x003540A8
        class _xTex *t; // r19
        class _xModel *model; // r2
        class iObjRuntime *r; // r17
        class _xObj *a; // r16
    }
}

// 0x003540B0 - 0x00354140 (0x90) xObjDBDraw__Fv
void xObjDBDraw() {
    { // 0x003540B0 - 0x00354140
        class iObjRuntime *r; // r3
        signed int i; // r16
    }
}

// 0x00354140 - 0x0035419C (0x5C) xObjDBSetLevelReset__Fv
void xObjDBSetLevelReset() {
    { // 0x00354140 - 0x0035419C
        signed int i; // r5
    }
}

// 0x003541A0 - 0x00354260 (0xC0) xObjDBLevelReset__Fv
void xObjDBLevelReset() {
    { // 0x003541A0 - 0x00354260
        signed int i; // r16
    }
}

// 0x00354260 - 0x0035426C (0xC) xObjDBReInitializeInterpreter__Fv
void xObjDBReInitializeInterpreter() {
    { // 0x00354260 - 0x0035426C
    }
}

// 0x00354270 - 0x003542C0 (0x50) objEffectSetFlag__Fiiii
void objEffectSetFlag(signed int objID, signed int flag, signed int mode) {
    { // 0x00354270 - 0x003542C0
    }
}

// 0x003542C0 - 0x0035432C (0x6C) objEffectReflect__Fiiiiiii
void objEffectReflect(signed int objID, signed int mode, signed int r, signed int g, signed int b) {
    { // 0x003542C0 - 0x0035432C
    }
}

// 0x00354330 - 0x003543AC (0x7C) objEffectFadeInfo__Fiiiiiii
void objEffectFadeInfo(signed int objID, signed int r, signed int g, signed int b, signed int mode, signed int idx) {
    { // 0x00354330 - 0x003543AC
    }
}

// 0x003543B0 - 0x003543FC (0x4C) objEffectFadeStart__Fiii
void objEffectFadeStart(signed int objID, signed int flag, signed int time) {
    { // 0x003543B0 - 0x003543FC
    }
}

// 0x00354400 - 0x0035442C (0x2C) objEffectFadeDone__Fi
signed int objEffectFadeDone(signed int objID) {
    { // 0x00354400 - 0x0035442C
    }
}

// 0x00354430 - 0x003546F8 (0x2C8) objGetRenderEffectsPtr__Fi
class _iRenderEffects *objGetRenderEffectsPtr(signed int objID) {
    { // 0x00354430 - 0x003546F8
        class iObjRuntime *r; // r16
        class _xObj *o; // r16
        class _iRenderEffects *effects; // r2
    }
}

// 0x00354700 - 0x003549D4 (0x2D4) objFlagsSet__Fii
signed int objFlagsSet(signed int objID, signed int flags) {
    { // 0x00354700 - 0x003549D4
        class iObjRuntime *r; // r16
        class _xObj *o; // r16
    }
}

// 0x003549E0 - 0x00354CCC (0x2EC) objFlagsSub__Fii
signed int objFlagsSub(signed int objID, signed int flags) {
    { // 0x003549E0 - 0x00354CCC
        class iObjRuntime *r; // r16
        class _xObj *o; // r16
    }
}

// 0x00354CD0 - 0x00354FAC (0x2DC) objFlagsAdd__Fii
signed int objFlagsAdd(signed int objID, signed int flags) {
    { // 0x00354CD0 - 0x00354FAC
        class iObjRuntime *r; // r16
        class _xObj *o; // r16
    }
}

// 0x00354FB0 - 0x00355498 (0x4E8) xObjDBInitialize__Fv
void xObjDBInitialize() {
    { // 0x00354FB0 - 0x00355498
        signed int i; // r5
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xObj.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int g_RTObjModelCount; // 0x004A4D34
class _xModel g_RTObjModel[110]; // 0x004E0080
// 0x003554A0 - 0x003554C8 (0x28) xObjSetOrientation__FP5_xObjfff
void xObjSetOrientation(class _xObj *a, float rx, float ry, float rz) {
    { // 0x003554A0 - 0x003554C8
        class iObjRuntime *r; // r2
    }
}

// 0x003554D0 - 0x003554F8 (0x28) xObjSetPosition__FP5_xObjfff
void xObjSetPosition(class _xObj *o, float x, float y, float z) {
    { // 0x003554D0 - 0x003554F8
        class iObjRuntime *r; // r2
    }
}

// 0x00355500 - 0x00355520 (0x20) xObjDraw__FP5_xObj
void xObjDraw(class _xObj *o) {
    { // 0x00355500 - 0x00355520
    }
}

// 0x00355520 - 0x00355528 (0x8) xObjIdle__FP5_xObj
void xObjIdle() {
    { // 0x00355520 - 0x00355528
    }
}

// 0x00355530 - 0x003555E8 (0xB8) xObjDestroy__FP5_xObj
void xObjDestroy(class _xObj *o) {
    { // 0x00355530 - 0x003555E8
    }
}

// 0x003555F0 - 0x003556E4 (0xF4) xObjInitialize__FP5_xObj
void xObjInitialize(class _xObj *o) {
    { // 0x003555F0 - 0x003556E4
        class iObjRuntime *r; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xTimestmp.c
// ==========================================================================
char timestamp[118]; // 0x004499E0

// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNWZ.cpp
// ==========================================================================
class tagxState g_nwzStates[15]; // 0x00449B60
static class tagxStateLink link_nwz_DieVortex[1]; // 0x004A3FE0
static class tagxStateLink link_nwz_InjuryRangeUpperBodyThread[1]; // 0x004A3FDC
static class tagxStateLink link_nwz_InjuryMeleeUpperBodyThread[1]; // 0x004A3FD8
static class tagxStateLink link_nwz_InjuryRangeFromIdle[1]; // 0x004A3FD4
static class tagxStateLink link_nwz_InjuryMeleeFromIdle[1]; // 0x004A3FD0
static class tagxStateLink link_nwz_AttackRangeSky[1]; // 0x004A3FCC
static class tagxStateLink link_nwz_AttackRangeThrow[1]; // 0x004A3FC8
static class tagxStateLink link_nwz_AttackRangeCape[1]; // 0x004A3FC4
static class tagxStateLink link_nwz_AttackMeleeKick[1]; // 0x004A3FC0
static class tagxStateLink link_nwz_BlockRight[1]; // 0x004A3FBC
static class tagxStateLink link_nwz_BlockLeft[1]; // 0x004A3FB8
static class tagxStateLink link_nwz_DodgeRight[1]; // 0x004A3FB4
static class tagxStateLink link_nwz_DodgeLeft[1]; // 0x004A3FB0
static class tagxStateLink link_nwz_Run[11]; // 0x00449B30
static class tagxStateLink link_nwz_Idle[19]; // 0x00449AE0
unsigned char g_nwzJointsUpperOnly[51]; // 0x00449AA0
unsigned char g_nwzJointsAll[51]; // 0x00449A60
// 0x003556F0 - 0x0035571C (0x2C) xNWZStateReset__Fv
void xNWZStateReset() {
    { // 0x003556F0 - 0x0035571C
    }
}

// 0x00355720 - 0x0035574C (0x2C) xNWZStateInit__Fv
void xNWZStateInit() {
    { // 0x00355720 - 0x0035574C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNWC.cpp
// ==========================================================================
class tagxState g_nwcStates[26]; // 0x00449E30
static class tagxStateLink link_nwc_ComboDeathAxe[1]; // 0x004A4024
static class tagxStateLink link_nwc_ComboDeathGun[1]; // 0x004A4020
static class tagxStateLink link_nwc_ComboMissDeath[1]; // 0x004A401C
static class tagxStateLink link_nwc_ComboIdleChainsaw[3]; // 0x00449E18
static class tagxStateLink link_nwc_ComboIdle[5]; // 0x00449E00
static class tagxStateLink link_nwc_DeathRange4[1]; // 0x004A4018
static class tagxStateLink link_nwc_DeathRange3[1]; // 0x004A4014
static class tagxStateLink link_nwc_DeathRange2[1]; // 0x004A4010
static class tagxStateLink link_nwc_DeathRange1[1]; // 0x004A400C
static class tagxStateLink link_nwc_DeathMelee4[1]; // 0x004A4008
static class tagxStateLink link_nwc_DeathMelee3[1]; // 0x004A4004
static class tagxStateLink link_nwc_DeathMelee2[1]; // 0x004A4000
static class tagxStateLink link_nwc_DeathMelee1[1]; // 0x004A3FFC
static class tagxStateLink link_nwc_InjuryInWalkKeepWalking[1]; // 0x004A3FF8
static class tagxStateLink link_nwc_InjuryInWalkStop[1]; // 0x004A3FF4
static class tagxStateLink link_nwc_InjuryMeleeStrong[1]; // 0x004A3FF0
static class tagxStateLink link_nwc_InjuryStrongInComboIdle[1]; // 0x004A3FEC
static class tagxStateLink link_nwc_InjuryMelee[1]; // 0x004A3FE8
static class tagxStateLink link_nwc_AttackStrong[3]; // 0x00449DF0
static class tagxStateLink link_nwc_AttackRight[3]; // 0x00449DE0
static class tagxStateLink link_nwc_AttackLeft[3]; // 0x00449DD0
static class tagxStateLink link_nwc_DodgeLeft[1]; // 0x004A3FE4
static class tagxStateLink link_nwc_Walk[11]; // 0x00449DA0
static class tagxStateLink link_nwc_HopPain[12]; // 0x00449D70
static class tagxStateLink link_nwc_Hop[12]; // 0x00449D40
static class tagxStateLink link_nwc_Idle[18]; // 0x00449CF0
unsigned char g_nwcJointsUpperOnly[14]; // 0x00449CD8
unsigned char g_nwcJointsAll[14]; // 0x00449CC8
// 0x00355750 - 0x0035577C (0x2C) xNWCStateReset__Fv
void xNWCStateReset() {
    { // 0x00355750 - 0x0035577C
    }
}

// 0x00355780 - 0x003557AC (0x2C) xNWCStateInit__Fv
void xNWCStateInit() {
    { // 0x00355780 - 0x003557AC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNTR.cpp
// ==========================================================================
class tagxState g_ntrStates[6]; // 0x0044A0F0
static class tagxStateLink link_ntr_Death[1]; // 0x004A4038
static class tagxStateLink link_ntr_InjuryMiddle[1]; // 0x004A4034
static class tagxStateLink link_ntr_AttackFar[1]; // 0x004A4030
static class tagxStateLink link_ntr_AttackNear2[1]; // 0x004A402C
static class tagxStateLink link_ntr_AttackNear1[1]; // 0x004A4028
static class tagxStateLink link_ntr_Idle[12]; // 0x0044A0C0
unsigned char g_ntrJointsAll[17]; // 0x0044A0A0
// 0x003557B0 - 0x003557DC (0x2C) xNTRStateReset__Fv
void xNTRStateReset() {
    { // 0x003557B0 - 0x003557DC
    }
}

// 0x003557E0 - 0x0035580C (0x2C) xNTRStateInit__Fv
void xNTRStateInit() {
    { // 0x003557E0 - 0x0035580C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNTA.cpp
// ==========================================================================
class tagxState g_ntaStates[3]; // 0x0044A1B0
static class tagxStateLink link_nta_Die[1]; // 0x004A4044
static class tagxStateLink link_nta_Attack[1]; // 0x004A4040
static class tagxStateLink link_nta_Idle[9]; // 0x0044A180
unsigned char g_ntaJointsAll[4]; // 0x004A403C
// 0x00355810 - 0x0035583C (0x2C) xNTAStateReset__Fv
void xNTAStateReset() {
    { // 0x00355810 - 0x0035583C
    }
}

// 0x00355840 - 0x0035586C (0x2C) xNTAStateInit__Fv
void xNTAStateInit() {
    { // 0x00355840 - 0x0035586C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNSL.cpp
// ==========================================================================
class tagxState g_nslStates[22]; // 0x0044A3E0
static class tagxStateLink link_nsl_DeathStalactite[1]; // 0x004A404C
static class tagxStateLink link_nsl_InjuryStalactite[1]; // 0x004A4048
static class tagxStateLink link_nsl_AttackMeleeRightRun[2]; // 0x0044A3D0
static class tagxStateLink link_nsl_AttackMeleeLeftRun[2]; // 0x0044A3C8
static class tagxStateLink link_nsl_AttackMeleeRightWalk[2]; // 0x0044A3C0
static class tagxStateLink link_nsl_AttackMeleeLeftWalk[2]; // 0x0044A3B8
static class tagxStateLink link_nsl_AttackMeleeRightFromRRun[2]; // 0x0044A3B0
static class tagxStateLink link_nsl_AttackMeleeLeftFromRRun[2]; // 0x0044A3A8
static class tagxStateLink link_nsl_AttackMeleeRightFromIdle[2]; // 0x0044A3A0
static class tagxStateLink link_nsl_AttackMeleeLeftFromIdle[2]; // 0x0044A398
static class tagxStateLink link_nsl_DodgeRightFromRRun[2]; // 0x0044A390
static class tagxStateLink link_nsl_DodgeLeftFromRRun[2]; // 0x0044A388
static class tagxStateLink link_nsl_DodgeRightFromIdle[2]; // 0x0044A380
static class tagxStateLink link_nsl_DodgeLeftFromIdle[2]; // 0x0044A378
static class tagxStateLink link_nsl_Run[13]; // 0x0044A340
static class tagxStateLink link_nsl_Walk[13]; // 0x0044A300
static class tagxStateLink link_nsl_CallBatsFromRRun[2]; // 0x0044A2F0
static class tagxStateLink link_nsl_CallBatsFromIdle[2]; // 0x0044A2E8
static class tagxStateLink link_nsl_LaughterFromRRun[2]; // 0x0044A2E0
static class tagxStateLink link_nsl_LaughterFromIdle[2]; // 0x0044A2D8
static class tagxStateLink link_nsl_RRun[17]; // 0x0044A290
static class tagxStateLink link_nsl_Idle[17]; // 0x0044A240
unsigned char g_nslJointsUpperOnly[18]; // 0x0044A220
unsigned char g_nslJointsAll[18]; // 0x0044A200
// 0x00355870 - 0x0035589C (0x2C) xNSLStateReset__Fv
void xNSLStateReset() {
    { // 0x00355870 - 0x0035589C
    }
}

// 0x003558A0 - 0x003558CC (0x2C) xNSLStateInit__Fv
void xNSLStateInit() {
    { // 0x003558A0 - 0x003558CC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNSK.cpp
// ==========================================================================
class tagxState g_nskStates[37]; // 0x0044A750
static class tagxStateLink link_nsk_ComboDeathAxe[1]; // 0x004A40C8
static class tagxStateLink link_nsk_ComboDeathGun[1]; // 0x004A40C4
static class tagxStateLink link_nsk_ComboMissDeath[1]; // 0x004A40C0
static class tagxStateLink link_nsk_ComboIdleChainsaw[3]; // 0x0044A740
static class tagxStateLink link_nsk_ComboIdle[3]; // 0x0044A730
static class tagxStateLink link_nsk_DieBrokenSpine[1]; // 0x004A40BC
static class tagxStateLink link_nsk_DieFallForward[1]; // 0x004A40B8
static class tagxStateLink link_nsk_DieFlyBack[1]; // 0x004A40B4
static class tagxStateLink link_nsk_DieHeartAttack[1]; // 0x004A40B0
static class tagxStateLink link_nsk_InjuryUpperBodyThread[1]; // 0x004A40AC
static class tagxStateLink link_nsk_InjuryStrongFromRRun[1]; // 0x004A40A8
static class tagxStateLink link_nsk_InjuryFromRRun[1]; // 0x004A40A4
static class tagxStateLink link_nsk_InjuryStrongFromIdle[1]; // 0x004A40A0
static class tagxStateLink link_nsk_InjuryFromIdle[1]; // 0x004A409C
static class tagxStateLink link_nsk_AttackStrongSlapRRun[1]; // 0x004A4098
static class tagxStateLink link_nsk_AttackStrongSpinRRun[1]; // 0x004A4094
static class tagxStateLink link_nsk_AttackMeleeRightRRun[1]; // 0x004A4090
static class tagxStateLink link_nsk_AttackMeleeLeftRRun[1]; // 0x004A408C
static class tagxStateLink link_nsk_AttackStrongSlapIdle[1]; // 0x004A4088
static class tagxStateLink link_nsk_AttackStrongSpinIdle[1]; // 0x004A4084
static class tagxStateLink link_nsk_AttackMeleeRightIdle[1]; // 0x004A4080
static class tagxStateLink link_nsk_AttackMeleeLeftIdle[1]; // 0x004A407C
static class tagxStateLink link_nsk_BlockRightReady[1]; // 0x004A4078
static class tagxStateLink link_nsk_BlockLeftReady[1]; // 0x004A4074
static class tagxStateLink link_nsk_DodgeRightReady[1]; // 0x004A4070
static class tagxStateLink link_nsk_DodgeLeftReady[1]; // 0x004A406C
static class tagxStateLink link_nsk_BlockRightIdle[1]; // 0x004A4068
static class tagxStateLink link_nsk_BlockLeftIdle[1]; // 0x004A4064
static class tagxStateLink link_nsk_DodgeRightIdle[1]; // 0x004A4060
static class tagxStateLink link_nsk_DodgeLeftIdle[1]; // 0x004A405C
static class tagxStateLink link_nsk_Run[12]; // 0x0044A700
static class tagxStateLink link_nsk_Walk[12]; // 0x0044A6D0
static class tagxStateLink link_nsk_StandRRun[22]; // 0x0044A670
static class tagxStateLink link_nsk_Idle[22]; // 0x0044A610
static class tagxStateLink link_nsk_Dive[1]; // 0x004A4058
static class tagxStateLink link_nsk_PopUp[1]; // 0x004A4054
static class tagxStateLink link_nsk_PreBirth[1]; // 0x004A4050
unsigned char g_nskJointsUpperOnly[16]; // 0x0044A600
unsigned char g_nskJointsAll[16]; // 0x0044A5F0
// 0x003558D0 - 0x003558FC (0x2C) xNSKStateReset__Fv
void xNSKStateReset() {
    { // 0x003558D0 - 0x003558FC
    }
}

// 0x00355900 - 0x0035592C (0x2C) xNSKStateInit__Fv
void xNSKStateInit() {
    { // 0x00355900 - 0x0035592C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\XNSB.cpp
// ==========================================================================
class tagxState g_nsbStates[9]; // 0x0044ABC0
static class tagxStateLink link_nsb_DieRange[1]; // 0x004A40D4
static class tagxStateLink link_nsb_DieMelee[1]; // 0x004A40D0
static class tagxStateLink link_nsb_Injury[1]; // 0x004A40CC
static class tagxStateLink link_nsb_MeleeWeakFromRRun[2]; // 0x0044ABB8
static class tagxStateLink link_nsb_MeleeWeakFromIdle[2]; // 0x0044ABB0
static class tagxStateLink link_nsb_FlyFast[11]; // 0x0044AB80
static class tagxStateLink link_nsb_FlySlow[11]; // 0x0044AB50
static class tagxStateLink link_nsb_RRun[12]; // 0x0044AB20
static class tagxStateLink link_nsb_Idle[12]; // 0x0044AAF0
unsigned char g_nsbJointsAll[17]; // 0x0044AAD0
// 0x00355930 - 0x0035595C (0x2C) xNSBStateReset__Fv
void xNSBStateReset() {
    { // 0x00355930 - 0x0035595C
    }
}

// 0x00355960 - 0x0035598C (0x2C) xNSBStateInit__Fv
void xNSBStateInit() {
    { // 0x00355960 - 0x0035598C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNSA.cpp
// ==========================================================================
class tagxState g_nsaStates[34]; // 0x0044AE20
static class tagxStateLink link_nsa_ComboDeathAxe[1]; // 0x004A4144
static class tagxStateLink link_nsa_ComboDeathGun[1]; // 0x004A4140
static class tagxStateLink link_nsa_ComboMissDeath[1]; // 0x004A413C
static class tagxStateLink link_nsa_ComboIdleChainsaw[3]; // 0x0044AE08
static class tagxStateLink link_nsa_ComboIdle[3]; // 0x0044ADF8
static class tagxStateLink link_nsa_WakeOpossum2[1]; // 0x004A4138
static class tagxStateLink link_nsa_DieOpossum2[1]; // 0x004A4134
static class tagxStateLink link_nsa_WakeOpossum1[1]; // 0x004A4130
static class tagxStateLink link_nsa_DieOpossum1[1]; // 0x004A412C
static class tagxStateLink link_nsa_DieRange[1]; // 0x004A4128
static class tagxStateLink link_nsa_DieMelee[1]; // 0x004A4124
static class tagxStateLink link_nsa_InjuryStrongMoveThread[1]; // 0x004A4120
static class tagxStateLink link_nsa_InjuryMoveThread[1]; // 0x004A411C
static class tagxStateLink link_nsa_InjuryUpperBody[1]; // 0x004A4118
static class tagxStateLink link_nsa_InjuryStrongUpperBody[1]; // 0x004A4114
static class tagxStateLink link_nsa_InjuryThread[1]; // 0x004A4110
static class tagxStateLink link_nsa_InjuryStrongThread[1]; // 0x004A410C
static class tagxStateLink link_nsa_AttackStrong[1]; // 0x004A4108
static class tagxStateLink link_nsa_AttackMeleeRight[1]; // 0x004A4104
static class tagxStateLink link_nsa_AttackMeleeLeft[1]; // 0x004A4100
static class tagxStateLink link_nsa_RotateCClkwise[1]; // 0x004A40FC
static class tagxStateLink link_nsa_RotateClkwise[1]; // 0x004A40F8
static class tagxStateLink link_nsa_BlockRightReady[1]; // 0x004A40F4
static class tagxStateLink link_nsa_BlockLeftReady[1]; // 0x004A40F0
static class tagxStateLink link_nsa_DodgeRightReady[1]; // 0x004A40EC
static class tagxStateLink link_nsa_DodgeLeftReady[1]; // 0x004A40E8
static class tagxStateLink link_nsa_BlockRight[1]; // 0x004A40E4
static class tagxStateLink link_nsa_BlockLeft[1]; // 0x004A40E0
static class tagxStateLink link_nsa_DodgeRight[1]; // 0x004A40DC
static class tagxStateLink link_nsa_DodgeLeft[1]; // 0x004A40D8
static class tagxStateLink link_nsa_Run[14]; // 0x0044ADC0
static class tagxStateLink link_nsa_Walk[14]; // 0x0044AD80
static class tagxStateLink link_nsa_StandRRun[22]; // 0x0044AD20
static class tagxStateLink link_nsa_Idle[24]; // 0x0044ACC0
unsigned char g_nsaJointsUpperOnly[16]; // 0x0044ACB0
unsigned char g_nsaJointsAll[16]; // 0x0044ACA0
// 0x00355990 - 0x003559BC (0x2C) xNSAStateReset__Fv
void xNSAStateReset() {
    { // 0x00355990 - 0x003559BC
    }
}

// 0x003559C0 - 0x003559EC (0x2C) xNSAStateInit__Fv
void xNSAStateInit() {
    { // 0x003559C0 - 0x003559EC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNPK.cpp
// ==========================================================================
class tagxState g_npkStates[19]; // 0x0044B280
static class tagxStateLink link_npk_Die[1]; // 0x004A4170
static class tagxStateLink link_npk_InjuryLongInRun[1]; // 0x004A416C
static class tagxStateLink link_npk_InjuryShortInRun[1]; // 0x004A4168
static class tagxStateLink link_npk_InjuryLongInWalk[1]; // 0x004A4164
static class tagxStateLink link_npk_InjuryShortInWalk[1]; // 0x004A4160
static class tagxStateLink link_npk_InjuryLongInIdleReady[1]; // 0x004A415C
static class tagxStateLink link_npk_InjuryShortInIdleReady[1]; // 0x004A4158
static class tagxStateLink link_npk_InjuryLongInIdle[1]; // 0x004A4154
static class tagxStateLink link_npk_InjuryShortInIdle[1]; // 0x004A4150
static class tagxStateLink link_npk_AttackMeleeRightIdleReady[3]; // 0x0044B270
static class tagxStateLink link_npk_AttackMeleeLeftIdleReady[3]; // 0x0044B260
static class tagxStateLink link_npk_AttackMeleeRightIdle[3]; // 0x0044B250
static class tagxStateLink link_npk_AttackMeleeLeftIdle[3]; // 0x0044B240
static class tagxStateLink link_npk_DodgeRight[1]; // 0x004A414C
static class tagxStateLink link_npk_DodgeLeft[1]; // 0x004A4148
static class tagxStateLink link_npk_Run[12]; // 0x0044B210
static class tagxStateLink link_npk_Walk[12]; // 0x0044B1E0
static class tagxStateLink link_npk_IdleReady[16]; // 0x0044B1A0
static class tagxStateLink link_npk_Idle[16]; // 0x0044B160
unsigned char g_npkJointsUpperOnly[15]; // 0x00000000
unsigned char g_npkJointsAll[15]; // 0x0044B150
// 0x003559F0 - 0x00355A1C (0x2C) xNPKStateReset__Fv
void xNPKStateReset() {
    { // 0x003559F0 - 0x00355A1C
    }
}

// 0x00355A20 - 0x00355A4C (0x2C) xNPKStateInit__Fv
void xNPKStateInit() {
    { // 0x00355A20 - 0x00355A4C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNPG.cpp
// ==========================================================================
class tagxState g_npgStates[17]; // 0x0044B5E0
static class tagxStateLink link_npg_Death1[1]; // 0x004A4184
static class tagxStateLink link_npg_Injury2Left[1]; // 0x004A4180
static class tagxStateLink link_npg_InjuryDown[1]; // 0x004A417C
static class tagxStateLink link_npg_AttackHeadButtFromRRun[3]; // 0x0044B5D0
static class tagxStateLink link_npg_AttackGoreFromRRun[3]; // 0x0044B5C0
static class tagxStateLink link_npg_AttackHeadButtFromIdle[3]; // 0x0044B5B0
static class tagxStateLink link_npg_AttackGoreFromIdle[3]; // 0x0044B5A0
static class tagxStateLink link_npg_DodgeRightFromRRun[3]; // 0x0044B590
static class tagxStateLink link_npg_DodgeLeftFromRRun[3]; // 0x0044B580
static class tagxStateLink link_npg_DodgeRightFromIdle[3]; // 0x0044B570
static class tagxStateLink link_npg_DodgeLeftFromIdle[3]; // 0x0044B560
static class tagxStateLink link_npg_Run[12]; // 0x0044B530
static class tagxStateLink link_npg_Walk[12]; // 0x0044B500
static class tagxStateLink link_npg_PopUp[1]; // 0x004A4178
static class tagxStateLink link_npg_IdleReadyToRun[16]; // 0x0044B4C0
static class tagxStateLink link_npg_PreBirth[1]; // 0x004A4174
static class tagxStateLink link_npg_Idle[17]; // 0x0044B470
unsigned char g_npgJointsAll[17]; // 0x0044B450
// 0x00355A50 - 0x00355A7C (0x2C) xNPGStateReset__Fv
void xNPGStateReset() {
    { // 0x00355A50 - 0x00355A7C
    }
}

// 0x00355A80 - 0x00355AAC (0x2C) xNPGStateInit__Fv
void xNPGStateInit() {
    { // 0x00355A80 - 0x00355AAC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNNR.cpp
// ==========================================================================
class tagxState g_nnrStates[18]; // 0x0044B900
static class tagxStateLink link_nnr_DieJumping[1]; // 0x004A419C
static class tagxStateLink link_nnr_Die[1]; // 0x004A4198
static class tagxStateLink link_nnr_InjuryJumpingFromRRun[1]; // 0x004A4194
static class tagxStateLink link_nnr_InjuryJumpingFromIdle[1]; // 0x004A4190
static class tagxStateLink link_nnr_InjuryFrontAsThread[1]; // 0x004A418C
static class tagxStateLink link_nnr_InjuryStrongAsThread[1]; // 0x004A4188
static class tagxStateLink link_nnr_AttackJumpFromRRun[3]; // 0x0044B8F0
static class tagxStateLink link_nnr_AttackJumpFromIdle[3]; // 0x0044B8E0
static class tagxStateLink link_nnr_DodgeRightFromRRun[3]; // 0x0044B8D0
static class tagxStateLink link_nnr_DodgeLeftFromRRun[3]; // 0x0044B8C0
static class tagxStateLink link_nnr_DodgeRightFromIdle[3]; // 0x0044B8B0
static class tagxStateLink link_nnr_DodgeLeftFromIdle[3]; // 0x0044B8A0
static class tagxStateLink link_nnr_HopFromRRun[3]; // 0x0044B890
static class tagxStateLink link_nnr_HopFromIdle[3]; // 0x0044B880
static class tagxStateLink link_nnr_Run[12]; // 0x0044B850
static class tagxStateLink link_nnr_Walk[12]; // 0x0044B820
static class tagxStateLink link_nnr_IdleReadyRun[16]; // 0x0044B7E0
static class tagxStateLink link_nnr_Idle[16]; // 0x0044B7A0
unsigned char g_nnrJointsUpperOnly[17]; // 0x00000000
unsigned char g_nnrJointsAll[17]; // 0x0044B780
// 0x00355AB0 - 0x00355ADC (0x2C) xNNRStateReset__Fv
void xNNRStateReset() {
    { // 0x00355AB0 - 0x00355ADC
    }
}

// 0x00355AE0 - 0x00355B0C (0x2C) xNNRStateInit__Fv
void xNNRStateInit() {
    { // 0x00355AE0 - 0x00355B0C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNNQ.cpp
// ==========================================================================
class tagxState g_nnqStates[18]; // 0x0044BC30
static class tagxStateLink link_nnq_DieJumping[1]; // 0x004A41B4
static class tagxStateLink link_nnq_Die[1]; // 0x004A41B0
static class tagxStateLink link_nnq_InjuryJumpingFromRRun[1]; // 0x004A41AC
static class tagxStateLink link_nnq_InjuryJumpingFromIdle[1]; // 0x004A41A8
static class tagxStateLink link_nnq_InjuryFrontAsThread[1]; // 0x004A41A4
static class tagxStateLink link_nnq_InjuryStrongAsThread[1]; // 0x004A41A0
static class tagxStateLink link_nnq_AttackJumpFromRRun[3]; // 0x0044BC20
static class tagxStateLink link_nnq_AttackJumpFromIdle[3]; // 0x0044BC10
static class tagxStateLink link_nnq_DodgeRightFromRRun[3]; // 0x0044BC00
static class tagxStateLink link_nnq_DodgeLeftFromRRun[3]; // 0x0044BBF0
static class tagxStateLink link_nnq_DodgeRightFromIdle[3]; // 0x0044BBE0
static class tagxStateLink link_nnq_DodgeLeftFromIdle[3]; // 0x0044BBD0
static class tagxStateLink link_nnq_HopFromRRun[3]; // 0x0044BBC0
static class tagxStateLink link_nnq_HopFromIdle[3]; // 0x0044BBB0
static class tagxStateLink link_nnq_Run[12]; // 0x0044BB80
static class tagxStateLink link_nnq_Walk[12]; // 0x0044BB50
static class tagxStateLink link_nnq_IdleReadyRun[16]; // 0x0044BB10
static class tagxStateLink link_nnq_Idle[16]; // 0x0044BAD0
unsigned char g_nnqJointsUpperOnly[17]; // 0x00000000
unsigned char g_nnqJointsAll[17]; // 0x0044BAB0
// 0x00355B10 - 0x00355B3C (0x2C) xNNQStateReset__Fv
void xNNQStateReset() {
    { // 0x00355B10 - 0x00355B3C
    }
}

// 0x00355B40 - 0x00355B6C (0x2C) xNNQStateInit__Fv
void xNNQStateInit() {
    { // 0x00355B40 - 0x00355B6C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNNP.cpp
// ==========================================================================
class tagxState g_nnpStates[18]; // 0x0044BF60
static class tagxStateLink link_nnp_DieJumping[1]; // 0x004A41CC
static class tagxStateLink link_nnp_Die[1]; // 0x004A41C8
static class tagxStateLink link_nnp_InjuryJumpingFromRRun[1]; // 0x004A41C4
static class tagxStateLink link_nnp_InjuryJumpingFromIdle[1]; // 0x004A41C0
static class tagxStateLink link_nnp_InjuryFrontAsThread[1]; // 0x004A41BC
static class tagxStateLink link_nnp_InjuryStrongAsThread[1]; // 0x004A41B8
static class tagxStateLink link_nnp_AttackJumpFromRRun[3]; // 0x0044BF50
static class tagxStateLink link_nnp_AttackJumpFromIdle[3]; // 0x0044BF40
static class tagxStateLink link_nnp_DodgeRightFromRRun[3]; // 0x0044BF30
static class tagxStateLink link_nnp_DodgeLeftFromRRun[3]; // 0x0044BF20
static class tagxStateLink link_nnp_DodgeRightFromIdle[3]; // 0x0044BF10
static class tagxStateLink link_nnp_DodgeLeftFromIdle[3]; // 0x0044BF00
static class tagxStateLink link_nnp_HopFromRRun[3]; // 0x0044BEF0
static class tagxStateLink link_nnp_HopFromIdle[3]; // 0x0044BEE0
static class tagxStateLink link_nnp_Run[12]; // 0x0044BEB0
static class tagxStateLink link_nnp_Walk[12]; // 0x0044BE80
static class tagxStateLink link_nnp_IdleReadyRun[16]; // 0x0044BE40
static class tagxStateLink link_nnp_Idle[16]; // 0x0044BE00
unsigned char g_nnpJointsUpperOnly[17]; // 0x00000000
unsigned char g_nnpJointsAll[17]; // 0x0044BDE0
// 0x00355B70 - 0x00355B9C (0x2C) xNNPStateReset__Fv
void xNNPStateReset() {
    { // 0x00355B70 - 0x00355B9C
    }
}

// 0x00355BA0 - 0x00355BCC (0x2C) xNNPStateInit__Fv
void xNNPStateInit() {
    { // 0x00355BA0 - 0x00355BCC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNNM.cpp
// ==========================================================================
class tagxState g_nnmStates[26]; // 0x0044C1C0
static class tagxStateLink link_nnm_ComboDeathAxe[1]; // 0x004A4224
static class tagxStateLink link_nnm_ComboDeathGun[1]; // 0x004A4220
static class tagxStateLink link_nnm_ComboMissDeath[1]; // 0x004A421C
static class tagxStateLink link_nnm_ComboIdleChainsaw[3]; // 0x0044C1B0
static class tagxStateLink link_nnm_ComboIdle[3]; // 0x0044C1A0
static class tagxStateLink link_nnm_DeathRange4[1]; // 0x004A4218
static class tagxStateLink link_nnm_DeathRange3[1]; // 0x004A4214
static class tagxStateLink link_nnm_DeathRange2[1]; // 0x004A4210
static class tagxStateLink link_nnm_DeathRange1[1]; // 0x004A420C
static class tagxStateLink link_nnm_DeathMelee4[1]; // 0x004A4208
static class tagxStateLink link_nnm_DeathMelee3[1]; // 0x004A4204
static class tagxStateLink link_nnm_DeathMelee2[1]; // 0x004A4200
static class tagxStateLink link_nnm_DeathMelee1[1]; // 0x004A41FC
static class tagxStateLink link_nnm_InjuryRangeWalk[1]; // 0x004A41F8
static class tagxStateLink link_nnm_InjuryMeleeWalk[1]; // 0x004A41F4
static class tagxStateLink link_nnm_InjuryRangeIdle[1]; // 0x004A41F0
static class tagxStateLink link_nnm_InjuryMeleeIdle[1]; // 0x004A41EC
static class tagxStateLink link_nnm_InjuryMeleeComboIdle[1]; // 0x004A41E8
static class tagxStateLink link_nnm_AttackMelee[1]; // 0x004A41E4
static class tagxStateLink link_nnm_AttackRangeRight[1]; // 0x004A41E0
static class tagxStateLink link_nnm_AttackRangeLeft[1]; // 0x004A41DC
static class tagxStateLink link_nnm_AttackRangeFront[1]; // 0x004A41D8
static class tagxStateLink link_nnm_DodgeRight[1]; // 0x004A41D4
static class tagxStateLink link_nnm_DodgeLeft[1]; // 0x004A41D0
static class tagxStateLink link_nnm_Walk[12]; // 0x0044C170
static class tagxStateLink link_nnm_Idle[18]; // 0x0044C120
unsigned char g_nnmJointsUpperOnly[14]; // 0x00000000
unsigned char g_nnmJointsAll[14]; // 0x0044C110
// 0x00355BD0 - 0x00355BFC (0x2C) xNNMStateReset__Fv
void xNNMStateReset() {
    { // 0x00355BD0 - 0x00355BFC
    }
}

// 0x00355C00 - 0x00355C2C (0x2C) xNNMStateInit__Fv
void xNNMStateInit() {
    { // 0x00355C00 - 0x00355C2C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNMR.cpp
// ==========================================================================
class tagxState g_nmrStates[4]; // 0x0044C450
static class tagxStateLink link_nmr_GiveObject[1]; // 0x004A4230
static class tagxStateLink link_nmr_TalkShort[1]; // 0x004A422C
static class tagxStateLink link_nmr_TalkLong[1]; // 0x004A4228
static class tagxStateLink link_nmr_Work[3]; // 0x0044C440
unsigned char g_nmrJointsUpperOnly[16]; // 0x00000000
unsigned char g_nmrJointsAll[16]; // 0x0044C430
// 0x00355C30 - 0x00355C5C (0x2C) xNMRStateReset__Fv
void xNMRStateReset() {
    { // 0x00355C30 - 0x00355C5C
    }
}

// 0x00355C60 - 0x00355C8C (0x2C) xNMRStateInit__Fv
void xNMRStateInit() {
    { // 0x00355C60 - 0x00355C8C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNMM.cpp
// ==========================================================================
class tagxState g_nmmStates[23]; // 0x0044C570
static class tagxStateLink link_nmm_ComboDeathAxe[1]; // 0x004A427C
static class tagxStateLink link_nmm_ComboDeathGun[1]; // 0x004A4278
static class tagxStateLink link_nmm_ComboMissDeath[1]; // 0x004A4274
static class tagxStateLink link_nmm_ComboIdleChainsaw[3]; // 0x0044C560
static class tagxStateLink link_nmm_ComboIdle[3]; // 0x0044C550
static class tagxStateLink link_nmm_DeathRange1[1]; // 0x004A4270
static class tagxStateLink link_nmm_DeathMelee1[1]; // 0x004A426C
static class tagxStateLink link_nmm_InjuryInThread[1]; // 0x004A4268
static class tagxStateLink link_nmm_InjuryStrongInRun[1]; // 0x004A4264
static class tagxStateLink link_nmm_InjuryFrontInRun[1]; // 0x004A4260
static class tagxStateLink link_nmm_InjuryStrongInIdle[1]; // 0x004A425C
static class tagxStateLink link_nmm_InjuryFrontInIdle[1]; // 0x004A4258
static class tagxStateLink link_nmm_AttackRangePounceStab[1]; // 0x004A4254
static class tagxStateLink link_nmm_AttackRangePounce[1]; // 0x004A4250
static class tagxStateLink link_nmm_AttackMeleeStrong[1]; // 0x004A424C
static class tagxStateLink link_nmm_AttackMeleeRight[1]; // 0x004A4248
static class tagxStateLink link_nmm_AttackMeleeLeft[1]; // 0x004A4244
static class tagxStateLink link_nmm_BlockRight[1]; // 0x004A4240
static class tagxStateLink link_nmm_BlockLeft[1]; // 0x004A423C
static class tagxStateLink link_nmm_DodgeRight[1]; // 0x004A4238
static class tagxStateLink link_nmm_DodgeLeft[1]; // 0x004A4234
static class tagxStateLink link_nmm_Run[11]; // 0x0044C520
static class tagxStateLink link_nmm_Idle[21]; // 0x0044C4C0
unsigned char g_nmmJointsUpperOnly[16]; // 0x00000000
unsigned char g_nmmJointsAll[16]; // 0x0044C4B0
// 0x00355C90 - 0x00355CBC (0x2C) xNMMStateReset__Fv
void xNMMStateReset() {
    { // 0x00355C90 - 0x00355CBC
    }
}

// 0x00355CC0 - 0x00355CEC (0x2C) xNMMStateInit__Fv
void xNMMStateInit() {
    { // 0x00355CC0 - 0x00355CEC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNMI.cpp
// ==========================================================================
class tagxState g_nmiStates[19]; // 0x0044C950
static class tagxStateLink link_nmi_DeathMelee3[1]; // 0x004A42A8
static class tagxStateLink link_nmi_InjuryRightShortFromIdleUpper[1]; // 0x004A42A4
static class tagxStateLink link_nmi_InjuryRightLongFromIdleUpper[1]; // 0x004A42A0
static class tagxStateLink link_nmi_InjuryLeftShortFromIdleUpper[1]; // 0x004A429C
static class tagxStateLink link_nmi_InjuryLeftLongFromIdleUpper[1]; // 0x004A4298
static class tagxStateLink link_nmi_InjuryRightShortFromIdle[1]; // 0x004A4294
static class tagxStateLink link_nmi_InjuryRightLongFromIdle[1]; // 0x004A4290
static class tagxStateLink link_nmi_InjuryLeftShortFromIdle[1]; // 0x004A428C
static class tagxStateLink link_nmi_InjuryLeftLongFromIdle[1]; // 0x004A4288
static class tagxStateLink link_nmi_AttackRangeRight[5]; // 0x0044C930
static class tagxStateLink link_nmi_AttackRangeLeft[5]; // 0x0044C910
static class tagxStateLink link_nmi_AttackMeleeRight[5]; // 0x0044C8F0
static class tagxStateLink link_nmi_AttackMeleeLeft[5]; // 0x0044C8D0
static class tagxStateLink link_nmi_WalkLow[13]; // 0x0044C890
static class tagxStateLink link_nmi_Walk[11]; // 0x0044C860
static class tagxStateLink link_nmi_Turn180Right[1]; // 0x004A4284
static class tagxStateLink link_nmi_Turn180Left[1]; // 0x004A4280
static class tagxStateLink link_nmi_IdleLow[14]; // 0x0044C820
static class tagxStateLink link_nmi_Idle[16]; // 0x0044C7E0
unsigned char g_nmiJointsUpperOnly[32]; // 0x0044C7C0
unsigned char g_nmiJointsAll[32]; // 0x0044C7A0
// 0x00355CF0 - 0x00355D1C (0x2C) xNMIStateReset__Fv
void xNMIStateReset() {
    { // 0x00355CF0 - 0x00355D1C
    }
}

// 0x00355D20 - 0x00355D4C (0x2C) xNMIStateInit__Fv
void xNMIStateInit() {
    { // 0x00355D20 - 0x00355D4C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNMG.cpp
// ==========================================================================
class tagxState g_nmgStates[17]; // 0x0044CBB0
static class tagxStateLink link_nmg_DeathRange4[1]; // 0x004A42E4
static class tagxStateLink link_nmg_DeathRange3[1]; // 0x004A42E0
static class tagxStateLink link_nmg_DeathRange2[1]; // 0x004A42DC
static class tagxStateLink link_nmg_DeathRange1[1]; // 0x004A42D8
static class tagxStateLink link_nmg_DeathMelee4[1]; // 0x004A42D4
static class tagxStateLink link_nmg_DeathMelee3[1]; // 0x004A42D0
static class tagxStateLink link_nmg_DeathMelee2[1]; // 0x004A42CC
static class tagxStateLink link_nmg_DeathMelee1[1]; // 0x004A42C8
static class tagxStateLink link_nmg_InjuryRangeWalk[1]; // 0x004A42C4
static class tagxStateLink link_nmg_InjuryMeleeWalk[1]; // 0x004A42C0
static class tagxStateLink link_nmg_InjuryRangeIdle[1]; // 0x004A42BC
static class tagxStateLink link_nmg_InjuryMeleeIdle[1]; // 0x004A42B8
static class tagxStateLink link_nmg_AttackStrong[1]; // 0x004A42B4
static class tagxStateLink link_nmg_AttackMeleeRight[1]; // 0x004A42B0
static class tagxStateLink link_nmg_AttackMeleeLeft[1]; // 0x004A42AC
static class tagxStateLink link_nmg_Walk[11]; // 0x0044CB80
static class tagxStateLink link_nmg_Idle[14]; // 0x0044CB40
unsigned char g_nmgJointsUpperOnly[16]; // 0x0044CB30
unsigned char g_nmgJointsAll[16]; // 0x0044CB20
// 0x00355D50 - 0x00355D7C (0x2C) xNMGStateReset__Fv
void xNMGStateReset() {
    { // 0x00355D50 - 0x00355D7C
    }
}

// 0x00355D80 - 0x00355DAC (0x2C) xNMGStateInit__Fv
void xNMGStateInit() {
    { // 0x00355D80 - 0x00355DAC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNMC.cpp
// ==========================================================================
class tagxState g_nmcStates[23]; // 0x0044CE10
static class tagxStateLink link_nmc_ComboDeathAxe[1]; // 0x004A4330
static class tagxStateLink link_nmc_ComboDeathGun[1]; // 0x004A432C
static class tagxStateLink link_nmc_ComboMissDeath[1]; // 0x004A4328
static class tagxStateLink link_nmc_ComboIdleChainsaw[3]; // 0x0044CE00
static class tagxStateLink link_nmc_ComboIdle[3]; // 0x0044CDF0
static class tagxStateLink link_nmc_DeathRange1[1]; // 0x004A4324
static class tagxStateLink link_nmc_DeathMelee1[1]; // 0x004A4320
static class tagxStateLink link_nmc_InjuryInThread[1]; // 0x004A431C
static class tagxStateLink link_nmc_InjuryStrongInRun[1]; // 0x004A4318
static class tagxStateLink link_nmc_InjuryFrontInRun[1]; // 0x004A4314
static class tagxStateLink link_nmc_InjuryStrongInIdle[1]; // 0x004A4310
static class tagxStateLink link_nmc_InjuryFrontInIdle[1]; // 0x004A430C
static class tagxStateLink link_nmc_AttackRangePounceSlash[1]; // 0x004A4308
static class tagxStateLink link_nmc_AttackRangePounce[1]; // 0x004A4304
static class tagxStateLink link_nmc_AttackMeleeStrong[1]; // 0x004A4300
static class tagxStateLink link_nmc_AttackMeleeRight[1]; // 0x004A42FC
static class tagxStateLink link_nmc_AttackMeleeLeft[1]; // 0x004A42F8
static class tagxStateLink link_nmc_BlockRight[1]; // 0x004A42F4
static class tagxStateLink link_nmc_BlockLeft[1]; // 0x004A42F0
static class tagxStateLink link_nmc_DodgeRight[1]; // 0x004A42EC
static class tagxStateLink link_nmc_DodgeLeft[1]; // 0x004A42E8
static class tagxStateLink link_nmc_Run[11]; // 0x0044CDC0
static class tagxStateLink link_nmc_Idle[21]; // 0x0044CD60
unsigned char g_nmcJointsUpperOnly[16]; // 0x00000000
unsigned char g_nmcJointsAll[16]; // 0x0044CD50
// 0x00355DB0 - 0x00355DDC (0x2C) xNMCStateReset__Fv
void xNMCStateReset() {
    { // 0x00355DB0 - 0x00355DDC
    }
}

// 0x00355DE0 - 0x00355E0C (0x2C) xNMCStateInit__Fv
void xNMCStateInit() {
    { // 0x00355DE0 - 0x00355E0C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNMA.cpp
// ==========================================================================
class tagxState g_nmaStates[21]; // 0x0044D0F0
static class tagxStateLink link_nma_ComboDeathAxe[1]; // 0x004A4374
static class tagxStateLink link_nma_ComboDeathGun[1]; // 0x004A4370
static class tagxStateLink link_nma_ComboMissDeath[1]; // 0x004A436C
static class tagxStateLink link_nma_ComboIdleChainsaw[3]; // 0x0044D0E0
static class tagxStateLink link_nma_ComboIdle[3]; // 0x0044D0D0
static class tagxStateLink link_nma_DeathRange1[1]; // 0x004A4368
static class tagxStateLink link_nma_DeathMelee1[1]; // 0x004A4364
static class tagxStateLink link_nma_InjuryInThread[1]; // 0x004A4360
static class tagxStateLink link_nma_InjuryStrongInRun[1]; // 0x004A435C
static class tagxStateLink link_nma_InjuryFrontInRun[1]; // 0x004A4358
static class tagxStateLink link_nma_InjuryStrongInIdle[1]; // 0x004A4354
static class tagxStateLink link_nma_InjuryFrontInIdle[1]; // 0x004A4350
static class tagxStateLink link_nma_AttackBowQuick[1]; // 0x004A434C
static class tagxStateLink link_nma_AttackBow[1]; // 0x004A4348
static class tagxStateLink link_nma_AttackMelee[1]; // 0x004A4344
static class tagxStateLink link_nma_BlockRight[1]; // 0x004A4340
static class tagxStateLink link_nma_BlockLeft[1]; // 0x004A433C
static class tagxStateLink link_nma_DodgeRight[1]; // 0x004A4338
static class tagxStateLink link_nma_DodgeLeft[1]; // 0x004A4334
static class tagxStateLink link_nma_Run[11]; // 0x0044D0A0
static class tagxStateLink link_nma_Idle[19]; // 0x0044D050
unsigned char g_nmaJointsUpperOnly[16]; // 0x00000000
unsigned char g_nmaJointsAll[16]; // 0x0044D040
// 0x00355E10 - 0x00355E3C (0x2C) xNMAStateReset__Fv
void xNMAStateReset() {
    { // 0x00355E10 - 0x00355E3C
    }
}

// 0x00355E40 - 0x00355E6C (0x2C) xNMAStateInit__Fv
void xNMAStateInit() {
    { // 0x00355E40 - 0x00355E6C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNHM.cpp
// ==========================================================================
class tagxState g_nhmStates[27]; // 0x0044D430
static class tagxStateLink link_nhm_ComboDeathAxe[1]; // 0x004A43C8
static class tagxStateLink link_nhm_ComboDeathGun[1]; // 0x004A43C4
static class tagxStateLink link_nhm_ComboMissDeath[1]; // 0x004A43C0
static class tagxStateLink link_nhm_ComboIdleChainsaw[3]; // 0x0044D420
static class tagxStateLink link_nhm_ComboIdle[3]; // 0x0044D410
static class tagxStateLink link_nhm_DeathRange4[1]; // 0x004A43BC
static class tagxStateLink link_nhm_DeathRange3[1]; // 0x004A43B8
static class tagxStateLink link_nhm_DeathRange2[1]; // 0x004A43B4
static class tagxStateLink link_nhm_DeathRange1[1]; // 0x004A43B0
static class tagxStateLink link_nhm_DeathMelee4[1]; // 0x004A43AC
static class tagxStateLink link_nhm_DeathMelee3[1]; // 0x004A43A8
static class tagxStateLink link_nhm_DeathMelee2[1]; // 0x004A43A4
static class tagxStateLink link_nhm_DeathMelee1[1]; // 0x004A43A0
static class tagxStateLink link_nhm_InjuryInCombo[1]; // 0x004A439C
static class tagxStateLink link_nhm_InjuryLongInWalk[1]; // 0x004A4398
static class tagxStateLink link_nhm_InjuryShortInWalk[1]; // 0x004A4394
static class tagxStateLink link_nhm_InjuryLongInIdle[1]; // 0x004A4390
static class tagxStateLink link_nhm_InjuryShortInIdle[1]; // 0x004A438C
static class tagxStateLink link_nhm_AttackMeleeStrong[1]; // 0x004A4388
static class tagxStateLink link_nhm_AttackMeleeRight[1]; // 0x004A4384
static class tagxStateLink link_nhm_AttackMeleeLeft[1]; // 0x004A4380
static class tagxStateLink link_nhm_DodgeRight[1]; // 0x004A437C
static class tagxStateLink link_nhm_DodgeLeft[1]; // 0x004A4378
static class tagxStateLink link_nhm_Run[12]; // 0x0044D3E0
static class tagxStateLink link_nhm_Walk[12]; // 0x0044D3B0
static class tagxStateLink link_nhm_RRun[18]; // 0x0044D360
static class tagxStateLink link_nhm_Idle[18]; // 0x0044D310
unsigned char g_nhmJointsAll[17]; // 0x0044D2F0
// 0x00355E70 - 0x00355E9C (0x2C) xNHMStateReset__Fv
void xNHMStateReset() {
    { // 0x00355E70 - 0x00355E9C
    }
}

// 0x00355EA0 - 0x00355ECC (0x2C) xNHMStateInit__Fv
void xNHMStateInit() {
    { // 0x00355EA0 - 0x00355ECC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xnhf.cpp
// ==========================================================================
class tagxState g_nhfStates[17]; // 0x0044D770
static class tagxStateLink link_nhf_ComboDeathAxe[1]; // 0x004A43FC
static class tagxStateLink link_nhf_ComboDeathGun[1]; // 0x004A43F8
static class tagxStateLink link_nhf_ComboMissDeath[1]; // 0x004A43F4
static class tagxStateLink link_nhf_ComboIdleChainsaw[3]; // 0x0044D760
static class tagxStateLink link_nhf_ComboIdle[3]; // 0x0044D750
static class tagxStateLink link_nhf_Death[1]; // 0x004A43F0
static class tagxStateLink link_nhf_InjuryInComboIdle[1]; // 0x004A43EC
static class tagxStateLink link_nhf_InjuryLongInWalk[1]; // 0x004A43E8
static class tagxStateLink link_nhf_InjuryShortInWalk[1]; // 0x004A43E4
static class tagxStateLink link_nhf_InjuryLongInIdle[1]; // 0x004A43E0
static class tagxStateLink link_nhf_InjuryShortInIdle[1]; // 0x004A43DC
static class tagxStateLink link_nhf_AttackMeleeRight[1]; // 0x004A43D8
static class tagxStateLink link_nhf_AttackMeleeLeft[1]; // 0x004A43D4
static class tagxStateLink link_nhf_DodgeRight[1]; // 0x004A43D0
static class tagxStateLink link_nhf_DodgeLeft[1]; // 0x004A43CC
static class tagxStateLink link_nhf_Walk[11]; // 0x0044D720
static class tagxStateLink link_nhf_Idle[16]; // 0x0044D6E0
unsigned char g_nhfJointsAll[17]; // 0x0044D6C0
// 0x00355ED0 - 0x00355EFC (0x2C) xNHFStateReset__Fv
void xNHFStateReset() {
    { // 0x00355ED0 - 0x00355EFC
    }
}

// 0x00355F00 - 0x00355F2C (0x2C) xNHFStateInit__Fv
void xNHFStateInit() {
    { // 0x00355F00 - 0x00355F2C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNHA.cpp
// ==========================================================================
class tagxState g_nhaStates[8]; // 0x0044D960
static class tagxStateLink link_nha_DodgeRightReady[1]; // 0x004A440C
static class tagxStateLink link_nha_DodgeLeftReady[1]; // 0x004A4408
static class tagxStateLink link_nha_DodgeRight[1]; // 0x004A4404
static class tagxStateLink link_nha_DodgeLeft[1]; // 0x004A4400
static class tagxStateLink link_nha_Run[2]; // 0x0044D958
static class tagxStateLink link_nha_Walk[2]; // 0x0044D950
static class tagxStateLink link_nha_StandRRun[4]; // 0x0044D940
static class tagxStateLink link_nha_Idle[4]; // 0x0044D930
unsigned char g_nhaJointsAll[17]; // 0x0044D910
// 0x00355F30 - 0x00355F5C (0x2C) xNHAStateReset__Fv
void xNHAStateReset() {
    { // 0x00355F30 - 0x00355F5C
    }
}

// 0x00355F60 - 0x00355F8C (0x2C) xNHAStateInit__Fv
void xNHAStateInit() {
    { // 0x00355F60 - 0x00355F8C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNGM.cpp
// ==========================================================================
class tagxState g_ngmStates[28]; // 0x0044DB80
static class tagxStateLink link_ngm_ComboDeathAxe[1]; // 0x004A4464
static class tagxStateLink link_ngm_ComboDeathGun[1]; // 0x004A4460
static class tagxStateLink link_ngm_ComboMissDeath[1]; // 0x004A445C
static class tagxStateLink link_ngm_ComboIdleChainsaw[3]; // 0x0044DB68
static class tagxStateLink link_ngm_ComboIdle[3]; // 0x0044DB58
static class tagxStateLink link_ngm_DeathRange4[1]; // 0x004A4458
static class tagxStateLink link_ngm_DeathRange3[1]; // 0x004A4454
static class tagxStateLink link_ngm_DeathRange2[1]; // 0x004A4450
static class tagxStateLink link_ngm_DeathRange1[1]; // 0x004A444C
static class tagxStateLink link_ngm_DeathMelee4[1]; // 0x004A4448
static class tagxStateLink link_ngm_DeathMelee3[1]; // 0x004A4444
static class tagxStateLink link_ngm_DeathMelee2[1]; // 0x004A4440
static class tagxStateLink link_ngm_DeathMelee1[1]; // 0x004A443C
static class tagxStateLink link_ngm_InjuryRangeThread[1]; // 0x004A4438
static class tagxStateLink link_ngm_InjuryMeleeThread[1]; // 0x004A4434
static class tagxStateLink link_ngm_InjuryRange[1]; // 0x004A4430
static class tagxStateLink link_ngm_InjuryMelee[1]; // 0x004A442C
static class tagxStateLink link_ngm_AttackMelee[1]; // 0x004A4428
static class tagxStateLink link_ngm_AttackRangeRight[1]; // 0x004A4424
static class tagxStateLink link_ngm_AttackRangeLeft[1]; // 0x004A4420
static class tagxStateLink link_ngm_AttackRangeFront[1]; // 0x004A441C
static class tagxStateLink link_ngm_DodgeRight[1]; // 0x004A4418
static class tagxStateLink link_ngm_DodgeLeft[1]; // 0x004A4414
static class tagxStateLink link_ngm_Run[13]; // 0x0044DB20
static class tagxStateLink link_ngm_Walk[13]; // 0x0044DAE0
static class tagxStateLink link_ngm_RRun[19]; // 0x0044DA90
static class tagxStateLink link_ngm_PreBirth[1]; // 0x004A4410
static class tagxStateLink link_ngm_Idle[20]; // 0x0044DA40
unsigned char g_ngmJointsUpperOnly[18]; // 0x00000000
unsigned char g_ngmJointsAll[18]; // 0x0044DA20
// 0x00355F90 - 0x00355FBC (0x2C) xNGMStateReset__Fv
void xNGMStateReset() {
    { // 0x00355F90 - 0x00355FBC
    }
}

// 0x00355FC0 - 0x00355FEC (0x2C) xNGMStateInit__Fv
void xNGMStateInit() {
    { // 0x00355FC0 - 0x00355FEC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNDD.cpp
// ==========================================================================
class tagxState g_nddStates[30]; // 0x0044DFF0
static class tagxStateLink link_ndd_ComboDeathAxe[1]; // 0x004A449C
static class tagxStateLink link_ndd_ComboDeathGun[1]; // 0x004A4498
static class tagxStateLink link_ndd_ComboMissDeath[1]; // 0x004A4494
static class tagxStateLink link_ndd_ComboIdleChainsaw[3]; // 0x0044DFE0
static class tagxStateLink link_ndd_ComboIdle[3]; // 0x0044DFD0
static class tagxStateLink link_ndd_DeathRange1[1]; // 0x004A4490
static class tagxStateLink link_ndd_DeathMelee2[1]; // 0x004A448C
static class tagxStateLink link_ndd_DeathMelee1[1]; // 0x004A4488
static class tagxStateLink link_ndd_InjuryFrontInWalk[1]; // 0x004A4484
static class tagxStateLink link_ndd_InjuryFrontFromAttackRRun[1]; // 0x004A4480
static class tagxStateLink link_ndd_InjuryFrontFromAttack[1]; // 0x004A447C
static class tagxStateLink link_ndd_InjuryFront[1]; // 0x004A4478
static class tagxStateLink link_ndd_AttackVomitFromRRun[3]; // 0x0044DFC0
static class tagxStateLink link_ndd_AttackStrongFromRRun[3]; // 0x0044DFB0
static class tagxStateLink link_ndd_AttackRightFromRRun[3]; // 0x0044DFA0
static class tagxStateLink link_ndd_AttackLeftFromRRun[3]; // 0x0044DF90
static class tagxStateLink link_ndd_AttackVomit[3]; // 0x0044DF80
static class tagxStateLink link_ndd_AttackStrong[3]; // 0x0044DF70
static class tagxStateLink link_ndd_AttackRight[3]; // 0x0044DF60
static class tagxStateLink link_ndd_AttackLeft[3]; // 0x0044DF50
static class tagxStateLink link_ndd_DodgeRight[2]; // 0x0044DF48
static class tagxStateLink link_ndd_DodgeLeft[2]; // 0x0044DF40
static class tagxStateLink link_ndd_Run[12]; // 0x0044DF10
static class tagxStateLink link_ndd_Walk[12]; // 0x0044DEE0
static class tagxStateLink link_ndd_Dive[1]; // 0x004A4474
static class tagxStateLink link_ndd_FlyThroughWindow[1]; // 0x004A4470
static class tagxStateLink link_ndd_PopUp[1]; // 0x004A446C
static class tagxStateLink link_ndd_RRun[20]; // 0x0044DE90
static class tagxStateLink link_ndd_Idle[20]; // 0x0044DE40
static class tagxStateLink link_ndd_PreBirth[1]; // 0x004A4468
unsigned char g_nddJointsUpperOnly[17]; // 0x00000000
unsigned char g_nddJointsAll[17]; // 0x0044DE20
// 0x00355FF0 - 0x0035601C (0x2C) xNDDStateReset__Fv
void xNDDStateReset() {
    { // 0x00355FF0 - 0x0035601C
    }
}

// 0x00356020 - 0x0035604C (0x2C) xNDDStateInit__Fv
void xNDDStateInit() {
    { // 0x00356020 - 0x0035604C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNCU.cpp
// ==========================================================================
class tagxState g_ncuStates[7]; // 0x0044E330
static class tagxStateLink link_ncu_InjuryStrong[1]; // 0x004A44B0
static class tagxStateLink link_ncu_Injury[1]; // 0x004A44AC
static class tagxStateLink link_ncu_AttackRangeShoot[1]; // 0x004A44A8
static class tagxStateLink link_ncu_AttackMeleeBite[1]; // 0x004A44A4
static class tagxStateLink link_ncu_AttackMeleeSwipe[1]; // 0x004A44A0
static class tagxStateLink link_ncu_Walk[3]; // 0x0044E318
static class tagxStateLink link_ncu_Idle[6]; // 0x0044E300
unsigned char g_ncuJointsUpperOnly[59]; // 0x00000000
unsigned char g_ncuJointsAll[59]; // 0x0044E2C0
// 0x00356050 - 0x0035607C (0x2C) xNCUStateReset__Fv
void xNCUStateReset() {
    { // 0x00356050 - 0x0035607C
    }
}

// 0x00356080 - 0x003560AC (0x2C) xNCUStateInit__Fv
void xNCUStateInit() {
    { // 0x00356080 - 0x003560AC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNCG.cpp
// ==========================================================================
class tagxState g_ncgStates[30]; // 0x0044E520
static class tagxStateLink link_ncg_ComboDeathAxe[1]; // 0x004A4510
static class tagxStateLink link_ncg_ComboDeathGun[1]; // 0x004A450C
static class tagxStateLink link_ncg_ComboMissDeath[1]; // 0x004A4508
static class tagxStateLink link_ncg_ComboIdleChainsaw[3]; // 0x0044E510
static class tagxStateLink link_ncg_ComboIdle[3]; // 0x0044E500
static class tagxStateLink link_ncg_DieRange[1]; // 0x004A4504
static class tagxStateLink link_ncg_DieMelee[1]; // 0x004A4500
static class tagxStateLink link_ncg_InjuryStrongUpperBodyThread[1]; // 0x004A44FC
static class tagxStateLink link_ncg_InjuryFrontUpperBodyThread[1]; // 0x004A44F8
static class tagxStateLink link_ncg_InjuryStrongIdleReady[1]; // 0x004A44F4
static class tagxStateLink link_ncg_InjuryFrontIdleReady[1]; // 0x004A44F0
static class tagxStateLink link_ncg_InjuryStrongIdle[1]; // 0x004A44EC
static class tagxStateLink link_ncg_InjuryFrontInComboIdle[1]; // 0x004A44E8
static class tagxStateLink link_ncg_InjuryFrontIdle[1]; // 0x004A44E4
static class tagxStateLink link_ncg_AttackMeleeStrongIdleReady[1]; // 0x004A44E0
static class tagxStateLink link_ncg_AttackMeleeStabIdleReady[1]; // 0x004A44DC
static class tagxStateLink link_ncg_AttackMeleeRightIdleReady[1]; // 0x004A44D8
static class tagxStateLink link_ncg_AttackMeleeLeftIdleReady[1]; // 0x004A44D4
static class tagxStateLink link_ncg_AttackMeleeStrongIdle[1]; // 0x004A44D0
static class tagxStateLink link_ncg_AttackMeleeStabIdle[1]; // 0x004A44CC
static class tagxStateLink link_ncg_AttackMeleeRightIdle[1]; // 0x004A44C8
static class tagxStateLink link_ncg_AttackMeleeLeftIdle[1]; // 0x004A44C4
static class tagxStateLink link_ncg_DodgeRightIdleReady[1]; // 0x004A44C0
static class tagxStateLink link_ncg_DodgeLeftIdleReady[1]; // 0x004A44BC
static class tagxStateLink link_ncg_DodgeRightIdle[1]; // 0x004A44B8
static class tagxStateLink link_ncg_DodgeLeftIdle[1]; // 0x004A44B4
static class tagxStateLink link_ncg_Run[12]; // 0x0044E4D0
static class tagxStateLink link_ncg_Walk[12]; // 0x0044E4A0
static class tagxStateLink link_ncg_IdleReady[19]; // 0x0044E450
static class tagxStateLink link_ncg_Idle[19]; // 0x0044E400
unsigned char g_ncgJointsUpperOnly[15]; // 0x0044E3E8
unsigned char g_ncgJointsAll[15]; // 0x0044E3D8
// 0x003560B0 - 0x003560DC (0x2C) xNCGStateReset__Fv
void xNCGStateReset() {
    { // 0x003560B0 - 0x003560DC
    }
}

// 0x003560E0 - 0x0035610C (0x2C) xNCGStateInit__Fv
void xNCGStateInit() {
    { // 0x003560E0 - 0x0035610C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNCB.cpp
// ==========================================================================
class tagxState g_ncbStates[30]; // 0x0044E910
static class tagxStateLink link_ncb_DeathRange4[1]; // 0x004A4578
static class tagxStateLink link_ncb_DeathRange3[1]; // 0x004A4574
static class tagxStateLink link_ncb_DeathRange2[1]; // 0x004A4570
static class tagxStateLink link_ncb_DeathRange1[1]; // 0x004A456C
static class tagxStateLink link_ncb_DeathMelee4[1]; // 0x004A4568
static class tagxStateLink link_ncb_DeathMelee3[1]; // 0x004A4564
static class tagxStateLink link_ncb_DeathMelee2[1]; // 0x004A4560
static class tagxStateLink link_ncb_DeathMelee1[1]; // 0x004A455C
static class tagxStateLink link_ncb_Injury2WhileMoving[1]; // 0x004A4558
static class tagxStateLink link_ncb_Injury1WhileMoving[1]; // 0x004A4554
static class tagxStateLink link_ncb_Injury2Ready[1]; // 0x004A4550
static class tagxStateLink link_ncb_Injury1Ready[1]; // 0x004A454C
static class tagxStateLink link_ncb_Injury2[1]; // 0x004A4548
static class tagxStateLink link_ncb_Injury1[1]; // 0x004A4544
static class tagxStateLink link_ncb_AttackSwipeReady[1]; // 0x004A4540
static class tagxStateLink link_ncb_AttackSpinReady[1]; // 0x004A453C
static class tagxStateLink link_ncb_AttackLeftReady[1]; // 0x004A4538
static class tagxStateLink link_ncb_AttackSwipe[1]; // 0x004A4534
static class tagxStateLink link_ncb_AttackSpin[1]; // 0x004A4530
static class tagxStateLink link_ncb_AttackLeft[1]; // 0x004A452C
static class tagxStateLink link_ncb_DodgeRightReady[1]; // 0x004A4528
static class tagxStateLink link_ncb_DodgeLeftReady[1]; // 0x004A4524
static class tagxStateLink link_ncb_DodgeRight[1]; // 0x004A4520
static class tagxStateLink link_ncb_DodgeLeft[1]; // 0x004A451C
static class tagxStateLink link_ncb_Run[12]; // 0x0044E8E0
static class tagxStateLink link_ncb_Walk[12]; // 0x0044E8B0
static class tagxStateLink link_ncb_StandRRun[17]; // 0x0044E860
static class tagxStateLink link_ncb_Idle[17]; // 0x0044E810
static class tagxStateLink link_ncb_PopUp[1]; // 0x004A4518
static class tagxStateLink link_ncb_PreBirth[1]; // 0x004A4514
unsigned char g_ncbJointsUpperOnly[16]; // 0x0044E800
unsigned char g_ncbJointsAll[16]; // 0x0044E7F0
// 0x00356110 - 0x0035613C (0x2C) xNCBStateReset__Fv
void xNCBStateReset() {
    { // 0x00356110 - 0x0035613C
    }
}

// 0x00356140 - 0x0035616C (0x2C) xNCBStateInit__Fv
void xNCBStateInit() {
    { // 0x00356140 - 0x0035616C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNBA.cpp
// ==========================================================================
class tagxState g_nbaStates[21]; // 0x0044ECB0
static class tagxStateLink link_nba_InjuryStrongUpperBodyThread[1]; // 0x004A45B8
static class tagxStateLink link_nba_InjuryFrontUpperBodyThread[1]; // 0x004A45B4
static class tagxStateLink link_nba_InjuryStrongFullBodyThread[1]; // 0x004A45B0
static class tagxStateLink link_nba_InjuryFrontFullBodyThread[1]; // 0x004A45AC
static class tagxStateLink link_nba_AttackStrongFromRRun[1]; // 0x004A45A8
static class tagxStateLink link_nba_AttackBothHandsFromRRun[1]; // 0x004A45A4
static class tagxStateLink link_nba_AttackBackhandFromRRun[1]; // 0x004A45A0
static class tagxStateLink link_nba_AttackStrongFromIdle[1]; // 0x004A459C
static class tagxStateLink link_nba_AttackBothHandsFromIdle[1]; // 0x004A4598
static class tagxStateLink link_nba_AttackBackhandFromIdle[1]; // 0x004A4594
static class tagxStateLink link_nba_DodgeRightFromRRun[1]; // 0x004A4590
static class tagxStateLink link_nba_DodgeLeftFromRRun[1]; // 0x004A458C
static class tagxStateLink link_nba_DodgeRightFromIdle[1]; // 0x004A4588
static class tagxStateLink link_nba_DodgeLeftFromIdle[1]; // 0x004A4584
static class tagxStateLink link_nba_Run[4]; // 0x0044ECA0
static class tagxStateLink link_nba_WalkBack[3]; // 0x0044EC90
static class tagxStateLink link_nba_Walk[4]; // 0x0044EC80
static class tagxStateLink link_nba_StandRRun[10]; // 0x0044EC50
static class tagxStateLink link_nba_Idle1FromRRun[1]; // 0x004A4580
static class tagxStateLink link_nba_Idle1FromIdle[1]; // 0x004A457C
static class tagxStateLink link_nba_Idle[11]; // 0x0044EC20
unsigned char g_nbaJointsUpperOnly[17]; // 0x0044EC00
unsigned char g_nbaJointsAll[17]; // 0x0044EBE0
// 0x00356170 - 0x0035619C (0x2C) xNBAStateReset__Fv
void xNBAStateReset() {
    { // 0x00356170 - 0x0035619C
    }
}

// 0x003561A0 - 0x003561CC (0x2C) xNBAStateInit__Fv
void xNBAStateInit() {
    { // 0x003561A0 - 0x003561CC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNAZ.cpp
// ==========================================================================
class tagxState g_nazStates[22]; // 0x0044EF80
static class tagxStateLink link_naz_InjuryRangeFrontNeck[1]; // 0x004A4600
static class tagxStateLink link_naz_InjuryMeleeDownNeck[1]; // 0x004A45FC
static class tagxStateLink link_naz_InjuryMeleeUpNeck[1]; // 0x004A45F8
static class tagxStateLink link_naz_InjuryMeleeRightNeck[1]; // 0x004A45F4
static class tagxStateLink link_naz_InjuryMeleeLeftNeck[1]; // 0x004A45F0
static class tagxStateLink link_naz_InjuryMeleeFrontNeck[1]; // 0x004A45EC
static class tagxStateLink link_naz_InjuryRangeFront[1]; // 0x004A45E8
static class tagxStateLink link_naz_InjuryMeleeFront[1]; // 0x004A45E4
static class tagxStateLink link_naz_AttackRangeButtNeck[1]; // 0x004A45E0
static class tagxStateLink link_naz_AttackRangeBiteNeck[1]; // 0x004A45DC
static class tagxStateLink link_naz_AttackRangeJolt[1]; // 0x004A45D8
static class tagxStateLink link_naz_AttackMeleeStab[1]; // 0x004A45D4
static class tagxStateLink link_naz_AttackMeleeSlash[1]; // 0x004A45D0
static class tagxStateLink link_naz_DodgeRightNeck[1]; // 0x004A45CC
static class tagxStateLink link_naz_DodgeLeftNeck[1]; // 0x004A45C8
static class tagxStateLink link_naz_DodgeRight[1]; // 0x004A45C4
static class tagxStateLink link_naz_DodgeLeft[1]; // 0x004A45C0
static class tagxStateLink link_naz_WalkNeck[7]; // 0x0044EF60
static class tagxStateLink link_naz_Walk[3]; // 0x0044EF50
static class tagxStateLink link_naz_IdleNeck[12]; // 0x0044EF20
static class tagxStateLink link_naz_RemoveKnife[1]; // 0x004A45BC
static class tagxStateLink link_naz_Idle[9]; // 0x0044EEF0
unsigned char g_nazJointsUpperOnly[57]; // 0x00000000
unsigned char g_nazJointsAll[57]; // 0x0044EEB0
// 0x003561D0 - 0x003561FC (0x2C) xNAZStateReset__Fv
void xNAZStateReset() {
    { // 0x003561D0 - 0x003561FC
    }
}

// 0x00356200 - 0x0035622C (0x2C) xNAZStateInit__Fv
void xNAZStateInit() {
    { // 0x00356200 - 0x0035622C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNavPnt.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x00356230 - 0x00356284 (0x54) xNavPointIsLinked__FP9xNavPointP9xNavPoint
signed int xNavPointIsLinked(class xNavPoint *np1, class xNavPoint *np2) {
    { // 0x00356230 - 0x00356284
        unsigned int i; // r6
    }
}

// 0x00356290 - 0x00356474 (0x1E4) xNavPointAStarGetNodeHeuristic__FP9xNavPointP9xNavPointP9xNavPoint
float xNavPointAStarGetNodeHeuristic(class xNavPoint *np, class xNavPoint *start, class xNavPoint *goal) {
    { // 0x00356290 - 0x00356474
        float cross; // 
        float dy2; // r29+0x70
        float dx2; // 
        float dy1; // 
        float dx1; // 
        float fudge; // 
        float i; // 
        float h; // 
    }
}

// 0x00356480 - 0x003564A8 (0x28) xNavPointSquareDistanceFromPoint__FP9xNavPointP8Vector3D
float xNavPointSquareDistanceFromPoint(class xNavPoint *np, class Vector3D *p) {
    { // 0x00356480 - 0x003564A8
    }
}

// 0x003564B0 - 0x00356638 (0x188) xNavPointPathfinderFreeLink__FP9xNavPointP9xNavPoint
void xNavPointPathfinderFreeLink(class xNavPoint *np, class xNavPoint *np2) {
    { // 0x003564B0 - 0x00356638
        unsigned int i; // r16
    }
}

// 0x00356640 - 0x003566B8 (0x78) xNavPointPathfinderOccupyLink__FP9xNavPointP9xNavPoint
void xNavPointPathfinderOccupyLink(class xNavPoint *np, class xNavPoint *np2) {
    { // 0x00356640 - 0x003566B8
        unsigned int i; // r7
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xNAB.cpp
// ==========================================================================
class tagxState g_nabStates[13]; // 0x0044F230
static class tagxStateLink link_nab_InjuryRange[1]; // 0x004A4624
static class tagxStateLink link_nab_InjuryMelee[1]; // 0x004A4620
static class tagxStateLink link_nab_AttackMeleeBackhand[1]; // 0x004A461C
static class tagxStateLink link_nab_AttackMeleeHammer3[1]; // 0x004A4618
static class tagxStateLink link_nab_AttackMeleeSaw[1]; // 0x004A4614
static class tagxStateLink link_nab_RotateCClkwise[2]; // 0x0044F220
static class tagxStateLink link_nab_RotateClkwise[2]; // 0x0044F218
static class tagxStateLink link_nab_BlockRight[1]; // 0x004A4610
static class tagxStateLink link_nab_BlockLeft[1]; // 0x004A460C
static class tagxStateLink link_nab_DodgeRight[1]; // 0x004A4608
static class tagxStateLink link_nab_DodgeLeft[1]; // 0x004A4604
static class tagxStateLink link_nab_Run[3]; // 0x0044F208
static class tagxStateLink link_nab_Idle[14]; // 0x0044F1D0
unsigned char g_nabJointsUpperOnly[57]; // 0x00000000
unsigned char g_nabJointsAll[57]; // 0x0044F190
// 0x003566C0 - 0x003566EC (0x2C) xNABStateReset__Fv
void xNABStateReset() {
    { // 0x003566C0 - 0x003566EC
    }
}

// 0x003566F0 - 0x0035671C (0x2C) xNABStateInit__Fv
void xNABStateInit() {
    { // 0x003566F0 - 0x0035671C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xModel.cpp
// ==========================================================================
class _xModel g_RTObjModel[110]; // 0x004E0080
signed int g_RTObjModelCount; // 0x004A4D34
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _xModel g_DNPCModel[5]; // 0x004E0440
class _xModel g_SNPCModel[5]; // 0x004E0410
class _xModel g_BossModel[2]; // 0x004E0400
class _xModel g_AshModel[1]; // 0x004E03F0
signed int g_DNPCModelCount; // 0x004A4D44
signed int g_SNPCModelCount; // 0x004A4D40
signed int g_BossModelCount; // 0x004A4D3C
signed int g_AshModelCount; // 0x004A4D38
// 0x00356720 - 0x00356838 (0x118) xModelInit__Fv
void xModelInit() {
    { // 0x00356720 - 0x00356838
        signed int i; // r3
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xMessMgr.cpp
// ==========================================================================
void (xMessageDistributeMessageFromScripts)(signed int, signed int, unsigned int, signed int, signed int, unsigned int, unsigned int, unsigned int); // 0x003570C0
void (xMessageRegisterInterestFromScripts)(unsigned int, unsigned int, signed int, signed int, signed int, signed int, signed int); // 0x003576A0
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagxActorDBData *gadb; // 0x004A4658
static unsigned int interpreterMethod; // 0x004A4D64
static class xMessage debugMessagesSent[75]; // 0x004E1630
static signed int debugMessagesSentCount; // 0x004A4D60
signed int gMessageTrafficCount; // 0x004A4D5C
static unsigned int maxRedoCount; // 0x004A4D58
static unsigned int redoCount; // 0x004A4D54
static class MessageRedo redoList[10]; // 0x004E1420
static unsigned int maxWaitingCount; // 0x004A4D50
static unsigned int waitingCount; // 0x004A4D4C
static class xMessage waitingList[50]; // 0x004E0B80
unsigned int registrationCount; // 0x004A4D48
static class MessageRegistration registrationList[75]; // 0x004E0470
// 0x00356840 - 0x00356848 (0x8) xMessageDebug__Fi
void xMessageDebug() {
    { // 0x00356840 - 0x00356848
    }
}

// 0x00356850 - 0x00356B5C (0x30C) xMessagePrivateProcessMessage__FP8xMessage
static void xMessagePrivateProcessMessage(class xMessage *cMessage) {
    { // 0x00356850 - 0x00356B5C
        unsigned int i; // r18
        signed int status; // r2
        class MessageRegistration *cRegistration; // r17
        unsigned int j; // r16
    }
}

// 0x00356B60 - 0x00356E38 (0x2D8) xMessageProcessAllWaiting__Fv
void xMessageProcessAllWaiting() {
    { // 0x00356B60 - 0x00356E38
        unsigned int stillWaiting; // r18
        class MessageRedo *rMessage; // r17
        class xMessage *cMessage; // r17
        unsigned int i; // r16
    }
}

// 0x00356E40 - 0x003570B4 (0x274) xMessageAddRedoMessage__FPC8xMessageUiUi
static void xMessageAddRedoMessage(class xMessage *theMessage, unsigned int type, unsigned int id) {
    { // 0x00356E40 - 0x003570B4
        signed int i; // r3
    }
}

// 0x003570C0 - 0x003573B0 (0x2F0) xMessageDistributeMessageFromScripts__FiiUiiiUiUiUi
void xMessageDistributeMessageFromScripts(signed int st, signed int si, unsigned int m, signed int rt, signed int ri, unsigned int d1, unsigned int d2, unsigned int d3) {
    { // 0x003570C0 - 0x003573B0
        signed int i; // r3
        class xMessage aMessage; // r29+0x20
    }
}

// 0x003573B0 - 0x0035769C (0x2EC) xMessageDistributeMessage__FPC8xMessage
void xMessageDistributeMessage(class xMessage *theMessage) {
    { // 0x003573B0 - 0x0035769C
        signed int i; // r3
    }
}

// 0x003576A0 - 0x00357A48 (0x3A8) xMessageRegisterInterestFromScripts__FUiUiiiiii
void xMessageRegisterInterestFromScripts(unsigned int myType, unsigned int myId, signed int sType, signed int sId, signed int mess) {
    { // 0x003576A0 - 0x00357A48
        unsigned int i; // r3
    }
}

// 0x00357A50 - 0x00357A5C (0xC) xMessageReInitializeInterpreter__Fv
void xMessageReInitializeInterpreter() {
    { // 0x00357A50 - 0x00357A5C
    }
}

// 0x00357A60 - 0x00357A70 (0x10) xMessageSoftInit__Fv
void xMessageSoftInit() {
    { // 0x00357A60 - 0x00357A70
    }
}

// 0x00357A70 - 0x00357ACC (0x5C) xMessageInit__Fv
void xMessageInit() {
    { // 0x00357A70 - 0x00357ACC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xMemMgr.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static signed int currentIdSize; // 0x004A4DC8
static signed int lastId; // 0x004A4628
static unsigned int l_debugStaticLevelResetBlockUsed; // 0x004A4DC4
static unsigned int l_debugPreloadBlockMapUsed; // 0x004A4DC0
static unsigned int *l_debugPreloadBlockMap[20000]; // 0x00A846E0
static unsigned int l_debugLevelBlockMapUsed; // 0x004A4DBC
static unsigned int *l_debugLevelBlockMap[20000]; // 0x00A70E60
static unsigned int l_debugStaticBlockMapUsed; // 0x004A4DB8
static unsigned int *l_debugStaticBlockMap[20000]; // 0x00A5D5E0
unsigned char *l_levelResetStaticPointer; // 0x004A4DB4
unsigned int l_levelResetStaticUsed; // 0x004A4DB0
static unsigned int xMemCurrentPreloadPointerLocked; // 0x004A4DAC
static unsigned char *xMemCurrentPreloadPointer; // 0x004A4DA8
static unsigned int xMemCurrentLevelPointerLocked; // 0x004A4DA4
static unsigned char *xMemCurrentLevelPointer; // 0x004A4DA0
static unsigned int xMemCurrentStaticPointerLocked; // 0x004A4D9C
static unsigned char *xMemCurrentStaticPointer; // 0x004A4D98
static unsigned int l_currentTempUsed; // 0x004A4D94
static unsigned int l_currentPreloadUsed; // 0x004A4D90
static unsigned int l_currentLevelUsed; // 0x004A4D8C
unsigned int l_currentStaticUsed; // 0x004A4D88
static unsigned int l_preloadPoolSize; // 0x004A4D84
static unsigned int l_levelPoolSize; // 0x004A4D80
static unsigned int l_staticPoolSize; // 0x004A4D7C
static signed int l_tempIsLocked; // 0x004A4D78
static signed int l_amInitialized; // 0x004A4D74
static unsigned char *l_preloadPool; // 0x004A4D70
static unsigned char *l_levelPool; // 0x004A4D6C
static unsigned char *l_staticPool; // 0x004A4D68
static unsigned int l_preloadPoolBuffer[20256]; // 0x00A49960
static unsigned int l_levelPoolBuffer[20256]; // 0x00A35CE0
static unsigned int l_staticPoolBuffer[1396334]; // 0x004E2320
// 0x00357AD0 - 0x00357DA4 (0x2D4) MEMCheckIntegrity__Fv
void MEMCheckIntegrity() {
    { // 0x00357AD0 - 0x00357DA4
        unsigned int i; // r16
    }
}

// 0x00357DB0 - 0x00357E4C (0x9C) memRecordRequest__FUiUiUiPCcUi
void memRecordRequest(unsigned int id, unsigned int count, unsigned int size, char *file, unsigned int line) {
    { // 0x00357DB0 - 0x00357E4C
    }
}

// 0x00357E50 - 0x00357FC8 (0x178) MEMUnlockTempMemory__Fv
void MEMUnlockTempMemory() {
    { // 0x00357E50 - 0x00357FC8
    }
}

// 0x00357FD0 - 0x00358288 (0x2B8) MEMLockTempMemory__FUiUiPCcUi
void *MEMLockTempMemory(unsigned int count, unsigned int size) {
    { // 0x00357FD0 - 0x00358288
        signed int over; // r2
        signed int offset; // r2
        unsigned char *base; // r3
        unsigned int sizeToAlloc; // r16
        void *retVal; // r2
    }
}

// 0x00358290 - 0x003582E0 (0x50) MEMStaticMemoryLevelReset__Fv
unsigned char *MEMStaticMemoryLevelReset() {
    { // 0x00358290 - 0x003582E0
        unsigned int *end; // r4
        unsigned int *clear; // r3
    }
}

// 0x003582E0 - 0x00358300 (0x20) MEMStaticMemorySetLevelReset__Fv
void MEMStaticMemorySetLevelReset() {
    { // 0x003582E0 - 0x00358300
    }
}

// 0x00358300 - 0x00358614 (0x314) MEMGetStaticMemory__FUiUiUiPCcUi
void *MEMGetStaticMemory(unsigned int count, unsigned int size, char *file, unsigned int line) {
    { // 0x00358300 - 0x00358614
        unsigned int i; // r4
        unsigned char *p; // r3
        signed int total; // r17
        void *retValue; // r16
    }
}

// 0x00358620 - 0x00358668 (0x48) MEMReset__Fv
signed short MEMReset() {
    { // 0x00358620 - 0x00358668
    }
}

// 0x00358670 - 0x00358704 (0x94) MEMInit__Fv
signed short MEMInit() {
    { // 0x00358670 - 0x00358704
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xMain.cpp
// ==========================================================================
signed int gBootState; // 0x004A4E20
signed int jumpBuffer[11]; // 0x00B06DD0
signed int debugOpFlag; // 0x004A4FAC
signed int debugCallFlag; // 0x004A4684
signed int debugThread; // 0x004A4688
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
unsigned int g_scenePendingScene; // 0x004A4CD0
signed int gGameState; // 0x004A4640
unsigned int gTime; // 0x004A4E10
unsigned long gTimerValue; // 0x004A5340
class PerfTimes g_perfTimes; // 0x00AA6AA0
class xScene *xScene_current; // 0x004A4E08
signed int goIntoStartMenu; // 0x004A4634
signed int veryFirstTime; // 0x004A4630
static unsigned int interpreterMethodInterface; // 0x004A4DD4
static unsigned int interpreterMethod; // 0x004A4DD0
unsigned int dg_colors[5]; // 0x0044F370
class _XDG dg[5]; // 0x00A9B7A0
unsigned int gDebugGraph; // 0x004A462C
unsigned char gCartBuffer[14400]; // 0x00A97F60
signed int jmpret; // 0x004A4DCC
// 0x00358710 - 0x00358718 (0x8) xShutdown__Fv
void xShutdown() {
    { // 0x00358710 - 0x00358718
    }
}

// 0x00358720 - 0x00358B78 (0x458) xIdle__Fv
void xIdle() {
    { // 0x00358720 - 0x00358B78
        static signed int count; // 0x004A4DD8
        static signed char init; // 0x004A4DDC
    }
}

// 0x00358B80 - 0x00359098 (0x518) xInitGame__Fi
void xInitGame() {
    { // 0x00358B80 - 0x00359098
        class xMessage messageToSend; // r29+0xb0
        char *assetName; // r17
        signed int tick; // r16
        float mult; // 
        unsigned int lastId; // r2
        unsigned int sceneId; // r17
        signed int sceneNumber; // r16
        class XStream stream; // r29+0x80
    }
}

// 0x003590A0 - 0x00359110 (0x70) xMainReInitializeInterpreter__Fv
void xMainReInitializeInterpreter() {
    { // 0x003590A0 - 0x00359110
    }
}

// 0x00359110 - 0x00359260 (0x150) xInit__Fv
void xInit() {
    { // 0x00359110 - 0x00359260
        char tmp[128]; // r29+0x10
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xList.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static class xListItem gListArray[2000]; // 0x00A9BA40
static class xListItem *m_dead; // 0x004A4DE0
// 0x00359260 - 0x003592A4 (0x44) xListFind__FP5xListPv
void *xListFind(class xList *l, void *item) {
    { // 0x00359260 - 0x003592A4
        class xListItem *idx; // r3
    }
}

// 0x003592B0 - 0x00359848 (0x598) xListPop__FP5xListPv
void *xListPop(class xList *l, void *item) {
    { // 0x003592B0 - 0x00359848
        class xListItem *idx; // r16
    }
}

// 0x00359850 - 0x00359968 (0x118) xListPopAll__FP5xList
void xListPopAll(class xList *l) {
    { // 0x00359850 - 0x00359968
        void *item; // r2
    }
}

// 0x00359970 - 0x00359B20 (0x1B0) xListAppendList__FP5xListP5xList
void xListAppendList(class xList *a, class xList *b) {
    { // 0x00359970 - 0x00359B20
    }
}

// 0x00359B20 - 0x00359E70 (0x350) xListPush__FP5xListPv
signed int xListPush(class xList *l, void *item) {
    { // 0x00359B20 - 0x00359E70
        class xListItem *li; // r2
    }
}

// 0x00359E70 - 0x00359EAC (0x3C) xListCountActive__FP5xList
signed int xListCountActive(class xList *l) {
    { // 0x00359E70 - 0x00359EAC
        class xListItem *item; // r3
        signed int count; // r2
    }
}

// 0x00359EB0 - 0x0035A000 (0x150) xListInit__FP5xList
void xListInit(class xList *l) {
    { // 0x00359EB0 - 0x0035A000
        class xListItem *item; // r17
        signed int i; // r16
    }
}

// 0x0035A000 - 0x0035A118 (0x118) xListDispose__FP5xList
void xListDispose(class xList *l) {
    { // 0x0035A000 - 0x0035A118
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xLight.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x0035A120 - 0x0035A128 (0x8) xLightIdle__FP6xLight
void xLightIdle() {
    { // 0x0035A120 - 0x0035A128
    }
}

// 0x0035A130 - 0x0035A2A0 (0x170) xLightInitRuntime__FP6xLight
void xLightInitRuntime(class xLight *l) {
    { // 0x0035A130 - 0x0035A2A0
        class iLightResource *rsrc; // r2
        class _iLightRuntime *r; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xGui.cpp
// ==========================================================================
void (iGuiItemRenderFunc_Image)(class _xGuiItem *, class _xGuiSys *); // 0x00386530
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _xGuiSys *gActiveGuiSys; // 0x004A4DF0
class _xGuiSys gGuiSys[52]; // 0x00AA5B80
class _xGuiItem gGuiItem[180]; // 0x00AA1800
float gZOffsetDelta; // 0x004A4638
float gZOffset; // 0x004A4DEC
unsigned char gFX1Dir; // 0x004A4DE8
signed short gFX1Color; // 0x004A4DE4
// 0x0035A2A0 - 0x0035A368 (0xC8) xGuiSysGetFlag__FP8_xGuiSys
signed int xGuiSysGetFlag(class _xGuiSys *sys) {
    { // 0x0035A2A0 - 0x0035A368
    }
}

// 0x0035A370 - 0x0035A444 (0xD4) xGuiSysSetFlag__FP8_xGuiSysi
void xGuiSysSetFlag(class _xGuiSys *sys, signed int flag) {
    { // 0x0035A370 - 0x0035A444
    }
}

// 0x0035A450 - 0x0035A52C (0xDC) xGuiSysAddFlag__FP8_xGuiSysi
void xGuiSysAddFlag(class _xGuiSys *sys, signed int flag) {
    { // 0x0035A450 - 0x0035A52C
    }
}

// 0x0035A530 - 0x0035A61C (0xEC) xGuiSysRemoveFlag__FP8_xGuiSysi
void xGuiSysRemoveFlag(class _xGuiSys *sys, signed int flag) {
    { // 0x0035A530 - 0x0035A61C
    }
}

// 0x0035A620 - 0x0035A6E8 (0xC8) xGuiItemGetFlag__FP9_xGuiItem
signed int xGuiItemGetFlag(class _xGuiItem *item) {
    { // 0x0035A620 - 0x0035A6E8
    }
}

// 0x0035A6F0 - 0x0035A7C4 (0xD4) xGuiItemSetFlag__FP9_xGuiItemi
void xGuiItemSetFlag(class _xGuiItem *item, signed int flag) {
    { // 0x0035A6F0 - 0x0035A7C4
    }
}

// 0x0035A7D0 - 0x0035A8AC (0xDC) xGuiItemAddFlag__FP9_xGuiItemi
void xGuiItemAddFlag(class _xGuiItem *item, signed int flag) {
    { // 0x0035A7D0 - 0x0035A8AC
    }
}

// 0x0035A8B0 - 0x0035A99C (0xEC) xGuiItemRemoveFlag__FP9_xGuiItemi
void xGuiItemRemoveFlag(class _xGuiItem *item, signed int flag) {
    { // 0x0035A8B0 - 0x0035A99C
    }
}

// 0x0035A9A0 - 0x0035AA78 (0xD8) xGuiItemSetDataID__FP9_xGuiItemi
void xGuiItemSetDataID(class _xGuiItem *item, signed int id) {
    { // 0x0035A9A0 - 0x0035AA78
    }
}

// 0x0035AA80 - 0x0035AC20 (0x1A0) xGuiItemLink__FP9_xGuiItemiP9_xGuiItemi
void xGuiItemLink(class _xGuiItem *a, signed int dira, class _xGuiItem *b, signed int dirb) {
    { // 0x0035AA80 - 0x0035AC20
    }
}

// 0x0035AC20 - 0x0035AEF4 (0x2D4) xGuiItemCreate__FUiP8_xGuiSys
class _xGuiItem *xGuiItemCreate(unsigned int flag, class _xGuiSys *sys) {
    { // 0x0035AC20 - 0x0035AEF4
        class _xGuiItem *t; // r3
        class _xGuiItem *g; // r16
        signed int j; // r3
        signed int i; // r4
    }
}

// 0x0035AF00 - 0x0035AFD4 (0xD4) xGuiItemSetRenderFunc__FP9_xGuiItemPv
void xGuiItemSetRenderFunc(class _xGuiItem *i, void *func) {
    { // 0x0035AF00 - 0x0035AFD4
    }
}

// 0x0035AFE0 - 0x0035B180 (0x1A0) xGuiSysLink__FP8_xGuiSysiP8_xGuiSysi
void xGuiSysLink(class _xGuiSys *a, signed int dira, class _xGuiSys *b, signed int dirb) {
    { // 0x0035AFE0 - 0x0035B180
    }
}

// 0x0035B180 - 0x0035B254 (0xD4) xGuiSysSetRenderFunc__FP8_xGuiSysPv
void xGuiSysSetRenderFunc(class _xGuiSys *s, void *func) {
    { // 0x0035B180 - 0x0035B254
    }
}

// 0x0035B260 - 0x0035B318 (0xB8) xGuiSysCreate__FUi
class _xGuiSys *xGuiSysCreate(unsigned int flag) {
    { // 0x0035B260 - 0x0035B318
        class _xGuiSys *s; // r2
        signed int j; // r4
        signed int i; // r5
    }
}

// 0x0035B320 - 0x0035B490 (0x170) xGuiSysSet__FP8_xGuiSys
void xGuiSysSet(class _xGuiSys *sys) {
    { // 0x0035B320 - 0x0035B490
        class _xGuiItem *item; // r2
        class _xGuiSys *oldSys; // r17
    }
}

// 0x0035B490 - 0x0035B498 (0x8) xGuiSysSetDim__FP8_xGuiSysiii
void xGuiSysSetDim() {
    { // 0x0035B490 - 0x0035B498
    }
}

// 0x0035B4A0 - 0x0035B4A8 (0x8) xGuiSysAdjustPos__FP8_xGuiSysiii
void xGuiSysAdjustPos() {
    { // 0x0035B4A0 - 0x0035B4A8
    }
}

// 0x0035B4B0 - 0x0035B4B8 (0x8) xGuiSysSetPos__FP8_xGuiSysiii
void xGuiSysSetPos() {
    { // 0x0035B4B0 - 0x0035B4B8
    }
}

// 0x0035B4C0 - 0x0035B758 (0x298) xGuiSysFuncCall__FP8_xGuiSysi
void xGuiSysFuncCall(class _xGuiSys *sys, signed int func) {
    { // 0x0035B4C0 - 0x0035B758
    }
}

// 0x0035B760 - 0x0035BB3C (0x3DC) xGuiItemFuncCall__FP8_xGuiSysP9_xGuiItemi
void xGuiItemFuncCall(class _xGuiSys *sys, class _xGuiItem *item, signed int func) {
    { // 0x0035B760 - 0x0035BB3C
    }
}

// 0x0035BB40 - 0x0035BCB0 (0x170) xGuiItemAdvanceText__FP9_xGuiItemi
void xGuiItemAdvanceText(class _xGuiItem *item) {
    { // 0x0035BB40 - 0x0035BCB0
    }
}

// 0x0035BCB0 - 0x0035C010 (0x360) xGuiItemSelect__FP8_xGuiSysP9_xGuiItem
void xGuiItemSelect(class _xGuiSys *sys, class _xGuiItem *item) {
    { // 0x0035BCB0 - 0x0035C010
    }
}

// 0x0035C010 - 0x0035C2F0 (0x2E0) xGuiSelectItemDir__FP8_xGuiSysi
void xGuiSelectItemDir(class _xGuiSys *sys, signed int dir) {
    { // 0x0035C010 - 0x0035C2F0
        class _xGuiItem *tmp; // r2
    }
}

// 0x0035C2F0 - 0x0035C2F8 (0x8) xGuiItemAdjustPos__FP9_xGuiItemiii
void xGuiItemAdjustPos() {
    { // 0x0035C2F0 - 0x0035C2F8
    }
}

// 0x0035C300 - 0x0035C410 (0x110) xGuiItemSetColor__FP9_xGuiItemiiii
void xGuiItemSetColor(class _xGuiItem *item, signed int r, signed int g, signed int b, signed int a) {
    { // 0x0035C300 - 0x0035C410
    }
}

// 0x0035C410 - 0x0035C418 (0x8) xGuiItemSetPos__FP9_xGuiItemiii
void xGuiItemSetPos() {
    { // 0x0035C410 - 0x0035C418
    }
}

// 0x0035C420 - 0x0035C428 (0x8) xGuiIdle__FUl
void xGuiIdle() {
    { // 0x0035C420 - 0x0035C428
    }
}

// 0x0035C430 - 0x0035C710 (0x2E0) xGuiDebug__Fi
void xGuiDebug(signed int mode) {
    { // 0x0035C430 - 0x0035C710
        class _xGuiSys *s; // r17
        signed int i; // r16
    }
}

// 0x0035C710 - 0x0035C818 (0x108) xGuiDraw__Fv
void xGuiDraw() {
    { // 0x0035C710 - 0x0035C818
        class _xGuiSys *s; // r2
        signed int i; // r16
    }
}

// 0x0035C820 - 0x0035C828 (0x8) xGuiSetLevelReset__Fv
void xGuiSetLevelReset() {
    { // 0x0035C820 - 0x0035C828
    }
}

// 0x0035C830 - 0x0035CC80 (0x450) xGuiInit__Fv
void xGuiInit() {
    { // 0x0035C830 - 0x0035CC80
        signed int j; // r3
        signed int i; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xGState.cpp
// ==========================================================================
void (xScriptGStateChange)(signed int); // 0x0035CC80
signed int gQuit; // 0x004A4F30
signed int (xPadMapperFunc_Player)(class tagxPad *, class tagxPadAction *, signed int); // 0x0033FD60
void (xPadActionFunc_Player)(class tagxPad *, signed int, class tagxPadAction *); // 0x0033FD50
void (xPadActionFunc_GuiMenu)(class tagxPad *, signed int, class tagxPadAction *); // 0x003A75B0
signed int (xPadMapperFunc_GuiMenu)(class tagxPad *, class tagxPadAction *, signed int); // 0x003A7160
signed int gPlayerType; // 0x004A4E00
unsigned long gTimerValue; // 0x004A5340
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int gGameState; // 0x004A4640
float gTimeSec; // 0x004A4C8C
static unsigned int gSavedTimeSec; // 0x004A4DF8
static unsigned int interpreterMethod; // 0x004A4DF4
signed int doStateChange; // 0x004A463C
// 0x0035CC80 - 0x0035CC8C (0xC) xScriptGStateChange__Fi
void xScriptGStateChange(signed int toState) {
    { // 0x0035CC80 - 0x0035CC8C
    }
}

// 0x0035CC90 - 0x0035CE94 (0x204) xGStateChange__Fi
void xGStateChange(signed int toState) {
    { // 0x0035CC90 - 0x0035CE94
        static signed int timeWhenWeEnteredInventory; // 0x004A4DFC
    }
}

// 0x0035CEA0 - 0x0035D0C4 (0x224) xGStateSet__Fi
void xGStateSet(signed int toState) {
    { // 0x0035CEA0 - 0x0035D0C4
    }
}

// 0x0035D0D0 - 0x0035D10C (0x3C) xGStateIdle__Fv
void xGStateIdle() {
    { // 0x0035D0D0 - 0x0035D10C
    }
}

// 0x0035D110 - 0x0035D11C (0xC) xGStateReInitializeInterpreter__Fv
void xGStateReInitializeInterpreter() {
    { // 0x0035D110 - 0x0035D11C
    }
}

// 0x0035D120 - 0x0035D154 (0x34) xGStateInit__Fv
void xGStateInit() {
    { // 0x0035D120 - 0x0035D154
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xGlobals.cpp
// ==========================================================================
signed int gBootState; // 0x004A4E20
unsigned int g_currentPlayerRotateSpeed; // 0x004A4648
unsigned int g_maxPlayerRotateSpeed; // 0x004A4644
signed int gGameState; // 0x004A4640
signed int gDebugShadowMode; // 0x00000000
signed int gDebugMaskMode; // 0x00000000
signed int gDebugDrawDebugInfo; // 0x004A4E1C
unsigned int gTimeDelta; // 0x004A4E18
unsigned int gTimeLastFrame; // 0x004A4E14
unsigned int gTime; // 0x004A4E10
class xScene *xScene_preload; // 0x004A4E0C
class xScene *xScene_current; // 0x004A4E08
class xScene *xScene_sceneBuffer[2]; // 0x00AA67F0
class tag_xCameraControl gCamera; // 0x00AA67C0
class xPlate gPlate; // 0x00AA67B0
signed int gPlayerID; // 0x004A4E04
signed int gPlayerType; // 0x004A4E00

// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xFont.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static char *blankText; // 0x004A464C
unsigned int dpy; // 0x004A4E30
unsigned int dpx; // 0x004A4E2C
unsigned int draw; // 0x004A4E28
unsigned int dbFont; // 0x004A4E24
class xFontBox_t BoxList[3]; // 0x00AA69D0
class xFontInfo_t FontList[3]; // 0x00AA6800
// 0x0035D160 - 0x0035D300 (0x1A0) xFontBoxSetBackdrop__FP10xFontBox_tUiUi
void xFontBoxSetBackdrop(class xFontBox_t *box, unsigned int color, unsigned int opaque) {
    { // 0x0035D160 - 0x0035D300
    }
}

// 0x0035D300 - 0x0035D4B4 (0x1B4) xFontBoxSetJustification__FP10xFontBox_tUi
void xFontBoxSetJustification(class xFontBox_t *box, unsigned int mode) {
    { // 0x0035D300 - 0x0035D4B4
    }
}

// 0x0035D4C0 - 0x0035D6AC (0x1EC) xFontBoxSetInset__FP10xFontBox_tffff
void xFontBoxSetInset(class xFontBox_t *box, float inset_left, float inset_right, float inset_top, float inset_bottom) {
    { // 0x0035D4C0 - 0x0035D6AC
    }
}

// 0x0035D6B0 - 0x0035D83C (0x18C) xFontBoxSetSpacing__FP10xFontBox_tff
void xFontBoxSetSpacing(class xFontBox_t *box, float x_spacing, float y_spacing) {
    { // 0x0035D6B0 - 0x0035D83C
    }
}

// 0x0035D840 - 0x0035D9C0 (0x180) xFontBoxSetColor__FP10xFontBox_tUi
void xFontBoxSetColor(class xFontBox_t *box, unsigned int color) {
    { // 0x0035D840 - 0x0035D9C0
    }
}

// 0x0035D9C0 - 0x0035DB4C (0x18C) xFontBoxSetText__FP10xFontBox_tPc
void xFontBoxSetText(class xFontBox_t *box, char *text) {
    { // 0x0035D9C0 - 0x0035DB4C
    }
}

// 0x0035DB50 - 0x0035DCC0 (0x170) xFontBoxDestroy__FP10xFontBox_t
void xFontBoxDestroy(class xFontBox_t *box) {
    { // 0x0035DB50 - 0x0035DCC0
    }
}

// 0x0035DCC0 - 0x0035DF08 (0x248) xFontBoxRender__FP10xFontBox_t
char *xFontBoxRender(class xFontBox_t *box) {
    { // 0x0035DCC0 - 0x0035DF08
        unsigned int maxy; // r19
        unsigned int maxx; // r18
        unsigned int miny; // r16
        unsigned int minx; // r17
        unsigned int maxheight; // r2
        unsigned int maxwidth; // r2
        char *t; // r2
        char *s; // r16
    }
}

// 0x0035DF10 - 0x0035E458 (0x548) JustifyLine__FP10xFontBox_tPcUiUi
static char *JustifyLine(class xFontBox_t *box, char *text, unsigned int maxwidth, unsigned int maxheight) {
    { // 0x0035DF10 - 0x0035E458
        char *end; // r20
        char *t; // r2
        char *s; // r19
        unsigned int testheight; // r29+0xcc
        unsigned int testwidth; // r29+0xc8
        unsigned int whitespace_width; // r18
        unsigned int height; // r17
        unsigned int width; // r16
    }
}

// 0x0035E460 - 0x0035EBC0 (0x760) ProcessWord__FP10xFontBox_tPcPUiPUi
static char *ProcessWord(class xFontBox_t *box, char *text, unsigned int *width, unsigned int *height) {
    { // 0x0035E460 - 0x0035EBC0
        class xFontInfo_t *font; // r2
        char c; // r3
        unsigned int b; // r2
        unsigned int g; // r2
        unsigned int r; // r2
        unsigned int color; // r18
        unsigned int fontID; // r2
        char *s; // r16
    }
}

// 0x0035EBC0 - 0x0035EDFC (0x23C) xFontBoxCreate__FffffUiUi
class xFontBox_t *xFontBoxCreate(float x, float y, float w, float h, unsigned int id, unsigned int color) {
    { // 0x0035EBC0 - 0x0035EDFC
        class xFontInfo_t *font; // r4
        class xFontBox_t *box; // r16
        unsigned int i; // r3
    }
}

// 0x0035EE00 - 0x0035EE6C (0x6C) xFontRender__Fv
void xFontRender() {
    { // 0x0035EE00 - 0x0035EE6C
        class xFontBox_t *box; // r17
        unsigned int i; // r16
    }
}

// 0x0035EE70 - 0x0035EEE4 (0x74) xFontExit__Fv
void xFontExit() {
    { // 0x0035EE70 - 0x0035EEE4
        class xFontInfo_t *font; // r17
        unsigned int i; // r16
    }
}

// 0x0035EEF0 - 0x0035F0A0 (0x1B0) xFontInit__Fv
void xFontInit() {
    { // 0x0035EEF0 - 0x0035F0A0
        unsigned int i; // r5
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xFMV.cpp
// ==========================================================================
signed int (xPadMapperFunc_FmvMenu)(class tagxPad *, class tagxPadAction *, signed int); // 0x0035F0A0
void (xPadActionFunc_FmvMenu)(class tagxPad *, signed int, class tagxPadAction *); // 0x0035F4E0
signed int g_sceneSwapPending; // 0x004A4CCC
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static signed int sAudioFlags; // 0x004A4E44
static signed int sAudioAssetID; // 0x004A4E40
unsigned char gFMVPlaying; // 0x004A4E3C
unsigned char gAudioLoop; // 0x004A4E38
unsigned char gAudioStatus; // 0x004A4E34
// 0x0035F0A0 - 0x0035F4DC (0x43C) xPadMapperFunc_FmvMenu__FP7tagxPadP13tagxPadActioni
signed int xPadMapperFunc_FmvMenu(class tagxPad *pad, class tagxPadAction *actions, signed int maxActions) {
    { // 0x0035F0A0 - 0x0035F4DC
        class tagxPadAction *a; // r5
        class _XBUTTON *b; // r4
        signed int i; // r3
        signed int actionCount; // r2
    }
}

// 0x0035F4E0 - 0x0035F4E8 (0x8) xPadActionFunc_FmvMenu__FP7tagxPadiP13tagxPadAction
void xPadActionFunc_FmvMenu() {
    { // 0x0035F4E0 - 0x0035F4E8
    }
}

// 0x0035F4F0 - 0x0035F5A8 (0xB8) xFMVAudioName__FUiPc
void xFMVAudioName(char *s) {
    { // 0x0035F4F0 - 0x0035F5A8
    }
}

// 0x0035F5B0 - 0x0035F5C0 (0x10) xFMVResumeAudio__Fv
void xFMVResumeAudio() {
    { // 0x0035F5B0 - 0x0035F5C0
    }
}

// 0x0035F5C0 - 0x0035F5D0 (0x10) xFMVPauseAudio__Fv
void xFMVPauseAudio() {
    { // 0x0035F5C0 - 0x0035F5D0
    }
}

// 0x0035F5D0 - 0x0035F5F8 (0x28) xFMVStopAudio__Fv
void xFMVStopAudio() {
    { // 0x0035F5D0 - 0x0035F5F8
    }
}

// 0x0035F600 - 0x0035F620 (0x20) xFMVIdle__Fv
unsigned int xFMVIdle() {
    { // 0x0035F600 - 0x0035F620
    }
}

// 0x0035F620 - 0x0035F62C (0xC) xFMVGetAudioStatus__Fv
unsigned int xFMVGetAudioStatus() {
    { // 0x0035F620 - 0x0035F62C
    }
}

// 0x0035F630 - 0x0035F684 (0x54) xFMVStartAudio__FUiUi
void xFMVStartAudio(unsigned int audioID, unsigned int flags) {
    { // 0x0035F630 - 0x0035F684
    }
}

// 0x0035F690 - 0x0035F848 (0x1B8) xFMVPlayMovie__FUiUi
void xFMVPlayMovie(unsigned int movieID, unsigned int flags) {
    { // 0x0035F690 - 0x0035F848
        signed int controller; // r18
        char movieName[32]; // r29+0x60
        signed int returnVal; // r2
        void (*savedAction)(class tagxPad *, signed int, class tagxPadAction *); // r17
        signed int (*savedMapper)(class tagxPad *, class tagxPadAction *, signed int); // r16
    }
}

// 0x0035F850 - 0x0035F878 (0x28) xFMVFullLevelReset__Fv
void xFMVFullLevelReset() {
    { // 0x0035F850 - 0x0035F878
    }
}

// 0x0035F880 - 0x0035F8A8 (0x28) xFMVLevelReset__Fv
void xFMVLevelReset() {
    { // 0x0035F880 - 0x0035F8A8
    }
}

// 0x0035F8B0 - 0x0035F8D0 (0x20) xFMVInit__Fv
void xFMVInit() {
    { // 0x0035F8B0 - 0x0035F8D0
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xDebug.cpp
// ==========================================================================
signed int gDebugDrawDebugInfo; // 0x004A4E1C
unsigned int l_currentStaticUsed; // 0x004A4D88
unsigned int frametime; // 0x004A4F40
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static char dg_field_str[28][15]; // 0x0044F3C0
static char dg_color_str[6][8]; // 0x0044F390
static unsigned int dg_color[6]; // 0x00000000
static class _XDG dg[6]; // 0x00AAC300
signed char gDGfield[6]; // 0x0044F388
unsigned char gDGbits; // 0x004A4654
unsigned char gDGmode; // 0x004A4650
unsigned char gDGon; // 0x004A4E70
static signed int sTestSndID; // 0x004A4E6C
signed int parDebugMode; // 0x004A4E68
signed int msgDebugMode; // 0x004A4E64
signed int animDebugMode; // 0x004A4E60
signed int guiDebugMode; // 0x004A4E5C
signed int routeDebugMode; // 0x004A4E58
signed int sndDebugMode; // 0x004A4E54
unsigned char dbfont_display; // 0x004A4E50
unsigned char debug_mode; // 0x004A4E4C
static signed int statRecordLoc; // 0x004A4E48
static class PerfTimes statRecord[100]; // 0x00AA6B80
class PerfTimes g_perfTimes; // 0x00AA6AA0
static char dm_names[18][16]; // 0x00000000
// 0x0035F8D0 - 0x0036011C (0x84C) xDebugGraphIdle__Fv
void xDebugGraphIdle() {
    { // 0x0035F8D0 - 0x0036011C
        unsigned long *ptr; // r2
        unsigned int val; // r2
        unsigned int stack; // r65535
        unsigned int i; // r16
    }
}

// 0x00360120 - 0x00360130 (0x10) xDebugGraphInit__Fv
void xDebugGraphInit() {
    { // 0x00360120 - 0x00360130
    }
}

// 0x00360130 - 0x003603DC (0x2AC) xDebugGraphRender__FP4_XDG
void xDebugGraphRender(class _XDG *gp) {
    { // 0x00360130 - 0x003603DC
        unsigned char b; // r20
        unsigned char g; // r19
        unsigned char r; // r18
        class _RwV2d p1; // r29+0x98
        class _RwV2d p0; // r29+0x90
        signed int idx; // r17
        signed int i; // r16
    }
}

// 0x003603E0 - 0x00360608 (0x228) xDebugGraphSetup__FP4_XDGfUiiiiUi
void xDebugGraphSetup(class _XDG *gp, float mult, unsigned int graphcolor, signed int tickmin, signed int tickmax, signed int tickstep, unsigned int tickcolor) {
    { // 0x003603E0 - 0x00360608
    }
}

// 0x00360610 - 0x00360C58 (0x648) xDebugModeService__Fv
void xDebugModeService() {
    { // 0x00360610 - 0x00360C58
    }
}

// 0x00360C60 - 0x00360CC8 (0x68) xDebugUpdateStats__Fv
void xDebugUpdateStats() {
    { // 0x00360C60 - 0x00360CC8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xCond.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class xScene *xScene_current; // 0x004A4E08
// 0x00360CD0 - 0x00361214 (0x544) xConditionGameEventBase__FP10xConditionUiUiUiUi
void xConditionGameEventBase(class xCondition *c, unsigned int eventType, unsigned int eventCmd, unsigned int eventData) {
    { // 0x00360CD0 - 0x00361214
        class iConditionResource *r; // r3
    }
}

// 0x00361220 - 0x0036174C (0x52C) xConditionGameEvent__FP10xConditionUiUiUiUi
void xConditionGameEvent(class xCondition *c, unsigned int eventWho, unsigned int eventType, unsigned int eventCmd, unsigned int eventData) {
    { // 0x00361220 - 0x0036174C
        class iConditionResource *r; // r16
        signed int dataMatch; // r2
        signed int cmdMatch; // r23
        signed int typeMatch; // r30
        signed int whoMatch; // r22
        signed int osMatch; // r2
    }
}

// 0x00361750 - 0x003619BC (0x26C) xConditionEvaluate__FP10xCondition
signed int xConditionEvaluate(class xCondition *c) {
    { // 0x00361750 - 0x003619BC
        class xCondition *idx; // r16
        signed int isTrue; // r2
    }
}

// 0x003619C0 - 0x00361A94 (0xD4) xConditionInitRuntime__FP17iConditionRuntime
void xConditionInitRuntime(class iConditionRuntime *c) {
    { // 0x003619C0 - 0x00361A94
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xCart.cpp
// ==========================================================================
class tag_xCameraControl gCamera; // 0x00AA67C0
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int requestCartRestore; // 0x004A4660
float gTimeSec; // 0x004A4C8C
class xScene *xScene_current; // 0x004A4E08
static unsigned char *restoreDataBuffer; // 0x004A4E84
static signed int restoreDiskInfo; // 0x004A4E80
static unsigned int interpreterMethodRestore; // 0x004A4E7C
static unsigned int interpreterMethodSave; // 0x004A4E78
signed int xCartDescriptionTable[17][2]; // 0x0044F570
signed int gCartTime; // 0x004A4E74
class tagxCartControl gCartControl; // 0x00AAC620
// 0x00361AA0 - 0x00361AC0 (0x20) xCartSlotInfo__Fiii
void xCartSlotInfo(signed int cart, signed int slot, signed int addr) {
    { // 0x00361AA0 - 0x00361AC0
    }
}

// 0x00361AC0 - 0x00361BD0 (0x110) xCartError__Fv
signed int xCartError() {
    { // 0x00361AC0 - 0x00361BD0
    }
}

// 0x00361BD0 - 0x00361C68 (0x98) xCartFormat__Fv
signed int xCartFormat() {
    { // 0x00361BD0 - 0x00361C68
        signed int result; // r16
    }
}

// 0x00361C70 - 0x00361C90 (0x20) xCartIsCardPresent__Fv
signed int xCartIsCardPresent() {
    { // 0x00361C70 - 0x00361C90
    }
}

// 0x00361C90 - 0x00361CB0 (0x20) xCartGetCardFreeBlocks__Fi
signed int xCartGetCardFreeBlocks(signed int slot) {
    { // 0x00361C90 - 0x00361CB0
    }
}

// 0x00361CB0 - 0x00361CD0 (0x20) xCartCountValidCardSlots__Fi
signed int xCartCountValidCardSlots() {
    { // 0x00361CB0 - 0x00361CD0
    }
}

// 0x00361CD0 - 0x00361CF0 (0x20) xCartCheckValidSlot__Fi
signed int xCartCheckValidSlot(signed int slot) {
    { // 0x00361CD0 - 0x00361CF0
    }
}

// 0x00361CF0 - 0x00361D10 (0x20) xCartCountValidSlots__Fi
signed int xCartCountValidSlots(signed int cart) {
    { // 0x00361CF0 - 0x00361D10
    }
}

// 0x00361D10 - 0x00361D74 (0x64) xCartDeleteGame__Fi
signed int xCartDeleteGame(signed int slot) {
    { // 0x00361D10 - 0x00361D74
    }
}

// 0x00361D80 - 0x00361E2C (0xAC) xCartSaveGameDone__Fi
signed int xCartSaveGameDone() {
    { // 0x00361D80 - 0x00361E2C
    }
}

// 0x00361E30 - 0x00362110 (0x2E0) xCartSaveGame__Fi
signed int xCartSaveGame(signed int slot) {
    { // 0x00361E30 - 0x00362110
        signed int hour; // r18
        signed int min; // r17
        signed int sec; // r16
        unsigned int sceneId; // r5
        static char *unknown; // 0x004A4E88
        static signed char init; // 0x004A4E8C
    }
}

// 0x00362110 - 0x00362150 (0x40) xCartLoadGameDone__Fi
signed int xCartLoadGameDone() {
    { // 0x00362110 - 0x00362150
    }
}

// 0x00362150 - 0x003621C0 (0x70) xCartLoadGame__Fi
signed int xCartLoadGame(signed int slot) {
    { // 0x00362150 - 0x003621C0
    }
}

// 0x003621C0 - 0x0036222C (0x6C) xCartAccessCardDone__Fi
signed int xCartAccessCardDone() {
    { // 0x003621C0 - 0x0036222C
    }
}

// 0x00362230 - 0x003622B8 (0x88) xCartAccessCard__Fi
signed int xCartAccessCard(signed int cart) {
    { // 0x00362230 - 0x003622B8
    }
}

// 0x003622C0 - 0x00362468 (0x1A8) xCartIdle__Fv
void xCartIdle() {
    { // 0x003622C0 - 0x00362468
    }
}

// 0x00362470 - 0x00362810 (0x3A0) xCartRestoreComplete__Fv
void xCartRestoreComplete() {
    { // 0x00362470 - 0x00362810
        unsigned char *buffer; // r2
        unsigned short size; // r29+0x6e
        signed int nextDisk; // r18
        signed int time; // r2
        signed int tmp; // r29+0x68
        signed int dataSize; // r2
        unsigned int cameraId; // r2
        unsigned int sceneId; // r17
        signed int dataUsed; // r2
        unsigned char *dataPointer; // r16
    }
}

// 0x00362810 - 0x00362D0C (0x4FC) xCartSave__Fi
void xCartSave(signed int write) {
    { // 0x00362810 - 0x00362D0C
        unsigned int dataSize; // r29+0x68
        unsigned short compressionBufferSize; // r29+0x6e
        unsigned short dataBufferSize; // r29+0x6c
        signed int dataUsed; // r17
        unsigned char *sizePointer; // r16
        unsigned char *dataPointer; // r2
        unsigned char *compressionBuffer; // r16
        unsigned int *javaBuffer; // r2
    }
}

// 0x00362D10 - 0x00362E30 (0x120) xCartSavePlayer__FPUci
void xCartSavePlayer(unsigned char *buffer, signed int size) {
    { // 0x00362D10 - 0x00362E30
        signed int dataSize; // r29+0x5c
        unsigned int *javaBuffer; // r16
    }
}

// 0x00362E30 - 0x00362E54 (0x24) xCartBeep__Fv
void xCartBeep() {
    { // 0x00362E30 - 0x00362E54
    }
}

// 0x00362E60 - 0x00362EA8 (0x48) xCartEnd__Fv
void xCartEnd() {
    { // 0x00362E60 - 0x00362EA8
    }
}

// 0x00362EB0 - 0x00362F0C (0x5C) xCartBegin__Fv
void xCartBegin() {
    { // 0x00362EB0 - 0x00362F0C
    }
}

// 0x00362F10 - 0x00362F20 (0x10) xCartReInitializeInterpreter__Fv
void xCartReInitializeInterpreter() {
    { // 0x00362F10 - 0x00362F20
    }
}

// 0x00362F20 - 0x003630E8 (0x1C8) xCartInit__Fv
signed int xCartInit() {
    { // 0x00362F20 - 0x003630E8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xAsset.cpp
// ==========================================================================
class _xModel g_AshModel[0]; // 0x004E03F0
signed int g_AshModelCount; // 0x004A4D38
class xSkel g_AshSkeleton[0]; // 0x004B52F8
signed int g_AshSkeletonCount; // 0x004A4CB4
class _xModel g_BossModel[0]; // 0x004E0400
signed int g_BossModelCount; // 0x004A4D3C
class xSkel g_BossSkeleton[0]; // 0x004B5300
signed int g_BossSkeletonCount; // 0x004A4CB8
class _xModel g_SNPCModel[0]; // 0x004E0410
signed int g_SNPCModelCount; // 0x004A4D40
class xSkel g_SNPCSkeleton[0]; // 0x004B5310
signed int g_SNPCSkeletonCount; // 0x004A4CBC
class _xModel g_DNPCModel[0]; // 0x004E0440
signed int g_DNPCModelCount; // 0x004A4D44
class xSkel g_DNPCSkeleton[0]; // 0x004B5340
signed int g_DNPCSkeletonCount; // 0x004A4CC0
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _xModel g_RTObjModel[0]; // 0x004E0080
signed int g_RTObjModelCount; // 0x004A4D34
static void (*instanceTable[33])(unsigned int, void **, void **, void **); // 0x0044F600
unsigned int assetBufferResetPoint; // 0x004A4E9C
unsigned int assetBufferRemain; // 0x004A4E98
char *assetBufferNext; // 0x004A4E94
char *assetBuffer; // 0x004A4E90
// 0x003630F0 - 0x003631F0 (0x100) xCharRTObjGeometryInstance__FUiPPvPPvPPv
void xCharRTObjGeometryInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x003630F0 - 0x003631F0
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x003631F0 - 0x00363290 (0xA0) xCharRTObjGeometryGetMesh__FUiPi
class tagP2Mesh *xCharRTObjGeometryGetMesh(unsigned int assetID, signed int *meshCount) {
    { // 0x003631F0 - 0x00363290
        unsigned int id; // r6
        signed int i; // r4
    }
}

// 0x00363290 - 0x00363414 (0x184) xCharDNPCSkeletonInstance__FUiPPvPPvPPv
void xCharDNPCSkeletonInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x00363290 - 0x00363414
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x00363420 - 0x00363660 (0x240) xCharDNPCGeometryInstance__FUiPPvPPvPPv
void xCharDNPCGeometryInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x00363420 - 0x00363660
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x00363660 - 0x003637E4 (0x184) xCharSNPCSkeletonInstance__FUiPPvPPvPPv
void xCharSNPCSkeletonInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x00363660 - 0x003637E4
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x003637F0 - 0x00363974 (0x184) xCharSNPCGeometryInstance__FUiPPvPPvPPv
void xCharSNPCGeometryInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x003637F0 - 0x00363974
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x00363980 - 0x00363B04 (0x184) xCharBossSkeletonInstance__FUiPPvPPvPPv
void xCharBossSkeletonInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x00363980 - 0x00363B04
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x00363B10 - 0x00363C94 (0x184) xCharBossGeometryInstance__FUiPPvPPvPPv
void xCharBossGeometryInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x00363B10 - 0x00363C94
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x00363CA0 - 0x00363E24 (0x184) xCharAshSkeletonInstance__FUiPPvPPvPPv
void xCharAshSkeletonInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x00363CA0 - 0x00363E24
        unsigned int id; // r5
        signed int i; // r4
    }
}

// 0x00363E30 - 0x003640F8 (0x2C8) xCharAshGeometryInstance__FUiPPvPPvPPv
void xCharAshGeometryInstance(unsigned int assetID, void **resource, void **runtime) {
    { // 0x00363E30 - 0x003640F8
        unsigned int id; // r4
        signed int i; // r16
    }
}

// 0x00364100 - 0x00364254 (0x154) xAssetInstance__FUiPPvPPvPPv
void xAssetInstance(unsigned int assetID, void **resource, void **persistent, void **runtime) {
    { // 0x00364100 - 0x00364254
        signed int type; // r16
    }
}

// 0x00364260 - 0x00364288 (0x28) xAssetLevelReset__Fv
void xAssetLevelReset() {
    { // 0x00364260 - 0x00364288
    }
}

// 0x00364290 - 0x003642A0 (0x10) xAssetSetLevelReset__Fv
void xAssetSetLevelReset() {
    { // 0x00364290 - 0x003642A0
    }
}

// 0x003642A0 - 0x003642A8 (0x8) xAssetInit__Fv
void xAssetInit() {
    { // 0x003642A0 - 0x003642A8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xAssert.cpp
// ==========================================================================
// 0x003642B0 - 0x003642DC (0x2C) xUnimplemented__FPCc
void xUnimplemented(char *funcName) {
    { // 0x003642B0 - 0x003642DC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xAnim.cpp
// ==========================================================================
signed int gPlayerType; // 0x004A4E00
class tagxActorDBData *gadb; // 0x004A4658
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static float pendingHeight; // 0x004A4EC4
static class _xActor *pendingHeightActor; // 0x004A4EC0
static signed int pendingHeightReset; // 0x004A4EBC
static unsigned int interpreterMethodSFX; // 0x004A4EB8
static unsigned int interpreterMethodVFXRumble; // 0x004A4EB4
static unsigned int interpreterMethodVFX; // 0x004A4EB0
class tagxFXEntry *g_fxTable; // 0x00000000
static class tagXAnimControl g_activeAnimations[50]; // 0x00AACCD0
class tagXAnimSet g_DNPCAnimations[5]; // 0x00AACC60
class tagXAnimSet g_SNPCAnimations[5]; // 0x00AACBF0
class tagXAnimSet g_BossAnimations[2]; // 0x00AACBC0
class tagXAnimSet g_AshAnimations[1]; // 0x00AACBA0
signed int g_DNPCAnimationsCount; // 0x004A4EAC
signed int g_SNPCAnimationsCount; // 0x004A4EA8
signed int g_BossAnimationsCount; // 0x004A4EA4
signed int g_AshAnimationsCount; // 0x004A4EA0
// 0x003642E0 - 0x0036436C (0x8C) xAnimPostIdle__Fv
void xAnimPostIdle() {
    { // 0x003642E0 - 0x0036436C
        class tagxPadAction action; // r29+0x30
        class _xActor *a; // r2
        class tagXAnimControl *act; // r17
        signed int i; // r16
    }
}

// 0x00364370 - 0x00364AB0 (0x740) xAnimIdle__Fv
void xAnimIdle() {
    { // 0x00364370 - 0x00364AB0
        signed int assetID; // r20
        class _xActor *a; // r19
        class tagXAnimTrigger trigger; // r29+0xf8
        class xMessage theMessage; // r29+0x90
        class _xActor *xa; // r3
        class _xActor *xa; // r2
        class xMessage theMessage; // r29+0x60
        class Vector3D rot; // r29+0xe8
        class Vector3D trans2; // r29+0xd8
        class Vector3D trans1; // r29+0xc8
        signed int endAnim; // r5
        class tagiAnimResource *ar; // r18
        class tagXAnim *anim; // r2
        class tagXAnimControl *act; // r17
        signed int i; // r16
    }
}

// 0x00364AB0 - 0x00365274 (0x7C4) xAnimIdleFX__FPvP8tagXAnimi
void xAnimIdleFX(void *actorVoid, class tagXAnim *anim, signed int frame) {
    { // 0x00364AB0 - 0x00365274
        class Vector3D v2; // r29+0x188
        class Vector3D v1; // r29+0x178
        class tagMATRIX pm; // r29+0x130
        class tagMATRIX m; // r29+0xf0
        class Vector3D *v2; // r20
        class Vector3D *v1; // r19
        float dur; // 
        float right; // 
        float left; // 
        signed int scriptHandled; // r29+0x19c
        class _iVFXResource *idx; // r18
        signed int scriptHandled; // r29+0x198
        class _iSFXResource *idx; // r18
        signed int matrixBuilt; // r30
        signed int i; // r17
        class tagiAnimResource *r; // r22
        class iActorRuntime *ar; // r16
        class _xActor *actor; // r2
    }
}

// 0x00365280 - 0x0036562C (0x3AC) xAnimDebug__Fi
void xAnimDebug(signed int mode) {
    { // 0x00365280 - 0x0036562C
        char stateName[32]; // r29+0x170
        char jointMask[32]; // r29+0x150
        signed int j; // r5
        signed int i; // r17
        char name[256]; // r29+0x50
        class tagiAnimResource *ar; // r2
        class tagXAnim *anim; // r19
        class tagXAnimControl *act; // r16
        static char buffer[32]; // 0x00AAD630
    }
}

// 0x00365630 - 0x00365810 (0x1E0) doBulletCheck__FP7_xActorP8Vector3DP8Vector3D
void doBulletCheck(class _xActor *actor) {
    { // 0x00365630 - 0x00365810
        class xMessage theMessage; // r29+0x80
        class _xActor *npc; // r19
        signed int i; // r18
        signed int fov; // r17
        signed int bulletRange; // r16
    }
}

// 0x00365810 - 0x003658C4 (0xB4) xAnimActorSetNewPosition__FPv
void xAnimActorSetNewPosition(void *actor) {
    { // 0x00365810 - 0x003658C4
        unsigned int i; // r3
    }
}

// 0x003658D0 - 0x00365974 (0xA4) xAnimResetActor__FPv
void xAnimResetActor(void *actor) {
    { // 0x003658D0 - 0x00365974
        signed int i; // r3
    }
}

// 0x00365980 - 0x003659F4 (0x74) xAnimDeleteActive__FP15tagXAnimControl
void xAnimDeleteActive(class tagXAnimControl *curr) {
    { // 0x00365980 - 0x003659F4
        class _xActor *xa; // r3
        class xMessage theMessage; // r29+0x20
    }
}

// 0x00365A00 - 0x00365A0C (0xC) xAnimChangeActive__FP15tagXAnimControli
void xAnimChangeActive(class tagXAnimControl *curr, signed int state) {
    { // 0x00365A00 - 0x00365A0C
    }
}

// 0x00365A10 - 0x00365B50 (0x140) xAnimChainActive__FP15tagXAnimControlP8tagXAnimPUciUiUi
class tagXAnimControl *xAnimChainActive(class tagXAnimControl *curr, class tagXAnim *anim, unsigned char *activeJoints, signed int state, unsigned int startEvent, unsigned int endEvent) {
    { // 0x00365A10 - 0x00365B50
    }
}

// 0x00365B50 - 0x00365CD4 (0x184) xAnimReplaceActive__FP15tagXAnimControlP8tagXAnimPUciUiUi
void xAnimReplaceActive(class tagXAnimControl *curr, class tagXAnim *anim, unsigned char *activeJoints, signed int state, unsigned int enterEvent, unsigned int exitEvent) {
    { // 0x00365B50 - 0x00365CD4
        class xMessage theMessage; // r29+0xb0
        class xMessage theMessage; // r29+0x80
        class _xActor *xa; // r16
    }
}

// 0x00365CE0 - 0x00365F90 (0x2B0) xAnimAddActive__FP8tagXAnimPUciPviUiUi
class tagXAnimControl *xAnimAddActive(class tagXAnim *anim, unsigned char *activeJoints, signed int state, void *c, signed int push, unsigned int startEvent, unsigned int endEvent) {
    { // 0x00365CE0 - 0x00365F90
        class xMessage theMessage; // r29+0xa0
        class _xActor *xa; // r3
        class tagXAnimControl *act; // r16
        signed int lastFound; // r5
        signed int j; // r4
        signed int i; // r2
    }
}

// 0x00365F90 - 0x00366348 (0x3B8) xAnimFind__FUi
class tagXAnim *xAnimFind(unsigned int assetID) {
    { // 0x00365F90 - 0x00366348
        signed int checkIdOnly; // r7
        signed int idOnly; // r6
        signed int p; // r4
        signed int i; // r9
        signed int count; // r8
        class tagXAnim *a; // r2
        class tagXAnim *list; // r5
        signed int assetType; // r3
    }
}

// 0x00366350 - 0x00366528 (0x1D8) xAnimLevelReset__Fv
void xAnimLevelReset() {
    { // 0x00366350 - 0x00366528
        class _xActor *xa; // r4
        signed int i; // r3
    }
}

// 0x00366530 - 0x00366544 (0x14) xAnimReInitializeInterpreter__Fv
void xAnimReInitializeInterpreter() {
    { // 0x00366530 - 0x00366544
    }
}

// 0x00366550 - 0x003667D8 (0x288) xAnimInit__Fv
void xAnimInit() {
    { // 0x00366550 - 0x003667D8
        signed int i; // r3
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xActorDB.cpp
// ==========================================================================
signed int gDebugDrawDebugInfo; // 0x004A4E1C
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int gLOSOld; // 0x00000000
static signed int padActionMethodIndex; // 0x004A4ECC
static class charstate charState[40]; // 0x0044F690
class charstate *sinfo; // 0x004A4EC8
class tagxActorDBData *gadb; // 0x004A4658
class tagxActorDBData gActorDBData; // 0x00AAD650
// 0x003667E0 - 0x00366B68 (0x388) xActorDBSetPositionActor__Fiiii
void xActorDBSetPositionActor(signed int objectType, signed int actorID, signed int toObjectType, signed int toActorID) {
    { // 0x003667E0 - 0x00366B68
    }
}

// 0x00366B70 - 0x00366CD4 (0x164) xActorDBSetPositionNavigationPoint__Fiii
void xActorDBSetPositionNavigationPoint(signed int objectType, signed int actorID, signed int navID) {
    { // 0x00366B70 - 0x00366CD4
        class xNavPoint *nav; // r5
    }
}

// 0x00366CE0 - 0x00366EA0 (0x1C0) xActorDBSetPosition__FiiP8Vector3D
void xActorDBSetPosition(signed int objectType, signed int actorID, class Vector3D *p) {
    { // 0x00366CE0 - 0x00366EA0
    }
}

// 0x00366EA0 - 0x00366FB4 (0x114) xActorDBGetActor__Fii
class _xActor *xActorDBGetActor(signed int objectType, signed int actorID) {
    { // 0x00366EA0 - 0x00366FB4
    }
}

// 0x00366FC0 - 0x00367184 (0x1C4) xActorDBConsoleAction__Fiii
signed int xActorDBConsoleAction(signed int objectType, signed int actorID, signed int action) {
    { // 0x00366FC0 - 0x00367184
        class tagxPadAction a; // r29+0x60
    }
}

// 0x00367190 - 0x0036745C (0x2CC) xActorDBSetRoute__Fiii
void xActorDBSetRoute(signed int objectType, signed int actorID, signed int routeID) {
    { // 0x00367190 - 0x0036745C
        class iActorRuntime *r; // r16
    }
}

// 0x00367460 - 0x00367618 (0x1B8) xActorDBIdle__Fv
void xActorDBIdle() {
    { // 0x00367460 - 0x00367618
        class xMessage theMessage; // r29+0x40
        class iActorRuntime *r; // r18
        class _xActor *a; // r17
        signed int i; // r16
    }
}

// 0x00367620 - 0x00367628 (0x8) xActorDBDebugRoute__Fi
void xActorDBDebugRoute() {
    { // 0x00367620 - 0x00367628
    }
}

// 0x00367630 - 0x00367638 (0x8) xActorDBDebug__Fv
void xActorDBDebug() {
    { // 0x00367630 - 0x00367638
    }
}

// 0x00367640 - 0x003679B4 (0x374) xActorDBSlide__FP7_xActor
void xActorDBSlide(class _xActor *a) {
    { // 0x00367640 - 0x003679B4
        signed int oldApplyVel; // r17
        signed int flip; // r2
        class Vector3D tmp; // r29+0x70
        class Vector3D vhn; // r29+0x60
        class Vector3D d; // r29+0x50
        class tagxPhysicsObject *o; // r17
        class iActorRuntime *r; // r16
    }
}

// 0x003679C0 - 0x00367C20 (0x260) xActorDBPadActionSendRemap__FiiiP13tagxPadAction
void xActorDBPadActionSendRemap(signed int type, signed int id, signed int count, class tagxPadAction *actions) {
    { // 0x003679C0 - 0x00367C20
        unsigned int remappedAction; // r29+0x8c
        signed int i; // r16
        class iActorRuntime *r; // r2
        class tagXAnimControl *active; // r2
        class tagxPhysicsObject *phys; // r2
        class _xActor *actor; // r2
        class tagXAnim *anim; // r2
        class tagxPadAction *a; // r2
    }
}

// 0x00367C20 - 0x00367ECC (0x2AC) xActorDBDestroy__Fii
void xActorDBDestroy(signed int objectType, signed int id) {
    { // 0x00367C20 - 0x00367ECC
        class _xActor *a; // r16
        class iActorRuntime *r; // r3
    }
}

// 0x00367ED0 - 0x00368370 (0x4A0) xActorDBSetup__FiiUiUiUi
void xActorDBSetup(signed int type, signed int id, unsigned int modelType, unsigned int skeletonType, unsigned int textureType) {
    { // 0x00367ED0 - 0x00368370
        class tagxActorDBMemMgr *mem; // r2
        class xSkel *skeleton; // r22
        class _xModel *model; // r21
        class iActorRuntime *r; // r20
        class _xActor *a; // r19
    }
}

// 0x00368370 - 0x003686C8 (0x358) xActorDBCreate__FiUiUiUi
signed int xActorDBCreate(signed int type, unsigned int modelType, unsigned int skeletonType, unsigned int textureType) {
    { // 0x00368370 - 0x003686C8
        signed int a; // r16
    }
}

// 0x003686D0 - 0x003687E0 (0x110) xActorDBActiveRemove__FP7_xActor
void xActorDBActiveRemove(class _xActor *a) {
    { // 0x003686D0 - 0x003687E0
        signed int i; // r5
    }
}

// 0x003687E0 - 0x00368860 (0x80) xActorDBDraw__Fv
void xActorDBDraw() {
    { // 0x003687E0 - 0x00368860
        class _xActor *a; // r2
        signed int i; // r16
    }
}

// 0x00368860 - 0x0036886C (0xC) xActorDBReInitializeInterpreter__Fv
void xActorDBReInitializeInterpreter() {
    { // 0x00368860 - 0x0036886C
    }
}

// 0x00368870 - 0x00368940 (0xD0) xActorDBFullReset__Fv
void xActorDBFullReset() {
    { // 0x00368870 - 0x00368940
        signed int i; // r16
    }
}

// 0x00368940 - 0x00368A10 (0xD0) xActorDBLevelReset__Fv
void xActorDBLevelReset() {
    { // 0x00368940 - 0x00368A10
        signed int i; // r16
    }
}

// 0x00368A10 - 0x00369388 (0x978) xActorDBInitialize__Fv
void xActorDBInitialize() {
    { // 0x00368A10 - 0x00369388
        signed int a; // r2
        signed int i; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xActor.cpp
// ==========================================================================
signed int gBootState; // 0x004A4E20
class tagxActorDBData *gadb; // 0x004A4658
class tagxObjDBData *godb; // 0x004A3FAC
signed int gPlayerType; // 0x004A4E00
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
unsigned int g_currentPlayerRotateSpeed; // 0x004A4648
float g_DEGTORAD; // 0x00485A58
static unsigned int interpreterMethod; // 0x004A4ED0
// 0x00369390 - 0x003693C8 (0x38) xActorCalloutFlyBack__FP7_xActor
void xActorCalloutFlyBack(class _xActor *a) {
    { // 0x00369390 - 0x003693C8
        class tagxPhysicsObject *phys; // r2
    }
}

// 0x003693D0 - 0x00369408 (0x38) xActorCalloutFlyFast__FP7_xActor
void xActorCalloutFlyFast(class _xActor *a) {
    { // 0x003693D0 - 0x00369408
        class tagxPhysicsObject *phys; // r2
    }
}

// 0x00369410 - 0x00369448 (0x38) xActorCalloutFlySlow__FP7_xActor
void xActorCalloutFlySlow(class _xActor *a) {
    { // 0x00369410 - 0x00369448
        class tagxPhysicsObject *phys; // r2
    }
}

// 0x00369450 - 0x00369458 (0x8) xActorCalloutRunBack__FP7_xActor
void xActorCalloutRunBack() {
    { // 0x00369450 - 0x00369458
    }
}

// 0x00369460 - 0x00369468 (0x8) xActorCalloutRun__FP7_xActor
void xActorCalloutRun() {
    { // 0x00369460 - 0x00369468
    }
}

// 0x00369470 - 0x00369478 (0x8) xActorCalloutWalkBack__FP7_xActor
void xActorCalloutWalkBack() {
    { // 0x00369470 - 0x00369478
    }
}

// 0x00369480 - 0x00369488 (0x8) xActorCalloutWalk__FP7_xActor
void xActorCalloutWalk() {
    { // 0x00369480 - 0x00369488
    }
}

// 0x00369490 - 0x0036958C (0xFC) xActorCalloutRotateCounterClockwise__FP7_xActor
void xActorCalloutRotateCounterClockwise(class _xActor *a) {
    { // 0x00369490 - 0x0036958C
    }
}

// 0x00369590 - 0x00369688 (0xF8) xActorCalloutRotateClockwise__FP7_xActor
void xActorCalloutRotateClockwise(class _xActor *a) {
    { // 0x00369590 - 0x00369688
    }
}

// 0x00369690 - 0x00369834 (0x1A4) xActorGetMatrix__FP7_xActorP9tagMATRIX
void xActorGetMatrix(class _xActor *a, class tagMATRIX *actorMatrix) {
    { // 0x00369690 - 0x00369834
        class tagxPhysicsObject *p; // r17
        class tagMATRIX ms; // r29+0x190
        class tagMATRIX m1; // r29+0x150
        class tagMATRIX matrix; // r29+0x110
        class tagMATRIX mz; // r29+0xd0
        class tagMATRIX my; // r29+0x90
        class tagMATRIX mx; // r29+0x50
        class iActorRuntime *r; // r3
    }
}

// 0x00369840 - 0x003698D0 (0x90) xActorSetOrientation__FP7_xActorfff
void xActorSetOrientation(class _xActor *a, float rx, float ry, float rz) {
    { // 0x00369840 - 0x003698D0
        class iActorRuntime *r; // r16
    }
}

// 0x003698D0 - 0x00369964 (0x94) xActorSetLookat__FP7_xActorf
void xActorSetLookat(class _xActor *a) {
    { // 0x003698D0 - 0x00369964
        float ry; // r29+0x30
        class iActorRuntime *r; // r2
    }
}

// 0x00369970 - 0x00369A10 (0xA0) xActorAdjustOrientation__FP7_xActorfff
void xActorAdjustOrientation(class _xActor *a, float rx, float ry, float rz) {
    { // 0x00369970 - 0x00369A10
        class iActorRuntime *r; // r2
    }
}

// 0x00369A10 - 0x00369A50 (0x40) xActorSetPosition__FP7_xActorfff
void xActorSetPosition(class _xActor *a, float x, float y, float z) {
    { // 0x00369A10 - 0x00369A50
        class iActorRuntime *r; // r2
    }
}

// 0x00369A50 - 0x00369CE4 (0x294) xActorActorInFOV__FP7_xActorP7_xActor
signed int xActorActorInFOV(class _xActor *a, class _xActor *b) {
    { // 0x00369A50 - 0x00369CE4
    }
}

// 0x00369CF0 - 0x00369D10 (0x20) xActorDraw__FP7_xActor
void xActorDraw(class _xActor *a) {
    { // 0x00369CF0 - 0x00369D10
    }
}

// 0x00369D10 - 0x0036A1C8 (0x4B8) xActorIdle__FP7_xActor
void xActorIdle(class _xActor *obj) {
    { // 0x00369D10 - 0x0036A1C8
        class iActorRuntime *aa; // r6
        signed int t; // r3
        class tagXAnimControl *active; // r18
        class tagxPhysicsObject *phys; // r17
        class tagXAnim *anim; // r16
        class Vector3D rot; // r29+0x80
        class Vector3D trans3; // r29+0x70
        class Vector3D trans2; // r29+0x60
    }
}

// 0x0036A1D0 - 0x0036A638 (0x468) xActorIdleRoute__FP7_xActori
void xActorIdleRoute(class _xActor *actor) {
    { // 0x0036A1D0 - 0x0036A638
        float mag; // 
        class _xActor *target_actor; // r18
        class _xObj *target_obj; // r18
        class xNavPoint *nextNav; // r2
        class Vector3D v; // r29+0x70
        class iActorRuntime *a; // r17
        class tagxPhysicsObject *p; // r17
        class xRoute *r; // r16
    }
}

// 0x0036A640 - 0x0036AD00 (0x6C0) xActorIdleAdvanceRoute__FP7_xActorP6xRoute
void xActorIdleAdvanceRoute(class _xActor *actor, class xRoute *r) {
    { // 0x0036A640 - 0x0036AD00
        signed int id; // r3
        float ang; // r29+0xd0
        class xNavPoint *cnp; // r18
        class tagxPhysicsObject *p; // r17
        class Vector3D *cpos; // r2
        class iActorRuntime *a; // r16
        class Vector3D v; // r29+0xc0
        class Vector3D posb; // r29+0xb0
        class Vector3D posa; // r29+0xa0
        class xNavPoint *np; // r21
    }
}

// 0x0036AD00 - 0x0036AE14 (0x114) xActorConsoleMgr__FP7_xActoriP13tagxPadActioni
signed int xActorConsoleMgr(class _xActor *actor, signed int count, class tagxPadAction *actions, signed int remap) {
    { // 0x0036AD00 - 0x0036AE14
        signed int validAction; // r17
        class tagXAnimControl *active; // r2
        class tagxPadAction *a; // r2
        signed int i; // r16
    }
}

// 0x0036AE20 - 0x0036AE28 (0x8) xActorDestroy__FP7_xActor
void xActorDestroy() {
    { // 0x0036AE20 - 0x0036AE28
    }
}

// 0x0036AE30 - 0x0036AEB4 (0x84) xActorInitialize__FP7_xActor
void xActorInitialize(class _xActor *a) {
    { // 0x0036AE30 - 0x0036AEB4
    }
}

// 0x0036AEC0 - 0x0036AECC (0xC) xActorReInitializeInterpreter__Fv
void xActorReInitializeInterpreter() {
    { // 0x0036AEC0 - 0x0036AECC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xAction.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int gGameState; // 0x004A4640
static signed int scriptSoundLoadIndex; // 0x004A4ED4
// 0x0036AED0 - 0x0036B270 (0x3A0) xActionExecute__FP7xAction
void xActionExecute(class xAction *a) {
    { // 0x0036AED0 - 0x0036B270
        class iActionResource *r; // r16
    }
}

// 0x0036B270 - 0x0036B27C (0xC) xActionReInitializeInterpreter__Fv
void xActionReInitializeInterpreter() {
    { // 0x0036B270 - 0x0036B27C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iTex.cpp
// ==========================================================================
signed int g_TextureCount; // 0x004A4C98
class _xTex *g_Textures[200]; // 0x004AD440
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x0036B280 - 0x0036B404 (0x184) iTexAssignObj__FP5_xObjP5_xTex
void iTexAssignObj(class _xObj *obj, class _xTex *tex) {
    { // 0x0036B280 - 0x0036B404
        class _iTexRuntime *tr; // r16
        class iObjRuntime *ob; // r3
    }
}

// 0x0036B410 - 0x0036B594 (0x184) iTexAssign__FP7_xActorP5_xTex
void iTexAssign(class _xActor *actor, class _xTex *tex) {
    { // 0x0036B410 - 0x0036B594
        class _iTexRuntime *tr; // r16
        class iActorRuntime *ar; // r3
    }
}

// 0x0036B5A0 - 0x0036B768 (0x1C8) iTexUnloadAll__Fv
void iTexUnloadAll() {
    { // 0x0036B5A0 - 0x0036B768
        signed int t; // r16
        class _xTex *tex; // r3
    }
}

// 0x0036B770 - 0x0036B778 (0x8) iTexLoad__FP5_xTexPUc
void iTexLoad() {
    { // 0x0036B770 - 0x0036B778
    }
}

// 0x0036B780 - 0x0036B980 (0x200) iTexLevelReset__Fv
void iTexLevelReset() {
    { // 0x0036B780 - 0x0036B980
        signed int i; // r16
    }
}

// 0x0036B980 - 0x0036BAD0 (0x150) iTexSetLevelReset__Fv
void iTexSetLevelReset() {
    { // 0x0036B980 - 0x0036BAD0
        signed int i; // r16
    }
}

// 0x0036BAD0 - 0x0036BB34 (0x64) iTexInit__Fv
void iTexInit() {
    { // 0x0036BAD0 - 0x0036BB34
        signed int max; // r2
        signed int size; // r2
        signed int i; // r5
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iStream.cpp
// ==========================================================================
signed int assetBufferRemain; // 0x004A4E98
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
unsigned int currentID; // 0x004A465C
char currentFileName[32]; // 0x00AADFC0
class _RwStream *currentFileID; // 0x004A4ED8
// 0x0036BB40 - 0x0036BC14 (0xD4) iStreamAbort__FUi
void iStreamAbort(unsigned int id) {
    { // 0x0036BB40 - 0x0036BC14
    }
}

// 0x0036BC20 - 0x0036C52C (0x90C) iStreamDoRead__FP7XStreamPFP7XStream_vi
void iStreamDoRead(class XStream *stream, void (*callback)(class XStream *), signed int reloadSound) {
    { // 0x0036BC20 - 0x0036C52C
        signed int stat; // r2
        signed int stat; // r2
        unsigned char *dataStart; // r19
        signed int assetID; // r2
        signed int lastSound; // r4
        signed int si; // r2
        signed int padSize; // r18
        signed int type; // r3
        signed int xfrSize; // r3
        unsigned int dramSize; // r17
        unsigned int sectors; // r18
        unsigned int bytes; // r16
        signed int h; // r2
    }
}

// 0x0036C530 - 0x0036C554 (0x24) iStreamRead__FP7XStreamPFP7XStream_v
void iStreamRead(class XStream *stream, void (*callback)(class XStream *)) {
    { // 0x0036C530 - 0x0036C554
    }
}

// 0x0036C560 - 0x0036C61C (0xBC) iStreamWait__FUi
void iStreamWait(unsigned int id) {
    { // 0x0036C560 - 0x0036C61C
    }
}

// 0x0036C620 - 0x0036C704 (0xE4) iStreamSeek__FUiUi
void iStreamSeek(unsigned int id) {
    { // 0x0036C620 - 0x0036C704
        char name[32]; // r29+0x20
        signed int tens; // r3
        signed int scn; // r6
        signed int seq; // r5
        signed int act; // r3
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iSndMgr.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _xSndMgrData gSndMgr; // 0x004B1F00
unsigned char p2_voice_status[64]; // 0x00AADFE0
static class AudioStream_s *myNextAudioStream; // 0x004A4EF0
static class AudioStream_s *myAudioStreams; // 0x004A4EEC
signed int NextStrPointerInitialized; // 0x00000000
signed int p2SndMgrSafeToAudioDriverInterrupt; // 0x004A4EE8
signed int p2SndMgrSafeToAudioInterrupt; // 0x004A4EE4
unsigned int SRAMbase; // 0x004A4EE0
unsigned int sram_addr; // 0x004A4EDC
// 0x0036C710 - 0x0036C75C (0x4C) iSndMgrCreateStream__FPci
void *iSndMgrCreateStream(char *FileName, signed int looping) {
    { // 0x0036C710 - 0x0036C75C
        class AudioStream_s *pStream; // r2
    }
}

// 0x0036C760 - 0x0036C768 (0x8) iSndMgrSetVoiceFrequency__Fii
void iSndMgrSetVoiceFrequency() {
    { // 0x0036C760 - 0x0036C768
    }
}

// 0x0036C770 - 0x0036C7CC (0x5C) iSndMgrStopSound__Fi
void iSndMgrStopSound(signed int soundVoice) {
    { // 0x0036C770 - 0x0036C7CC
        class _iSFX *sp; // r5
    }
}

// 0x0036C7D0 - 0x0036C94C (0x17C) iSndMgrPlaySoundVolLoop__Fiii
signed int iSndMgrPlaySoundVolLoop(signed int soundID, signed int soundVol, signed int loopCount) {
    { // 0x0036C7D0 - 0x0036C94C
        class _iWAV *wav; // r16
        class _iSFX *sfx; // r3
        signed int voice; // r2
    }
}

// 0x0036C950 - 0x0036CB60 (0x210) iSndMgrPlaySoundVolFreq__Fiii
signed int iSndMgrPlaySoundVolFreq(signed int soundID, signed int soundVol, signed int freq) {
    { // 0x0036C950 - 0x0036CB60
        class _iWAV *wav; // r16
        class _iSFX *sfx; // r3
        signed int voice; // r2
    }
}

// 0x0036CB60 - 0x0036CD50 (0x1F0) iSndMgrPlaySoundVol__Fii
signed int iSndMgrPlaySoundVol(signed int soundID, signed int soundVol) {
    { // 0x0036CB60 - 0x0036CD50
        class _iWAV *wav; // r16
        class _iSFX *sfx; // r3
        signed int voice; // r2
    }
}

// 0x0036CD50 - 0x0036CF50 (0x200) iSndMgrPlaySound__FP5_iSFX
signed int iSndMgrPlaySound(class _iSFX *sfx) {
    { // 0x0036CD50 - 0x0036CF50
        unsigned int voice; // r4
        class _iWAV *wav; // r3
    }
}

// 0x0036CF50 - 0x0036D188 (0x238) iSndMgrLoadSoundBuffer__FPUcUii
signed int iSndMgrLoadSoundBuffer(unsigned char *buffer, unsigned int soundSize, signed int soundID) {
    { // 0x0036CF50 - 0x0036D188
        unsigned int oldSRAMAddr; // r2
        unsigned char *dst; // r2
        unsigned int soundDataSize; // r2
        class _iWAV *wav; // r16
    }
}

// 0x0036D190 - 0x0036D1C4 (0x34) iSndMgrFixFreq__Fii
void iSndMgrFixFreq(signed int soundID, signed int newFreq) {
    { // 0x0036D190 - 0x0036D1C4
        class _iWAV *wav; // r2
    }
}

// 0x0036D1D0 - 0x0036D1E0 (0x10) iSndMgrGetSRAMSize__Fv
unsigned int iSndMgrGetSRAMSize() {
    { // 0x0036D1D0 - 0x0036D1E0
    }
}

// 0x0036D1E0 - 0x0036DF24 (0xD44) iSndMgrTransferToSRAM__FPUcUiP5_iWAV
signed int iSndMgrTransferToSRAM(unsigned char *buffer, unsigned int size, class _iWAV *wav) {
    { // 0x0036D1E0 - 0x0036DF24
        class _iWAV *tmp; // r16
        signed int bank; // r17
        signed int s; // r16
        signed int i; // r2
        unsigned int spusize; // r2
        unsigned int count; // r16
    }
}

// 0x0036DF30 - 0x0036E55C (0x62C) iSndMgrBankMakeRoom__FiPUiUi
signed int iSndMgrBankMakeRoom(signed int bank, unsigned int *addr, unsigned int size) {
    { // 0x0036DF30 - 0x0036E55C
        unsigned int delta; // r2
        signed int s; // r5
        class _iWAV *oldprev; // r19
        class _iWAV *old; // r18
        class _iWAV *prev; // r17
        class _iWAV *idx; // r16
    }
}

// 0x0036E560 - 0x0036E628 (0xC8) iSndMgrUnpause__Fv
void iSndMgrUnpause() {
    { // 0x0036E560 - 0x0036E628
    }
}

// 0x0036E630 - 0x0036E6E4 (0xB4) iSndMgrPause__Fv
void iSndMgrPause() {
    { // 0x0036E630 - 0x0036E6E4
    }
}

// 0x0036E6F0 - 0x0036EBC4 (0x4D4) p2SndMgrUpdateVoice__FiiP5_iSFX
void p2SndMgrUpdateVoice(signed int status, class _iSFX *sp) {
    { // 0x0036E6F0 - 0x0036EBC4
        signed int voice; // r2
    }
}

// 0x0036EBD0 - 0x0036EF78 (0x3A8) p2SndMgrCalcVol__FP5_iSFX
void p2SndMgrCalcVol(class _iSFX *sp) {
    { // 0x0036EBD0 - 0x0036EF78
        signed int vol_right; // r17
        signed int vol_left; // r16
        float lrad2; // 
        float rad2; // 
        float distance; // 
        float vol; // 
    }
}

// 0x0036EF80 - 0x0036F0FC (0x17C) iSndMgrIdle__FUl
void iSndMgrIdle() {
    { // 0x0036EF80 - 0x0036F0FC
        class _iSFX *sp; // r17
        unsigned int i; // r16
    }
}

// 0x0036F100 - 0x0036F108 (0x8) iSndMgrDebugMode__Fi
void iSndMgrDebugMode() {
    { // 0x0036F100 - 0x0036F108
    }
}

// 0x0036F110 - 0x0036F5C4 (0x4B4) iSndMgrFreeAll__Fv
void iSndMgrFreeAll() {
    { // 0x0036F110 - 0x0036F5C4
        signed int s; // r17
        signed int i; // r16
    }
}

// 0x0036F5D0 - 0x0036FB7C (0x5AC) iSndMgrLevelReset__Fv
void iSndMgrLevelReset() {
    { // 0x0036F5D0 - 0x0036FB7C
        signed int s; // r17
        signed int i; // r16
    }
}

// 0x0036FB80 - 0x0036FC4C (0xCC) iSndMgrSetLevelReset__Fv
void iSndMgrSetLevelReset() {
    { // 0x0036FB80 - 0x0036FC4C
        signed int i; // r3
    }
}

// 0x0036FC50 - 0x0036FCBC (0x6C) iSndMgrDispose__Fv
void iSndMgrDispose() {
    { // 0x0036FC50 - 0x0036FCBC
        class AudioStream_s *next; // r16
        class AudioStream_s *cur; // r2
    }
}

// 0x0036FCC0 - 0x0036FD78 (0xB8) iSndMgrInit__Fv
void iSndMgrInit() {
    { // 0x0036FCC0 - 0x0036FD78
        unsigned int i; // r4
        unsigned int driverSize; // r2
        unsigned int *driverImage; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iSkel.cpp
// ==========================================================================
// 0x0036FD80 - 0x0036FD90 (0x10) iSkelAssign__FP7_xActorP5xSkel
void iSkelAssign(class _xActor *actor, class xSkel *skeleton) {
    { // 0x0036FD80 - 0x0036FD90
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iScrFx.cpp
// ==========================================================================
unsigned int gScreenSaver; // 0x004A4F34
class __xScrFxMgr gScrFx; // 0x004B5368
signed int p2MainLetterboxDelta; // 0x004A4EF8
signed int p2MainLetterboxHeight; // 0x004A4EF4
// 0x0036FD90 - 0x0036FEB0 (0x120) iScrFxFinishFade__Fv
void iScrFxFinishFade() {
    { // 0x0036FD90 - 0x0036FEB0
    }
}

// 0x0036FEB0 - 0x003700E4 (0x234) iScrFxRender__Fv
void iScrFxRender() {
    { // 0x0036FEB0 - 0x003700E4
    }
}

// 0x003700F0 - 0x00370100 (0x10) iScrFxShake__Fi
void iScrFxShake() {
    { // 0x003700F0 - 0x00370100
    }
}

// 0x00370100 - 0x00370108 (0x8) iScrFxFadeIn__Fv
void iScrFxFadeIn() {
    { // 0x00370100 - 0x00370108
    }
}

// 0x00370110 - 0x00370118 (0x8) iScrFxFadeOut__Fv
void iScrFxFadeOut() {
    { // 0x00370110 - 0x00370118
    }
}

// 0x00370120 - 0x00370148 (0x28) iScrFxLetterbox__Fi
void iScrFxLetterbox(signed int flag) {
    { // 0x00370120 - 0x00370148
    }
}

// 0x00370150 - 0x0037021C (0xCC) iScrFxIdle__Fv
void iScrFxIdle() {
    { // 0x00370150 - 0x0037021C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iPlate.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class xPlate gPlate; // 0x00AA67B0
signed int gGameState; // 0x004A4640
unsigned int sTexMemHack; // 0x00000000
signed int after_fmv_master_volume; // 0x00000000
unsigned int blank_frames; // 0x00000000
float bgi; // 0x00000000
unsigned char actual_mask_pixels; // 0x00000000
float mask_alpha; // 0x00000000
unsigned short mask_color; // 0x00000000
class _iPlateResource gPlateList[300]; // 0x00000000
signed int gPlateCount; // 0x004A4F00
class _iPlateRuntime *g_PlateMgr; // 0x004A4EFC
unsigned char dxoff; // 0x00000000
static char dbfont_chars[51]; // 0x00000000
// 0x00370220 - 0x00370248 (0x28) p2PlateRender__Fv
void p2PlateRender() {
    { // 0x00370220 - 0x00370248
    }
}

// 0x00370250 - 0x00370548 (0x2F8) iPlateReset__Fv
void iPlateReset() {
    { // 0x00370250 - 0x00370548
        signed int i; // r16
    }
}

// 0x00370550 - 0x00370558 (0x8) p2PlateExit__Fv
void p2PlateExit() {
    { // 0x00370550 - 0x00370558
    }
}

// 0x00370560 - 0x00370568 (0x8) p2MaskExit__Fv
void p2MaskExit() {
    { // 0x00370560 - 0x00370568
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iPhyObj.cpp
// ==========================================================================
class tagxActorDBData *gadb; // 0x004A4658
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagxObjDBData *godb; // 0x004A3FAC
class xScene *xScene_current; // 0x004A4E08
signed int bonk_shift; // 0x00000000
unsigned int bonk_threshold; // 0x00000000
// 0x00370570 - 0x0037090C (0x39C) iPhysicsTrace2D_OBJECT__FP17tagxPhysicsObjectP8Vector3DP8Vector3DP8Vector3DPf
unsigned int iPhysicsTrace2D_OBJECT(class tagxPhysicsObject *o, class Vector3D *a, class Vector3D *b, class Vector3D *c, float *ct) {
    { // 0x00370570 - 0x0037090C
        class Vector3D sc; // r29+0x100
        class Vector3D sb; // r29+0xf0
        class Vector3D sa; // r29+0xe0
        float bradSquared; // r29+0x110
        float newDistSquared; // 
        float distSquared; // 
        unsigned int lineFloorMaskLo; // r22
        unsigned int lineFloorMaskHi; // r23
        unsigned int lineFloorMask; // r2
        class xLine2D line; // r29+0xb0
        signed int i; // r17
        signed int hit; // r2
        class iObjRuntime *r; // r16
    }
}

// 0x00370910 - 0x00370C20 (0x310) iPhysicsTrace2D_ACTOR__FP17tagxPhysicsObjectP8Vector3DP8Vector3DP8Vector3DPf
unsigned int iPhysicsTrace2D_ACTOR(class tagxPhysicsObject *o, class Vector3D *a, class Vector3D *b, class Vector3D *c, float *ct) {
    { // 0x00370910 - 0x00370C20
        class Vector3D sc; // r29+0xa0
        class Vector3D sb; // r29+0x90
        class Vector3D sa; // r29+0x80
        float bradSquared; // r29+0xb0
        float newDistSquared; // 
        float distSquared; // 
        class _xActor *actor; // r5
        signed int i; // r4
        signed int hit; // r2
        class iActorRuntime *r; // r3
    }
}

// 0x00370C20 - 0x003710F0 (0x4D0) iPhysicsTrace2D_NOWALKTRIGGER__FP17tagxPhysicsObjectP8Vector3DP8Vector3DP8Vector3DPf
unsigned int iPhysicsTrace2D_NOWALKTRIGGER(class tagxPhysicsObject *o, class Vector3D *a, class Vector3D *b, class Vector3D *c, float *ct) {
    { // 0x00370C20 - 0x003710F0
        unsigned int *maskp; // r22
        unsigned int mask; // r3
        unsigned int lineFloorMaskLo; // r23
        unsigned int lineFloorMaskHi; // r30
        unsigned int lineFloorMask; // r2
        class _RwV2d lineHitVector[2]; // r29+0x110
        signed int hittrigger; // r18
        signed int j; // r2
        signed int i; // r17
        signed int hit; // r16
        class xTrigger *idx; // r2
        class xLine2D line; // r29+0xe0
    }
}

// 0x003710F0 - 0x00371724 (0x634) iPhysicsTrace2D_WALKTRIGGER__FP17tagxPhysicsObjectP8Vector3DP8Vector3DP8Vector3DPf
unsigned int iPhysicsTrace2D_WALKTRIGGER(class tagxPhysicsObject *o, class Vector3D *a, class Vector3D *b, class Vector3D *c, float *ct) {
    { // 0x003710F0 - 0x00371724
        class Vector3D p2d; // r29+0x140
        class Vector3D p2d; // r29+0x130
        unsigned int mask; // r3
        unsigned int lineFloorMaskLo; // r23
        unsigned int lineFloorMaskHi; // r30
        unsigned int lineFloorMask; // r2
        class _RwV2d lineHitVector[2]; // r29+0x120
        unsigned int i; // r19
        signed int vis; // r2
        class xSpan *span; // r3
        class xSpanBuffer sb; // r29+0xf0
        class xTrigger *idx; // r18
        class xTrigger *end; // r17
        class xTrigger *start; // r16
        class xLine2D line; // r29+0xc0
    }
}

// 0x00371730 - 0x00371968 (0x238) iPhysicsTrace2D__FP17tagxPhysicsObjectP8Vector3DP8Vector3DP8Vector3DPf
unsigned int iPhysicsTrace2D(class tagxPhysicsObject *o, class Vector3D *a, class Vector3D *b, class Vector3D *c, float *ct) {
    { // 0x00371730 - 0x00371968
    }
}

// 0x00371970 - 0x00371AB0 (0x140) iPhysicsObjectInitialize__FP17tagxPhysicsObject
void iPhysicsObjectInitialize(class tagxPhysicsObject *p) {
    { // 0x00371970 - 0x00371AB0
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iParMgr.cpp
// ==========================================================================
void (iParMgrRenderParSys_ExpandingRing)(class tagxParSys *); // 0x0037E590
void (iParMgrRenderParSys_ExpandingSphere)(class tagxParSys *); // 0x0037DB80
void (iParMgrRenderParSys_DrawLight)(class tagxParSys *); // 0x0037D750
void (iParMgrRenderParSys_DrawFlatPoly)(class tagxParSys *); // 0x0037ED10
void (iParMgrRenderParSys_DrawBulletStreak)(class tagxParSys *); // 0x0037F060
void (iParMgrRenderParSys_DrawRegionRain)(class tagxParSys *); // 0x0037EF30
void (iParMgrRenderParSys_Sprite)(class tagxParSys *); // 0x0037D660
void (iParMgrRenderParSys_Twinkle)(class tagxParSys *); // 0x0037D550
void (iParMgrRenderParSys_TwoTriStrip)(class tagxParSys *); // 0x0037F250
class tagxParSys gParSys[37]; // 0x004B94B0
float g_PI; // 0x00485A40
float g_PI_DIV_2; // 0x00485A50
float g_2_PI; // 0x00485A48
float gFloorQuickHeight[32][32]; // 0x00AF9BA0
class xScene *xScene_current; // 0x004A4E08
float (*matrix_sp)[16]; // 0x004A4F90
float proj_farnear; // 0x004A4F6C
float SCALE; // 0x00000000
float proj_fovr; // 0x00000000
float CX; // 0x00000000
float CY; // 0x00000000
float g_EPSILON; // 0x00485A68
float g_DEGTORAD; // 0x00485A58
class xParCmd *gParEmitter[37]; // 0x004B98C0
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagParCmd_Jet *gJet; // 0x004A4F08
class _iParSphereMode sSphereMode[7]; // 0x0044FC80
class _iParRingMode sRingInfo[14]; // 0x0044FA50
static float gColorTable[256]; // 0x00AAE160
static signed int gColorTableInit; // 0x004A4F04
class _xTex *gParTex[37]; // 0x00AAE0C0
void (*gParDrawFunc[37])(class tagxParSys *); // 0x00AAE020
static char dbfont_chars[51]; // 0x00000000
// 0x00371AB0 - 0x00371E2C (0x37C) iParIniVFXDaggerAttack__FP10tagxParSysP13tagParCmdList
void iParIniVFXDaggerAttack(class tagParCmdList *list) {
    { // 0x00371AB0 - 0x00371E2C
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00371E30 - 0x00372628 (0x7F8) iParIniVFXFireBall__FP10tagxParSysP13tagParCmdList
void iParIniVFXFireBall(class tagParCmdList *list) {
    { // 0x00371E30 - 0x00372628
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_SpawnVFX *d; // r17
        class tagParCmd_SpawnVFX *d; // r17
        class tagParCmd_KillPlane *d; // r17
        class tagParCmd_Move *d; // r17
        class tagParCmd_Accelerate *d; // r17
        class tagParCmd_Texture *d; // r18
        class tagParCmd_Emitter *d; // r17
        class xParCmd *cmd; // r17
    }
}

// 0x00372630 - 0x00372A64 (0x434) iParIniVFXSplash__FP10tagxParSysP13tagParCmdList
void iParIniVFXSplash(class tagParCmdList *list) {
    { // 0x00372630 - 0x00372A64
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00372A70 - 0x00372EA4 (0x434) iParIniVFXEnergyBlow__FP10tagxParSysP13tagParCmdList
void iParIniVFXEnergyBlow(class tagParCmdList *list) {
    { // 0x00372A70 - 0x00372EA4
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00372EB0 - 0x003732E4 (0x434) iParIniVFXEnergySuck__FP10tagxParSysP13tagParCmdList
void iParIniVFXEnergySuck(class tagParCmdList *list) {
    { // 0x00372EB0 - 0x003732E4
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x003732F0 - 0x00373724 (0x434) iParIniVFXMoltenLeadPour__FP10tagxParSysP13tagParCmdList
void iParIniVFXMoltenLeadPour(class tagParCmdList *list) {
    { // 0x003732F0 - 0x00373724
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00373730 - 0x00373B64 (0x434) iParIniVFXWaterPour__FP10tagxParSysP13tagParCmdList
void iParIniVFXWaterPour(class tagParCmdList *list) {
    { // 0x00373730 - 0x00373B64
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00373B70 - 0x00373FA4 (0x434) iParIniVFXLightShaft__FP10tagxParSysP13tagParCmdList
void iParIniVFXLightShaft(class tagParCmdList *list) {
    { // 0x00373B70 - 0x00373FA4
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00373FB0 - 0x00374318 (0x368) iParIniVFXExpandingRing__FP10tagxParSysP13tagParCmdList
void iParIniVFXExpandingRing(class tagParCmdList *list) {
    { // 0x00373FB0 - 0x00374318
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00374320 - 0x00374754 (0x434) iParIniVFXExpandingSphere__FP10tagxParSysP13tagParCmdList
void iParIniVFXExpandingSphere(class tagParCmdList *list) {
    { // 0x00374320 - 0x00374754
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00374760 - 0x00374BB8 (0x458) iParIniVFXTwinkle__FP10tagxParSysP13tagParCmdList
void iParIniVFXTwinkle(class tagParCmdList *list) {
    { // 0x00374760 - 0x00374BB8
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00374BC0 - 0x0037502C (0x46C) iParIniVFXLight__FP10tagxParSysP13tagParCmdList
void iParIniVFXLight(class tagParCmdList *list) {
    { // 0x00374BC0 - 0x0037502C
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00375030 - 0x00375490 (0x460) iParIniVFXFootprint__FP10tagxParSysP13tagParCmdList
void iParIniVFXFootprint(class tagParCmdList *list) {
    { // 0x00375030 - 0x00375490
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00375490 - 0x00375908 (0x478) iParIniVFXBreath__FP10tagxParSysP13tagParCmdList
void iParIniVFXBreath(class tagParCmdList *list) {
    { // 0x00375490 - 0x00375908
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00375910 - 0x00375F54 (0x644) iParIniVFXSpark__FP10tagxParSysP13tagParCmdList
void iParIniVFXSpark(class tagParCmdList *list) {
    { // 0x00375910 - 0x00375F54
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Bounce *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Accelerate *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00375F60 - 0x003765CC (0x66C) iParIniVFXRegionFog__FP10tagxParSysP13tagParCmdList
void iParIniVFXRegionFog(class tagParCmdList *list) {
    { // 0x00375F60 - 0x003765CC
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Bounce *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Accelerate *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x003765D0 - 0x00376E44 (0x874) iParIniVFXRegionLeaves__FP10tagxParSysP13tagParCmdList
void iParIniVFXRegionLeaves(class tagParCmdList *list) {
    { // 0x003765D0 - 0x00376E44
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Bounce *d; // r17
        class tagParCmd_Move *d; // r17
        class tagParCmd_Turbulence *d; // r17
        class tagParCmd_Jet *d; // r17
        class tagParCmd_Accelerate *d; // r17
        class tagParCmd_Texture *d; // r18
        class tagParCmd_Emitter *d; // r17
        class xParCmd *cmd; // r17
    }
}

// 0x00376E50 - 0x00377480 (0x630) iParIniVFXRegionDust__FP10tagxParSysP13tagParCmdList
void iParIniVFXRegionDust(class tagParCmdList *list) {
    { // 0x00376E50 - 0x00377480
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_OrbitPoint *d; // r16
        class tagParCmd_RandomDisplace *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00377480 - 0x00377C5C (0x7DC) iParIniVFXRegionRain__FP10tagxParSysP13tagParCmdList
void iParIniVFXRegionRain(class tagParCmdList *list) {
    { // 0x00377480 - 0x00377C5C
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_KillPlane *d; // r17
        class tagParCmd_KillOld *d; // r17
        class tagParCmd_KillInvis *d; // r17
        class tagParCmd_Move *d; // r17
        class tagParCmd_Accelerate *d; // r17
        class tagParCmd_Texture *d; // r18
        class tagParCmd_Emitter *d; // r17
        class xParCmd *cmd; // r17
    }
}

// 0x00377C60 - 0x003780E4 (0x484) iParIniVFXRainSplatter__FP10tagxParSysP13tagParCmdList
void iParIniVFXRainSplatter(class tagParCmdList *list) {
    { // 0x00377C60 - 0x003780E4
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x003780F0 - 0x00378704 (0x614) iParIniVFXMagicPortal__FP10tagxParSysP13tagParCmdList
void iParIniVFXMagicPortal(class tagParCmdList *list) {
    { // 0x003780F0 - 0x00378704
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_RotMove *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00378710 - 0x00378B90 (0x480) iParIniVFXBloodPool__FP10tagxParSysP13tagParCmdList
void iParIniVFXBloodPool(class tagParCmdList *list) {
    { // 0x00378710 - 0x00378B90
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x00378B90 - 0x0037941C (0x88C) iParIniVFXBlackVomit__FP10tagxParSysP13tagParCmdList
void iParIniVFXBlackVomit(class tagParCmdList *list) {
    { // 0x00378B90 - 0x0037941C
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_KillOld *d; // r17
        class tagParCmd_RandomDisplace *d; // r17
        class tagParCmd_Bounce *d; // r17
        class tagParCmd_Move *d; // r17
        class tagParCmd_Accelerate *d; // r17
        class tagParCmd_Texture *d; // r18
        class tagParCmd_Emitter *d; // r17
        class xParCmd *cmd; // r17
    }
}

// 0x00379420 - 0x00379C74 (0x854) iParIniVFXExplodingEarth__FP10tagxParSysP13tagParCmdList
void iParIniVFXExplodingEarth(class tagParCmdList *list) {
    { // 0x00379420 - 0x00379C74
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_KillOld *d; // r17
        class tagParCmd_Bounce *d; // r17
        class tagParCmd_Move *d; // r17
        class tagParCmd_Accelerate *d; // r17
        class tagParCmd_Texture *d; // r18
        class tagParCmd_Emitter *d; // r17
        class xParCmd *cmd; // r17
    }
}

// 0x00379C80 - 0x0037A2BC (0x63C) iParIniVFXBloodSlash__FP10tagxParSysP13tagParCmdList
void iParIniVFXBloodSlash(class tagParCmdList *list) {
    { // 0x00379C80 - 0x0037A2BC
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_KillPlane *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Accelerate *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037A2C0 - 0x0037A8FC (0x63C) iParIniVFXBloodDrip__FP10tagxParSysP13tagParCmdList
void iParIniVFXBloodDrip(class tagParCmdList *list) {
    { // 0x0037A2C0 - 0x0037A8FC
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_KillPlane *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Accelerate *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037A900 - 0x0037AF48 (0x648) iParIniVFXBloodSpurt__FP10tagxParSysP13tagParCmdList
void iParIniVFXBloodSpurt(class tagParCmdList *list) {
    { // 0x0037A900 - 0x0037AF48
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_KillPlane *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Accelerate *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037AF50 - 0x0037B3B8 (0x468) iParIniVFXFire__FP10tagxParSysP13tagParCmdList
void iParIniVFXFire(class tagParCmdList *list) {
    { // 0x0037AF50 - 0x0037B3B8
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037B3C0 - 0x0037B824 (0x464) iParIniVFXMuzzleFlash__FP10tagxParSysP13tagParCmdList
void iParIniVFXMuzzleFlash(class tagParCmdList *list) {
    { // 0x0037B3C0 - 0x0037B824
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037B830 - 0x0037BF4C (0x71C) iParIniVFXBoneFragment__FP10tagxParSysP13tagParCmdList
void iParIniVFXBoneFragment(class tagParCmdList *list) {
    { // 0x0037B830 - 0x0037BF4C
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Bounce *d; // r17
        class tagParCmd_Move *d; // r17
        class tagParCmd_RotMove *d; // r17
        class tagParCmd_Accelerate *d; // r17
        class tagParCmd_Texture *d; // r18
        class tagParCmd_Emitter *d; // r17
        class xParCmd *cmd; // r17
    }
}

// 0x0037BF50 - 0x0037C5D4 (0x684) iParIniVFXShellEject__FP10tagxParSysP13tagParCmdList
void iParIniVFXShellEject(class tagParCmdList *list) {
    { // 0x0037BF50 - 0x0037C5D4
        class tagParCmd_Bounce *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Jet *d; // r16
        class tagParCmd_Accelerate *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037C5E0 - 0x0037CB34 (0x554) iParIniVFXBulletStreak__FP10tagxParSysP13tagParCmdList
void iParIniVFXBulletStreak(class tagParCmdList *list) {
    { // 0x0037C5E0 - 0x0037CB34
        class tagParCmd_SpawnVFX *d; // r16
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037CB40 - 0x0037D0B0 (0x570) iParIniVFXGroundPuff__FP10tagxParSysP13tagParCmdList
void iParIniVFXGroundPuff(class tagParCmdList *list) {
    { // 0x0037CB40 - 0x0037D0B0
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Accelerate *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037D0B0 - 0x0037D54C (0x49C) iParIniVFXSmoke__FP10tagxParSysP13tagParCmdList
void iParIniVFXSmoke(class tagParCmdList *list) {
    { // 0x0037D0B0 - 0x0037D54C
        class tagParCmd_KillOld *d; // r16
        class tagParCmd_Move *d; // r16
        class tagParCmd_Texture *d; // r17
        class tagParCmd_Emitter *d; // r16
        class xParCmd *cmd; // r16
    }
}

// 0x0037D550 - 0x0037D65C (0x10C) iParMgrRenderParSys_Twinkle__FP10tagxParSys
void iParMgrRenderParSys_Twinkle(class tagxParSys *ps) {
    { // 0x0037D550 - 0x0037D65C
        float dy; // r29+0x20
        float dx; // r29+0x20
        float z; // 
        float y; // r29+0x20
        float x; // r29+0x20
        class xPar *idx; // r16
    }
}

// 0x0037D660 - 0x0037D74C (0xEC) iParMgrRenderParSys_Sprite__FP10tagxParSys
void iParMgrRenderParSys_Sprite(class tagxParSys *ps) {
    { // 0x0037D660 - 0x0037D74C
        class Vector3D v[3]; // r29+0x20
        float z; // r29+0x50
        float y; // r29+0x50
        float x; // r29+0x50
        class xPar *idx; // r16
    }
}

// 0x0037D750 - 0x0037DB74 (0x424) iParMgrRenderParSys_DrawLight__FP10tagxParSys
void iParMgrRenderParSys_DrawLight(class tagxParSys *ps) {
    { // 0x0037D750 - 0x0037DB74
        float plane; // r29+0x70
        signed int zbit; // r5
        signed int xbit; // r18
        float a; // r29+0x70
        float b; // r29+0x70
        float g; // r29+0x70
        float r; // r29+0x70
        float hsize; // r29+0x70
        class tagiFloor *f; // r17
        float zow; // 
        float xow; // 
        class xPar *idx; // r16
    }
}

// 0x0037DB80 - 0x0037E590 (0xA10) iParMgrRenderParSys_ExpandingSphere__FP10tagxParSys
void iParMgrRenderParSys_ExpandingSphere(class tagxParSys *ps) {
    { // 0x0037DB80 - 0x0037E590
        float ca3; // r29+0xb0
        float sa3; // 
        float ca2; // r29+0xb0
        float sa2; // 
        float ca1; // 
        float sa1; // 
        float r2; // 
        float r1; // 
        float a3; // 
        float a2; // 
        float a1; // 
        float h2; // 
        float h1; // 
        float radius; // r29+0xac
        float alpha; // r29+0xb0
        float blue; // r29+0xb0
        float green; // r29+0xb0
        float red; // r29+0xb0
        class _iParSphereMode *o; // r19
        class xPar *p; // r18
        unsigned int j; // r17
        signed int i; // r16
    }
}

// 0x0037E590 - 0x0037ED0C (0x77C) iParMgrRenderParSys_ExpandingRing__FP10tagxParSys
void iParMgrRenderParSys_ExpandingRing(class tagxParSys *ps) {
    { // 0x0037E590 - 0x0037ED0C
        float cosval; // r29+0xc0
        float sinval; // 
        float angle; // 
        float tilt; // 
        float height; // r29+0xbc
        float oradius; // r29+0xb8
        float radius; // r29+0xb4
        float alpha; // r29+0xc0
        float blue; // r29+0xc0
        float green; // r29+0xc0
        float red; // r29+0xc0
        class _iParRingMode *m; // r18
        class xPar *p; // r17
        signed int i; // r16
    }
}

// 0x0037ED10 - 0x0037EF2C (0x21C) iParMgrRenderParSys_DrawFlatPoly__FP10tagxParSys
void iParMgrRenderParSys_DrawFlatPoly(class tagxParSys *ps) {
    { // 0x0037ED10 - 0x0037EF2C
        float a; // r29+0x20
        float b; // r29+0x20
        float g; // r29+0x20
        float r; // r29+0x20
        float hsize; // r29+0x20
        class xPar *idx; // r16
    }
}

// 0x0037EF30 - 0x0037F054 (0x124) iParMgrRenderParSys_DrawRegionRain__FP10tagxParSys
void iParMgrRenderParSys_DrawRegionRain(class tagxParSys *ps) {
    { // 0x0037EF30 - 0x0037F054
        class Vector3D v[3]; // r29+0x20
        float a; // r29+0x50
        float b; // r29+0x50
        float g; // r29+0x50
        float r; // r29+0x50
        class xPar *idx; // r16
    }
}

// 0x0037F060 - 0x0037F244 (0x1E4) iParMgrRenderParSys_DrawBulletStreak__FP10tagxParSys
void iParMgrRenderParSys_DrawBulletStreak(class tagxParSys *ps) {
    { // 0x0037F060 - 0x0037F244
        float a; // r29+0x20
        float b; // r29+0x20
        float g; // r29+0x20
        float r; // r29+0x20
        class xPar *idx; // r16
    }
}

// 0x0037F250 - 0x0037F480 (0x230) iParMgrRenderParSys_TwoTriStrip__FP10tagxParSys
void iParMgrRenderParSys_TwoTriStrip(class tagxParSys *ps) {
    { // 0x0037F250 - 0x0037F480
        float a; // r29+0x30
        float b; // r29+0x30
        float g; // r29+0x30
        float r; // r29+0x30
        float hsize; // 
        class xPar *idx; // r16
    }
}

// 0x0037F480 - 0x0037F7E0 (0x360) iParComputeUnitInfo__FP17tagParCmd_Texture
void iParComputeUnitInfo(class tagParCmd_Texture *d) {
    { // 0x0037F480 - 0x0037F7E0
    }
}

// 0x0037F7E0 - 0x0037F8B0 (0xD0) iParMgrRender__Fv
void iParMgrRender() {
    { // 0x0037F7E0 - 0x0037F8B0
        float m[16]; // r29+0x20
        signed int i; // r16
    }
}

// 0x0037F8B0 - 0x0037FB60 (0x2B0) iParMgrInit__Fv
void iParMgrInit() {
    { // 0x0037F8B0 - 0x0037FB60
        unsigned int assetID; // r2
        signed int i; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iPad.cpp
// ==========================================================================
class tagPadMgrData console; // 0x004DF540
void (p2PadSetVibration)(class tagxPad *, unsigned int, unsigned int); // 0x00380D70
signed int (checkRumbleAbility)(signed int); // 0x00380C50
float gTimeSec; // 0x004A4C8C
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int g_sceneSwapPending; // 0x004A4CCC
class __xScrFxMgr gScrFx; // 0x004B5368
class xScene *xScene_current; // 0x004A4E08
unsigned long gTimerValue; // 0x004A5340
unsigned int g_maxPlayerRotateSpeed; // 0x004A4644
unsigned int g_currentPlayerRotateSpeed; // 0x004A4648
static unsigned short sPadMapping_Game[16]; // 0x0044FDA0
static unsigned short sPadMapping_Debug[16]; // 0x00000000
static signed int accTable[15]; // 0x0044FD50
unsigned int gSoftReset; // 0x004A4F14
signed int sNumPads; // 0x004A4F10
unsigned int gLastInputTime; // 0x004A4F0C
static unsigned char sPadData[2][32]; // 0x00AAE780
static u_long128 sPadDMA[2][16]; // 0x00AAE580
// 0x0037FB60 - 0x0037FBA4 (0x44) iPadInitFunc_Game__FP7tagxPad
void iPadInitFunc_Game(class tagxPad *p) {
    { // 0x0037FB60 - 0x0037FBA4
        signed int i; // r6
    }
}

// 0x0037FBB0 - 0x00380A14 (0xE64) iConsoleMgrRead__FP7tagxPadiUl
signed int iConsoleMgrRead(class tagxPad *pad, signed int numPads, unsigned long theTime) {
    { // 0x0037FBB0 - 0x00380A14
        class _XBUTTON *b; // r6
        class _XBUTTON *b; // r2
        class _tagNativePadData *dp; // r5
        class tagxPad *pp; // r4
        class _tagNativePadData *dp; // r18
        class tagxPad *pp; // r2
        signed int startErrorTime; // r2
        signed int allowMessages; // r29+0xdc
        unsigned int db; // r2
        class xFontBox_t *box; // r19
        class _tagNativePadData *dp; // r18
        class tagxPad *pp; // r2
        signed int last_ry; // r22
        signed int last_rx; // r23
        signed int last_ly; // r30
        signed int last_lx; // r29+0xc0
        signed int controllerCount; // r17
        signed int j; // r3
        signed int i; // r16
        unsigned int buttons_held; // r2
        static signed int firstRead; // 0x004A4F20
        static signed int readIndexMap[2]; // 0x0044FD90
        static signed int killRumble; // 0x004A4F18
        static signed char init; // 0x004A4F24
        static signed char init; // 0x004A4F1C
    }
}

// 0x00380A20 - 0x00380A28 (0x8) iConsoleMgrIdle__FUl
void iConsoleMgrIdle() {
    { // 0x00380A20 - 0x00380A28
    }
}

// 0x00380A30 - 0x00380C00 (0x1D0) iConsoleMgrInit__FiP7tagxPad
signed short iConsoleMgrInit(signed int maxControllers, class tagxPad *pad) {
    { // 0x00380A30 - 0x00380C00
        class tagxPad *p; // r17
        signed int i; // r16
    }
}

// 0x00380C00 - 0x00380C50 (0x50) iConsoleMgrRegisterScriptCalls__Fv
void iConsoleMgrRegisterScriptCalls() {
    { // 0x00380C00 - 0x00380C50
    }
}

// 0x00380C50 - 0x00380D68 (0x118) checkRumbleAbility__Fi
signed int checkRumbleAbility(signed int port) {
    { // 0x00380C50 - 0x00380D68
    }
}

// 0x00380D70 - 0x00380E90 (0x120) p2PadSetVibration__FP7tagxPadUiUi
void p2PadSetVibration(class tagxPad *pad, unsigned int act0, unsigned int act1) {
    { // 0x00380D70 - 0x00380E90
        signed int slot; // r16
        signed int port; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iObj.cpp
// ==========================================================================
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int gObjHackRenderTranslucent; // 0x004A4F2C
float gObjShadowHeight; // 0x004A4F28
// 0x00380E90 - 0x00381430 (0x5A0) iObjDraw__FP5_xObj
void iObjDraw(class _xObj *obj) {
    { // 0x00380E90 - 0x00381430
        class tagxPhysicsObject *p; // r16
        float *fp; // r2
        float m[16]; // r29+0x60
        class iObjRuntime *r; // r18
        class Vector3D pos; // r29+0xa0
    }
}

// 0x00381430 - 0x00381474 (0x44) iObjInitialize__FP5_xObj
void iObjInitialize(class _xObj *a) {
    { // 0x00381430 - 0x00381474
        class iObjRuntime *r; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iNavPnt.cpp
// ==========================================================================
// 0x00381480 - 0x003814A4 (0x24) iNavPointLineOfSightPoint__FP9xNavPointP8Vector3Di
signed int iNavPointLineOfSightPoint(class xNavPoint *np, class Vector3D *p, signed int physObj) {
    { // 0x00381480 - 0x003814A4
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iModel.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x003814B0 - 0x00381630 (0x180) iModelAssign__FP7_xActorP7_xModel
void iModelAssign(class _xActor *actor, class _xModel *model) {
    { // 0x003814B0 - 0x00381630
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\imesh.cpp
// ==========================================================================
// 0x00381630 - 0x00381748 (0x118) p2MeshInit__FP9tagP2MeshiP11tagP2VERTEXP8Vector3DP15tagP2MeshVertexiPUsP9tagP2Mesh
void p2MeshInit(class tagP2Mesh *mesh, signed int vertexCount, class tagP2VERTEX *p2vPtr, class Vector3D *normals, class tagP2MeshVertex *vPtr, signed int indexCount, unsigned short *iPtr, class tagP2Mesh *next) {
    { // 0x00381630 - 0x00381748
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iMatrix.cpp
// ==========================================================================
float g_EPSILON; // 0x00485A68
// 0x00381750 - 0x003818D0 (0x180) iMatrixVectorMatrixMultiply__FP8Vector3DP8Vector3DP9tagMATRIX
signed int iMatrixVectorMatrixMultiply(class Vector3D *vDest, class Vector3D *vSrc, class tagMATRIX *mat) {
    { // 0x00381750 - 0x003818D0
        float w; // 
        float z; // 
        float y; // 
        float x; // 
    }
}

// 0x003818D0 - 0x003819D4 (0x104) iMatrixMatrixMultiply__FP9tagMATRIXP9tagMATRIXP9tagMATRIX
void iMatrixMatrixMultiply(class tagMATRIX *q, class tagMATRIX *a, class tagMATRIX *b) {
    { // 0x003818D0 - 0x003819D4
        float pM[16]; // r29+0x40
        float *pB; // r17
        float *pA; // r16
        signed int k; // r6
        signed int j; // r5
        signed int i; // r4
    }
}

// 0x003819E0 - 0x00381A94 (0xB4) iMatrixSetRotateZMatrixRIGHT__FP9tagMATRIXf
void iMatrixSetRotateZMatrixRIGHT(class tagMATRIX *m, float fRads) {
    { // 0x003819E0 - 0x00381A94
        float sinr; // 
        float cosr; // 
    }
}

// 0x00381AA0 - 0x00381B54 (0xB4) iMatrixSetRotateZMatrix__FP9tagMATRIXf
void iMatrixSetRotateZMatrix(class tagMATRIX *m, float fRads) {
    { // 0x00381AA0 - 0x00381B54
        float sinr; // 
        float cosr; // 
    }
}

// 0x00381B60 - 0x00381C14 (0xB4) iMatrixSetRotateYMatrixRIGHT__FP9tagMATRIXf
void iMatrixSetRotateYMatrixRIGHT(class tagMATRIX *m, float fRads) {
    { // 0x00381B60 - 0x00381C14
        float sinr; // 
        float cosr; // 
    }
}

// 0x00381C20 - 0x00381CD4 (0xB4) iMatrixSetRotateYMatrix__FP9tagMATRIXf
void iMatrixSetRotateYMatrix(class tagMATRIX *m, float fRads) {
    { // 0x00381C20 - 0x00381CD4
        float sinr; // 
        float cosr; // 
    }
}

// 0x00381CE0 - 0x00381D94 (0xB4) iMatrixSetRotateXMatrixRIGHT__FP9tagMATRIXf
void iMatrixSetRotateXMatrixRIGHT(class tagMATRIX *m, float fRads) {
    { // 0x00381CE0 - 0x00381D94
        float sinr; // 
        float cosr; // 
    }
}

// 0x00381DA0 - 0x00381E54 (0xB4) iMatrixSetRotateXMatrix__FP9tagMATRIXf
void iMatrixSetRotateXMatrix(class tagMATRIX *m, float fRads) {
    { // 0x00381DA0 - 0x00381E54
        float sinr; // 
        float cosr; // 
    }
}

// 0x00381E60 - 0x00381ECC (0x6C) iMatrixSetScaleMatrix__FP9tagMATRIXfff
void iMatrixSetScaleMatrix(class tagMATRIX *m, float sx, float sy, float sz) {
    { // 0x00381E60 - 0x00381ECC
    }
}

// 0x00381ED0 - 0x00382028 (0x158) iMatrixCalcShadowMatrix__FP9tagMATRIXPfP8Vector3D
void iMatrixCalcShadowMatrix(class tagMATRIX *shadowMat, float *groundPlane, class Vector3D *lightPos) {
    { // 0x00381ED0 - 0x00382028
        float dot; // r29+0x0
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iMath.cpp
// ==========================================================================
float g_EPSILON; // 0x00485A68
float g_HUGE; // 0x00000000
float g_RADTODEG; // 0x00485A60
float g_DEGTORAD; // 0x00485A58
float g_INV_PI; // 0x00000000
float g_PI_DIV_4; // 0x00000000
float g_PI_DIV_2; // 0x00485A50
float g_2_PI; // 0x00485A48
float g_PI; // 0x00485A40
// 0x00382030 - 0x003820D8 (0xA8) iMathFindPlane__FPfP8Vector3DP8Vector3DP8Vector3D
void iMathFindPlane(float *plane, class Vector3D *p0, class Vector3D *p1, class Vector3D *p2) {
    { // 0x00382030 - 0x003820D8
    }
}

// 0x003820E0 - 0x003825B4 (0x4D4) iMathPointInArc__FP8Vector3DP8Vector3Dfff
signed int iMathPointInArc(class Vector3D *p, class Vector3D *arcpos, float arclength, float arcrad1, float arcrad2) {
    { // 0x003820E0 - 0x003825B4
        float distance; // r29+0x50
        signed int thetaGood; // r16
        float theta; // r29+0x50
    }
}

// 0x003825C0 - 0x00382650 (0x90) iMathAngleOfVectorXZ__FP8Vector3D
float iMathAngleOfVectorXZ(class Vector3D *v) {
    { // 0x003825C0 - 0x00382650
        float angle; // r29+0x30
        float ydiff; // r29+0x30
        float xdiff; // 
    }
}

// 0x00382650 - 0x003826F8 (0xA8) iMathRotateXYZVectorRIGHT__FP8Vector3DP8Vector3DP8Vector3D
void iMathRotateXYZVectorRIGHT(class Vector3D *rot, class Vector3D *v, class Vector3D *result) {
    { // 0x00382650 - 0x003826F8
        class tagMATRIX m2; // r29+0x140
        class tagMATRIX m1; // r29+0x100
        class tagMATRIX mz; // r29+0xc0
        class tagMATRIX my; // r29+0x80
        class tagMATRIX mx; // r29+0x40
    }
}

// 0x00382700 - 0x003827A8 (0xA8) iMathRotateXYZVector__FP8Vector3DP8Vector3DP8Vector3D
void iMathRotateXYZVector(class Vector3D *rot, class Vector3D *v, class Vector3D *result) {
    { // 0x00382700 - 0x003827A8
        class tagMATRIX m2; // r29+0x140
        class tagMATRIX m1; // r29+0x100
        class tagMATRIX mz; // r29+0xc0
        class tagMATRIX my; // r29+0x80
        class tagMATRIX mx; // r29+0x40
    }
}

// 0x003827B0 - 0x003829F0 (0x240) iMathIntersectLine2DLine2D__FP7xLine2DP7xLine2DP6_RwV2d
signed int iMathIntersectLine2DLine2D(class xLine2D *a, class xLine2D *b, class _RwV2d *i) {
    { // 0x003827B0 - 0x003829F0
        float denom; // 
        float s; // 
        float num2; // r29+0x60
        float r; // 
        float num; // r29+0x60
        float YD; // 
        float XD; // 
        float YC; // 
        float XC; // 
        float YB; // 
        float XB; // 
        float YA; // 
        float XA; // 
        float ib; // r29+0x60
        float ia; // 
    }
}

// 0x003829F0 - 0x00382BCC (0x1DC) iMathIntersectVector2DCircle__F6_RwV2d6_RwV2d6_RwV2dfPfPf
float iMathIntersectVector2DCircle(class _RwV2d ray, class _RwV2d ray_dir, class _RwV2d center, float r, float *rt1, float *rt2) {
    { // 0x003829F0 - 0x00382BCC
        float t2; // r29+0x60
        float t1; // 
        float oo_2a; // 
        float sd; // r29+0x60
        float discriminant; // r29+0x60
        float c; // 
        float b; // 
        float a; // 
        float m; // r29+0x60
        float l; // r29+0x60
        float j; // r29+0x60
        float i; // r29+0x60
    }
}

// 0x00382BD0 - 0x0038314C (0x57C) iMathPointIn2DPoly4__FP8Vector3DP8Vector3DP8Vector3DP8Vector3DP8Vector3D
signed int iMathPointIn2DPoly4(class Vector3D *point, class Vector3D *v1, class Vector3D *v2, class Vector3D *v3, class Vector3D *v4) {
    { // 0x00382BD0 - 0x0038314C
        class Vector3D tmpVec; // r29+0xd0
        float d; // 
        class Vector3D up; // r29+0xc0
        class Vector3D p[4]; // r29+0x90
        class Vector3D n[4]; // r29+0x60
        class Vector3D u[4]; // r29+0x30
        class Vector3D vec[4]; // r29+0x0
        float tmpY; // r29+0xe0
        signed int i; // r2
    }
}

// 0x00383150 - 0x00383354 (0x204) iMathPointIn2DRect__FP8Vector3DP8Vector3DP8Vector3D
signed int iMathPointIn2DRect(class Vector3D *point, class Vector3D *v1, class Vector3D *v2) {
    { // 0x00383150 - 0x00383354
    }
}

// 0x00383360 - 0x00383830 (0x4D0) iMathPointIn2DTri__FP8Vector3DP8Vector3DP8Vector3DP8Vector3D
signed int iMathPointIn2DTri(class Vector3D *point, class Vector3D *v1, class Vector3D *v2, class Vector3D *v3) {
    { // 0x00383360 - 0x00383830
        float tmpY; // 
        class Vector3D tmpVec; // r29+0x80
        signed int i; // r2
        float d; // r29+0x90
        class Vector3D up; // r29+0x70
        class Vector3D p[3]; // r29+0x40
        class Vector3D n[3]; // r29+0x10
    }
}

// 0x00383830 - 0x0038385C (0x2C) iMathVector2DSub__FP6_RwV2dP6_RwV2dP6_RwV2d
class _RwV2d *iMathVector2DSub(class _RwV2d *a, class _RwV2d *b, class _RwV2d *result) {
    { // 0x00383830 - 0x0038385C
    }
}

// 0x00383860 - 0x003838DC (0x7C) iMathVector2DMagnitude__FP6_RwV2d
float iMathVector2DMagnitude(class _RwV2d *v) {
    { // 0x00383860 - 0x003838DC
    }
}

// 0x003838E0 - 0x0038391C (0x3C) iMathVectorMul__FP8Vector3DP8Vector3DP8Vector3D
class Vector3D *iMathVectorMul(class Vector3D *a, class Vector3D *b, class Vector3D *result) {
    { // 0x003838E0 - 0x0038391C
    }
}

// 0x00383920 - 0x0038394C (0x2C) iMathVectorSubXZ__FP8Vector3DP8Vector3DP8Vector3D
class Vector3D *iMathVectorSubXZ(class Vector3D *a, class Vector3D *b, class Vector3D *r) {
    { // 0x00383920 - 0x0038394C
    }
}

// 0x00383950 - 0x0038398C (0x3C) iMathVectorSub__FP8Vector3DP8Vector3DP8Vector3D
class Vector3D *iMathVectorSub(class Vector3D *a, class Vector3D *b, class Vector3D *result) {
    { // 0x00383950 - 0x0038398C
    }
}

// 0x00383990 - 0x00383A1C (0x8C) iMathVectorMagnitude__FP8Vector3D
float iMathVectorMagnitude(class Vector3D *v) {
    { // 0x00383990 - 0x00383A1C
    }
}

// 0x00383A20 - 0x00383AFC (0xDC) iMathVectorNormalize__FP8Vector3DP8Vector3D
class Vector3D *iMathVectorNormalize(class Vector3D *v, class Vector3D *retVal) {
    { // 0x00383A20 - 0x00383AFC
        float oodiv; // 
    }
}

// 0x00383B00 - 0x00383B48 (0x48) iMathVectorInv__FP8Vector3D
class Vector3D *iMathVectorInv(class Vector3D *a) {
    { // 0x00383B00 - 0x00383B48
    }
}

// 0x00383B50 - 0x00383BB0 (0x60) iMathCrossProduct__FP8Vector3DP8Vector3DP8Vector3D
class Vector3D *iMathCrossProduct(class Vector3D *a, class Vector3D *b, class Vector3D *c) {
    { // 0x00383B50 - 0x00383BB0
    }
}

// 0x00383BB0 - 0x00383BE0 (0x30) iMathDotProduct__FP8Vector3DP8Vector3D
float iMathDotProduct(class Vector3D *a, class Vector3D *b) {
    { // 0x00383BB0 - 0x00383BE0
    }
}

// 0x00383BE0 - 0x00383C10 (0x30) iMathVectorSquareMagnitude__FP8Vector3D
float iMathVectorSquareMagnitude(class Vector3D *v) {
    { // 0x00383BE0 - 0x00383C10
    }
}

// 0x00383C10 - 0x00383C74 (0x64) iMathSquareRoot__Ff
float iMathSquareRoot(float val) {
    { // 0x00383C10 - 0x00383C74
    }
}

// 0x00383C80 - 0x00383D80 (0x100) iMathAngleAdd__Fff
float iMathAngleAdd(float a, float b) {
    { // 0x00383C80 - 0x00383D80
    }
}

// 0x00383D80 - 0x00383E7C (0xFC) iMathAngleClamp__Ff
float iMathAngleClamp(float a) {
    { // 0x00383D80 - 0x00383E7C
    }
}

// 0x00383E80 - 0x00383E88 (0x8) iMathInit__Fv
void iMathInit() {
    { // 0x00383E80 - 0x00383E88
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iMain.cpp
// ==========================================================================
signed int gDebugDrawDebugInfo; // 0x004A4E1C
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int g_sceneSwapPending; // 0x004A4CCC
unsigned int gSoftReset; // 0x004A4F14
unsigned char gCartBuffer[14400]; // 0x00A97F60
signed int g_scenePendingMarker; // 0x004A4CD8
class tagxActorDBData *gadb; // 0x004A4658
signed int gPlayerType; // 0x004A4E00
class Vector3D g_scenePendingAng; // 0x004B5378
unsigned int g_scenePendingScene; // 0x004A4CD0
float gTimeSec; // 0x004A4C8C
signed int gCartTime; // 0x004A4E74
class tag_xCameraControl gCamera; // 0x00AA67C0
signed int g_scenePendingCamera; // 0x004A4CD4
class xScene *xScene_current; // 0x004A4E08
class RWInfo gRWInfo; // 0x00B02B80
signed int gBootState; // 0x004A4E20
unsigned long lastTimerValue; // 0x004ACA30
float deltaFrame; // 0x004A4C88
unsigned long gTimerValue; // 0x004A5340
unsigned int gTime; // 0x004A4E10
unsigned int gTimeLastFrame; // 0x004A4E14
unsigned int gTimeDelta; // 0x004A4E18
unsigned long p2render_t0; // 0x00000000
unsigned int turbo; // 0x00000000
unsigned int afr; // 0x00000000
static unsigned int afr_ticks[30]; // 0x00000000
static unsigned int afr_ct; // 0x00000000
unsigned int frametime; // 0x004A4F40
unsigned char screen_dump; // 0x00000000
static class _Env environment[10]; // 0x00AAE7C0
static signed int envCount; // 0x004A4F3C
signed int gHaveRenderedCharactersAlready; // 0x004A4F38
static float bgz; // 0x00000000
static unsigned int gLastDoorCheck; // 0x00000000
unsigned int gScreenSaver; // 0x004A4F34
unsigned int gReadTimeout; // 0x00000000
unsigned int _vsync_count; // 0x00000000
signed int requestCartRestore; // 0x004A4660
static unsigned int texY; // 0x00000000
static unsigned int texX; // 0x00000000
signed int gQuit; // 0x004A4F30
static char dbfont_chars[51]; // 0x00000000
// 0x00383E90 - 0x00383F60 (0xD0) p2MainDoLevelSwap__Fv
void p2MainDoLevelSwap() {
    { // 0x00383E90 - 0x00383F60
    }
}

// 0x00383F60 - 0x00383F90 (0x30) iMainKill__Fv
void iMainKill() {
    { // 0x00383F60 - 0x00383F90
    }
}

// 0x00383F90 - 0x0038405C (0xCC) iMainInitGame__Fv
void iMainInitGame() {
    { // 0x00383F90 - 0x0038405C
    }
}

// 0x00384060 - 0x003840D4 (0x74) iMain__Fv
signed int iMain() {
    { // 0x00384060 - 0x003840D4
    }
}

// 0x003840E0 - 0x00384108 (0x28) iMainNextFrame__FUl
void iMainNextFrame() {
    { // 0x003840E0 - 0x00384108
    }
}

// 0x00384110 - 0x00384330 (0x220) iMainRender__Fv
void iMainRender() {
    { // 0x00384110 - 0x00384330
        unsigned int t1; // r17
        unsigned int t0; // r16
    }
}

// 0x00384330 - 0x003843D4 (0xA4) iInitEnvGet__FPc
char *iInitEnvGet(char *name) {
    { // 0x00384330 - 0x003843D4
        signed int env; // r16
    }
}

// 0x003843E0 - 0x00384400 (0x20) iInitEnvRead__FPci
void iInitEnvRead() {
    { // 0x003843E0 - 0x00384400
    }
}

// 0x00384400 - 0x00384934 (0x534) iInitEnvRead__Fv
void iInitEnvRead() {
    { // 0x00384400 - 0x00384934
        unsigned int numBytesRead; // r29+0x3fc
        char buffer[640]; // r29+0x170
        char *b; // r18
        unsigned int size; // r16
        signed short err; // r2
        char *eol; // r17
        char *eq; // r16
        class tagFile ini; // r29+0x60
    }
}

// 0x00384940 - 0x00384A00 (0xC0) p2Render__Fv
void p2Render() {
    { // 0x00384940 - 0x00384A00
    }
}

// 0x00384A00 - 0x00384A3C (0x3C) iRenderFlip__Fv
void iRenderFlip() {
    { // 0x00384A00 - 0x00384A3C
    }
}

// 0x00384A40 - 0x00384A7C (0x3C) iRenderEnd__Fv
void iRenderEnd() {
    { // 0x00384A40 - 0x00384A7C
    }
}

// 0x00384A80 - 0x00384AE0 (0x60) iRenderBegin__Fv
signed int iRenderBegin() {
    { // 0x00384A80 - 0x00384AE0
        class _RwRGBA BKGColor; // r29+0x1c
    }
}

// 0x00384AE0 - 0x00384B88 (0xA8) p2SetCameraPos__FP9_RwCamerafff
void p2SetCameraPos(class _RwCamera *cam, float x, float y, float z) {
    { // 0x00384AE0 - 0x00384B88
        class _RwFrame *f; // r17
        class _RwMatrix *m; // r16
        class _RwV3d *new_pos; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iMach.cpp
// ==========================================================================
char timestamp[0]; // 0x004499E0
// 0x00384B90 - 0x00384CA8 (0x118) iMachDisplayBuildInfo__Fv
void iMachDisplayBuildInfo() {
    { // 0x00384B90 - 0x00384CA8
        signed int delta; // r2
        signed int B; // r20
        signed int G; // r19
        signed int R; // r18
        class xFontBox_t *box; // r16
        unsigned int ct; // r17
    }
}

// 0x00384CB0 - 0x00384CDC (0x2C) InitHardware__Fv
void InitHardware() {
    { // 0x00384CB0 - 0x00384CDC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iLine.cpp
// ==========================================================================
static signed int sDefaultLineColorB; // 0x004A4670
static signed int sDefaultLineColorG; // 0x004A466C
static signed int sDefaultLineColorR; // 0x004A4668
static char dbfont_chars[51]; // 0x00000000
// 0x00384CE0 - 0x00384D20 (0x40) iLineDrawScreenRGB__FP6_RwV2dP6_RwV2diii
void iLineDrawScreenRGB(class _RwV2d *p1, class _RwV2d *p2) {
    { // 0x00384CE0 - 0x00384D20
    }
}

// 0x00384D20 - 0x00384D28 (0x8) iCircleDrawRGB__FP8Vector3Dfiii
void iCircleDrawRGB() {
    { // 0x00384D20 - 0x00384D28
    }
}

// 0x00384D30 - 0x00384D50 (0x20) iLineDrawXYZ__Fffffff
void iLineDrawXYZ(float x1, float y1, float z1, float x2, float y2, float z2) {
    { // 0x00384D30 - 0x00384D50
    }
}

// 0x00384D50 - 0x00384D88 (0x38) iLineDrawRGB__FP8Vector3DP8Vector3Diii
void iLineDrawRGB(class Vector3D *p1, class Vector3D *p2) {
    { // 0x00384D50 - 0x00384D88
    }
}

// 0x00384D90 - 0x00384DC8 (0x38) iLineDraw__FP8Vector3DP8Vector3D
void iLineDraw(class Vector3D *p1, class Vector3D *p2) {
    { // 0x00384D90 - 0x00384DC8
    }
}

// 0x00384DD0 - 0x00384DE4 (0x14) iLineSetColor__Fiii
void iLineSetColor(signed int r, signed int g, signed int b) {
    { // 0x00384DD0 - 0x00384DE4
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iGui.cpp
// ==========================================================================
float gZOffsetDelta; // 0x004A4638
float gZOffset; // 0x004A4DEC
signed int gGameState; // 0x004A4640
signed short gFX1Color; // 0x004A4DE4
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x00384DF0 - 0x00385328 (0x538) iGuiItemRenderFunc_MayaClip__FP9_xGuiItemP8_xGuiSys
void iGuiItemRenderFunc_MayaClip(class _xGuiItem *gui, class _xGuiSys *sys) {
    { // 0x00384DF0 - 0x00385328
        signed short offy; // r3
        signed short offx; // r6
        class Vector3D b; // r29+0xd0
        class Vector3D a; // r29+0xc0
        signed int meshCount; // r29+0xdc
        class tagP2Mesh *mesh; // r2
    }
}

// 0x00385330 - 0x003857D0 (0x4A0) iGuiItemRenderFunc_Maya__FP9_xGuiItemP8_xGuiSys
void iGuiItemRenderFunc_Maya(class _xGuiItem *gui, class _xGuiSys *sys) {
    { // 0x00385330 - 0x003857D0
        class Vector3D b; // r29+0x80
        class Vector3D a; // r29+0x70
        class _xTex *tex; // r17
        signed int meshCount; // r29+0x8c
        class tagP2Mesh *mesh; // r16
    }
}

// 0x003857D0 - 0x00385F90 (0x7C0) iGuiItemRenderFunc_Text__FP9_xGuiItemP8_xGuiSys
void iGuiItemRenderFunc_Text(class _xGuiItem *gui, class _xGuiSys *sys) {
    { // 0x003857D0 - 0x00385F90
        char *off; // r2
        signed short offy; // r3
        signed short offx; // r16
        char *text; // r17
        class xFontBox_t *fontBox; // r16
        class _xTex *tex; // r2
        class Vector3D b; // r29+0x80
        class Vector3D a; // r29+0x70
        signed int meshCount; // r29+0x8c
        class tagP2Mesh *mesh; // r2
    }
}

// 0x00385F90 - 0x00386524 (0x594) iGuiItemRenderFunc_TextNumber__FP9_xGuiItemP8_xGuiSys
void iGuiItemRenderFunc_TextNumber(class _xGuiItem *gui, class _xGuiSys *sys) {
    { // 0x00385F90 - 0x00386524
        signed short offy; // r2
        signed short offx; // r16
        char text[16]; // r29+0x60
        class xFontBox_t *fontBox; // r16
        class Vector3D b; // r29+0x80
        class Vector3D a; // r29+0x70
        signed int meshCount; // r29+0x8c
        class tagP2Mesh *mesh; // r2
    }
}

// 0x00386530 - 0x0038657C (0x4C) iGuiItemRenderFunc_Image__FP9_xGuiItemP8_xGuiSys
void iGuiItemRenderFunc_Image(class _xGuiItem *gui, class _xGuiSys *sys) {
    { // 0x00386530 - 0x0038657C
    }
}

// 0x00386580 - 0x00386588 (0x8) iGuiItemRenderFunc_DebugOutline__FP9_xGuiItemP8_xGuiSys
void iGuiItemRenderFunc_DebugOutline() {
    { // 0x00386580 - 0x00386588
    }
}

// 0x00386590 - 0x00386598 (0x8) iGuiInit__Fv
void iGuiInit() {
    { // 0x00386590 - 0x00386598
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iGeom.cpp
// ==========================================================================
float g_DEGTORAD; // 0x00485A58
class iCamera icamera; // 0x00AFF3D0
class tagxActorDBData *gadb; // 0x004A4658
class tagxParSys gParSys[37]; // 0x004B94B0
class xScene *xScene_current; // 0x004A4E08
unsigned char debug_mode; // 0x004A4E4C
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
float zclip; // 0x00000000
unsigned int see_face; // 0x00000000
unsigned int see_mesh; // 0x00000000
signed int razNormalHome; // 0x00000000
signed int razNormalize; // 0x00000000
signed int razNormalXform; // 0x00000000
unsigned char show_rots; // 0x00000000
unsigned char show_skeleton; // 0x00000000
static class Vector3D ambient; // 0x00000000
static signed int lightCount; // 0x004A4F48
static class Vector3D lightColor[20]; // 0x00000000
static float lightDelta[20]; // 0x00000000
static class Vector4D light[20]; // 0x00AAEA40
static float lightShadeFactor[20]; // 0x00000000
static class Vector3D wlight; // 0x00000000
static signed int shadeGeom; // 0x00000000
static class Vector3D debuglightcolor; // 0x00000000
static class Vector3D debuglightoffset; // 0x00000000
static class Vector3D debuglight; // 0x00000000
static signed int debugAmbient; // 0x00000000
class Vector3D sunlight; // 0x0044FE20
signed int realshadow; // 0x004A4F44
float pass1AlphaCol; // 0x00000000
float pass1ShadowCol; // 0x00000000
float pass0AlphaCol; // 0x00000000
float pass0ShadowCol; // 0x00000000
signed int ignorePass; // 0x00000000
static char dbfont_chars[51]; // 0x00000000
// 0x003865A0 - 0x003869E8 (0x448) iGeomGetJointWorldPos__FP5xSkeliP8Vector3D
void iGeomGetJointWorldPos(class xSkel *skel, signed int jointIndex, class Vector3D *p) {
    { // 0x003865A0 - 0x003869E8
        class tagiJointRuntime *joint; // r3
    }
}

// 0x003869F0 - 0x003869F8 (0x8) iEffectReflect__FP15_iRenderEffectsiiii
void iEffectReflect() {
    { // 0x003869F0 - 0x003869F8
    }
}

// 0x00386A00 - 0x00386A34 (0x34) iEffectSetFlag__FP15_iRenderEffectsii
void iEffectSetFlag(class _iRenderEffects *effects, signed int flag, signed int onoff) {
    { // 0x00386A00 - 0x00386A34
    }
}

// 0x00386A40 - 0x00386B14 (0xD4) iEffectDone__FP15_iRenderEffects
signed int iEffectDone(class _iRenderEffects *effects) {
    { // 0x00386A40 - 0x00386B14
    }
}

// 0x00386B20 - 0x00386C10 (0xF0) iEffectFadeStart__FP15_iRenderEffectsii
void iEffectFadeStart(class _iRenderEffects *effects, signed int flag, signed int time) {
    { // 0x00386B20 - 0x00386C10
    }
}

// 0x00386C10 - 0x00386F4C (0x33C) iEffectFadeInfo__FP15_iRenderEffectsiiiii
void iEffectFadeInfo(class _iRenderEffects *fx, signed int r, signed int g, signed int b, signed int mode, signed int idx) {
    { // 0x00386C10 - 0x00386F4C
        unsigned char *color; // r5
    }
}

// 0x00386F50 - 0x00386FCC (0x7C) iGeomInitRenderEffects__FP7_p2Geom
void iGeomInitRenderEffects(class _p2Geom *geom) {
    { // 0x00386F50 - 0x00386FCC
        class _iRenderEffects *fx; // r2
    }
}

// 0x00386FD0 - 0x00386FD8 (0x8) iMeshDraw__FiP9tagP2MeshP5_xTexP7_p2Geom
void iMeshDraw() {
    { // 0x00386FD0 - 0x00386FD8
    }
}

// 0x00386FE0 - 0x00386FE8 (0x8) p2GeomDrawScreenBoxARGB__Fiiiiiiii
void p2GeomDrawScreenBoxARGB() {
    { // 0x00386FE0 - 0x00386FE8
    }
}

// 0x00386FF0 - 0x00386FF8 (0x8) p2GeomDrawScreenBox__Fiiiiiii
void p2GeomDrawScreenBox() {
    { // 0x00386FF0 - 0x00386FF8
    }
}

// 0x00387000 - 0x00387008 (0x8) p2GeomDrawScreen__FPvP9tagP2Meshiiss
void p2GeomDrawScreen() {
    { // 0x00387000 - 0x00387008
    }
}

// 0x00387010 - 0x00387460 (0x450) iGeomSetUVUsingMeshPart__FUiiUii
void iGeomSetUVUsingMeshPart(unsigned int destModelID, signed int destPartID, unsigned int modelID, signed int partID) {
    { // 0x00387010 - 0x00387460
        signed int i; // r5
        class tagP2Mesh *dest; // r17
        class tagP2Mesh *source; // r16
        signed int destCount; // r29+0x7c
        signed int sourceCount; // r29+0x78
    }
}

// 0x00387460 - 0x00387854 (0x3F4) iGeomCalcBoundingBox__FUiiP8Vector3DP8Vector3D
void iGeomCalcBoundingBox(unsigned int modelID, signed int partID, class Vector3D *a, class Vector3D *b) {
    { // 0x00387460 - 0x00387854
        signed int partCount; // r29+0xac
        class tagP2VERTEX *v; // r5
        class tagP2Mesh *mesh; // r17
        signed int j; // r4
        signed int start; // r2
        signed int i; // r16
    }
}

// 0x00387860 - 0x00387B14 (0x2B4) iGeomSetMeshVisibility__FPvii
void iGeomSetMeshVisibility(void *data, signed int number, signed int visible) {
    { // 0x00387860 - 0x00387B14
        signed int i; // r18
        class tagP2Mesh *mesh; // r17
        class _p2Geom *geom; // r16
    }
}

// 0x00387B20 - 0x00387B28 (0x8) p2GeomDraw__FP7_p2GeomPvP7_xActor
void p2GeomDraw() {
    { // 0x00387B20 - 0x00387B28
    }
}

// 0x00387B30 - 0x00387F38 (0x408) p2GeomDrawShadow__FP7_p2GeomPvP7_xActorP5_xObj
void p2GeomDrawShadow(class _p2Geom *geom, class _xActor *actor, class _xObj *obj) {
    { // 0x00387B30 - 0x00387F38
        class tagxPhysicsObject *p; // r16
        class Vector3D a; // r29+0x1e0
        class tagMATRIX sm; // r29+0x160
        class Vector3D gp2; // r29+0x1d0
        class Vector3D gp1; // r29+0x1c0
        class Vector3D gp0; // r29+0x1b0
        float gplane[4]; // r29+0x150
        float alphaCol; // r29+0x1f0
        float shadowcol; // r29+0x1f0
        float objmatrix[16]; // r29+0x110
        float world[16]; // r29+0xd0
        float result[16]; // r29+0x90
        float m[16]; // r29+0x50
        class Vector3D origin; // r29+0x1a0
        class tagP2Mesh *mesh; // r2
        unsigned int i; // r4
    }
}

// 0x00387F40 - 0x00387F70 (0x30) iGeomAssign__FP7_xActorP7_xModel
void iGeomAssign(class _xActor *actor, class _xModel *model) {
    { // 0x00387F40 - 0x00387F70
        class _p2Geom *geom; // r2
        class iActorRuntime *r; // r6
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iFont.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class xFontInfo_t FontList[3]; // 0x00AA6800
static char dbfont_chars[51]; // 0x00000000
// 0x00387F70 - 0x00387F78 (0x8) iFontDrawChar__FcfUiP11xFontInfo_t
void iFontDrawChar() {
    { // 0x00387F70 - 0x00387F78
    }
}

// 0x00387F80 - 0x00387F88 (0x8) iFontDrawBackdrop__FP10xFontBox_t
void iFontDrawBackdrop() {
    { // 0x00387F80 - 0x00387F88
    }
}

// 0x00387F90 - 0x0038807C (0xEC) xFontFreeTexture__FUi
void xFontFreeTexture(unsigned int id) {
    { // 0x00387F90 - 0x0038807C
        class xFontInfo_t *font; // r3
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iFMV.cpp
// ==========================================================================
class _xSndMgrData gSndMgr; // 0x004B1F00
unsigned char gAudioStatus; // 0x004A4E34
unsigned char gAudioMem[307200]; // 0x00AAEBA0
unsigned int gFMVassetID; // 0x00000000
// 0x00388080 - 0x003880B8 (0x38) iFMVStopAudio__Fv
unsigned int iFMVStopAudio() {
    { // 0x00388080 - 0x003880B8
    }
}

// 0x003880C0 - 0x003881D8 (0x118) iFMVStartAudio__FUiUi
unsigned int iFMVStartAudio(unsigned int audioID, unsigned int flags) {
    { // 0x003880C0 - 0x003881D8
        char audioFileName[64]; // r29+0x20
    }
}

// 0x003881E0 - 0x003881EC (0xC) iFMVPlayMovie__FUiUi
unsigned int iFMVPlayMovie() {
    { // 0x003881E0 - 0x003881EC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\ifloor.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static signed int sLastTriangle; // 0x004A4674
float gFloorQuickHeight[32][32]; // 0x00AF9BA0
// 0x003881F0 - 0x003881F8 (0x8) iFloorDrawZBuffer__FP9tagiFloor
void iFloorDrawZBuffer() {
    { // 0x003881F0 - 0x003881F8
    }
}

// 0x00388200 - 0x00388470 (0x270) iFloorDebugDraw__FP9tagiFloor
void iFloorDebugDraw(class tagiFloor *f) {
    { // 0x00388200 - 0x00388470
        signed int j; // r2
        signed int i; // r16
        static class Vector3D s[4]; // 0x00AFABA0
    }
}

// 0x00388470 - 0x00388E80 (0xA10) iFloorGetHeightIdx__FP9tagiFloorfffPUi
float iFloorGetHeightIdx(class tagiFloor *f, float x, float y, float z, unsigned int *triIdx) {
    { // 0x00388470 - 0x00388E80
        float ooarea; // 
        float v3z; // r29+0xa0
        float v3x; // 
        float v2z; // 
        float v2x; // 
        float v1z; // 
        float v1x; // 
        float bw; // 
        float bv; // 
        float bu; // 
        float ooarea; // 
        float v3z; // r29+0xa0
        float v3x; // 
        float v2z; // 
        float v2x; // 
        float v1z; // 
        float v1x; // 
        float bw; // 
        float bv; // 
        float bu; // 
        unsigned int ti; // r16
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        unsigned int i; // r16
        class Vector3D p; // r29+0x90
    }
}

// 0x00388E80 - 0x00389604 (0x784) iFloorGetHeight__FP9tagiFloorfff
float iFloorGetHeight(class tagiFloor *f, float x, float y, float z) {
    { // 0x00388E80 - 0x00389604
        float ooarea; // r29+0x90
        float bw; // 
        float bv; // 
        float bu; // 
        float ooarea; // r29+0x90
        float bw; // 
        float bv; // 
        float bu; // 
        unsigned int pmasklo; // r18
        unsigned int pmaskhi; // r17
        unsigned int pmask; // r2
        unsigned int i; // r16
        class Vector3D p; // r29+0x80
    }
}

// 0x00389610 - 0x00389948 (0x338) iFloorLoadFromBuffer__FP9tagiFloorPUc
void iFloorLoadFromBuffer(class tagiFloor *f, unsigned char *data) {
    { // 0x00389610 - 0x00389948
    }
}

// 0x00389950 - 0x00389C0C (0x2BC) iFloorGetMaskLine2D__FP9tagiFloorP7xLine2D
unsigned int iFloorGetMaskLine2D(class tagiFloor *f, class xLine2D *l) {
    { // 0x00389950 - 0x00389C0C
        unsigned int himask; // r6
        unsigned int lomask; // r5
        unsigned int i; // r2
        float ooh; // 
        float oow; // 
        unsigned int tmp; // r3
        float zd[2]; // r29+0x58
        float xd[2]; // r29+0x50
    }
}

// 0x00389C10 - 0x00389EBC (0x2AC) iFloorGetMaskRadius__FP9tagiFloorfff
unsigned int iFloorGetMaskRadius(class tagiFloor *f, float x, float z, float r) {
    { // 0x00389C10 - 0x00389EBC
        unsigned int himask; // r5
        unsigned int lomask; // r4
        unsigned int i; // r2
        unsigned int tmp; // r3
        float zd[2]; // r29+0x58
        float xd[2]; // r29+0x50
    }
}

// 0x00389EC0 - 0x00389F60 (0xA0) iFloorGetMask__FP9tagiFloorfff
unsigned int iFloorGetMask(class tagiFloor *f, float x, float z) {
    { // 0x00389EC0 - 0x00389F60
        unsigned int zbit; // r2
        unsigned int xbit; // r16
        float zd; // 
        float xd; // 
    }
}

// 0x00389F60 - 0x00389FB0 (0x50) iFloorLevelReset__FP9tagiFloor
void iFloorLevelReset(class tagiFloor *f) {
    { // 0x00389F60 - 0x00389FB0
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iFile.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int sFileSystemInit; // 0x004A4F4C
static char dbfont_chars[51]; // 0x00000000
// 0x00389FB0 - 0x0038A260 (0x2B0) iFileRead__FP7tagFileUiPUcPUi
signed int iFileRead(class tagFile *f, unsigned int numBytes, unsigned char *buf, unsigned int *numRead) {
    { // 0x00389FB0 - 0x0038A260
        unsigned char *dst_buffer; // r2
        unsigned int skip_start_bytes; // r2
        unsigned int skip_end_bytes; // r2
        unsigned int skip_bytes; // r2
        unsigned int seek_loc; // r2
        unsigned int count; // r17
        static signed char init; // 0x004A4F54
        static unsigned char *sectorBufferU8; // 0x004A4F50
        static unsigned int sectorBuffer[512]; // 0x00AFABD0
    }
}

// 0x0038A260 - 0x0038A35C (0xFC) iFileClose__FP7tagFile
signed int iFileClose(class tagFile *f) {
    { // 0x0038A260 - 0x0038A35C
    }
}

// 0x0038A360 - 0x0038A428 (0xC8) iFileOpen__FP7tagFilei
signed int iFileOpen(class tagFile *f) {
    { // 0x0038A360 - 0x0038A428
        signed int index; // r17
        char path[128]; // r29+0x30
    }
}

// 0x0038A430 - 0x0038A670 (0x240) iFileGetSize__FP7tagFile
unsigned int iFileGetSize(class tagFile *f) {
    { // 0x0038A430 - 0x0038A670
        signed int err; // r2
        signed int file_size; // r17
        signed int wasOpen; // r16
    }
}

// 0x0038A670 - 0x0038A7A4 (0x134) iFileInit__FP7tagFilePc
signed int iFileInit(class tagFile *f, char *theName) {
    { // 0x0038A670 - 0x0038A7A4
    }
}

// 0x0038A7B0 - 0x0038A7D0 (0x20) iFileSystemShutdown__Fv
signed int iFileSystemShutdown() {
    { // 0x0038A7B0 - 0x0038A7D0
    }
}

// 0x0038A7D0 - 0x0038A7FC (0x2C) iFileSystemInit__Fv
signed int iFileSystemInit() {
    { // 0x0038A7D0 - 0x0038A7FC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iCart.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagxCartControl gCartControl; // 0x00AAC620
static char *msg_02_01; // 0x004A4680
static char *msg_01_01[8]; // 0x0044FE50
static signed int lOp; // 0x004A467C
static signed int lStatus; // 0x004A4678
unsigned char *sBupWork; // 0x004A4F60
static void *gBupBuf; // 0x00000000
unsigned int cbParam; // 0x00000000
signed int cbStatus; // 0x004A4F5C
signed int cbOp; // 0x004A4F58
signed int cbDrive; // 0x00000000
unsigned char p2CartDataBuffer[16384]; // 0x00AFB3D0
// 0x0038A800 - 0x0038A858 (0x58) iCartBeep__Fi
void iCartBeep(signed int flag) {
    { // 0x0038A800 - 0x0038A858
        static signed int beepActive; // 0x004A4F64
        static signed char init; // 0x004A4F68
    }
}

// 0x0038A860 - 0x0038A86C (0xC) iCartIsCardPresent__Fv
signed int iCartIsCardPresent() {
    { // 0x0038A860 - 0x0038A86C
    }
}

// 0x0038A870 - 0x0038A92C (0xBC) iCartTextFind__Fi
char *iCartTextFind(signed int assetId) {
    { // 0x0038A870 - 0x0038A92C
        signed int value; // r4
        signed int group; // r3
    }
}

// 0x0038A930 - 0x0038AA08 (0xD8) iCartFormat__Fv
signed int iCartFormat() {
    { // 0x0038A930 - 0x0038AA08
        signed int cart; // r17
        signed int cmd; // r29+0x3c
        signed int result; // r29+0x38
        signed int retry; // r16
    }
}

// 0x0038AA10 - 0x0038AA1C (0xC) iCartCountValidCardSlots__Fv
signed int iCartCountValidCardSlots() {
    { // 0x0038AA10 - 0x0038AA1C
    }
}

// 0x0038AA20 - 0x0038AA34 (0x14) iCartSetSlot__Fi
signed int iCartSetSlot(signed int slot) {
    { // 0x0038AA20 - 0x0038AA34
    }
}

// 0x0038AA40 - 0x0038AA4C (0xC) iCartCheckValidSlot__Fi
signed int iCartCheckValidSlot() {
    { // 0x0038AA40 - 0x0038AA4C
    }
}

// 0x0038AA50 - 0x0038AB10 (0xC0) iCartGetSlotTime__Fi
signed int iCartGetSlotTime(signed int slot) {
    { // 0x0038AA50 - 0x0038AB10
        signed int s; // r2
        signed int m; // r17
        signed int h; // r16
        signed int l; // r2
        char *src; // r16
        char timeStr[16]; // r29+0x30
    }
}

// 0x0038AB10 - 0x0038AB1C (0xC) iCartGetCardFreeBlocks__Fi
signed int iCartGetCardFreeBlocks() {
    { // 0x0038AB10 - 0x0038AB1C
        signed int bfree; // r2
    }
}

// 0x0038AB20 - 0x0038AB28 (0x8) iCartDelete__Fi
void iCartDelete() {
    { // 0x0038AB20 - 0x0038AB28
    }
}

// 0x0038AB30 - 0x0038AB38 (0x8) iCartSlotInfo__FiiPUc
void iCartSlotInfo() {
    { // 0x0038AB30 - 0x0038AB38
    }
}

// 0x0038AB40 - 0x0038AB58 (0x18) iCartGetCompressionBuffer__FPUs
unsigned char *iCartGetCompressionBuffer(unsigned short *size) {
    { // 0x0038AB40 - 0x0038AB58
    }
}

// 0x0038AB60 - 0x0038AB78 (0x18) iCartGetDataBuffer__FPUs
unsigned char *iCartGetDataBuffer(unsigned short *size) {
    { // 0x0038AB60 - 0x0038AB78
    }
}

// 0x0038AB80 - 0x0038ABA8 (0x28) iCartWrite__Fv
void iCartWrite() {
    { // 0x0038AB80 - 0x0038ABA8
    }
}

// 0x0038ABB0 - 0x0038ABDC (0x2C) iCartRead__Fv
void iCartRead() {
    { // 0x0038ABB0 - 0x0038ABDC
    }
}

// 0x0038ABE0 - 0x0038ABEC (0xC) iCartAppendHeader__FPUc
signed int iCartAppendHeader() {
    { // 0x0038ABE0 - 0x0038ABEC
    }
}

// 0x0038ABF0 - 0x0038AC14 (0x24) iCartSetDescription__FPUc
void iCartSetDescription(unsigned char *desc) {
    { // 0x0038ABF0 - 0x0038AC14
        signed int length; // r2
    }
}

// 0x0038AC20 - 0x0038AC2C (0xC) iCartGetHeaderSize__Fv
signed int iCartGetHeaderSize() {
    { // 0x0038AC20 - 0x0038AC2C
    }
}

// 0x0038AC30 - 0x0038AC48 (0x18) iCartOpen__Fv
signed int iCartOpen() {
    { // 0x0038AC30 - 0x0038AC48
    }
}

// 0x0038AC50 - 0x0038AC5C (0xC) iCartCountValidSlots__Fi
signed int iCartCountValidSlots() {
    { // 0x0038AC50 - 0x0038AC5C
    }
}

// 0x0038AC60 - 0x0038AD24 (0xC4) iCartIdle__Fv
void iCartIdle() {
    { // 0x0038AC60 - 0x0038AD24
    }
}

// 0x0038AD30 - 0x0038AD50 (0x20) iCartRefresh__Fv
void iCartRefresh() {
    { // 0x0038AD30 - 0x0038AD50
    }
}

// 0x0038AD50 - 0x0038AD5C (0xC) iCartAccessCard__Fi
signed int iCartAccessCard() {
    { // 0x0038AD50 - 0x0038AD5C
    }
}

// 0x0038AD60 - 0x0038AD88 (0x28) iCartEnd__Fv
void iCartEnd() {
    { // 0x0038AD60 - 0x0038AD88
    }
}

// 0x0038AD90 - 0x0038AE84 (0xF4) iCartBegin__Fv
void iCartBegin() {
    { // 0x0038AD90 - 0x0038AE84
        signed int memneed; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iCamera.cpp
// ==========================================================================
class tag_xCameraControl gCamera; // 0x00AA67C0
class xScene *xScene_current; // 0x004A4E08
signed int gBossBattle; // 0x004A4F70
class iCamera icamera; // 0x00AFF3D0
float proj_farnear; // 0x004A4F6C
float proj_fovr; // 0x00000000
signed int theGlobalFrame; // 0x00000000
static char dbfont_chars[51]; // 0x00000000
// 0x0038AE90 - 0x0038AE9C (0xC) iCameraClearBossBattle__Fv
void iCameraClearBossBattle() {
    { // 0x0038AE90 - 0x0038AE9C
    }
}

// 0x0038AEA0 - 0x0038AEA8 (0x8) iCameraSetCamera__FP18tag_xCameraControl
void iCameraSetCamera() {
    { // 0x0038AEA0 - 0x0038AEA8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iAudioStream.cpp
// ==========================================================================
signed int p2SndMgrSafeToAudioInterrupt; // 0x004A4EE4
static class AudioStream_s *AudioStreamBuf[4]; // 0x00000000
static void *StreamPlayBuffer[4]; // 0x00000000
static void *StreamTransferBuffer[4]; // 0x00000000
static void *StreamHeaderBuffer[4]; // 0x00000000
static signed int StreamBufferUsed[4]; // 0x00000000
static signed int AudioStreamUniqueID; // 0x00000000
static char snd_mgr_err_str[32]; // 0x00000000
static char out_of_mem_str[14]; // 0x00000000
static char par_inv_str[21]; // 0x00000000
static signed int AudioStreamSystemInitialized; // 0x00000000
static char NoReintStr[30]; // 0x00000000
static char MustCallInitStr[30]; // 0x00000000
static signed int h_size; // 0x00000000
static class descriptor *h_start; // 0x00000000
static class descriptor free_h; // 0x00000000
static class descriptor alloc_h; // 0x00000000
// 0x0038AEB0 - 0x0038AEB8 (0x8) iAudioStreamDestroy__FP13AudioStream_s
void iAudioStreamDestroy() {
    { // 0x0038AEB0 - 0x0038AEB8
    }
}

// 0x0038AEC0 - 0x0038AEC8 (0x8) iAudioStreamUnpause__FP13AudioStream_s
void iAudioStreamUnpause() {
    { // 0x0038AEC0 - 0x0038AEC8
    }
}

// 0x0038AED0 - 0x0038AED8 (0x8) iAudioStreamPause__FP13AudioStream_s
void iAudioStreamPause() {
    { // 0x0038AED0 - 0x0038AED8
    }
}

// 0x0038AEE0 - 0x0038AEEC (0xC) iAudioStreamCreate__FPci
void *iAudioStreamCreate() {
    { // 0x0038AEE0 - 0x0038AEEC
    }
}

// 0x0038AEF0 - 0x0038AEF8 (0x8) iAudioStreamDeinitSystem__Fv
void iAudioStreamDeinitSystem() {
    { // 0x0038AEF0 - 0x0038AEF8
    }
}

// 0x0038AF00 - 0x0038AF08 (0x8) iAudioStreamInitSystem__FPvi
void iAudioStreamInitSystem() {
    { // 0x0038AF00 - 0x0038AF08
    }
}

// 0x0038AF10 - 0x0038AF20 (0x10) iAudioStreamPaused__FP13AudioStream_s
signed int iAudioStreamPaused(class AudioStream_s *pStream) {
    { // 0x0038AF10 - 0x0038AF20
    }
}

// 0x0038AF20 - 0x0038AF30 (0x10) iAudioStreamPlaying__FP13AudioStream_s
signed int iAudioStreamPlaying(class AudioStream_s *pStream) {
    { // 0x0038AF20 - 0x0038AF30
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iAsset.cpp
// ==========================================================================
class _xModel g_AshModel[0]; // 0x004E03F0
signed int g_AshModelCount; // 0x004A4D38
class xSkel g_AshSkeleton[0]; // 0x004B52F8
signed int g_AshSkeletonCount; // 0x004A4CB4
class tagXAnimSet g_AshAnimations[0]; // 0x00AACBA0
signed int g_AshAnimationsCount; // 0x004A4EA0
class _xModel g_SNPCModel[0]; // 0x004E0410
signed int g_SNPCModelCount; // 0x004A4D40
class xSkel g_SNPCSkeleton[0]; // 0x004B5310
signed int g_SNPCSkeletonCount; // 0x004A4CBC
class tagXAnimSet g_SNPCAnimations[0]; // 0x00AACBF0
signed int g_SNPCAnimationsCount; // 0x004A4EA8
class _xModel g_DNPCModel[0]; // 0x004E0440
signed int g_DNPCModelCount; // 0x004A4D44
class xSkel g_DNPCSkeleton[0]; // 0x004B5340
signed int g_DNPCSkeletonCount; // 0x004A4CC0
class tagXAnimSet g_DNPCAnimations[0]; // 0x00AACC60
signed int g_DNPCAnimationsCount; // 0x004A4EAC
class xScene *xScene_sceneBuffer[2]; // 0x00AA67F0
class xScene *xScene_preload; // 0x004A4E0C
class xScene *xScene_current; // 0x004A4E08
class _xSndMgrData gSndMgr; // 0x004B1F00
class _xModel g_RTObjModel[0]; // 0x004E0080
signed int g_RTObjModelCount; // 0x004A4D34
class _xModel g_BossModel[0]; // 0x004E0400
signed int g_BossModelCount; // 0x004A4D3C
class xSkel g_BossSkeleton[0]; // 0x004B5300
signed int g_BossSkeletonCount; // 0x004A4CB8
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class tagXAnimSet g_BossAnimations[0]; // 0x00AACBC0
signed int g_BossAnimationsCount; // 0x004A4EA4
// 0x0038AF30 - 0x0038AF58 (0x28) iEnvTextInit__FP7XStream
void iEnvTextInit(class XStream *stream) {
    { // 0x0038AF30 - 0x0038AF58
    }
}

// 0x0038AF60 - 0x0038B084 (0x124) iCharBossAnimAllInit__FP7XStream
void iCharBossAnimAllInit(class XStream *stream) {
    { // 0x0038AF60 - 0x0038B084
    }
}

// 0x0038B090 - 0x0038B1A8 (0x118) iCharBossAnimReadyInit__FP7XStream
void iCharBossAnimReadyInit(class XStream *stream) {
    { // 0x0038B090 - 0x0038B1A8
    }
}

// 0x0038B1B0 - 0x0038B1F4 (0x44) iCharBossSkeletonInstance__FUiPvPvPv
void iCharBossSkeletonInstance(unsigned int assetID, void *resource, void *runtime) {
    { // 0x0038B1B0 - 0x0038B1F4
    }
}

// 0x0038B200 - 0x0038B254 (0x54) iCharBossSkeletonInit__FP7XStream
void iCharBossSkeletonInit(class XStream *stream) {
    { // 0x0038B200 - 0x0038B254
    }
}

// 0x0038B260 - 0x0038B268 (0x8) iCharBossGeometryInstance__FUiPvPvPv
void iCharBossGeometryInstance() {
    { // 0x0038B260 - 0x0038B268
    }
}

// 0x0038B270 - 0x0038B428 (0x1B8) iCharBossGeometryInit__FP7XStream
void iCharBossGeometryInit(class XStream *stream) {
    { // 0x0038B270 - 0x0038B428
        class _i3DObjInfo info; // r29+0x40
    }
}

// 0x0038B430 - 0x0038B504 (0xD4) iCharTextureInit__FP7XStream
void iCharTextureInit(class XStream *s) {
    { // 0x0038B430 - 0x0038B504
        class _xTex *next; // r16
    }
}

// 0x0038B510 - 0x0038B530 (0x20) iEnvPlateInit__FP7XStream
void iEnvPlateInit(class XStream *s) {
    { // 0x0038B510 - 0x0038B530
    }
}

// 0x0038B530 - 0x0038B5E0 (0xB0) iEnvFrameInit__FP7XStream
void iEnvFrameInit() {
    { // 0x0038B530 - 0x0038B5E0
    }
}

// 0x0038B5E0 - 0x0038B690 (0xB0) iSpriteInit__FP7XStream
void iSpriteInit() {
    { // 0x0038B5E0 - 0x0038B690
    }
}

// 0x0038B690 - 0x0038B740 (0xB0) iEnvColormapInit__FP7XStream
void iEnvColormapInit() {
    { // 0x0038B690 - 0x0038B740
    }
}

// 0x0038B740 - 0x0038B7F0 (0xB0) iCharColormapInit__FP7XStream
void iCharColormapInit() {
    { // 0x0038B740 - 0x0038B7F0
    }
}

// 0x0038B7F0 - 0x0038B7F8 (0x8) iCharRTObjGeometryInstance__FUiPvPvPv
void iCharRTObjGeometryInstance() {
    { // 0x0038B7F0 - 0x0038B7F8
    }
}

// 0x0038B800 - 0x0038B8D8 (0xD8) iCharRTObjGeometryInit__FP7XStream
void iCharRTObjGeometryInit(class XStream *stream) {
    { // 0x0038B800 - 0x0038B8D8
        class _i3DObjInfo info; // r29+0x20
    }
}

// 0x0038B8E0 - 0x0038BAF8 (0x218) iCharRTObjTextureInit__FP7XStream
void iCharRTObjTextureInit(class XStream *s) {
    { // 0x0038B8E0 - 0x0038BAF8
        class _xTex *next; // r16
    }
}

// 0x0038BB00 - 0x0038BBB0 (0xB0) iEnvMIDIDataInit__FP7XStream
void iEnvMIDIDataInit() {
    { // 0x0038BB00 - 0x0038BBB0
    }
}

// 0x0038BBB0 - 0x0038BC60 (0xB0) iEnvLightListInit__FP7XStream
void iEnvLightListInit() {
    { // 0x0038BBB0 - 0x0038BC60
    }
}

// 0x0038BC60 - 0x0038BD10 (0xB0) iEnvScriptInit__FP7XStream
void iEnvScriptInit() {
    { // 0x0038BC60 - 0x0038BD10
    }
}

// 0x0038BD10 - 0x0038BDC0 (0xB0) iCharRTObjAnimReadyInit__FP7XStream
void iCharRTObjAnimReadyInit() {
    { // 0x0038BD10 - 0x0038BDC0
    }
}

// 0x0038BDC0 - 0x0038BE70 (0xB0) iCharRTObjAnimAllInit__FP7XStream
void iCharRTObjAnimAllInit() {
    { // 0x0038BDC0 - 0x0038BE70
    }
}

// 0x0038BE70 - 0x0038BF20 (0xB0) iCharMIDIDataInit__FP7XStream
void iCharMIDIDataInit() {
    { // 0x0038BE70 - 0x0038BF20
    }
}

// 0x0038BF20 - 0x0038C064 (0x144) iCharSoundBankInit__FP7XStream
void iCharSoundBankInit(class XStream *s) {
    { // 0x0038BF20 - 0x0038C064
        unsigned int i; // r23
        unsigned char *sample; // r22
        unsigned char *base; // r21
        unsigned int *u32ptr; // r20
        signed int soundID; // r2
        unsigned short nextRate; // r19
        unsigned short nextSize; // r18
        unsigned int nextOffset; // r2
        unsigned int nextID; // r17
        unsigned int count; // r16
    }
}

// 0x0038C070 - 0x0038C1B4 (0x144) iEnvSoundBankInit__FP7XStream
void iEnvSoundBankInit(class XStream *stream) {
    { // 0x0038C070 - 0x0038C1B4
        unsigned int i; // r23
        unsigned char *sample; // r22
        unsigned char *base; // r21
        unsigned int *u32ptr; // r20
        signed int soundID; // r2
        unsigned short nextRate; // r19
        unsigned short nextSize; // r18
        unsigned int nextOffset; // r2
        unsigned int nextID; // r17
        unsigned int count; // r16
    }
}

// 0x0038C1C0 - 0x0038C888 (0x6C8) iEnvSceneInit__FP7XStream
void iEnvSceneInit(class XStream *stream) {
    { // 0x0038C1C0 - 0x0038C888
        signed int offset; // r6
        signed int i; // r3
        class xNavLink *xnavlink; // r21
        class xNavPoint *xnavpoint; // r5
        class xLight *xlight; // r5
        class xMarker *xmarker; // r5
        class xCamera *xcamera; // r6
        class xEntryPoint *xentryPoint; // r5
        class xCondition *xcondition; // r20
        class xAction *xaction; // r19
        class xRule *xrule; // r22
        class xTrigger *xtrigger; // r5
        class xLight *lightBuffer; // r2
        class xEntryPoint *entryPointBuffer; // r29+0x188
        class xMarker *markerBuffer; // r29+0x184
        class xCamera *cameraBuffer; // r29+0x180
        class xCondition *conditionBuffer; // r2
        class xAction *actionBuffer; // r2
        class xRule *ruleBuffer; // r2
        class iLightResource *light; // r23
        class iEntryPointResource *entryPoint; // r30
        class iMarkerResource *marker; // r29+0x170
        class iCameraResource *camera; // r18
        class iConditionResource *condition; // r29+0x160
        class iActionResource *action; // r29+0x150
        class iRuleResource *rule; // r17
        class iTriggerResource *trigger; // r16
        unsigned char *lightPointer; // r2
        unsigned char *markerPointer; // r2
        unsigned char *cameraPointer; // r2
        unsigned char *navLinkPointer; // r2
        unsigned char *navPointPointer; // r29+0x140
        unsigned char *entryPointPointer; // r2
        unsigned char *conditionPointer; // r2
        unsigned char *actionPointer; // r2
        unsigned char *rulePointer; // r2
        unsigned char *triggerPointer; // r2
        unsigned char *floorPointer; // r19
        unsigned char *pU8; // r4
        unsigned int *pU32; // r3
        signed int lightOffset; // r3
        signed int lightCount; // r29+0x130
        signed int markerOffset; // r5
        signed int markerCount; // r29+0x120
        signed int cameraOffset; // r5
        signed int cameraCount; // r29+0x110
        signed int navLinkOffset; // r5
        signed int navLinkCount; // r29+0x100
        signed int navPointOffset; // r5
        signed int navPointCount; // r29+0xf0
        signed int entryPointOffset; // r5
        signed int entryPointCount; // r29+0xe0
        signed int conditionOffset; // r5
        signed int conditionCount; // r29+0xd0
        signed int actionOffset; // r5
        signed int actionCount; // r29+0xc0
        signed int ruleOffset; // r5
        signed int ruleCount; // r29+0xb0
        signed int triggerOffset; // r5
        signed int triggerCount; // r29+0xa0
        signed int floorOffset; // r5
        signed int floorSize; // r2
    }
}

// 0x0038C890 - 0x0038C9B4 (0x124) iCharDNPCAnimAllInit__FP7XStream
void iCharDNPCAnimAllInit(class XStream *stream) {
    { // 0x0038C890 - 0x0038C9B4
    }
}

// 0x0038C9C0 - 0x0038CAD8 (0x118) iCharDNPCAnimReadyInit__FP7XStream
void iCharDNPCAnimReadyInit(class XStream *stream) {
    { // 0x0038C9C0 - 0x0038CAD8
    }
}

// 0x0038CAE0 - 0x0038CB24 (0x44) iCharDNPCSkeletonInstance__FUiPvPvPv
void iCharDNPCSkeletonInstance(unsigned int assetID, void *resource, void *runtime) {
    { // 0x0038CAE0 - 0x0038CB24
    }
}

// 0x0038CB30 - 0x0038CB84 (0x54) iCharDNPCSkeletonInit__FP7XStream
void iCharDNPCSkeletonInit(class XStream *stream) {
    { // 0x0038CB30 - 0x0038CB84
    }
}

// 0x0038CB90 - 0x0038CB98 (0x8) iCharDNPCGeometryInstance__FUiPvPvPv
void iCharDNPCGeometryInstance() {
    { // 0x0038CB90 - 0x0038CB98
    }
}

// 0x0038CBA0 - 0x0038CD58 (0x1B8) iCharDNPCGeometryInit__FP7XStream
void iCharDNPCGeometryInit(class XStream *stream) {
    { // 0x0038CBA0 - 0x0038CD58
        class _i3DObjInfo info; // r29+0x40
    }
}

// 0x0038CD60 - 0x0038CE84 (0x124) iCharSNPCAnimAllInit__FP7XStream
void iCharSNPCAnimAllInit(class XStream *stream) {
    { // 0x0038CD60 - 0x0038CE84
    }
}

// 0x0038CE90 - 0x0038CFA8 (0x118) iCharSNPCAnimReadyInit__FP7XStream
void iCharSNPCAnimReadyInit(class XStream *stream) {
    { // 0x0038CE90 - 0x0038CFA8
    }
}

// 0x0038CFB0 - 0x0038CFF4 (0x44) iCharSNPCSkeletonInstance__FUiPvPvPv
void iCharSNPCSkeletonInstance(unsigned int assetID, void *resource, void *runtime) {
    { // 0x0038CFB0 - 0x0038CFF4
    }
}

// 0x0038D000 - 0x0038D054 (0x54) iCharSNPCSkeletonInit__FP7XStream
void iCharSNPCSkeletonInit(class XStream *stream) {
    { // 0x0038D000 - 0x0038D054
    }
}

// 0x0038D060 - 0x0038D068 (0x8) iCharSNPCGeometryInstance__FUiPvPvPv
void iCharSNPCGeometryInstance() {
    { // 0x0038D060 - 0x0038D068
    }
}

// 0x0038D070 - 0x0038D228 (0x1B8) iCharSNPCGeometryInit__FP7XStream
void iCharSNPCGeometryInit(class XStream *stream) {
    { // 0x0038D070 - 0x0038D228
        class _i3DObjInfo info; // r29+0x40
    }
}

// 0x0038D230 - 0x0038D350 (0x120) iCharAshAnimAllInit__FP7XStream
void iCharAshAnimAllInit(class XStream *stream) {
    { // 0x0038D230 - 0x0038D350
    }
}

// 0x0038D350 - 0x0038D464 (0x114) iCharAshAnimReadyInit__FP7XStream
void iCharAshAnimReadyInit(class XStream *stream) {
    { // 0x0038D350 - 0x0038D464
    }
}

// 0x0038D470 - 0x0038D4B4 (0x44) iCharAshSkeletonInstance__FUiPvPvPv
void iCharAshSkeletonInstance(unsigned int assetID, void *resource, void *runtime) {
    { // 0x0038D470 - 0x0038D4B4
    }
}

// 0x0038D4C0 - 0x0038D514 (0x54) iCharAshSkeletonInit__FP7XStream
void iCharAshSkeletonInit(class XStream *stream) {
    { // 0x0038D4C0 - 0x0038D514
    }
}

// 0x0038D520 - 0x0038D528 (0x8) iCharAshGeometryInstance__FUiPvPvPv
void iCharAshGeometryInstance() {
    { // 0x0038D520 - 0x0038D528
    }
}

// 0x0038D530 - 0x0038D6E4 (0x1B4) iCharAshGeometryInit__FP7XStream
void iCharAshGeometryInit(class XStream *stream) {
    { // 0x0038D530 - 0x0038D6E4
        class _i3DObjInfo info; // r29+0x40
    }
}

// 0x0038D6F0 - 0x0038D984 (0x294) iGetAnimations__FP7XStreamPUiPP8tagXAnim
void iGetAnimations(class XStream *stream, unsigned int *count, class tagXAnim **anims) {
    { // 0x0038D6F0 - 0x0038D984
        class tagiAnimFrame *joint; // r4
        class tagiAnimResource *ar; // r22
        float *data; // r21
        signed int frameCount; // r20
        signed int a; // r19
        unsigned int j; // r18
        class tagXAnim *xa; // r17
        signed int animCount; // r16
        unsigned int *pCount; // r2
    }
}

// 0x0038D990 - 0x0038DBA0 (0x210) iGet3DObjInfo__FP7XStreamP11_i3DObjInfo
void iGet3DObjInfo(class XStream *stream, class _i3DObjInfo *info) {
    { // 0x0038D990 - 0x0038DBA0
        unsigned char *gData; // r3
        signed int softSkin; // r8
        class _iGeomInfo *geom; // r19
        class tagP2Mesh *next; // r2
        class tagP2Mesh *curr; // r18
        class tagP2Mesh *meshBuffer; // r17
        unsigned int *ptr; // r3
        unsigned int i; // r16
        char *data; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iAssert.cpp
// ==========================================================================
signed int gAssertSkip; // 0x00000000
char vAssertBuf2[128]; // 0x00AFF460
char vAssertBuf1[128]; // 0x00000000
// 0x0038DBA0 - 0x0038DBA8 (0x8) iVisualAssert__FPcPc
void iVisualAssert() {
    { // 0x0038DBA0 - 0x0038DBA8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iAnim.cpp
// ==========================================================================
unsigned int animFrame; // 0x004A4F78
unsigned int animID; // 0x004A4F74
// 0x0038DBB0 - 0x0038DCA4 (0xF4) iAnimGetFrame__FP8tagXAnimiiP8Vector3DP8Vector3D
void iAnimGetFrame(class tagXAnim *anim, signed int joint, signed int frame, class Vector3D *trans, class Vector3D *rot) {
    { // 0x0038DBB0 - 0x0038DCA4
        class tagiAnimFrame *aj; // r9
        class tagiAnimResource *ar; // r9
    }
}

// 0x0038DCB0 - 0x0038DEFC (0x24C) iAnimSetCharJoints__FP15tagXAnimControlP15tagXAnimTrigger
signed int iAnimSetCharJoints(class tagXAnimControl *act) {
    { // 0x0038DCB0 - 0x0038DEFC
        signed int frame; // r2
        signed int frame; // r2
        signed int activeJoint; // r7
        signed int i; // r5
        class tagiSkelRuntime *skel; // r3
        class tagiAnimResource *data; // r2
        class _xActor *actor; // r3
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iActor.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
signed int gPlayerType; // 0x004A4E00
class xScene *xScene_current; // 0x004A4E08
float gActorShadowHeight; // 0x004A4F7C
unsigned short g_tpageInfo[20]; // 0x00000000
signed int g_textureCount; // 0x00000000
static char dbfont_chars[51]; // 0x00000000
// 0x0038DF00 - 0x0038E078 (0x178) iActorDraw__FP7_xActor
void iActorDraw(class _xActor *act) {
    { // 0x0038DF00 - 0x0038E078
        class Vector3D pos; // r29+0x80
        float m[16]; // r29+0x40
        class _p2Geom *geom; // r2
        float *fp; // r2
        class tagxPhysicsObject *p; // r18
        class iActorRuntime *r; // r16
    }
}

// 0x0038E080 - 0x0038E218 (0x198) iActorGeometryControl__FP7_xActorii
void iActorGeometryControl(class _xActor *a, signed int partNumber, signed int flag) {
    { // 0x0038E080 - 0x0038E218
    }
}

// 0x0038E220 - 0x0038E338 (0x118) iActorInitialize__FP7_xActor
void iActorInitialize(class _xActor *a) {
    { // 0x0038E220 - 0x0038E338
        signed int i; // r3
        class iActorRuntime *r; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\i_prim.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class xScene *xScene_current; // 0x004A4E08
unsigned int dpx; // 0x004A4E2C
unsigned int dpy; // 0x004A4E30
float dbfont_dy; // 0x00000000
float dbfont_dx; // 0x00000000
unsigned int dumpit; // 0x00000000
float dbfont_z; // 0x00000000
static signed int sDBFontInit; // 0x00000000
char *dbfp; // 0x00000000
char dbfont_buf[1430]; // 0x00000000
char _dbfbuf[256]; // 0x00000000
static class Vector3D origin; // 0x00000000
float line_z_bias; // 0x00000000
float line_width; // 0x00000000
class xColor line_color; // 0x004A4F88
static unsigned short sLineVertIndex[4000]; // 0x00AFF4E0
static unsigned int num_lines; // 0x004A4F84
static class _LineRec *lines; // 0x004A4F80
static char dbfont_chars[51]; // 0x00000000
// 0x0038E340 - 0x0038E7E0 (0x4A0) p2PrimDrawHealthBar__Fffff
void p2PrimDrawHealthBar(float x0, float y0, float x1, float y1) {
    { // 0x0038E340 - 0x0038E7E0
        unsigned short index[5]; // r29+0x110
        class _RwSky2DVertex vert[4]; // r29+0x10
    }
}

// 0x0038E7E0 - 0x0038E7E8 (0x8) DBFontFlush__Fv
void DBFontFlush() {
    { // 0x0038E7E0 - 0x0038E7E8
    }
}

// 0x0038E7F0 - 0x0038E7F8 (0x8) DBFontExit__Fv
void DBFontExit() {
    { // 0x0038E7F0 - 0x0038E7F8
    }
}

// 0x0038E800 - 0x0038E884 (0x84) RenderLines__Fv
void RenderLines() {
    { // 0x0038E800 - 0x0038E884
        class Vector3D *offset; // r2
        class _LineRec *l; // r16
    }
}

// 0x0038E890 - 0x0038EA5C (0x1CC) DrawLineSC__Fffffff
void DrawLineSC(float ax, float ay, float az, float bx, float by, float bz) {
    { // 0x0038E890 - 0x0038EA5C
        class _LineRec *l; // r7
    }
}

// 0x0038EA60 - 0x0038EC8C (0x22C) DrawLine__Fffffff
void DrawLine(float ax, float ay, float az, float bx, float by, float bz) {
    { // 0x0038EA60 - 0x0038EC8C
        class _LineRec *l; // r7
    }
}

// 0x0038EC90 - 0x0038ECFC (0x6C) iPrimDrawPoly__FPC8Vector3DPC8Vector3DPC8Vector3D
void iPrimDrawPoly(class Vector3D *vert0, class Vector3D *vert1, class Vector3D *vert2) {
    { // 0x0038EC90 - 0x0038ECFC
        class Vector3D *offset; // r2
    }
}

// 0x0038ED00 - 0x0038ED0C (0xC) ResetLines__Fv
void ResetLines() {
    { // 0x0038ED00 - 0x0038ED0C
    }
}

// 0x0038ED10 - 0x0038EE34 (0x124) InitLines__Fv
void InitLines() {
    { // 0x0038ED10 - 0x0038EE34
        signed int i; // r5
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\i_mat.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
float CY; // 0x00000000
float CX; // 0x00000000
float SCALE; // 0x00000000
static signed short matrix_stack_maxdepth; // 0x004A4F98
static signed short matrix_stack_depth; // 0x004A4F94
float (*matrix_sp)[16]; // 0x004A4F90
static float (*matrix_stack)[16]; // 0x004A4F8C
// 0x0038EE40 - 0x0038EF50 (0x110) RotMatrixZ__FPA16_ff
void RotMatrixZ(float (*a)[16], float ang) {
    { // 0x0038EE40 - 0x0038EF50
        float rot[16]; // r29+0x40
        float (*m)[16]; // r16
        float c; // 
        float s; // 
    }
}

// 0x0038EF50 - 0x0038F060 (0x110) RotMatrixY__FPA16_ff
void RotMatrixY(float (*a)[16], float ang) {
    { // 0x0038EF50 - 0x0038F060
        float rot[16]; // r29+0x40
        float (*m)[16]; // r16
        float c; // 
        float s; // 
    }
}

// 0x0038F060 - 0x0038F170 (0x110) RotMatrixX__FPA16_ff
void RotMatrixX(float (*a)[16], float ang) {
    { // 0x0038F060 - 0x0038F170
        float rot[16]; // r29+0x40
        float (*m)[16]; // r16
        float c; // 
        float s; // 
    }
}

// 0x0038F170 - 0x0038F238 (0xC8) ScaleMatrix__FPA16_ffff
void ScaleMatrix(float (*a)[16], float sx, float sy, float sz) {
    { // 0x0038F170 - 0x0038F238
        float S[16]; // r29+0x40
        float (*m)[16]; // r16
        float *s; // r2
    }
}

// 0x0038F240 - 0x0038F6D8 (0x498) MultMatrix__FPA16_fPA16_fPA16_f
void MultMatrix(float (*R)[16], float (*A)[16], float (*B)[16]) {
    { // 0x0038F240 - 0x0038F6D8
        float *r; // r5
        float *b; // r2
        float *a; // r2
        float temp[16]; // r29+0x60
        signed int usetemp; // r4
    }
}

// 0x0038F6E0 - 0x0038F73C (0x5C) IdMatrix__FPA16_f
void IdMatrix(float (*a)[16]) {
    { // 0x0038F6E0 - 0x0038F73C
        float *m; // r16
    }
}

// 0x0038F740 - 0x0038F76C (0x2C) SetCurrentMatrix__FPA16_f
void SetCurrentMatrix(float (*m)[16]) {
    { // 0x0038F740 - 0x0038F76C
    }
}

// 0x0038F770 - 0x0038F798 (0x28) GetCurrentMatrix__FPA16_f
void GetCurrentMatrix(float (*m)[16]) {
    { // 0x0038F770 - 0x0038F798
    }
}

// 0x0038F7A0 - 0x0038F898 (0xF8) PopMatrixStack__FUi
void PopMatrixStack(unsigned int num) {
    { // 0x0038F7A0 - 0x0038F898
    }
}

// 0x0038F8A0 - 0x0038F9A8 (0x108) PushMatrixStack__FPA16_f
void PushMatrixStack(float (*m)[16]) {
    { // 0x0038F8A0 - 0x0038F9A8
    }
}

// 0x0038F9B0 - 0x0038FBF8 (0x248) InitMatrixStack__FUi
void InitMatrixStack(unsigned int maxdepth) {
    { // 0x0038F9B0 - 0x0038FBF8
        unsigned int num; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xLook.cpp
// ==========================================================================
class xScene *xScene_current; // 0x004A4E08
float g_PI_DIV_2; // 0x00485A50
float g_PI; // 0x00485A40
float g_2_PI; // 0x00485A48
float g_PI_DIV_4; // 0x00000000
float g_DEGTORAD; // 0x00485A58
class tagxActorDBData *gadb; // 0x004A4658
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static class xLookMgr gLookMgr; // 0x00B01420
// 0x00396C70 - 0x00396EDC (0x26C) xLookActor__FP7_xActor
void xLookActor(class _xActor *a) {
    { // 0x00396C70 - 0x00396EDC
        float distance; // 
        unsigned short duration; // r17
        unsigned char priority; // r16
        class xLook *look; // r2
    }
}

// 0x00396EE0 - 0x003970BC (0x1DC) xLookActorRemove__FP7_xActor
void xLookActorRemove(class _xActor *a) {
    { // 0x00396EE0 - 0x003970BC
        class xLook *look; // r16
        signed int i; // r5
    }
}

// 0x003970C0 - 0x003973D4 (0x314) xLookObj__FP5_xObj
void xLookObj(class _xObj *o) {
    { // 0x003970C0 - 0x003973D4
        float distance; // 
        unsigned short duration; // r17
        unsigned char priority; // r16
        class xLook *look; // r2
    }
}

// 0x003973E0 - 0x00397508 (0x128) xLookObjRemove__FP5_xObj
void xLookObjRemove(class _xObj *o) {
    { // 0x003973E0 - 0x00397508
        class xLook *look; // r16
        signed int i; // r6
    }
}

// 0x00397510 - 0x003979B0 (0x4A0) xLookGetSlot__FUcUsfi
class xLook *xLookGetSlot(unsigned char priority, unsigned short duration, float distance, signed int addFlag) {
    { // 0x00397510 - 0x003979B0
        unsigned char flag; // r3
        class xLookMgr *mgr; // r7
        unsigned int equalIndex; // r2
        unsigned int lowestIndex; // r6
        unsigned int i; // r5
        class xLook *idx; // r4
        class xLook *look; // r16
    }
}

// 0x003979B0 - 0x00397B04 (0x154) xLookLevelReset__Fv
void xLookLevelReset() {
    { // 0x003979B0 - 0x00397B04
        class xLookMgr *mgr; // r18
        class xLook *look; // r17
        signed int i; // r16
    }
}

// 0x00397B10 - 0x00397F10 (0x400) xLookDebug__Fv
void xLookDebug() {
    { // 0x00397B10 - 0x00397F10
        class Vector3D d; // r29+0x170
        float distance; // r29+0x180
        class Vector3D b; // r29+0x160
        class Vector3D a; // r29+0x150
        char name[128]; // r29+0xc0
        char str[128]; // r29+0x40
        signed int hasLookAt; // r2
        class xLook *lookAt; // r2
        float lookAtDistance; // r29+0x180
        signed int good; // r18
        signed int priority; // r2
        class Vector3D p; // r29+0x140
        class xLook *look; // r17
        signed int i; // r16
    }
}

// 0x00397F10 - 0x00397F4C (0x3C) xLookInit__Fv
void xLookInit() {
    { // 0x00397F10 - 0x00397F4C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\skel\RWInfo.cpp
// ==========================================================================
class RWInfo gRWInfo; // 0x00B02B80

// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iEnvironment.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class Vector3D *gBSPOffset; // 0x004A4FA4
class _RwFrame *mDefaultLightFrame; // 0x004A4FA0
class _RpLight *mDefaultLight; // 0x004A4F9C
// 0x0039C6A0 - 0x0039C790 (0xF0) iEnvDraw__FP15_tagiEnvRuntime
void iEnvDraw(class _tagiEnvRuntime *env) {
    { // 0x0039C6A0 - 0x0039C790
    }
}

// 0x0039C790 - 0x0039C870 (0xE0) iEnvLoad__FP15_tagiEnvRuntimePc
signed int iEnvLoad(class _tagiEnvRuntime *env, char *filename) {
    { // 0x0039C790 - 0x0039C870
    }
}

// 0x0039C870 - 0x0039CCD4 (0x464) EnvRWLoadBSP__FP15_tagiEnvRuntimePc
static signed int EnvRWLoadBSP(class _tagiEnvRuntime *env, char *filename) {
    { // 0x0039C870 - 0x0039CCD4
        unsigned int f; // r16
        class _RwV3d *dir_z; // r2
        class _RwMatrix *m; // r16
        class _RwSurfaceProperties surf_props; // r29+0x60
        class _RwStream *stream; // r16
        class _RwV3d *offset; // r2
        class _RwRGBAReal rgb2; // r29+0x50
        class _RwRGBAReal rgb; // r29+0x40
    }
}

// 0x0039CCE0 - 0x0039CE68 (0x188) EnvRwLoadTextureDictionary__FP15_tagiEnvRuntimePc
static signed int EnvRwLoadTextureDictionary(class _tagiEnvRuntime *env, char *env_filename) {
    { // 0x0039CCE0 - 0x0039CE68
        class _RwTexDictionary *oldTexDict; // r16
        signed int len; // r2
        char filename[256]; // r29+0x30
        class _RwStream *stream; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xEnvironment.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x0039CE70 - 0x0039CFE4 (0x174) xEnvDraw__FP8_tagxEnv
void xEnvDraw(class _tagxEnv *env) {
    { // 0x0039CE70 - 0x0039CFE4
    }
}

// 0x0039CFF0 - 0x0039D10C (0x11C) xEnvLoad__FP8_tagxEnvPc
signed int xEnvLoad(class _tagxEnv *env, char *filename) {
    { // 0x0039CFF0 - 0x0039D10C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Lib\MAVA\JDebug.cpp
// ==========================================================================
static signed int currentIndentLevel; // 0x004A4FA8
// 0x0039D110 - 0x0039D19C (0x8C) JDebugPrintIntMessage__FPCci
void JDebugPrintIntMessage(char *msg, signed int num) {
    { // 0x0039D110 - 0x0039D19C
        char buf[256]; // r29+0x20
    }
}

// 0x0039D1A0 - 0x0039D22C (0x8C) JDebugPrintMessage__FPCc
void JDebugPrintMessage(char *msg) {
    { // 0x0039D1A0 - 0x0039D22C
        char buf[256]; // r29+0x20
    }
}

// 0x0039D230 - 0x0039D23C (0xC) JDebugSetIndentLevel__Fi
void JDebugSetIndentLevel(signed int newLevel) {
    { // 0x0039D230 - 0x0039D23C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Lib\MAVA\JFrame.cpp
// ==========================================================================
class JExecutableDescriptor gExecutableDescriptor; // 0x0044FEC0
// 0x0039D240 - 0x0039D280 (0x40) ExecutionFrameInit__FP14ExecutionFrameP17JExecutableMethodPUiPUi
void ExecutionFrameInit(class ExecutionFrame *inst, class JExecutableMethod *meth, unsigned int *lvp, unsigned int *opp) {
    { // 0x0039D240 - 0x0039D280
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Lib\MAVA\JInterpr.cpp
// ==========================================================================
unsigned char MemoryPoolDataPool[37888]; // 0x00B071F0
unsigned long gTimerValue; // 0x004A5340
unsigned int MemoryPoolAllocated; // 0x004A4FC8
unsigned int MemoryPoolLevelAllocated; // 0x004A4FCC
class JExecutableDescriptor gExecutableDescriptor; // 0x0044FEC0
static signed int sDebugMaxFrameCount[20]; // 0x00B071A0
static signed int sDebugMaxFrameCountInit; // 0x004A4FC4
unsigned long threadTotalTime[20]; // 0x00B06E00
signed int jumpBuffer[11]; // 0x00B06DD0
static signed int doLevelMemoryAllocate; // 0x004A4FC0
static unsigned int currentThreadFrameNumber; // 0x004A4FBC
static class ExecutionThread *currentThread; // 0x004A4FB8
static signed int externalFuncCount; // 0x004A4FB4
static class ExternalFunc externalFuncList[286]; // 0x00B06060
static signed int threadCount; // 0x004A4FB0
static class ExecutionThread threadList[20]; // 0x00B02B90
signed int debugThread; // 0x004A4688
signed int debugCallFlag; // 0x004A4684
signed int debugOpFlag; // 0x004A4FAC
// 0x0039D280 - 0x0039D624 (0x3A4) InterpreterDumpThreadsToScreen__Fii
char *InterpreterDumpThreadsToScreen() {
    { // 0x0039D280 - 0x0039D624
        char *className; // r19
        char *lastClass; // r18
        char dispStr[2048]; // r29+0x50
        signed int j; // r17
        signed int i; // r16
    }
}

// 0x0039D630 - 0x0039D8D4 (0x2A4) InterpreterDumpThreads__Fv
char *InterpreterDumpThreads() {
    { // 0x0039D630 - 0x0039D8D4
        char *className; // r19
        char *lastClass; // r18
        signed int j; // r17
        signed int i; // r16
    }
}

// 0x0039D8E0 - 0x003A238C (0x4AAC) InterpreterExecuteFrame__FP14ExecutionFrame
signed int InterpreterExecuteFrame(class ExecutionFrame *inputFrame) {
    { // 0x0039D8E0 - 0x003A238C
        signed int i; // r19
        class ExecutionFrame *prevFrame; // r19
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int constant; // r5
        signed int index; // r4
        unsigned int c2; // r5
        unsigned int c1; // r2
        unsigned int i2; // r4
        unsigned int i1; // r2
        unsigned int theConstValue; // r3
        signed short theConstMapValue; // r4
        signed int ind; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int instanceClassIndex; // r4
        void *ptr; // r2
        class JExecutableClass *file; // r3
        unsigned int *ref; // r2
        unsigned char *theArray; // r2
        unsigned int units; // r2
        unsigned int type; // r3
        unsigned char *theArray; // r2
        signed int units; // r2
        unsigned int theConstValue; // r2
        signed short theConstMapValue; // r2
        signed int ind; // r2
        signed int bc2; // r3
        signed int bc1; // r2
        unsigned int value; // r3
        class JExecutableField *newField; // r2
        signed int key; // r4
        signed int i; // r8
        signed int theOffset; // r3
        signed int searchKey; // r7
        signed int npairs; // r6
        signed int defaultOffset; // r2
        unsigned char *base; // r2
        signed int io; // r5
        signed int _off; // r3
        signed int index; // r7
        signed int highIndex; // r5
        signed int lowIndex; // r6
        signed int defaultOffset; // r2
        unsigned char *base; // r2
        unsigned int ind; // r2
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        unsigned int v2; // r3
        unsigned int v1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        unsigned int v2; // r3
        unsigned int v1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v2; // r3
        signed int v1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v2; // r3
        signed int v1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v2; // r3
        signed int v1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v2; // r3
        signed int v1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v2; // r3
        signed int v1; // r2
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v2; // r3
        signed int v1; // r2
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed short tmp; // r2
        signed int tmp; // r2
        unsigned int index; // r2
        unsigned int v1; // r3
        unsigned int v2; // r2
        unsigned int v1; // r3
        unsigned int v2; // r2
        unsigned int v1; // r3
        unsigned int v2; // r2
        unsigned int v1; // r3
        unsigned int v2; // r2
        signed int v1; // r3
        unsigned int v2; // r2
        signed int v1; // r3
        unsigned int v2; // r2
        signed int v1; // r2
        signed int v1; // r3
        signed int v2; // r2
        signed int v1; // r3
        signed int v2; // r2
        signed int b; // r3
        signed int a; // r2
        signed int v1; // r3
        signed int v2; // r2
        signed int b; // r3
        signed int a; // r2
        unsigned int w2; // r3
        unsigned int w1; // r2
        unsigned int w4; // r5
        unsigned int w3; // r4
        unsigned int w2; // r3
        unsigned int w1; // r2
        unsigned int w3; // r4
        unsigned int w2; // r3
        unsigned int w1; // r2
        unsigned int w2; // r3
        unsigned int w1; // r2
        unsigned int w3; // r4
        unsigned int w2; // r3
        unsigned int w1; // r2
        unsigned int w2; // r3
        unsigned int w1; // r2
        unsigned int *debugRef; // r5
        unsigned char *ref; // r2
        signed int index; // r4
        unsigned char value; // r3
        unsigned int *debugRef; // r5
        unsigned char *ref; // r2
        signed int index; // r4
        unsigned char value; // r3
        unsigned int *ref; // r5
        unsigned int index; // r4
        unsigned int value; // r3
        signed int *ref; // r5
        unsigned int index; // r4
        signed int value; // r3
        unsigned int *debugRef; // r4
        unsigned char *ref; // r2
        signed int index; // r3
        unsigned int *debugRef; // r4
        unsigned char *ref; // r2
        signed int index; // r3
        signed int *ref; // r4
        signed int index; // r3
        unsigned int theConstValue; // r2
        signed short theConstMapValue; // r4
        signed int ind; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        unsigned int val; // r2
        unsigned int theConstValue; // r2
        signed short theConstMapValue; // r4
        unsigned int ind; // r2
        unsigned int val; // r2
        signed short value; // r2
        unsigned short bc2; // r2
        unsigned short bc1; // r2
        unsigned int theConstValue; // r3
        signed short theConstMapValue; // r4
        signed int ind; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        unsigned char *theInstance; // r2
        class JExecutableClass *file; // r19
        unsigned int *ref; // r4
        unsigned int index; // r3
        signed int _off; // r4
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int i; // r6
        class ExecutionFrame *newFrame; // r2
        class JExecutableMethod *newMethod; // r19
        signed int v1; // r3
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int _off; // r2
        unsigned int bc2; // r3
        unsigned int bc1; // r2
        signed int ind; // r2
        signed int bc2; // r3
        signed int bc1; // r2
        class JExecutableField *newField; // r2
        unsigned int theConstValue; // r3
        signed int ind; // r2
        signed int bc2; // r3
        signed int bc1; // r2
        class JExecutableField *newField; // r2
        signed int ind; // r2
        signed int bc2; // r3
        signed int bc1; // r2
        void *ptr; // r4
        class JExecutableField *newField; // r2
        unsigned int fieldIndex; // r5
        signed int ind; // r2
        signed int bc2; // r3
        signed int bc1; // r2
        void *ptr; // r4
        class JExecutableField *newField; // r3
        unsigned int *lvp; // r6
        class ExecutionFrame *newFrame; // r2
        signed int i; // r5
        signed int j; // r20
        signed int i; // r19
        signed int threadToAwake; // r2
        signed int threadToSuspend; // r2
        unsigned int flags; // r3
        class ExternalFunc *theExternalFunc; // r29+0x130
        char *val; // r2
        signed int len; // r20
        signed int _off; // r3
        signed int parm8; // r2
        signed int parm7; // r2
        signed int parm6; // r29+0x120
        signed int parm5; // r29+0x110
        signed int parm4; // r29+0x100
        signed int parm3; // r23
        signed int parm2; // r19
        signed int parm1; // r22
        signed int _off2; // r4
        signed int bc2; // r3
        signed int bc1; // r2
        class JExecutableMethod *newMethod; // r19
        unsigned int theConstValue; // r2
        signed short theConstMapValue; // r2
        signed int ind; // r2
        signed int bc2; // r3
        signed int bc1; // r2
        void *ptr; // r19
        unsigned int value; // r20
        class JExecutableField *newField; // r22
        class JExecutableMethod *tm; // r2
        class JExecutableClass *instanceClass; // r2
        signed int _off; // r3
        signed int bc2; // r3
        signed int bc1; // r2
        signed int i; // r6
        class ExecutionFrame *newFrame; // r2
        class JExecutableMethod *newMethod; // r19
        class JExecutableMethod *tm; // r2
        class JExecutableClass *instanceClass; // r2
        unsigned int methodIndex; // r4
        signed int _off; // r4
        signed int bc2; // r3
        signed int bc1; // r2
        signed int i; // r6
        class ExecutionFrame *newFrame; // r2
        class JExecutableMethod *newMethod; // r19
        class JExecutableMethod *execMethodPool; // r29+0xf0
        class JExecutableField *execFieldPool; // r29+0xe0
        signed short *myExecConstantMapPool; // r29+0xd0
        unsigned int *execConstantValuePool; // r29+0xc0
        class JExecutableClass *execClassList; // r18
        unsigned int currentThreadFrameNumberSave; // r29+0xb0
        class ExecutionThread *currentThreadSave; // r29+0xa0
        unsigned int *fastLv; // r30
        unsigned int *fastOpTop; // r17
        unsigned char *fastPc; // r16
        unsigned int op; // r2
        static char buf2[256]; // 0x00B06FA0
        static char buf1[256]; // 0x00B06EA0
        static char buf3[256]; // 0x00B070A0
    }
}

// 0x003A2390 - 0x003A245C (0xCC) InterpreterRegisterExternalFunction__FiPCcPv
void InterpreterRegisterExternalFunction(signed int funcType, char *name, void *func) {
    { // 0x003A2390 - 0x003A245C
    }
}

// 0x003A2460 - 0x003A2744 (0x2E4) InterpreterDoExternalCallIII_I__FiiUiUiiiiPi
signed int InterpreterDoExternalCallIII_I(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr, signed int parm1, signed int parm2, signed int parm3, signed int *retVal) {
    { // 0x003A2460 - 0x003A2744
        signed int status; // r2
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A2750 - 0x003A29F8 (0x2A8) InterpreterDoExternalCallI_I__FiiUiUiiPi
signed int InterpreterDoExternalCallI_I(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr, signed int parm1, signed int *retVal) {
    { // 0x003A2750 - 0x003A29F8
        signed int status; // r2
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A2A00 - 0x003A2DB4 (0x3B4) InterpreterDoExternalCallIIIIIIIIIII_I__FiiUiUiiiiiiiiiiiiPi
signed int InterpreterDoExternalCallIIIIIIIIIII_I(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr, signed int parm1, signed int parm2, signed int parm3, signed int parm4, signed int parm5, signed int parm6, signed int parm7, signed int parm8, signed int parm9, signed int parm10, signed int parm11, signed int *retVal) {
    { // 0x003A2A00 - 0x003A2DB4
        signed int status; // r2
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A2DC0 - 0x003A309C (0x2DC) InterpreterDoExternalCallPIII__FiiUiUiiiii
signed int InterpreterDoExternalCallPIII(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr, signed int parm1, signed int parm2, signed int parm3, signed int parm4) {
    { // 0x003A2DC0 - 0x003A309C
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A30A0 - 0x003A337C (0x2DC) InterpreterDoExternalCallIIII__FiiUiUiiiii
signed int InterpreterDoExternalCallIIII(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr, signed int parm1, signed int parm2, signed int parm3, signed int parm4) {
    { // 0x003A30A0 - 0x003A337C
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A3380 - 0x003A3620 (0x2A0) InterpreterDoExternalCallII__FiiUiUiii
signed int InterpreterDoExternalCallII(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr, signed int parm1, signed int parm2) {
    { // 0x003A3380 - 0x003A3620
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A3620 - 0x003A38A4 (0x284) InterpreterDoExternalCallI__FiiUiUii
signed int InterpreterDoExternalCallI(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr, signed int parm1) {
    { // 0x003A3620 - 0x003A38A4
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A38B0 - 0x003A3B10 (0x260) InterpreterDoExternalCall__FiiUiUi
signed int InterpreterDoExternalCall(signed int threadFlag, signed int execFlag, unsigned int pauseFlags, unsigned int methodPtr) {
    { // 0x003A38B0 - 0x003A3B10
        unsigned int threadIndex; // r17
        class ExecutionThread *thisThread; // r16
        class JExecutableMethod *theMethod; // r2
    }
}

// 0x003A3B10 - 0x003A3C50 (0x140) InterpreterFindCall__FPCcPCcPCc
unsigned int InterpreterFindCall(char *className, char *methodName, char *descriptor) {
    { // 0x003A3B10 - 0x003A3C50
        class JExecutableMethod *theMethod; // r16
        class JExecutableClass *theClass; // r2
    }
}

// 0x003A3C50 - 0x003A40F8 (0x4A8) InterpreterProcessEvent__Fiii
unsigned int InterpreterProcessEvent(signed int objectType, signed int objectId, signed int eventType) {
    { // 0x003A3C50 - 0x003A40F8
        signed int *eventList; // r4
        signed int j; // r3
        signed int *eventList; // r4
        signed int j; // r3
        unsigned int returnCount; // r17
        signed int i; // r16
    }
}

// 0x003A4100 - 0x003A4534 (0x434) InterpreterExecuteThreads__FUi
void InterpreterExecuteThreads(unsigned int pauseMask) {
    { // 0x003A4100 - 0x003A4534
        unsigned int currentTime; // r2
        signed int threadStatus; // r2
        signed int i; // r16
    }
}

// 0x003A4540 - 0x003A4674 (0x134) InterpreterDestroyAllThreads__Fv
void InterpreterDestroyAllThreads() {
    { // 0x003A4540 - 0x003A4674
        signed int i; // r16
    }
}

// 0x003A4680 - 0x003A48A8 (0x228) InterpreterDestroyThreadSearch__Fiii
void InterpreterDestroyThreadSearch(signed int objectType, signed int objectId, signed int eventType) {
    { // 0x003A4680 - 0x003A48A8
        signed int i; // r16
    }
}

// 0x003A48B0 - 0x003A49E4 (0x134) InterpreterAllocateThread__Fv
unsigned int InterpreterAllocateThread() {
    { // 0x003A48B0 - 0x003A49E4
        signed int theThread; // r16
        signed int i; // r4
    }
}

// 0x003A49F0 - 0x003A4C18 (0x228) InterpreterInit__Fi
void InterpreterInit(signed int maxPools) {
    { // 0x003A49F0 - 0x003A4C18
        signed int j; // r4
        signed int i; // r5
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Lib\MAVA\JMem.cpp
// ==========================================================================
signed int jumpBuffer[11]; // 0x00B06DD0
signed int jumpBuffer[11]; // 0x00B06DD0
class JExecutableDescriptor gExecutableDescriptor; // 0x0044FEC0
unsigned int MemoryPoolLevelAllocated; // 0x004A4FCC
unsigned char MemoryPoolLevelPool[2560]; // 0x00B105F0
unsigned int MemoryPoolAllocated; // 0x004A4FC8
unsigned char MemoryPoolDataPool[37888]; // 0x00B071F0
// 0x003A4C20 - 0x003A4D20 (0x100) MemoryPoolLevelAllocate__Fii
void *MemoryPoolLevelAllocate(signed int unitCount, signed int unitSize) {
    { // 0x003A4C20 - 0x003A4D20
        void *retVal; // r2
    }
}

// 0x003A4D20 - 0x003A4E24 (0x104) MemoryPoolAllocate__Fii
void *MemoryPoolAllocate(signed int unitCount, signed int unitSize) {
    { // 0x003A4D20 - 0x003A4E24
        void *retVal; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Lib\MAVA\JMethod.cpp
// ==========================================================================
class JExecutableDescriptor gExecutableDescriptor; // 0x0044FEC0
// 0x003A4E30 - 0x003A4FA4 (0x174) JExecutableClassGetParentDataWords__FP16JExecutableClass
signed int JExecutableClassGetParentDataWords(class JExecutableClass *inst) {
    { // 0x003A4E30 - 0x003A4FA4
        class JExecutableClass *file; // r16
    }
}

// 0x003A4FB0 - 0x003A50C0 (0x110) JExecutableClassGetDataWords__FP16JExecutableClass
signed int JExecutableClassGetDataWords(class JExecutableClass *inst) {
    { // 0x003A4FB0 - 0x003A50C0
    }
}

// 0x003A50C0 - 0x003A5258 (0x198) JExecutableClassResolveMethodByStringIndex__FP16JExecutableClassUiUi
class JExecutableMethod *JExecutableClassResolveMethodByStringIndex(class JExecutableClass *inst, unsigned int nameIndex, unsigned int descrIndex) {
    { // 0x003A50C0 - 0x003A5258
        signed int i; // r7
        class JExecutableMethod *executableMethods; // r3
    }
}

// 0x003A5260 - 0x003A5544 (0x2E4) JExecutableClassResolveMethod__FP16JExecutableClassPCcPCc
class JExecutableMethod *JExecutableClassResolveMethod(class JExecutableClass *inst, char *name, char *descr) {
    { // 0x003A5260 - 0x003A5544
        signed int i; // r17
        class JExecutableMethod *executableMethods; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Lib\MAVA\JPools.cpp
// ==========================================================================
signed int jumpBuffer[11]; // 0x00B06DD0
class JExecutableDescriptor gExecutableDescriptor; // 0x0044FEC0

// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Lib\MAVA\JThread.cpp
// ==========================================================================
signed int jumpBuffer[11]; // 0x00B06DD0
signed int jumpBuffer[11]; // 0x00B06DD0
signed int jumpBuffer[11]; // 0x00B06DD0
signed int jumpBuffer[11]; // 0x00B06DD0
signed int maxOpCount; // 0x004A4FE4
signed int maxLvCount; // 0x004A4FE0
signed int currentOpCount; // 0x004A4FDC
signed int currentLvCount; // 0x004A4FD8
static signed int opLastFree; // 0x004A4FD4
static signed int opPoolMap[120]; // 0x00B120D0
static signed int lvLastFree; // 0x004A4FD0
static signed int lvPoolMap[120]; // 0x00B11EF0
static unsigned int ExecutionThreadOpStackPool[480]; // 0x00B11770
static unsigned int ExecutionThreadLvPool[480]; // 0x00B10FF0
// 0x003A5550 - 0x003A5690 (0x140) ExecutionThreadPopFrame__FP15ExecutionThread
class ExecutionFrame *ExecutionThreadPopFrame(class ExecutionThread *inst) {
    { // 0x003A5550 - 0x003A5690
    }
}

// 0x003A5690 - 0x003A5788 (0xF8) ExecutionThreadPushFrame__FP15ExecutionThreadP17JExecutableMethod
class ExecutionFrame *ExecutionThreadPushFrame(class ExecutionThread *inst, class JExecutableMethod *meth) {
    { // 0x003A5690 - 0x003A5788
        signed int lvIndex; // r17
        signed int opIndex; // r16
    }
}

// 0x003A5790 - 0x003A579C (0xC) ExecutionThreadStop__FP15ExecutionThread
void ExecutionThreadStop(class ExecutionThread *inst) {
    { // 0x003A5790 - 0x003A579C
    }
}

// 0x003A57A0 - 0x003A57B0 (0x10) ExecutionThreadStart__FP15ExecutionThread
void ExecutionThreadStart(class ExecutionThread *inst) {
    { // 0x003A57A0 - 0x003A57B0
    }
}

// 0x003A57B0 - 0x003A591C (0x16C) getOpSlot__Fi
static signed int getOpSlot(signed int size) {
    { // 0x003A57B0 - 0x003A591C
        signed int returnIndex; // r2
        signed int entriesNeeded; // r3
        signed int j; // r7
        signed int i; // r6
    }
}

// 0x003A5920 - 0x003A5AA4 (0x184) getLvSlot__Fi
static signed int getLvSlot(signed int size) {
    { // 0x003A5920 - 0x003A5AA4
        signed int returnIndex; // r2
        signed int entriesNeeded; // r3
        signed int j; // r7
        signed int i; // r6
    }
}

// 0x003A5AB0 - 0x003A5B00 (0x50) ExecutionThreadInitPools__Fv
void ExecutionThreadInitPools() {
    { // 0x003A5AB0 - 0x003A5B00
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xModule.cpp
// ==========================================================================
unsigned long m_currentTime; // 0x00B122B0

// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xModuleFactory.cpp
// ==========================================================================
struct @anon0 __vt__7xModule; // 0x004A2790
struct @anon1 __vt__8xAnimMgr; // 0x004A2840
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x003A5B00 - 0x003A5DF4 (0x2F4) create__14xModuleFactoryFi
class xModule *create(signed int type) {
    { // 0x003A5B00 - 0x003A5DF4
        class xModule *m; // r16
    }
}


// ==========================================================================
// C:\CodeWarrior\PS2 Support\Msl\MSL_C++\MSL_Common\Include\vector
// ==========================================================================
// 0x003A5E00 - 0x003A5E18 (0x18) __ct__Q23std38__vector_pod<Ui,Q23std13allocator<Ui>>Fv
void *__ct(class __vector_pod *this) {
    { // 0x003A5E00 - 0x003A5E18
    }
}

// 0x00400240 - 0x00400330 (0xF0) reserve__Q23std38__vector_pod<Ui,Q23std13allocator<Ui>>FUi
void reserve(class __vector_pod *this, unsigned int n) {
    { // 0x00400240 - 0x00400330
        unsigned int *olddata; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xModule.h
// ==========================================================================
// 0x003A5E20 - 0x003A5E28 (0x8) scriptRegister__7xModuleFv
void scriptRegister() {
    { // 0x003A5E20 - 0x003A5E28
    }
}

// 0x003A5E30 - 0x003A5E3C (0xC) instance__7xModuleFv
class xModule *instance() {
    { // 0x003A5E30 - 0x003A5E3C
    }
}

// 0x003A5E40 - 0x003A5E48 (0x8) render__7xModuleFv
void render() {
    { // 0x003A5E40 - 0x003A5E48
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zMain.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
unsigned long gTimerValue; // 0x004A5340
unsigned long m_currentTime; // 0x00B122B0
signed int gQuit; // 0x004A4F30
struct @anon0 __vt__5zMain; // 0x004A27C0
// 0x003A5E50 - 0x003A6030 (0x1E0)
signed int main() {
    { // 0x003A5E50 - 0x003A6030
        class zMain *sd2Main; // r16
    }
}

// 0x003A6030 - 0x003A609C (0x6C) render__5zMainFv
void render(class zMain *this) {
    { // 0x003A6030 - 0x003A609C
    }
}

// 0x003A60A0 - 0x003A6110 (0x70) nextFrame__5zMainFv
void nextFrame(class zMain *this) {
    { // 0x003A60A0 - 0x003A6110
    }
}

// 0x003A6110 - 0x003A6170 (0x60) restart__5zMainFv
void restart(class zMain *this) {
    { // 0x003A6110 - 0x003A6170
    }
}

// 0x003A6170 - 0x003A61D8 (0x68) kill__5zMainFv
void kill(class zMain *this) {
    { // 0x003A6170 - 0x003A61D8
    }
}

// 0x003A61E0 - 0x003A62E8 (0x108) init__5zMainFv
void init(class zMain *this) {
    { // 0x003A61E0 - 0x003A62E8
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xGuiScript.cpp
// ==========================================================================
void (xScriptGuiSysAddFlag)(signed int, signed int); // 0x003A6620
void (xScriptGuiSysAdjustPos)(signed int, signed int, signed int, signed int); // 0x003AAD70
signed int (xScriptGuiSysCreate)(signed int); // 0x003AC010
signed int (xScriptGuiSysGetFlag)(signed int); // 0x003A62F0
void (xScriptGuiSysLink)(signed int, signed int, signed int, signed int); // 0x003AA8A0
void (xScriptGuiSysRemoveFlag)(signed int, signed int); // 0x003A67C0
void (xScriptGuiSysSet)(signed int); // 0x003ABA00
void (xScriptGuiSysSetFlag)(signed int, signed int); // 0x003A6480
void (xScriptGuiSysSetPadObj)(signed int, signed int); // 0x003ABB90
void (xScriptGuiSysSetPos)(signed int, signed int, signed int, signed int); // 0x003AB840
void (xScriptGuiSysSetDim)(signed int, signed int, signed int, signed int); // 0x003AABB0
void (xScriptGuiSysSetRenderFunc)(signed int, signed int); // 0x003ABD20
void (xScriptGuiSysSetOffset)(signed int, signed int, signed int); // 0x003AB690
void (xScriptGuiSysAdjustOffset)(signed int, signed int, signed int); // 0x003AB4D0
void (xScriptGuiSysOffsetMoveTo)(signed int, signed int, signed int, signed int, signed int); // 0x003AAF30
signed int (xScriptGuiSysAtOffset)(signed int, signed int, signed int); // 0x003AB300
void (xScriptGuiItemAdvanceText)(signed int, signed int); // 0x003AA490
signed int (xScriptGuiItemCreate)(signed int, signed int); // 0x003AA630
void (xScriptGuiItemSelect)(signed int, signed int); // 0x003AA1A0
void (xScriptGuiItemFuncCall)(signed int, signed int, signed int); // 0x003A9B80
void (xScriptGuiItemSetDataID)(signed int, signed int); // 0x003A7900
void (xScriptGuiItemSetFlag)(signed int, signed int); // 0x003A6AF0
void (xScriptGuiItemSetPos)(signed int, signed int, signed int, signed int); // 0x003A9630
void (xScriptGuiItemSetOffset)(signed int, signed int, signed int); // 0x003A99C0
void (xScriptGuiItemAdjustOffset)(signed int, signed int, signed int); // 0x003A97F0
void (xScriptGuiItemSetRenderFunc)(signed int, signed int); // 0x003A9E80
void (xScriptGuiItemSetPadObj)(signed int, signed int); // 0x003A6FD0
void (xScriptGuiItemSetColor)(signed int, signed int, signed int, signed int, signed int); // 0x003A8C00
void (xScriptGuiItemSetMayaInfo)(signed int, signed int, signed int, signed int); // 0x003A8760
void (xScriptGuiItemSetMayaInfoTextureID)(signed int, signed int); // 0x003A85D0
void (xScriptGuiItemSetMayaInfoModelID)(signed int, signed int); // 0x003A8440
void (xScriptGuiItemSetMayaVal)(signed int, signed int, signed int); // 0x003A8010
void (xScriptGuiItemSetPartID)(signed int, signed int); // 0x003A89C0
signed int (xScriptGuiItemGetDataID)(signed int); // 0x003A92E0
void (xScriptGuiItemSetPartUVUsingPart)(signed int, signed int, signed int); // 0x003A7DB0
void (xScriptGuiItemAdjustPos)(signed int, signed int, signed int, signed int); // 0x003A9470
void (xScriptGuiItemLink)(signed int, signed int, signed int, signed int); // 0x003A7AA0
signed int (xScriptGuiItemGetFlag)(signed int); // 0x003A6960
void (xScriptGuiItemAddFlag)(signed int, signed int); // 0x003A6C90
void (xScriptGuiItemRemoveFlag)(signed int, signed int); // 0x003A6E30
void (iGuiItemRenderFunc_DebugOutline)(class _xGuiItem *, class _xGuiSys *); // 0x00386580
void (iGuiItemRenderFunc_Image)(class _xGuiItem *, class _xGuiSys *); // 0x00386530
void (iGuiItemRenderFunc_Text)(class _xGuiItem *, class _xGuiSys *); // 0x003857D0
void (iGuiItemRenderFunc_TextNumber)(class _xGuiItem *, class _xGuiSys *); // 0x00385F90
void (iGuiItemRenderFunc_Maya)(class _xGuiItem *, class _xGuiSys *); // 0x00385330
void (iGuiItemRenderFunc_MayaClip)(class _xGuiItem *, class _xGuiSys *); // 0x00384DF0
class _xGuiSys *gActiveGuiSys; // 0x004A4DF0
class _xGuiItem gGuiItem[180]; // 0x00AA1800
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _xGuiSys gGuiSys[52]; // 0x00AA5B80
static unsigned int interpreterMethodSys; // 0x004A4FEC
static unsigned int interpreterMethodItem; // 0x004A4FE8
// 0x003A62F0 - 0x003A6478 (0x188) xScriptGuiSysGetFlag__Fi
signed int xScriptGuiSysGetFlag(signed int sysIdx) {
    { // 0x003A62F0 - 0x003A6478
    }
}

// 0x003A6480 - 0x003A6618 (0x198) xScriptGuiSysSetFlag__Fii
void xScriptGuiSysSetFlag(signed int sysIdx, signed int flag) {
    { // 0x003A6480 - 0x003A6618
    }
}

// 0x003A6620 - 0x003A67B8 (0x198) xScriptGuiSysAddFlag__Fii
void xScriptGuiSysAddFlag(signed int sysIdx, signed int flag) {
    { // 0x003A6620 - 0x003A67B8
    }
}

// 0x003A67C0 - 0x003A6958 (0x198) xScriptGuiSysRemoveFlag__Fii
void xScriptGuiSysRemoveFlag(signed int sysIdx, signed int flag) {
    { // 0x003A67C0 - 0x003A6958
    }
}

// 0x003A6960 - 0x003A6AE8 (0x188) xScriptGuiItemGetFlag__Fi
signed int xScriptGuiItemGetFlag(signed int itemIdx) {
    { // 0x003A6960 - 0x003A6AE8
    }
}

// 0x003A6AF0 - 0x003A6C88 (0x198) xScriptGuiItemSetFlag__Fii
void xScriptGuiItemSetFlag(signed int itemIdx, signed int flag) {
    { // 0x003A6AF0 - 0x003A6C88
    }
}

// 0x003A6C90 - 0x003A6E28 (0x198) xScriptGuiItemAddFlag__Fii
void xScriptGuiItemAddFlag(signed int itemIdx, signed int flag) {
    { // 0x003A6C90 - 0x003A6E28
    }
}

// 0x003A6E30 - 0x003A6FC8 (0x198) xScriptGuiItemRemoveFlag__Fii
void xScriptGuiItemRemoveFlag(signed int itemIdx, signed int flag) {
    { // 0x003A6E30 - 0x003A6FC8
    }
}

// 0x003A6FD0 - 0x003A7160 (0x190) xScriptGuiItemSetPadObj__Fii
void xScriptGuiItemSetPadObj(signed int itemIdx, signed int padobj) {
    { // 0x003A6FD0 - 0x003A7160
    }
}

// 0x003A7160 - 0x003A75A4 (0x444) xPadMapperFunc_GuiMenu__FP7tagxPadP13tagxPadActioni
signed int xPadMapperFunc_GuiMenu(class tagxPad *pad, class tagxPadAction *actions, signed int maxActions) {
    { // 0x003A7160 - 0x003A75A4
        class tagxPadAction *a; // r5
        class _XBUTTON *b; // r4
        signed int i; // r3
        signed int actionCount; // r2
    }
}

// 0x003A75B0 - 0x003A77EC (0x23C) xPadActionFunc_GuiMenu__FP7tagxPadiP13tagxPadAction
void xPadActionFunc_GuiMenu(signed int count, class tagxPadAction *actions) {
    { // 0x003A75B0 - 0x003A77EC
        class _xGuiSys *activeSys; // r18
        class tagxPadAction *a; // r17
        signed int i; // r16
    }
}

// 0x003A77F0 - 0x003A78FC (0x10C) xGuiSendPadAction__FP8_xGuiSysP9_xGuiItemi
void xGuiSendPadAction(class _xGuiSys *sys, class _xGuiItem *item, signed int action) {
    { // 0x003A77F0 - 0x003A78FC
    }
}

// 0x003A7900 - 0x003A7A98 (0x198) xScriptGuiItemSetDataID__Fii
void xScriptGuiItemSetDataID(signed int itemIdx, signed int dataID) {
    { // 0x003A7900 - 0x003A7A98
    }
}

// 0x003A7AA0 - 0x003A7DB0 (0x310) xScriptGuiItemLink__Fiiii
void xScriptGuiItemLink(signed int itemA, signed int dira, signed int itemB, signed int dirb) {
    { // 0x003A7AA0 - 0x003A7DB0
    }
}

// 0x003A7DB0 - 0x003A8004 (0x254) xScriptGuiItemSetPartUVUsingPart__Fiii
void xScriptGuiItemSetPartUVUsingPart(signed int itemIdx, signed int modelID, signed int partID) {
    { // 0x003A7DB0 - 0x003A8004
        class _xGuiItem *i; // r2
    }
}

// 0x003A8010 - 0x003A843C (0x42C) xScriptGuiItemSetMayaVal__Fiii
void xScriptGuiItemSetMayaVal(signed int itemIdx, signed int valIdx, signed int value) {
    { // 0x003A8010 - 0x003A843C
        class _xGuiItem *i; // r3
    }
}

// 0x003A8440 - 0x003A85D0 (0x190) xScriptGuiItemSetMayaInfoModelID__Fii
void xScriptGuiItemSetMayaInfoModelID(signed int itemIdx, signed int modelID) {
    { // 0x003A8440 - 0x003A85D0
        class _xGuiItem *i; // r3
    }
}

// 0x003A85D0 - 0x003A8760 (0x190) xScriptGuiItemSetMayaInfoTextureID__Fii
void xScriptGuiItemSetMayaInfoTextureID(signed int itemIdx, signed int textureID) {
    { // 0x003A85D0 - 0x003A8760
        class _xGuiItem *i; // r3
    }
}

// 0x003A8760 - 0x003A89B4 (0x254) xScriptGuiItemSetMayaInfo__Fiiii
void xScriptGuiItemSetMayaInfo(signed int itemIdx, signed int modelID, signed int textureID, signed int partID) {
    { // 0x003A8760 - 0x003A89B4
        class _xGuiItem *i; // r3
    }
}

// 0x003A89C0 - 0x003A8BF4 (0x234) xScriptGuiItemSetPartID__Fii
void xScriptGuiItemSetPartID(signed int itemIdx, signed int partID) {
    { // 0x003A89C0 - 0x003A8BF4
        class _xGuiItem *i; // r3
    }
}

// 0x003A8C00 - 0x003A92D8 (0x6D8) xScriptGuiItemSetColor__Fiiiii
void xScriptGuiItemSetColor(signed int itemIdx, signed int r, signed int g, signed int b, signed int a) {
    { // 0x003A8C00 - 0x003A92D8
    }
}

// 0x003A92E0 - 0x003A9464 (0x184) xScriptGuiItemGetDataID__Fi
signed int xScriptGuiItemGetDataID(signed int itemIdx) {
    { // 0x003A92E0 - 0x003A9464
        class _xGuiItem *i; // r2
    }
}

// 0x003A9470 - 0x003A9628 (0x1B8) xScriptGuiItemAdjustPos__Fiiii
void xScriptGuiItemAdjustPos(signed int itemIdx, signed int dx, signed int dy, signed int dz) {
    { // 0x003A9470 - 0x003A9628
    }
}

// 0x003A9630 - 0x003A97E8 (0x1B8) xScriptGuiItemSetPos__Fiiii
void xScriptGuiItemSetPos(signed int itemIdx, signed int x, signed int y, signed int z) {
    { // 0x003A9630 - 0x003A97E8
    }
}

// 0x003A97F0 - 0x003A99C0 (0x1D0) xScriptGuiItemAdjustOffset__Fiii
void xScriptGuiItemAdjustOffset(signed int itemIdx, signed int dx, signed int dy) {
    { // 0x003A97F0 - 0x003A99C0
    }
}

// 0x003A99C0 - 0x003A9B80 (0x1C0) xScriptGuiItemSetOffset__Fiii
void xScriptGuiItemSetOffset(signed int itemIdx, signed int dx, signed int dy) {
    { // 0x003A99C0 - 0x003A9B80
    }
}

// 0x003A9B80 - 0x003A9E80 (0x300) xScriptGuiItemFuncCall__Fiii
void xScriptGuiItemFuncCall(signed int sysIdx, signed int idx, signed int func) {
    { // 0x003A9B80 - 0x003A9E80
    }
}

// 0x003A9E80 - 0x003AA1A0 (0x320) xScriptGuiItemSetRenderFunc__Fii
void xScriptGuiItemSetRenderFunc(signed int itemIdx, signed int funcID) {
    { // 0x003A9E80 - 0x003AA1A0
    }
}

// 0x003AA1A0 - 0x003AA490 (0x2F0) xScriptGuiItemSelect__Fii
void xScriptGuiItemSelect(signed int sysIdx, signed int idx) {
    { // 0x003AA1A0 - 0x003AA490
    }
}

// 0x003AA490 - 0x003AA628 (0x198) xScriptGuiItemAdvanceText__Fii
void xScriptGuiItemAdvanceText(signed int idx, signed int amount) {
    { // 0x003AA490 - 0x003AA628
    }
}

// 0x003AA630 - 0x003AA898 (0x268) xScriptGuiItemCreate__Fii
signed int xScriptGuiItemCreate(signed int flag, signed int sysIdx) {
    { // 0x003AA630 - 0x003AA898
        class _xGuiItem *item; // r16
    }
}

// 0x003AA8A0 - 0x003AABB0 (0x310) xScriptGuiSysLink__Fiiii
void xScriptGuiSysLink(signed int sysA, signed int dira, signed int sysB, signed int dirb) {
    { // 0x003AA8A0 - 0x003AABB0
    }
}

// 0x003AABB0 - 0x003AAD68 (0x1B8) xScriptGuiSysSetDim__Fiiii
void xScriptGuiSysSetDim(signed int sysIdx, signed int w, signed int h, signed int d) {
    { // 0x003AABB0 - 0x003AAD68
    }
}

// 0x003AAD70 - 0x003AAF28 (0x1B8) xScriptGuiSysAdjustPos__Fiiii
void xScriptGuiSysAdjustPos(signed int sysIdx, signed int dx, signed int dy, signed int dz) {
    { // 0x003AAD70 - 0x003AAF28
    }
}

// 0x003AAF30 - 0x003AB2F8 (0x3C8) xScriptGuiSysOffsetMoveTo__Fiiiii
void xScriptGuiSysOffsetMoveTo(signed int sysID, signed int xoff, signed int yoff, signed int dx, signed int dy) {
    { // 0x003AAF30 - 0x003AB2F8
        class _xGuiSys *sys; // r4
    }
}

// 0x003AB300 - 0x003AB4C8 (0x1C8) xScriptGuiSysAtOffset__Fiii
signed int xScriptGuiSysAtOffset(signed int sysID, signed int xoff, signed int yoff) {
    { // 0x003AB300 - 0x003AB4C8
        class _xGuiSys *sys; // r4
    }
}

// 0x003AB4D0 - 0x003AB688 (0x1B8) xScriptGuiSysAdjustOffset__Fiii
void xScriptGuiSysAdjustOffset(signed int sysIdx, signed int dx, signed int dy) {
    { // 0x003AB4D0 - 0x003AB688
        class _xGuiSys *sys; // r5
    }
}

// 0x003AB690 - 0x003AB838 (0x1A8) xScriptGuiSysSetOffset__Fiii
void xScriptGuiSysSetOffset(signed int sysIdx, signed int dx, signed int dy) {
    { // 0x003AB690 - 0x003AB838
        class _xGuiSys *sys; // r4
    }
}

// 0x003AB840 - 0x003AB9F8 (0x1B8) xScriptGuiSysSetPos__Fiiii
void xScriptGuiSysSetPos(signed int sysIdx, signed int x, signed int y, signed int z) {
    { // 0x003AB840 - 0x003AB9F8
    }
}

// 0x003ABA00 - 0x003ABB88 (0x188) xScriptGuiSysSet__Fi
void xScriptGuiSysSet(signed int idx) {
    { // 0x003ABA00 - 0x003ABB88
    }
}

// 0x003ABB90 - 0x003ABD20 (0x190) xScriptGuiSysSetPadObj__Fii
void xScriptGuiSysSetPadObj(signed int idx, signed int padobj) {
    { // 0x003ABB90 - 0x003ABD20
    }
}

// 0x003ABD20 - 0x003AC010 (0x2F0) xScriptGuiSysSetRenderFunc__Fii
void xScriptGuiSysSetRenderFunc(signed int sys, signed int funcID) {
    { // 0x003ABD20 - 0x003AC010
    }
}

// 0x003AC010 - 0x003AC108 (0xF8) xScriptGuiSysCreate__Fi
signed int xScriptGuiSysCreate(signed int flag) {
    { // 0x003AC010 - 0x003AC108
        class _xGuiSys *sys; // r16
    }
}

// 0x003AC110 - 0x003AC120 (0x10) xGuiReInitializeInterpreter__Fv
void xGuiReInitializeInterpreter() {
    { // 0x003AC110 - 0x003AC120
    }
}

// 0x003AC120 - 0x003AC598 (0x478) xGuiRegisterExternals__Fv
void xGuiRegisterExternals() {
    { // 0x003AC120 - 0x003AC598
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xActorDBScript.cpp
// ==========================================================================
void (actorSetVisible)(signed int, signed int, signed int); // 0x003EA3A0
void (actorSetAttrFOV)(signed int, signed int, signed int); // 0x003E7FF0
void (actorSetAttrSight)(signed int, signed int, signed int); // 0x003E7C30
void (actorSetBRadius)(signed int, signed int, signed int); // 0x003E7880
void (actorDestroy)(signed int, signed int); // 0x003E7420
signed int (actorCreate)(signed int, signed int, signed int, signed int); // 0x003E7780
void (actorSetTexture)(signed int, signed int, signed int); // 0x003E7440
void (xActorDBSetRoute)(signed int, signed int, signed int); // 0x00367190
signed int (actorRouteStatus)(signed int, signed int); // 0x003E9270
signed int (actorRoutePosition)(signed int, signed int); // 0x003E9080
signed int (xActorDBConsoleAction)(signed int, signed int, signed int); // 0x00366FC0
signed int (actorInTrigger)(signed int, signed int, signed int); // 0x003DA3E0
signed int (actorInEntryPoint)(signed int, signed int, signed int); // 0x003DA8E0
void (actorSetCollisionFlags)(signed int, signed int, signed int); // 0x003D9060
signed int (actorGetAngle)(signed int, signed int); // 0x003E16A0
signed int (actorGetRelativeAngle)(signed int, signed int, signed int, signed int); // 0x003DD330
signed int (actorGetRelativeMarkerAngle)(signed int, signed int, signed int); // 0x003DCFF0
void (actorTurn)(signed int, signed int, signed int); // 0x003D9680
signed int (actorGetNearestMarker)(signed int, signed int, signed int, signed int); // 0x003DC410
signed int (actorGetNearestNavPoint)(signed int, signed int, signed int, signed int); // 0x003DC860
signed int (actorGetPositionX)(signed int, signed int); // 0x003EA140
signed int (actorGetPositionY)(signed int, signed int); // 0x003E9EE0
signed int (actorGetPositionZ)(signed int, signed int); // 0x003E9C80
signed int (actorGetRotY)(signed int, signed int); // 0x003E9A20
signed int (actorGetFlags)(signed int, signed int); // 0x003E97D0
signed int (actorHaveLOSToDir)(signed int, signed int, signed int, signed int); // 0x003E11D0
signed int (actorHaveLOSToMarker)(signed int, signed int, signed int); // 0x003E0C30
signed int (actorHaveLOSToSpot)(signed int, signed int, signed int); // 0x003DFF70
signed int (actorHaveLOSToNavPoint)(signed int, signed int, signed int); // 0x003DF4C0
signed int (actorHaveLOSToActor)(signed int, signed int, signed int, signed int); // 0x003E0520
signed int (actorHaveLOSToObject)(signed int, signed int, signed int); // 0x003DFA70
signed int (actorIsFacingActor)(signed int, signed int, signed int, signed int, signed int); // 0x003DDE80
signed int (actorIsFacingMarker)(signed int, signed int, signed int, signed int); // 0x003DEF60
signed int (actorIsFacingNavPoint)(signed int, signed int, signed int, signed int); // 0x003DE9F0
signed int (actorIsFacingPosition)(signed int, signed int, signed int, signed int); // 0x003DE4A0
signed int (actorIsFacingObj)(signed int, signed int, signed int, signed int); // 0x003DD880
void (actorFace)(signed int, signed int, signed int); // 0x003D9360
void (actorFaceActor)(signed int, signed int, signed int, signed int); // 0x003E2930
void (actorFaceMarker)(signed int, signed int, signed int); // 0x003E38D0
void (actorFaceNavPoint)(signed int, signed int, signed int); // 0x003E3630
void (actorFacePosition)(signed int, signed int, signed int); // 0x003E2DD0
void (actorFaceObject)(signed int, signed int, signed int); // 0x003E31B0
signed int (actorGetPickupObject)(signed int, signed int, signed int); // 0x003DCCB0
signed int (actorMoveStrafe)(signed int, signed int, signed int); // 0x003E24F0
signed int (actorMoveForward)(signed int, signed int, signed int); // 0x003E21D0
signed int (actorMoveVertical)(signed int, signed int, signed int); // 0x003E19A0
signed int (actorInFOV)(signed int, signed int, signed int, signed int); // 0x003D9F60
signed int (actorIsVisible)(signed int, signed int, signed int, signed int); // 0x003D99A0
signed int (actorDistanceToActor)(signed int, signed int, signed int, signed int); // 0x003E6DD0
signed int (actorDistanceToActor3D)(signed int, signed int, signed int, signed int); // 0x003E67A0
signed int (actorDistanceToObj)(signed int, signed int, signed int); // 0x003E61B0
signed int (actorDistanceToObj3D)(signed int, signed int, signed int); // 0x003E5BE0
signed int (actorDistanceToNavPoint)(signed int, signed int, signed int); // 0x003E4000
signed int (actorDistanceToNavPoint3D)(signed int, signed int, signed int); // 0x003E3B60
signed int (actorDistanceToMarker)(signed int, signed int, signed int); // 0x003E4970
signed int (actorDistanceToMarker3D)(signed int, signed int, signed int); // 0x003E44E0
signed int (actorDistanceToRecordedPosition)(signed int, signed int, signed int); // 0x003E5170
signed int (actorDistanceToRecordedPosition3D)(signed int, signed int, signed int); // 0x003E4E30
signed int (actorDistanceBetweenSpots)(signed int, signed int, signed int, signed int); // 0x003E54E0
signed int (actorDistanceBetweenSpots3D)(signed int, signed int, signed int, signed int); // 0x003E5880
void (actorMoveAwayFromActor)(signed int, signed int, signed int, signed int, signed int); // 0x003E1C20
void (actorRecordActorSpot)(signed int, signed int, signed int, signed int, signed int); // 0x003DBE20
void (actorRecordMarkerSpot)(signed int, signed int, signed int, signed int); // 0x003DB8F0
void (actorRecordNavPointSpot)(signed int, signed int, signed int, signed int); // 0x003DB3B0
void (actorRecordObjectSpot)(signed int, signed int, signed int, signed int); // 0x003DADE0
void (actorSetPositionFloorHeight)(signed int, signed int, signed int); // 0x003E83A0
void (actorSetPosition)(signed int, signed int, signed int, signed int, signed int); // 0x003E8700
void (actorSetPositionMarker)(signed int, signed int, signed int); // 0x003E8DD0
void (actorSetPositionMarkerAbsolute)(signed int, signed int, signed int); // 0x003E8B50
void (actorSetPositionNavigationPoint)(signed int, signed int, signed int); // 0x003E8920
void (actorSetPositionActor)(signed int, signed int, signed int, signed int); // 0x003E94F0
void (actorChangeVFXAction)(signed int); // 0x003D81D0
void (actorChangeSFXAction)(signed int); // 0x003D81A0
void (actorSetPartVisible)(signed int, signed int, signed int, signed int, signed int); // 0x003D7E60
signed int (actorFlagsAdd)(signed int, signed int, signed int); // 0x003D8DE0
signed int (actorFlagsSub)(signed int, signed int, signed int); // 0x003D8B50
signed int (actorFlagsSet)(signed int, signed int, signed int); // 0x003D88F0
signed int (actorFlagsGet)(signed int, signed int); // 0x003D86A0
void (actorEffectFadeInfo)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x003EA9E0
void (actorEffectFadeStart)(signed int, signed int, signed int, signed int); // 0x003EABE0
signed int (actorEffectFadeDone)(signed int, signed int); // 0x003EADB0
void (actorEffectReflect)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x003EA7F0
void (actorEffectSetFlag)(signed int, signed int, signed int, signed int); // 0x003EA620
float g_PI_DIV_2; // 0x00485A50
float g_PI; // 0x00485A40
float g_2_PI; // 0x00485A48
float g_RADTODEG; // 0x00485A60
class tagxObjDBData *godb; // 0x004A3FAC
class xScene *xScene_current; // 0x004A4E08
float g_DEGTORAD; // 0x00485A58
class tagxActorDBData *gadb; // 0x004A4658
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x003D7E60 - 0x003D8198 (0x338) actorSetPartVisible__Fiiiii
void actorSetPartVisible(signed int actorType, signed int actorID, signed int partID, signed int makeVisible, signed int hideOtherParts) {
    { // 0x003D7E60 - 0x003D8198
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003D81A0 - 0x003D81C8 (0x28) actorChangeSFXAction__Fi
void actorChangeSFXAction() {
    { // 0x003D81A0 - 0x003D81C8
    }
}

// 0x003D81D0 - 0x003D81F8 (0x28) actorChangeVFXAction__Fi
void actorChangeVFXAction() {
    { // 0x003D81D0 - 0x003D81F8
    }
}

// 0x003D8200 - 0x003D8698 (0x498) xActorDebugGetActorType__FiPc
void xActorDebugGetActorType(signed int type, char *string) {
    { // 0x003D8200 - 0x003D8698
    }
}

// 0x003D86A0 - 0x003D88E4 (0x244) actorFlagsGet__Fii
signed int actorFlagsGet(signed int actorType, signed int actorID) {
    { // 0x003D86A0 - 0x003D88E4
        class _xActor *a; // r16
    }
}

// 0x003D88F0 - 0x003D8B44 (0x254) actorFlagsSet__Fiii
signed int actorFlagsSet(signed int actorType, signed int actorID, signed int flags) {
    { // 0x003D88F0 - 0x003D8B44
        class _xActor *a; // r16
    }
}

// 0x003D8B50 - 0x003D8DD4 (0x284) actorFlagsSub__Fiii
signed int actorFlagsSub(signed int actorType, signed int actorID, signed int flags) {
    { // 0x003D8B50 - 0x003D8DD4
        class _xActor *a; // r16
    }
}

// 0x003D8DE0 - 0x003D9054 (0x274) actorFlagsAdd__Fiii
signed int actorFlagsAdd(signed int actorType, signed int actorID, signed int flags) {
    { // 0x003D8DE0 - 0x003D9054
        class _xActor *a; // r16
    }
}

// 0x003D9060 - 0x003D9354 (0x2F4) actorSetCollisionFlags__Fiii
void actorSetCollisionFlags(signed int actorType, signed int actorID, signed int flags) {
    { // 0x003D9060 - 0x003D9354
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003D9360 - 0x003D9674 (0x314) actorFace__Fiii
void actorFace(signed int objectType, signed int actorID, signed int degrees) {
    { // 0x003D9360 - 0x003D9674
        class iActorRuntime *r; // r2
        class _xActor *a; // r16
    }
}

// 0x003D9680 - 0x003D999C (0x31C) actorTurn__Fiii
void actorTurn(signed int objectType, signed int actorID, signed int degDelta) {
    { // 0x003D9680 - 0x003D999C
        class iActorRuntime *r; // r2
        class _xActor *a; // r16
    }
}

// 0x003D99A0 - 0x003D9F60 (0x5C0) actorIsVisible__Fiiii
signed int actorIsVisible(signed int objectType, signed int actorID, signed int targetObjectType, signed int targetActorID) {
    { // 0x003D99A0 - 0x003D9F60
        class iActorRuntime *br; // r17
        class _xActor *b; // r17
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003D9F60 - 0x003DA3D8 (0x478) actorInFOV__Fiiii
signed int actorInFOV(signed int objectType, signed int actorID, signed int targetObjectType, signed int targetActorID) {
    { // 0x003D9F60 - 0x003DA3D8
        class _xActor *b; // r17
        class _xActor *a; // r16
    }
}

// 0x003DA3E0 - 0x003DA8DC (0x4FC) actorInTrigger__Fiii
signed int actorInTrigger(signed int objectType, signed int actorID, signed int triggerID) {
    { // 0x003DA3E0 - 0x003DA8DC
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003DA8E0 - 0x003DADDC (0x4FC) actorInEntryPoint__Fiii
signed int actorInEntryPoint(signed int objectType, signed int actorID, signed int entryPointID) {
    { // 0x003DA8E0 - 0x003DADDC
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003DADE0 - 0x003DB3A8 (0x5C8) actorRecordObjectSpot__Fiiii
void actorRecordObjectSpot(signed int actorType, signed int actorID, signed int objID, signed int memPosIndex) {
    { // 0x003DADE0 - 0x003DB3A8
        class _xObj *b; // r17
        class _xActor *a; // r16
    }
}

// 0x003DB3B0 - 0x003DB8E8 (0x538) actorRecordNavPointSpot__Fiiii
void actorRecordNavPointSpot(signed int actorType, signed int actorID, signed int navID, signed int memPosIndex) {
    { // 0x003DB3B0 - 0x003DB8E8
        class _xActor *a; // r16
    }
}

// 0x003DB8F0 - 0x003DBE1C (0x52C) actorRecordMarkerSpot__Fiiii
void actorRecordMarkerSpot(signed int actorType, signed int actorID, signed int markerID, signed int memPosIndex) {
    { // 0x003DB8F0 - 0x003DBE1C
        class _xActor *a; // r16
    }
}

// 0x003DBE20 - 0x003DC408 (0x5E8) actorRecordActorSpot__Fiiiii
void actorRecordActorSpot(signed int objectType, signed int actorID, signed int targetObjectType, signed int targetActorID, signed int memoryPosIndex) {
    { // 0x003DBE20 - 0x003DC408
        class _xActor *b; // r17
        class _xActor *a; // r16
    }
}

// 0x003DC410 - 0x003DC854 (0x444) actorGetNearestMarker__Fiiii
signed int actorGetNearestMarker(signed int actorType, signed int actorID, signed int minDistance, signed int maxDistance) {
    { // 0x003DC410 - 0x003DC854
        float max; // 
        float min; // 
        class xMarker *m; // r18
        float sqrMag; // r29+0xb0
        float nearestSqrMag; // 
        class Vector3D d; // r29+0xa0
        class Vector3D pos; // r29+0x90
        signed int nearest; // r17
        signed int i; // r16
        class _xActor *a; // r16
    }
}

// 0x003DC860 - 0x003DCCAC (0x44C) actorGetNearestNavPoint__Fiiii
signed int actorGetNearestNavPoint(signed int actorType, signed int actorID, signed int minDistance, signed int maxDistance) {
    { // 0x003DC860 - 0x003DCCAC
        float max; // 
        float min; // 
        class xNavPoint *n; // r18
        float sqrMag; // r29+0xb0
        float nearestSqrMag; // 
        class Vector3D d; // r29+0xa0
        class Vector3D pos; // r29+0x90
        signed int nearest; // r17
        signed int i; // r16
        class _xActor *a; // r16
    }
}

// 0x003DCCB0 - 0x003DCFF0 (0x340) actorGetPickupObject__Fiii
signed int actorGetPickupObject(signed int actorType, signed int actorID, signed int whichOne) {
    { // 0x003DCCB0 - 0x003DCFF0
        signed int distance; // r2
        class _xObj *obj; // r3
        class _xActor *a; // r18
        signed int count; // r17
        signed int i; // r16
    }
}

// 0x003DCFF0 - 0x003DD324 (0x334) actorGetRelativeMarkerAngle__Fiii
signed int actorGetRelativeMarkerAngle(signed int objectType, signed int actorID, signed int markerID) {
    { // 0x003DCFF0 - 0x003DD324
        float ang; // r29+0x70
        signed int anglem; // r2
        class Vector3D v; // r29+0x60
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003DD330 - 0x003DD874 (0x544) actorGetRelativeAngle__Fiiii
signed int actorGetRelativeAngle(signed int objectType, signed int actorID, signed int targetObjectType, signed int targetActorID) {
    { // 0x003DD330 - 0x003DD874
        float ang; // r29+0x80
        signed int anglem; // r2
        class Vector3D v; // r29+0x70
        class iActorRuntime *ar; // r17
        class _xActor *b; // r16
        class _xActor *a; // r16
    }
}

// 0x003DD880 - 0x003DDE74 (0x5F4) actorIsFacingObj__Fiiii
signed int actorIsFacingObj(signed int objectType, signed int actorID, signed int objID, signed int delta) {
    { // 0x003DD880 - 0x003DDE74
        class iActorRuntime *ar; // r17
        class _xObj *b; // r16
        class _xActor *a; // r16
    }
}

// 0x003DDE80 - 0x003DE498 (0x618) actorIsFacingActor__Fiiiii
signed int actorIsFacingActor(signed int objectType, signed int actorID, signed int targetObjectType, signed int targetActorID, signed int delta) {
    { // 0x003DDE80 - 0x003DE498
        class iActorRuntime *ar; // r17
        class _xActor *b; // r16
        class _xActor *a; // r16
    }
}

// 0x003DE4A0 - 0x003DE9F0 (0x550) actorIsFacingPosition__Fiiii
signed int actorIsFacingPosition(signed int actorType, signed int actorID, signed int posID, signed int delta) {
    { // 0x003DE4A0 - 0x003DE9F0
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003DE9F0 - 0x003DEF54 (0x564) actorIsFacingNavPoint__Fiiii
signed int actorIsFacingNavPoint(signed int actorType, signed int actorID, signed int navID, signed int delta) {
    { // 0x003DE9F0 - 0x003DEF54
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003DEF60 - 0x003DF4B4 (0x554) actorIsFacingMarker__Fiiii
signed int actorIsFacingMarker(signed int actorType, signed int actorID, signed int markerID, signed int delta) {
    { // 0x003DEF60 - 0x003DF4B4
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003DF4C0 - 0x003DFA70 (0x5B0) actorHaveLOSToNavPoint__Fiii
signed int actorHaveLOSToNavPoint(signed int actorType, signed int actorID, signed int navID) {
    { // 0x003DF4C0 - 0x003DFA70
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003DFA70 - 0x003DFF6C (0x4FC) actorHaveLOSToObject__Fiii
signed int actorHaveLOSToObject(signed int actorType, signed int actorID, signed int objID) {
    { // 0x003DFA70 - 0x003DFF6C
        class _xObj *b; // r17
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003DFF70 - 0x003E0518 (0x5A8) actorHaveLOSToSpot__Fiii
signed int actorHaveLOSToSpot(signed int actorType, signed int actorID, signed int spotID) {
    { // 0x003DFF70 - 0x003E0518
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E0520 - 0x003E0C24 (0x704) actorHaveLOSToActor__Fiiii
signed int actorHaveLOSToActor(signed int actorType, signed int actorID, signed int targetActorType, signed int targetActorID) {
    { // 0x003E0520 - 0x003E0C24
        class iActorRuntime *br; // r17
        class _xActor *b; // r17
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E0C30 - 0x003E11D0 (0x5A0) actorHaveLOSToMarker__Fiii
signed int actorHaveLOSToMarker(signed int actorType, signed int actorID, signed int markerID) {
    { // 0x003E0C30 - 0x003E11D0
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E11D0 - 0x003E16A0 (0x4D0) actorHaveLOSToDir__Fiiii
signed int actorHaveLOSToDir(signed int actorType, signed int actorID, signed int deltaX, signed int deltaZ) {
    { // 0x003E11D0 - 0x003E16A0
        class Vector3D rot; // r29+0x90
        class Vector3D d; // r29+0x80
        class Vector3D pos; // r29+0x70
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003E16A0 - 0x003E199C (0x2FC) actorGetAngle__Fii
signed int actorGetAngle(signed int objectType, signed int actorID) {
    { // 0x003E16A0 - 0x003E199C
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003E19A0 - 0x003E1C1C (0x27C) actorMoveVertical__Fiii
signed int actorMoveVertical(signed int actorType, signed int actorID, signed int amount) {
    { // 0x003E19A0 - 0x003E1C1C
        class iActorRuntime *r; // r16
        class _xActor *a; // r2
    }
}

// 0x003E1C20 - 0x003E21CC (0x5AC) actorMoveAwayFromActor__Fiiiii
void actorMoveAwayFromActor(signed int type, signed int id, signed int fromType, signed int fromID, signed int distance) {
    { // 0x003E1C20 - 0x003E21CC
        float ang; // r29+0xc0
        class Vector3D rot; // r29+0xb0
        class Vector3D m; // r29+0xa0
        class Vector3D dm; // r29+0x90
        class Vector3D d; // r29+0x80
        class iActorRuntime *br; // r17
        class _xActor *b; // r2
        class iActorRuntime *ar; // r16
        class _xActor *a; // r2
    }
}

// 0x003E21D0 - 0x003E24E8 (0x318) actorMoveForward__Fiii
signed int actorMoveForward(signed int objectType, signed int actorID, signed int amount) {
    { // 0x003E21D0 - 0x003E24E8
        class Vector3D rot; // r29+0x70
        class Vector3D d; // r29+0x60
        class iActorRuntime *r; // r16
        class _xActor *a; // r2
    }
}

// 0x003E24F0 - 0x003E2924 (0x434) actorMoveStrafe__Fiii
signed int actorMoveStrafe(signed int objectType, signed int actorID, signed int amount) {
    { // 0x003E24F0 - 0x003E2924
        class Vector3D rot; // r29+0x70
        class Vector3D d; // r29+0x60
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003E2930 - 0x003E2DD0 (0x4A0) actorFaceActor__Fiiii
void actorFaceActor(signed int actorType, signed int actorID, signed int targetObjectType, signed int targetActorID) {
    { // 0x003E2930 - 0x003E2DD0
        class Vector3D d; // r29+0x70
        class iActorRuntime *br; // r18
        class _xActor *b; // r2
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003E2DD0 - 0x003E31A8 (0x3D8) actorFacePosition__Fiii
void actorFacePosition(signed int actorType, signed int actorID, signed int posID) {
    { // 0x003E2DD0 - 0x003E31A8
        class Vector3D d; // r29+0x60
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003E31B0 - 0x003E3630 (0x480) actorFaceObject__Fiii
void actorFaceObject(signed int actorType, signed int actorID, signed int objID) {
    { // 0x003E31B0 - 0x003E3630
        class _xObj *b; // r18
        class Vector3D d; // r29+0x60
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003E3630 - 0x003E38D0 (0x2A0) actorFaceNavPoint__Fiii
void actorFaceNavPoint(signed int actorType, signed int actorID, signed int navID) {
    { // 0x003E3630 - 0x003E38D0
        class Vector3D d; // r29+0x60
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003E38D0 - 0x003E3B60 (0x290) actorFaceMarker__Fiii
void actorFaceMarker(signed int actorType, signed int actorID, signed int markerID) {
    { // 0x003E38D0 - 0x003E3B60
        class Vector3D d; // r29+0x60
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003E3B60 - 0x003E3FF8 (0x498) actorDistanceToNavPoint3D__Fiii
signed int actorDistanceToNavPoint3D(signed int actorType, signed int actorID, signed int navID) {
    { // 0x003E3B60 - 0x003E3FF8
        signed int mag; // r2
        class Vector3D d; // r29+0x60
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E4000 - 0x003E44D8 (0x4D8) actorDistanceToNavPoint__Fiii
signed int actorDistanceToNavPoint(signed int actorType, signed int actorID, signed int navID) {
    { // 0x003E4000 - 0x003E44D8
        signed int mag; // r2
        class _RwV2d vb; // r29+0x78
        class _RwV2d va; // r29+0x70
        class _RwV2d d; // r29+0x68
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E44E0 - 0x003E4968 (0x488) actorDistanceToMarker3D__Fiii
signed int actorDistanceToMarker3D(signed int actorType, signed int actorID, signed int markerID) {
    { // 0x003E44E0 - 0x003E4968
        signed int mag; // r2
        class Vector3D d; // r29+0x60
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E4970 - 0x003E4E30 (0x4C0) actorDistanceToMarker__Fiii
signed int actorDistanceToMarker(signed int actorType, signed int actorID, signed int markerID) {
    { // 0x003E4970 - 0x003E4E30
        signed int mag; // r2
        class _RwV2d vb; // r29+0x78
        class _RwV2d va; // r29+0x70
        class _RwV2d d; // r29+0x68
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E4E30 - 0x003E516C (0x33C) actorDistanceToRecordedPosition3D__Fiii
signed int actorDistanceToRecordedPosition3D(signed int objectType, signed int actorID, signed int recordedIndex) {
    { // 0x003E4E30 - 0x003E516C
        signed int mag; // r2
        class Vector3D d; // r29+0x60
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003E5170 - 0x003E54E0 (0x370) actorDistanceToRecordedPosition__Fiii
signed int actorDistanceToRecordedPosition(signed int objectType, signed int actorID, signed int recordedIndex) {
    { // 0x003E5170 - 0x003E54E0
        signed int mag; // r2
        class _RwV2d vb; // r29+0x78
        class _RwV2d va; // r29+0x70
        class _RwV2d d; // r29+0x68
        class iActorRuntime *ar; // r17
        class _xActor *a; // r16
    }
}

// 0x003E54E0 - 0x003E5874 (0x394) actorDistanceBetweenSpots__Fiiii
signed int actorDistanceBetweenSpots(signed int actorType, signed int actorID, signed int spot1, signed int spot2) {
    { // 0x003E54E0 - 0x003E5874
        signed int mag; // r2
        class _RwV2d vb; // r29+0x88
        class _RwV2d va; // r29+0x80
        class _RwV2d d; // r29+0x78
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E5880 - 0x003E5BD4 (0x354) actorDistanceBetweenSpots3D__Fiiii
signed int actorDistanceBetweenSpots3D(signed int actorType, signed int actorID, signed int spot1, signed int spot2) {
    { // 0x003E5880 - 0x003E5BD4
        signed int mag; // r2
        class Vector3D d; // r29+0x70
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E5BE0 - 0x003E61A4 (0x5C4) actorDistanceToObj3D__Fiii
signed int actorDistanceToObj3D(signed int objectType, signed int actorID, signed int objID) {
    { // 0x003E5BE0 - 0x003E61A4
        signed int mag; // r2
        class Vector3D d; // r29+0x60
        class iObjRuntime *br; // r17
        class _xObj *b; // r17
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E61B0 - 0x003E6794 (0x5E4) actorDistanceToObj__Fiii
signed int actorDistanceToObj(signed int objectType, signed int actorID, signed int objID) {
    { // 0x003E61B0 - 0x003E6794
        signed int mag; // r2
        class _RwV2d vb; // r29+0x78
        class _RwV2d va; // r29+0x70
        class _RwV2d d; // r29+0x68
        class iObjRuntime *br; // r17
        class _xObj *b; // r17
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E67A0 - 0x003E6DC4 (0x624) actorDistanceToActor3D__Fiiii
signed int actorDistanceToActor3D(signed int objectType, signed int actorID, signed int targetObjectType, signed int targetActorID) {
    { // 0x003E67A0 - 0x003E6DC4
        float mag; // r29+0x80
        class Vector3D d; // r29+0x70
        class iActorRuntime *br; // r17
        class _xActor *b; // r17
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E6DD0 - 0x003E7414 (0x644) actorDistanceToActor__Fiiii
signed int actorDistanceToActor(signed int objectType, signed int actorID, signed int targetObjectType, signed int targetActorID) {
    { // 0x003E6DD0 - 0x003E7414
        float mag; // r29+0x90
        class _RwV2d vb; // r29+0x88
        class _RwV2d va; // r29+0x80
        class _RwV2d d; // r29+0x78
        class iActorRuntime *br; // r17
        class _xActor *b; // r17
        class iActorRuntime *ar; // r16
        class _xActor *a; // r16
    }
}

// 0x003E7420 - 0x003E7440 (0x20) actorDestroy__Fii
void actorDestroy(signed int objectType, signed int actorID) {
    { // 0x003E7420 - 0x003E7440
    }
}

// 0x003E7440 - 0x003E7780 (0x340) actorSetTexture__Fiii
void actorSetTexture(signed int actorType, signed int actorID, signed int textureID) {
    { // 0x003E7440 - 0x003E7780
        class _xTex *t; // r2
        class iActorRuntime *r; // r2
        class _xActor *a; // r16
    }
}

// 0x003E7780 - 0x003E7880 (0x100) actorCreate__Fiiii
signed int actorCreate(signed int objectType, signed int modelID, signed int skeletonID, signed int textureID) {
    { // 0x003E7780 - 0x003E7880
    }
}

// 0x003E7880 - 0x003E7C30 (0x3B0) actorSetBRadius__Fiii
void actorSetBRadius(signed int objectType, signed int actorID, signed int bradius) {
    { // 0x003E7880 - 0x003E7C30
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003E7C30 - 0x003E7FE4 (0x3B4) actorSetAttrSight__Fiii
void actorSetAttrSight(signed int objectType, signed int actorID, signed int sight) {
    { // 0x003E7C30 - 0x003E7FE4
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003E7FF0 - 0x003E83A0 (0x3B0) actorSetAttrFOV__Fiii
void actorSetAttrFOV(signed int objectType, signed int actorID, signed int fov) {
    { // 0x003E7FF0 - 0x003E83A0
        class iActorRuntime *r; // r16
        class _xActor *a; // r16
    }
}

// 0x003E83A0 - 0x003E8700 (0x360) actorSetPositionFloorHeight__Fiii
void actorSetPositionFloorHeight(signed int actorType, signed int actorID, signed int y) {
    { // 0x003E83A0 - 0x003E8700
        class tagxPhysicsObject *p; // r2
    }
}

// 0x003E8700 - 0x003E8920 (0x220) actorSetPosition__Fiiiii
void actorSetPosition(signed int objectType, signed int actorID, signed int x, signed int y, signed int z) {
    { // 0x003E8700 - 0x003E8920
    }
}

// 0x003E8920 - 0x003E8B50 (0x230) actorSetPositionNavigationPoint__Fiii
void actorSetPositionNavigationPoint(signed int objectType, signed int actorID, signed int navID) {
    { // 0x003E8920 - 0x003E8B50
    }
}

// 0x003E8B50 - 0x003E8DC8 (0x278) actorSetPositionMarkerAbsolute__Fiii
void actorSetPositionMarkerAbsolute(signed int objectType, signed int actorID, signed int markerID) {
    { // 0x003E8B50 - 0x003E8DC8
        class xMarker *marker; // r2
    }
}

// 0x003E8DD0 - 0x003E9074 (0x2A4) actorSetPositionMarker__Fiii
void actorSetPositionMarker(signed int objectType, signed int actorID, signed int markerID) {
    { // 0x003E8DD0 - 0x003E9074
        class iActorRuntime *r; // r2
        class xMarker *marker; // r5
    }
}

// 0x003E9080 - 0x003E9268 (0x1E8) actorRoutePosition__Fii
signed int actorRoutePosition(signed int objectType, signed int actorID) {
    { // 0x003E9080 - 0x003E9268
    }
}

// 0x003E9270 - 0x003E94E4 (0x274) actorRouteStatus__Fii
signed int actorRouteStatus(signed int actorType, signed int actorID) {
    { // 0x003E9270 - 0x003E94E4
        class xRoute *route; // r16
        class iActorRuntime *r; // r4
    }
}

// 0x003E94F0 - 0x003E97D0 (0x2E0) actorSetPositionActor__Fiiii
void actorSetPositionActor(signed int objectType, signed int actorID, signed int toObjectType, signed int toActorID) {
    { // 0x003E94F0 - 0x003E97D0
    }
}

// 0x003E97D0 - 0x003E9A14 (0x244) actorGetFlags__Fii
signed int actorGetFlags(signed int type, signed int id) {
    { // 0x003E97D0 - 0x003E9A14
    }
}

// 0x003E9A20 - 0x003E9C7C (0x25C) actorGetRotY__Fii
signed int actorGetRotY(signed int type, signed int id) {
    { // 0x003E9A20 - 0x003E9C7C
    }
}

// 0x003E9C80 - 0x003E9EDC (0x25C) actorGetPositionZ__Fii
signed int actorGetPositionZ(signed int type, signed int id) {
    { // 0x003E9C80 - 0x003E9EDC
    }
}

// 0x003E9EE0 - 0x003EA13C (0x25C) actorGetPositionY__Fii
signed int actorGetPositionY(signed int type, signed int id) {
    { // 0x003E9EE0 - 0x003EA13C
    }
}

// 0x003EA140 - 0x003EA39C (0x25C) actorGetPositionX__Fii
signed int actorGetPositionX(signed int type, signed int id) {
    { // 0x003EA140 - 0x003EA39C
    }
}

// 0x003EA3A0 - 0x003EA618 (0x278) actorSetVisible__Fiii
void actorSetVisible(signed int type, signed int id, signed int visible) {
    { // 0x003EA3A0 - 0x003EA618
    }
}

// 0x003EA620 - 0x003EA7F0 (0x1D0) actorEffectSetFlag__Fiiii
void actorEffectSetFlag(signed int actorType, signed int actorID, signed int flag, signed int mode) {
    { // 0x003EA620 - 0x003EA7F0
    }
}

// 0x003EA7F0 - 0x003EA9E0 (0x1F0) actorEffectReflect__Fiiiiiii
void actorEffectReflect(signed int actorType, signed int actorID, signed int r, signed int g, signed int b, signed int mode) {
    { // 0x003EA7F0 - 0x003EA9E0
    }
}

// 0x003EA9E0 - 0x003EABE0 (0x200) actorEffectFadeInfo__Fiiiiiii
void actorEffectFadeInfo(signed int actorType, signed int actorID, signed int r, signed int g, signed int b, signed int mode, signed int idx) {
    { // 0x003EA9E0 - 0x003EABE0
    }
}

// 0x003EABE0 - 0x003EADB0 (0x1D0) actorEffectFadeStart__Fiiii
void actorEffectFadeStart(signed int actorType, signed int actorID, signed int flag, signed int time) {
    { // 0x003EABE0 - 0x003EADB0
    }
}

// 0x003EADB0 - 0x003EAF60 (0x1B0) actorEffectFadeDone__Fii
signed int actorEffectFadeDone(signed int actorType, signed int actorID) {
    { // 0x003EADB0 - 0x003EAF60
    }
}

// 0x003EAF60 - 0x003EB8E0 (0x980) xActorDBScriptRegister__Fv
void xActorDBScriptRegister() {
    { // 0x003EAF60 - 0x003EB8E0
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xSndMgrScript.cpp
// ==========================================================================
signed int (xSndMgrScriptActorPlaySound)(signed int, signed int, signed int, signed int, signed int); // 0x003ECEE0
signed int (xSndMgrIsSoundDone)(signed int); // 0x003ECCF0
signed int (xSndMgrIsSoundIDDone)(signed int); // 0x003ECAB0
signed int (xSndMgrScriptPlaySoundVol)(signed int, signed int); // 0x003ECA90
signed int (xSndMgrScriptPlaySoundVolLoop)(signed int, signed int, signed int); // 0x003ECA70
void (xSndMgrSetVoiceFlags)(signed int, unsigned int); // 0x003EC8D0
void (xSndMgrSubVoiceFlags)(signed int, unsigned int); // 0x003EC700
void (xSndMgrAddVoiceFlags)(signed int, unsigned int); // 0x003EC550
void (xSndMgrScriptSetListenerRadius)(signed int); // 0x003EC500
void (xSndMgrScriptSetVoiceRadius)(signed int, signed int); // 0x003EC350
void (xSndMgrScriptSetVoicePosition)(signed int, signed int, signed int, signed int); // 0x003EC150
void (xSndMgrScriptSetVoicePositionMarker)(signed int, signed int); // 0x003EBE30
void (xSndMgrScriptStopSound)(signed int); // 0x003EBE10
signed int (xSndMgrScriptGetSNDID)(signed int); // 0x003EBDF0
void (xSndMgrScriptVoiceSetFrequency)(signed int, signed int); // 0x003EBC40
void (xSndMgrScriptVoiceSetVolume)(signed int, signed int); // 0x003EBAD0
void (xSndMgrScriptSetMasterVolume)(signed int); // 0x003EB940
signed int (xSndMgrScriptGetMasterVolume)(); // 0x003EB930
signed int (xSndMgrScriptGetStereo)(); // 0x003EB920
void (xSndMgrScriptSetStereo)(signed int); // 0x003EB8E0
class tagxActorDBData *gadb; // 0x004A4658
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _xSndMgrData gSndMgr; // 0x004B1F00
// 0x003EB8E0 - 0x003EB920 (0x40) xSndMgrScriptSetStereo__Fi
void xSndMgrScriptSetStereo(signed int stereoOn) {
    { // 0x003EB8E0 - 0x003EB920
    }
}

// 0x003EB920 - 0x003EB930 (0x10) xSndMgrScriptGetStereo__Fv
signed int xSndMgrScriptGetStereo() {
    { // 0x003EB920 - 0x003EB930
    }
}

// 0x003EB930 - 0x003EB940 (0x10) xSndMgrScriptGetMasterVolume__Fv
signed int xSndMgrScriptGetMasterVolume() {
    { // 0x003EB930 - 0x003EB940
    }
}

// 0x003EB940 - 0x003EBACC (0x18C) xSndMgrScriptSetMasterVolume__Fi
void xSndMgrScriptSetMasterVolume(signed int volume) {
    { // 0x003EB940 - 0x003EBACC
    }
}

// 0x003EBAD0 - 0x003EBC38 (0x168) xSndMgrScriptVoiceSetVolume__Fii
void xSndMgrScriptVoiceSetVolume(signed int voice) {
    { // 0x003EBAD0 - 0x003EBC38
    }
}

// 0x003EBC40 - 0x003EBDEC (0x1AC) xSndMgrScriptVoiceSetFrequency__Fii
void xSndMgrScriptVoiceSetFrequency(signed int voice, signed int frequency) {
    { // 0x003EBC40 - 0x003EBDEC
    }
}

// 0x003EBDF0 - 0x003EBE10 (0x20) xSndMgrScriptGetSNDID__Fi
signed int xSndMgrScriptGetSNDID(signed int assetID) {
    { // 0x003EBDF0 - 0x003EBE10
    }
}

// 0x003EBE10 - 0x003EBE30 (0x20) xSndMgrScriptStopSound__Fi
void xSndMgrScriptStopSound(signed int soundVoice) {
    { // 0x003EBE10 - 0x003EBE30
    }
}

// 0x003EBE30 - 0x003EC14C (0x31C) xSndMgrScriptSetVoicePositionMarker__Fii
void xSndMgrScriptSetVoicePositionMarker(signed int voice, signed int markerID) {
    { // 0x003EBE30 - 0x003EC14C
        class iMarkerResource *r; // r16
    }
}

// 0x003EC150 - 0x003EC344 (0x1F4) xSndMgrScriptSetVoicePosition__Fiiii
void xSndMgrScriptSetVoicePosition(signed int voice, signed int x, signed int y, signed int z) {
    { // 0x003EC150 - 0x003EC344
    }
}

// 0x003EC350 - 0x003EC4F4 (0x1A4) xSndMgrScriptSetVoiceRadius__Fii
void xSndMgrScriptSetVoiceRadius(signed int voice, signed int r) {
    { // 0x003EC350 - 0x003EC4F4
    }
}

// 0x003EC500 - 0x003EC548 (0x48) xSndMgrScriptSetListenerRadius__Fi
void xSndMgrScriptSetListenerRadius(signed int radius) {
    { // 0x003EC500 - 0x003EC548
    }
}

// 0x003EC550 - 0x003EC6F4 (0x1A4) xSndMgrAddVoiceFlags__FiUi
void xSndMgrAddVoiceFlags(signed int voice, unsigned int flags) {
    { // 0x003EC550 - 0x003EC6F4
    }
}

// 0x003EC700 - 0x003EC8CC (0x1CC) xSndMgrSubVoiceFlags__FiUi
void xSndMgrSubVoiceFlags(signed int voice, unsigned int flags) {
    { // 0x003EC700 - 0x003EC8CC
    }
}

// 0x003EC8D0 - 0x003ECA6C (0x19C) xSndMgrSetVoiceFlags__FiUi
void xSndMgrSetVoiceFlags(signed int voice, unsigned int flags) {
    { // 0x003EC8D0 - 0x003ECA6C
    }
}

// 0x003ECA70 - 0x003ECA90 (0x20) xSndMgrScriptPlaySoundVolLoop__Fiii
signed int xSndMgrScriptPlaySoundVolLoop(signed int soundID, signed int soundVol, signed int loopCount) {
    { // 0x003ECA70 - 0x003ECA90
    }
}

// 0x003ECA90 - 0x003ECAB0 (0x20) xSndMgrScriptPlaySoundVol__Fii
signed int xSndMgrScriptPlaySoundVol(signed int soundID, signed int soundVol) {
    { // 0x003ECA90 - 0x003ECAB0
    }
}

// 0x003ECAB0 - 0x003ECCEC (0x23C) xSndMgrIsSoundIDDone__Fi
signed int xSndMgrIsSoundIDDone(signed int soundID) {
    { // 0x003ECAB0 - 0x003ECCEC
        signed int i; // r2
    }
}

// 0x003ECCF0 - 0x003ECED4 (0x1E4) xSndMgrIsSoundDone__Fi
signed int xSndMgrIsSoundDone(signed int soundVoice) {
    { // 0x003ECCF0 - 0x003ECED4
        class _iSFX *sp; // r3
    }
}

// 0x003ECEE0 - 0x003ED150 (0x270) xSndMgrScriptActorPlaySound__Fiiiii
signed int xSndMgrScriptActorPlaySound(signed int actorType, signed int actorID, signed int soundID, signed int soundVol, signed int soundFreq) {
    { // 0x003ECEE0 - 0x003ED150
        class iActorRuntime *ar; // r16
        class _xActor *a; // r2
    }
}

// 0x003ED150 - 0x003ED398 (0x248) xSndMgrRegisterExternals__Fv
void xSndMgrRegisterExternals() {
    { // 0x003ED150 - 0x003ED398
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zModuleFactory.cpp
// ==========================================================================
struct @anon0 __vt__7xModule; // 0x004A2790
struct @anon1 __vt__5zMain; // 0x004A27C0
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x003ED3A0 - 0x003ED748 (0x3A8) create__14zModuleFactoryFi
class xModule *create(signed int type) {
    { // 0x003ED3A0 - 0x003ED748
        class xModule *m; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zDemo.cpp
// ==========================================================================
class xGameObjMgr *gGameObjMgr; // 0x004A5070
class xAnimMgr *gAnimMgr; // 0x004A5060
class Vector3D *gBSPOffset; // 0x004A4FA4
class xScene *xScene_current; // 0x004A4E08
class xColor line_color; // 0x004A4F88
struct @anon1 __vt__7xModule; // 0x004A2790
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class RWInfo gRWInfo; // 0x00B02B80
struct @anon0 __vt__5zDemo; // 0x004A27F0
unsigned int bad_guy_idle_ID; // 0x004A5044
unsigned int bad_guy_anim_ID; // 0x004A5040
class zCharacter *bad_guy; // 0x004A503C
unsigned int anim_ID_grass1; // 0x004A5030
unsigned int anim_ID_snack1; // 0x004A502C
unsigned int anim_ID_walk_back; // 0x004A5028
unsigned int anim_ID_idle_scared; // 0x004A5024
unsigned int anim_ID_extra_scratch; // 0x004A5020
unsigned int anim_ID_walk; // 0x004A501C
unsigned int anim_ID_run; // 0x004A5018
unsigned int anim_ID_ouch; // 0x004A5014
unsigned int anim_ID_jump; // 0x004A5010
unsigned int anim_ID_idle; // 0x004A500C
class zCharacter *gPlayer; // 0x004A5008
class zCharacter *gVillain; // 0x004A5004
static class zDemo *m_globalDemo; // 0x004A5000
static char dbfont_chars[51]; // 0x00000000
// 0x003ED750 - 0x003ED778 (0x28) PadButtonCircle__Fv
void PadButtonCircle() {
    { // 0x003ED750 - 0x003ED778
    }
}

// 0x003ED780 - 0x003ED7A4 (0x24) PadButtonCross__Fv
void PadButtonCross() {
    { // 0x003ED780 - 0x003ED7A4
    }
}

// 0x003ED7B0 - 0x003ED894 (0xE4) zDemoQuickReset__Fv
void zDemoQuickReset() {
    { // 0x003ED7B0 - 0x003ED894
    }
}

// 0x003ED8A0 - 0x003ED980 (0xE0) zDemoPlayerLostLife__Fv
void zDemoPlayerLostLife() {
    { // 0x003ED8A0 - 0x003ED980
    }
}

// 0x003ED980 - 0x003ED9C8 (0x48) PadRightStick__Fff
void PadRightStick(float x, float y) {
    { // 0x003ED980 - 0x003ED9C8
    }
}

// 0x003ED9D0 - 0x003EDA10 (0x40) PadLeftStick__Fff
void PadLeftStick(float x, float y) {
    { // 0x003ED9D0 - 0x003EDA10
    }
}

// 0x003EDA10 - 0x003EDA40 (0x30) __ct__5zDemoFv
void *__ct(class zDemo *this) {
    { // 0x003EDA10 - 0x003EDA40
    }
}

// 0x003EDA40 - 0x003EDAF8 (0xB8) render__5zDemoFv
void render(class zDemo *this) {
    { // 0x003EDA40 - 0x003EDAF8
        float scale; // r29+0x30
    }
}

// 0x003EDB00 - 0x003EDD80 (0x280) SnackCollisionTest__5zDemoFv
void SnackCollisionTest(class zDemo *this) {
    { // 0x003EDB00 - 0x003EDD80
        class _RwV3d *pos; // r2
        class _RwMatrix *m; // r20
        class _RwFrame *f; // r19
        class _RwV3d *pos; // r2
        class _RwMatrix *m; // r20
        class _RwFrame *f; // r19
        signed int i; // r17
        class _RwV3d *snack_pos; // r2
        float dist; // 
        float z; // 
        float y; // 
        float x; // 
        class _RwV3d *ch; // r16
    }
}

// 0x003EDD80 - 0x003EDE78 (0xF8) nextFrame__5zDemoFv
void nextFrame(class zDemo *this) {
    { // 0x003EDD80 - 0x003EDE78
    }
}

// 0x003EDE80 - 0x003EDF8C (0x10C) restart__5zDemoFv
void restart(class zDemo *this) {
    { // 0x003EDE80 - 0x003EDF8C
        class _RwV3d *pos; // r2
        class _RwMatrix *m; // r19
        class _RwFrame *f; // r18
        signed int i; // r16
    }
}

// 0x003EDF90 - 0x003EDF98 (0x8) kill__5zDemoFv
void kill() {
    { // 0x003EDF90 - 0x003EDF98
    }
}

// 0x003EDFA0 - 0x003EE838 (0x898) init__5zDemoFv
void init(class zDemo *this) {
    { // 0x003EDFA0 - 0x003EE838
        signed int i; // r16
        signed int i; // r16
        float grass_pos[123]; // r29+0x80
        class Vector3D bad_pos; // r29+0x270
        static signed int healthIndex; // 0x004A5034
        static signed char init; // 0x004A5038
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zCharacter.cpp
// ==========================================================================
class _RpAnimSequence * (_clumpAddSequence)(class _RpAnimSequence *, void *); // 0x00000000
class _RwFrame * (_animClumpForAllFramesAddSequencesCallBack)(class _RwFrame *, void *); // 0x00000000
class _rpAtomic * (zSkinSetupAtomicCallback)(class _rpAtomic *, void *); // 0x003F0BB0
class zCharacter *bad_guy; // 0x004A503C
float deltaFrame; // 0x004A4C88
class _rpAtomic * (hackUpdateSkinAtomic)(class _rpAtomic *, void *); // 0x003F0340
float g_RADTODEG; // 0x00485A60
class Vector3D *gBSPOffset; // 0x004A4FA4
class xScene *xScene_current; // 0x004A4E08
unsigned int anim_ID_ouch; // 0x004A5014
unsigned int anim_ID_extra_scratch; // 0x004A5020
unsigned int anim_ID_idle; // 0x004A500C
unsigned int anim_ID_idle_scared; // 0x004A5024
unsigned int anim_ID_jump; // 0x004A5010
float g_DEGTORAD; // 0x00485A58
unsigned int anim_ID_run; // 0x004A5018
unsigned int anim_ID_walk_back; // 0x004A5028
unsigned int anim_ID_walk; // 0x004A501C
class xAnimMgr *gAnimMgr; // 0x004A5060
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _rpAtomic * (zSkinAnimSetupAtomicCallback)(class _rpAtomic *, void *); // 0x003F0870
struct @anon0 __vt__10zCharacter; // 0x004A2820
static class _SkinData zSkinData; // 0x004502E0
static unsigned char was_active_last_frame; // 0x004A504C
static signed int cur_point; // 0x004A5048
// 0x003EE840 - 0x003EE84C (0xC) SetPlayerFlag__10zCharacterFi
void SetPlayerFlag(class zCharacter *this, signed int val) {
    { // 0x003EE840 - 0x003EE84C
    }
}

// 0x003EE850 - 0x003EE884 (0x34) getHurt__10zCharacterFv
void getHurt(class zCharacter *this) {
    { // 0x003EE850 - 0x003EE884
    }
}

// 0x003EE890 - 0x003EF004 (0x774) MoveForward__10zCharacterFf
void MoveForward(class zCharacter *this, float delta) {
    { // 0x003EE890 - 0x003EF004
        signed int newState; // r16
    }
}

// 0x003EF010 - 0x003EF4A0 (0x490) Turn__10zCharacterFf
void Turn(class zCharacter *this, float delta) {
    { // 0x003EF010 - 0x003EF4A0
        signed int newState; // r7
    }
}

// 0x003EF4A0 - 0x003EF738 (0x298) Jump__10zCharacterFv
void Jump(class zCharacter *this) {
    { // 0x003EF4A0 - 0x003EF738
        signed int newState; // r16
    }
}

// 0x003EF740 - 0x003EF748 (0x8) render__10zCharacterFv
void render() {
    { // 0x003EF740 - 0x003EF748
    }
}

// 0x003EF750 - 0x003EF8EC (0x19C) setState__10zCharacterFi
void setState(class zCharacter *this, signed int state) {
    { // 0x003EF750 - 0x003EF8EC
        static signed int idleCountDown; // 0x004A5058
        static signed char init; // 0x004A505C
    }
}

// 0x003EF8F0 - 0x003F016C (0x87C) nextFrame__10zCharacterFv
void nextFrame(class zCharacter *this) {
    { // 0x003EF8F0 - 0x003F016C
        unsigned char mats[10]; // r29+0x138
        signed int total; // r2
        class Vector3D no_bsp_pos; // r29+0x128
        class _RwMatrix *m; // r20
        class _RwFrame *f; // r19
        class _RwV3d up; // r29+0x118
        class _RwV3d *rw_local_z; // r2
        class _RwMatrix *matrix; // r2
        class Vector3D motion_2d; // r29+0x108
        class tagCollideInfo collision; // r29+0x80
        class Vector3D nn; // r29+0xf8
        class Vector3D n; // r29+0xe8
        unsigned char res; // r2
        unsigned char mater[4]; // r29+0x14c
        class Vector3D ppp; // r29+0xd8
        float delta; // r29+0x148
        class Vector3D *orig; // r17
        class Vector3D *pos; // r16
        class _RwV3d p; // r29+0xc8
        static signed char init; // 0x004A5054
        static float scared_anim_time; // 0x004A5050
    }
}

// 0x003F0170 - 0x003F01C0 (0x50) StoreCurrentHeight__10zCharacterFv
void StoreCurrentHeight(class zCharacter *this) {
    { // 0x003F0170 - 0x003F01C0
        class _RwV3d *pos; // r2
        class _RwMatrix *m; // r2
        class _RwFrame *f; // r2
    }
}

// 0x003F01C0 - 0x003F0334 (0x174) calcFloorHeight__10zCharacterFv
void calcFloorHeight(class zCharacter *this) {
    { // 0x003F01C0 - 0x003F0334
        class tagCollideInfo stab_collision; // r29+0x30
        class Vector3D *pos; // r16
    }
}

// 0x003F0340 - 0x003F03A0 (0x60) hackUpdateSkinAtomic__FP9_rpAtomicPv
static class _rpAtomic *hackUpdateSkinAtomic(class _rpAtomic *atomic, void *data) {
    { // 0x003F0340 - 0x003F03A0
        class _RpSkin *skin; // r2
        float delta; // 
    }
}

// 0x003F03A0 - 0x003F0464 (0xC4) restart__10zCharacterFv
void restart(class zCharacter *this) {
    { // 0x003F03A0 - 0x003F0464
    }
}

// 0x003F0470 - 0x003F0478 (0x8) kill__10zCharacterFv
void kill() {
    { // 0x003F0470 - 0x003F0478
    }
}

// 0x003F0480 - 0x003F0530 (0xB0) init__10zCharacterFv
void init() {
    { // 0x003F0480 - 0x003F0530
    }
}

// 0x003F0530 - 0x003F0754 (0x224) init__10zCharacterFPc
void init(class zCharacter *this, char *filename) {
    { // 0x003F0530 - 0x003F0754
    }
}

// 0x003F0760 - 0x003F0864 (0x104) SetCurrentAnim__10zCharacterFP9xAnimDataUi
void SetCurrentAnim(class zCharacter *this, class xAnimData *pAnimData, unsigned int aid) {
    { // 0x003F0760 - 0x003F0864
    }
}

// 0x003F0870 - 0x003F08DC (0x6C) zSkinAnimSetupAtomicCallback__FP9_rpAtomicPv
static class _rpAtomic *zSkinAnimSetupAtomicCallback(class _rpAtomic *atomic, void *data) {
    { // 0x003F0870 - 0x003F08DC
        class _RpSkinAnim *ska; // r2
        class _RpSkin *sk; // r2
    }
}

// 0x003F08E0 - 0x003F094C (0x6C) LoadHackModel__FPc
class _RpClump *LoadHackModel(char *filename) {
    { // 0x003F08E0 - 0x003F094C
        class _RpClump *clump; // r16
    }
}

// 0x003F0950 - 0x003F0BAC (0x25C) DffLoad__FPc
class _RpClump *DffLoad(char *filename) {
    { // 0x003F0950 - 0x003F0BAC
        signed int res; // r2
        class _RpClump *clump; // r17
        class _RwStream *stream; // r16
    }
}

// 0x003F0BB0 - 0x003F0C14 (0x64) zSkinSetupAtomicCallback__FP9_rpAtomicPv
class _rpAtomic *zSkinSetupAtomicCallback(class _rpAtomic *atomic) {
    { // 0x003F0BB0 - 0x003F0C14
        class _RpSkin *skin; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xRouteDBScript.cpp
// ==========================================================================
signed int (xScriptRouteDBRouteCreate)(); // 0x003F0DC0
void (xScriptRouteDBRouteDraw)(signed int); // 0x003F0DA0
void (xScriptRouteDBRouteDestroy)(signed int); // 0x003F0D80
signed int (xScriptRouteDBRouteStatus)(signed int); // 0x003F0D60
void (xScriptRouteDBRouteAppend)(signed int, signed int); // 0x003F0D40
signed int (xScriptRouteDBRouteToActor)(signed int, signed int, signed int, signed int); // 0x003F0D20
signed int (xScriptRouteDBRouteToPlayer)(signed int, signed int); // 0x003F0D00
signed int (xScriptRouteDBRouteToMarker)(signed int, signed int, signed int); // 0x003F0CE0
signed int (xScriptRouteDBRouteMarkerToMarker)(signed int, signed int); // 0x003F0CC0
signed int (xScriptRouteDBRouteToRTObj)(signed int, signed int, signed int); // 0x003F0CA0
signed int (xScriptRouteDBRouteToNavigationPoint)(signed int, signed int, signed int); // 0x003F0C80
signed int (xScriptRouteDBRouteNavToNav)(signed int, signed int); // 0x003F0C60
signed int (xScriptRouteDBRouteToMemoryPosition)(signed int, signed int, signed int); // 0x003F0C40
void (xScriptRouteDBSetRouteUseage)(signed int, signed int); // 0x003F0C20
// 0x003F0C20 - 0x003F0C40 (0x20) xScriptRouteDBSetRouteUseage__Fii
void xScriptRouteDBSetRouteUseage(signed int id, signed int useageType) {
    { // 0x003F0C20 - 0x003F0C40
    }
}

// 0x003F0C40 - 0x003F0C60 (0x20) xScriptRouteDBRouteToMemoryPosition__Fiii
signed int xScriptRouteDBRouteToMemoryPosition(signed int objectType, signed int actorID, signed int index) {
    { // 0x003F0C40 - 0x003F0C60
    }
}

// 0x003F0C60 - 0x003F0C80 (0x20) xScriptRouteDBRouteNavToNav__Fii
signed int xScriptRouteDBRouteNavToNav(signed int na, signed int nb) {
    { // 0x003F0C60 - 0x003F0C80
    }
}

// 0x003F0C80 - 0x003F0CA0 (0x20) xScriptRouteDBRouteToNavigationPoint__Fiii
signed int xScriptRouteDBRouteToNavigationPoint(signed int objectType, signed int actorID, signed int index) {
    { // 0x003F0C80 - 0x003F0CA0
    }
}

// 0x003F0CA0 - 0x003F0CC0 (0x20) xScriptRouteDBRouteToRTObj__Fiii
signed int xScriptRouteDBRouteToRTObj(signed int actorType, signed int actorID, signed int objIndex) {
    { // 0x003F0CA0 - 0x003F0CC0
    }
}

// 0x003F0CC0 - 0x003F0CE0 (0x20) xScriptRouteDBRouteMarkerToMarker__Fii
signed int xScriptRouteDBRouteMarkerToMarker(signed int ma, signed int mb) {
    { // 0x003F0CC0 - 0x003F0CE0
    }
}

// 0x003F0CE0 - 0x003F0D00 (0x20) xScriptRouteDBRouteToMarker__Fiii
signed int xScriptRouteDBRouteToMarker(signed int objectType, signed int actorID, signed int index) {
    { // 0x003F0CE0 - 0x003F0D00
    }
}

// 0x003F0D00 - 0x003F0D20 (0x20) xScriptRouteDBRouteToPlayer__Fii
signed int xScriptRouteDBRouteToPlayer(signed int objectID, signed int actorID) {
    { // 0x003F0D00 - 0x003F0D20
    }
}

// 0x003F0D20 - 0x003F0D40 (0x20) xScriptRouteDBRouteToActor__Fiiii
signed int xScriptRouteDBRouteToActor(signed int fromObjectType, signed int fromActorID, signed int toObjectType, signed int toActorID) {
    { // 0x003F0D20 - 0x003F0D40
    }
}

// 0x003F0D40 - 0x003F0D60 (0x20) xScriptRouteDBRouteAppend__Fii
void xScriptRouteDBRouteAppend(signed int routeID, signed int routeAppendID) {
    { // 0x003F0D40 - 0x003F0D60
    }
}

// 0x003F0D60 - 0x003F0D80 (0x20) xScriptRouteDBRouteStatus__Fi
signed int xScriptRouteDBRouteStatus(signed int id) {
    { // 0x003F0D60 - 0x003F0D80
    }
}

// 0x003F0D80 - 0x003F0DA0 (0x20) xScriptRouteDBRouteDestroy__Fi
void xScriptRouteDBRouteDestroy(signed int routeID) {
    { // 0x003F0D80 - 0x003F0DA0
    }
}

// 0x003F0DA0 - 0x003F0DC0 (0x20) xScriptRouteDBRouteDraw__Fi
void xScriptRouteDBRouteDraw(signed int routeID) {
    { // 0x003F0DA0 - 0x003F0DC0
    }
}

// 0x003F0DC0 - 0x003F0DE0 (0x20) xScriptRouteDBRouteCreate__Fv
signed int xScriptRouteDBRouteCreate() {
    { // 0x003F0DC0 - 0x003F0DE0
    }
}

// 0x003F0DE0 - 0x003F0F80 (0x1A0) xRouteDBRegisterExternals__Fv
void xRouteDBRegisterExternals() {
    { // 0x003F0DE0 - 0x003F0F80
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xParMgrScript.cpp
// ==========================================================================
void (xScriptParMgrSpawnMarker)(signed int, signed int, signed int); // 0x003F22C0
void (xScriptParMgrSpawnMarkerOffset)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x003F1D10
void (xScriptParMgrSpawnActorOffset)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x003F1870
void (xScriptParMgrSpawnActorJoint)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x003F0F80
void (xScriptParMgrSpawnActorJointRnd)(signed int, signed int, signed int, signed int, signed int, signed int, signed int); // 0x003F1370
class xScene *xScene_current; // 0x004A4E08
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x003F0F80 - 0x003F1368 (0x3E8) xScriptParMgrSpawnActorJoint__Fiiiiiii
void xScriptParMgrSpawnActorJoint(signed int vfxType, signed int actorType, signed int actorID, signed int joint, signed int offX, signed int offY, signed int offZ) {
    { // 0x003F0F80 - 0x003F1368
        class tagMATRIX m; // r29+0xa0
        class Vector3D posB; // r29+0xf0
        class Vector3D posA; // r29+0xe0
        class iActorRuntime *r; // r18
        class _xActor *a; // r17
    }
}

// 0x003F1370 - 0x003F186C (0x4FC) xScriptParMgrSpawnActorJointRnd__Fiiiiiii
void xScriptParMgrSpawnActorJointRnd(signed int vfxType, signed int actorType, signed int actorID, signed int joint, signed int count, signed int scale, signed int mode) {
    { // 0x003F1370 - 0x003F186C
        class Vector3D posB; // r29+0x110
        class Vector3D posA; // r29+0x100
        class Vector3D pos; // r29+0xf0
        class iActorRuntime *r; // r19
        class _xActor *a; // r18
        class tagMATRIX m; // r29+0xb0
        signed int hscale; // r17
        signed int i; // r18
    }
}

// 0x003F1870 - 0x003F1D10 (0x4A0) xScriptParMgrSpawnActorOffset__Fiiiiiii
void xScriptParMgrSpawnActorOffset(signed int vfxType, signed int actorType, signed int actorID, signed int offX, signed int offY, signed int offZ, signed int mode) {
    { // 0x003F1870 - 0x003F1D10
        float flr; // r29+0xc0
        class Vector3D posB; // r29+0xb0
        class Vector3D posA; // r29+0xa0
        class iActorRuntime *r; // r17
        class _xActor *a; // r17
    }
}

// 0x003F1D10 - 0x003F22BC (0x5AC) xScriptParMgrSpawnMarkerOffset__Fiiiiiii
void xScriptParMgrSpawnMarkerOffset(signed int vfxType, signed int markerA, signed int markerB, signed int offX, signed int offY, signed int offZ, signed int mode) {
    { // 0x003F1D10 - 0x003F22BC
        class Vector3D b; // r29+0xb0
        class Vector3D a; // r29+0xa0
    }
}

// 0x003F22C0 - 0x003F2710 (0x450) xScriptParMgrSpawnMarker__Fiii
void xScriptParMgrSpawnMarker(signed int vfxType, signed int markerA, signed int markerB) {
    { // 0x003F22C0 - 0x003F2710
        class Vector3D *b; // r2
        class Vector3D *a; // r2
    }
}

// 0x003F2710 - 0x003F27B4 (0xA4) xParMgrRegisterExternals__Fv
void xParMgrRegisterExternals() {
    { // 0x003F2710 - 0x003F27B4
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xFMVScript.cpp
// ==========================================================================
void (xFMVPlayMovie)(unsigned int, unsigned int); // 0x0035F690
void (xFMVStartAudio)(unsigned int, unsigned int); // 0x0035F630
unsigned int (xFMVGetAudioStatus)(); // 0x0035F620
void (xFMVStopAudio)(); // 0x0035F5D0
// 0x003FF5B0 - 0x003FF638 (0x88) xFMVRegisterExternals__Fv
void xFMVRegisterExternals() {
    { // 0x003FF5B0 - 0x003FF638
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xCartScript.cpp
// ==========================================================================
void (xScriptCartFormat)(); // 0x003FF860
signed int (xScriptCartLoadGame)(signed int); // 0x003FF840
signed int (xScriptCartLoadGameDone)(signed int); // 0x003FF820
signed int (xScriptCartSaveGame)(signed int); // 0x003FF800
signed int (xScriptCartSaveGameDone)(signed int); // 0x003FF7E0
signed int (xScriptCartDeleteGame)(signed int); // 0x003FF7C0
signed int (xScriptCartCountValidCardSlots)(signed int); // 0x003FF700
signed int (xScriptCartCountValidSlots)(signed int); // 0x003FF740
signed int (xScriptCartCheckValidSlot)(signed int); // 0x003FF720
signed int (xScriptCartAccessCard)(signed int); // 0x003FF7A0
signed int (xScriptCartAccessCardDone)(signed int); // 0x003FF780
void (xScriptCartSlotInfo)(signed int, signed int, signed int); // 0x003FF6E0
void (xScriptCartBegin)(); // 0x003FF6C0
void (xScriptCartEnd)(); // 0x003FF6A0
signed int (xScriptCartError)(); // 0x003FF760
signed int (xScriptCartGetCardFreeBlocks)(signed int); // 0x003FF680
signed int (xScriptCartIsCardPresent)(); // 0x003FF660
void (xScriptCartBeep)(); // 0x003FF640
// 0x003FF640 - 0x003FF660 (0x20) xScriptCartBeep__Fv
void xScriptCartBeep() {
    { // 0x003FF640 - 0x003FF660
    }
}

// 0x003FF660 - 0x003FF680 (0x20) xScriptCartIsCardPresent__Fv
signed int xScriptCartIsCardPresent() {
    { // 0x003FF660 - 0x003FF680
    }
}

// 0x003FF680 - 0x003FF6A0 (0x20) xScriptCartGetCardFreeBlocks__Fi
signed int xScriptCartGetCardFreeBlocks(signed int slot) {
    { // 0x003FF680 - 0x003FF6A0
    }
}

// 0x003FF6A0 - 0x003FF6C0 (0x20) xScriptCartEnd__Fv
void xScriptCartEnd() {
    { // 0x003FF6A0 - 0x003FF6C0
    }
}

// 0x003FF6C0 - 0x003FF6E0 (0x20) xScriptCartBegin__Fv
void xScriptCartBegin() {
    { // 0x003FF6C0 - 0x003FF6E0
    }
}

// 0x003FF6E0 - 0x003FF700 (0x20) xScriptCartSlotInfo__Fiii
void xScriptCartSlotInfo(signed int cart, signed int slot, signed int data) {
    { // 0x003FF6E0 - 0x003FF700
    }
}

// 0x003FF700 - 0x003FF720 (0x20) xScriptCartCountValidCardSlots__Fi
signed int xScriptCartCountValidCardSlots(signed int cart) {
    { // 0x003FF700 - 0x003FF720
    }
}

// 0x003FF720 - 0x003FF740 (0x20) xScriptCartCheckValidSlot__Fi
signed int xScriptCartCheckValidSlot(signed int slot) {
    { // 0x003FF720 - 0x003FF740
    }
}

// 0x003FF740 - 0x003FF760 (0x20) xScriptCartCountValidSlots__Fi
signed int xScriptCartCountValidSlots(signed int cart) {
    { // 0x003FF740 - 0x003FF760
    }
}

// 0x003FF760 - 0x003FF780 (0x20) xScriptCartError__Fv
signed int xScriptCartError() {
    { // 0x003FF760 - 0x003FF780
    }
}

// 0x003FF780 - 0x003FF7A0 (0x20) xScriptCartAccessCardDone__Fi
signed int xScriptCartAccessCardDone(signed int card) {
    { // 0x003FF780 - 0x003FF7A0
    }
}

// 0x003FF7A0 - 0x003FF7C0 (0x20) xScriptCartAccessCard__Fi
signed int xScriptCartAccessCard(signed int card) {
    { // 0x003FF7A0 - 0x003FF7C0
    }
}

// 0x003FF7C0 - 0x003FF7E0 (0x20) xScriptCartDeleteGame__Fi
signed int xScriptCartDeleteGame(signed int slot) {
    { // 0x003FF7C0 - 0x003FF7E0
    }
}

// 0x003FF7E0 - 0x003FF800 (0x20) xScriptCartSaveGameDone__Fi
signed int xScriptCartSaveGameDone(signed int slot) {
    { // 0x003FF7E0 - 0x003FF800
    }
}

// 0x003FF800 - 0x003FF820 (0x20) xScriptCartSaveGame__Fi
signed int xScriptCartSaveGame(signed int slot) {
    { // 0x003FF800 - 0x003FF820
    }
}

// 0x003FF820 - 0x003FF840 (0x20) xScriptCartLoadGameDone__Fi
signed int xScriptCartLoadGameDone(signed int slot) {
    { // 0x003FF820 - 0x003FF840
    }
}

// 0x003FF840 - 0x003FF860 (0x20) xScriptCartLoadGame__Fi
signed int xScriptCartLoadGame(signed int slot) {
    { // 0x003FF840 - 0x003FF860
    }
}

// 0x003FF860 - 0x003FF880 (0x20) xScriptCartFormat__Fv
void xScriptCartFormat() {
    { // 0x003FF860 - 0x003FF880
    }
}

// 0x003FF880 - 0x003FFAAC (0x22C) xCartRegisterExternals__Fv
void xCartRegisterExternals() {
    { // 0x003FF880 - 0x003FFAAC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xAnimMgr.cpp
// ==========================================================================
class _reent *_impure_ptr; // 0x00445DAC
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
struct @anon0 __vt__8xAnimMgr; // 0x004A2840
class xAnimMgr *gAnimMgr; // 0x004A5060
// 0x003FFBD0 - 0x003FFC24 (0x54) getAnimData__8xAnimMgrFUi
class xAnimData *getAnimData(class xAnimMgr *this, unsigned int anim_ID) {
    { // 0x003FFBD0 - 0x003FFC24
        class xAnimData **ea; // r2
        class xAnimData **ai; // r2
    }
}

// 0x003FFC30 - 0x003FFFC4 (0x394) loadAnim__8xAnimMgrFPCcPCcUi
unsigned int loadAnim(class xAnimMgr *this, char *filename, char *name) {
    { // 0x003FFC30 - 0x003FFFC4
        class xAnimData *dat; // r29+0x9c
        class iAnimData *res; // r29+0x98
    }
}

// 0x003FFFD0 - 0x003FFFD8 (0x8) nextFrame__8xAnimMgrFv
void nextFrame() {
    { // 0x003FFFD0 - 0x003FFFD8
    }
}

// 0x003FFFE0 - 0x004000B8 (0xD8) kill__8xAnimMgrFv
void kill(class xAnimMgr *this) {
    { // 0x003FFFE0 - 0x004000B8
        class iAnimData **del; // r3
        class iAnimData **re; // r2
        class iAnimData **ri; // r2
        class xAnimData **del; // r3
        class xAnimData **ae; // r2
        class xAnimData **ai; // r2
    }
}

// 0x004000C0 - 0x0040023C (0x17C) init__8xAnimMgrFv
void init(class xAnimMgr *this) {
    { // 0x004000C0 - 0x0040023C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xAnimMgr.h
// ==========================================================================
// 0x003FFAB0 - 0x003FFB78 (0xC8) __dt__8xAnimMgrFv
void *__dt(class xAnimMgr *this) {
    { // 0x003FFAB0 - 0x003FFB78
    }
}

// 0x003FFB80 - 0x003FFBCC (0x4C) restart__8xAnimMgrFv
void restart(class xAnimMgr *this) {
    { // 0x003FFB80 - 0x003FFBCC
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xGameObj.cpp
// ==========================================================================
struct @anon0 __vt__8xGameObj; // 0x004A2870
static unsigned int sGameUniqueID; // 0x004A5064
// 0x00400330 - 0x00400394 (0x64) GetLocalZ__8xGameObjFv
class _RwV3d *GetLocalZ(class xGameObj *this) {
    { // 0x00400330 - 0x00400394
        class _RwMatrix *m; // r2
        class _RwFrame *f; // r2
    }
}

// 0x004003A0 - 0x00400404 (0x64) GetPos__8xGameObjFv
class _RwV3d *GetPos(class xGameObj *this) {
    { // 0x004003A0 - 0x00400404
        class _RwMatrix *m; // r2
        class _RwFrame *f; // r2
    }
}

// 0x00400410 - 0x004004B8 (0xA8) SetPos__8xGameObjFfff
void SetPos(class xGameObj *this, float x, float y, float z) {
    { // 0x00400410 - 0x004004B8
        class _RwFrame *f; // r17
        class _RwMatrix *m; // r16
        class _RwV3d *new_pos; // r2
    }
}

// 0x004004C0 - 0x00400568 (0xA8) SetPos__8xGameObjFP8Vector3D
void SetPos(class xGameObj *this, class Vector3D *v) {
    { // 0x004004C0 - 0x00400568
    }
}

// 0x00400570 - 0x004005A4 (0x34) __ct__8xGameObjFv
void *__ct(class xGameObj *this) {
    { // 0x00400570 - 0x004005A4
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iCollide.cpp
// ==========================================================================
class _RpCollisionTriangle * (iRayCastCallback)(class _RpIntersection *, class _RpWorldSector *, class _RpCollisionTriangle *, float, void *); // 0x00400660
class _RpCollisionTriangle * (iSphereCastCallback)(class _RpIntersection *, class _RpWorldSector *, class _RpCollisionTriangle *, float, void *); // 0x00000000
class xScene *xScene_current; // 0x004A4E08
class Vector3D *gBSPOffset; // 0x004A4FA4
class _RpCollisionTriangle * (WorldColCallback)(class _RpIntersection *, class _RpWorldSector *, class _RpCollisionTriangle *, float, void *); // 0x00000000
static signed int mCollideResult; // 0x004A5068
// 0x004005B0 - 0x00400660 (0xB0) iRayCastfrompoint__FP14tagCollideInfo
signed int iRayCastfrompoint(class tagCollideInfo *info) {
    { // 0x004005B0 - 0x00400660
        class _RwLine *ray; // r2
        class _RpIntersection intersection; // r29+0x10
        class _RpWorld *World; // r2
    }
}

// 0x00400660 - 0x00400700 (0xA0) iRayCastCallback__FP15_RpIntersectionP14_RpWorldSectorP20_RpCollisionTrianglefPv
static class _RpCollisionTriangle *iRayCastCallback(class _RpCollisionTriangle *triangle, float distance, void *info_data) {
    { // 0x00400660 - 0x00400700
        class tagCollideInfo *info; // r2
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xCollide.cpp
// ==========================================================================
// 0x00404190 - 0x004041B0 (0x20) xCollideRay__FP14tagCollideInfo
signed int xCollideRay(class tagCollideInfo *info) {
    { // 0x00404190 - 0x004041B0
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iAnimMgr.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
static unsigned char SkinPipeInitialized; // 0x004A506C
// 0x004041B0 - 0x00404280 (0xD0) iAnimMgrInit__Fv
void iAnimMgrInit() {
    { // 0x004041B0 - 0x00404280
        unsigned char stat; // r3
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iAnimData.cpp
// ==========================================================================
struct @anon0 __vt__9iAnimData; // 0x004A2890
// 0x00404280 - 0x004042F0 (0x70) __dt__9iAnimDataFv
void *__dt(class iAnimData *this) {
    { // 0x00404280 - 0x004042F0
    }
}

// 0x004042F0 - 0x00404388 (0x98) __ct__9iAnimDataFPCcPCc
void *__ct(class iAnimData *this, char *filename, char *name) {
    { // 0x004042F0 - 0x00404388
        char *act_name; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xGameObjMgr.cpp
// ==========================================================================
struct @anon1 __vt__7xModule; // 0x004A2790
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
struct @anon0 __vt__11xGameObjMgr; // 0x004A28A0
class xGameObjMgr *gGameObjMgr; // 0x004A5070
// 0x00404390 - 0x00404484 (0xF4) gameObjAdd__11xGameObjMgrFP8xGameObj
void gameObjAdd(class xGameObjMgr *this, class xGameObj *obj) {
    { // 0x00404390 - 0x00404484
    }
}

// 0x00404490 - 0x004044B4 (0x24) gameObjCreate__11xGameObjMgrFUi
class xGameObj *gameObjCreate(unsigned int type) {
    { // 0x00404490 - 0x004044B4
    }
}

// 0x004044C0 - 0x00404514 (0x54) render__11xGameObjMgrFv
void render(class xGameObjMgr *this) {
    { // 0x004044C0 - 0x00404514
        class xGameObj *o; // r16
    }
}

// 0x00404520 - 0x00404574 (0x54) nextFrame__11xGameObjMgrFv
void nextFrame(class xGameObjMgr *this) {
    { // 0x00404520 - 0x00404574
        class xGameObj *o; // r16
    }
}

// 0x00404580 - 0x004045D4 (0x54) restart__11xGameObjMgrFv
void restart(class xGameObjMgr *this) {
    { // 0x00404580 - 0x004045D4
        class xGameObj *o; // r16
    }
}

// 0x004045E0 - 0x00404634 (0x54) kill__11xGameObjMgrFv
void kill(class xGameObjMgr *this) {
    { // 0x004045E0 - 0x00404634
        class xGameObj *o; // r16
    }
}

// 0x00404640 - 0x0040464C (0xC) init__11xGameObjMgrFv
void init(class xGameObjMgr *this) {
    { // 0x00404640 - 0x0040464C
    }
}

// 0x00404650 - 0x0040467C (0x2C) __ct__11xGameObjMgrFv
void *__ct(class xGameObjMgr *this) {
    { // 0x00404650 - 0x0040467C
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xGameObjFactory.cpp
// ==========================================================================
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x00404680 - 0x00404740 (0xC0) create__15xGameObjFactoryFUi
class xGameObj *create() {
    { // 0x00404680 - 0x00404740
        class xGameObj *go; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zGameObjMgr.cpp
// ==========================================================================
class xGameObjMgr *gGameObjMgr; // 0x004A5070
struct @anon0 __vt__11zGameObjMgr; // 0x004A28D0
// 0x00404740 - 0x00404764 (0x24) gameObjCreate__11zGameObjMgrFUi
class xGameObj *gameObjCreate(unsigned int type) {
    { // 0x00404740 - 0x00404764
    }
}

// 0x00404770 - 0x004047B4 (0x44) __ct__11zGameObjMgrFv
void *__ct(class zGameObjMgr *this) {
    { // 0x00404770 - 0x004047B4
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zGameObjFactory.cpp
// ==========================================================================
struct @anon2 __vt__10zCharacter; // 0x004A2820
struct @anon1 __vt__14xGameCharacter; // 0x004A2900
struct @anon0 __vt__15zGameObjVillain; // 0x004A2970
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
// 0x004047C0 - 0x00404980 (0x1C0) create__15zGameObjFactoryFUi
class xGameObj *create(unsigned int type) {
    { // 0x004047C0 - 0x00404980
        class xGameObj *go; // r16
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\x\xAnimData.cpp
// ==========================================================================
struct @anon0 __vt__13iAnimInstance; // 0x004A2930
struct @anon1 __vt__9xAnimData; // 0x004A2920
// 0x00404980 - 0x004049EC (0x6C) __dt__9xAnimDataFv
void *__dt(class xAnimData *this) {
    { // 0x00404980 - 0x004049EC
    }
}

// 0x004049F0 - 0x00404A24 (0x34) __ct__9xAnimDataFUiUiP9iAnimData
void *__ct(class xAnimData *this, unsigned int asset_id, unsigned int anim_ID, class iAnimData *res) {
    { // 0x004049F0 - 0x00404A24
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Engine\p2\iAnimData.h
// ==========================================================================
// 0x00404A30 - 0x00404A88 (0x58) __dt__13iAnimInstanceFv
void *__dt(class iAnimInstance *this) {
    { // 0x00404A30 - 0x00404A88
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zGameObj.cpp
// ==========================================================================
class xScene *xScene_current; // 0x004A4E08
float g_RADTODEG; // 0x00485A60
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
struct @anon0 __vt__8zGameObj; // 0x004A2940
// 0x00404A90 - 0x00404B64 (0xD4) render__8zGameObjFv
void render(class zGameObj *this) {
    { // 0x00404A90 - 0x00404B64
    }
}

// 0x00404B70 - 0x00404C4C (0xDC) nextFrame__8zGameObjFv
void nextFrame(class zGameObj *this) {
    { // 0x00404B70 - 0x00404C4C
        class _RwV3d p; // r29+0x50
        class Vector3D *pos; // r2
        class _RwMatrix *m; // r17
        class _RwFrame *f; // r16
        class _RwV3d up; // r29+0x40
    }
}

// 0x00404C50 - 0x00404C58 (0x8) restart__8zGameObjFv
void restart() {
    { // 0x00404C50 - 0x00404C58
    }
}

// 0x00404C60 - 0x00404C68 (0x8) kill__8zGameObjFv
void kill() {
    { // 0x00404C60 - 0x00404C68
    }
}

// 0x00404C70 - 0x00404CB8 (0x48) init__8zGameObjFPc
void init(class zGameObj *this, char *filename) {
    { // 0x00404C70 - 0x00404CB8
    }
}

// 0x00404CC0 - 0x00404DD0 (0x110) init__8zGameObjFv
void init(class zGameObj *this) {
    { // 0x00404CC0 - 0x00404DD0
    }
}

// 0x00404DD0 - 0x00404E28 (0x58) __dt__8zGameObjFv
void *__dt(class zGameObj *this) {
    { // 0x00404DD0 - 0x00404E28
    }
}

// 0x00404E30 - 0x00404E70 (0x40) __ct__8zGameObjFv
void *__ct(class zGameObj *this) {
    { // 0x00404E30 - 0x00404E70
    }
}


// ==========================================================================
// C:\Projects\ScoobyDoo\Code\Game\z\zGameObjVillain.cpp
// ==========================================================================
class _RpAnimSequence * (_clumpAddSequence)(class _RpAnimSequence *, void *); // 0x00000000
class _RwFrame * (_animClumpForAllFramesAddSequencesCallBack)(class _RwFrame *, void *); // 0x00000000
float deltaFrame; // 0x004A4C88
class _rpAtomic * (hackUpdateSkinAtomic)(class _rpAtomic *, void *); // 0x004056C0
class xScene *xScene_current; // 0x004A4E08
class Vector3D *gBSPOffset; // 0x004A4FA4
float g_RADTODEG; // 0x00485A60
unsigned long gTimerValue; // 0x004A5340
unsigned int bad_guy_idle_ID; // 0x004A5044
unsigned int bad_guy_anim_ID; // 0x004A5040
class xAnimMgr *gAnimMgr; // 0x004A5060
class XStreamHeader currentHeader; // 0x004AD860
char vAssertBuf2[128]; // 0x00AFF460
class _rpAtomic * (zSkinAnimSetupAtomicCallback)(class _rpAtomic *, void *); // 0x004059D0
float g_DEGTORAD; // 0x00485A58
struct @anon0 __vt__15zGameObjVillain; // 0x004A2970
static class _SkinData zSkinData; // 0x00000000
static unsigned char was_active_last_frame; // 0x00000000
static signed int cur_point; // 0x004A5074
// 0x00404E70 - 0x00404E78 (0x8) render__15zGameObjVillainFv
void render() {
    { // 0x00404E70 - 0x00404E78
    }
}

// 0x00404E80 - 0x004056BC (0x83C) nextFrame__15zGameObjVillainFv
void nextFrame(class zGameObjVillain *this) {
    { // 0x00404E80 - 0x004056BC
        class _RwMatrix *m; // r20
        class _RwFrame *f; // r18
        class _RwV3d up; // r29+0xe0
        class _RwV3d *rw_local_z; // r2
        class _RwMatrix *matrix; // r2
        class Vector3D motion_2d; // r29+0xd0
        class tagCollideInfo collision; // r29+0x80
        float delta; // r29+0xec
        class Vector3D *orig; // r17
        class Vector3D *pos; // r16
        class _RwV3d p; // r29+0xc0
    }
}

// 0x004056C0 - 0x00405720 (0x60) hackUpdateSkinAtomic__FP9_rpAtomicPv
static class _rpAtomic *hackUpdateSkinAtomic(class _rpAtomic *atomic, void *data) {
    { // 0x004056C0 - 0x00405720
        class _RpSkin *skin; // r2
        float delta; // 
    }
}

// 0x00405720 - 0x00405728 (0x8) restart__15zGameObjVillainFv
void restart() {
    { // 0x00405720 - 0x00405728
    }
}

// 0x00405730 - 0x00405738 (0x8) kill__15zGameObjVillainFv
void kill() {
    { // 0x00405730 - 0x00405738
    }
}

// 0x00405740 - 0x004057F0 (0xB0) init__15zGameObjVillainFv
void init() {
    { // 0x00405740 - 0x004057F0
    }
}

// 0x004057F0 - 0x004059C4 (0x1D4) init__15zGameObjVillainFPc
void init(class zGameObjVillain *this, char *filename) {
    { // 0x004057F0 - 0x004059C4
    }
}

// 0x004059D0 - 0x00405A3C (0x6C) zSkinAnimSetupAtomicCallback__FP9_rpAtomicPv
static class _rpAtomic *zSkinAnimSetupAtomicCallback(class _rpAtomic *atomic, void *data) {
    { // 0x004059D0 - 0x00405A3C
        class _RpSkinAnim *ska; // r2
        class _RpSkin *sk; // r2
    }
}

