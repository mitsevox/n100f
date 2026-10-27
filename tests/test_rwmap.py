"""tools/splits/rwmap.py places RenderWare units by local alignment of function sizes.
A placement must find a unit's run among unrelated functions, and the committed
RenderWare units must keep the library order the tool enforces.

    python3 -m unittest tests/test_rwmap.py
"""

import csv
import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import rwmap  # noqa: E402


class Placement(unittest.TestCase):
    def test_finds_run(self):
        sizes = [0x40, 0x1C4, 0x3C, 0x18, 0x34, 0x148, 0x170, 0x18, 0x90, 0x2C]
        found = rwmap.local_placements(sizes, [0x3C, 0x18, 0x34, 0x148, 0x170])
        score, lo, hi, pairs = found[0]
        self.assertEqual((lo, hi), (2, 6))
        self.assertEqual(pairs, [(2, 0), (3, 1), (4, 2), (5, 3), (6, 4)])

    def test_skips_stripped_function(self):
        # The second upstream function was stripped from the target.
        sizes = [0x3C, 0x34, 0x148, 0x170]
        _, lo, hi, pairs = rwmap.local_placements(sizes, [0x3C, 0x18, 0x34, 0x148, 0x170])[0]
        self.assertEqual([j for _, j in pairs], [0, 2, 3, 4])

    def test_nothing_below_threshold(self):
        self.assertEqual(rwmap.local_placements([0x10, 0x20], [0x400, 0x500]), [])


class GapPairs(unittest.TestCase):
    def test_pairs_in_order(self):
        # Equal and near sizes pair in order; an unrelated size stays unpaired.
        self.assertEqual(rwmap.gap_pairs([0x14, 0x3C, 0x2B4, 0x80], [0x3C, 0x2B0, 0x80]), [(1, 0), (2, 1), (3, 2)])

    def test_no_crossing(self):
        self.assertEqual(len(rwmap.gap_pairs([0x40, 0x80], [0x80, 0x40])), 1)


class Committed(unittest.TestCase):
    def test_library_order(self):
        libs = [lib["lib"] for lib in json.loads((ROOT / "config/GIHE78/rwmap.json").read_text())["libraries"]]
        rows = list(csv.DictReader((ROOT / "config/GIHE78/rw_units.tsv").open(), delimiter="\t"))
        order = [libs.index(r["library"]) for r in rows]
        self.assertEqual(order, sorted(order))
        starts = [int(r["text_start"], 16) for r in rows]
        self.assertEqual(starts, sorted(starts))
        self.assertLessEqual({r["method"] for r in rows}, {"chain", "gap", "fill", "node"})
        for r in rows:
            if r["method"] == "node":
                self.assertRegex(r["unit"], r"/node\w+\.c$")


if __name__ == "__main__":
    unittest.main()
