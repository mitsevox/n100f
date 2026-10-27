# Library code

The library block holds MetroTRK, the Metrowerks runtime, the MSL C library and the Dolphin SDK, followed by Bink, the CARD library and RenderWare. [splits.md](splits.md#library-units) describes how the first four were mapped, and [Bink units](splits.md#bink-units) how Bink was. This page covers how their source comes in.

## Build configuration

`configure.py` lists every mapped library unit as a `NonMatching` object, so objdiff measures each unit against its split target as soon as its source exists. Each library uses the profile the unit map was placed with (`config/GIHE78/libmap.json` "profiles"):

| Helper | Libraries | Compiler | Flags | Progress category |
| --- | --- | --- | --- | --- |
| `TRKLib` | `TRK_MINNOW_DOLPHIN` | GC/1.3 | `cflags_trk` | `trk` |
| `RuntimeLib` | `Runtime.PPCEABI.H`, `MSL_C.PPCEABI.bare.H` | GC/1.3.2, or GC/1.3 per object | `cflags_runtime` | `runtime` |
| `DolphinLib` | `dolphin/<library>` | GC/1.2.5n | `cflags_sdk` | `sdk` |
| `BinkLib` | `binkngc` | ProDG/3.5 | `cflags_bink` | `bink` |

The flags are shown compatible with the target, not established. An MSL object that placed under only one compiler sets that version. The others take the library default, and their import must confirm it. Each import also reconfirms a pick that rests on a partial placement: `alloc.c` is GC/1.3 on 2 of its 34 functions.

Bink is ProDG (SN Systems GCC) code, built through `tools/prodg_cc.py`. `fft.c` compiles to its unit under every ProDG version from 3.5 to 3.9.3, and Ooura's FFT compiles to none of its functions under CodeWarrior, so the family is established but the version is not. `cflags_bink` is likewise shown compatible, not established: it rests on `fft.c` only. `ProDG/3.5` is the only version the wrapper supports; the first import whose code separates the versions settles it.

GC/1.2.5n writes `.comment` version 8 (compiler 2.3.0.1). GC/1.3 and GC/1.3.2 both write version 10 (2.4.2.1), which is the project default (`mw_comment_version`). The SDK units therefore carry `comment:8` in `splits.txt`.

## Headers

Library headers declare only what the reconstructed units use, as game headers do.

| Path | Holds |
| --- | --- |
| `include/dolphin/` | Planned: Dolphin SDK public headers under the SDK's names, created by the first import that needs each one. The SDK's base types go in `dolphin/types.h`. |
| `include/libc/` | MSL C headers, shared with game code (`string.h`, `stdlib.h`) |
| `src/<library>/` | Planned: private headers of one library, next to its source |

Library code uses the SDK's type names. Game code keeps `types.h`.

When an import needs a declaration:

- Search the existing headers first, and extend the header that should own the declaration rather than adding a second copy.
- Add declarations in the order the original header would, so that parallel imports touch different lines.
- Record each header's upstream in CREDITS.md.

## Importing a unit

- Every function copied from an upstream is reviewed against this project's standards, whichever repository it comes from. A match in an upstream is evidence that the code exists, not that its source is acceptable.
- Check the upstream's licence at the pinned commit, and the licence of any code that the upstream itself reused.
- Originally handwritten assembly needs provenance (CONTRIBUTING.md, "Compiler versions and flags"). A library function may be linked as assembly only when the original library shipped it as assembly, shown by the original library itself (a vendor-shipped `.s` file, for example) or by the target. A decompilation's `.s` file or `asm` block is that project's choice and is never provenance, and a failed match is never evidence of assembly. Each such function also needs an evidence row showing that no compiler emits it, and review checks every one, as the project owner decided for library assembly. `asm` blocks in C source that the compiler builds are compiled source, not this exception.
- Map the unit's data sections when its source compiles, and record them in `evidence.tsv`.

## Bink

No public decompilation supplies Bink source this project can import:

- bfbb (`d8e8eac`) has the same Bink build (see [Bink units](splits.md#bink-units)), but 10 of its 15 Bink files are empty and the rest are unfinished. Its `src/bink/include/bink.h`, `radbase.h` and `rad3d.h` are RAD Game Tools' SDK headers (`radbase.h` carries RAD's copyright string), and its `binkread.c` embeds RAD's logo movie as `LogoData`. bfbb's CC0 dedication cannot cover RAD's material, so none of it is copied.
- tssm (`fc32870`) has a Bink header and no Bink source.

`bink/src/sdk/fft.c` is Takuya Ooura's general-purpose FFT package (`fftsg.c`) built for `float`: the function names match, and nine functions of the current release (`fft.tgz`, SHA-256 `52bb637c70b971958ec79c9c8752b1df5ff0218a4db4510e60826e0cb79b5296`) compile to the target unchanged apart from the type. Five Bink functions (`cftrec1`, `cftrec2`, `cftexp1`, `cftexp2`, `cftfx42`) follow the package's pre-2006 structure and are fitted to the target. The licence on Ooura's FFT page allows use, copying, modification and distribution of modified code for any purpose, on condition that modified code refers to the package. His notice is in CREDITS.md, not in the source, as the project owner decided. `src/bink/src/sdk/fft.c` reproduces the unit's whole `.text` and its `.rodata` under every ProDG version from 3.5 to 3.9.3, so it does not settle the version either. It stays `NonMatching` until the MSL float wrappers it calls (`atanf`, `cosf` and `sinf`, `0x800F38F8`, `0x800F38D4` and `0x800F38B0`) are named, because the link resolves those calls by name.

Bink headers are this project's own, declaring only what the reconstructed units use. The logo movie stays out of the repository: its data is split from `main.dol` at build time like any other unmapped data.
