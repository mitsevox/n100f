# Game-unit splits

`config/GIHE78/splits.txt` names 123 game units (`flat/<file>`). They come from `tools/splits/recover.py`, which compares the target with the PS2 USA retail v1.01 executable SLUS-20349 (SHA-1 `ebe640193e0ceb38deca343a0b576cfcba2d5920`). That executable keeps its DWARF.

The names and extents are claims, not established facts. In `evidence.tsv`:
- each unit's extent is an `inferred` `unit` row with `provisional` or `open` edges;
- each source path is a separate `related` `unit-anchor` row, since it comes from another build's DWARF.

`config/GIHE78/split_units.tsv` lists all 134 candidate units, and `config/GIHE78/split_edges.tsv` gives every text and data edge with its class and the number of references that cross it.

## CARD

The CARD library, `0x80130990` to `0x801372EC`, is the SDK's debug build: its 102 assertion calls to `OSPanic` (`0x80105548`) are compiled in, and its code compiles from ac-decomp's `src/static/dolphin/card` with `DEBUG=1` under GC/1.2.5n at `-opt level=0 -inline off -schedule off -sym on -char signed`, the flags of the SDK's debug libraries. `-sym on` changes the code: without it the same sources place 8 functions instead of 68, and under the release `sdk` profile they place 1. `-char signed` places one function more than `-char unsigned` (`__CARDCompareFileName`, in `CARDOpen.c`) and the same in every other object, so one signed profile covers the library. The `card_debug` profile holds these flags, with the rest of `configure.py`'s base flags so the two compile the same way, and the ac-decomp candidate that uses it carries its own `region`, so it places nothing outside CARD and the other candidates place nothing inside it. The library itself starts at `0x80130990`: the `blr` there is the empty `__CARDDefaultApiCallback`, the first function of `CARDBios.c`, which compiles to that one instruction directly before `__CARDSyncCallback`. libmap does not place it, because a lone `blr` matches anywhere; the `CARDBios.c` unit starts at `0x80130990`, where the imported object links whole.

The units follow the SDK's link order: CARDBios, CARDUnlock, CARDRdwr, CARDBlock, CARDDir, CARDCheck, CARDMount, CARDFormat, CARDOpen, CARDCreate, CARDRead, CARDWrite, CARDDelete, CARDStat, CARDStatEx and CARDNet. The linker removed functions the game never calls (`CARDWrite`, `CARDFastDelete`, ...), so an object matches whole only where nothing was removed. libmap places the rest only partially: ac-decomp's sources are an older SDK revision, and some assertion line numbers in `CARDBios.c`, `CARDMount.c`, `CARDFormat.c` and `CARDUnlock.c` differ from the target's. libmap names only `__CARDGetControlBlock` and `__CARDIsPublic`, because the map-backed symbol lists come from release builds, where the other functions have different sizes.

The import ([CARD](libraries.md#card)) links all 16 objects `Matching`, with the assertion lines taken from the target, so every edge is now the edge of a linked object. Its 99 functions are named from the imported source (`name` rows in `evidence.tsv`), and each object's `.data`, `.sdata` and `.bss` are mapped from the compiled object's relocations. `CARDDelete.c` and `CARDNet.c` have no `.data` in the target: their strings belong only to functions the linker removed.

The 14 KB after CARD, `0x801372EC` to `0x8013AA64`, is not CARD. It resembles RenderWare's RpCollision plugin as BfBB links it, without matching it in order: of the first 7 function sizes, 4 equal BfBB's `ctgeom.c` sizes in the same positions (0x12C, 0x1D8, 0x3A8, and 0xF8 for `RpAtomicForAllIntersections` at `0x8013880C`), and the longest run of equal sizes is 2. The RenderWare split settles it.

## Method

1. **Anchors.** Anchors come in two kinds.
    - **Name anchors.** A function whose body `evidence.tsv` establishes exactly (a `function` row with `target`, `exact`, `exact`, such as an exact BfBB body) is pinned to the PS2 function of the same mangled name, when exactly one shared PS2 function has that name. The current run pins 184. Positional names are not used, because they were read off an earlier alignment and would only repeat it. That includes an exact body written for this project rather than taken from a related build (its `function` row says so, as xUtil_yesno(v) and xUtilShutdown do): the body matches, but the name comes from position. A name anchor replaces any fingerprint anchor that shares either of its functions or crosses it in link order, and name anchors that break link order would be dropped (none do).
    - **Fingerprint anchors.** Every function on both platforms is fingerprinted by the string literals and float constants it loads and by the functions it calls. GC references come from dtk's relocations. PS2 references come from `lui`/`addiu`, `$gp` and `lui`+`mtc1` patterns, and calls from `jal`. A GC call counts only when its target is an exact-body function, named as above; the PS2 call is named by the DWARF. Unique, mutual-best matches become anchors. A match on calls alone needs two shared callees, since one shared callee fits any wrapper. The current run keeps 335 after the name anchors take precedence.
2. **Link order.** The GC game block, from `0x800055E0` to the start of GC-only code at `0x800D7E00`, is linked in case-insensitive alphabetical order of the DWARF compile-unit names under `E:\SD2\Game\flat\`. Most anchors lie in one increasing run under that order (280 of the 300 literal anchors of the first run). The PS2 ELF links in a different order, so its addresses are not a template.
3. **Alignment.** The PS2 functions, sorted by file name and then address, are aligned to the GC game functions by size, with the anchors pinned. A PS2 tail call (at most 16 bytes, one `j`) and a GC wrapper (`0x20` to `0x30` bytes, one `bl`) count as the same size, since the PS2 build compiles a call in tail position to a jump where the GC build keeps a stack frame and a `bl`. The alignment runs twice. The first pass refuses the pair when both callees are named (the PS2 one by the DWARF, the GC one as an exact body) and the names differ. The second pass also names each GC callee that is not an exact body by the PS2 function the first pass aligned it to, and refuses the pair when the callees differ in name or in unit. Without it, an unnamed GC callee matches any tail call, and wrappers in a run of them pair one function off. Of the 189 tail-call pairs of the first pass, 19 have a GC callee that the same pass aligned to a PS2 function other than the tail call's target. Of the 185 pairs of the second pass, measured against the second pass's own alignment, none do. A GC function skipped between two PS2 functions of the same unit costs more than one skipped at a unit edge, so GC-only code collects between units instead of splitting one. This is what separates xSurface.cpp (`0x80040EE4` to `0x80040FCC`) from the GC-only memory-card code of xTRC.cpp.
4. **Text edges.** Up to eight functions on either side of an edge may move, but each unit lends only half its aligned functions to each of its edges. Each candidate cut is scored by how many references land on the wrong side of the best data cut in every section. A section counts when the window functions load items no other unit loads: either items between the two units' own items, or items one of the two units already loads. In a section that counts only for the second reason, only such items are scored, since a global that many units load says nothing about this edge. This is what ties xSurfaceInit at `0x80040F58`, which uses `.sbss` `0x8025EBD0`, to xSurface.cpp. Moving an anchor counts as three such references. A static initialiser named in `.ctors` must end its unit, because MW emits `__sinit` last.
    - One best cut: it is used, and its crossing count is recorded.
    - Several cuts tie on data: they are told apart by the calls they would separate. Calls into the window from functions of either unit outside it count first. Calls out of the window count only when the alignment's cut is not among the cuts still tied, because a wrapper that calls into the next unit would otherwise drag the cut onto the wrong side. Calls between two window functions do not count, since both move with the cut. One best cut on calls is used and labelled "single cut by call references". Otherwise only the cuts that also tie on calls go on to the two rules below.
    - Several cuts still tie, and the alignment's cut is among them: the alignment's cut is used and labelled "alignment only", or "alignment confirmed by outgoing calls" when calls out of the window single it out among the ties.
    - Several cuts still tie, and the alignment's cut is not among them: the tied functions stay in dtk's automatic objects, and both edges are `open`.

    An open edge then shrinks from both ends. The next unassigned function joins a neighbour when its references from code reach that neighbour and not the other one: calls to or from its functions (an address taken in code counts as a call), or loads of data that only its functions load. References from data, such as function pointers in tables, are not counted. Calls into other units, such as library code, do not count either way. The callee of a GC wrapper that the alignment pairs with a PS2 tail call reaches the unit of the PS2 callee: `0x80011B2C` joins iParMgr.cpp as `iParMgrRender`, and `0x80009CC8` joins iDraw.cpp as `iDrawBegin`. A function that ends its unit with `__sinit` takes nothing after it. The edge class records how many functions joined, and an edge with none left reads "closed by references".
    A unit that is written (step 6) then takes any bordering function that no written unit holds, either unassigned or in a `weak` unit, when that function loads a `.sdata2` literal that, among the written units, only its functions load. MW pools literals per unit, so such a function cannot belong elsewhere. The edge class records "joined by .sdata2 literals". Two functions join this way: `0x8000B97C` (iFont.cpp, taken from weak iFX.cpp) and `0x80016A30` (xAnim.cpp, taken from weak ms4sound.cpp).
5. **Data edges.** Every data section follows the same unit order. Each unit's unit-local items (loaded only by that unit's functions) must therefore appear in link order. The tool keeps the largest set of them whose owners never go backwards, and an item outside that set marks no range. Such an item is usually a global that another unit defines and only one unit loads, like an `xMath3.cpp` vector constant. An item that several assigned units load is then placed with one of them when, of all its users, only that one fits between the units of the kept items around it in link order. This is a heuristic: it assumes the item is defined by one of the units that load it, which holds for a global its own unit uses but not for one only other units use. `globalCamera` at `0x8025E870` is the case that prompted this: iCamera.cpp and iEnv.cpp both load it, it sits between iAnim.cpp's and iCollide.cpp's `.sbss` items, and only iCamera.cpp fits there. The current run places 36 items this way. Between consecutive units that load unit-local items in a section, the cut falls on an 8-byte-aligned symbol boundary, at the candidate with the fewest crossing references. Items between two units that unassigned functions load belong to no unit, and the edge is `open`. An item inside a written range that an unassigned function loads stays in the range, but it counts as a crossing reference on the edge facing that function. This is the data edge in the same section if one exists, and the text edge otherwise. A range that cannot start on an aligned symbol is left to dtk.
6. **Emission.** A unit is written in either of two cases:
    - it has an anchor, and its GC function count is 0.5x to 2x its PS2 count;
    - it has no anchor, and the counts differ by at most one.

    The other 11 candidates (five platform `i*` files, ms4sound.cpp, xDebug.cpp, xDraw.cpp, xSnd.cpp, xTRC.cpp and zEntSimpleObj.cpp) are listed in `split_units.tsv` as `weak` and stay in dtk's automatic objects.
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
- 50 of the 113 text edges between emitted units are "alignment only": they have no data or call evidence beyond the size alignment. 8 more are confirmed only by outgoing calls, and 7 rest on call references alone.
- iDraw.cpp is written on 3 of its 4 PS2 functions, `0x80009CC0` to `0x80009CCC`, all empty stubs. One of them is placed only because `xDrawBegin` calls it where PS2 tail-calls `iDrawBegin`. zPortal.cpp and zRenderState.cpp have no anchor at all; they are written because their function counts match the PS2 units exactly once the tail calls pair.
- PS2 v1.01 is a sibling build, not the target. Platform `i*` units differ on GC and are mostly unsplit.
- The weak `iAnim.cpp` candidate at `0x80005AFC` is contradicted: BfBB's `iAnim.cpp` is GC platform code, and its `iAnimBlend` sits in the GC-only block at `0x800D7E00`.
- Fingerprint anchors are literal and call matches, and a short function can be paired with the wrong PS2 function in the same unit. The literal anchor at `0x800072D4` was one: it pairs with `iCameraCreate`, but the function is the exact `iCameraSetNearFarClip` body, so the name anchor now replaces it.
- zEnv.cpp starts at `0x800A9F78` on a fingerprint anchor: its calls to the exact-body `xClimateSetSnow` and `xClimateSetRain` match the PS2 `zEnvEventCB`. Its edge with zEntVillain.cpp is a single cut by call references, with no data evidence.
- Between iSystem.cpp and iTime.cpp, the eight functions from `0x80016600` to `0x80016900` stay unassigned. Neither data nor calls decide a side under the rules above. iSystem.cpp's `0x80016044` calls `0x800168F4`, but it lies outside the edge's alignment span, and counting it would move all eight into iSystem.cpp and put four more unit-local data items out of link order. `0x80016898` among them is the DOL entry routine.
- Between iParMgr.cpp and isavegame.cpp, `0x80011B30` and `0x80011B34` stay unassigned, although xParMgr.cpp calls them where PS2 calls `iParMgrUpdate` and `iParMgrInit`. Neither PS2 caller is a tail call (`xParMgrUpdate` takes a float, `xParMgrInit` makes two calls), so the tail-call rule does not reach them. isavegame.cpp starts at `0x80011B74`, which only isavegame code calls.
- xTRC.cpp (`0x80040FCC` to `0x80044780`) is weak: 30 GC functions against 2 on PS2, because the GC build adds memory-card TRC code that calls `iSG*`. Its extent is in `split_units.tsv`, but it is not written, since the 0.5x to 2x rule cannot tell this from a misalignment. Earlier runs wrote a one-function xTRC.cpp at `0x80044714` (`xTRCInit`), whose start had been pushed past the xTRCPad anchor at `0x80044478` by the weak xSurface.cpp in front of it. That unit is dropped rather than kept as a fragment of a file whose real start is `0x80040FCC`.
- xCutscene.cpp has no aligned function since the tail-call rule, so `split_units.tsv` lists 134 candidates, not 135. Between xDebug.cpp and xDraw.cpp, neither of them written, `0x80021CB0` aligns with xDebug.cpp but shares `.sbss` `0x8025E410` with xDraw.cpp.
- xMath3.cpp is a partial unit in `configure.py`. The tail-call rule gave it the wrappers `xMath3Exit` and `xMath3Init` (`0x8002F8E0`, `0x8002F900`), which count as unmatched, so the game total went from 267 to 269 functions with 77 still exact.
- iModel.cpp runs to `0x8000EBC0` on call references, not on PS2 sizes alone: `0x8000E994` passes `0x8000EA5C` to `RpClumpForAllAtomics`, `0x8000EA5C` passes the GC-only static `0x8000EB64`, and `0x8000EB60` is the empty `iModelInit`, which xModel calls.
- Header inline copies (`xMath3.h`, `xEnt.h`, `xModel.h`) are left to whichever unit holds them.
- This does not cover the GC-only code from `0x800D7E00` (it opens with `iAnimBlend`: an instruction match, with relocations masked, to BfBB's `gc/iAnim.cpp` under GC/1.3.2; see `evidence.tsv`). MetroTRK, the runtime, MSL, the first part of the Dolphin SDK and the CARD library are mapped separately, under [Library units](#library-units), Bink under [Bink units](#bink-units) and RenderWare under [RenderWare units](#renderware-units).

# Library units

`splits.txt` also names 141 library units after the game units: 125 from `0x800DE530` to `0x80111EC0`, and the CARD library's 16 from `0x80130990` to `0x801372EC`, after Bink. 136 of them come from `tools/splits/libmap.py`, which compiles public decompilations of the same libraries and looks for their code in the target. The table counts libmap's placements, as `config/GIHE78/lib_units.tsv` records them; import corrections are described after it.

| Library | Units | Whole-object matches | Functions placed |
| --- | --- | --- | --- |
| `TRK_MINNOW_DOLPHIN` (MetroTRK v0.10) | 21 | 14 | 119 |
| `Runtime.PPCEABI.H` | 3 | 3 | 16 |
| `MSL_C.PPCEABI.bare.H` | 35 | 27 | 110 |
| `dolphin/<library>` (SDK, OS built Apr 16 2002) | 61 | 22 | 515 |
| `dolphin/card` (the SDK's debug build) | 16 | 5 | 68 |

Each unit stays in the original bytes until its source lands and passes review; [libraries.md](libraries.md#imported-units) lists the imported ones. `config/GIHE78/lib_units.tsv` lists each unit as libmap placed it, with its match kind, function counts, chosen source and the compiler versions that produced it.

Imports correct the map where compiled source shows more than libmap could place. Functions of 8 to 16 bytes are too common in the target to place by body, so they were left out of the first map. The MetroTRK import added a unit, `mutex_TRK.c`, at `0x800E2BEC` (three 8-byte functions between `support.c` and `notify.c`, the order the related maps give). It extended `msghndlr.c` to `0x800E232C` to take `SetTRKConnected` and `GetTRKConnected`, and gave `mem_TRK.c` its `.init` functions `TRK_memset` and `TRK_memcpy` at `0x80003100`. It also split `targsupp.s` (four 8-byte trap stubs at `0x800E4660`) off the end of `targimpl.c`, started `targimpl.c` at `0x800E2DD0` to take `__TRK_get_MSR` and `__TRK_set_MSR`, extended `dolphin_trk.c` to `0x800E47C0` to take `EnableMetroTRKInterrupts`, and gave `dolphin_trk.c` its `.init` function `__TRK_reset` at `0x80005088`. That places 16 more TRK functions than the table shows. The MSL import extended `string.c` to `0x800EE7BC` to take `strlen` (a 0x1C-byte body that occurs once in the target) and named 15 of the matched functions that had kept `fn_` labels, from the imported sources, with `inferred` or `related` rows. The os import added three SDK units, each its own object in Animal Crossing's map: `OSArena.c` at `0x8010469C` (the four arena accessors, between `OSAlloc.c` and `OSAudioSystem.c`), `OSFont.c` at `0x80105890` (`OSGetFontEncode`, split from the front of `OSInterrupt.c`) and `OSMutex.c` at `0x80106448` (`__OSUnlockAllMutex`, whose final `blr` dtk had split off as a separate 4-byte function).

## Method

1. **Candidates.** `config/GIHE78/libmap.json` lists each upstream with its URL, pinned commit and licence, the compiler profiles (`sdk`: GC/1.2.5n; `runtime`, `msl` and `trk`: GC/1.3 and GC/1.3.2), and the source globs to try, less any `exclude` list. Each MSL upstream is listed twice, under `runtime` and under `msl` (the library's own flags: deferred inlining and signed `char`), because some upstreams write their files in the target's order and others in Metrowerks' source order, which deferred inlining reverses. sonicheroes' `mem.c` is excluded: it joins what the Wind Waker map shows as two objects, `mem.c` and `mem_funcs.c`. The tool checks that every checkout is at its pinned commit.
2. **Probe.** Each source file is compiled under its profile with every listed version. In each function, the fields a relocation fills are masked. A function places the object when its masked body occurs exactly once in `main.dol`, at a function start, at full size.
3. **Placement.** The whole object `.text` is first compared in place. If it matches, the unit's extent and every function in it are exact. Otherwise the unit keeps the longest run of unique hits that is in order both in the object and in the target, with gaps no wider than twice the object distance plus `0x400`. For each object path, the best placement wins: a whole-object match first, then the most matched bytes, then manifest order. Placements may not overlap.
4. **Extents.** A whole-object match fixes dtk's function boundaries: inner functions dtk invented are removed, an overlapping predecessor is cut short, functions dtk missed are added, and sizes are corrected. The tool prints a `note:` for each change.
5. **Names.** A matched function takes its upstream name only when that name labels a function of the same size in a symbol list the manifest marks as map-backed. Those lists are Animal Crossing (GAFE01, from its `static.map`), The Wind Waker (GZLE01, from the debug maps) and Final Fantasy Crystal Chronicles (GCCE01, from its debug symbols). Other matched functions keep their `fn_` labels: 47 today, 37 whose upstream name is in none of the three lists and 10 whose name is there only at another size (`fopen`, `setvbuf`, ...). A function that is static in the compiled object gets `scope:local`.
6. **Paths.** Runtime files go under `Runtime.PPCEABI.H/`, MSL under `MSL_C.PPCEABI.bare.H/`, MetroTRK under `TRK_MINNOW_DOLPHIN/`, and SDK files under `dolphin/<library>/`, with the library taken from ac-decomp's directory layout.

In `evidence.tsv`, each unit has a `target` `unit` row whose edges are `exact` where the next unit abuts it and `open` otherwise. Each placed function has a `target` `function` row, and each name libmap applies has a `related` `name` row that cites the symbol lists. Three `compiler` rows record the per-object versions: MetroTRK is GC/1.3, MSL is GC/1.3 with deferred inlining and signed char (the `msl` profile), and `__va_arg.c` is GC/1.3.2 with the runtime profile. The `linker` row records the one linker-script change: `config/GIHE78/ldscript.tpl` is dtk's default template with `_db_stack_addr` at `_stack_addr + 0x1000`, which the relink needs once `InitMetroTRK` and `OSInit` are named.

7. **dtk signatures.** The new names also switch on dtk's built-in signatures (`src/analysis/signatures.rs`), which dtk applies at the address of the symbol each one is keyed on. A matching signature names its function and every symbol its relocations reach, and a split writes those names back to `symbols.txt`. This adds 134 names: 26 functions, 9 `.text` labels and 99 data objects, among them `OSReport` at `0x80015F00`, an empty 0x50-byte variadic body inside the game block whose original unit is unknown. `tools/splits/sigrows.py` re-checks the same signatures against `main.dol` and writes a `signature` `name` row, and a `function` row for functions, for every name in `symbols.txt` that has no `name` row. Each row cites the dtk version, the signature file and the entry hash. A function's end is `exact` only when the signature itself covers the body; otherwise it is dtk's analysis and `provisional`. The same pass covers 11 older dtk signature names from the baseline that had no rows (`GXInit`, `gx`, `gxData`, `PPCMfhid2`, ...).

## Reproducing

The tool needs local checkouts of the upstreams at their pinned commits and the compilers from `configure.py`. Run it against the committed `symbols.txt`:

```sh
python3 tools/splits/libmap.py --dol orig/GIHE78/sys/main.dol \
  --compilers build/compilers --wrapper build/tools/wibo \
  --checkout ac-decomp=../ac-decomp --checkout tww=../tww ...one per upstream... \
  --splits libunits.txt --units config/GIHE78/lib_units.tsv \
  --evidence librows.tsv --symbols-out config/GIHE78/symbols.txt
```

On the committed files this is a fixed point up to the import corrections above (and the unit and function rows that imports rewrite or replace): `libunits.txt` equals the libmap units in `splits.txt` (the Bink units sit between the SDK units and CARD), `librows.tsv` equals the tool's rows in `evidence.tsv`, and `symbols.txt` comes back unchanged. Then check the signature rows with a decomp-toolkit checkout at the pinned version:

```sh
python3 tools/splits/sigrows.py --dtk-src ../decomp-toolkit --dol orig/GIHE78/sys/main.dol \
  --symbols config/GIHE78/symbols.txt --evidence config/GIHE78/evidence.tsv --output sigrows.tsv
```

On the committed files it writes no rows. From the baseline, the sequence is: run libmap against the baseline `symbols.txt`, make the manual changes below, run `dtk dol split` twice so the signature names settle in `symbols.txt` (`__OSArenaHi` only appears on the second pass), run libmap again, then append the libmap and sigrows rows. The first run needed three manual changes, which are now in `symbols.txt`. `fn_800E2E1C` was split at `0x800E2FB0`, because dtk's `TRKInterruptHandler` signature relocates to `TRKExceptionHandler` there. And five data labels (`lbl_801CA2B8`, `lbl_801CA308`, `lbl_801CA358`, `lbl_8025F484`, `lbl_8025F574`) were removed, because they overlapped the `__files`, `__OSLastInterruptTime` and `LastResetEnd` objects that dtk's signatures create.

## Limits

- Data sections are split only where an import has reproduced them. Each import PR maps its unit's data when the source compiles.
- An `open` edge only bounds what matched. The real object can extend past it, and 65 of libmap's 136 units are partial placements.
- Library units count as named files in the public report. Function bytes are unchanged; named functions rose from 461 to 1,148, and to 1,150 with CARD (the Bink names below add 336).
- Upstream source is evidence that the code exists, not that its source meets this project's standards. Every function copied in a later PR is reviewed like any other.
- The SDK units carry `comment:8` in `splits.txt`, as GC/1.2.5n writes; see [libraries.md](libraries.md).

# Bink units

`splits.txt` names 15 Bink units from `0x80111EC0` to `0x80130990`, after the last SDK part 1 unit (`dolphin/dvd/fstload.c`, whose whole `.text` matches up to `0x80111EC0`).

BfBB links the same Bink build. Its GQPE78 Bink block (`0x80196880` to `0x801B5350` in bfbb `d8e8eac` `config/GQPE78/symbols.txt`) has the same size, `0x1EAD0`, and all 336 of its functions sit at the same offsets with the same sizes as GIHE78's. So each bfbb Bink unit and function name carries over at an offset of `0x849C0`.

| Unit | Start | End | Functions |
| --- | --- | --- | --- |
| `bink/src/sdk/decode/ngc/binkngc.c` | `0x80111EC0` | `0x801123B4` | 16 |
| `bink/src/sdk/decode/ngc/ngcsnd.c` | `0x801123B4` | `0x80113890` | 29 |
| `bink/src/sdk/decode/binkread.c` | `0x80113890` | `0x801179EC` | 53 |
| `bink/src/sdk/decode/ngc/ngcfile.c` | `0x801179EC` | `0x801184CC` | 13 |
| `bink/src/sdk/decode/yuv.cpp` | `0x801184CC` | `0x8011F78C` | 97 |
| `bink/src/sdk/decode/binkacd.c` | `0x8011F78C` | `0x80120470` | 8 |
| `bink/shared/time/radcb.c` | `0x80120470` | `0x80120B54` | 20 |
| `bink/src/sdk/decode/expand.c` | `0x80120B54` | `0x801241E0` | 16 |
| `bink/src/sdk/popmal.c` | `0x801241E0` | `0x80124378` | 3 |
| `bink/src/sdk/decode/ngc/ngcrgb.c` | `0x80124378` | `0x801272BC` | 25 |
| `bink/src/sdk/decode/ngc/ngcyuy2.c` | `0x801272BC` | `0x80127FC8` | 8 |
| `bink/src/sdk/varbits.c` | `0x80127FC8` | `0x801281AC` | 1 |
| `bink/src/sdk/fft.c` | `0x801281AC` | `0x8012C490` | 32 |
| `bink/src/sdk/dct.c` | `0x8012C490` | `0x8012D564` | 8 |
| `bink/src/sdk/bitplane.c` | `0x8012D564` | `0x80130990` | 7 |

bfbb does not say where its Bink paths, boundaries and names come from, so they are `related` and their inner edges start `provisional`. An edge becomes `exact` when an import reproduces the unit on either side of it, as `fft.c` has done for the `varbits.c` end, both `fft.c` edges and the `dct.c` start. Its list includes static functions and a local `gcc2_compiled.` label, the marker GCC 2 emits at the start of each object's `.text`, at each of the 15 unit starts, which suggests a symbol table from RAD's library rather than names chosen by hand. Two checks support them:

- None of the 190 functions bfbb marks `scope:local` is called from outside its unit (455 direct calls to Bink functions checked in the whole of `.text`).
- Game code calls only Bink's public API: `BinkOpen`, `BinkClose`, `BinkDoFrame`, `BinkNextFrame`, `BinkWait`, `BinkGoto`, `BinkCopyToBuffer`, `BinkSetVolume`, `BinkGetError`, `RADSetMemory` and `RADSetAudioMemory`.

In `evidence.tsv` each unit has a `unit` row, each function a `function` row and a `related` `name` row. The `unit` and `function` rows are `related` until an import reproduces the unit, and `target` after (so far `fft.c`, `radcb.c`, `popmal.c` and `varbits.c`). A `compiler` row records that the block is ProDG code. A unit's data sections are split when its import reproduces them; so far those are `fft.c`'s, `varbits.c`'s and `dct.c`'s `.rodata` and `popmal.c`'s `.bss` and `.sdata`.

# RenderWare units

`splits.txt` also names 63 RenderWare units between `0x801372EC` and `0x8018E5C0`, from `tools/splits/rwmap.py`. GIHE78 links RenderWare 3.3.0.2: `RwEngineGetVersion` (`0x80169F10`) returns `0x33002`. There is no public RenderWare source to compile, so these units and names come from BfBB's RenderWare 3.5 splits and symbols (`bfbbdecomp/bfbb` `d8e8eac`, GQPE78), placed by function size sequence. The input is `config/GIHE78/rwmap.json` and the report is `config/GIHE78/rw_units.tsv`.

| Library | Units |
| --- | --- |
| `rpcollis` | 3 |
| `rphanim` | 1 |
| `rpmatfx` | 5 |
| `rpskin` | 1 |
| `rpusrdat` | 1 |
| `rpworld` | 15 |
| `rwcore` | 37 |

The units cover 661 of the region's 1,006 functions and 0x2D1C0 of its 0x572D4 bytes. 591 of those functions are named: 528 by equal size, 58 by position and 5 that already had names from game-code callers (`RpWorldDestroy`, `RpWorldAddCamera`, `RpWorldRemoveCamera`, `RwCameraShowRaster`, `_rwFrameSyncDirty`), all of which the alignment reproduces.

## Method

1. **Placement.** For each BfBB unit under `rwsdk/`, the sizes of its functions, in address order, are aligned locally against the GIHE78 functions of the region. GIHE78 is an older RenderWare and its linker strips unreferenced functions, so a unit is a subsequence of BfBB's with some sizes changed. An equal size scores `3 + min(size, 0x320) / 0xC8`, a size within 8 bytes or 6% scores 0.3, any other pair −2.5 and a gap −1. A placement is trimmed to its outermost equal sizes and needs a score of 6, and either two equal sizes or one of at least 0x200 bytes.
2. **Order.** The tool keeps the highest-scoring chain of non-overlapping placements that keeps library order (`rpcollis`, `rphanim`, `rpmatfx`, `rpptank`, `rpskin`, `rpusrdat`, `rpworld`, `rtanim`, `rtintsec`, `rtslerp`, `rwcore`, as BfBB links them). In `rpworld`, the `rt*` tools and `rwcore`, units must also keep BfBB's order. Plugin objects may appear in any order within their library: GIHE78 links `rpmatfx.c` before the GameCube matfx objects.
3. **Names.** An aligned pair of equal size takes BfBB's name when it is at least 0x20 bytes, or when both of its GIHE78 neighbours are equal-size pairs of the same unit. Between two such names, functions take BfBB's names by position when both sides hold the same number of functions and each GIHE78/BfBB size ratio is between 0.75 and 1.33. A name BfBB marks static is dropped if a function outside the unit calls it directly (any `bl` in `main.dol`'s text; calls through pointers are not checked). A unit needs at least two names; six single-name placements (`multiTexGcnPipe.c`, `skingcng.c`, `nodeGameCubeAtomicAllInOne.c`, `nodeGameCubeWorldSectorAllInOne.c`, `geomcond.c`, `rtslerp.c`) and one with none (`nodeDolphinSubmitNoLight.c`) are left out.
4. **Extents.** A unit runs from its first to its last named function. It widens to take in any function that calls one of its static names, since a static is only reachable from its own object. An edge is `provisional` where the neighbouring unit's named function abuts it and `open` otherwise. Functions between units stay unassigned.

Each unit has a `related` `unit` row in `evidence.tsv`. Each new name has a `related` `name` row, which states whether it was named by equal size or by position, and a `function` row whose edges are dtk's inference (`provisional`). The rows also cite corroboration that the tool does not use for placement:

- The PS2 June 2001 prototype symbol table (`references/.../PROTO-PS2-2001-06-11`), an RW 3.x build of this game, lists 374 of the new names.
- The PS2 retail DWARF lists 22 of the new names, the `ctgeom.c`, `ctbsp.c` and `rpcollis.c` functions, in `flat\p2rwcoll\ctgeom_.c`, `ctbsp_.c` and `rpcollis_.c`: Heavy Iron compiled the collision plugin into the PS2 game.
- The PS2 January 2001 and Xbox prototypes keep RenderWare source paths as strings (`rwsdk/world/baclump.c`, `//RenderWare/RW34Active/rwsdk/src/plcore/baimmedi.c#2`, ...). A unit row says when its path is among them.

BfBB's RenderWare names and paths are a related build's; there is no GC debug symbol. bfbb does not state where they come from. 186 of the new names are functions bfbb marks static, whose names public RenderWare headers cannot supply, and 206 rest on bfbb alone: neither the PS2 prototype symbol table nor the PS2 retail DWARF lists them. No leaked RenderWare source was used, and no Criterion RenderWare source or SDK headers.

## Reproducing

The tool needs a checkout of `bfbbdecomp/bfbb` at the pinned commit; only `config/GQPE78/` is read.

```sh
python3 tools/splits/rwmap.py --dol orig/GIHE78/sys/main.dol --checkout ../bfbb \
  --splits rwunits.txt --units config/GIHE78/rw_units.tsv \
  --evidence rwrows.tsv --symbols-out config/GIHE78/symbols.txt
```

On the committed files this is a fixed point: `rwunits.txt` equals the RenderWare part of `splits.txt`, `rwrows.tsv` equals the tool's rows in `evidence.tsv`, and `symbols.txt` comes back unchanged. Names that other rows already cover are skipped, with a note saying whether they agree.

## Limits

- Placement rests on sizes. Equal sizes in BfBB order are strong evidence, but a positional name could swap with a neighbour of similar size; those rows say so.
- Scores near the threshold are weak. The best placement the chain rejects scores 14.0 (`vtxfmt.c` at `0x80158DC4`, inside the chosen `gcpipe.c`; `vtxfmt.c` itself is placed at `0x8015D4E0`), and ten chosen units score 14.0 or less (`rw_units.tsv` "score"). Their names rest on the library order as much as on their sizes.
- The compiler profile of the RenderWare objects is not established, so `configure.py` does not list them, and data sections are not split.
- 345 functions stay unassigned. The largest gaps are `0x8017EDA0`–`0x801869A8` (31 KB after `bapipe.c`, where nothing in BfBB lines up), `0x801542DC`–`0x8015885C` and `0x80159374`–`0x8015D4E0` (world pipeline objects), and the 3.3 animation code around `rphanim.c`, which differs from 3.5.
- `0x801372EC`, where CARD ends, is where the search starts, not a mapped edge. `ctgeom.c` probably starts there: `fn_801372EC` (0x3D8 bytes) is the line-leaf callback whose address `GeometryForAllLineIntersections` loads (`0x8013791C`), the role of BfBB's `GeomLeafNodeForAllLineIntersections` (0x374 bytes), but no equal-size name precedes it, so the unit starts at `0x801376C4`. `0x80138904`–`0x8013AA64` is probably `ctworld.c`: its last function calls the first, as `RpCollisionWorldForAllIntersections` calls `WorldForAllLineWorldSectorIntersections`. Only one of its ten sizes equals BfBB's, so it is not placed.
