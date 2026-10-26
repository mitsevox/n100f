# Credits

Imported and reused source keeps its origin, pinned revision, license and local modifications here. Copyright and license notices in imported files remain intact.

No game or SDK source has been imported yet.

## Build tooling

`configure.py`, `tools/*.py` (except project-specific additions), `.vscode/`, `.flake8`, `.gitattributes` and `docs/dtk/` come from [encounter/dtk-template](https://github.com/encounter/dtk-template/tree/d366876408b754891a0392badbc63bff3490469f), revision `d366876408b754891a0392badbc63bff3490469f`, published under CC0 1.0 (`tools/LICENSE.dtk-template`). Local changes: project configuration in `configure.py`, removed example libraries and REL helper, and merged ignore rules.

`tools/patches/dtk-v1.8.3-rom-copy-info.patch` modifies [encounter/decomp-toolkit](https://github.com/encounter/decomp-toolkit/tree/v1.8.3) v1.8.3 (MIT/Apache-2.0) so that `_rom_copy_info` is also searched for in the first text section. `tools/project.py` additionally passes a non-default DOL entry symbol to mwld.
