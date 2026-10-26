# Scooby-Doo! Night of 100 Frights

A faithful matching decompilation of **Scooby-Doo! Night of 100 Frights for the Nintendo GameCube** (Heavy Iron Studios / THQ, 2002). The goal is to reconstruct source that compiles back to a byte-identical copy of the original game executable, with names, types, and source structure supported by evidence.

> **Bootstrap status:** this repository is being set up. Items marked `TO_VERIFY` still need target evidence; items marked `TO_IMPLEMENT` have no working implementation yet. Nothing here is a verified build until those markers are gone.

## Target

USA release, **GIHE78**, disc revision **0**.

Original `main.dol` (1,964,512 bytes, entry point `0x80016898`) SHA-1:

```text
c77fb29d39b5898933e8ef4d1ccd2ddac404554e
```

The disc carries no linker map or symbols. Configuration for this target lives in [`config/GIHE78/`](config/GIHE78/README.md).

## Building

You will need your own copy of the game. Keep the disc image and extracted files outside public Git.

The build uses [decomp-toolkit](https://github.com/encounter/decomp-toolkit) through the [dtk-template](https://github.com/encounter/dtk-template) structure, with Python 3 and [ninja](https://ninja-build.org/). Pinned tool versions are in `configure.py`.

1. Extract the disc and place its `sys/main.dol` at `orig/GIHE78/sys/main.dol`.
2. Build decomp-toolkit 1.8.3 with [`tools/patches/dtk-v1.8.3-rom-copy-info.patch`](tools/patches/dtk-v1.8.3-rom-copy-info.patch). Stock dtk looks for `_rom_copy_info` only in the section containing the entry point, and this game enters through a routine in `.text`:

```sh
git clone --branch v1.8.3 https://github.com/encounter/decomp-toolkit
git -C decomp-toolkit apply "$PWD/tools/patches/dtk-v1.8.3-rom-copy-info.patch"
cargo build --release --manifest-path decomp-toolkit/Cargo.toml
```

3. Configure and build:

```sh
python3 configure.py --dtk decomp-toolkit/target/release/dtk
ninja
```

With no source units, this is the original-object baseline: dtk splits the DOL, mwld relinks it, and `ninja` checks the result against `config/GIHE78/build.sha1`. It is byte-identical. Game code uses CodeWarrior for GameCube 1.3.2 and the Dolphin SDK uses 1.2.5n; the evidence is in [`docs/compiler_profile/`](docs/compiler_profile/README.md). See [`docs/dtk/`](docs/dtk/getting_started.md) for the upstream tool documentation.

CI runs the same build in the private, digest-pinned build environment from [mitsevox/n100f-build](https://github.com/mitsevox/n100f-build) (`.github/workflows/validate.yml`), which provides the original executable, the patched dtk, compilers and binutils.

## Contributing

The project owner is **Lucas ([mitsevox](https://github.com/mitsevox))**, the sole merge authority. The owner has delegated review clearance and merging to a designated merge gate that acts under the owner's account; see [owner delegation](CONTRIBUTING.md#owner-delegation-merge-gate).

See [CONTRIBUTING.md](CONTRIBUTING.md) for the work and review process. AI agents start with [AGENTS.md](AGENTS.md). Imported and reused source is credited in [CREDITS.md](CREDITS.md).
