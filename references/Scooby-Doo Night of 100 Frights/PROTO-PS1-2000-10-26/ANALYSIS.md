# Build Analysis: Scooby-Doo! Night of 100 Frights (PS1 Prototype, Oct 26 2000)

> [!NOTE]
> **Legal Compliance & Clean-Room Preservation**:
> No executables, disc images, or game archives are stored here. Only derived metadata (symbol tables, debug-info exports, section layouts, file listings) and diagnostic strings are preserved. The source executables stay local; their SHA-1 is recorded for provenance.

## Build & Disc Metadata
- **Platform**: Sony PlayStation (CD-ROM, MODE2/2352)
- **Volume created**: 2000-10-26 23:26
- **Executable**: `PSX.EXE`, 370,688 bytes, SHA-1 `0a7a9af4b815d7d3203d0296a1d3a4d1e2711488`
- **Build stamp**: `PSX build: Oct 26 2000, 22:49:56`, "(c) 2000 Heavy Iron Studios"
- **Header**: pc0 `0x8005735C`, load `0x80010000`, sp `0x801FFFF0`, Sony NTSC-U marker

## Executive Summary
This is a PS1 build from before the cancellation and reboot. It's a stripped PSX-EXE with no symbols or debug info. Data is the old `.P1`/`.STR`/`.TIM` format, levels are listed in `LEVELS.INI`, and libraries are Sony PsyQ (1997-1998 `$Id$` stamps). None of the code is expected to survive into the GameCube build. It's kept for history and completeness.

## Exported Directory Structure

```text
PROTO-PS1-2000-10-26/
├── ANALYSIS.md
├── symbols/
│   ├── exe_header.tsv
│   └── source_anchors.tsv
├── strings/
│   └── debug_strings.txt
└── disc/
    ├── filelist.tsv
    ├── SYSTEM.CNF
    ├── ED.INI
    └── LEVELS.INI
```
