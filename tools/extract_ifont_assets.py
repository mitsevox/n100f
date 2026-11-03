#!/usr/bin/env python3
"""Generate private iFont array initializers from the verified rev1 original.

The four texture dictionaries and three spacing tables are original assets.
Only this extractor belongs in Git; its generated header stays under build/.
"""
import argparse
import hashlib
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
TARGET = "GIHE78-rev1"
BASE = 0x801AA4A0
RECORDS = (
    ("font1_tex", 0x00000, 0x80D8),
    ("font1_spacing", 0x080D8, 0x200),
    ("font0_tex", 0x082D8, 0x80D8),
    ("font0_spacing", 0x103B0, 0x200),
    ("debug_tex", 0x105B0, 0x8D8),
    ("debug_spacing", 0x10E88, 0x200),
    ("buttons_tex", 0x11088, 0x8B8),
)


def read_range(data, address, size):
    if len(data) < 0xE4:
        raise ValueError("truncated DOL header")
    offsets = struct.unpack_from(">18I", data, 0)
    addresses = struct.unpack_from(">18I", data, 0x48)
    sizes = struct.unpack_from(">18I", data, 0x90)
    for offset, base, length in zip(offsets, addresses, sizes):
        if base <= address and address + size <= base + length:
            start = offset + address - base
            result = data[start:start + size]
            if len(result) != size:
                raise ValueError("truncated font payload")
            return result
    raise ValueError("font payload is outside initialized DOL sections")


def generate(data):
    lines = ["// Generated original assets. Keep this file out of public Git."]
    for name, offset, size in RECORDS:
        payload = read_range(data, BASE + offset, size)
        if name.endswith("_tex"):
            kind, length, version = struct.unpack_from("<III", payload)
            if (kind, length, version) != (0x16, size - 12, 0x0C02FFFF):
                raise ValueError("unexpected texture dictionary header: " + name)
        if name.endswith("_spacing"):
            lines.append("static iFontSpacing %s[256] = {" % name)
            for start in range(0, size, 2):
                lines.append("    { 0x%02X, 0x%02X }," % tuple(payload[start:start + 2]))
        else:
            lines.append("static U8 %s[0x%X] = {" % (name, size))
            for start in range(0, size, 16):
                lines.append("    " + ", ".join("0x%02X" % byte for byte in payload[start:start + 16]) + ",")
        lines.append("};")
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", choices=[TARGET], required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    original = ROOT / "orig" / args.target / "sys/main.dol"
    expected = (ROOT / "config" / args.target / "build.sha1").read_text().split()[0]
    data = original.read_bytes()
    if hashlib.sha1(data).hexdigest() != expected:
        raise SystemExit("original DOL does not match config/%s/build.sha1" % args.target)
    try:
        text = generate(data)
    except ValueError as error:
        raise SystemExit(str(error)) from error
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if not args.output.exists() or args.output.read_text() != text:
        args.output.write_text(text)


if __name__ == "__main__":
    main()
