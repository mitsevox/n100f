# Onboarding GIHE78-rev1: the checklist

The procedure that takes `GIHE78-rev1` (Scooby-Doo! Night of 100 Frights, GameCube, USA, disc revision 1) from `archived` to an active, published target of this repository. It is written for someone with no other context: every step names the command, the files it touches and the output it must produce, and ends with a gate. **A failed gate means stop, write down what happened (the "How to report back" section at the end) and report. Never improvise past a gate.**

The generic version of this procedure is the wiki chapter [Multiple targets](https://mitsevox.github.io/wiki/multi-target/); this file is the one to follow here, because it carries this repository's facts.

## Who does what

- **AGENT** steps are done in a checkout of this repository, on a branch, and end in a pull request. The agent never merges, never pushes to `main`, never changes repository settings, the build image or its digest, and never commits an original executable.
- **OWNER** steps are done by the project owner (`mitsevox`) on GitHub or in the private build repository. They are marked `OWNER` and give the exact clicks. The agent stops at each of them and reports.
- Step 5 is the agent's as written; it reads the toolkit's output, and any change it turns out to need beyond its sub-steps is the owner's decision, reported under "Unknowns".

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
| Generated build files | This repository's `tools/project.py` writes `build.ninja` and `objdiff.json` under `build/<id>/` (stock dtk-template writes them at the repository root); every command below uses the per-target paths |
| Rev0's inputs beyond the scaffold | `config/GIHE78-rev0/config.yml` has `mw_comment_version: 10`, `ldscript_template: config/GIHE78-rev0/ldscript.tpl` and an `extract:` entry for `CardData`; `tools/project.py` passes the entry point to the linker for every target (rev0 README, "Baseline") |
| Tools this procedure needs | `tools/new_target.py`, `tools/matching_scope.py`, `tools/versions.py`, `tools/targets.py` as the kit sync of 2026-10-28 provides them (prerequisite 2 below) |
| Not this procedure | `tools/activate_target.py` selects objdiff's workspace view for a target; it does not activate anything |

## Prerequisites

Prerequisites 1 to 4 are states of `main` and the owner's approvals; check them before cloning, so that the branch never needs `main` merged in later.

1. **The site's audit series is merged.** The target job's `Check the report` step uses the action `mitsevox/mitsevox.github.io/.github/actions/check-report@main`. Gate: `curl -sS -o /dev/null -w '%{http_code}\n' https://raw.githubusercontent.com/mitsevox/mitsevox.github.io/main/.github/actions/check-report/action.yml` prints `200`.
2. **This repository's kit sync is merged.** Gate: in a checkout of `main`, `python3 tools/matching_scope.py show | head -3` prints three lines of the form `<unit>: all` or `<unit>: none`, `python3 tools/new_target.py --help` mentions `adopted`, `grep -c 'check-report@main' .github/workflows/validate.yml` prints `1`, `grep -c 'measuring_tools.py --verify' .github/workflows/validate.yml` prints `1` and `grep -c 'matching_scope.py check' .github/workflows/validate.yml` prints `1`. If a command fails with `No such file` or an unknown option, or a count is `0`, the sync has not landed; stop.
3. **OWNER: step 1 is complete and on `main`.** The owner's digest pull request (step 1, sub-step 5) is merged. Gate: `grep 'BUILD_IMAGE:' .github/workflows/validate.yml` on `main` prints a digest other than `502c957567a21fd77283fad6d437ae3b6cf19856a8b456157040b6d36cb83773`, and the image run that printed it is linked from that pull request.
4. **OWNER: downloads and the Rust toolchain approved.** AGENTS.md says to ask the owner before downloads, and this procedure makes some: prerequisite 6 clones decomp-toolkit and `cargo build` fetches its crates, and the first build of a target (the first `ninja`, not `configure.py`) downloads the tools `configure.py` pins (objdiff at its tag, the compilers pack, binutils and, on Linux, wibo at `wibo_tag`, which `tools/project.py` fetches) unless `--objdiff`, `--compilers`, `--binutils` and `--wrapper` (this repository's `configure.py` flag for wibo) name local copies. Gate: the owner's written approval of those downloads, or the local paths to use instead, in the task's issue or the chat the report goes to; and `cargo --version` prints a version (a Rust toolchain installed, or its installation approved the same way).
5. **The checkout.** `git clone https://github.com/mitsevox/n100f && cd n100f && git switch -c activate-GIHE78-rev1`. Gate: `git status` says `On branch activate-GIHE78-rev1`, `nothing to commit, working tree clean`.
6. **The toolkit, built with the patch.** Follow the README's Building section, step 2 (clone decomp-toolkit at `v1.8.3`, apply `tools/patches/dtk-v1.8.3-rom-copy-info.patch`, `cargo build --release`). Gate: `decomp-toolkit/target/release/dtk --version` prints a line containing `1.8.3`; quote the line in the report.
7. **Your own copy of the rev0 original** at `orig/GIHE78-rev0/sys/main.dol`. Gate: `sha1sum orig/GIHE78-rev0/sys/main.dol` prints `c77fb29d39b5898933e8ef4d1ccd2ddac404554e`.
8. **The rev0 baseline builds.** `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && ninja -f build/GIHE78-rev0/build.ninja`. Gate: the last line is `build/GIHE78-rev0/main.dol: OK`.
9. **The checks pass on main.** `python3 -m unittest discover -s tests -p 'test_*.py' && python3 tools/targets.py check && python3 tools/versions.py check && python3 tools/evidence.py check && python3 tools/matching_scope.py check`. Gate: the unittest run ends with `OK (skipped=1)` (the ProDG wrapper test skips without the toolchain); the four tools print, in order, a line starting `targets: 2 listed`, a line starting `versions: 2 targets`, `evidence: no problems` and `matching_scope: no problems`.

## Step 1 (OWNER): the rev1 original into the build image

Files touched, in `mitsevox/n100f-build`: `orig/GIHE78-rev1/main.dol`, `orig/GIHE78-rev1/main.dol.sha1`, `Dockerfile`, `.github/workflows/image.yml`, `README.md` (its list of the originals the image holds gains rev1: the path `/orig/GIHE78-rev1/sys/main.dol`, the SHA-1 and the size, in the form of rev0's entry); in `mitsevox/n100f`: `.github/workflows/validate.yml` (sub-step 5, the digest line).

1. Bring mitsevox/n100f-build#3 up to date with `main` (it predates #4): on the PR page, "Update branch", or locally `git switch archive/gc-usa-rev1-dol && git merge main`. Gate: the branch contains both `orig/GIHE78-rev0/main.dol` and `orig/GIHE78-rev1/main.dol`, and `sha1sum orig/GIHE78-rev1/main.dol` prints `cc84bc42b1054b572b5bb65968cf80b71ff41d0b`.
2. In `Dockerfile`, after the two rev0 lines, add:

    ```dockerfile
    COPY orig/GIHE78-rev1/main.dol /orig/GIHE78-rev1/sys/main.dol
    RUN echo "cc84bc42b1054b572b5bb65968cf80b71ff41d0b  /orig/GIHE78-rev1/sys/main.dol" | sha1sum -c -
    ```

3. In `.github/workflows/image.yml`, in the `Verify toolchain inside image` step, add the line `sha1sum /orig/GIHE78-rev1/sys/main.dol &&` after the rev0 `sha1sum` line.
4. Push, let the pull request's image workflow run, merge it. Gate: the `main` run of the image workflow is green and its summary prints one line `ghcr.io/mitsevox/n100f-build@sha256:<64 hex digits>`. Then, in the package `n100f-build` under your GitHub packages, confirm the visibility is still **private** and that `mitsevox/n100f` still has **Read** under Manage Actions access.
5. The digest into this repository, by you: on a branch of `mitsevox/n100f`, replace the 64 hex digits in the `BUILD_IMAGE:` line of `.github/workflows/validate.yml` with the digest from sub-step 4, keep the comment above it naming the image run, and open a pull request titled `Build image with the rev1 original (GIHE78-rev0)` with the Scope line `Tier C` (one line of workflow, no source); merge it (squash). Nobody but the owner changes the digest, which is why this is not part of the agent's step 6. This needs this repository's kit sync merged first (prerequisite 2), because the gate relies on its workflow: `grep -c 'check-report@main' .github/workflows/validate.yml` and `grep -c 'measuring_tools.py --verify' .github/workflows/validate.yml` each print `1` on `main` (today `List measuring tools` runs on `main` only and verifies nothing). Gate: on that pull request `Validate GIHE78-rev0` is green with the new image, its `List measuring tools` step included (the image's dtk still reports `1.8.3`), and after the merge `grep 'BUILD_IMAGE:' .github/workflows/validate.yml` on `main` prints the new digest. Collision risk: none, one line of `validate.yml`.

Hand the agent your copy of `orig/GIHE78-rev1/main.dol` by a private channel (never through the public repository, an issue, a pull request or a chat log that is published); the digest is public in `validate.yml`.

## Step 2 (AGENT): the rev1 original locally

Files touched: none in Git (`orig/*/*` is ignored).

1. Place the file at `orig/GIHE78-rev1/sys/main.dol`.
2. Gate: `sha1sum orig/GIHE78-rev1/sys/main.dol` prints `cc84bc42b1054b572b5bb65968cf80b71ff41d0b`, `stat -c %s orig/GIHE78-rev1/sys/main.dol` prints `2032480`, and `git status --short` prints nothing (the file is ignored). If `git status` lists anything under `orig/`, do not commit; stop and report.

## Step 3 (AGENT): scaffold the target's directory

Files touched: `config/GIHE78-rev1/config.yml` and `config/GIHE78-rev1/evidence.tsv` (written); `config/GIHE78-rev1/README.md` and `orig/GIHE78-rev1/.gitkeep` (adopted, unchanged).

1. `python3 tools/new_target.py GIHE78-rev1`
2. Gate: the output is exactly these four lines, in this order:

    ```text
    adopted config/GIHE78-rev1/README.md (a placeholder written while the target waited: rewrite it, its status and identity change when the target is activated)
    wrote config/GIHE78-rev1/config.yml
    wrote config/GIHE78-rev1/evidence.tsv
    adopted orig/GIHE78-rev1/.gitkeep
    ```

    A line starting `new_target: refusing to overwrite` means the directory holds something the tool does not know; stop and report the line. The tool runs once: run again after it wrote its files, it prints `new_target: refusing to overwrite config/GIHE78-rev1/README.md`, because the README is no longer the directory's only file; that is expected and needs no action. The README is rewritten in step 6.
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

1. Record the state before: `grep -c 'Object(Matching,' configure.py` prints a number; call it N (180 at commits 626918e, 81e3273 and 56b8521; it changes with every merged `Matching` unit, so use what you see). `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && cp build/GIHE78-rev0/build.ninja /tmp/before.ninja && cp build/GIHE78-rev0/objdiff.json /tmp/before.json`.
2. `python3 tools/matching_scope.py pin GIHE78-rev0` (rev0 is the only active target, so the full set the tool requires is this one id).
3. Gate: the first output line is `pinned N Matching declaration(s) to MatchingFor(GIHE78-rev0); NonMatching, Equivalent and MatchingFor declarations untouched` with the same N; the second line says what to compare, which sub-step 5 does. A line starting `matching_scope: Matching is used outside` or `matching_scope: Object calls whose first argument is not` means `configure.py` uses `Matching`, or declares a unit, in a way the tool does not rewrite; stop and report the listed lines.
4. Gate: `grep -c 'Object(Matching,' configure.py` prints `0`, and `grep -c 'Object(MatchingFor("GIHE78-rev0"),' configure.py` prints N.
5. Gate, the existing target is unchanged: `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && cmp build/GIHE78-rev0/build.ninja /tmp/before.ninja && cmp build/GIHE78-rev0/objdiff.json /tmp/before.json && echo identical` prints `identical` as its last line (configure prints its own lines first; `cmp` prints nothing when the files agree); then `ninja -f build/GIHE78-rev0/build.ninja` ends with `build/GIHE78-rev0/main.dol: OK`.
6. Gate: `python3 tools/versions.py check` prints a line starting `versions: 2 targets` and no error (the two `MatchingFor` ids are known targets).

## Step 5 (AGENT): the first split of rev1, every unit NonMatching

This step reads the toolkit's output. Stop at the first failed gate and report; every change beyond the ones written here is the owner's decision.

Files touched: `config/targets.json` (one word), `config/GIHE78-rev1/config.yml` (two keys appended), `config/GIHE78-rev1/ldscript.tpl` (copied), `config/GIHE78-rev1/build.sha1`, `config/GIHE78-rev1/symbols.txt` and `splits.txt` (written by the toolkit's analysis), `config/GIHE78-rev1/evidence.tsv` (the rows of sub-step 9).

1. Set `"status": "active"` on the `GIHE78-rev1` entry of `config/targets.json`, one word changed, nothing else: `configure.py` takes its version list from `tools/targets.py` and accepts active targets only. Gate: `git diff --stat -- config/targets.json` ends with `1 file changed, 1 insertion(+), 1 deletion(-)`. From here `python3 tools/targets.py check` reports the target's missing files until sub-step 6; that is expected.
2. Copy rev0's inputs beyond the scaffold (Facts table): `cp config/GIHE78-rev0/ldscript.tpl config/GIHE78-rev1/ldscript.tpl`, then `printf 'mw_comment_version: 10\nldscript_template: config/GIHE78-rev1/ldscript.tpl\n' >> config/GIHE78-rev1/config.yml`. Do not copy the `extract:` entry: it names the symbol `CardData`, which a toolkit-generated map does not have; it returns with the map transfer (the note after step 7). Gate: `tail -n 2 config/GIHE78-rev1/config.yml` prints the two lines, and `cmp config/GIHE78-rev0/ldscript.tpl config/GIHE78-rev1/ldscript.tpl` prints nothing. Both keys are hypotheses (unknown 3) until sub-step 5 passes.
3. `echo "cc84bc42b1054b572b5bb65968cf80b71ff41d0b  build/GIHE78-rev1/main.dol" > config/GIHE78-rev1/build.sha1` (one line in `sha1sum` format: the SHA-1, two spaces, the built path), before the first build: the build reads it, its hash check being `dtk shasum -c` against this file (`tools/project.py`, the `check` rule), and ninja stops on a missing input. Gate: `python3 tools/targets.py check` prints exactly two problem lines, `config/GIHE78-rev1/splits.txt is missing` and `config/GIHE78-rev1/symbols.txt is missing`, and exits 1 (expected: the two files come from the first build).
4. `python3 configure.py --version GIHE78-rev1 --dtk decomp-toolkit/target/release/dtk`. Gate: it exits 0 and `build/GIHE78-rev1/build.ninja` exists.
5. `ninja -f build/GIHE78-rev1/build.ninja`. The first build runs the patched toolkit's analysis of the new original, which writes the first `config/GIHE78-rev1/symbols.txt` and `splits.txt`, the way the rev0 map was first made (`config/GIHE78-rev0/README.md`, "Baseline"); dtk-template's Getting Started (`docs/getting_started.md`, Creating a New Project) describes that first run and says the project may not build yet at that point. The patch is in the binary from prerequisite 6, and `tools/project.py` passes the entry point to the linker for every target. Keep the whole log: the report asks for it. Gate: the two files exist and are not empty (`wc -l config/GIHE78-rev1/symbols.txt config/GIHE78-rev1/splits.txt` prints two counts above 0), and the build ends with `build/GIHE78-rev1/main.dol: OK`. If the build stops earlier, copy the error text and stop: the candidates for a rev1 difference are listed under "Unknowns" below, and which of them applies is the owner's decision, not a guess.
6. Gate: `python3 tools/targets.py check` prints `targets: 2 listed, every file and hash agrees`.
7. Gate, nothing links on rev1: `python3 tools/matching_scope.py show | grep -c ': all$'` prints `0` (and exits 1, which is grep's way of saying that no line matched: expected here).
8. `ninja -f build/GIHE78-rev1/build.ninja progress build/GIHE78-rev1/report.json`. The measurement's `all_source` compiles the source of the units the target's split names (`tools/project.py`, `add_unit`: a unit is built from source only when `configure.py` declares an object of the same name), and a toolkit-generated split names none of this project's units, so nothing compiles on rev1 and no unit is scored above zero. Gate: `python3 -c "import json; m=json.load(open('build/GIHE78-rev1/report.json'))['measures']; print(m.get('complete_code', 0), m.get('complete_data', 0))"` prints `0 0` (the report omits zero values, so an absent count is 0).
9. The evidence rows for what this step did, appended to `config/GIHE78-rev1/evidence.tsv` (tab separated; columns, kinds and origins as `config/README.md` defines them, and its rule for tool output: one row per tool run, kind `name` or `unit-anchor`, origin `signature` or `related`, the subject naming the tool with its inputs and revision, the evidence the path of the regenerable output):
    - kind `name`, start `-`, end `-`, subject `dtk v1.8.3 + tools/patches/dtk-v1.8.3-rom-copy-info.patch, dol split analysis of main.dol SHA-1 cc84bc42b1054b572b5bb65968cf80b71ff41d0b`, origin `signature`, both edges `-`, evidence: `Tool: dtk v1.8.3 with tools/patches/dtk-v1.8.3-rom-copy-info.patch, dol split analysis; signatures: dtk's built-in set at v1.8.3, no external signature file. Output: config/GIHE78-rev1/symbols.txt and splits.txt, regenerated by deleting both and building again: every unit auto-generated, every function a neutral tool label, no source boundary claimed; the relink is byte-identical.` (`config/README.md` asks a signature claim for the tool version and the signature identifier: this is both.)
    - kind `linker`, start `-`, end `-`, subject `config/GIHE78-rev1/ldscript.tpl`, origin `inferred`, both edges `-`, evidence: `Copied from config/GIHE78-rev0/ldscript.tpl (debugger stack at _stack_addr + 0x1000, rev0's linker rows) before the first build; not tested without it.` (or what was tested, if the owner asked for the test).
    - kind `compiler`, start `-`, end `-`, subject `mw_comment_version`, origin `related`, both edges `-`, evidence: `10, rev0's value (GC/1.3 and GC/1.3.2 write .comment version 10); whether rev1's units were compiled by the same profiles is unknown 6.`

    Gate: `python3 tools/versions.py check && python3 tools/evidence.py check` prints a line starting `versions: 2 targets` and then `evidence: no problems`.

## Step 6 (AGENT): activate, and let CI build both targets

Files touched: `config/GIHE78-rev1/README.md` (rewritten from the placeholder), `README.md` (the Targets table row for rev1: `active`). Not `.github/workflows/validate.yml`: the owner changed its digest in step 1.

1. Gate first, right before the commit: `python3 tools/matching_scope.py show | grep -c ': all$'` prints `0` (and exits 1) once more. A unit pull request merged to `main` after the branch was created can add a plain `Object(Matching, ...)`, which from this pull request on claims rev1; `Tool tests` runs `python3 tools/matching_scope.py check` on the pull request's merge with `main` and fails on such a unit once both targets are active. Sub-step 6 is the remedy.
2. Rewrite `config/GIHE78-rev1/README.md` in the form of rev0's: the target's facts (size and SHA-1 from step 2; the entry point from the `Entry point` line of `decomp-toolkit/target/release/dtk dol info orig/GIHE78-rev1/sys/main.dol`, which prints the DOL header), the file table, and a "Baseline" section saying what the first relink needed (the two keys of step 5.2, and anything the owner decided under the unknowns). In `README.md` (Targets), set rev1's row to `active` and replace the sentence `` `GIHE78-rev1` is identity metadata only: no build and no progress. `` with rev1's facts in the form of rev0's sentence (file, size, entry point, and whether the disc has a linker map or symbols).
3. `git add -A && git status --short`. Gate: the list is exactly `configure.py`, `config/targets.json`, `config/GIHE78-rev1/config.yml`, `config/GIHE78-rev1/ldscript.tpl`, `config/GIHE78-rev1/evidence.tsv`, `config/GIHE78-rev1/splits.txt`, `config/GIHE78-rev1/symbols.txt`, `config/GIHE78-rev1/build.sha1`, `config/GIHE78-rev1/README.md` and `README.md`; nothing under `orig/` or `build/`, and not `.github/workflows/validate.yml`. Anything else: stop and report.
4. Commit (`git commit`) with the subject `Activate GIHE78-rev1 at its byte-identical baseline (GIHE78-rev0, GIHE78-rev1)`, push the branch, and open a draft pull request with the same title. Its body follows `.github/pull_request_template.md`: Scope `Tier A` (the pull request adds `splits.txt` and `symbols.txt`; `tools/tier.py` fails a lower claim); the Targets table with one row per target (`GIHE78-rev0`: byte-identical yes, no unit touched, delta 0, no evidence rows; `GIHE78-rev1`: byte-identical yes, every unit `NonMatching`, linked 0, the three rows of step 5.9); the commands of steps 4 and 5 with their outputs under Evidence and validation; under Review, the digest `validate.yml` names and the owner's pull request that set it.
5. Gate, CI: the run shows the jobs `Plan affected targets`, `Tool tests`, `Validate GIHE78-rev0`, `Validate GIHE78-rev1` and `Validate targets`, all green. In `Validate GIHE78-rev1`, the step `Check the report` prints a line starting `checked 1 target(s)`. The `Check the scaffold` step prints no warning naming `GIHE78-rev1`. A red job: copy the failing step's log lines and stop, except `Tool tests` failing on `matching_scope.py check`, which sub-step 6 handles.
6. If `Tool tests` fails on `matching_scope.py check` (a unit pull request merged to `main` after the branch was created), with the tree committed: `git fetch origin && git merge origin/main` (a merge commit; no rebase, no force-push). If that prints `CONFLICT` (a unit line inserted between pinned lines, or a promotion next to one, does this): `git merge --abort`, confirm that `git status --short` prints nothing on the committed branch, then report the conflicting lines (`git diff HEAD...origin/main -- configure.py`) and stop; resolving a conflict in `configure.py` is the owner's decision, not this procedure's. Without a conflict, record the merged state before the edit: `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && cp build/GIHE78-rev0/build.ninja /tmp/merged.ninja && cp build/GIHE78-rev0/objdiff.json /tmp/merged.json`. `python3 tools/matching_scope.py check` lists the lines; change each listed `Object(Matching,` to `Object(MatchingFor("GIHE78-rev0"),` with an editor (`configure.py` is Python: no sed and no scripted rewrite, and `pin` refuses now that two targets are active). Gate: `python3 tools/matching_scope.py check` prints `matching_scope: no problems`; `python3 configure.py --version GIHE78-rev0 --dtk decomp-toolkit/target/release/dtk && cmp build/GIHE78-rev0/build.ninja /tmp/merged.ninja && cmp build/GIHE78-rev0/objdiff.json /tmp/merged.json && echo identical` prints `identical` as its last line and `ninja -f build/GIHE78-rev0/build.ninja` ends with `build/GIHE78-rev0/main.dol: OK` (rev0 unchanged by the edit); `python3 tools/matching_scope.py show | grep -c ': all$'` prints `0` (and exits 1, as in 5.7). Commit with the subject `Pin the units merged meanwhile to GIHE78-rev0`, push, and return to sub-step 5.
7. Report and stop. Tier A needs, on the final revision, one accuracy record, which here is the justified `N/A` form (`Accuracy: N/A - every symbol on GIHE78-rev1 is a toolkit label; no name or boundary is proposed`), and one hostile record, whose `Unit boundaries` row is answered by the tool-output row of step 5.9; each starts `Revision: <full sha>` and `Targets: GIHE78-rev0, GIHE78-rev1`. Both come from reviewers who did not write this pull request: an author cannot review itself, so do not post them yourself. Ask the owner for the independent reviewers in the report; the pull request stays a draft until both records are posted, and the owner marks it ready.

## Step 7 (OWNER): merge and publish

1. Merge the pull request (squash). Gate: the `main` run's `Validate GIHE78-rev1` job ends with `Publish to the site` printing `published n100f/GIHE78-rev1 <sha7>` (the first seven characters of the merge commit), and `Validate GIHE78-rev0` prints its own `published` line.
2. Gate: `https://mitsevox.github.io/decomp-data/data/n100f/GIHE78-rev1/latest.json` exists and its `summary.code.linked` and `summary.data.linked` are `0`; `https://mitsevox.github.io/games/n100f/?target=GIHE78-rev1` shows the target; `https://mitsevox.github.io/games/n100f/` still shows rev0's numbers, unchanged.
3. Gate, settings: `python3 tools/repo_settings.py --repo mitsevox/n100f --check` from a checkout of the site repository reports `no drift`.

After this step the target is active and the procedure is complete. From here every unit pull request declares the targets a unit links on: `Object(MatchingFor("GIHE78-rev0"), ...)`, and `Object(MatchingFor("GIHE78-rev0", "GIHE78-rev1"), ...)` once it links on rev1 too. Plain `Matching` is the form of a repository with one active target only: `Tool tests` runs `python3 tools/matching_scope.py check`, which fails it while two or more targets are active. Carrying rev0's map to rev1 is a separate pull request, specified in the wiki chapter (the transfer tool does not exist yet and is the first of the unknowns below); nothing in this checklist copies rev0's splits, symbols or progress to rev1. That transfer is also where rev0's `extract:` entry for `CardData` returns: when `CARDUnlock.c` is named on rev1, its `CardData` (the 352-byte DSP program at `.data:0x801DC580` on rev0) needs its rev1 address, a `data` row for the bytes and a `name` row carried from rev0, and only then does that unit's source compile on rev1.

## Unknowns that only the rev1 bytes answer

These cannot be settled from the documentation; they are what step 5 finds out, and each is reported, never guessed around.

1. Whether rev1 enters through a routine in `.text` like rev0 (entry point `0x80016898`), so that the `_rom_copy_info` patch to dtk is needed for rev1 too, or enters through `__start` like a stock DOL, in which case the patched dtk still works but the fact is recorded in `config/GIHE78-rev1/README.md`.
2. Whether `_rom_copy_info` and `_bss_init_info` sit in the section that holds `__start`, as on rev0, and whether rev1's `__start` keeps its address (`0x80005304` on rev0) or moves; the relink depends on the name at that address.
3. Whether rev1's relink needs the two keys step 5.2 copies from rev0 (`mw_comment_version: 10`, `ldscript_template` with the debugger stack at `_stack_addr + 0x1000`). They are copied before the first build, so a byte-identical relink does not say whether they were needed; the rows of step 5.9 say `not tested without it` unless the owner asks for the test.
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

Then: the commit of this checklist you followed (`git log -1 --format=%H -- docs/onboarding-GIHE78-rev1.md` on `main`); the `dtk --version` line (prerequisite 6) and N (step 4); the full log of step 5.5, the first build with its analysis; the answers to unknowns 1 and 2 as the toolkit's output and `dtk dol info` show them, on PASS too; the branch name and the head commit (`git rev-parse HEAD`), the pull request URL if one exists, `git status --short` as it stands; and, for a FAIL, the full text of the failing command's output and which of the unknowns above it looks like, if any, without acting on it. Never paste the contents of an original executable, and never attach one.
