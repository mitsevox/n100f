# The shared-site report

n100f's progress is published on the shared site, https://mitsevox.github.io/, through the site's
report-v1 contract (in the site repo `mitsevox/mitsevox.github.io`, see
`docs/reporting/platform.md` and `contract-v1.md`; also the handbook's
[Website chapter](https://mitsevox.github.io/wiki/website-integration/)). The format follows
mvp2005, which uses the same dtk/objdiff pipeline. The game repository never deploys the site.

## How a report is made

`.github/workflows/validate.yml` runs on every PR and `main` push:

1. **Build GIHE78**: a fresh build in the pinned n100f-build image, with no build cache.
   `ninja progress` depends on the `main.dol` hash check, so a non-identical build fails here.
   The measurement time is recorded right after `ninja`. The job uploads `GIHE78_report`.
2. **Site report** (`needs: build`), in order:
   - *Download verified progress report*: that run's `GIHE78_report`.
   - *Export public report inputs*: `tools/public_report.py inputs` turns objdiff's
     `report.json` into `build/public-input/summary.json`, `map.json` and `timeline.json`.
     `tools/public_report.py metadata` writes the execution facts: commit, tree, the workflow
     file's Git blob, the pinned image and tools, run and attempt, and the measurement and
     publication times.
   - *Package and validate public report*: the site's own `tools/package_report.py`, checked
     out at a pinned site revision, builds `manifest.json` and `receipt.json` and validates the
     package against `config/GIHE78/producer.json`.
   - *Upload public report*: artifact `public-report`, public JSON only.

PR runs make the same artifact as evidence only. The site imports only successful `push` or
`workflow_dispatch` runs on `main`. It authenticates the run, job, steps, workflow blob, tree and
artifact against its copy of the producer policy, and never runs anything from the package.

## What the numbers mean

| Field | From report.json |
|---|---|
| code / data `totalBytes` | `total_code` / `total_data` |
| `linkedBytes` | `complete_code` / `complete_data` (units linked from our source) |
| `matchedBytes` | `matched_code` / `matched_data` (bytes objdiff compares equal) |
| code `fuzzyEquivalentBytes` | sum over functions of size × fuzzy % (an exact function counts its whole size) |
| data `fuzzyEquivalentBytes` | sum over data and bss sections of size × that section's `fuzzy_match_percent` (objdiff byte-diffs each data section) |
| `functions.total` / `exact` | every function in the report / those at 100% |
| `functions.named` | functions whose symbol is not an `fn_` address label |
| `files.total` | every unit in the report, dtk's auto-generated address ranges included |
| `files.named` | units that are not auto-generated ranges |
| `files.exact` | named units whose code and data bytes all match |

The file counts are provisional. 123 game units are named from recovered splits (see
[splits.md](splits.md)), and 145 library units (MetroTRK, runtime, MSL, the first part of the
Dolphin SDK and CARD) are named by compiling public decompilations of those libraries (see
[Library units](splits.md#library-units)), 15 Bink units are named from bfbb's identical Bink
layout (see [Bink units](splits.md#bink-units)), and 89 RenderWare units are placed, 71 by aligning bfbb's
RenderWare function sizes and 18 pipeline node objects from the game's own node definitions (see [RenderWare units](splits.md#renderware-units)); the rest are dtk's auto-generated address ranges.
Today 374 of the report's 519 files are named (the si, pad, vi and dvd import absorbed seven of dtk's auto-generated ranges: six data ranges, now mapped to those units' data sections, and the `.text` gap `auto_01_8010F20C_text`, now the end of `dvdfs.c`; it left one alignment gap, `auto_06_8025C108_bss`, between `dvdlow.c` and `dvd.c`. The exi, ai, ar, base and dsp import added two units, `amcstubs/AmcExi2Stubs.c` and `dsp/dsp_debug.c`, and absorbed nine more: six `.text` gaps around `ai.c`, `ar.c`, `PPCArch.c`, `dsp.c` and `EXIBios.c`, `dsp.c`'s `.data` and two `.sbss` ranges. Two ranges moved: `auto_01_800F5C78_text` now starts at `0x800F5DD0`, after `arq.c`, and `auto_08_8025F260_sbss` at `0x8025F2F0`. The AX import added two units, `ax/AX.c` and `ax/AXAux.c`, and absorbed eight more: the five `.text` gaps from `auto_01_800F5DD0_text` to `auto_01_800F7624_text` between the AX units, `AXVPB.c`'s `.bss` (`auto_06_8024A840_bss`) and two `.sbss` ranges (`auto_08_8025F2F0_sbss` and `auto_08_8025F380_sbss`). Three ranges now end earlier: `auto_06_802428D0_bss` at `0x80242BD0`, before `AXAlloc.c`, and `auto_09_802621A8_sdata2` at `0x80262680`, before `AXVPB.c`, and `auto_05_801CABD0_data` now starts at `0x801CAC80`, the unsplit `axDspSlave`, as `auto_05_801CAC80_data`). The game
names come from the PS2 v1.01 DWARF and the boundaries are inferred, so no game unit boundary has been
checked against Heavy Iron's objects. The basis string says so. Library units placed by a
whole-object match have exact extents; the others have open edges. RenderWare unit edges are `provisional` or `open`. Naming those units did not change
what `files.named` means, so the methodology and generation stay the same.
Today's 2,607 named functions are 655 from before the library map, 864 library names, 336 Bink names, 105 names from the CARD import and 647 RenderWare names. The 655
are the names dtk's analysis assigned in `symbols.txt` (MW runtime and some GX SDK functions, plus
`__fill_mem`, which dtk names once `memset` is named), `__start`, the seven names the xMorph unit
added (its three functions and their four callees), 334 SLUS-20349 DWARF names applied from
related-build evidence (168 by an exact bfbb body match and 166 by positions in units whose function
count and pins agree with the DWARF), and 266 names added with the partial units: their callees, the
zCounter and zFog functions (by bfbb body and DWARF position), the xSerial Read/Write and
xSGReadData/xSGWriteData overload groups (by matched caller, body shape and DWARF order), the xbinio,
xhipio and xpkrsvc functions reached through their function tables (by table slot and DWARF order),
the iMath functions and two xutil functions (by DWARF position), the five names the zLaugh unit
added (its three functions and their callees `xSndPlay` and `iSndLookup`, by DWARF order and call site), the 22 names the zEntButton unit added (its 12 functions by DWARF order and 10 callees by call site), the 36 names the zEntShaggy Shaggy0 work added (its 11 Shaggy0 functions by DWARF order, 19 Shaggy1 callbacks by their slots in the Shaggy0 animation-table transitions, and 6 callees by call site), the 27 names the Shaggy1 callback work added (23 more Shaggy1 callbacks and `zEntShaggy_GetSwap` by DWARF order, and their callees `xAngleClamp`, `xMathSolveQuadratic` and `zEntHangable_SetShaggy` by call site), the 5 names the Shaggy1 table-builder work added (`Shaggy1_PlayerStates`, `zEntShaggy_Anim1` and `Shaggy1_PlayerTransitions` by DWARF order, and `Shaggy5_Action` and `Shaggy5_ActionCB` by their slot in the Shaggy1 animation table), the 50 names the Shaggy2, Shaggy3, Shaggy5 and Shaggy8 work added (45 functions by DWARF order and by their slots in the animation tables, the unit's static initializer `__sinit_zEntShaggy_cpp`, and the callees `zPlatformEventCB`, `xEntIsVisible`, `xModelRender` and `xAnimStateNewEffect` by call site), and the 37 names the Shaggy1 core and update/event-callback work added (21 functions by DWARF order, and 16 callees by call site, among them `printf`, whose 13 GC call sites pair with PS2 `printf` calls), and the 12 names the throw-target and scene setup work added (`Shaggy1_ChooseThrowTarget`, `NearestPointObb2D`, `Shaggy1_BonusAnims`, `zEntShaggy_SceneStart` and `zEntShaggy_SceneReset` by DWARF order, and the callees `iAnimEval`, `zParEmitterFind`, `iModelVertCount`, `xModelInstanceAlloc`, `xModelInstanceFree`, `xModelInstanceAttach` and `iModelTagIndex` by call site; the GC-only bound switch at `0x80074C1C`, which the PS2 build inlines, keeps its `fn_` label). The 864 library names are 680 of the
697 names `tools/splits/libmap.py` applies from map-backed symbol lists at the same size (the other
17 functions were already named the same way: 16 by dtk, and `strncpy` by the CARD import, which still counts it), 26 functions that dtk's signatures name
once those names exist, the two `.init` functions after `__start` (`__init_hardware` and
`__flush_cache`, named from the AC and TWW map-backed symbols with the SDK import), `__DSP_debug_printf` (named from the TWW symbols with the SDK import), 20 names the MetroTRK import added (`SetTRKConnected`, `GetTRKConnected`, the three mutex functions, `TRK_memset`, `TRK_memcpy`, `__TRK_reset`, `__TRK_get_MSR`, `__TRK_set_MSR`, `EnableMetroTRKInterrupts`, the four `targsupp.s` stubs, `mslsupp.c`'s `convertFileMode`, `__open_file`, `__close_file` and `__close_console`, and `gTRKInterruptVectorTable`), and 32 names the MSL import added (14 functions of the imported units that libmap left unnamed, their callees `__sys_free`, `__sys_alloc` and `__dec2num` (in `ansi_fp.c`, which libmap maps but this import leaves unlinked), and the 15 math functions at `0x800F01E4`-`0x800F391C`: the `w_*` wrappers, their float forms and the cores they call), and 9 names the os import added (`OSInit`, `__OSDBJump`, `OSGetStackPointer`, `OSGetCurrentContext`, `OSSetArenaLo`, `__OSUnlockAllMutex`, `OSGetCurrentThread` and `DVDSetAutoInvalidation` from the AC and TWW map-backed symbols, and `PPCSetFpNonIEEEMode`, which dtk's `OSInit` signature names once `OSInit` is named), and 28 names the gx import added (26 gx functions that libmap left unnamed, each named from the linked object it matches, with `related` rows where the AC, TWW or FFCC symbols list the name and `inferred` rows where none does, and their callees `PPCSync` and `PPCMfwpar`), and 13 names the si, pad, vi and dvd import added (functions that libmap left unnamed, each named from the object it matches: ten with `related` rows from the AC, TWW or FFCC symbols, and `DVDSeekAsyncPrio`, `DVDSeekAbsAsyncPrio` and `DVDCancelAll` with `inferred` rows, since the link needs their compiled names), and 11 names the exi, ai, ar, base and dsp import added (functions that libmap left unnamed, each named from the object it matches: `AIRegisterDMACallback`, `ARRegisterDMACallback`, `PPCMthid0`, `DSPCheckInit` and `DSPAssertTask` with `related` rows from the AC, TWW or FFCC symbols, and `ARFree`, `ARQRemoveRequest`, `ARQFlushQueue`, `DSPReset`, `DSPHalt` and `DSPGetDMAStatus` with `inferred` rows, since game, Bink or AX code calls each of them), and 42 names the AX import added (31 with `related` rows from the FFCC symbols, where 12 of the bodies differ in size from FFCC's later SDK, and the 11 `AXSetVoice` functions from `AXSetVoiceSrcType` to `AXSetVoiceSrcRatio` with `inferred` rows from bfbb's `AXVPB.c`); see
`config/GIHE78/evidence.tsv`. The 336 Bink names are bfbb's names for the functions at the same offsets of the same Bink build, as `related` rows. The 105 CARD-import names are the 97 CARD functions libmap left unnamed, each named from the imported object it matches exactly, and the eight functions outside CARD those objects call (`OSPanic`, `OSPhysicalToCached`, `OSCachedToPhysical`, `DSPInit`, `DSPAddTask`, `EXIDetach`, `EXIGetID` and `strncpy`), named from the map-backed symbol lists and confirmed by the relocations of the linked objects. The 639 RenderWare names are placed by `tools/splits/rwmap.py` as `related` rows: 553 of bfbb's names by equal size, 58 by position, and 28 node getters, bodies and inits from the game's own node definitions (18 `RxNodeDefinitionGet` names that the PS2 June 2001 prototype symbol table lists, and 10 bfbb names). bfbb does not say where its names come from. The first RenderWare matches added 8 more: `_rwVectorSetMultFn`, `_rwV3dNormalize`, `RwV3dTransformPoints`, `RwV3dTransformVectors` and `VectorMultPoint` in or next to `bavector.c`, and their callees `RwFreeListCreate`, `RwFreeListDestroy` and `RwFreeListForAllUsed`, each named from the PS2 June 2001 prototype symbol table by role, as `related` rows. A named function is not a matched one. The MetroTRK exception vector table (`0x80003154`-`0x80005088`, 7,988 bytes of `.init` code) became visible to objdiff when `mem_TRK.c` took the `.init` code before it, so it was counted as one unnamed function, `fn_80003154`, until its source was imported. That added one function and 7,988 code bytes to the totals. With `__exception.s` imported it is the same span under the table's own label, `gTRKInterruptVectorTable`, so it now counts as named. The os import removed one function: dtk had split the final `blr` of `__OSUnlockAllMutex` (`0x801064B4`) off as a 4-byte `fn_801064B4`, which the linked `OSMutex.c` object gives back to `__OSUnlockAllMutex`. The gx import removed three auto-generated ranges and added one named unit: the gaps after `GXFifo.c`, after `GXMisc.c` and after `GXPixel.c` went to `GXFifo.c`, `GXGeometry.c` and the new `GXStubs.c` and `GXDisplayList.c`, so the file total fell from 540 to 538.

The methodology is `objdiff-report-v1`, generation 1. Change either in `tools/public_report.py`
when what a number means changes, so old rows keep their meaning.

## The map

`map.json` (map-v1) holds two trees over the same bytes. The site checks that they cover exactly
the same addresses and add up to the summary:

- `files`, which the page draws. Each unit gets one code tile holding one leaf per function (its
  address and full size), and one data tile spanning all its data sections. Tiles are in
  address order.
- `sections` groups the functions by DOL code section (`.init`, `.text`), and puts the data
  tiles in one `Data sections` group. Each data tile's fuzzy bytes are the sum over its sections, the same expression the summary uses.

## The timeline

`timeline.json` (timeline-v1) holds one row per successful main run: the revision, the
measurement time, and the code/data total and linked bytes. Each main run restores the rows of
the last successful main run that published a `public-report` (checked against its manifest)
and appends its own `measured` row. A rerun of a revision the timeline already holds changes
nothing. Main runs queue in `validate.yml`'s concurrency group. Every row the site holds as
accepted (its `report-data` and `main` locks) must come back unchanged, or the job fails before
the site would reject the package.

The timeline starts empty only when no earlier main run published a `public-report` and the
site holds no n100f package. If the last published artifact has expired (90 days) and the site
holds nothing, the job fails rather than restart the history.

The contributor module is not supplied, because attribution tracking is undecided. The manifest
declares it unsupported.

## The producer policy

`config/GIHE78/producer.json` is the policy the site holds for this repo. On the site it lives at
`config/n100f-producer.json`, and the two copies must stay identical. It names the workflow, the
job (`Site report`), the required steps, the artifact, the target hash and the tool identities:
the build image digest, dtk with the SHA-256 of the local patch, and objdiff's tag.

Any change to `validate.yml` changes its Git blob. The packaging step then fails until the new
blob is added to `workflowBlobs` here, and the site rejects the next import until the same blob
is approved in its copy. Changing the build image, the dtk patch or `objdiff_tag` works the same
way through `tools`. Both are policy changes: they ship as a PR here and a matching PR on the
site.

Get the blob with `git hash-object .github/workflows/validate.yml`.
