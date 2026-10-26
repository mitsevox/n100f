"""tools/splits/recover.py reads unit extents from splits.txt, whose unit headers may carry
attributes such as `comment:8`. A header the parser skips would fold that unit's sections
into the unit before it, so every committed unit must come back.

    python3 -m unittest tests/test_splits_headers.py
"""

import re
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import recover  # noqa: E402


class UnitExtents(unittest.TestCase):
    def test_header_attributes(self):
        lines = [
            "flat/xMorph.cpp:\n",
            "\t.text       start:0x80005000 end:0x80005100\n",
            "dolphin/ai/ai.c: comment:8\n",
            "\t.text       start:0x800F439C end:0x800F4C5C\n",
        ]
        self.assertEqual(recover.unit_extents(lines), {
            ("flat/xMorph", ".text"): (0x80005000, 0x80005100),
            ("dolphin/ai/ai", ".text"): (0x800F439C, 0x800F4C5C),
        })

    def test_committed_splits(self):
        lines = (ROOT / "config" / "GIHE78" / "splits.txt").read_text().splitlines(keepends=True)
        expected = {}
        unit = None
        for line in lines:
            if line and not line[0].isspace() and ":" in line and not line.startswith("Sections:"):
                unit = re.sub(r"\.\w+$", "", line.split(":", 1)[0])
            m = re.match(r"\s+(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)", line)
            if m and unit:
                expected[(unit, m[1])] = (int(m[2], 16), int(m[3], 16))
        self.assertEqual(recover.unit_extents(lines), expected)
        self.assertTrue(any(u.startswith("dolphin/") for u, _ in expected))


if __name__ == "__main__":
    unittest.main()
