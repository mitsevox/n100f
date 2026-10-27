# Target-scoped shared-site reports

The game has one site entry, with independent targets selected by game ID/region and disc
revision. `config/targets.json` is the versioned inventory. `GIHE78-rev0` is active and supports
report-v2; `GIHE78-rev1` is archived identity metadata only, without a validated report or build.
The report-v2 consumer must be deployed before this producer cutover.

## CI and independent publication

`Validate` runs on PRs, main pushes and manual audits:

1. `Plan affected targets` evaluates the full changed-path diff using `config/ci-targets.json`.
   Deleted paths and both sides of renames count. Shared inputs select their dependents;
   unknown inputs conservatively select all buildable targets. Docs-only and archived-only
   edits run tool tests without binary builds. Manual audits select all buildable targets.
2. `Tool tests` validates adapters/planning/review-gate logic.
3. Each selected `Validate <targetId>` job runs independently with `fail-fast: false`. It provides
   only that target's original, verifies its SHA-1, configures, builds and hash-checks the complete
   executable, measures with objdiff, checks the ProDG wrapper, exports, packages and uploads
   `report-v2-<targetId>`. All outputs, including Ninja/objdiff configurations, are target-local.
4. `Validate targets` aggregates required jobs for the merge gate. A failed required target
   blocks the PR. The owner coordinates replacing the old `Build GIHE78` required check with
   this stable check; neither branch protection nor owner clearance is changed by this PR.

Caches and publication reuse remain disabled. Compiler versions, profiles and comparison
methodology remain unchanged. `config/<id>/build.json` records stable numeric/macro build
constants; rev0 retains `BUILD_VERSION=0` and `VERSION_GIHE78` despite its explicit directory name.

The site authenticates the exact main run/attempt, workflow blob, target job/required steps,
shared prerequisites, source tree and artifact digest. A successful target may publish from a
completed workflow with a failed sibling. It cannot publish when its own validation or shared
prerequisites fail. PR packages remain evidence only; cancelled workflows cannot publish.

## Output and identity

`tools/public_report.py inputs --version <id>` writes summary/map/timeline JSON under
`build/<id>/public-input/`. `metadata --version <id>` writes the exact canonical target object,
full Git revision/tree, approved tool identities, methodology/generation, measurement/publication
UTC times and GitHub run/attempt/workflow blob. The pinned site packager produces a strict
report-v2 manifest and execution-v2 receipt under `build/<id>/public-report/`.

The target object carries `id`, `platform`, `gameId`, `region`, integer `discRevision`, and original
executable `sha1`. Git `revision` remains a commit SHA. Map-v1 and timeline-v1 payloads retain
their existing geometry and metric definitions. Code/data fuzzy are independently measured;
linked, exact and fuzzy bytes are never summed. Descriptive names and exact inventories keep
existing definitions. Contributor data remains unsupported; contribution acceptance stays off.

## History and legacy adoption

History is independent per target. The exporter searches completed main runs for a successful
job and matching target artifact, including a successful target in an otherwise failed run,
and restores accepted target-specific site history. It fails on target identity/checksum or
history mismatch. It never uses another revision's measurements.

The registry declares one exact old `GIHE78`/GameCube/executable-SHA tuple as the rev0 alias.
Only rev0 may restore legacy `public-report` history. All old rows remain byte-for-byte unchanged;
old manifests, receipts, policies and snapshot paths stay immutable. New reports use the explicit
rev0 identity without changing methodology/generation because metric meaning has not changed.
Rev1 begins with no history and cannot inherit rev0 rows.

A network/history recovery failure stops export. An expired unpublished artifact is not permission
to restart history; restore accepted history explicitly. Main runs queue without cancellation.

## Policy and rollout

`config/<id>/producer.json` is copied verbatim to the site's per-target policy. It approves the
exact workflow blob, artifact/job/step names, target/hash, shared jobs and tools. Every workflow,
build-image, dtk-patch or objdiff-tag change requires a matching reviewed policy update. Obtain
the workflow Git blob with `git hash-object .github/workflows/validate.yml` after final edits.

Rollout order:

1. Consumer support lands on the site, leaving the current v1 feed active.
2. The owner merges the private-original layout PR; its verified main image digest is pinned here.
3. Final producer CI/reviews pass, the owner coordinates its required check and merges this change.
4. The site copies the exact producer policy and target registry, archives its v1 policy, adopts
   the immutable legacy pointer via the explicit alias, and imports a real main v2 package.
5. Confirm all old history rows and metrics persist before declaring the cutover complete.

`notify-portfolio.yml` uses the reviewed v2 notifier. It only requests a refresh; site import
still performs full authentication. Policy-sync has one explicitly registered source per target;
it proposes changes and never approves new targets or merges them.
