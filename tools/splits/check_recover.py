#!/usr/bin/env python3
"""Check that the committed split reports are what tools/splits/recover.py writes today.

recover.py's output changes when an exact-body `function` row lands in evidence.tsv (it adds a
name anchor and call features; docs/splits.md, Reproducing). A PR that adds such a row and does
not re-run the tool leaves the `flat/` units of splits.txt, split_units.tsv and split_edges.tsv
stale. This re-runs recover.py into a scratch directory and compares the three outputs with the
committed files. With --write it replaces them instead, so the fix is one command.

Run it after a dtk split of the target (CI runs it after the build):

    python3 tools/splits/check_recover.py --target GIHE78-rev0 [--write] [--ps2-elf PATH]

The PS2 ELF is private. The build image has it at /orig/SLUS-20349/SLUS_203.49, and a clone of
the n100f-build repository at orig/SLUS-20349/SLUS_203.49. Without --ps2-elf the tool looks in
the image path, then in an n100f-build clone beside this checkout.

A target without split_units.tsv has no recovered units and passes.
"""

import argparse
import difflib
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PS2_ELF = 'SLUS-20349/SLUS_203.49'
# The build image's /orig, then a clone of n100f-build beside this checkout.
PS2_ELF_DIRS = ('/orig', os.path.join(os.path.dirname(ROOT), 'n100f-build', 'orig'))
DIFF_LINES = 40


def is_header(line):
    return bool(line) and not line[0].isspace() and ':' in line and not line.startswith('Sections:')


def unit_region(text, prefix):
    """(start, end) character offsets of the recovered units in splits.txt: from the blank line
    before the first `prefix` unit through the last line of the last one, which is the text
    recover.py writes with --splits.  The units must be contiguous."""
    lines = text.splitlines(keepends=True)
    offs = [0]
    for line in lines:
        offs.append(offs[-1] + len(line))
    heads = [i for i, line in enumerate(lines) if is_header(line)]
    mine = [i for i in heads if lines[i].startswith(prefix)]
    if not mine:
        raise ValueError(f'splits.txt has no {prefix} units')
    first = mine[0]
    after = next((i for i in heads if i > first and not lines[i].startswith(prefix)), None)
    if any(i > after for i in mine if after is not None):
        raise ValueError(f'the {prefix} units of splits.txt are not contiguous')
    if first == 0 or lines[first - 1].strip():
        raise ValueError(f'splits.txt needs a blank line before {lines[first].strip()}')
    end = len(lines) if after is None else after
    while end > first and not lines[end - 1].strip():
        end -= 1
    return offs[first - 1], offs[end]


def splice(text, prefix, units):
    lo, hi = unit_region(text, prefix)
    return text[:lo] + units + text[hi:]


def report(name, old, new):
    diff = list(difflib.unified_diff(old.splitlines(keepends=True), new.splitlines(keepends=True),
                                     f'committed/{name}', f'recover.py/{name}'))
    sys.stdout.writelines(diff[:DIFF_LINES])
    if len(diff) > DIFF_LINES:
        print(f'... {len(diff) - DIFF_LINES} more diff lines')
    if diff and not diff[-1].endswith('\n'):
        print()


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--target', default='GIHE78-rev0')
    ap.add_argument('--dol', help='default: orig/<target>/sys/main.dol')
    ap.add_argument('--ps2-elf', help='default: /orig/%s, else ../n100f-build/orig/%s' % (PS2_ELF, PS2_ELF))
    ap.add_argument('--objdir', help='default: build/<target>/obj')
    ap.add_argument('--prefix', default='flat/')
    ap.add_argument('--write', action='store_true', help='update the committed files instead of checking them')
    args = ap.parse_args()

    cfg = os.path.join(ROOT, 'config', args.target)
    files = {name: os.path.join(cfg, name) for name in ('splits.txt', 'split_units.tsv', 'split_edges.tsv')}
    if not os.path.exists(files['split_units.tsv']):
        print(f'{args.target}: no split_units.tsv, nothing to check')
        return 0
    if not args.ps2_elf:
        args.ps2_elf = next((p for p in (os.path.join(d, PS2_ELF) for d in PS2_ELF_DIRS) if os.path.exists(p)), None)
        if not args.ps2_elf:
            print(f'error: no PS2 ELF. Pass --ps2-elf <path>: the build image has /orig/{PS2_ELF}, a clone '
                  f'of n100f-build has orig/{PS2_ELF}', file=sys.stderr)
            return 2
    dol = args.dol or os.path.join(ROOT, 'orig', args.target, 'sys', 'main.dol')
    objdir = args.objdir or os.path.join(ROOT, 'build', args.target, 'obj')

    with tempfile.TemporaryDirectory() as tmp:
        out = {name: os.path.join(tmp, name) for name in ('units.txt', 'split_units.tsv', 'split_edges.tsv')}
        cmd = [sys.executable, os.path.join(ROOT, 'tools', 'splits', 'recover.py'),
               '--dol', dol, '--ps2-elf', args.ps2_elf,
               '--symbols', os.path.join(cfg, 'symbols.txt'), '--evidence', os.path.join(cfg, 'evidence.tsv'),
               '--objdir', objdir, '--prefix', args.prefix,
               '--splits', out['units.txt'], '--edges', out['split_edges.tsv'], '--units', out['split_units.tsv']]
        run = subprocess.run(cmd, cwd=ROOT)
        if run.returncode:
            print(f'recover.py failed (exit {run.returncode}); its messages are above', file=sys.stderr)
            return run.returncode
        fresh = {name: open(out[name], encoding='utf-8').read() for name in ('split_units.tsv', 'split_edges.tsv')}
        units = open(out['units.txt'], encoding='utf-8').read()

    committed = {name: open(path, encoding='utf-8').read() for name, path in files.items()}
    fresh['splits.txt'] = splice(committed['splits.txt'], args.prefix, units)
    stale = [name for name in files if committed[name] != fresh[name]]

    if args.write:
        for name in stale:
            with open(files[name], 'w', encoding='utf-8', newline='') as f:
                f.write(fresh[name])
            print(f'updated config/{args.target}/{name}')
        if not stale:
            print(f'{args.target}: split reports already current')
        return 0
    if not stale:
        print(f'{args.target}: split reports match recover.py')
        return 0
    for name in stale:
        report(name, committed[name], fresh[name])
    print(f'\nerror: config/{args.target}/{", ".join(stale)} differ from what tools/splits/recover.py writes '
          f'for this revision.\nAn exact-body `function` row in evidence.tsv changes recover.py\'s output '
          f'(docs/splits.md, Reproducing). After a build of {args.target}, run\n\n'
          f'    python3 tools/splits/check_recover.py --target {args.target} --write\n\n'
          f'and commit the files it updates. It finds the PS2 ELF at /orig/{PS2_ELF} (build image) or in '
          f'an n100f-build clone beside this checkout (orig/{PS2_ELF}); elsewhere pass --ps2-elf <path>.', file=sys.stderr)
    return 1


if __name__ == '__main__':
    sys.exit(main())
