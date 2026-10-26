# Compiler profile evidence

The target has no symbols, so compiler versions are established by compiling known source under each candidate compiler and looking for the exact output in `main.dol`. The source used is [bfbbdecomp/bfbb](https://github.com/bfbbdecomp/bfbb) at `d8e8eac38dd617ee72161f4cb2da4498bc89aad6`: Battle for Bikini Bottom runs a later revision of the same Heavy Iron engine, and that project ships Dolphin SDK and MSL sources. The bfbb source is only a stimulus. A hit says which compiler produces those instructions; it does not establish the target's names, types or unit boundaries.

The claims are the `compiler` rows in [`config/GIHE78/evidence.tsv`](../../config/GIHE78/evidence.tsv). In short:

| Scope | Profile | Status |
| --- | --- | --- |
| Heavy Iron game code | GC/1.3.2 (mwcceppc 2.4.2 build 81) | Version established by the intersection of the bodies below. |
| Game flags | `-O4,s`, `-char signed`, `-inline auto,deferred` | `-O4,s` and `-char signed` supported. Inlining is provisional: some bodies match only with `-inline off`. Other flags are untested BfBB or template values. |
| Dolphin SDK | GC/1.2.5n | Version established; flags are bfbb's and only shown compatible. |
| MSL / Runtime | — | Undetermined (mixed results). |
| Linker | GC/1.3.2 | Same release as the compiler. Every mwld from 1.0 to 2.6 relinks the baseline identically. |

No single body identifies 1.3.2. Each excludes part of the candidate range, and only their intersection leaves 1.3.2:

| Target body | Words | Matches under | Flags file |
| --- | --- | --- | --- |
| 0x8000C1FC (iBoxIsectRay form) | 109 | 1.3.2 only | `game_inline_off.flags` |
| 0x8002E280 (xMat3x3RMulVec form) | 25 | 1.3, 1.3.2 | `game_inline_off.flags` |
| 0x800D2E9C, 0x800D2E4C (zSurfaceGetSlide*Angle) | 20 | 1.3, 1.3.2 | `game_inline_off.flags` |
| 0x8002BBF4 (HIPLBlockEnter), 0x80032610 (xParGroupAddPar) | 128, 124 | 1.3.2 to 2.7 | `game.flags` |
| 0x80038130, 0x80038258 (xSGAdd{Load,Save}Client) | 74, 53 | 1.3.2 to 2.7 | `game.flags` |

`GC/1.3.2r` in the decomp.dev compiler pack is a community-patched 1.3.2 made for another game's rodata pooling, not a Metrowerks release. It produces the same results as 1.3.2 here. Its objects' `.comment` gives 2.4.2 (shared with 1.3); the pack's `info.txt` gives 2.4.2 build 81.

## Reproducing

[`tools/compiler_probe.py`](../../tools/compiler_probe.py) compiles each source with each version, masks the bits that relocations fill in (branch targets, address halves, small-data offsets), and searches the DOL's text sections for every function of six words or more. It needs only Python and the compilers. `--summary 10` prints, per version, the units that failed and the bodies of 10+ words found exactly once, then every such body that some versions find and others do not.

```sh
git clone https://github.com/bfbbdecomp/bfbb ../bfbb && git -C ../bfbb checkout d8e8eac38dd617ee72161f4cb2da4498bc89aad6
D=docs/compiler_profile
probe() { python3 tools/compiler_probe.py --dol orig/GIHE78/sys/main.dol \
    --compilers build/compilers --wrapper build/tools/wibo --root ../bfbb --summary 10 "$@"; }
probe --versions 1.3,1.3.2,1.3.2r,2.0,2.0p1,2.5,2.6,2.7 --flags-file $D/game.flags --sources-file $D/game_units.txt
probe --versions 1.3,1.3.2,2.0,2.0p1,2.5,2.6,2.7 --flags-file $D/game_inline_off.flags --sources-file $D/game_units.txt \
    src/SB/Core/gc/iMath3.cpp src/SB/Game/zSurface.cpp src/SB/Core/x/xCamera.cpp
probe --versions 1.0,1.1,1.1p1,1.2.5,1.2.5n,1.3,1.3.2 --flags-file $D/game_no_gccinc.flags --sources-file $D/game_units.txt
probe --versions 1.2.5,1.2.5n,1.3.2 --flags-file $D/sdk.flags --sources-file $D/sdk_units.txt
```

| File | Use |
| --- | --- |
| `game.flags` | The `cflags_game` profile with bfbb's include paths. |
| `game_inline_off.flags` | The same with `-inline off`, so small inline helpers stay standalone bodies. |
| `game_no_gccinc.flags` | The same without `-gccinc`, which 1.0 to 1.2.5n reject. |
| `sdk.flags` | bfbb's SDK flags without `-gccinc`. |
| `game_units.txt`, `sdk_units.txt` | The bfbb units marked Matching upstream (72 game, 88 SDK). |

The flag counts in `evidence.tsv` come from editing `game.flags` one setting at a time, in the order the row lists, and reading the 1.3.2 line of the summary. Keep probe output out of Git.

## Caveats

- bfbb's source is from a later game. When it differs from N100F's code, a function misses under every compiler, which is no evidence either way. Two functions match only under 1.3: `xSG_sv_flipproc` at `0x80036D50` and `zConditionalEventCB` at `0x80045C34`. Under 1.3.2, bfbb's source inlines callees into them that the target leaves out of line. Other bodies in the same `xsavegame` unit exclude 1.3, and one unit cannot have two compilers, so these two are read as source differences.
- The inlining setting is provisional. `-inline auto,deferred` finds 31 more single-hit bodies than `-inline off`, but `-inline off` alone matches three: the `xSGWriteData` wrappers at `0x80037498`–`0x80037510`, `zEntTriggerEventCB` at `0x8007ACF8`, and the out-of-line `xMat3x3RMulVec` at `0x8002E280`. Some units may have been built with different inlining, or bfbb's source may differ there. Either way, that needs unit-level evidence before any per-unit override.
- Flag results are counted over a sample of 72 units.
