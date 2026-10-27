"""tools/splits/recover.py resolves relocation targets through a name -> addresses index
built from symbols.txt. A name listed twice at one address (a function and a label) must
resolve to that address, while a name at two addresses stays ambiguous.

    python3 -m unittest tests/test_recover_names.py
"""

import os
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools" / "splits"))
import recover  # noqa: E402

SYMBOLS = """\
gTRKInterruptVectorTable = .init:0x80003154; // type:function size:0x1F34 scope:global
gTRKInterruptVectorTable = .init:0x80003154; // type:label scope:global
sLocal = .sbss:0x8025ED00; // type:object size:0x4 scope:local
sLocal = .sbss:0x8025EE00; // type:object size:0x4 scope:local
xAngleClamp__Ff = .text:0x8002C8C4; // type:function size:0x3C
"""


class NameIndex(unittest.TestCase):
    def setUp(self):
        fd, self.path = tempfile.mkstemp(suffix=".txt")
        with os.fdopen(fd, "w") as f:
            f.write(SYMBOLS)

    def tearDown(self):
        os.unlink(self.path)

    def test_index(self):
        named = recover.name_index(recover.load_symbols(self.path))
        self.assertEqual(named["gTRKInterruptVectorTable"], [0x80003154])
        self.assertEqual(named["sLocal"], [0x8025ED00, 0x8025EE00])
        self.assertEqual(named["xAngleClamp__Ff"], [0x8002C8C4])


if __name__ == "__main__":
    unittest.main()
