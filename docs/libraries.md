# Library code

The library block holds MetroTRK, the Metrowerks runtime, the MSL C library and the Dolphin SDK, followed by Bink, the CARD library and RenderWare. [splits.md](splits.md#library-units) describes how the first four were mapped. This page covers how their source comes in.

## Build configuration

`configure.py` lists every mapped library unit as a `NonMatching` object, so objdiff measures each unit against its split target as soon as its source exists. Each library uses the profile the unit map was placed with (`config/GIHE78/libmap.json` "profiles"):

| Helper | Libraries | Compiler | Flags | Progress category |
| --- | --- | --- | --- | --- |
| `TRKLib` | `TRK_MINNOW_DOLPHIN` | GC/1.3 | `cflags_trk` | `trk` |
| `RuntimeLib` | `Runtime.PPCEABI.H`, `MSL_C.PPCEABI.bare.H` | GC/1.3.2, or GC/1.3 per object | `cflags_runtime` | `runtime` |
| `DolphinLib` | `dolphin/<library>` | GC/1.2.5n | `cflags_sdk` | `sdk` |

The flags are shown compatible with the target, not established. An MSL object that placed under only one compiler sets that version. The others take the library default, and their import must confirm it.

GC/1.2.5n writes `.comment` version 8 (compiler 2.3.0.1). GC/1.3 and GC/1.3.2 both write version 10 (2.4.2.1), which is the project default (`mw_comment_version`). The SDK units therefore carry `comment:8` in `splits.txt`.

## Headers

Library headers declare only what the reconstructed units use, as game headers do.

| Path | Holds |
| --- | --- |
| `include/dolphin/` | Dolphin SDK public headers under the SDK's names (`dolphin/os.h`, `dolphin/gx.h`, ...). `dolphin/types.h` holds the SDK's base types. |
| `include/libc/` | MSL C headers (`string.h`, `stdlib.h`, ...), shared with game code |
| `src/<library>/` | Private headers of one library, next to its source |

Library code includes `dolphin/types.h` and uses the SDK's type names. Game code keeps `types.h`.

When an import needs a declaration:

- Search the existing headers first, and extend the header that should own the declaration rather than adding a second copy.
- Add declarations in the order the original header would, so that parallel imports touch different lines.
- Record each header's upstream in CREDITS.md.

## Importing a unit

- Every function copied from an upstream is reviewed against this project's standards, whichever repository it comes from. A match in an upstream is evidence that the code exists, not that its source is acceptable.
- Check the upstream's licence at the pinned commit, and the licence of any code that the upstream itself reused.
- Hand-written assembly is accepted only where the original was assembly. Each such function needs an evidence row showing that no compiler emits it.
- Map the unit's data sections when its source compiles, and record them in `evidence.tsv`.
- RenderWare source comes only from what the target and public, licensed references establish. Leaked RenderWare source is never used.
