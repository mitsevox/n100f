# GIHE78 target configuration

USA release, disc revision 0. `main.dol` is 1,964,512 bytes with entry point `0x80016898` and SHA-1 `c77fb29d39b5898933e8ef4d1ccd2ddac404554e`. `.bss` starts at `0x801DF1C0` with size `0x83B34`. The disc has no linker map or symbols.

| File | Purpose |
| --- | --- |
| `config.yml` | decomp-toolkit project input: target executable, hash, symbol and split files |
| `build.sha1` | Expected hash of the complete built executable |
| `symbols.txt`, `splits.txt` | Symbol and split configuration. These are toolkit inputs, not recovered names or source boundaries; claims about either belong in `evidence.tsv`. |
| `evidence.tsv` | Curated claims in the [evidence format](../README.md) |

## Mapping onto the handbook stages

| Handbook stage | Implementation |
| --- | --- |
| Original-byte baseline | `python3 configure.py && ninja` with no `Matching` objects: dtk splits the DOL, relinks it and checks `build.sha1` |
| Accepted-source build | Objects marked `Matching` in `configure.py` replace their split ranges; the complete DOL must still match `build.sha1` |
| Independent comparisons | Objects marked `NonMatching` stay in their normal `src/` files; objdiff measures them against the split target objects while the original bytes remain linked |
| Promotion | Change the object from `NonMatching` to `Matching` after review; the source path does not move |

`Equivalent` objects and `--non-matching` builds are local diagnostics and never count as linked progress. Per-object `cflags` overrides need the same unit-level evidence as library profiles.

## Status

`TO_IMPLEMENT`: initial analysis, `symbols.txt` and `splits.txt`, the baseline relink, compiler and linker version evidence (`mw_comment_version`, `config.linker_version`), CI and report export.
