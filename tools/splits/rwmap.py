#!/usr/bin/env python3
"""Map GIHE78's RenderWare units and names from BfBB's RenderWare splits.

There is no public RenderWare source to compile, so units are placed by
function size sequence. BfBB (GQPE78, config/GIHE78-rev0/rwmap.json "upstream")
links RenderWare 3.5 with the same GameCube libraries and lists each object's
functions in address order. GIHE78 links 3.3.0.2 and its linker strips
unreferenced functions, so a unit's functions are a subsequence of BfBB's with
some sizes changed.

1. Placement. For each BfBB unit, a local alignment of its function sizes
   against the GIHE78 functions of the region scores an equal size
   3 + min(size, 0x320) / 0xC8, a size within 8 bytes or 6% 0.3, any other
   pair -2.5 and a gap -1. Each unit keeps up to three non-overlapping
   placements scoring 6 or more that contain at least two equal sizes, or one
   of 0x200 bytes or more, trimmed to their outermost equal sizes.
2. Order. Placements are taken best first, one per unit and without overlap.
   Of those, the highest-scoring set that keeps library order is chosen. Units
   of an "ordered" library must also keep BfBB's unit order; plugin objects may
   appear in any order within their library (GIHE78 links rpmatfx.c before the
   GameCube matfx objects).
3. Names. Inside a placement, an aligned pair of equal size takes BfBB's name
   when it is 0x20 bytes or more, or when both its neighbours in GIHE78 are
   also equal-size pairs of the unit (class A). Between two consecutive class A
   pairs, functions take BfBB's names by position when both sides hold the
   same number of functions and every size ratio is within 0.75 to 1.33
   (class B). A name BfBB marks static must only be called (bl, anywhere in
   main.dol's text) from inside its unit; otherwise the name is dropped. A
   function that already has a name row from other evidence keeps it; the
   tool reports whether the two agree.
4. Extents. A unit runs from its first to its last aligned function, widened
   to take in any function that calls one of its static names. A unit needs
   at least two names. An edge is
   provisional where the neighbouring unit's aligned function abuts it, and
   open otherwise.
5. Gaps. In each gap between mapped units, a unit takes the next function past
   its outermost name when it has the same size as the upstream next one, or a
   further one when as many functions lie between on both sides (or it is 0x20
   bytes or more). The gap's functions are then paired in order with the
   unmapped upstream units that may sit there; one with two names is mapped
   ("gap"), and the only one that may sit between two units of its library,
   both ending at their upstream ends, fills the gap when it pairs with an
   equal size of 0x20 bytes or more ("fill"). Last, a 12-byte getter returning
   an RxNodeDefinition named "<node>.csl" makes a node object ("node") when the
   PS2 June 2001 prototype lists RxNodeDefinitionGet<node> and a prototype
   source path rwsdk/<dir>/node<node>.c shares the previous unit's directory;
   it ends with its getter and starts after the previous unit unless something
   outside calls the functions before its body. Functions between units stay
   unassigned.

Corroboration, not used for placement: the PS2 June 2001 prototype symbol
table (an RW 3.x build of this game), the PS2 retail DWARF (which compiles the
collision plugin as flat/p2rwcoll), and RenderWare source paths left in the
PS2 January 2001 and Xbox prototypes.

    python3 tools/splits/rwmap.py --dol orig/GIHE78-rev0/sys/main.dol \\
        --checkout ../bfbb --splits rwunits.txt --units config/GIHE78-rev0/rw_units.tsv \\
        --evidence rwrows.tsv --symbols-out config/GIHE78-rev0/symbols.txt
"""

import argparse
import collections
import json
import re
import struct
import subprocess
import sys

SYMBOL = re.compile(r'(\S+) = \.text:0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)(.*)')
NEUTRAL = re.compile(r'fn_[0-9A-F]{8}$')
GAP = -1.0
MIN_SCORE = 6.0
KEEP_PER_UNIT = 3
CLASS_A_MIN = 0x20
RATIO = (0.75, 1.33)


def pair_score(a, b):
    if a == b:
        return 3 + min(a, 0x320) / 0xC8
    if abs(a - b) <= 8 or abs(a - b) <= 0.06 * max(a, b):
        return 0.3
    return -2.5


def load_functions(path, lo=0, hi=1 << 32):
    out = []
    for line in open(path):
        m = SYMBOL.match(line)
        if m and lo <= int(m[2], 16) < hi:
            out.append((int(m[2], 16), int(m[3], 16), m[1], 'scope:local' in m[4]))
    return sorted(out)


def load_units(path, prefix):
    units, unit = [], None
    for line in open(path):
        if line[:1].strip() and line.rstrip().endswith(':'):
            unit = line.rstrip()[:-1]
        m = re.match(r'\s+\.text\s+start:0x([0-9A-Fa-f]+) end:0x([0-9A-Fa-f]+)', line)
        if m and unit and unit.startswith(prefix):
            units.append((unit[len(prefix):], int(m[1], 16), int(m[2], 16)))
    return units


def calls(dol, start, size):
    """Targets of the bl instructions in [start, start + size)."""
    raw, sections = dol
    for off, base, length in sections:
        if base <= start and start + size <= base + length:
            words = struct.unpack_from(f'>{size // 4}I', raw, off + start - base)
            break
    else:
        return []
    out = []
    for k, w in enumerate(words):
        if w >> 26 == 18 and w & 3 == 1:
            li = w & 0x03FFFFFC
            out.append(start + 4 * k + (li - 0x04000000 if li & 0x02000000 else li))
    return out


def local_placements(sizes, seq):
    """Up to KEEP_PER_UNIT non-overlapping local alignments of seq against sizes, best first."""
    n, m = len(sizes), len(seq)
    h = [[0.0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        row, up, s = h[i], h[i - 1], sizes[i - 1]
        for j in range(1, m + 1):
            row[j] = max(0.0, up[j - 1] + pair_score(s, seq[j - 1]), up[j] + GAP, row[j - 1] + GAP)
    cells = sorted(((h[i][j], -i, j) for i in range(1, n + 1) for j in range(1, m + 1) if h[i][j] >= MIN_SCORE),
                   reverse=True)
    found = []
    for score, i, j in cells:
        i, pairs = -i, []
        while i > 0 and j > 0 and h[i][j] > 0:
            if abs(h[i][j] - (h[i - 1][j - 1] + pair_score(sizes[i - 1], seq[j - 1]))) < 1e-9:
                pairs.append((i - 1, j - 1))
                i, j = i - 1, j - 1
            elif abs(h[i][j] - (h[i - 1][j] + GAP)) < 1e-9:
                i -= 1
            else:
                j -= 1
        pairs.reverse()
        lo, hi = pairs[0][0], pairs[-1][0]
        if any(lo <= b and a <= hi for _, a, b, _ in found):
            continue
        found.append((score, lo, hi, pairs))
        if len(found) == KEEP_PER_UNIT:
            break
    return found


def gap_pairs(sizes, seq):
    """Order-preserving pairs of equal or near sizes with the largest total pair_score (no gap cost)."""
    n, m = len(sizes), len(seq)
    h = [[0.0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        for j in range(1, m + 1):
            s = pair_score(sizes[i - 1], seq[j - 1])
            h[i][j] = max(h[i - 1][j], h[i][j - 1], h[i - 1][j - 1] + s if s > 0 else 0.0)
    pairs, i, j = [], n, m
    while i and j:
        s = pair_score(sizes[i - 1], seq[j - 1])
        if s > 0 and abs(h[i][j] - h[i - 1][j - 1] - s) < 1e-9:
            pairs.append((i - 1, j - 1))
            i, j = i - 1, j - 1
        elif h[i][j] == h[i - 1][j]:
            i -= 1
        else:
            j -= 1
    return pairs[::-1]


def dol_word(dol, addr):
    raw, sections = dol
    for off, base, length in sections:
        if base <= addr and addr + 4 <= base + length:
            return struct.unpack_from('>I', raw, off + addr - base)[0]
    return None


def node_definitions(dol, gc):
    """{index: (node name, RxNodeDefinition address)} for 12-byte functions returning an RxNodeDefinition.

    Such a getter is lis/addi r3 of the definition, whose first word points to the node's "<name>.csl"."""
    out = {}
    for k, (addr, size, _, _) in enumerate(gc):
        if size != 0xC:
            continue
        i0, i1, i2 = (dol_word(dol, addr + 4 * n) for n in range(3))
        if (i0 >> 26 != 15 or (i0 >> 16) & 31 or i1 >> 26 != 14 or (i1 >> 21) & 31 != 3
                or (i1 >> 16) & 31 != (i0 >> 21) & 31 or i2 != 0x4E800020):
            continue
        lo = i1 & 0xFFFF
        target = ((i0 & 0xFFFF) << 16) + (lo - 0x10000 if lo & 0x8000 else lo)
        text = dol_word(dol, target)
        chars = bytearray()
        while text is not None and len(chars) < 64:
            word = dol_word(dol, text + len(chars))
            if word is None:
                break
            chars += word.to_bytes(4, 'big')
            if 0 in chars:
                break
        name = chars.split(b'\0')[0].decode('latin-1')
        if 0 in chars and name.endswith('.csl'):
            out[k] = (name[:-4], target)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--dol', required=True)
    ap.add_argument('--manifest', default='config/GIHE78-rev0/rwmap.json')
    ap.add_argument('--symbols', default='config/GIHE78-rev0/symbols.txt')
    ap.add_argument('--checkout', required=True, help='local checkout of the manifest upstream at its pinned commit')
    ap.add_argument('--splits', required=True)
    ap.add_argument('--units', required=True)
    ap.add_argument('--evidence', required=True)
    ap.add_argument('--evidence-in', default='config/GIHE78-rev0/evidence.tsv', help='existing rows, to skip names they cover')
    ap.add_argument('--symbols-out')
    args = ap.parse_args()
    man = json.load(open(args.manifest))
    up = man['upstream']
    head = subprocess.run(['git', '-C', args.checkout, 'rev-parse', 'HEAD'], capture_output=True, text=True).stdout.strip()
    if head != up['commit']:
        sys.exit(f"{up['name']}: checkout is at {head or '?'}, the manifest pins {up['commit']}")
    pin = f"{up['name']} {up['commit'][:7]}"
    lo, hi = (int(x, 16) for x in man['region'])

    gc = load_functions(args.symbols, lo, hi)
    sizes = [f[1] for f in gc]
    at = {f[0]: k for k, f in enumerate(gc)}
    upstream = load_functions(f"{args.checkout}/{up['symbols']}")
    bunits = load_units(f"{args.checkout}/{up['splits']}", up['unit_prefix'])

    def library(path):
        for k, lib in enumerate(man['libraries']):
            if any(path.startswith(d) for d in lib['dirs']):
                return k, lib
        sys.exit(f'{path}: no library in the manifest')

    # 1. Placements per upstream unit.
    cands = []
    for rank, (path, start, end) in enumerate(bunits):
        funcs = [f for f in upstream if start <= f[0] < end]
        for score, a, b, pairs in local_placements(sizes, [f[1] for f in funcs]):
            ends = [k for k, (i, j) in enumerate(pairs) if sizes[i] == funcs[j][1]]
            if not ends:
                continue
            pairs = pairs[ends[0]:ends[-1] + 1]
            equal = [sizes[i] for i, j in pairs if sizes[i] == funcs[j][1]]
            if len(equal) >= 2 or max(equal, default=0) >= 0x200:
                k, lib = library(path)
                a, b = pairs[0][0], pairs[-1][0]
                cands.append({'path': path, 'lib': lib['lib'], 'score': score, 'lo': a, 'hi': b,
                              'key': (k, rank if lib['ordered'] else 0),
                              'pairs': [(i, funcs[j]) for i, j in pairs], 'ufuncs': funcs})

    # 2. The highest-scoring chain of non-overlapping placements that keeps library order;
    #    a unit placed twice keeps its better placement.
    cands.sort(key=lambda c: (c['lo'], -c['score']))
    best, prev = [c['score'] for c in cands], [-1] * len(cands)
    for i, c in enumerate(cands):
        for j in range(i):
            p = cands[j]
            if p['hi'] < c['lo'] and p['key'] <= c['key'] and p['path'] != c['path'] and best[j] + c['score'] > best[i]:
                best[i], prev[i] = best[j] + c['score'], j
    chain, i = [], max(range(len(cands)), key=lambda k: (best[k], -k))
    while i >= 0:
        chain.append(cands[i])
        i = prev[i]
    top = {}
    for c in chain:
        if c['path'] not in top or c['score'] > top[c['path']]['score']:
            top[c['path']] = c
    units = sorted(top.values(), key=lambda c: c['lo'])
    for c in sorted(cands, key=lambda c: -c['score']):
        if c not in units and c['score'] >= 9:
            print(f"note: {c['path']} placement at 0x{gc[c['lo']][0]:08X} (score {c['score']:.1f}) not chosen",
                  file=sys.stderr)

    # 3. Names.
    for u in units:
        pairs = u['pairs']
        equal = [sizes[i] == f[1] for i, f in pairs]
        index = {i: k for k, (i, _) in enumerate(pairs)}
        u['names'] = {}
        for k, (i, f) in enumerate(pairs):
            if not equal[k]:
                continue
            context = all(i + d in index and equal[index[i + d]] for d in (-1, 1))
            if f[1] >= CLASS_A_MIN or context:
                u['names'][i] = (f, 'A')
        anchors = sorted(u['names'])
        uorder = {f[0]: k for k, f in enumerate(u['ufuncs'])}
        for a, b in zip(anchors, anchors[1:]):
            ua, ub = uorder[u['names'][a][0][0]], uorder[u['names'][b][0][0]]
            if b - a > 1 and b - a == ub - ua:
                fill = [(i, u['ufuncs'][ua + i - a]) for i in range(a + 1, b)]
                if all(RATIO[0] <= sizes[i] / f[1] <= RATIO[1] for i, f in fill):
                    for i, f in fill:
                        u['names'][i] = (f, 'B')

    # 4. Extents, widened over callers of static names.
    callers = collections.defaultdict(set)
    for k, f in enumerate(gc):
        for t in calls(dol_read(args.dol), f[0], f[1]):
            if t in at:
                callers[at[t]].add(k)
    foreign = collections.defaultdict(set)
    for f in load_functions(args.symbols):
        if not lo <= f[0] < hi:
            for t in calls(dol_read(args.dol), f[0], f[1]):
                if t in at:
                    foreign[at[t]].add(f[0])
    for u in units:
        u['lo'] = min(u['names'], default=u['lo'])
        u['hi'] = max(u['names'], default=u['hi'])
    for u in units:
        if len(u['names']) < 2:
            print(f"note: {u['path']} placed at 0x{gc[u['lo']][0]:08X} keeps {len(u['names'])} name(s); not mapped",
                  file=sys.stderr)
    units = [u for u in units if len(u['names']) >= 2]
    def keep_statics(units):
        for n, u in enumerate(units):
            floor = units[n - 1]['hi'] if n else -1
            ceil = units[n + 1]['lo'] if n + 1 < len(units) else len(gc)
            for i, (f, cls) in sorted(u['names'].items()):
                if not f[3]:
                    continue
                outside = [c for c in callers[i] if not u['lo'] <= c <= u['hi']]
                if foreign[i] or any(c <= floor or c >= ceil for c in outside):
                    print(f'note: static {f[2]} at 0x{gc[i][0]:08X} is called from another unit; not named',
                          file=sys.stderr)
                    del u['names'][i]
                    continue
                for c in outside:
                    u['lo'], u['hi'] = min(u['lo'], c), max(u['hi'], c)
    keep_statics(units)

    # Corroborating references.
    proto = collections.defaultdict(list)
    for line in open(man['references']['ps2_proto_symtab']):
        r = line.rstrip('\n').split('\t')
        if len(r) > 5 and r[2] == 'FUNC':
            proto[r[5]].append(int(r[1]))
    retail = {}
    for line in open(man['references']['ps2_retail_dwarf']):
        r = line.rstrip('\n').split('\t')
        if len(r) > 6 and 'p2rwcoll' in r[-1]:
            retail[r[3]] = (int(r[1]), r[-1].split('\\')[-1])
    paths = collections.defaultdict(set)
    for ref in man['references']['path_strings']:
        build = ref.split('/')[2]
        for line in open(ref):
            m = re.search(r'rwsdk/(\S+?\.c)', line)
            if m:
                paths[m[1]].add(build)

    # 5. Gaps between mapped units.
    dol = dol_read(args.dol)
    nodes = node_definitions(dol, gc)
    rank = {path: n for n, (path, _, _) in enumerate(bunits)}

    def unit_key(path):
        k, lib = library(path)
        return (k, rank.get(path, 0) if lib['ordered'] else 0)

    def edge(u, b, forward):
        """Extend u from its outermost name over the gap up to b (exclusive), along its upstream tail or head."""
        named = sorted(u['names'])
        order = {f[0]: j for j, f in enumerate(u['ufuncs'])}
        if forward:
            j0 = order[u['names'][named[-1]][0][0]] + 1
            seq, gidx = u['ufuncs'][j0:], list(range(named[-1] + 1, b))
        else:
            j0 = order[u['names'][named[0]][0][0]]
            seq, gidx = u['ufuncs'][:j0][::-1], list(range(named[0] - 1, b, -1))
        ri = rj = 0
        for i, j in gap_pairs([sizes[g] for g in gidx], [f[1] for f in seq]):
            f = seq[j]
            if sizes[gidx[i]] != f[1] or not (i - ri == j - rj or (i == ri and f[1] >= CLASS_A_MIN)):
                break
            if i - ri == j - rj:
                for x in range(ri, i):
                    fx = seq[rj + x - ri]
                    if RATIO[0] <= sizes[gidx[x]] / fx[1] <= RATIO[1]:
                        u['names'][gidx[x]] = (fx, 'B')
            u['names'][gidx[i]] = (f, 'A' if f[1] >= CLASS_A_MIN else 'E')
            ri, rj = i + 1, j + 1
        for g in gidx[:ri]:
            u['lo'], u['hi'] = min(u['lo'], g), max(u['hi'], g)

    def node_roles(k, pair_of, members):
        """Name the body and pipeline init that getter k's RxNodeDefinition points at, from the paired upstream unit."""
        out = {}
        for off in (4, 16):
            target = dol_word(dol, nodes[k][1] + off)
            i = at.get(target)
            if i is not None and i in pair_of and pair_of[i] in members:
                out[i] = (pair_of[i], 'R')
        return out

    def rest(u, forward):
        """Upstream functions past u's outermost name (none for a node object, which ends with its getter)."""
        if not u['ufuncs']:
            return 0
        order = {f[0]: j for j, f in enumerate(u['ufuncs'])}
        j = order[u['names'][max(u['names']) if forward else min(u['names'])][0][0]]
        return len(u['ufuncs']) - 1 - j if forward else j

    for u in units:
        u['method'] = 'chain'
    snapshot = sorted(units, key=lambda u: u['lo'])
    for n in range(len(snapshot) + 1):
        prev = snapshot[n - 1] if n else None
        nxt = snapshot[n] if n < len(snapshot) else None
        if prev:
            edge(prev, nxt['lo'] if nxt else len(gc), True)
        if nxt:
            edge(nxt, prev['hi'] if prev else -1, False)
        a, b = prev['hi'] + 1 if prev else 0, nxt['lo'] if nxt else len(gc)
        if a >= b:
            continue
        lo_key, hi_key = (prev['key'] if prev else (-1,)), (nxt['key'] if nxt else (len(man['libraries']),))
        mapped = {u['path'] for u in units}
        seq = [(path, f) for path, start, end in bunits
               if path not in mapped and lo_key <= unit_key(path) <= hi_key
               for f in upstream if start <= f[0] < end]
        pairs = gap_pairs(sizes[a:b], [f[1] for _, f in seq])
        pair_of = {a + i: seq[j][1] for i, j in pairs}
        path_of = {a + i: seq[j][0] for i, j in pairs}
        slot = {a + i: j for i, j in pairs}
        found = []
        for path in dict.fromkeys(path_of.values()):
            idx = sorted(i for i in path_of if path_of[i] == path)
            equal = {i for i in idx if sizes[i] == pair_of[i][1]}
            names = {}
            for i in equal:
                context = all(i + d in equal and path_of[i + d] == path and slot[i + d] == slot[i] + d for d in (-1, 1))
                if pair_of[i][1] >= CLASS_A_MIN or context:
                    names[i] = (pair_of[i], 'A')
            members = [f for p, f in seq if p == path]
            order = {f[0]: j for j, f in enumerate(members)}
            anchors = sorted(names)
            for x, y in zip(anchors, anchors[1:]):
                ux, uy = order[names[x][0][0]], order[names[y][0][0]]
                if y - x > 1 and y - x == uy - ux and all(
                        RATIO[0] <= sizes[i] / members[ux + i - x][1] <= RATIO[1] for i in range(x + 1, y)):
                    for i in range(x + 1, y):
                        names[i] = (members[ux + i - x], 'B')
            for k in idx:
                if k in nodes and k in equal and pair_of[k][2].startswith('RxNodeDefinitionGet'):
                    names[k] = (pair_of[k], 'N')
                    names.update(node_roles(k, pair_of, members))
            # The only upstream unit that can sit between two units of its library, both of which end at
            # their upstream ends, fills the gap when it pairs in order with an equal size of 0x20 or more.
            sole = (len(set(path_of.values())) == 1 == len({p for p, _ in seq}) and prev and nxt
                    and prev['lib'] == nxt['lib'] == library(path)[1]['lib'] and not rest(prev, True)
                    and not rest(nxt, False) and len(idx) >= 2 and any(pair_of[i][1] >= CLASS_A_MIN for i in equal))
            if len(names) >= 2 or sole:
                found.append({'path': path, 'lib': library(path)[1]['lib'], 'score': 0.0, 'key': unit_key(path),
                              'lo': a if sole else min(names), 'hi': b - 1 if sole else max(names), 'names': names,
                              'ufuncs': members, 'pairs': [], 'method': 'fill' if sole else 'gap'})
        found.sort(key=lambda u: u['lo'])
        for m, u in enumerate(found):
            if m and found[m - 1]['hi'] >= u['lo']:
                print(f"note: {u['path']} gap placement at 0x{gc[u['lo']][0]:08X} overlaps; not mapped", file=sys.stderr)
                continue
            # Same number of functions on both sides out to an abutting neighbour: the extent reaches it.
            if u['method'] == 'gap':
                order = {f[0]: j for j, f in enumerate(u['ufuncs'])}
                floor = found[m - 1]['hi'] + 1 if m else a
                if order[u['names'][u['lo']][0][0]] == u['lo'] - floor and (m or prev):
                    u['lo'] = floor
                ceil = found[m + 1]['lo'] - 1 if m + 1 < len(found) else b - 1
                if (len(u['ufuncs']) - 1 - order[u['names'][u['hi']][0][0]] == ceil - u['hi']
                        and (m + 1 < len(found) or nxt)):
                    u['hi'] = ceil
            units.append(u)
        # Node objects with no upstream unit: the getter returns a definition named "<node>.csl", the PS2
        # prototype lists RxNodeDefinitionGet<node>, and a prototype source string names node<node>.c in
        # the directory of the unit before it. The object ends with its getter.
        for k in sorted(nodes):
            if not a <= k < b or any(u['lo'] <= k <= u['hi'] for u in units):
                continue
            node, definition = nodes[k]
            name = 'RxNodeDefinitionGet' + node
            before = max((u for u in units if u['hi'] < k), key=lambda u: u['hi'], default=None)
            body = at.get(dol_word(dol, definition + 4))
            where = [p for p in paths if p.endswith('/node' + node + '.c')
                     and before and p.rsplit('/', 1)[0] == before['path'].rsplit('/', 1)[0]]
            if name not in proto or len(where) != 1 or body is None or not before['hi'] < body < k:
                continue
            start = before['hi'] + 1
            if any(foreign[i] or any(not start <= c <= k for c in callers[i]) for i in range(start, body)):
                start = body
            units.append({'path': where[0], 'lib': library(where[0])[1]['lib'], 'score': 0.0,
                          'key': unit_key(where[0]), 'lo': start, 'hi': k, 'ufuncs': [], 'pairs': [],
                          'names': {k: ((0, 0xC, name, False), 'N')}, 'method': 'node', 'node': (node, definition)})
    units.sort(key=lambda u: u['lo'])
    keep_statics(units)

    # Name rows this tool did not write, so a run on the committed files reproduces its own rows.
    others = {}
    for line in open(args.evidence_in):
        r = line.rstrip('\n').split('\t')
        if r[0] == 'name' and r[1] != '-' and 'tools/splits/rwmap.py' not in r[7]:
            others[int(r[1], 16)] = r[3]
    used_names = {f[2] for f in load_functions(args.symbols) if not NEUTRAL.match(f[2]) and not lo <= f[0] < hi}
    seen = collections.Counter(f[2] for u in units for f, _ in u['names'].values())
    rows, renames, report = [], {}, []
    for n, u in enumerate(units):
        start, end = gc[u['lo']][0], gc[u['hi']][0] + gc[u['hi']][1]
        left = n > 0 and units[n - 1]['hi'] + 1 == u['lo']
        right = n + 1 < len(units) and units[n + 1]['lo'] == u['hi'] + 1
        named = sorted(u['names'].items())
        ka = sum(cls in 'AE' for _, (_, cls) in named)
        kb = sum(cls == 'B' for _, (_, cls) in named)
        first = named[0]
        seen_in = sorted(paths.get(u['path'], ()))
        where = (f" The path rwsdk/{u['path']} is also a source string in the {' and '.join(seen_in)} builds."
                 if seen_in else '')
        edges = ("An edge is provisional where the next unit's aligned function abuts it; otherwise the object "
                 "may extend past it.")
        if u['method'] == 'node':
            node, definition = u['node']
            how = (f"tools/splits/rwmap.py node object: its last function 0x{gc[u['hi']][0]:08X} returns the "
                   f"RxNodeDefinition at 0x{definition:08X}, whose name is \"{node}.csl\"; the PS2 June 2001 "
                   f"prototype symbol table lists {first[1][0][2]}, and the {' and '.join(seen_in)} builds name "
                   f"this node's source rwsdk/{u['path']}, in the directory of the unit before it. "
                   f"{u['hi'] - u['lo'] + 1} GC functions in the extent. The object is taken to end with its "
                   f"getter: in the node objects bfbb maps (nodeGameCubeAtomicAllInOne.c, "
                   f"nodeGameCubeWorldSectorAllInOne.c, nodeDolphinSubmitNoLight.c) a getter ends the object or "
                   f"is followed by the next node's functions. "
                   + ("It starts after the unit before it: no function between that unit and the node body the "
                      "definition points at is called (bl) from outside this extent."
                      if u['lo'] < at[dol_word(dol, definition + 4)] else
                      "It starts at the node body the definition points at; the functions before it are not "
                      "assigned.")
                   + " The compiler profile and data ownership are not established.")
        else:
            how = (f"tools/splits/rwmap.py {dict(chain='size-sequence', gap='gap', fill='gap-fill')[u['method']]} placement of "
                   f"{pin} {up['splits']} unit {up['unit_prefix']}{u['path']} (RenderWare 3.5, "
                   f"{len(u['ufuncs'])} functions)"
                   + {'chain': '', 'gap': ', aligned in order inside the gap between the mapped units on either side',
                      'fill': ', the only unit of its library that can sit between the units on either side, which '
                              'both end at their upstream ends; it pairs in order with the gap\'s functions, at least '
                              'one of them at an equal size of 0x20 bytes or more, so it takes the whole gap'}[u['method']]
                   + f": {u['hi'] - u['lo'] + 1} GC functions in the extent, {len(named)} named ({ka} by equal "
                   f"size), first 0x{gc[first[0]][0]:08X}={first[1][0][2]}. {edges}{where} The compiler "
                   f"profile and data ownership are not established.")
        rows.append(['unit', f'0x{start:08X}', f'0x{end:08X}', up['unit_prefix'] + u['path'], 'related',
                     'provisional' if left else 'open', 'provisional' if right else 'open', how])
        report.append([up['unit_prefix'] + u['path'], f'0x{start:08X}', f'0x{end:08X}', u['lib'], u['method'],
                       str(u['hi'] - u['lo'] + 1), str(len(u['ufuncs'])), str(ka), str(kb),
                       str(len(named) - ka - kb), f"{u['score']:.1f}"])
        for i, (f, cls) in named:
            a, s, name = gc[i][0], gc[i][1], f[2]
            if a in others:
                print(f"note: 0x{a:08X} already has a name row for {others[a]}; "
                      f"{'it agrees' if others[a] == name else f'{name} is not applied'}", file=sys.stderr)
                continue
            if not NEUTRAL.match(gc[i][2]) and gc[i][2] != name:
                print(f'note: 0x{a:08X} is already named {gc[i][2]}, not {name}; left as is', file=sys.stderr)
                continue
            if seen[name] > 1 or name in used_names:
                print(f'note: {name} at 0x{a:08X} is not unique; not named', file=sys.stderr)
                continue
            renames[a] = name
            unit = f"unit {up['unit_prefix']}{u['path']}"
            if cls == 'A':
                how = f"{name} is a 0x{f[1]:X}-byte function in {pin} {up['symbols']}, the same size as this one"
            elif cls == 'B':
                how = (f"{name} (0x{f[1]:X} bytes in {pin} {up['symbols']}, 0x{s:X} here) holds this position "
                       f"between two equal-size names, with the same number of functions between them on both sides")
            elif cls == 'E':
                how = (f"{name} (0x{f[1]:X} bytes in {pin} {up['symbols']}) and this function, the same size, lie "
                       f"the same number of functions past this unit's outermost size-placed name in both builds")
            elif cls == 'N' and u['method'] == 'node':
                how = (f"this function returns the RxNodeDefinition at 0x{u['node'][1]:08X}, whose name is "
                       f"\"{u['node'][0]}.csl\"; RenderWare names a node's getter RxNodeDefinitionGet<node>, "
                       f"and the PS2 June 2001 prototype symbol table lists {name}")
            elif cls == 'N':
                how = (f"{name} is the 0xC-byte node getter of {pin} {unit}; this function, paired with it in "
                       f"order, returns the RxNodeDefinition at 0x{nodes[i][1]:08X} (\"{nodes[i][0]}.csl\")")
            else:
                getter, role = next((k, r) for k, (_, c) in named if c == 'N'
                                    for off, r in ((4, 'node body'), (16, 'pipeline-node init'))
                                    if dol_word(dol, nodes[k][1] + off) == a)
                how = (f"the RxNodeDefinition returned by 0x{gc[getter][0]:08X} ({u['names'][getter][0][2]}) "
                       f"points its {role} at this function; {pin} lists {name} (0x{f[1]:X} bytes) in {unit}, "
                       f"paired with this function in order")
            how += (' (tools/splits/rwmap.py node object).' if u['method'] == 'node' else
                    f", aligned in the placement of {unit} (tools/splits/rwmap.py).")
            if f[3]:
                how += ' BfBB lists it as static; every direct GC call to it (bl, whole text) comes from inside this unit.'
            notes = []
            if name in retail:
                notes.append(f'the PS2 retail SLUS-20349 DWARF lists {name} ({retail[name][0]} bytes) in '
                             f'flat\\p2rwcoll\\{retail[name][1]}')
            if name in proto:
                notes.append(f"the PS2 June 2001 prototype symbol table lists {name} ({proto[name][0]} bytes)")
            if notes:
                how += ' Also, ' + '; '.join(notes) + '.'
            how += ' Positional: a swap of similar-sized neighbours would go unnoticed.' if cls == 'B' else ''
            how += ' A related build\'s name; there is no GC debug symbol.'
            rows.append(['function', f'0x{a:08X}', f'0x{a + s:08X}', name, 'target', 'provisional', 'provisional',
                         f"dtk 1.8.3 function inference. "
                         + ('A 12-byte lis/addi/blr getter.' if u['method'] == 'node' else
                            f"{pin} lists {name} at 0x{f[1]:X} bytes.")
                         + " Neither edge has been checked against a compiled body."])
            rows.append(['name', f'0x{a:08X}', '-', name, 'related', '-', '-', how])

    with open(args.splits, 'w') as out:
        for r in rows:
            if r[0] == 'unit':
                out.write(f'\n{r[3]}:\n\t.text       start:{r[1]} end:{r[2]}\n')
    with open(args.units, 'w') as out:
        out.write('unit\ttext_start\ttext_end\tlibrary\tmethod\tfunctions\tupstream_functions\tnamed_equal\t'
                  'named_position\tnamed_node\tscore\n')
        for r in report:
            out.write('\t'.join(r) + '\n')
    with open(args.evidence, 'w') as out:
        for r in rows:
            out.write('\t'.join(r) + '\n')
    if args.symbols_out:
        lines = open(args.symbols).readlines()
        with open(args.symbols_out, 'w') as out:
            for line in lines:
                m = SYMBOL.match(line)
                if m and int(m[2], 16) in renames:
                    line = renames[int(m[2], 16)] + line[len(m[1]):]
                out.write(line)
    byclass = collections.Counter(c for u in units for i, (f, c) in u['names'].items() if gc[i][0] in renames)
    print(f"{len(cands)} placements, {len(units)} units, {len(renames)} names "
          f"({byclass['A'] + byclass['E']} equal size, {byclass['B']} by position, "
          f"{byclass['N'] + byclass['R']} from node definitions)", file=sys.stderr)

_dol = {}


def dol_read(path):
    if path not in _dol:
        raw = open(path, 'rb').read()
        h = struct.unpack('>18I18I18I', raw[:0xD8])
        _dol[path] = (raw, [(h[k], h[18 + k], h[36 + k]) for k in range(18) if h[36 + k]])
    return _dol[path]


if __name__ == '__main__':
    main()
