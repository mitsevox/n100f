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


# A zVarEntryTable shape: the table (0) is loaded only by zConditional, its strings (1, 2) lie
# inside zVar's .rodata range, so table and strings are zVar's.
GDATA = [
    (0x801C8CB0, 0x6C0, ".data", "zVarEntryTable", False, None),
    (0x801A7020, 0xB, ".rodata", "lbl_801A7020", False, "string"),
    (0x801A702C, 0xD, ".rodata", "lbl_801A702C", False, "string"),
]
ZVAR_RODATA = (0x801A7010, 0x801A7B10)


def in_zvar(d):
    lo, hi = ZVAR_RODATA
    return "zVar" if GDATA[d][2] == ".rodata" and lo <= GDATA[d][0] < hi else None


class AssignGroups(unittest.TestCase):
    units = {"zConditional": 0, "zUI": 1, "zVar": 2}

    def test_table_follows_its_strings(self):
        out, conflicts = recover.assign_groups([[0, 1, 2]], in_zvar, {0: {"zConditional"}}, {0}, {}, self.units)
        self.assertEqual((out, conflicts), ({0: "zVar", 1: "zVar", 2: "zVar"}, 0))

    def test_sole_loader_when_no_range_holds(self):
        out, conflicts = recover.assign_groups([[0, 1, 2]], lambda d: None, {0: {"zConditional"}}, {0}, {}, self.units)
        self.assertEqual((out, conflicts), ({1: "zConditional", 2: "zConditional"}, 0))

    def test_literal_loaded_by_another_unit_is_a_conflict(self):
        users = {0: {"zConditional"}, 1: {"zUI"}}  # an lbl_ that only zUI loads: an unnamed global
        out, conflicts = recover.assign_groups([[0, 1, 2]], in_zvar, users, {0}, {}, self.units)
        self.assertEqual((out, conflicts), ({}, 1))

    def test_two_ranges_is_a_conflict(self):
        out, conflicts = recover.assign_groups([[0, 1, 2]], lambda d: ["zUI", "zVar", None][d], {}, {0}, {}, self.units)
        self.assertEqual((out, conflicts), ({}, 1))

    def test_claimed_items_keep_their_claim(self):
        out, _ = recover.assign_groups([[0, 1, 2]], in_zvar, {}, {0}, {0: "zUI"}, self.units)
        self.assertEqual(out, {1: "zVar", 2: "zVar"})


CDATA = [
    (0x80227748, 0x1270, ".bss", "gSnd", False, None),     # 0: many loaders
    (0x802289B8, 0x30, ".bss", "lbl_802289B8", False, None),  # 1: loaded only by xTRC
    (0x80300000, 0x10, ".bss", "lbl_80300000", False, None),  # 2: no loader
]
EMITTED = {"xSnd.cpp", "xTRC.cpp", "zMain.cpp"}


class CheckDataClaims(unittest.TestCase):
    users = {0: {"xSnd.cpp", "zMain.cpp", None}, 1: {"xTRC.cpp"}}

    def check(self, claims, inferred=None):
        return recover.check_data_claims(claims, CDATA, self.users, inferred or {}, EMITTED)

    def test_shared_global_is_accepted(self):
        accepted, refused = self.check({("xSnd.cpp", ".bss"): (0x80227748, 0x802289B8)})
        self.assertEqual((accepted, refused), ({0: "xSnd.cpp"}, []))

    def test_item_only_another_unit_loads_is_refused(self):
        accepted, refused = self.check({("xSnd.cpp", ".bss"): (0x80227748, 0x802289E8)})
        self.assertEqual(accepted, {})
        self.assertIn("loaded only by xTRC.cpp", refused[0])

    def test_overlap_with_inferred_range_is_refused(self):
        accepted, refused = self.check({("zMain.cpp", ".bss"): (0x80300000, 0x80300010)},
                                       {("xTRC.cpp", ".bss"): (0x802289B8, 0x80300008)})
        self.assertEqual(accepted, {})
        self.assertIn("overlaps xTRC.cpp .bss", refused[0])

    def test_overlap_with_another_claim_is_refused(self):
        accepted, refused = self.check({("xSnd.cpp", ".bss"): (0x80227748, 0x802289B8),
                                        ("zMain.cpp", ".bss"): (0x80227750, 0x80227760)})
        self.assertEqual(accepted, {0: "xSnd.cpp"})
        self.assertIn("overlaps the claim of xSnd.cpp", refused[0])

    def test_unwritten_unit_is_refused(self):
        accepted, refused = self.check({("zMenu.cpp", ".bss"): (0x80300000, 0x80300010)})
        self.assertEqual(accepted, {})
        self.assertIn("not written", refused[0])


if __name__ == "__main__":
    unittest.main()
