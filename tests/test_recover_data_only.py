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

    def test_gap_not_at_host_end(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1008), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("neither a tail", refused[0])

    def test_gap_leaves_items(self):
        data = [(0x1000, 0x10, ".data", "sLocal"), (0x1010, 0x04, ".data", "gTable"),
                (0x1014, 0x04, ".data", "sLeft"), (0x1018, 0x08, ".data", "sNext")]
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1014))],
                                                  self.ONLY, data, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("sLeft", refused[0])

    def test_second_claim_on_one_host(self):
        only = {"zACol.cpp": {"gTable"}, "zADat.cpp": {"gTable"}}
        data = [(0x1000, 0x10, ".data", "sLocal"), (0x1010, 0x04, ".data", "gTable"),
                (0x1014, 0x04, ".data", "gTable"), (0x1018, 0x08, ".data", "sNext")]
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018)),
                                                   (("zADat.cpp", ".data"), (0x1014, 0x1018))],
                                                  only, data, final, self.ORDER)
        self.assertEqual(hosted, {"zA.cpp": [("zACol.cpp", ".data", 0x1010, 0x1018)]})
        self.assertIn("already hosts", refused[0])

    def test_claim_ends_inside_an_item(self):
        data = [(0x1000, 0x10, ".data", "sLocal"), (0x1010, 0x08, ".data", "gTable"),
                (0x1018, 0x08, ".data", "sNext")]
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1014))],
                                                  self.ONLY, data, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("crosses its edge", refused[0])

    def test_tail_then_gap_on_one_host(self):
        only = {"zACol.cpp": {"gTable"}, "zADat.cpp": {"gTable"}}
        data = [(0x1000, 0x08, ".data", "sLocal"), (0x1008, 0x08, ".data", "gTable"),
                (0x1010, 0x08, ".data", "gTable"), (0x1018, 0x08, ".data", "sNext")]
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1018, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1008, 0x1010)),
                                                   (("zADat.cpp", ".data"), (0x1010, 0x1018))],
                                                  only, data, final, self.ORDER)
        self.assertEqual(hosted, {"zA.cpp": [("zACol.cpp", ".data", 0x1008, 0x1010)]})
        self.assertIn("already hosts", refused[0])

    def test_gap_with_no_later_range(self):
        data = [(0x1000, 0x10, ".data", "sLocal"), (0x1010, 0x04, ".data", "gTable")]
        final = {("zA.cpp", ".data"): (0x1000, 0x1010)}
        gaps = set()
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, data, final, self.ORDER, gaps)
        self.assertEqual(refused, [])
        self.assertEqual(gaps, {("zACol.cpp", ".data")})

    def test_gap_reached_by_a_range(self):
        final = {("zA.cpp", ".data"): (0x1000, 0x1010), ("zB.cpp", ".data"): (0x1014, 0x1020)}
        hosted, refused = recover.carve_data_only([(("zACol.cpp", ".data"), (0x1010, 0x1018))],
                                                  self.ONLY, DATA, final, self.ORDER)
        self.assertEqual(hosted, {})
        self.assertIn("neither a tail", refused[0])


class DataEdgeRows(unittest.TestCase):
    HOSTED = [("zACol.cpp", ".data", 0x1010, 0x1018)]

    def test_gap_claim_fills_the_gap(self):
        rows = recover.data_edge_rows(".data", "zA.cpp", "zB.cpp", (0x1000, 0x1010), (0x1018, 0x1020),
                                      "open: 2 items between aligned candidates; 3 in-range loads by "
                                      "functions outside the written units", 5, self.HOSTED,
                                      {("zACol.cpp", ".data")})
        self.assertEqual(rows, [
            (".data", "zA.cpp", "zACol.cpp", "0x00001010", "0x00001010",
             "data-only unit claimed by a data row in evidence.tsv", 0),
            (".data", "zACol.cpp", "zB.cpp", "0x00001018", "0x00001018",
             "data-only unit claim fills the gap; 3 in-range loads by functions outside the written units", 3)])

    def test_tail_claim_keeps_the_class(self):
        rows = recover.data_edge_rows(".data", "zA.cpp", "zB.cpp", (0x1000, 0x1010), (0x1018, 0x1020),
                                      "cut at the fewest crossing references", 1, self.HOSTED, set())
        self.assertEqual(rows[1], (".data", "zACol.cpp", "zB.cpp", "0x00001018", "0x00001018",
                                   "cut at the fewest crossing references", 1))

    def test_no_claim(self):
        rows = recover.data_edge_rows(".data", "zA.cpp", "zB.cpp", (0x1000, 0x1010), None,
                                      "open", 0, [], set())
        self.assertEqual(rows, [(".data", "zA.cpp", "zB.cpp", "0x00001010", "-",
                                 "open; a side left unsplit", 0)])


if __name__ == "__main__":
    unittest.main()
