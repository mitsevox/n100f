# Matching tools

Tools for the inner loop of matching, chosen from the [handbook's Tools chapter](https://mitsevox.github.io/wiki/tools/) by running each tool's check against this project's code (2026-10-29, `main` at `2830fe7`, GIHE78-rev0). Two are scripts in this repository; two are installed per machine and never copied in. None of them makes a match acceptable on its own: a result is a hint until it reads as plausible original source ([CONTRIBUTING](../CONTRIBUTING.md#matching-integrity)).

All of them need a configured and built target (`configure.py`, then `ninja`).

## Trial a variant: `tools/trial.py`

Compiles one variant of a unit with the unit's own command from `build.ninja` and says whether one function is identical to the target, using objdiff-cli's one-shot diff. No rebuild, no report, and nothing in the source tree or `build/` is written: the compile runs in a scratch directory that mirrors the repository through links, with the variant at the unit's own path, so quoted includes and `__FILE__` resolve exactly as in the build.

```sh
python3 tools/trial.py src/flat/zEntShaggy.cpp zEntShaggy_SceneStart__FP6zScene --diff
python3 tools/trial.py /tmp/variant.cpp zEntShaggy_SceneStart__FP6zScene --as src/flat/zEntShaggy.cpp
```

Exit status 0 means identical, 1 different, 2 does not compile. Scores follow the progress report: a relocation to a differently named symbol (the target's `lbl_` literal-pool entries against our `@NNN`) is not a difference; `--strict` counts it.

Check: the verdict (identical or not) of every function of all 317 source units against the report of the same build: 3,622 of 3,627 agree. The five that do not: two Dolphin SDK statics the target calls `fn_8010F20C` and `fn_801116BC`, which the source names differently, so the one-shot diff cannot pair them by name; and `BinkOpen`, `read_rle_samples` and `ReadHuffTable` (ProDG), where the one-shot diff reports an indexed load (`lwzx`, `lbzx`) the report accepts.

## Declaration order: `tools/declorder.py`

On CodeWarrior the order of local declarations decides how the register allocator numbers, and so ranks, the locals. This tool tries the orders of a function's local declarations through `trial.py` and reports the best. It moves only declarations whose order cannot change what the code does: one per line in the run that opens a block, of pointer, scalar or `--pod` type, with an initializer that calls and assigns nothing and reads none of the other moved locals. Anything else in the run stays put and splits it. Up to `--max` (5040) orders are tried exhaustively, the reverse first; above that, a deterministic local search moves one declaration at a time while the score improves.

```sh
python3 tools/declorder.py src/flat/zEntShaggy.cpp zEntShaggy_SceneStart__FP6zScene --list
python3 tools/declorder.py src/flat/zEntShaggy.cpp zEntShaggy_SceneStart__FP6zScene --out /tmp/best.cpp
```

`--block N` picks an inner block, `--line` an overload, `--name` the spelling of a definition the symbol does not give. `--out` writes the best order to a file for review; the tool never edits the source.

Check: KNOWN_ANSWER

## Out-of-repository tools

These are installed on the machine that uses them, pinned to the revision below, and never copied into this repository. Their output (dumps, drafts) contains the game's code and stays local.

### mwcc-debugger

[cadmic/mwcc-debugger](https://github.com/cadmic/mwcc-debugger) at `bad9cea2423bed957188c930086f9dabe669d30c` runs the compiler under [encounter/retrowin32](https://github.com/encounter/retrowin32) (branch `gdb-stub`, `11dbea5a68af21121511a6577a2d4a2f917da6dc`, built with `cargo build -p retrowin32 -F x86-unicorn --profile lto`) and gdb, and dumps one function's frontend trees, the backend code between passes and the register allocator's priority order. It answers why a near miss allocates differently; what to change is still a question of plausible source.

It supports GC/2.6, not GC/1.3.2. Check: every unit this project compiles with GC/1.3.2 was compiled again with GC/2.6 under the same flags and diffed function by function. Game code: 1,071 of 1,085 functions identical (893 of 898 matched ones). RenderWare: 483 of 505. The game functions that differ are `xMovePointGetNext`, `zEntPlayer_Render`, `VSUP_ds2_toPos`, the six `VANM_mktable_*` tables and `VNAV_find_telepoint` in zEntVillain, `ZLOB_shoot_legacy`, and `zSurfaceGetSlickness`, `zSurfaceGetSlideStopAngle` and `zSurfaceGetSlideStartAngle` (which [the compiler profile](compiler_profile/README.md) uses to tell 1.3.2 from 2.x). For those, the dumps describe another compiler. Run time is about 25 s per function. Pass the unit's command from `ninja -t commands` with `GC/2.6` for `GC/1.3.2`, without `wibo`, `sjiswrap` or `transform_dep.py`, plus `-sym on`.

The repository has no licence file, so it is used only from its own clone.

### m2c

[matt-kempster/m2c](https://github.com/matt-kempster/m2c) at `c632946b8cfbe8fce442b522dba92dfa72f1f317` (GPL-3.0) turns a function's assembly from `build/<id>/asm/` into a C draft (`-t ppc-mwcc-c --valid-syntax --deterministic-vars`). Its `--context` parser reads C only, and this project's headers are C++ (members, inheritance, references, inline bodies), so a context needs a C-ified copy, and the asm's mangled names need demangling before the context's prototypes apply.

Check: drafts for ten functions already matched, swapped into their units and scored with `trial.py`: six compile, two of them identical (`zGameIsPaused`, `xParFree`) and four at 99.47, 98.59, 92.23 and 72.79 percent; four do not compile as C++ (implicit pointer conversions, a call it could not type). Use it for a first draft of a function with no counterpart in bfbb or the PS2 debug information, then rewrite the draft in the project's names, types and style. Its drafts are never committed.

## Left out

- decomp-permuter: most of what its random search finds improves register allocation by accident and is a fake match; `declorder.py` covers the part of its search that cannot change behavior.
- GCC dump flags: only the ProDG (Bink) code would use them.
- coddog and Ghidra BSim: rev1 is tied to rev0 by its byte-identical relink and `tools/port_symbols.py`, and the library and RenderWare maps already name shared code.
- The compiler probe the handbook describes is [`tools/compiler_probe.py`](../tools/compiler_probe.py).
