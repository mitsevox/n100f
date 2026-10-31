"""tools/splits/recover.py writes a unit its size rule leaves weak when a `unit` row in
evidence.tsv claims that unit's .text extent. Only rows for the tool's own path prefix
with both addresses and no `open` edge count; the unit key drops the prefix, as split_units.tsv names it.

    python3 -m unittest tests/test_recover_claims.py
"""

import io
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
    ["unit", "0x80002000", "0x80002100", "flat/edge.cpp", "inferred", "provisional", "open", "..."],
    ["unit-anchor", "0x80044478", "-", "flat/xTRCPad.cpp", "related", "-", "-", "..."],
    ["function", "0x80044714", "0x80044780", "flat/xTRCInit.cpp", "target", "exact", "exact", "..."],
]


class ClaimedUnits(unittest.TestCase):
    def test_prefix_rows_only(self):
        self.assertEqual(recover.claimed_units(ROWS, "flat/"), {"xTRC.cpp": (0x80040FCC, 0x80044780)})

    def test_other_prefix(self):
        self.assertEqual(recover.claimed_units(ROWS, "gc/"), {"iAnim.cpp": (0x800D7E00, 0x800D84A0)})


def layout():
    """Five 0x10-byte functions: A (written) holds 0-1, W (weak) holds 2-3, B (written) holds 4."""
    funcs = [(0x100 + 0x10 * k, 0x10) for k in range(5)]
    game = list(range(5))
    unit_of = {0: "A", 1: "A", 2: "W", 3: "W", 4: "B"}
    tstart = {"A": 0, "W": 2, "B": 4}
    tend = {"A": 1, "W": 3, "B": 4}
    order = ["A", "W", "B"]
    return funcs, game, unit_of, tstart, tend, order


class ApplyClaims(unittest.TestCase):
    def run_claims(self, claims, state):
        funcs, game, unit_of, tstart, tend, order = state
        tclass, tconf = {}, {}
        log = io.StringIO()
        applied = recover.apply_claims(claims, funcs, game, unit_of, tstart, tend, order,
                                       lambda w: w == "W", tclass, tconf, log=log)
        return applied, log.getvalue(), tclass

    def test_claim_takes_weak_unit(self):
        state = layout()
        applied, log, tclass = self.run_claims({"C": (0x120, 0x140)}, state)
        _, _, unit_of, tstart, tend, order = state
        self.assertEqual(applied, ["C"])
        self.assertEqual(log, "")
        self.assertEqual(order, ["A", "C", "B"])
        self.assertEqual((tstart["C"], tend["C"]), (2, 3))
        self.assertEqual(unit_of[2], "C")
        self.assertIn(("A", "C"), tclass)
        self.assertIn(("C", "B"), tclass)

    def test_claim_shrinks_weak_unit(self):
        state = layout()
        applied, _, _ = self.run_claims({"C": (0x130, 0x140)}, state)
        _, _, _, tstart, tend, order = state
        self.assertEqual(applied, ["C"])
        self.assertEqual((tstart["W"], tend["W"]), (2, 2))
        self.assertEqual(order, ["A", "W", "C", "B"])

    def test_claim_on_written_unit_skipped(self):
        state = layout()
        applied, log, _ = self.run_claims({"C": (0x110, 0x130)}, state)
        self.assertEqual(applied, [])
        self.assertIn("written unit", log)
        self.assertEqual(state[2][1], "A")

    def test_partial_function_skipped(self):
        state = layout()
        applied, log, _ = self.run_claims({"C": (0x120, 0x138)}, state)
        self.assertEqual(applied, [])
        self.assertIn("whole functions", log)

    def test_claim_must_contain_current_extent(self):
        state = layout()
        funcs, game, unit_of, tstart, tend, order = state
        unit_of[3] = "C"
        tend["W"] = 2
        tstart["C"] = tend["C"] = 3
        order.insert(2, "C")
        applied, log, _ = self.run_claims({"C": (0x120, 0x130)}, state)
        self.assertEqual(applied, [])
        self.assertIn("current extent", log)
        self.assertEqual((tstart["C"], tend["C"]), (3, 3))

    def test_overlapping_claims_second_skipped(self):
        state = layout()
        applied, log, _ = self.run_claims({"C": (0x120, 0x140), "D": (0x130, 0x140)}, state)
        self.assertEqual(applied, ["C"])
        self.assertIn("claim D", log)


if __name__ == "__main__":
    unittest.main()
