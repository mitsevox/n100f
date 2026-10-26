// Type definitions recovered from DWARF 1 (.debug).
// One entry per distinct definition; '// from' names the first
// compile unit that described it. Offsets are member offsets.

// from iCollide.cpp
enum Dimension {
    ZDIM = 2,
    YDIM = 1,
    XDIM = 0
};

// from iFile.cpp
enum IFILE_READSECTOR_STATUS {
    IFILE_RDSTAT_FAIL = 3,
    IFILE_RDSTAT_DONE = 2,
    IFILE_RDSTAT_INPROG = 1,
    IFILE_RDSTAT_NOOP = 0
};

// from iCollide.cpp
enum RpIntersectType {
    rpINTERSECTTYPEFORCEENUMSIZEINT = 2147483647,
    rpINTERSECTATOMIC = 5,
    rpINTERSECTBOX = 4,
    rpINTERSECTSPHERE = 3,
    rpINTERSECTPOINT = 2,
    rpINTERSECTLINE = 1,
    rpINTERSECTNONE = 0
};

// from xFX.cpp
enum RpMatFXMaterialFlags {
    rpMATFXFORCEENUMSIZEINT = 2147483647,
    rpMATFXNUMEFFECTS = 4,
    rpMATFXEFFECTMAX = 5,
    rpMATFXEFFECTDUAL = 4,
    rpMATFXEFFECTBUMPENVMAP = 3,
    rpMATFXEFFECTENVMAP = 2,
    rpMATFXEFFECTBUMPMAP = 1,
    rpMATFXEFFECTNULL = 0
};

// from p2im3dhack.cpp
enum RpMeshHeaderFlags {
    rpMESHHEADERFLAGSFORCEENUMSIZEINT = 2147483647,
    rpMESHHEADERUNINDEXED = 256,
    rpMESHHEADERPRIMMASK = 255,
    rpMESHHEADERPOINTLIST = 16,
    rpMESHHEADERPOLYLINE = 8,
    rpMESHHEADERLINELIST = 4,
    rpMESHHEADERTRIFAN = 2,
    rpMESHHEADERTRISTRIP = 1
};

// from iMorph.cpp
enum RpUserDataFormat {
    rpSTRINGUSERDATA = 3,
    rpREALUSERDATA = 2,
    rpINTUSERDATA = 1,
    rpNAUSERDATAFORMAT = 0
};

// from xScrFx.cpp
enum RpWorldRenderOrder {
    rpWORLDRENDERORDERFORCEENUMSIZEINT = 2147483647,
    rpWORLDRENDERBACK2FRONT = 2,
    rpWORLDRENDERFRONT2BACK = 1,
    rpWORLDRENDERNARENDERORDER = 0
};

// from iFont.cpp
enum RwBlendFunction {
    rwBLENDFUNCTIONFORCEENUMSIZEINT = 2147483647,
    rwBLENDSRCALPHASAT = 11,
    rwBLENDINVDESTCOLOR = 10,
    rwBLENDDESTCOLOR = 9,
    rwBLENDINVDESTALPHA = 8,
    rwBLENDDESTALPHA = 7,
    rwBLENDINVSRCALPHA = 6,
    rwBLENDSRCALPHA = 5,
    rwBLENDINVSRCCOLOR = 4,
    rwBLENDSRCCOLOR = 3,
    rwBLENDONE = 2,
    rwBLENDZERO = 1,
    rwBLENDNABLEND = 0
};

// from xScrFx.cpp
enum RwCameraProjection {
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = 2147483647,
    rwPARALLEL = 2,
    rwPERSPECTIVE = 1,
    rwNACAMERAPROJECTION = 0
};

// from xScrFx.cpp
enum RwFogType {
    rwFOGTYPEFORCEENUMSIZEINT = 2147483647,
    rwFOGTYPEEXPONENTIAL2 = 3,
    rwFOGTYPEEXPONENTIAL = 2,
    rwFOGTYPELINEAR = 1,
    rwFOGTYPENAFOGTYPE = 0
};

// from iFX.cpp
enum RwFrustumTestResult {
    rwFRUSTUMTESTRESULTFORCEENUMSIZEINT = 2147483647,
    rwSPHEREINSIDE = 2,
    rwSPHEREBOUNDARY = 1,
    rwSPHEREOUTSIDE = 0
};

// from p2im3dhack.cpp
enum RwPrimitiveType {
    rwPRIMITIVETYPEFORCEENUMSIZEINT = 2147483647,
    rwPRIMTYPEPOINTLIST = 6,
    rwPRIMTYPETRIFAN = 5,
    rwPRIMTYPETRISTRIP = 4,
    rwPRIMTYPETRILIST = 3,
    rwPRIMTYPEPOLYLINE = 2,
    rwPRIMTYPELINELIST = 1,
    rwPRIMTYPENAPRIMTYPE = 0
};

// from iFont.cpp
enum RwShadeMode {
    rwSHADEMODEFORCEENUMSIZEINT = 2147483647,
    rwSHADEMODEGOURAUD = 2,
    rwSHADEMODEFLAT = 1,
    rwSHADEMODENASHADEMODE = 0
};

// from iModel.cpp
enum RwStreamAccessType {
    rwSTREAMACCESSTYPEFORCEENUMSIZEINT = 2147483647,
    rwSTREAMAPPEND = 3,
    rwSTREAMWRITE = 2,
    rwSTREAMREAD = 1,
    rwNASTREAMACCESS = 0
};

// from iModel.cpp
enum RwStreamType {
    rwSTREAMTYPEFORCEENUMSIZEINT = 2147483647,
    rwSTREAMCUSTOM = 4,
    rwSTREAMMEMORY = 3,
    rwSTREAMFILENAME = 2,
    rwSTREAMFILE = 1,
    rwNASTREAM = 0
};

// from xScrFx.cpp
enum RwTextureAddressMode {
    rwTEXTUREADDRESSMODEFORCEENUMSIZEINT = 2147483647,
    rwTEXTUREADDRESSBORDER = 4,
    rwTEXTUREADDRESSCLAMP = 3,
    rwTEXTUREADDRESSMIRROR = 2,
    rwTEXTUREADDRESSWRAP = 1,
    rwTEXTUREADDRESSNATEXTUREADDRESS = 0
};

// from xScrFx.cpp
enum RwTextureFilterMode {
    rwTEXTUREFILTERMODEFORCEENUMSIZEINT = 2147483647,
    rwFILTERLINEARMIPLINEAR = 6,
    rwFILTERLINEARMIPNEAREST = 5,
    rwFILTERMIPLINEAR = 4,
    rwFILTERMIPNEAREST = 3,
    rwFILTERLINEAR = 2,
    rwFILTERNEAREST = 1,
    rwFILTERNAFILTERMODE = 0
};

// from xScrFx.cpp
enum RwVideoModeFlag {
    rwVIDEOMODEFLAGFORCEENUMSIZEINT = 2147483647,
    rwVIDEOMODEFSAA1 = 16,
    rwVIDEOMODEFSAA0 = 8,
    rwVIDEOMODEFFINTERLACE = 4,
    rwVIDEOMODEINTERLACE = 2,
    rwVIDEOMODEEXCLUSIVE = 1
};

// from xScrFx.cpp
enum RxClusterForcePresent {
    rxCLUSTERFORCEPRESENTFORCEENUMSIZEINT = 2147483647,
    rxCLFORCEPRESENT = 1,
    rxCLALLOWABSENT = 0
};

// from xScrFx.cpp
enum RxClusterValid {
    rxCLUSTERVALIDFORCEENUMSIZEINT = 2147483647,
    rxCLVALID_INVALID = 2,
    rxCLVALID_VALID = 1,
    rxCLVALID_NOCHANGE = 0
};

// from xScrFx.cpp
enum RxClusterValidityReq {
    rxCLUSTERVALIDITYREQFORCEENUMSIZEINT = 2147483647,
    rxCLREQ_OPTIONAL = 2,
    rxCLREQ_REQUIRED = 1,
    rxCLREQ_DONTWANT = 0
};

// from p2im3dhack.cpp
enum RxInstanceFlags {
    rxINSTANCEFORCEENUMSIZEINT = 2147483647,
    rxINSTANCEMASK = 524287,
    rxINSTANCEALL = 524272,
    rxINSTANCEUSER4 = 262144,
    rxINSTANCEUSER3 = 131072,
    rxINSTANCEUSER2 = 65536,
    rxINSTANCEUSER1 = 32768,
    rxINSTANCEUV8 = 16384,
    rxINSTANCEUV7 = 8192,
    rxINSTANCEUV6 = 4096,
    rxINSTANCEUV5 = 2048,
    rxINSTANCEUV4 = 1024,
    rxINSTANCEUV3 = 512,
    rxINSTANCEUV2 = 256,
    rxINSTANCEUV1 = 128,
    rxINSTANCEUV = 128,
    rxINSTANCERGBA = 64,
    rxINSTANCENORMAL = 32,
    rxINSTANCEXYZ = 16,
    rxINSTANCETYPEMASK = 14,
    rxINSTANCEFULLINSTANCE = 8,
    rxINSTANCECONGRUENTINSTANCE = 4,
    rxINSTANCEINPLACEINSTANCE = 2,
    rxINSTANCEDONTINSTANCE = 1,
    rxINSTANCENAINSTANCEFLAG = 0
};

// from xScrFx.cpp
enum RxNodeDefEditable {
    rxNODEDEFEDITABLEFORCEENUMSIZEINT = 2147483647,
    rxNODEDEFEDITABLE = 1,
    rxNODEDEFCONST = 0
};

// from iFX.cpp
enum RxPS2ObjectType {
    rxOBJTYPEFORCEENUMSIZEINT = 2147483647,
    rxOBJTYPE_IM3D = 2,
    rxOBJTYPE_ATOMIC = 1,
    rxOBJTYPE_WORLDSECTOR = 0
};

// from iFX.cpp
enum RxSkyTransTypeFlags {
    rxSKYTRANSTYPEFLAGSFORCEENUMSIZEINT = 2147483647,
    rxSKYTRANSTYPECULL = 32,
    rxSKYTRANSTYPELINE = 16,
    rxSKYTRANSTYPEISO = 8,
    rxSKYTRANSTYPELIST = 4,
    rxSKYTRANSTYPECLIP = 2,
    rxSKYTRANSTYPEFOG = 1,
    rxSKYTRANSTYPENATRANSTYPE = 0
};

// from xbinio.cpp
enum XFILE_READSECTOR_STATUS {
    XFILE_RDSTAT_FAIL = 3,
    XFILE_RDSTAT_DONE = 2,
    XFILE_RDSTAT_INPROG = 1,
    XFILE_RDSTAT_NOOP = 0
};

// from zGameState.cpp
enum _GameOstrich {
    eGameOstrich_Total = 3,
    eGameOstrich_InScene = 2,
    eGameOstrich_PlayingMovie = 1,
    eGameOstrich_Loading = 0
};

// from zEntPlayer.cpp
enum _LostChanceFX {
    LCFX_Total = 3,
    LCFX_Water = 2,
    LCFX_StdShock = 1,
    LCFX_None = 0
};

// from zRenderState.cpp
enum _SDRenderState {
    SDRS_Total = -1,
    SDRS_SkyBack = 12,
    SDRS_Tide = 11,
    SDRS_SkidMark = 10,
    SDRS_Lightning = 9,
    SDRS_Particles = 8,
    SDRS_HUD = 7,
    SDRS_Font = 6,
    SDRS_Projectile = 5,
    SDRS_Bubble = 4,
    SDRS_AlphaModels = 3,
    SDRS_OpaqueModels = 2,
    SDRS_Default = 1,
    SDRS_Unknown = 0
};

// from xScrFx.cpp
enum _tagCamType {
    eCamType_Total = 6,
    eCamType_Shoulder = 5,
    eCamType_StaticFollow = 4,
    eCamType_Path = 3,
    eCamType_Static = 2,
    eCamType_FollowWithOffset = 1,
    eCamType_Follow = 0
};

// from zEntPickup.cpp
enum _tagGiveFXType {
    GFXT_PowerupSpecial = 9,
    GFXT_SDLogo = 8,
    GFXT_Ghost = 7,
    GFXT_Bubbles = 6,
    GFXT_Gum = 5,
    GFXT_QMark = 4,
    GFXT_Mmmm = 3,
    GFXT_Crumbs = 2,
    GFXT_None = 1,
    GFXT_Unknown = 0
};

// from zEntVillain.cpp
enum _tagLightningType {
    eLT_Total = 2,
    eLT_Rotating = 1,
    eLT_Line = 0
};

// from iPad.cpp
enum _tagPadInit {
    ePadInit_Finished9 = 10,
    ePadInit_Complete8b = 9,
    ePadInit_Complete8a = 8,
    ePadInit_PressureSTest7 = 7,
    ePadInit_PressureS6 = 6,
    ePadInit_EnableRumbleTest5 = 5,
    ePadInit_EnableRumble4 = 4,
    ePadInit_EnableAnalog3LetsAllPissOffChris = 3,
    ePadInit_EnableAnalog3 = 2,
    ePadInit_WaitStable2 = 1,
    ePadInit_Open1 = 0
};

// from iPad.cpp
enum _tagPadState {
    ePad_Total = 4,
    ePad_Missing = 3,
    ePad_Enabled = 2,
    ePad_DisabledError = 1,
    ePad_Disabled = 0
};

// from iPad.cpp
enum _tagRumbleType {
    eRumbleForceU32 = 2147483647,
    eRumble_Total = 11,
    eRumble_VeryHeavy = 10,
    eRumble_VeryHeavyHi = 9,
    eRumble_Heavy = 8,
    eRumble_HeavyHi = 7,
    eRumble_Medium = 6,
    eRumble_MediumHi = 5,
    eRumble_Light = 4,
    eRumble_LightHi = 3,
    eRumble_VeryLight = 2,
    eRumble_VeryLightHi = 1,
    eRumble_Off = 0
};

// from zEntDestructObj.cpp
enum _tagSDRumbleType {
    SDR_Total = 16,
    SDR_FireworkBang = 15,
    SDR_Test = 14,
    SDR_EventHeavy = 13,
    SDR_EventMedium = 12,
    SDR_EventLight = 11,
    SDR_DustDestroyedObj = 10,
    SDR_XploDestroyedObj = 9,
    SDR_WebDestroyed = 8,
    SDR_GotScared = 7,
    SDR_ScreechingStop = 6,
    SDR_FleaScratch = 5,
    SDR_VillainCollideScare = 4,
    SDR_Buttbounce = 3,
    SDR_ButtbounceJump = 2,
    SDR_Headbutt = 1,
    SDR_Digging = 0
};

// from zScene.cpp
enum _tagSceneAreas {
    eSceneArea_count = 30,
    eSceneArea_S004 = 29,
    eSceneArea_S001 = 28,
    eSceneArea_P004 = 27,
    eSceneArea_P001 = 26,
    eSceneArea_B001 = 25,
    eSceneArea_O005 = 24,
    eSceneArea_O001 = 23,
    eSceneArea_R004 = 22,
    eSceneArea_R001 = 21,
    eSceneArea_I004 = 20,
    eSceneArea_I001 = 19,
    eSceneArea_W027 = 18,
    eSceneArea_W023 = 17,
    eSceneArea_W020 = 16,
    eSceneArea_L017 = 15,
    eSceneArea_L015 = 14,
    eSceneArea_L011 = 13,
    eSceneArea_F008 = 12,
    eSceneArea_F004 = 11,
    eSceneArea_F001 = 10,
    eSceneArea_G007 = 9,
    eSceneArea_G003 = 8,
    eSceneArea_G001 = 7,
    eSceneArea_C005 = 6,
    eSceneArea_C001 = 5,
    eSceneArea_E007 = 4,
    eSceneArea_E005 = 3,
    eSceneArea_E001 = 2,
    eSceneArea_H001 = 1,
    eSceneArea_Unknown = 0
};

// from iPad.cpp
enum _tagTRCState {
    TRC_Total = 8,
    TRC_DiskInvalid = 7,
    TRC_DiskTrayClosed = 6,
    TRC_DiskTrayOpen = 5,
    TRC_PadInvalidType = 4,
    TRC_PadInvalidNoAnalog = 3,
    TRC_PadInserted = 2,
    TRC_PadMissing = 1,
    TRC_Unknown = 0
};

// from xScrFx.cpp
enum _tagTransType {
    eTransType_Total = 10,
    eTransType_Interp4Rev = 9,
    eTransType_Interp3Rev = 8,
    eTransType_Interp2Rev = 7,
    eTransType_Interp1Rev = 6,
    eTransType_Linear = 5,
    eTransType_Interp4 = 4,
    eTransType_Interp3 = 3,
    eTransType_Interp2 = 2,
    eTransType_Interp1 = 1,
    eTransType_None = 0
};

// from xCollide.cpp
enum _xCollsIdx {
    k_XCOLLS_IDX_COUNT = 6,
    k_XCOLLS_IDX_RIGHT = 5,
    k_XCOLLS_IDX_REAR = 4,
    k_XCOLLS_IDX_LEFT = 3,
    k_XCOLLS_IDX_FRONT = 2,
    k_XCOLLS_IDX_CEIL = 1,
    k_XCOLLS_IDX_FLOOR = 0
};

// from zEntVillain.cpp
enum eAmmoTypes {
    eAMMO_MAXCOUNT = 2,
    eAMMO_SoapBar = 1,
    eAMMO_BubbleGum = 0
};

// from zFMV.cpp
enum eFMVFile {
    eFMVFileCount = 41,
    eFMVFile_Cin14 = 40,
    eFMVFile_Cin13 = 39,
    eFMVFile_Cin12 = 38,
    eFMVFile_Cin11 = 37,
    eFMVFile_Cin10 = 36,
    eFMVFile_Cin9 = 35,
    eFMVFile_Cin8 = 34,
    eFMVFile_Cin7 = 33,
    eFMVFile_Cin6 = 32,
    eFMVFile_Cin5 = 31,
    eFMVFile_Cin4 = 30,
    eFMVFile_Cin3 = 29,
    eFMVFile_Cin2 = 28,
    eFMVFile_Cin1 = 27,
    eFMVFile_RenderWareLOGO = 26,
    eFMVFile_EndCredits = 25,
    eFMVFile_Credits = 24,
    eFMVFile_WBExtra4 = 23,
    eFMVFile_WBExtra3 = 22,
    eFMVFile_WBExtra2 = 21,
    eFMVFile_WBExtra1 = 20,
    eFMVFile_WBExtra0 = 19,
    eFMVFile_WarnerBrosLOGO = 18,
    eFMVFile_THQLOGO = 17,
    eFMVFile_Demo15 = 16,
    eFMVFile_Demo14 = 15,
    eFMVFile_Demo13 = 14,
    eFMVFile_Demo12 = 13,
    eFMVFile_Demo11 = 12,
    eFMVFile_Demo10 = 11,
    eFMVFile_Demo9 = 10,
    eFMVFile_Demo8 = 9,
    eFMVFile_Demo7 = 8,
    eFMVFile_Demo6 = 7,
    eFMVFile_Demo5 = 6,
    eFMVFile_Demo4 = 5,
    eFMVFile_Demo3 = 4,
    eFMVFile_Demo2 = 3,
    eFMVFile_Demo1 = 2,
    eFMVFile_SDTitleSequence = 1,
    eFMVFile_HeavyIronLOGO = 0
};

// from zGame.cpp
enum eGameMode {
    eGameMode_Count = 12,
    eGameMode_Game = 11,
    eGameMode_ConceptArtGallery = 10,
    eGameMode_MonsterGallery = 9,
    eGameMode_WorldMap = 8,
    eGameMode_Pause = 7,
    eGameMode_Save = 6,
    eGameMode_Options = 5,
    eGameMode_Load = 4,
    eGameMode_Start = 3,
    eGameMode_Title = 2,
    eGameMode_Intro = 1,
    eGameMode_Boot = 0
};

// from zHud.cpp
enum eHudDropShadowMode {
    DROPSHADOW_NORMAL = 1,
    DROPSHADOW_NONE = 0
};

// from zHud.cpp
enum eHudElement {
    eHudCount = 10,
    eHudHealthbar = 9,
    eHudGumpack = 8,
    eHudSoapbox = 7,
    eHudGameStats = 6,
    eHudMessage = 5,
    eHudSoap = 4,
    eHudGum = 3,
    eHudSnacks = 2,
    eHudChances = 1,
    eHudPlayerStatus = 0
};

// from zHud.cpp
enum eHudInterpType {
    eHudInterpTypeCount = 3,
    eHudInterpTypeAccel = 2,
    eHudInterpTypeDecel = 1,
    eHudInterpTypeLinear = 0
};

// from zHud.cpp
enum eHudModel {
    eHudModelCount = 42,
    eHudModelBubblegumPiece5 = 41,
    eHudModelBubblegumPiece4 = 40,
    eHudModelBubblegumPiece3 = 39,
    eHudModelBubblegumPiece2 = 38,
    eHudModel_err = 37,
    eHudModelClue = 36,
    eHudModelEnemyIcon = 35,
    eHudModelSandwich5 = 34,
    eHudModelSandwich4 = 33,
    eHudModelSandwich3 = 32,
    eHudModelSandwich2 = 31,
    eHudModelSandwich1 = 30,
    eHudModelHead5 = 29,
    eHudModelHead4 = 28,
    eHudModelHead3 = 27,
    eHudModelHead2 = 26,
    eHudModelHead1 = 25,
    eHudModelHead0 = 24,
    eHudModelSphere = 23,
    eHudModelHotSauce = 22,
    eHudModelMagnifyingGlass = 21,
    eHudModelScoobyCollar = 20,
    eHudModelIndividualScoobySnack = 19,
    eHudModelShovel = 18,
    eHudModelUmbrella_open = 17,
    eHudModelUmbrella_closed = 16,
    eHudModelFootballHelmut = 15,
    eHudModelLightingBolt = 14,
    eHudModelSkull = 13,
    eHudModelDivingHelmut = 12,
    eHudModelSpring = 11,
    eHudModelBlackKnightHelmut = 10,
    eHudModelPlungerSuctionCup = 9,
    eHudModelFlowerPot = 8,
    eHudModelLampShade = 7,
    eHudModelBubblePowerUp = 6,
    eHudModelBoxSoap = 5,
    eHudModelBarSoap = 4,
    eHudModelPinkBunnySlippers = 3,
    eHudModelBubblegumMachine = 2,
    eHudModelBubblegumPack = 1,
    eHudModelBubblegumPiece = 0
};

// from zHud.cpp
enum eHudStatusAng {
    eHudStatusAngCount = 11,
    eHudStatusAngTest2 = 10,
    eHudStatusAngTest1 = 9,
    eHudStatusAngHyperReverse = 8,
    eHudStatusAngHyper = 7,
    eHudStatusAngFastReverse = 6,
    eHudStatusAngFast = 5,
    eHudStatusAngSlowReverse = 4,
    eHudStatusAngSlow = 3,
    eHudStatusAngIdleReverse = 2,
    eHudStatusAngIdle = 1,
    eHudStatusAngStop = 0
};

// from zHud.cpp
enum eHudStatusColor {
    eHudStatusColorCount = 6,
    eHudStatusColorOpaque3 = 5,
    eHudStatusColorOpaque2 = 4,
    eHudStatusColorOpaque1 = 3,
    eHudStatusColorLightBlue = 2,
    eHudStatusColorTransparent = 1,
    eHudStatusColorSolid = 0
};

// from zHud.cpp
enum eHudStatusPos {
    eHudStatusPosCount = 10,
    eHudStatusPosOnScreenAlt1 = 9,
    eHudStatusPosMiddleOfScreen = 8,
    eHudStatusPosNudgeRight = 7,
    eHudStatusPosNudgeLeft = 6,
    eHudStatusPosNudgeDown = 5,
    eHudStatusPosNudgeUp = 4,
    eHudStatusPosOffScreenAlternate2 = 3,
    eHudStatusPosOffScreenAlternate1 = 2,
    eHudStatusPosOffScreenDefault = 1,
    eHudStatusPosOnScreen = 0
};

// from zHud.cpp
enum eHudStatusSize {
    eHudStatusSizeCount = 9,
    eHudStatusSizeHyperReverse = 8,
    eHudStatusSizeHyper = 7,
    eHudStatusSizeZeroVert = 6,
    eHudStatusSizeZeroHoriz = 5,
    eHudStatusSizeSquash = 4,
    eHudStatusSizeBig = 3,
    eHudStatusSizeSmall = 2,
    eHudStatusSizeZero = 1,
    eHudStatusSizeDefault = 0
};

// from isavegame.cpp
enum en_ASYNC_OPCODE {
    ISG_OPER_LOAD = 3,
    ISG_OPER_SAVE = 2,
    ISG_OPER_INIT = 1,
    ISG_OPER_NOOP = 0
};

// from isavegame.cpp
enum en_ASYNC_OPERR {
    ISG_OPERR_TGTPREP = 14,
    ISG_OPERR_TGTREM = 13,
    ISG_OPERR_TGTERR = 12,
    ISG_OPERR_LDOPEN = 11,
    ISG_OPERR_LDREAD = 10,
    ISG_OPERR_LDINIT = 9,
    ISG_OPERR_SVOPEN = 8,
    ISG_OPERR_SVWRITE = 7,
    ISG_OPERR_SVINIT = 6,
    ISG_OPERR_SVNOSPACE = 5,
    ISG_OPERR_GAMEDIR = 4,
    ISG_OPERR_INITFAIL = 3,
    ISG_OPERR_MULTIOPER = 2,
    ISG_OPERR_NOOPER = 1,
    ISG_OPERR_NONE = 0
};

// from isavegame.cpp
enum en_ASYNC_OPSTAT {
    ISG_OPSTAT_SUCCESS = 1,
    ISG_OPSTAT_INPROG = 0,
    ISG_OPSTAT_FAILURE = -1
};

// from zEnt.cpp
enum en_BEHAVIOUR_GOAL {
    ZVIL_GOAL_NOMORE = 33,
    ZVIL_GOAL_DIE = 32,
    ZVIL_GOAL_ACTION = 31,
    ZVIL_GOAL_KNOCKBACK = 30,
    ZVIL_GOAL_TELEPORT = 29,
    ZVIL_GOAL_BLOCKHEAD = 28,
    ZVIL_GOAL_HURT = 27,
    ZVIL_GOAL_GUMMED = 26,
    ZVIL_GOAL_SOAPED = 25,
    ZVIL_GOAL_DAZED = 24,
    ZVIL_GOAL_TAUNT = 23,
    ZVIL_GOAL_SPELLCAST = 22,
    ZVIL_GOAL_LOBBING = 21,
    ZVIL_GOAL_CQCATTACK = 20,
    ZVIL_GOAL_CHASE = 19,
    ZVIL_GOAL_BULLREADY = 18,
    ZVIL_GOAL_ATTACKREADY = 17,
    ZVIL_GOAL_INVESTIGATE = 16,
    ZVIL_GOAL_SURPRIZE = 15,
    ZVIL_GOAL_LISTEN = 14,
    ZVIL_GOAL_LOOKFORHERO = 13,
    ZVIL_GOAL_TALK = 12,
    ZVIL_GOAL_LEAP = 11,
    ZVIL_GOAL_AMBUSH = 10,
    ZVIL_GOAL_GUARD = 9,
    ZVIL_GOAL_FOLLOW = 8,
    ZVIL_GOAL_MINIPATROL = 7,
    ZVIL_GOAL_PATROL = 6,
    ZVIL_GOAL_BORED = 5,
    ZVIL_GOAL_IDLE = 4,
    ZVIL_GOAL_GOONLINE = 3,
    ZVIL_GOAL_STARTUP = 2,
    ZVIL_GOAL_BASEINIT = 1,
    ZVIL_GOAL_AFTERLIFE = 0,
    ZVIL_GOAL_NONE = -1
};

// from zEntVillain.cpp
enum en_BGG_PHASE {
    BGG_PHAZ_SHOCKWAVE = 4,
    BGG_PHAZ_FALL = 3,
    BGG_PHAZ_FLY = 2,
    BGG_PHAZ_BUILDUP = 1,
    BGG_PHAZ_UNKNOWN = 0
};

// from xbinio.cpp
enum en_BIO_ASYNC_ERRCODES {
    BINIO_ASYNC_FORCEENUMSIZEINT = 2147483647,
    BINIO_ASYNC_DONE = 2,
    BINIO_ASYNC_INPROG = 1,
    BINIO_ASYNC_NOOP = 0,
    BINIO_ASYNC_FAIL = -1
};

// from isavegame.cpp
enum en_CHGCODE {
    ISG_CHG_GAMELIST = 2,
    ISG_CHG_TARGET = 1,
    ISG_CHG_NONE = 0
};

// from zDispatcher.cpp
enum en_DISPATCH_COMMAND {
    ZDSP_CMD_NOMORE = 23,
    ZDSP_CHECKPNT_SET = 22,
    ZDSP_CMD_GMODE_SET = 21,
    ZDSP_CMD_GMODE_GET = 20,
    ZDSP_CMD_GSTATE_SET = 19,
    ZDSP_CMD_GSTATE_GET = 18,
    ZDSP_CMD_SFXVOL_DECR = 17,
    ZDSP_CMD_SFXVOL_INCR = 16,
    ZDSP_CMD_SFXVOL_SET = 15,
    ZDSP_CMD_SFXVOL_GET = 14,
    ZDSP_CMD_MUSVOL_DECR = 13,
    ZDSP_CMD_MUSVOL_INCR = 12,
    ZDSP_CMD_MUSVOL_SET = 11,
    ZDSP_CMD_MUSVOL_GET = 10,
    ZDSP_CMD_SNDVOL_DECR = 9,
    ZDSP_CMD_SNDVOL_INCR = 8,
    ZDSP_CMD_SNDVOL_SET = 7,
    ZDSP_CMD_SNDVOL_GET = 6,
    ZDSP_CMD_SNDMOD_SET = 5,
    ZDSP_CMD_SNDMOD_GET = 4,
    ZDSP_CMD_CTRL_VIBESET = 3,
    ZDSP_CMD_CTRL_VIBEGET = 2,
    ZDSP_CMD_CTRL_CFGSET = 1,
    ZDSP_CMD_CTRL_CFGGET = 0,
    ZDSP_CMD_INITDFLT = -1
};

// from xbinio.cpp
enum en_FIOERRCODES {
    FIOERR_USERABORT = 4,
    FIOERR_SEEKFAIL = 3,
    FIOERR_WRITEFAIL = 2,
    FIOERR_READFAIL = 1,
    FIOERR_NONE = 0
};

// from isavegame.cpp
enum en_ISGMCA_STATUS {
    ISG_MCA_STAT_DONE = 1,
    ISG_MCA_STAT_INPROG = 0,
    ISG_MCA_STAT_DONE_ERR = -1
};

// from isavegame.cpp
enum en_ISGMC_ERRSTATUS {
    ISGMC_ERR_WRITE = 6,
    ISGMC_ERR_READ = 5,
    ISGMC_ERR_CLOSE = 4,
    ISGMC_ERR_OPEN = 3,
    ISGMC_ERR_MKDIR = 2,
    ISGMC_ERR_NOMEMCARD = 1,
    ISGMC_ERR_NONE = 0
};

// from isavegame.cpp
enum en_ISG_IOMODE {
    ISG_IOMODE_APPEND = 3,
    ISG_IOMODE_WRITE = 2,
    ISG_IOMODE_READ = 1
};

// from xpkrsvc.cpp
enum en_LAYER_TYPE {
    PKR_LTYPE_NOMORE = 10,
    PKR_LTYPE_CUTSCENETOC = 9,
    PKR_LTYPE_CUTSCENE = 8,
    PKR_LTYPE_SNDTOC = 7,
    PKR_LTYPE_SRAM = 6,
    PKR_LTYPE_VRAM = 5,
    PKR_LTYPE_ANIMATION = 4,
    PKR_LTYPE_MODEL = 3,
    PKR_LTYPE_BSP = 2,
    PKR_LTYPE_TEXTURE = 1,
    PKR_LTYPE_DEFAULT = 0,
    PKR_LTYPE_ALL = -1
};

// from zLobMaster.cpp
enum en_LOBMASTER_PARAMID {
    ZLOB_PARM_NOMORE = 22,
    ZLOB_PARM_RESTPERIOD = 21,
    ZLOB_PARM_COLLIDEDELAY = 20,
    ZLOB_PARM_TUMBLE = 19,
    ZLOB_PARM_NUMBOUNCE = 18,
    ZLOB_PARM_MVPTBULL = 17,
    ZLOB_PARM_PAYLOAD = 16,
    ZLOB_PARM_HEAVYFACTOR = 15,
    ZLOB_PARM_ARCFACTOR = 14,
    ZLOB_PARM_AMMO = 13,
    ZLOB_PARM_SALVO = 12,
    ZLOB_PARM_DEBRISANGLE = 11,
    ZLOB_PARM_DST_MAXDIST = 10,
    ZLOB_PARM_TYM_MAXLIFE = 9,
    ZLOB_PARM_SCL_MODEL = 8,
    ZLOB_PARM_SPD_RANDPCT = 7,
    ZLOB_PARM_SPD_LAUNCH = 6,
    ZLOB_PARM_DIR_LAUNCH = 5,
    ZLOB_PARM_POS_LAUNCH = 4,
    ZLOB_PARM_PRJTYPEID = 3,
    ZLOB_PARM_LOBMASTERTYPE = 2,
    ZLOB_PARM_LOBFLAGS = 1,
    ZLOB_PARM_RAWASS = 0
};

// from zLobMaster.cpp
enum en_LOB_MASTER_TYPE {
    LOBMASTER_NOMORE = 4,
    LOBMASTER_PICKUP = 3,
    LOBMASTER_DEBRIS = 2,
    LOBMASTER_ARROW = 1,
    LOBMASTER_CUSTOM = 0
};

// from zLobMaster.cpp
enum en_LOB_POWERUP_TYPE {
    ZLOB_PUPTYP_NOMORE = 7,
    ZLOB_PUPTYP_ADDSNACK = 6,
    ZLOB_PUPTYP_ADDSOAP = 5,
    ZLOB_PUPTYP_ADDGUM = 4,
    ZLOB_PUPTYP_ADDSCARE = 3,
    ZLOB_PUPTYP_ADDCHANCE = 2,
    ZLOB_PUPTYP_DAMAGE = 1,
    ZLOB_PUPTYP_NOTHING = 0
};

// from isavegame.cpp
enum en_MEMCARD_SEEKPT {
    ISG_MCSEEK_END = 2,
    ISG_MCSEEK_CUR = 1,
    ISG_MCSEEK_TOP = 0
};

// from isavegame.cpp
enum en_NAMEGEN_TYPE {
    ISG_NGTYP_ICONTHUM = 3,
    ISG_NGTYP_CONFIG = 2,
    ISG_NGTYP_GAMEFILE = 1,
    ISG_NGTYP_GAMEDIR = 0
};

// from zEnt.cpp
enum en_NPC_ANIMSTATES {
    ZVIL_ANIM_NOMORE = 147,
    ZVIL_ANIM_SIMP_ACTION = 146,
    ZVIL_ANIM_CRIT_SMASHED = 145,
    ZVIL_ANIM_CRIT_SPIDERDROP = 144,
    ZVIL_ANIM_CRIT_BATSLEEP = 143,
    ZVIL_ANIM_CRIT_MOVE = 142,
    ZVIL_ANIM_SHAM_TAUNT = 141,
    ZVIL_ANIM_SHAM_MOVE = 140,
    ZVIL_ANIM_GANG_TALKANDTURN = 139,
    ZVIL_ANIM_GANG_TALK3 = 138,
    ZVIL_ANIM_GANG_TALK2 = 137,
    ZVIL_ANIM_GANG_TALK1 = 136,
    ZVIL_ANIM_GANG_TALK = 135,
    ZVIL_ANIM_GANG_BOSSACTION = 134,
    ZVIL_ANIM_GANG_ACTION = 133,
    ZVIL_ANIM_GANG_RUNCHASED = 132,
    ZVIL_ANIM_GANG_RUN = 131,
    ZVIL_ANIM_GANG_WALK = 130,
    ZVIL_ANIM_GANG_TURN = 129,
    ZVIL_ANIM_GANG_FIDGET3 = 128,
    ZVIL_ANIM_GANG_FIDGET2 = 127,
    ZVIL_ANIM_GANG_FIDGET1 = 126,
    ZVIL_ANIM_GANG_FIDGET = 125,
    ZVIL_ANIM_GANG_ALTIDLE3 = 124,
    ZVIL_ANIM_GANG_ALTIDLE2 = 123,
    ZVIL_ANIM_GANG_ALTIDLE1 = 122,
    ZVIL_ANIM_MM_BA_DEATH = 121,
    ZVIL_ANIM_MM_BA_DEATH_BEGIN = 120,
    ZVIL_ANIM_MM_BA_KNOCKED_END = 119,
    ZVIL_ANIM_MM_BA_KNOCKED_LOOP = 118,
    ZVIL_ANIM_MM_BA_KNOCKED_BEGIN = 117,
    ZVIL_ANIM_MM_BA_STUNNED_END = 116,
    ZVIL_ANIM_MM_BA_STUNNED_LOOP = 115,
    ZVIL_ANIM_MM_BA_STUNNED_BEGIN = 114,
    ZVIL_ANIM_MM_BA_SHIELD_END = 113,
    ZVIL_ANIM_MM_BA_SHIELD_LOOP = 112,
    ZVIL_ANIM_MM_BA_SHIELD_BEGIN = 111,
    ZVIL_ANIM_MM_BA_ATTACK = 110,
    ZVIL_ANIM_MM_BA_ATTACK_LOOP = 109,
    ZVIL_ANIM_MM_BA_ATTACK_BEGIN = 108,
    ZVIL_ANIM_MM_BA_REACT = 107,
    ZVIL_ANIM_MM_BA_TAUNT = 106,
    ZVIL_ANIM_MM_BA_BACKSTEP = 105,
    ZVIL_ANIM_MM_BA_TURN = 104,
    ZVIL_ANIM_MM_BA_MOVE = 103,
    ZVIL_ANIM_MM_BA_IDLE = 102,
    ZVIL_ANIM_MM_FP_JUMP = 101,
    ZVIL_ANIM_MM_FP_ACTION = 100,
    ZVIL_ANIM_MM_FP_TAUNT = 99,
    ZVIL_ANIM_MM_FP_MOVE = 98,
    ZVIL_ANIM_RB_DEATH_LOOP = 97,
    ZVIL_ANIM_RB_LOOK_END = 96,
    ZVIL_ANIM_RB_LOOK_LOOP = 95,
    ZVIL_ANIM_RB_LOOK_BEGIN = 94,
    ZVIL_ANIM_RB_REACT = 93,
    ZVIL_ANIM_RB_ATTACKCATCH = 92,
    ZVIL_ANIM_RB_ATTACKWAIT = 91,
    ZVIL_ANIM_RB_ATTACKTHROW = 90,
    ZVIL_ANIM_RB_ATTACKPUNCH = 89,
    ZVIL_ANIM_RB_TAUNTLAUGH = 88,
    ZVIL_ANIM_RB_TAUNTTHROAT = 87,
    ZVIL_ANIM_RB_TAUNTFEEL = 86,
    ZVIL_ANIM_RB_CHASE = 85,
    ZVIL_ANIM_RB_TURN = 84,
    ZVIL_ANIM_RB_PATROL = 83,
    ZVIL_ANIM_RB_IDLEWIND = 82,
    ZVIL_ANIM_GG_HURT2 = 81,
    ZVIL_ANIM_GG_HURT1 = 80,
    ZVIL_ANIM_GG_HURT = 79,
    ZVIL_ANIM_GG_TAUNT1 = 78,
    ZVIL_ANIM_GG_TAUNT = 77,
    ZVIL_ANIM_GG_ATTACK = 76,
    ZVIL_ANIM_GG_BUILDLOOP = 75,
    ZVIL_ANIM_GG_BUILDBEGIN = 74,
    ZVIL_ANIM_GG_CHASERIGHT = 73,
    ZVIL_ANIM_GG_CHASELEFT = 72,
    ZVIL_ANIM_GG_CHASE = 71,
    ZVIL_ANIM_BK_DEATH = 70,
    ZVIL_ANIM_BK_GLOAT2 = 69,
    ZVIL_ANIM_BK_GLOAT1 = 68,
    ZVIL_ANIM_BK_GLOAT = 67,
    ZVIL_ANIM_BK_SHOCKSKEL = 66,
    ZVIL_ANIM_BK_SHOCK = 65,
    ZVIL_ANIM_BK_ATTACK = 64,
    ZVIL_ANIM_BK_PATROLFLIP = 63,
    ZVIL_ANIM_BK_PATROL = 62,
    ZVIL_ANIM_BK_TURN = 61,
    ZVIL_ANIM_BK_WALK = 60,
    ZVIL_ANIM_BK_SCARYIDLE = 59,
    ZVIL_ANIM_FALL_SPLAT = 58,
    ZVIL_ANIM_FALL_LOOP = 57,
    ZVIL_ANIM_JUMP_LAND = 56,
    ZVIL_ANIM_JUMP_LOOP = 55,
    ZVIL_ANIM_JUMP_BEGIN = 54,
    ZVIL_ANIM_DEATH_IDLE = 53,
    ZVIL_ANIM_DEATH_DAZED = 52,
    ZVIL_ANIM_DEATH_ABOVE = 51,
    ZVIL_ANIM_DEATH = 50,
    ZVIL_ANIM_DAZED_RECOVER = 49,
    ZVIL_ANIM_DAZED = 48,
    ZVIL_ANIM_DAZED_BEGIN = 47,
    ZVIL_ANIM_BOUNCED_WHEN_SOAPED = 46,
    ZVIL_ANIM_HIT_WHEN_SOAPED = 45,
    ZVIL_ANIM_SOAPED_BREAKFREE = 44,
    ZVIL_ANIM_SOAPED_RESIST = 43,
    ZVIL_ANIM_SOAPED = 42,
    ZVIL_ANIM_HIT_BY_SOAP = 41,
    ZVIL_ANIM_BOUNCED_WHEN_GUMMED = 40,
    ZVIL_ANIM_HIT_WHEN_GUMMED = 39,
    ZVIL_ANIM_GUMMED_BREAKFREE = 38,
    ZVIL_ANIM_GUMMED_RESIST = 37,
    ZVIL_ANIM_GUMMED = 36,
    ZVIL_ANIM_HIT_BY_GUM = 35,
    ZVIL_ANIM_EXT_COMBO = 34,
    ZVIL_ANIM_EXT_ATTACK_LOOP = 33,
    ZVIL_ANIM_EXT_ATTACK_START = 32,
    ZVIL_ANIM_LOB_COMBO = 31,
    ZVIL_ANIM_LOB_ATTACK = 30,
    ZVIL_ANIM_COMBO2 = 29,
    ZVIL_ANIM_COMBO1 = 28,
    ZVIL_ANIM_COMBO = 27,
    ZVIL_ANIM_ATTACK2 = 26,
    ZVIL_ANIM_ATTACK1 = 25,
    ZVIL_ANIM_ATTACK = 24,
    ZVIL_ANIM_ATTACKIDLE = 23,
    ZVIL_ANIM_INVESTIGATE = 22,
    ZVIL_ANIM_INVESTIGATE_START = 21,
    ZVIL_ANIM_LOOK_LOOP = 20,
    ZVIL_ANIM_LOOK_START = 19,
    ZVIL_ANIM_HEAR_LOOP = 18,
    ZVIL_ANIM_HEAR_START = 17,
    ZVIL_ANIM_TAUNT2 = 16,
    ZVIL_ANIM_TAUNT1 = 15,
    ZVIL_ANIM_TAUNT = 14,
    ZVIL_ANIM_SURPRIZE = 13,
    ZVIL_ANIM_ACTION_MOVING = 12,
    ZVIL_ANIM_ACTION_NOMOVE = 11,
    ZVIL_ANIM_CHASE = 10,
    ZVIL_ANIM_TURN = 9,
    ZVIL_ANIM_POSE = 8,
    ZVIL_ANIM_MOVE = 7,
    ZVIL_ANIM_FIDGET_BORED2 = 6,
    ZVIL_ANIM_FIDGET_BORED1 = 5,
    ZVIL_ANIM_BORED = 4,
    ZVIL_ANIM_FIDGET_IDLE2 = 3,
    ZVIL_ANIM_FIDGET_IDLE1 = 2,
    ZVIL_ANIM_IDLE = 1,
    ZVIL_ANIM_UNKNOWN = 0
};

// from zEntVillain.cpp
enum en_NPC_INI_PARAMETERS {
    ZVIL_PARM_NOMORE = 49,
    ZVIL_PARM_DMG_ENDTIME = 48,
    ZVIL_PARM_DMG_BEGTIME = 47,
    ZVIL_PARM_MINDIFFICULTY = 46,
    ZVIL_PARM_SPAWNMASK = 45,
    ZVIL_PARM_BND2_SPH_RADIUS = 44,
    ZVIL_PARM_BND2_SPH_CENTER = 43,
    ZVIL_PARM_BND_SPH_RADIUS = 42,
    ZVIL_PARM_BND_SPH_CENTER = 41,
    ZVIL_PARM_VERT_RBSWORDPIVOT = 40,
    ZVIL_PARM_VERT_RBSWORDHANDLE = 39,
    ZVIL_PARM_VERT_LOSEYEBALL = 38,
    ZVIL_PARM_VERT_DMG_BOUNCE = 37,
    ZVIL_PARM_VERT_ATTACKALT = 36,
    ZVIL_PARM_VERT_ATTACK = 35,
    ZVIL_PARM_VERT_WEAPON = 34,
    ZVIL_PARM_VERT_GUM_CUSTB = 33,
    ZVIL_PARM_VERT_GUM_CUSTA = 32,
    ZVIL_PARM_VERT_GUM_ELBOW = 31,
    ZVIL_PARM_VERT_GUM_RFOOT = 30,
    ZVIL_PARM_VERT_GUM_LFOOT = 29,
    ZVIL_PARM_WANDER_ARC = 28,
    ZVIL_PARM_SANIM_CHASE = 27,
    ZVIL_PARM_SANIM_MOVE = 26,
    ZVIL_PARM_SANIM_IDLE = 25,
    ZVIL_PARM_MDLSCL_BASE = 24,
    ZVIL_PARM_TURN_RATE = 23,
    ZVIL_PARM_CHASE_RATE = 22,
    ZVIL_PARM_MOVE_RATE = 21,
    ZVIL_PARM_PURSUE_RADIUS = 20,
    ZVIL_PARM_DETECT_HYTOFF = 19,
    ZVIL_PARM_DETECT_HEIGHT = 18,
    ZVIL_PARM_DETECT_RADIUS = 17,
    ZVIL_PARM_LOB_HEAVY = 16,
    ZVIL_PARM_LOB_ARC = 15,
    ZVIL_PARM_LOB_RELOAD_TIME = 14,
    ZVIL_PARM_LOB_PRJTYPE = 13,
    ZVIL_PARM_LOB_SALVO = 12,
    ZVIL_PARM_LOB_SPEED = 11,
    ZVIL_PARM_LOB_RANGE = 10,
    ZVIL_PARM_GLOAT_TIME = 9,
    ZVIL_PARM_FOV_ANGLE = 8,
    ZVIL_PARM_EXTEND_TIME = 7,
    ZVIL_PARM_EXTEND_WIDTH = 6,
    ZVIL_PARM_EXTEND_RATE = 5,
    ZVIL_PARM_EXTEND_RANGE = 4,
    ZVIL_PARM_DAZED_TIME = 3,
    ZVIL_PARM_GUMMED_TIME = 2,
    ZVIL_PARM_BUBBLED_TIME = 1,
    ZVIL_PARM_HIT_POINTS = 0,
    ZVIL_PARM_NONE = -1
};

// from zEnt.cpp
enum en_NPC_MODE {
    ZVIL_MODE_NOMORE = 8,
    ZVIL_MODE_DEATH = 7,
    ZVIL_MODE_RETREAT = 6,
    ZVIL_MODE_BATTLE = 5,
    ZVIL_MODE_ALERT = 4,
    ZVIL_MODE_NORMAL = 3,
    ZVIL_MODE_READY = 2,
    ZVIL_MODE_SHUTDOWN = 1,
    ZVIL_MODE_NOCHANGE = 0
};

// from zEnt.cpp
enum en_NPC_PERSONALITIES {
    ZVIL_PERSON_SCOOBYGANG = 7,
    ZVIL_PERSON_BOSS = 6,
    ZVIL_PERSON_AGGRESSIVE = 5,
    ZVIL_PERSON_LEADER = 4,
    ZVIL_PERSON_BULLY = 3,
    ZVIL_PERSON_WIRED = 2,
    ZVIL_PERSON_WIMP = 1,
    ZVIL_PERSON_NORMAL = 0,
    ZVIL_PERSON_BOGUS = -1
};

// from zEntVillain.cpp
enum en_NPC_QUADRANT {
    ZVIL_QUAD_RIGHT = 4,
    ZVIL_QUAD_LEFT = 3,
    ZVIL_QUAD_BACK = 2,
    ZVIL_QUAD_FRONT = 1,
    ZVIL_QUAD_UNKNOWN = 0
};

// from zEnt.cpp
enum en_NPC_SOUND {
    VSND_LISTEND = 32768,
    ZVIL_SND_NOMORE = 67,
    VSND_TYP_MMLAUGH = 66,
    VSND_TYP_MMDIE = 65,
    VSND_TYP_REAPPEAR = 64,
    VSND_TYP_DISAPPEAR = 63,
    VSND_TYP_ALMOSTKILLED = 62,
    VSND_TYP_ATTACKBUILD = 61,
    VSND_TYP_BUTTONPRESS = 60,
    VSND_TYP_FOREPLAYHURT = 59,
    VSND_TYP_TRASHTALK = 58,
    VSND_TYP_ESCAPING = 57,
    VSND_TYP_BLOCKING = 56,
    VSND_TYP_BKDIE = 55,
    VSND_TYP_BKZAPPED = 54,
    VSND_TYP_HAVEFLOAT = 53,
    VSND_TYP_NEEDFLOAT = 52,
    VSND_TYP_EXTRASPRING = 51,
    VSND_TYP_NEEDTARBOOTS = 50,
    VSND_TYP_ONELINERTOO = 49,
    VSND_TYP_ONELINER = 48,
    VSND_TYP_CHEERING = 47,
    VSND_TYP_ZSFXTALK = 46,
    VSND_TYP_SPELL_LOOP = 45,
    VSND_TYP_SPELL_PLAY = 44,
    VSND_TYP_SPELL_CAST = 43,
    VSND_TYP_PRJ_SPLAT = 42,
    VSND_TYP_PRJ_EXPLODE = 41,
    VSND_TYP_PRJ_FLIGHT = 40,
    VSND_TYP_PRJ_LAUNCH = 39,
    VSND_TYP_PLAYERDEATH = 38,
    VSND_TYP_HOLOFADE = 37,
    VSND_TYP_DIE = 36,
    VSND_TYP_FALL = 35,
    VSND_TYP_OUCHCRASH = 34,
    VSND_TYP_OUCH = 33,
    VSND_TYP_GUMSTRETCH = 32,
    VSND_TYP_GUMBOUNCE = 31,
    VSND_TYP_GUMMED = 30,
    VSND_TYP_GUMSTOP = 29,
    VSND_TYP_GUMBEGIN = 28,
    VSND_TYP_SOAPED = 27,
    VSND_TYP_SOAPSTOP = 26,
    VSND_TYP_SOAPBEGIN = 25,
    VSND_TYP_DIZZY = 24,
    VSND_TYP_LAUGH = 23,
    VSND_TYP_WEPEXPLODE = 22,
    VSND_TYP_WEPFLIGHT = 21,
    VSND_TYP_WEPLAUNCH = 20,
    VSND_TYP_PUNCH = 19,
    VSND_TYP_KICK = 18,
    VSND_TYP_ATTACKSWIPE = 17,
    VSND_TYP_ATTACK = 16,
    VSND_TYP_AMBUSH = 15,
    VSND_TYP_EXCLAIM = 14,
    VSND_TYP_CURIOUS = 13,
    VSND_TYP_LISTEN = 12,
    VSND_TYP_LOOK = 11,
    VSND_TYP_CLANKING = 10,
    VSND_TYP_SPLASH = 9,
    VSND_TYP_SPLISH = 8,
    VSND_TYP_MUMBLE = 7,
    VSND_TYP_SNORE = 6,
    VSND_TYP_IDLESLOP = 5,
    VSND_TYP_MOVE_ALT = 4,
    VSND_TYP_MOVE = 3,
    VSND_TYP_DIALOGUE = 2,
    VSND_TYP_SPOOKYROOM = 1,
    VSND_TYP_ENCOUNTER = 0,
    VSND_TYP_BOGUS = -1
};

// from zEnt.cpp
enum en_NPC_STIMULUS {
    ZVIL_STIM_NOMORE = 40,
    ZVIL_STIM_MMPUSHBUTTON = 39,
    ZVIL_STIM_HURTBOSS = 38,
    ZVIL_STIM_GANGBOSSACTION = 37,
    ZVIL_STIM_GUARDWIDGET = 36,
    ZVIL_STIM_GOTOAFTERLIFE = 35,
    ZVIL_STIM_KILLSELF = 34,
    ZVIL_STIM_GUMMED = 33,
    ZVIL_STIM_SOAPED = 32,
    ZVIL_STIM_KILLED = 31,
    ZVIL_STIM_STUNNING = 30,
    ZVIL_STIM_BUMPED = 29,
    ZVIL_STIM_BOUNCED = 28,
    ZVIL_STIM_HEADBUTT = 27,
    ZVIL_STIM_COLLIDE = 26,
    ZVIL_STIM_DOTAUNT = 25,
    ZVIL_STIM_DOACTION = 24,
    ZVIL_STIM_ATTACKHERO = 23,
    ZVIL_STIM_SHOCKED = 22,
    ZVIL_STIM_HEARDHERO = 21,
    ZVIL_STIM_SAWHERO = 20,
    ZVIL_STIM_LOOKFORHERO = 19,
    ZVIL_STIM_STOPTALK = 18,
    ZVIL_STIM_STARTTALK = 17,
    ZVIL_STIM_ENDTRESPASS = 16,
    ZVIL_STIM_TRESPASS = 15,
    ZVIL_STIM_STOPGUARD = 14,
    ZVIL_STIM_STARTGUARD = 13,
    ZVIL_STIM_STOPPATROL = 12,
    ZVIL_STIM_STARTPATROL = 11,
    ZVIL_STIM_FOLLOWWIDGET = 10,
    ZVIL_STIM_GOTOWIDGET = 9,
    ZVIL_STIM_GOTOPOS = 8,
    ZVIL_STIM_GOTOSLEEP = 7,
    ZVIL_STIM_WAKEUP = 6,
    ZVIL_STIM_TRANGOAL = 5,
    ZVIL_STIM_ENTERGOAL = 4,
    ZVIL_STIM_EXITGOAL = 3,
    ZVIL_STIM_MAKEREADY = 2,
    ZVIL_STIM_OFTHEGODS = 1,
    ZVIL_STIM_NONE = 0
};

// from zEntVillain.cpp
enum en_NPC_TIMERS {
    ZVIL_TYMR_NOMORE = 32,
    ZVIL_TYMR_WEPLIFE = 31,
    ZVIL_TYMR_GENERIC = 30,
    ZVIL_TYMR_CURGOAL = 29,
    ZVIL_TYMR_MINANIMPLAY = 28,
    ZVIL_TWIN_IGNORESNEAK = 27,
    ZVIL_TWIN_POSTATTACK = 26,
    ZVIL_TWIN_MINGUM = 25,
    ZVIL_TDUR_NAVDELAY = 24,
    ZVIL_TDUR_DECAY = 23,
    ZVIL_TDUR_HURT = 22,
    ZVIL_TDUR_BUILDUP = 21,
    ZVIL_TDUR_EXTRELOAD = 20,
    ZVIL_TDUR_LOBRELOAD = 19,
    ZVIL_TDUR_EXTEND = 18,
    ZVIL_TDUR_SEARCH = 17,
    ZVIL_TDUR_HEAR = 16,
    ZVIL_TDUR_LOOK = 15,
    ZVIL_TDUR_SOAP = 14,
    ZVIL_TDUR_GUM = 13,
    ZVIL_TDUR_DAZED = 12,
    ZVIL_TDUR_TAUNT = 11,
    ZVIL_TDUR_ALERT = 10,
    ZVIL_TDUR_STDBY = 9,
    ZVIL_TTIL_QUIP_OK = 8,
    ZVIL_TTIL_NEXTVOICE = 7,
    ZVIL_TTIL_ANIMSOUND = 6,
    ZVIL_TTIL_NEXTCHASE = 5,
    ZVIL_TTIL_RECOVERY = 4,
    ZVIL_TTIL_CHKFORNAV = 3,
    ZVIL_TTIL_TAUNT = 2,
    ZVIL_TTIL_BORED = 1,
    ZVIL_TTIL_FIDGET = 0
};

// from zEnt.cpp
enum en_PROJECTILE_EVENT {
    ZPRJ_EVNT_NOMORE = 17,
    ZPRJ_EVNT_VIL_AFTERMATH = 16,
    ZPRJ_EVNT_VIL_BUILDUP = 15,
    ZPRJ_EVNT_BOUNCE = 14,
    ZPRJ_EVNT_STARTRESTING = 13,
    ZPRJ_EVNT_DEATH = 12,
    ZPRJ_EVNT_SELFDESTRUCT = 11,
    ZPRJ_EVNT_DETONATE = 10,
    ZPRJ_EVNT_HITSTUFF = 9,
    ZPRJ_EVNT_ATREST = 8,
    ZPRJ_EVNT_INFLIGHT = 7,
    ZPRJ_EVNT_LAUNCH = 6,
    ZPRJ_EVNT_HITPLYR = 5,
    ZPRJ_EVNT_ABORT = 4,
    ZPRJ_EVNT_ENDTIME = 3,
    ZPRJ_EVNT_NEWTIME = 2,
    ZPRJ_EVNT_TRANSTATE = 1,
    ZPRJ_EVNT_ACTIVE = 0
};

// from zEntVillain.cpp
enum en_RBS_STAGE {
    RBS_STAGE_INBOUND = 4002,
    RBS_STAGE_OUTBOUND = 4001,
    RBS_STAGE_PRELIGHT = 4000
};

// from xpkrsvc.cpp
enum en_READ_ASYNC_STATUS {
    HIP_RDSTAT_NOASYNC = 4,
    HIP_RDSTAT_NOBYPASS = 3,
    HIP_RDSTAT_FAILED = 2,
    HIP_RDSTAT_SUCCESS = 1,
    HIP_RDSTAT_INPROG = 0,
    HIP_RDSTAT_NONE = -1
};

// from xsavegame.cpp
enum en_SAVEGAME_MODE {
    XSG_MODE_SAVE = 11,
    XSG_MODE_LOAD = 10
};

// from zEntVillain.cpp
enum en_VPAR_EMITTER_ROLE {
    VPAR_EMIT_EXCLAIM = 17,
    VPAR_EMIT_QUESTMARK = 16,
    VPAR_EMIT_GUARD = 15,
    VPAR_EMIT_BORED = 14,
    VPAR_EMIT_TELEPORT = 13,
    VPAR_EMIT_RESPAWN = 12,
    VPAR_EMIT_POOF = 11,
    VPAR_EMIT_DECAY = 10,
    VPAR_EMIT_DYING = 9,
    VPAR_EMIT_STUN = 8,
    VPAR_EMIT_HEADBUTT = 7,
    VPAR_EMIT_BUTTBOUNCE = 6,
    VPAR_EMIT_POPGUM = 5,
    VPAR_EMIT_GUMMY = 4,
    VPAR_EMIT_POPSOAP = 3,
    VPAR_EMIT_SOAPY = 2,
    VPAR_EMIT_EXTATTACK = 1,
    VPAR_EMIT_WALLPRINT = 0,
    VPAR_EMIT_BOGUS = -1
};

// from xsavegame.cpp
enum en_XSGASYNC_STATUS {
    XSG_ASTAT_FAILED = 3,
    XSG_ASTAT_SUCCESS = 2,
    XSG_ASTAT_INPROG = 1,
    XSG_ASTAT_NOOP = 0
};

// from zEntVillain.cpp
enum en_ZPRJ_CATAIM_FORM {
    ZPRJ_CAIM_GRAVBALL = 1,
    ZPRJ_CAIM_ARROW = 0
};

// from zEnt.cpp
enum en_ZPRJ_CATDATA_FORM {
    ZPRJ_CDAT_BOUNCE = 2,
    ZPRJ_CDAT_GRAVBALL = 1,
    ZPRJ_CDAT_ARROW = 0
};

// from zEnt.cpp
enum en_ZPRJ_CATEGORY {
    ZPRJ_CAT_NOMORE = 13,
    ZPRJ_CAT_BANGDEBRIS = 12,
    ZPRJ_CAT_STDDEBRIS = 11,
    ZPRJ_CAT_PICKUP = 10,
    ZPRJ_CAT_MMFLASK = 9,
    ZPRJ_CAT_RBSWORD = 8,
    ZPRJ_CAT_GGSHOCK = 7,
    ZPRJ_CAT_BKAXE = 6,
    ZPRJ_CAT_BUBGUM = 5,
    ZPRJ_CAT_BUBSOAP = 4,
    ZPRJ_CAT_BOUNCEBALL = 3,
    ZPRJ_CAT_GRAVBALL = 2,
    ZPRJ_CAT_ARROW = 1,
    ZPRJ_CAT_UNKNOWN = 0
};

// from zEntProjectile.cpp
enum en_ZPRJ_DOCOL_HEURISTICS {
    ZPRJ_DOCOL_NOMORE = 7,
    ZPRJ_DOCOL_VELBOUND = 6,
    ZPRJ_DOCOL_TOGGLE = 5,
    ZPRJ_DOCOL_SINE = 4,
    ZPRJ_DOCOL_RANDPCT = 3,
    ZPRJ_DOCOL_ALWAYS = 2,
    ZPRJ_DOCOL_CUSTCAT = 1,
    ZPRJ_DOCOL_CUSTOWNER = 0
};

// from zEntProjectile.cpp
enum en_ZPRJ_FX_TYPE {
    ZPRJ_FXTYPE_TOTAL = 16,
    ZPRJ_FXTYPE_UNUSED5 = 15,
    ZPRJ_FXTYPE_UNUSED4 = 14,
    ZPRJ_FXTYPE_UNUSED3 = 13,
    ZPRJ_FXTYPE_MASTERMIND_BLACK = 12,
    ZPRJ_FXTYPE_MASTERMIND_GREEN = 11,
    ZPRJ_FXTYPE_MASTERMIND_RED = 10,
    ZPRJ_FXTYPE_REDBEARD = 9,
    ZPRJ_FXTYPE_WITCHDOCTOR = 8,
    ZPRJ_FXTYPE_BLACKKNIGHT = 7,
    ZPRJ_FXTYPE_ICE = 6,
    ZPRJ_FXTYPE_NODAMAGE_FIREBALL = 5,
    ZPRJ_FXTYPE_FIREBALL = 4,
    ZPRJ_FXTYPE_TAR = 3,
    ZPRJ_FXTYPE_WOOD = 2,
    ZPRJ_FXTYPE_SMOKE = 1,
    ZPRJ_FXTYPE_NONE = 0
};

// from zEntProjectile.cpp
enum en_ZPRJ_MODEL_INDEX {
    ZPRJ_MDLIDX_NOMORE = 2,
    ZPRJ_MDLIDX_POSTHIT = 1,
    ZPRJ_MDLIDX_STD = 0
};

// from zEntProjectile.cpp
enum en_ZPRJ_SOLVER_METHODS {
    ZPRJ_ATGTSOLV_METHC = 2,
    ZPRJ_ATGTSOLV_METHB = 1,
    ZPRJ_ATGTSOLV_METHA = 0
};

// from zEnt.cpp
enum en_ZPRJ_STATES {
    ZPRJ_STATE_DEAD = 3,
    ZPRJ_STATE_ACTIVE = 2,
    ZPRJ_STATE_STANDBY = 1,
    ZPRJ_STATE_UNUSED = 0
};

// from zEnt.cpp
enum en_ZVILLAIN_BOUNCE_RESPONSE {
    ZVIL_BOUNCE_LAND = 4,
    ZVIL_BOUNCE_DAMAGE = 3,
    ZVIL_BOUNCE_GUM = 2,
    ZVIL_BOUNCE_NORMAL = 1,
    ZVIL_BOUNCE_NONE = 0
};

// from zEnt.cpp
enum en_ZVILLAIN_BUBBLE_RESPONSE {
    ZVIL_BUBBLE_BOUNCE = 3,
    ZVIL_BUBBLE_BREAK = 2,
    ZVIL_BUBBLE_ENVELOP = 1,
    ZVIL_BUBBLE_NONE = 0
};

// from zEnt.cpp
enum en_ZVILLAIN_BUMPED_RESPONSE {
    ZVIL_BUMPED_ATTACK = 4,
    ZVIL_BUMPED_DAMAGE = 3,
    ZVIL_BUMPED_GUM = 2,
    ZVIL_BUMPED_SOAP = 1,
    ZVIL_BUMPED_NONE = 0
};

// from zEnt.cpp
enum en_ZVILLAIN_HEADBUTT_RESPONSE {
    ZVIL_HEADBUTT_DAMAGE = 4,
    ZVIL_HEADBUTT_HITGANG = 3,
    ZVIL_HEADBUTT_CONTINUE = 2,
    ZVIL_HEADBUTT_NORMAL = 1,
    ZVIL_HEADBUTT_NONE = 0
};

// from zEnt.cpp
enum en_ZVIL_FLAG_GROUPS {
    ZVIL_FGRP_NOMORE = 12,
    ZVIL_FGRP_ALOW = 11,
    ZVIL_FGRP_VULN = 10,
    ZVIL_FGRP_SPYK = 9,
    ZVIL_FGRP_MOVE = 8,
    ZVIL_FGRP_DTCT = 7,
    ZVIL_FGRP_ATAK = 6,
    ZVIL_FGRP_ABLE = 5,
    ZVIL_FGRP_PENBY = 4,
    ZVIL_FGRP_CHKBY = 3,
    ZVIL_FGRP_PEN = 2,
    ZVIL_FGRP_CHK = 1,
    ZVIL_FGRP_PHYS = 0
};

// from zEnt.cpp
enum en_ZVIL_GOAL_SPOT {
    ZVIL_GSPOT_HURT_ALTB = 16,
    ZVIL_GSPOT_HURT_ALTA = 15,
    ZVIL_GSPOT_TAUNT_ALTB = 14,
    ZVIL_GSPOT_TAUNT_ALTA = 13,
    ZVIL_GSPOT_MOVERIGHT = 12,
    ZVIL_GSPOT_MOVELEFT = 11,
    ZVIL_GSPOT_ACTION_ALTA = 10,
    ZVIL_GSPOT_STATERECOVER = 9,
    ZVIL_GSPOT_BUBBREAKFREE = 8,
    ZVIL_GSPOT_BUBRESIST = 7,
    ZVIL_GSPOT_BUBBOUNCE = 6,
    ZVIL_GSPOT_BUBHIT = 5,
    ZVIL_GSPOT_KICKATTACK = 4,
    ZVIL_GSPOT_ATTACKLINGER = 3,
    ZVIL_GSPOT_FIDGET = 2,
    ZVIL_GSPOT_RESUME = 1,
    ZVIL_GSPOT_START = 0
};

// from zEntVillain.cpp
enum en_ZVIL_MODEL_VERTEX {
    ZVIL_MDLVERT_NOMORE = 12,
    ZVIL_MDLVERT_SWORDPIVOT = 11,
    ZVIL_MDLVERT_SWORDHANDLE = 10,
    ZVIL_MDLVERT_LOSEYEBALL = 9,
    ZVIL_MDLVERT_DMG_BOUNCE = 8,
    ZVIL_MDLVERT_ATTACKALT = 7,
    ZVIL_MDLVERT_ATTACK = 6,
    ZVIL_MDLVERT_WEAPON = 5,
    ZVIL_MDLVERT_GUM_CUSTB = 4,
    ZVIL_MDLVERT_GUM_CUSTA = 3,
    ZVIL_MDLVERT_GUM_ELBOW = 2,
    ZVIL_MDLVERT_GUM_RFOOT = 1,
    ZVIL_MDLVERT_GUM_LFOOT = 0
};

// from zEnt.cpp
enum en_ZVIL_NOTICE {
    ZVIL_NOTE_NOMORE = 3,
    ZVIL_NOTE_OLDTIME = 2,
    ZVIL_NOTE_PRECALC = 1,
    ZVIL_NOTE_PREPARE = 0
};

// from zEnt.cpp
enum en_ZVIL_QUERY {
    ZVIL_Q_NOMORE = 26,
    ZVIL_Q_PLYRHEARD = 25,
    ZVIL_Q_PLYRSEEN = 24,
    ZVIL_Q_CANHEARPLYR = 23,
    ZVIL_Q_CANSEEPLYR = 22,
    ZVIL_Q_PLYRINLOS = 21,
    ZVIL_Q_PLYRINFOV = 20,
    ZVIL_Q_PLYRNEAR = 19,
    ZVIL_Q_PLYRTOOCLOSE = 18,
    ZVIL_Q_PLYRDETECTED = 17,
    ZVIL_Q_CANDETECT = 16,
    ZVIL_Q_SHOULDATTACK = 15,
    ZVIL_Q_CANATTACK = 14,
    ZVIL_Q_CANPURSUE = 13,
    ZVIL_Q_RUNAWAY = 12,
    ZVIL_Q_CANCHASE = 11,
    ZVIL_Q_CANPATROL = 10,
    ZVIL_Q_CANGUARD = 9,
    ZVIL_Q_HAVENAV = 8,
    ZVIL_Q_NEEDNAV = 7,
    ZVIL_Q_DOFOLLOW = 6,
    ZVIL_Q_HAVEGUARDPOS = 5,
    ZVIL_Q_HAVEANCHOR = 4,
    ZVIL_Q_PITFALL = 3,
    ZVIL_Q_ASLEEP = 2,
    ZVIL_Q_ALIVE = 1,
    ZVIL_Q_DOUPDATE = 0
};

// from zEntVillain.cpp
enum en_ZVMM_FLASK_STAGE {
    MAST_STAGE_AFTERMATH = 3,
    MAST_STAGE_TOSS = 2,
    MAST_STAGE_TEASE = 1,
    MAST_STAGE_UNKNOWN = 0
};

// from zEntVillain.cpp
enum en_ZVMM_FLASK_TYPE {
    MAST_FLSK_NOMORE = 3,
    MAST_FLSK_SMOG = 2,
    MAST_FLSK_ACID = 1,
    MAST_FLSK_FIRE = 0
};

// from zEntVillain.cpp
enum en_zEntVillainSubTypeID {
    eEntVillainType_NoMore = 50,
    eEntVillainProfessor = 49,
    eEntVillainGroundsKeeper = 48,
    eEntVillainHolly = 47,
    eEntVillainCaveMan = 46,
    eEntVillainCaptainMoody = 45,
    eEntVillainBossMastermind = 44,
    eEntVillainBossRedbeard = 43,
    eEntVillainBossGreenGhost = 42,
    eEntVillainBossBlackKnight = 41,
    eEntVillainTypeVelma = 40,
    eEntVillainTypeDaphne = 39,
    eEntVillainTypeFred = 38,
    eEntVillainTypeShaggy9 = 37,
    eEntVillainTypeShaggy8 = 36,
    eEntVillainTypeShaggy7 = 35,
    eEntVillainTypeShaggy6 = 34,
    eEntVillainTypeShaggy5 = 33,
    eEntVillainTypeShaggy4 = 32,
    eEntVillainTypeShaggy3 = 31,
    eEntVillainTypeShaggy2 = 30,
    eEntVillainTypeShaggy1 = 29,
    eEntVillainTypeShaggy0 = 28,
    eEntVillainTypeBird = 27,
    eEntVillainTypeKillerPlant = 26,
    eEntVillainTypeHand = 25,
    eEntVillainTypeSpider = 24,
    eEntVillainTypeFlyingFish = 23,
    eEntVillainTypeRat = 22,
    eEntVillainTypeCrab = 21,
    eEntVillainTypeZombie = 20,
    eEntVillainTypeWolfman = 19,
    eEntVillainTypeWitchDoctor = 18,
    eEntVillainTypeWitch = 17,
    eEntVillainTypeTarMonster = 16,
    eEntVillainTypeSpaceKook = 15,
    eEntVillainTypeShark = 14,
    eEntVillainTypeScarecrow = 13,
    eEntVillainTypeRobot = 12,
    eEntVillainTypeRedbeard = 11,
    eEntVillainTypePhantom = 10,
    eEntVillainTypeHeadlessSpecter = 9,
    eEntVillainTypeGreenGhost = 8,
    eEntVillainTypeGhostDiver = 7,
    eEntVillainTypeGhost = 6,
    eEntVillainTypeGargoyle = 5,
    eEntVillainTypeSeaCreature = 4,
    eEntVillainTypeCreeper = 3,
    eEntVillainTypeBuckMasters = 2,
    eEntVillainTypeBlackKnight = 1,
    eEntVillainTypeBat = 0,
    eEntVillainType_Undefined = -1
};

// from zEntVillain.cpp
enum en_zEntVillain_StartBehaviour {
    ZVIL_START_AMBUSH = 3,
    ZVIL_START_SLEEPING = 2,
    ZVIL_START_DEFAULT = 1,
    ZVIL_START_PREBIRTH = 0
};

// from iFX.cpp
enum iFXWarbModes {
    IFX_WARB_WEB = 6,
    IFX_WARB_TREE = 5,
    IFX_WARB_DAN2 = 4,
    IFX_WARB_DAN1 = 3,
    IFX_WARB_WATER_RIPPLE = 2,
    IFX_WARB_WATER_WAVE = 1,
    IFX_WARB_NONE = 0
};

// from xScrFx.cpp
enum rxEmbeddedPacketState {
    rxEMBEDDEDPACKETSTATEFORCEENUMSIZEINT = 2147483647,
    rxPKST_PENDING = 3,
    rxPKST_INUSE = 2,
    rxPKST_UNUSED = 1,
    rxPKST_PACKETLESS = 0
};

// from videodec.c
enum sceMpegCbType {
    sceMpegCbStr = 6,
    sceMpegCbTimeStamp = 5,
    sceMpegCbBackground = 4,
    sceMpegCbRestartDMA = 3,
    sceMpegCbStopDMA = 2,
    sceMpegCbNodata = 1,
    sceMpegCbError = 0
};

// from iFile.cpp
enum xFileDriveStatus {
    XFILE_ERROR = 6,
    XFILE_SEEKING = 5,
    XFILE_PAUSED = 4,
    XFILE_READING = 3,
    XFILE_SPINNING = 2,
    XFILE_STOPPED = 1,
    XFILE_TRAY_OPEN = 0
};

// from iFile.cpp
enum xFileDriveType {
    XFILE_DRIVE_CONSOLE_HD = 1,
    XFILE_DRIVE_CDVD = 0
};

// from xAnim.cpp
// size: 0xF0
class @class {
    /* 0x0000 */ class @class _reent;
    /* 0x0000 */ class @class _unused;
};

// from xAnim.cpp
// size: 0x58
class @class {
    /* 0x0000 */ unsigned int _unused_rand;
    /* 0x0004 */ char *_strtok_last;
    /* 0x0008 */ char _asctime_buf[26];
    /* 0x0024 */ class tm _localtime_buf;
    /* 0x0048 */ signed int _gamma_signgam;
    /* 0x0050 */ unsigned long long _rand_next;
};

// from xAnim.cpp
// size: 0xF0
class @class {
    /* 0x0000 */ unsigned char *_nextf[30];
    /* 0x0078 */ unsigned int _nmalloc[30];
};

// from iSnd.cpp
// size: 0x30
class ADPCM_FILENAME_INFO {
    /* 0x0000 */ char Filename[32];
    /* 0x0020 */ signed int hip_idx;
    /* 0x0024 */ signed int Pitch;
    /* 0x0028 */ signed int LoopInfo;
    /* 0x002C */ signed int ID;
};

// from iAnimSKB.cpp
// size: 0x20
class ANMHeader {
    /* 0x0000 */ unsigned int chunkType;
    /* 0x0004 */ unsigned int chunkLength;
    /* 0x0008 */ unsigned int chunkVersion;
    /* 0x000C */ signed int hanimVersion;
    /* 0x0010 */ signed int hanimTypeID;
    /* 0x0014 */ unsigned int numFrames;
    /* 0x0018 */ signed int flags;
    /* 0x001C */ float duration;
};

// from iAnimSKB.cpp
// size: 0x24
class ANMKey {
    /* 0x0000 */ float time;
    /* 0x0004 */ float quat[4];
    /* 0x0014 */ float tran[3];
    /* 0x0020 */ unsigned int prevFrame;
};

// from ctworld_.c
// size: 0x1C
class AtomicTestParam {
    /* 0x0000 */ class CallBackParam cbParam;
    /* 0x000C */ class RpWorldSector *worldSector;
    /* 0x0010 */ class RpAtomic * (*atomicCallBack)(class RpAtomic *, void *);
    /* 0x0014 */ class RpAtomic *testAtomic;
    /* 0x0018 */ void *testData;
};

// from read.c
// size: 0x5C
class AudioDec {
    /* 0x0000 */ signed int state;
    /* 0x0004 */ class SpuStreamHeader sshd;
    /* 0x0024 */ class SpuStreamBody ssbd;
    /* 0x002C */ signed int hdrCount;
    /* 0x0030 */ unsigned char *data;
    /* 0x0034 */ signed int put;
    /* 0x0038 */ signed int count;
    /* 0x003C */ signed int size;
    /* 0x0040 */ signed int totalBytes;
    /* 0x0044 */ signed int iopBuff;
    /* 0x0048 */ signed int iopBuffSize;
    /* 0x004C */ signed int iopLastPos;
    /* 0x0050 */ signed int iopPausePos;
    /* 0x0054 */ signed int totalBytesSent;
    /* 0x0058 */ signed int iopZero;
};

// from ctgeom_.c
// size: 0xC
class BBoxCollParam {
    /* 0x0000 */ class RpGeometry *geometry;
    /* 0x0004 */ class GeomCallBack *callBack;
    /* 0x0008 */ class RwBBox *bbox;
};

// from iDebug.cpp
// size: 0x38
class BmpMegaHeader {
    /* 0x0000 */ char prepad[2];
    /* 0x0002 */ char bfType[2];
    /* 0x0004 */ class tagBITMAPFILEHEADER bmFile;
    /* 0x0010 */ class tagBITMAPINFOHEADER bmInfo;
};

// from ctworld_.c
// size: 0xC
class CallBackParam {
    /* 0x0000 */ class RpIntersection *intersection;
    /* 0x0004 */ class IntersectionCallBack u;
    /* 0x0008 */ void *data;
};

// from iMorph.cpp
// size: 0x20
class DirtyMorph {
    /* 0x0000 */ unsigned int count;
    /* 0x0004 */ float scale;
    /* 0x0008 */ signed short weight[4];
    /* 0x0010 */ signed short *v_array[4];
};

// from xutil.cpp
// size: 0x2
class DoubleChar {
    /* 0x0000 */ unsigned char byte1;
    /* 0x0001 */ unsigned char byte2;
};

// from xCutscene.cpp
// size: 0x28
class FakeEntityInfo {
    /* 0x0000 */ class RpAtomic *model;
    /* 0x0004 */ class xCutsceneAnimHdr *ahdr;
    /* 0x0008 */ float time;
    /* 0x000C */ signed short *v_array[4];
    /* 0x001C */ signed short weight[4];
    /* 0x0024 */ float scale;
};

// from ctgeom_.c
// size: 0xC
class GeomCallBack {
    /* 0x0000 */ class RpIntersection *intersection;
    /* 0x0004 */ class RpCollisionTriangle * (*func)(class RpIntersection *, class RpCollisionTriangle *, float, void *);
    /* 0x0008 */ void *data;
};

// from zScene.cpp
// size: 0x8
class IDBasePair {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ class xBase *base;
};

// from ctworld_.c
// size: 0x4
class IntersectionCallBack {
    /* 0x0000 */ class RpWorldSector * (*sectorCB)(class RpIntersection *, class RpWorldSector *, void *);
    /* 0x0000 */ class RpCollisionTriangle * (*worldCB)(class RpIntersection *, class RpWorldSector *, class RpCollisionTriangle *, float, void *);
    /* 0x0000 */ class RpAtomic * (*atomicCB)(class RpIntersection *, class RpWorldSector *, class RpAtomic *, float, void *);
};

// from ctgeom_.c
// size: 0x18
class LineCollParam {
    /* 0x0000 */ class RpGeometry *geometry;
    /* 0x0004 */ class GeomCallBack *callBack;
    /* 0x0008 */ class RwLine *line;
    /* 0x000C */ class RwV3d delta;
};

// from ctworld_.c
// size: 0x48
class PolyLineTestParam {
    /* 0x0000 */ class RpV3dGradient grad;
    /* 0x0018 */ class RwLine clippedLine;
    /* 0x0030 */ class RpWorldSector *worldSector;
    /* 0x0034 */ class RwV3d *start;
    /* 0x0038 */ class RwV3d delta;
    /* 0x0044 */ class CallBackParam *cbParam;
};

// from ctworld_.c
// size: 0x2C
class PolyTestParam {
    /* 0x0000 */ class RwBBox bbox;
    /* 0x0018 */ class RpWorldSector *worldSector;
    /* 0x001C */ signed int (*leafTest)(signed int, signed int, void *);
    /* 0x0020 */ signed int (*v303leafTest)(signed int, signed int, void *);
    /* 0x0024 */ void *leafTestData;
    /* 0x0028 */ class CallBackParam *cbParam;
};

// from zVolume.cpp
// size: 0x50
class PreCalcOcclude {
    /* 0x0000 */ class _xVec4 DepthVec;
    /* 0x0010 */ class _xVec4 FrustVec[4];
};

// from vibuf.c
// size: 0x10
class QWORD {
    /* 0x0000 */ u_long128 q;
    /* 0x0000 */ unsigned long l[2];
    /* 0x0000 */ unsigned int i[4];
    /* 0x0000 */ unsigned short s[8];
    /* 0x0000 */ unsigned char c[16];
};

// from readbuf.c
// size: 0x5000C
class ReadBuf {
    /* 0x0000 */ unsigned char data[327680];
    /* 0x50000 */ signed int put;
    /* 0x50004 */ signed int count;
    /* 0x50008 */ signed int size;
};

// from xScrFx.cpp
// size: 0x70
class RpAtomic {
    /* 0x0000 */ class RwObjectHasFrame object;
    /* 0x0014 */ class RwResEntry *repEntry;
    /* 0x0018 */ class RpGeometry *geometry;
    /* 0x001C */ class RwSphere boundingSphere;
    /* 0x002C */ class RwSphere worldBoundingSphere;
    /* 0x003C */ class RpClump *clump;
    /* 0x0040 */ class RwLLLink inClumpLink;
    /* 0x0048 */ class RpAtomic * (*renderCallBack)(class RpAtomic *);
    /* 0x004C */ class RpInterpolator interpolator;
    /* 0x0060 */ unsigned short renderFrame;
    /* 0x0062 */ unsigned short pad;
    /* 0x0064 */ class RwLinkList llWorldSectorsInAtomic;
    /* 0x006C */ class RxPipeline *pipeline;
};

// from xScrFx.cpp
// size: 0x20
class RpClump {
    /* 0x0000 */ class RwObject object;
    /* 0x0008 */ class RwLinkList atomicList;
    /* 0x0010 */ class RwLLLink inWorldLink;
    /* 0x0018 */ class RpClump * (*callback)(class RpClump *, void *);
    /* 0x001C */ unsigned short renderFrame;
    /* 0x001E */ unsigned short pad;
};

// from ctbsp_.c
// size: 0x10
class RpCollBSPBranchNode {
    /* 0x0000 */ unsigned short type;
    /* 0x0002 */ unsigned char leftType;
    /* 0x0003 */ unsigned char rightType;
    /* 0x0004 */ unsigned short leftNode;
    /* 0x0006 */ unsigned short rightNode;
    /* 0x0008 */ float leftValue;
    /* 0x000C */ float rightValue;
};

// from ctbsp_.c
// size: 0x4
class RpCollBSPLeafNode {
    /* 0x0000 */ unsigned short numPolygons;
    /* 0x0002 */ unsigned short firstPolygon;
};

// from ctbsp_.c
// size: 0x10
class RpCollBSPTree {
    /* 0x0000 */ unsigned int numLeafNodes;
    /* 0x0004 */ class RpCollBSPBranchNode *branchNodes;
    /* 0x0008 */ class RpCollBSPLeafNode *leafNodes;
    /* 0x000C */ unsigned char pad[4];
};

// from ctworld_.c
// size: 0x8
class RpCollBSPTriangle {
    /* 0x0000 */ unsigned short vertIndex[3];
    /* 0x0006 */ signed short index;
};

// from xScrFx.cpp
// size: 0x3
class RpCollSector {
    /* 0x0000 */ unsigned char cType;
    /* 0x0001 */ unsigned char vertex;
    /* 0x0002 */ unsigned char start;
};

// from ctworld_.c
// size: 0x4
class RpCollisionBuildParam {
    /* 0x0000 */ signed int dummy;
};

// from ctgeom_.c
// size: 0x10
class RpCollisionData {
    /* 0x0000 */ signed int flags;
    /* 0x0004 */ class RpCollBSPTree *tree;
    /* 0x0008 */ signed int numTriangles;
    /* 0x000C */ unsigned short *triangleMap;
};

// from iCollide.cpp
// size: 0x28
class RpCollisionTriangle {
    /* 0x0000 */ class RwV3d normal;
    /* 0x000C */ class RwV3d point;
    /* 0x0018 */ signed int index;
    /* 0x001C */ class RwV3d *vertices[3];
};

// from xScrFx.cpp
// size: 0x6C
class RpGeometry {
    /* 0x0000 */ class RwObject object;
    /* 0x0008 */ unsigned int flags;
    /* 0x000C */ unsigned short lockedSinceLastInst;
    /* 0x000E */ signed short refCount;
    /* 0x0010 */ signed int numTriangles;
    /* 0x0014 */ signed int numVertices;
    /* 0x0018 */ signed int numMorphTargets;
    /* 0x001C */ signed int numTexCoordSets;
    /* 0x0020 */ class RpMaterialList matList;
    /* 0x002C */ class RpTriangle *triangles;
    /* 0x0030 */ class RwRGBA *preLitLum;
    /* 0x0034 */ class RwTexCoords *texCoords[8];
    /* 0x0054 */ class RwSurfaceProperties ignoredSurfaceProps;
    /* 0x0060 */ class RpMeshHeader *mesh;
    /* 0x0064 */ class RwResEntry *repEntry;
    /* 0x0068 */ class RpMorphTarget *morphTarget;
};

// from iModel.cpp
// size: 0x14
class RpHAnimAnimation {
    /* 0x0000 */ class RpHAnimInterpolatorInfo *interpInfo;
    /* 0x0004 */ signed int numFrames;
    /* 0x0008 */ signed int flags;
    /* 0x000C */ float duration;
    /* 0x0010 */ void *pFrames;
};

// from iModel.cpp
// size: 0xC
class RpHAnimAtomicGlobalVars {
    /* 0x0000 */ signed int engineOffset;
    /* 0x0004 */ class RwFreeList *HAnimFreeList;
    /* 0x0008 */ class RwFreeList *HAnimAnimationFreeList;
};

// from iModel.cpp
// size: 0x5C
class RpHAnimHierarchy {
    /* 0x0000 */ signed int flags;
    /* 0x0004 */ signed int numNodes;
    /* 0x0008 */ class RpHAnimAnimation *pCurrentAnim;
    /* 0x000C */ float currentTime;
    /* 0x0010 */ void *pNextFrame;
    /* 0x0014 */ class RpHAnimHierarchy * (*pAnimCallBack)(class RpHAnimHierarchy *, void *);
    /* 0x0018 */ void *pAnimCallBackData;
    /* 0x001C */ float animCallBackTime;
    /* 0x0020 */ class RpHAnimHierarchy * (*pAnimLoopCallBack)(class RpHAnimHierarchy *, void *);
    /* 0x0024 */ void *pAnimLoopCallBackData;
    /* 0x0028 */ class RwMatrixTag *pMatrixArray;
    /* 0x002C */ void *pMatrixArrayUnaligned;
    /* 0x0030 */ class RpHAnimNodeInfo *pNodeInfo;
    /* 0x0034 */ class RwFrame *parentFrame;
    /* 0x0038 */ signed int maxKeyFrameSize;
    /* 0x003C */ signed int currentKeyFrameSize;
    /* 0x0040 */ void (*keyFrameToMatrixCB)(class RwMatrixTag *, void *);
    /* 0x0044 */ void (*keyFrameBlendCB)(void *, void *, void *, float);
    /* 0x0048 */ void (*keyFrameInterpolateCB)(void *, void *, void *, float);
    /* 0x004C */ void (*keyFrameAddCB)(void *, void *, void *);
    /* 0x0050 */ class RpHAnimHierarchy *parentHierarchy;
    /* 0x0054 */ signed int offsetInParent;
    /* 0x0058 */ signed int rootParentOffset;
};

// from iModel.cpp
// size: 0x28
class RpHAnimInterpolatorInfo {
    /* 0x0000 */ signed int typeID;
    /* 0x0004 */ signed int keyFrameSize;
    /* 0x0008 */ void (*keyFrameToMatrixCB)(class RwMatrixTag *, void *);
    /* 0x000C */ void (*keyFrameBlendCB)(void *, void *, void *, float);
    /* 0x0010 */ void (*keyFrameInterpolateCB)(void *, void *, void *, float);
    /* 0x0014 */ void (*keyFrameAddCB)(void *, void *, void *);
    /* 0x0018 */ void (*keyFrameMulRecipCB)(void *, void *);
    /* 0x001C */ class RpHAnimAnimation * (*keyFrameStreamReadCB)(class RwStream *, class RpHAnimAnimation *);
    /* 0x0020 */ signed int (*keyFrameStreamWriteCB)(class RpHAnimAnimation *, class RwStream *);
    /* 0x0024 */ signed int (*keyFrameStreamGetSizeCB)(class RpHAnimAnimation *);
};

// from iModel.cpp
// size: 0x10
class RpHAnimNodeInfo {
    /* 0x0000 */ signed int nodeID;
    /* 0x0004 */ signed int nodeIndex;
    /* 0x0008 */ signed int flags;
    /* 0x000C */ class RwFrame *pFrame;
};

// from xScrFx.cpp
// size: 0x14
class RpInterpolator {
    /* 0x0000 */ signed int flags;
    /* 0x0004 */ signed short startMorphTarget;
    /* 0x0006 */ signed short endMorphTarget;
    /* 0x0008 */ float time;
    /* 0x000C */ float recipTime;
    /* 0x0010 */ float position;
};

// from iCollide.cpp
// size: 0x18
class RpIntersectData {
    /* 0x0000 */ class RwLine line;
    /* 0x0000 */ class RwV3d point;
    /* 0x0000 */ class RwSphere sphere;
    /* 0x0000 */ class RwBBox box;
    /* 0x0000 */ void *object;
};

// from iCollide.cpp
// size: 0x1C
class RpIntersection {
    /* 0x0000 */ class RpIntersectData t;
    /* 0x0018 */ enum RpIntersectType type;
};

// from xScrFx.cpp
// size: 0x40
class RpLight {
    /* 0x0000 */ class RwObjectHasFrame object;
    /* 0x0014 */ float radius;
    /* 0x0018 */ class RwRGBAReal color;
    /* 0x0028 */ float minusCosAngle;
    /* 0x002C */ class RwLinkList WorldSectorsInLight;
    /* 0x0034 */ class RwLLLink inWorld;
    /* 0x003C */ unsigned short lightFrame;
    /* 0x003E */ unsigned short pad;
};

// from xScrFx.cpp
// size: 0x1C
class RpMaterial {
    /* 0x0000 */ class RwTexture *texture;
    /* 0x0004 */ class RwRGBA color;
    /* 0x0008 */ class RxPipeline *pipeline;
    /* 0x000C */ class RwSurfaceProperties surfaceProps;
    /* 0x0018 */ signed short refCount;
    /* 0x001A */ signed short pad;
};

// from xScrFx.cpp
// size: 0xC
class RpMaterialList {
    /* 0x0000 */ class RpMaterial **materials;
    /* 0x0004 */ signed int numMaterials;
    /* 0x0008 */ signed int space;
};

// from p2im3dhack.cpp
// size: 0xC
class RpMesh {
    /* 0x0000 */ unsigned short *indices;
    /* 0x0004 */ unsigned int numIndices;
    /* 0x0008 */ class RpMaterial *material;
};

// from xScrFx.cpp
// size: 0x10
class RpMeshHeader {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ unsigned short numMeshes;
    /* 0x0006 */ unsigned short serialNum;
    /* 0x0008 */ unsigned int totalIndicesInMesh;
    /* 0x000C */ unsigned int firstMeshOffset;
};

// from xScrFx.cpp
// size: 0x1C
class RpMorphTarget {
    /* 0x0000 */ class RpGeometry *parentGeom;
    /* 0x0004 */ class RwSphere boundingSphere;
    /* 0x0014 */ class RwV3d *verts;
    /* 0x0018 */ class RwV3d *normals;
};

// from ctworld_.c
// size: 0x18
class RpPlaneSector {
    /* 0x0000 */ signed int type;
    /* 0x0004 */ float value;
    /* 0x0008 */ class RpSector *leftSubTree;
    /* 0x000C */ class RpSector *rightSubTree;
    /* 0x0010 */ float leftValue;
    /* 0x0014 */ float rightValue;
};

// from xScrFx.cpp
// size: 0x8
class RpPolygon {
    /* 0x0000 */ unsigned short matIndex;
    /* 0x0002 */ unsigned short vertIndex[3];
};

// from xScrFx.cpp
// size: 0x4
class RpSector {
    /* 0x0000 */ signed int type;
};

// from xScrFx.cpp
// size: 0x8
class RpTriangle {
    /* 0x0000 */ unsigned short vertIndex[3];
    /* 0x0006 */ signed short matIndex;
};

// from iMorph.cpp
// size: 0x10
class RpUserDataArray {
    /* 0x0000 */ char *name;
    /* 0x0004 */ enum RpUserDataFormat format;
    /* 0x0008 */ signed int numElements;
    /* 0x000C */ void *data;
};

// from ctbsp_.c
// size: 0x18
class RpV3dGradient {
    /* 0x0000 */ float dydx;
    /* 0x0004 */ float dzdx;
    /* 0x0008 */ float dxdy;
    /* 0x000C */ float dzdy;
    /* 0x0010 */ float dxdz;
    /* 0x0014 */ float dydz;
};

// from xScrFx.cpp
// size: 0x4
class RpVertexNormal {
    /* 0x0000 */ signed char x;
    /* 0x0001 */ signed char y;
    /* 0x0002 */ signed char z;
    /* 0x0003 */ unsigned char pad;
};

// from xScrFx.cpp
// size: 0x7C
class RpWorld {
    /* 0x0000 */ class RwObject object;
    /* 0x0008 */ unsigned int flags;
    /* 0x000C */ enum RpWorldRenderOrder renderOrder;
    /* 0x0010 */ class RpMaterialList matList;
    /* 0x001C */ class RpSector *rootSector;
    /* 0x0020 */ signed int numTexCoordSets;
    /* 0x0024 */ signed int numClumpsInWorld;
    /* 0x0028 */ class RwLLLink *currentClumpLink;
    /* 0x002C */ class RwLinkList clumpList;
    /* 0x0034 */ class RwLinkList lightList;
    /* 0x003C */ class RwLinkList directionalLightList;
    /* 0x0044 */ class RwV3d worldOrigin;
    /* 0x0050 */ class RwBBox boundingBox;
    /* 0x0068 */ class RwSurfaceProperties surfaceProps;
    /* 0x0074 */ class RpWorldSector * (*renderCallBack)(class RpWorldSector *);
    /* 0x0078 */ class RxPipeline *pipeline;
};

// from xScrFx.cpp
// size: 0x94
class RpWorldSector {
    /* 0x0000 */ signed int type;
    /* 0x0004 */ class RpPolygon *polygons;
    /* 0x0008 */ class RwV3d *vertices;
    /* 0x000C */ class RpVertexNormal *normals;
    /* 0x0010 */ class RwTexCoords *texCoords[8];
    /* 0x0030 */ class RwRGBA *preLitLum;
    /* 0x0034 */ class RwResEntry *repEntry;
    /* 0x0038 */ class RwLinkList collAtomicsInWorldSector;
    /* 0x0040 */ class RwLinkList noCollAtomicsInWorldSector;
    /* 0x0048 */ class RwLinkList lightsInWorldSector;
    /* 0x0050 */ class RwBBox boundingBox;
    /* 0x0068 */ class RwBBox tightBoundingBox;
    /* 0x0080 */ class RpCollSector *colSectorRoot;
    /* 0x0084 */ class RpMeshHeader *mesh;
    /* 0x0088 */ class RxPipeline *pipeline;
    /* 0x008C */ unsigned short matListWindowBase;
    /* 0x008E */ unsigned short numVertices;
    /* 0x0090 */ unsigned short numPolygons;
    /* 0x0092 */ unsigned short pad;
};

// from iAnim.cpp
// size: 0x10
class RtQuat {
    /* 0x0000 */ class RwV3d imag;
    /* 0x000C */ float real;
};

// from iAnim.cpp
// size: 0x28
class RtQuatSlerpCache {
    /* 0x0000 */ class RtQuat raFrom;
    /* 0x0010 */ class RtQuat raTo;
    /* 0x0020 */ float omega;
    /* 0x0024 */ signed int nearlyZeroOm;
};

// from xScrFx.cpp
// size: 0x18
class RwBBox {
    /* 0x0000 */ class RwV3d sup;
    /* 0x000C */ class RwV3d inf;
};

// from xScrFx.cpp
// size: 0x190
class RwCamera {
    /* 0x0000 */ class RwObjectHasFrame object;
    /* 0x0014 */ enum RwCameraProjection projectionType;
    /* 0x0018 */ class RwCamera * (*beginUpdate)(class RwCamera *);
    /* 0x001C */ class RwCamera * (*endUpdate)(class RwCamera *);
    /* 0x0020 */ class RwMatrixTag viewMatrix;
    /* 0x0060 */ class RwRaster *frameBuffer;
    /* 0x0064 */ class RwRaster *zBuffer;
    /* 0x0068 */ class RwV2d viewWindow;
    /* 0x0070 */ class RwV2d recipViewWindow;
    /* 0x0078 */ class RwV2d viewOffset;
    /* 0x0080 */ float nearPlane;
    /* 0x0084 */ float farPlane;
    /* 0x0088 */ float fogPlane;
    /* 0x008C */ float zScale;
    /* 0x0090 */ float zShift;
    /* 0x0094 */ unsigned short renderFrame;
    /* 0x0096 */ unsigned short pad;
    /* 0x0098 */ class RwFrustumPlane frustumPlanes[6];
    /* 0x0110 */ class RwBBox frustumBoundBox;
    /* 0x0128 */ class RwV3d frustumCorners[8];
};

// from iEnv.cpp
// size: 0x14
class RwChunkHeaderInfo {
    /* 0x0000 */ unsigned int type;
    /* 0x0004 */ unsigned int length;
    /* 0x0008 */ unsigned int version;
    /* 0x000C */ unsigned int buildNum;
    /* 0x0010 */ signed int isComplex;
};

// from iSystem.cpp
// size: 0x4
class RwEngineOpenParams {
    /* 0x0000 */ void *displayID;
};

// from p2skyfs.c
// size: 0x2C
class RwFileFunctions {
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
    /* 0x0028 */ signed int (*rwftell)(void *);
};

// from xScrFx.cpp
// size: 0xB0
class RwFrame {
    /* 0x0000 */ class RwObject object;
    /* 0x0008 */ class RwLLLink inDirtyListLink;
    /* 0x0010 */ class RwMatrixTag modelling;
    /* 0x0050 */ class RwMatrixTag ltm;
    /* 0x0090 */ class RwLinkList objectList;
    /* 0x0098 */ class RwFrame *child;
    /* 0x009C */ class RwFrame *next;
    /* 0x00A0 */ class RwFrame *root;
};

// from iModel.cpp
// size: 0x4
class RwFreeBlock {
    /* 0x0000 */ class RwFreeBlock *nextBlock;
};

// from iModel.cpp
// size: 0x28
class RwFreeList {
    /* 0x0000 */ void **freeListStack;
    /* 0x0004 */ void **freeListStackTop;
    /* 0x0008 */ class RwFreeBlock *firstBlock;
    /* 0x000C */ signed int blockSize;
    /* 0x0010 */ signed int entrySize;
    /* 0x0014 */ signed int alignmentMinusOne;
    /* 0x0018 */ signed int entriesPerBlock;
    /* 0x001C */ signed int entriesAllocated;
    /* 0x0020 */ class RwLLLink lFreeList;
};

// from xScrFx.cpp
// size: 0x14
class RwFrustumPlane {
    /* 0x0000 */ class RwPlane plane;
    /* 0x0010 */ unsigned char closestX;
    /* 0x0011 */ unsigned char closestY;
    /* 0x0012 */ unsigned char closestZ;
    /* 0x0013 */ unsigned char pad;
};

// from iDebug.cpp
// size: 0x1C
class RwImage {
    /* 0x0000 */ signed int flags;
    /* 0x0004 */ signed int width;
    /* 0x0008 */ signed int height;
    /* 0x000C */ signed int depth;
    /* 0x0010 */ signed int stride;
    /* 0x0014 */ unsigned char *cpPixels;
    /* 0x0018 */ class RwRGBA *palette;
};

// from xScrFx.cpp
// size: 0x8
class RwLLLink {
    /* 0x0000 */ class RwLLLink *next;
    /* 0x0004 */ class RwLLLink *prev;
};

// from iCollide.cpp
// size: 0x18
class RwLine {
    /* 0x0000 */ class RwV3d start;
    /* 0x000C */ class RwV3d end;
};

// from xScrFx.cpp
// size: 0x8
class RwLinkList {
    /* 0x0000 */ class RwLLLink link;
};

// from xScrFx.cpp
// size: 0x40
class RwMatrixTag {
    /* 0x0000 */ class RwV3d right;
    /* 0x000C */ unsigned int flags;
    /* 0x0010 */ class RwV3d up;
    /* 0x001C */ unsigned int pad1;
    /* 0x0020 */ class RwV3d at;
    /* 0x002C */ unsigned int pad2;
    /* 0x0030 */ class RwV3d pos;
    /* 0x003C */ unsigned int pad3;
};

// from iModel.cpp
// size: 0x10
class RwMatrixWeights {
    /* 0x0000 */ float w0;
    /* 0x0004 */ float w1;
    /* 0x0008 */ float w2;
    /* 0x000C */ float w3;
};

// from iModel.cpp
// size: 0x8
class RwMemory {
    /* 0x0000 */ unsigned char *start;
    /* 0x0004 */ unsigned int length;
};

// from iSystem.cpp
// size: 0x10
class RwMemoryFunctions {
    /* 0x0000 */ void * (*rwmalloc)(unsigned int);
    /* 0x0004 */ void (*rwfree)(void *);
    /* 0x0008 */ void * (*rwrealloc)(void *, unsigned int);
    /* 0x000C */ void * (*rwcalloc)(unsigned int, unsigned int);
};

// from p2im3dhack.cpp
// size: 0x8
class RwMeshCache {
    /* 0x0000 */ unsigned int lengthOfMeshesArray;
    /* 0x0004 */ class RwResEntry *meshes[1];
};

// from p2skyfs.c
// size: 0x8
class RwModuleInfo {
    /* 0x0000 */ signed int globalsOffset;
    /* 0x0004 */ signed int numInstances;
};

// from xScrFx.cpp
// size: 0x8
class RwObject {
    /* 0x0000 */ unsigned char type;
    /* 0x0001 */ unsigned char subType;
    /* 0x0002 */ unsigned char flags;
    /* 0x0003 */ unsigned char privateFlags;
    /* 0x0004 */ void *parent;
};

// from xScrFx.cpp
// size: 0x14
class RwObjectHasFrame {
    /* 0x0000 */ class RwObject object;
    /* 0x0008 */ class RwLLLink lFrame;
    /* 0x0010 */ class RwObjectHasFrame * (*sync)(class RwObjectHasFrame *);
};

// from xScrFx.cpp
// size: 0x10
class RwPlane {
    /* 0x0000 */ class RwV3d normal;
    /* 0x000C */ float distance;
};

// from xScrFx.cpp
// size: 0x4
class RwRGBA {
    /* 0x0000 */ unsigned char red;
    /* 0x0001 */ unsigned char green;
    /* 0x0002 */ unsigned char blue;
    /* 0x0003 */ unsigned char alpha;
};

// from xScrFx.cpp
// size: 0x10
class RwRGBAReal {
    /* 0x0000 */ float red;
    /* 0x0004 */ float green;
    /* 0x0008 */ float blue;
    /* 0x000C */ float alpha;
};

// from xScrFx.cpp
// size: 0x34
class RwRaster {
    /* 0x0000 */ class RwRaster *parent;
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

// from xScrFx.cpp
// size: 0x18
class RwResEntry {
    /* 0x0000 */ class RwLLLink link;
    /* 0x0008 */ signed int size;
    /* 0x000C */ void *owner;
    /* 0x0010 */ class RwResEntry **ownerRef;
    /* 0x0014 */ void (*destroyNotify)(class RwResEntry *);
};

// from xScrFx.cpp
// size: 0x40
class RwSky2DVertex {
    /* 0x0000 */ class RwSky2DVertexAlignmentOverlay u;
};

// from xScrFx.cpp
// size: 0x40
class RwSky2DVertexAlignmentOverlay {
    /* 0x0000 */ class RwSky2DVertexFields els;
    /* 0x0000 */ u_long128 qWords[4];
};

// from xScrFx.cpp
// size: 0x40
class RwSky2DVertexFields {
    /* 0x0000 */ class RwV3d scrVertex;
    /* 0x000C */ float camVertex_z;
    /* 0x0010 */ float u;
    /* 0x0014 */ float v;
    /* 0x0018 */ float recipZ;
    /* 0x001C */ float pad1;
    /* 0x0020 */ class RwRGBAReal color;
    /* 0x0030 */ class RwV3d objNormal;
    /* 0x003C */ float pad2;
};

// from p2skyfs.c
// size: 0x10C
class RwSkyFSState {
    /* 0x0000 */ signed int gdfsOpenFiles;
    /* 0x0004 */ char SubDir[256];
    /* 0x0104 */ class RwModuleInfo gdfsModuleInfo;
};

// from xScrFx.cpp
// size: 0x10
class RwSphere {
    /* 0x0000 */ class RwV3d center;
    /* 0x000C */ float radius;
};

// from ctbsp_.c
// size: 0x4
class RwSplitBits {
    /* 0x0000 */ float nReal;
    /* 0x0000 */ signed int nInt;
    /* 0x0000 */ unsigned int nUInt;
};

// from iModel.cpp
// size: 0x24
class RwStream {
    /* 0x0000 */ enum RwStreamType type;
    /* 0x0004 */ enum RwStreamAccessType accessType;
    /* 0x0008 */ signed int position;
    /* 0x000C */ class RwStreamUnion Type;
    /* 0x0020 */ signed int rwOwned;
};

// from iModel.cpp
// size: 0x14
class RwStreamCustom {
    /* 0x0000 */ signed int (*sfnclose)(void *);
    /* 0x0004 */ unsigned int (*sfnread)(void *, void *, unsigned int);
    /* 0x0008 */ signed int (*sfnwrite)(void *, void *, unsigned int);
    /* 0x000C */ signed int (*sfnskip)(void *, unsigned int);
    /* 0x0010 */ void *data;
};

// from iModel.cpp
// size: 0x4
class RwStreamFile {
    /* 0x0000 */ void *fpFile;
    /* 0x0000 */ void *constfpFile;
};

// from iModel.cpp
// size: 0xC
class RwStreamMemory {
    /* 0x0000 */ unsigned int position;
    /* 0x0004 */ unsigned int nSize;
    /* 0x0008 */ unsigned char *memBlock;
};

// from iModel.cpp
// size: 0x14
class RwStreamUnion {
    /* 0x0000 */ class RwStreamMemory memory;
    /* 0x0000 */ class RwStreamFile file;
    /* 0x0000 */ class RwStreamCustom custom;
};

// from xScrFx.cpp
// size: 0xC
class RwSurfaceProperties {
    /* 0x0000 */ float ambient;
    /* 0x0004 */ float specular;
    /* 0x0008 */ float diffuse;
};

// from xScrFx.cpp
// size: 0x8
class RwTexCoords {
    /* 0x0000 */ float u;
    /* 0x0004 */ float v;
};

// from xScrFx.cpp
// size: 0x18
class RwTexDictionary {
    /* 0x0000 */ class RwObject object;
    /* 0x0008 */ class RwLinkList texturesInDict;
    /* 0x0010 */ class RwLLLink lInInstance;
};

// from xScrFx.cpp
// size: 0x60
class RwTexture {
    /* 0x0000 */ class RwRaster *raster;
    /* 0x0004 */ class RwTexDictionary *dict;
    /* 0x0008 */ class RwLLLink lInDictionary;
    /* 0x0010 */ char name[32];
    /* 0x0030 */ char mask[32];
    /* 0x0050 */ signed int refCount;
    /* 0x0054 */ enum RwTextureFilterMode filtering;
    /* 0x0058 */ enum RwTextureAddressMode addressingU;
    /* 0x005C */ enum RwTextureAddressMode addressingV;
};

// from xScrFx.cpp
// size: 0x8
class RwV2d {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
};

// from xScrFx.cpp
// size: 0xC
class RwV3d {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
};

// from xScrFx.cpp
// size: 0x10
class RwVideoMode {
    /* 0x0000 */ signed int width;
    /* 0x0004 */ signed int height;
    /* 0x0008 */ signed int depth;
    /* 0x000C */ enum RwVideoModeFlag flags;
};

// from p2im3dhack.cpp
// size: 0x28
class RxCamSpace3DVertex {
    /* 0x0000 */ class RwV3d cameraVertex;
    /* 0x000C */ unsigned char clipFlags;
    /* 0x000D */ unsigned char pad[3];
    /* 0x0010 */ class RwRGBAReal col;
    /* 0x0020 */ float u;
    /* 0x0024 */ float v;
};

// from xScrFx.cpp
// size: 0x1C
class RxCluster {
    /* 0x0000 */ unsigned short flags;
    /* 0x0002 */ unsigned short stride;
    /* 0x0004 */ void *data;
    /* 0x0008 */ void *currentData;
    /* 0x000C */ unsigned int numAlloced;
    /* 0x0010 */ unsigned int numUsed;
    /* 0x0014 */ class RxPipelineCluster *clusterRef;
    /* 0x0018 */ unsigned int attributes;
};

// from xScrFx.cpp
// size: 0x10
class RxClusterDefinition {
    /* 0x0000 */ char *name;
    /* 0x0004 */ unsigned int defaultStride;
    /* 0x0008 */ unsigned int defaultAttributes;
    /* 0x000C */ char *attributeSet;
};

// from xScrFx.cpp
// size: 0xC
class RxClusterRef {
    /* 0x0000 */ class RxClusterDefinition *clusterDef;
    /* 0x0004 */ enum RxClusterForcePresent forcePresent;
    /* 0x0008 */ unsigned int reserved;
};

// from zEntVillain.cpp
// size: 0x4
class RxColorUnion {
    /* 0x0000 */ class RwRGBA preLitColor;
    /* 0x0000 */ class RwRGBA color;
};

// from xScrFx.cpp
// size: 0x1C
class RxHeap {
    /* 0x0000 */ unsigned int superBlockSize;
    /* 0x0004 */ class rxHeapSuperBlockDescriptor *head;
    /* 0x0008 */ class rxHeapBlockHeader *headBlock;
    /* 0x000C */ class rxHeapFreeBlock *freeBlocks;
    /* 0x0010 */ unsigned int entriesAlloced;
    /* 0x0014 */ unsigned int entriesUsed;
    /* 0x0018 */ signed int dirty;
};

// from xScrFx.cpp
// size: 0x14
class RxIoSpec {
    /* 0x0000 */ unsigned int numClustersOfInterest;
    /* 0x0004 */ class RxClusterRef *clustersOfInterest;
    /* 0x0008 */ enum RxClusterValidityReq *inputRequirements;
    /* 0x000C */ unsigned int numOutputs;
    /* 0x0010 */ class RxOutputSpec *outputs;
};

// from p2im3dhack.cpp
// size: 0xD0
class RxMeshStateVector {
    /* 0x0000 */ signed int Flags;
    /* 0x0004 */ void *SourceObject;
    /* 0x0010 */ class RwMatrixTag Obj2World;
    /* 0x0050 */ class RwMatrixTag Obj2Cam;
    /* 0x0090 */ class RwSurfaceProperties SurfaceProperties;
    /* 0x009C */ class RwTexture *Texture;
    /* 0x00A0 */ class RwRGBA MatCol;
    /* 0x00A4 */ class RxPipeline *Pipeline;
    /* 0x00A8 */ enum RwPrimitiveType PrimType;
    /* 0x00AC */ unsigned int NumElements;
    /* 0x00B0 */ unsigned int NumVertices;
    /* 0x00B4 */ signed int ClipFlagsOr;
    /* 0x00B8 */ signed int ClipFlagsAnd;
    /* 0x00BC */ void *SourceMesh;
    /* 0x00C0 */ void *DataObject;
};

// from xScrFx.cpp
// size: 0x40
class RxNodeDefinition {
    /* 0x0000 */ char *name;
    /* 0x0004 */ class RxNodeMethods nodeMethods;
    /* 0x0020 */ class RxIoSpec io;
    /* 0x0034 */ unsigned int pipelineNodePrivateDataSize;
    /* 0x0038 */ enum RxNodeDefEditable editable;
    /* 0x003C */ signed int InputPipesCnt;
};

// from xScrFx.cpp
// size: 0x1C
class RxNodeMethods {
    /* 0x0000 */ signed int (*nodeBody)(class RxPipelineNode *, class RxPipelineNodeParam *);
    /* 0x0004 */ signed int (*nodeInit)(class RxNodeDefinition *);
    /* 0x0008 */ void (*nodeTerm)(class RxNodeDefinition *);
    /* 0x000C */ signed int (*pipelineNodeInit)(class RxPipelineNode *);
    /* 0x0010 */ void (*pipelineNodeTerm)(class RxPipelineNode *);
    /* 0x0014 */ signed int (*pipelineNodeConfig)(class RxPipelineNode *, class RxPipeline *);
    /* 0x0018 */ unsigned int (*configMsgHandler)(class RxPipelineNode *, unsigned int, unsigned int, void *);
};

// from zEntVillain.cpp
// size: 0x24
class RxObjSpace3DVertex {
    /* 0x0000 */ class RwV3d objVertex;
    /* 0x000C */ class RxColorUnion c;
    /* 0x0010 */ class RwV3d objNormal;
    /* 0x001C */ float u;
    /* 0x0020 */ float v;
};

// from iFX.cpp
// size: 0x4
class RxObjUnion {
    /* 0x0000 */ class RpWorldSector *worldSector;
    /* 0x0000 */ class RpAtomic *atomic;
    /* 0x0000 */ void *agnostic;
};

// from xScrFx.cpp
// size: 0xC
class RxOutputSpec {
    /* 0x0000 */ char *name;
    /* 0x0004 */ enum RxClusterValid *outputClusters;
    /* 0x0008 */ enum RxClusterValid allOtherClusters;
};

// from p2im3dhack.cpp
// size: 0x48
class RxPS2AllPipeData {
    /* 0x0000 */ class rxNodePS2AllPvtData *objPvtData;
    /* 0x0004 */ class rxNodePS2AllMatPvtData *matPvtData;
    /* 0x0008 */ void *sourceObject;
    /* 0x000C */ class RpMeshHeader *meshHeader;
    /* 0x0010 */ class RwMeshCache *meshCache;
    /* 0x0014 */ enum RxInstanceFlags objInstance;
    /* 0x0018 */ unsigned int objIdentifier;
    /* 0x001C */ float spExtra;
    /* 0x0020 */ signed int numMorphTargets;
    /* 0x0024 */ unsigned int fastMorphing;
    /* 0x0028 */ unsigned char transType;
    /* 0x0029 */ unsigned char primType;
    /* 0x002A */ unsigned char matModulate;
    /* 0x002B */ unsigned char vu1CodeIndex;
    /* 0x002C */ class RpMesh *mesh;
    /* 0x0030 */ class RwResEntry **cacheEntryRef;
    /* 0x0034 */ enum RxInstanceFlags meshInstance;
    /* 0x0038 */ unsigned int meshIdentifier;
    /* 0x003C */ class RwSurfaceProperties *surfProps;
    /* 0x0040 */ class RwTexture *texture;
    /* 0x0044 */ class RwRGBA matCol;
};

// from iFX.cpp
// size: 0x14
class RxPS2DMASessionRecord {
    /* 0x0000 */ enum RxPS2ObjectType objType;
    /* 0x0004 */ class RxObjUnion sourceObject;
    /* 0x0008 */ enum RwFrustumTestResult inFrustum;
    /* 0x000C */ unsigned short serialNum;
    /* 0x000E */ unsigned char vu1CodeIndex;
    /* 0x000F */ unsigned char primType;
    /* 0x0010 */ enum RxSkyTransTypeFlags transType;
};

// from xScrFx.cpp
// size: 0x30
class RxPacket {
    /* 0x0000 */ unsigned short flags;
    /* 0x0002 */ unsigned short numClusters;
    /* 0x0004 */ class RxPipeline *pipeline;
    /* 0x0008 */ unsigned int *inputToClusterSlot;
    /* 0x000C */ unsigned int *slotsContinue;
    /* 0x0010 */ class RxPipelineCluster **slotClusterRefs;
    /* 0x0014 */ class RxCluster clusters[1];
};

// from xScrFx.cpp
// size: 0x34
class RxPipeline {
    /* 0x0000 */ signed int locked;
    /* 0x0004 */ unsigned int numNodes;
    /* 0x0008 */ class RxPipelineNode *nodes;
    /* 0x000C */ unsigned int packetNumClusterSlots;
    /* 0x0010 */ enum rxEmbeddedPacketState embeddedPacketState;
    /* 0x0014 */ class RxPacket *embeddedPacket;
    /* 0x0018 */ unsigned int numInputRequirements;
    /* 0x001C */ class RxPipelineRequiresCluster *inputRequirements;
    /* 0x0020 */ void *superBlock;
    /* 0x0024 */ unsigned int superBlockSize;
    /* 0x0028 */ unsigned int entryPoint;
    /* 0x002C */ unsigned int pluginId;
    /* 0x0030 */ unsigned int pluginData;
};

// from xScrFx.cpp
// size: 0x8
class RxPipelineCluster {
    /* 0x0000 */ class RxClusterDefinition *clusterRef;
    /* 0x0004 */ unsigned int creationAttributes;
};

// from xScrFx.cpp
// size: 0x28
class RxPipelineNode {
    /* 0x0000 */ class RxNodeDefinition *nodeDef;
    /* 0x0004 */ unsigned int numOutputs;
    /* 0x0008 */ unsigned int *outputs;
    /* 0x000C */ class RxPipelineCluster **slotClusterRefs;
    /* 0x0010 */ unsigned int *slotsContinue;
    /* 0x0014 */ void *privateData;
    /* 0x0018 */ unsigned int *inputToClusterSlot;
    /* 0x001C */ class RxPipelineNodeTopSortData *topSortData;
    /* 0x0020 */ void *initializationData;
    /* 0x0024 */ unsigned int initializationDataSize;
};

// from xScrFx.cpp
// size: 0x8
class RxPipelineNodeParam {
    /* 0x0000 */ void *dataParam;
    /* 0x0004 */ class RxHeap *heap;
};

// from xScrFx.cpp
// size: 0xC
class RxPipelineNodeTopSortData {
    /* 0x0000 */ unsigned int numIns;
    /* 0x0004 */ unsigned int numInsVisited;
    /* 0x0008 */ class rxReq *req;
};

// from xScrFx.cpp
// size: 0xC
class RxPipelineRequiresCluster {
    /* 0x0000 */ class RxClusterDefinition *clusterDef;
    /* 0x0004 */ enum RxClusterValidityReq rqdOrOpt;
    /* 0x0008 */ unsigned int slotIndex;
};

// from iFont.cpp
// size: 0x30
class RxRenderStateVector {
    /* 0x0000 */ unsigned int Flags;
    /* 0x0004 */ enum RwShadeMode ShadeMode;
    /* 0x0008 */ enum RwBlendFunction SrcBlend;
    /* 0x000C */ enum RwBlendFunction DestBlend;
    /* 0x0010 */ class RwRaster *TextureRaster;
    /* 0x0014 */ enum RwTextureAddressMode AddressModeU;
    /* 0x0018 */ enum RwTextureAddressMode AddressModeV;
    /* 0x001C */ enum RwTextureFilterMode FilterMode;
    /* 0x0020 */ class RwRGBA BorderColor;
    /* 0x0024 */ enum RwFogType FogType;
    /* 0x0028 */ class RwRGBA FogColor;
    /* 0x002C */ unsigned char *FogTable;
};

// from iSnd.cpp
// size: 0xC
class SIPLoadInfo {
    /* 0x0000 */ unsigned int HIPhash;
    /* 0x0004 */ unsigned int SPUaddr;
    /* 0x0008 */ unsigned int EEinfo_max;
};

// from xutil.cpp
// size: 0x2
class SJIS_STRUCT {
    /* 0x0000 */ unsigned short sjisChar;
    /* 0x0000 */ class DoubleChar doubleChar;
};

// from iAnimSKB.cpp
// size: 0xC
class SKAHeader {
    /* 0x0000 */ unsigned int numFrames;
    /* 0x0004 */ unsigned int flags;
    /* 0x0008 */ float duration;
};

// from iAnimSKB.cpp
// size: 0x24
class SKAKey {
    /* 0x0000 */ float quat[4];
    /* 0x0010 */ float tran[3];
    /* 0x001C */ float time;
    /* 0x0020 */ unsigned int prevFrame;
};

// from zEntVillain.cpp
// size: 0x34
class ST_CAT_AIMDATA {
    /* 0x0000 */ enum en_ZPRJ_CATAIM_FORM idx_caim;
    /* 0x0004 */ float spd_base;
    /* 0x0008 */ class _xVec3 dir_base;
    /* 0x0014 */ class st_CAT_AIMARROW arrow;
    /* 0x0014 */ class st_CAT_AIMGRAV grav;
};

// from vibuf.c
// size: 0x18
class SemaParam {
    /* 0x0000 */ signed int currentCount;
    /* 0x0004 */ signed int maxCount;
    /* 0x0008 */ signed int initCount;
    /* 0x000C */ signed int numWaitThreads;
    /* 0x0010 */ unsigned int attr;
    /* 0x0014 */ unsigned int option;
};

// from zEntShaggy.cpp
// size: 0xB8
class ShaggyGuts {
    /* 0x0000 */ class _zEnt *ent;
    /* 0x0004 */ unsigned int shaggyType;
    /* 0x0008 */ class _zEnt *swap;
    /* 0x000C */ float oldY;
    /* 0x0010 */ unsigned int prefIdle;
    /* 0x0014 */ unsigned int prefMove;
    /* 0x0018 */ unsigned int actionIndex;
    /* 0x001C */ float idleTimer;
    /* 0x0020 */ float talkTimer;
    /* 0x0024 */ unsigned int talkFlag;
    /* 0x0028 */ unsigned int talkRand;
    /* 0x002C */ float talkSwitch;
    /* 0x0030 */ class _zEntHangable *hangme;
    /* 0x0034 */ float hangspin;
    /* 0x0038 */ float facerot;
    /* 0x003C */ class _zMovePoint *facept;
    /* 0x0040 */ unsigned short chimneyState;
    /* 0x0042 */ unsigned short chimneyRand;
    /* 0x0044 */ float chimneyTimer;
    /* 0x0048 */ float chimneyHeight;
    /* 0x004C */ float chimneyVel;
    /* 0x0050 */ float chimneyGrav;
    /* 0x0054 */ float timeLaunch;
    /* 0x0058 */ float timeTop;
    /* 0x005C */ float timeEvent;
    /* 0x0060 */ class _zPlatform *chimneyPlat;
    /* 0x0064 */ class _xVec3 oldAt;
    /* 0x0070 */ class _xVec3 oldAt2;
    /* 0x007C */ unsigned int stopMode;
    /* 0x0080 */ float stopParam;
    /* 0x0084 */ unsigned int stopEvent;
    /* 0x0088 */ unsigned int runEvent;
    /* 0x008C */ class _zEnt *witchFollow;
    /* 0x0090 */ class _zEnt *witchDrop;
    /* 0x0094 */ class _xVec3 dropDiff;
    /* 0x00A0 */ float eatTimer;
    /* 0x00A4 */ class _zEnt *geronimoChase;
    /* 0x00A8 */ unsigned int barrelButted;
    /* 0x00AC */ unsigned int barrelBounced;
    /* 0x00B0 */ unsigned int barrelFellOver;
    /* 0x00B4 */ float swapTimer;
};

// from zEntShaggy.cpp
// size: 0xC
class ShaggyMagnet {
    /* 0x0000 */ class _zEnt *shaggy;
    /* 0x0004 */ class xBase *target;
    /* 0x0008 */ float radius;
};

// from iSnd.cpp
// size: 0x4
class SndBlockEntry {
    /* 0x0000 */ unsigned int offset : 18 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int size : 14 /* bit_offset 18 */;
};

// from iSnd.cpp
// size: 0x20
class SndBlockHdr {
    /* 0x0000 */ unsigned int SPUaddr;
    /* 0x0004 */ char HIPname[16];
    /* 0x0014 */ unsigned int sector;
    /* 0x0018 */ unsigned short offset;
    /* 0x001A */ unsigned short index;
    /* 0x001C */ unsigned short num_effects;
    /* 0x001E */ unsigned short num_streams;
};

// from iSnd.cpp
// size: 0x8
class SndInfo {
    /* 0x0000 */ unsigned int num_effects;
    /* 0x0004 */ unsigned int num_streams;
};

// from ctgeom_.c
// size: 0x10
class SphereCollParam {
    /* 0x0000 */ class RpGeometry *geometry;
    /* 0x0004 */ class GeomCallBack *callBack;
    /* 0x0008 */ class RwSphere *sphere;
    /* 0x000C */ float recipRadius;
};

// from read.c
// size: 0x8
class SpuStreamBody {
    /* 0x0000 */ char id[4];
    /* 0x0004 */ signed int size;
};

// from read.c
// size: 0x20
class SpuStreamHeader {
    /* 0x0000 */ char id[4];
    /* 0x0004 */ signed int size;
    /* 0x0008 */ signed int type;
    /* 0x000C */ signed int rate;
    /* 0x0010 */ signed int ch;
    /* 0x0014 */ signed int interSize;
    /* 0x0018 */ signed int loopStart;
    /* 0x001C */ signed int loopEnd;
};

// from strfile.c
// size: 0x34
class StrFile {
    /* 0x0000 */ signed int isOnCD;
    /* 0x0004 */ signed int size;
    /* 0x0008 */ class sceCdlFILE fp;
    /* 0x002C */ unsigned char *iopBuf;
    /* 0x0030 */ signed int fd;
};

// from ctworld_.c
// size: 0x18
class TestLine {
    /* 0x0000 */ class RwLine *line;
    /* 0x0004 */ class RwV3d dir;
    /* 0x0010 */ float length;
    /* 0x0014 */ float recipLength;
};

// from ctworld_.c
// size: 0x8
class TestSphere {
    /* 0x0000 */ class RwSphere *sphere;
    /* 0x0004 */ float recipRadius;
};

// from ps2_mpeg.c
// size: 0x30
class ThreadParam {
    /* 0x0000 */ signed int status;
    /* 0x0004 */ void *entry;
    /* 0x0008 */ void *stack;
    /* 0x000C */ signed int stackSize;
    /* 0x0010 */ void *gpReg;
    /* 0x0014 */ signed int initPriority;
    /* 0x0018 */ signed int currentPriority;
    /* 0x001C */ unsigned int attr;
    /* 0x0020 */ unsigned int option;
    /* 0x0024 */ signed int waitType;
    /* 0x0028 */ signed int waitId;
    /* 0x002C */ signed int wakeupCount;
};

// from videodec.c
// size: 0x18
class TimeStamp {
    /* 0x0000 */ signed long pts;
    /* 0x0008 */ signed long dts;
    /* 0x0010 */ signed int pos;
    /* 0x0014 */ signed int len;
};

// from iSnd.cpp
// size: 0x30
class VAGheader {
    /* 0x0000 */ char id[4];
    /* 0x0004 */ unsigned int version;
    /* 0x0008 */ unsigned int reserved;
    /* 0x000C */ unsigned int data_size;
    /* 0x0010 */ unsigned int frequency;
    /* 0x0014 */ unsigned int reserved2[3];
    /* 0x0020 */ char name[16];
};

// from videodec.c
// size: 0x60
class ViBuf {
    /* 0x0000 */ u_long128 *data;
    /* 0x0004 */ u_long128 *tag;
    /* 0x0008 */ signed int n;
    /* 0x000C */ signed int dmaStart;
    /* 0x0010 */ signed int dmaN;
    /* 0x0014 */ signed int readBytes;
    /* 0x0018 */ signed int buffSize;
    /* 0x001C */ class sceIpuDmaEnv env;
    /* 0x0040 */ signed int sema;
    /* 0x0044 */ signed int isActive;
    /* 0x0048 */ signed long totalBytes;
    /* 0x0050 */ class TimeStamp *ts;
    /* 0x0054 */ signed int n_ts;
    /* 0x0058 */ signed int count_ts;
    /* 0x005C */ signed int wt_ts;
};

// from videodec.c
// size: 0xB8
class VideoDec {
    /* 0x0000 */ class sceMpeg mpeg;
    /* 0x0048 */ class ViBuf vibuf;
    /* 0x00A8 */ unsigned int state;
    /* 0x00AC */ signed int sema;
    /* 0x00B0 */ signed int hid_endimage;
    /* 0x00B4 */ signed int hid_vblank;
};

// from vobuf.c
// size: 0x14
class VoBuf {
    /* 0x0000 */ class VoData *data;
    /* 0x0004 */ class VoTag *tag;
    /* 0x0008 */ signed int write;
    /* 0x000C */ signed int count;
    /* 0x0010 */ signed int size;
};

// from vobuf.c
// size: 0x151800
class VoData {
    /* 0x0000 */ unsigned char v[1382400];
};

// from vobuf.c
// size: 0x40440
class VoTag {
    /* 0x0000 */ signed int status;
    /* 0x0004 */ signed int dummy[15];
    /* 0x0040 */ unsigned int v[2][32896];
};

// from xAnim.cpp
// size: 0x18
class _Bigint {
    /* 0x0000 */ class _Bigint *_next;
    /* 0x0004 */ signed int _k;
    /* 0x0008 */ signed int _maxwds;
    /* 0x000C */ signed int _sign;
    /* 0x0010 */ signed int _wds;
    /* 0x0014 */ unsigned int _x[1];
};

// from zHologram.cpp
// size: 0x10
class _HoloBurst {
    /* 0x0000 */ float r;
    /* 0x0004 */ float g;
    /* 0x0008 */ float b;
    /* 0x000C */ float total_time;
};

// from xShadow.cpp
// size: 0x60
class _ProjectionParam {
    /* 0x0000 */ class RwV3d at;
    /* 0x0010 */ class RwMatrixTag invMatrix;
    /* 0x0050 */ unsigned char shadowValue;
    /* 0x0054 */ signed int fade;
    /* 0x0058 */ unsigned int numIm3DBatch;
};

// from zEnt.cpp
// size: 0xC
class _ShadowParams {
    /* 0x0000 */ unsigned int type;
    /* 0x0004 */ float at;
    /* 0x0008 */ float rad;
};

// from xAnim.cpp
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

// from xAnim.cpp
// size: 0x8
class __sbuf {
    /* 0x0000 */ unsigned char *_base;
    /* 0x0004 */ signed int _size;
};

// from xAnim.cpp
// size: 0x88
class _atexit {
    /* 0x0000 */ class _atexit *_next;
    /* 0x0004 */ signed int _ind;
    /* 0x0008 */ void (*_fns[32])();
};

// from xAnim.cpp
// size: 0xC
class _glue {
    /* 0x0000 */ class _glue *_next;
    /* 0x0004 */ signed int _niobs;
    /* 0x0008 */ class __sFILE *_iobs;
};

// from xScrFx.cpp
// size: 0x4
class _iCollis {
    /* 0x0000 */ signed int unknown;
};

// from xAnim.cpp
// size: 0x2F0
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

// from p2im3dhack.cpp
// size: 0x30
class _rwIm3DPoolStash {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ class RwMatrixTag *ltm;
    /* 0x0008 */ unsigned int numVerts;
    /* 0x000C */ class RxObjSpace3DVertex *objVerts;
    /* 0x0010 */ class RxCamSpace3DVertex *camVerts;
    /* 0x0014 */ class RwSky2DVertex *devVerts;
    /* 0x0018 */ class RxMeshStateVector *meshState;
    /* 0x001C */ class RxRenderStateVector *renderState;
    /* 0x0020 */ class RxPipeline *pipeline;
    /* 0x0024 */ enum RwPrimitiveType primType;
    /* 0x0028 */ unsigned short *indices;
    /* 0x002C */ unsigned int numIndices;
};

// from p2im3dhack.cpp
// size: 0x5
class _rwPS2AllNormalLUTs {
    /* 0x0000 */ unsigned char counterGen;
    /* 0x0001 */ unsigned char startIndOffset;
    /* 0x0002 */ unsigned char shiftMask[1];
    /* 0x0003 */ unsigned char shiftMask2[1];
    /* 0x0004 */ unsigned char vertInc[1];
};

// from p2im3dhack.cpp
// size: 0xB
class _rwPS2AllTriFanLUTs {
    /* 0x0000 */ unsigned char counterGen;
    /* 0x0001 */ unsigned char startIndOffset;
    /* 0x0002 */ unsigned char shiftMask[3];
    /* 0x0005 */ unsigned char shiftMask2[3];
    /* 0x0008 */ unsigned char vertInc[3];
};

// from disp.c
// size: 0x10
class _sceDmaTag {
    /* 0x0000 */ unsigned short qwc;
    /* 0x0002 */ unsigned char mark;
    /* 0x0003 */ unsigned char id;
    /* 0x0004 */ class _sceDmaTag *next;
    /* 0x0008 */ unsigned int p[2];
};

// from ms4sound.cpp
// size: 0x28
class _sif_client_data {
    /* 0x0000 */ class _sif_rpc_data rpcd;
    /* 0x0010 */ unsigned int command;
    /* 0x0014 */ void *buff;
    /* 0x0018 */ void *cbuff;
    /* 0x001C */ void (*func)(void *);
    /* 0x0020 */ void *para;
    /* 0x0024 */ class _sif_serve_data *serve;
};

// from ms4sound.cpp
// size: 0x18
class _sif_queue_data {
    /* 0x0000 */ signed int key;
    /* 0x0004 */ signed int active;
    /* 0x0008 */ class _sif_serve_data *link;
    /* 0x000C */ class _sif_serve_data *start;
    /* 0x0010 */ class _sif_serve_data *end;
    /* 0x0014 */ class _sif_queue_data *next;
};

// from ms4sound.cpp
// size: 0x10
class _sif_rpc_data {
    /* 0x0000 */ void *paddr;
    /* 0x0004 */ unsigned int pid;
    /* 0x0008 */ signed int tid;
    /* 0x000C */ unsigned int mode;
};

// from ms4sound.cpp
// size: 0x44
class _sif_serve_data {
    /* 0x0000 */ unsigned int command;
    /* 0x0004 */ void * (*func)(unsigned int, void *, signed int);
    /* 0x0008 */ void *buff;
    /* 0x000C */ signed int size;
    /* 0x0010 */ void * (*cfunc)(unsigned int, void *, signed int);
    /* 0x0014 */ void *cbuff;
    /* 0x0018 */ signed int csize;
    /* 0x001C */ class _sif_client_data *client;
    /* 0x0020 */ void *paddr;
    /* 0x0024 */ unsigned int fno;
    /* 0x0028 */ void *receive;
    /* 0x002C */ signed int rsize;
    /* 0x0030 */ signed int rmode;
    /* 0x0034 */ unsigned int rid;
    /* 0x0038 */ class _sif_serve_data *link;
    /* 0x003C */ class _sif_serve_data *next;
    /* 0x0040 */ class _sif_queue_data *base;
};

// from zActionLine.cpp
// size: 0x38
class _tagActionLine {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ class _xVec3 pos[4];
    /* 0x0034 */ float time_left;
};

// from xScrFx.cpp
// size: 0x18
class _tagCamInfo {
    /* 0x0000 */ class _tagxCamFollow f;
    /* 0x0000 */ class _tagxCamStatic s;
    /* 0x0000 */ class _tagxCamShoulder sh;
    /* 0x0000 */ class _tagxCamPath p;
    /* 0x0000 */ class _tagxCamStaticFollow sf;
};

// from zScene.cpp
// size: 0x24
class _tagClimate {
    /* 0x0000 */ class _tagRain rain;
    /* 0x0010 */ class _tagWind wind;
};

// from zScene.cpp
// size: 0x8
class _tagEmitCircle {
    /* 0x0000 */ float radius;
    /* 0x0004 */ float deflection;
};

// from zScene.cpp
// size: 0x1C
class _tagEmitLine {
    /* 0x0000 */ class _xVec3 pos1;
    /* 0x000C */ class _xVec3 pos2;
    /* 0x0018 */ float radius;
};

// from zScene.cpp
// size: 0xC
class _tagEmitOffsetPoint {
    /* 0x0000 */ class _xVec3 offset;
};

// from zScene.cpp
// size: 0x8
class _tagEmitRect {
    /* 0x0000 */ float x_len;
    /* 0x0004 */ float z_len;
};

// from zScene.cpp
// size: 0x4
class _tagEmitSphere {
    /* 0x0000 */ float radius;
};

// from zScene.cpp
// size: 0x4
class _tagEmitVolume {
    /* 0x0000 */ unsigned int emit_volumeID;
};

// from zScene.cpp
// size: 0x24
class _tagFeetSkidMark {
    /* 0x0000 */ signed int total_feet;
    /* 0x0004 */ class _tagSkidEdge *edge[4];
    /* 0x0014 */ signed int first_idx;
    /* 0x0018 */ signed int total_edges;
    /* 0x001C */ float sample_timer;
    /* 0x0020 */ float remove_timer;
};

// from zGameExtras.cpp
// size: 0x24
class _tagFirework {
    /* 0x0000 */ signed int state;
    /* 0x0004 */ float timer;
    /* 0x0008 */ class _xVec3 vel;
    /* 0x0014 */ class _xVec3 pos;
    /* 0x0020 */ float fuel;
};

// from zFeet.cpp
// size: 0x8
class _tagFoot {
    /* 0x0000 */ class tagzFeet *feet;
    /* 0x0004 */ signed int idx;
};

// from zHologram.cpp
// size: 0x20
class _tagHologram {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ class _xEnt *ent;
    /* 0x0008 */ float src_r;
    /* 0x000C */ float src_g;
    /* 0x0010 */ float src_b;
    /* 0x0014 */ float time_passed;
    /* 0x0018 */ signed int cur_burst;
    /* 0x001C */ signed int alpha_max;
};

// from zEntPickup.cpp
// size: 0x18
class _tagKeyShake {
    /* 0x0000 */ class _xVec3 orig_pos;
    /* 0x000C */ float shake_timer;
    /* 0x0010 */ float shake_it;
    /* 0x0014 */ class _zUI *ui;
};

// from zEntVillain.cpp
// size: 0x1EC
class _tagLightning {
    /* 0x0000 */ enum _tagLightningType type;
    /* 0x0004 */ unsigned int flags;
    /* 0x0008 */ class _xVec3 base_point[16];
    /* 0x00C8 */ class _xVec3 point[16];
    /* 0x0188 */ signed int total_points;
    /* 0x018C */ float arc_height;
    /* 0x0190 */ float thickness;
    /* 0x0194 */ class _tagLightningLine line;
    /* 0x0194 */ class _tagLightningRot rot;
    /* 0x01DC */ unsigned char r;
    /* 0x01DD */ unsigned char g;
    /* 0x01DE */ unsigned char b;
    /* 0x01DF */ unsigned char alpha_pad;
    /* 0x01E0 */ float rand_radius;
    /* 0x01E4 */ float time_left;
    /* 0x01E8 */ float time_total;
};

// from zEntVillain.cpp
// size: 0x34
class _tagLightningAdd {
    /* 0x0000 */ enum _tagLightningType type;
    /* 0x0004 */ float setup_degrees;
    /* 0x0008 */ float move_degrees;
    /* 0x000C */ float rot_radius;
    /* 0x0010 */ class _xVec3 *start;
    /* 0x0014 */ class _xVec3 *end;
    /* 0x0018 */ signed int total_points;
    /* 0x001C */ float time;
    /* 0x0020 */ float arc_height;
    /* 0x0024 */ float thickness;
    /* 0x0028 */ unsigned char r;
    /* 0x0029 */ unsigned char g;
    /* 0x002A */ unsigned char b;
    /* 0x002B */ unsigned char alpha_pad;
    /* 0x002C */ float rand_radius;
    /* 0x0030 */ unsigned int flags;
};

// from zEntVillain.cpp
// size: 0x4
class _tagLightningLine {
    /* 0x0000 */ float unused;
};

// from zEntVillain.cpp
// size: 0x48
class _tagLightningRot {
    /* 0x0000 */ float deg[16];
    /* 0x0040 */ float degrees;
    /* 0x0044 */ float height;
};

// from iPad.cpp
// size: 0x2
class _tagPadAnalog {
    /* 0x0000 */ signed char x;
    /* 0x0001 */ signed char y;
};

// from zLight.cpp
// size: 0x8
class _tagPartLink {
    /* 0x0000 */ void *data;
    /* 0x0004 */ class _tagPartLink *next;
};

// from zLight.cpp
// size: 0xC
class _tagPartSpace {
    /* 0x0000 */ signed int total;
    /* 0x0004 */ class _tagPartLink head;
};

// from zLight.cpp
// size: 0x40
class _tagPartition {
    /* 0x0000 */ class _xVec3 min;
    /* 0x000C */ class _xVec3 max;
    /* 0x0018 */ class _xVec3 space_dim;
    /* 0x0024 */ signed int total_x;
    /* 0x0028 */ signed int total_y;
    /* 0x002C */ signed int total_z;
    /* 0x0030 */ class _tagPartSpace *space;
    /* 0x0034 */ class _tagPartSpace global;
};

// from zEntPlayer.cpp
// size: 0xD0
class _tagPlayerFX {
    /* 0x0000 */ class _zParEmitter *emit_land[23];
    /* 0x005C */ class _zParEmitter *emit_stop[23];
    /* 0x00B8 */ class _zParEmitter *emit_scratch;
    /* 0x00BC */ class _zParEmitter *emit_digup[2];
    /* 0x00C4 */ float timer_stop;
    /* 0x00C8 */ float timer_scratch;
    /* 0x00CC */ float timer_digup;
};

// from zEntPlayer.cpp
// size: 0x230
class _tagPlayerSndFX {
    /* 0x0000 */ signed short ammo_pitch;
    /* 0x0002 */ signed short special_pitch;
    /* 0x0004 */ signed short token_pitch;
    /* 0x0006 */ signed short bite_pitch;
    /* 0x0008 */ signed short snackcounter_pitch;
    /* 0x000A */ signed short save_pitch;
    /* 0x000C */ signed short key_pitch;
    /* 0x000E */ signed short info_pitch;
    /* 0x0010 */ signed short error_pitch;
    /* 0x0014 */ unsigned int ammo_id;
    /* 0x0018 */ unsigned int special_id;
    /* 0x001C */ unsigned int token_id;
    /* 0x0020 */ unsigned int bite_id;
    /* 0x0024 */ unsigned int snackcounter_id;
    /* 0x0028 */ unsigned int save_id;
    /* 0x002C */ unsigned int key_id;
    /* 0x0030 */ unsigned int info_id;
    /* 0x0034 */ unsigned int error_id;
    /* 0x0038 */ unsigned int van_id;
    /* 0x003C */ signed short butt_pitch;
    /* 0x003E */ signed short head_pitch;
    /* 0x0040 */ signed short butt_snd;
    /* 0x0042 */ signed short head_snd;
    /* 0x0044 */ unsigned int butt_id[3];
    /* 0x0050 */ unsigned int head_id[3];
    /* 0x005C */ unsigned int buttmiss_id;
    /* 0x0060 */ unsigned int skid_id[3];
    /* 0x006C */ signed int vo_snd;
    /* 0x0070 */ unsigned int dig_id[2];
    /* 0x0078 */ unsigned int scare_id[2];
    /* 0x0080 */ unsigned int excite_id[13];
    /* 0x00B4 */ unsigned int danger_id[9];
    /* 0x00D8 */ unsigned int leap_id[12];
    /* 0x0108 */ unsigned int see_geronimo_id[1];
    /* 0x010C */ unsigned int see_shaggy_id[5];
    /* 0x0120 */ unsigned int see_holly_id[3];
    /* 0x012C */ unsigned int see_monster_id[5];
    /* 0x0140 */ unsigned int see_groundskeeper_id[1];
    /* 0x0144 */ unsigned int see_warpgate_id[1];
    /* 0x0148 */ unsigned int need_more_snacks_id[3];
    /* 0x0154 */ unsigned int success_id[1];
    /* 0x0158 */ unsigned int see_token_id[3];
    /* 0x0164 */ unsigned int see_crab_id[2];
    /* 0x016C */ unsigned int see_rat_id[4];
    /* 0x017C */ unsigned int see_gargoyle_id[2];
    /* 0x0184 */ unsigned int see_creeper_id[2];
    /* 0x018C */ unsigned int see_ghost_diver_id[2];
    /* 0x0194 */ unsigned int see_sea_creature_id[2];
    /* 0x019C */ unsigned int see_headless_specter_id[2];
    /* 0x01A4 */ unsigned int see_scarecrow_id[3];
    /* 0x01B0 */ unsigned int see_space_kook_id[2];
    /* 0x01B8 */ unsigned int see_tar_monster_id[4];
    /* 0x01C8 */ unsigned int see_witch_id[2];
    /* 0x01D0 */ unsigned int see_robot_id[2];
    /* 0x01D8 */ unsigned int see_wolfman_id[2];
    /* 0x01E0 */ unsigned int see_ghost_id[4];
    /* 0x01F0 */ unsigned int see_zombie_id[2];
    /* 0x01F8 */ unsigned int sorry_id[4];
    /* 0x0208 */ unsigned int collect_all_snacks_id[1];
    /* 0x020C */ unsigned int eat_sandwich_id[2];
    /* 0x0214 */ unsigned int eat_turkey_id[2];
    /* 0x021C */ unsigned int eat_cake_id[2];
    /* 0x0224 */ unsigned int eat_snacks_id[3];
};

// from zScene.cpp
// size: 0x10
class _tagRain {
    /* 0x0000 */ signed int rain;
    /* 0x0004 */ float strength;
    /* 0x0008 */ class _zParEmitter *rain_emitter;
    /* 0x000C */ class _zParEmitter *snow_emitter;
};

// from zScene.cpp
// size: 0x1C
class _tagSkidEdge {
    /* 0x0000 */ class _xVec3 centre;
    /* 0x000C */ class _xVec3 local_x;
    /* 0x0018 */ float width;
};

// from xTRC.cpp
// size: 0x4
class _tagTRCDiskInfo {
    /* 0x0000 */ enum _tagTRCState state;
};

// from iPad.cpp
// size: 0xC
class _tagTRCPadInfo : public _tagiTRCPadInfo {
    /* 0x0004 */ signed int id;
    /* 0x0008 */ enum _tagTRCState state;
};

// from zScene.cpp
// size: 0x14
class _tagWind {
    /* 0x0000 */ float strength;
    /* 0x0004 */ float angle;
    /* 0x0008 */ class _xVec3 dir;
};

// from iPad.cpp
// size: 0x4
class _tagiPad {
    /* 0x0000 */ signed int port;
};

// from iPad.cpp
// size: 0x4
class _tagiTRCPadInfo {
    /* 0x0000 */ enum _tagPadInit pad_init;
};

// from xScrFx.cpp
// size: 0x4
class _tagp2CamStaticAsset {
    /* 0x0000 */ unsigned int unused;
};

// from xScrFx.cpp
// size: 0x4
class _tagp2CamStaticFollowAsset {
    /* 0x0000 */ float rubber_band;
};

// from xScrFx.cpp
// size: 0xB0
class _tagxCam : public xBase {
    /* 0x0010 */ unsigned int valid_flags;
    /* 0x0014 */ enum _tagCamType cam_type;
    /* 0x0018 */ unsigned int flags;
    /* 0x0020 */ class _xMat4x3 mat;
    /* 0x0060 */ class _xQuat rot;
    /* 0x0070 */ float fov;
    /* 0x0074 */ float trans_time;
    /* 0x0078 */ enum _tagTransType trans_type;
    /* 0x007C */ class _xVec3 view_offset;
    /* 0x0088 */ class _tagCamInfo u;
    /* 0x00A0 */ class xCamAsset *asset;
};

// from xScrFx.cpp
// size: 0x18
class _tagxCamFollow {
    /* 0x0000 */ float rotation;
    /* 0x0004 */ float distance;
    /* 0x0008 */ float height;
    /* 0x000C */ float rubber_band;
    /* 0x0010 */ float start_speed;
    /* 0x0014 */ float end_speed;
};

// from xScrFx.cpp
// size: 0x18
class _tagxCamFollowAsset {
    /* 0x0000 */ float rotation;
    /* 0x0004 */ float distance;
    /* 0x0008 */ float height;
    /* 0x000C */ float rubber_band;
    /* 0x0010 */ float start_speed;
    /* 0x0014 */ float end_speed;
};

// from xScrFx.cpp
// size: 0x220
class _tagxCamFrame {
    /* 0x0000 */ class RwCamera *lo_cam;
    /* 0x0010 */ class _tagxCam orig_cam;
    /* 0x00C0 */ class _tagxCam hi_cam;
    /* 0x0170 */ class _xVec4 frustplane[8];
    /* 0x01F0 */ class _tagxCam *t_cam;
    /* 0x01F4 */ float t_time;
    /* 0x01F8 */ float g_time;
    /* 0x01FC */ class _xEnv *env;
    /* 0x0200 */ class _xVec3 *target_pos;
    /* 0x0204 */ class _xMat4x3 *target_frame;
    /* 0x0208 */ class _xVec3 pre_align_dir;
    /* 0x0214 */ class _tagxCam **cameras;
    /* 0x0218 */ unsigned int total_cameras;
};

// from xScrFx.cpp
// size: 0x8
class _tagxCamPath {
    /* 0x0000 */ unsigned int assetID;
    /* 0x0004 */ float time_end;
};

// from xScrFx.cpp
// size: 0xC
class _tagxCamPathAsset {
    /* 0x0000 */ unsigned int assetID;
    /* 0x0004 */ float time_end;
    /* 0x0008 */ float time_delay;
};

// from xScrFx.cpp
// size: 0x14
class _tagxCamShoulder {
    /* 0x0000 */ float distance;
    /* 0x0004 */ float height;
    /* 0x0008 */ float realign_speed;
    /* 0x000C */ float realign_delay;
    /* 0x0010 */ signed int state;
};

// from xScrFx.cpp
// size: 0x10
class _tagxCamShoulderAsset {
    /* 0x0000 */ float distance;
    /* 0x0004 */ float height;
    /* 0x0008 */ float realign_speed;
    /* 0x000C */ float realign_delay;
};

// from xScrFx.cpp
// size: 0x4
class _tagxCamStatic {
    /* 0x0000 */ unsigned int unused;
};

// from xScrFx.cpp
// size: 0x4
class _tagxCamStaticFollow {
    /* 0x0000 */ float rubber_band;
};

// from iPad.cpp
// size: 0x68
class _tagxPad {
    /* 0x0000 */ unsigned char value[22];
    /* 0x0016 */ unsigned char last_value[22];
    /* 0x002C */ unsigned int on;
    /* 0x0030 */ unsigned int pressed;
    /* 0x0034 */ unsigned int released;
    /* 0x0038 */ class _tagPadAnalog analog1;
    /* 0x003A */ class _tagPadAnalog analog2;
    /* 0x003C */ enum _tagPadState state;
    /* 0x0040 */ unsigned int flags;
    /* 0x0044 */ class _tagxRumble rumble_head;
    /* 0x0054 */ signed short port;
    /* 0x0056 */ signed short slot;
    /* 0x0058 */ class _tagiPad context;
    /* 0x005C */ float al2d_timer;
    /* 0x0060 */ float ar2d_timer;
    /* 0x0064 */ float d_timer;
};

// from iPad.cpp
// size: 0x10
class _tagxRumble {
    /* 0x0000 */ enum _tagRumbleType type;
    /* 0x0004 */ float seconds;
    /* 0x0008 */ class _tagxRumble *next;
    /* 0x000C */ signed short active;
    /* 0x000E */ unsigned short fxflags;
};

// from xScrFx.cpp
// size: 0x24
class _xBBox {
    /* 0x0000 */ class _xVec3 center;
    /* 0x000C */ class _xBox box;
};

// from xScrFx.cpp
// size: 0x38
class _xBound {
    /* 0x0000 */ unsigned char type;
    /* 0x0001 */ unsigned char pad[3];
    /* 0x0004 */ class _xSphere sph;
    /* 0x0004 */ class _xBBox box;
    /* 0x0004 */ class _xCylinder cyl;
    /* 0x0028 */ class _xMat4x3 *mat;
    /* 0x002C */ class xQCData qcd;
};

// from xScrFx.cpp
// size: 0x18
class _xBox {
    /* 0x0000 */ class _xVec3 upper;
    /* 0x000C */ class _xVec3 lower;
};

// from xScrFx.cpp
// size: 0x44
class _xCollis {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ unsigned int oid;
    /* 0x0008 */ void *optr;
    /* 0x000C */ float dist;
    /* 0x0010 */ class _xVec3 norm;
    /* 0x001C */ class _xVec3 tohit;
    /* 0x0028 */ class _xVec3 depen;
    /* 0x0034 */ class _xVec3 hdng;
    /* 0x0040 */ class _iCollis ixtra;
};

// from xScrFx.cpp
// size: 0x14
class _xCylinder {
    /* 0x0000 */ class _xVec3 center;
    /* 0x000C */ float r;
    /* 0x0010 */ float h;
};

// from xScrFx.cpp
// size: 0xE8
class _xEnt : public xBase {
    /* 0x0010 */ class p2EntAsset *asset;
    /* 0x0014 */ unsigned char flags;
    /* 0x0015 */ unsigned char subType;
    /* 0x0016 */ unsigned char pflags;
    /* 0x0017 */ unsigned char moreFlags;
    /* 0x0018 */ unsigned char pad0[3];
    /* 0x001B */ unsigned char num_ffx;
    /* 0x001C */ unsigned short idx;
    /* 0x001E */ unsigned short num_updates;
    /* 0x0020 */ float material_alpha;
    /* 0x0024 */ float fade_tmr;
    /* 0x0028 */ unsigned char collType;
    /* 0x0029 */ unsigned char collLev;
    /* 0x002A */ unsigned char chkby;
    /* 0x002B */ unsigned char penby;
    /* 0x002C */ class xModelInstance *model;
    /* 0x0030 */ void (*update)(class _xEnt *, class _xScene *, float);
    /* 0x0034 */ void (*bupdate)(class _xEnt *, class _xVec3 *);
    /* 0x0038 */ void (*move)(class _xEnt *, class _xScene *, float, class _xEntFrame *);
    /* 0x003C */ void (*render)(class _xEnt *);
    /* 0x0040 */ class _xEntFrame *frame;
    /* 0x0044 */ class _xEntCollis *collis;
    /* 0x0048 */ class _xBound bound;
    /* 0x0080 */ float updateTimer;
    /* 0x0084 */ float updateAcc;
    /* 0x0088 */ void (*updateTimerFunc)(class _xEnt *);
    /* 0x008C */ void (*transl)(class _xEnt *, class _xVec3 *);
    /* 0x0090 */ class iShadowCache ishadow;
    /* 0x00D8 */ class xSurface *surf;
    /* 0x00DC */ class xFFX *ffx;
    /* 0x00E0 */ class _xEnt *driver;
    /* 0x00E4 */ class _xEntShadow *ent_shadow;
};

// from xScrFx.cpp
// size: 0x4FC
class _xEntCollis {
    /* 0x0000 */ unsigned char chk;
    /* 0x0001 */ unsigned char pen;
    /* 0x0002 */ unsigned char env_sidx;
    /* 0x0003 */ unsigned char env_eidx;
    /* 0x0004 */ unsigned char npc_sidx;
    /* 0x0005 */ unsigned char npc_eidx;
    /* 0x0006 */ unsigned char dyn_sidx;
    /* 0x0007 */ unsigned char dyn_eidx;
    /* 0x0008 */ unsigned char stat_sidx;
    /* 0x0009 */ unsigned char stat_eidx;
    /* 0x000A */ unsigned char trig_idx;
    /* 0x000B */ unsigned char idx;
    /* 0x000C */ class _xCollis colls[18];
    /* 0x04D4 */ void (*post)(class _xEnt *, class _xScene *, float, class _xEntCollis *);
    /* 0x04D8 */ unsigned int (*depenq)(class _xEnt *, class _xEnt *, class _xScene *, float, class _xCollis *);
    /* 0x04DC */ unsigned char trigs_cur[8];
    /* 0x04E4 */ unsigned char trigs_prev[8];
    /* 0x04EC */ unsigned char trigs_entered[8];
    /* 0x04F4 */ unsigned char trigs_exited[8];
};

// from xScrFx.cpp
// size: 0x50
class _xEntDrive {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ float otm;
    /* 0x0008 */ float otmr;
    /* 0x000C */ float os;
    /* 0x0010 */ float tm;
    /* 0x0014 */ float tmr;
    /* 0x0018 */ float s;
    /* 0x001C */ class _xEnt *odriver;
    /* 0x0020 */ class _xEnt *driver;
    /* 0x0024 */ class _xEnt *driven;
    /* 0x0028 */ class _xVec3 op;
    /* 0x0034 */ class _xVec3 p;
    /* 0x0040 */ class _xVec3 q;
    /* 0x004C */ float yaw;
};

// from zScene.cpp
// size: 0x44
class _xEntERData {
    /* 0x0000 */ class _xVec3 a;
    /* 0x000C */ class _xVec3 b;
    /* 0x0018 */ class _xVec3 dir;
    /* 0x0024 */ float et;
    /* 0x0028 */ float wet;
    /* 0x002C */ float rt;
    /* 0x0030 */ float wrt;
    /* 0x0034 */ float p;
    /* 0x0038 */ float brt;
    /* 0x003C */ float ert;
    /* 0x0040 */ signed int state;
};

// from xScrFx.cpp
// size: 0xF0
class _xEntFrame {
    /* 0x0000 */ class _xMat4x3 mat;
    /* 0x0040 */ class _xMat4x3 dmat;
    /* 0x0080 */ class _xVec3 oldpos;
    /* 0x008C */ class _xVec3 oldvel;
    /* 0x0098 */ class _xRot oldrot;
    /* 0x00A8 */ class _xRot drot;
    /* 0x00B8 */ class _xRot rot;
    /* 0x00C8 */ class _xVec3 dpos;
    /* 0x00D4 */ class _xVec3 dvel;
    /* 0x00E0 */ class _xVec3 vel;
    /* 0x00EC */ unsigned int mode;
};

// from zScene.cpp
// size: 0x40
class _xEntMPData {
    /* 0x0000 */ float curdist;
    /* 0x0004 */ float speed;
    /* 0x0008 */ class _xMovePoint *dest;
    /* 0x000C */ class _xMovePoint *src;
    /* 0x0010 */ class xSpline3 *spl;
    /* 0x0014 */ float dist;
    /* 0x0018 */ unsigned int padalign;
    /* 0x0020 */ class _xQuat aquat;
    /* 0x0030 */ class _xQuat bquat;
};

// from zScene.cpp
// size: 0x4C
class _xEntMechData {
    /* 0x0000 */ class _xVec3 apos;
    /* 0x000C */ class _xVec3 bpos;
    /* 0x0018 */ class _xVec3 dir;
    /* 0x0024 */ float arot;
    /* 0x0028 */ float brot;
    /* 0x002C */ float ss;
    /* 0x0030 */ float sr;
    /* 0x0034 */ signed int state;
    /* 0x0038 */ float tsfd;
    /* 0x003C */ float trfd;
    /* 0x0040 */ float tsbd;
    /* 0x0044 */ float trbd;
    /* 0x0048 */ float *rotptr;
};

// from zScene.cpp
// size: 0x80
class _xEntMotion {
    /* 0x0000 */ class p2EntMotionAsset *asset;
    /* 0x0004 */ unsigned char type;
    /* 0x0005 */ unsigned char pad;
    /* 0x0006 */ unsigned short flags;
    /* 0x0008 */ float t;
    /* 0x000C */ float tmr;
    /* 0x0010 */ float d;
    /* 0x0020 */ class _xEntERData er;
    /* 0x0020 */ class _xEntOrbitData orb;
    /* 0x0020 */ class _xEntSplineData spl;
    /* 0x0020 */ class _xEntMPData mp;
    /* 0x0020 */ class _xEntMechData mech;
    /* 0x0020 */ class _xEntPenData pen;
    /* 0x0070 */ class _xEnt *owner;
    /* 0x0074 */ class _xEnt *target;
};

// from zScene.cpp
// size: 0x28
class _xEntOrbitData {
    /* 0x0000 */ class _xVec3 orig;
    /* 0x000C */ class _xVec3 c;
    /* 0x0018 */ float a;
    /* 0x001C */ float b;
    /* 0x0020 */ float p;
    /* 0x0024 */ float w;
};

// from zScene.cpp
// size: 0x50
class _xEntPenData {
    /* 0x0000 */ class _xVec3 top;
    /* 0x000C */ float w;
    /* 0x0010 */ class _xMat4x3 omat;
};

// from xScrFx.cpp
// size: 0x18
class _xEntShadow {
    /* 0x0000 */ class _xVec3 pos;
    /* 0x000C */ class _xVec3 vec;
};

// from zScene.cpp
// size: 0x4
class _xEntSplineData {
    /* 0x0000 */ signed int unknown;
};

// from xScrFx.cpp
// size: 0x40
class _xEnv {
    /* 0x0000 */ class iEnv *geom;
    /* 0x0010 */ class iEnv ienv;
};

// from iLight.cpp
// size: 0x10
class _xFColor {
    /* 0x0000 */ float r;
    /* 0x0004 */ float g;
    /* 0x0008 */ float b;
    /* 0x000C */ float a;
};

// from xScrFx.cpp
// size: 0x20
class _xFadeData {
    /* 0x0000 */ signed int active;
    /* 0x0004 */ signed int hold;
    /* 0x0008 */ class iColor_tag src;
    /* 0x000C */ class iColor_tag dest;
    /* 0x0010 */ float time_passed;
    /* 0x0014 */ float time_total;
    /* 0x0018 */ void (*cb)();
    /* 0x001C */ signed int fancy;
};

// from xMath3.cpp
// size: 0x2C
class _xIsect {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ float penned;
    /* 0x0008 */ float contained;
    /* 0x000C */ float lapped;
    /* 0x0010 */ class _xVec3 point;
    /* 0x001C */ class _xVec3 norm;
    /* 0x0028 */ float dist;
};

// from xQuickCull.cpp
// size: 0x18
class _xLine3 {
    /* 0x0000 */ class _xVec3 p1;
    /* 0x000C */ class _xVec3 p2;
};

// from zParEmitter.cpp
// size: 0x10
class _xMat2x2 {
    /* 0x0000 */ class _xVec2 right;
    /* 0x0008 */ class _xVec2 up;
};

// from xMath3.cpp
// size: 0x30
class _xMat3x3 {
    /* 0x0000 */ class _xVec3 right;
    /* 0x000C */ signed int flags;
    /* 0x0010 */ class _xVec3 up;
    /* 0x001C */ unsigned int pad1;
    /* 0x0020 */ class _xVec3 at;
    /* 0x002C */ unsigned int pad2;
};

// from xScrFx.cpp
// size: 0x40
class _xMat4x3 {
    /* 0x0000 */ class _xVec3 right;
    /* 0x000C */ signed int flags;
    /* 0x0010 */ class _xVec3 up;
    /* 0x001C */ unsigned int pad1;
    /* 0x0020 */ class _xVec3 at;
    /* 0x002C */ unsigned int pad2;
    /* 0x0030 */ class _xVec3 pos;
    /* 0x003C */ unsigned int pad3;
};

// from zScene.cpp
// size: 0x30
class _xMovePoint : public xBase {
    /* 0x0010 */ class p2MovePointAsset *asset;
    /* 0x0014 */ class _xVec3 *pos;
    /* 0x0018 */ class _xMovePoint **nodes;
    /* 0x001C */ class _xMovePoint *prev;
    /* 0x0020 */ unsigned int node_wt_sum;
    /* 0x0024 */ unsigned char on;
    /* 0x0025 */ unsigned char flg_props;
    /* 0x0026 */ unsigned char pad[2];
    /* 0x0028 */ float delay;
    /* 0x002C */ class xSpline3 *spl;
};

// from iMath3.cpp
// size: 0x10
class _xPlane {
    /* 0x0000 */ class _xVec3 norm;
    /* 0x000C */ float d;
};

// from xScrFx.cpp
// size: 0x10
class _xQuat {
    /* 0x0000 */ class _xVec3 v;
    /* 0x000C */ float s;
};

// from iMath3.cpp
// size: 0x24
class _xRay3 {
    /* 0x0000 */ class _xVec3 origin;
    /* 0x000C */ class _xVec3 dir;
    /* 0x0018 */ float min_t;
    /* 0x001C */ float max_t;
    /* 0x0020 */ signed int flags;
};

// from xModel.cpp
// size: 0x10
class _xRect {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float w;
    /* 0x000C */ float h;
};

// from xScrFx.cpp
// size: 0x10
class _xRot {
    /* 0x0000 */ class _xVec3 axis;
    /* 0x000C */ float angle;
};

// from xScrFx.cpp
// size: 0x6C
class _xScene {
    /* 0x0000 */ unsigned short flags;
    /* 0x0002 */ unsigned short num_ents;
    /* 0x0004 */ unsigned short num_trigs;
    /* 0x0006 */ unsigned short num_stats;
    /* 0x0008 */ unsigned short num_dyns;
    /* 0x000A */ unsigned short num_npcs;
    /* 0x000C */ unsigned short num_act_ents;
    /* 0x000E */ unsigned short num_nact_ents;
    /* 0x0010 */ float gravity;
    /* 0x0014 */ float drag;
    /* 0x0018 */ float friction;
    /* 0x001C */ unsigned short num_ents_allocd;
    /* 0x001E */ unsigned short num_trigs_allocd;
    /* 0x0020 */ unsigned short num_stats_allocd;
    /* 0x0022 */ unsigned short num_dyns_allocd;
    /* 0x0024 */ unsigned short num_npcs_allocd;
    /* 0x0028 */ class _xEnt **trigs;
    /* 0x002C */ class _xEnt **stats;
    /* 0x0030 */ class _xEnt **dyns;
    /* 0x0034 */ class _xEnt **npcs;
    /* 0x0038 */ class _xEnt **act_ents;
    /* 0x003C */ class _xEnt **nact_ents;
    /* 0x0040 */ class _xEnv *env;
    /* 0x0044 */ class xMemPool mempool;
    /* 0x0060 */ class xBase * (*resolvID)(unsigned int);
    /* 0x0064 */ char * (*base2Name)(class xBase *);
    /* 0x0068 */ char * (*id2Name)(unsigned int);
};

// from xScrFx.cpp
// size: 0x10
class _xSphere {
    /* 0x0000 */ class _xVec3 center;
    /* 0x000C */ float r;
};

// from iMath3.cpp
// size: 0x24
class _xTri3 {
    /* 0x0000 */ class _xVec3 p1;
    /* 0x000C */ class _xVec3 p2;
    /* 0x0018 */ class _xVec3 p3;
};

// from xCamera.cpp
// size: 0x8
class _xVec2 {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
};

// from xScrFx.cpp
// size: 0xC
class _xVec3 {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
};

// from xScrFx.cpp
// size: 0x10
class _xVec4 {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
    /* 0x000C */ float w;
};

// from zConditional.cpp
// size: 0x14
class _zConditional : public xBase {
    /* 0x0010 */ class zCondAsset *asset;
};

// from zEntPickup.cpp
// size: 0x18
class _zCounter : public xBase {
    /* 0x0010 */ class zCounterAsset *asset;
    /* 0x0014 */ signed short count;
    /* 0x0016 */ unsigned char state;
    /* 0x0017 */ unsigned char pad;
};

// from xScrFx.cpp
// size: 0x20
class _zCutsceneMgr : public xBase {
    /* 0x0010 */ class zCutsceneMgrAsset *tasset;
    /* 0x0014 */ class xCutscene *csn;
    /* 0x0018 */ unsigned int stop;
    /* 0x001C */ class zCutsceneZbufferHack *zhack;
};

// from xScrFx.cpp
// size: 0xEC
class _zEnt : public _xEnt {
    /* 0x00E8 */ class xAnimTable *atbl;
};

// from zEnt.cpp
// size: 0x118
class _zEntBubble : public _zEnt {
    /* 0x00EC */ unsigned int state;
    /* 0x00F0 */ class _zEnt *villain;
    /* 0x00F4 */ float bubbleScale;
    /* 0x00F8 */ float timer;
    /* 0x00FC */ float stuckTimer;
    /* 0x0100 */ class _xVec3 envelopStart;
    /* 0x010C */ float fx_timer;
    /* 0x0110 */ float fx_scale;
    /* 0x0114 */ class xPar *particle;
};

// from zEntButton.cpp
// size: 0x190
class _zEntButton : public _zEnt {
    /* 0x00EC */ class zEntButtonAsset *basset;
    /* 0x00F0 */ class _xEntMotion motion;
    /* 0x0170 */ unsigned int state;
    /* 0x0174 */ float speed;
    /* 0x0178 */ unsigned int oldState;
    /* 0x017C */ signed int oldMotState;
    /* 0x0180 */ unsigned int counter;
};

// from zEntDestructObj.cpp
// size: 0x100
class _zEntDestructObj : public _zEnt {
    /* 0x00EC */ class zEntDestructObjAsset *dasset;
    /* 0x00F0 */ unsigned int state;
    /* 0x00F4 */ unsigned int healthCnt;
    /* 0x00F8 */ float fx_timer;
    /* 0x00FC */ class _zParEmitter *fx_emitter;
};

// from xScrFx.cpp
// size: 0x144
class _zEntHangable : public _zEnt {
    /* 0x00EC */ class p2EntHangableAsset *hangInfo;
    /* 0x00F0 */ class _xVec3 pivot;
    /* 0x00FC */ class _xVec3 endpos;
    /* 0x0108 */ class _xVec3 vel;
    /* 0x0114 */ class _xVec3 swingplane;
    /* 0x0120 */ float grabTimer;
    /* 0x0124 */ float spin;
    /* 0x0128 */ unsigned int state;
    /* 0x012C */ class _zEnt *shaggy;
    /* 0x0130 */ signed int enabled;
    /* 0x0134 */ class _zEnt *follow;
    /* 0x0138 */ signed int moving;
    /* 0x013C */ float candle_timer;
    /* 0x0140 */ signed int candle_state;
};

// from zScene.cpp
// size: 0x168
class _zEntPickup : public _zEnt {
    /* 0x00EC */ unsigned int state;
    /* 0x00F0 */ class zAssetPickup *p;
    /* 0x00F4 */ void *anim;
    /* 0x00F8 */ float animTime;
    /* 0x00FC */ float timer;
    /* 0x0100 */ float shake_timer;
    /* 0x0104 */ class _xVec3 shake_pos;
    /* 0x0110 */ signed int snackGateInfoDisplayed;
    /* 0x0114 */ float fx_timer;
    /* 0x0118 */ class xPar *fx_par;
    /* 0x011C */ class _zParEmitter *fx_emit;
    /* 0x0120 */ float fx_scale;
    /* 0x0124 */ class _xVec3 grab_pos;
    /* 0x0130 */ unsigned int flyflags;
    /* 0x0134 */ signed int flg_opts;
    /* 0x0138 */ class _xVec3 vel;
    /* 0x0144 */ class _xVec3 droppos;
    /* 0x0150 */ class xBase *followTarget;
    /* 0x0154 */ class _xVec3 followOffset;
    /* 0x0160 */ class xBase *dropParent;
    /* 0x0164 */ class _xEnt *useThisEntPos;
};

// from zEntPlayer.cpp
// size: 0xF4
class _zEntSimpleObj : public _zEnt {
    /* 0x00EC */ class p2SimpleObjAsset *sasset;
    /* 0x00F0 */ unsigned int sflags;
};

// from zGame.cpp
// size: 0x14
class _zEnv : public xBase {
    /* 0x0010 */ class p2EnvAsset *easset;
};

// from zFog.cpp
// size: 0x14
class _zFog : public xBase {
    /* 0x0010 */ class zFogAsset *tasset;
};

// from xEnt.cpp
// size: 0x14
class _zGroup : public xBase {
    /* 0x0010 */ class zGroupAsset *asset;
};

// from zEnt.cpp
// size: 0x24
class _zGust : public xBase {
    /* 0x0010 */ unsigned int flags;
    /* 0x0014 */ class zGustAsset *asset;
    /* 0x0018 */ class _zVolume *volume;
    /* 0x001C */ class _zVolume *fx_volume;
    /* 0x0020 */ float debris_timer;
};

// from zMain.cpp
// size: 0x8
class _zHipDepend {
    /* 0x0000 */ unsigned int sceneID;
    /* 0x0004 */ unsigned int dependID;
};

// from zHud.cpp
// size: 0x118
class _zHudElement {
    /* 0x0000 */ enum eHudElement hudType;
    /* 0x0004 */ void (*hudInit)();
    /* 0x0008 */ void (*hudSetup)(class _xScene *);
    /* 0x000C */ void (*hudUpdate)(class _xScene *, float);
    /* 0x0010 */ void (*hudRender)();
    /* 0x0014 */ unsigned int count;
    /* 0x0018 */ char text[256];
};

// from zLight.cpp
// size: 0x64
class _zLight : public xBase {
    /* 0x0010 */ unsigned int flags;
    /* 0x0014 */ class zLightAsset *tasset;
    /* 0x0018 */ class iLight light;
    /* 0x0054 */ class xBase *attached_to;
    /* 0x0058 */ signed int true_idx;
    /* 0x005C */ float *reg;
    /* 0x0060 */ signed int effect_idx;
};

// from zScene.cpp
// size: 0x34
class _zMovePoint : public _xMovePoint {
    /* 0x0030 */ unsigned int unused;
};

// from zScene.cpp
// size: 0x50
class _zParEmitter : public xBase {
    /* 0x0010 */ class zParEmitterAsset *tasset;
    /* 0x0014 */ class xParGroup *group;
    /* 0x0018 */ unsigned char emit_flags;
    /* 0x0019 */ unsigned char emit_pad[3];
    /* 0x001C */ float emit_interval_current;
    /* 0x0020 */ unsigned char rot[3];
    /* 0x0024 */ class iModelTag tag;
    /* 0x0044 */ void *attachTo;
    /* 0x0048 */ void *parSys;
    /* 0x004C */ void *emit_volume;
};

// from zEntVillain.cpp
// size: 0x54
class _zParEmitterCustomSettings {
    /* 0x0000 */ unsigned int custom_flags;
    /* 0x0004 */ unsigned char count;
    /* 0x0005 */ unsigned char count_variation;
    /* 0x0008 */ float interval;
    /* 0x000C */ unsigned int attachToID;
    /* 0x0010 */ class _xVec3 pos;
    /* 0x001C */ class _xVec3 vel;
    /* 0x0028 */ float vel_angle_variation;
    /* 0x002C */ unsigned char color_birth[4];
    /* 0x0030 */ unsigned char color_death[4];
    /* 0x0034 */ float size_birth;
    /* 0x0038 */ float size_birth_variation;
    /* 0x003C */ float size_death;
    /* 0x0040 */ float life;
    /* 0x0044 */ float life_variation;
    /* 0x0048 */ unsigned char rot[3];
    /* 0x004C */ float xzcircle_radius;
    /* 0x0050 */ float emit_interval_current;
};

// from zScene.cpp
// size: 0x24
class _zParGroup : public xParGroup {
};

// from zPendulum.cpp
// size: 0x180
class _zPendulum : public _zEnt {
    /* 0x00F0 */ class _xEntMotion motion;
    /* 0x0170 */ float lt;
    /* 0x0174 */ float q1t;
    /* 0x0178 */ float q3t;
};

// from zScene.cpp
// size: 0x190
class _zPlatform : public _zEnt {
    /* 0x00EC */ class p2PlatformAsset *passet;
    /* 0x00F0 */ class _xEntMotion motion;
    /* 0x0170 */ unsigned short state;
    /* 0x0172 */ unsigned short plat_flags;
    /* 0x0174 */ float tmr;
    /* 0x0178 */ signed int ctr;
    /* 0x017C */ class _xMovePoint *src;
    /* 0x0180 */ class xModelInstance *am;
    /* 0x0184 */ class xModelInstance *bm;
    /* 0x0188 */ signed int moving;
};

// from xScrFx.cpp
// size: 0x14
class _zPortal : public xBase {
    /* 0x0010 */ class zPortalAsset *passet;
};

// from zScript.cpp
// size: 0x1C
class _zScript : public xBase {
    /* 0x0010 */ class zScriptAsset *tasset;
    /* 0x0014 */ unsigned char state;
    /* 0x0015 */ unsigned char more;
    /* 0x0016 */ unsigned char pad[2];
    /* 0x0018 */ float time;
};

// from zTimer.cpp
// size: 0x1C
class _zTimer : public xBase {
    /* 0x0010 */ class zTimerAsset *tasset;
    /* 0x0014 */ unsigned char state;
    /* 0x0015 */ unsigned char pad[3];
    /* 0x0018 */ float secondsLeft;
};

// from zEntPickup.cpp
// size: 0xF8
class _zUI : public _zEnt {
    /* 0x00EC */ class zUIAsset *sasset;
    /* 0x00F0 */ unsigned int uiFlags;
    /* 0x00F4 */ unsigned int uiButton;
};

// from zUIFont.cpp
// size: 0x100
class _zUIFont : public _zUI {
    /* 0x00F8 */ class zUIFontAsset *fasset;
    /* 0x00FC */ unsigned short uiFontFlags;
    /* 0x00FE */ unsigned short uiFontHackFlags;
};

// from zEntPlayer.cpp
// size: 0x14
class _zVolume : public xBase {
    /* 0x0010 */ class zVolumeAsset *asset;
};

// from ctworld_.c
// size: 0x8
class buildCallBackData {
    /* 0x0000 */ class RpCollisionBuildParam *param;
    /* 0x0004 */ signed int error;
};

// from iAnimSKB.cpp
// size: 0x1C
class iAnimSKBHeader {
    /* 0x0000 */ unsigned int Magic;
    /* 0x0004 */ unsigned int Flags;
    /* 0x0008 */ unsigned short BoneCount;
    /* 0x000A */ unsigned short TimeCount;
    /* 0x000C */ unsigned int KeyCount;
    /* 0x0010 */ float Scale[3];
};

// from iAnimSKB.cpp
// size: 0x10
class iAnimSKBKey {
    /* 0x0000 */ unsigned short TimeIndex;
    /* 0x0002 */ signed short Quat[4];
    /* 0x000A */ signed short Tran[3];
};

// from xScrFx.cpp
// size: 0x4
class iColor_tag {
    /* 0x0000 */ unsigned char r;
    /* 0x0001 */ unsigned char g;
    /* 0x0002 */ unsigned char b;
    /* 0x0003 */ unsigned char a;
};

// from xScrFx.cpp
// size: 0x30
class iEnv {
    /* 0x0000 */ class RpWorld *world;
    /* 0x0004 */ class RpWorld *collision;
    /* 0x0008 */ class RpWorld *fx;
    /* 0x000C */ class RpWorld *camera;
    /* 0x0010 */ class RpLight *light[2];
    /* 0x0018 */ class RwFrame *light_frame[2];
    /* 0x0020 */ signed int memlvl;
};

// from xScrFx.cpp
// size: 0x1C
class iFogParams {
    /* 0x0000 */ enum RwFogType type;
    /* 0x0004 */ float start;
    /* 0x0008 */ float stop;
    /* 0x000C */ float density;
    /* 0x0010 */ class RwRGBA fogcolor;
    /* 0x0014 */ class RwRGBA bgcolor;
    /* 0x0018 */ unsigned char *table;
};

// from xFont.cpp
// size: 0x8
class iFontInfo {
    /* 0x0000 */ class RwTexture *texture;
    /* 0x0004 */ class RwRaster *raster;
};

// from xFont.cpp
// size: 0x2
class iFontSpacing {
    /* 0x0000 */ unsigned char ox;
    /* 0x0001 */ unsigned char dx;
};

// from iLight.cpp
// size: 0x3C
class iLight {
    /* 0x0000 */ unsigned int type;
    /* 0x0004 */ class RpLight *hw;
    /* 0x0008 */ class _xSphere sph;
    /* 0x0018 */ float radius_sq;
    /* 0x001C */ class _xFColor color;
    /* 0x002C */ class _xVec3 dir;
    /* 0x0038 */ float coneangle;
};

// from xScrFx.cpp
// size: 0x20
class iModelTag {
    /* 0x0000 */ class _xVec3 v;
    /* 0x000C */ unsigned int matidx;
    /* 0x0010 */ float wt[4];
};

// from xScrFx.cpp
// size: 0x48
class iShadowCache {
    /* 0x0000 */ class _xVec3 pos;
    /* 0x000C */ class _xVec3 right;
    /* 0x0018 */ class _xVec3 corner[4];
};

// from iSnd.cpp
// size: 0x10
class iSndCutsceneCallbackInfo {
    /* 0x0000 */ unsigned int addr;
    /* 0x0004 */ unsigned int id;
    /* 0x0008 */ unsigned int seek;
    /* 0x000C */ unsigned int size;
};

// from ms4sound.cpp
// size: 0x18
class iSndFileInfo {
    /* 0x0000 */ unsigned int assetID;
    /* 0x0004 */ unsigned short flags;
    /* 0x0006 */ unsigned short freq;
    /* 0x0008 */ unsigned int lsn;
    /* 0x000C */ unsigned short offset;
    /* 0x000E */ unsigned short ID;
    /* 0x0010 */ unsigned int size;
    /* 0x0014 */ unsigned int hip_idx;
};

// from iSnd.cpp
// size: 0xC
class iSndInfo {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ class iSndVol vol;
    /* 0x0008 */ unsigned int pitch;
};

// from iSnd.cpp
// size: 0x4
class iSndVol {
    /* 0x0000 */ signed short volL;
    /* 0x0002 */ signed short volR;
};

// from ctbsp_.c
// size: 0x8
class nodeInfo {
    /* 0x0000 */ unsigned int type;
    /* 0x0004 */ unsigned int index;
};

// from xScrFx.cpp
// size: 0x8
class p2BaseAsset {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ unsigned char baseType;
    /* 0x0005 */ unsigned char linkCount;
    /* 0x0006 */ unsigned short baseFlags;
};

// from xScrFx.cpp
// size: 0x50
class p2EntAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned char flags;
    /* 0x0009 */ unsigned char subtype;
    /* 0x000A */ unsigned char pflags;
    /* 0x000B */ unsigned char moreFlags;
    /* 0x000C */ unsigned int surfaceID;
    /* 0x0010 */ class _xVec3 ang;
    /* 0x001C */ class _xVec3 pos;
    /* 0x0028 */ class _xVec3 scale;
    /* 0x0034 */ float redMult;
    /* 0x0038 */ float greenMult;
    /* 0x003C */ float blueMult;
    /* 0x0040 */ float seeThru;
    /* 0x0044 */ float seeThruSpeed;
    /* 0x0048 */ unsigned int modelInfoID;
    /* 0x004C */ unsigned int animListID;
};

// from xScrFx.cpp
// size: 0x20
class p2EntHangableAsset {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ float pivotOffset;
    /* 0x0008 */ float leverArm;
    /* 0x000C */ float gravity;
    /* 0x0010 */ float accel;
    /* 0x0014 */ float decay;
    /* 0x0018 */ float grabDelay;
    /* 0x001C */ float stopDecel;
};

// from zScene.cpp
// size: 0x30
class p2EntMotionAsset {
    /* 0x0000 */ unsigned char type;
    /* 0x0001 */ unsigned char pad;
    /* 0x0002 */ unsigned short flags;
    /* 0x0004 */ class p2EntMotionERData er;
    /* 0x0004 */ class p2EntMotionOrbitData orb;
    /* 0x0004 */ class p2EntMotionSplineData spl;
    /* 0x0004 */ class p2EntMotionMPData mp;
    /* 0x0004 */ class p2EntMotionMechData mech;
    /* 0x0004 */ class p2EntMotionPenData pen;
};

// from zScene.cpp
// size: 0x28
class p2EntMotionERData {
    /* 0x0000 */ class _xVec3 ret_pos;
    /* 0x000C */ class _xVec3 ext_dpos;
    /* 0x0018 */ float ext_tm;
    /* 0x001C */ float ext_wait_tm;
    /* 0x0020 */ float ret_tm;
    /* 0x0024 */ float ret_wait_tm;
};

// from zScene.cpp
// size: 0xC
class p2EntMotionMPData {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ unsigned int mp_id;
    /* 0x0008 */ float speed;
};

// from zScene.cpp
// size: 0x2C
class p2EntMotionMechData {
    /* 0x0000 */ unsigned char type;
    /* 0x0001 */ unsigned char flags;
    /* 0x0002 */ unsigned char sld_axis;
    /* 0x0003 */ unsigned char rot_axis;
    /* 0x0004 */ float sld_dist;
    /* 0x0008 */ float sld_tm;
    /* 0x000C */ float sld_acc_tm;
    /* 0x0010 */ float sld_dec_tm;
    /* 0x0014 */ float rot_dist;
    /* 0x0018 */ float rot_tm;
    /* 0x001C */ float rot_acc_tm;
    /* 0x0020 */ float rot_dec_tm;
    /* 0x0024 */ float ret_delay;
    /* 0x0028 */ float post_ret_delay;
};

// from zScene.cpp
// size: 0x18
class p2EntMotionOrbitData {
    /* 0x0000 */ class _xVec3 center;
    /* 0x000C */ float w;
    /* 0x0010 */ float h;
    /* 0x0014 */ float period;
};

// from zScene.cpp
// size: 0x14
class p2EntMotionPenData {
    /* 0x0000 */ unsigned char flags;
    /* 0x0001 */ unsigned char plane;
    /* 0x0002 */ unsigned char pad[2];
    /* 0x0004 */ float len;
    /* 0x0008 */ float range;
    /* 0x000C */ float period;
    /* 0x0010 */ float phase;
};

// from zScene.cpp
// size: 0x4
class p2EntMotionSplineData {
    /* 0x0000 */ signed int unknown;
};

// from zEntPickup.cpp
// size: 0x8
class p2EntPickupAsset {
    /* 0x0000 */ unsigned int pickupHash;
    /* 0x0004 */ unsigned short pickupFlags;
    /* 0x0006 */ unsigned short pickupValue;
};

// from zEntPlayer.cpp
// size: 0x40
class p2EntTriggerAsset {
    /* 0x0000 */ class _xVec3 p[4];
    /* 0x0030 */ class _xVec3 direction;
    /* 0x003C */ unsigned int flags;
};

// from zEnt.cpp
// size: 0x74
class p2EntVillainAsset {
    /* 0x0000 */ float activateRadius;
    /* 0x0004 */ float activateFOV;
    /* 0x0008 */ float detectHeight;
    /* 0x000C */ float detectHeightOffset;
    /* 0x0010 */ float speedMovement;
    /* 0x0014 */ float speedPursue;
    /* 0x0018 */ float speedTurn;
    /* 0x001C */ float pursuitRange;
    /* 0x0020 */ unsigned short durDazedState;
    /* 0x0022 */ unsigned short durGloatState;
    /* 0x0024 */ unsigned short durGummedState;
    /* 0x0026 */ unsigned short durBubbleState;
    /* 0x0028 */ unsigned char hitpoints;
    /* 0x0029 */ unsigned char behaviorState;
    /* 0x002A */ unsigned short pad;
    /* 0x002C */ unsigned int villFlags;
    /* 0x0030 */ float lobSpeed;
    /* 0x0034 */ float lobDurReload;
    /* 0x0038 */ float lobRange;
    /* 0x003C */ unsigned int lobSalvo;
    /* 0x0040 */ unsigned int projectileTypeID;
    /* 0x0044 */ unsigned int mvptBullseyeID;
    /* 0x0048 */ float lobArcness;
    /* 0x004C */ float lobHeavy;
    /* 0x0050 */ float extenderRange;
    /* 0x0054 */ float extenderWidth;
    /* 0x0058 */ float extenderDuration;
    /* 0x005C */ float extenderRate;
    /* 0x0060 */ float extenderReloadTime;
    /* 0x0064 */ unsigned int movePointAssetID;
    /* 0x0068 */ unsigned int pathAssetID;
    /* 0x006C */ signed int minPlayerPowerups;
    /* 0x0070 */ signed int minGameDifficulty;
};

// from zGame.cpp
// size: 0x40
class p2EnvAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned int bspAssetID;
    /* 0x000C */ unsigned int startCameraAssetID;
    /* 0x0010 */ unsigned int climateFlags;
    /* 0x0014 */ float climateStrengthMin;
    /* 0x0018 */ float climateStrengthMax;
    /* 0x001C */ float padF1;
    /* 0x0020 */ float padF2;
    /* 0x0024 */ float padF3;
    /* 0x0028 */ unsigned int bspCollisionAssetID;
    /* 0x002C */ unsigned int bspFXAssetID;
    /* 0x0030 */ unsigned int bspCameraAssetID;
    /* 0x0034 */ unsigned int bspMapperID;
    /* 0x0038 */ unsigned int bspMapperCollisionID;
    /* 0x003C */ unsigned int bspMapperFXID;
};

// from xScrFx.cpp
// size: 0x20
class p2LinkAsset {
    /* 0x0000 */ unsigned short srcEvent;
    /* 0x0002 */ unsigned short dstEvent;
    /* 0x0004 */ unsigned int dstAssetID;
    /* 0x0008 */ float param[4];
    /* 0x0018 */ unsigned int paramWidgetAssetID;
    /* 0x001C */ unsigned int chkAssetID;
};

// from zGame.cpp
// size: 0xC
class p2MarkerAsset {
    /* 0x0000 */ class _xVec3 pos;
};

// from zScene.cpp
// size: 0x20
class p2MovePointAsset : public p2BaseAsset {
    /* 0x0008 */ class _xVec3 pos;
    /* 0x0014 */ unsigned short wt;
    /* 0x0016 */ unsigned char on;
    /* 0x0017 */ unsigned char bezIndex;
    /* 0x0018 */ unsigned char flg_props;
    /* 0x0019 */ unsigned char pad;
    /* 0x001A */ unsigned short numPoints;
    /* 0x001C */ float delay;
};

// from zScene.cpp
// size: 0x28
class p2PlatformAsset {
    /* 0x0000 */ unsigned char type;
    /* 0x0001 */ unsigned char pad;
    /* 0x0002 */ unsigned short flags;
    /* 0x0004 */ class p2PlatformERData er;
    /* 0x0004 */ class p2PlatformOrbitData orb;
    /* 0x0004 */ class p2PlatformSplineData spl;
    /* 0x0004 */ class p2PlatformMPData mp;
    /* 0x0004 */ class p2PlatformMechData mech;
    /* 0x0004 */ class p2PlatformPenData pen;
    /* 0x0004 */ class p2PlatformConvBeltData cb;
    /* 0x0004 */ class p2PlatformFallingData fall;
    /* 0x0004 */ class p2PlatformFRData fr;
    /* 0x0004 */ class p2PlatformBreakawayData ba;
    /* 0x0004 */ class p2PlatformSpringboardData sb;
    /* 0x0004 */ class p2PlatformTeeterData teet;
};

// from zScene.cpp
// size: 0xC
class p2PlatformBreakawayData {
    /* 0x0000 */ float ba_delay;
    /* 0x0004 */ unsigned int bustModelID;
    /* 0x0008 */ float reset_delay;
};

// from zScene.cpp
// size: 0x4
class p2PlatformConvBeltData {
    /* 0x0000 */ float speed;
};

// from zScene.cpp
// size: 0x4
class p2PlatformERData {
    /* 0x0000 */ signed int nodata;
};

// from zScene.cpp
// size: 0x10
class p2PlatformFRData {
    /* 0x0000 */ float fspeed;
    /* 0x0004 */ float rspeed;
    /* 0x0008 */ float ret_delay;
    /* 0x000C */ float post_ret_delay;
};

// from zScene.cpp
// size: 0x8
class p2PlatformFallingData {
    /* 0x0000 */ float speed;
    /* 0x0004 */ unsigned int bustModelID;
};

// from zScene.cpp
// size: 0x4
class p2PlatformMPData {
    /* 0x0000 */ signed int nodata;
};

// from zScene.cpp
// size: 0x4
class p2PlatformMechData {
    /* 0x0000 */ signed int nodata;
};

// from zScene.cpp
// size: 0x4
class p2PlatformOrbitData {
    /* 0x0000 */ signed int nodata;
};

// from zScene.cpp
// size: 0x4
class p2PlatformPenData {
    /* 0x0000 */ signed int nodata;
};

// from zScene.cpp
// size: 0x4
class p2PlatformSplineData {
    /* 0x0000 */ signed int nodata;
};

// from zScene.cpp
// size: 0x24
class p2PlatformSpringboardData {
    /* 0x0000 */ float jmph[3];
    /* 0x000C */ unsigned int animID[3];
    /* 0x0018 */ class _xVec3 jmpdir;
};

// from zScene.cpp
// size: 0xC
class p2PlatformTeeterData {
    /* 0x0000 */ float itilt;
    /* 0x0004 */ float maxtilt;
    /* 0x0008 */ float invmass;
};

// from zEntPlayer.cpp
// size: 0xC
class p2SimpleObjAsset {
    /* 0x0000 */ float animSpeed;
    /* 0x0004 */ unsigned int initAnimState;
    /* 0x0008 */ unsigned char collType;
    /* 0x0009 */ unsigned char flags;
};

// from iTime.cpp
// size: 0xC
class prof_info {
    /* 0x0000 */ unsigned int addr;
    /* 0x0004 */ unsigned int len;
    /* 0x0008 */ unsigned int count;
};

// from p2im3dhack.cpp
// size: 0x8
class rwPS2AllClusterInstanceInfo {
    /* 0x0000 */ unsigned int attrib;
    /* 0x0004 */ unsigned int stride;
};

// from p2im3dhack.cpp
// size: 0x8
class rwPS2AllClusterQuickInfo {
    /* 0x0000 */ u_long128 *data;
    /* 0x0004 */ unsigned int stride;
};

// from p2im3dhack.cpp
// size: 0x18
class rwPS2AllFieldRec {
    /* 0x0000 */ signed int numVerts;
    /* 0x0004 */ signed int morphNumVerts;
    /* 0x0008 */ signed int dataoffset;
    /* 0x000C */ signed int morphDataoffset;
    /* 0x0010 */ signed short skip;
    /* 0x0012 */ signed short morphSkip;
    /* 0x0014 */ signed short reverse;
    /* 0x0016 */ unsigned char vuoffset;
    /* 0x0017 */ unsigned char pad[1];
};

// from p2im3dhack.cpp
// size: 0x12
class rwPS2AllIndexLUTs {
    /* 0x0000 */ unsigned short fakeIndices[1];
    /* 0x0002 */ class _rwPS2AllNormalLUTs normal;
    /* 0x0007 */ class _rwPS2AllTriFanLUTs triFan;
};

// from p2im3dhack.cpp
// size: 0xE
class rwPS2AllPrimTypeLUT {
    /* 0x0000 */ unsigned char vertToIndRatio[7];
    /* 0x0007 */ unsigned char vertToIndOffset[7];
};

// from p2im3dhack.cpp
// size: 0x120
class rwPS2AllResEntryFormat {
    /* 0x0000 */ unsigned char batchRound;
    /* 0x0001 */ unsigned char stripReverse;
    /* 0x0002 */ unsigned char pad[2];
    /* 0x0004 */ unsigned int maxInputSize;
    /* 0x0008 */ signed int batchSize;
    /* 0x000C */ signed int batchesPerTag;
    /* 0x0010 */ signed int morphBatchSize;
    /* 0x0014 */ signed int morphBatchesPerTag;
    /* 0x0018 */ class rwPS2AllFieldRec fieldRec[11];
};

// from p2im3dhack.cpp
// size: 0x190
class rwPS2AllResEntryHeader {
    /* 0x0000 */ signed int refCnt;
    /* 0x0004 */ signed int clrCnt;
    /* 0x0008 */ u_long128 *data;
    /* 0x000C */ unsigned int numVerts;
    /* 0x0010 */ unsigned int objIdentifier;
    /* 0x0014 */ unsigned int meshIdentifier;
    /* 0x0018 */ signed int batchSize;
    /* 0x001C */ signed int numBatches;
    /* 0x0020 */ signed int batchesPerTag;
    /* 0x0024 */ signed int morphStart;
    /* 0x0028 */ signed int morphFinish;
    /* 0x002C */ signed int morphNum;
    /* 0x0030 */ class rwPS2AllClusterQuickInfo clquickinfo[11];
    /* 0x0088 */ class rwPS2AllFieldRec fieldRec[11];
};

// from xScrFx.cpp
// size: 0x20
class rxHeapBlockHeader {
    /* 0x0000 */ class rxHeapBlockHeader *prev;
    /* 0x0004 */ class rxHeapBlockHeader *next;
    /* 0x0008 */ unsigned int size;
    /* 0x000C */ class rxHeapFreeBlock *freeEntry;
    /* 0x0010 */ unsigned int pad[4];
};

// from xScrFx.cpp
// size: 0x8
class rxHeapFreeBlock {
    /* 0x0000 */ unsigned int size;
    /* 0x0004 */ class rxHeapBlockHeader *ptr;
};

// from xScrFx.cpp
// size: 0xC
class rxHeapSuperBlockDescriptor {
    /* 0x0000 */ void *start;
    /* 0x0004 */ unsigned int size;
    /* 0x0008 */ class rxHeapSuperBlockDescriptor *next;
};

// from p2im3dhack.cpp
// size: 0x2FC
class rxNodePS2AllMatPvtData {
    /* 0x0000 */ signed int (*im3DPreMeshCB)(class RxPS2AllPipeData *);
    /* 0x0004 */ signed int (*meshInstanceTestCB)(class RxPS2AllPipeData *);
    /* 0x0008 */ class RwResEntry * (*resEntryAllocCB)(class RxPS2AllPipeData *, class RwResEntry **, unsigned int, void (*)(class RwResEntry *));
    /* 0x000C */ signed int (*instanceCB)(class RxPS2AllPipeData *, void **, unsigned int);
    /* 0x0010 */ signed int (*bridgeCB)(class RxPS2AllPipeData *);
    /* 0x0014 */ signed int (*im3DPostMeshCB)(class RxPS2AllPipeData *);
    /* 0x0018 */ signed int (*im3DMeshSplitCB)(class RxPS2AllPipeData *);
    /* 0x001C */ signed int (*postMeshCB)(class RxPS2AllPipeData *);
    /* 0x0020 */ signed int vifOffset;
    /* 0x0024 */ void **vu1CodeArray;
    /* 0x0028 */ unsigned int codeArrayLength;
    /* 0x002C */ class rwPS2AllClusterInstanceInfo clinfo[11];
    /* 0x0084 */ unsigned int cliIndex[11];
    /* 0x00B0 */ enum RpMeshHeaderFlags pipeType;
    /* 0x00B4 */ unsigned char totallyOpaque;
    /* 0x00B5 */ unsigned char numStripes;
    /* 0x00B6 */ unsigned char sizeOnVU;
    /* 0x00B7 */ unsigned char pad0;
    /* 0x00B8 */ class rwPS2AllResEntryFormat strip;
    /* 0x01D8 */ class rwPS2AllResEntryFormat list;
    /* 0x02F8 */ unsigned int magicValue;
};

// from p2im3dhack.cpp
// size: 0x58
class rxNodePS2AllPvtData {
    /* 0x0000 */ class rwPS2AllPrimTypeLUT primLUT;
    /* 0x000E */ unsigned char pad[2];
    /* 0x0010 */ signed int (*objectSetupCB)(class RxPS2AllPipeData *, class RwMatrixTag **);
    /* 0x0014 */ signed int (*objectFinalizeCB)(class RxPS2AllPipeData *);
    /* 0x0018 */ class RxPipeline *groupPipe;
    /* 0x001C */ unsigned int lightOffset;
    /* 0x0020 */ unsigned int lightBufferSize;
    /* 0x0024 */ u_long128 *nullLightBlock;
    /* 0x0028 */ float nullLightBuffer[12];
};

// from iSystem.cpp
// size: 0x8
class sceCdCLOCK {
    /* 0x0000 */ unsigned char stat;
    /* 0x0001 */ unsigned char second;
    /* 0x0002 */ unsigned char minute;
    /* 0x0003 */ unsigned char hour;
    /* 0x0004 */ unsigned char pad;
    /* 0x0005 */ unsigned char day;
    /* 0x0006 */ unsigned char month;
    /* 0x0007 */ unsigned char year;
};

// from iFile.cpp
// size: 0x4
class sceCdRMode {
    /* 0x0000 */ unsigned char trycount;
    /* 0x0001 */ unsigned char spindlctrl;
    /* 0x0002 */ unsigned char datapattern;
    /* 0x0003 */ unsigned char pad;
};

// from iFile.cpp
// size: 0x24
class sceCdlFILE {
    /* 0x0000 */ unsigned int lsn;
    /* 0x0004 */ unsigned int size;
    /* 0x0008 */ char name[16];
    /* 0x0018 */ unsigned char date[8];
    /* 0x0020 */ unsigned int flag;
};

// from disp.c
// size: 0x90
class sceDmaChan {
    /* 0x0000 */ class tD_CHCR chcr;
    /* 0x0004 */ unsigned int p0[3];
    /* 0x0010 */ void *madr;
    /* 0x0014 */ unsigned int p1[3];
    /* 0x0020 */ unsigned int qwc;
    /* 0x0024 */ unsigned int p2[3];
    /* 0x0030 */ class _sceDmaTag *tadr;
    /* 0x0034 */ unsigned int p3[3];
    /* 0x0040 */ void *as0;
    /* 0x0044 */ unsigned int p4[3];
    /* 0x0050 */ void *as1;
    /* 0x0054 */ unsigned int p5[3];
    /* 0x0060 */ unsigned int p6[4];
    /* 0x0070 */ unsigned int p7[4];
    /* 0x0080 */ void *sadr;
    /* 0x0084 */ unsigned int p8[3];
};

// from disp.c
// size: 0x10
class sceGifPacket {
    /* 0x0000 */ unsigned int *pCurrent;
    /* 0x0004 */ u_long128 *pBase;
    /* 0x0008 */ u_long128 *pDmaTag;
    /* 0x000C */ unsigned long *pGifTag;
};

// from ps2_mpeg.c
// size: 0x10
class sceGifTag {
    /* 0x0000 */ unsigned long NLOOP : 15 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long EOP : 1 /* bit_offset 15 */;
    /* 0x0000 */ unsigned long pad16 : 16 /* bit_offset 16 */;
    /* 0x0000 */ unsigned long id : 14 /* bit_offset 32 */;
    /* 0x0000 */ unsigned long PRE : 1 /* bit_offset 46 */;
    /* 0x0000 */ unsigned long PRIM : 11 /* bit_offset 47 */;
    /* 0x0000 */ unsigned long FLG : 2 /* bit_offset 58 */;
    /* 0x0000 */ unsigned long NREG : 4 /* bit_offset 60 */;
    /* 0x0008 */ unsigned long REGS0 : 4 /* bit_offset 0 */;
    /* 0x0008 */ unsigned long REGS1 : 4 /* bit_offset 4 */;
    /* 0x0008 */ unsigned long REGS2 : 4 /* bit_offset 8 */;
    /* 0x0008 */ unsigned long REGS3 : 4 /* bit_offset 12 */;
    /* 0x0008 */ unsigned long REGS4 : 4 /* bit_offset 16 */;
    /* 0x0008 */ unsigned long REGS5 : 4 /* bit_offset 20 */;
    /* 0x0008 */ unsigned long REGS6 : 4 /* bit_offset 24 */;
    /* 0x0008 */ unsigned long REGS7 : 4 /* bit_offset 28 */;
    /* 0x0008 */ unsigned long REGS8 : 4 /* bit_offset 32 */;
    /* 0x0008 */ unsigned long REGS9 : 4 /* bit_offset 36 */;
    /* 0x0008 */ unsigned long REGS10 : 4 /* bit_offset 40 */;
    /* 0x0008 */ unsigned long REGS11 : 4 /* bit_offset 44 */;
    /* 0x0008 */ unsigned long REGS12 : 4 /* bit_offset 48 */;
    /* 0x0008 */ unsigned long REGS13 : 4 /* bit_offset 52 */;
    /* 0x0008 */ unsigned long REGS14 : 4 /* bit_offset 56 */;
    /* 0x0008 */ unsigned long REGS15 : 4 /* bit_offset 60 */;
};

// from ps2_mpeg.c
// size: 0x60
class sceGsClear {
    /* 0x0000 */ class sceGsTest testa;
    /* 0x0008 */ signed long testaaddr;
    /* 0x0010 */ class sceGsPrim prim;
    /* 0x0018 */ signed long primaddr;
    /* 0x0020 */ class sceGsRgbaq rgbaq;
    /* 0x0028 */ signed long rgbaqaddr;
    /* 0x0030 */ class sceGsXyz xyz2a;
    /* 0x0038 */ signed long xyz2aaddr;
    /* 0x0040 */ class sceGsXyz xyz2b;
    /* 0x0048 */ signed long xyz2baddr;
    /* 0x0050 */ class sceGsTest testb;
    /* 0x0058 */ signed long testbaddr;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsColclamp {
    /* 0x0000 */ unsigned long CLAMP : 1 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long pad01 : 63 /* bit_offset 1 */;
};

// from ps2_mpeg.c
// size: 0x230
class sceGsDBuff {
    /* 0x0000 */ class sceGsDispEnv disp[2];
    /* 0x0050 */ class sceGifTag giftag0;
    /* 0x0060 */ class sceGsDrawEnv1 draw0;
    /* 0x00E0 */ class sceGsClear clear0;
    /* 0x0140 */ class sceGifTag giftag1;
    /* 0x0150 */ class sceGsDrawEnv1 draw1;
    /* 0x01D0 */ class sceGsClear clear1;
};

// from ps2_mpeg.c
// size: 0x28
class sceGsDispEnv {
    /* 0x0000 */ class tGS_PMODE pmode;
    /* 0x0008 */ class tGS_SMODE2 smode2;
    /* 0x0010 */ class tGS_DISPFB2 dispfb;
    /* 0x0018 */ class tGS_DISPLAY2 display;
    /* 0x0020 */ class tGS_BGCOLOR bgcolor;
};

// from ps2_mpeg.c
// size: 0x80
class sceGsDrawEnv1 {
    /* 0x0000 */ class sceGsFrame frame1;
    /* 0x0008 */ unsigned long frame1addr;
    /* 0x0010 */ class sceGsZbuf zbuf1;
    /* 0x0018 */ signed long zbuf1addr;
    /* 0x0020 */ class sceGsXyoffset xyoffset1;
    /* 0x0028 */ signed long xyoffset1addr;
    /* 0x0030 */ class sceGsScissor scissor1;
    /* 0x0038 */ signed long scissor1addr;
    /* 0x0040 */ class sceGsPrmodecont prmodecont;
    /* 0x0048 */ signed long prmodecontaddr;
    /* 0x0050 */ class sceGsColclamp colclamp;
    /* 0x0058 */ signed long colclampaddr;
    /* 0x0060 */ class sceGsDthe dthe;
    /* 0x0068 */ signed long dtheaddr;
    /* 0x0070 */ class sceGsTest test1;
    /* 0x0078 */ signed long test1addr;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsDthe {
    /* 0x0000 */ unsigned long DTHE : 1 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long pad01 : 63 /* bit_offset 1 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsFrame {
    /* 0x0000 */ unsigned long FBP : 9 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long pad09 : 7 /* bit_offset 9 */;
    /* 0x0000 */ unsigned long FBW : 6 /* bit_offset 16 */;
    /* 0x0000 */ unsigned long pad22 : 2 /* bit_offset 22 */;
    /* 0x0000 */ unsigned long PSM : 6 /* bit_offset 24 */;
    /* 0x0000 */ unsigned long pad30 : 2 /* bit_offset 30 */;
    /* 0x0000 */ unsigned long FBMSK : 32 /* bit_offset 32 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsPrim {
    /* 0x0000 */ unsigned long PRIM : 3 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long IIP : 1 /* bit_offset 3 */;
    /* 0x0000 */ unsigned long TME : 1 /* bit_offset 4 */;
    /* 0x0000 */ unsigned long FGE : 1 /* bit_offset 5 */;
    /* 0x0000 */ unsigned long ABE : 1 /* bit_offset 6 */;
    /* 0x0000 */ unsigned long AA1 : 1 /* bit_offset 7 */;
    /* 0x0000 */ unsigned long FST : 1 /* bit_offset 8 */;
    /* 0x0000 */ unsigned long CTXT : 1 /* bit_offset 9 */;
    /* 0x0000 */ unsigned long FIX : 1 /* bit_offset 10 */;
    /* 0x0000 */ unsigned long pad11 : 53 /* bit_offset 11 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsPrmodecont {
    /* 0x0000 */ unsigned long AC : 1 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long pad01 : 63 /* bit_offset 1 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsRgbaq {
    /* 0x0000 */ unsigned int R : 8 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int G : 8 /* bit_offset 8 */;
    /* 0x0000 */ unsigned int B : 8 /* bit_offset 16 */;
    /* 0x0000 */ unsigned int A : 8 /* bit_offset 24 */;
    /* 0x0004 */ float Q;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsScissor {
    /* 0x0000 */ unsigned long SCAX0 : 11 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long pad11 : 5 /* bit_offset 11 */;
    /* 0x0000 */ unsigned long SCAX1 : 11 /* bit_offset 16 */;
    /* 0x0000 */ unsigned long pad27 : 5 /* bit_offset 27 */;
    /* 0x0000 */ unsigned long SCAY0 : 11 /* bit_offset 32 */;
    /* 0x0000 */ unsigned long pad43 : 5 /* bit_offset 43 */;
    /* 0x0000 */ unsigned long SCAY1 : 11 /* bit_offset 48 */;
    /* 0x0000 */ unsigned long pad59 : 5 /* bit_offset 59 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsTest {
    /* 0x0000 */ unsigned long ATE : 1 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long ATST : 3 /* bit_offset 1 */;
    /* 0x0000 */ unsigned long AREF : 8 /* bit_offset 4 */;
    /* 0x0000 */ unsigned long AFAIL : 2 /* bit_offset 12 */;
    /* 0x0000 */ unsigned long DATE : 1 /* bit_offset 14 */;
    /* 0x0000 */ unsigned long DATM : 1 /* bit_offset 15 */;
    /* 0x0000 */ unsigned long ZTE : 1 /* bit_offset 16 */;
    /* 0x0000 */ unsigned long ZTST : 2 /* bit_offset 17 */;
    /* 0x0000 */ unsigned long pad19 : 45 /* bit_offset 19 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsXyoffset {
    /* 0x0000 */ unsigned long OFX : 16 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long pad16 : 16 /* bit_offset 16 */;
    /* 0x0000 */ unsigned long OFY : 16 /* bit_offset 32 */;
    /* 0x0000 */ unsigned long pad48 : 16 /* bit_offset 48 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsXyz {
    /* 0x0000 */ unsigned long X : 16 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long Y : 16 /* bit_offset 16 */;
    /* 0x0000 */ unsigned long Z : 32 /* bit_offset 32 */;
};

// from ps2_mpeg.c
// size: 0x8
class sceGsZbuf {
    /* 0x0000 */ unsigned long ZBP : 9 /* bit_offset 0 */;
    /* 0x0000 */ unsigned long pad09 : 15 /* bit_offset 9 */;
    /* 0x0000 */ unsigned long PSM : 4 /* bit_offset 24 */;
    /* 0x0000 */ unsigned long pad28 : 4 /* bit_offset 28 */;
    /* 0x0000 */ unsigned long ZMSK : 1 /* bit_offset 32 */;
    /* 0x0000 */ unsigned long pad33 : 31 /* bit_offset 33 */;
};

// from videodec.c
// size: 0x24
class sceIpuDmaEnv {
    /* 0x0000 */ unsigned int d4madr;
    /* 0x0004 */ unsigned int d4tadr;
    /* 0x0008 */ unsigned int d4qwc;
    /* 0x000C */ unsigned int d4chcr;
    /* 0x0010 */ unsigned int d3madr;
    /* 0x0014 */ unsigned int d3qwc;
    /* 0x0018 */ unsigned int d3chcr;
    /* 0x001C */ unsigned int ipubp;
    /* 0x0020 */ unsigned int ipuctrl;
};

// from isavegame.cpp
// size: 0x3C4
class sceMcIconSys {
    /* 0x0000 */ unsigned char Head[4];
    /* 0x0004 */ unsigned short Reserv1;
    /* 0x0006 */ unsigned short OffsLF;
    /* 0x0008 */ unsigned int Reserv2;
    /* 0x000C */ unsigned int TransRate;
    /* 0x0010 */ signed int BgColor[4][4];
    /* 0x0050 */ float LightDir[3][4];
    /* 0x0080 */ float LightColor[3][4];
    /* 0x00B0 */ float Ambient[4];
    /* 0x00C0 */ unsigned char TitleName[68];
    /* 0x0104 */ unsigned char FnameView[64];
    /* 0x0144 */ unsigned char FnameCopy[64];
    /* 0x0184 */ unsigned char FnameDel[64];
    /* 0x01C4 */ unsigned char Reserve3[512];
};

// from isavegame.cpp
// size: 0x8
class sceMcStDateTime {
    /* 0x0000 */ unsigned char Resv2;
    /* 0x0001 */ unsigned char Sec;
    /* 0x0002 */ unsigned char Min;
    /* 0x0003 */ unsigned char Hour;
    /* 0x0004 */ unsigned char Day;
    /* 0x0005 */ unsigned char Month;
    /* 0x0006 */ unsigned short Year;
};

// from isavegame.cpp
// size: 0x40
class sceMcTblGetDir {
    /* 0x0000 */ class sceMcStDateTime _Create;
    /* 0x0008 */ class sceMcStDateTime _Modify;
    /* 0x0010 */ unsigned int FileSizeByte;
    /* 0x0014 */ unsigned short AttrFile;
    /* 0x0016 */ unsigned short Reserve1;
    /* 0x0018 */ unsigned int Reserve2;
    /* 0x001C */ unsigned int PdaAplNo;
    /* 0x0020 */ unsigned char EntryName[32];
};

// from videodec.c
// size: 0x48
class sceMpeg {
    /* 0x0000 */ signed int width;
    /* 0x0004 */ signed int height;
    /* 0x0008 */ signed int frameCount;
    /* 0x0010 */ signed long pts;
    /* 0x0018 */ signed long dts;
    /* 0x0020 */ unsigned long flags;
    /* 0x0028 */ signed long pts2nd;
    /* 0x0030 */ signed long dts2nd;
    /* 0x0038 */ unsigned long flags2nd;
    /* 0x0040 */ void *sys;
};

// from videodec.c
// size: 0x20
class sceMpegCbData {
    /* 0x0000 */ enum sceMpegCbType type;
    /* 0x0000 */ class sceMpegCbDataError error;
    /* 0x0000 */ class sceMpegCbDataTimeStamp ts;
    /* 0x0000 */ class sceMpegCbDataStr str;
};

// from videodec.c
// size: 0x8
class sceMpegCbDataError {
    /* 0x0000 */ enum sceMpegCbType type;
    /* 0x0004 */ char *errMessage;
};

// from videodec.c
// size: 0x20
class sceMpegCbDataStr {
    /* 0x0000 */ enum sceMpegCbType type;
    /* 0x0004 */ unsigned char *header;
    /* 0x0008 */ unsigned char *data;
    /* 0x000C */ unsigned int len;
    /* 0x0010 */ signed long pts;
    /* 0x0018 */ signed long dts;
};

// from videodec.c
// size: 0x18
class sceMpegCbDataTimeStamp {
    /* 0x0000 */ enum sceMpegCbType type;
    /* 0x0008 */ signed long pts;
    /* 0x0010 */ signed long dts;
};

// from iSnd.cpp
// size: 0x10
class sceSifDmaData {
    /* 0x0000 */ unsigned int data;
    /* 0x0004 */ unsigned int addr;
    /* 0x0008 */ unsigned int size;
    /* 0x000C */ unsigned int mode;
};

// from p2skyfs.c
// size: 0x2814
class skyFile {
    /* 0x0000 */ signed int gdfs;
    /* 0x0004 */ signed int POS;
    /* 0x0008 */ signed int SOF;
    /* 0x000C */ unsigned char readBuffer[10240];
    /* 0x280C */ unsigned int bufferPos;
    /* 0x2810 */ signed int bufferValid;
};

// from zEntVillain.cpp
// size: 0x2C
class st_BGG_QUAKEBALL {
    /* 0x0000 */ signed int flg_stat;
    /* 0x0004 */ enum en_BGG_PHASE phase;
    /* 0x0008 */ float rad_build;
    /* 0x000C */ float spd_flight;
    /* 0x0010 */ class _xVec3 pos_tgt;
    /* 0x001C */ float tym_flightMax;
    /* 0x0020 */ float rad_shock;
    /* 0x0024 */ float hyt_shock;
    /* 0x0028 */ float tym_shock;
};

// from xbinio.cpp
// size: 0x38
class st_BINIO_XTRADATA {
    /* 0x0000 */ char *dbl_buf;
    /* 0x0004 */ signed int dblbuf_size;
    /* 0x0008 */ signed int dbl_beg;
    /* 0x000C */ signed int dbl_end;
    /* 0x0010 */ signed int dbl_amt;
    /* 0x0014 */ unsigned int fpos;
    /* 0x0018 */ char *asyn_data;
    /* 0x001C */ signed int asyn_amt;
    /* 0x0020 */ signed int asyn_elesize;
    /* 0x0024 */ signed int asyn_ismot;
    /* 0x0028 */ enum en_BIO_ASYNC_ERRCODES asyn_status;
    /* 0x002C */ unsigned int pad[3];
};

// from zEntVillain.cpp
// size: 0x3C
class st_BKAXE_DATA {
    /* 0x0000 */ signed int flg_stat;
    /* 0x0004 */ class _tagLightning *strk[8];
    /* 0x0024 */ float rad_zap;
    /* 0x0028 */ float dst_zapdrop;
    /* 0x002C */ float tym_zapdur;
    /* 0x0030 */ class _xVec3 planes;
};

// from zEntVillain.cpp
// size: 0x18
class st_CAT_AIMARROW {
    /* 0x0000 */ class _xVec3 pos_src;
    /* 0x000C */ class _xVec3 pos_tgt;
};

// from zEntVillain.cpp
// size: 0x20
class st_CAT_AIMGRAV {
    /* 0x0000 */ class _xVec3 pos_src;
    /* 0x000C */ class _xVec3 pos_tgt;
    /* 0x0018 */ float fac_arcness;
    /* 0x001C */ float acc_gravity;
};

// from xLinkList.cpp
// size: 0xC
class st_ELIST {
    /* 0x0000 */ class st_ENODE *head;
    /* 0x0004 */ class st_ENODE *tail;
    /* 0x0008 */ class st_ENODE *tailprev;
};

// from zEnt.cpp
// size: 0x8
class st_ENODE {
    /* 0x0000 */ class st_ENODE *next;
    /* 0x0004 */ class st_ENODE *prev;
};

// from xbinio.cpp
// size: 0x68
class st_FILELOADINFO {
    /* 0x0000 */ void (*destroy)(class st_FILELOADINFO *);
    /* 0x0004 */ signed int (*readBytes)(class st_FILELOADINFO *, char *, signed int);
    /* 0x0008 */ signed int (*readMShorts)(class st_FILELOADINFO *, signed short *, signed int);
    /* 0x000C */ signed int (*readMLongs)(class st_FILELOADINFO *, signed int *, signed int);
    /* 0x0010 */ signed int (*readMFloats)(class st_FILELOADINFO *, float *, signed int);
    /* 0x0014 */ signed int (*readMDoubles)(class st_FILELOADINFO *, double *, signed int);
    /* 0x0018 */ signed int (*readIShorts)(class st_FILELOADINFO *, signed short *, signed int);
    /* 0x001C */ signed int (*readILongs)(class st_FILELOADINFO *, signed int *, signed int);
    /* 0x0020 */ signed int (*readIFloats)(class st_FILELOADINFO *, float *, signed int);
    /* 0x0024 */ signed int (*readIDoubles)(class st_FILELOADINFO *, double *, signed int);
    /* 0x0028 */ signed int (*skipBytes)(class st_FILELOADINFO *, signed int);
    /* 0x002C */ signed int (*seekSpot)(class st_FILELOADINFO *, signed int);
    /* 0x0030 */ void (*setDoubleBuf)(class st_FILELOADINFO *, char *, signed int);
    /* 0x0034 */ void (*discardDblBuf)(class st_FILELOADINFO *);
    /* 0x0038 */ signed int (*asyncIRead)(class st_FILELOADINFO *, signed int, char *, signed int, signed int);
    /* 0x003C */ signed int (*asyncMRead)(class st_FILELOADINFO *, signed int, char *, signed int, signed int);
    /* 0x0040 */ enum en_BIO_ASYNC_ERRCODES (*asyncReadStatus)(class st_FILELOADINFO *);
    /* 0x0044 */ unsigned int lockid;
    /* 0x0048 */ enum en_FIOERRCODES error;
    /* 0x004C */ unsigned int basesector;
    /* 0x0050 */ void *privdata;
    /* 0x0054 */ void *xtradata;
    /* 0x0058 */ void *asyndata;
    /* 0x005C */ signed int filesize;
    /* 0x0060 */ signed int remain;
    /* 0x0064 */ signed int position;
};

// from xhipio.cpp
// size: 0x44
class st_FILESAVEINFO {
    /* 0x0000 */ void (*destroy)(class st_FILESAVEINFO *);
    /* 0x0004 */ signed int (*writeBytes)(class st_FILESAVEINFO *, char *, signed int);
    /* 0x0008 */ signed int (*writeMShorts)(class st_FILESAVEINFO *, signed short *, signed int);
    /* 0x000C */ signed int (*writeMLongs)(class st_FILESAVEINFO *, signed int *, signed int);
    /* 0x0010 */ signed int (*writeMFloats)(class st_FILESAVEINFO *, float *, signed int);
    /* 0x0014 */ signed int (*writeMDoubles)(class st_FILESAVEINFO *, double *, signed int);
    /* 0x0018 */ signed int (*writeIShorts)(class st_FILESAVEINFO *, signed short *, signed int);
    /* 0x001C */ signed int (*writeILongs)(class st_FILESAVEINFO *, signed int *, signed int);
    /* 0x0020 */ signed int (*writeIFloats)(class st_FILESAVEINFO *, float *, signed int);
    /* 0x0024 */ signed int (*writeIDoubles)(class st_FILESAVEINFO *, double *, signed int);
    /* 0x0028 */ signed int (*seekSpot)(class st_FILESAVEINFO *, signed int);
    /* 0x002C */ signed int length;
    /* 0x0030 */ signed int position;
    /* 0x0034 */ enum en_FIOERRCODES error;
    /* 0x0038 */ unsigned int lockid;
    /* 0x003C */ void *privdata;
    /* 0x0040 */ void *xtradata;
};

// from xhipio.cpp
// size: 0x10
class st_HIPLOADBLOCK {
    /* 0x0000 */ signed int endpos;
    /* 0x0004 */ unsigned int blk_id;
    /* 0x0008 */ signed int blk_remain;
    /* 0x000C */ signed int flags;
};

// from xhipio.cpp
// size: 0xA8
class st_HIPLOADDATA {
    /* 0x0000 */ class st_FILELOADINFO *fli;
    /* 0x0004 */ signed int lockid;
    /* 0x0008 */ signed int bypass;
    /* 0x000C */ signed int bypass_recover;
    /* 0x0010 */ unsigned int base_sector;
    /* 0x0014 */ signed int use_async;
    /* 0x0018 */ enum en_READ_ASYNC_STATUS asyn_stat;
    /* 0x001C */ signed int pos;
    /* 0x0020 */ signed int top;
    /* 0x0024 */ signed int readTop;
    /* 0x0028 */ class st_HIPLOADBLOCK stk[8];
};

// from xpkrsvc.cpp
// size: 0x34
class st_HIPLOADFUNCS {
    /* 0x0000 */ class st_HIPLOADDATA * (*create)(char *, char *, signed int);
    /* 0x0004 */ void (*destroy)(class st_HIPLOADDATA *);
    /* 0x0008 */ unsigned int (*basesector)(class st_HIPLOADDATA *);
    /* 0x000C */ unsigned int (*enter)(class st_HIPLOADDATA *);
    /* 0x0010 */ void (*exit)(class st_HIPLOADDATA *);
    /* 0x0014 */ signed int (*readBytes)(class st_HIPLOADDATA *, char *, signed int);
    /* 0x0018 */ signed int (*readShorts)(class st_HIPLOADDATA *, signed short *, signed int);
    /* 0x001C */ signed int (*readLongs)(class st_HIPLOADDATA *, signed int *, signed int);
    /* 0x0020 */ signed int (*readFloats)(class st_HIPLOADDATA *, float *, signed int);
    /* 0x0024 */ signed int (*readString)(class st_HIPLOADDATA *, char *);
    /* 0x0028 */ signed int (*setBypass)(class st_HIPLOADDATA *, signed int, signed int);
    /* 0x002C */ void (*setSpot)(class st_HIPLOADDATA *, signed int);
    /* 0x0030 */ enum en_READ_ASYNC_STATUS (*pollRead)(class st_HIPLOADDATA *);
};

// from xhipio.cpp
// size: 0xC
class st_HIPSAVEBLOCK {
    /* 0x0000 */ signed int pos;
    /* 0x0004 */ signed int len;
    /* 0x0008 */ signed int flags;
};

// from xhipio.cpp
// size: 0x74
class st_HIPSAVEDATA {
    /* 0x0000 */ class st_FILESAVEINFO *fsi;
    /* 0x0004 */ signed int lockid;
    /* 0x0008 */ signed int pos;
    /* 0x000C */ signed int top;
    /* 0x0010 */ signed int writeTop;
    /* 0x0014 */ class st_HIPSAVEBLOCK stk[8];
};

// from xhipio.cpp
// size: 0x2C
class st_HIPSAVEFUNCS {
    /* 0x0000 */ class st_HIPSAVEDATA * (*create)(char *);
    /* 0x0004 */ void (*destroy)(class st_HIPSAVEDATA *);
    /* 0x0008 */ void (*open)(class st_HIPSAVEDATA *, unsigned int);
    /* 0x000C */ void (*close)(class st_HIPSAVEDATA *);
    /* 0x0010 */ void (*writeBytes)(class st_HIPSAVEDATA *, char *, signed int);
    /* 0x0014 */ void (*writeShorts)(class st_HIPSAVEDATA *, signed short *, signed int);
    /* 0x0018 */ void (*writeLongs)(class st_HIPSAVEDATA *, signed int *, signed int);
    /* 0x001C */ void (*writeFloats)(class st_HIPSAVEDATA *, float *, signed int);
    /* 0x0020 */ void (*writeString)(class st_HIPSAVEDATA *, char *);
    /* 0x0024 */ signed int (*curSpot)(class st_HIPSAVEDATA *);
    /* 0x0028 */ signed int (*spotLong)(class st_HIPSAVEDATA *, signed int, unsigned int);
};

// from isavegame.cpp
// size: 0x94
class st_ISGSESSION {
    /* 0x0000 */ class st_ISG_MEMCARD_DATA *mcdata;
    /* 0x0004 */ char gameroot[64];
    /* 0x0044 */ char gamedir[64];
    /* 0x0084 */ enum en_ASYNC_OPCODE as_curop;
    /* 0x0088 */ enum en_ASYNC_OPSTAT as_opstat;
    /* 0x008C */ enum en_ASYNC_OPERR as_operr;
    /* 0x0090 */ enum en_CHGCODE chgcode;
};

// from isavegame.cpp
// size: 0x100
class st_ISG_MEMCARD_DATA {
    /* 0x0000 */ signed int mcport;
    /* 0x0004 */ signed int mcslot;
    /* 0x0008 */ signed int mcfp;
    /* 0x000C */ enum en_ISG_IOMODE fmode;
    /* 0x0010 */ char gamepath[64];
    /* 0x0080 */ class sceMcTblGetDir finfo;
    /* 0x00C0 */ signed int cur_mcop;
    /* 0x00C4 */ enum en_ISGMC_ERRSTATUS mcerr;
    /* 0x00C8 */ signed int allow_cache;
};

// from zLobMaster.cpp
// size: 0x10
class st_LOBMASTER_CONFIG {
    /* 0x0000 */ class zLobMasterAsset *rawass;
    /* 0x0004 */ class _xVec3 dir_launch;
};

// from zLobMaster.cpp
// size: 0x90
class st_LOBMASTER_DATA : public xBase {
    /* 0x0010 */ signed int flg_lmopts;
    /* 0x0014 */ float ang_debrisCone;
    /* 0x0018 */ signed int cnt_ammo;
    /* 0x001C */ signed int cnt_salvo;
    /* 0x0020 */ class _zMovePoint *nav_bullbase;
    /* 0x0024 */ class _zMovePoint *nav_nextbull;
    /* 0x0028 */ class _zMovePoint *nav_prevbull;
    /* 0x002C */ signed int flg_suitcase;
    /* 0x0030 */ class _xVec3 pos_altLaunch;
    /* 0x003C */ class _xVec3 dir_altLaunch;
    /* 0x0048 */ class _xBound bnd_altLaunch;
    /* 0x0080 */ class st_LOBMASTER_CONFIG cfg;
};

// from zLobMaster.cpp
// size: 0x8
class st_LOBMASTER_DEPOT {
    /* 0x0000 */ class st_LOBMASTER_DATA *raw_pool;
    /* 0x0004 */ signed int raw_cnt;
};

// from zLobMaster.cpp
// size: 0x2C
class st_LOBMASTER_QUEUE {
    /* 0x0000 */ signed int flg_stat;
    /* 0x0004 */ float tmr_delay;
    /* 0x0008 */ unsigned int aid_lobber;
    /* 0x000C */ class _xVec3 pos_launch;
    /* 0x0018 */ class _xVec3 dir_launch;
    /* 0x0024 */ signed int cnt_salvo;
    /* 0x0028 */ signed int idx_current;
};

// from zEntProjectile.cpp
// size: 0x14
class st_NEWT_USERDATA {
    /* 0x0000 */ float g;
    /* 0x0004 */ float g_2;
    /* 0x0008 */ float y_y0;
    /* 0x000C */ float v0;
    /* 0x0010 */ signed int cnt_iter;
};

// from zEntVillain.cpp
// size: 0xF0
class st_NPCTYPE_CONFIG {
    /* 0x0000 */ signed int hitpts;
    /* 0x0004 */ signed int misc_flags;
    /* 0x0008 */ class _xVec3 scl_modelBase;
    /* 0x0014 */ class _xVec3 sanim_idle;
    /* 0x0020 */ class _xVec3 sanim_move;
    /* 0x002C */ class _xVec3 sanim_chase;
    /* 0x0038 */ float rad_bound;
    /* 0x003C */ class _xVec3 vec_bndoffset;
    /* 0x0048 */ float rad_bound2;
    /* 0x004C */ class _xVec3 vec_bnd2offset;
    /* 0x0058 */ float tym_attack_begin;
    /* 0x005C */ float tym_attack_end;
    /* 0x0060 */ class _xVec3 pos_verts[12];
};

// from zEntVillain.cpp
// size: 0x8
class st_NPC_BONUS {
    /* 0x0000 */ signed int flg_info;
    /* 0x0004 */ class _zEntPickup *pickup;
};

// from zEntVillain.cpp
// size: 0x8
class st_NPC_SOUND_ENTRY {
    /* 0x0000 */ enum en_zEntVillainSubTypeID viltyp;
    /* 0x0004 */ class st_NPC_SOUND_RECORDS *sndList;
};

// from zEntVillain.cpp
// size: 0xC
class st_NPC_SOUND_PROPERTIES {
    /* 0x0000 */ enum en_NPC_SOUND sndEvent;
    /* 0x0004 */ signed int flg_vsnd;
    /* 0x0008 */ float tym_delayStart;
};

// from zEnt.cpp
// size: 0x1C
class st_NPC_SOUND_QUEUE {
    /* 0x0000 */ enum en_NPC_SOUND sndEvent;
    /* 0x0004 */ unsigned int sndDirect;
    /* 0x0008 */ signed int flg_frolic;
    /* 0x000C */ float tmr_delay;
    /* 0x0010 */ signed int cnt_repeat;
    /* 0x0014 */ float radius;
    /* 0x0018 */ float tym_repeatDelay;
};

// from zEntVillain.cpp
// size: 0xC
class st_NPC_SOUND_RECORDS {
    /* 0x0000 */ enum en_NPC_SOUND typ_sound;
    /* 0x0004 */ char *nam_sound;
    /* 0x0008 */ unsigned int aid_sound;
};

// from zEntVillain.cpp
// size: 0x10
class st_NPC_TALLY_RECORD {
    /* 0x0000 */ signed int cnt_hitgum;
    /* 0x0004 */ signed int cnt_hitsoap;
    /* 0x0008 */ signed int cnt_killed;
    /* 0x000C */ signed int cnt_mortals;
};

// from xpkrsvc.cpp
// size: 0x28
class st_PACKER_ASSETTYPE {
    /* 0x0000 */ unsigned int typetag;
    /* 0x0004 */ unsigned int tflags;
    /* 0x0008 */ signed int typalign;
    /* 0x000C */ void * (*readXForm)(void *, unsigned int, void *, unsigned int, unsigned int *);
    /* 0x0010 */ void * (*writeXForm)(void *, unsigned int, void *, void *, unsigned int, unsigned int *);
    /* 0x0014 */ signed int (*assetLoaded)(void *, unsigned int, void *, signed int);
    /* 0x0018 */ void * (*makeData)(void *, unsigned int, void *, signed int *, signed int *);
    /* 0x001C */ void (*cleanup)(void *, unsigned int, void *);
    /* 0x0020 */ void (*assetUnloaded)(void *, unsigned int);
    /* 0x0024 */ void (*writePeek)(void *, unsigned int, void *, char *);
};

// from xpkrsvc.cpp
// size: 0x60
class st_PACKER_ATOC_NODE {
    /* 0x0000 */ unsigned int aid;
    /* 0x0004 */ unsigned int asstype;
    /* 0x0008 */ signed int d_off;
    /* 0x000C */ signed int d_size;
    /* 0x0010 */ signed int d_pad;
    /* 0x0014 */ unsigned int d_chksum;
    /* 0x0018 */ signed int assalign;
    /* 0x001C */ signed int infoflag;
    /* 0x0020 */ signed int loadflag;
    /* 0x0024 */ char *memloc;
    /* 0x0028 */ signed int x_size;
    /* 0x002C */ signed int readcnt;
    /* 0x0030 */ signed int readrem;
    /* 0x0034 */ class st_PACKER_ASSETTYPE *typeref;
    /* 0x0038 */ class st_HIPLOADDATA *ownpkg;
    /* 0x003C */ class st_PACKER_READ_DATA *ownpr;
    /* 0x0040 */ char name[32];
};

// from xpkrsvc.cpp
// size: 0x28
class st_PACKER_LTOC_NODE {
    /* 0x0000 */ enum en_LAYER_TYPE laytyp;
    /* 0x0004 */ class st_XORDEREDARRAY assref;
    /* 0x0014 */ signed int flg_ldstat;
    /* 0x0018 */ signed int danglecnt;
    /* 0x001C */ unsigned int chksum;
    /* 0x0020 */ signed int laysize;
    /* 0x0024 */ char *laymem;
};

// from xpkrsvc.cpp
// size: 0x8F8
class st_PACKER_READ_DATA {
    /* 0x0000 */ class st_PACKER_ASSETTYPE *types;
    /* 0x0004 */ void *userdata;
    /* 0x0008 */ unsigned int opts;
    /* 0x000C */ unsigned int pkgver;
    /* 0x0010 */ signed int cltver;
    /* 0x0014 */ signed int subver;
    /* 0x0018 */ signed int compatver;
    /* 0x001C */ class st_HIPLOADDATA *pkg;
    /* 0x0020 */ unsigned int base_sector;
    /* 0x0024 */ signed int lockid;
    /* 0x0028 */ char packfile[128];
    /* 0x00A8 */ signed int asscnt;
    /* 0x00AC */ signed int laycnt;
    /* 0x00B0 */ class st_XORDEREDARRAY asstoc;
    /* 0x00C0 */ class st_XORDEREDARRAY laytoc;
    /* 0x00D0 */ class st_PACKER_ATOC_NODE *pool_anode;
    /* 0x00D4 */ signed int pool_nextaidx;
    /* 0x00D8 */ class st_XORDEREDARRAY typelist[129];
    /* 0x08E8 */ signed long time_made;
    /* 0x08F0 */ signed long time_mod;
};

// from xpkrsvc.cpp
// size: 0x40
class st_PACKER_READ_FUNCS {
    /* 0x0000 */ unsigned int api_ver;
    /* 0x0004 */ class st_PACKER_READ_DATA * (*Init)(void *, char *, unsigned int, signed int *, class st_PACKER_ASSETTYPE *);
    /* 0x0008 */ void (*Done)(class st_PACKER_READ_DATA *);
    /* 0x000C */ signed int (*LoadLayer)(class st_PACKER_READ_DATA *, enum en_LAYER_TYPE);
    /* 0x0010 */ unsigned int (*GetAssetSize)(class st_PACKER_READ_DATA *, unsigned int);
    /* 0x0014 */ void * (*LoadAsset)(class st_PACKER_READ_DATA *, unsigned int, char *, void *);
    /* 0x0018 */ void * (*AssetByType)(class st_PACKER_READ_DATA *, unsigned int, signed int, unsigned int *);
    /* 0x001C */ signed int (*AssetCount)(class st_PACKER_READ_DATA *, unsigned int);
    /* 0x0020 */ signed int (*IsAssetReady)(class st_PACKER_READ_DATA *, unsigned int);
    /* 0x0024 */ signed int (*SetActive)(class st_PACKER_READ_DATA *, enum en_LAYER_TYPE);
    /* 0x0028 */ char * (*AssetName)(class st_PACKER_READ_DATA *, unsigned int);
    /* 0x002C */ unsigned int (*GetBaseSector)(class st_PACKER_READ_DATA *);
    /* 0x0030 */ signed int (*GetAssetInfo)(class st_PACKER_READ_DATA *, unsigned int, class st_PKR_ASSET_TOCINFO *);
    /* 0x0034 */ signed int (*GetAssetInfoByType)(class st_PACKER_READ_DATA *, unsigned int, signed int, class st_PKR_ASSET_TOCINFO *);
    /* 0x0038 */ signed int (*PkgHasAsset)(class st_PACKER_READ_DATA *, unsigned int);
    /* 0x003C */ unsigned int (*PkgTimeStamp)(class st_PACKER_READ_DATA *);
};

// from xpkrsvc.cpp
// size: 0x14
class st_PKR_ASSET_TOCINFO {
    /* 0x0000 */ unsigned int aid;
    /* 0x0004 */ unsigned int sector;
    /* 0x0008 */ unsigned int plus_offset;
    /* 0x000C */ unsigned int size;
    /* 0x0010 */ void *mempos;
};

// from zEntProjectile.cpp
// size: 0x2C
class st_PROJECTILE_DEPOT {
    /* 0x0000 */ signed int typecnt;
    /* 0x0004 */ class st_ZPRJ_TYPERECORD *typetable;
    /* 0x0008 */ signed int typeidx;
    /* 0x000C */ signed int rawcnt;
    /* 0x0010 */ class zEntProjectile *rawpool;
    /* 0x0014 */ class st_ELIST prjpool;
    /* 0x0020 */ class st_ELIST prjinuse;
};

// from zEntVillain.cpp
// size: 0x730
class st_REDBEARD_SWORD : public _xEnt {
    /* 0x00E8 */ signed int flg_stat;
    /* 0x00EC */ enum en_RBS_STAGE stage;
    /* 0x00F0 */ unsigned int aid_mdlFile;
    /* 0x00F4 */ class RpAtomic *mdlFile;
    /* 0x00F8 */ class st_zEntVillain *rbvil;
    /* 0x00FC */ class xModelInstance *mdl_animsword;
    /* 0x0100 */ class _xEntFrame raw_frame;
    /* 0x01F0 */ class _xEntCollis raw_collis;
    /* 0x06EC */ class _xEnt *ent_colchk[5];
    /* 0x0700 */ float tym_age;
    /* 0x0704 */ float dst_far;
    /* 0x0708 */ float spd_fly;
    /* 0x070C */ class _xVec3 dir_end;
    /* 0x0718 */ class _xVec3 pos_end;
};

// from xserializer.cpp
// size: 0x10
class st_SERIAL_CLIENTINFO {
    /* 0x0000 */ unsigned int idtag;
    /* 0x0004 */ signed int *membuf;
    /* 0x0008 */ signed int trueoff;
    /* 0x000C */ signed int actsize;
};

// from zMain.cpp
// size: 0x8
class st_SERIAL_PERCID_SIZE {
    /* 0x0000 */ unsigned int idtag;
    /* 0x0004 */ signed int needsize;
};

// from zEnt.cpp
// size: 0x28
class st_STIMULUS_CONTEXT_DATA {
    /* 0x0000 */ enum en_NPC_STIMULUS stim;
    /* 0x0004 */ class _zEnt *tgt_ent;
    /* 0x0008 */ enum en_BEHAVIOUR_GOAL gods_goal;
    /* 0x0008 */ class _zEntBubble *bubble;
    /* 0x0008 */ class _xVec3 dir_headbutt;
    /* 0x0008 */ class _xCollis *colrec;
    /* 0x0008 */ class _xVec3 pos_guard;
    /* 0x0008 */ class _xVec3 gotopos;
    /* 0x0008 */ class xBase *widge;
    /* 0x0008 */ signed int use_alt_action;
    /* 0x0008 */ signed int mm_teleport_hurtz_doggie;
    /* 0x0008 */ class zSFX *talkSFX;
    /* 0x0014 */ float off_goto;
    /* 0x0014 */ float mag_headbutt;
    /* 0x0014 */ float amt_stun;
    /* 0x0018 */ signed int res_gods;
    /* 0x0018 */ enum en_ZVILLAIN_BUBBLE_RESPONSE res_bubble;
    /* 0x0018 */ enum en_ZVILLAIN_HEADBUTT_RESPONSE res_headbutt;
    /* 0x0018 */ enum en_ZVILLAIN_BOUNCE_RESPONSE res_buttbounce;
    /* 0x0018 */ enum en_ZVILLAIN_BUMPED_RESPONSE res_bumped;
    /* 0x001C */ signed int res_bogus;
    /* 0x0020 */ enum en_BEHAVIOUR_GOAL trangoal;
    /* 0x0024 */ enum en_NPC_MODE tranmode;
};

// from xstransvc.cpp
// size: 0x884
class st_STRAN_DATA {
    /* 0x0000 */ class st_STRAN_SCENE hipscn[8];
    /* 0x0880 */ unsigned int loadlock;
};

// from xstransvc.cpp
// size: 0x110
class st_STRAN_SCENE {
    /* 0x0000 */ unsigned int scnid;
    /* 0x0004 */ signed int lockid;
    /* 0x0008 */ class st_PACKER_READ_DATA *spkg;
    /* 0x000C */ void *userdata;
    /* 0x0010 */ char fnam[256];
};

// from xpkrsvc.cpp
// size: 0x10
class st_XORDEREDARRAY {
    /* 0x0000 */ void **list;
    /* 0x0004 */ signed int cnt;
    /* 0x0008 */ signed int max;
    /* 0x000C */ signed int warnlvl;
};

// from xsavegame.cpp
// size: 0x40
class st_XSAVEGAME_CLIENT {
    /* 0x0000 */ unsigned int idtag;
    /* 0x0004 */ signed int (*cltinfo)(void *, class st_XSAVEGAME_DATA *, signed int *, signed int *);
    /* 0x0008 */ signed int (*cltproc)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_WRITECONTEXT *);
    /* 0x000C */ signed int (*cltload)(void *, class st_XSAVEGAME_DATA *, class st_XSAVEGAME_READCONTEXT *, unsigned int, signed int);
    /* 0x0010 */ void *cltdata;
    /* 0x0014 */ signed int needamt;
    /* 0x0018 */ signed int maxamt;
    /* 0x001C */ signed int realamt;
    /* 0x0020 */ char *buf_sizepos;
    /* 0x0024 */ char *buf_maxpos;
    /* 0x0028 */ signed int blokact;
    /* 0x002C */ signed int blokmax;
    /* 0x0030 */ char *blokpos;
    /* 0x0034 */ char *readpos;
    /* 0x0038 */ signed int readamt;
    /* 0x003C */ signed int readremain;
};

// from xsavegame.cpp
// size: 0x20F0
class st_XSAVEGAME_DATA {
    /* 0x0000 */ signed int gfile_idx;
    /* 0x0004 */ enum en_SAVEGAME_MODE mode;
    /* 0x0008 */ unsigned int stage;
    /* 0x000C */ signed int gslot;
    /* 0x0010 */ char label[64];
    /* 0x0050 */ signed int thumIndex;
    /* 0x0058 */ signed long playtime;
    /* 0x0060 */ char *membuf;
    /* 0x0064 */ signed int memsize;
    /* 0x0068 */ char *buf_curpos;
    /* 0x006C */ char *buf_sizespot;
    /* 0x0070 */ char *buf_cksmspot;
    /* 0x0074 */ signed int totamt;
    /* 0x0078 */ unsigned int chksum;
    /* 0x007C */ unsigned int upd_tally;
    /* 0x0080 */ signed int cltneed;
    /* 0x0084 */ signed int cltmax;
    /* 0x0088 */ signed int chdrneed;
    /* 0x008C */ signed int stkcnt;
    /* 0x0090 */ class st_XSAVEGAME_CLIENT cltstk[128];
    /* 0x2090 */ class st_XSAVEGAME_CLIENT dfltloadclt;
    /* 0x20D0 */ unsigned int file_chksum;
    /* 0x20D4 */ unsigned int read_chksum;
    /* 0x20D8 */ signed int readsize;
    /* 0x20DC */ char *loadbuf;
    /* 0x20E0 */ signed int loadsize;
    /* 0x20E4 */ char *walkpos;
    /* 0x20E8 */ signed int walkremain;
    /* 0x20EC */ class st_ISGSESSION *isgsess;
};

// from xsavegame.cpp
// size: 0x50
class st_XSAVEGAME_LEADER {
    /* 0x0000 */ char gameLabel[64];
    /* 0x0040 */ signed int thumIconIndex;
    /* 0x0048 */ signed long gametime;
};

// from xserializer.cpp
// size: 0x24
class st_XSERIAL_DATA_PRIV {
    /* 0x0000 */ signed int flg_info;
    /* 0x0004 */ signed int *bitbuf;
    /* 0x0008 */ signed int buf_bytcnt;
    /* 0x000C */ class st_SERIAL_CLIENTINFO *cltbuf;
    /* 0x0010 */ class st_SERIAL_CLIENTINFO *cltnext;
    /* 0x0014 */ class st_XORDEREDARRAY cltlist;
};

// from zDispatcher.cpp
// size: 0x10
class st_ZDISPATCH_CONTEXT {
    /* 0x0000 */ enum en_DISPATCH_COMMAND cmd;
    /* 0x0004 */ void *indata;
    /* 0x0008 */ void *inxtra;
    /* 0x000C */ void *result;
};

// from zDispatcher.cpp
// size: 0x18
class st_ZDISPATCH_DATA : public xBase {
    /* 0x0010 */ class p2BaseAsset *rawass;
    /* 0x0014 */ signed int placeholder;
};

// from zDispatcher.cpp
// size: 0x8
class st_ZDISPATCH_DEPOT {
    /* 0x0000 */ class st_ZDISPATCH_DATA *raw_pool;
    /* 0x0004 */ signed int raw_cnt;
};

// from zEntProjectile.cpp
// size: 0xA4
class st_ZPRJ_AUTOCALC_INFO {
    /* 0x0000 */ enum en_ZPRJ_SOLVER_METHODS method;
    /* 0x0004 */ class st_ZPRJ_SOLV_METHOD_A metha;
    /* 0x0004 */ class st_ZPRJ_SOLV_METHOD_B methb;
    /* 0x0004 */ class st_ZPRJ_SOLV_METHOD_C methc;
};

// from zEnt.cpp
// size: 0x24
class st_ZPRJ_CATDATA {
    /* 0x0000 */ enum en_ZPRJ_CATDATA_FORM idx_cdat;
    /* 0x0004 */ class st_ZPRJ_CAT_ARROW arrow;
    /* 0x0004 */ class st_ZPRJ_CAT_GRAVBALL gravball;
    /* 0x0004 */ class st_ZPRJ_CAT_BOUNCEBALL bounce;
    /* 0x0004 */ class st_ZPRJ_CAT_PICKUP pickup;
};

// from zEntVillain.cpp
// size: 0x28
class st_ZPRJ_CATEGORY_FUNCS {
    /* 0x0000 */ signed int flg_cback;
    /* 0x0004 */ signed int (*attach)(class zEntProjectile *);
    /* 0x0008 */ void (*detach)(class zEntProjectile *);
    /* 0x000C */ signed int (*logix)(class zEntProjectile *, float, class zScene *, class _xEntFrame *);
    /* 0x0010 */ signed int (*calcmove)(class zEntProjectile *, float, class zScene *, class _xEntFrame *);
    /* 0x0014 */ signed int (*notify)(class zEntProjectile *, enum en_PROJECTILE_EVENT, void *);
    /* 0x0018 */ signed int (*render)(class zEntProjectile *);
    /* 0x001C */ signed int (*aimer)(class zEntProjectile *, class ST_CAT_AIMDATA *, signed int);
    /* 0x0020 */ signed int (*collide)(class zEntProjectile *, class zScene *, float);
    /* 0x0024 */ void (*dbgUpdate)(class zEntProjectile *, signed int);
};

// from zEntVillain.cpp
// size: 0xC
class st_ZPRJ_CATEGORY_PROPS {
    /* 0x0000 */ signed int flg_props;
    /* 0x0004 */ unsigned char flg_physics;
    /* 0x0005 */ unsigned char flg_chk;
    /* 0x0006 */ unsigned char flg_pen;
    /* 0x0007 */ unsigned char pad;
    /* 0x0008 */ signed int flg_categ;
};

// from zEnt.cpp
// size: 0x18
class st_ZPRJ_CAT_ARROW {
    /* 0x0000 */ float acc_gravity;
    /* 0x0004 */ signed int amStuck;
    /* 0x0008 */ class _xEnt *ent_stuck;
    /* 0x000C */ class _xVec3 pos_stuck;
};

// from zEnt.cpp
// size: 0x20
class st_ZPRJ_CAT_BOUNCEBALL : public st_ZPRJ_CAT_GRAVBALL {
    /* 0x0004 */ signed int atRest;
    /* 0x0008 */ float tym_atrest;
    /* 0x000C */ signed int cnt_curBounce;
    /* 0x0010 */ signed int cnt_maxBounce;
    /* 0x0014 */ float fac_elastic;
    /* 0x0018 */ float pct_randKick;
    /* 0x001C */ signed int skip_next_bounce;
};

// from zEnt.cpp
// size: 0x4
class st_ZPRJ_CAT_GRAVBALL {
    /* 0x0000 */ float acc_gravity;
};

// from zEnt.cpp
// size: 0x4
class st_ZPRJ_CAT_PICKUP {
    /* 0x0000 */ signed int nada;
};

// from zEnt.cpp
// size: 0x3C
class st_ZPRJ_COMMON {
    /* 0x0000 */ class _xVec3 vec_flex;
    /* 0x000C */ class _xMat3x3 mat_flex;
};

// from zEnt.cpp
// size: 0xC
class st_ZPRJ_LINKNODE {
    /* 0x0000 */ class st_ENODE node;
    /* 0x0008 */ class zEntProjectile *backref;
};

// from zEntProjectile.cpp
// size: 0x40
class st_ZPRJ_SOLV_METHOD_A {
    /* 0x0000 */ class _xVec3 vel_launch;
    /* 0x000C */ class _xVec3 dir_launch;
    /* 0x0018 */ float spd_muzzle;
    /* 0x001C */ class _xVec3 pos_launch;
    /* 0x0028 */ class _xVec3 pos_target;
    /* 0x0034 */ float acc_gravity;
    /* 0x0038 */ float fac_arcness;
    /* 0x003C */ float dst_range;
};

// from zEntProjectile.cpp
// size: 0xA0
class st_ZPRJ_SOLV_METHOD_B {
    /* 0x0000 */ signed int flg_stat;
    /* 0x0004 */ class _xVec3 vel_launch;
    /* 0x0010 */ class _xVec3 acc_gravity;
    /* 0x001C */ class _xVec3 pos_base;
    /* 0x0028 */ float spd_base;
    /* 0x002C */ class _xVec3 dir_base;
    /* 0x0038 */ class _xVec3 pos_tgt;
    /* 0x0044 */ class _xVec3 vel_tgt;
    /* 0x0050 */ float dst_tgtXZ;
    /* 0x0054 */ float spd_max;
    /* 0x0058 */ float spd_min;
    /* 0x005C */ signed int cnt_subspd;
    /* 0x0060 */ float ang_thmax;
    /* 0x0064 */ float ang_thmin;
    /* 0x0068 */ signed int cnt_subang;
    /* 0x006C */ float acc_maxgrav;
    /* 0x0070 */ float acc_mingrav;
    /* 0x0074 */ signed int cnt_subgrav;
    /* 0x0078 */ signed int is_winner;
    /* 0x007C */ signed int cnt_pause;
    /* 0x0080 */ float last_speed;
    /* 0x0084 */ float last_angle;
    /* 0x0088 */ float last_grav;
    /* 0x008C */ class st_NEWT_USERDATA newtdata;
};

// from zEntProjectile.cpp
// size: 0x2C
class st_ZPRJ_SOLV_METHOD_C {
    /* 0x0000 */ class _xVec3 vel_launch;
    /* 0x000C */ float spd_muzzle;
    /* 0x0010 */ float acc_gravity;
    /* 0x0014 */ class _xVec3 pos_launch;
    /* 0x0020 */ class _xVec3 pos_target;
};

// from zEntProjectile.cpp
// size: 0x14
class st_ZPRJ_TYPERECORD {
    /* 0x0000 */ unsigned int aid_ptype;
    /* 0x0004 */ class zProjectileTypeAsset *src_ptype;
    /* 0x0008 */ enum en_ZPRJ_CATEGORY categ;
    /* 0x000C */ class RpAtomic *mdlFile[2];
};

// from zEnt.cpp
// size: 0x1CC
class st_ZVIL_CONFIG_REFERENCE {
    /* 0x0000 */ class p2EntVillainAsset *vilp2ass;
    /* 0x0004 */ class p2EntAsset *asset;
    /* 0x0008 */ float spd_move;
    /* 0x000C */ float spd_chase;
    /* 0x0010 */ float spd_turn;
    /* 0x0014 */ float ds2_detect;
    /* 0x0018 */ float hyt_detect;
    /* 0x001C */ float hyt_dtctoff;
    /* 0x0020 */ float fov_detect;
    /* 0x0024 */ float ds2_pursue;
    /* 0x0028 */ float ds2_fromhome;
    /* 0x002C */ float spd_lob;
    /* 0x0030 */ float dst_lob;
    /* 0x0034 */ signed int cnt_lobSalvo;
    /* 0x0038 */ unsigned int prjTypeID;
    /* 0x003C */ float dst_extend;
    /* 0x0040 */ float wid_extend;
    /* 0x0044 */ float spd_extend;
    /* 0x0048 */ float ang_wander;
    /* 0x004C */ float fac_lobarc;
    /* 0x0050 */ float fac_lobheavy;
    /* 0x0054 */ float tymref_list[32];
    /* 0x00D4 */ signed int msk_condspawn;
    /* 0x00D8 */ signed int minDifficulty;
    /* 0x00DC */ signed int hitpts;
    /* 0x00E0 */ signed int misc_flags;
    /* 0x00E4 */ class _xVec3 scl_modelBase;
    /* 0x00F0 */ class _xVec3 sanim_idle;
    /* 0x00FC */ class _xVec3 sanim_move;
    /* 0x0108 */ class _xVec3 sanim_chase;
    /* 0x0114 */ float rad_bound;
    /* 0x0118 */ class _xVec3 vec_bndoffset;
    /* 0x0124 */ float rad_bound2;
    /* 0x0128 */ class _xVec3 vec_bnd2offset;
    /* 0x0134 */ float tym_attack_begin;
    /* 0x0138 */ float tym_attack_end;
    /* 0x013C */ class _xVec3 pos_verts[12];
};

// from zEntVillain.cpp
// size: 0x60
class st_ZVIL_NPCTYPE_LOOKUP {
    /* 0x0000 */ signed int enabled;
    /* 0x0004 */ float decay;
    /* 0x0008 */ float ds2_atpos;
    /* 0x000C */ float strength;
    /* 0x0010 */ float inv_mass;
    /* 0x0014 */ signed int dmg_give;
    /* 0x0018 */ float dst_noisy;
    /* 0x001C */ float rad_bound;
    /* 0x0020 */ class _xVec3 vec_bndoff;
    /* 0x002C */ float rad_bound2;
    /* 0x0030 */ class _xVec3 vec_bnd2off;
    /* 0x003C */ signed int ability;
    /* 0x0040 */ signed int abl_atak;
    /* 0x0044 */ signed int abl_dtct;
    /* 0x0048 */ signed int abl_move;
    /* 0x004C */ signed int abl_spyk;
    /* 0x0050 */ signed int abl_vuln;
    /* 0x0054 */ enum en_NPC_PERSONALITIES typ_person;
    /* 0x0058 */ unsigned char flg_physics;
    /* 0x0059 */ unsigned char pad[3];
    /* 0x005C */ unsigned char flg_chk;
    /* 0x005D */ unsigned char flg_pen;
    /* 0x005E */ unsigned char flg_chkby;
    /* 0x005F */ unsigned char flg_penby;
};

// from zEnt.cpp
// size: 0x34
class st_ZVIL_NPC_HANDLERFUNCS {
    /* 0x0000 */ signed int flg_funcs;
    /* 0x0004 */ signed int (*flags)(class st_zEntVillain *, enum en_ZVIL_FLAG_GROUPS, enum en_BEHAVIOUR_GOAL);
    /* 0x0008 */ signed int (*query)(class st_zEntVillain *, enum en_ZVIL_QUERY);
    /* 0x000C */ signed int (*sysevent)(class st_zEntVillain *, class xBase *, unsigned int, float *, class xBase *, class st_STIMULUS_CONTEXT_DATA *);
    /* 0x0010 */ signed int (*notice)(class st_zEntVillain *, enum en_ZVIL_NOTICE, class st_STIMULUS_CONTEXT_DATA *);
    /* 0x0014 */ signed int (*aware)(class st_zEntVillain *, class _xScene *, float, class st_STIMULUS_CONTEXT_DATA *);
    /* 0x0018 */ signed int (*stimulus)(class st_zEntVillain *, class st_STIMULUS_CONTEXT_DATA *, class st_STIMULUS_CONTEXT_DATA *);
    /* 0x001C */ signed int (*trangoal)(class st_zEntVillain *, class st_STIMULUS_CONTEXT_DATA *, float);
    /* 0x0020 */ signed int (*calcmove)(class st_zEntVillain *, class _xScene *, float, class _xEntFrame *);
    /* 0x0024 */ signed int (*respond)(class st_zEntVillain *, class st_STIMULUS_CONTEXT_DATA *);
    /* 0x0028 */ signed int (*pickanim)(class st_zEntVillain *, enum en_BEHAVIOUR_GOAL, enum en_ZVIL_GOAL_SPOT);
    /* 0x002C */ signed int (*animtran)(class st_zEntVillain *, class xAnimTransition *, class xAnimSingle *);
    /* 0x0030 */ signed int (*xtrarend)(class st_zEntVillain *);
};

// from zEnt.cpp
// size: 0x64
class st_ZVIL_PRECALC {
    /* 0x0000 */ signed int flg_status;
    /* 0x0004 */ float ds2_VtoP;
    /* 0x0008 */ float dst_VtoP;
    /* 0x000C */ float ang_VtoP;
    /* 0x0010 */ class _xVec3 dir_VtoP;
    /* 0x001C */ class _xVec3 vec_VtoP;
    /* 0x0028 */ unsigned int plyr_mvinf;
    /* 0x002C */ signed int flg_plyr;
    /* 0x0030 */ signed int flg_xtra;
    /* 0x0034 */ float dif_plyrCtr;
    /* 0x0038 */ float dif_vilCtr;
    /* 0x003C */ float hyt_vil;
    /* 0x0040 */ float hyt_plyr;
    /* 0x0044 */ float ds2_anchor;
    /* 0x0048 */ float ds2_grdpos;
    /* 0x004C */ float cos_faceang;
    /* 0x0050 */ float sin_faceang;
    /* 0x0054 */ float ds2_plyrToAnchor;
    /* 0x0058 */ class _xVec3 pos_intercept;
};

// from zEntVillain.cpp
// size: 0x30
class st_ZVMM_FLASK_DATA {
    /* 0x0000 */ signed int flg_stat;
    /* 0x0004 */ enum en_ZVMM_FLASK_STAGE stage;
    /* 0x0008 */ enum en_ZVMM_FLASK_TYPE typ_flask;
    /* 0x000C */ class _xVec3 pos_tgt;
    /* 0x0018 */ float spd_flight;
    /* 0x001C */ float arcness;
    /* 0x0020 */ float heavy;
    /* 0x0024 */ float tym_aftermath;
    /* 0x0028 */ float hyt_aftermath;
    /* 0x002C */ float rad_aftermath;
};

// from xstransvc.cpp
// size: 0x14
class st_xST_XASSETINFO {
    /* 0x0000 */ unsigned int aid;
    /* 0x0004 */ unsigned int sector;
    /* 0x0008 */ unsigned int plus_offset;
    /* 0x000C */ unsigned int size;
    /* 0x0010 */ void *mempos;
};

// from zEnt.cpp
// size: 0x790
class st_zEntVillain : public _zEnt {
    /* 0x00EC */ class st_ZVIL_NPC_HANDLERFUNCS *vhf;
    /* 0x00F0 */ signed int ability;
    /* 0x00F4 */ signed int abl_atak;
    /* 0x00F8 */ signed int abl_dtct;
    /* 0x00FC */ signed int abl_move;
    /* 0x0100 */ signed int abl_spyk;
    /* 0x0104 */ signed int abl_vuln;
    /* 0x0108 */ enum en_NPC_PERSONALITIES typ_person;
    /* 0x010C */ signed int flg_misc;
    /* 0x0110 */ enum en_NPC_MODE mode;
    /* 0x0114 */ enum en_BEHAVIOUR_GOAL goal;
    /* 0x0118 */ enum en_BEHAVIOUR_GOAL goaldflt;
    /* 0x011C */ signed int standby;
    /* 0x0120 */ signed int anim_playing;
    /* 0x0124 */ signed int anim_playsync;
    /* 0x0128 */ enum en_NPC_ANIMSTATES anim_playnext;
    /* 0x012C */ float anim_loopmult;
    /* 0x0130 */ class _xVec3 sanim_idle;
    /* 0x013C */ class _xVec3 sanim_move;
    /* 0x0148 */ class _xVec3 sanim_chase;
    /* 0x0154 */ float spd_move;
    /* 0x0158 */ float spd_chase;
    /* 0x015C */ float spd_turn;
    /* 0x0160 */ float ds2_detect;
    /* 0x0164 */ float fov_detect;
    /* 0x0168 */ float hyt_detect;
    /* 0x016C */ float hyt_dtctoff;
    /* 0x0170 */ float ds2_pursue;
    /* 0x0174 */ float spd_lob;
    /* 0x0178 */ float dst_lob;
    /* 0x017C */ signed int cnt_lobSalvo;
    /* 0x0180 */ unsigned int prjTypeID;
    /* 0x0184 */ class _xVec3 vec_lobSpin;
    /* 0x0190 */ float dst_extend;
    /* 0x0194 */ float wid_extend;
    /* 0x0198 */ float spd_extend;
    /* 0x019C */ class _xVec3 pos_exttgt;
    /* 0x01A8 */ float ang_wander;
    /* 0x01AC */ float fac_lobarc;
    /* 0x01B0 */ float fac_lobheavy;
    /* 0x01B4 */ class iModelTag tag_vert[12];
    /* 0x0334 */ signed int flg_vert;
    /* 0x0338 */ class tagzFeet feet;
    /* 0x0344 */ class zSFX *sfx_curTalk;
    /* 0x0348 */ class _xBound bound2;
    /* 0x0380 */ signed int allow;
    /* 0x0384 */ class _zEntBubble *bubble;
    /* 0x0388 */ float scl_nombub;
    /* 0x038C */ class xModelInstance *mdl_gumBlob;
    /* 0x0390 */ signed int hitpts;
    /* 0x0394 */ unsigned int flinch;
    /* 0x0398 */ signed int goodattack;
    /* 0x039C */ class _zMovePoint *nav_mvptlist;
    /* 0x03A0 */ class _zMovePoint *nav_next;
    /* 0x03A4 */ class _zMovePoint *nav_prev;
    /* 0x03A8 */ signed int need_navpt;
    /* 0x03AC */ float dst_navTween;
    /* 0x03B0 */ signed int domove;
    /* 0x03B4 */ class _xVec3 pos_moveto;
    /* 0x03C0 */ class _xVec3 dlt_moveto;
    /* 0x03CC */ class _xVec3 dir_moveto;
    /* 0x03D8 */ float ang_moveto;
    /* 0x03DC */ class _xVec3 vel_moveto;
    /* 0x03E8 */ float dlt_faceang;
    /* 0x03EC */ float ang_mdlface;
    /* 0x03F0 */ float spd_moveto;
    /* 0x03F4 */ float wiz_moveto;
    /* 0x03F8 */ float ang_spiral;
    /* 0x03FC */ float rad_spiral;
    /* 0x0400 */ class _xVec3 pos_vortex;
    /* 0x040C */ class _xVec3 vec_knocked;
    /* 0x0418 */ class _xVec3 pos_bowling;
    /* 0x0424 */ class _xVec3 dir_bowling;
    /* 0x0430 */ class _xVec3 pos_pursue_home;
    /* 0x043C */ class _xVec3 pos_guard_home;
    /* 0x0448 */ class _xVec3 pos_goto;
    /* 0x0454 */ float ds2_goto_keepaway;
    /* 0x0458 */ class _xEnt *tgt_follow;
    /* 0x045C */ class _zMovePoint *tgt_bullseye;
    /* 0x0460 */ class _zEnt *tgt_protect;
    /* 0x0464 */ signed int tgt_stat;
    /* 0x0468 */ float ang_listen;
    /* 0x046C */ class zEntProjectile *prj_reserved;
    /* 0x0470 */ float tmr_list[32];
    /* 0x04F0 */ class st_ZVIL_PRECALC precalc;
    /* 0x0554 */ class st_NPC_SOUND_QUEUE snd_queue[4];
    /* 0x05C4 */ class st_ZVIL_CONFIG_REFERENCE vilcfg;
};

// from zEntVillain.cpp
// size: 0x14
class st_zEntVillain_ParEmitInfo {
    /* 0x0000 */ char *emitname;
    /* 0x0004 */ enum en_VPAR_EMITTER_ROLE emitrole;
    /* 0x0008 */ enum en_zEntVillainSubTypeID viltype;
    /* 0x000C */ signed int flg_need;
    /* 0x0010 */ signed int flg_omit;
};

// from zEntVillain.cpp
// size: 0x20
class st_zEntVillain_ParEmitRecord {
    /* 0x0000 */ enum en_VPAR_EMITTER_ROLE emitrole;
    /* 0x0004 */ enum en_zEntVillainSubTypeID viltype;
    /* 0x0008 */ signed int priority;
    /* 0x000C */ unsigned int vilowner;
    /* 0x0010 */ unsigned int emitterID;
    /* 0x0014 */ class _zParEmitter *emitctxt;
    /* 0x0018 */ signed int flg_need;
    /* 0x001C */ signed int flg_omit;
};

// from disp.c
// size: 0x4
class tD_CHCR {
    /* 0x0000 */ unsigned int DIR : 1 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int p0 : 1 /* bit_offset 1 */;
    /* 0x0000 */ unsigned int MOD : 2 /* bit_offset 2 */;
    /* 0x0000 */ unsigned int ASP : 2 /* bit_offset 4 */;
    /* 0x0000 */ unsigned int TTE : 1 /* bit_offset 6 */;
    /* 0x0000 */ unsigned int TIE : 1 /* bit_offset 7 */;
    /* 0x0000 */ unsigned int STR : 1 /* bit_offset 8 */;
    /* 0x0000 */ unsigned int p1 : 7 /* bit_offset 9 */;
    /* 0x0000 */ unsigned int TAG : 16 /* bit_offset 16 */;
};

// from ps2_mpeg.c
// size: 0x8
class tGS_BGCOLOR {
    /* 0x0000 */ unsigned int R : 8 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int G : 8 /* bit_offset 8 */;
    /* 0x0000 */ unsigned int B : 8 /* bit_offset 16 */;
    /* 0x0000 */ unsigned int p0 : 8 /* bit_offset 24 */;
    /* 0x0004 */ unsigned int p1;
};

// from ps2_mpeg.c
// size: 0x8
class tGS_DISPFB2 {
    /* 0x0000 */ unsigned int FBP : 9 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int FBW : 6 /* bit_offset 9 */;
    /* 0x0000 */ unsigned int PSM : 5 /* bit_offset 15 */;
    /* 0x0000 */ unsigned int p0 : 12 /* bit_offset 20 */;
    /* 0x0004 */ unsigned int DBX : 11 /* bit_offset 0 */;
    /* 0x0004 */ unsigned int DBY : 11 /* bit_offset 11 */;
    /* 0x0004 */ unsigned int p1 : 10 /* bit_offset 22 */;
};

// from ps2_mpeg.c
// size: 0x8
class tGS_DISPLAY2 {
    /* 0x0000 */ unsigned int DX : 12 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int DY : 11 /* bit_offset 12 */;
    /* 0x0000 */ unsigned int MAGH : 4 /* bit_offset 23 */;
    /* 0x0000 */ unsigned int MAGV : 2 /* bit_offset 27 */;
    /* 0x0000 */ unsigned int p0 : 3 /* bit_offset 29 */;
    /* 0x0004 */ unsigned int DW : 12 /* bit_offset 0 */;
    /* 0x0004 */ unsigned int DH : 11 /* bit_offset 12 */;
    /* 0x0004 */ unsigned int p1 : 9 /* bit_offset 23 */;
};

// from ps2_mpeg.c
// size: 0x8
class tGS_PMODE {
    /* 0x0000 */ unsigned int EN1 : 1 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int EN2 : 1 /* bit_offset 1 */;
    /* 0x0000 */ unsigned int CRTMD : 3 /* bit_offset 2 */;
    /* 0x0000 */ unsigned int MMOD : 1 /* bit_offset 5 */;
    /* 0x0000 */ unsigned int AMOD : 1 /* bit_offset 6 */;
    /* 0x0000 */ unsigned int SLBG : 1 /* bit_offset 7 */;
    /* 0x0000 */ unsigned int ALP : 8 /* bit_offset 8 */;
    /* 0x0000 */ unsigned int p0 : 16 /* bit_offset 16 */;
    /* 0x0004 */ unsigned int p1;
};

// from ps2_mpeg.c
// size: 0x8
class tGS_SMODE2 {
    /* 0x0000 */ unsigned int INT : 1 /* bit_offset 0 */;
    /* 0x0000 */ unsigned int FFMD : 1 /* bit_offset 1 */;
    /* 0x0000 */ unsigned int DPMS : 2 /* bit_offset 2 */;
    /* 0x0000 */ unsigned int p0 : 28 /* bit_offset 4 */;
    /* 0x0004 */ unsigned int p1;
};

// from iDebug.cpp
// size: 0xC
class tagBITMAPFILEHEADER {
    /* 0x0000 */ signed int bfSize;
    /* 0x0004 */ signed short bfReserved1;
    /* 0x0006 */ signed short bfReserved2;
    /* 0x0008 */ signed int bfOffBits;
};

// from iDebug.cpp
// size: 0x28
class tagBITMAPINFOHEADER {
    /* 0x0000 */ signed int biSize;
    /* 0x0004 */ signed int biWidth;
    /* 0x0008 */ signed int biHeight;
    /* 0x000C */ signed short biPlanes;
    /* 0x000E */ signed short biBitCount;
    /* 0x0010 */ signed int biCompression;
    /* 0x0014 */ signed int biSizeImage;
    /* 0x0018 */ signed int biXPelsPerMeter;
    /* 0x001C */ signed int biYPelsPerMeter;
    /* 0x0020 */ signed int biClrUsed;
    /* 0x0024 */ signed int biClrImportant;
};

// from iFile.cpp
// size: 0xB0
class tag_iFile {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ char path[128];
    /* 0x0084 */ signed int fd;
    /* 0x0088 */ class sceCdlFILE file;
    /* 0x00AC */ void (*cb)(signed int);
};

// from iFile.cpp
// size: 0xC0
class tag_xFile {
    /* 0x0000 */ char relname[16];
    /* 0x0010 */ class tag_iFile ps;
};

// from iParMgr.cpp
// size: 0x5280
class tagiRenderArrays {
    /* 0x0000 */ unsigned short m_index[960];
    /* 0x0780 */ class RxObjSpace3DVertex m_vertex[480];
    /* 0x4B00 */ float m_vertexTZ[480];
};

// from iParMgr.cpp
// size: 0x80
class tagiRenderInput {
    /* 0x0000 */ unsigned short *m_index;
    /* 0x0004 */ class RxObjSpace3DVertex *m_vertex;
    /* 0x0008 */ float *m_vertexTZ;
    /* 0x000C */ unsigned int m_mode;
    /* 0x0010 */ signed int m_vertexType;
    /* 0x0014 */ signed int m_vertexTypeSize;
    /* 0x0018 */ signed int m_indexCount;
    /* 0x001C */ signed int m_vertexCount;
    /* 0x0020 */ class _xMat4x3 m_camViewMatrix;
    /* 0x0060 */ class _xVec4 m_camViewR;
    /* 0x0070 */ class _xVec4 m_camViewU;
};

// from zEnt.cpp
// size: 0xC
class tagzFeet {
    /* 0x0000 */ signed int total;
    /* 0x0004 */ class _xEnt *owner;
    /* 0x0008 */ signed short entity_type;
    /* 0x000A */ signed short pitch;
};

// from xAnim.cpp
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

// from xScrFx.cpp
// size: 0x8
class xAnimActiveEffect {
    /* 0x0000 */ class xAnimEffect *Effect;
    /* 0x0004 */ unsigned int Handle;
};

// from xScrFx.cpp
// size: 0x14
class xAnimEffect {
    /* 0x0000 */ class xAnimEffect *Next;
    /* 0x0004 */ unsigned int Flags;
    /* 0x0008 */ float StartTime;
    /* 0x000C */ float EndTime;
    /* 0x0010 */ unsigned int (*Callback)(unsigned int, class xAnimActiveEffect *, class xAnimSingle *, void *);
};

// from xScrFx.cpp
// size: 0x44
class xAnimFile {
    /* 0x0000 */ class xAnimFile *Next;
    /* 0x0004 */ char *Name;
    /* 0x0008 */ unsigned int ID;
    /* 0x000C */ unsigned int FileFlags;
    /* 0x0010 */ float Duration;
    /* 0x0014 */ float TimeOffset;
    /* 0x0018 */ float PhysBase[4];
    /* 0x0028 */ float PhysDelta[4];
    /* 0x0038 */ unsigned short BoneCount;
    /* 0x003A */ unsigned char NumAnims[2];
    /* 0x003C */ void **RawData;
    /* 0x0040 */ void *Physics;
};

// from xScrFx.cpp
// size: 0x60
class xAnimPlay {
    /* 0x0000 */ class xAnimPlay *Next;
    /* 0x0004 */ unsigned short NumSingle;
    /* 0x0006 */ unsigned short BoneCount;
    /* 0x0008 */ class xAnimSingle *Single;
    /* 0x000C */ void *Object;
    /* 0x0010 */ class xAnimTable *Table;
    /* 0x0014 */ class _xVec3 Translate[2];
    /* 0x002C */ unsigned int pad1;
    /* 0x0030 */ class _xQuat Quat[2];
    /* 0x0050 */ float Yaw;
    /* 0x0054 */ class xMemPool *Pool;
    /* 0x0058 */ class xModelInstance *ModelInst;
    /* 0x005C */ unsigned int pad2;
};

// from xScrFx.cpp
// size: 0x60
class xAnimSingle {
    /* 0x0000 */ unsigned int SingleFlags;
    /* 0x0004 */ class xAnimState *State;
    /* 0x0008 */ float Time;
    /* 0x000C */ float CurrentSpeed;
    /* 0x0010 */ float BilinearLerp[2];
    /* 0x0018 */ class xAnimEffect *Effect;
    /* 0x001C */ unsigned int ActiveCount;
    /* 0x0020 */ class xAnimActiveEffect *ActiveList;
    /* 0x0024 */ class xAnimPlay *Play;
    /* 0x0028 */ float LastTime;
    /* 0x002C */ class _xVec3 LastTranslation;
    /* 0x0038 */ float LastYaw;
    /* 0x003C */ unsigned int pad;
    /* 0x0040 */ class _xQuat LastQuat;
    /* 0x0050 */ class xAnimTransition *Sync;
    /* 0x0054 */ class xAnimTransition *Tran;
    /* 0x0058 */ class xAnimSingle *Blend;
    /* 0x005C */ float BlendFactor;
};

// from xScrFx.cpp
// size: 0x38
class xAnimState {
    /* 0x0000 */ class xAnimState *Next;
    /* 0x0004 */ char *Name;
    /* 0x0008 */ unsigned int ID;
    /* 0x000C */ unsigned int Flags;
    /* 0x0010 */ unsigned int UserFlags;
    /* 0x0014 */ float Speed;
    /* 0x0018 */ class xAnimFile *Data;
    /* 0x001C */ class xAnimEffect *Effects;
    /* 0x0020 */ class xAnimTransitionList *Default;
    /* 0x0024 */ class xAnimTransitionList *List;
    /* 0x0028 */ float *BoneBlend;
    /* 0x002C */ float *TimeSnap;
    /* 0x0030 */ float FadeRecip;
    /* 0x0034 */ unsigned short *FadeOffset;
};

// from xScrFx.cpp
// size: 0x18
class xAnimTable {
    /* 0x0000 */ class xAnimTable *Next;
    /* 0x0004 */ char *Name;
    /* 0x0008 */ class xAnimTransition *TransitionList;
    /* 0x000C */ class xAnimState *StateList;
    /* 0x0010 */ unsigned int AnimIndex;
    /* 0x0014 */ unsigned int MorphIndex;
};

// from xScrFx.cpp
// size: 0x2C
class xAnimTransition {
    /* 0x0000 */ class xAnimTransition *Next;
    /* 0x0004 */ class xAnimState *Dest;
    /* 0x0008 */ unsigned int (*Conditional)(class xAnimTransition *, class xAnimSingle *, void *);
    /* 0x000C */ unsigned int (*Callback)(class xAnimTransition *, class xAnimSingle *, void *);
    /* 0x0010 */ unsigned int Flags;
    /* 0x0014 */ unsigned int UserFlags;
    /* 0x0018 */ float SrcTime;
    /* 0x001C */ float DestTime;
    /* 0x0020 */ unsigned short Priority;
    /* 0x0022 */ unsigned short QueuePriority;
    /* 0x0024 */ float BlendRecip;
    /* 0x0028 */ unsigned short *BlendOffset;
};

// from xScrFx.cpp
// size: 0x8
class xAnimTransitionList {
    /* 0x0000 */ class xAnimTransitionList *Next;
    /* 0x0004 */ class xAnimTransition *T;
};

// from xScrFx.cpp
// size: 0x10
class xBase {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ unsigned char baseType;
    /* 0x0005 */ unsigned char linkCount;
    /* 0x0006 */ unsigned short baseFlags;
    /* 0x0008 */ class p2LinkAsset *link;
    /* 0x000C */ signed int (*eventFunc)(class xBase *, class xBase *, unsigned int, float *, class xBase *);
};

// from xScrFx.cpp
// size: 0x88
class xCamAsset : public p2BaseAsset {
    /* 0x0008 */ class _xVec3 pos;
    /* 0x0014 */ class _xVec3 at;
    /* 0x0020 */ class _xVec3 up;
    /* 0x002C */ class _xVec3 right;
    /* 0x0038 */ class _xVec3 view_offset;
    /* 0x0044 */ signed short offset_start_frames;
    /* 0x0046 */ signed short offset_end_frames;
    /* 0x0048 */ float fov;
    /* 0x004C */ float trans_time;
    /* 0x0050 */ enum _tagTransType trans_type;
    /* 0x0054 */ unsigned int flags;
    /* 0x0058 */ float fade_up;
    /* 0x005C */ float fade_down;
    /* 0x0060 */ class _tagxCamFollowAsset cam_follow;
    /* 0x0060 */ class _tagxCamShoulderAsset cam_shoulder;
    /* 0x0060 */ class _tagp2CamStaticAsset cam_static;
    /* 0x0060 */ class _tagxCamPathAsset cam_path;
    /* 0x0060 */ class _tagp2CamStaticFollowAsset cam_staticFollow;
    /* 0x0078 */ unsigned int valid_flags;
    /* 0x007C */ unsigned int markerid[2];
    /* 0x0084 */ unsigned char cam_type;
    /* 0x0085 */ unsigned char pad[3];
};

// from zScene.cpp
// size: 0x10
class xCoef {
    /* 0x0000 */ float a[4];
};

// from zScene.cpp
// size: 0x30
class xCoef3 {
    /* 0x0000 */ class xCoef x;
    /* 0x0010 */ class xCoef y;
    /* 0x0020 */ class xCoef z;
};

// from xScrFx.cpp
// size: 0x138
class xCutscene {
    /* 0x0000 */ class xCutsceneInfo *Info;
    /* 0x0004 */ class xCutsceneData *Data;
    /* 0x0008 */ unsigned int *TimeChunkOffs;
    /* 0x000C */ unsigned int *Visibility;
    /* 0x0010 */ class xCutsceneBreak *BreakList;
    /* 0x0014 */ class xCutsceneTime *Play;
    /* 0x0018 */ class xCutsceneTime *Stream;
    /* 0x001C */ unsigned int Waiting;
    /* 0x0020 */ void *RawBuf;
    /* 0x0024 */ void *AlignBuf;
    /* 0x0028 */ float Time;
    /* 0x002C */ float CamTime;
    /* 0x0030 */ unsigned int PlayIndex;
    /* 0x0034 */ unsigned int Ready;
    /* 0x0038 */ signed int DataLoading;
    /* 0x003C */ unsigned int GotData;
    /* 0x0040 */ unsigned int ShutDownWait;
    /* 0x0044 */ float PlaybackSpeed;
    /* 0x0048 */ unsigned int Opened;
    /* 0x004C */ class tag_xFile File;
    /* 0x010C */ unsigned int CurrSector;
    /* 0x0110 */ void *MemBuf;
    /* 0x0114 */ void *MemCurr;
    /* 0x0118 */ unsigned int SndStarted;
    /* 0x011C */ unsigned int SndNumChannel;
    /* 0x0120 */ unsigned int SndChannelReq[2];
    /* 0x0128 */ unsigned int SndAssetID[2];
    /* 0x0130 */ unsigned int SndHandle[2];
};

// from xCutscene.cpp
// size: 0x10
class xCutsceneAnimHdr {
    /* 0x0000 */ unsigned int RootIndex;
    /* 0x0004 */ float Translate[3];
};

// from xScrFx.cpp
// size: 0x8
class xCutsceneBreak {
    /* 0x0000 */ float Time;
    /* 0x0004 */ signed int Index;
};

// from xScrFx.cpp
// size: 0x10
class xCutsceneData {
    /* 0x0000 */ unsigned int DataType;
    /* 0x0004 */ unsigned int AssetID;
    /* 0x0008 */ unsigned int ChunkSize;
    /* 0x000C */ unsigned int FileOffset;
    /* 0x000C */ void *DataPtr;
};

// from xScrFx.cpp
// size: 0x50
class xCutsceneInfo {
    /* 0x0000 */ unsigned int Magic;
    /* 0x0004 */ unsigned int AssetID;
    /* 0x0008 */ unsigned int NumData;
    /* 0x000C */ unsigned int NumTime;
    /* 0x0010 */ unsigned int MaxModel;
    /* 0x0014 */ unsigned int MaxBufEven;
    /* 0x0018 */ unsigned int MaxBufOdd;
    /* 0x001C */ unsigned int HeaderSize;
    /* 0x0020 */ unsigned int VisCount;
    /* 0x0024 */ unsigned int VisSize;
    /* 0x0028 */ unsigned int BreakCount;
    /* 0x002C */ unsigned int pad;
    /* 0x0030 */ char SoundLeft[16];
    /* 0x0040 */ char SoundRight[16];
};

// from xCutscene.cpp
// size: 0x8
class xCutsceneMphFrame {
    /* 0x0000 */ float Time;
    /* 0x0004 */ unsigned int Flags;
};

// from xScrFx.cpp
// size: 0x10
class xCutsceneTime {
    /* 0x0000 */ float StartTime;
    /* 0x0004 */ float EndTime;
    /* 0x0008 */ unsigned int NumData;
    /* 0x000C */ unsigned int ChunkIndex;
};

// from xEnt.cpp
// size: 0x10
class xFFX {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ void (*doEffect)(class _xEnt *, class _xScene *, float, void *);
    /* 0x0008 */ void *fdata;
    /* 0x000C */ class xFFX *next;
};

// from zEntPlayer.cpp
// size: 0x44
class xFFXRotMatchState {
    /* 0x0000 */ signed int lgrounded;
    /* 0x0004 */ class _xVec3 lfup;
    /* 0x0010 */ class _xVec3 lfat;
    /* 0x001C */ class _xVec3 plfat;
    /* 0x0028 */ float tmr;
    /* 0x002C */ float mrate;
    /* 0x0030 */ float tmatch;
    /* 0x0034 */ float rrate;
    /* 0x0038 */ float trelax;
    /* 0x003C */ float max_decl;
    /* 0x0040 */ class xFFXRotMatchState *next;
};

// from zPlatform.cpp
// size: 0x24
class xFFXShakeState {
    /* 0x0000 */ class _xVec3 disp;
    /* 0x000C */ float dur;
    /* 0x0010 */ float freq;
    /* 0x0014 */ float tmr;
    /* 0x0018 */ float alpha;
    /* 0x001C */ float lval;
    /* 0x0020 */ class xFFXShakeState *next;
};

// from zMain.cpp
// size: 0x4C
class xFontBox {
    /* 0x0000 */ signed int flags;
    /* 0x0004 */ float x;
    /* 0x0008 */ float y;
    /* 0x000C */ float w;
    /* 0x0010 */ float h;
    /* 0x0014 */ unsigned int font;
    /* 0x0018 */ class iColor_tag color;
    /* 0x001C */ class iColor_tag saved_color;
    /* 0x0020 */ float draw_width;
    /* 0x0024 */ float draw_height;
    /* 0x0028 */ float x_spacing;
    /* 0x002C */ float y_spacing;
    /* 0x0030 */ float depth;
    /* 0x0034 */ float inset_left;
    /* 0x0038 */ float inset_right;
    /* 0x003C */ float inset_top;
    /* 0x0040 */ float inset_bottom;
    /* 0x0044 */ char *text;
    /* 0x0048 */ class iColor_tag backdrop_color;
};

// from xFont.cpp
// size: 0x10
class xFontGlobals_t {
    /* 0x0000 */ float dpx;
    /* 0x0004 */ float dpy;
    /* 0x0008 */ unsigned int draw;
    /* 0x000C */ float aspect_ratio;
};

// from xFont.cpp
// size: 0x1A8
class xFontInfo {
    /* 0x0000 */ unsigned short id;
    /* 0x0002 */ unsigned short flags;
    /* 0x0004 */ unsigned short char_width;
    /* 0x0006 */ unsigned short char_height;
    /* 0x0008 */ float oo_tex_width;
    /* 0x000C */ float oo_tex_height;
    /* 0x0010 */ float u;
    /* 0x0014 */ float v;
    /* 0x0018 */ unsigned short layout;
    /* 0x001A */ unsigned short pad;
    /* 0x001C */ unsigned char lookup[128];
    /* 0x009C */ class iFontSpacing pspc[128];
    /* 0x019C */ char *characters;
    /* 0x01A0 */ class iFontInfo ps;
};

// from xMemMgr.cpp
// size: 0x10
class xHeapState_tag {
    /* 0x0000 */ unsigned int curr;
    /* 0x0004 */ unsigned short blk_ct;
    /* 0x0006 */ unsigned short pad;
    /* 0x0008 */ unsigned int used;
    /* 0x000C */ unsigned int wasted;
};

// from zMain.cpp
// size: 0x214
class xIniFile {
    /* 0x0000 */ signed int NumValues;
    /* 0x0004 */ signed int NumSections;
    /* 0x0008 */ class xIniValue *Values;
    /* 0x000C */ class xIniSection *Sections;
    /* 0x0010 */ void *mem;
    /* 0x0014 */ char name[256];
    /* 0x0114 */ char pathname[256];
};

// from zMain.cpp
// size: 0xC
class xIniSection {
    /* 0x0000 */ char *sec;
    /* 0x0004 */ signed int first;
    /* 0x0008 */ signed int count;
};

// from zMain.cpp
// size: 0x8
class xIniValue {
    /* 0x0000 */ char *tok;
    /* 0x0004 */ char *val;
};

// from xMemMgr.cpp
// size: 0xC
class xMemArea_tag {
    /* 0x0000 */ unsigned int addr;
    /* 0x0004 */ unsigned int size;
    /* 0x0008 */ unsigned int flags;
};

// from xMemMgr.cpp
// size: 0x1C
class xMemBlkInfo_tag {
    /* 0x0000 */ class xMemBlock_tag *header;
    /* 0x0004 */ unsigned int pre;
    /* 0x0008 */ unsigned int block;
    /* 0x000C */ unsigned int post;
    /* 0x0010 */ unsigned int curr;
    /* 0x0014 */ unsigned int waste;
    /* 0x0018 */ unsigned int total;
};

// from xMemMgr.cpp
// size: 0x8
class xMemBlock_tag {
    /* 0x0000 */ unsigned int addr;
    /* 0x0004 */ unsigned int size;
};

// from xMemMgr.cpp
// size: 0x9C
class xMemHeap_tag {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ unsigned int hard_base;
    /* 0x0008 */ unsigned int size;
    /* 0x000C */ signed short opp_heap[2];
    /* 0x0010 */ class xHeapState_tag state[8];
    /* 0x0090 */ unsigned short state_idx;
    /* 0x0092 */ unsigned short max_blks;
    /* 0x0094 */ class xMemBlock_tag *blk;
    /* 0x0098 */ class xMemBlock_tag *lastblk;
};

// from xMemMgr.cpp
// size: 0x30
class xMemInfo_tag {
    /* 0x0000 */ class xMemArea_tag system;
    /* 0x000C */ class xMemArea_tag stack;
    /* 0x0018 */ class xMemArea_tag DRAM;
    /* 0x0024 */ class xMemArea_tag SRAM;
};

// from xScrFx.cpp
// size: 0x1C
class xMemPool {
    /* 0x0000 */ void *FreeList;
    /* 0x0004 */ unsigned short NextOffset;
    /* 0x0006 */ unsigned short Flags;
    /* 0x0008 */ void *UsedList;
    /* 0x000C */ void (*InitCB)(class xMemPool *, void *);
    /* 0x0010 */ void *Buffer;
    /* 0x0014 */ unsigned short Size;
    /* 0x0016 */ unsigned short NumRealloc;
    /* 0x0018 */ unsigned int Total;
};

// from xEnt.cpp
// size: 0x20
class xModCollRec {
    /* 0x0000 */ class _xEnt *owner;
    /* 0x0004 */ unsigned int nusing;
    /* 0x0008 */ signed long ctime;
    /* 0x0010 */ signed long ltime;
    /* 0x0018 */ float frac;
};

// from xScrFx.cpp
// size: 0x30
class xModelInstance {
    /* 0x0000 */ class xModelInstance *Next;
    /* 0x0004 */ class xModelInstance *Parent;
    /* 0x0008 */ class xModelPool *Pool;
    /* 0x000C */ class xAnimPlay *Anim;
    /* 0x0010 */ class RpAtomic *Data;
    /* 0x0014 */ void *Object;
    /* 0x0018 */ unsigned short Flags;
    /* 0x001A */ unsigned char BoneCount;
    /* 0x001B */ unsigned char BoneIndex;
    /* 0x001C */ unsigned char *BoneRemap;
    /* 0x0020 */ class RwMatrixTag *Mat;
    /* 0x0024 */ class _xVec3 Scale;
};

// from xScrFx.cpp
// size: 0xC
class xModelPool {
    /* 0x0000 */ class xModelPool *Next;
    /* 0x0004 */ unsigned int NumMatrices;
    /* 0x0008 */ class xModelInstance *List;
};

// from xMorph.cpp
// size: 0x30
class xMorphFrame {
    /* 0x0000 */ class RpAtomic *Model;
    /* 0x0004 */ float RecipTime;
    /* 0x0008 */ float Scale;
    /* 0x000C */ unsigned short Flags;
    /* 0x000E */ unsigned short NumVerts;
    /* 0x0010 */ signed short *Targets[4];
    /* 0x0020 */ signed short WeightStart[4];
    /* 0x0028 */ signed short WeightEnd[4];
};

// from xMorph.cpp
// size: 0x10
class xMorphSeqFile {
    /* 0x0000 */ unsigned int Magic;
    /* 0x0004 */ unsigned int Flags;
    /* 0x0008 */ unsigned int TimeCount;
    /* 0x000C */ unsigned int ModelCount;
};

// from xMorph.cpp
// size: 0x20
class xMorphTargetFile {
    /* 0x0000 */ unsigned int Magic;
    /* 0x0004 */ unsigned short NumTargets;
    /* 0x0006 */ unsigned short NumVerts;
    /* 0x0008 */ unsigned int Flags;
    /* 0x000C */ float Scale;
    /* 0x0010 */ class _xVec3 Center;
    /* 0x001C */ float Radius;
};

// from zScene.cpp
// size: 0x50
class xPar {
    /* 0x0000 */ class xPar *m_next;
    /* 0x0004 */ class xPar *m_prev;
    /* 0x0008 */ float m_lifetime;
    /* 0x000C */ unsigned char m_c[4];
    /* 0x0010 */ class _xVec3 m_pos;
    /* 0x001C */ float m_size;
    /* 0x0020 */ class _xVec3 m_vel;
    /* 0x002C */ float m_sizeVel;
    /* 0x0030 */ unsigned char m_flag;
    /* 0x0031 */ unsigned char m_mode;
    /* 0x0032 */ unsigned char m_texIdx[2];
    /* 0x0034 */ unsigned char m_rotdeg[3];
    /* 0x0037 */ unsigned char pad8;
    /* 0x0038 */ unsigned int pad32;
    /* 0x003C */ class zParEmitterAsset *m_asset;
    /* 0x0040 */ float m_cvel[4];
};

// from zScene.cpp
// size: 0x4
class xParCmd {
    /* 0x0000 */ unsigned int flag;
};

// from zParCmd.cpp
// size: 0x14
class xParCmdAccelerate : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 acc;
};

// from zParCmd.cpp
// size: 0x18
class xParCmdAlpha3rdPolyReg : public xParCmdAsset {
    /* 0x0008 */ float constant;
    /* 0x000C */ float a;
    /* 0x0010 */ float a2;
    /* 0x0014 */ float a3;
};

// from zParCmd.cpp
// size: 0x14
class xParCmdApplyCamMat : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 apply;
};

// from zParCmd.cpp
// size: 0xC
class xParCmdApplyWind : public xParCmdAsset {
    /* 0x0008 */ float strength;
};

// from zScene.cpp
// size: 0x8
class xParCmdAsset {
    /* 0x0000 */ unsigned int type;
    /* 0x0004 */ unsigned char enabled;
    /* 0x0005 */ unsigned char mode;
    /* 0x0006 */ unsigned char pad[2];
};

// from zParCmd.cpp
// size: 0x10
class xParCmdFollow : public xParCmdAsset {
    /* 0x0008 */ float gravity;
    /* 0x000C */ float epsilon;
};

// from zParCmd.cpp
// size: 0x2C
class xParCmdJet : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 center;
    /* 0x0014 */ class _xVec3 acc;
    /* 0x0020 */ float gravity;
    /* 0x0024 */ float epsilon;
    /* 0x0028 */ float radiusSqr;
};

// from zParCmd.cpp
// size: 0x10
class xParCmdKillDistance : public xParCmdAsset {
    /* 0x0008 */ float dSqr;
    /* 0x000C */ unsigned int kill_greater_than;
};

// from zParCmd.cpp
// size: 0x10
class xParCmdKillSlow : public xParCmdAsset {
    /* 0x0008 */ float speedLimitSqr;
    /* 0x000C */ unsigned int kill_less_than;
};

// from zParCmd.cpp
// size: 0x14
class xParCmdMove : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 dir;
};

// from zParCmd.cpp
// size: 0x14
class xParCmdMoveRandomPar : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 dim;
};

// from zParCmd.cpp
// size: 0x2C
class xParCmdOrbitLine : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 p;
    /* 0x0014 */ class _xVec3 axis;
    /* 0x0020 */ float gravity;
    /* 0x0024 */ float epsilon;
    /* 0x0028 */ float maxRadiusSqr;
};

// from zParCmd.cpp
// size: 0x20
class xParCmdOrbitPoint : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 center;
    /* 0x0014 */ float gravity;
    /* 0x0018 */ float epsilon;
    /* 0x001C */ float maxRadiusSqr;
};

// from zParCmd.cpp
// size: 0x10
class xParCmdPlayerCollision : public xParCmdAsset {
    /* 0x0008 */ float min;
    /* 0x000C */ float max;
};

// from zParCmd.cpp
// size: 0x14
class xParCmdRandomVelocityPar : public xParCmdAsset {
    /* 0x0008 */ float x;
    /* 0x000C */ float y;
    /* 0x0010 */ float z;
};

// from zParCmd.cpp
// size: 0x20
class xParCmdRotPar : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 min;
    /* 0x0014 */ class _xVec3 max;
};

// from zParCmd.cpp
// size: 0x20
class xParCmdRotateAround : public xParCmdAsset {
    /* 0x0008 */ class _xVec3 pos;
    /* 0x0014 */ float unused1;
    /* 0x0018 */ float radius_growth;
    /* 0x001C */ float yaw;
};

// from zParCmd.cpp
// size: 0x18
class xParCmdScale3rdPolyReg : public xParCmdAsset {
    /* 0x0008 */ float constant;
    /* 0x000C */ float a;
    /* 0x0010 */ float a2;
    /* 0x0014 */ float a3;
};

// from zScene.cpp
// size: 0x24
class xParCmdTex : public xParCmdAsset {
    /* 0x0008 */ float x1;
    /* 0x000C */ float y1;
    /* 0x0010 */ float x2;
    /* 0x0014 */ float y2;
    /* 0x0018 */ unsigned char birthMode;
    /* 0x0019 */ unsigned char rows;
    /* 0x001A */ unsigned char cols;
    /* 0x001B */ unsigned char unit_count;
    /* 0x001C */ float unit_width;
    /* 0x0020 */ float unit_height;
};

// from zParCmd.cpp
// size: 0x18
class xParCmdTexAnim : public xParCmdAsset {
    /* 0x0008 */ unsigned char anim_mode;
    /* 0x0009 */ unsigned char anim_wrap_mode;
    /* 0x000A */ unsigned char pad_anim;
    /* 0x000B */ unsigned char throttle_spd_less_than;
    /* 0x000C */ float throttle_spd_sqr;
    /* 0x0010 */ float throttle_time;
    /* 0x0014 */ float throttle_time_elapsed;
};

// from zScene.cpp
// size: 0x24
class xParGroup {
    /* 0x0000 */ class xPar *m_root;
    /* 0x0004 */ class xPar *m_dead;
    /* 0x0008 */ signed int m_num_of_particles;
    /* 0x000C */ unsigned char m_alive;
    /* 0x000D */ unsigned char m_killWhenDead;
    /* 0x000E */ unsigned char m_active;
    /* 0x000F */ unsigned char m_visible;
    /* 0x0010 */ unsigned char m_culled;
    /* 0x0011 */ unsigned char m_priority;
    /* 0x0012 */ unsigned char m_flags;
    /* 0x0013 */ unsigned char m_regidx;
    /* 0x0014 */ class xParGroup *m_next;
    /* 0x0018 */ class xParGroup *m_prev;
    /* 0x001C */ void (*draw)(void *, class xParGroup *);
    /* 0x0020 */ class xParCmdTex *m_cmdTex;
};

// from xScrFx.cpp
// size: 0xC
class xQCData {
    /* 0x0000 */ unsigned int xmask;
    /* 0x0004 */ unsigned int ymask;
    /* 0x0008 */ unsigned int zmask;
};

// from xDebug.cpp
// size: 0x10
class xSB {
    /* 0x0000 */ char *buf;
    /* 0x0004 */ unsigned int max;
    /* 0x0008 */ char *cur;
    /* 0x000C */ char *disp;
};

// from xEnt.cpp
// size: 0x1C
class xSerial {
    /* 0x0000 */ unsigned int idtag;
    /* 0x0004 */ signed int baseoff;
    /* 0x0008 */ class st_SERIAL_CLIENTINFO *ctxtdata;
    /* 0x000C */ signed int warned;
    /* 0x0010 */ signed int curele;
    /* 0x0014 */ signed int bitidx;
    /* 0x0018 */ signed int bittally;
};

// from iSnd.cpp
// size: 0xD30
class xSndGlobals {
    /* 0x0000 */ unsigned int stereo;
    /* 0x0004 */ unsigned int SndCount;
    /* 0x0008 */ class xSndVol mastervol;
    /* 0x0010 */ class xSndVol streamvol;
    /* 0x0018 */ class xSndVol fxvol;
    /* 0x0020 */ class xSndVoiceInfo voice[48];
    /* 0x0CE0 */ class _xVec3 right;
    /* 0x0CEC */ class _xVec3 up;
    /* 0x0CF8 */ class _xVec3 at;
    /* 0x0D04 */ class _xVec3 pos;
    /* 0x0D10 */ float radius2;
    /* 0x0D14 */ class _xVec3 nearpos;
    /* 0x0D20 */ class _xVec3 farpos;
    /* 0x0D2C */ unsigned int suspendCD;
};

// from iSnd.cpp
// size: 0x44
class xSndVoiceInfo {
    /* 0x0000 */ unsigned int assetID;
    /* 0x0004 */ unsigned int sndID;
    /* 0x0008 */ unsigned int parentID;
    /* 0x000C */ class _xVec3 *parentPos;
    /* 0x0010 */ signed int internalID;
    /* 0x0014 */ unsigned short flags;
    /* 0x0016 */ unsigned short priority;
    /* 0x0018 */ float vol;
    /* 0x001C */ float pitch;
    /* 0x0020 */ unsigned int sample_rate;
    /* 0x0024 */ unsigned int deadct;
    /* 0x0028 */ class _xVec3 pos;
    /* 0x0034 */ float radius2;
    /* 0x0038 */ class iSndInfo ps;
};

// from iSnd.cpp
// size: 0x8
class xSndVol {
    /* 0x0000 */ float volL;
    /* 0x0004 */ float volR;
};

// from zScene.cpp
// size: 0x2C
class xSpline3 {
    /* 0x0000 */ unsigned short type;
    /* 0x0002 */ unsigned short flags;
    /* 0x0004 */ unsigned int N;
    /* 0x0008 */ unsigned int allocN;
    /* 0x000C */ class _xVec3 *points;
    /* 0x0010 */ float *time;
    /* 0x0014 */ class _xVec3 *p12;
    /* 0x0018 */ class _xVec3 *bctrl;
    /* 0x001C */ float *knot;
    /* 0x0020 */ class xCoef3 *coef;
    /* 0x0024 */ unsigned int arcSample;
    /* 0x0028 */ float *arcLength;
};

// from xScrFx.cpp
// size: 0x28
class xSurface : public xBase {
    /* 0x0010 */ unsigned int idx;
    /* 0x0014 */ unsigned int type;
    /* 0x0018 */ unsigned int mat_idx;
    /* 0x0018 */ class _xEnt *ent;
    /* 0x0018 */ void *obj;
    /* 0x001C */ float friction;
    /* 0x0020 */ unsigned char state;
    /* 0x0021 */ unsigned char pad[3];
    /* 0x0024 */ void *moprops;
};

// from zEntVillain.cpp
// size: 0x18
class zAnimFxSound {
    /* 0x0000 */ unsigned int ID;
    /* 0x0004 */ float vol;
    /* 0x0008 */ float pitch;
    /* 0x000C */ unsigned int priority;
    /* 0x0010 */ unsigned int flags;
    /* 0x0014 */ float radius;
};

// from zAnimList.cpp
// size: 0x28
class zAnimListAsset {
    /* 0x0000 */ unsigned int ids[10];
};

// from zAssetTypes.cpp
// size: 0x18
class zAssetAnimEffect {
    /* 0x0000 */ unsigned int StateID;
    /* 0x0004 */ float StartTime;
    /* 0x0008 */ float EndTime;
    /* 0x000C */ unsigned int Flags;
    /* 0x0010 */ unsigned int EffectType;
    /* 0x0014 */ unsigned int UserDataSize;
};

// from zAssetTypes.cpp
// size: 0x20
class zAssetAnimFile {
    /* 0x0000 */ unsigned int FileFlags;
    /* 0x0004 */ float Duration;
    /* 0x0008 */ float TimeOffset;
    /* 0x000C */ unsigned short NumAnims[2];
    /* 0x0010 */ void **RawData;
    /* 0x0014 */ signed int Physics;
    /* 0x0018 */ signed int StartPose;
    /* 0x001C */ signed int EndPose;
};

// from zAssetTypes.cpp
// size: 0x14
class zAssetAnimState {
    /* 0x0000 */ unsigned int StateID;
    /* 0x0004 */ unsigned int FileIndex;
    /* 0x0008 */ unsigned int EffectCount;
    /* 0x000C */ unsigned int EffectOffset;
    /* 0x0010 */ float Speed;
};

// from zAssetTypes.cpp
// size: 0x14
class zAssetAnimTable {
    /* 0x0000 */ unsigned int Magic;
    /* 0x0004 */ unsigned int NumRaw;
    /* 0x0008 */ unsigned int NumFiles;
    /* 0x000C */ unsigned int NumStates;
    /* 0x0010 */ unsigned int ConstructFunc;
};

// from zEnt.cpp
// size: 0xC
class zAssetModelInfo {
    /* 0x0000 */ unsigned int Magic;
    /* 0x0004 */ unsigned int NumModelInst;
    /* 0x0008 */ unsigned int AnimTableID;
};

// from zEnt.cpp
// size: 0x8
class zAssetModelParam {
    /* 0x0000 */ unsigned int HashID;
    /* 0x0004 */ unsigned char WordLength;
    /* 0x0005 */ unsigned char String[3];
};

// from zScene.cpp
// size: 0x14
class zAssetPickup {
    /* 0x0000 */ unsigned int pickupHash;
    /* 0x0004 */ unsigned char pickupType;
    /* 0x0005 */ unsigned char pickupIndex;
    /* 0x0006 */ unsigned short pickupFlags;
    /* 0x0008 */ unsigned int quantity;
    /* 0x000C */ unsigned int modelID;
    /* 0x0010 */ unsigned int animID;
};

// from xScrFx.cpp
// size: 0x8
class zAssetPickupTable {
    /* 0x0000 */ unsigned int Magic;
    /* 0x0004 */ unsigned int Count;
};

// from xScrFx.cpp
// size: 0x14
class zCheckPoint {
    /* 0x0000 */ class _xVec3 pos;
    /* 0x000C */ float rot;
    /* 0x0010 */ unsigned int initCamID;
};

// from zParCmd.cpp
// size: 0xC
class zCmdInfo {
    /* 0x0000 */ unsigned int type;
    /* 0x0004 */ unsigned int size;
    /* 0x0008 */ void (*func)(class zParCmd *, class xParGroup *, float);
};

// from zConditional.cpp
// size: 0x14
class zCondAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned int constNum;
    /* 0x000C */ unsigned int expr1;
    /* 0x0010 */ unsigned int op;
};

// from zEntPickup.cpp
// size: 0xC
class zCounterAsset : public p2BaseAsset {
    /* 0x0008 */ signed short count;
};

// from zCutsceneMgr.cpp
// size: 0x48
class zCutSceneNames {
    /* 0x0000 */ char name[64];
    /* 0x0040 */ signed int played;
    /* 0x0044 */ signed int skipOK;
};

// from zCutsceneMgr.cpp
// size: 0x8
class zCutsceneBoundHack {
    /* 0x0000 */ char *name;
    /* 0x0004 */ float radius;
};

// from xScrFx.cpp
// size: 0xC8
class zCutsceneMgrAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned int cutsceneAssetID;
    /* 0x000C */ unsigned int flags;
    /* 0x0010 */ float interpSpeed;
    /* 0x0014 */ float startTime[15];
    /* 0x0050 */ float endTime[15];
    /* 0x008C */ unsigned int emitID[15];
};

// from xScrFx.cpp
// size: 0x10
class zCutsceneZbuffer {
    /* 0x0000 */ float start;
    /* 0x0004 */ float end;
    /* 0x0008 */ float near;
    /* 0x000C */ float far;
};

// from xScrFx.cpp
// size: 0x44
class zCutsceneZbufferHack {
    /* 0x0000 */ char *name;
    /* 0x0004 */ class zCutsceneZbuffer times[4];
};

// from zEGenerator.cpp
// size: 0x68
class zEGenAsset : public p2EntAsset {
    /* 0x0050 */ class _xVec3 src_dpos;
    /* 0x005C */ unsigned char damage_type;
    /* 0x005D */ unsigned char flags;
    /* 0x0060 */ float ontime;
    /* 0x0064 */ unsigned int onAnimID;
};

// from zEntPlayer.cpp
// size: 0x11C
class zEGenerator : public _zEnt {
    /* 0x00EC */ unsigned short flags;
    /* 0x00EE */ unsigned short num_dsts;
    /* 0x00F0 */ float tmr;
    /* 0x00F4 */ class xAnimFile *afile;
    /* 0x00F8 */ class _xVec3 src_pos;
    /* 0x0104 */ class _xVec3 dst_pos;
    /* 0x0110 */ class xBase *dst;
    /* 0x0114 */ class _tagLightning *lfx[2];
};

// from zEntButton.cpp
// size: 0x14
class zEntButtonAsset {
    /* 0x0000 */ unsigned int actMethod;
    /* 0x0004 */ signed int initButtonState;
    /* 0x0008 */ signed int isReset;
    /* 0x000C */ signed int isSpringBB;
    /* 0x0010 */ unsigned char buttonFlags;
};

// from zEntDestructObj.cpp
// size: 0x20
class zEntDestructObjAsset {
    /* 0x0000 */ float animSpeed;
    /* 0x0004 */ unsigned int initAnimState;
    /* 0x0008 */ unsigned int health;
    /* 0x000C */ unsigned int spawnItemID;
    /* 0x0010 */ unsigned int dflags;
    /* 0x0014 */ unsigned char collType;
    /* 0x0015 */ unsigned char fxType;
    /* 0x0016 */ unsigned char pad[2];
    /* 0x0018 */ float blast_radius;
    /* 0x001C */ float blast_strength;
};

// from zEntPickup.cpp
// size: 0xC
class zEntPickupWarpPoint {
    /* 0x0000 */ signed int id;
    /* 0x0004 */ unsigned int status;
    /* 0x0008 */ unsigned int sceneID;
};

// from zEnt.cpp
// size: 0x330
class zEntProjectile : public _xEnt {
    /* 0x00E8 */ signed int flg_prop;
    /* 0x00EC */ signed int flg_categ;
    /* 0x00F0 */ unsigned char flg_chk;
    /* 0x00F1 */ unsigned char flg_pen;
    /* 0x00F2 */ unsigned char pad[2];
    /* 0x00F4 */ void *owner;
    /* 0x00F8 */ void *ownerContext;
    /* 0x00FC */ signed int (*ownerNotify)(class zEntProjectile *, enum en_PROJECTILE_EVENT, void *);
    /* 0x0100 */ signed int (*ownerDoCollide)(class zEntProjectile *, class zScene *, float);
    /* 0x0104 */ float tmr_alive;
    /* 0x0108 */ float tmr_curstate;
    /* 0x010C */ class _xVec3 pos_home;
    /* 0x0118 */ float tym_maxlife;
    /* 0x011C */ float ds2_maxlife;
    /* 0x0120 */ class st_ZPRJ_COMMON common;
    /* 0x015C */ class st_ZPRJ_CATDATA cat_data;
    /* 0x0180 */ void *xtradata;
    /* 0x0184 */ class _xCollis *plyr_colrec;
    /* 0x0188 */ class st_ZPRJ_LINKNODE linknode;
    /* 0x0194 */ enum en_ZPRJ_STATES state;
    /* 0x0198 */ signed int flg_stat;
    /* 0x019C */ float pct_docol;
    /* 0x01A0 */ float tmr_coldelay;
    /* 0x01A4 */ enum en_ZPRJ_CATEGORY cat_proj;
    /* 0x01A8 */ class st_ZPRJ_TYPERECORD *ptype;
    /* 0x01AC */ float tmr_emit;
    /* 0x01B0 */ class _xEntFrame raw_frame;
    /* 0x02A0 */ class p2EntAsset raw_asset;
    /* 0x02F0 */ class _xEntShadow raw_shadow;
    /* 0x0308 */ class zGustData raw_gust;
};

// from zEntTrigger.cpp
// size: 0xEC
class zEntTrigger : public _zEnt {
};

// from zFMV.cpp
// size: 0x28
class zFMVFile {
    /* 0x0000 */ enum eFMVFile type;
    /* 0x0004 */ char fileName[32];
    /* 0x0024 */ unsigned int fileNameID;
};

// from zFog.cpp
// size: 0x24
class zFogAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned char bkgndColor[4];
    /* 0x000C */ unsigned char fogColor[4];
    /* 0x0010 */ float fogDensity;
    /* 0x0014 */ float fogStart;
    /* 0x0018 */ float fogStop;
    /* 0x001C */ float transitionTime;
    /* 0x0020 */ unsigned char fogType;
    /* 0x0021 */ unsigned char padFog[3];
};

// from xScrFx.cpp
// size: 0xD0
class zGlobalGameStats {
    /* 0x0000 */ float gameTime;
    /* 0x0004 */ unsigned int visited;
    /* 0x0008 */ float levelTime;
    /* 0x000C */ float levelTime_sum;
    /* 0x0010 */ float areaTime;
    /* 0x0014 */ float areaTime_sum;
    /* 0x0018 */ float roomTime;
    /* 0x001C */ float roomTime_sum;
    /* 0x0020 */ float timeRunning;
    /* 0x0024 */ float timeRunning_sum;
    /* 0x0028 */ float timeWalking;
    /* 0x002C */ float timeWalking_sum;
    /* 0x0030 */ unsigned int scares;
    /* 0x0034 */ unsigned int scares_sum;
    /* 0x0038 */ unsigned int deaths;
    /* 0x003C */ unsigned int deaths_sum;
    /* 0x0040 */ unsigned int singleJumps;
    /* 0x0044 */ unsigned int singleJumps_sum;
    /* 0x0048 */ unsigned int doubleJumps;
    /* 0x004C */ unsigned int doubleJumps_sum;
    /* 0x0050 */ unsigned int gumShot;
    /* 0x0054 */ unsigned int gumShot_sum;
    /* 0x0058 */ unsigned int gumHit;
    /* 0x005C */ unsigned int gumHit_sum;
    /* 0x0060 */ unsigned int gumEmpty;
    /* 0x0064 */ unsigned int gumEmpty_sum;
    /* 0x0068 */ unsigned int soapShot;
    /* 0x006C */ unsigned int soapShot_sum;
    /* 0x0070 */ unsigned int soapHit;
    /* 0x0074 */ unsigned int soapHit_sum;
    /* 0x0078 */ unsigned int soapEmpty;
    /* 0x007C */ unsigned int soapEmpty_sum;
    /* 0x0080 */ unsigned int food;
    /* 0x0084 */ unsigned int food_sum;
    /* 0x0088 */ unsigned int snackBoxes;
    /* 0x008C */ unsigned int snackBoxes_sum;
    /* 0x0090 */ unsigned int enemyKills;
    /* 0x0094 */ unsigned int enemyKills_sum;
    /* 0x0098 */ unsigned int enemyCount;
    /* 0x009C */ unsigned int enemyCount_sum;
    /* 0x00A0 */ unsigned int specials;
    /* 0x00A4 */ unsigned int specials_tot;
    /* 0x00A8 */ unsigned int tokens;
    /* 0x00AC */ unsigned int tokens_tot;
    /* 0x00B0 */ unsigned int snacks;
    /* 0x00B4 */ unsigned int snacks_tot;
    /* 0x00B8 */ unsigned int chances;
    /* 0x00BC */ unsigned int chances_tot;
    /* 0x00C0 */ unsigned int keys;
    /* 0x00C4 */ unsigned int keys_tot;
    /* 0x00C8 */ unsigned int clues;
    /* 0x00CC */ unsigned int clues_tot;
};

// from xScrFx.cpp
// size: 0x3330
class zGlobals {
    /* 0x0000 */ class _tagxCamFrame camera;
    /* 0x0220 */ class _tagxPad *pad0;
    /* 0x0224 */ class _tagxPad *pad1;
    /* 0x0228 */ class zPlayerGlobals player;
    /* 0x0888 */ class zAssetPickupTable *pickupTable;
    /* 0x088C */ signed int profile;
    /* 0x0890 */ class _zCutsceneMgr *cmgr;
    /* 0x0894 */ char profFunc[6][128];
    /* 0x0B94 */ class zScene *sceneCur;
    /* 0x0B98 */ class zScene *scenePreload;
    /* 0x0B9C */ signed int sceneFirst;
    /* 0x0BA0 */ char sceneStart[32];
    /* 0x0BC0 */ class iFogParams fog;
    /* 0x0BDC */ class iFogParams fogA;
    /* 0x0BF8 */ class iFogParams fogB;
    /* 0x0C18 */ signed long fog_t0;
    /* 0x0C20 */ signed long fog_t1;
    /* 0x0C28 */ class zGlobalGameStats stats;
    /* 0x0CF8 */ class zGlobalGameStats areaStats[30];
    /* 0x2558 */ class zGlobalGameStats levelStats[13];
    /* 0x2FE8 */ class zGlobalGameStats worldStats[4];
    /* 0x3328 */ signed int option_vibration;
    /* 0x332C */ float update_dt;
};

// from xEnt.cpp
// size: 0xC
class zGroupAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned short itemCount;
    /* 0x000A */ unsigned short groupFlags;
};

// from zGust.cpp
// size: 0x28
class zGustAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned int flags;
    /* 0x000C */ unsigned int volumeID;
    /* 0x0010 */ unsigned int effectID;
    /* 0x0014 */ class _xVec3 vel;
    /* 0x0020 */ float fade;
    /* 0x0024 */ float partMod;
};

// from zEnt.cpp
// size: 0x24
class zGustData {
    /* 0x0000 */ unsigned int gust_on;
    /* 0x0004 */ class _zGust *g[4];
    /* 0x0014 */ float lerp[4];
};

// from zHud.cpp
// size: 0x10
class zHudColor {
    /* 0x0000 */ float rm;
    /* 0x0004 */ float gm;
    /* 0x0008 */ float bm;
    /* 0x000C */ float a;
};

// from zHud.cpp
// size: 0x40
class zHudFrame {
    /* 0x0000 */ class _xVec3 pos;
    /* 0x000C */ class _xVec3 size;
    /* 0x0018 */ class _xVec3 ang;
    /* 0x0024 */ class _xVec3 dang;
    /* 0x0030 */ class zHudColor color;
};

// from zHud.cpp
// size: 0x234
class zHudItem {
    /* 0x0000 */ class xModelInstance *model;
    /* 0x0004 */ class xAnimTable *atbl;
    /* 0x0008 */ class zHudFrame hframe;
    /* 0x0048 */ enum eHudStatusPos statusPos;
    /* 0x004C */ enum eHudStatusSize statusSize;
    /* 0x0050 */ enum eHudStatusAng statusAng;
    /* 0x0054 */ enum eHudStatusColor statusColor;
    /* 0x0058 */ char text[64];
    /* 0x0098 */ signed int statusFX;
    /* 0x009C */ float shake;
    /* 0x00A0 */ class zHudStatusPos zHudStatusPosTable[10];
    /* 0x00F0 */ class zHudStatusAng zHudStatusAngTable[11];
    /* 0x0174 */ class zHudStatusSize zHudStatusSizeTable[9];
    /* 0x01BC */ class zHudStatusColor zHudStatusColorTable[6];
};

// from zHud.cpp
// size: 0xC
class zHudModel {
    /* 0x0000 */ enum eHudModel type;
    /* 0x0004 */ char *modelName;
    /* 0x0008 */ unsigned int modelNameID;
};

// from zHud.cpp
// size: 0xC
class zHudStatusAng {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;
};

// from zHud.cpp
// size: 0x14
class zHudStatusColor {
    /* 0x0000 */ float rm;
    /* 0x0004 */ float gm;
    /* 0x0008 */ float bm;
    /* 0x000C */ float a;
    /* 0x0010 */ enum eHudInterpType interp;
};

// from zHud.cpp
// size: 0x8
class zHudStatusPos {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
};

// from zHud.cpp
// size: 0x8
class zHudStatusSize {
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
};

// from xScrFx.cpp
// size: 0x10
class zJumpParam {
    /* 0x0000 */ float PeakHeight;
    /* 0x0004 */ float TimeGravChange;
    /* 0x0008 */ float TimeHold;
    /* 0x000C */ float ImpulseVel;
};

// from zLaugh.cpp
// size: 0x14
class zLaughSituation {
    /* 0x0000 */ unsigned int countMax;
    /* 0x0004 */ float delay;
    /* 0x0008 */ float punchDelay;
    /* 0x000C */ float elapsedTime;
    /* 0x0010 */ unsigned int count;
};

// from zLight.cpp
// size: 0x44
class zLightAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned char lightType;
    /* 0x0009 */ unsigned char lightEffect;
    /* 0x000A */ unsigned char lightPad[2];
    /* 0x000C */ unsigned int lightFlags;
    /* 0x0010 */ float lightColor[4];
    /* 0x0020 */ class _xVec3 lightDir;
    /* 0x002C */ float lightConeAngle;
    /* 0x0030 */ class _xSphere lightSphere;
    /* 0x0040 */ unsigned int attachID;
};

// from zLobMaster.cpp
// size: 0x80
class zLobMasterAsset : public p2BaseAsset {
    /* 0x0008 */ signed int lobMasterType;
    /* 0x000C */ unsigned int projectileTypeID;
    /* 0x0010 */ class _xVec3 pos_launch;
    /* 0x001C */ class _xVec3 rot_launch;
    /* 0x0028 */ float spd_launch;
    /* 0x002C */ float spd_randPct;
    /* 0x0030 */ class _xVec3 scl_model;
    /* 0x003C */ signed int flg_enablers;
    /* 0x0040 */ float tym_maxlife;
    /* 0x0044 */ float dst_maxdist;
    /* 0x0048 */ unsigned int aid_mvptID;
    /* 0x004C */ signed int cnt_salvo;
    /* 0x0050 */ signed int cnt_ammo;
    /* 0x0054 */ float fac_arccoeff;
    /* 0x0058 */ signed int ang_debrisCone;
    /* 0x005C */ signed int numBounce;
    /* 0x0060 */ signed int typ_powerup;
    /* 0x0064 */ float heavyFactor;
    /* 0x0068 */ class _xVec3 tumbleRotation;
    /* 0x0074 */ float collideDelay;
    /* 0x0078 */ float atRestPeriod;
    /* 0x007C */ unsigned int mode;
};

// from zSurface.cpp
// size: 0x8
class zMaterialMapAsset {
    /* 0x0000 */ unsigned int id;
    /* 0x0004 */ unsigned int count;
};

// from zSurface.cpp
// size: 0x8
class zMaterialMapEntry {
    /* 0x0000 */ unsigned int surfaceAssetID;
    /* 0x0004 */ unsigned int materialIndex;
};

// from zMusic.cpp
// size: 0x2C
class zMusicSituation {
    /* 0x0000 */ signed int track;
    /* 0x0004 */ unsigned int priority;
    /* 0x0008 */ unsigned int countMax;
    /* 0x000C */ float delay;
    /* 0x0010 */ float punchDelay;
    /* 0x0014 */ float elapsedTime;
    /* 0x0018 */ unsigned int count;
    /* 0x001C */ unsigned int sndid;
    /* 0x0020 */ signed int situation;
    /* 0x0024 */ signed int music_enum;
    /* 0x0028 */ signed int game_state;
};

// from zMusic.cpp
// size: 0x14
class zMusicTrackInfo {
    /* 0x0000 */ unsigned int snd_id;
    /* 0x0004 */ unsigned int loop;
    /* 0x0008 */ class zMusicSituation *situation;
    /* 0x000C */ unsigned int assetID;
    /* 0x0010 */ float lastVol;
};

// from zScene.cpp
// size: 0x8
class zParCmd : public xParCmd {
    /* 0x0004 */ class xParCmdAsset *tasset;
};

// from zScene.cpp
// size: 0x78
class zParEmitterAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned char emit_flags;
    /* 0x0009 */ unsigned char emit_type;
    /* 0x000A */ unsigned char count;
    /* 0x000B */ unsigned char count_variation;
    /* 0x000C */ float interval;
    /* 0x0010 */ class _tagEmitCircle e_circle;
    /* 0x0010 */ class _tagEmitSphere e_sphere;
    /* 0x0010 */ class _tagEmitRect e_rect;
    /* 0x0010 */ class _tagEmitLine e_line;
    /* 0x0010 */ class _tagEmitVolume e_volume;
    /* 0x0010 */ class _tagEmitOffsetPoint e_offsetp;
    /* 0x002C */ unsigned int attachToID;
    /* 0x0030 */ unsigned int parSysID;
    /* 0x0034 */ class _xVec3 pos;
    /* 0x0040 */ class _xVec3 vel;
    /* 0x004C */ float vel_angle_variation;
    /* 0x0050 */ unsigned char color_birth[4];
    /* 0x0054 */ unsigned char color_death[4];
    /* 0x0058 */ float size_birth;
    /* 0x005C */ float size_birth_variation;
    /* 0x0060 */ float size_death;
    /* 0x0064 */ float life;
    /* 0x0068 */ float life_variation;
    /* 0x006C */ unsigned char pad_emit[2];
    /* 0x006E */ unsigned char cull_mode;
    /* 0x0070 */ float cull_dist_sqr;
    /* 0x0074 */ unsigned char max_emit;
};

// from zScene.cpp
// size: 0x28
class zParSys : public xBase {
    /* 0x0010 */ class zParSysAsset *tasset;
    /* 0x0014 */ unsigned int cmdCount;
    /* 0x0018 */ class zParCmd *cmd;
    /* 0x001C */ class zParSys *parent;
    /* 0x0020 */ class _zParGroup *group;
    /* 0x0024 */ unsigned char visible;
};

// from zScene.cpp
// size: 0x20
class zParSysAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned int type;
    /* 0x000C */ unsigned int parentParSysID;
    /* 0x0010 */ unsigned int textureID;
    /* 0x0014 */ unsigned char parFlags;
    /* 0x0015 */ unsigned char priority;
    /* 0x0016 */ unsigned short maxPar;
    /* 0x0018 */ unsigned char renderFunc;
    /* 0x0019 */ unsigned char renderSrcBlendMode;
    /* 0x001A */ unsigned char renderDstBlendMode;
    /* 0x001B */ unsigned char cmdCount;
    /* 0x001C */ unsigned int cmdSize;
};

// from zParSys.cpp
// size: 0x8
class zParSysInfo {
    /* 0x0000 */ signed int type;
    /* 0x0004 */ void (*func)(void *, class xParGroup *);
};

// from xScrFx.cpp
// size: 0x660
class zPlayerGlobals {
    /* 0x0000 */ class _zEnt ent;
    /* 0x00EC */ unsigned int Visible;
    /* 0x00F0 */ unsigned int Health;
    /* 0x00F4 */ signed int Speed;
    /* 0x00F8 */ signed int Sneak;
    /* 0x00FC */ signed int Teeter;
    /* 0x0100 */ signed int CanSlip;
    /* 0x0104 */ float SlipFadeTimer;
    /* 0x0108 */ signed int Slide;
    /* 0x010C */ float SlideTimer;
    /* 0x0110 */ signed int Stepping;
    /* 0x0114 */ signed int JumpState;
    /* 0x0118 */ signed int LastJumpState;
    /* 0x011C */ float JumpTimer;
    /* 0x0120 */ float LookAroundTimer;
    /* 0x0124 */ unsigned int LookAroundRand;
    /* 0x0128 */ unsigned int LastProjectile;
    /* 0x012C */ float DecelRun;
    /* 0x0130 */ float DecelRunSpeed;
    /* 0x0134 */ float HotsauceTimer;
    /* 0x0138 */ float LeanLerp;
    /* 0x013C */ unsigned int HeadbuttHit;
    /* 0x0140 */ float ScareTimer;
    /* 0x0144 */ class xBase *ScareSource;
    /* 0x0148 */ unsigned int ScareShaggy;
    /* 0x014C */ float CowerTimer;
    /* 0x0150 */ float DamageTimer;
    /* 0x0154 */ float ControlOffTimer;
    /* 0x0158 */ float HelmetTimer;
    /* 0x015C */ unsigned int WorldDisguise;
    /* 0x0160 */ unsigned int Bounced;
    /* 0x0164 */ float FallDeathTimer;
    /* 0x0168 */ float HeadbuttVel;
    /* 0x016C */ float HeadbuttTimer;
    /* 0x0170 */ unsigned int SpecialReceived;
    /* 0x0174 */ class _xEnt *MountChimney;
    /* 0x0178 */ float MountChimOldY;
    /* 0x017C */ float ShockRadius;
    /* 0x0180 */ float ShockRadiusOld;
    /* 0x0184 */ float Face_ScareTimer;
    /* 0x0188 */ unsigned int Face_ScareRandom;
    /* 0x018C */ unsigned int Face_Event;
    /* 0x0190 */ float Face_EventTimer;
    /* 0x0194 */ float Face_PantTimer;
    /* 0x0198 */ unsigned int Face_AnimSpecific;
    /* 0x019C */ unsigned int IdleRand;
    /* 0x01A0 */ float IdleMinorTimer;
    /* 0x01A4 */ float IdleMajorTimer;
    /* 0x01A8 */ float IdleSitTimer;
    /* 0x01AC */ signed int Transparent;
    /* 0x01B0 */ class _zEnt *FireTarget;
    /* 0x01B4 */ class _zEnt *DamageSource;
    /* 0x01B8 */ unsigned int ControlOn;
    /* 0x01BC */ unsigned int ControlOnEvent;
    /* 0x01C0 */ unsigned int AutoMoveSpeed;
    /* 0x01C4 */ float AutoMoveDist;
    /* 0x01C8 */ class _xVec3 AutoMoveTarget;
    /* 0x01D4 */ class xBase *AutoMoveObject;
    /* 0x01D8 */ class _zEnt *Diggable;
    /* 0x01DC */ float DigTimer;
    /* 0x01E0 */ class zShaggy1Globals Shaggy1;
    /* 0x02C8 */ class _zEnt *Shaggy8Call;
    /* 0x02CC */ float Shaggy8CallTimer;
    /* 0x02D0 */ class _zEntHangable *HangFound;
    /* 0x02D4 */ class _zEntHangable *HangEnt;
    /* 0x02D8 */ class _zEntHangable *HangEntLast;
    /* 0x02DC */ class _xVec3 HangPivot;
    /* 0x02E8 */ class _xVec3 HangVel;
    /* 0x02F4 */ float HangLength;
    /* 0x02F8 */ class _xVec3 HangStartPos;
    /* 0x0304 */ float HangStartLerp;
    /* 0x0308 */ class iModelTag HangPawTag[4];
    /* 0x0388 */ float HangPawOffset;
    /* 0x038C */ float HangElapsed;
    /* 0x0390 */ float Jump_CurrGravity;
    /* 0x0394 */ float Jump_HoldTimer;
    /* 0x0398 */ float Jump_ChangeTimer;
    /* 0x039C */ signed int Jump_CanDouble;
    /* 0x03A0 */ signed int Jump_CanFloat;
    /* 0x03A4 */ signed int Jump_SpringboardStart;
    /* 0x03A8 */ class _zPlatform *Jump_Springboard;
    /* 0x03AC */ signed int cheat_mode;
    /* 0x03B0 */ unsigned int Inv_Clue;
    /* 0x03B4 */ unsigned int Inv_Key;
    /* 0x03B8 */ unsigned int Inv_Chances;
    /* 0x03BC */ unsigned int Inv_ScoobySnacks;
    /* 0x03C0 */ unsigned int Inv_Specials;
    /* 0x03C4 */ unsigned int Inv_Mtokens[1];
    /* 0x03C8 */ signed int Inv_Ammo[2];
    /* 0x03D0 */ signed int Inv_AmmoMax[2];
    /* 0x03D8 */ class iModelTag BubbleTag;
    /* 0x03F8 */ class _xEntDrive drv;
    /* 0x0448 */ class xSurface *floor_surf;
    /* 0x044C */ class _xVec3 floor_norm;
    /* 0x0458 */ signed int slope;
    /* 0x045C */ class _xCollis earc_coll;
    /* 0x04A0 */ class _xSphere head_sph;
    /* 0x04B0 */ class iModelTag center_tag;
    /* 0x04D0 */ class iModelTag head_tag;
    /* 0x04F0 */ unsigned int TongueFlags[2];
    /* 0x04F8 */ class _xVec3 RootUp;
    /* 0x0504 */ class _xVec3 RootUpTarget;
    /* 0x0510 */ class zPlayerStatic s;
    /* 0x064C */ class zCheckPoint cp;
};

// from zEntPlayer.cpp
// size: 0x1C
class zPlayerSituation {
    /* 0x0000 */ unsigned int priority;
    /* 0x0004 */ unsigned int countMax;
    /* 0x0008 */ float delay;
    /* 0x000C */ float punchDelay;
    /* 0x0010 */ float elapsedTime;
    /* 0x0014 */ unsigned int count;
    /* 0x0018 */ unsigned int type;
};

// from xScrFx.cpp
// size: 0x13C
class zPlayerStatic {
    /* 0x0000 */ unsigned short AnalogMin;
    /* 0x0002 */ unsigned short AnalogMax;
    /* 0x0004 */ float SpeedSneak[4];
    /* 0x0014 */ float SpeedWalk[4];
    /* 0x0024 */ float SpeedRun[4];
    /* 0x0034 */ float SpeedAir[4];
    /* 0x0044 */ float SpeedHotsauce[4];
    /* 0x0054 */ float AnimSneak[3];
    /* 0x0060 */ float AnimWalk[3];
    /* 0x006C */ float Gravity;
    /* 0x0070 */ float JumpGravity;
    /* 0x0074 */ float GravSmooth;
    /* 0x0078 */ float FloatSpeed;
    /* 0x007C */ float ButtsmashSpeed;
    /* 0x0080 */ class zJumpParam Jump;
    /* 0x0090 */ class zJumpParam Double;
    /* 0x00A0 */ class zJumpParam Bounce;
    /* 0x00B0 */ class zJumpParam Spring;
    /* 0x00C0 */ class zJumpParam Toss;
    /* 0x00D0 */ float HeadbuttSpeedMin;
    /* 0x00D4 */ float HeadbuttSpeedMax;
    /* 0x00D8 */ float HeadbuttAccel;
    /* 0x00DC */ float HeadbuttTimeMin;
    /* 0x00E0 */ float HeadbuttTimeMax;
    /* 0x00E4 */ float HeadbuttTurn;
    /* 0x00E8 */ float FireDelay;
    /* 0x00EC */ float FireRepeat;
    /* 0x00F0 */ float FirePress;
    /* 0x00F4 */ float FireTargetYaw;
    /* 0x00F8 */ float FireTargetPitch;
    /* 0x00FC */ float FireTargetDist;
    /* 0x0100 */ float FireTargetRadius;
    /* 0x0104 */ float FireTrack;
    /* 0x0108 */ float FireGumSpeed;
    /* 0x010C */ float FireSoapSpeed;
    /* 0x0110 */ float FireGumTimer;
    /* 0x0114 */ float FireSoapTimer;
    /* 0x0118 */ unsigned int Initial_Specials;
    /* 0x011C */ unsigned int TakeDamage;
    /* 0x0120 */ float StartSlideAngle;
    /* 0x0124 */ float StopSlideAngle;
    /* 0x0128 */ float RotMatchMaxAngle;
    /* 0x012C */ float RotMatchMatchTime;
    /* 0x0130 */ float RotMatchRelaxTime;
    /* 0x0134 */ unsigned short AmmoCapacityGum;
    /* 0x0136 */ unsigned short AmmoCapacitySoap;
    /* 0x0138 */ unsigned short AmmoSceneGum;
    /* 0x013A */ unsigned short AmmoSceneSoap;
};

// from xScrFx.cpp
// size: 0x18
class zPortalAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned int assetCameraID;
    /* 0x000C */ unsigned int assetMarkerID;
    /* 0x0010 */ float ang;
    /* 0x0014 */ unsigned int sceneID;
};

// from zScene.cpp
// size: 0x44
class zProjectileTypeAsset : public p2BaseAsset {
    /* 0x0008 */ signed int efxType;
    /* 0x000C */ unsigned int modelID;
    /* 0x0010 */ unsigned int animID;
    /* 0x0014 */ unsigned int atRestModelID;
    /* 0x0018 */ unsigned int atRestAnimID;
    /* 0x001C */ signed int destructEnabled;
    /* 0x0020 */ float destructTime;
    /* 0x0024 */ float destructDist;
    /* 0x0028 */ signed int oriented;
    /* 0x002C */ signed int extra_space[6];
};

// from zEnt.cpp
// size: 0x18
class zSFX : public xBase {
    /* 0x0010 */ class zSFXAsset *asset;
    /* 0x0014 */ unsigned int sndID;
};

// from zEnt.cpp
// size: 0x30
class zSFXAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned short flagsSFX;
    /* 0x000A */ unsigned short freq;
    /* 0x000C */ float freqm;
    /* 0x0010 */ unsigned int soundAssetID;
    /* 0x0014 */ unsigned int attachID;
    /* 0x0018 */ unsigned char loopCount;
    /* 0x0019 */ unsigned char priority;
    /* 0x001A */ unsigned char volume;
    /* 0x001B */ unsigned char pad;
    /* 0x001C */ class _xVec3 pos;
    /* 0x0028 */ float range;
    /* 0x002C */ float range_falloff;
};

// from zVar.cpp
// size: 0x64
class zSaveLoadGame {
    /* 0x0000 */ char label[64];
    /* 0x0040 */ char date[32];
    /* 0x0060 */ signed int progress;
};

// from zSaveLoad.cpp
// size: 0xC
class zSaveLoadUI {
    /* 0x0000 */ unsigned int entry;
    /* 0x0004 */ unsigned int nameID;
    /* 0x0008 */ char *name;
};

// from xScrFx.cpp
// size: 0x1DC
class zScene : public _xScene {
    /* 0x006C */ unsigned int sceneID;
    /* 0x0070 */ class _zPortal *pendingPortal;
    /* 0x0074 */ unsigned int num_ents;
    /* 0x0074 */ unsigned int num_base;
    /* 0x0078 */ class xBase **base;
    /* 0x0078 */ class _zEnt **ents;
    /* 0x007C */ unsigned int baseCount[43];
    /* 0x0128 */ class xBase *baseList[43];
    /* 0x01D4 */ unsigned int num_bubbles;
    /* 0x01D8 */ class _zEnt **bubbles;
};

// from zScene.cpp
// size: 0x24
class zSceneArea {
    /* 0x0000 */ char *desc;
    /* 0x0004 */ enum _tagSceneAreas index;
    /* 0x0008 */ unsigned int totalSnacks;
    /* 0x000C */ unsigned int constTotalSnacks;
    /* 0x0010 */ unsigned int totalMTokens;
    /* 0x0014 */ unsigned int sceneIDs[4];
};

// from zScene.cpp
// size: 0x10
class zSceneLevel {
    /* 0x0000 */ char *desc;
    /* 0x0004 */ char name;
    /* 0x0008 */ signed int index;
    /* 0x000C */ unsigned int totalClues;
};

// from zScene.cpp
// size: 0x18
class zSceneObjectInstanceDesc {
    /* 0x0000 */ char *name;
    /* 0x0004 */ signed int baseType;
    /* 0x0008 */ unsigned int assetType;
    /* 0x000C */ unsigned int sizeRuntime;
    /* 0x0010 */ unsigned int (*func)(class zScene *, class zSceneObjectInstanceDesc *, unsigned int);
    /* 0x0014 */ void (*objectInitFunc)(void *, void *);
};

// from zScript.cpp
// size: 0x10
class zScriptAsset : public p2BaseAsset {
    /* 0x0008 */ float scriptStartTime;
    /* 0x000C */ unsigned int eventCount;
};

// from zScript.cpp
// size: 0x20
class zScriptEventAsset {
    /* 0x0000 */ float time;
    /* 0x0004 */ unsigned int widget;
    /* 0x0008 */ unsigned int paramEvent;
    /* 0x000C */ float param[4];
    /* 0x001C */ unsigned int paramWidget;
};

// from xScrFx.cpp
// size: 0xE8
class zShaggy1Globals {
    /* 0x0000 */ class _zEnt *GrabTry;
    /* 0x0004 */ class _zEnt *Grab;
    /* 0x0008 */ float GrabRot;
    /* 0x000C */ float GrabOffset;
    /* 0x0010 */ float GrabTimer;
    /* 0x0014 */ class _xVec3 ReleaseTrans;
    /* 0x0020 */ class _xVec3 HangTrans;
    /* 0x002C */ class _xVec3 LastDelta;
    /* 0x0038 */ unsigned int ThrowEnded;
    /* 0x003C */ float ExtraTransY;
    /* 0x0040 */ class _xVec3 ThrowPos;
    /* 0x004C */ class _xVec3 ThrowVel;
    /* 0x0058 */ unsigned int CatchMe;
    /* 0x005C */ unsigned int CatchYou;
    /* 0x0060 */ class _zEnt *HangTarget;
    /* 0x0064 */ float ThrowTimer;
    /* 0x0068 */ class _xVec3 HangStart;
    /* 0x0074 */ class _xVec3 HangRelative;
    /* 0x0080 */ unsigned int SetDown;
    /* 0x0084 */ unsigned int GrabOn;
    /* 0x0088 */ unsigned int ThrowState;
    /* 0x008C */ float ThrowRot;
    /* 0x0090 */ float CatchHeight;
    /* 0x0094 */ float CatchVel;
    /* 0x0098 */ class xBase *ThrowTgt;
    /* 0x009C */ float ThrowTgtAngle;
    /* 0x00A0 */ float ThrowTgtPeak;
    /* 0x00A4 */ float ThrowTgtOffset;
    /* 0x00A8 */ unsigned int ThrowTgtAdjust;
    /* 0x00AC */ class _xVec3 ThrowTgtVec;
    /* 0x00B8 */ float ThrowTgtSpeed;
    /* 0x00BC */ float ThrowAccel;
    /* 0x00C0 */ class _zEnt *Hangable[10];
};

// from xEnt.cpp
// size: 0xD0
class zSurfAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned char game_damage_type;
    /* 0x0009 */ unsigned char game_sticky;
    /* 0x000A */ unsigned char game_damage_flags;
    /* 0x000B */ unsigned char surf_type;
    /* 0x000C */ unsigned char phys_pad;
    /* 0x000D */ unsigned char sld_start;
    /* 0x000E */ unsigned char sld_stop;
    /* 0x000F */ unsigned char phys_flags;
    /* 0x0010 */ float friction;
    /* 0x0014 */ class zSurfMatFX matfx;
    /* 0x002C */ class zSurfColorFX colorfx;
    /* 0x0034 */ unsigned int texture_anim_flags;
    /* 0x0038 */ class zSurfTextureAnim texture_anim[2];
    /* 0x0050 */ unsigned int uvfx_flags;
    /* 0x0054 */ class zSurfUVFX uvfx[2];
    /* 0x00CC */ unsigned char on;
    /* 0x00CD */ unsigned char surf_pad[3];
};

// from xEnt.cpp
// size: 0x8
class zSurfColorFX {
    /* 0x0000 */ unsigned short flags;
    /* 0x0002 */ unsigned short mode;
    /* 0x0004 */ float speed;
};

// from xEnt.cpp
// size: 0x18
class zSurfMatFX {
    /* 0x0000 */ unsigned int flags;
    /* 0x0004 */ unsigned int bumpmapID;
    /* 0x0008 */ unsigned int envmapID;
    /* 0x000C */ float shininess;
    /* 0x0010 */ float bumpiness;
    /* 0x0014 */ unsigned int dualmapID;
};

// from xEnt.cpp
// size: 0xC
class zSurfTextureAnim {
    /* 0x0000 */ unsigned short pad;
    /* 0x0002 */ unsigned short mode;
    /* 0x0004 */ unsigned int group;
    /* 0x0008 */ float speed;
};

// from xEnt.cpp
// size: 0x3C
class zSurfUVFX {
    /* 0x0000 */ signed int mode;
    /* 0x0004 */ float rot;
    /* 0x0008 */ float rot_spd;
    /* 0x000C */ class _xVec3 trans;
    /* 0x0018 */ class _xVec3 trans_spd;
    /* 0x0024 */ class _xVec3 scale;
    /* 0x0030 */ class _xVec3 scale_spd;
};

// from xEnt.cpp
// size: 0x18
class zSurfacePropTexAnim {
    /* 0x0000 */ unsigned short mode;
    /* 0x0004 */ float speed;
    /* 0x0008 */ float frame;
    /* 0x000C */ unsigned int group;
    /* 0x0010 */ signed int group_idx;
    /* 0x0014 */ class xBase *group_ptr;
};

// from xEnt.cpp
// size: 0x3C
class zSurfacePropUVFX {
    /* 0x0000 */ signed int mode;
    /* 0x0004 */ float rot;
    /* 0x0008 */ float rot_spd;
    /* 0x000C */ class _xVec3 trans;
    /* 0x0018 */ class _xVec3 trans_spd;
    /* 0x0024 */ class _xVec3 scale;
    /* 0x0030 */ class _xVec3 scale_spd;
};

// from xEnt.cpp
// size: 0xB4
class zSurfaceProps {
    /* 0x0000 */ class zSurfAsset *asset;
    /* 0x0004 */ unsigned int texanim_flags;
    /* 0x0008 */ class zSurfacePropTexAnim texanim[2];
    /* 0x0038 */ unsigned int uvfx_flags;
    /* 0x003C */ class zSurfacePropUVFX uvfx[2];
};

// from zTimer.cpp
// size: 0xC
class zTimerAsset : public p2BaseAsset {
    /* 0x0008 */ float seconds;
};

// from zEntPickup.cpp
// size: 0x7C
class zUIAsset : public p2EntAsset {
    /* 0x0050 */ unsigned int uiFlags;
    /* 0x0054 */ unsigned short dim[2];
    /* 0x0058 */ unsigned int textureID;
    /* 0x005C */ float uva[2];
    /* 0x0064 */ float uvb[2];
    /* 0x006C */ float uvc[2];
    /* 0x0074 */ float uvd[2];
};

// from zUIFont.cpp
// size: 0x9C
class zUIFontAsset : public zUIAsset {
    /* 0x007C */ unsigned short uiFontFlags;
    /* 0x007E */ unsigned char mode;
    /* 0x007F */ unsigned char fontID;
    /* 0x0080 */ unsigned int textAssetID;
    /* 0x0084 */ unsigned char bcolor[4];
    /* 0x0088 */ unsigned char color[4];
    /* 0x008C */ unsigned short inset[4];
    /* 0x0094 */ unsigned short space[2];
    /* 0x0098 */ unsigned short cdim[2];
};

// from zVar.cpp
// size: 0x10
class zVarEntry {
    /* 0x0000 */ unsigned int entry;
    /* 0x0004 */ unsigned int varNameID;
    /* 0x0008 */ char *varName;
    /* 0x000C */ unsigned int (*varCB)();
};

// from zVar.cpp
// size: 0xC
class zVarMagicEntry {
    /* 0x0000 */ unsigned int varNameID;
    /* 0x0004 */ char *varName;
    /* 0x0008 */ char * (*varCB)(char *);
};

// from zEntPlayer.cpp
// size: 0x50
class zVolumeAsset : public p2BaseAsset {
    /* 0x0008 */ unsigned int flags;
    /* 0x000C */ class _xBound bound;
    /* 0x0044 */ float rot;
    /* 0x0048 */ float xpivot;
    /* 0x004C */ float zpivot;
};
