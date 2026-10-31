"""Choosing and widening link candidates (tools/link_candidates.py)."""
import unittest

from tools.link_candidates import Names,pending,relabelled,widen
from tools.port_symbols import Symbol

CONFIGURE='''Matching = True
def MatchingFor(*ids):
    return True
config.libs = [
    Object(MatchingFor("A", "B"), "both.cpp"),
    Object(MatchingFor("A"), "only_a.cpp"),
    Object(MatchingFor("B"), "only_b.cpp"),
    Object(NonMatching, "none.cpp"),
]
'''


class Declarations(unittest.TestCase):
    def test_pending_names_the_source_but_not_the_target(self):
        self.assertEqual([(u,ids) for _,u,ids in pending(CONFIGURE,'A','B')],[('only_a.cpp',['A'])])

    def test_widen_adds_the_target(self):
        rows=[(row,ids) for row,_,ids in pending(CONFIGURE,'A','B')]
        new=widen(CONFIGURE,rows,'B')
        self.assertIn('Object(MatchingFor("A", "B"), "only_a.cpp")',new)
        self.assertEqual(pending(new,'A','B'),[])
        self.assertEqual(new.replace('Object(MatchingFor("A", "B"), "only_a.cpp")','Object(MatchingFor("A"), "only_a.cpp")'),CONFIGURE)

    def test_widen_refuses_a_row_without_the_declaration(self):
        with self.assertRaises(ValueError):widen(CONFIGURE,[(1,['A'])],'B')


class Naming(unittest.TestCase):
    names=Names([Symbol(l) for l in (
        'f = .text:0x80001000; // type:function size:0x40',
        'lbl_80001010 = .text:0x80001010; // type:label',
        'g = .data:0x80002000; // type:object size:0x10',
    )])

    def test_start_of_a_symbol_is_its_name(self):
        self.assertEqual(self.names(0x80001000),['f'])

    def test_inside_a_function_is_marked(self):
        self.assertEqual(self.names(0x80001010),['<inside f+0x10>'])

    def test_inside_data_is_the_object(self):
        self.assertEqual(self.names(0x80002008),['g'])

    def test_nothing_there(self):
        self.assertEqual(self.names(0x80000000),[])


class Labels(unittest.TestCase):
    source='U32 lbl_80003000;\nvoid g() { fn_80001000(); lbl_80003000 = 1; }\n// alt_lbl_80009999 is not one\n'

    def test_a_label_the_sibling_lacks_holds_the_unit_back(self):
        self.assertEqual(relabelled(self.source,{'fn_80001000'}),['lbl_80003000'])

    def test_labels_the_sibling_defines_do_not(self):
        self.assertEqual(relabelled(self.source,{'fn_80001000','lbl_80003000'}),[])


if __name__=='__main__':
    unittest.main()
