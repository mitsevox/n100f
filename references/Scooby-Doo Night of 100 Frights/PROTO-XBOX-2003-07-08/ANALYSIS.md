# Build Analysis: Scooby-Doo! Night of 100 Frights (Xbox Prototype, Jul 8 2003)

> [!NOTE]
> **Legal Compliance & Clean-Room Preservation**:
> No executables, disc images, or game archives are stored here. Only derived metadata (symbol tables, debug-info exports, section layouts, file listings) and diagnostic strings are preserved. The source executables stay local; their SHA-1 is recorded for provenance.

## Build & Disc Metadata
- **Platform**: Microsoft Xbox (XDVDFS)
- **Executable**: `default.xbe`, 1,781,760 bytes, SHA-1 `a300ab9f957f18d81e6688cb99bd9aaef12bc5d9`
- **XBE timestamp**: 2003-07-08 21:02:09 UTC. Title ID `0x07777777` (placeholder, not a retail ID)
- **Debug path**: `c:\SD2XB\ScoobyXB\Master\ScoobyXB.exe` ("Master" configuration)
- **XDK**: 1.0.5558 (XAPILIB, D3D8, D3DX8, XGRAPHC, DSOUND, XMV, XBOXKRNL, LIBCMT)
- **Compiler**: MSVC (Xbox). This is a different toolchain from GameCube.

## Executive Summary
This is a late port of the finished game. It's a stripped XBE with no PDB on the disc, so there are no symbols. What it does have:

* 98 RenderWare 3.4 `$Id$` stamps (`//RenderWare/RW34Active/rwsdk/...`) giving exact RW source revisions.
* Diagnostic strings, many shared with the GameCube build.
* `vn/readme.txt`, an internal Heavy Iron design note on splitting villains into per-level HIP files. It's useful context for the HIP loader code.

## Exported Directory Structure

```text
PROTO-XBOX-2003-07-08/
├── ANALYSIS.md
├── symbols/
│   ├── xbe_sections.tsv
│   ├── xbe_libraries.tsv
│   └── source_anchors.tsv
├── strings/
│   └── debug_strings.txt
└── disc/
    ├── filelist.tsv
    ├── sd2.ini
    └── vn_readme.txt
```
