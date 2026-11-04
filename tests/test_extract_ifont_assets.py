import struct
import unittest

from tools.extract_ifont_assets import BASE, RECORDS, generate, read_range


def synthetic_dol():
    size = RECORDS[-1][1] + RECORDS[-1][2]
    data = bytearray(0x100 + size)
    struct.pack_into(">I", data, 0x1C, 0x100)  # First data section.
    struct.pack_into(">I", data, 0x64, BASE)
    struct.pack_into(">I", data, 0xAC, size)
    for index, (name, offset, length) in enumerate(RECORDS):
        data[0x100 + offset:0x100 + offset + length] = bytes([index + 1]) * length
        if name.endswith("_tex"):
            struct.pack_into("<III", data, 0x100 + offset, 0x16, length - 12, 0x0C02FFFF)
    return data


class FontAssetTests(unittest.TestCase):
    def test_maps_virtual_addresses_to_file_offsets(self):
        data = synthetic_dol()
        self.assertEqual(read_range(data, BASE + 0x80D8, 4), b"\x02" * 4)
        text = generate(data)
        declarations = [line for line in text.splitlines() if line.startswith("static ")]
        self.assertEqual(declarations, [
            "static iFontSpacing %s[256] = {" % name if name.endswith("_spacing")
            else "static U8 %s[0x%X] = {" % (name, size)
            for name, _, size in RECORDS
        ])
        self.assertIn("{ 0x02, 0x02 }", text)

    def test_rejects_wrong_chunk_size(self):
        data = synthetic_dol()
        struct.pack_into("<I", data, 0x104, 0)
        with self.assertRaisesRegex(ValueError, "texture dictionary"):
            generate(data)

    def test_rejects_missing_or_truncated_ranges(self):
        data = synthetic_dol()
        with self.assertRaisesRegex(ValueError, "outside"):
            read_range(data, BASE - 4, 4)
        with self.assertRaisesRegex(ValueError, "truncated font"):
            generate(data[:-1])
        with self.assertRaisesRegex(ValueError, "truncated DOL"):
            generate(b"")
