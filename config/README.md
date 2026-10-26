# Evidence records

Each target keeps curated claims in `evidence.tsv`, alongside its build configuration. Record one claim per row; add rows when they support a source or configuration change. Keep bulk tool output and experiment histories outside public Git.

| Column | Meaning |
| --- | --- |
| `kind` | `name`, `function`, `data`, `unit-anchor`, `unit`, `type`, `field`, `external`, `compiler`, `assembly`, `source-form`, or `linker`. |
| `start`, `end` | Target virtual addresses in hexadecimal; range ends are exclusive. Use `-` for unknown or inapplicable addresses. |
| `subject` | Symbol, source path, type, or field concerned. Neutral address labels do not claim original names. |
| `origin` | `target`, `signature`, `related`, or `inferred`, as defined below. |
| `start_boundary`, `end_boundary` | Independent edge claims: `exact`, `provisional`, `open`, or `-`. |
| `evidence` | A concise, reproducible locator and explanation of what it establishes and what remains unknown. |

`target` means the claim is directly present in the target's symbols, debug information, text, or bytes. `signature` means a named tool signature supplied it. `related` means it comes from another identified build. `inferred` means interpretation of the code. A matching body from a related build does not by itself establish the target's original name or source file.

For a `name` claim, `target` requires explicit target name evidence; behavior alone belongs under `inferred`. Record the function's extent in a separate `function` row so a name claim cannot silently establish its boundaries. Types and fields likewise need their own evidence.

`exact` means evidence establishes that edge. `provisional` means a candidate edge still needs confirmation. `open` marks an anchor with the true start at or before it, or the true end at or after it. Use `-` where no edge is claimed. A `unit-anchor` identifies evidence of a source path, not a unit's extent or ownership of neighboring functions.

Evidence must be usable by another contributor: identify target addresses and observations, or a reference's revision/hash and symbol or record. Include a tool version and signature identifier for signature claims. A local absolute path or an unexplained score is insufficient. Keep raw exports out of the table.

These records describe evidence, not matching progress or approval. Function and name rows supply public function labels; they do not rename executable symbols or alter build splits. Record descriptive method names and compiler-generated lifecycle roles as inferred, not recovered original symbols. Once the source build and progress export exist, export must reject supported source functions without a corresponding name record. Changes to executable symbols and unit splits require corresponding evidence and the contribution review passes. Review findings, tested revisions, and the owner's acceptance remain in the PR.

How bounded `unit` rows feed the public maps is `TO_IMPLEMENT`. Mapping alone will grant no linked or matched credit and does not change build splits.
