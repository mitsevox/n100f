# Game-unit splits

`config/GIHE78/splits.txt` names 100 game units (`flat/<file>`). They come from `tools/splits/recover.py`, which compares the target with the PS2 USA retail v1.01 executable SLUS-20349 (SHA-1 `ebe640193e0ceb38deca343a0b576cfcba2d5920`). That executable keeps its DWARF.

The names and extents are claims, not established facts. In `evidence.tsv`:
- each unit's extent is an `inferred` `unit` row with `provisional` or `open` edges;
- each source path is a separate `related` `unit-anchor` row, since it comes from another build's DWARF.

`config/GIHE78/split_units.tsv` lists all 135 candidate units, and `config/GIHE78/split_edges.tsv` gives every text and data edge with its class and the number of references that cross it.

## Method

1. **Anchors.** Every function on both platforms is fingerprinted by the string literals and float constants it loads. GC references come from dtk's relocations. PS2 references come from `lui`/`addiu`, `$gp` and `lui`+`mtc1` patterns. Unique, mutual-best matches become anchors; the current run finds 300.
2. **Link order.** The GC game block, from `0x800055E0` to the start of GC-only code at `0x800D7E00`, is linked in case-insensitive alphabetical order of the DWARF compile-unit names under `E:\SD2\Game\flat\`. 280 of the 300 anchors lie in one increasing run under that order. The PS2 ELF links in a different order, so its addresses are not a template.
3. **Alignment.** The PS2 functions, sorted by file name and then address, are aligned to the GC game functions by size, with the anchors pinned.
4. **Text edges.** Up to eight functions on either side of an edge may move, but each unit lends only half its aligned functions to each of its edges. Each candidate cut is scored by how many references land on the wrong side of the best data cut in every section. Moving an anchor counts as three such references. A static initialiser named in `.ctors` must end its unit, because MW emits `__sinit` last.
    - One best cut: it is used, and its crossing count is recorded.
    - Several cuts tie, and the alignment's cut is among them: the alignment's cut is used and labelled "alignment only".
    - Several cuts tie, and the alignment's cut is not among them: the tied functions stay in dtk's automatic objects, and both edges are `open`.

    An open edge then shrinks from both ends. The next unassigned function joins a neighbour when its references reach that neighbour and not the other one: calls to its functions, or loads of data that only its functions load. Calls into other units, such as library code, do not count either way. A function that ends its unit with `__sinit` takes nothing after it. The edge class records how many functions joined, and an edge with none left reads "closed by references".
5. **Data edges.** Every data section follows the same unit order. Each unit's unit-local items (loaded only by that unit's functions) must therefore appear in link order. The tool keeps the largest set of them whose owners never go backwards, and an item outside that set marks no range. Such an item is usually a global that another unit defines and only one unit loads, like an `xMath3.cpp` vector constant. Between consecutive units that load unit-local items in a section, the cut falls on an 8-byte-aligned symbol boundary, at the candidate with the fewest crossing references. Items between two units that unassigned functions load belong to no unit, and the edge is `open`. An item inside a written range that an unassigned function loads stays in the range, but it counts as a crossing reference on the edge facing that function. This is the data edge in the same section if one exists, and the text edge otherwise. A range that cannot start on an aligned symbol is left to dtk.
6. **Emission.** A unit is written in either of two cases:
    - it has an anchor, and its GC function count is 0.5x to 2x its PS2 count;
    - it has no anchor, and the counts differ by at most one.

    The other 35 candidates, mostly platform `i*` files, are listed in `split_units.tsv` as `weak` and stay in dtk's automatic objects.
7. **Checks.** The tool exits non-zero if any function loads a `.sdata2` item inside another unit's `.sdata2` range. MW pools literals per unit, so a cross-unit load means a wrong edge. A global `const` placed in `.sdata2` would also trip this check; none does today. As advice only, it also reports a unit range that holds the same float twice. Pooled literals are merged, but a named static `const` is not merged with a literal.

All five `.ctors` initialisers end up inside emitted units: zEntPlayer, zEntShaggy, zHud, zScene and zUI.

## Reproducing

The PS2 ELF is in the private build image at `/orig/SLUS-20349/SLUS_203.49`. After a dtk split (`ninja` or `dtk dol split`), run:

```sh
python3 tools/splits/recover.py --dol orig/GIHE78/sys/main.dol \
  --ps2-elf /orig/SLUS-20349/SLUS_203.49 \
  --splits units.txt --edges config/GIHE78/split_edges.tsv --units config/GIHE78/split_units.tsv
```

`units.txt` is the unit part of `splits.txt`. The relocations, and so the output, are the same whichever split produced the objects. Relocations to symbols that `symbols.txt` names are resolved through `symbols.txt`, so naming functions does not change the output.

## Limits

- A byte-identical relink only shows that dtk and mwld accept the cuts. It says nothing about where Heavy Iron's objects began and ended.
- 52 of the 76 text edges between emitted units are "alignment only": they have no data evidence beyond the size alignment.
- PS2 v1.01 is a sibling build, not the target. Platform `i*` units differ on GC and are mostly unsplit.
- The weak `iAnim.cpp` candidate at `0x80005AFC` is contradicted: BfBB's `iAnim.cpp` is GC platform code, and its `iAnimBlend` sits in the GC-only block at `0x800D7E00`.
- Anchors are literal matches, and a short function can be paired with the wrong PS2 function in the same unit. The iCamera anchor at `0x800072D4` is one: it is `iCameraSetNearFarClip`, not `iCameraCreate`, and its end edge rests on that pairing (see `evidence.tsv`).
- `zEnv.cpp` is misaligned. `zEnvRender` (an exact BfBB body, see `evidence.tsv`) is at `0x800AA058`, but the size alignment puts the unit at `0x800AA118`, so it stays `weak`, and the five functions from `0x800A9F68` stay unassigned. The functions before them, back to `0x800A9B8C`, reach only zEntVillain and have joined it.
- Header inline copies (`xMath3.h`, `xEnt.h`, `xModel.h`) are left to whichever unit holds them.
- This does not cover the GC-only code from `0x800D7E00` (it opens with `iAnimBlend`: an instruction match, with relocations masked, to BfBB's `gc/iAnim.cpp` under GC/1.3.2; see `evidence.tsv`), MetroTRK, the runtime, the Dolphin SDK or RenderWare.
