#!/usr/bin/env python3
"""Propose GIHE78 game-unit splits from the PS2 SLUS-20349 v1.01 DWARF.

Method (docs/splits.md has the reasoning and limits):
  1. Fingerprint every GC and PS2 function by the string literals and float
     constants it loads.  Unique mutual-best matches become anchors.
  2. Order the PS2 functions by case-insensitive source file name (the GC link
     order) and align them to the GC game block by size, with anchors pinned.
  3. Cut .text between neighbouring units, then cut each data section between
     consecutive units at an 8-byte-aligned symbol boundary, choosing the cuts
     with the fewest references that would cross them.

Inputs are the target DOL, the PS2 ELF, the DWARF function export in
references/, symbols.txt and the objects of a dtk split (for relocations).
Output is the unit part of splits.txt plus a TSV describing every edge.
Nothing here establishes original names; see config/README.md.
"""

import argparse
import bisect
import collections
import csv
import glob
import math
import os
import re
import struct
import sys

GAME_END = 0x800D8000  # first GC-only platform code after zVolume
CTORS = 0x8018E5C0
DATA_SECTIONS = ['.rodata', '.data', '.bss', '.sdata', '.sbss', '.sdata2', '.sbss2']
DATA_ALIGN = 8
PS2_ONLY = ('p2', 'ps2', 'fakeprintf', 'crt0', 'gcc_wrapper', 'ctbsp', 'ctgeom', 'ctworld',
            'rpcollis', 'rtintsec', 'vobuf', 'videodec', 'vibuf', 'strfile', 'readbuf',
            'read.c', 'disp.c', 'audiodec')


def s16(x):
    return x - 0x10000 if x & 0x8000 else x


class Dol:
    def __init__(self, path):
        self.raw = open(path, 'rb').read()
        h = struct.unpack('>18I18I18I', self.raw[:0xD8])
        self.secs = [(h[18 + i], h[i], h[36 + i]) for i in range(18) if h[36 + i]]

    def read(self, addr, n):
        for a, o, s in self.secs:
            if a <= addr < a + s:
                return self.raw[o + addr - a:o + addr - a + n]
        return None

    def u32(self, addr):
        b = self.read(addr, 4)
        return struct.unpack('>I', b)[0] if b and len(b) == 4 else None

    def cstr(self, addr):
        b = self.read(addr, 128)
        return printable(b)


def printable(b):
    if not b:
        return None
    b = b.split(b'\0')[0]
    if len(b) >= 4 and all(32 <= c < 127 or c in (9, 10) for c in b):
        return b.decode()
    return None


def find_small_data_bases(dol, init_addr):
    """r1/r2/r13 set-up routine: lis/ori pairs for r2 and r13."""
    regs = {}
    for p in range(init_addr, init_addr + 0x20, 4):
        w = dol.u32(p)
        op, rd, ra = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op == 15 and ra == 0:
            regs[rd] = (w & 0xFFFF) << 16
        elif op == 24 and rd == ra:
            regs[rd] |= w & 0xFFFF
    return regs[2], regs[13]


def load_symbols(path):
    out = []
    for line in open(path):
        m = re.match(r'(\S+) = (\.\w+):0x([0-9A-F]+); // (.*)', line)
        if not m:
            continue
        size = re.search(r'size:0x([0-9A-F]+)', m[4])
        out.append((int(m[3], 16), int(size[1], 16) if size else 0, m[2], m[1],
                    'type:function' in m[4]))
    out.sort()
    return out


def load_relocations(objdir):
    """(source address, target address) for every relocation in dtk's split objects.

    dtk names local labels <prefix>_<ADDR>, so each section's load address follows
    from any such symbol's value."""
    out = []
    for path in sorted(glob.glob(os.path.join(objdir, '**', '*.o'), recursive=True)):
        raw = open(path, 'rb').read()
        shoff, = struct.unpack_from('>I', raw, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from('>HHH', raw, 0x2E)
        shdrs = [struct.unpack_from('>IIIIIIIIII', raw, shoff + i * shentsize) for i in range(shnum)]

        def name(strtab, off):
            o = shdrs[strtab][4] + off
            return raw[o:raw.index(b'\0', o)].decode()

        symtab = next(i for i, s in enumerate(shdrs) if s[1] == 2)
        st = shdrs[symtab]
        syms = []
        for k in range(st[5] // 16):
            nm, value, size, info, other, shndx = struct.unpack_from('>IIIBBH', raw, st[4] + k * 16)
            syms.append((name(st[6], nm), value, shndx))
        base = {}
        for nm, value, shndx in syms:
            m = re.search(r'_([0-9A-F]{8})$', nm)
            if m and 0 < shndx < 0xFF00:
                base.setdefault(shndx, set()).add(int(m[1], 16) - value)
        for s in shdrs:
            if s[1] != 4:  # SHT_RELA
                continue
            bases = base.get(s[7])
            if not bases or len(bases) != 1:
                continue
            b, = bases
            for k in range(s[5] // 12):
                off, info, addend = struct.unpack_from('>IIi', raw, s[4] + k * 12)
                m = re.search(r'_([0-9A-F]{8})$', syms[info >> 8][0])
                if m:
                    out.append((b + off, int(m[1], 16) + addend))
    return out


class Ps2:
    def __init__(self, elf, dwarf_tsv):
        self.raw = open(elf, 'rb').read()
        phoff, = struct.unpack_from('<I', self.raw, 0x1C)
        phentsize, phnum = struct.unpack_from('<HH', self.raw, 0x2A)
        self.segs = []
        for i in range(phnum):
            typ, off, vaddr, paddr, filesz = struct.unpack_from('<IIIII', self.raw, phoff + i * phentsize)
            if typ == 1:
                self.segs.append((vaddr, off, filesz))
        self.funcs = []
        for r in csv.DictReader(open(dwarf_tsv, encoding='utf-8'), delimiter='\t'):
            self.funcs.append((int(r['Address'], 16), int(r['Size']), r['Name'],
                               r['Source_File'].split('\\')[-1]))
        self.funcs.sort()
        self.gp = self.find_gp()

    def read(self, addr, n):
        for a, o, s in self.segs:
            if a <= addr < a + s:
                return self.raw[o + addr - a:o + addr - a + n]
        return None

    def u32(self, addr):
        b = self.read(addr, 4)
        return struct.unpack('<I', b)[0] if b and len(b) == 4 else None

    def find_gp(self):
        """crt0 passes _gp in a0 (lui a0 / addiu a0) and later moves it to gp."""
        start = self.segs[0][0]
        hi = {}
        for p in range(start, start + 0x100, 4):
            w = self.u32(p)
            op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
            if op == 15:
                hi[rt] = (w & 0xFFFF) << 16
            elif op == 9 and rs == rt and rt in hi:
                hi[rt] = (hi[rt] + s16(w & 0xFFFF)) & 0xFFFFFFFF
            elif op == 0 and (w & 0x3F) == 0x25 and ((w >> 11) & 31) == 28:
                return hi[rs]
        raise SystemExit('gp set-up not found in crt0')

    def features(self, addr, size):
        hi, out = {}, set()
        for p in range(addr, addr + size, 4):
            w = self.u32(p)
            op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
            if op == 15:
                hi[rt] = imm << 16
            elif op == 17 and rs == 4:  # mtc1
                if rt in hi and hi[rt]:
                    out.add('F:%08x' % hi[rt])
            elif op in (9, 13, 32, 33, 35, 36, 37, 40, 41, 43, 49, 57):
                if rs == 28:
                    v = (self.gp + s16(imm)) & 0xFFFFFFFF
                elif rs in hi:
                    v = (hi[rs] + (imm if op == 13 else s16(imm))) & 0xFFFFFFFF
                    if op == 13:
                        hi[rt] = v
                else:
                    continue
                if op == 49:
                    f = self.u32(v)
                    if f:
                        out.add('F:%08x' % f)
                s = printable(self.read(v, 128))
                if s:
                    out.add('S:' + s)
        return out


def gc_features(dol, r2, addr, size, refs):
    out = set()
    for v in refs:
        s = dol.cstr(v)
        if s:
            out.add('S:' + s)
    for p in range(addr, addr + size, 4):
        w = dol.u32(p)
        if w >> 26 == 48 and ((w >> 16) & 31) == 2:  # lfs fN, d(r2)
            f = dol.u32(r2 + s16(w & 0xFFFF))
            if f:
                out.add('F:%08x' % f)
    return out


def anchors(ps2_feats, gc_feats):
    """Unique, mutual-best weighted-Jaccard matches."""
    pc = collections.Counter(x for f in ps2_feats.values() for x in f)
    gcnt = collections.Counter(x for f in gc_feats.values() for x in f)
    common = set(pc) & set(gcnt)
    weight = {x: math.log(4000 / (pc[x] + gcnt[x])) for x in common}
    inverted = collections.defaultdict(list)
    for g, f in gc_feats.items():
        for x in f & common:
            inverted[x].append(g)

    def sim(p, g):
        a, b = ps2_feats[p] & common, gc_feats[g] & common
        return sum(weight[x] for x in a & b) / sum(weight[x] for x in a | b)

    best = {}
    for p, f in ps2_feats.items():
        cands = {g for x in f & common for g in inverted[x]}
        scored = sorted(((sim(p, g), g) for g in cands), key=lambda t: (-t[0], t[1]))
        if scored:
            best[p] = scored[:2]
    top_for_g = collections.defaultdict(list)
    for p, sc in best.items():
        top_for_g[sc[0][1]].append((sc[0][0], p))
    out = {}
    for p, sc in best.items():
        s, g = sc[0]
        second = sc[1][0] if len(sc) > 1 else 0
        if s >= 0.4 and s - second >= 0.15 and max(top_for_g[g])[1] == p:
            out[p] = g
    return out


def align(ps_sizes, gc_sizes, pins):
    """Global alignment of two size sequences; pins maps ps index -> gc index."""
    n, m = len(ps_sizes), len(gc_sizes)
    gap = -0.8
    pinned_g = set(pins.values())
    lg = [math.log2(max(s, 4)) for s in gc_sizes]
    prev = [j * gap for j in range(m + 1)]
    back = [bytearray(m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        lp = math.log2(max(ps_sizes[i - 1], 4))
        pin = pins.get(i - 1)
        row = [i * gap] + [0.0] * m
        bk = back[i]
        for j in range(1, m + 1):
            if pin is not None:
                sub = 20.0 if pin == j - 1 else -50.0
            elif j - 1 in pinned_g:
                sub = -50.0
            else:
                sub = max(1.5 - 3 * abs(lg[j - 1] - lp), -2.0)
            d, u, l = prev[j - 1] + sub, prev[j] + gap, row[j - 1] + gap
            if d >= u and d >= l:
                row[j] = d
            elif u >= l:
                row[j], bk[j] = u, 1
            else:
                row[j], bk[j] = l, 2
        prev = row
    i, j, pairs = n, m, []
    while i > 0 and j > 0:
        t = back[i][j]
        if t == 0:
            pairs.append((i - 1, j - 1))
            i, j = i - 1, j - 1
        elif t == 1:
            i -= 1
        else:
            j -= 1
    return pairs[::-1]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--dol', required=True)
    ap.add_argument('--ps2-elf', required=True)
    ap.add_argument('--dwarf', default='references/Scooby-Doo Night of 100 Frights/SLUS-20349/symbols/dwarf_functions.tsv')
    ap.add_argument('--symbols', default='config/GIHE78/symbols.txt')
    ap.add_argument('--objdir', default='build/GIHE78/obj', help='objects from a dtk split of the DOL')
    ap.add_argument('--prefix', default='flat/')
    ap.add_argument('--splits', required=True, help='write the unit entries here')
    ap.add_argument('--edges', required=True, help='write the edge report (TSV) here')
    ap.add_argument('--units', help='write a per-unit report (TSV) here')
    args = ap.parse_args()

    dol = Dol(args.dol)
    syms = load_symbols(args.symbols)
    start = next(s for s in syms if s[3] == '__start')
    # __start's first call sets r1/r2/r13
    first_bl = next(p for p in range(start[0], start[0] + start[1], 4) if dol.u32(p) >> 26 == 18)
    r2, _ = find_small_data_bases(dol, first_bl + s16(dol.u32(first_bl) & 0xFFFC))
    funcs = [s for s in syms if s[4] and s[2] == '.text']
    faddr = [f[0] for f in funcs]
    data = [s for s in syms if not s[4] and s[2] in DATA_SECTIONS]
    daddr = [d[0] for d in data]

    def fidx(a):
        i = bisect.bisect_right(faddr, a) - 1
        return i if i >= 0 and a < funcs[i][0] + funcs[i][1] else None

    def didx(a):
        i = bisect.bisect_right(daddr, a) - 1
        return i if i >= 0 else None

    relocs = load_relocations(args.objdir)
    if not relocs:
        raise SystemExit('no relocations: run the dtk split first')
    func_targets = collections.defaultdict(list)
    func_data = collections.defaultdict(set)
    data_users = collections.defaultdict(set)
    for src, dst in relocs:
        f = fidx(src)
        if f is None:
            continue
        func_targets[f].append(dst)
        d = didx(dst)
        if d is not None and (not data[d][1] or dst < data[d][0] + data[d][1]):
            func_data[f].add(d)
            data_users[d].add(f)

    ps2 = Ps2(args.ps2_elf, args.dwarf)
    gc_feats = {j: gc_features(dol, r2, f[0], f[1], func_targets[j]) for j, f in enumerate(funcs)}
    gc_feats = {j: f for j, f in gc_feats.items() if f}
    ps2_feats = {i: ps2.features(f[0], f[1]) for i, f in enumerate(ps2.funcs)}
    ps2_feats = {i: f for i, f in ps2_feats.items() if f}
    seeds = anchors(ps2_feats, gc_feats)

    shared = [i for i, f in enumerate(ps2.funcs)
              if not f[3].startswith(PS2_ONLY) and not f[3].endswith('.h')]
    shared.sort(key=lambda i: (ps2.funcs[i][3].lower(), ps2.funcs[i][0]))
    game = [j for j, f in enumerate(funcs) if f[0] < GAME_END]
    gpos = {j: k for k, j in enumerate(game)}
    spos = {i: k for k, i in enumerate(shared)}
    pins = {spos[i]: gpos[g] for i, g in seeds.items() if i in spos and g in gpos}
    pairs = align([ps2.funcs[i][1] for i in shared], [funcs[j][1] for j in game], pins)

    core, anchored = {}, collections.Counter()
    seeded = set(seeds.values())
    for a, b in pairs:
        unit, g = ps2.funcs[shared[a]][3], game[b]
        lo, hi = core.get(unit, (g, g))
        core[unit] = (min(lo, g), max(hi, g))
        if g in seeded:
            anchored[unit] += 1
    order = sorted(core, key=lambda u: core[u][0])
    for u, v in zip(order, order[1:]):
        if core[u][1] >= core[v][0]:
            raise SystemExit(f'units overlap: {u} {v}')

    def owned(lo, hi, sec):
        return {d for f in range(lo, hi + 1) for d in func_data[f] if data[d][2] == sec}

    # Text cuts: the split k (A owns ..k) with the fewest data references crossing
    # the cuts implied by each neighbour's own data.
    tstart, tend, tclass = {order[0]: core[order[0]][0]}, {order[-1]: core[order[-1]][1]}, {}
    for A, B in zip(order, order[1:]):
        lo, hi = core[A][1], core[B][0]
        if hi - lo == 1:
            tend[A], tstart[B], tclass[(A, B)] = lo, hi, 'adjacent anchors'
            continue
        span = range(core[A][0], core[B][1] + 1)
        scores = []
        for k in range(lo, hi):
            total = 0
            for sec in DATA_SECTIONS:
                a, b = owned(*core[A], sec), owned(*core[B], sec)
                if not a or not b or max(a) >= min(b):
                    continue
                window = range(max(a) + 1, min(b))
                cands = [d for d in list(window) + [min(b)] if data[d][0] % DATA_ALIGN == 0] or [min(b)]
                total += min(sum(1 for f in span for d in func_data[f]
                                 if d in window and ((f <= k and d >= c) or (f > k and d < c)))
                             for c in cands)
            scores.append((total, k))
        best = min(s for s, _ in scores)
        ks = [k for s, k in scores if s == best]
        if len(ks) == 1:
            tend[A], tstart[B] = ks[0], ks[0] + 1
            tclass[(A, B)] = 'data references'
        else:
            tend[A], tstart[B] = lo, hi
            tclass[(A, B)] = f'open: {hi - lo - 1} unassigned functions'

    unit_of = {f: u for u in order for f in range(tstart[u], tend[u] + 1)}
    users = {d: {unit_of.get(f) for f in fs} for d, fs in data_users.items()}

    # Data cuts between consecutive units that own unit-local items in a section.
    ranges, dclass = {}, {}
    for sec in DATA_SECTIONS:
        seq = []
        for u in order:
            ds = [d for d in owned(tstart[u], tend[u], sec) if users[d] == {u}]
            if ds:
                seq.append([u, min(ds), max(ds), len(ds)])
        changed = True
        while changed:  # drop the smaller of two overlapping neighbours
            changed = False
            for i in range(len(seq) - 1):
                if seq[i][2] >= seq[i + 1][1]:
                    del seq[i if seq[i][3] < seq[i + 1][3] else i + 1]
                    changed = True
                    break
        for u, mn, mx, _ in seq:
            end = data[mx + 1][0] if mx + 1 < len(data) and data[mx + 1][2] == sec else data[mx][0] + data[mx][1]
            ranges[(u, sec)] = [data[mn][0], end]
        for (u, _, mx, _), (v, mn, _, _) in zip(seq, seq[1:]):
            window = range(mx + 1, mn)
            cands = [d for d in range(mx + 1, mn + 1) if data[d][0] % DATA_ALIGN == 0] or [mn]
            scored = [(sum(1 for d in window for w in users.get(d, ())
                           if (w == u and d >= c) or (w == v and d < c)), c) for c in cands]
            best = min(s for s, _ in scored)
            cs = [c for s, c in scored if s == best]
            ranges[(u, sec)][1] = data[cs[0]][0]
            ranges[(v, sec)][0] = data[cs[-1]][0]
            dclass[(u, v, sec)] = ('single aligned candidate' if len(cs) == 1
                                   else f'open: {len(cs)} aligned candidates', best)

    ps2_count = collections.Counter(ps2.funcs[i][3] for i in shared)

    def status(u):
        n, ref = tend[u] - tstart[u] + 1, ps2_count[u]
        if anchored[u] and 0.5 <= n / ref <= 2:
            return 'anchored'
        if not anchored[u] and abs(n - ref) <= 1:
            return 'count-match'
        return 'weak'

    ctors = [(CTORS + 4 * k, dol.u32(CTORS + 4 * k)) for k in range(8)]
    with open(args.splits, 'w') as out:
        for u in order:
            if status(u) == 'weak':
                continue
            lo, hi = funcs[tstart[u]][0], funcs[tend[u]][0] + funcs[tend[u]][1]
            out.write(f'\n{args.prefix}{u}:\n\t.text       start:0x{lo:08X} end:0x{hi:08X}\n')
            for slot, target in ctors:
                if target and lo <= target < hi:
                    out.write(f'\t.ctors      start:0x{slot:08X} end:0x{slot + 4:08X}\n')
            for sec in DATA_SECTIONS:
                r = ranges.get((u, sec))
                if r and r[0] < r[1]:
                    out.write(f'\t{sec:<11} start:0x{r[0]:08X} end:0x{r[1]:08X}\n')
    with open(args.edges, 'w') as out:
        out.write('section\tleft_unit\tright_unit\tedge\tclass\tconflicts\n')
        for A, B in zip(order, order[1:]):
            if 'weak' in (status(A), status(B)):
                continue
            out.write(f'.text\t{A}\t{B}\t0x{funcs[tstart[B]][0]:08X}\t{tclass[(A, B)]}\t0\n')
        kept = [(k, c) for k, c in dclass.items() if 'weak' not in (status(k[0]), status(k[1]))]
        for (u, v, sec), (cls, conflicts) in sorted(kept, key=lambda kv: (kv[0][2], ranges[(kv[0][1], kv[0][2])][0])):
            out.write(f'{sec}\t{u}\t{v}\t0x{ranges[(v, sec)][0]:08X}\t{cls}\t{conflicts}\n')
    if args.units:
        with open(args.units, 'w') as out:
            out.write('unit\ttext_start\ttext_end\tgc_functions\tps2_functions\tanchors\tstatus\n')
            for u in order:
                out.write(f'{u}\t0x{funcs[tstart[u]][0]:08X}\t0x{funcs[tend[u]][0] + funcs[tend[u]][1]:08X}\t'
                          f'{tend[u] - tstart[u] + 1}\t{ps2_count[u]}\t{anchored[u]}\t{status(u)}\n')
    print(f'anchors {len(seeds)}, aligned {len(pairs)}, units {len(order)} '
          f'({sum(status(u) != "weak" for u in order)} emitted), '
          f'data edges {len(dclass)}', file=sys.stderr)


if __name__ == '__main__':
    main()
