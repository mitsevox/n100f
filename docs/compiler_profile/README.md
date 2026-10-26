# Compiler profile evidence

The target has no symbols, so compiler versions are established by compiling known source under each candidate compiler and looking for the exact output in `main.dol`. The source used is [bfbbdecomp/bfbb](https://github.com/bfbbdecomp/bfbb) at `d8e8eac38dd617ee72161f4cb2da4498bc89aad6`: Battle for Bikini Bottom runs a later revision of the same Heavy Iron engine, and that project ships Dolphin SDK and MSL sources. The bfbb source is only a stimulus. A hit says which compiler produces those instructions; it does not establish the target's names, types or unit boundaries.

The claims are the `compiler` rows in [`config/GIHE78/evidence.tsv`](../../config/GIHE78/evidence.tsv). In short:

| Scope | Profile | Status |
| --- | --- | --- |
| Heavy Iron game code | GC/1.3.2, `-O4,s -inline auto,deferred -char signed`, other flags as in `cflags_game` | Version established. The three named flags are supported; the rest are BfBB values that showed no effect. |
| Dolphin SDK | GC/1.2.5n | Version established; flags are bfbb's and only shown compatible. |
| MSL / Runtime | — | Undetermined (mixed results). |
| Linker | GC/1.3.2 | Same release as the compiler. Every mwld from 1.0 to 2.6 relinks the baseline identically. |

`GC/1.3.2r` in the decomp.dev compiler pack is a community-patched 1.3.2 made for another game's rodata pooling, not a Metrowerks release, so it is not a candidate.

## Reproducing

[`tools/compiler_probe.py`](../../tools/compiler_probe.py) compiles each source with each version, masks the bits that relocations fill in (branch targets, address halves, small-data offsets), and reports where every function of six words or more occurs in the DOL's text sections. It needs only Python and the compilers.

```sh
git clone https://github.com/bfbbdecomp/bfbb ../bfbb && git -C ../bfbb checkout d8e8eac38dd617ee72161f4cb2da4498bc89aad6
python3 tools/compiler_probe.py --dol orig/GIHE78/sys/main.dol \
    --compilers build/compilers --wrapper build/tools/wibo --root ../bfbb \
    --versions 1.3,1.3.2,2.0,2.0p1,2.6 --flags-file docs/compiler_profile/bfbb_probe.flags \
    src/SB/Core/gc/iMath3.cpp src/SB/Core/x/xsavegame.cpp src/SB/Core/x/xhipio.cpp > probe.json
```

Compare versions per function: the useful results are bodies that occur exactly once and only under some versions. Short bodies often occur many times and say little. Keep the JSON output out of Git.

## Caveats

- bfbb's source is from a later game. When it differs from N100F's code, a function misses under every compiler, which is no evidence either way. Two functions (`xSG_sv_flipproc` at `0x80036D50`, `zConditionalEventCB` at `0x80045C34`) match only under 1.3, because 1.3 declines inlining that 1.3.2 performs on bfbb's source. Several other functions in the same `xsavegame` unit match only under 1.3.2, and one unit cannot have two compilers, so these two are read as source differences.
- Flag results are counted over a sample of 72 units. A unit compiled with different settings is possible and needs its own evidence before any per-unit override.
