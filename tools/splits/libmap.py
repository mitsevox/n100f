#!/usr/bin/env python3
"""Map GIHE78's MetroTRK, runtime, MSL, Dolphin SDK and CARD objects from public decompilations.

Every candidate source in config/GIHE78/libmap.json is compiled under its
profile and each compiler version listed there. A function whose masked body
occurs exactly once in main.dol, at a function start of the same size,
places the object; the whole object .text is then compared in place. A
placement must lie inside the manifest "region", or inside the candidate's own
"region" where it sets one. For each
object the best placement wins (a whole-object match first, then the most
matched bytes, then manifest order) and placements may not overlap.

Outputs:
  --splits    library unit entries for splits.txt (.text only; data ownership
              is added when a unit's source is imported and its sections compile),
              with the profile's .comment version where it differs from the default
  --units     a TSV report of every chosen unit
  --evidence  unit, function, name and compiler rows for evidence.tsv
  --symbols-out  symbols.txt with the supported names applied

A name is applied only when it labels a function of that size in one of the
upstream symbol lists that the manifest marks as coming from a map or debug
file ("names"). Other functions keep their neutral labels.

    python3 tools/splits/libmap.py --dol orig/GIHE78/sys/main.dol \\
        --compilers build/compilers --wrapper build/tools/wibo \\
        --checkout ac-decomp=../ac-decomp --checkout tww=../tww ... \\
        --splits libunits.txt --units config/GIHE78/lib_units.tsv \\
        --evidence librows.tsv --symbols-out config/GIHE78/symbols.txt
"""

import argparse
import collections
import glob
import json
import os
import re
import shlex
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

RELOC_MASKS = {1: 0xFFFFFFFF, 4: 0xFFFF, 5: 0xFFFF, 6: 0xFFFF, 10: 0x03FFFFFC, 11: 0xFFFC, 109: 0x1FFFFF}
MIN_ANCHOR_WORDS = 4
RUNTIME_FILES = {'__va_arg.c', 'global_destructor_chain.c', 'runtime.c', '__mem.c', 'ptmf.c',
                 '__init_cpp_exceptions.cpp', 'nmwexception.cp', 'gecko_exceptionppc.cp'}
NEUTRAL = re.compile(r'(fn|lbl|func)_[0-9A-Fa-f]{8}$')


class Text:
    """The DOL's main text section, indexed by word."""

    def __init__(self, path):
        raw = open(path, 'rb').read()
        h = struct.unpack('>18I18I18I', raw[:0xD8])
        off, self.base, size = h[1], h[19], h[37]
        self.words = struct.unpack_from(f'>{size // 4}I', raw, off)
        self.index = collections.defaultdict(list)
        for n, w in enumerate(self.words):
            self.index[w].append(n)

    def equal(self, at, words, masks):
        if at < 0 or at + len(words) > len(self.words):
            return False
        return all((self.words[at + k] ^ w) & m == 0 for k, (w, m) in enumerate(zip(words, masks)))

    def search(self, words, masks):
        pivot = next((k for k, m in enumerate(masks) if m == 0xFFFFFFFF), None)
        if pivot is None:
            return []
        return [c - pivot for c in self.index.get(words[pivot], ()) if self.equal(c - pivot, words, masks)]


def read_object(path):
    """Return (.text words, masks, [(offset, size, name, binding)]) of a relocatable object."""
    data = open(path, 'rb').read()
    shoff, = struct.unpack_from('>I', data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('>HHH', data, 0x2E)
    sh = [struct.unpack_from('>10I', data, shoff + i * shentsize) for i in range(shnum)]

    def name(table, o):
        start = sh[table][4] + o
        return data[start:data.index(b'\0', start)].decode()

    names = [name(shstrndx, h[0]) for h in sh]
    if '.text' not in names:
        return None
    ti = names.index('.text')
    words = list(struct.unpack_from(f'>{sh[ti][5] // 4}I', data, sh[ti][4]))
    masks = [0xFFFFFFFF] * len(words)
    for h in sh:
        if h[1] == 4 and h[7] == ti:
            for o in range(h[4], h[4] + h[5], 12):
                r_offset, r_info, _ = struct.unpack_from('>IIi', data, o)
                masks[r_offset // 4] &= ~RELOC_MASKS.get(r_info & 0xFF, 0xFFFFFFFF)
    funcs = []
    for h in sh:
        if h[1] != 2:
            continue
        for o in range(h[4], h[4] + h[5], 16):
            st_name, value, size, info, _, shndx = struct.unpack_from('>IIIBBH', data, o)
            if info & 0xF == 2 and shndx == ti and size:
                funcs.append((value, size, name(h[6], st_name), 'global' if info >> 4 else 'local'))
    return words, masks, sorted(funcs)


def compile_one(job):
    args, cand, version, source = job
    root = args.checkout[cand['upstream']]
    profile = args.manifest['profiles'][cand['profile']]
    flags = shlex.split(profile['flags'])
    flags += [f for i in cand.get('include', []) for f in ('-i', i)]
    flags += [f'-D{d}' for d in cand.get('defines', [])]
    compiler = os.path.join(args.compilers, 'GC', version, 'mwcceppc.exe')
    with tempfile.TemporaryDirectory() as tmp:
        obj = os.path.join(tmp, 'o.o')
        cmd = ([args.wrapper] if args.wrapper else []) + [compiler, *flags, '-c', source, '-o', obj]
        proc = subprocess.run(cmd, cwd=root, capture_output=True, text=True)
        if proc.returncode or not os.path.exists(obj):
            return job, None
        return job, read_object(obj)


def place(text, obj):
    """Find where the object sits in the DOL: whole-object match, else the in-order unique hits."""
    words, masks, funcs = obj
    hits = []
    for off, size, name, _ in funcs:
        if size // 4 < MIN_ANCHOR_WORDS:
            continue
        w, m = words[off // 4:(off + size) // 4], masks[off // 4:(off + size) // 4]
        found = text.search(w, m)
        if len(found) == 1:
            hits.append((off, size, name, text.base + 4 * found[0]))
    if not hits:
        return None
    for off, _, _, addr in hits:
        at = (addr - text.base) // 4 - off // 4
        if text.equal(at, words, masks):
            base = text.base + 4 * at
            return True, [(o, s, n, base + o) for o, s, n, _ in funcs]
    # Longest chain of hits in object order and address order, each step no wider than
    # twice the object distance plus slack for changed function sizes.
    n = len(hits)
    length, prev = [1] * n, [-1] * n
    for i in range(n):
        for j in range(i):
            gap = hits[i][3] - hits[j][3]
            if 0 < gap <= 2 * (hits[i][0] - hits[j][0]) + 0x400 and length[j] + 1 > length[i]:
                length[i], prev[i] = length[j] + 1, j
    i = max(range(n), key=lambda k: (length[k], -k))
    chain = []
    while i >= 0:
        chain.append(hits[i])
        i = prev[i]
    return False, chain[::-1]


def load_symbols(path):
    funcs = {}
    for line in open(path):
        m = re.match(r'(\S+) = \.text:0x([0-9A-F]+); // type:function size:0x([0-9A-F]+)', line)
        if m:
            funcs[int(m[2], 16)] = (m[1], int(m[3], 16))
    return funcs


def upstream_names(args):
    """name -> {size: [upstream]} from the map-backed upstream symbol lists."""
    out = collections.defaultdict(lambda: collections.defaultdict(list))
    for up, info in args.manifest['upstreams'].items():
        if 'names' not in info or up not in args.checkout:
            continue
        for line in open(os.path.join(args.checkout[up], info['names'])):
            m = re.match(r'(\S+) = \.text:0x[0-9A-Fa-f]+; // type:function size:0x([0-9A-Fa-f]+)', line)
            if m and not NEUTRAL.match(m[1]):
                out[m[1]][int(m[2], 16)].append(up)
    return out


def unit_path(cand, source, sdk_dirs):
    base = os.path.basename(source)
    if base.lower() in RUNTIME_FILES:
        return f'Runtime.PPCEABI.H/{base}'
    if cand['library'] == 'dolphin':
        sub = sdk_dirs.get(base.lower()) or os.path.basename(os.path.dirname(source))
        return f'dolphin/{sub}/{base}'
    return f"{cand['library']}/{base}"


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--dol', required=True)
    ap.add_argument('--manifest', default='config/GIHE78/libmap.json')
    ap.add_argument('--symbols', default='config/GIHE78/symbols.txt')
    ap.add_argument('--compilers', required=True, help='directory containing GC/<version>/')
    ap.add_argument('--wrapper', help='wibo or wine, when not on Windows')
    ap.add_argument('--checkout', action='append', default=[], metavar='UPSTREAM=PATH',
                    help='local checkout of a manifest upstream at its pinned commit')
    ap.add_argument('--jobs', type=int, default=os.cpu_count())
    ap.add_argument('--splits', required=True)
    ap.add_argument('--units', required=True)
    ap.add_argument('--evidence', required=True)
    ap.add_argument('--symbols-out')
    args = ap.parse_args()
    args.manifest = json.load(open(args.manifest))
    args.checkout = {k: os.path.abspath(v) for k, v in (c.split('=', 1) for c in args.checkout)}
    args.compilers = os.path.abspath(args.compilers)
    args.wrapper = args.wrapper and os.path.abspath(args.wrapper)
    for up, path in args.checkout.items():
        head = subprocess.run(['git', '-C', path, 'rev-parse', 'HEAD'], capture_output=True, text=True).stdout.strip()
        if head != args.manifest['upstreams'][up]['commit']:
            sys.exit(f'{up}: checkout is at {head or "?"}, the manifest pins {args.manifest["upstreams"][up]["commit"]}')
    lo, hi = (int(x, 16) for x in args.manifest['region'])
    text = Text(args.dol)
    symbols = load_symbols(args.symbols)

    jobs = []
    order = {}
    for rank, cand in enumerate(args.manifest['candidates']):
        if cand['upstream'] not in args.checkout:
            continue
        root = args.checkout[cand['upstream']]
        sources = sorted({os.path.relpath(p, root) for pat in cand['sources'] for p in glob.glob(os.path.join(root, pat))})
        for v in args.manifest['profiles'][cand['profile']]['versions']:
            for s in sources:
                jobs.append((args, cand, v, s))
                order[(rank, v, s)] = len(order)
    sdk_dirs = {}
    if 'ac-decomp' in args.checkout:
        for p in glob.glob(os.path.join(args.checkout['ac-decomp'], 'src/static/dolphin/*/*.c')):
            sdk_dirs[os.path.basename(p).lower()] = os.path.basename(os.path.dirname(p))

    placements = []
    with ThreadPoolExecutor(args.jobs) as ex:
        for (_, cand, version, source), obj in ex.map(compile_one, jobs):
            if not obj or not obj[2]:
                continue
            found = place(text, obj)
            if not found:
                continue
            whole, funcs = found
            funcs = [f for f in funcs if whole or symbols.get(f[3], (None, None))[1] == f[1]]
            if not funcs:
                continue
            start, end = funcs[0][3], funcs[-1][3] + funcs[-1][1]
            clo, chi = (int(x, 16) for x in cand['region']) if 'region' in cand else (lo, hi)
            if not clo <= start < end <= chi:
                continue
            rank = args.manifest['candidates'].index(cand)
            placements.append({'cand': cand, 'version': version, 'source': source, 'whole': whole,
                               'start': start, 'end': end, 'funcs': funcs, 'nfuncs': len(obj[2]),
                               'bytes': sum(f[1] for f in funcs), 'rank': order[(rank, version, source)],
                               'local': {f[2] for f in obj[2] if f[3] == 'local'},
                               'path': unit_path(cand, source, sdk_dirs)})

    # Versions that give the same placement are recorded together.
    same = collections.defaultdict(set)
    for p in placements:
        same[(p['path'], p['start'], p['end'], p['whole'], p['bytes'])].add((p['cand']['upstream'], p['version']))
    placements.sort(key=lambda p: (not p['whole'], -p['bytes'], p['rank']))
    units, taken = [], []
    used = set()
    for p in placements:
        if p['path'].lower() in used or any(p['start'] < b and a < p['end'] for a, b in taken):
            continue
        p['agree'] = sorted(same[(p['path'], p['start'], p['end'], p['whole'], p['bytes'])])
        units.append(p)
        used.add(p['path'].lower())
        taken.append((p['start'], p['end']))
    units.sort(key=lambda p: p['start'])

    # A whole-object match fixes every function extent inside the unit.
    table = dict(symbols)
    resized, added, removed = {}, set(), set()
    for u in units:
        if not u['whole']:
            continue
        starts = {f[3]: f[1] for f in u['funcs']}
        for a in [a for a in table if u['start'] <= a < u['end'] and a not in starts]:
            removed.add(a)
            del table[a]
        before = max((a for a in table if a < u['start']), default=None)
        if before is not None and before + table[before][1] > u['start']:
            table[before] = (table[before][0], u['start'] - before)
            resized[before] = table[before][1]
        for a, size in starts.items():
            if a not in table:
                table[a] = (f'fn_{a:08X}', size)
                added.add(a)
            elif table[a][1] != size:
                table[a] = (table[a][0], size)
                resized[a] = size
    for a in sorted(set(resized) | added | removed):
        what = 'added' if a in added else 'removed' if a in removed else f'resized to 0x{resized[a]:X}'
        print(f'note: function 0x{a:08X} {what} by a whole-object match', file=sys.stderr)
    symbols = table

    names = upstream_names(args)
    renames = {}
    local = set()
    rows = []
    commit = lambda up: args.manifest['upstreams'][up]['commit'][:7]
    for i, u in enumerate(units):
        up = u['cand']['upstream']
        left = i > 0 and units[i - 1]['end'] == u['start']
        right = i + 1 < len(units) and units[i + 1]['start'] == u['end']
        versions = sorted({v for a, v in u['agree'] if a == up})
        how = (f"the whole .text of {up} {commit(up)} {u['source']} ({u['nfuncs']} functions) matches here"
               if u['whole'] else
               f"{len(u['funcs'])} of the {u['nfuncs']} functions of {up} {commit(up)} {u['source']} match here, in object order")
        rows.append(['unit', f"0x{u['start']:08X}", f"0x{u['end']:08X}", u['path'], 'target',
                     'exact' if left else 'open', 'exact' if right else 'open',
                     f"Compiled with GC/{' and GC/'.join(versions)} ({u['cand']['profile']} profile, config/GIHE78/libmap.json), "
                     f"{how} (relocated fields masked). An edge is exact where the next unit abuts it; "
                     f"otherwise the object may extend past it. Data ownership is not mapped yet."])
        for off, size, name, addr in u['funcs']:
            label = symbols.get(addr, (f'fn_{addr:08X}',))[0]
            supported = names.get(name, {}).get(size)
            if supported and symbols.get(addr, (None, None))[1] != size:
                print(f'check: 0x{addr:08X} {name} is 0x{size:X} bytes in the object; symbols.txt has '
                      f'{symbols.get(addr, ("no function",))[0]} there, so it is not renamed', file=sys.stderr)
                supported = None
            if supported:
                renames[addr] = name
                label = name
                if name in u['local']:
                    local.add(addr)
            alone = '' if u['whole'] else ' This body occurs only here in main.dol.'
            rows.append(['function', f'0x{addr:08X}', f'0x{addr + size:08X}', label, 'target', 'exact', 'exact',
                         f"{name} in {up} {commit(up)} {u['source']} compiles (GC/{versions[0]}) to these bytes with "
                         f"relocated fields masked.{alone}"])
            if supported:
                srcs = sorted(set(supported))
                desc = '; '.join(f"{args.manifest['upstreams'][s]['names_source']} ({s} {commit(s)} "
                                 f"{args.manifest['upstreams'][s]['names']})" for s in srcs)
                rows.append(['name', f'0x{addr:08X}', '-', name, 'related', '-', '-',
                             f'{name} labels a 0x{size:X}-byte function in {desc}. The body here is the compiled '
                             f"{name} of {up} {commit(up)} {u['source']}; there is no GC debug symbol."])

    with open(args.splits, 'w') as out:
        for u in units:
            comment = args.manifest['profiles'][u['cand']['profile']].get('comment')
            attrs = f' comment:{comment}' if comment is not None else ''
            out.write(f"\n{u['path']}:{attrs}\n\t.text       start:0x{u['start']:08X} end:0x{u['end']:08X}\n")
    with open(args.units, 'w') as out:
        out.write('unit\ttext_start\ttext_end\tmatch\tfunctions\tsource_functions\tsource\tversions\n')
        for u in units:
            out.write(f"{u['path']}\t0x{u['start']:08X}\t0x{u['end']:08X}\t{'whole' if u['whole'] else 'partial'}\t"
                      f"{len(u['funcs'])}\t{u['nfuncs']}\t{u['cand']['upstream']}:{u['source']}\t"
                      f"{','.join(f'{a}@GC/{v}' for a, v in u['agree'])}\n")
    with open(args.evidence, 'w') as out:
        for r in rows:
            out.write('\t'.join(r) + '\n')
    if args.symbols_out:
        pending = sorted(added)
        lines = open(args.symbols).readlines()  # --symbols-out may be the same file
        with open(args.symbols_out, 'w') as out:
            for line in lines:
                m = re.match(r'(\S+) = (\.\w+):0x([0-9A-F]+); // type:(\w+)( size:0x[0-9A-F]+)?(.*)', line)
                if m and m[2] == '.text':
                    a = int(m[3], 16)
                    while pending and pending[0] < a:
                        b = pending.pop(0)
                        out.write(f'{renames.get(b, table[b][0])} = .text:0x{b:08X}; // type:function size:0x{table[b][1]:X}\n')
                    if m[4] == 'function':
                        if a in removed:
                            continue
                        rest = m[6]
                        if a in local:
                            rest = re.sub(r' scope:\w+', '', rest) + ' scope:local'
                        line = f'{renames.get(a, m[1])} = .text:0x{a:08X}; // type:function size:0x{table[a][1]:X}{rest}\n'
                out.write(line)
    whole = sum(u['whole'] for u in units)
    print(f'{len(jobs)} compiles, {len(placements)} placements, {len(units)} units ({whole} whole), '
          f"{sum(len(u['funcs']) for u in units)} functions, {len(renames)} names", file=sys.stderr)


if __name__ == '__main__':
    main()
