# Build Analysis: Scooby-Doo! Night of 100 Frights (PS2 Prototype, Jan 31 2001)

> [!NOTE]
> **Legal Compliance & Clean-Room Preservation**:
> No executables, disc images, or game archives are stored here. Only derived metadata (symbol tables, debug-info exports, section layouts, file listings) and diagnostic strings are preserved. The source executables stay local; their SHA-1 is recorded for provenance.

## Build & Disc Metadata
- **Platform**: Sony PlayStation 2 (CD-ROM, MODE2/2352)
- **Volume**: `THISISATEST`, created 2001-01-25. Executable dated 2001-01-31.
- **Local image name**: "Scooby-Doo! Night of 100 Frights (USA) (Beta 1)"
- **Executable**: `XXXX_999.99` (placeholder serial), 7,253,692 bytes, SHA-1 `af2a1be3ab6497a9ec3585aa3d9668139f514dfe`, entry `0x00200008`
- **Compiler**: `MW MIPS C Compiler (2.4.1.01)`

## Executive Summary
This is the earliest PS2 build of the rebooted game. It is **fully unstripped**:

* **ELF `.symtab`**: 17,648 symbols (4,137 functions), covering game code, RenderWare 3 and the SCE libraries.
* **DWARF 1**: 1,396 functions with signatures, 2,327 variables, 374 type definitions, 20,894 line rows across 175 source files under `C:\Projects\ScoobyDoo\Code\{Engine,Game}\...`. DWARF addresses match `.symtab` exactly for 1,394 of 1,396 functions. The other two are duplicate-named statics.
* **`.relmain`**: 1.3 MB of relocations for the `main` section. Not exported yet, but useful for data-reference recovery.
* **RenderWare paths**: 190 `c:/Dev/rwdev/rwsdk/...` source anchors.

**Engine generation**: most units here (`xActor`, `xNWZ`, `xNav*`, `xRoute`, `JDebug`...) come from an **older engine than retail**. The `xEnt`/`xScene`/`zEnt*` architecture of the retail game appears in the Jun 2001 build. Use this build for libraries and history, not as a direct naming source for GIHE78.

## Exported Directory Structure

```text
PROTO-PS2-2001-01-31/
├── ANALYSIS.md
├── symbols/
│   ├── elf_sections.tsv
│   ├── symtab.tsv             # 17,648 ELF symbols: address, size, type, binding, section, name
│   ├── source_files.tsv
│   ├── dwarf_functions.tsv
│   ├── dwarf_variables.tsv
│   ├── dwarf_types.h
│   ├── dwarf_dump.cpp
│   ├── line_table.tsv
│   └── source_anchors.tsv     # 190 RenderWare / skeleton source paths
├── strings/
│   └── debug_strings.txt
└── disc/
    ├── filelist.tsv
    ├── SYSTEM.CNF
    └── DISK_ED.INI
```
