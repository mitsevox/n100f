// Type definitions recovered from DWARF 1 (.debug).
// One entry per distinct definition; '// from' names the first
// compile unit that described it. Offsets are member offsets.

// from viewskel.cpp
enum AMode {
    ASpinClump = 1,
    ANoAction = 0
};

// from viewskel.cpp
enum MMMode {
    MMPanLight = 9,
    MMMoveCamera = 8,
    MMAimCamera = 7,
    MMDragClumpInZ = 6,
    MMDragClump = 5,
    MMSpinClump = 4,
    MMTiltCamera = 3,
    MMPanAndZoomCamera = 2,
    MMScrollBackdrop = 1,
    MMNoAction = 0
};

// from menu.cpp
enum RsEventStatus {
    rsEVENTNOTPROCESSED = 2,
    rsEVENTPROCESSED = 1,
    rsEVENTERROR = 0
};

// from menu.cpp
enum RsMenuEvent {
    rsPADANALOGUERIGHT = 8,
    rsPADANALOGUELEFT = 7,
    rsPADBUTTONUP = 6,
    rsPADBUTTONDOWN = 5,
    rsKEYUP = 4,
    rsKEYDOWN = 3,
    rsMOUSEMOVE = 2,
    rsLEFTBUTTONUP = 1,
    rsLEFTBUTTONDOWN = 0
};

// from viewskel.cpp
enum VMode {
    VModeWorld = 1,
    VModeClump = 0
};

// from zCharacter.cpp
enum _RpAnimType {
    rpANIMTYPEFORCEENUMSIZEINT = 2147483647,
    rpROTATE = 2,
    rpTRANSLATE = 1,
    rpERR = 0
};

// from viewskel.cpp
enum _RpIntersectType {
    rpINTERSECTTYPEFORCEENUMSIZEINT = 2147483647,
    rpINTERSECTATOMIC = 5,
    rpINTERSECTBOX = 4,
    rpINTERSECTSPHERE = 3,
    rpINTERSECTPOINT = 2,
    rpINTERSECTLINE = 1,
    rpINTERSECTNONE = 0
};

// from viewskel.cpp
enum _RwCameraProjection {
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = 2147483647,
    rwPARALLEL = 2,
    rwPERSPECTIVE = 1,
    rwNACAMERAPROJECTION = 0
};

// from sky.cpp
enum _RwDebugType {
    rwDEBUGTYPEFORCEENUMSIZEINT = 2147483647,
    rwDEBUGTRACE = 4,
    rwDEBUGMESSAGE = 3,
    rwDEBUGERROR = 2,
    rwDEBUGASSERT = 1,
    rwNADEBUGTYPE = 0
};

// from view.cpp
enum _RwStreamAccessType {
    rwSTREAMACCESSTYPEFORCEENUMSIZEINT = 2147483647,
    rwSTREAMAPPEND = 3,
    rwSTREAMWRITE = 2,
    rwSTREAMREAD = 1,
    rwNASTREAMACCESS = 0
};

// from view.cpp
enum _RwStreamType {
    rwSTREAMTYPEFORCEENUMSIZEINT = 2147483647,
    rwSTREAMCUSTOM = 4,
    rwSTREAMMEMORY = 3,
    rwSTREAMFILENAME = 2,
    rwSTREAMFILE = 1,
    rwNASTREAM = 0
};

// from viewskel.cpp
enum _RwTextureAddressMode {
    rwTEXTUREADDRESSMODEFORCEENUMSIZEINT = 2147483647,
    rwTEXTUREADDRESSBORDER = 4,
    rwTEXTUREADDRESSCLAMP = 3,
    rwTEXTUREADDRESSMIRROR = 2,
    rwTEXTUREADDRESSWRAP = 1,
    rwTEXTUREADDRESSNATEXTUREADDRESS = 0
};

// from viewskel.cpp
enum _RwTextureFilterMode {
    rwTEXTUREFILTERMODEFORCEENUMSIZEINT = 2147483647,
    rwFILTERLINEARMIPLINEAR = 6,
    rwFILTERLINEARMIPNEAREST = 5,
    rwFILTERMIPLINEAR = 4,
    rwFILTERMIPNEAREST = 3,
    rwFILTERLINEAR = 2,
    rwFILTERNEAREST = 1,
    rwFILTERNAFILTERMODE = 0
};

// from sky.cpp
enum _RwVideoModeFlag {
    rwVIDEOMODEFLAGFORCEENUMSIZEINT = 2147483647,
    rwVIDEOMODEforceEnumSize = 2147483647,
    rwVIDEOMODEFSAA1 = 16,
    rwVIDEOMODEFSAA0 = 8,
    rwVIDEOMODEFFINTERLACE = 4,
    rwVIDEOMODEINTERLACE = 2,
    rwVIDEOMODEEXCLUSIVE = 1
};

// from viewskel.cpp
enum _RxClusterForcePresent {
    rxCLUSTERFORCEPRESENTFORCEENUMSIZEINT = 2147483647,
    rxCLFORCEPRESENT = 1,
    rxCLALLOWABSENT = 0
};

// from viewskel.cpp
enum _RxClusterValid {
    rxCLUSTERVALIDFORCEENUMSIZEINT = 2147483647,
    rxCLVALID_INVALID = 2,
    rxCLVALID_VALID = 1,
    rxCLVALID_NOCHANGE = 0
};

// from viewskel.cpp
enum _RxClusterValidityReq {
    rxCLUSTERVALIDITYREQFORCEENUMSIZEINT = 2147483647,
    rxCLREQ_OPTIONAL = 2,
    rxCLREQ_REQUIRED = 1,
    rxCLREQ_DONTWANT = 0
};

// from viewskel.cpp
enum _RxNodeDefEditable {
    rxNODEDEFEDITABLEFORCEENUMSIZEINT = 2147483647,
    rxNODEDEFEDITABLE = 1,
    rxNODEDEFCONST = 0
};

// from viewskel.cpp
enum _rpWorldRenderOrder {
    rpWORLDRENDERORDERFORCEENUMSIZEINT = 2147483647,
    rpWORLDRENDERBACK2FRONT = 2,
    rpWORLDRENDERFRONT2BACK = 1,
    rpWORLDRENDERNARENDERORDER = 0
};

// from viewskel.cpp
enum _rxEmbeddedPacketState {
    rxEMBEDDEDPACKETSTATEFORCEENUMSIZEINT = 2147483647,
    rxPKST_PENDING = 3,
    rxPKST_INUSE = 2,
    rxPKST_UNUSED = 1,
    rxPKST_PACKETLESS = 0
};

// from xParCmd.cpp
enum eParCmdTag {
    eParCmdTagCount = 38,
    eParCmdModulateColorTag = 37,
    eParCmdTurbulenceTag = 36,
    eParCmdKillInvisTag = 35,
    eParCmdTextureTag = 34,
    eParCmdRotMoveTag = 33,
    eParCmdKillPlaneTag = 32,
    eParCmdSpawnVFXTag = 31,
    eParCmdKillBoxTag = 30,
    eParCmdLimitVelocityTag = 29,
    eParCmdEnableCmdTag = 28,
    eParCmdDisableCmdTag = 27,
    eParCmdKillCmdTag = 26,
    eParCmdEmitterTag = 25,
    eParCmdAccelerateTag = 24,
    eParCmdKillOldTag = 23,
    eParCmdKillSlowTag = 22,
    eParCmdFrictionTag = 21,
    eParCmdVortexTag = 20,
    eParCmdTargetSizeTag = 19,
    eParCmdTargetColorTag = 18,
    eParCmdSourceTag = 17,
    eParCmdSinkVelocityTag = 16,
    eParCmdSinkTag = 15,
    eParCmdRestoreTag = 14,
    eParCmdRandomVelocityTag = 13,
    eParCmdRandomDisplaceTag = 12,
    eParCmdRandomAccelTag = 11,
    eParCmdOrbitPointTag = 10,
    eParCmdOrbitLineTag = 9,
    eParCmdMoveTag = 8,
    eParCmdJetTag = 7,
    eParCmdGravitateTag = 6,
    eParCmdFollowTag = 5,
    eParCmdExplosionTag = 4,
    eParCmdDampingTag = 3,
    eParCmdCopyVertexBTag = 2,
    eParCmdBounceTag = 1,
    eParCmdHeaderTag = 0
};

// from ios.cpp
enum event {
    yfmt_event = 1886348034,
    ue_event = 1651337473,
    se_event = 1634886912
};

// from ios.cpp
enum fmtflags {
    oatfield = 1818624260,
    sefield = 1633812554,
    justfield = 1684078768,
    percase = 1886732288,
    itbuf = 1853169664,
    ipws = 1802702848,
    owpos = 1752369152,
    owpoint = 1752368128,
    owbase = 1752367616,
    ientific = 1668481280,
    ght = 1769078912,
    t = 1668218944,
    ft = 1701576736,
    ternal = 1852375056,
    x = 1701314568,
    xed = 1768292356,
    c = 1701052418,
    olalpha = 1868693505
};

// from ios.cpp
enum iostate {
    lbit = 1767990788,
    bit = 1718576386,
    bit = 1684103681,
    dbit = 1869571840
};

// from xCart.cpp
enum p2CartActions {
    I_CART_FORMAT = 3,
    I_CART_READ = 2,
    I_CART_WRITE = 1,
    I_CART_IDLE = 0
};

// from xCart.cpp
enum p2CartStatus {
    I_CART_READ_END = 5,
    I_CART_READ_READ = 4,
    I_CART_READ_BEGIN = 3,
    I_CART_WRITE_END = 2,
    I_CART_WRITE_WRITE = 1,
    I_CART_WRITE_BEGIN = 0
};

// from xRouteDB.cpp
// size: 0x8
class @class {
    /* 0x0000 */ class xRouteTargetActor m_actor;
    /* 0x0000 */ class xRouteTargetObject m_object;
    /* 0x0000 */ class xRouteTargetTrigger m_trigger;
    /* 0x0000 */ class xRouteTargetNavPoint m_navPoint;
    /* 0x0000 */ class xRouteTargetMarker m_marker;
};

// from xRouteDB.cpp
// size: 0x4
class @class {
    /* 0x0000 */ class xRouteUseagePatrol m_patrol;
    /* 0x0000 */ class xRouteUseageOneShot m_oneShot;
};

// from iAudioStream.cpp
// size: 0x4
class @class {
    /* 0x0000 */ class @class ts;
    /* 0x0000 */ signed int tgsz;
};

// from iAudioStream.cpp
// size: 0x4
class @class {
    /* 0x0000 */ unsigned int size : 31 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int tag : 1 /* bit_offset 31 */;
};

// from i_prim.cpp
// size: 0x4
class @class {
    /* 0x0000 */ class _RwRGBA preLitColor;
    /* 0x0000 */ class _RwRGBA color;
};

// from i_prim.cpp
// size: 0x4
class @class {
    /* 0x0000 */ unsigned char bAlpha;
    /* 0x0001 */ unsigned char bBlue;
    /* 0x0002 */ unsigned char bGreen;
    /* 0x0003 */ unsigned char bRed;
};

// from JFrame.cpp
// size: 0x4
class @class {
    /* 0x0000 */ unsigned int word1;
};

// from JInterpr.cpp
// size: 0x4
class @class {
    /* 0x0000 */ void *untyped;
    /* 0x0000 */ void (*v_v_funcPtr)();
    /* 0x0000 */ void (*v_s_funcPtr)(char *);
    /* 0x0000 */ signed int (*i_si_funcPtr)(char *, signed int);
    /* 0x0000 */ signed int (*i_ssi_funcPtr)(char *, char *, signed int);
    /* 0x0000 */ void (*v_u_funcPtr)(unsigned int);
    /* 0x0000 */ void (*v_is_funcPtr)(signed int, char *);
    /* 0x0000 */ void (*v_iis_funcPtr)(signed int, signed int, char *);
    /* 0x0000 */ void (*v_iss_funcPtr)(signed int, char *, char *);
    /* 0x0000 */ void (*v_isss_funcPtr)(signed int, char *, char *, char *);
    /* 0x0000 */ void (*v_isii_funcPtr)(signed int, char *, signed int, signed int);
    /* 0x0000 */ void (*v_issii_funcPtr)(signed int, char *, char *, signed int, signed int);
    /* 0x0000 */ signed int (*i_issiii_funcPtr)(signed int, char *, char *, signed int, signed int, signed int);
    /* 0x0000 */ void (*v_i_funcPtr)(signed int);
    /* 0x0000 */ signed int (*i_v_funcPtr)();
    /* 0x0000 */ signed int (*i_i_funcPtr)(signed int);
    /* 0x0000 */ signed int (*i_ii_funcPtr)(signed int, signed int);
    /* 0x0000 */ signed int (*i_iii_funcPtr)(signed int, signed int, signed int);
    /* 0x0000 */ signed int (*i_iiii_funcPtr)(signed int, signed int, signed int, signed int);
    /* 0x0000 */ signed int (*i_iiiii_funcPtr)(signed int, signed int, signed int, signed int, signed int);
    /* 0x0000 */ void (*v_ii_funcPtr)(signed int, signed int);
    /* 0x0000 */ void (*v_iii_funcPtr)(signed int, signed int, signed int);
    /* 0x0000 */ void (*v_iiii_funcPtr)(signed int, signed int, signed int, signed int);
    /* 0x0000 */ void (*v_iiiii_funcPtr)(signed int, signed int, signed int, signed int, signed int);
    /* 0x0000 */ void (*v_iiiiiii_funcPtr)(signed int, signed int, signed int, signed int, signed int, signed int, signed int);
    /* 0x0000 */ void (*v_iiiiiiii_funcPtr)(signed int, signed int, signed int, signed int, signed int, signed int, signed int, signed int);
};

// from xAnimMgr.cpp
// size: 0xF0
class @class {
    /* 0x0000 */ class @class _reent;
    /* 0x0000 */ class @class _unused;
};

// from xAnimMgr.cpp
// size: 0x4C
class @class {
    /* 0x0000 */ unsigned int _rand_next;
    /* 0x0004 */ char *_strtok_last;
    /* 0x0008 */ char _asctime_buf[26];
    /* 0x0024 */ class tm _localtime_buf;
    /* 0x0048 */ signed int _gamma_signgam;
};

// from xAnimMgr.cpp
// size: 0xF0
class @class {
    /* 0x0000 */ unsigned char *_nextf[30];
    /* 0x0078 */ unsigned int _nmalloc[30];
};

// from viewskel.cpp
// size: 0x24
class AppCamera : public Camera {
    /* 0x0010 */ class _RwRaster *backdrop;
    /* 0x0014 */ float cameraDistance;
    /* 0x0018 */ class _RwV2d offset;
    /* 0x0020 */ class _RpWorld *world;
};

// from iSndMgr.cpp
// size: 0x34
class AudioStream_s {
    /* 0x0000 */ signed int id;
    /* 0x0004 */ class AudioStream_s *pNext;
    /* 0x0008 */ signed int status;
    /* 0x000C */ void *pAmStream;
    /* 0x0010 */ signed int PendingRequestCounter;
    /* 0x0014 */ unsigned int interruptsTillEnd;
    /* 0x0018 */ char *pFileName;
    /* 0x001C */ void *pPlayBuffer;
    /* 0x0020 */ void *pTransferBuffer;
    /* 0x0024 */ signed int QueueRequested;
    /* 0x0028 */ signed int Queued;
    /* 0x002C */ void (*m_callback)(class AudioStream_s *);
    /* 0x0030 */ signed short lastVol;
    /* 0x0032 */ signed short lastVolAtten;
};

// from viewskel.cpp
// size: 0x10
class Camera {
    /* 0x0000 */ class _RwCamera *camera;
    /* 0x0004 */ signed int m_Preset;
    /* 0x0008 */ float m_TetherLength;
    /* 0x000C */ float m_TetherHeightPerc;
};

// from ios.cpp
// size: 0x10
class CharArray {
    /* 0x0000 */ unsigned int size_;
    /* 0x0004 */ class _EmptyMemberOpt alloc_;
    /* 0x0008 */ unsigned int refcount_;
    /* 0x000C */ char *data_;
};

// from JFrame.cpp
// size: 0x20
class ExecutionFrame {
    /* 0x0000 */ class JExecutableMethod *theMethod;
    /* 0x0004 */ unsigned char *pc8;
    /* 0x0008 */ unsigned char *origPc8;
    /* 0x000C */ unsigned int *lvPool;
    /* 0x0010 */ unsigned int *opTop;
    /* 0x0014 */ unsigned short lvPoolIndex;
    /* 0x0016 */ unsigned short opPoolIndex;
    /* 0x0018 */ unsigned char returnEncountered;
    /* 0x0019 */ unsigned char exceptionThrown;
    /* 0x001A */ unsigned char returnWords;
    /* 0x001B */ unsigned char lastException;
    /* 0x001C */ void *previousFrame;
};

// from JInterpr.cpp
// size: 0x2A4
class ExecutionThread {
    /* 0x0000 */ class ExecutionFrame frameStack[20];
    /* 0x0280 */ unsigned int wakeParameter;
    /* 0x0284 */ unsigned int wakeParameter2;
    /* 0x0288 */ unsigned int wakeParameter3;
    /* 0x028C */ unsigned int wakeCondition;
    /* 0x0290 */ signed int frameCount;
    /* 0x0294 */ unsigned int amAlive;
    /* 0x0298 */ unsigned int amActive;
    /* 0x029C */ unsigned int previousWakeParameter;
    /* 0x02A0 */ unsigned int pauseFlags;
};

// from JInterpr.cpp
// size: 0xC
class ExternalFunc {
    /* 0x0000 */ char *funcName;
    /* 0x0004 */ signed int funcType;
    /* 0x0008 */ class @class f;
};

// from JFrame.cpp
// size: 0x14
class JExecutableClass {
    /* 0x0000 */ signed short classNameIndex;
    /* 0x0002 */ signed short constantIndex;
    /* 0x0004 */ signed short resolvedParentDataWords;
    /* 0x0006 */ signed short resolvedDataWords;
    /* 0x0008 */ signed short executableMethodIndexInPool;
    /* 0x000A */ signed short executableFieldIndexInPool;
    /* 0x000C */ unsigned short executableMethodsCount;
    /* 0x000E */ unsigned short executableFieldsCount;
    /* 0x0010 */ signed short superClassIndex;
    /* 0x0012 */ unsigned char myIndex;
    /* 0x0013 */ unsigned char amInitialized;
};

// from JFrame.cpp
// size: 0x38
class JExecutableDescriptor {
    /* 0x0000 */ unsigned int fieldPoolUsed;
    /* 0x0004 */ unsigned int methodPoolUsed;
    /* 0x0008 */ unsigned int classCount;
    /* 0x000C */ unsigned int codePoolUsed;
    /* 0x0010 */ unsigned int stringPoolUsed;
    /* 0x0014 */ unsigned int constantValuePoolUsed;
    /* 0x0018 */ unsigned int constantMapPoolUsed;
    /* 0x001C */ class JExecutableField *fieldPool;
    /* 0x0020 */ class JExecutableMethod *methodPool;
    /* 0x0024 */ class JExecutableClass *classList;
    /* 0x0028 */ unsigned char *codePoolU8;
    /* 0x002C */ unsigned char *stringPool;
    /* 0x0030 */ unsigned int *constantValuePool;
    /* 0x0034 */ signed short *constantMapPool;
};

// from JFrame.cpp
// size: 0x8
class JExecutableField {
    /* 0x0000 */ class @class value;
    /* 0x0004 */ signed short resolvedOffset;
    /* 0x0006 */ unsigned short resolvedWords;
};

// from JFrame.cpp
// size: 0x18
class JExecutableMethod {
    /* 0x0000 */ unsigned short methodNameIndex;
    /* 0x0002 */ unsigned short descriptorIndex;
    /* 0x0004 */ unsigned short accessFlags;
    /* 0x0006 */ signed char parameterWordCount;
    /* 0x0007 */ signed char isExternal;
    /* 0x0008 */ unsigned short specialFunctionFlags;
    /* 0x000A */ signed short executableClassIndex;
    /* 0x000C */ signed short externalRecordIndex;
    /* 0x000E */ unsigned short maxStack;
    /* 0x0010 */ unsigned short maxLocals;
    /* 0x0012 */ unsigned short codeLength;
    /* 0x0014 */ signed int codeIndexInPool;
};

// from menu.cpp
// size: 0x28
class MenuEntryTag {
    /* 0x0000 */ signed int type;
    /* 0x0004 */ char *description;
    /* 0x0008 */ void *target;
    /* 0x000C */ signed int minValue;
    /* 0x0010 */ signed int maxValue;
    /* 0x0014 */ char **enumStrings;
    /* 0x0018 */ signed int shortcut;
    /* 0x001C */ signed int (*triggerCallBack)();
    /* 0x0020 */ class MenuEntryTag *next;
    /* 0x0024 */ class MenuEntryTag *prev;
};

// from menu.cpp
// size: 0x44
class MenuStateTag {
    /* 0x0000 */ class _RwRaster *InverseCharset;
    /* 0x0004 */ class _RwRaster *MainCharset;
    /* 0x0008 */ class MenuEntryTag *activeEntry;
    /* 0x000C */ class MenuEntryTag *menuList;
    /* 0x0010 */ class _RwRGBA MBackground;
    /* 0x0014 */ class _RwRGBA MForeground;
    /* 0x0018 */ signed int Shifted;
    /* 0x001C */ unsigned int lastKeyPressed;
    /* 0x0020 */ signed int longestEntryLength;
    /* 0x0024 */ signed int mode;
    /* 0x0028 */ signed int numEntries;
    /* 0x002C */ signed int numEntriesPerColumn;
    /* 0x0030 */ signed int numHelpLines;
    /* 0x0034 */ signed int topHelpLine;
    /* 0x0038 */ unsigned int timeOfLastAutoRepeat;
    /* 0x003C */ unsigned int timeOfLastChange;
    /* 0x0040 */ unsigned int timeOfLastKeyPress;
};

// from xMessMgr.cpp
// size: 0x34
class MessageRedo {
    /* 0x0000 */ class xMessage theMessage;
    /* 0x002C */ unsigned int targetType;
    /* 0x0030 */ unsigned int targetId;
};

// from xMessMgr.cpp
// size: 0x18
class MessageRegistration {
    /* 0x0000 */ void (*myHandler)(class xMessage *);
    /* 0x0004 */ unsigned int myType;
    /* 0x0008 */ unsigned char myIdList[4];
    /* 0x000C */ signed int senderType;
    /* 0x0010 */ signed int senderId;
    /* 0x0014 */ signed int message;
};

// from viewskel.cpp
// size: 0x1C
class PadMap {
    /* 0x0000 */ class RwSkel *skel;
    /* 0x0004 */ class _RsMouseStatus ms;
};

// from xMain.cpp
// size: 0xE0
class PerfTimes {
    /* 0x0000 */ unsigned long total;
    /* 0x0008 */ unsigned long lasttotal;
    /* 0x0010 */ unsigned long console;
    /* 0x0018 */ unsigned long sound;
    /* 0x0020 */ unsigned long anim;
    /* 0x0028 */ unsigned long animPost;
    /* 0x0030 */ unsigned long actor;
    /* 0x0038 */ unsigned long obj;
    /* 0x0040 */ unsigned long script;
    /* 0x0048 */ unsigned long scriptPF;
    /* 0x0050 */ unsigned long particle;
    /* 0x0058 */ unsigned long path;
    /* 0x0060 */ unsigned long route;
    /* 0x0068 */ unsigned long gui;
    /* 0x0070 */ unsigned long cart;
    /* 0x0078 */ unsigned long gstate;
    /* 0x0080 */ unsigned long msg;
    /* 0x0088 */ unsigned long x1;
    /* 0x0090 */ unsigned long x2;
    /* 0x0098 */ unsigned long x3;
    /* 0x00A0 */ unsigned long scrfx;
    /* 0x00A8 */ unsigned long fmv;
    /* 0x00B0 */ unsigned long xidle;
    /* 0x00B8 */ unsigned long xsceneidle;
    /* 0x00C0 */ unsigned long camera;
    /* 0x00C8 */ unsigned long decode;
    /* 0x00D0 */ unsigned long main0V;
    /* 0x00D8 */ unsigned long plate;
};

// from viewskel.cpp
// size: 0x8
class Platform {
    /* 0x0000 */ signed int quit;
};

// from sky.cpp
// size: 0x18
class Ps : public Platform {
    /* 0x0008 */ signed int fullScreen;
    /* 0x000C */ class _RwV2d lastMousePos;
    /* 0x0014 */ signed int validDelta;
};

// from sky.cpp
// size: 0x8
class RWInfo {
    /* 0x0000 */ class AppCamera *m_camera;
    /* 0x0004 */ class RwSkel *m_skel;
};

// from viewskel.cpp
// size: 0x14
class RsMouse {
    /* 0x0000 */ class _RwV2d pos;
    /* 0x0008 */ class _RwRaster *raster;
    /* 0x000C */ class PsMouse *psmouse;
    /* 0x0010 */ signed int draw;
};

// from viewskel.cpp
// size: 0x18
class RwSkel {
    /* 0x0000 */ char *appName;
    /* 0x0004 */ signed int maximumWidth;
    /* 0x0008 */ signed int maximumHeight;
    /* 0x000C */ class RsMouse *mouse;
    /* 0x0010 */ class Platform *ps;
};

// from camera.cpp
// size: 0xC
class Vector3D {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
};

// from xState.cpp
// size: 0x10
class Vector4D {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
    /* 0x000C */ float t;
};

// from viewskel.cpp
// size: 0x28
class ViewApp {
    /* 0x0000 */ class _RpWorld *world;
    /* 0x0004 */ class _RpClump *clump;
    /* 0x0008 */ class _RpLight *dirLight;
    /* 0x000C */ class _RpLight *ambLight;
    /* 0x0010 */ class AppCamera *appCamera;
    /* 0x0014 */ class _rpAtomic *pickedAtomic;
    /* 0x0018 */ class _RwMatrix *spinMatrix;
    /* 0x001C */ enum VMode viewingMode;
    /* 0x0020 */ enum MMMode mouseMoveMode;
    /* 0x0024 */ enum AMode animMode;
};

// from viewskel.cpp
// size: 0x20
class ViewSkel : public RwSkel {
    /* 0x0018 */ class ViewApp *Viewer;
    /* 0x001C */ class PadMap *PdMap;
};

// from xStream.cpp
// size: 0x28
class XStream {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ signed int currentOffset;
    /* 0x0008 */ unsigned int *data;
    /* 0x000C */ unsigned int length;
    /* 0x0010 */ unsigned int offset;
    /* 0x0014 */ unsigned int done;
    /* 0x0018 */ unsigned int assetID;
    /* 0x001C */ class tagXStreamDirectoryEntry *dir;
    /* 0x0020 */ unsigned int count;
    /* 0x0024 */ unsigned int index;
};

// from xTrigger.cpp
// size: 0x4698
class XStreamHeader {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ unsigned int versionID;
    /* 0x0008 */ unsigned char month;
    /* 0x0009 */ unsigned char day;
    /* 0x000A */ unsigned char hour;
    /* 0x000B */ unsigned char minute;
    /* 0x000C */ unsigned int mdsOffset;
    /* 0x0010 */ unsigned int mdsLength;
    /* 0x0014 */ class tagXStreamMDSDirectory directory;
    /* 0x3E98 */ char safety[2048];
};

// from xAnimMgr.cpp
// size: 0x18
class _Bigint {
    /* 0x0000 */ class _Bigint *_next;
    /* 0x0004 */ signed int _k;
    /* 0x0008 */ signed int _maxwds;
    /* 0x000C */ signed int _sign;
    /* 0x0010 */ signed int _wds;
    /* 0x0014 */ unsigned int _x[1];
};

// from ios.cpp
// size: 0x4
class _EmptyMemberOpt : public allocator {
    /* 0x0000 */ class CharArray *m_;
};

// from ios.cpp
// size: 0x4
class _EmptyMemberOpt : public allocator {
    /* 0x0000 */ unsigned int m_;
};

// from iMain.cpp
// size: 0x40
class _Env {
    /* 0x0000 */ char m_name[32];
    /* 0x0020 */ char m_value[32];
};

// from xState.cpp
// size: 0x6
class _FCinfo {
    /* 0x0000 */ unsigned short nidx[3];
};

// from i_prim.cpp
// size: 0x48
class _LineRec {
    /* 0x0000 */ class _RxObjSpace3DVertex a;
    /* 0x0024 */ class _RxObjSpace3DVertex b;
};

// from ios.cpp
// size: 0x8
class _MSLstring {
    /* 0x0000 */ class _RefCountedPtr data_;
};

// from xTex.cpp
// size: 0x10
class _PVRheader {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ unsigned int nTextureDataSize;
    /* 0x0008 */ unsigned int nTextureType;
    /* 0x000C */ unsigned short nWidth;
    /* 0x000E */ unsigned short nHeight;
};

// from ios.cpp
// size: 0x8
class _RefCountedPtr {
    /* 0x0000 */ class __locale_imp *ptr_;
    /* 0x0004 */ signed int *refCount_;
};

// from ios.cpp
// size: 0x8
class _RefCountedPtr {
    /* 0x0000 */ char *ptr_;
    /* 0x0004 */ signed int *refCount_;
};

// from zCharacter.cpp
// size: 0x1C
class _RpAnimSequence {
    /* 0x0000 */ signed int refCnt;
    /* 0x0004 */ char *name;
    /* 0x0008 */ enum _RpAnimType type;
    /* 0x000C */ signed int numKeys;
    /* 0x0010 */ void *keys;
    /* 0x0014 */ signed int numInterpolators;
    /* 0x0018 */ class _rpAnimInterpolator *interps;
};

// from viewskel.cpp
// size: 0x20
class _RpClump {
    /* 0x0000 */ class _RwObject object;
    /* 0x0008 */ class _RwLinkList atomicList;
    /* 0x0010 */ class _RwLLLink inWorldLink;
    /* 0x0018 */ class _RpClump * (*callback)(class _RpClump *, void *);
    /* 0x001C */ unsigned short renderFrame;
};

// from viewskel.cpp
// size: 0x3
class _RpCollSector {
    /* 0x0000 */ unsigned char cType;
    /* 0x0001 */ unsigned char vertex;
    /* 0x0002 */ unsigned char start;
};

// from view.cpp
// size: 0x28
class _RpCollisionTriangle {
    /* 0x0000 */ class _RwV3d normal;
    /* 0x000C */ class _RwV3d point;
    /* 0x0018 */ signed int index;
    /* 0x001C */ class _RwV3d *vertices[3];
};

// from viewskel.cpp
// size: 0x44
class _RpGeometry {
    /* 0x0000 */ class _RwObject object;
    /* 0x0008 */ unsigned short lockedSinceLastInst;
    /* 0x000A */ signed short refCount;
    /* 0x000C */ class _RpMaterialList matList;
    /* 0x0018 */ signed int numTriangles;
    /* 0x001C */ signed int numVertices;
    /* 0x0020 */ signed int numMorphTargets;
    /* 0x0024 */ class _RpTriangle *triangles;
    /* 0x0028 */ class _RwRGBA *preLitLum;
    /* 0x002C */ class _RwTexCoords *vertexTexCoords;
    /* 0x0030 */ class _RwSurfaceProperties ignoredSurfaceProps;
    /* 0x003C */ class _RpMeshHeader *mesh;
    /* 0x0040 */ class _RpMorphTarget *morphTarget;
};

// from viewskel.cpp
// size: 0x18
class _RpIntersectData {
    /* 0x0000 */ class _RwLine line;
    /* 0x0000 */ class _RwV3d point;
    /* 0x0000 */ class _RwSphere sphere;
    /* 0x0000 */ class _RwBBox box;
    /* 0x0000 */ void *object;
};

// from viewskel.cpp
// size: 0x1C
class _RpIntersection {
    /* 0x0000 */ class _RpIntersectData t;
    /* 0x0018 */ enum _RpIntersectType type;
};

// from viewskel.cpp
// size: 0x40
class _RpLight {
    /* 0x0000 */ class _RwObjectHasFrame object;
    /* 0x0014 */ float radius;
    /* 0x0018 */ class _RwRGBAReal color;
    /* 0x0028 */ float minusCosAngle;
    /* 0x002C */ class _RwLinkList WorldSectorsInLight;
    /* 0x0034 */ class _RwLLLink inWorld;
    /* 0x003C */ unsigned short lightFrame;
};

// from viewskel.cpp
// size: 0x1C
class _RpMaterial {
    /* 0x0000 */ class _RwTexture *texture;
    /* 0x0004 */ class _RwRGBA color;
    /* 0x0008 */ class _RxPipeline *pipeline;
    /* 0x000C */ class _RwSurfaceProperties surfaceProps;
    /* 0x0018 */ signed short refCount;
    /* 0x001A */ signed short pad;
};

// from viewskel.cpp
// size: 0xC
class _RpMaterialList {
    /* 0x0000 */ class _RpMaterial **materials;
    /* 0x0004 */ signed int numMaterials;
    /* 0x0008 */ signed int space;
};

// from viewskel.cpp
// size: 0x10
class _RpMeshHeader {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ unsigned short numMeshes;
    /* 0x0006 */ unsigned short serialNum;
    /* 0x0008 */ unsigned int totalIndicesInMesh;
    /* 0x000C */ unsigned int firstMeshOffset;
};

// from viewskel.cpp
// size: 0x1C
class _RpMorphTarget {
    /* 0x0000 */ class _RpGeometry *parentGeom;
    /* 0x0004 */ class _RwSphere boundingSphere;
    /* 0x0014 */ class _RwV3d *verts;
    /* 0x0018 */ class _RwV3d *normals;
};

// from viewskel.cpp
// size: 0x8
class _RpPolygon {
    /* 0x0000 */ unsigned short matIndex;
    /* 0x0002 */ unsigned short vertIndex[3];
};

// from viewskel.cpp
// size: 0x4
class _RpSector {
    /* 0x0000 */ signed int type;
};

// from zCharacter.cpp
// size: 0x20
class _RpSkin {
    /* 0x0000 */ signed int numBones;
    /* 0x0004 */ class _RpSkinBoneInfo *pBoneInfo;
    /* 0x0008 */ signed int totalVertices;
    /* 0x000C */ class _RpGeometry *pGeometry;
    /* 0x0010 */ class _RpSkinSkeleton *pCurrentSkeleton;
    /* 0x0014 */ unsigned int *pMatrixIndexMap;
    /* 0x0018 */ class _RwMatrixWeights *pMatrixWeightsMap;
    /* 0x001C */ void *pPlatformData;
};

// from zDemo.cpp
// size: 0x10
class _RpSkinAnim {
    /* 0x0000 */ signed int numFrames;
    /* 0x0004 */ signed int flags;
    /* 0x0008 */ float duration;
    /* 0x000C */ class _RpSkinFrame *pFrames;
};

// from zCharacter.cpp
// size: 0x60
class _RpSkinBoneInfo {
    /* 0x0000 */ signed int boneTag;
    /* 0x0004 */ signed int boneIndex;
    /* 0x0008 */ signed int flags;
    /* 0x0010 */ class _RwMatrix boneToSkinMat;
    /* 0x0050 */ class _RwFrame *pFrame;
};

// from zDemo.cpp
// size: 0x24
class _RpSkinFrame {
    /* 0x0000 */ class _RtQuat q;
    /* 0x0010 */ class _RwV3d t;
    /* 0x001C */ float time;
    /* 0x0020 */ class _RpSkinFrame *prevFrame;
};

// from zCharacter.cpp
// size: 0x2C
class _RpSkinSkeleton {
    /* 0x0000 */ signed int numBones;
    /* 0x0004 */ class _RpSkinAnim *pCurrentAnim;
    /* 0x0008 */ float currentTime;
    /* 0x000C */ class _RpSkinFrame *pNextFrame;
    /* 0x0010 */ class _RpSkinSkeleton * (*pAnimCallBack)(class _RpSkinSkeleton *, void *);
    /* 0x0014 */ void *pAnimCallBackData;
    /* 0x0018 */ float animCallBackTime;
    /* 0x001C */ class _RpSkinSkeleton * (*pAnimLoopCallBack)(class _RpSkinSkeleton *, void *);
    /* 0x0020 */ void *pAnimLoopCallBackData;
    /* 0x0024 */ class _RwMatrix *pMatrixArray;
    /* 0x0028 */ void *pMatrixArrayUnaligned;
};

// from viewskel.cpp
// size: 0x8
class _RpTriangle {
    /* 0x0000 */ unsigned short vertIndex[3];
    /* 0x0006 */ signed short matIndex;
};

// from viewskel.cpp
// size: 0x4
class _RpVertexNormal {
    /* 0x0000 */ signed char x;
    /* 0x0001 */ signed char y;
    /* 0x0002 */ signed char z;
    /* 0x0003 */ unsigned char pad;
};

// from viewskel.cpp
// size: 0x74
class _RpWorld {
    /* 0x0000 */ class _RwObject object;
    /* 0x0008 */ enum _rpWorldRenderOrder renderOrder;
    /* 0x000C */ class _RpMaterialList matList;
    /* 0x0018 */ class _RpSector *rootSector;
    /* 0x001C */ signed int numClumpsInWorld;
    /* 0x0020 */ class _RwLLLink *currentClumpLink;
    /* 0x0024 */ class _RwLinkList clumpList;
    /* 0x002C */ class _RwLinkList lightList;
    /* 0x0034 */ class _RwLinkList directionalLightList;
    /* 0x003C */ class _RwV3d worldOrigin;
    /* 0x0048 */ class _RwBBox boundingBox;
    /* 0x0060 */ class _RwSurfaceProperties surfaceProps;
    /* 0x006C */ class _RpWorldSector * (*renderCallBack)(class _RpWorldSector *);
    /* 0x0070 */ class _RxPipeline *pipeline;
};

// from viewskel.cpp
// size: 0x60
class _RpWorldSector {
    /* 0x0000 */ signed int type;
    /* 0x0004 */ class _RpPolygon *polygons;
    /* 0x0008 */ class _RwV3d *vertices;
    /* 0x000C */ class _RpVertexNormal *normals;
    /* 0x0010 */ class _RwTexCoords *vertexTexCoords;
    /* 0x0014 */ class _RwRGBA *preLitLum;
    /* 0x0018 */ class _rwResEntryTag *repEntry;
    /* 0x001C */ class _RwLinkList collAtomicsInWorldSector;
    /* 0x0024 */ class _RwLinkList noCollAtomicsInWorldSector;
    /* 0x002C */ class _RwLinkList lightsInWorldSector;
    /* 0x0034 */ class _RwBBox boundingBox;
    /* 0x004C */ class _RpCollSector *colSectorRoot;
    /* 0x0050 */ class _RpMeshHeader *mesh;
    /* 0x0054 */ class _RxPipeline *pipeline;
    /* 0x0058 */ unsigned short matListWindowBase;
    /* 0x005A */ unsigned short numVertices;
    /* 0x005C */ unsigned short numPolygons;
};

// from viewskel.cpp
// size: 0x8
class _RsKeyStatus {
    /* 0x0000 */ signed int keyScanCode;
    /* 0x0004 */ signed int keyCharCode;
};

// from viewskel.cpp
// size: 0x18
class _RsMouseStatus {
    /* 0x0000 */ class _RwV2d pos;
    /* 0x0008 */ class _RwV2d delta;
    /* 0x0010 */ signed int shift;
    /* 0x0014 */ signed int control;
};

// from viewskel.cpp
// size: 0x8
class _RsPadButtonStatus {
    /* 0x0000 */ signed int padID;
    /* 0x0004 */ unsigned int padButtons;
};

// from menu.cpp
// size: 0x14
class _RtCharsetDesc {
    /* 0x0000 */ signed int width;
    /* 0x0004 */ signed int height;
    /* 0x0008 */ signed int count;
    /* 0x000C */ signed int tilewidth;
    /* 0x0010 */ signed int tileheight;
};

// from zDemo.cpp
// size: 0x10
class _RtQuat {
    /* 0x0000 */ class _RwV3d imag;
    /* 0x000C */ float real;
};

// from viewskel.cpp
// size: 0x18
class _RwBBox {
    /* 0x0000 */ class _RwV3d sup;
    /* 0x000C */ class _RwV3d inf;
};

// from viewskel.cpp
// size: 0x190
class _RwCamera {
    /* 0x0000 */ class _RwObjectHasFrame object;
    /* 0x0014 */ enum _RwCameraProjection projectionType;
    /* 0x0018 */ class _RwCamera * (*beginUpdate)(class _RwCamera *);
    /* 0x001C */ class _RwCamera * (*endUpdate)(class _RwCamera *);
    /* 0x0020 */ class _RwMatrix viewMatrix;
    /* 0x0060 */ class _RwRaster *frameBuffer;
    /* 0x0064 */ class _RwRaster *zBuffer;
    /* 0x0068 */ class _RwV2d viewWindow;
    /* 0x0070 */ class _RwV2d recipViewWindow;
    /* 0x0078 */ class _RwV2d viewOffset;
    /* 0x0080 */ float nearPlane;
    /* 0x0084 */ float farPlane;
    /* 0x0088 */ float fogPlane;
    /* 0x008C */ float zScale;
    /* 0x0090 */ float zShift;
    /* 0x0094 */ unsigned short renderFrame;
    /* 0x0098 */ class _RwFrustumPlane frustumPlanes[6];
    /* 0x0110 */ class _RwBBox frustumBoundBox;
    /* 0x0128 */ class _RwV3d frustumCorners[8];
};

// from skel.cpp
// size: 0x4
class _RwEngineOpenParams {
    /* 0x0000 */ void *displayID;
};

// from skyfs.cpp
// size: 0x28
class _RwFileFunctions {
    /* 0x0000 */ signed int (*rwfexist)(char *);
    /* 0x0004 */ void * (*rwfopen)(char *, char *);
    /* 0x0008 */ signed int (*rwfclose)(void *);
    /* 0x000C */ unsigned int (*rwfread)(void *, unsigned int, unsigned int, void *);
    /* 0x0010 */ unsigned int (*rwfwrite)(void *, unsigned int, unsigned int, void *);
    /* 0x0014 */ char * (*rwfgets)(char *, signed int, void *);
    /* 0x0018 */ signed int (*rwfputs)(char *, void *);
    /* 0x001C */ signed int (*rwfeof)(void *);
    /* 0x0020 */ signed int (*rwfseek)(void *, signed long, signed int);
    /* 0x0024 */ signed int (*rwfflush)(void *);
};

// from viewskel.cpp
// size: 0xB0
class _RwFrame {
    /* 0x0000 */ class _RwObject object;
    /* 0x0010 */ class _RwMatrix modelling;
    /* 0x0050 */ class _RwMatrix ltm;
    /* 0x0090 */ class _RwLLLink inDirtyListLink;
    /* 0x0098 */ class _RwLinkList objectList;
    /* 0x00A0 */ class _RwFrame *child;
    /* 0x00A4 */ class _RwFrame *next;
    /* 0x00A8 */ class _RwFrame *root;
};

// from viewskel.cpp
// size: 0x14
class _RwFrustumPlane {
    /* 0x0000 */ class _RwPlane plane;
    /* 0x0010 */ unsigned char closestX;
    /* 0x0011 */ unsigned char closestY;
    /* 0x0012 */ unsigned char closestZ;
    /* 0x0013 */ unsigned char pad;
};

// from mouse.cpp
// size: 0x1C
class _RwImage {
    /* 0x0000 */ signed int flags;
    /* 0x0004 */ signed int width;
    /* 0x0008 */ signed int height;
    /* 0x000C */ signed int depth;
    /* 0x0010 */ signed int stride;
    /* 0x0014 */ unsigned char *cpPixels;
    /* 0x0018 */ class _RwRGBA *palette;
};

// from viewskel.cpp
// size: 0x8
class _RwLLLink {
    /* 0x0000 */ class _RwLLLink *next;
    /* 0x0004 */ class _RwLLLink *prev;
};

// from viewskel.cpp
// size: 0x18
class _RwLine {
    /* 0x0000 */ class _RwV3d start;
    /* 0x000C */ class _RwV3d end;
};

// from viewskel.cpp
// size: 0x8
class _RwLinkList {
    /* 0x0000 */ class _RwLLLink link;
};

// from viewskel.cpp
// size: 0x40
class _RwMatrix {
    /* 0x0000 */ class _RwV3d right;
    /* 0x000C */ unsigned int flags;
    /* 0x0010 */ class _RwV3d up;
    /* 0x001C */ unsigned int pad1;
    /* 0x0020 */ class _RwV3d at;
    /* 0x002C */ unsigned int pad2;
    /* 0x0030 */ class _RwV3d pos;
    /* 0x003C */ unsigned int pad3;
};

// from zCharacter.cpp
// size: 0x10
class _RwMatrixWeights {
    /* 0x0000 */ float w0;
    /* 0x0004 */ float w1;
    /* 0x0008 */ float w2;
    /* 0x000C */ float w3;
};

// from sky.cpp
// size: 0x10
class _RwMemoryFunctions {
    /* 0x0000 */ void * (*rwmalloc)(unsigned int);
    /* 0x0004 */ void (*rwfree)(void *);
    /* 0x0008 */ void * (*rwrealloc)(void *, unsigned int);
    /* 0x000C */ void * (*rwcalloc)(unsigned int, unsigned int);
};

// from skyfs.cpp
// size: 0x8
class _RwModuleInfo {
    /* 0x0000 */ signed int globalsOffset;
    /* 0x0004 */ signed int numInstances;
};

// from viewskel.cpp
// size: 0x8
class _RwObject {
    /* 0x0000 */ unsigned char type;
    /* 0x0001 */ unsigned char subType;
    /* 0x0002 */ unsigned char flags;
    /* 0x0003 */ unsigned char privateFlags;
    /* 0x0004 */ void *parent;
};

// from viewskel.cpp
// size: 0x14
class _RwObjectHasFrame {
    /* 0x0000 */ class _RwObject object;
    /* 0x0008 */ class _RwLLLink lFrame;
    /* 0x0010 */ class _RwObjectHasFrame * (*sync)(class _RwObjectHasFrame *);
};

// from viewskel.cpp
// size: 0x10
class _RwPlane {
    /* 0x0000 */ class _RwV3d normal;
    /* 0x000C */ float distance;
};

// from viewskel.cpp
// size: 0x4
class _RwRGBA {
    /* 0x0000 */ unsigned char red;
    /* 0x0001 */ unsigned char green;
    /* 0x0002 */ unsigned char blue;
    /* 0x0003 */ unsigned char alpha;
};

// from viewskel.cpp
// size: 0x10
class _RwRGBAReal {
    /* 0x0000 */ float red;
    /* 0x0004 */ float green;
    /* 0x0008 */ float blue;
    /* 0x000C */ float alpha;
};

// from viewskel.cpp
// size: 0x34
class _RwRaster {
    /* 0x0000 */ class _RwRaster *parent;
    /* 0x0004 */ unsigned char *cpPixels;
    /* 0x0008 */ unsigned char *palette;
    /* 0x000C */ signed int width;
    /* 0x0010 */ signed int height;
    /* 0x0014 */ signed int depth;
    /* 0x0018 */ signed int stride;
    /* 0x001C */ signed short nOffsetX;
    /* 0x001E */ signed short nOffsetY;
    /* 0x0020 */ unsigned char cType;
    /* 0x0021 */ unsigned char cFlags;
    /* 0x0022 */ unsigned char privateFlags;
    /* 0x0023 */ unsigned char cFormat;
    /* 0x0024 */ unsigned char *originalPixels;
    /* 0x0028 */ signed int originalWidth;
    /* 0x002C */ signed int originalHeight;
    /* 0x0030 */ signed int originalStride;
};

// from viewskel.cpp
// size: 0x10
class _RwRect {
    /* 0x0000 */ signed int x;
    /* 0x0004 */ signed int y;
    /* 0x0008 */ signed int w;
    /* 0x000C */ signed int h;
};

// from i_prim.cpp
// size: 0x40
class _RwSky2DVertex {
    /* 0x0000 */ class _RwSky2DVertexAlignmentOverlay u;
};

// from i_prim.cpp
// size: 0x40
class _RwSky2DVertexAlignmentOverlay {
    /* 0x0000 */ class _RwSky2DVertexFields els;
    /* 0x0000 */ u_long128 qWords[4];
};

// from i_prim.cpp
// size: 0x40
class _RwSky2DVertexFields {
    /* 0x0000 */ class _RwV3d scrVertex;
    /* 0x000C */ float camVertex_z;
    /* 0x0010 */ float u;
    /* 0x0014 */ float v;
    /* 0x0018 */ float recipZ;
    /* 0x001C */ float pad1;
    /* 0x0020 */ class _RwRGBAReal color;
    /* 0x0030 */ class _RwV3d objNormal;
    /* 0x003C */ float pad2;
};

// from viewskel.cpp
// size: 0x10
class _RwSphere {
    /* 0x0000 */ class _RwV3d center;
    /* 0x000C */ float radius;
};

// from view.cpp
// size: 0x24
class _RwStream {
    /* 0x0000 */ enum _RwStreamType type;
    /* 0x0004 */ enum _RwStreamAccessType accessType;
    /* 0x0008 */ signed int position;
    /* 0x000C */ class _RwStreamUnion Type;
    /* 0x0020 */ signed int rwOwned;
};

// from view.cpp
// size: 0x14
class _RwStreamCustom {
    /* 0x0000 */ signed int (*sfnclose)(void *);
    /* 0x0004 */ unsigned int (*sfnread)(void *, void *, unsigned int);
    /* 0x0008 */ signed int (*sfnwrite)(void *, void *, unsigned int);
    /* 0x000C */ signed int (*sfnskip)(void *, unsigned int);
    /* 0x0010 */ void *data;
};

// from view.cpp
// size: 0x4
class _RwStreamFile {
    /* 0x0000 */ void *fpFile;
};

// from view.cpp
// size: 0xC
class _RwStreamMemory {
    /* 0x0000 */ unsigned int position;
    /* 0x0004 */ unsigned int nSize;
    /* 0x0008 */ unsigned char *memBlock;
};

// from view.cpp
// size: 0x14
class _RwStreamUnion {
    /* 0x0000 */ class _RwStreamMemory memory;
    /* 0x0000 */ class _RwStreamFile file;
    /* 0x0000 */ class _RwStreamCustom custom;
};

// from viewskel.cpp
// size: 0xC
class _RwSurfaceProperties {
    /* 0x0000 */ float ambient;
    /* 0x0004 */ float specular;
    /* 0x0008 */ float diffuse;
};

// from viewskel.cpp
// size: 0x8
class _RwTexCoords {
    /* 0x0000 */ float u;
    /* 0x0004 */ float v;
};

// from viewskel.cpp
// size: 0x18
class _RwTexDictionary {
    /* 0x0000 */ class _RwObject object;
    /* 0x0008 */ class _RwLinkList texturesInDict;
    /* 0x0010 */ class _RwLLLink lInInstance;
};

// from viewskel.cpp
// size: 0x60
class _RwTexture {
    /* 0x0000 */ class _RwRaster *raster;
    /* 0x0004 */ class _RwTexDictionary *dict;
    /* 0x0008 */ class _RwLLLink lInDictionary;
    /* 0x0010 */ char name[32];
    /* 0x0030 */ char mask[32];
    /* 0x0050 */ signed int refCount;
    /* 0x0054 */ enum _RwTextureFilterMode filtering;
    /* 0x0058 */ enum _RwTextureAddressMode addressingU;
    /* 0x005C */ enum _RwTextureAddressMode addressingV;
};

// from viewskel.cpp
// size: 0x8
class _RwV2d {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
};

// from viewskel.cpp
// size: 0xC
class _RwV3d {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
};

// from sky.cpp
// size: 0x10
class _RwVideoMode {
    /* 0x0000 */ signed int width;
    /* 0x0004 */ signed int height;
    /* 0x0008 */ signed int depth;
    /* 0x000C */ enum _RwVideoModeFlag flags;
};

// from viewskel.cpp
// size: 0x1C
class _RxCluster {
    /* 0x0000 */ unsigned short flags;
    /* 0x0002 */ unsigned short stride;
    /* 0x0004 */ void *data;
    /* 0x0008 */ void *currentData;
    /* 0x000C */ unsigned int numAlloced;
    /* 0x0010 */ unsigned int numUsed;
    /* 0x0014 */ class _RxClusterUnion clusterUnion;
    /* 0x0018 */ unsigned int attributes;
};

// from viewskel.cpp
// size: 0x10
class _RxClusterDefinition {
    /* 0x0000 */ char *name;
    /* 0x0004 */ unsigned int defaultStride;
    /* 0x0008 */ unsigned int defaultAttributes;
    /* 0x000C */ char *attributeSet;
};

// from viewskel.cpp
// size: 0xC
class _RxClusterRef {
    /* 0x0000 */ class _RxClusterDefinition *clusterDef;
    /* 0x0004 */ enum _RxClusterForcePresent forcePresent;
    /* 0x0008 */ unsigned int reserved;
};

// from viewskel.cpp
// size: 0x4
class _RxClusterUnion {
    /* 0x0000 */ class _RxClusterDefinition *clusterDef;
    /* 0x0000 */ class _RxPipelineCluster *clusterRef;
};

// from viewskel.cpp
// size: 0x18
class _RxHeap {
    /* 0x0000 */ unsigned int superBlockSize;
    /* 0x0004 */ class _rxHeapSuperBlockDescriptor *head;
    /* 0x0008 */ class _rxHeapBlockHeader *headBlock;
    /* 0x000C */ class _rxHeapFreeBlock *freeBlocks;
    /* 0x0010 */ unsigned int entriesAlloced;
    /* 0x0014 */ unsigned int entriesUsed;
};

// from viewskel.cpp
// size: 0x14
class _RxIoSpec {
    /* 0x0000 */ unsigned int numClustersOfInterest;
    /* 0x0004 */ class _RxClusterRef *clustersOfInterest;
    /* 0x0008 */ enum _RxClusterValidityReq *inputRequirements;
    /* 0x000C */ unsigned int numOutputs;
    /* 0x0010 */ class _RxOutputSpec *outputs;
};

// from viewskel.cpp
// size: 0x40
class _RxNodeDefinition {
    /* 0x0000 */ char *name;
    /* 0x0004 */ class _RxNodeMethods nodeMethods;
    /* 0x0020 */ class _RxIoSpec io;
    /* 0x0034 */ unsigned int pipelineNodePrivateDataSize;
    /* 0x0038 */ enum _RxNodeDefEditable editable;
    /* 0x003C */ signed int InputPipesCnt;
};

// from viewskel.cpp
// size: 0x1C
class _RxNodeMethods {
    /* 0x0000 */ signed int (*nodeBody)(class _RxPipelineNode *, class _RxPipelineNodeParam *);
    /* 0x0004 */ signed int (*nodeInit)(class _RxNodeDefinition *);
    /* 0x0008 */ void (*nodeTerm)(class _RxNodeDefinition *);
    /* 0x000C */ signed int (*pipelineNodeInit)(class _RxPipelineNode *);
    /* 0x0010 */ void (*pipelineNodeTerm)(class _RxPipelineNode *);
    /* 0x0014 */ signed int (*pipelineNodeConfig)(class _RxPipelineNode *, class _RxPipeline *);
    /* 0x0018 */ unsigned int (*configMsgHandler)(class _RxPipelineNode *, unsigned int, unsigned int, void *);
};

// from i_prim.cpp
// size: 0x24
class _RxObjSpace3DVertex {
    /* 0x0000 */ class _RwV3d objVertex;
    /* 0x000C */ class @class c;
    /* 0x0010 */ class _RwV3d objNormal;
    /* 0x001C */ float u;
    /* 0x0020 */ float v;
};

// from viewskel.cpp
// size: 0xC
class _RxOutputSpec {
    /* 0x0000 */ char *name;
    /* 0x0004 */ enum _RxClusterValid *outputClusters;
    /* 0x0008 */ enum _RxClusterValid allOtherClusters;
};

// from viewskel.cpp
// size: 0x30
class _RxPacket {
    /* 0x0000 */ unsigned short flags;
    /* 0x0002 */ unsigned short numClusters;
    /* 0x0004 */ class _RxPipeline *pipeline;
    /* 0x0008 */ unsigned int *inputToClusterSlot;
    /* 0x000C */ unsigned int *slotsContinue;
    /* 0x0010 */ class _RxPipelineCluster **slotClusterRefs;
    /* 0x0014 */ class _RxCluster clusters[1];
};

// from viewskel.cpp
// size: 0x2C
class _RxPipeline {
    /* 0x0000 */ signed int locked;
    /* 0x0004 */ unsigned int numNodes;
    /* 0x0008 */ class _RxPipelineNode *nodes;
    /* 0x000C */ void *nodesBlock;
    /* 0x0010 */ unsigned int packetNumClusterSlots;
    /* 0x0014 */ enum _rxEmbeddedPacketState embeddedPacketState;
    /* 0x0018 */ class _RxPacket *embeddedPacket;
    /* 0x001C */ unsigned int numInputRequirements;
    /* 0x0020 */ class _RxPipelineRequiresCluster *inputRequirements;
    /* 0x0024 */ unsigned int *superBlock;
    /* 0x0028 */ unsigned int entryPoint;
};

// from viewskel.cpp
// size: 0x8
class _RxPipelineCluster {
    /* 0x0000 */ class _RxClusterDefinition *clusterRef;
    /* 0x0004 */ unsigned int creationAttributes;
};

// from viewskel.cpp
// size: 0x20
class _RxPipelineNode {
    /* 0x0000 */ class _RxNodeDefinition *nodeDef;
    /* 0x0004 */ unsigned int numOutputs;
    /* 0x0008 */ unsigned int *outputs;
    /* 0x000C */ class _RxPipelineCluster **slotClusterRefs;
    /* 0x0010 */ unsigned int *slotsContinue;
    /* 0x0014 */ void *privateData;
    /* 0x0018 */ unsigned int *inputToClusterSlot;
    /* 0x001C */ class _RxPipelineNodeTopSortData *topSortData;
};

// from viewskel.cpp
// size: 0x8
class _RxPipelineNodeParam {
    /* 0x0000 */ void *dataParam;
    /* 0x0004 */ class _RxHeap *heap;
};

// from viewskel.cpp
// size: 0x14
class _RxPipelineNodeTopSortData {
    /* 0x0000 */ unsigned int numIns;
    /* 0x0004 */ unsigned int numInsVisited;
    /* 0x0008 */ class _rxReq *req;
    /* 0x000C */ void *initialisationData;
    /* 0x0010 */ unsigned int initialisationDataSize;
};

// from viewskel.cpp
// size: 0xC
class _RxPipelineRequiresCluster {
    /* 0x0000 */ class _RxClusterDefinition *clusterDef;
    /* 0x0004 */ enum _RxClusterValidityReq rqdOrOpt;
    /* 0x0008 */ unsigned int slotIndex;
};

// from zCharacter.cpp
// size: 0xC
class _SkinData {
    /* 0x0000 */ class _RpSkin *skin;
    /* 0x0004 */ class _rpAtomic *atomic;
    /* 0x0008 */ float angle;
};

// from xPlayer.cpp
// size: 0x10
class _XBUTTON {
    /* 0x0000 */ unsigned int m_id;
    /* 0x0004 */ unsigned int m_nativeID;
    /* 0x0008 */ unsigned int m_nativeJoyID;
    /* 0x000C */ unsigned int m_down : 13 /* bit_offset 0 */;
    /* 0x000C */ unsigned int m_last_down : 13 /* bit_offset 13 */;
    /* 0x000C */ unsigned int m_pressed : 1 /* bit_offset 26 */;
    /* 0x000C */ unsigned int m_last_pressed : 1 /* bit_offset 27 */;
    /* 0x000C */ unsigned int m_released : 1 /* bit_offset 28 */;
    /* 0x000C */ unsigned int m_last_released : 1 /* bit_offset 29 */;
    /* 0x000C */ unsigned int unused : 2 /* bit_offset 30 */;
};

// from xMain.cpp
// size: 0x84
class _XDG {
    /* 0x0000 */ unsigned short idx;
    /* 0x0002 */ signed short value[51];
    /* 0x0068 */ float mult;
    /* 0x006C */ unsigned int graphcolor;
    /* 0x0070 */ signed int tickmin;
    /* 0x0074 */ signed int tickmax;
    /* 0x0078 */ signed int tickstep;
    /* 0x007C */ unsigned int tickcolor;
    /* 0x0080 */ unsigned int flags;
};

// from ios.cpp
// size: 0x10
class __locale_imp {
    /* 0x0000 */ class vector facets_;
    /* 0x000C */ class basic_string name_;
};

// from xAnimMgr.cpp
// size: 0x58
class __sFILE {
    /* 0x0000 */ unsigned char *_p;
    /* 0x0004 */ signed int _r;
    /* 0x0008 */ signed int _w;
    /* 0x000C */ signed short _flags;
    /* 0x000E */ signed short _file;
    /* 0x0010 */ class __sbuf _bf;
    /* 0x0018 */ signed int _lbfsize;
    /* 0x001C */ void *_cookie;
    /* 0x0020 */ signed int (*_read)(void *, char *, signed int);
    /* 0x0024 */ signed int (*_write)(void *, char *, signed int);
    /* 0x0028 */ signed long (*_seek)(void *, signed long, signed int);
    /* 0x002C */ signed int (*_close)(void *);
    /* 0x0030 */ class __sbuf _ub;
    /* 0x0038 */ unsigned char *_up;
    /* 0x003C */ signed int _ur;
    /* 0x0040 */ unsigned char _ubuf[3];
    /* 0x0043 */ unsigned char _nbuf[1];
    /* 0x0044 */ class __sbuf _lb;
    /* 0x004C */ signed int _blksize;
    /* 0x0050 */ signed int _offset;
    /* 0x0054 */ class _reent *_data;
};

// from xAnimMgr.cpp
// size: 0x8
class __sbuf {
    /* 0x0000 */ unsigned char *_base;
    /* 0x0004 */ signed int _size;
};

// from ios.cpp
// size: 0xC
class __vector_imp : public __vector_pod, public compile_assert {
};

// from ios.cpp
// size: 0xC
class __vector_pod {
    /* 0x0000 */ class compressed_pair capacity_;
    /* 0x0004 */ unsigned int size_;
    /* 0x0008 */ unsigned int *data_;
};

// from xScrFx.cpp
// size: 0xC
class __xScrFxMgr {
    /* 0x0000 */ unsigned short m_letterbox;
    /* 0x0002 */ unsigned char m_startFadeOut;
    /* 0x0003 */ unsigned char m_doneFadeOut;
    /* 0x0004 */ unsigned char m_startFadeIn;
    /* 0x0005 */ unsigned char m_doneFadeIn;
    /* 0x0006 */ unsigned short m_fadeOutCounter;
    /* 0x0008 */ unsigned short m_fadeInCounter;
    /* 0x000A */ signed short shake;
};

// from xAnimMgr.cpp
// size: 0x88
class _atexit {
    /* 0x0000 */ class _atexit *_next;
    /* 0x0004 */ signed int _ind;
    /* 0x0008 */ void (*_fns[32])();
};

// from xAnimMgr.cpp
// size: 0xC
class _glue {
    /* 0x0000 */ class _glue *_next;
    /* 0x0004 */ signed int _niobs;
    /* 0x0008 */ class __sFILE *_iobs;
};

// from iAsset.cpp
// size: 0x808
class _i3DObjInfo {
    /* 0x0000 */ unsigned int count;
    /* 0x0004 */ class _iGeomInfo geom[64];
    /* 0x0804 */ class tagP2Mesh *meshBuffer;
};

// from camera.cpp
// size: 0x4
class _iEntryPointRuntime {
    /* 0x0000 */ unsigned int m_enabled;
};

// from iAsset.cpp
// size: 0x20
class _iGeomInfo {
    /* 0x0000 */ unsigned int vertexCount;
    /* 0x0004 */ unsigned int jointAssignmentCount;
    /* 0x0008 */ unsigned int facesCount;
    /* 0x000C */ void *p2vertex;
    /* 0x0010 */ void *normals;
    /* 0x0014 */ void *vertex;
    /* 0x0018 */ void *assignments;
    /* 0x001C */ void *faces;
};

// from camera.cpp
// size: 0x74
class _iLightRuntime {
    /* 0x0000 */ unsigned char m_enabled;
    /* 0x0001 */ unsigned char m_dir;
    /* 0x0002 */ unsigned char m_mode;
    /* 0x0004 */ float m_falloff[3];
    /* 0x0010 */ float m_weight;
    /* 0x0014 */ signed short m_frameSpeed;
    /* 0x0016 */ signed short m_frameCount;
    /* 0x0018 */ float m_speed;
    /* 0x001C */ class iLightResource m_anchor[2];
};

// from iParMgr.cpp
// size: 0x28
class _iParRingMode {
    /* 0x0000 */ float ring_radius;
    /* 0x0004 */ float ring_radius_delta;
    /* 0x0008 */ float ring_tilt;
    /* 0x000C */ float ring_tilt_delta;
    /* 0x0010 */ float ring_height;
    /* 0x0014 */ float ring_height_delta;
    /* 0x0018 */ unsigned int ring_color[3];
    /* 0x0024 */ signed char ring_abr;
    /* 0x0025 */ signed char ring_flat;
    /* 0x0026 */ signed char ring_segs;
    /* 0x0027 */ signed char pad;
};

// from iParMgr.cpp
// size: 0x1C
class _iParSphereMode {
    /* 0x0000 */ float sphere_radius;
    /* 0x0004 */ float sphere_radius_delta;
    /* 0x0008 */ unsigned int sphere_color[3];
    /* 0x0014 */ signed int sphere_fade;
    /* 0x0018 */ signed char sphere_abr;
    /* 0x0019 */ unsigned char sphere_segs;
    /* 0x001A */ signed char sphere_tex;
    /* 0x001B */ unsigned char sphere_hemi;
};

// from iPlate.cpp
// size: 0xC
class _iPlateMaskResource {
    /* 0x0000 */ unsigned char columnStart;
    /* 0x0001 */ unsigned char scanStart;
    /* 0x0002 */ unsigned char columnWidth;
    /* 0x0003 */ unsigned char columnHeight;
    /* 0x0004 */ float depth;
    /* 0x0008 */ unsigned char *data;
};

// from iPlate.cpp
// size: 0x8
class _iPlateResource {
    /* 0x0000 */ unsigned int maskCount;
    /* 0x0004 */ class _iPlateMaskResource *masks;
};

// from xGlobals.cpp
// size: 0x10
class _iPlateRuntime {
    /* 0x0000 */ signed int nextFrame;
    /* 0x0004 */ signed int cineFrame;
    /* 0x0008 */ signed int dispFrame;
    /* 0x000C */ float cinez;
};

// from xState.cpp
// size: 0x14
class _iRenderEffects {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ unsigned char colorA[4];
    /* 0x0008 */ unsigned char colorB[4];
    /* 0x000C */ unsigned char litcolor[4];
    /* 0x0010 */ signed char abr;
    /* 0x0011 */ signed char abrB;
    /* 0x0012 */ unsigned char fadetime;
    /* 0x0013 */ unsigned char fadectr;
};

// from xSndMgr.cpp
// size: 0x50
class _iSFX {
    /* 0x0000 */ signed int m_idx;
    /* 0x0004 */ signed int m_wavIdx;
    /* 0x0008 */ unsigned int m_flags;
    /* 0x000C */ unsigned int m_soundPriority;
    /* 0x0010 */ void *m_callback;
    /* 0x0014 */ unsigned int m_callbackData;
    /* 0x0018 */ class Vector3D m_position;
    /* 0x0024 */ class Vector3D m_velocity;
    /* 0x0030 */ float m_radius;
    /* 0x0034 */ unsigned short m_playChunk;
    /* 0x0036 */ unsigned short m_playChunkCount;
    /* 0x0038 */ unsigned int m_volC[2];
    /* 0x0040 */ unsigned int m_vol;
    /* 0x0044 */ unsigned int m_pan;
    /* 0x0048 */ signed short m_loopCount;
    /* 0x004A */ unsigned short m_freq;
    /* 0x004C */ unsigned short m_pitch;
    /* 0x004E */ unsigned short m_userRef;
};

// from xState.cpp
// size: 0x14
class _iSFXResource {
    /* 0x0000 */ unsigned int m_id;
    /* 0x0004 */ unsigned int m_joint;
    /* 0x0008 */ unsigned char m_frame;
    /* 0x0009 */ unsigned char m_vol;
    /* 0x000A */ unsigned char m_freq;
    /* 0x000B */ unsigned char m_valid;
    /* 0x000C */ unsigned int m_sndID;
    /* 0x0010 */ unsigned char m_flag;
    /* 0x0011 */ unsigned char m_pad[3];
};

// from xTex.cpp
// size: 0x20
class _iTexResource {
    /* 0x0000 */ unsigned int m_assetID;
    /* 0x0004 */ unsigned int pad[3];
    /* 0x0010 */ class _PVRheader header;
};

// from xTex.cpp
// size: 0x8
class _iTexRuntime {
    /* 0x0000 */ void *surface;
    /* 0x0004 */ unsigned int m_flags;
};

// from xState.cpp
// size: 0x24
class _iVFXResource {
    /* 0x0000 */ unsigned short m_frame;
    /* 0x0002 */ unsigned char m_id;
    /* 0x0003 */ unsigned char m_valid;
    /* 0x0004 */ unsigned int m_joint;
    /* 0x0008 */ class Vector3D m_a;
    /* 0x0014 */ class Vector3D m_b;
    /* 0x0020 */ unsigned char m_flag;
    /* 0x0021 */ unsigned char m_pad[3];
};

// from xSndMgr.cpp
// size: 0x54
class _iWAV {
    /* 0x0000 */ unsigned char m_used;
    /* 0x0001 */ unsigned char m_bank;
    /* 0x0002 */ unsigned short m_pad;
    /* 0x0004 */ signed int m_idx;
    /* 0x0008 */ char m_name[32];
    /* 0x0028 */ unsigned int m_assetID;
    /* 0x002C */ unsigned int m_size;
    /* 0x0030 */ unsigned int m_samples;
    /* 0x0034 */ unsigned int m_age;
    /* 0x0038 */ unsigned int m_addr;
    /* 0x003C */ unsigned int m_sramsize;
    /* 0x0040 */ unsigned int m_freq;
    /* 0x0044 */ unsigned int m_type;
    /* 0x0048 */ unsigned int m_locked;
    /* 0x004C */ unsigned int m_pitch;
    /* 0x0050 */ class _iWAV *m_next;
};

// from xState.cpp
// size: 0x20
class _p2Geom {
    /* 0x0000 */ unsigned int m_meshCount;
    /* 0x0004 */ class tagP2Mesh *m_mesh;
    /* 0x0008 */ class _iRenderEffects effects;
    /* 0x001C */ class _RpClump *m_clump;
};

// from xAnimMgr.cpp
// size: 0x2EC
class _reent {
    /* 0x0000 */ signed int _errno;
    /* 0x0004 */ class __sFILE *_stdin;
    /* 0x0008 */ class __sFILE *_stdout;
    /* 0x000C */ class __sFILE *_stderr;
    /* 0x0010 */ signed int _inc;
    /* 0x0014 */ char _emergency[25];
    /* 0x0030 */ signed int _current_category;
    /* 0x0034 */ char *_current_locale;
    /* 0x0038 */ signed int __sdidinit;
    /* 0x003C */ void (*__cleanup)(class _reent *);
    /* 0x0040 */ class _Bigint *_result;
    /* 0x0044 */ signed int _result_k;
    /* 0x0048 */ class _Bigint *_p5s;
    /* 0x004C */ class _Bigint **_freelist;
    /* 0x0050 */ signed int _cvtlen;
    /* 0x0054 */ char *_cvtbuf;
    /* 0x0058 */ class @class _new;
    /* 0x0148 */ class _atexit *_atexit;
    /* 0x014C */ class _atexit _atexit0;
    /* 0x01D4 */ void (**_sig_func)(signed int);
    /* 0x01D8 */ class _glue __sglue;
    /* 0x01E4 */ class __sFILE __sf[3];
};

// from zCharacter.cpp
// size: 0xC
class _rpAnimInterpolator {
    /* 0x0000 */ signed int startKeyFrame;
    /* 0x0004 */ signed int endKeyFrame;
    /* 0x0008 */ float time;
};

// from viewskel.cpp
// size: 0x70
class _rpAtomic {
    /* 0x0000 */ class _RwObjectHasFrame object;
    /* 0x0014 */ class _rwResEntryTag *repEntry;
    /* 0x0018 */ class _RpGeometry *geometry;
    /* 0x001C */ class _RwSphere boundingSphere;
    /* 0x002C */ class _RwSphere worldBoundingSphere;
    /* 0x003C */ class _RpClump *clump;
    /* 0x0040 */ class _RwLLLink inClumpLink;
    /* 0x0048 */ class _rpAtomic * (*renderCallBack)(class _rpAtomic *);
    /* 0x004C */ class _rpInterpolator interpolator;
    /* 0x0060 */ unsigned short renderFrame;
    /* 0x0062 */ unsigned short collisionFrame;
    /* 0x0064 */ class _RwLinkList llWorldSectorsInAtomic;
    /* 0x006C */ class _RxPipeline *pipeline;
};

// from viewskel.cpp
// size: 0x14
class _rpInterpolator {
    /* 0x0000 */ signed int flags;
    /* 0x0004 */ signed short startMorphTarget;
    /* 0x0006 */ signed short endMorphTarget;
    /* 0x0008 */ float time;
    /* 0x000C */ float recipTime;
    /* 0x0010 */ float position;
};

// from viewskel.cpp
// size: 0x18
class _rwResEntryTag {
    /* 0x0000 */ class _RwLLLink link;
    /* 0x0008 */ signed int size;
    /* 0x000C */ void *owner;
    /* 0x0010 */ class _rwResEntryTag **ownerRef;
    /* 0x0014 */ void (*destroyNotify)(class _rwResEntryTag *);
};

// from viewskel.cpp
// size: 0x20
class _rxHeapBlockHeader {
    /* 0x0000 */ class _rxHeapBlockHeader *prev;
    /* 0x0004 */ class _rxHeapBlockHeader *next;
    /* 0x0008 */ unsigned int size;
    /* 0x000C */ class _rxHeapFreeBlock *freeEntry;
    /* 0x0010 */ unsigned int pad[4];
};

// from viewskel.cpp
// size: 0x8
class _rxHeapFreeBlock {
    /* 0x0000 */ unsigned int size;
    /* 0x0004 */ class _rxHeapBlockHeader *ptr;
};

// from viewskel.cpp
// size: 0xC
class _rxHeapSuperBlockDescriptor {
    /* 0x0000 */ void *start;
    /* 0x0004 */ unsigned int size;
    /* 0x0008 */ class _rxHeapSuperBlockDescriptor *next;
};

// from xPlayer.cpp
// size: 0x28
class _tagNativePadData {
    /* 0x0000 */ unsigned char status;
    /* 0x0001 */ unsigned int id : 4 /* bit_offset 0 */;
    /* 0x0001 */ unsigned int size : 4 /* bit_offset 4 */;
    /* 0x0002 */ unsigned short buttons;
    /* 0x0004 */ unsigned char analog[4];
    /* 0x0008 */ unsigned char pressure[12];
    /* 0x0014 */ unsigned char reserved[12];
    /* 0x0020 */ unsigned short last_buttons;
    /* 0x0022 */ unsigned short pressed;
    /* 0x0024 */ unsigned short released;
};

// from camera.cpp
// size: 0x14
class _tagiEnvRuntime {
    /* 0x0000 */ class _RpWorld *world;
    /* 0x0004 */ class Vector3D world_origin;
    /* 0x0010 */ class _RwTexDictionary *tex_dict;
};

// from camera.cpp
// size: 0x4
class _tagxEnv {
    /* 0x0000 */ class _tagiEnvRuntime *m_runtime;
};

// from xState.cpp
// size: 0x18
class _xActor {
    /* 0x0000 */ void *m_resource;
    /* 0x0004 */ class iActorRuntime *m_runtime;
    /* 0x0008 */ unsigned int m_flags;
    /* 0x000C */ class tagxStateMgr *m_state;
    /* 0x0010 */ unsigned short m_objectType;
    /* 0x0012 */ unsigned short m_actorID;
    /* 0x0014 */ void *m_texture;
};

// from xGui.cpp
// size: 0x60
class _xGuiItem {
    /* 0x0000 */ unsigned char m_idx;
    /* 0x0001 */ unsigned char m_scriptCreateIndex;
    /* 0x0002 */ signed short m_textOffset;
    /* 0x0004 */ unsigned int m_flag;
    /* 0x0008 */ unsigned int m_rgba;
    /* 0x000C */ unsigned int m_dataID;
    /* 0x0010 */ class _xGuiMaya m_img;
    /* 0x0028 */ void *m_edobjPad;
    /* 0x002C */ void *m_func[5];
    /* 0x0040 */ class _xGuiItem *m_dir[6];
    /* 0x0058 */ class _xGuiItem *m_next;
    /* 0x005C */ class _xGuiItem *m_prev;
};

// from xGui.cpp
// size: 0x18
class _xGuiMaya {
    /* 0x0000 */ unsigned int m_modelID;
    /* 0x0004 */ unsigned int m_texID;
    /* 0x0008 */ signed char m_partID;
    /* 0x0009 */ signed char m_val[3];
    /* 0x000C */ signed short m_off[2];
    /* 0x0010 */ signed short m_bbox[4];
};

// from xGui.cpp
// size: 0x3C
class _xGuiSys {
    /* 0x0000 */ unsigned char m_idx;
    /* 0x0001 */ unsigned char m_scriptCreateCount;
    /* 0x0002 */ unsigned char m_pad[2];
    /* 0x0004 */ unsigned int m_flag;
    /* 0x0008 */ signed short m_off[2];
    /* 0x000C */ void *m_data;
    /* 0x0010 */ class _xGuiItem *m_item;
    /* 0x0014 */ class _xGuiItem *m_select;
    /* 0x0018 */ class _xGuiSys *m_dir[6];
    /* 0x0030 */ void *m_edobjPad;
    /* 0x0034 */ void *m_func[2];
};

// from xObjDB.cpp
// size: 0x8
class _xModel {
    /* 0x0000 */ class tagiModelResource *m_resource;
    /* 0x0004 */ class tagiModelRuntime *m_runtime;
};

// from xScene.cpp
// size: 0x14
class _xObj {
    /* 0x0000 */ void *m_resource;
    /* 0x0004 */ class iObjRuntime *m_runtime;
    /* 0x0008 */ unsigned int m_modelID;
    /* 0x000C */ void *m_texture;
    /* 0x0010 */ signed short m_objType;
    /* 0x0012 */ signed short m_objID;
};

// from xSndMgr.cpp
// size: 0x33F8
class _xSndMgrData {
    /* 0x0000 */ class _iSFX m_voice[64];
    /* 0x1400 */ class _iWAV m_id[96];
    /* 0x3380 */ class Vector3D m_listenPos;
    /* 0x338C */ class Vector3D m_listenRot;
    /* 0x3398 */ unsigned char m_masterVolume;
    /* 0x3399 */ unsigned char m_isPaused;
    /* 0x339A */ unsigned char m_inStereo;
    /* 0x339B */ unsigned char m_pad;
    /* 0x339C */ float m_listenRad;
    /* 0x33A0 */ unsigned short m_pad2;
    /* 0x33A2 */ unsigned char m_listenTouch;
    /* 0x33A3 */ unsigned char m_initialized;
    /* 0x33A4 */ unsigned short m_lastRef;
    /* 0x33A6 */ unsigned char m_paused;
    /* 0x33A7 */ unsigned char m_activeBank;
    /* 0x33A8 */ unsigned int m_bankMaxAddr[5];
    /* 0x33BC */ unsigned int m_bankMinAddr[5];
    /* 0x33D0 */ unsigned int m_bankIdxAddr[5];
    /* 0x33E4 */ class _iWAV *m_bankList[5];
};

// from xTex.cpp
// size: 0x8
class _xTex {
    /* 0x0000 */ class _iTexResource *m_resource;
    /* 0x0004 */ class _iTexRuntime *m_runtime;
};

// from ios.cpp
// size: 0x48
class basic_ios : public ios_base {
    /* 0x0040 */ class basic_ostream *tiestr_;
    /* 0x0044 */ char fill_;
};

// from ios.cpp
// size: 0x68
class basic_istream : public basic_ios {
    /* 0x0008 */ signed long gcount_;
};

// from ios.cpp
// size: 0x58
class basic_ostream : public basic_ios {
};

// from ios.cpp
// size: 0x24
class basic_streambuf {
    /* 0x0004 */ char *gbeg_;
    /* 0x0008 */ char *gnext_;
    /* 0x000C */ char *gend_;
    /* 0x0010 */ char *pbeg_;
    /* 0x0014 */ char *pnext_;
    /* 0x0018 */ char *pend_;
    /* 0x001C */ class locale loc_;
};

// from ios.cpp
// size: 0x4
class basic_string {
    /* 0x0000 */ class _EmptyMemberOpt alloc_;
};

// from xActorDB.cpp
// size: 0x18
class charstate {
    /* 0x0000 */ class tagxState *m_states;
    /* 0x0004 */ signed int m_animationBasedMovement;
    /* 0x0008 */ void (*m_triggerHandler)(class _xActor *, signed int, class tagXAnim *, class tagXAnimTrigger *);
    /* 0x000C */ signed int m_initialized;
    /* 0x0010 */ void (*m_initFunction)();
    /* 0x0014 */ void (*m_resetFunction)();
};

// from ios.cpp
// size: 0x4
class compressed_pair : public compressed_pair_imp {
};

// from ios.cpp
// size: 0x4
class compressed_pair_imp : public allocator {
    /* 0x0000 */ unsigned int second_;
};

// from iAudioStream.cpp
// size: 0x10
class descriptor {
    /* 0x0000 */ class descriptor *l;
    /* 0x0004 */ class @class u;
    /* 0x0008 */ signed int pad1;
    /* 0x000C */ signed int pad2;
};

// from ios.cpp
// size: 0x8
class facet {
    /* 0x0004 */ unsigned int ref_;
};

// from ios.cpp
// size: 0xC
class failure : public exception {
    /* 0x0004 */ class _MSLstring what_;
};

// from camera.cpp
// size: 0x8
class iActionResource {
    /* 0x0000 */ unsigned char m_cmdType;
    /* 0x0001 */ unsigned char m_cmd;
    /* 0x0002 */ unsigned short pad;
    /* 0x0004 */ unsigned int m_cmdParameter;
};

// from camera.cpp
// size: 0x4
class iActionRuntime {
    /* 0x0000 */ class xAction *m_and;
};

// from xState.cpp
// size: 0x134
class iActorRuntime {
    /* 0x0000 */ class tagXAnimControl *m_active;
    /* 0x0004 */ signed int m_available;
    /* 0x0008 */ float m_fovr;
    /* 0x000C */ float m_bradius;
    /* 0x0010 */ unsigned char m_onMaterial[4];
    /* 0x0014 */ signed int m_routeID;
    /* 0x0018 */ signed int m_routePosition;
    /* 0x001C */ class xNavPoint *m_routeNavPoint;
    /* 0x0020 */ signed int m_health;
    /* 0x0024 */ signed int m_sightFOV;
    /* 0x0028 */ signed int m_sight;
    /* 0x002C */ signed int m_quiet;
    /* 0x0030 */ signed int m_canFly;
    /* 0x0034 */ signed int m_canStrafe;
    /* 0x0038 */ class Vector3D m_memPosition[5];
    /* 0x0074 */ signed int m_groupID;
    /* 0x0078 */ signed int m_groupIgnore;
    /* 0x007C */ class _p2Geom m_geom;
    /* 0x009C */ void *m_skeleton;
    /* 0x00A0 */ void *m_data;
    /* 0x00A4 */ class tagxPhysicsObject m_phys;
    /* 0x012C */ float m_lookat;
    /* 0x0130 */ signed int m_haveSpottedAsh;
};

// from zDemo.cpp
// size: 0x10
class iAnimData {
    /* 0x0000 */ class _RpSkinAnim *skin_anim;
    /* 0x0004 */ char *m_name;
    /* 0x0008 */ float duration;
};

// from zDemo.cpp
// size: 0x8
class iAnimInstance {
    /* 0x0000 */ float time;
};

// from iGeom.cpp
// size: 0x8C
class iCamera {
    /* 0x0000 */ signed int roll;
    /* 0x0004 */ class Vector3D look_at;
    /* 0x0010 */ class Vector3D look_from;
    /* 0x001C */ class Vector3D pos;
    /* 0x0028 */ class Vector3D view;
    /* 0x0034 */ class Vector3D up;
    /* 0x0040 */ class Vector3D target_pos;
    /* 0x004C */ float m[16];
};

// from camera.cpp
// size: 0x5C
class iCameraResource {
    /* 0x0000 */ unsigned short m_plateOffset;
    /* 0x0002 */ unsigned short m_versions;
    /* 0x0004 */ class p2CameraVersion m_version[5];
    /* 0x0054 */ unsigned int m_frameCount;
    /* 0x0058 */ class p2Camera *m_cameras;
};

// from camera.cpp
// size: 0xC
class iConditionResource {
    /* 0x0000 */ unsigned char m_osFlags;
    /* 0x0001 */ unsigned char m_onFlags;
    /* 0x0002 */ unsigned char m_whoFlags;
    /* 0x0003 */ unsigned char m_operator;
    /* 0x0004 */ unsigned char m_cmdType;
    /* 0x0005 */ unsigned char m_cmd;
    /* 0x0006 */ unsigned short pad;
    /* 0x0008 */ unsigned int m_cmdParameter;
};

// from camera.cpp
// size: 0x8
class iConditionRuntime {
    /* 0x0000 */ class xCondition *m_operand;
    /* 0x0004 */ unsigned short m_match;
    /* 0x0006 */ unsigned short m_trigger;
};

// from camera.cpp
// size: 0x1C
class iDisplayCamera {
    /* 0x0000 */ class Vector3D m_pos;
    /* 0x000C */ class Vector3D m_rot;
    /* 0x0018 */ float m_fov;
};

// from camera.cpp
// size: 0x48
class iEntryPointResource {
    /* 0x0000 */ unsigned char m_type;
    /* 0x0001 */ unsigned char m_geomType;
    /* 0x0002 */ unsigned char m_nextCameraID;
    /* 0x0003 */ unsigned char m_nextMarkerID;
    /* 0x0004 */ class Vector3D m_p[4];
    /* 0x0034 */ unsigned int m_floorMask;
    /* 0x0038 */ unsigned int m_nextSceneAssetID;
    /* 0x003C */ class Vector3D m_nextAng;
};

// from camera.cpp
// size: 0x2C
class iLightResource {
    /* 0x0000 */ unsigned int m_type;
    /* 0x0004 */ class Vector3D m_pos;
    /* 0x0010 */ class Vector3D m_rot;
    /* 0x001C */ class Vector3D m_color;
    /* 0x0028 */ signed int m_fov;
};

// from camera.cpp
// size: 0xC
class iMarkerResource {
    /* 0x0000 */ class Vector3D m_pos;
};

// from xScene.cpp
// size: 0x94
class iObjRuntime {
    /* 0x0000 */ signed short m_flag;
    /* 0x0002 */ signed short m_pad;
    /* 0x0004 */ class _p2Geom *m_geom;
    /* 0x0008 */ float m_bradius;
    /* 0x000C */ class tagxPhysicsObject m_phys;
};

// from camera.cpp
// size: 0x10
class iRuleResource {
    /* 0x0000 */ unsigned int m_actionCount;
    /* 0x0004 */ class xAction *m_action;
    /* 0x0008 */ unsigned int m_conditionCount;
    /* 0x000C */ class xCondition *m_condition;
};

// from camera.cpp
// size: 0x48
class iTriggerResource {
    /* 0x0000 */ unsigned char m_type;
    /* 0x0001 */ unsigned char m_geomType;
    /* 0x0002 */ unsigned char m_enabled;
    /* 0x0003 */ unsigned char pad;
    /* 0x0004 */ class Vector3D m_p[4];
    /* 0x0034 */ unsigned int m_ruleCount;
    /* 0x0038 */ class xRule *m_rule;
    /* 0x003C */ unsigned int m_floorMask;
    /* 0x0040 */ unsigned int m_flags;
    /* 0x0044 */ unsigned int m_cameraLink;
};

// from ios.cpp
// size: 0x40
class ios_base {
    /* 0x0004 */ class pair *cb_vec_;
    /* 0x0008 */ unsigned int cb_siz_;
    /* 0x000C */ unsigned int cb_cap_;
    /* 0x0010 */ signed long *iarray_;
    /* 0x0014 */ unsigned int isize_;
    /* 0x0018 */ void **parray_;
    /* 0x001C */ unsigned int psize_;
    /* 0x0020 */ class locale *loc_;
    /* 0x0024 */ void *rdbuf_;
    /* 0x0028 */ signed long precision_;
    /* 0x0030 */ signed long width_;
    /* 0x0038 */ enum fmtflags fmtflags_;
    /* 0x003A */ enum iostate iostate_;
    /* 0x003B */ enum iostate exceptions_;
};

// from ios.cpp
// size: 0x8
class locale {
    /* 0x0000 */ class _RefCountedPtr imp_;
};

// from camera.cpp
// size: 0x1C
class p2Camera {
    /* 0x0000 */ class Vector3D m_pos;
    /* 0x000C */ class Vector3D m_rot;
    /* 0x0018 */ float m_fov;
};

// from camera.cpp
// size: 0x10
class p2CameraVersion {
    /* 0x0000 */ unsigned short m_introStart;
    /* 0x0002 */ unsigned short m_loopStart;
    /* 0x0004 */ unsigned short m_loopEnd;
    /* 0x0006 */ unsigned short m_exitEnd;
    /* 0x0008 */ unsigned short m_flashFrame;
    /* 0x000A */ unsigned char m_introRate;
    /* 0x000B */ unsigned char m_loopRate;
    /* 0x000C */ unsigned char m_exitRate;
    /* 0x000D */ unsigned char m_flashRate;
    /* 0x000E */ unsigned short m_flags;
};

// from ios.cpp
// size: 0x8
class pair {
    /* 0x0000 */ void (*first)(enum event, class ios_base &, signed int);
    /* 0x0004 */ signed int second;
};

// from skyfs.cpp
// size: 0x2814
class skyFile {
    /* 0x0000 */ signed int gdfs;
    /* 0x0004 */ signed int POS;
    /* 0x0008 */ signed int SOF;
    /* 0x000C */ unsigned char readBuffer[10240];
    /* 0x280C */ unsigned int bufferPos;
    /* 0x2810 */ signed int bufferValid;
};

// from camera.cpp
// size: 0x40
class tagCollideInfo {
    /* 0x0000 */ class _tagxEnv *env;
    /* 0x0004 */ class Vector3D orig;
    /* 0x0010 */ class Vector3D direction;
    /* 0x001C */ float max_dist;
    /* 0x0020 */ unsigned int flags;
    /* 0x0024 */ class Vector3D hit_pos;
    /* 0x0030 */ float hit_distance;
    /* 0x0034 */ class Vector3D hit_normal;
};

// from xMain.cpp
// size: 0x10C
class tagFile {
    /* 0x0000 */ char m_name[256];
    /* 0x0100 */ signed int m_state;
    /* 0x0104 */ signed int m_desc;
    /* 0x0108 */ unsigned int m_fpos;
};

// from xSkel.cpp
// size: 0x40
class tagMATRIX {
    /* 0x0000 */ float _11;
    /* 0x0004 */ float _12;
    /* 0x0008 */ float _13;
    /* 0x000C */ float _14;
    /* 0x0010 */ float _21;
    /* 0x0014 */ float _22;
    /* 0x0018 */ float _23;
    /* 0x001C */ float _24;
    /* 0x0020 */ float _31;
    /* 0x0024 */ float _32;
    /* 0x0028 */ float _33;
    /* 0x002C */ float _34;
    /* 0x0030 */ float _41;
    /* 0x0034 */ float _42;
    /* 0x0038 */ float _43;
    /* 0x003C */ float _44;
};

// from xState.cpp
// size: 0x38
class tagP2Mesh {
    /* 0x0000 */ signed int m_vertexCount;
    /* 0x0004 */ class tagP2VERTEX *m_vertex;
    /* 0x0008 */ class tagP2VERTEX *m_neutralPose0;
    /* 0x000C */ class tagP2MeshVertex *m_neutralPose1;
    /* 0x0010 */ class tagP2Weight *m_joints;
    /* 0x0014 */ signed int m_indexCount;
    /* 0x0018 */ unsigned short *m_index;
    /* 0x001C */ signed int m_visible;
    /* 0x0020 */ class _FCinfo *fc;
    /* 0x0024 */ class Vector3D *fnorm;
    /* 0x0028 */ class Vector3D *m_vnorm;
    /* 0x002C */ class Vector4D *m_vtnorm;
    /* 0x0030 */ class tagiMesh *m_next;
    /* 0x0034 */ signed int m_normalsBuilt;
};

// from xState.cpp
// size: 0xC
class tagP2MeshVertex {
    /* 0x0000 */ float m_x;
    /* 0x0004 */ float m_y;
    /* 0x0008 */ float m_z;
};

// from xState.cpp
// size: 0x14
class tagP2VERTEX {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
    /* 0x000C */ float tu;
    /* 0x0010 */ float tv;
};

// from xState.cpp
// size: 0xC
class tagP2Weight {
    /* 0x0000 */ unsigned short m_joint0;
    /* 0x0002 */ unsigned short m_joint1;
    /* 0x0004 */ float m_weight0;
    /* 0x0008 */ float m_weight1;
};

// from xPad.cpp
// size: 0x278
class tagPadMgrData {
    /* 0x0000 */ unsigned short m_initialized;
    /* 0x0002 */ unsigned short m_numControllers;
    /* 0x0008 */ class tagxPad m_pad[2];
};

// from xParMgr.cpp
// size: 0x14
class tagParCmdList {
    /* 0x0000 */ signed int m_numCmd;
    /* 0x0004 */ class xParCmd *m_cmd;
    /* 0x0008 */ class xParCmd *m_lastCmd;
    /* 0x000C */ class tagParCmdList *m_prev;
    /* 0x0010 */ class tagParCmdList *m_next;
};

// from xParExe.cpp
// size: 0x18
class tagParCmd_Accelerate {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ class Vector3D m_acc;
    /* 0x0010 */ unsigned int m_freq;
    /* 0x0014 */ unsigned int m_freqTime;
};

// from xParExe.cpp
// size: 0x20
class tagParCmd_Bounce {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[2];
    /* 0x0003 */ unsigned char m_noStatic;
    /* 0x0004 */ class Vector3D m_plane;
    /* 0x0010 */ class Vector3D m_dampVelPercent;
    /* 0x001C */ unsigned int m_sndAssetID;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_CopyVertexB {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_Damping {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParCmd.cpp
// size: 0xC
class tagParCmd_DisableCmd {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ signed int m_countdownToDisable;
    /* 0x0008 */ class xParCmd *m_cmdToDisable;
};

// from xParExe.cpp
// size: 0x74
class tagParCmd_Emitter {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0002 */ unsigned short m_emitLifeRandom;
    /* 0x0004 */ unsigned char m_emitMode;
    /* 0x0008 */ signed int m_emitType;
    /* 0x000C */ signed int m_emitAmount;
    /* 0x0010 */ signed int m_emitAmountRangeMin;
    /* 0x0014 */ signed int m_emitAmountRangeMax;
    /* 0x0018 */ unsigned char m_emitCVel[4];
    /* 0x001C */ unsigned char m_emitColor[4];
    /* 0x0020 */ float m_emitSize;
    /* 0x0024 */ float m_emitSizeRangeMin;
    /* 0x0028 */ float m_emitSizeRangeMax;
    /* 0x002C */ float m_emitSizeVel;
    /* 0x0030 */ class Vector3D m_emitVel;
    /* 0x003C */ class Vector3D m_emitVelVar;
    /* 0x0048 */ class Vector3D m_origin;
    /* 0x0054 */ class Vector3D m_p1;
    /* 0x0060 */ class Vector3D m_p2;
    /* 0x006C */ class xParCmd *m_cmdTex;
    /* 0x0070 */ unsigned short m_delayCount;
    /* 0x0072 */ unsigned short m_delay;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_EnableCmd {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParCmd.cpp
// size: 0x20
class tagParCmd_Explosion {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ class Vector3D m_center;
    /* 0x0010 */ float m_velocity;
    /* 0x0014 */ float m_magnitude;
    /* 0x0018 */ float m_lifetime;
    /* 0x001C */ float m_age;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_Follow {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParExe.cpp
// size: 0x10
class tagParCmd_Friction {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ class Vector3D m_friction;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_GravitateCmd {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_Header {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParExe.cpp
// size: 0x28
class tagParCmd_Jet {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ class Vector3D m_center;
    /* 0x0010 */ class Vector3D m_acc;
    /* 0x001C */ float m_gravity;
    /* 0x0020 */ float m_epsilon;
    /* 0x0024 */ float m_maxRadiusSqr;
};

// from xParExe.cpp
// size: 0x1C
class tagParCmd_KillBox {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[2];
    /* 0x0003 */ unsigned char m_killOut;
    /* 0x0004 */ class Vector3D m_p[2];
};

// from xParCmd.cpp
// size: 0xC
class tagParCmd_KillCmd {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ signed int m_countdownToKill;
    /* 0x0008 */ class xParCmd *m_cmdToKill;
};

// from xParExe.cpp
// size: 0x4
class tagParCmd_KillInvis {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad;
    /* 0x0002 */ signed short m_ageMin;
};

// from xParExe.cpp
// size: 0x8
class tagParCmd_KillOld {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ signed int m_ageLimit;
};

// from xParExe.cpp
// size: 0x10
class tagParCmd_KillPlane {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[2];
    /* 0x0003 */ unsigned char m_killBelow;
    /* 0x0004 */ class Vector3D m_plane;
};

// from xParExe.cpp
// size: 0x10
class tagParCmd_KillSlow {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[2];
    /* 0x0003 */ unsigned char m_velAbs;
    /* 0x0004 */ class Vector3D m_velMin;
};

// from xParExe.cpp
// size: 0x10
class tagParCmd_LimitVelocity {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ class Vector3D m_maxVelocity;
};

// from xParExe.cpp
// size: 0x8
class tagParCmd_ModulateColor {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[2];
    /* 0x0003 */ unsigned char m_flags;
    /* 0x0004 */ signed short m_ageMin;
    /* 0x0006 */ signed short m_ageMax;
};

// from xParExe.cpp
// size: 0x8
class tagParCmd_Move {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ signed int m_dt;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_OrbitLine {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParExe.cpp
// size: 0x1C
class tagParCmd_OrbitPoint {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ class Vector3D m_center;
    /* 0x0010 */ float m_gravity;
    /* 0x0014 */ float m_epsilon;
    /* 0x0018 */ float m_maxRadiusSqr;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_RandomAccel {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParExe.cpp
// size: 0x10
class tagParCmd_RandomDisplace {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_minAge;
    /* 0x0002 */ unsigned char m_delayCount;
    /* 0x0003 */ unsigned char m_delay;
    /* 0x0004 */ class Vector3D m_vel;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_RandomVelocity {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_Restore {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParExe.cpp
// size: 0x4
class tagParCmd_RotMove {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_rotadjust[3];
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_Sink {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_SinkVelocity {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_Source {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParExe.cpp
// size: 0xC
class tagParCmd_SpawnVFX {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_vfxType;
    /* 0x0002 */ unsigned char m_mode;
    /* 0x0003 */ unsigned char m_delay;
    /* 0x0004 */ unsigned char m_delayCount;
    /* 0x0005 */ unsigned char m_spawnProbability;
    /* 0x0006 */ unsigned short m_maxAge;
    /* 0x0008 */ unsigned char m_displace[3];
    /* 0x000B */ unsigned char m_pad;
};

// from xParCmd.cpp
// size: 0xC
class tagParCmd_TargetColor {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
    /* 0x0004 */ unsigned char m_targetColor[4];
    /* 0x0008 */ unsigned char m_velocity[4];
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_TargetSize {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xParExe.cpp
// size: 0x34
class tagParCmd_Texture {
    /* 0x0000 */ float m_x1;
    /* 0x0004 */ float m_y1;
    /* 0x0008 */ float m_x2;
    /* 0x000C */ float m_y2;
    /* 0x0010 */ signed char m_texBirth;
    /* 0x0011 */ unsigned char m_texType;
    /* 0x0012 */ unsigned char m_texAnim;
    /* 0x0013 */ unsigned char m_texDir;
    /* 0x0014 */ class Vector3D m_minVel;
    /* 0x0020 */ signed short m_animCount;
    /* 0x0022 */ signed short m_animWait;
    /* 0x0024 */ float m_unitW;
    /* 0x0028 */ float m_unitH;
    /* 0x002C */ float m_unitRW;
    /* 0x0030 */ unsigned char m_pad;
    /* 0x0031 */ unsigned char m_unitCount;
    /* 0x0032 */ unsigned char m_columns;
    /* 0x0033 */ unsigned char m_rows;
};

// from xParExe.cpp
// size: 0x1C
class tagParCmd_Turbulence {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[2];
    /* 0x0003 */ unsigned char m_dirSinCos;
    /* 0x0004 */ float m_ageMin;
    /* 0x0008 */ float m_ageMax;
    /* 0x000C */ float m_lift;
    /* 0x0010 */ float m_max_swirl_radius;
    /* 0x0014 */ float m_currentSpin;
    /* 0x0018 */ float m_spinRate;
};

// from xParCmd.cpp
// size: 0x4
class tagParCmd_Vortex {
    /* 0x0000 */ unsigned char m_free;
    /* 0x0001 */ unsigned char m_pad[3];
};

// from xState.cpp
// size: 0x8
class tagXAnim {
    /* 0x0000 */ class tagiAnimResource *m_resource;
    /* 0x0004 */ class tagiAnimRuntime *m_runtime;
};

// from xState.cpp
// size: 0x30
class tagXAnimControl {
    /* 0x0000 */ unsigned short m_active;
    /* 0x0002 */ unsigned char m_state;
    /* 0x0003 */ unsigned char m_chainState;
    /* 0x0004 */ class tagXAnimControl *m_chainFrom;
    /* 0x0008 */ class tagXAnim *m_anim;
    /* 0x000C */ void *m_char;
    /* 0x0010 */ signed short m_prevFrame;
    /* 0x0012 */ signed short m_frame;
    /* 0x0014 */ signed short m_incr;
    /* 0x0016 */ signed short m_doneHandled;
    /* 0x0018 */ unsigned char *m_activeJoints;
    /* 0x001C */ class Vector3D m_startPosition;
    /* 0x0028 */ class tagXAnimControl *m_next;
    /* 0x002C */ unsigned char m_startEvent;
    /* 0x002D */ unsigned char m_endEvent;
    /* 0x002E */ unsigned char m_loopEvent;
    /* 0x002F */ unsigned char pad;
};

// from xAnim.cpp
// size: 0x14
class tagXAnimSet {
    /* 0x0000 */ unsigned int m_assetID;
    /* 0x0004 */ unsigned int m_animationCount;
    /* 0x0008 */ class tagXAnim *m_animations;
    /* 0x000C */ unsigned int m_readyCount;
    /* 0x0010 */ class tagXAnim *m_ready;
};

// from xState.cpp
// size: 0x8
class tagXAnimTrigger {
    /* 0x0000 */ unsigned short m_frame;
    /* 0x0002 */ unsigned short m_type;
    /* 0x0004 */ unsigned int m_data;
};

// from xTrigger.cpp
// size: 0x10
class tagXStreamDirectoryEntry {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ unsigned int type;
    /* 0x0008 */ unsigned int offset;
    /* 0x000C */ unsigned int length;
};

// from xTrigger.cpp
// size: 0x3E84
class tagXStreamMDSDirectory {
    /* 0x0000 */ unsigned int count;
    /* 0x0004 */ class tagXStreamDirectoryEntry directory[1000];
};

// from xScene.cpp
// size: 0x2C
class tag_xCameraControl {
    /* 0x0000 */ signed int m_sceneID;
    /* 0x0004 */ signed int m_camera;
    /* 0x0008 */ signed int m_version;
    /* 0x000C */ signed int m_command;
    /* 0x0010 */ signed int m_startLoc;
    /* 0x0014 */ signed int m_startFrame;
    /* 0x0018 */ signed int m_speed;
    /* 0x001C */ signed int m_endLoc;
    /* 0x0020 */ signed int m_endFrame;
    /* 0x0024 */ signed int m_actualFrame;
    /* 0x0028 */ signed int m_animFrame;
};

// from xState.cpp
// size: 0x18
class tagiAnimFrame {
    /* 0x0000 */ float *m_tx;
    /* 0x0004 */ float *m_ty;
    /* 0x0008 */ float *m_tz;
    /* 0x000C */ float *m_rx;
    /* 0x0010 */ float *m_ry;
    /* 0x0014 */ float *m_rz;
};

// from xState.cpp
// size: 0x28
class tagiAnimResource {
    /* 0x0000 */ unsigned int m_assetID;
    /* 0x0004 */ unsigned int m_flags;
    /* 0x0008 */ unsigned short m_loopStart;
    /* 0x000A */ unsigned short m_loopEnd;
    /* 0x000C */ unsigned short m_introStart;
    /* 0x000E */ unsigned short m_introEnd;
    /* 0x0010 */ unsigned short m_exitStart;
    /* 0x0012 */ unsigned short m_exitEnd;
    /* 0x0014 */ unsigned short m_frameCount;
    /* 0x0016 */ unsigned short m_jointCount;
    /* 0x0018 */ unsigned short m_vfxCount;
    /* 0x001A */ unsigned short m_sfxCount;
    /* 0x001C */ class tagiAnimFrame *m_joints;
    /* 0x0020 */ class _iVFXResource *m_vfx;
    /* 0x0024 */ class _iSFXResource *m_sfx;
};

// from xState.cpp
// size: 0x8
class tagiAnimRuntime {
    /* 0x0000 */ void *m_sfx;
    /* 0x0004 */ void *m_vfx;
};

// from xCart.cpp
// size: 0x538
class tagiCartControl {
    /* 0x0000 */ signed int port;
    /* 0x0004 */ enum p2CartActions action;
    /* 0x0008 */ enum p2CartStatus state;
    /* 0x000C */ signed int firstOp;
    /* 0x0010 */ signed int retry;
    /* 0x0014 */ char name[32];
    /* 0x0034 */ signed int status[8];
    /* 0x0054 */ signed int count;
    /* 0x0058 */ char names[24][16];
    /* 0x01D8 */ char descs[24][36];
};

// from camera.cpp
// size: 0x40
class tagiFloor {
    /* 0x0000 */ float m_x1;
    /* 0x0004 */ float m_y1;
    /* 0x0008 */ float m_z1;
    /* 0x000C */ float m_x2;
    /* 0x0010 */ float m_y2;
    /* 0x0014 */ float m_z2;
    /* 0x0018 */ unsigned int m_unit;
    /* 0x001C */ float m_w;
    /* 0x0020 */ float m_h;
    /* 0x0024 */ unsigned int m_vCount;
    /* 0x0028 */ unsigned int m_nCount;
    /* 0x002C */ unsigned int m_tCount;
    /* 0x0030 */ class Vector3D *m_v;
    /* 0x0034 */ class Vector3D *m_n;
    /* 0x0038 */ class tagiFloorTri *m_t;
    /* 0x003C */ unsigned int *m_tmask;
};

// from camera.cpp
// size: 0x8
class tagiFloorTri {
    /* 0x0000 */ unsigned short m_v[3];
    /* 0x0006 */ unsigned short m_n;
};

// from xSkel.cpp
// size: 0x50
class tagiJointResource {
    /* 0x0000 */ class tagMATRIX m_orientMatrix;
    /* 0x0040 */ class Vector3D m_home;
    /* 0x004C */ unsigned int m_parent;
};

// from xSkel.cpp
// size: 0xA4
class tagiJointRuntime {
    /* 0x0000 */ float m_tx;
    /* 0x0004 */ float m_ty;
    /* 0x0008 */ float m_tz;
    /* 0x000C */ float m_rx;
    /* 0x0010 */ float m_ry;
    /* 0x0014 */ float m_rz;
    /* 0x0018 */ class Vector3D m_worldHome;
    /* 0x0024 */ class tagMATRIX m_localMatrix;
    /* 0x0064 */ class tagMATRIX m_worldMatrix;
};

// from xObjDB.cpp
// size: 0xC
class tagiModelResource {
    /* 0x0000 */ unsigned int m_assetID;
    /* 0x0004 */ signed int m_meshCount;
    /* 0x0008 */ class tagP2Mesh *m_mesh;
};

// from xObjDB.cpp
// size: 0x4
class tagiModelRuntime {
    /* 0x0000 */ signed int placeholder;
};

// from xSkel.cpp
// size: 0xC
class tagiSkelResource {
    /* 0x0000 */ unsigned int m_assetID;
    /* 0x0004 */ unsigned int m_jointCount;
    /* 0x0008 */ class tagiJointResource *m_joints;
};

// from xSkel.cpp
// size: 0x4
class tagiSkelRuntime {
    /* 0x0000 */ class tagiJointRuntime *m_joints;
};

// from xState.cpp
// size: 0x96C
class tagxActorDBData {
    /* 0x0000 */ class _xActor m_ash[1];
    /* 0x0018 */ class _xActor m_skullBat[4];
    /* 0x0078 */ class _xActor m_deadite[4];
    /* 0x00D8 */ class _xActor m_treeBoss[1];
    /* 0x00F0 */ class _xActor m_skeleton[4];
    /* 0x0150 */ class _xActor m_troopKnife[4];
    /* 0x01B0 */ class _xActor m_troopClaw[4];
    /* 0x0210 */ class _xActor m_troopRange[4];
    /* 0x0270 */ class _xActor m_troopLeaderBoss[1];
    /* 0x0288 */ class _xActor m_hellbillyFat[4];
    /* 0x02E8 */ class _xActor m_hellbillyThin[4];
    /* 0x0348 */ class _xActor m_demonPig[4];
    /* 0x03A8 */ class _xActor m_matriarchBoss[1];
    /* 0x03C0 */ class _xActor m_grandma[1];
    /* 0x03D8 */ class _xActor m_allard[1];
    /* 0x03F0 */ class _xActor m_deadAshBoss[1];
    /* 0x0408 */ class _xActor m_annieZombieBoss[1];
    /* 0x0420 */ class _xActor m_badAsh[1];
    /* 0x0438 */ class _xActor m_jenny[1];
    /* 0x0450 */ class _xActor m_evilHand[1];
    /* 0x0468 */ class _xActor m_warriorCultist[4];
    /* 0x04C8 */ class _xActor m_guildMember[4];
    /* 0x0528 */ class _xActor m_merchant[4];
    /* 0x0588 */ class _xActor m_alzeez[1];
    /* 0x05A0 */ class _xActor m_wizardBoss[1];
    /* 0x05B8 */ class _xActor m_mudGolemBoss[1];
    /* 0x05D0 */ class _xActor m_armorySkeleton[4];
    /* 0x0630 */ class _xActor m_armoryBoss[1];
    /* 0x0648 */ class _xActor m_invisibleMen[4];
    /* 0x06A8 */ class _xActor m_netherPet[4];
    /* 0x0708 */ class _xActor m_elderBeast[2];
    /* 0x0738 */ class _xActor m_cthulhuBoss[1];
    /* 0x0750 */ class _xActor m_treeArm[1];
    /* 0x0768 */ class _xActor m_headlessSkeleton[1];
    /* 0x0780 */ class _xActor m_netherPet2[4];
    /* 0x07E0 */ class _xActor m_netherPet3[4];
    /* 0x0840 */ class _xActor m_captain[1];
    /* 0x0858 */ class _xActor m_pumpKeeper[1];
    /* 0x0870 */ class _xActor *m_p[40];
    /* 0x0910 */ signed char m_plimit[40];
    /* 0x0938 */ class _xActor *m_active[13];
};

// from xActorDB.cpp
// size: 0x354
class tagxActorDBMemMgr {
    /* 0x0000 */ class _xActor *usedBy;
    /* 0x0004 */ class _xModel model;
    /* 0x000C */ class xSkel skeleton;
    /* 0x0014 */ class iActorRuntime actor;
    /* 0x0148 */ class tagxStateMgr state;
};

// from xCart.cpp
// size: 0x578
class tagxCartControl {
    /* 0x0000 */ unsigned char state;
    /* 0x0001 */ unsigned char cart;
    /* 0x0002 */ unsigned char slot;
    /* 0x0003 */ unsigned char done;
    /* 0x0004 */ unsigned char ok;
    /* 0x0005 */ unsigned char wrongDisk;
    /* 0x0006 */ unsigned char formatError;
    /* 0x0007 */ unsigned char pad;
    /* 0x0008 */ unsigned char saveID[12];
    /* 0x0014 */ unsigned char description[32];
    /* 0x0034 */ class tagiCartControl platform;
    /* 0x056C */ unsigned char *dataBuffer;
    /* 0x0570 */ unsigned char *compressionBuffer;
    /* 0x0574 */ unsigned short dataBufferSize;
    /* 0x0576 */ unsigned short compressionBufferSize;
};

// from xAnim.cpp
// size: 0x4
class tagxFXEntry {
    /* 0x0000 */ class tagXAnim *m_anim;
};

// from xLook.cpp
// size: 0x4
class tagxLookTarget {
    /* 0x0000 */ class _xActor *m_actor;
    /* 0x0000 */ class _xObj *m_obj;
    /* 0x0000 */ class xMarker *m_marker;
    /* 0x0000 */ class Vector3D *m_vectorp;
};

// from xScene.cpp
// size: 0x288
class tagxObjDBData {
    /* 0x0000 */ class _xObj m_o[32];
    /* 0x0280 */ signed int m_staticObjects;
    /* 0x0284 */ signed int m_staticAssets;
};

// from xPlayer.cpp
// size: 0x138
class tagxPad {
    /* 0x0000 */ unsigned long m_lastTime;
    /* 0x0008 */ unsigned long m_currentTime;
    /* 0x0010 */ unsigned char m_active;
    /* 0x0011 */ unsigned char m_playerNumber;
    /* 0x0012 */ unsigned char m_controllerNumber;
    /* 0x0013 */ unsigned char m_deviceType;
    /* 0x0014 */ signed int m_padHandle;
    /* 0x0018 */ void *m_funcInit;
    /* 0x001C */ void *m_funcMapper;
    /* 0x0020 */ void *m_funcAction;
    /* 0x0024 */ class _tagNativePadData *m_pad;
    /* 0x0028 */ class _XBUTTON m_xbutton[16];
    /* 0x0128 */ unsigned char m_analogActive;
    /* 0x0129 */ signed char m_analogXValue;
    /* 0x012A */ signed char m_analogYValue;
    /* 0x012B */ signed int m_shock : 3 /* bit_offset 0 */;
    /* 0x012C */ unsigned char m_vib[6];
};

// from xState.cpp
// size: 0xC
class tagxPadAction {
    /* 0x0000 */ unsigned int m_action;
    /* 0x0004 */ unsigned short m_button;
    /* 0x0006 */ unsigned short m_status;
    /* 0x0008 */ unsigned char pad;
    /* 0x0009 */ unsigned char m_analogActive;
    /* 0x000A */ signed char m_analogXValue;
    /* 0x000B */ signed char m_analogYValue;
};

// from xParMgr.cpp
// size: 0x1C
class tagxParSys {
    /* 0x0000 */ unsigned int m_idx;
    /* 0x0004 */ class xPar *m_root;
    /* 0x0008 */ class xPar *m_dead;
    /* 0x000C */ signed int m_num_of_particles;
    /* 0x0010 */ class tagParCmdList *m_cmd;
    /* 0x0014 */ unsigned char m_alive;
    /* 0x0015 */ unsigned char m_killWhenDead;
    /* 0x0016 */ unsigned char m_active;
    /* 0x0017 */ unsigned char m_visible;
    /* 0x0018 */ unsigned char m_culled;
    /* 0x0019 */ unsigned char m_deadGlobalPool;
    /* 0x001A */ unsigned char m_pad[2];
};

// from xState.cpp
// size: 0x88
class tagxPhysicsObject {
    /* 0x0000 */ class Vector3D m_ang;
    /* 0x000C */ class Vector3D m_angVel;
    /* 0x0018 */ class Vector3D m_pos;
    /* 0x0024 */ class Vector3D m_vel;
    /* 0x0030 */ class Vector3D m_acc;
    /* 0x003C */ class Vector3D m_vel_inst;
    /* 0x0048 */ class Vector3D m_scale;
    /* 0x0054 */ class Vector3D m_motion;
    /* 0x0060 */ float m_cbradius;
    /* 0x0064 */ unsigned char m_flag;
    /* 0x0065 */ unsigned char m_hitResult;
    /* 0x0066 */ unsigned short m_shadow_radius;
    /* 0x0068 */ unsigned char m_type;
    /* 0x0069 */ unsigned char m_hitFlags;
    /* 0x006A */ unsigned short m_hitTriggerNoWalkID;
    /* 0x006C */ class Vector3D m_hitVector;
    /* 0x0078 */ unsigned char m_hitActorType;
    /* 0x0079 */ unsigned char m_hitTriggerWalkID;
    /* 0x007A */ unsigned char m_hitActorID;
    /* 0x007B */ unsigned char m_hitObjectID;
    /* 0x007C */ float m_floorHeight;
    /* 0x0080 */ unsigned int m_floorIdx;
    /* 0x0084 */ unsigned int m_floorMask;
};

// from xPlayer.cpp
// size: 0xC
class tagxPlayer {
    /* 0x0000 */ signed char m_move;
    /* 0x0001 */ signed char m_rotate;
    /* 0x0002 */ unsigned char m_run;
    /* 0x0003 */ unsigned char m_turnSpeed;
    /* 0x0004 */ unsigned char m_rightWeapon;
    /* 0x0005 */ unsigned char m_leftWeapon;
    /* 0x0006 */ unsigned short pad2;
    /* 0x0008 */ class _xActor *m_actor;
};

// from xState.cpp
// size: 0x18
class tagxState {
    /* 0x0000 */ unsigned int m_assetID;
    /* 0x0004 */ signed int m_animState;
    /* 0x0008 */ unsigned char *m_activeJoints;
    /* 0x000C */ signed short m_calloutNumber;
    /* 0x000E */ unsigned char m_enterEvent;
    /* 0x000F */ unsigned char m_exitEvent;
    /* 0x0010 */ signed int m_linkCount;
    /* 0x0014 */ class tagxStateLink *m_links;
};

// from xState.cpp
// size: 0x4
class tagxStateLink {
    /* 0x0000 */ unsigned char m_command;
    /* 0x0001 */ unsigned char m_flags;
    /* 0x0002 */ unsigned short m_state;
};

// from xState.cpp
// size: 0x20C
class tagxStateMgr {
    /* 0x0000 */ signed int m_animationBasedMovement;
    /* 0x0004 */ signed int m_player;
    /* 0x0008 */ class tagxState *m_states;
    /* 0x000C */ class tagxState *m_current;
    /* 0x0010 */ void (*m_triggerFunc)(class _xActor *, signed int, class tagXAnim *, class tagXAnimTrigger *);
    /* 0x0014 */ signed int m_threadActive[7];
    /* 0x0030 */ class tagxStateThread m_thread[7];
};

// from xState.cpp
// size: 0x44
class tagxStateThread {
    /* 0x0000 */ class tagxState *m_state;
    /* 0x0004 */ class tagXAnimControl *m_animCtl;
    /* 0x0008 */ signed int m_deferredWaiting;
    /* 0x000C */ class tagxPadAction m_deferred;
    /* 0x0018 */ signed int m_rbp;
    /* 0x001C */ class tagxState *m_ring[10];
};

// from xText.cpp
// size: 0x8
class tagxText {
    /* 0x0000 */ unsigned int assetId;
    /* 0x0004 */ char *string;
};

// from xAnimMgr.cpp
// size: 0x24
class tm {
    /* 0x0000 */ signed int tm_sec;
    /* 0x0004 */ signed int tm_min;
    /* 0x0008 */ signed int tm_hour;
    /* 0x000C */ signed int tm_mday;
    /* 0x0010 */ signed int tm_mon;
    /* 0x0014 */ signed int tm_year;
    /* 0x0018 */ signed int tm_wday;
    /* 0x001C */ signed int tm_yday;
    /* 0x0020 */ signed int tm_isdst;
};

// from ios.cpp
// size: 0xC
class vector : public __vector_imp {
};

// from camera.cpp
// size: 0x8
class xAction {
    /* 0x0000 */ class iActionResource *m_resource;
    /* 0x0004 */ class iActionRuntime *m_runtime;
};

// from zDemo.cpp
// size: 0x14
class xAnimData : public iAnimInstance {
    /* 0x0008 */ unsigned int m_assetID;
    /* 0x000C */ unsigned int m_ID;
    /* 0x0010 */ class iAnimData *m_resource;
};

// from zDemo.cpp
// size: 0x4C
class xAnimMgr : public xModule {
    /* 0x0030 */ class vector anims;
    /* 0x003C */ class vector ress;
    /* 0x0048 */ unsigned int ID_counter;
};

// from camera.cpp
// size: 0x8
class xCamera {
    /* 0x0000 */ class iCameraResource *m_resource;
    /* 0x0004 */ void **m_runtime;
};

// from i_prim.cpp
// size: 0x4
class xColor {
    /* 0x0000 */ class @class byte;
    /* 0x0000 */ unsigned int value;
};

// from camera.cpp
// size: 0x8
class xCondition {
    /* 0x0000 */ class iConditionResource *m_resource;
    /* 0x0004 */ class iConditionRuntime *m_runtime;
};

// from camera.cpp
// size: 0x8
class xEntryPoint {
    /* 0x0000 */ class iEntryPointResource *m_resource;
    /* 0x0004 */ class _iEntryPointRuntime *m_runtime;
};

// from zDemo.cpp
// size: 0xC
class xFSMData {
    /* 0x0000 */ unsigned short m_fsmType;
    /* 0x0002 */ unsigned short m_fsmState;
    /* 0x0004 */ unsigned short m_fsmNextState;
    /* 0x0008 */ signed int m_fsmForceChange;
};

// from xFont.cpp
// size: 0x44
class xFontBox_t {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ float x;
    /* 0x0008 */ float y;
    /* 0x000C */ float w;
    /* 0x0010 */ float h;
    /* 0x0014 */ unsigned int font;
    /* 0x0018 */ unsigned int color;
    /* 0x001C */ unsigned int saved_color;
    /* 0x0020 */ float x_spacing;
    /* 0x0024 */ float y_spacing;
    /* 0x0028 */ float depth;
    /* 0x002C */ float inset_left;
    /* 0x0030 */ float inset_right;
    /* 0x0034 */ float inset_top;
    /* 0x0038 */ float inset_bottom;
    /* 0x003C */ char *text;
    /* 0x0040 */ unsigned int backdrop_color;
};

// from xFont.cpp
// size: 0x98
class xFontInfo_t {
    /* 0x0000 */ unsigned char type;
    /* 0x0001 */ unsigned char id;
    /* 0x0002 */ unsigned short flags;
    /* 0x0004 */ unsigned short char_width;
    /* 0x0006 */ unsigned short char_height;
    /* 0x0008 */ unsigned short draw_width;
    /* 0x000A */ unsigned short draw_height;
    /* 0x000C */ unsigned short tex_width;
    /* 0x000E */ unsigned short tex_height;
    /* 0x0010 */ unsigned short layout;
    /* 0x0012 */ unsigned short pad;
    /* 0x0014 */ unsigned char lookup[128];
    /* 0x0094 */ char *characters;
};

// from zDemo.cpp
// size: 0xCC
class xGameCharacter : public xGameObj {
};

// from zDemo.cpp
// size: 0xCC
class xGameObj {
    /* 0x0000 */ unsigned int m_objFlags;
    /* 0x0004 */ unsigned int m_objID;
    /* 0x0008 */ unsigned int m_objType;
    /* 0x000C */ class xFSMData m_fsmData;
    /* 0x0018 */ class _p2Geom m_geom;
    /* 0x0038 */ class xGameObj *m_next;
    /* 0x003C */ class xGameObj *m_prev;
    /* 0x0040 */ class tagxPhysicsObject m_phys;
};

// from zDemo.cpp
// size: 0x34
class xGameObjMgr : public xModule {
    /* 0x0030 */ class xGameObj *m_list;
};

// from camera.cpp
// size: 0x8
class xLight {
    /* 0x0000 */ class iLightResource *m_resource;
    /* 0x0004 */ class _iLightRuntime *m_runtime;
};

// from xTrigger.cpp
// size: 0x24
class xLine2D {
    /* 0x0000 */ class _RwV2d m_p[2];
    /* 0x0010 */ class _RwV2d m_i[2];
    /* 0x0020 */ signed int m_iCount;
};

// from xRouteDB.cpp
// size: 0x8
class xList {
    /* 0x0000 */ class xListItem *m_head;
    /* 0x0004 */ class xListItem *m_tail;
};

// from xRouteDB.cpp
// size: 0xC
class xListItem {
    /* 0x0000 */ void *m_item;
    /* 0x0004 */ class xListItem *m_next;
    /* 0x0008 */ class xListItem *m_prev;
};

// from xLook.cpp
// size: 0x10
class xLook {
    /* 0x0000 */ unsigned short m_decay;
    /* 0x0002 */ unsigned short m_decayCount;
    /* 0x0004 */ unsigned char m_priority;
    /* 0x0005 */ unsigned char m_flag;
    /* 0x0006 */ unsigned short m_wait;
    /* 0x0008 */ float m_distance;
    /* 0x000C */ class tagxLookTarget m_t;
};

// from xLook.cpp
// size: 0x128
class xLookMgr {
    /* 0x0000 */ unsigned char m_hasInterest;
    /* 0x0001 */ unsigned char m_lastSlot;
    /* 0x0002 */ unsigned short m_pad;
    /* 0x0004 */ class Vector3D m_lastPoint;
    /* 0x0010 */ class tagxPhysicsObject *m_lookFrom;
    /* 0x0014 */ class Vector3D m_lookFromP;
    /* 0x0020 */ float m_lastLookDelta;
    /* 0x0024 */ float m_lastLookDeltaY;
    /* 0x0028 */ class xLook m_look[16];
};

// from camera.cpp
// size: 0x8
class xMarker {
    /* 0x0000 */ class iMarkerResource *m_resource;
    /* 0x0004 */ void **m_runtime;
};

// from xPlayer.cpp
// size: 0x2C
class xMessage {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ signed int senderType;
    /* 0x0008 */ signed int senderId;
    /* 0x000C */ signed int recipientType;
    /* 0x0010 */ signed int recipientId;
    /* 0x0014 */ unsigned int message;
    /* 0x0018 */ unsigned int data1;
    /* 0x001C */ unsigned int data2;
    /* 0x0020 */ unsigned int data3;
    /* 0x0024 */ unsigned int data4;
    /* 0x0028 */ float deliveryTime;
};

// from xModuleFactory.cpp
// size: 0x30
class xModule {
    /* 0x0000 */ signed int m_type;
    /* 0x0004 */ char m_name[32];
    /* 0x0024 */ signed int m_init;
    /* 0x0028 */ signed int m_state;
};

// from camera.cpp
// size: 0x20
class xNavLink {
    /* 0x0000 */ unsigned char m_flags;
    /* 0x0001 */ unsigned char m_countLocked;
    /* 0x0002 */ unsigned char m_countOccupied;
    /* 0x0003 */ unsigned char m_unused2;
    /* 0x0004 */ unsigned int m_unused;
    /* 0x0008 */ float m_distance;
    /* 0x000C */ class Vector3D m_dir;
    /* 0x0018 */ class xNavPoint *m_linkTo;
    /* 0x001C */ class xNavPoint *m_linkFrom;
};

// from camera.cpp
// size: 0x2C
class xNavPoint {
    /* 0x0000 */ unsigned int m_linkCount;
    /* 0x0004 */ class xNavLink *m_link;
    /* 0x0008 */ class Vector3D m_pos;
    /* 0x0014 */ float m_cost;
    /* 0x0018 */ float m_heuristic;
    /* 0x001C */ float m_f;
    /* 0x0020 */ class xNavPoint *m_parent;
    /* 0x0024 */ unsigned short m_flags;
    /* 0x0026 */ unsigned short m_occupiedCount;
    /* 0x0028 */ unsigned int m_unused;
};

// from xParMgr.cpp
// size: 0x5C
class xPar {
    /* 0x0000 */ class xPar *m_next;
    /* 0x0004 */ class xPar *m_prev;
    /* 0x0008 */ class Vector3D m_pos;
    /* 0x0014 */ class Vector3D m_vel;
    /* 0x0020 */ class Vector3D m_acc;
    /* 0x002C */ float m_size;
    /* 0x0030 */ float m_sizeVel;
    /* 0x0034 */ signed short m_lifetime;
    /* 0x0036 */ unsigned char m_flag;
    /* 0x0037 */ unsigned char m_mode;
    /* 0x0038 */ unsigned char m_c[4];
    /* 0x003C */ signed char m_cvel[4];
    /* 0x0040 */ unsigned char m_c1[4];
    /* 0x0044 */ unsigned char m_c2[4];
    /* 0x0048 */ unsigned char m_rotdeg[3];
    /* 0x004B */ unsigned char m_texIdx;
    /* 0x004C */ float m_uva[2];
    /* 0x0054 */ float m_uvb[2];
};

// from xParMgr.cpp
// size: 0x14
class xParCmd {
    /* 0x0000 */ unsigned char m_tag;
    /* 0x0001 */ unsigned char m_flag;
    /* 0x0002 */ signed short m_count;
    /* 0x0004 */ void *m_method;
    /* 0x0008 */ void *m_data;
    /* 0x000C */ class xParCmd *m_next;
    /* 0x0010 */ class xParCmd *m_prev;
};

// from xPathfdr.cpp
// size: 0xE4
class xPathfinder {
    /* 0x0000 */ class xSearch m_search[4];
    /* 0x00D0 */ class xScene *m_scene;
    /* 0x00D4 */ class xList m_open;
    /* 0x00DC */ class xList m_closed;
};

// from xGlobals.cpp
// size: 0xC
class xPlate {
    /* 0x0000 */ signed int m_initialized;
    /* 0x0004 */ signed int m_currentIndex;
    /* 0x0008 */ class _iPlateRuntime *m_runtime;
};

// from xRouteDB.cpp
// size: 0x64
class xRoute {
    /* 0x0000 */ signed int m_physType;
    /* 0x0004 */ signed int m_flags;
    /* 0x0008 */ signed int m_status;
    /* 0x000C */ class xScene *m_scene;
    /* 0x0010 */ class Vector3D m_pointA;
    /* 0x001C */ class Vector3D m_pointB;
    /* 0x0028 */ class xNavPoint *m_npA;
    /* 0x002C */ class xNavPoint *m_npB;
    /* 0x0030 */ class xList m_list;
    /* 0x0038 */ class xSearch *m_search;
    /* 0x003C */ class xRouteTarget m_target;
    /* 0x0058 */ class xRouteUseage m_useage;
};

// from xRouteDB.cpp
// size: 0x44
class xRouteDB {
    /* 0x0000 */ class xRoute *m_route[16];
    /* 0x0040 */ class xScene *m_scene;
};

// from xRouteDB.cpp
// size: 0x1C
class xRouteTarget {
    /* 0x0000 */ signed char m_type;
    /* 0x0001 */ unsigned char m_hasLOSToTarget;
    /* 0x0002 */ unsigned char m_pad2[2];
    /* 0x0004 */ class Vector3D m_pos;
    /* 0x0010 */ float m_rerouteThreshold;
    /* 0x0014 */ class @class t;
};

// from xRouteDB.cpp
// size: 0x8
class xRouteTargetActor {
    /* 0x0000 */ signed int m_type;
    /* 0x0004 */ signed int m_id;
};

// from xRouteDB.cpp
// size: 0x4
class xRouteTargetMarker {
    /* 0x0000 */ signed int m_id;
};

// from xRouteDB.cpp
// size: 0x4
class xRouteTargetNavPoint {
    /* 0x0000 */ signed int m_id;
};

// from xRouteDB.cpp
// size: 0x4
class xRouteTargetObject {
    /* 0x0000 */ signed int m_id;
};

// from xRouteDB.cpp
// size: 0x4
class xRouteTargetTrigger {
    /* 0x0000 */ signed int m_id;
};

// from xRouteDB.cpp
// size: 0xC
class xRouteUseage {
    /* 0x0000 */ signed int m_type;
    /* 0x0004 */ signed int m_dir;
    /* 0x0008 */ class @class u;
};

// from xRouteDB.cpp
// size: 0x4
class xRouteUseageOneShot {
    /* 0x0000 */ signed int m_unused;
};

// from xRouteDB.cpp
// size: 0x4
class xRouteUseagePatrol {
    /* 0x0000 */ signed int m_patrolDir;
};

// from camera.cpp
// size: 0x8
class xRule {
    /* 0x0000 */ class iRuleResource *m_resource;
    /* 0x0004 */ void *m_runtime;
};

// from camera.cpp
// size: 0x4A8
class xScene {
    /* 0x0000 */ unsigned int m_sceneId;
    /* 0x0004 */ unsigned int m_cameraCount;
    /* 0x0008 */ class xCamera *m_camera;
    /* 0x000C */ class iDisplayCamera m_displayCamera;
    /* 0x0028 */ unsigned int m_lightCount;
    /* 0x002C */ class xLight *m_light;
    /* 0x0030 */ unsigned int m_triggerCount;
    /* 0x0034 */ class xTrigger *m_trigger;
    /* 0x0038 */ unsigned int m_triggerNoWalkCount;
    /* 0x003C */ class xTrigger **m_triggerNoWalk;
    /* 0x0040 */ unsigned int *m_triggerNoWalkFloorMask;
    /* 0x0044 */ unsigned int m_triggerWalkCount;
    /* 0x0048 */ class xTrigger **m_triggerWalk;
    /* 0x004C */ unsigned int m_triggerGenericCount;
    /* 0x0050 */ class xTrigger **m_triggerGeneric;
    /* 0x0054 */ unsigned int *m_triggerGenericFloorMask;
    /* 0x0058 */ unsigned int *m_triggerGenericGameEventFlags;
    /* 0x005C */ unsigned int m_triggerConditionCount;
    /* 0x0060 */ class xCondition *m_triggerCondition;
    /* 0x0064 */ unsigned int m_triggerEffectCount;
    /* 0x0068 */ class xTrigger **m_triggerEffect;
    /* 0x006C */ unsigned int m_triggerMaterialCount;
    /* 0x0070 */ class xTrigger **m_triggerMaterial;
    /* 0x0074 */ unsigned int m_markerCount;
    /* 0x0078 */ class xMarker *m_marker;
    /* 0x007C */ unsigned int m_entryPointCount;
    /* 0x0080 */ class xEntryPoint *m_entryPoint;
    /* 0x0084 */ unsigned int m_navPointCount;
    /* 0x0088 */ class xNavPoint *m_navPoint;
    /* 0x008C */ class tagiFloor *m_floor;
    /* 0x0090 */ signed int m_cullPlayerCount;
    /* 0x0094 */ class iTriggerResource *m_cullPlayer[20];
    /* 0x00E4 */ signed int m_cullNPCCount;
    /* 0x00E8 */ class iTriggerResource *m_cullNPC[20];
    /* 0x0138 */ signed int m_cullRTObjCount;
    /* 0x013C */ class iTriggerResource *m_cullRTObj[20];
    /* 0x018C */ signed int m_cullVFXCount;
    /* 0x0190 */ class iTriggerResource *m_cullVFX[20];
    /* 0x01E0 */ signed int m_cullSFXCount;
    /* 0x01E4 */ class iTriggerResource *m_cullSFX[20];
    /* 0x0234 */ signed int m_cullLightCount;
    /* 0x0238 */ class iTriggerResource *m_cullLight[20];
    /* 0x0288 */ signed int m_visPlayerCount;
    /* 0x028C */ class iTriggerResource *m_visPlayer[20];
    /* 0x02DC */ signed int m_visNPCCount;
    /* 0x02E0 */ class iTriggerResource *m_visNPC[20];
    /* 0x0330 */ signed int m_visRTObjCount;
    /* 0x0334 */ class iTriggerResource *m_visRTObj[20];
    /* 0x0384 */ signed int m_visVFXCount;
    /* 0x0388 */ class iTriggerResource *m_visVFX[20];
    /* 0x03D8 */ signed int m_visSFXCount;
    /* 0x03DC */ class iTriggerResource *m_visSFX[20];
    /* 0x042C */ signed int m_visLightCount;
    /* 0x0430 */ class iTriggerResource *m_visLight[20];
    /* 0x0480 */ class Vector3D m_ambientLight;
    /* 0x048C */ class Vector3D m_sunColor;
    /* 0x0498 */ class Vector3D m_sunDir;
    /* 0x04A4 */ class _tagxEnv m_env;
};

// from xRouteDB.cpp
// size: 0x34
class xSearch {
    /* 0x0000 */ signed char m_id;
    /* 0x0001 */ unsigned char m_method;
    /* 0x0002 */ unsigned char m_status;
    /* 0x0003 */ unsigned char m_flags;
    /* 0x0004 */ unsigned int m_startTime;
    /* 0x0008 */ class xNavPoint *m_start;
    /* 0x000C */ unsigned int m_finishTime;
    /* 0x0010 */ class xNavPoint *m_finish;
    /* 0x0014 */ class xNavPoint *m_goal;
    /* 0x0018 */ class xSearchPoint *m_finishSearchPoint;
    /* 0x001C */ unsigned int m_passes;
    /* 0x0020 */ class xList m_open;
    /* 0x0028 */ class xList m_closed;
    /* 0x0030 */ signed char m_waitForSearch;
    /* 0x0031 */ unsigned char m_pad[3];
};

// from xRouteDB.cpp
// size: 0x18
class xSearchPoint {
    /* 0x0000 */ signed char m_searchID;
    /* 0x0001 */ unsigned char m_depth;
    /* 0x0002 */ unsigned short m_pad;
    /* 0x0004 */ float m_f;
    /* 0x0008 */ float m_cost;
    /* 0x000C */ float m_heuristic;
    /* 0x0010 */ class xNavPoint *m_np;
    /* 0x0014 */ class xSearchPoint *m_parent;
};

// from xSkel.cpp
// size: 0x8
class xSkel {
    /* 0x0000 */ class tagiSkelResource *m_resource;
    /* 0x0004 */ class tagiSkelRuntime *m_runtime;
};

// from xSpanBuf.cpp
// size: 0x1C
class xSpan {
    /* 0x0000 */ float m_a;
    /* 0x0004 */ float m_b;
    /* 0x0008 */ float m_a2;
    /* 0x000C */ float m_b2;
    /* 0x0010 */ signed int m_userData;
    /* 0x0014 */ class xSpan *m_next;
    /* 0x0018 */ class xSpan *m_prev;
};

// from iPhyObj.cpp
// size: 0x24
class xSpanBuffer {
    /* 0x0000 */ class xSpan *m_head;
    /* 0x0004 */ class xSpan *m_tail;
    /* 0x0008 */ class xSpan m_span[1];
};

// from camera.cpp
// size: 0x4
class xTrigger {
    /* 0x0000 */ class iTriggerResource *m_resource;
};

// from zDemo.cpp
// size: 0xFC
class zCharacter : public xGameCharacter {
    /* 0x00CC */ signed int chris_hack_value;
    /* 0x00D0 */ signed int snack_delay;
    /* 0x00D4 */ signed int m_currentState;
    /* 0x00D8 */ signed int m_jumping;
    /* 0x00DC */ signed int m_doublejumping;
    /* 0x00E0 */ float m_gradient;
    /* 0x00E4 */ signed int m_player;
    /* 0x00E8 */ unsigned int current_animation_ID;
    /* 0x00EC */ class xAnimData *current_anim;
    /* 0x00F0 */ signed int m_health;
    /* 0x00F4 */ float m_storedHeight;
    /* 0x00F8 */ signed int m_resetTimer;
};

// from zMain.cpp
// size: 0x17C
class zDemo : public xModule {
    /* 0x0030 */ class zCharacter *m_chars;
    /* 0x0034 */ class zCharacter *m_snacks[40];
    /* 0x00D4 */ class zCharacter *m_grass[41];
    /* 0x0178 */ signed int m_lastCamera;
};

// from zGameObj.cpp
// size: 0xCC
class zGameObj : public xGameObj {
};

// from zGameObjMgr.cpp
// size: 0x34
class zGameObjMgr : public xGameObjMgr {
};

// from zGameObjVillain.cpp
// size: 0xF8
class zGameObjVillain : public xGameCharacter {
    /* 0x00CC */ signed int chris_hack_value;
    /* 0x00D0 */ signed int snack_delay;
    /* 0x00D4 */ signed int m_currentStateTime;
    /* 0x00D8 */ signed int m_currentState;
    /* 0x00DC */ signed int m_jumping;
    /* 0x00E0 */ signed int m_doublejumping;
    /* 0x00E4 */ float m_gradient;
    /* 0x00E8 */ unsigned int current_animation_ID;
    /* 0x00EC */ class xAnimData *current_anim;
    /* 0x00F0 */ signed int m_health;
    /* 0x00F4 */ float m_storedHeight;
};

// from zMain.cpp
// size: 0x3C
class zMain : public xModule {
    /* 0x0030 */ class zDemo *m_modDemo;
    /* 0x0034 */ class xModule *m_gameObjMgr;
    /* 0x0038 */ class xModule *m_gameAnimMgr;
};
