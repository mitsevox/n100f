# Credits

Imported and reused source keeps its origin, pinned revision, license and local modifications here. Copyright and license notices in imported files remain intact.

## Game source

`src/flat/xMorph.cpp` takes its function bodies from [bfbbdecomp/bfbb](https://github.com/bfbbdecomp/bfbb/blob/d8e8eac38dd617ee72161f4cb2da4498bc89aad6/src/SB/Core/x/xMorph.cpp) (`src/SB/Core/x/xMorph.cpp`, revision `d8e8eac38dd617ee72161f4cb2da4498bc89aad6`), published under CC0 1.0. Local changes: includes reduced to the headers this project defines.

`src/flat/xBase.cpp`, `xClimate.cpp`, `xIni.cpp`, `xordarray.cpp`, `xPar.cpp`, `xScrFx.cpp` and `xString.cpp` take the bodies of their functions from the files of the same names in bfbb's `src/SB/Core/x/` at the same revision (CC0 1.0). Each file holds only the functions whose compiled form matches the target. Local changes:

- Includes are reduced to the headers this project defines, and type names follow the SLUS-20349 DWARF (`p2BaseAsset`, `p2EnvAsset`).
- `xordarray.cpp` drops bfbb's redundant local initialisers and spells out the `XOrdCompareCallback`/`XOrdTestCallback` typedefs as function-pointer parameters, since the DWARF names no typedef.
- `xParAlloc` drops bfbb's `xPar** next` temporary and reads and clears `dead->m_next` directly.
- `xBase.cpp` writes bfbb's inline helpers `xBaseIsEnabled`, `xBaseEnable`, `xBaseDisable` and `xBaseValidate` out as flag operations.
- `xClimateVecFromAngle` writes bfbb's `xMat3x3Identity` out as the `memcpy` it performs and calls `xVec3Init`, which the DWARF places in `xMath3.h` and `include/xMath3.h` defines inline.
- `xClimateVecFromAngle` and `xIniGetIndex` keep bfbb's names but are declared `static inline` by choice: neither the target nor the SLUS-20349 DWARF has an out-of-line copy, so their original linkage is unknown.
- `xClimateInitAsset` stores `1` and `0` to `_tagRain::rain`, an `S32` in the DWARF, where bfbb writes `1.0f` and `0.0f`.
- `xScrFxLetterbox` sets 16.0f where bfbb reads `sLetterBoxSize`, as the target does.

`src/flat/iCamera.cpp`, `iEnv.cpp`, `xAnim.cpp`, `xMath3.cpp`, `xMovePoint.cpp` and `zSurface.cpp` take their function bodies the same way, from bfbb's `src/SB/Core/x/` (`iCamera`, `xAnim`, `xMath3`, `xMovePoint`), `src/SB/Core/gc/iEnv.cpp` and `src/SB/Game/zSurface.cpp` at the same revision (CC0 1.0). Local changes:

- Type names and layouts follow the SLUS-20349 DWARF. `zSurfAsset` replaces bfbb's `zSurfAssetBase`, which has trailing fields N100F lacks, and `zSurfacePropUVFX` has no min/max fields. `xMovePoint` is a typedef of `_xMovePoint`. `xAnimTableGetState` takes `char*`, as its DWARF mangling does.
- `zSurface.cpp` writes bfbb's `MAX_MAPPER` (1) and `DEG2RAD` macros out as literals, and keeps only the statics the matched functions use.
- `iCameraSetNearFarClip` and `sCameraNearClip` use a 0.2f near plane where bfbb has 0.05f, as the target does.

`src/flat/xEntMotion.cpp`, `xFFX.cpp`, `xserializer.cpp`, `zCounter.cpp`, `zFog.cpp`, `zGust.cpp` and `zLight.cpp` take their function bodies the same way, from bfbb's `src/SB/Core/x/` (`xEntMotion`, `xFFX`, `xserializer`, and `xCounter` and `xFog` for `zCounter` and `zFog`) and `src/SB/Game/` (`zGust`, `zLight`) at the same revision (CC0 1.0). Local changes:

- Type and function names follow the SLUS-20349 DWARF: `_xEnt`, `_xScene`, `_zGust`, `_zLight`, `_zVolume`, `_xEntMotion`, `_xEntMechData`, `p2EntMotionAsset`, `_zCounter`, `_zFog` and `zFogAsset` replace bfbb's names, and the event callbacks take `F32*`, not `const F32*`, as their DWARF mangling does. bfbb's `xCounterLoad`, `xCounterSave`, `xCounterReset`, `xFogEventCB` and `xFogInit` bodies appear as the DWARF's `zCounter` and `zFog` functions.
- `zFogInit(void*, void*)` holds the body of bfbb's `xFogInit(xBase*, xFogAsset*)` directly, since the DWARF lists only the `void*` form.
- `_xEntMotion` puts its motion-data union at +0x14, where the GameCube code reaches it, and declares only the `mech` member; `p2EntMotionAsset` likewise declares only `mech`. `k_XENTMOTION_STOPPED` is defined in the source as 0x4.
- `zGustTurnOn`, `zGustTurnOff`, `zGustToggleOn`, `zGustReset`, `zLightEffectSet`, `zLightDestroy`, `zLightReset`, `zFogReset`, `xEntMotionRun` and `xSER_init_tables` are declared `static inline` by choice: neither the target nor the DWARF has an out-of-line copy, so their original linkage is unknown.
- `zLight.cpp` makes `sLightTotal`, `sPartitionVolume` and `gNumTemporaryLights` file-local, as the DWARF does, and declares `sEffectInitFuncs` without its initializer. `zLight.cpp` and `zGust.cpp` drop the statics the matched functions do not use. `xFFXApplyOne`, `UpdateGustFX` and the `xSER_xsgclt_*` callbacks are file-local, as in bfbb and the DWARF.
- `xSerial` defines its empty `operator delete` inline in `include/xserializer.h` instead of after the destructor.
- `zLightAddLocalEnv` tests `zlight->flags & 1` where bfbb writes `zlight->flags & 1 != 0`, and `xSER_xsgclt_svinfo_clt` reads `st_SERIAL_CLIENTINFO::actsize`, the DWARF field at +0xC, where bfbb adds 12 to the raw pointer; both compile to the same code. The zero tables use `{ 0 }` initializers, and bfbb's `param_N` parameters take descriptive names.


`include/types.h` is bfbb's `include/types.h` at the same revision, reduced to the integer and float typedefs and `NULL`.

`include/xMorph.h` follows bfbb's `xMorph.h` at the same revision, with `xVec3` taken from the PS2 DWARF, where the type is `_xVec3`. Its structure layouts match the SLUS-20349 DWARF. `include/xBase.h`, `xClimate.h`, `xIni.h`, `xMath3.h`, `xordarray.h`, `xPar.h`, `xScrFx.h`, `xserializer.h`, `xString.h`, `zMenu.h`, `iAnim.h`, `iCamera.h`, `iEnv.h`, `xAnim.h`, `xMemMgr.h`, `xMovePoint.h`, `xShadow.h`, `xstransvc.h`, `xSurface.h`, `zScene.h`, `zSurface.h`, `iLight.h`, `xEntMotion.h`, `xEvent.h`, `xFColor.h`, `xFFX.h`, `xsavegame.h`, `xutil.h`, `zCounter.h`, `zFog.h`, `zGust.h`, `zLight.h` and `zVolume.h` declare only what those units use, with layouts from the SLUS-20349 DWARF. `xEvent.h` declares only the four events the code compares against, with bfbb's names and the target's values. `include/rwcore.h` and `rpworld.h` declare the RenderWare calls those units make; `RpLight`, `RwLLLink`, `RwLinkList`, `RwObject`, `RwObjectHasFrame`, `RwRGBA`, `RwRGBAReal`, `RpMaterialList` `RwV3d`, `RwBBox` and the members of `RpWorld` up to `boundingBox` follow the DWARF, which agrees there with bfbb's GameCube RenderWare headers, and the other RenderWare types stay opaque.

## Build tooling

`configure.py`, `tools/*.py` (except project-specific additions), `.vscode/`, `.flake8`, `.gitattributes` and `docs/dtk/` come from [encounter/dtk-template](https://github.com/encounter/dtk-template/tree/d366876408b754891a0392badbc63bff3490469f), revision `d366876408b754891a0392badbc63bff3490469f`, published under CC0 1.0 (`tools/LICENSE.dtk-template`). Local changes: project configuration in `configure.py`, removed example libraries and REL helper, and merged ignore rules.

`tools/patches/dtk-v1.8.3-rom-copy-info.patch` modifies [encounter/decomp-toolkit](https://github.com/encounter/decomp-toolkit/tree/v1.8.3) v1.8.3 (MIT/Apache-2.0) so that `_rom_copy_info` is also searched for in the first text section. `tools/project.py` additionally passes a non-default DOL entry symbol to mwld.
