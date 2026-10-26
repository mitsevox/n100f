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
        kind = re.search(r'data:(\w+)', m[4])
        out.append((int(m[3], 16), int(size[1], 16) if size else 0, m[2], m[1],
                    'type:function' in m[4], kind[1] if kind else None))
    out.sort()
    return out


def load_relocations(objdir):
    """(source address, target address) for every relocation in dtk's split objects.

    dtk names local labels <prefix>_<ADDR>, so each section's load address follows
    from any such symbol's value."""
    out, skipped = [], 0
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
                skipped += 1
                continue
            b, = bases
            for k in range(s[5] // 12):
                off, info, addend = struct.unpack_from('>IIi', raw, s[4] + k * 12)
                m = re.search(r'_([0-9A-F]{8})$', syms[info >> 8][0])
                if m:
                    out.append((b + off, int(m[1], 16) + addend))
    return out, skipped


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
    ap.add_argument('--game-end', type=lambda x: int(x, 0), default=0x800D7E00,
                    help='end of the game block; GC-only code starts here with iAnimBlend (BfBB gc/iAnim.cpp)')
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

    relocs, skipped = load_relocations(args.objdir)
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
    game = [j for j, f in enumerate(funcs) if f[0] < args.game_end]
    gpos = {j: k for k, j in enumerate(game)}
    spos = {i: k for k, i in enumerate(shared)}
    pins = {spos[i]: gpos[g] for i, g in seeds.items() if i in spos and g in gpos}
    pairs = align([ps2.funcs[i][1] for i in shared], [funcs[j][1] for j in game], pins)

    core, anchored, first_anchor = {}, collections.Counter(), {}
    seeded = set(seeds.values())
    for a, b in pairs:
        unit, g = ps2.funcs[shared[a]][3], game[b]
        lo, hi = core.get(unit, (g, g))
        core[unit] = (min(lo, g), max(hi, g))
        if g in seeded and seeds.get(shared[a]) == g:
            anchored[unit] += 1
            first_anchor.setdefault(unit, (g, shared[a]))
    order = sorted(core, key=lambda u: core[u][0])
    for u, v in zip(order, order[1:]):
        if core[u][1] >= core[v][0]:
            raise SystemExit(f'units overlap: {u} {v}')

    def owned(lo, hi, sec, skip=()):
        return {d for f in range(lo, hi + 1) if f not in skip
                for d in func_data[f] if data[d][2] == sec}

    # .ctors entries point at static initialisers, which MW emits last in their unit.
    ctors_base = next(s[0] for s in syms if s[3] == '_ctors')
    ctors = []
    while dol.u32(ctors_base + 4 * len(ctors)):
        ctors.append((ctors_base + 4 * len(ctors), dol.u32(ctors_base + 4 * len(ctors))))
    sinit = {fidx(t) for _, t in ctors}

    # Text cuts.  Around each edge, up to eight functions on either side may move.
    # A cut k (A owns ..k) is scored by the references that land on the wrong side
    # of the best data cut in each section: a function on B's side loading A's
    # data, or the reverse.  .sdata2 literals are unit-local, so they decide most
    # edges.  Moving an anchor across the cut costs three references.
    tstart, tend, tclass, tconf = {order[0]: core[order[0]][0]}, {order[-1]: core[order[-1]][1]}, {}, {}
    for A, B in zip(order, order[1:]):
        # each unit lends its first half to the edge before it and its second half to the edge after
        wlo = max(tstart[A], core[A][1] - 8, (core[A][0] + core[A][1] + 1) // 2)
        whi = min(core[B][0] + 8, (core[B][0] + core[B][1]) // 2)
        window = set(range(wlo, whi + 1))
        span = set(range(core[A][0], core[B][1] + 1))
        sides = []
        for sec in DATA_SECTIONS:
            a = owned(core[A][0], core[A][1], sec, window)
            b = owned(core[B][0], core[B][1], sec, window)
            if a and b and max(a) >= min(b):
                continue
            # items only this edge's functions load; a single cut must separate A's from B's
            local = {d for f in window for d in func_data[f]
                     if data[d][2] == sec and data_users[d] <= span}
            local = {d for d in local if (not a or d > max(a)) and (not b or d < min(b))}
            if local:
                cands = sorted(local | ({min(b)} if b else {max(local) + 1}))
                sides.append((sec, a, b, cands))

        def crossings(k):
            # an anchor moved to the other unit costs as much as three crossing references
            n = 3 * sum(1 for f in window if f in seeded and ((f <= k) != (f <= core[A][1])))
            for sec, a, b, cands in sides:
                n += min(sum(1 for f in window for d in func_data[f] if data[d][2] == sec
                             and ((f > k and (d < c or d in a)) or (f <= k and (d >= c or d in b))))
                         for c in cands)
            return n

        ks = range(max(wlo - 1, tstart[A]), min(whi, core[B][1] - 1) + 1)
        forced = [k for k in ks if k in sinit]
        scores = {k: crossings(k) for k in (forced or ks)}
        best = min(scores.values())
        ties = sorted(k for k, v in scores.items() if v == best)
        aligned_cut = core[A][1] if core[B][0] - core[A][1] == 1 else None
        if forced and len(ties) == 1:
            cut, cls = ties[0], 'static initialiser ends unit'
        elif len(ties) == 1:
            cut, cls = ties[0], 'single cut by data references'
        elif aligned_cut in ties:
            cut, cls = aligned_cut, f'alignment only ({len(ties)} cuts tie on data)'
        else:
            cut, cls = None, f'open: {ties[-1] - ties[0]} unassigned functions'
        if cut is None:
            tend[A], tstart[B] = ties[0], ties[-1] + 1
        else:
            tend[A], tstart[B] = cut, cut + 1
        tclass[(A, B)], tconf[(A, B)] = cls, best

    unit_of = {f: u for u in order for f in range(tstart[u], tend[u] + 1)}
    users = {d: {unit_of.get(f) for f in fs} for d, fs in data_users.items()}

    def aligned(d):
        return data[d][0] % DATA_ALIGN == 0

    # Data cuts between consecutive units that own unit-local items in a section.
    # Items that unassigned functions load stay out of every unit.
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
            ranges[(u, sec)] = [mn, end]
        for (u, _, mx, _), (v, mn, _, _) in zip(seq, seq[1:]):
            between = range(mx + 1, mn)
            floating = [d for d in between if None in users.get(d, ())]
            lo_c = min(floating) if floating else mn          # A may end no later than here
            hi_c = max(floating) + 1 if floating else mx + 1  # B may start no earlier than here
            cands = [d for d in range(mx + 1, mn + 1) if aligned(d)]
            a_end = [c for c in cands if c <= lo_c]
            b_start = [c for c in cands if c >= hi_c]
            if not floating:
                scored = {c: sum(1 for d in between for w in users.get(d, ())
                                 if (w == u and d >= c) or (w == v and d < c)) for c in cands}
                best = min(scored.values()) if scored else 0
                ties = [c for c in cands if scored[c] == best]
                a_end, b_start = ties[:1], ties[-1:]
            else:
                best = 0
            ranges[(u, sec)][1] = data[a_end[0]][0] if a_end else None
            ranges[(v, sec)][0] = b_start[-1] if b_start else None
            if floating:
                cls = f'open: {len(floating)} items loaded by unassigned functions'
            elif len(a_end) == 1 and a_end == b_start:
                cls = 'single aligned candidate'
            else:
                cls = f'open: {b_start[-1] - a_end[0] + 1 if a_end and b_start else 0} items between aligned candidates'
            dclass[(u, v, sec)] = (cls, best)

    # A section range is written only if it starts on an aligned symbol and ends
    # at a known edge; anything else stays in dtk's automatic objects.
    final = {}
    for (u, sec), (mn, end) in ranges.items():
        if mn is None or end is None:
            continue
        if not aligned(mn):
            back = [d for d in range(mn - 1, -1, -1) if data[d][2] == sec and aligned(d)]
            if not back or any(users.get(d) and users[d] != {u} for d in range(back[0], mn)):
                continue
            mn = back[0]
        if data[mn][0] < end:
            final[(u, sec)] = (data[mn][0], end)

    ps2_count = collections.Counter(ps2.funcs[i][3] for i in shared)

    def status(u):
        n, ref = tend[u] - tstart[u] + 1, ps2_count[u]
        if anchored[u] and 0.5 <= n / ref <= 2:
            return 'anchored'
        if not anchored[u] and abs(n - ref) <= 1:
            return 'count-match'
        return 'weak'

    # An item inside a written range that an unassigned function loads is a reference
    # from outside every unit.  It counts as a crossing on the edge facing that function:
    # the data edge in the same section if there is one, else the text edge.
    dnext = {(u, sec): v for (u, v, sec) in dclass}
    dprev = {(v, sec): u for (u, v, sec) in dclass}
    inside = collections.Counter()
    for d, (addr, _, sec, *_) in enumerate(data):
        for (u, s), (lo, hi) in final.items():
            if s != sec or not lo <= addr < hi or status(u) == 'weak':
                continue
            for f in data_users.get(d, ()):
                if f in unit_of:
                    continue
                if f < tstart[u]:
                    key = (dprev[(u, sec)], u, sec) if (u, sec) in dprev else (order[order.index(u) - 1], u)
                else:
                    key = (u, dnext[(u, sec)], sec) if (u, sec) in dnext else (u, order[order.index(u) + 1])
                inside[key] += 1
    for key, n in inside.items():
        note = f'; {n} in-range loads by unassigned functions'
        if len(key) == 3:
            dclass[key] = (dclass[key][0] + note, dclass[key][1] + n)
        else:
            tclass[key], tconf[key] = tclass[key] + note, tconf[key] + n

    # Target-only checks on the result.  MW pools .sdata2 literals per unit, so
    # no function may load a literal inside another unit's .sdata2 range (a global
    # const in .sdata2 would break this; none has turned up).  As advice only, a
    # unit range holding the same float twice is reported: pooled literals are
    # merged, but a named static const is not merged with a literal.
    emitted = [u for u in order if status(u) != 'weak']
    text_owner = {f: u for u in emitted for f in range(tstart[u], tend[u] + 1)}
    sd_owner = {}
    for u in emitted:
        if (u, '.sdata2') in final:
            lo, hi = final[(u, '.sdata2')]
            for d, item in enumerate(data):
                if item[2] == '.sdata2' and lo <= item[0] < hi:
                    sd_owner[d] = u
    locality = [(f, d) for d, fs in data_users.items() if d in sd_owner
                for f in fs if text_owner.get(f) != sd_owner[d]]
    duplicates = []
    for u in emitted:
        seen = {}
        for d in sorted(d for d, o in sd_owner.items() if o == u):
            if data[d][5] in ('float', 'double'):
                key = dol.read(data[d][0], data[d][1])
                if key in seen:
                    duplicates.append((seen[key], d))
                seen[key] = d
    for f, d in locality:
        print(f'check: 0x{funcs[f][0]:08X} ({text_owner.get(f) or "unassigned"}) loads '
              f'0x{data[d][0]:08X} in {sd_owner[d]} .sdata2', file=sys.stderr)
    for a, b in duplicates:
        print(f'advice: {sd_owner[a]} .sdata2 holds the float at 0x{data[a][0]:08X} '
              f'again at 0x{data[b][0]:08X}', file=sys.stderr)

    with open(args.splits, 'w') as out:
        for u in order:
            if status(u) == 'weak':
                continue
            lo, hi = funcs[tstart[u]][0], funcs[tend[u]][0] + funcs[tend[u]][1]
            out.write(f'\n{args.prefix}{u}:\n\t.text       start:0x{lo:08X} end:0x{hi:08X}\n')
            for slot, target in ctors:
                if lo <= target < hi:
                    out.write(f'\t.ctors      start:0x{slot:08X} end:0x{slot + 4:08X}\n')
            for sec in DATA_SECTIONS:
                r = final.get((u, sec))
                if r:
                    out.write(f'\t{sec:<11} start:0x{r[0]:08X} end:0x{r[1]:08X}\n')
    with open(args.edges, 'w') as out:
        out.write('section\tleft_unit\tright_unit\tleft_end\tright_start\tclass\tcrossing_refs\n')
        for A, B in zip(order, order[1:]):
            note = ''.join(f'; {w} left unsplit' for w in (A, B) if status(w) == 'weak')
            out.write(f'.text\t{A}\t{B}\t0x{funcs[tend[A]][0] + funcs[tend[A]][1]:08X}\t'
                      f'0x{funcs[tstart[B]][0]:08X}\t{tclass[(A, B)]}{note}\t{tconf[(A, B)]}\n')
        for (u, v, sec), (cls, conflicts) in sorted(dclass.items(), key=lambda kv: (kv[0][2], order.index(kv[0][0]))):
            a = final.get((u, sec)) if status(u) != 'weak' else None
            b = final.get((v, sec)) if status(v) != 'weak' else None
            out.write(f'{sec}\t{u}\t{v}\t{"0x%08X" % a[1] if a else "-"}\t{"0x%08X" % b[0] if b else "-"}\t'
                      f'{cls if a and b else cls + "; a side left unsplit"}\t{conflicts}\n')
    if args.units:
        with open(args.units, 'w') as out:
            out.write('unit\ttext_start\ttext_end\tgc_functions\tps2_functions\tanchors\tfirst_anchor\tstatus\n')
            for u in order:
                fa = first_anchor.get(u)
                fa = f'0x{funcs[fa[0]][0]:08X}={ps2.funcs[fa[1]][2]}' if fa else '-'
                out.write(f'{u}\t0x{funcs[tstart[u]][0]:08X}\t0x{funcs[tend[u]][0] + funcs[tend[u]][1]:08X}\t'
                          f'{tend[u] - tstart[u] + 1}\t{ps2_count[u]}\t{anchored[u]}\t{fa}\t{status(u)}\n')
    print(f'relocations {len(relocs)} ({skipped} relocation sections skipped), anchors {len(seeds)}, '
          f'aligned {len(pairs)}, units {len(order)} ({sum(status(u) != "weak" for u in order)} emitted), '
          f'data edges {len(dclass)}; .sdata2 locality violations {len(locality)}, '
          f'repeated floats {len(duplicates)} (advice)', file=sys.stderr)
    if locality:
        sys.exit(1)


if __name__ == '__main__':
    main()
