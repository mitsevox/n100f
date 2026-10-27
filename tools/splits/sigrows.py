#!/usr/bin/env python3
"""Write evidence rows for the names dtk's signature analysis puts in symbols.txt.

decomp-toolkit applies its built-in signatures (src/analysis/signatures.rs) at
the address of each symbol a signature is keyed on, and names the matched
function and every symbol its relocations reach. When a split writes
symbols.txt back, those names land there without a row. For each name in
symbols.txt that has no `name` row in evidence.tsv, this re-checks the same
signatures against main.dol and writes a `name` row (and a `function` row for
functions) citing the dtk version, the signature file and the entry hash.

    python3 tools/splits/sigrows.py --dtk-src ../decomp-toolkit \\
        --dol orig/GIHE78-rev0/sys/main.dol --symbols config/GIHE78-rev0/symbols.txt \\
        --evidence config/GIHE78-rev0/evidence.tsv --output sigrows.tsv

Names that no signature reaches are listed on stderr; they need their own rows.
"""

import argparse
import base64
import csv
import re
import struct
import subprocess
import sys

import yaml

SYMBOL = re.compile(r'(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // (.*)')
NEUTRAL = re.compile(r'(fn|lbl|jumptable|switch|@etb|@eti)_[0-9A-F]{8}$')


class Dol:
    def __init__(self, path):
        self.raw = open(path, 'rb').read()
        head = struct.unpack('>18I18I18I', self.raw[:216])
        self.sections = list(zip(head[:18], head[18:36], head[36:]))
        self.entry = struct.unpack('>I', self.raw[0xE0:0xE4])[0]

    def read(self, addr, size):
        for offset, start, length in self.sections:
            if length and start <= addr and addr + size <= start + length:
                return self.raw[offset + addr - start:offset + addr - start + size]
        return None


def matches(dol, addr, entry):
    sig = base64.b64decode(entry['signature'])
    data = dol.read(addr, len(sig) // 2)
    if data is None:
        return False
    for i in range(0, len(sig), 8):
        ins, mask = struct.unpack('>II', sig[i:i + 8])
        if struct.unpack('>I', data[i // 2:i // 2 + 4])[0] & mask != ins:
            return False
    return True


def signature_files(dtk_src):
    source = open(dtk_src + '/src/analysis/signatures.rs').read()
    source = '\n'.join(line for line in source.splitlines() if not line.strip().startswith('//'))
    keyed = re.findall(r'\(\s*"([^"]+)",\s*include_str!\("../../assets/signatures/([^"]+)"\),?\s*\)', source)
    return [('__start', '__start.yml')] + keyed


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    parser.add_argument('--dtk-src', required=True, help='decomp-toolkit checkout at the pinned version')
    parser.add_argument('--dol', required=True)
    parser.add_argument('--symbols', required=True)
    parser.add_argument('--evidence', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()

    version = subprocess.run(['git', '-C', args.dtk_src, 'describe', '--tags', '--always'],
                             capture_output=True, text=True).stdout.strip() or 'unknown'
    dol = Dol(args.dol)
    symbols = {}
    for line in open(args.symbols):
        m = SYMBOL.match(line)
        if m:
            attrs = dict(a.split(':', 1) for a in m.group(4).split() if ':' in a)
            symbols[m.group(1)] = (int(m.group(3), 16), m.group(2), attrs)
    covered = {(r[3], r[1]) for r in csv.reader(open(args.evidence), delimiter='\t') if r and r[0] == 'name'}

    # dtk checks a file at the address of the symbol it is keyed on; the first matching entry wins.
    reached = {}
    for key, path in signature_files(args.dtk_src):
        addr = dol.entry if key == '__start' else symbols.get(key, (None,))[0]
        if addr is None:
            continue
        for entry in yaml.safe_load(open(args.dtk_src + '/assets/signatures/' + path)):
            if not matches(dol, addr, entry):
                continue
            primary = entry['symbols'][entry['symbol']]
            for index, sym in enumerate(entry['symbols']):
                reached.setdefault(sym['name'], []).append(
                    (index == entry['symbol'], path, entry['hash'], primary['name'], addr, sym['size']))
            break

    rows, missing = [], []
    for name, (addr, section, attrs) in sorted(symbols.items(), key=lambda s: s[1][0]):
        if NEUTRAL.match(name) or (name, '0x%08X' % addr) in covered:
            continue
        hits = reached.get(name)
        if not hits:
            missing.append(name)
            continue
        hits.sort(key=lambda h: not h[0])
        is_primary, path, digest, primary, at, sig_size = hits[0]
        source = 'decomp-toolkit %s assets/signatures/%s, entry hash %s' % (version, path, digest)
        size = int(attrs.get('size', '0'), 16)
        if is_primary:
            how = 'the signature matches this 0x%X-byte body with relocated fields masked' % sig_size
        else:
            how = 'the signature matches %s at 0x%08X, and one of its relocations resolves to this address as %s' % (
                primary, at, name)
        others = sorted({h[1] for h in hits[1:]} - {path})
        also = ' Also reached from %s.' % ', '.join(others) if others else ''
        kind = attrs.get('type', 'object')
        if kind == 'function':
            end = 'exact' if is_primary and sig_size == size else 'provisional'
            extent = '' if end == 'exact' else ' The end is dtk analysis, not the signature.'
            rows.append(['function', '0x%08X' % addr, '0x%08X' % (addr + size), name, 'signature', 'exact', end,
                         'Named by dtk\'s analysis from %s: %s.%s%s' % (source, how, extent, also)])
        rows.append(['name', '0x%08X' % addr, '-', name, 'signature', '-', '-',
                     'Named by dtk\'s analysis from %s: %s (%s %s). dtk writes the name back to symbols.txt '
                     'when a split runs.%s' % (source, how, section, kind, also)])
    with open(args.output, 'w', newline='') as out:
        csv.writer(out, delimiter='\t', lineterminator='\n', quoting=csv.QUOTE_NONE, escapechar='\\').writerows(rows)
    if missing:
        print('no signature reaches: ' + ' '.join(missing), file=sys.stderr)
    print('%d rows' % len(rows))


if __name__ == '__main__':
    main()
