# Community references

`references/Scooby-Doo Night of 100 Frights/community/` holds text copies of the speedrunning community's encyclopedia, version-differences sheet and skips dictionary. They describe what players observed in the retail builds on real consoles and emulators. Some of it may rest on memory inspection or debug tools, but none of it is reverse-engineered evidence about the target code.

Rules for using them:

- Treat them as hints about gameplay behaviour: what a mechanic does, which version differs, where to look. They help find and name the code behind a mechanic; they do not establish it.
- Every claim in source, headers, names, comments or `config/GIHE78-rev0/evidence.tsv` must rest on this project's own evidence from the target binary or the symbol and debug references. A community observation alone is never enough for a name, a layout or a behavioural comment.
- An evidence row or PR may cite a community document as context next to the binary evidence, by file and section.
- Platform and version claims (PAL, NTSC, GameCube version 0 or 1, PS2, Xbox) are the community's observations. Verify them against the relevant build before relying on them.
- Credit for these documents lives in [CREDITS](../CREDITS.md#community-references) only. Source and header files do not mention them.
