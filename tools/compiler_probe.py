#!/usr/bin/env python3
"""Search the target DOL for functions compiled under candidate compiler profiles.

Compiles each source with every listed CodeWarrior version, masks the
relocated fields of each .text function, and reports where that exact
instruction sequence occurs in the target's text sections. A function that
appears under one compiler version and not another is version evidence; the
source itself is only a stimulus and establishes nothing about names.

    python3 tools/compiler_probe.py --dol orig/GIHE78/sys/main.dol \\
        --compilers build/compilers --wrapper build/tools/wibo \\
        --root ../bfbb --versions 1.3,1.3.2,2.0 \\
        --flags-file probe_flags.txt src/SB/Core/x/xsavegame.cpp ...

Output is JSON: {source: {version: {function: [words, [hit addresses]]}}}.
With --summary N it is a text table instead: per version, the units that
failed to compile and the bodies of at least N words found exactly once, then
every such body that is found under some versions and not others.
Nothing is written into the repository.
"""

import argparse
import json
import os
import shlex
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor

# Bits replaced by the linker for each PowerPC relocation type.
RELOC_MASKS = {
    1: 0xFFFFFFFF,  # R_PPC_ADDR32
    4: 0x0000FFFF,  # R_PPC_ADDR16_LO
    5: 0x0000FFFF,  # R_PPC_ADDR16_HI
    6: 0x0000FFFF,  # R_PPC_ADDR16_HA
    10: 0x03FFFFFC,  # R_PPC_REL24
    11: 0x0000FFFC,  # R_PPC_REL14
    109: 0x001FFFFF,  # R_PPC_EMB_SDA21 (base register and offset)
}
MIN_WORDS = 6


def dol_text(path):
    with open(path, "rb") as f:
        data = f.read()
    sections = []
    for i in range(7):
        off, addr, size = (struct.unpack_from(">I", data, base + 4 * i)[0]
                           for base in (0x00, 0x48, 0x90))
        if size:
            words = struct.unpack_from(f">{size // 4}I", data, off)
            index = {}
            for n, w in enumerate(words):
                index.setdefault(w, []).append(n)
            sections.append((addr, words, index))
    return sections


def elf_functions(path):
    """Yield (name, words, masks) for every sized function in a .text section."""
    with open(path, "rb") as f:
        data = f.read()
    shoff, = struct.unpack_from(">I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
    shdrs = [struct.unpack_from(">10I", data, shoff + i * shentsize) for i in range(shnum)]

    def name_at(table, off):
        start = shdrs[table][4] + off
        return data[start:data.index(b"\0", start)].decode()

    names = [name_at(shstrndx, h[0]) for h in shdrs]
    text = {i for i, n in enumerate(names) if n == ".text" or n.startswith(".text.")}
    words = {}
    masks = {}
    for i in text:
        h = shdrs[i]
        words[i] = struct.unpack_from(f">{h[5] // 4}I", data, h[4])
        masks[i] = [0xFFFFFFFF] * len(words[i])
    for h in shdrs:
        if h[1] == 4 and h[7] in text:  # SHT_RELA applying to a text section
            for off in range(h[4], h[4] + h[5], 12):
                r_offset, r_info, _ = struct.unpack_from(">IIi", data, off)
                masks[h[7]][r_offset // 4] &= ~RELOC_MASKS.get(r_info & 0xFF, 0xFFFFFFFF)
    for h in shdrs:
        if h[1] != 2:  # SHT_SYMTAB
            continue
        for off in range(h[4], h[4] + h[5], 16):
            st_name, value, size, info, _, shndx = struct.unpack_from(">IIIBBH", data, off)
            if info & 0xF == 2 and shndx in text and size:
                a, n = value // 4, size // 4
                yield name_at(h[6], st_name), words[shndx][a:a + n], masks[shndx][a:a + n]


def search(sections, words, masks):
    pivot = next((k for k, m in enumerate(masks) if m == 0xFFFFFFFF), None)
    hits = []
    if pivot is None:
        return hits
    for addr, text, index in sections:
        for c in index.get(words[pivot], ()):
            start = c - pivot
            if start < 0 or start + len(words) > len(text):
                continue
            if all((text[start + k] ^ w) & m == 0 for k, (w, m) in enumerate(zip(words, masks))):
                hits.append(f"0x{addr + 4 * start:08X}")
    return hits


def probe(args, sections, flags, version, source):
    compiler = os.path.join(args.compilers, "GC", version, "mwcceppc.exe")
    with tempfile.TemporaryDirectory() as tmp:
        obj = os.path.join(tmp, "probe.o")
        cmd = ([args.wrapper] if args.wrapper else []) + [compiler, *flags, "-c", source, "-o", obj]
        proc = subprocess.run(cmd, cwd=args.root, capture_output=True, text=True)
        if proc.returncode or not os.path.exists(obj):
            return {"error": (proc.stdout + proc.stderr).strip()[-400:]}
        return {name: [len(w), search(sections, w, m)]
                for name, w, m in elf_functions(obj) if len(w) >= MIN_WORDS}


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--dol", required=True)
    parser.add_argument("--compilers", required=True, help="directory containing GC/<version>/")
    parser.add_argument("--wrapper", help="wibo or wine, when not on Windows")
    parser.add_argument("--root", default=".", help="directory the sources and -i paths are relative to")
    parser.add_argument("--versions", required=True, help="comma-separated, e.g. 1.3,1.3.2,2.0")
    parser.add_argument("--flags-file", required=True, help="compiler flags, shell-quoted")
    parser.add_argument("--jobs", type=int, default=os.cpu_count())
    parser.add_argument("--sources-file", help="file listing sources, one per line")
    parser.add_argument("--summary", type=int, metavar="N", help="print the text summary for bodies of N+ words")
    parser.add_argument("sources", nargs="*")
    args = parser.parse_args()
    args.compilers = os.path.abspath(args.compilers)
    if args.wrapper and os.sep in args.wrapper:
        args.wrapper = os.path.abspath(args.wrapper)

    with open(args.flags_file, encoding="utf-8") as f:
        flags = shlex.split(f.read(), comments=True)
    if args.sources_file:
        with open(args.sources_file, encoding="utf-8") as f:
            args.sources += [line.strip() for line in f if line.strip() and not line.startswith("#")]
    if not args.sources:
        parser.error("no sources given")
    versions = args.versions.split(",")
    sections = dol_text(args.dol)
    jobs = [(v, s) for s in args.sources for v in versions]
    with ThreadPoolExecutor(args.jobs) as pool:
        results = list(pool.map(lambda j: probe(args, sections, flags, *j), jobs))
    out = {}
    for (version, source), result in zip(jobs, results):
        out.setdefault(source, {})[version] = result
    if args.summary is None:
        json.dump(out, sys.stdout, indent=1, sort_keys=True)
        print()
    else:
        summarize(out, versions, args.summary)


def summarize(out, versions, min_words):
    """Print unique-body counts per version and the bodies that separate versions."""
    def unique(result, name):
        entry = result.get(name)
        return entry[1][0] if entry and len(entry[1]) == 1 and entry[0] >= min_words else None

    print("version\tfailed_units\tunique_bodies")
    for v in versions:
        results = [units[v] for units in out.values()]
        failed = sum(1 for r in results if "error" in r)
        found = sum(1 for r in results if "error" not in r for n in r if unique(r, n))
        print(f"{v}\t{failed}\t{found}")
    print("\nsource\tfunction\twords\taddress\t" + "\t".join(versions))
    for source, units in sorted(out.items()):
        ok = [v for v in versions if "error" not in units[v]]
        for name in sorted({n for v in ok for n in units[v]}):
            hits = {v: unique(units[v], name) for v in ok}
            addrs = {a for a in hits.values() if a}
            if len(addrs) == 1 and None in hits.values():
                words = next(units[v][name][0] for v in ok if hits[v])
                marks = ["x" if hits.get(v) else ("-" if v in ok else "err") for v in versions]
                print(f"{source}\t{name}\t{words}\t{addrs.pop()}\t" + "\t".join(marks))


if __name__ == "__main__":
    main()
