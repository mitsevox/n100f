# Build Analysis: Scooby-Doo! Night of 100 Frights (USA PS2 Retail) (SLUS-20349)

> [!NOTE]
> **Legal Compliance & Clean-Room Preservation**:
> No executables, disc images, or game archives are stored here. Only derived metadata (symbol tables, debug-info exports, section layouts, file listings) and diagnostic strings are preserved. The source executables stay local; their SHA-1 is recorded for provenance.

## Build & Disc Metadata
- **Platform**: Sony PlayStation 2
- **Region**: North America (NTSC-U)
- **Serial**: `SLUS-20349`, volume `HEAVYIRON_SCOOBY`, created 2002-04-15
- **Revision**: `SYSTEM.CNF` reports `VER = 1.01`
- **Executable**: `SLUS_203.49`, 8,553,100 bytes, SHA-1 `ebe640193e0ceb38deca343a0b576cfcba2d5920`
- **Compiler**: `MW MIPS C Compiler (2.4.1.01)` (Metrowerks CodeWarrior for PS2)
- **Developer / Publisher**: Heavy Iron Studios / THQ

## Executive Summary
The retail master shipped **with its DWARF 1 debug sections intact** (`.debug` 5.5 MB, `.line` 570 KB). The symbol table is stripped, but DWARF carries everything that matters:

* **2,625 functions** with address, size, binding, MW-mangled name and full typed signature with parameter names.
* **1,738 global and file-static variables** with addresses and types.
* **725 distinct type definitions** (classes, unions, enums, typedefs) with member offsets and sizes.
* **Locals and scopes** for every function, with register or stack-slot assignment.
* **52,245 line-table rows** mapping addresses to source lines.
* **161 source files** under `E:\SD2\Game\flat\` (codename **SD2**). The `x*` (engine core), `z*` (game) and `i*` (platform layer) naming is the same Heavy Iron layout used by Battle for Bikini Bottom.

This version is the main DWARF reference for the GameCube target. The `x*`/`z*` code is shared across platforms. Only the `i*` files and the PS2 middleware (`ps2_mpeg`, `p2*`, `gcc_wrapper.c`) are PS2-specific.

## Exported Directory Structure

```text
SLUS-20349/
├── ANALYSIS.md
├── symbols/
│   ├── elf_sections.tsv       # ELF section layout
│   ├── source_files.tsv       # 161 compile units with function counts and PC ranges
│   ├── dwarf_functions.tsv    # 2,625 functions: address, size, binding, name, mangled, signature, file
│   ├── dwarf_variables.tsv    # 1,738 globals/statics: address, binding, name, mangled, type, file
│   ├── dwarf_types.h          # 725 type definitions with member offsets
│   ├── dwarf_dump.cpp         # Per-file dump: globals, functions, params, locals, scopes
│   ├── line_table.tsv         # Address -> source line for every unit
│   └── source_anchors.tsv     # Source path strings found in data
├── strings/
│   └── debug_strings.txt      # Asserts, format strings, paths (address-tagged)
└── disc/
    ├── filelist.tsv           # Disc listing (path, size, date)
    ├── SYSTEM.CNF
    └── SD2.INI                # Game boot configuration read by the engine
```

## Relevance to the GameCube Decompilation
1. **Names and signatures**: `dwarf_functions.tsv` gives real names and prototypes for most of the shared engine and game code in GIHE78.
2. **Struct layouts**: `dwarf_types.h` gives member names and offsets. MIPS and PowerPC are both 32-bit with matching basic type sizes, so most layouts carry straight over. Check anything with 64/128-bit members (`u_long128` is PS2-only).
3. **Function order**: `source_files.tsv` and `line_table.tsv` show per-file function order. CodeWarrior generally keeps source order, which helps with splitting units.
4. **Locals**: `dwarf_dump.cpp` lists local variable names and scopes, which is the strongest guide for reconstructing function bodies.

Regenerate with `python3 tools/references/extract.py elf SLUS_203.49 <outdir>`.
