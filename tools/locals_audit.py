#!/usr/bin/env python3
"""Compare each function's local variables with the SLUS-20349 DWARF.

    python3 tools/locals_audit.py --target GIHE78-rev0 [--unit flat/xMath.cpp] [--tsv out.tsv]

For every game unit (src/flat, src/gc) whose file name has a DWARF compile unit in
references/.../SLUS-20349/symbols/dwarf_dump.cpp, the source is parsed with clang
(the arguments come from build/<target>/compile_commands.json, so build the target
first) and each defined function's local variable names are compared with the names
the DWARF lists for the function of the same name. Parameters are not compared.

The DWARF lists one flat set of locals per function (nested scopes are merged) and
counts static locals; this tool does the same on the source side. A function that the
DWARF names more than once in a unit (overloads) is matched only when the parameter
count picks one entry. The result is a report, not evidence: GC and PS2 builds can
differ, and platform files (i*.cpp) are PS2 code in the DWARF.
"""

import argparse
import json
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DUMP = ROOT / "references" / "Scooby-Doo Night of 100 Frights" / "SLUS-20349" / "symbols" / "dwarf_dump.cpp"

UNIT_RE = re.compile(r"^// (E:\\SD2\\Game\\flat\\[^\\]+)$")
FUNC_RE = re.compile(r"^// 0x[0-9A-F]+ - 0x[0-9A-F]+ \(0x[0-9A-F]+\) (\S+)$")
LOCAL_RE = re.compile(r"^\s+(.+?)\s*;\s*//.*$")


def demangled_base(mangled):
    """Return (qualified name, parameter count or None) from a CW/PS2 mangled name."""
    m = re.match(r"^(.+?)__(\d+)([A-Za-z_]\w*)F(.*)$", mangled)
    if m and len(m.group(3)) == int(m.group(2)):
        return m.group(3) + "::" + m.group(1), None
    m = re.match(r"^(.+?)__F(.*)$", mangled)
    if m:
        return m.group(1), None
    return mangled, None


def parse_dwarf():
    units = defaultdict(list)  # basename lower -> [(qualname, mangled, header, [locals])]
    unit = None
    cur = None
    depth = 0
    for line in DUMP.read_text(encoding="utf-8", errors="replace").splitlines():
        m = UNIT_RE.match(line)
        if m:
            unit = m.group(1).split("\\")[-1].lower()
            cur = None
            continue
        if unit is None:
            continue
        if line.startswith("// ====="):
            continue
        m = FUNC_RE.match(line)
        if m:
            qual, _ = demangled_base(m.group(1))
            cur = {"qual": qual, "mangled": m.group(1), "header": None, "locals": []}
            units[unit].append(cur)
            depth = 0
            continue
        if cur is None:
            continue
        if cur["header"] is None and line.strip():
            cur["header"] = line.strip()
            continue
        s = line.strip()
        if s.startswith("{"):
            depth += 1
            continue
        if s == "}":
            depth -= 1
            if depth < 0:
                cur = None
            continue
        if depth >= 1:
            m = LOCAL_RE.match(line)
            if m:
                decl = m.group(1)
                decl = re.sub(r"\[[^\]]*\]", "", decl)
                decl = re.sub(r"\(\*?\s*(\w+)\)\s*\(.*\)$", r"\1", decl)
                name = re.findall(r"[A-Za-z_]\w*", decl)
                if name:
                    cur["locals"].append(name[-1])
    return units


def header_param_count(header):
    m = re.search(r"\((.*)\)\s*\{?$", header or "")
    if not m:
        return None
    inner = m.group(1).strip()
    if inner in ("", "void"):
        return 0
    depth = 0
    n = 1
    for ch in inner:
        if ch in "(<":
            depth += 1
        elif ch in ")>":
            depth -= 1
        elif ch == "," and depth == 0:
            n += 1
    return n


class FileTracker:
    """Follow clang's JSON AST, which prints a location's file only when it changes."""

    def __init__(self, main):
        self.main = main
        self.cur = None

    def see(self, loc):
        if not isinstance(loc, dict):
            return
        for key in ("spellingLoc", "expansionLoc"):
            if key in loc:
                self.see(loc[key])
        if "file" in loc:
            self.cur = loc["file"]

    def visit(self, node):
        self.see(node.get("loc"))
        rng = node.get("range")
        if isinstance(rng, dict):
            self.see(rng.get("begin"))
            self.see(rng.get("end"))


def source_functions(entry):
    args = entry["arguments"] if "arguments" in entry else entry["command"].split()
    out_i = args.index("-o") if "-o" in args else None
    if out_i is not None:
        args = args[:out_i] + args[out_i + 2:]
    args = [a for a in args if a != "-c"]
    cmd = args + ["-fsyntax-only", "-w", "-Xclang", "-ast-dump=json"]
    res = subprocess.run(cmd, cwd=entry.get("directory", ROOT), capture_output=True)
    if not res.stdout:
        raise RuntimeError(res.stderr.decode(errors="replace")[-400:])
    ast = json.loads(res.stdout)
    main = entry["file"]
    tr = FileTracker(main)
    funcs = []

    def collect_locals(node, acc):
        tr.visit(node)
        kind = node.get("kind")
        if kind == "VarDecl" and node.get("name"):
            acc.append(node["name"])
        if kind in ("LambdaExpr", "CXXRecordDecl") and acc is not None:
            pass
        for ch in node.get("inner", []) or []:
            collect_locals(ch, acc)

    def walk(node, scope):
        tr.visit(node)
        kind = node.get("kind")
        if kind in ("NamespaceDecl", "LinkageSpecDecl"):
            for ch in node.get("inner", []) or []:
                walk(ch, scope)
            return
        if kind == "CXXRecordDecl" and node.get("name"):
            for ch in node.get("inner", []) or []:
                walk(ch, scope + [node["name"]])
            return
        if kind in ("FunctionDecl", "CXXMethodDecl", "CXXConstructorDecl", "CXXDestructorDecl"):
            in_main = tr.cur == main
            inner = node.get("inner", []) or []
            params = 0
            body = None
            for ch in inner:
                if ch.get("kind") == "ParmVarDecl":
                    params += 1
                    tr.visit(ch)
                    for g in ch.get("inner", []) or []:
                        collect_locals(g, [])
                elif ch.get("kind") == "CompoundStmt":
                    body = ch
                else:
                    collect_locals(ch, [])
            if body is not None:
                locs = []
                collect_locals(body, locs)
                if in_main:
                    name = node.get("name", "")
                    parent = node.get("parentDeclContextId")
                    qual = "::".join(scope + [name]) if scope else name
                    funcs.append({"qual": qual, "name": name, "params": params,
                                  "locals": locs, "line": node.get("loc", {}).get("line"),
                                  "static_inline": bool(node.get("inline")) or node.get("storageClass") == "static",
                                  "out_of_class": parent is not None})
            return
        for ch in node.get("inner", []) or []:
            walk(ch, scope)

    walk(ast, [])
    # out-of-class method definitions: clang JSON gives no class scope on the
    # definition node; recover Class:: from the source text of the definition line.
    text = Path(main).read_text(encoding="utf-8", errors="replace").splitlines()
    for f in funcs:
        if "::" in f["qual"] or not f["line"]:
            continue
        lo = max(0, f["line"] - 3)
        snippet = " ".join(text[lo:f["line"]])
        m = re.search(r"([A-Za-z_]\w*)\s*::\s*~?" + re.escape(f["name"]) + r"\s*\(", snippet)
        if m:
            f["qual"] = m.group(1) + "::" + f["name"]
    return funcs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--target", default="GIHE78-rev0")
    ap.add_argument("--unit", action="append", help="limit to these units (e.g. flat/xMath.cpp)")
    ap.add_argument("--tsv", help="write one row per compared function")
    args = ap.parse_args()

    dwarf = parse_dwarf()
    cc = json.loads((ROOT / "build" / args.target / "compile_commands.json").read_text())
    rows = []
    totals = defaultdict(int)
    failed = []
    for entry in cc:
        rel = Path(entry["file"]).resolve().relative_to(ROOT / "src").as_posix()
        if not (rel.startswith("flat/") or rel.startswith("gc/")):
            continue
        if args.unit and rel not in args.unit:
            continue
        base = rel.split("/")[-1].lower()
        if base not in dwarf:
            totals["units_without_dwarf"] += 1
            continue
        totals["units"] += 1
        try:
            funcs = source_functions(entry)
        except Exception as e:  # noqa: BLE001
            failed.append((rel, str(e)))
            continue
        byqual = defaultdict(list)
        for d in dwarf[base]:
            byqual[d["qual"]].append(d)
        for f in funcs:
            cands = byqual.get(f["qual"], [])
            if len(cands) > 1:
                pc = [d for d in cands if header_param_count(d["header"]) == f["params"]]
                cands = pc if len(pc) == 1 else cands
            if not cands:
                totals["source_only"] += 1
                rows.append((rel, f["qual"], "no_dwarf_function", "", "", ",".join(sorted(set(f["locals"])))))
                continue
            if len(cands) > 1:
                totals["ambiguous"] += 1
                rows.append((rel, f["qual"], "ambiguous", "", "", ""))
                continue
            d = cands[0]
            ours = sorted(set(f["locals"]))
            theirs = sorted(set(d["locals"]))
            missing = [n for n in theirs if n not in ours]
            extra = [n for n in ours if n not in theirs]
            state = "same" if not missing and not extra else "differs"
            totals["compared"] += 1
            totals[state] += 1
            totals["missing_names"] += len(missing)
            totals["extra_names"] += len(extra)
            rows.append((rel, f["qual"], state, d["mangled"], ",".join(missing), ",".join(extra)))

    if args.tsv:
        with open(args.tsv, "w") as fh:
            fh.write("unit\tfunction\tstate\tdwarf_mangled\tdwarf_only\tsource_only\n")
            for r in rows:
                fh.write("\t".join(r) + "\n")
    for k in ("units", "units_without_dwarf", "compared", "same", "differs", "ambiguous",
              "source_only", "missing_names", "extra_names"):
        print(f"{k}: {totals[k]}")
    for rel, err in failed:
        print(f"FAILED {rel}: {err}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
