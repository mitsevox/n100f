"""Compiling a variant in a mirrored scratch tree and scoring one function (tools/trial.py)."""
import os
import tempfile
import unittest

from tools.trial import mirror,relocate,score,shown


def symbol(name,percent,target=0,instructions=()):
    return {'name':name,'match_percent':percent,'target_symbol':target,'instructions':list(instructions)}


class Relocate(unittest.TestCase):
    def test_moves_every_output_of_the_unit(self):
        command=('wibo mwcceppc.exe -i build/X/include -c src/flat/a.cpp -o build/X/src/flat'
                 ' && python3 tools/transform_dep.py build/X/src/flat/a.d build/X/src/flat/a.d')
        self.assertEqual(relocate(command,'build/X/src/flat/a.o','/s/.trial'),
                         'wibo mwcceppc.exe -i build/X/include -c src/flat/a.cpp -o /s/.trial'
                         ' && python3 tools/transform_dep.py /s/.trial/a.d /s/.trial/a.d')

    def test_refuses_a_command_writing_elsewhere(self):
        with self.assertRaises(ValueError):relocate('cc -c a.c -o build/Y/a.o','build/X/src/a.o','/s')


class Mirror(unittest.TestCase):
    def test_variant_sits_at_the_source_path_among_linked_siblings(self):
        with tempfile.TemporaryDirectory() as root,tempfile.TemporaryDirectory() as scratch:
            for path in ('src/flat/a.cpp','src/flat/a.h','src/other.c','include/x.h','build/tools/cc'):
                os.makedirs(os.path.join(root,os.path.dirname(path)),exist_ok=True)
                open(os.path.join(root,path),'w').close()
            place=mirror(root,'src/flat/a.cpp',scratch)
            self.assertEqual(str(place),os.path.join(scratch,'src','flat','a.cpp'))
            self.assertFalse(os.path.exists(place))
            for linked in ('include','build','src/other.c','src/flat/a.h'):
                self.assertTrue(os.path.islink(os.path.join(scratch,linked)),linked)
            for real in ('src','src/flat'):
                self.assertFalse(os.path.islink(os.path.join(scratch,real)),real)


class Score(unittest.TestCase):
    def diff(self,left,right):
        return {'left':{'symbols':left},'right':{'symbols':right}}

    def test_pairs_by_name_and_lists_differing_lines(self):
        same={'instruction':{'formatted':'li r3, 0x0'}}
        changed=({'instruction':{'formatted':'mr r31, r3'},'diff_kind':'DIFF_ARG_MISMATCH'},
                 {'instruction':{'formatted':'mr r30, r3'},'diff_kind':'DIFF_ARG_MISMATCH'})
        result=score(self.diff([symbol('g',100.0),symbol('f',97.5,7,[same,changed[0]])],
                               [symbol('f',97.5,7,[same,changed[1]])]),'f')
        self.assertEqual(result,(97.5,[('mr r31, r3','mr r30, r3','DIFF_ARG_MISMATCH')]))

    def test_missing_or_unpaired_scores_zero(self):
        self.assertEqual(score(self.diff([symbol('f',100.0)],[]),'f'),(0.0,[]))
        self.assertEqual(score(self.diff([symbol('f',0.0,None)],[symbol('f',0.0,None)]),'f'),(0.0,[]))
        self.assertEqual(score(self.diff([],[symbol('f',100.0)]),'f'),(0.0,[]))


class Shown(unittest.TestCase):
    def test_only_identical_shows_100(self):
        self.assertEqual(shown(100.0),'100.00')
        self.assertEqual(shown(99.99504),'99.99')
        self.assertEqual(shown(72.5),'72.50')


if __name__=='__main__':
    unittest.main()
