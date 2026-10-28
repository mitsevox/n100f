# Onboarding GIHE78-rev1: the checklist

The procedure that takes `GIHE78-rev1` (Scooby-Doo! Night of 100 Frights, GameCube, USA, disc revision 1) from `archived` to an active, published target of this repository. It is written for someone with no other context: every step names the command, the files it touches and the output it must produce, and ends with a gate. **A failed gate means stop, write down what happened (the "How to report back" section at the end) and report. Never improvise past a gate.**

The generic version of this procedure is the wiki chapter [Multiple targets](https://mitsevox.github.io/wiki/multi-target/); this file is the one to follow here, because it carries this repository's facts.

## Who does what

- **AGENT** steps are done in a checkout of this repository, on a branch, and end in a pull request. The agent never merges, never pushes to `main`, never changes repository settings, the build image or its digest, and never commits an original executable.
- **OWNER** steps are done by the project owner (`mitsevox`) on GitHub or in the private build repository. They are marked `OWNER` and give the exact clicks. The agent stops at each of them and reports.

## Facts

| What | Value |
| --- | --- |
| Public repository | `mitsevox/n100f` |
| Private build repository | `mitsevox/n100f-build` (holds the originals; never public) |
| Target active so far | `GIHE78-rev0`: `sys/main.dol`, 1,964,512 bytes, SHA-1 `c77fb29d39b5898933e8ef4d1ccd2ddac404554e`, entry point `0x80016898` |
| Target to activate | `GIHE78-rev1`: SHA-1 `cc84bc42b1054b572b5bb65968cf80b71ff41d0b`, 2,032,480 bytes (from the private pull request mitsevox/n100f-build#3), listed `archived` in `config/targets.json` |
| Its placeholder today | `config/GIHE78-rev1/README.md` (prose) and `orig/GIHE78-rev1/.gitkeep` |
| Build image today | `ghcr.io/mitsevox/n100f-build@sha256:502c957567a21fd77283fad6d437ae3b6cf19856a8b456157040b6d36cb83773` (the `BUILD_IMAGE` line of `.github/workflows/validate.yml`); it holds `/orig/GIHE78-rev0/sys/main.dol` and not rev1 |
| The rev1 original | In mitsevox/n100f-build#3 (branch `archive/gc-usa-rev1-dol`, file `orig/GIHE78-rev1/main.dol`), open and not merged; that pull request was based on a commit before #4 and must be brought up to date before merging |
| Toolkit | decomp-toolkit v1.8.3 with this repository's patch `tools/patches/dtk-v1.8.3-rom-copy-info.patch`; the README's Building section says how to build it |
| Tools this procedure needs | `tools/new_target.py`, `tools/matching_scope.py`, `tools/versions.py`, `tools/targets.py` as the kit sync of 2026-10-28 provides them (prerequisite 2 below) |
| Not this procedure | `tools/activate_target.py` selects objdiff's workspace view for a target; it does not activate anything |

## Prerequisites

1. **The site's audit series is merged.** The target job's `Check the report` step uses the action `mitsevox/mitsevox.github.io/.github/actions/check-report@main`. Gate: `curl -sS -o /dev/null -w '%{http_code}\n' https://raw.githubusercontent.com/mitsevox/mitsevox.github.io/main/.github/actions/check-report/action.yml` prints `200`.
2. **This repository's kit sync is merged.** Gate: in a checkout of `main`, `python3 tools/matching_scope.py show | head -3` prints three lines of the form `<unit>: all` or `<unit>: none`, and `python3 tools/new_target.py --help` mentions `adopted`. If either command fails with `No such file` or an unknown option, the sync has not landed; stop.
3. **The checkout.** `git clone https://github.com/mitsevox/n100f && cd n100f && git switch -c activate-GIHE78-rev1`. Gate: `git status` says `On branch activate-GIHE78-rev1`, `nothing to commit, working tree clean`.
4. **The toolkit, built with the patch.** Follow the README's Building section, step 2 (clone decomp-toolkit at `v1.8.3`, apply `tools/patches/dtk-v1.8.3-rom-copy-info.patch`, `cargo build --release`). Gate: `decomp-toolkit/target/release/dtk --version` prints a line containing `1.8.3`.
5. **Your own copy of the rev0 original** at `orig/GIHE78-rev0/sys/main.dol`. Gate: `sha1sum orig/GIHE78-rev0/sys/main.dol` prints `c77fb29d39b5898933e8ef4d1ccd2ddac404554e`.
6. **The rev0 baseline builds.** `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && ninja -f build/GIHE78-rev0/build.ninja`. Gate: the last line is `build/GIHE78-rev0/main.dol: OK`.
7. **The checks pass on main.** `python3 -m unittest discover -s tests -p 'test_*.py' && python3 tools/targets.py check && python3 tools/versions.py check && python3 tools/evidence.py check`. Gate: the unittest line ends with `OK`; the three tools print, in order, a line starting `targets: 2 listed`, a line starting `versions: 2 targets`, and `evidence: no problems`.

## Step 1 (OWNER): the rev1 original into the build image

Files touched, all in `mitsevox/n100f-build`: `orig/GIHE78-rev1/main.dol`, `orig/GIHE78-rev1/main.dol.sha1`, `Dockerfile`, `.github/workflows/image.yml`, `README.md`.

1. Bring mitsevox/n100f-build#3 up to date with `main` (it predates #4): on the PR page, "Update branch", or locally `git switch archive/gc-usa-rev1-dol && git merge main`. Gate: the branch contains both `orig/GIHE78-rev0/main.dol` and `orig/GIHE78-rev1/main.dol`, and `sha1sum orig/GIHE78-rev1/main.dol` prints `cc84bc42b1054b572b5bb65968cf80b71ff41d0b`.
2. In `Dockerfile`, after the two rev0 lines, add:

    ```dockerfile
    COPY orig/GIHE78-rev1/main.dol /orig/GIHE78-rev1/sys/main.dol
    RUN echo "cc84bc42b1054b572b5bb65968cf80b71ff41d0b  /orig/GIHE78-rev1/sys/main.dol" | sha1sum -c -
    ```

3. In `.github/workflows/image.yml`, in the `Verify toolchain inside image` step, add the line `sha1sum /orig/GIHE78-rev1/sys/main.dol &&` after the rev0 `sha1sum` line.
4. Push, let the pull request's image workflow run, merge it. Gate: the `main` run of the image workflow is green and its summary prints one line `ghcr.io/mitsevox/n100f-build@sha256:<64 hex digits>`. Write that digest down: it is the value for step 6. Then, in the package `n100f-build` under your GitHub packages, confirm the visibility is still **private** and that `mitsevox/n100f` still has **Read** under Manage Actions access.

Hand the agent the digest and your copy of `orig/GIHE78-rev1/main.dol` by a private channel (never through the public repository, an issue, a pull request or a chat log that is published).

## Step 2 (AGENT): the rev1 original locally

Files touched: none in Git (`orig/*/*` is ignored).

1. Place the file at `orig/GIHE78-rev1/sys/main.dol`.
2. Gate: `sha1sum orig/GIHE78-rev1/sys/main.dol` prints `cc84bc42b1054b572b5bb65968cf80b71ff41d0b`, `stat -c %s orig/GIHE78-rev1/sys/main.dol` prints `2032480`, and `git status --short` prints nothing (the file is ignored). If `git status` lists anything under `orig/`, do not commit; stop and report.

## Step 3 (AGENT): scaffold the target's directory

Files touched: `config/GIHE78-rev1/config.yml` and `config/GIHE78-rev1/evidence.tsv` (written); `config/GIHE78-rev1/README.md` and `orig/GIHE78-rev1/.gitkeep` (adopted, unchanged).

1. `python3 tools/new_target.py GIHE78-rev1`
2. Gate: the output is exactly these four lines, in this order:

    ```text
    adopted config/GIHE78-rev1/README.md
    wrote config/GIHE78-rev1/config.yml
    wrote config/GIHE78-rev1/evidence.tsv
    adopted orig/GIHE78-rev1/.gitkeep
    ```

    A line starting `new_target: refusing to overwrite` means the directory holds something the tool does not know; stop and report the line.
3. Gate: `cat config/GIHE78-rev1/config.yml` prints

    ```text
    object_base: orig/GIHE78-rev1
    object: sys/main.dol
    hash: cc84bc42b1054b572b5bb65968cf80b71ff41d0b
    symbols: config/GIHE78-rev1/symbols.txt
    splits: config/GIHE78-rev1/splits.txt
    ```

    and `cat config/GIHE78-rev1/evidence.tsv` prints the single header line `kind	start	end	subject	origin	start_boundary	end_boundary	evidence` (tab separated).

## Step 4 (AGENT): pin every unit's scope to rev0

Files touched: `configure.py`.

1. Record the state before: `grep -c 'Object(Matching,' configure.py` prints a number; call it N (180 at commit 626918e; it changes with every merged unit, so use what you see). `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && cp build/GIHE78-rev0/build.ninja /tmp/before.ninja && cp build/GIHE78-rev0/objdiff.json /tmp/before.json`.
2. `python3 tools/matching_scope.py pin GIHE78-rev0`
3. Gate: the first output line is `pinned N Matching declaration(s) to MatchingFor(GIHE78-rev0); NonMatching, Equivalent and MatchingFor declarations untouched` with the same N. A line starting `matching_scope: Matching is used outside` means `configure.py` uses `Matching` in a way the tool does not rewrite; stop and report the listed lines.
4. Gate: `grep -c 'Object(Matching,' configure.py` prints `0`, and `grep -c 'Object(MatchingFor("GIHE78-rev0"),' configure.py` prints N.
5. Gate, the existing target is unchanged: `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && cmp build/GIHE78-rev0/build.ninja /tmp/before.ninja && cmp build/GIHE78-rev0/objdiff.json /tmp/before.json && echo identical` prints `identical` and nothing else; then `ninja -f build/GIHE78-rev0/build.ninja` ends with `build/GIHE78-rev0/main.dol: OK`.
6. Gate: `python3 tools/versions.py check` prints a line starting `versions: 2 targets` and no error (the two `MatchingFor` ids are known targets).

## Step 5 (AGENT): the first split of rev1, every unit NonMatching

Files touched: `config/GIHE78-rev1/symbols.txt`, `config/GIHE78-rev1/splits.txt` (written by the toolkit's analysis), `config/GIHE78-rev1/build.sha1`.

1. `python3 configure.py --version GIHE78-rev1 --dtk decomp-toolkit/target/release/dtk`. Gate: it exits 0 and `build/GIHE78-rev1/build.ninja` exists. If it refuses `--version GIHE78-rev1`, the target is not active in the registry yet: that is expected here, so set `"status": "active"` on the `GIHE78-rev1` entry of `config/targets.json` now (one word changed, nothing else) and run the command again.
2. `ninja -f build/GIHE78-rev1/build.ninja`. The toolkit's analysis of the new original writes the first `config/GIHE78-rev1/symbols.txt` and `splits.txt`, the way the rev0 map was first made (`config/GIHE78-rev0/README.md`, "Baseline"). Gate: the two files exist and are not empty (`wc -l config/GIHE78-rev1/symbols.txt config/GIHE78-rev1/splits.txt` prints two counts above 0), and the build ends with `build/GIHE78-rev1/main.dol: OK`. If the build stops earlier, copy the error text and stop: the known candidates for a rev1 difference are listed under "Unknowns" below, and which of them applies is the owner's decision, not a guess.
3. `echo "cc84bc42b1054b572b5bb65968cf80b71ff41d0b  build/GIHE78-rev1/main.dol" > config/GIHE78-rev1/build.sha1`. Gate: `python3 tools/targets.py check` prints `targets: 2 listed, every file and hash agrees`.
4. Gate, nothing links on rev1: `python3 tools/matching_scope.py show | grep -c ': all$'` prints `0`.
5. `ninja -f build/GIHE78-rev1/build.ninja progress build/GIHE78-rev1/report.json`. Gate: `python3 -c "import json; m=json.load(open('build/GIHE78-rev1/report.json'))['measures']; print(m.get('complete_code', 0), m.get('complete_data', 0))"` prints `0 0`.
6. `python3 tools/versions.py check && python3 tools/evidence.py check`. Gate: no error line; `evidence: no problems`.

## Step 6 (AGENT): activate, and let CI build both targets

Files touched: `config/targets.json` (status, done in step 5 if not before), `.github/workflows/validate.yml` (the digest), `config/GIHE78-rev1/README.md` (one sentence: the target is active from this pull request on; keep the rest), `README.md` (the Targets table row for rev1: `active`).

1. In `.github/workflows/validate.yml`, replace the digest in the `BUILD_IMAGE:` line with the one from step 1. Gate: `grep -c 'BUILD_IMAGE: ghcr.io/mitsevox/n100f-build@sha256:' .github/workflows/validate.yml` prints `1`, and the 64 hex digits are the owner's.
2. `git add -A && git status --short`. Gate: the list is exactly `configure.py`, `config/targets.json`, `config/GIHE78-rev1/config.yml`, `config/GIHE78-rev1/evidence.tsv`, `config/GIHE78-rev1/splits.txt`, `config/GIHE78-rev1/symbols.txt`, `config/GIHE78-rev1/build.sha1`, `config/GIHE78-rev1/README.md`, `README.md` and `.github/workflows/validate.yml`; nothing under `orig/` or `build/`. Anything else: stop and report.
3. Commit (`git commit`) with the subject `Activate GIHE78-rev1 at its byte-identical baseline (GIHE78-rev0, GIHE78-rev1)`, push the branch, and open a draft pull request with the same title. Its body: Scope `Tier A` (the pull request adds `splits.txt` and `symbols.txt`), the Targets table with one row per target (`GIHE78-rev0`: byte-identical yes, no unit touched, delta 0; `GIHE78-rev1`: byte-identical yes, every unit `NonMatching`, linked 0), the commands of steps 4 and 5 with their outputs under Evidence and validation, and the digest's provenance (the image run) under Review.
4. Gate, CI: the run shows the jobs `Plan affected targets`, `Tool tests`, `Validate GIHE78-rev0`, `Validate GIHE78-rev1` and `Validate targets`, all green. In `Validate GIHE78-rev1`, the step `Check the report` prints `checked 1 target(s)`. The `Check the scaffold` step prints no warning naming `GIHE78-rev1`. A red job: copy the failing step's log lines and stop.
5. Review as the tier requires (one accuracy record and one hostile record, each with `Targets: GIHE78-rev0, GIHE78-rev1`), then mark the pull request ready for the owner.

## Step 7 (OWNER): merge and publish

1. Merge the pull request (squash). Gate: the `main` run's `Validate GIHE78-rev1` job ends with `Publish to the site` printing `published n100f/GIHE78-rev1 <7-char sha>`, and `Validate GIHE78-rev0` prints its own `published` line.
2. Gate: `https://mitsevox.github.io/decomp-data/data/n100f/GIHE78-rev1/latest.json` exists and its `summary.code.linked` is `0`; `https://mitsevox.github.io/games/n100f/?target=GIHE78-rev1` shows the target; `https://mitsevox.github.io/games/n100f/` still shows rev0's numbers, unchanged.
3. Gate, settings: `python3 tools/repo_settings.py --repo mitsevox/n100f --check` from a checkout of the site repository reports `no drift`.

After this step the target is active and the procedure is complete. Carrying rev0's map to rev1 is a separate pull request, specified in the wiki chapter (the transfer tool does not exist yet and is the first of the unknowns below); nothing in this checklist copies rev0's splits, symbols or progress to rev1.

## Unknowns that only the rev1 bytes answer

These cannot be settled from the documentation; they are what step 5 finds out, and each is reported, never guessed around.

1. Whether rev1 enters through a routine in `.text` like rev0 (entry point `0x80016898`), so that the `_rom_copy_info` patch to dtk is needed for rev1 too, or enters through `__start` like a stock DOL, in which case the patched dtk still works but the fact is recorded in `config/GIHE78-rev1/README.md`.
2. Whether `_rom_copy_info` and `_bss_init_info` sit in the section that holds `__start`, as on rev0, and whether rev1's `__start` keeps its address (`0x80005304` on rev0) or moves; the relink depends on the name at that address.
3. Whether rev1's relink needs rev0's extra `config.yml` lines (`mw_comment_version: 10`, `ldscript_template` with the debugger stack at `_stack_addr + 0x1000`, the `extract:` entry for `CardData`). The scaffold writes none of them; if the first relink is not byte-identical, these are the candidates, and copying any of them is a decision with its own evidence row.
4. The address deltas: the DOL is 67,968 bytes larger than rev0; which sections grew and by how much is the first thing the transfer tool must measure.
5. Which functions and data differ in bytes between the two revisions; nothing is known until the transfer's anchoring runs.
6. Whether every library profile of rev0 (CodeWarrior GC/1.3.2 game code, GC/1.2.5n SDK, GC/1.3 MetroTRK and MSL, ProDG for Bink) holds for rev1; a profile that differs is a `compiler` row in `config/GIHE78-rev1/evidence.tsv` and a profile keyed by `config.version` in `configure.py`.

## How to report back

Report in this form, one entry per step or gate you ran, in order, and stop at the first failed gate:

```text
Step <number>, gate <number>
Command: <the exact command line>
Actual output: <the lines the gate names, verbatim; the last 20 lines when the gate names none>
Expected output: <what this checklist says the gate expects>
Result: PASS or FAIL
```

Then: the branch name and the head commit (`git rev-parse HEAD`), the pull request URL if one exists, `git status --short` as it stands, and, for a FAIL, the full text of the failing command's output and which of the unknowns above it looks like, if any, without acting on it. Never paste the contents of an original executable, and never attach one.
