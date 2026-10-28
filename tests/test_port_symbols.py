"""The instruction model of the symbol port (tools/port_symbols.py)."""
import unittest

from tools.port_symbols import Side,Symbol,is_address,walk,written


def code(*words):
    return list(zip(range(0x80010000,0x80010000+4*len(words),4),words))


class Written(unittest.TestCase):
    def test_register_writes(self):
        self.assertEqual(written(0x7C6903A6),set())        # mtctr r3
        self.assertEqual(written(0x7C64292E),set())        # stwx r3,r4,r5
        self.assertEqual(written(0x7C832378),{3})          # mr r3,r4
        self.assertEqual(written(0x7C64282E),{3})          # lwzx r3,r4,r5
        self.assertEqual(written(0x7C632214),{3})          # add r3,r3,r4
        self.assertEqual(written(0x84830008),{3,4})        # lwzu r4,8(r3)
        self.assertEqual(written(0x9421FFF0),{1})          # stwu r1,-16(r1)
        self.assertEqual(written(0xBBA10014),{29,30,31})   # lmw r29,20(r1)


class Walk(unittest.TestCase):
    def masked(self,*words):
        return [w for w,_ in walk(code(*words),{},is_address)]

    def test_constant_lis_is_not_masked(self):
        # lis r3,0x70; ori r3,r3,0: a constant, not an address.
        self.assertEqual(self.masked(0x3C600070,0x60630000),[0x3C600070,0x60630000])

    def test_address_pair_is_masked_across_mtctr(self):
        # lis r9,0x8020; mtctr r3; addi r3,r9,0x1234
        out=walk(code(0x3D208020,0x7C6903A6,0x38691234),{},is_address)
        self.assertEqual([w for w,_ in out],[0x3D200000,0x7C6903A6,0x38690000])
        self.assertEqual(out[2][1],0x80201234)

    def test_join_with_different_halves_is_not_masked(self):
        # lis r9,0x8020; beq +8; lis r9,0x8030; addi r3,r9,0x10
        self.assertEqual(self.masked(0x3D208020,0x41820008,0x3D208030,0x38690010),
                         [0x3D208020,0x41820008,0x3D208030,0x38690010])

    def test_join_with_the_same_half_masks_both(self):
        self.assertEqual(self.masked(0x3D208020,0x41820008,0x3D208020,0x38690010),
                         [0x3D200000,0x41820008,0x3D200000,0x38690000])

    def test_nothing_is_known_after_an_unconditional_branch(self):
        # lis r9,0x8020; b +8; addi r3,r9,0x10 (no path reaches it)
        self.assertEqual(self.masked(0x3D208020,0x48000008,0x38690010)[2],0x38690010)

    def test_a_data_entry_starts_with_nothing_known(self):
        words=code(0x3D208020,0x38690010)
        out=walk(words,{},is_address,{words[1][0]})
        self.assertEqual([w for w,_ in out],[0x3D208020,0x38690010])


class Adopt(unittest.TestCase):
    def side(self):
        # This side's toolkit split one function in two: 0xAC bytes, then a 4-byte one.
        side=object.__new__(Side)
        fn=lambda a,n:Symbol('fn_%08X = .text:0x%08X; // type:function size:0x%X'%(a,a,n))
        side.functions={0x1000:(fn(0x1000,0xAC),[],b'short'),0x10AC:(fn(0x10AC,4),[],b'tail')}
        side.code=lambda addr,size:([],b'whole' if (addr,size)==(0x1000,0xB0) else b'other')
        return side

    def other(self,size,digest):
        return (Symbol('f = .text:0x2000; // type:function size:0x%X'%size),[],digest)

    def test_a_longer_function_covering_the_split_pairs(self):
        side=self.side()
        self.assertTrue(side.adopt(0x1000,self.other(0xB0,b'whole')))
        self.assertEqual(sorted(side.functions),[0x1000])

    def test_different_contents_do_not_pair(self):
        side=self.side()
        self.assertFalse(side.adopt(0x1000,self.other(0xB0,b'elsewhere')))
        self.assertEqual(sorted(side.functions),[0x1000,0x10AC])

    def test_a_paired_function_is_not_swallowed(self):
        side=self.side()
        self.assertFalse(side.adopt(0x1000,self.other(0xB0,b'whole'),{0x10AC}))
        self.assertEqual(sorted(side.functions),[0x1000,0x10AC])

    def test_a_shorter_function_does_not_replace(self):
        self.assertFalse(self.side().adopt(0x1000,self.other(0x80,b'whole')))


if __name__=='__main__':
    unittest.main()
