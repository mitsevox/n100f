# Agent instructions

Read [CONTRIBUTING.md](CONTRIBUTING.md) before working: it defines the review tiers, the task loop, matching integrity, the evidence records and the hostile-review checklist. The [README](README.md) names the targets, the owner and the build commands.

- Keep original executables, disc images, assets, official SDK files (headers, libraries, documentation) and trial output out of public Git, and never discuss leaked material in public. Use the pinned, owner-approved build environment and do not alter supporting dependencies or their access controls; report dependency problems in an issue for the owner's decision.
- Work on a dedicated branch in your own checkout or worktree. Open a draft PR early with the unit names and target ids in its title; check open PRs before starting. Preserve unrelated changes and coordinate live overlaps on shared files.
- Keep recovered facts, hypotheses, function-level matches, combined-build results and review verdicts distinct. Report what was actually checked, on which revision and for which targets.
- Preserve useful partial reconstruction in normal source files as `NonMatching` objects so CI measures it. Original bytes stay in the verified linked build until promotion. Matching incompleteness alone is not a review failure.
- Record evidence for names, boundaries, types and compiler profiles in `config/<target id>/evidence.tsv` ([format](config/README.md)) in the same PR as the change it supports.
- State the PR's tier in its Scope line and complete the records that tier requires. An author cannot provide its own independent review. A `SHIP` verdict means that review passed; it does not approve a merge.
- **Only the project owner authorizes and performs merges into `main`.** No agent enables auto-merge, pushes to `main`, force-pushes `main` or another contributor's branch, or changes branch rules, repository settings, the build image or its digest. Push a work branch or open a PR only when authorized for the task, then leave it for the owner's decision.
- Address review feedback without weakening checks, hiding mismatches or changing project rules to accommodate a patch. Propose workflow improvements through a separate PR.
- Keep handoffs concise: changes, evidence, validation per target, unresolved findings and review state. Do not add running status diaries to the repository.
- A function becomes exact only with its name, its evidence and its comment in the same PR; there is no later naming pass.
- Never delete a file you have not checked first: make the delete depend on the check. Edit C, C++ and headers with an editor or a saved script whose diff you read, never with sed, heredocs, echo or `python -c` (they mangle backslashes).
- Ask the owner before downloads, purchases, posts or messages anywhere. Tokens and secrets are the owner's to create and store; never ask for their values.
- Working with the owner: plain, short English that says what each code area does in the game; the concept first, then the owner's green light, then small steps; numbers exactly as measured, never estimated by feel. At every change of project phase, re-read these rules and ask the owner about any rule written for the previous phase.

Before calling a shared change safe for every target, an agent has done four things, per active target:

1. Built every active target and confirmed `config/<target id>/build.sha1` on each.
2. Run objdiff per target for each touched unit and reported the `Matching` state and progress delta in the PR's Targets table.
3. Run `python3 tools/versions.py diff` on every file it added a `#if VERSION_` block to, and read the result.
4. Added evidence rows to each affected target's `evidence.tsv`.

## Project

- **Toolchain.** Heavy Iron game code is CodeWarrior GC/1.3.2 and the Dolphin SDK GC/1.2.5n; MSL C, the runtime, RenderWare and Bink have their own profiles. The evidence and the flag files are in [docs/compiler_profile/](docs/compiler_profile/README.md); `configure.py` chooses the profile per library. Bink builds with ProDG through `tools/prodg_cc.py`, which the `Project checks` step tests against the driver in the build image.
- **dtk patch.** The build image's decomp-toolkit carries [tools/patches/dtk-v1.8.3-rom-copy-info.patch](tools/patches/dtk-v1.8.3-rom-copy-info.patch): this game enters through a routine in `.text`, where stock dtk does not look for `_rom_copy_info`. Do not build without it.
- **References.** Text extractions of the reference builds' symbols and debug information are in `references/` by the owner's decision (2026-10-26); they corroborate, they are not target evidence. Community speedrunning documents there are playthrough observations only ([community references](docs/community_references.md)). Library provenance and the libmap tool are in [docs/libraries.md](docs/libraries.md); split decisions in [docs/splits.md](docs/splits.md).
- **Investigation.** State the question first. Disassembly, references and Ghidra answer what the target does; controlled compiler experiments under the documented profile answer why plausible source differs; a bounded headless Dolphin trace answers which behavior fits an execution. None establishes original source alone. When evidence stays insufficient, keep the original assembly linked and report the gap.
- **objdiff view.** objdiff watches only its project directory, so `python3 tools/activate_target.py --target GIHE78-rev0` copies that target's generated `objdiff.json` and `compile_commands.json` to the root; open the root workspace. The canonical files stay in `build/<target id>/`.
