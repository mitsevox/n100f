# Library code

The library block holds MetroTRK, the Metrowerks runtime, the MSL C library and the Dolphin SDK, followed by Bink, the CARD library and RenderWare. [splits.md](splits.md#library-units) describes how the first four were mapped. This page covers how their source comes in.

## Build configuration

`configure.py` lists every mapped library unit as a `NonMatching` object, so objdiff measures each unit against its split target as soon as its source exists. Each library uses the profile the unit map was placed with (`config/GIHE78/libmap.json` "profiles"):

| Helper | Libraries | Compiler | Flags | Progress category |
| --- | --- | --- | --- | --- |
| `TRKLib` | `TRK_MINNOW_DOLPHIN` | GC/1.3 | `cflags_trk` | `trk` |
| `RuntimeLib` | `Runtime.PPCEABI.H`, `MSL_C.PPCEABI.bare.H` | GC/1.3.2, or GC/1.3 per object | `cflags_runtime` | `runtime` |
| `DolphinLib` | `dolphin/<library>` | GC/1.2.5n | `cflags_sdk` | `sdk` |

The flags are shown compatible with the target, not established. An MSL object that placed under only one compiler sets that version. The others take the library default, and their import must confirm it. Each import also reconfirms a pick that rests on a partial placement: `alloc.c` is GC/1.3 on 2 of its 34 functions.

GC/1.2.5n writes `.comment` version 8 (compiler 2.3.0.1). GC/1.3 and GC/1.3.2 both write version 10 (2.4.2.1), which is the project default (`mw_comment_version`). The SDK units therefore carry `comment:8` in `splits.txt`.

## Headers

Library headers declare only what the reconstructed units use, as game headers do.

| Path | Holds |
| --- | --- |
| `include/dolphin/` | Dolphin SDK public headers under the SDK's names, created by the first import that needs each one. The SDK's base types are in `dolphin/types.h`. |
| `include/libc/` | MSL C headers, shared with game code (`string.h`, `stdlib.h`) |
| `src/<library>/` | Private headers of one library, next to its source, included as `"<library>/<header>"` through the `-i src` path in `cflags_base` (`TRK_MINNOW_DOLPHIN/trk.h`) |

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

## Imported units

| Library | Linked (`Matching`) | Still in original bytes |
| --- | --- | --- |
| `TRK_MINNOW_DOLPHIN` | `mainloop.c`, `nubevent.c`, `nubinit.c`, `msg.c`, `serpoll.c`, `usr_put.c`, `dispatch.c`, `msghndlr.c`, `support.c`, `mutex_TRK.c`, `notify.c`, `mem_TRK.c`, `main_TRK.c`, `targcont.c`, `target_options.c` | `msgbuf.c`, `flush_cache.c`, `targimpl.c`, `dolphin_trk.c`, `mpc_7xx_603e.c`, `dolphin_trk_glue.c`, `mslsupp.c`, and the assembly of `__exception.s` and `targsupp.s` |
