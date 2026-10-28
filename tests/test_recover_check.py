"""tools/splits/check_recover.py compares recover.py's --splits output with the `flat/` units of
splits.txt. That output opens each unit with a blank line, so the region it replaces runs from
the blank line before the first `flat/` unit through the last line of the last one.

    python3 -m unittest tests/test_recover_check.py
"""

import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import check_recover  # noqa: E402

SPLITS = (
    "Sections:\n"
    "\t.text       type:code align:16\n"
    "\n"
    "flat/a.cpp:\n"
    "\t.text       start:0x80005000 end:0x80005100\n"
    "\n"
    "flat/b.cpp:\n"
    "\t.text       start:0x80005100 end:0x80005200\n"
    "\n"
    "gc/c.cpp:\n"
    "\t.text       start:0x80005200 end:0x80005300\n"
)


class UnitRegion(unittest.TestCase):
    def test_region_is_recover_output(self):
        lo, hi = check_recover.unit_region(SPLITS, "flat/")
        self.assertEqual(SPLITS[lo:hi],
                         "\nflat/a.cpp:\n\t.text       start:0x80005000 end:0x80005100\n"
                         "\nflat/b.cpp:\n\t.text       start:0x80005100 end:0x80005200\n")

    def test_splice_keeps_other_units(self):
        new = "\nflat/a.cpp:\n\t.text       start:0x80005000 end:0x80005200\n"
        self.assertEqual(check_recover.splice(SPLITS, "flat/", new),
                         SPLITS.replace("0x80005100\n\nflat/b.cpp:\n\t.text       start:0x80005100 end:", ""))

    def test_units_at_end(self):
        text = SPLITS.split("\ngc/")[0]
        lo, hi = check_recover.unit_region(text, "flat/")
        self.assertEqual(text[hi:], "")

    def test_not_contiguous(self):
        with self.assertRaises(ValueError):
            check_recover.unit_region(SPLITS + "\nflat/d.cpp:\n\t.text       start:0x80005300 end:0x80005400\n", "flat/")

    def test_committed_round_trip(self):
        text = (ROOT / "config" / "GIHE78-rev0" / "splits.txt").read_text()
        lo, hi = check_recover.unit_region(text, "flat/")
        region = text[lo:hi]
        self.assertTrue(region.startswith("\nflat/"))
        self.assertTrue(text[hi:].startswith("\n") and not text[hi:].startswith("\nflat/"))
        self.assertEqual(region.count("\nflat/"), sum(1 for line in text.splitlines() if line.startswith("flat/")))
        self.assertEqual(check_recover.splice(text, "flat/", region), text)


if __name__ == "__main__":
    unittest.main()
