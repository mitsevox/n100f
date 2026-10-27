# Library code

The library block holds MetroTRK, the Metrowerks runtime, the MSL C library and the Dolphin SDK, followed by Bink, the CARD library and RenderWare. [splits.md](splits.md#library-units) describes how the first four and CARD were mapped, and [Bink units](splits.md#bink-units) how Bink was. This page covers how their source comes in.

## Build configuration

`configure.py` lists every mapped library unit as a `NonMatching` object, so objdiff measures each unit against its split target as soon as its source exists. Each library uses the profile the unit map was placed with (`config/GIHE78/libmap.json` "profiles"):

| Helper | Libraries | Compiler | Flags | Progress category |
| --- | --- | --- | --- | --- |
| `TRKLib` | `TRK_MINNOW_DOLPHIN` | GC/1.3 | `cflags_trk` | `trk` |
| `RuntimeLib` | `Runtime.PPCEABI.H`, `MSL_C.PPCEABI.bare.H` | GC/1.3.2, or GC/1.3 per object | `cflags_runtime` | `runtime` |
| `DolphinLib` | `dolphin/<library>` | GC/1.2.5n | `cflags_sdk` | `sdk` |
| `BinkLib` | `binkngc` | ProDG/3.5 | `cflags_bink` | `bink` |
| `CardLib` | `card` (`dolphin/card/`) | GC/1.2.5n | `cflags_card` | `sdk` |

The flags are shown compatible with the target, not established. An MSL object that placed under only one compiler sets that version. The others take the library default, and their import must confirm it. Each import also reconfirms a pick that rests on a partial placement: `alloc.c` is GC/1.3 on 2 of its 34 functions.

Bink is ProDG (SN Systems GCC) code, built through `tools/prodg_cc.py`. `fft.c` compiles to its unit under every ProDG version from 3.5 to 3.9.3, and Ooura's FFT compiles to none of its functions under CodeWarrior, so the family is established but the version is not. `cflags_bink` is likewise shown compatible, not established: it rests on `fft.c` only. `ProDG/3.5` is the only version the wrapper supports; the first import whose code separates the versions settles it.

GC/1.2.5n writes `.comment` version 8 (compiler 2.3.0.1). GC/1.3 and GC/1.3.2 both write version 10 (2.4.2.1), which is the project default (`mw_comment_version`). The SDK and CARD units therefore carry `comment:8` in `splits.txt`.

CARD is the SDK's debug build ([splits.md](splits.md#card)). `cflags_card` compiles it at `-opt level=0 -inline off -schedule off -sym on -char signed` with `DEBUG=1`, the same flags as libmap's `card_debug` profile, so its assertions are compiled in, whatever `--debug` the build uses.

## Imported units

| Unit | Compiler | Data | Source |
| --- | --- | --- | --- |
| `Runtime.PPCEABI.H/__va_arg.c` | GC/1.3.2 (1.3 differs; 1.3.2 through 2.7 are identical) | none | C |
| `Runtime.PPCEABI.H/global_destructor_chain.c` | GC/1.3.2 (1.3 also matches) | `.dtors`, `.sbss` | C |
| `Runtime.PPCEABI.H/runtime.c` | GC/1.3.2 (1.3 also matches) | `.rodata` | asm functions, each with an `assembly` row in `evidence.tsv` |
| `TRK_MINNOW_DOLPHIN/` `mainloop.c`, `nubevent.c`, `nubinit.c`, `msg.c`, `serpoll.c`, `usr_put.c`, `dispatch.c`, `msghndlr.c`, `mutex_TRK.c`, `notify.c`, `mem_TRK.c`, `main_TRK.c`, `targcont.c`, `target_options.c` | GC/1.3 | each unit's `.rodata`, `.data` and `.bss`, as `data` rows in `evidence.tsv` | C |

All of them are `Matching` and link byte-identical.

## Headers

Library headers declare only what the reconstructed units use, as game headers do.

| Path | Holds |
| --- | --- |
| `include/dolphin/` | Dolphin SDK public headers under the SDK's names (`dolphin/os.h` and its `dolphin/os/*.h` parts, `dolphin/db.h`, `dolphin/si.h`, ...), each created by the first import that needs it. The SDK's base types are in `dolphin/types.h`. |
| `include/libc/` | MSL C headers, shared with game code (`string.h`, `stdlib.h`, `stdarg.h`) |
| `src/<library>/` | Private headers of one library, next to its source, included as `"<library>/<header>"` through an `-i src` path in that library's flags (`cflags_sdk` for `dolphin/dvd/__dvd.h`, `cflags_trk` for `TRK_MINNOW_DOLPHIN/trk.h`) |

Dolphin SDK code uses the SDK's type names. The runtime and MSL use plain C types. Game code keeps `types.h`.

When an import needs a declaration:

- Search the existing headers first, and extend the header that should own the declaration rather than adding a second copy.
- Add declarations in the order the original header would, so that parallel imports touch different lines.
- Record each header's upstream in CREDITS.md.

## Importing a unit

- Every function copied from an upstream is reviewed against this project's standards, whichever repository it comes from. A match in an upstream is evidence that the code exists, not that its source is acceptable.
- Check the upstream's licence at the pinned commit, and the licence of any code that the upstream itself reused.
- Originally handwritten assembly needs provenance (CONTRIBUTING.md, "Compiler versions and flags"). A library function may be linked as assembly only when the original library shipped it as assembly, shown by the original library itself (a vendor-shipped `.s` file, for example) or by the target. A decompilation's `.s` file or `asm` block is that project's choice and is never provenance, and a failed match is never evidence of assembly. Each such function also needs an evidence row showing that no compiler emits it, and review checks every one, as the project owner decided for library assembly. `asm` blocks in C source that the compiler builds are compiled source, not this exception.
- Map the unit's data sections when its source compiles, and record them in `evidence.tsv`. Each section's start is where the target's relocations to it land, at the offsets the compiled object's relocations give. Its end is the compiled size, rounded up to the next object when padding follows. One `data` row per section names the unit and section. Where a data symbol in `symbols.txt` spans the edge, split it there.

## Bink

No public decompilation supplies Bink source this project can import:

- bfbb (`d8e8eac`) has the same Bink build (see [Bink units](splits.md#bink-units)), but 10 of its 15 Bink files are empty and the rest are unfinished. Its `src/bink/include/bink.h`, `radbase.h` and `rad3d.h` are RAD Game Tools' SDK headers (`radbase.h` carries RAD's copyright string), and its `binkread.c` embeds RAD's logo movie as `LogoData`. bfbb's CC0 dedication cannot cover RAD's material, so none of it is copied.
- tssm (`fc32870`) has a Bink header and no Bink source.

`bink/src/sdk/fft.c` is Takuya Ooura's general-purpose FFT package (`fftsg.c`) built for `float`: the function names match, and nine functions of the current release (`fft.tgz`, SHA-256 `52bb637c70b971958ec79c9c8752b1df5ff0218a4db4510e60826e0cb79b5296`) compile to the target unchanged apart from the type. Five Bink functions (`cftrec1`, `cftrec2`, `cftexp1`, `cftexp2`, `cftfx42`) follow the package's pre-2006 structure and are fitted to the target. The licence on Ooura's FFT page allows use, copying, modification and distribution of modified code for any purpose, on condition that modified code refers to the package. His notice is in CREDITS.md, not in the source, as the project owner decided. `src/bink/src/sdk/fft.c` reproduces the unit's whole `.text` and its `.rodata` under every ProDG version from 3.5 to 3.9.3, so it does not settle the version either. It stays `NonMatching` until the MSL float wrappers it calls (`atanf`, `cosf` and `sinf`, `0x800F38F8`, `0x800F38D4` and `0x800F38B0`) are named, because the link resolves those calls by name.

`bink/shared/time/radcb.c`, `bink/src/sdk/popmal.c` and `bink/src/sdk/varbits.c` are this project's own reconstructions from the target, and are `Matching`. Each compiles to its unit's whole `.text` and data under every ProDG version from 3.5 to 3.9.3, byte for byte once the relocations are resolved, so they do not settle the version either. Four details of their source come from the target's code, not from style:

- `radcb.c`: the `state` and `suspends` counters of a handler and of a callback are `volatile`. The target reloads each one after storing it and keeps every access in source order, which only a volatile access does.
- `radcb.c` reads the bus clock at `0x800000F8` as a member of a structure at `0x80000000`. The target loads it with `lis 0x8000` and `lwz 0xF8` and no relocation. GCC folds a constant pointer such as the SDK's `*(u32*)(OS_BASE_CACHED | 0x00F8)` into `lis`/`ori` and a zero-offset load. An extern `__OSBusClock` would give the right instructions, but it would need a symbol at an address outside every section.
- `popmal.c` calls `memcpy` and `radcb.c` calls `memset` with no prototype in scope. The target sets up both calls with `crclr 6`, which GCC emits only for a call to an unprototyped or variadic function. So neither file includes `string.h`.
- `varbits.c` declares `_bitlevels` 64-byte aligned. That alignment is what places it at `+0xC0` after `VarBitsLens` and pads the section to `0x180` bytes, which is where `fft.c`'s `.rodata` starts.

Under GCC, `dolphin/types.h` defines `AT_ADDRESS` as empty, so the ProDG units can include `dolphin/os.h`. The MW units are unchanged.

Bink headers are this project's own, declaring only what the reconstructed units use. The logo movie stays out of the repository: its data is split from `main.dol` at build time like any other unmapped data.
