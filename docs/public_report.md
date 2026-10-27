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
[splits.md](splits.md)), and 135 library units (MetroTRK, runtime, MSL, the first part of the
Dolphin SDK and CARD) are named by compiling public decompilations of those libraries (see
[Library units](splits.md#library-units)), 15 Bink units are named from bfbb's identical Bink
layout (see [Bink units](splits.md#bink-units)), and 63 RenderWare units are placed by aligning bfbb's
RenderWare function sizes (see [RenderWare units](splits.md#renderware-units)); the rest are dtk's auto-generated address ranges.
Today 336 of the report's 520 files are named. The game
names come from the PS2 v1.01 DWARF and the boundaries are inferred, so no game unit boundary has been
checked against Heavy Iron's objects. The basis string says so. Library units placed by a
whole-object match have exact extents; the others have open edges. RenderWare unit edges are `provisional` or `open`. Naming those units did not change
what `files.named` means, so the methodology and generation stay the same.
Today's 2,215 named functions are 488 from before the library map, 700 library names, 336 Bink names, 105 names from the CARD import and 586 RenderWare names. The 488
are the names dtk's analysis assigned in `symbols.txt` (MW runtime and some GX SDK functions, plus
`__fill_mem`, which dtk names once `memset` is named), `__start`, the seven names the xMorph unit
added (its three functions and their four callees), 334 SLUS-20349 DWARF names applied from
related-build evidence (168 by an exact bfbb body match and 166 by positions in units whose function
count and pins agree with the DWARF), and 99 names added with the partial units: their callees, the
zCounter and zFog functions (by bfbb body and DWARF position), the xSerial Read/Write and
xSGReadData/xSGWriteData overload groups (by matched caller, body shape and DWARF order), the xbinio,
xhipio and xpkrsvc functions reached through their function tables (by table slot and DWARF order),
the iMath functions and two xutil functions (by DWARF position), the five names the zLaugh unit
added (its three functions and their callees `xSndPlay` and `iSndLookup`, by DWARF order and call site), and the 22 names the zEntButton unit added (its 12 functions by DWARF order and 10 callees by call site). The 700 library names are 663 of the
678 names `tools/splits/libmap.py` applies from map-backed symbol lists at the same size (the other
15 functions were already named the same way by dtk), 26 functions that dtk's signatures name
once those names exist, the two `.init` functions after `__start` (`__init_hardware` and
`__flush_cache`, named from the AC and TWW map-backed symbols with the SDK import), `__DSP_debug_printf` (named from the TWW symbols with the SDK import), and 8 names the MetroTRK import added (`SetTRKConnected`, `GetTRKConnected`, the three mutex functions, `TRK_memset`, `TRK_memcpy` and `__TRK_reset`); see
`config/GIHE78/evidence.tsv`. The 336 Bink names are bfbb's names for the functions at the same offsets of the same Bink build, as `related` rows. The 105 CARD-import names are the 97 CARD functions libmap left unnamed, each named from the imported object it matches exactly, and the eight functions outside CARD those objects call (`OSPanic`, `OSPhysicalToCached`, `OSCachedToPhysical`, `DSPInit`, `DSPAddTask`, `EXIDetach`, `EXIGetID` and `strncpy`), named from the map-backed symbol lists and confirmed by the relocations of the linked objects. The 586 RenderWare names are bfbb's names placed by `tools/splits/rwmap.py`, 528 by equal size and 58 by position, as `related` rows; bfbb does not say where they come from. A named function is not a matched one. The MetroTRK exception vector table (`0x80003154`-`0x80005088`, 7,988 bytes of `.init` code) became visible to objdiff when `mem_TRK.c` took the `.init` code before it, so it is counted as one unnamed function, `fn_80003154`, until its source is imported. That added one function and 7,988 code bytes to the totals.

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
