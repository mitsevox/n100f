# Build Analysis: Scooby-Doo! Night of 100 Frights (USA GameCube - Primary Target) (DOL-GIHE-USA)

> [!NOTE]
> **Legal Compliance & Clean-Room Preservation**:
> No executables, disc images, or game archives are stored here. Only derived metadata (symbol tables, debug-info exports, section layouts, file listings) and diagnostic strings are preserved. The source executables stay local; their SHA-1 is recorded for provenance.

## Build & Disc Metadata
- **Platform**: Nintendo GameCube
- **Region**: North America (NTSC-U)
- **Game ID**: `GIHE78`, disc 0, revision 0
- **Target Hash (SHA-1)**: `c77fb29d39b5898933e8ef4d1ccd2ddac404554e` (`main.dol`, 1,964,512 bytes)
- **Entry**: `0x80016898`
- **SDK markers**: `Dolphin OS $Revision: 52 $`, build dates `Dec 17 2001` and `Apr 16 2002`, "Metrowerks CW runtime library", "Metrowerks Target Resident Kernel for PowerPC"
- **Developer / Publisher**: Heavy Iron Studios / THQ (2002)

## Executive Summary
This is the decompilation target. Like other retail DOLs it has no symbols or debug sections. The names come from the PS2 retail DWARF (`SLUS-20349`) and the Jun 2001 symbol table.

* **Section layout**: `.text0` `0x80003100`, `.text1` `0x800055E0` (1,609,696 bytes), six data sections, `.bss` `0x801DF1C0` (539,444 bytes).
* **Source anchors**: `iSystem.cpp`, the Dolphin `CARD*` library units, and `dvdfs.c`.
* **Packer logs** (`disc/packer_logs/`): the retail disc shipped **92 Heavy Iron HIP packer logs** (`*.log`, one per HIP). Each lists package version (`HIPA` 2, client 4.6), layers, and every asset by type, ID and size. They're the reference for the HIP/asset loader and asset-type enums.

## Exported Directory Structure

```text
DOL-GIHE-USA/
├── ANALYSIS.md
├── symbols/
│   ├── dol_sections.tsv
│   └── source_anchors.tsv
├── strings/
│   └── debug_strings.txt
└── disc/
    ├── filelist.tsv           # FST listing (334 files)
    ├── sd2.ini
    └── packer_logs/           # 92 HIP packer logs, original paths
```
