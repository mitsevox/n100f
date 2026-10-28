# Library code

The library block holds MetroTRK, the Metrowerks runtime, the MSL C library and the Dolphin SDK, followed by Bink, the CARD library and RenderWare. [splits.md](splits.md#library-units) describes how the first four and CARD were mapped, [Bink units](splits.md#bink-units) how Bink was, and [RenderWare units](splits.md#renderware-units) how RenderWare was. This page covers how their source comes in.

## Build configuration

`configure.py` lists every mapped library unit as a `NonMatching` object, so objdiff measures each unit against its split target as soon as its source exists. Each library uses the profile the unit map was placed with (`config/GIHE78-rev0/libmap.json` "profiles"):

| Helper | Libraries | Compiler | Flags | Progress category |
| --- | --- | --- | --- | --- |
| `TRKLib` | `TRK_MINNOW_DOLPHIN` | GC/1.3 | `cflags_trk` | `trk` |
| `RuntimeLib` | `Runtime.PPCEABI.H` | GC/1.3.2 | `cflags_runtime` | `runtime` |
| `MSLLib` | `MSL_C.PPCEABI.bare.H` | GC/1.3 | `cflags_msl` | `runtime` |
| `DolphinLib` | `dolphin/<library>` | GC/1.2.5n | `cflags_sdk` | `sdk` |
| `BinkLib` | `binkngc` | ProDG/3.5 | `cflags_bink` | `bink` |
| `CardLib` | `card` (`dolphin/card/`) | GC/1.2.5n | `cflags_card` | `sdk` |
| `RWLib` | `rpcollis`, `rphanim`, `rpmatfx`, `rpskin`, `rpusrdat`, `rpworld`, `rwcore` (`rwsdk/`) | GC/1.3.2 | `cflags_rw` | `renderware` |

The 89 RenderWare units in `config/GIHE78-rev0/rw_units.tsv` are listed NonMatching, one `RWLib` per library. Their compiler is GC/1.3 or GC/1.3.2, with 1.3 and 1.3.2 not separated; `RWLib` uses GC/1.3.2, the game's release. `cflags_rw` sets the established settings: `-O4`, `-char signed`, no contraction (expressed as `-fp_contract off`) and no `-use_lmw_stmw on`. `-lang=c` and `-common off` are carried over from `docs/compiler_profile/rw.flags`; they are not established, because the probe stimuli have no globals and do not test the language mode, and they are to be confirmed when RenderWare data is split. The rest are the base flags, which are not established either ([compiler profile](compiler_profile/README.md#renderware)). The probe covered `rwcore` only; the 23 exact `rpworld` functions below now support the same profile for the world library, and the other plugins take it on the assumption that one build produced the whole SDK. Never use leaked RenderWare source for them.

### Reconstructed RenderWare sources

`src/rwsdk/` holds sources written for this project from the target's own disassembly; no Criterion RenderWare source was used. Identifier names, and layouts checked against the target, may come from bfbb's `include/rwsdk/` headers, which are copies of the RenderWare SDK headers. By the project owner's decision, identifier names may come from there, and so may SDK macro bodies and enums (2026-10-28, n100f#74), each credited in [CREDITS.md](../CREDITS.md); no RenderWare function code, comments or other header text is copied, and leaked RenderWare source is never used. The units stay NonMatching (data is not split), and 43 of their functions compile to the target bytes with every relocation resolving to the target's symbol:

| Unit | Exact | Not yet |
| --- | --- | --- |
| `rwsdk/src/plcore/bavector.c` | 7 of 10 | `VectorMultVector` (register allocation and scheduling), `_rwSqrt` and `_rwInvSqrt` (see below) |
| `rwsdk/src/plcore/rwstring.c` | 3 of 5 | `StrChr`, `StrRChr` (register allocation) |
| `rwsdk/src/plcore/baerr.c` | 3 of 3 | |
| `rwsdk/src/plcore/bafsys.c` | 3 of 4 | `_rwFileSystemOpen`, not written: it stores MSL stdio functions that are still address labels |
| `rwsdk/src/plcore/resmem.c` | 4 of 4 | |
| `rwsdk/world/basector.c` | 6 of 6 | |
| `rwsdk/world/bamateri.c` | 7 of 8 | the function at `0x8014CFC8`, which opens the light module ([RenderWare limits](splits.md#limits-2)) |
| `rwsdk/world/balight.c` | 10 of 12 | `RpLightGetConeAngle`, `RpLightStreamRead` |

Structure layouts come from the SLUS-20349 DWARF where it has the type (game code includes RenderWare's headers) and from the target's accesses otherwise; `evidence.tsv` has a `type` or `field` row for each. The GameCube layout can differ: `RwTexture.refCount` is at `+0x54` here and `+0x50` in the DWARF. `include/rwcore.h` and `include/rpworld.h` now declare their types with `typedef` so the same headers serve the C RenderWare units and the C++ game code. Type, member, typedef and macro names the DWARF lacks (`RwGlobals`, `RwError`, `RwPluginRegistry`, the plugin callback typedefs, `RWSRCGLOBAL`) follow bfbb's `include/rwsdk/rwplcore.h` and `rpworld.h` (copies of the SDK headers) as related names. The file-local module data (`VectorModuleData`, `ErrorModuleData`, `MaterialModuleData`, `LightModuleData`) and resource-heap types (`ResHeap`, `ResHeapBlock`) have no source and use this project's own names. Source-form details the target fixes:

- `_rwSqrt` and `_rwInvSqrt` (59% and 62%) are not counted as exact. The target stores the argument to the stack and reloads the word after each store; under this profile only a `volatile` access reproduces that, and nothing shows the original used one, so the source keeps a plain pointer ([CONTRIBUTING](../CONTRIBUTING.md) bars a qualifier whose only justification is matching bytes).
- The resource heap tests a block's `used` word as `~used & 1` where the block must be free (`nor` then `clrlwi.`) and as `used & 1` where it must be in use (`clrlwi.` alone). `!(used & 1)` and `(used & 1) == 0` compile to `clrlwi.` with the opposite branch and do not match.
- `VectorMultVector` computes `x * right + y * up` for all three components before adding the `at` terms and storing: the target loads `right.y`, `right.z`, `up.y` and `up.z` before the first store. The source does the same, but the register allocation still differs.


The flags are shown compatible with the target, not established. MSL is one profile for the whole library: the twelve imported units match under GC/1.3 with `-inline auto,deferred` and `-char signed`, six of them differ under GC/1.3.2, and none needs GC/1.3.2. Each later MSL import reconfirms the profile on its own unit.

Bink is ProDG (SN Systems GCC) code, built through `tools/prodg_cc.py`. `fft.c` compiles to its unit under every ProDG version from 3.5 to 3.9.3, and Ooura's FFT compiles to none of its functions under CodeWarrior, so the family is established but the version is not. `cflags_bink` is likewise shown compatible, not established: it rests on `fft.c` only. `ProDG/3.5` is the only version the wrapper supports; the first import whose code separates the versions settles it.

GC/1.2.5n writes `.comment` version 8 (compiler 2.3.0.1). GC/1.3 and GC/1.3.2 both write version 10 (2.4.2.1), which is the project default (`mw_comment_version`). The SDK and CARD units therefore carry `comment:8` in `splits.txt`.

CARD is the SDK's debug build ([splits.md](splits.md#card)). `cflags_card` compiles it at `-opt level=0 -inline off -schedule off -sym on -char signed` with `DEBUG=1`, the same flags as libmap's `card_debug` profile, so its assertions are compiled in, whatever `--debug` the build uses.

## Imported units

| Unit | Compiler | Data | Source |
| --- | --- | --- | --- |
| `Runtime.PPCEABI.H/__va_arg.c` | GC/1.3.2 (with `cflags_runtime`, 1.3 differs and 1.3.2 through 2.7 are identical; with `-char signed` added, 1.3 also matches) | none | C |
| `Runtime.PPCEABI.H/global_destructor_chain.c` | GC/1.3.2 (1.3 also matches) | `.dtors`, `.sbss` | C |
| `Runtime.PPCEABI.H/runtime.c` | GC/1.3.2 (1.3 also matches) | `.rodata` | asm functions, each with an `assembly` row in `evidence.tsv` |
| `TRK_MINNOW_DOLPHIN/` `mainloop.c`, `nubevent.c`, `nubinit.c`, `msg.c`, `serpoll.c`, `usr_put.c`, `dispatch.c`, `msghndlr.c`, `mutex_TRK.c`, `notify.c`, `mem_TRK.c`, `main_TRK.c`, `targcont.c`, `target_options.c` | GC/1.3 | each unit's `.rodata`, `.data` and `.bss`, as `data` rows in `evidence.tsv` | C |
| `MSL_C.PPCEABI.bare.H/abort_exit.c` | GC/1.3 (msl profile) | `.bss`, `.sbss` | C |
| `MSL_C.PPCEABI.bare.H/alloc.c` | GC/1.3 (msl profile) | `.rodata`, `.bss`, `.sbss` | C |
| `MSL_C.PPCEABI.bare.H/arith.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/ctype.c` | GC/1.3 (msl profile) | `.data` | C |
| `MSL_C.PPCEABI.bare.H/mbstring.c` | GC/1.3 (msl profile) | `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/mem.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/mem_funcs.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/qsort.c` | GC/1.3 (msl profile) | none | C |
| `MSL_C.PPCEABI.bare.H/rand.c` | GC/1.3 (msl profile) | `.sdata` | C |
| `MSL_C.PPCEABI.bare.H/string.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata` | C |
| `MSL_C.PPCEABI.bare.H/strtold.c` | GC/1.3 (msl profile) | `.rodata`, `.sdata2` | C |
| `MSL_C.PPCEABI.bare.H/strtoul.c` | GC/1.3 (msl profile) | `.data` | C |
| `TRK_MINNOW_DOLPHIN/` `flush_cache.c`, `targimpl.c`, `dolphin_trk.c`, `mpc_7xx_603e.c`, `dolphin_trk_glue.c` | GC/1.3 | `data` rows in `evidence.tsv` | C, and asm functions, each with an `assembly` row in `evidence.tsv` |
| `TRK_MINNOW_DOLPHIN/targsupp.s` | assembled | none | assembly, with an `assembly` row per stub |
| `TRK_MINNOW_DOLPHIN/__exception.s` | assembled | none | assembly (the exception vector table), with an `assembly` row |
| `TRK_MINNOW_DOLPHIN/msgbuf.c` | GC/1.3 | `.rodata`, `.bss` | C |
| `TRK_MINNOW_DOLPHIN/mslsupp.c` | GC/1.3 | none | C |
| `dolphin/card/*.c` (16 objects) | GC/1.2.5n, `card_debug` flags | `.data`, `.sdata`, `.bss` | C ([CARD](#card)) |
| `dolphin/` SDK objects (`ai`, `amcstubs`, `ar`, `base`, `db`, `si`, `mtx`, `ax`, `dsp`, `dvd`, `exi`, `pad`, `vi`, `OdemuExi2`, `os` from `OS.c` to `OSTime.c`, and `gx` from `GXInit.c` to `GXPerf.c`) | GC/1.2.5n, `cflags_sdk` | each unit's sections, as `data` rows | C; asm functions where the original was assembly, each with an `assembly` row |

All of them are `Matching` and link byte-identical, except `dolphin/os/OSMemory.c`, `OSReset.c` and `OSRtc.c`, `dolphin/vi/vi.c`, and `TRK_MINNOW_DOLPHIN/msgbuf.c` and `mslsupp.c`. Each of the three os units compiles to the target's code except one function's stack frame, which is smaller than the target's unless an unused local is added (CREDITS.md, "Dolphin SDK"), so they stay `NonMatching` and the original bytes stay linked. `msgbuf.c` matches in 15 of its 19 functions: the compiler inlines `TRKAppendBuffer` into the other four, where the target calls it, and only a per-function `#pragma dont_inline` changes that (the `source-form` row for the unit in `evidence.tsv`), so it stays `NonMatching` too. `mslsupp.c` matches in eight of its nine functions: `convertFileMode` ends the binary flag with a narrowing that only sonicheroes' no-op `& 0xFF` mask reproduces, which this project does not use (the `source-form` row), so it stays `NonMatching` as well. `vi.c` matches in 14 of its 18 functions: without ac-decomp's `#pragma dont_inline` around `getTiming`, the compiler inlines `getTiming` into `__VIInit`, `VIInit` and `VIConfigure`, where the target calls it, and `VIGetNextField` needs ac-decomp's unused `u8 unused[4]` local, so it stays `NonMatching` too (unit row). `Pad.c` keeps the write-only `chanShift` local in `PADRead` that ac-decomp, tww, bfbb and FFCC-Decomp have; the owner accepted it on that evidence (source-form row). `__DSPHandler` keeps an unused `tmp_task`, `__DSP_boot_task` a write-only `mail`, `__fstLoad` keeps a write-only `arenaHi`, and `__OSInitAudioSystem` keeps a write-only `errFlag`, each under the write-only locals rule ("Importing a unit") with a source-form row. The AX units come from bfbb's AX files, which are from a 2003 SDK, cut back to the 2001 AX the target links: a 0xC0-byte `AXPB` with no low-pass filter, two output frames with no buffer mode or thread queue, no Dolby Pro Logic II or compressor commands, and a single 32-entry mix-cycle table. Each difference has a `source-form` or `type` row, and `__AXGetAuxBInput` tests the AuxA callback as the target does. `axDspSlave`, the DSP microcode image, and `axDspSlaveLength` stay unsplit data. The MSL units build under the `msl` profile in `configure.py`: GC/1.3 with `-inline auto,deferred` and `-char signed`. The `compiler` row in `evidence.tsv` records why.

## Headers

Library headers declare only what the reconstructed units use, as game headers do.

| Path | Holds |
| --- | --- |
| `include/dolphin/` | Dolphin SDK public headers under the SDK's names (`dolphin/os.h` and its `dolphin/os/*.h` parts, `dolphin/db.h`, `dolphin/si.h`, ...), each created by the first import that needs it. The SDK's base types are in `dolphin/types.h`. |
| `include/libc/` | MSL C headers, shared with game code (`ctype.h`, `eof.h`, `errno.h`, `float.h`, `limits.h`, `locale.h`, `math.h`, `mem_funcs.h`, `pool_alloc.h`, `stdarg.h`, `stddef.h`, `stdio.h`, `stdlib.h`, `string.h`) |
| `include/runtime/` | Runtime library headers (`NMWException.h`) |
| `src/<library>/` | Private headers of one library, next to its source, included as `"<library>/<header>"` through an `-i src` path in that library's flags (`cflags_sdk` for `dolphin/dvd/__dvd.h` and `dolphin/gx/__gx.h`, `cflags_trk` for `TRK_MINNOW_DOLPHIN/trk.h`, `cflags_card` for `dolphin/card/__card.h`, `cflags_msl` for `MSL_C.PPCEABI.bare.H/ansi_fp.h` and `scanf.h`) |

Dolphin SDK code uses the SDK's type names. The runtime and MSL use plain C types. Game code keeps `types.h`.

When an import needs a declaration:

- Search the existing headers first, and extend the header that should own the declaration rather than adding a second copy.
- Add declarations in the order the original header would, so that parallel imports touch different lines.
- Record each header's upstream in CREDITS.md.

## Importing a unit

- Every function copied from an upstream is reviewed against this project's standards, whichever repository it comes from. A match in an upstream is evidence that the code exists, not that its source is acceptable.
- Check the upstream's licence at the pinned commit, and the licence of any code that the upstream itself reused.
- Originally handwritten assembly needs provenance (CONTRIBUTING.md, "Compiler versions and flags"). A library function may be linked as assembly only when the original library shipped it as assembly, shown by the original library itself (a vendor-shipped `.s` file, for example) or by the target. A decompilation's `.s` file or `asm` block is that project's choice and is never provenance, and a failed match is never evidence of assembly. Each such function also needs an evidence row showing that no compiler emits it, and review checks every one, as the project owner decided for library assembly. `asm` blocks in C source that the compiler builds are compiled source, not this exception.
- Check the unit's locals against the owner's 2026-10-27 write-only locals rule ([CONTRIBUTING.md](../CONTRIBUTING.md#write-only-and-unused-locals)): compile with MWCC `-w unusedvar`, scan for locals that are set but never read, and report each hit in the PR as deleted, dropped as padding, or kept with a `source-form` row.
- Map the unit's data sections when its source compiles, and record them in `evidence.tsv`. Each section's start is where the target's relocations to it land, at the offsets the compiled object's relocations give. Its end is the compiled size, rounded up to the next object when padding follows. One `data` row per section names the unit and section. Where a data symbol in `symbols.txt` spans the edge, split it there.

## CARD

The CARD source is ac-decomp's, revised against the target (CREDITS.md, "Dolphin SDK"). Points that apply to the debug build:

- `DEBUG=1` selects the SDK's debug `os.h`, where `OSPhysicalToCached` and `OSCachedToPhysical` are functions rather than macros. The target calls them, so `include/dolphin/os.h` declares the function forms under `DEBUG`.
- An assertion's message is its condition as text, so the spacing inside the condition is part of the target's `.data` (`(u32) card->size`). Do not run `clang-format` over these files: it rewrites those conditions.
- The sources keep functions the linker strips, such as `CARDFastOpen`. With `-str reuse` the compiler pools each object's strings in order of first use, and a stripped function can be the first user of a string that kept code shares (`CARDFastOpen` asserts `0 <= chan && chan < 2` before `CARDOpen` does), so only the full source reproduces each `.data` section. These functions have no code in the target to check them against.
- `CardData`, the 352-byte DSP program in `CARDUnlock.c`, is binary data. `config/GIHE78-rev0/config.yml` extracts it from `main.dol` at build time (`extract`, raw header `dolphin/card/CardData.inc`), and `CARDUnlock.c` includes it, so the bytes are never committed.
- The Bink logo movie at `0x801CDF48` contains a word that looks like a pointer into CARD's `.data`. Its symbol is marked `noreloc`, so the build does not turn that word into a relocation against a CARD symbol.

## Bink

No public decompilation supplies Bink source this project can import:

- bfbb (`d8e8eac`) has the same Bink build (see [Bink units](splits.md#bink-units)), but 10 of its 15 Bink files are empty and the rest are unfinished. Its `src/bink/include/bink.h`, `radbase.h` and `rad3d.h` are RAD Game Tools' SDK headers (`radbase.h` carries RAD's copyright string), and its `binkread.c` embeds RAD's logo movie as `LogoData`. bfbb's CC0 dedication cannot cover RAD's material, so none of it is copied.
- tssm (`fc32870`) has a Bink header and no Bink source.

`bink/src/sdk/fft.c` is Takuya Ooura's general-purpose FFT package (`fftsg.c`) built for `float`: the function names match, and nine functions of the current release (`fft.tgz`, SHA-256 `52bb637c70b971958ec79c9c8752b1df5ff0218a4db4510e60826e0cb79b5296`) compile to the target unchanged apart from the type. Five Bink functions (`cftrec1`, `cftrec2`, `cftexp1`, `cftexp2`, `cftfx42`) follow the package's pre-2006 structure and are fitted to the target. The licence on Ooura's FFT page allows use, copying, modification and distribution of modified code for any purpose, on condition that modified code refers to the package. His notice is in CREDITS.md, not in the source, as the project owner decided. `src/bink/src/sdk/fft.c` reproduces the unit's whole `.text` and its `.rodata` under every ProDG version from 3.5 to 3.9.3, so it does not settle the version either. It is `Matching`: the link resolves its calls to the MSL float wrappers (`atanf`, `cosf` and `sinf`, `0x800F38F8`, `0x800F38D4` and `0x800F38B0`) by name, and the MSL import names them.

`bink/shared/time/radcb.c`, `bink/src/sdk/popmal.c` and `bink/src/sdk/varbits.c` are this project's own reconstructions from the target, and are `Matching`. Each compiles to its unit's whole `.text` and data under every ProDG version from 3.5 to 3.9.3, byte for byte once the relocations are resolved, so they do not settle the version either. Five details of their source come from the target's code, not from style:

- `radcb.c`: the `state` and `suspends` counters of a handler and of a callback are `volatile`. The target reloads each one after storing it and keeps every access in source order, which only a volatile access does.
- `radcb.c` reads the bus clock at `0x800000F8` as a member of a structure at `0x80000000`. The target loads it with `lis 0x8000` and `lwz 0xF8` and no relocation. GCC folds a constant pointer such as the SDK's `*(u32*)(OS_BASE_CACHED | 0x00F8)` into `lis`/`ori` and a zero-offset load. An extern `__OSBusClock` would give the right instructions, but it would need a symbol at an address outside every section.
- `popmal.c` calls `memcpy` and `radcb.c` calls `memset` with no prototype in scope. The target sets up both calls with `crclr 6`, which GCC emits only for a call to an unprototyped or variadic function. So neither file includes `string.h`.
- `varbits.c` indexes `VarBitsLens` in its 8-bit loop through a local, `len = 8`, rather than the literal `VarBitsLens[8]`. The target keeps the byte offset in a callee-saved register: it saves `r31` in a 16-byte frame, sets `li r31,32` before the loop and loads with `lwzx r0,r12,r31`. With the literal, GCC folds the offset into `lwz r0,32(r12)` and drops the frame, and 11 lines differ. No literal or `const` form reproduces the target, so the original indexed through a non-constant width at that point. A form that uses the width everywhere, masks included, does not reproduce it either, so the exact original form is not established.
- `varbits.c` declares `_bitlevels` 64-byte aligned. That alignment is what places it at `+0xC0` after `VarBitsLens` and pads the section to `0x180` bytes, which is where `fft.c`'s `.rodata` starts.

`bink/src/sdk/dct.c` is a `NonMatching` partial reconstruction. Its 8x8 transforms are the Independent JPEG Group's fast integer DCTs, `jpeg_fdct_ifast` (`jfdctfst.c`) and `jpeg_idct_ifast` (`jidctfst.c`) from IJG release 6b, which RAD adapted. The reconstruction follows IJG 6b statement for statement and keeps IJG's variable names; RAD's changes, which the target shows, are listed with IJG's notice in CREDITS.md ([Bink DCT](../CREDITS.md#bink-dct)). `FastFDCT8x8`, `FastFDCTs8x8` and the three table wrappers (`FastIDCT8x8`, `FastIDCT8x8d`, `FastmIDCT8x8`) compile to the target byte for byte under ProDG 3.5 to 3.9.3, and so do the four quantisation tables in `.rodata`. Three functions still differ:

- `fastidct8x8`: the column pass matches except for the registers. The row pass spills fewer values into the `t[8]` array than the target does, so it is 14 instructions short.
- `fastidct8x8d` writes each output pixel twice across and each row twice down, as 32-bit words. Its shape is close, but the register allocation differs throughout.
- `FastmIDCT8x8WithMotion` adds each output pixel to an 8-byte-stride motion block. It has the same register allocation and spill differences as `fastidct8x8`.

The 8 zero bytes between `fft.c`'s `.rodata` and the tables (`0x801A8E18`-`0x801A8E20`) are unassigned. The object's `.rodata` has 4-byte alignment, so the tables will not land at `0x801A8E20` until the source or the split accounts for those bytes.

Under GCC, `dolphin/types.h` defines `AT_ADDRESS` as empty, so the ProDG units can include `dolphin/os.h`. The MW units are unchanged.

Bink headers are this project's own, declaring only what the reconstructed units use. The logo movie stays out of the repository: its data is split from `main.dol` at build time like any other unmapped data.
