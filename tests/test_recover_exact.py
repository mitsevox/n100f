"""tools/splits/recover.py pins a function to the PS2 function of the same name only when
evidence.tsv gives it an exact body whose name came with the body. A `function` row whose
evidence carries recover.OWN_SOURCE_MARKER holds a body written for this project, so it
must not become a name anchor.

    python3 -m unittest tests/test_recover_exact.py
"""

import csv
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import recover  # noqa: E402

EVIDENCE = ROOT / "config" / "GIHE78-rev0" / "evidence.tsv"


def row(kind, start, subject, origin, sb, eb, text):
    return [kind, start, "-", subject, origin, sb, eb, text]


class ExactBodies(unittest.TestCase):
    def test_selection(self):
        rows = [
            row("function", "0x80044780", "xUtil_yesno__Ff", "target", "exact", "exact",
                "The 27-word body tools/compiler_probe.py compiles from bfbb is found once."),
            row("function", "0x800447EC", "xUtil_yesno__Fv", "target", "exact", "exact",
                "objdiff scores it 100%. The body is " + recover.OWN_SOURCE_MARKER + "."),
            row("function", "0x80044828", "a", "target", "provisional", "provisional", "dtk."),
            row("function", "0x80044830", "b", "inferred", "exact", "exact", "inferred."),
            row("name", "0x80044780", "xUtil_yesno__Ff", "related", "-", "-", "bfbb."),
        ]
        self.assertEqual(recover.exact_bodies(rows), {0x80044780: "xUtil_yesno__Ff"})

    def test_repository_rows(self):
        with open(EVIDENCE, encoding="utf-8") as f:
            rows = list(csv.reader(f, delimiter="\t"))
        exact = recover.exact_bodies(rows)
        marked = {int(r[1], 16) for r in rows
                  if r[0] == "function" and recover.OWN_SOURCE_MARKER in r[7]}
        # The two positional xutil names of #23 stay out of the anchors.
        self.assertIn(0x800447EC, marked)
        self.assertIn(0x80044BBC, marked)
        self.assertFalse(marked & exact.keys())
        # A marked row never also claims a single-hit related-build body, which would
        # make its name a body match after all.
        for r in rows:
            if r[0] == "function" and recover.OWN_SOURCE_MARKER in r[7]:
                self.assertNotIn("is found once in the DOL", r[7], r[3])


if __name__ == "__main__":
    unittest.main()
