# Scooby-Doo! Night of 100 Frights

A faithful matching decompilation of **Scooby-Doo! Night of 100 Frights for the Nintendo GameCube** (Heavy Iron Studios / THQ, 2002). The goal is to reconstruct source that compiles back to a byte-identical copy of the original game executable, with names, types, and source structure supported by evidence.

> **Bootstrap status:** this repository is being set up. Items marked `TO_VERIFY` need the original executable; items marked `TO_IMPLEMENT` have no working implementation yet. Nothing here is a verified build until those markers are gone.

## Target

| Fact | Value |
| --- | --- |
| Platform | Nintendo GameCube |
| Region / game ID | `TO_VERIFY` (USA release expected; confirm from the disc header) |
| Disc revision | `TO_VERIFY` |
| Executable | `main.dol` |
| `main.dol` SHA-1 | `TO_VERIFY` |
| Objective | Faithful byte-identical matching reconstruction |

The target directory under `config/` is named for the verified game ID once it is established.

## Building

You will need your own copy of the game. Keep the original executable and disc image outside public Git.

Supported hosts, pinned tools and exact commands: `TO_IMPLEMENT`. Baseline relink, accepted-source build and independent comparison are documented separately once they exist.

## Contributing

The project owner is **Lucas ([mitsevox](https://github.com/mitsevox))**, the sole merge authority.

See [CONTRIBUTING.md](CONTRIBUTING.md) for the work and review process. AI agents start with [AGENTS.md](AGENTS.md). Imported and reused source is credited in [CREDITS.md](CREDITS.md).
