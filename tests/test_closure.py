"""Listing what keeps an unlinked unit from linking (tools/closure.py)."""
import contextlib
import io
import json
import tempfile
from pathlib import Path
import unittest

from tests.test_literal_check import code,elf
from tools.closure import main,order_state,section_issues,section_sizes,unit_row
from tools.literal_check import Elf

BLR=0x4E800020
FUNC=2;OBJECT=1;GLOBAL=1;WEAK=2


def unit_objects(directory,target_order,compiled_order,target_data,compiled_data,weak=0):
    """A target and a compiled object: one .text function per name in the given order, and data
    sections {name: size}; `weak` adds a weak local static of that size to the compiled .sdata2."""
    def write(path,order,data,weak_size):
        sections=[('.text',1,code(*[BLR]*len(order)))]+[(n,8 if 'bss' in n else 1,s if 'bss' in n else b'\0'*s)
                                                         for n,s in data.items()]
        symbols=[(name,4*i,4,GLOBAL,FUNC,1) for i,name in enumerate(order)]
        if weak_size:
            sections.append(('.sdata2',1,b'\0'*weak_size))
            symbols.append(('_half$localstatic3$sqrtf__Ff',0,weak_size,WEAK,OBJECT,len(sections)))
        elf(path,sections,symbols,{})
    target=Path(directory)/'target.o';compiled=Path(directory)/'compiled.o'
    write(target,target_order,target_data,0)
    write(compiled,compiled_order,compiled_data,weak)
    return target,compiled


class Order(unittest.TestCase):
    def test_states(self):
        t=[('.text',0,'a'),('.text',4,'b'),('.text',8,'c')]
        self.assertEqual(order_state(t,t),('same',0,3))
        self.assertEqual(order_state(t,[('.text',0,'c'),('.text',4,'b'),('.text',8,'a')]),('reversed',3,3))
        self.assertEqual(order_state(t,[('.text',0,'b'),('.text',4,'a'),('.text',8,'c')]),('mixed',1,3))

    def test_compiled_only_functions_are_ignored(self):
        t=[('.text',0,'a'),('.text',4,'b')]
        c=[('.text',0,'a'),('.text',4,'inlined_elsewhere'),('.text',8,'b')]
        self.assertEqual(order_state(t,c),('same',0,1))

    def test_fewer_than_two_shared(self):
        self.assertEqual(order_state([('.text',0,'a')],[('.text',0,'a')]),('-',0,0))


class Sections(unittest.TestCase):
    def test_kinds(self):
        issues=section_issues({'.data':100,'.bss':48,'.rodata':16},{'.data':40,'.sdata2':12,'.rodata':20})
        # .rodata is 4 bytes off, inside the slack; the rest come back in DATA order.
        self.assertEqual(issues,[('.data','size',100,40),('.bss','missing',48,0),('.sdata2','unsplit',0,12)])

    def test_slack_below_eight_bytes(self):
        self.assertEqual(section_issues({'.data':100},{'.data':104}),[])
        self.assertEqual(section_issues({},{'.sdata2':4}),[])

    def test_weak_local_static_is_not_counted(self):
        with tempfile.TemporaryDirectory() as d:
            target,compiled=unit_objects(d,['a'],['a'],{'.sdata2':8},{},weak=16)
            self.assertEqual(section_sizes(Elf(target))['.sdata2'],8)
            # The compiled .sdata2 holds only the weak static, so nothing of the unit's own is left.
            self.assertEqual(section_sizes(Elf(compiled))['.sdata2'],0)


class Row(unittest.TestCase):
    def test_blockers_and_rank(self):
        with tempfile.TemporaryDirectory() as d:
            target,compiled=unit_objects(d,['a','b'],['b','a'],{'.data':64},{'.data':32})
            unit={'name':'main/u','target_path':str(target),'base_path':str(compiled),
                  'metadata':{'progress_categories':['game']}}
            scores={'measures':{'total_code':'1024','total_data':'1024'},
                    'functions':[{'name':'a','size':'4','fuzzy_match_percent':100.0},
                                 {'name':'b','size':'4','fuzzy_match_percent':99.5}],
                    'sections':[{'name':'.data','size':'64','fuzzy_match_percent':50.0},
                                {'name':'.text','size':'8','fuzzy_match_percent':99.0}]}
            row=unit_row(unit,scores)
            self.assertEqual(row['order'],'reversed')
            self.assertEqual(row['sections'],[('.data','size',64,32)])
            # One function miss, one data miss, the order and one section problem.
            self.assertEqual(row['blockers'],4)
            self.assertEqual(row['per_blocker'],512)

    def test_missing_compiled_object(self):
        row=unit_row({'name':'main/u','base_path':'/nonexistent.o','target_path':'/nonexistent.o'},{})
        self.assertEqual(row['order'],'no object')
        self.assertEqual(row['blockers'],0)


class Report(unittest.TestCase):
    def test_main_reads_the_build(self):
        with tempfile.TemporaryDirectory() as d:
            root=Path(d)
            target,compiled=unit_objects(d,['a'],['a'],{},{})
            build=root/'build'/'T';build.mkdir(parents=True)
            (build/'objdiff.json').write_text(json.dumps({'units':[
                {'name':'main/linked','metadata':{'complete':True}},
                {'name':'main/open','target_path':str(target),'base_path':str(compiled),'metadata':{}}]}))
            (build/'report.json').write_text(json.dumps({'units':[
                {'name':'main/open','measures':{'total_code':'4'},'functions':[],'sections':[]},
                {'name':'main/auto','metadata':{'auto_generated':True},'sections':[{'name':'.bss','size':'32'}]}]}))
            out=root/'rows.json'
            import tools.closure as closure
            saved=closure.ROOT
            closure.ROOT=root
            try:
                with contextlib.redirect_stdout(io.StringIO()):
                    self.assertEqual(main(['--target','T','--json',str(out)]),0)
            finally:
                closure.ROOT=saved
            rows=json.loads(out.read_text())
            self.assertEqual([r['unit'] for r in rows],['main/open'])
            self.assertEqual(rows[0]['blockers'],0)


if __name__=='__main__':
    unittest.main()
