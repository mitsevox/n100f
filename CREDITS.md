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

`include/types.h` is bfbb's `include/types.h` at the same revision, reduced to the integer and float typedefs and `NULL`.

`include/xMorph.h` follows bfbb's `xMorph.h` at the same revision, with `xVec3` taken from the PS2 DWARF, where the type is `_xVec3`. Its structure layouts match the SLUS-20349 DWARF. `include/xBase.h`, `xClimate.h`, `xIni.h`, `xMath3.h`, `xordarray.h`, `xPar.h`, `xScrFx.h`, `xserializer.h`, `xString.h` and `zMenu.h` declare only what those units use, with layouts from the SLUS-20349 DWARF.

## Build tooling

`configure.py`, `tools/*.py` (except project-specific additions), `.vscode/`, `.flake8`, `.gitattributes` and `docs/dtk/` come from [encounter/dtk-template](https://github.com/encounter/dtk-template/tree/d366876408b754891a0392badbc63bff3490469f), revision `d366876408b754891a0392badbc63bff3490469f`, published under CC0 1.0 (`tools/LICENSE.dtk-template`). Local changes: project configuration in `configure.py`, removed example libraries and REL helper, and merged ignore rules.

`tools/patches/dtk-v1.8.3-rom-copy-info.patch` modifies [encounter/decomp-toolkit](https://github.com/encounter/decomp-toolkit/tree/v1.8.3) v1.8.3 (MIT/Apache-2.0) so that `_rom_copy_info` is also searched for in the first text section. `tools/project.py` additionally passes a non-default DOL entry symbol to mwld.
