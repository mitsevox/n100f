# Scooby-Doo! Night of 100 Frights

A matching decompilation of **Scooby-Doo! Night of 100 Frights** (GameCube, Heavy Iron Studios / THQ, 2002). The goal is source that compiles back to a byte-identical copy of each original executable, with names, types and source structure supported by evidence.

## Targets

| Id | Region | Revision | Status | Original SHA-1 |
| --- | --- | --- | --- | --- |
| `GIHE78-rev0` | USA | 0 | active | `c77fb29d39b5898933e8ef4d1ccd2ddac404554e` |
| `GIHE78-rev1` | USA | 1 | active | `cc84bc42b1054b572b5bb65968cf80b71ff41d0b` |

`config/targets.json` is the registry; each target's inputs are in `config/<id>/`. The default remains `GIHE78-rev0`. Its original is `sys/main.dol`, 1,964,512 bytes, entry point `0x80016898`, with no linker map or symbols. `GIHE78-rev1` has a byte-identical relink from original split objects and starts with no source units linked; its original is 2,032,480 bytes with entry point `0x80016A00`. See its [target notes](config/GIHE78-rev1/README.md) for the initial map and startup-name evidence.

## Building

You need your own copy of the game disc. Keep disc images and extracted files outside public Git.

1. Place the original at `orig/<id>/sys/main.dol`.
2. Build decomp-toolkit 1.8.3 with [`tools/patches/dtk-v1.8.3-rom-copy-info.patch`](tools/patches/dtk-v1.8.3-rom-copy-info.patch). Stock dtk looks for `_rom_copy_info` only in the section holding the entry point, and this game enters through a routine in `.text`:

    ```sh
    git clone --branch v1.8.3 https://github.com/encounter/decomp-toolkit
    git -C decomp-toolkit apply "$PWD/tools/patches/dtk-v1.8.3-rom-copy-info.patch"
    cargo build --release --manifest-path decomp-toolkit/Cargo.toml
    ```

3. `python3 configure.py --version <id> --dtk decomp-toolkit/target/release/dtk` then `ninja -f build/<id>/build.ninja`. The build fails unless the relinked executable matches `config/<id>/build.sha1`.
4. `ninja -f build/<id>/build.ninja progress build/<id>/report.json` measures the target.

To check every active target: `for id in $(python3 -c 'from tools.targets import build_targets; print(*build_targets())'); do python3 configure.py --version "$id" --dtk decomp-toolkit/target/release/dtk && ninja -f "build/$id/build.ninja"; done`.

Pinned tool versions are in `configure.py`; tools for trying one variant of a function, and the out-of-repository matching tools, are in [docs/matching_tools.md](docs/matching_tools.md); the build structure is [dtk-template](https://github.com/encounter/dtk-template) at the revision named in `CREDITS.md`. Compiler profiles and their evidence are in [docs/compiler_profile/](docs/compiler_profile/README.md). For objdiff's source watcher, `python3 tools/activate_target.py --target <id>` makes that target the root workspace's view.

## CI and the progress site

`.github/workflows/validate.yml` builds every affected target in the private build image from [mitsevox/n100f-build](https://github.com/mitsevox/n100f-build) and publishes each measurement to the [progress site](https://mitsevox.github.io/games/n100f/) on `main`. Pull requests publish nothing.

## Contributing

The project owner is **[mitsevox](https://github.com/mitsevox)**, the sole merge authority. See [CONTRIBUTING.md](CONTRIBUTING.md) for the tiers, the task loop and the review checklist; agents start with [AGENTS.md](AGENTS.md).

## Layout

| Directory | Purpose |
| --- | --- |
| `src/`, `include/` | Shared reconstructed source and headers; version differences under `#if VERSION_<ID>` (`include/version.h`) |
| `config/<id>/` | One target's toolkit inputs, `build.sha1` and `evidence.tsv` |
| `orig/<id>/` | Where the original goes locally; ignored by Git |
| `build/<id>/` | Generated; ignored |
| `tools/`, `tests/` | The kit tools, dtk-template's tools and the project's own, with their tests |
| `docs/`, `references/` | Durable project knowledge and regenerable reference material |

## Credits and licenses

Imported source is credited in [CREDITS.md](CREDITS.md); licenses are under `licenses/`.
