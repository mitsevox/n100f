# Game-unit splits

`config/GIHE78/splits.txt` names 121 game units (`flat/<file>`). They come from `tools/splits/recover.py`, which compares the target with the PS2 USA retail v1.01 executable SLUS-20349 (SHA-1 `ebe640193e0ceb38deca343a0b576cfcba2d5920`). That executable keeps its DWARF.

The names and extents are claims, not established facts. In `evidence.tsv`:
- each unit's extent is an `inferred` `unit` row with `provisional` or `open` edges;
- each source path is a separate `related` `unit-anchor` row, since it comes from another build's DWARF.

`config/GIHE78/split_units.tsv` lists all 134 candidate units, and `config/GIHE78/split_edges.tsv` gives every text and data edge with its class and the number of references that cross it.

## Method

1. **Anchors.** Anchors come in two kinds.
    - **Name anchors.** A function whose body `evidence.tsv` establishes exactly (a `function` row with `target`, `exact`, `exact`, such as an exact BfBB body) is pinned to the PS2 function of the same mangled name, when exactly one shared PS2 function has that name. The current run pins 186. Positional names are not used, because they were read off an earlier alignment and would only repeat it. A name anchor replaces any fingerprint anchor that shares either of its functions or crosses it in link order, and name anchors that break link order would be dropped (none do).
    - **Fingerprint anchors.** Every function on both platforms is fingerprinted by the string literals and float constants it loads and by the functions it calls. GC references come from dtk's relocations. PS2 references come from `lui`/`addiu`, `$gp` and `lui`+`mtc1` patterns, and calls from `jal`. A GC call counts only when its target is an exact-body function, named as above; the PS2 call is named by the DWARF. Unique, mutual-best matches become anchors. A match on calls alone needs two shared callees, since one shared callee fits any wrapper. The current run keeps 335 after the name anchors take precedence.
2. **Link order.** The GC game block, from `0x800055E0` to the start of GC-only code at `0x800D7E00`, is linked in case-insensitive alphabetical order of the DWARF compile-unit names under `E:\SD2\Game\flat\`. Most anchors lie in one increasing run under that order (280 of the 300 literal anchors of the first run). The PS2 ELF links in a different order, so its addresses are not a template.
3. **Alignment.** The PS2 functions, sorted by file name and then address, are aligned to the GC game functions by size, with the anchors pinned. A PS2 tail call (at most 16 bytes, one `j`) and a GC wrapper (`0x20` to `0x30` bytes, one `bl`) count as the same size, since the PS2 build compiles a call in tail position to a jump where the GC build keeps a stack frame and a `bl`. The pair is refused when both callees are named (the PS2 one by the DWARF, the GC one as an exact body) and the names differ.
4. **Text edges.** Up to eight functions on either side of an edge may move, but each unit lends only half its aligned functions to each of its edges. Each candidate cut is scored by how many references land on the wrong side of the best data cut in every section. Moving an anchor counts as three such references. A static initialiser named in `.ctors` must end its unit, because MW emits `__sinit` last.
    - One best cut: it is used, and its crossing count is recorded.
    - Several cuts tie on data: they are told apart by the calls they would separate. Calls into the window from functions of either unit outside it count first. Calls out of the window count only when the alignment's cut is not among the cuts still tied, because a wrapper that calls into the next unit would otherwise drag the cut onto the wrong side. Calls between two window functions do not count, since both move with the cut. One best cut on calls is used and labelled "single cut by call references". Otherwise only the cuts that also tie on calls go on to the two rules below.
    - Several cuts still tie, and the alignment's cut is among them: the alignment's cut is used and labelled "alignment only".
    - Several cuts still tie, and the alignment's cut is not among them: the tied functions stay in dtk's automatic objects, and both edges are `open`.

    An open edge then shrinks from both ends. The next unassigned function joins a neighbour when its references from code reach that neighbour and not the other one: calls to or from its functions (an address taken in code counts as a call), or loads of data that only its functions load. References from data, such as function pointers in tables, are not counted. Calls into other units, such as library code, do not count either way. A function that ends its unit with `__sinit` takes nothing after it. The edge class records how many functions joined, and an edge with none left reads "closed by references".
    A unit that is written (step 6) then takes any bordering function that no written unit holds, either unassigned or in a `weak` unit, when that function loads a `.sdata2` literal that, among the written units, only its functions load. MW pools literals per unit, so such a function cannot belong elsewhere. The edge class records "joined by .sdata2 literals". Two functions join this way: `0x8000B97C` (iFont.cpp, taken from weak iFX.cpp) and `0x80016A30` (xAnim.cpp, taken from weak ms4sound.cpp).
5. **Data edges.** Every data section follows the same unit order. Each unit's unit-local items (loaded only by that unit's functions) must therefore appear in link order. The tool keeps the largest set of them whose owners never go backwards, and an item outside that set marks no range. Such an item is usually a global that another unit defines and only one unit loads, like an `xMath3.cpp` vector constant. An item that several assigned units load is then placed with one of them when, of all its users, only that one fits between the units of the kept items around it in link order. This is a heuristic: it assumes the item is defined by one of the units that load it, which holds for a global its own unit uses but not for one only other units use. `globalCamera` at `0x8025E870` is the case that prompted this: iCamera.cpp and iEnv.cpp both load it, it sits between iAnim.cpp's and iCollide.cpp's `.sbss` items, and only iCamera.cpp fits there. The current run places 35 items this way. Between consecutive units that load unit-local items in a section, the cut falls on an 8-byte-aligned symbol boundary, at the candidate with the fewest crossing references. Items between two units that unassigned functions load belong to no unit, and the edge is `open`. An item inside a written range that an unassigned function loads stays in the range, but it counts as a crossing reference on the edge facing that function. This is the data edge in the same section if one exists, and the text edge otherwise. A range that cannot start on an aligned symbol is left to dtk.
6. **Emission.** A unit is written in either of two cases:
    - it has an anchor, and its GC function count is 0.5x to 2x its PS2 count;
    - it has no anchor, and the counts differ by at most one.

    The other 13 candidates (six platform `i*` files, ms4sound.cpp and six others) are listed in `split_units.tsv` as `weak` and stay in dtk's automatic objects.
7. **Checks.** The tool exits non-zero if any function loads a `.sdata2` item inside another unit's `.sdata2` range. MW pools literals per unit, so a cross-unit load means a wrong edge. A global `const` placed in `.sdata2` would also trip this check; none does today. As advice only, it also reports a unit range that holds the same float twice. Pooled literals are merged, but a named static `const` is not merged with a literal.

All five `.ctors` initialisers end up inside emitted units: zEntPlayer, zEntShaggy, zHud, zScene and zUI.

## Reproducing

The PS2 ELF is in the private build image at `/orig/SLUS-20349/SLUS_203.49`. After a dtk split (`ninja` or `dtk dol split`), run:

```sh
python3 tools/splits/recover.py --dol orig/GIHE78/sys/main.dol \
  --ps2-elf /orig/SLUS-20349/SLUS_203.49 \
  --splits units.txt --edges config/GIHE78/split_edges.tsv --units config/GIHE78/split_units.tsv
```

`units.txt` is the unit part of `splits.txt`. The relocations, and so the output, are the same whichever split produced the objects. Relocations to symbols that `symbols.txt` names are resolved through `symbols.txt`, so renaming functions does not change the output. Adding an exact-body `function` row to `evidence.tsv` can: it adds a name anchor and call features. Re-run the tool when one lands and commit what changes. Names are looked up in `symbols.txt` before dtk's address-suffixed labels. A name that `symbols.txt` gives to more than one symbol, such as two file-local statics, resolves in the object that defines it, to the one of its addresses that lies inside that object's range for the section in `splits.txt`. (dtk gives a static that another object references an address suffix, so only statics referenced from their own object reach this.) The tool stops when an object references such a name without defining it, or when not exactly one of its addresses lies inside the defining object's range.

## Limits

- A byte-identical relink only shows that dtk and mwld accept the cuts. It says nothing about where Heavy Iron's objects began and ended.
- 66 of the 109 text edges between emitted units are "alignment only": they have no data or call evidence beyond the size alignment, and 6 more rest on call references alone. zPortal.cpp and zRenderState.cpp have no anchor at all; they are written because their function counts match the PS2 units exactly once the tail calls pair.
- PS2 v1.01 is a sibling build, not the target. Platform `i*` units differ on GC and are mostly unsplit.
- The weak `iAnim.cpp` candidate at `0x80005AFC` is contradicted: BfBB's `iAnim.cpp` is GC platform code, and its `iAnimBlend` sits in the GC-only block at `0x800D7E00`.
- Fingerprint anchors are literal and call matches, and a short function can be paired with the wrong PS2 function in the same unit. The literal anchor at `0x800072D4` was one: it pairs with `iCameraCreate`, but the function is the exact `iCameraSetNearFarClip` body, so the name anchor now replaces it.
- zEnv.cpp starts at `0x800A9F78` on a fingerprint anchor: its calls to the exact-body `xClimateSetSnow` and `xClimateSetRain` match the PS2 `zEnvEventCB`. Its edge with zEntVillain.cpp is a single cut by call references, with no data evidence.
- Between iSystem.cpp and iTime.cpp, the eight functions from `0x80016600` to `0x80016900` stay unassigned. Neither data nor calls decide a side under the rules above. iSystem.cpp's `0x80016044` calls `0x800168F4`, but it lies outside the edge's alignment span, and counting it would move all eight into iSystem.cpp and put four more unit-local data items out of link order. `0x80016898` among them is the DOL entry routine.
- Between iParMgr.cpp and isavegame.cpp, three small functions (`0x80011B2C`, `0x80011B30`, `0x80011B34`) stay unassigned: nothing they reference or that references them decides a side. isavegame.cpp starts at `0x80011B74`, which only isavegame code calls.
- iModel.cpp runs to `0x8000EBC0` on call references, not on PS2 sizes alone: `0x8000E994` passes `0x8000EA5C` to `RpClumpForAllAtomics`, `0x8000EA5C` passes the GC-only static `0x8000EB64`, and `0x8000EB60` is the empty `iModelInit`, which xModel calls.
- Header inline copies (`xMath3.h`, `xEnt.h`, `xModel.h`) are left to whichever unit holds them.
- This does not cover the GC-only code from `0x800D7E00` (it opens with `iAnimBlend`: an instruction match, with relocations masked, to BfBB's `gc/iAnim.cpp` under GC/1.3.2; see `evidence.tsv`), MetroTRK, the runtime, the Dolphin SDK or RenderWare.
