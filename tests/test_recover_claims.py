"""tools/splits/recover.py writes a unit its size rule leaves weak when a `unit` row in
evidence.tsv claims that unit's .text extent. Only rows for the tool's own path prefix
with both addresses count; the unit key drops the prefix, as split_units.tsv names it.

    python3 -m unittest tests/test_recover_claims.py
"""

import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import recover  # noqa: E402

ROWS = [
    ["unit", "0x80040FCC", "0x80044780", "flat/xTRC.cpp", "inferred", "provisional", "provisional", "..."],
    ["unit", "0x800D7E00", "0x800D84A0", "gc/iAnim.cpp", "inferred", "exact", "exact", "..."],
    ["unit", "0x80001000", "-", "flat/open.cpp", "inferred", "open", "-", "..."],
    ["unit-anchor", "0x80044478", "-", "flat/xTRCPad.cpp", "related", "-", "-", "..."],
    ["function", "0x80044714", "0x80044780", "flat/xTRCInit.cpp", "target", "exact", "exact", "..."],
]


class ClaimedUnits(unittest.TestCase):
    def test_prefix_rows_only(self):
        self.assertEqual(recover.claimed_units(ROWS, "flat/"), {"xTRC.cpp": (0x80040FCC, 0x80044780)})

    def test_other_prefix(self):
        self.assertEqual(recover.claimed_units(ROWS, "gc/"), {"iAnim.cpp": (0x800D7E00, 0x800D84A0)})


if __name__ == "__main__":
    unittest.main()
