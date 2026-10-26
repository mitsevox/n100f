# Build Analysis: Scooby-Doo! Night of 100 Frights (PS2 Prototype, Jun 11 2001)

> [!NOTE]
> **Legal Compliance & Clean-Room Preservation**:
> No executables, disc images, or game archives are stored here. Only derived metadata (symbol tables, debug-info exports, section layouts, file listings) and diagnostic strings are preserved. The source executables stay local; their SHA-1 is recorded for provenance.

## Build & Disc Metadata
- **Platform**: Sony PlayStation 2 (CD-ROM, MODE2/2352)
- **Volume**: `THISISATEST` (reused 2001-01-25 descriptor). Game files dated 2001-06-11.
- **Local image name**: "Scooby-Doo! Night of 100 Frights (USA) (Beta 2)"
- **Executable**: `XXXX_999.99`, 2,218,332 bytes, SHA-1 `730afc26adf3042c442159144be14600255e8707`, entry `0x00200008`
- **Compiler**: `MW MIPS C Compiler (2.4.1.01)`

## Executive Summary
* **ELF `.symtab` only**: 11,715 symbols (2,394 functions, 719 objects). No DWARF, which matches the lead.
* This is the first build on the **retail engine architecture**: `xEnt*`, `xScene*`, HIP archives (`FOO1-4.HIP`, `TECH.HIP`), `SD2.INI`. Its symbol names line up with retail and GameCube code.
* The names are MW-mangled, so they include parameter types (e.g. `xEntHitTrigsNoDepen__FP5_xEntP7_xScenePv`). That makes them a useful cross-check against retail DWARF signatures for functions that changed between builds.

## Exported Directory Structure

```text
PROTO-PS2-2001-06-11/
├── ANALYSIS.md
├── symbols/
│   ├── elf_sections.tsv
│   ├── symtab.tsv
│   └── source_anchors.tsv
├── strings/
│   └── debug_strings.txt
└── disc/
    ├── filelist.tsv
    ├── SYSTEM.CNF
    └── SD2.INI
```
