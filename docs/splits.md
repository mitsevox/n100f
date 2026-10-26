# Game-unit splits

`config/GIHE78/splits.txt` names 110 game units (`flat/<file>`). They come from `tools/splits/recover.py`, which compares the target with the PS2 USA retail v1.01 executable (SLUS-20349, SHA-1 `ebe640193e0ceb38deca343a0b576cfcba2d5920`). That executable keeps its DWARF. Every unit and edge is a provisional claim, recorded in `evidence.tsv` with origin `inferred`.

## Method

1. **Anchors.** Each function on both platforms is fingerprinted by the string literals and float constants it loads. GC references come from dtk's relocations; PS2 references come from `lui`/`addiu`, `$gp` and `lui`+`mtc1` patterns. Matches that are unique and mutual-best become anchors. The current run finds 300.
2. **Link order.** The GC game block (0x800055E0 to about 0x800D8000) is linked in case-insensitive alphabetical order of the DWARF compile-unit names under `E:\SD2\Game\flat\`. 280 of 300 anchors fit that order (longest increasing run). The PS2 ELF links in a different order, so its addresses are not a template.
3. **Alignment.** The PS2 functions, sorted by file name and then address, are aligned to the GC game functions by size, with the anchors pinned.
4. **Text edges.** Between two neighbouring units, the cut is placed where the fewest data references would cross the data edges the neighbours imply. When several cuts tie, the functions between the two units' outermost aligned functions stay in dtk's automatic objects, and the edge is `open`.
5. **Data edges.** Every data section follows the same unit order. Each cut falls on an 8-byte-aligned symbol boundary between two consecutive units that own literals or statics in that section. "Own" means only that unit's functions reference them. The cut chosen is the one with the fewest crossing references.
6. **Emission.** A unit is written only if it has an anchor and its GC function count is within 0.5x to 2x of its PS2 count, or if it has no anchor and the counts differ by at most one. The other 26 units (mostly platform `i*` files, plus a few with count mismatches) are reported but left unsplit.

A possible target-only cross-check, not yet in the tool: MW pools literals per unit, so two copies of the same `.sdata2` literal must belong to different units.

## Reproducing

The PS2 ELF is in the private build image at `/orig/SLUS-20349/SLUS_203.49`. After a dtk split (`ninja` or `dtk dol split`):

```sh
python3 tools/splits/recover.py --dol orig/GIHE78/sys/main.dol \
  --ps2-elf /orig/SLUS-20349/SLUS_203.49 \
  --splits units.txt --edges edges.tsv --units units.tsv
```

`units.txt` is the unit part of `splits.txt`. The output does not depend on which split produced the objects, because the relocations are the same.

## Limits

- The relink is byte-identical, which proves the cuts are consistent. It does not prove they are where the original units began and ended.
- PS2 v1.01 is a sibling build, not the target. Platform `i*` units differ on GC and are mostly unanchored.
- Header inline copies (`xMath3.h`, `xEnt.h`, `xModel.h`) are left to whichever unit contains them.
- The GC-only code after `zVolume`, plus MetroTRK, the runtime, the Dolphin SDK and RenderWare, is not covered here.
