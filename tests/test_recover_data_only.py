"""tools/splits/recover.py gives a PS2 compile unit with globals and no functions (a data-only
unit, such as zEntVillainCol.cpp) the range a `data` row claims for it, carved off the end of
the range of the unit linked right before it, or placed in the gap after that range when no
unit's range reaches it.

    python3 -m unittest tests/test_recover_data_only.py
"""

import os
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import recover  # noqa: E402

VARIABLES = (
    "Address\tBinding\tName\tMangled\tType\tSource_File\n"
    "0x00000100\tGLOBAL\tgTable\t\tunsigned char [4]\tE:\\SD2\\Game\\flat\\zA.cpp\n"
    "0x00000100\tGLOBAL\tgTable\t\tunsigned char [4]\tE:\\SD2\\Game\\flat\\zACol.cpp\n"
    "0x00000200\tLOCAL\tsHidden\t\tint\tE:\\SD2\\Game\\flat\\zACol.cpp\n"
    "0x00000300\tGLOBAL\tgOther\t\tint\tC:\\usr\\lib\\other.c\n"
)

# (address, size, section, name): zA.cpp's .data holds a local table, then gTable.
DATA = [
    (0x1000, 0x10, ".data", "sLocal"),
    (0x1010, 0x04, ".data", "gTable"),
    (0x1018, 0x08, ".data", "sNext"),
]


class DataOnlyUnits(unittest.TestCase):
    def setUp(self):
        fd, self.path = tempfile.mkstemp(suffix=".tsv")
        with os.fdopen(fd, "w", encoding="utf-8") as f:
            f.write(VARIABLES)

    def tearDown(self):
        os.unlink(self.path)

    def test_units_without_functions(self):
        self.assertEqual(recover.data_only_units(self.path, {"zA.cpp"}), {"zACol.cpp": {"gTable"}})


class CarveDataOnly(unittest.TestCase):
    ONLY = {"zACol.cpp": {"gTable"}}
    ORDER = ["zA.cpp", "zB.cpp"]

    def test_tail_is_carved(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1018)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(refused, [])
        self.assertEqual(hosted, {"zA.cpp": [("zACol.cpp", ".data", 0x1010, 0x1018)]})
        self.assertEqual(final[("zA.cpp", ".data")], (0x1000, 0x1010))

    def test_not_a_tail(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("neither a tail", refused[0])
        self.assertEqual(final[("zA.cpp", ".data")], (0x1000, 0x1020))

    def test_foreign_item(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1020))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("sNext", refused[0])

    def test_whole_range(self):
        final = {("zA.cpp", ".data"): (0x1010, 0x1018)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("neither a tail", refused[0])

    def test_gap_is_claimed(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(refused, [])
        self.assertEqual(hosted, {"zA.cpp": [("zACol.cpp", ".data", 0x1010, 0x1018)]})
        self.assertEqual(final[("zA.cpp", ".data")], (0x1000, 0x1010))

    def test_gap_claim_is_recorded(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        gaps = set()
        recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                self.ONLY, DATA, final, self.ORDER, gaps)
        self.assertEqual(gaps, {("zACol.cpp", ".data")})

    def test_gap_past_a_later_range(self):
        # zB.cpp's range lies between zA.cpp's and the claim, so the claim breaks link order.
        data = DATA + [(0x1030, 0x04, ".data", "gTable")]
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1030, 0x1038))],
                                                  self.ONLY, data, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("neither a tail", refused[0])

    def test_gap_reached_by_a_range(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1014, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("neither a tail", refused[0])


if __name__ == "__main__":
    unittest.main()
