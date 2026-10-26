"""Minimal DWARF version 1 reader for Metrowerks-built executables.

Reads the ``.debug`` section into a tree of DIEs and renders types,
functions and variables as C-like text. Handles both byte orders so the
same reader serves the little-endian PS2 builds and big-endian GameCube
material.
"""

import struct

# Tags.
TAG = {
    0x01: "array_type", 0x02: "class_type", 0x03: "entry_point",
    0x04: "enumeration_type", 0x05: "formal_parameter",
    0x06: "global_subroutine", 0x07: "global_variable", 0x0A: "label",
    0x0B: "lexical_block", 0x0C: "local_variable", 0x0D: "member",
    0x0F: "pointer_type", 0x10: "reference_type", 0x11: "compile_unit",
    0x12: "string_type", 0x13: "structure_type", 0x14: "subroutine",
    0x15: "subroutine_type", 0x16: "typedef", 0x17: "union_type",
    0x18: "unspecified_parameters", 0x19: "variant", 0x1A: "common_block",
    0x1B: "common_inclusion", 0x1C: "inheritance",
    0x1D: "inlined_subroutine", 0x1E: "module", 0x1F: "ptr_to_member_type",
    0x20: "set_type", 0x21: "subrange_type", 0x22: "with_stmt",
}

# Attributes (name << 4 | form) that the renderer uses.
AT_SIBLING = 0x0012
AT_LOCATION = 0x0023
AT_NAME = 0x0038
AT_FUND_TYPE = 0x0055
AT_MOD_FUND_TYPE = 0x0063
AT_USER_DEF_TYPE = 0x0072
AT_MOD_U_D_TYPE = 0x0083
AT_SUBSCR_DATA = 0x00A3
AT_BYTE_SIZE = 0x00B6
AT_BIT_OFFSET = 0x00C5
AT_BIT_SIZE = 0x00D6
AT_ELEMENT_LIST = 0x00F4
AT_LOW_PC = 0x0111
AT_HIGH_PC = 0x0121
AT_MEMBER = 0x0142
AT_PRODUCER = 0x0258

FUND = {
    0x0001: "char", 0x0002: "signed char", 0x0003: "unsigned char",
    0x0004: "short", 0x0005: "signed short", 0x0006: "unsigned short",
    0x0007: "int", 0x0008: "signed int", 0x0009: "unsigned int",
    0x000A: "long", 0x000B: "signed long", 0x000C: "unsigned long",
    0x000D: "void*", 0x000E: "float", 0x000F: "double",
    0x0010: "long double", 0x0011: "complex", 0x0012: "double complex",
    0x0014: "void", 0x0015: "bool", 0x0016: "long double complex",
    0x0017: "label", 0x8008: "long long", 0x8108: "signed long long",
    0x8208: "unsigned long long", 0xA510: "u_long128",
}

MOD_POINTER, MOD_REFERENCE, MOD_CONST, MOD_VOLATILE = 1, 2, 3, 4

OP_REG, OP_BASEREG, OP_ADDR, OP_CONST = 1, 2, 3, 4
OP_DEREF2, OP_DEREF4, OP_ADD = 5, 6, 7


class Die:
    __slots__ = ("off", "tag", "attrs", "children", "parent")

    def __init__(self, off, tag, attrs):
        self.off = off
        self.tag = tag
        self.attrs = attrs
        self.children = []
        self.parent = None

    def get(self, at, default=None):
        return self.attrs.get(at, default)

    @property
    def name(self):
        return self.attrs.get(AT_NAME)

    def strings(self):
        """String-form attributes other than the name (e.g. mangled names)."""
        return {k: v for k, v in self.attrs.items()
                if (k & 0xF) == 8 and k != AT_NAME and k != AT_PRODUCER}


class Dwarf1:
    def __init__(self, data, base=0, endian="<"):
        self.d = data
        self.base = base  # section offset that DIE references are relative to
        self.e = endian
        self.dies = {}
        self.roots = []
        self._parse()

    def _u16(self, b, o):
        return struct.unpack_from(self.e + "H", b, o)[0]

    def _u32(self, b, o):
        return struct.unpack_from(self.e + "I", b, o)[0]

    def _parse(self):
        d, n = self.d, len(self.d)
        entries = []
        i = 0
        while i + 4 <= n:
            ln = self._u32(d, i)
            if ln < 8:
                entries.append((i, None, None, i + max(ln, 4)))
                i += max(ln, 4)
                continue
            tag = self._u16(d, i + 4)
            attrs = {}
            j, end = i + 6, i + ln
            while j + 2 <= end:
                at = self._u16(d, j)
                j += 2
                form = at & 0xF
                if form in (1, 2, 6):
                    v = self._u32(d, j); j += 4
                elif form == 5:
                    v = self._u16(d, j); j += 2
                elif form == 7:
                    v = d[j:j + 8]; j += 8
                elif form == 3:
                    sz = self._u16(d, j); v = d[j + 2:j + 2 + sz]; j += 2 + sz
                elif form == 4:
                    sz = self._u32(d, j); v = d[j + 4:j + 4 + sz]; j += 4 + sz
                elif form == 8:
                    k = d.index(b"\0", j)
                    v = d[j:k].decode("latin1"); j = k + 1
                else:
                    break
                attrs[at] = v
            entries.append((i, tag, attrs, end))
            i = end
        # Rebuild the tree: an entry whose sibling lies past its own end owns
        # the following entries up to the null entry that closes the list.
        stack = []
        for off, tag, attrs, end in entries:
            if tag is None:
                if stack:
                    stack.pop()
                continue
            die = Die(off, tag, attrs)
            self.dies[off + self.base] = die
            if stack:
                die.parent = stack[-1]
                stack[-1].children.append(die)
            else:
                self.roots.append(die)
            sib = attrs.get(AT_SIBLING)
            if sib is not None and sib - self.base > end:
                stack.append(die)

    # ---- type rendering -------------------------------------------------

    def ref(self, off):
        return self.dies.get(off)

    def type_of(self, die):
        """Return (base, prefix_ops) describing die's type, or None."""
        a = die.attrs
        if AT_FUND_TYPE in a:
            return ("fund", a[AT_FUND_TYPE], [])
        if AT_USER_DEF_TYPE in a:
            return ("udt", a[AT_USER_DEF_TYPE], [])
        if AT_MOD_FUND_TYPE in a:
            b = a[AT_MOD_FUND_TYPE]
            return ("fund", self._u16(b, len(b) - 2), list(b[:-2]))
        if AT_MOD_U_D_TYPE in a:
            b = a[AT_MOD_U_D_TYPE]
            return ("udt", self._u32(b, len(b) - 4), list(b[:-4]))
        return None

    def type_name(self, t, inner=""):
        """Render type t around declarator text ``inner``."""
        if t is None:
            return ("void " + inner).strip()
        kind, val, mods = t
        decl = inner
        # Modifiers read outermost first ("pointer to const char"), so apply
        # them to the declarator from the innermost side.
        for m in mods:
            if m == MOD_POINTER:
                decl = "*" + decl
            elif m == MOD_REFERENCE:
                decl = "&" + decl
            elif m == MOD_CONST:
                decl = " const " + decl if decl else " const"
            elif m == MOD_VOLATILE:
                decl = " volatile " + decl if decl else " volatile"
        decl = decl.strip()
        if kind == "fund":
            base = FUND.get(val, "fund_0x%04X" % val)
            return (base + " " + decl).strip()
        return self.udt_name(val, decl)

    def udt_name(self, off, decl):
        die = self.ref(off)
        if die is None:
            return ("type_0x%X %s" % (off, decl)).strip()
        tg = TAG.get(die.tag)
        if tg == "array_type":
            return self.array_name(die, decl)
        if tg in ("subroutine_type",):
            ret = self.type_name(self.type_of(die), "")
            params = ", ".join(self.param_list(die))
            d = "(%s)" % decl if decl else "(*)"
            return "%s %s(%s)" % (ret, d, params)
        if tg == "ptr_to_member_type":
            cls = self.ref(die.get(0x01D2))  # AT_containing_type
            cn = cls.name if cls is not None and cls.name else "?"
            return self.type_name(self.type_of(die), "%s::*%s" % (cn, decl))
        kw = {"structure_type": "struct", "union_type": "union",
              "class_type": "class", "enumeration_type": "enum"}.get(tg, "")
        nm = die.name or "<anon_0x%X>" % off
        if tg == "typedef":
            kw = ""
        return ("%s %s %s" % (kw, nm, decl)).strip()

    def array_name(self, die, decl):
        b = die.get(AT_SUBSCR_DATA, b"")
        dims, elem, i = [], None, 0
        while i < len(b):
            fmt = b[i]; i += 1
            if fmt == 0:  # FMT_FT_C_C: index type, constant bounds
                lo = struct.unpack_from(self.e + "i", b, i + 2)[0]
                hi = struct.unpack_from(self.e + "i", b, i + 6)[0]
                dims.append(hi - lo + 1)
                i += 10
            elif fmt == 8:  # FMT_ET: element type attribute follows
                at = self._u16(b, i); i += 2
                form = at & 0xF
                if form == 5:
                    elem = ("fund", self._u16(b, i), []); i += 2
                elif form == 2:
                    elem = ("udt", self._u32(b, i), []); i += 4
                elif form == 3:
                    sz = self._u16(b, i); blk = b[i + 2:i + 2 + sz]; i += 2 + sz
                    if at == AT_MOD_FUND_TYPE:
                        elem = ("fund", self._u16(blk, len(blk) - 2), list(blk[:-2]))
                    else:
                        elem = ("udt", self._u32(blk, len(blk) - 4), list(blk[:-4]))
                break
            else:
                break
        if decl.startswith("*") or decl.startswith("&"):
            decl = "(%s)" % decl
        suffix = "".join("[%d]" % n for n in dims)
        return self.type_name(elem, decl + suffix)

    def param_list(self, die):
        out = []
        for c in die.children:
            tg = TAG.get(c.tag)
            if tg == "formal_parameter":
                out.append(self.type_name(self.type_of(c), c.name or ""))
            elif tg == "unspecified_parameters":
                out.append("...")
        return out

    # ---- locations ------------------------------------------------------

    def location(self, die):
        b = die.get(AT_LOCATION)
        if b is None:
            return None
        st, i, reg = [], 0, None
        while i < len(b):
            op = b[i]; i += 1
            if op in (OP_REG, OP_BASEREG, OP_ADDR, OP_CONST):
                v = self._u32(b, i); i += 4
                if op == OP_REG:
                    return ("reg", v)
                if op == OP_BASEREG:
                    reg = v
                    st.append(0)
                else:
                    st.append(v)
            elif op == OP_ADD and len(st) >= 2:
                y = st.pop(); x = st.pop(); st.append((x + y) & 0xFFFFFFFF)
            elif op in (OP_DEREF2, OP_DEREF4):
                pass
            else:
                break
        if reg is not None:
            off = st[-1] if st else 0
            if off & 0x80000000:
                off -= 1 << 32
            return ("stack", reg, off)
        if st:
            return ("addr", st[-1])
        return None

    def member_offset(self, die):
        loc = self.location(die)
        if loc and loc[0] == "addr":
            return loc[1]
        return None

    def enum_values(self, die):
        b = die.get(AT_ELEMENT_LIST, b"")
        out, i = [], 0
        while i + 4 <= len(b):
            v = struct.unpack_from(self.e + "i", b, i)[0]
            k = b.index(b"\0", i + 4)
            out.append((b[i + 4:k].decode("latin1"), v))
            i = k + 1
        out.reverse()
        return out
