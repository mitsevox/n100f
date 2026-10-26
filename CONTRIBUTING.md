# Contributing

Recover plausible original source, supported by evidence. A byte match establishes compiler equivalence; it does not establish correct names, types, source boundaries, or intent. The [README](README.md) identifies the target, project owner, and build setup.

## Setup and authority

Contributions contain source, tools, and reviewed metadata. Do not publish original game binaries, disc images or assets. Symbol and debug-information extractions from the reference builds are published in `references/` by the owner's decision; they are corroborating references, not target evidence. Keep experiments and bulk research out of public Git.

Clone the repository and follow the [build setup](README.md#building). Arrange access to any required reference material with the project owner. Checkout locations are a local choice; a contributor workflow must not depend on another person's absolute paths.

Use your own checkout and contribution branch:

```sh
git clone https://github.com/mitsevox/n100f.git
cd n100f
git switch -c your-task
```

Setup, baseline and build commands are listed in the README once implemented.

The supported hosts and target hash are in the README. Missing reference access is a setup issue for the owner, not a reason to change supporting dependencies. Reserve a bounded active task under [work coordination](#work-coordination); identify dependency branches and coordinate overlapping edits before starting.

Contribution branches, PRs, reviews, and required CI checks belong in this repository. Use the owner-approved build environment at its pinned version. Supporting build dependencies are maintained by the owner; contributors and agents must not change them to accommodate a patch. Report missing inputs or tooling problems in an issue here for the owner's decision.

**Only the project owner authorizes and performs merges into `main`.** Contributors and agents may prepare branches, submit PRs, and review changes. A reviewer verdict or passing CI never grants merge authority. Contributors must not push directly to `main` or enable automatic merging.

## Work coordination

Backlog and research issues do not reserve code. Older subsystem or address-range claims are backlog references, not exclusive reservations; retain only the batch currently being worked on.

Before starting, check open PRs and active branches, then record a bounded reservation in an issue or draft PR: the specific functions or unit action, your branch, and shared-file dependencies. Reserve current work, not an entire subsystem or future batches. Existing partial source is open to improvement and promotion unless that scope is actively reserved; preserve original contribution credit.

A reservation expires after 48 hours without a meaningful progress update. Renew it with a link to pushed work, concrete findings, or an explicit handoff. Release scope you pause or abandon, and check existing branches and PRs before taking over expired work so useful work is preserved rather than duplicated.

Coordinate shared source files, headers, types, tables and compilation-group changes even when function scopes differ. Record how overlapping edits will be separated or integrated. An old claim alone does not require permission to proceed; a live overlap requires coordination. These reservations prevent conflicting edits, not ownership of the source. Review, matching-integrity exceptions and merge authority are unchanged.

## Work and review passes

1. **Reserve active work.** Follow [work coordination](#work-coordination), state the bounded batch and shared-file dependencies, and work on a dedicated branch from current `main` in your own checkout or worktree. Coordinate live overlaps before editing.
2. **Establish boundaries and types.** Inspect the target and reference evidence before reconstructing the unit. Before declaring a shared address, search existing headers and accepted units and reuse the established declaration. Reconcile dependency names and ownership with current `main`; do not retain duplicate layouts or stale claims that an accepted symbol is unrecovered. Record unknowns explicitly. Bounded unit mappings must appear in both progress exports; mapping changes require the export checks, without increasing source credit. A related build is corroboration, not proof of the target layout.
3. **Reconstruct and match.** Recover source in the original codebase's style. Compile, compare instructions and data, and use the [investigation loop](#investigation-and-escalation) when needed. Keep discarded trial variants local; retain useful partial reconstruction in the contribution branch. Per-function comparisons measure reconstruction progress; the combined build checks the bytes selected for linking.
4. **Name and explain.** Include provenance for recovered names, types, fields, and boundaries in reviewed metadata using the [evidence format](config/README.md). Cite the actual loads, stores and relevant caller/callee behavior for inferred field widths and signatures; related subsystem accesses do not establish a declaration. Keep unaccessed regions opaque and unsupported meanings unresolved. Keep readable names distinct from claims of original names. Record supported names for compiled methods and generated lifecycle functions individually; unresolved functions retain neutral address labels. Comments follow the standard below.
5. **Self-check.** Verify the complete target binary, unit code and data, and regressions against the base revision. List unresolved functions and shared-file changes. Verify supported compiled-function names appear in both public progress exports; a source name alone is not an export check. Confirm that neutral identifiers receive no named-function credit merely because their bytes match. Record individually verified exact functions, whether linked or measured in a partial unit, in the contributor ledger in the same PR (ledger format `TO_IMPLEMENT`; attribution tracking is pending the owner's decision). Attribute the original source contributor and preserve that credit through integration or batching; review and merge work do not transfer it. Exact comparison-only functions receive contributor credit before whole-unit promotion. Fuzzy functions retain their source provenance without exact-function credit. Check that every exact function has attribution and that the unique credited inventory agrees with the public exact count. Do not claim a function score proves a complete unit.
6. **Blind accuracy review.** An independent reviewer examines the scoped functions with proposed names and comments withheld, then reconciles its interpretation against the evidence. Record disagreements and their resolution in the PR.
7. **Hostile review.** A fresh reviewer applies the checklist below. Resolve every finding by fixing it or providing evidence. Use the same reviewer for a delta recheck where available; follow the focused recheck rules below. Substantive reconstruction changes return through the relevant passes for the affected scope.
8. **Submit for the owner's decision.** Required CI must pass on the final PR revision, with no unexplained regression. Attach the reviewed revision, validation results, accuracy review, and hostile-review verdict. Only the project owner may accept and merge it.

**Partial reconstructed functions may be committed, submitted and merged before reaching 100% byte equality.** A measured mismatch alone is not a review failure or a reason to withhold useful source. Review source fidelity, evidence and comparison integrity separately from matching completeness; document mismatches and unresolved reconstruction questions. Do not require every function in a contribution to match, or require whole-unit promotion, merely to accept reviewed partial source. Unsupported matching tricks and invalid evidence remain review failures.

Partial function bodies stay in their normal source files, registered as `NonMatching` objects in `configure.py`. See [stage mapping](config/GIHE78/README.md#mapping-onto-the-handbook-stages). CI measures partial similarity and exact unlinked functions independently of the verified linked build. Once merged, these registered comparisons publish measured fuzzy and exact-but-unlinked progress on both public reports. Incomplete matching does not require the PR to remain a draft after its scoped reviews and final CI pass. Promotion changes the object to `Matching`; it does not move the source file.

Partial units follow the same accuracy and hostile-review gates, assessed for the submitted partial-source scope. A reviewer may return PASS/SHIP with documented matching incompleteness when source fidelity, evidence, comparisons and applicable validation pass. The complete executable must remain byte-identical using the accepted source plus original bytes for unpromoted ranges; the comparison build measures the partial bodies separately. This requirement does not demand byte equality from an unlinked comparison body. Their original assembly remains linked until replacement is validated; partial source is not reported as a completed unit. Retain unresolved identifiers when evidence is insufficient rather than fabricate recovery.

For changes with dependencies on other PRs, identify those dependencies and tested revisions. Documentation-only changes require appropriate checks, not an invented binary-validation result.

Use existing independent accuracy and hostile reviews when their revision, reviewer independence, scope and supporting evidence are adequate. Add focused review for missing coverage or new questions rather than repeat an adequate initial pass. New reconstruction, materially changed behavior, types or boundaries, compiler-profile or comparison changes, insufficient evidence and suspicious matches return through the relevant full passes.

After fixes, use a delta recheck by default, not another full-PR review. The reviewer checks each prior finding, the correction and its affected behavior, declarations, callers, boundaries, comparison settings and evidence. Carry forward unchanged coverage by linking the prior review and identifying its revision. For each finding, record whether it is resolved or remains open. Do not repeat unrelated disassembly, source interpretation or validation already covered by adequate evidence.

A replacement reviewer may continue from the earlier independent record and supporting evidence; reviewer replacement alone does not require a full restart. If that evidence is unavailable or inadequate, or a change invalidates earlier conclusions, expand only the affected scope and state why. A full-PR restart requires a concrete reason that invalidates coverage across the PR; finding a localized defect alone is not such a reason.

After refreshing from `main`, carry findings forward only after explicitly verifying the diff and affected dependencies. Recheck conflict resolutions and affected shared declarations and units. Post final-revision review records that cite the earlier records, identify the checked delta and explain why unchanged findings still apply; an earlier verdict alone does not cover the new head.

Required CI must verify the final revision's complete target binary, unit code/data, regressions and applicable progress exports. CI may derive both public reports from one validated progress computation; both map checks and final receipt/provenance validation still run before merge. Share validation artifacts tied to that exact revision so reviewers can assess coverage and results without each duplicating complete builds and test suites. Run focused checks while resolving findings, then complete required CI on the final integrated revision. Review comments and clearance records alone do not require another build. When source or build inputs change, earlier CI cannot substitute for final-revision validation. Retest unchanged checks locally only to investigate a failure, changed dependency or evidence gap, and record the reason. Focused review and shared validation do not relax independence, evidence, review records, CI or owner approval.

Caches and publication reuse are disabled. Every PR and `main` run validates fresh until the owner enables an acceleration mechanism after its own acceptance tests. Every run's `Site report` job turns the build's `report.json` into a `report-v1` package (artifact `public-report`) for the shared site; the site imports it from successful `main` runs only. Each main run restores the timeline from the previous main run's package and appends its own measurement. See [docs/public_report.md](docs/public_report.md). A change to `validate.yml` is a site policy change: add its new Git blob to `config/GIHE78/producer.json` in the same PR, and the owner approves the same blob on the site.

Compatible bounded contributions may be integrated in a batch with each original PR, author revision and review coverage identified. Review the combined delta and affected dependencies, and require final-revision records and CI. Link the originals and close them as incorporated only after the combined PR merges. Compiler-profile and comparison changes require separate review and validation.

## Investigation and escalation

Contributors and agents choose investigations according to the unresolved question. Additional tools are not required for every function. Escalate when controlled experiments stop yielding useful evidence, or when uncertainty about behavior, types, or boundaries could invalidate the reconstruction. A high matching percentage does not resolve those uncertainties.

| Question | Useful investigation |
| --- | --- |
| What does the target do, reference, or own? | Inspect disassembly, references, and available debug information; use Ghidra for bounded static analysis. |
| Why does plausible source produce different instructions? | Run controlled compiler experiments under the documented profile and inspect relevant compiler intermediate output. |
| Which behavioral interpretation fits an observed execution? | Capture a bounded runtime trace with a debugger, such as headless Dolphin. |

State the question before investigating. Check decompiler output against instructions; record runtime observations with their inputs and conditions. Neither establishes original source by itself. Record the tested revision, tool version and settings, conclusion, supporting evidence, and remaining uncertainty in the task's evidence or review record. Keep databases, dumps, and detailed experiments private.

Return to reconstruction and comparison when the question is resolved. If evidence remains insufficient, retain the original assembly in the linked build and report the gap. Preserve useful partial source with its supported facts, explicit unknowns and valid comparison registration; this is distinct from discarded trial variants. Missing essential evidence can still block review, but byte mismatch alone cannot. Tool choice does not relax matching integrity, combined-build validation, independent reviews, or owner approval. Reviewers may require further investigation when submitted evidence is insufficient.

## Matching integrity

Do not manufacture a match through unsupported compiler or comparison manipulation:

- No per-function flags, optimization pragmas, or compiler attributes introduced to force code generation.
- No inline assembly, embedded instruction bytes, or patched compiler output substituting for originally compiled source.
- No invented helpers, casts, volatile qualifiers, padding, or control flow whose only justification is obtaining matching bytes.
- No altered symbol boundaries, excluded comparisons, or progress categories that conceal mismatches.
- No behavioral changes made to satisfy the compiler.

Compiler versions and flags need evidence at the original compilation-unit or library level. A function-specific exception requires evidence that the original used it and the project owner's explicit approval. Originally handwritten assembly likewise requires provenance.

An evidence-supported source form may be identified through its compiler output. A suspected fake match remains a local experiment, outside accepted source, unless the project owner explicitly authorizes a documented exception. A `fake match` label is not permission. Otherwise leave the original assembly linked and the recovery unresolved.

Put compiler reasoning, exception evidence, and matching experiments in metadata or the PR, not explanatory matching comments in reconstructed source. Preserve original assertions and diagnostics when the target requires them.

## Source readability and comments

Names, types, and structure should carry most of the readability. The source should read as a coherent original codebase with helpful clarification.

Comments explain intent, meaningful behavior, invariants, or non-obvious relationships using vocabulary supported by the code and references. Do not narrate obvious statements or require a comment on every function.

Keep matching scores, compiler explanations, agent notes, review history, and speculative claims out of source comments. Uncertainty and provenance belong in evidence records or the PR. Do not turn a hypothesis into an authoritative name or comment.

## Hostile-review checklist

The first reviewer must be independent of the reconstruction and its prior reasoning. Supply the scoped source and headers, target disassembly and relevant data, provenance records, unit boundaries, compiler settings, comparison configuration, and validation results for an identified revision. References must be accessible to the reviewer; absent essential evidence is a blocked check, not permission to assume correctness.

Review all submitted reconstructed functions, including partial ones:

| Check | Required result |
| --- | --- |
| Source fidelity | Behavior and source structure are supported by the target and references; no unsupported matching tricks. |
| Compiler settings | Compiler and flags have evidence at the appropriate scope. Inspect per-function overrides and verify evidence and owner approval for any exception. |
| Types and layouts | Declarations reflect available evidence, including sizes, offsets, signatures, and shared ownership. Unknowns are explicit; no fabricated layouts or match-only substitutes. |
| Names | Provenance supports original-name claims. Inferred names describe what the code establishes without presenting guesses as recovered facts. |
| Comments | Comments clarify meaning and readability without obvious narration, speculation, compiler explanations, or process notes. Absence of an unnecessary comment is not a failure. |
| Unit boundaries | Splits and code/data ownership have evidence. Provisional boundaries are identified; no artificial fragmentation or grouping to inflate progress. |
| Comparison integrity | No patched output, excluded comparisons, altered symbol ranges, or misleading categories conceal mismatches. Partial and linked results remain distinct. |
| Validation | Required checks cover the reviewed revision, complete target output, unit code/data, and regressions against the base revision. No substituted scores or unverified success claims. |

Report `PASS`, `FAIL`, `BLOCKED`, or a justified `N/A` for each check. Findings identify the file/line or configuration location, supporting evidence, and required correction or missing information. Do not invent what the original author would have written or treat stylistic preference as evidence.

End with the reviewed revision and one verdict: `SHIP` (no unresolved failures or blocked checks), `FIX` (localized corrections), `REDO` (unsupported tricks or fundamental reconstruction problems), or `BLOCKED` (essential evidence or validation unavailable). A verdict does not authorize merging.

After fixes, apply the delta recheck rules above. In the final record, cite the earlier review, list the checked changes and finding resolutions, and identify carried-forward coverage. Newly discovered defects may still be reported on unchanged code. Additional changes require coverage and validation for the resulting revision; an old verdict alone does not cover new changes.

## Review records and owner clearance

Post each pass as a separate PR discussion comment. Start with `Revision: FULL_COMMIT_SHA`, followed immediately by `Accuracy: PASS` or `Accuracy: N/A - scope-based explanation` for accuracy; use `Hostile: SHIP`, `Hostile: FIX`, `Hostile: REDO`, or `Hostile: BLOCKED` for hostile review. Each header field occurs once, outside quotes or code fences. Then include reviewer identity, scope, evidence, findings, checklist results where applicable, and recheck results. Link any longer review evidence from that record. Do not write a successful record before the review happens.

After checking reviewer independence, coverage, and resolution of findings, only the owner posts clearance in this form:

```text
<!-- owner-review-clearance -->
Revision: FULL_COMMIT_SHA
Accuracy: https://github.com/mitsevox/n100f/pull/NUMBER#issuecomment-ID
Hostile: https://github.com/mitsevox/n100f/pull/NUMBER#issuecomment-ID
```

`Owner review clearance` checks that the latest owner clearance identifies the current head, points to two distinct records on that PR, and that those records contain the same revision and passing verdicts. It remains pending for drafts, absent clearance, stale revisions, or invalid records. New commits require reviews covering the resulting revision and new owner clearance. Edit the latest clearance to remove its revision or evidence links to withdraw it; changes to linked discussion records also trigger revalidation.

The check authenticates the owner's clearance and record references; it cannot prove reviewer independence or reasoning quality. Those judgments remain the owner's responsibility. Agents must not post clearance on the owner's behalf without explicit authorization. Clearance never authorizes a contributor or agent to merge.

The workflow evaluates trusted base-branch code with read access to PR records and permission to publish the status. It does not check out or execute a contribution branch or use build/reference credentials. Its comment triggers become available after the owner merges the bootstrap workflow into the default branch.

## Feedback

Use PR discussions for review findings and issues for unresolved research or recurring tool problems. Keep detailed failed experiments in private research. Correct recurring failures at their cause instead of repeating workarounds.

Change permanent instructions through a focused PR when the agreed process needs correction. Identify affected accepted code or tools and propose the necessary revalidation; do not rewrite the rules to justify the current patch. Workflow changes also require the project owner's approval.
