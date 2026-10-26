# References

Clean-room reference material for the decompilation, laid out the same way as `mitsevox/nflstreet2`: `references/<Game>/<Build>/` with an `ANALYSIS.md`, `symbols/`, `strings/` and `disc/`.

No executables, disc images or game archives are stored here. Every file is derived metadata or text read from the discs. Source executable SHA-1s are in each `ANALYSIS.md`. Exports are regenerated with `tools/references/extract.py`.

## Scooby-Doo! Night of 100 Frights

| Build | Platform | Symbols | DWARF | Use |
|---|---|---|---|---|
| [`DOL-GIHE-USA`](Scooby-Doo%20Night%20of%20100%20Frights/DOL-GIHE-USA/ANALYSIS.md) | GameCube | – | – | **Target.** Sections, strings, HIP packer logs |
| [`SLUS-20349`](Scooby-Doo%20Night%20of%20100%20Frights/SLUS-20349/ANALYSIS.md) | PS2 retail v1.01 | – | **Yes** | Primary naming source: 2,625 functions, 725 types, locals, lines |
| [`PROTO-PS2-2001-06-11`](Scooby-Doo%20Night%20of%20100%20Frights/PROTO-PS2-2001-06-11/ANALYSIS.md) | PS2 prototype | 11,715 | – | First retail-engine build, mangled names |
| [`PROTO-PS2-2001-01-31`](Scooby-Doo%20Night%20of%20100%20Frights/PROTO-PS2-2001-01-31/ANALYSIS.md) | PS2 prototype | 17,648 | Yes | Older engine generation, RW3 and SCE libraries |
| [`PROTO-XBOX-2003-07-08`](Scooby-Doo%20Night%20of%20100%20Frights/PROTO-XBOX-2003-07-08/ANALYSIS.md) | Xbox prototype | – | – | RW 3.4 revisions, design note |
| [`PROTO-PS1-2000-10-26`](Scooby-Doo%20Night%20of%20100%20Frights/PROTO-PS1-2000-10-26/ANALYSIS.md) | PS1 prototype | – | – | Pre-reboot. History only |

Each PS2 build reports `MW MIPS C Compiler (2.4.1.01)`, and the GameCube DOL carries Metrowerks runtime markers. That's evidence that Heavy Iron used CodeWarrior on both platforms.

Notes on the DWARF exports:
- MW records every aggregate as `class`. `dwarf_types.h` keeps that, so read `class` as struct/class.
- Functions referenced through pointers also show up as `global_variable` entries with a function type (e.g. animation callbacks). They're kept as-is.
- Static functions sometimes carry no parameter entries. The mangled name still encodes their parameter types.
