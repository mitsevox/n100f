"""tools/splits/recover.py places data that no function loads.

literal_groups ties a table to the literals it points to, and to statics that only
data points to, so one unit gets them all; a pointer to a global that functions load
ties nothing.  claimed_data reads the `data` rows of evidence.tsv that claim a unit's
section range (a shared global's owner, which the run cannot tell).

    python3 -m unittest tests/test_recover_data_groups.py
"""

import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import recover  # noqa: E402

# load_symbols() rows: (addr, size, section, name, is_function, kind)
DATA = [
    (0x80200000, 8, ".data", "nameTable", False, None),        # 0: loaded by a function
    (0x80200008, 4, ".data", "sharedGlobal", False, None),     # 1: loaded by functions
    (0x80200010, 8, ".data", "innerTable", False, None),       # 2: only data points to it
    (0x80300000, 6, ".rodata", "lbl_80300000", False, "string"),  # 3: a literal
    (0x80300008, 6, ".rodata", "lbl_80300008", False, "string"),  # 4: a literal
    (0x80400000, 8, ".data", "otherTable", False, None),       # 5: loaded by a function
]


class LiteralGroups(unittest.TestCase):
    def test_table_chain_and_global(self):
        # nameTable -> innerTable -> literal 3; nameTable -> literal 4; otherTable -> sharedGlobal
        refs = {2: {0}, 3: {2}, 4: {0}, 1: {5}}
        users = {0: {100}, 1: {101, 102}, 5: {103}}
        groups = recover.literal_groups(DATA, refs, users)
        self.assertEqual(groups, [[0, 2, 3, 4]])

    def test_named_global_pointer_ties_nothing(self):
        groups = recover.literal_groups(DATA, {1: {0}}, {0: {100}, 1: {101}})
        self.assertEqual(groups, [])


def row(subject, start="0x80227748", end="0x802289B8", a="exact", b="exact"):
    return ["data", start, end, subject, "target", a, b, "evidence"]


class ClaimedData(unittest.TestCase):
    def test_unit_section_rows_claim(self):
        rows = [row("flat/xSnd.cpp .bss"), row("flat/zMain.cpp .bss", "0x80234AE0", "0x80237DE0", "exact", "provisional")]
        self.assertEqual(recover.claimed_data(rows, "flat/"), {
            ("xSnd.cpp", ".bss"): (0x80227748, 0x802289B8),
            ("zMain.cpp", ".bss"): (0x80234AE0, 0x80237DE0),
        })

    def test_other_rows_claim_nothing(self):
        rows = [
            row("gSnd"),                                   # a symbol, not a unit section
            row("flat/xSnd.cpp .bss", a="open"),           # an open edge
            row("flat/xSnd.cpp .text"),                    # not a data section
            row("gc/iMix.c .bss"),                         # another prefix
            row("flat/xSnd.cpp .bss", start="-"),          # no address
            row("flat/zScene.cpp lbl_801C82A0 initializer"),
        ]
        self.assertEqual(recover.claimed_data(rows, "flat/"), {})


if __name__ == "__main__":
    unittest.main()
