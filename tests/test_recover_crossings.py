"""tools/splits/recover.py counts loads of items inside a written data range by
functions that no written unit holds. A function in a unit left unsplit (weak) is
outside every written unit just like an unassigned one, so its loads count; loads
by a function of a written unit do not.

    python3 -m unittest tests/test_recover_crossings.py
"""

import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import recover  # noqa: E402

# items: (addr, size, section)
DATA = [
    (0x80200000, 4, ".data"),   # 0: in A's range
    (0x80200004, 4, ".data"),   # 1: in A's range
    (0x80200008, 4, ".data"),   # 2: past A's range
    (0x80300000, 4, ".sdata"),  # 3: in A's .sdata range
]
RANGES = {("A", ".data"): (0x80200000, 0x80200008), ("A", ".sdata"): (0x80300000, 0x80300004)}
TSTART = {"A": 10}
# functions 0-4 lie before A, 10-12 are A's own, 20 lies after A.
HELD = {10: "A", 11: "A", 12: "A", 30: "C"}
WEAK = {3, 20}  # functions of a unit left unsplit


def holder(f):
    return None if f in WEAK else HELD.get(f)


class OutsideLoads(unittest.TestCase):
    def test_counts(self):
        users = {
            0: {10, 2},       # A's own load does not count; unassigned 2 does (before A)
            1: {3, 20, 30},   # weak 3 (before) and weak 20 (after) count; written unit C does not
            2: {4},           # outside every written range
            3: {20},          # weak 20 in another section's range
        }
        got = recover.outside_loads(DATA, RANGES, users, holder, TSTART)
        self.assertEqual(dict(got), {
            ("A", ".data", True): 2,
            ("A", ".data", False): 1,
            ("A", ".sdata", False): 1,
        })

    def test_held_by_any_unit_hides_weak_loads(self):
        # The old rule skipped every function some unit held, weak or not.
        users = {1: {3, 20}}
        self.assertEqual(sum(recover.outside_loads(DATA, RANGES, users, holder, TSTART).values()), 2)
        held_any = {**HELD, 3: "W", 20: "W"}.get
        self.assertEqual(sum(recover.outside_loads(DATA, RANGES, users, held_any, TSTART).values()), 0)


if __name__ == "__main__":
    unittest.main()
