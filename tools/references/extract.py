#!/usr/bin/env python3
"""Export clean-room reference metadata from local executables.

Writes symbol tables, DWARF 1 exports, section layouts, source anchors and
diagnostic strings for one build into a references/ directory. Inputs are
local executables that never enter Git; only the derived text is written.

    python3 tools/references/extract.py elf  SLUS_203.49   OUTDIR
    python3 tools/references/extract.py dol  main.dol      OUTDIR
    python3 tools/references/extract.py xbe  default.xbe   OUTDIR
    python3 tools/references/extract.py psx  PSX.EXE       OUTDIR
"""

import argparse
import os
import re
import struct
import sys
from collections import OrderedDict, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from dwarf1 import (  # noqa: E402
    AT_BIT_OFFSET, AT_BIT_SIZE, AT_BYTE_SIZE, AT_HIGH_PC, AT_LOW_PC, TAG,
    Dwarf1,
)

AT_MW_MANGLED = 0x2008
PATH_RE = re.compile(rb"[A-Za-z0-9_:\\/.\- ]+\.(?:c|cpp|cc|h|hpp|s|inl)\b",
                     re.I)
DIAG_RE = re.compile(
    r"%[-+ #0]*\d*(?:\.\d+)?[hlL]*[diouxXeEfgGcspn]|assert|error|warn|fail|"
    r"debug|invalid|cannot|can't|unable|overflow|host:|cdrom|\.c\b|\.cpp\b|"
    r"\.h\b|\.hip\b|\.ini\b|\.\w{3}\b", re.I)


def write(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)


def tsv(path, header, rows):
    write(path, "\t".join(header) + "\n" +
          "".join("\t".join(str(c) for c in r) + "\n" for r in rows))


def hx(v, w=8):
    return "0x%0*X" % (w, v)


# ---- strings ---------------------------------------------------------------

def scan_strings(blob, base, minlen=5):
    """Yield (address, text) for NUL-terminated printable ASCII strings."""
    for m in re.finditer(rb"[\x09\x20-\x7e]{%d,}\x00" % minlen, blob):
        s = m.group()[:-1].decode("ascii")
        if sum(ch.isalpha() for ch in s) < max(3, len(s) // 3):
            continue
        yield base + m.start(), s


def export_strings(out, regions, title):
    """regions: iterable of (base_address, bytes)."""
    found = OrderedDict()
    for base, blob in regions:
        for addr, s in scan_strings(blob, base):
            found.setdefault(s, addr)
    diag = [(a, s) for s, a in found.items() if DIAG_RE.search(s)]
    anchors = [(a, s) for s, a in found.items()
               if PATH_RE.fullmatch(s.encode("ascii", "ignore").strip())
               or re.search(r"[\\/][\w.\-]+\.(?:c|cpp|h)\b", s)]
    lines = ["# %s" % title,
             "# Diagnostic, assert, format and path strings: %d unique" %
             len(diag), ""]
    lines += ["%s\t%s" % (hx(a), s.replace("\t", "\\t")) for a, s in diag]
    write(os.path.join(out, "strings", "debug_strings.txt"),
          "\n".join(lines) + "\n")
    tsv(os.path.join(out, "symbols", "source_anchors.tsv"),
        ("Address", "Path"), [(hx(a), s) for a, s in anchors])
    return len(diag), len(anchors)


# ---- ELF -------------------------------------------------------------------

SHT = {0: "NULL", 1: "PROGBITS", 2: "SYMTAB", 3: "STRTAB", 8: "NOBITS",
       9: "REL", 0x70000005: "MIPS_DWARF", 0x70000006: "MIPS_REGINFO"}
STT = {0: "NOTYPE", 1: "OBJECT", 2: "FUNC", 3: "SECTION", 4: "FILE"}
STB = {0: "LOCAL", 1: "GLOBAL", 2: "WEAK"}


class Elf:
    def __init__(self, data):
        self.d = data
        if data[:4] != b"\x7fELF":
            raise SystemExit("not an ELF")
        self.e = "<" if data[5] == 1 else ">"
        u = lambda f, o: struct.unpack_from(self.e + f, data, o)[0]
        self.entry = u("I", 0x18)
        shoff, shnum, shstr = u("I", 0x20), u("H", 0x30), u("H", 0x32)
        self.secs = []
        raw = [struct.unpack_from(self.e + "10I", data, shoff + i * 40)
               for i in range(shnum)]
        st = raw[shstr]
        names = data[st[4]:st[4] + st[5]]
        for i, s in enumerate(raw):
            nm = names[s[0]:names.index(b"\0", s[0])].decode("latin1")
            self.secs.append(dict(idx=i, name=nm, type=s[1], flags=s[2],
                                  addr=s[3], off=s[4], size=s[5], link=s[6]))
        self.by_name = {s["name"]: s for s in self.secs}

    def body(self, s):
        return self.d[s["off"]:s["off"] + s["size"]]

    def symbols(self):
        out = []
        for s in self.secs:
            if s["type"] != 2:
                continue
            strtab = self.body(self.secs[s["link"]])
            b = self.body(s)
            for i in range(len(b) // 16):
                nm, val, size, info, other, shndx = struct.unpack_from(
                    self.e + "IIIBBH", b, i * 16)
                if i == 0:
                    continue
                name = strtab[nm:strtab.index(b"\0", nm)].decode("latin1")
                sec = (self.secs[shndx]["name"] if 0 < shndx < len(self.secs)
                       else {0: "UND", 0xFFF1: "ABS", 0xFFF2: "COMMON"}.get(
                           shndx, hex(shndx)))
                out.append((val, size, STT.get(info & 0xF, info & 0xF),
                            STB.get(info >> 4, info >> 4), sec, name))
        return out


def export_elf(path, out):
    data = open(path, "rb").read()
    elf = Elf(data)
    summary = {"entry": elf.entry, "size": len(data)}
    tsv(os.path.join(out, "symbols", "elf_sections.tsv"),
        ("Section", "Type", "Address", "Offset", "Size"),
        [(s["name"], SHT.get(s["type"], hx(s["type"])), hx(s["addr"]),
          hx(s["off"]), s["size"]) for s in elf.secs if s["idx"]])
    com = elf.by_name.get(".comment")
    if com:
        summary["comment"] = [c.decode("latin1") for c in
                              elf.body(com).split(b"\0") if c]
    syms = elf.symbols()
    if syms:
        syms.sort(key=lambda r: (r[0], r[5]))
        tsv(os.path.join(out, "symbols", "symtab.tsv"),
            ("Address", "Size", "Type", "Binding", "Section", "Name"),
            [(hx(a), sz, t, b, sec, n) for a, sz, t, b, sec, n in syms])
        summary["symbols"] = len(syms)
        summary["func_symbols"] = sum(1 for r in syms if r[2] == "FUNC")
    dbg = elf.by_name.get(".debug")
    if dbg:
        dw = Dwarf1(elf.body(dbg), endian=elf.e)
        summary.update(export_dwarf(dw, out))
        line = elf.by_name.get(".line")
        if line:
            summary["line_rows"] = export_lines(dw, elf.body(line), elf.e, out)
    regions = [(s["addr"], elf.body(s)) for s in elf.secs
               if s["type"] == 1 and s["addr"] and s["size"]]
    summary["strings"] = export_strings(
        out, regions, "Strings from %s" % os.path.basename(path))
    return summary


# ---- DWARF 1 ---------------------------------------------------------------

def short_file(p):
    return re.split(r"[\\/]", p)[-1] if p else ""


class DwarfExport:
    def __init__(self, dw):
        self.dw = dw

    def ty(self, die, inner=""):
        return self.dw.type_name(self.dw.type_of(die), inner)

    def signature(self, fn):
        name = fn.name or "?"
        params = self.dw.param_list(fn)
        return self.ty(fn, "%s(%s)" % (name, ", ".join(params)))

    def loc_text(self, die):
        loc = self.dw.location(die)
        if loc is None:
            return ""
        if loc[0] == "reg":
            return "r%d" % loc[1]
        if loc[0] == "stack":
            return "r%d%+#x" % (loc[1], loc[2])
        return hx(loc[1])

    # Types are emitted once per distinct definition.
    def type_key(self, die):
        mem = tuple((m.name, self.dw.member_offset(m), self.ty(m))
                    for m in die.children if m.tag == 0x0D)
        return (die.tag, die.name, die.get(AT_BYTE_SIZE), mem,
                tuple(self.dw.enum_values(die)) if die.tag == 0x04 else ())

    def render_type(self, die):
        dw = self.dw
        tg = TAG[die.tag]
        size = die.get(AT_BYTE_SIZE)
        name = die.name or "<anon_0x%X>" % die.off
        if tg == "enumeration_type":
            vals = ",\n".join("    %s = %d" % kv for kv in dw.enum_values(die))
            return "enum %s {\n%s\n};" % (name, vals)
        if tg == "typedef":
            return "typedef %s;" % self.ty(die, name)
        kw = {"structure_type": "struct", "class_type": "class",
              "union_type": "union"}[tg]
        bases = [re.sub(r"^(class|struct|union) ", "", self.ty(c))
                 for c in die.children if c.tag == 0x1C]
        head = "%s %s" % (kw, name)
        if bases:
            head += " : " + ", ".join("public " + b for b in bases)
        lines = ["// size: %s" % (hx(size, 0) if size is not None else "?"),
                 head + " {"]
        for m in die.children:
            if m.tag != 0x0D:
                continue
            off = dw.member_offset(m)
            decl = self.ty(m, m.name or "")
            if AT_BIT_SIZE in m.attrs:
                decl += " : %d" % m.attrs[AT_BIT_SIZE]
                decl += " /* bit_offset %d */" % m.attrs.get(AT_BIT_OFFSET, 0)
            lines.append("    /* %s */ %s;" %
                         (hx(off, 4) if off is not None else "  ?   ", decl))
        lines.append("};")
        return "\n".join(lines)

    def body(self, die, depth, lines):
        pad = "    " * depth
        for c in die.children:
            tg = TAG.get(c.tag)
            if tg == "local_variable":
                loc = self.dw.location(c)
                where = self.loc_text(c)
                static = "static " if loc and loc[0] == "addr" else ""
                lines.append("%s%s%s; // %s" % (pad, static,
                                                self.ty(c, c.name or ""),
                                                where))
            elif tg == "lexical_block":
                lines.append("%s{ // %s - %s" % (
                    pad, hx(c.get(AT_LOW_PC, 0)), hx(c.get(AT_HIGH_PC, 0))))
                self.body(c, depth + 1, lines)
                lines.append(pad + "}")
            elif tg in ("subroutine", "global_subroutine"):
                self.function(c, depth, lines)

    def function(self, fn, depth, lines):
        pad = "    " * depth
        lo, hi = fn.get(AT_LOW_PC), fn.get(AT_HIGH_PC)
        mangled = fn.get(AT_MW_MANGLED)
        note = "// %s - %s (%s)" % (hx(lo), hx(hi), hx(hi - lo, 0)) \
            if lo is not None and hi is not None else "// (no code)"
        if mangled and mangled != fn.name:
            note += " " + mangled
        lines.append(pad + note)
        static = "static " if fn.tag == 0x14 else ""
        lines.append("%s%s%s {" % (pad, static, self.signature(fn)))
        self.body(fn, depth + 1, lines)
        lines.append(pad + "}")


def export_dwarf(dw, out):
    ex = DwarfExport(dw)
    funcs, vars_, files = [], [], OrderedDict()
    types, type_files = OrderedDict(), {}
    dump = defaultdict(list)

    def walk_types(die, cu_name):
        for c in die.children:
            if c.tag in (0x02, 0x04, 0x13, 0x16, 0x17) and c.name:
                if c.tag == 0x16 or c.children or c.tag == 0x04:
                    k = ex.type_key(c)
                    if k not in types:
                        types[k] = c
                        type_files[k] = cu_name
            if c.tag in (0x02, 0x13, 0x17, 0x06, 0x14, 0x0B):
                walk_types(c, cu_name)

    for cu in dw.roots:
        if cu.tag != 0x11:
            continue
        name = cu.name or "?"
        f = files.setdefault(name, dict(funcs=0, lo=None, hi=None))
        lo, hi = cu.get(AT_LOW_PC), cu.get(AT_HIGH_PC)
        if lo is not None and hi is not None and hi > lo:
            f["lo"] = lo if f["lo"] is None else min(f["lo"], lo)
            f["hi"] = hi if f["hi"] is None else max(f["hi"], hi)
        walk_types(cu, name)
        for c in cu.children:
            tg = TAG.get(c.tag)
            if tg in ("global_subroutine", "subroutine"):
                f["funcs"] += 1
                lo, hi = c.get(AT_LOW_PC), c.get(AT_HIGH_PC)
                funcs.append((lo if lo is not None else 0,
                              (hi - lo) if lo is not None and hi else 0,
                              "GLOBAL" if c.tag == 0x06 else "LOCAL",
                              c.name or "", c.get(AT_MW_MANGLED, ""),
                              ex.signature(c), name))
                ex.function(c, 0, dump[name])
                dump[name].append("")
            elif tg in ("global_variable", "local_variable"):
                loc = dw.location(c)
                addr = loc[1] if loc and loc[0] == "addr" else None
                vars_.append((addr if addr is not None else 0,
                              "GLOBAL" if c.tag == 0x07 else "LOCAL",
                              c.name or "", c.get(AT_MW_MANGLED, ""),
                              ex.ty(c), name))
                dump[name].insert(0, "%s%s; // %s" % (
                    "" if c.tag == 0x07 else "static ",
                    ex.ty(c, c.name or ""),
                    hx(addr) if addr is not None else "extern"))

    funcs.sort(key=lambda r: r[0])
    vars_.sort(key=lambda r: r[0])
    tsv(os.path.join(out, "symbols", "dwarf_functions.tsv"),
        ("Address", "Size", "Binding", "Name", "Mangled", "Signature",
         "Source_File"),
        [(hx(a), s, b, n, m, sig, f) for a, s, b, n, m, sig, f in funcs])
    tsv(os.path.join(out, "symbols", "dwarf_variables.tsv"),
        ("Address", "Binding", "Name", "Mangled", "Type", "Source_File"),
        [(hx(a), b, n, m, t, f) for a, b, n, m, t, f in vars_])
    tsv(os.path.join(out, "symbols", "source_files.tsv"),
        ("Source_File", "Functions", "Low_PC", "High_PC"),
        [(n, f["funcs"], hx(f["lo"]) if f["lo"] is not None else "",
          hx(f["hi"]) if f["hi"] is not None else "")
         for n, f in files.items()])

    by_kind = defaultdict(list)
    for k, die in types.items():
        by_kind[die.tag].append((die.name, type_files[k], die))
    hdr = ["// Type definitions recovered from DWARF 1 (.debug).",
           "// One entry per distinct definition; '// from' names the first",
           "// compile unit that described it. Offsets are member offsets.",
           ""]
    for tag in (0x04, 0x16, 0x13, 0x02, 0x17):
        for name, src, die in sorted(by_kind[tag], key=lambda r: r[0]):
            hdr.append("// from %s" % short_file(src))
            hdr.append(ex.render_type(die))
            hdr.append("")
    write(os.path.join(out, "symbols", "dwarf_types.h"), "\n".join(hdr))

    dl = ["// Per-source-file DWARF 1 dump: globals, statics and functions",
          "// with parameters, locals (register or stack slot) and scopes.",
          ""]
    for name in files:
        dl.append("// " + "=" * 74)
        dl.append("// " + name)
        dl.append("// " + "=" * 74)
        dl.extend(dump[name])
        dl.append("")
    write(os.path.join(out, "symbols", "dwarf_dump.cpp"), "\n".join(dl))

    return {"dwarf_functions": len(funcs), "dwarf_variables": len(vars_),
            "source_files": len(files), "types": len(types),
            "producer": sorted({cu.get(0x0258) for cu in dw.roots
                                if cu.get(0x0258)})}


def export_lines(dw, data, e, out):
    """DWARF 1 .line: per unit, u32 length, u32 base, then (line, col, delta)."""
    rows, seen = [], set()
    for cu in dw.roots:
        off = cu.get(0x0106)  # AT_stmt_list
        if cu.tag != 0x11 or off is None or off in seen:
            continue
        seen.add(off)
        ln, base = struct.unpack_from(e + "II", data, off)
        i = off + 8
        while i + 10 <= off + ln:
            line, col, delta = struct.unpack_from(e + "IHI", data, i)
            i += 10
            if line:
                rows.append((base + delta, line, cu.name or ""))
    rows.sort()
    tsv(os.path.join(out, "symbols", "line_table.tsv"),
        ("Address", "Line", "Source_File"),
        [(hx(a), l, f) for a, l, f in rows])
    return len(rows)


# ---- GameCube DOL ----------------------------------------------------------

def export_dol(path, out):
    d = open(path, "rb").read()
    offs = struct.unpack_from(">18I", d, 0)
    addrs = struct.unpack_from(">18I", d, 0x48)
    sizes = struct.unpack_from(">18I", d, 0x90)
    bss_a, bss_s, entry = struct.unpack_from(">3I", d, 0xD8)
    rows, regions = [], []
    for i in range(18):
        if not sizes[i]:
            continue
        nm = ".text%d" % i if i < 7 else ".data%d" % (i - 7)
        rows.append((nm, hx(offs[i]), hx(addrs[i]), sizes[i]))
        if i >= 7:
            regions.append((addrs[i], d[offs[i]:offs[i] + sizes[i]]))
    rows.append((".bss", "", hx(bss_a), bss_s))
    tsv(os.path.join(out, "symbols", "dol_sections.tsv"),
        ("Section", "Offset", "Address", "Size"), rows)
    s = export_strings(out, regions, "Strings from %s" %
                       os.path.basename(path))
    return {"entry": entry, "size": len(d), "strings": s}


# ---- Xbox XBE --------------------------------------------------------------

def export_xbe(path, out):
    d = open(path, "rb").read()
    u = lambda o: struct.unpack_from("<I", d, o)[0]
    base = u(0x104)
    va = lambda a: a - base
    cert = d[va(u(0x118)):]
    title_id = struct.unpack_from("<I", cert, 8)[0]
    title = cert[0xC:0xC + 80].decode("utf-16-le").split("\0")[0]
    nsec, sec_hdr = u(0x11C), va(u(0x120))
    rows, regions = [], []
    for i in range(nsec):
        h = struct.unpack_from("<9I", d, sec_hdr + i * 56)
        flags, vaddr, vsize, raddr, rsize, nameaddr = h[:6]
        nm = d[va(nameaddr):d.index(b"\0", va(nameaddr))].decode("latin1")
        rows.append((nm, hx(vaddr), vsize, hx(raddr), rsize, hx(flags)))
        if nm != ".text":
            regions.append((vaddr, d[raddr:raddr + rsize]))
    tsv(os.path.join(out, "symbols", "xbe_sections.tsv"),
        ("Section", "Address", "VirtualSize", "RawOffset", "RawSize",
         "Flags"), rows)
    nlib, lib = u(0x160), va(u(0x164))
    libs = []
    for i in range(nlib):
        nm, maj, mnr, bld, fl = struct.unpack_from("<8sHHHH", d, lib + i * 16)
        libs.append((nm.rstrip(b"\0").decode("latin1"),
                     "%d.%d.%d" % (maj, mnr, bld), hx(fl, 4)))
    tsv(os.path.join(out, "symbols", "xbe_libraries.tsv"),
        ("Library", "Version", "Flags"), libs)
    pdb = d[va(u(0x14C)):d.index(b"\0", va(u(0x14C)))].decode("latin1")
    s = export_strings(out, regions, "Strings from %s" %
                       os.path.basename(path))
    return {"title": title, "title_id": hx(title_id), "base": base,
            "timestamp": u(0x114), "debug_path": pdb,
            "entry_enc": u(0x128), "libraries": libs, "strings": s}


# ---- PS1 PSX-EXE -----------------------------------------------------------

def export_psx(path, out):
    d = open(path, "rb").read()
    pc0, gp0, t_addr, t_size = struct.unpack_from("<4I", d, 0x10)
    sp = struct.unpack_from("<I", d, 0x30)[0]
    region = d[0x4C:0x80].split(b"\0")[0].decode("latin1")
    tsv(os.path.join(out, "symbols", "exe_header.tsv"),
        ("Field", "Value"),
        [("pc0", hx(pc0)), ("gp0", hx(gp0)), ("t_addr", hx(t_addr)),
         ("t_size", t_size), ("sp", hx(sp)), ("marker", region)])
    s = export_strings(out, [(t_addr, d[0x800:0x800 + t_size])],
                       "Strings from %s" % os.path.basename(path))
    return {"entry": pc0, "load": t_addr, "size": t_size, "strings": s}


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("kind", choices=("elf", "dol", "xbe", "psx"))
    ap.add_argument("input")
    ap.add_argument("out")
    a = ap.parse_args()
    fn = {"elf": export_elf, "dol": export_dol, "xbe": export_xbe,
          "psx": export_psx}[a.kind]
    for k, v in fn(a.input, a.out).items():
        print("%s: %s" % (k, v))


if __name__ == "__main__":
    main()
