"""Finding the movable local declarations of a function (tools/declorder.py)."""
import unittest

from tools.declorder import apply,blocks,candidates,count,find_body,neighbours,segments,source_name

SOURCE='''static int helper(int x);

void zThing_Update(zThing* t, F32 dt)
{
    S32 i;
    xVec3 pos;
    zEnt* ent = t->ent;
    U32 flags = 0;
    // a comment
    F32 speed = t->speed * dt;
    F32 scale = speed;
    char* name = zThingName(t);
    S32& ref = t->count;

    for (i = 0; i < 4; i++)
    {
        U8 a;
        U8 b;
        a = i;
    }
    helper(i);
}

int helper(int x) { return x; }
'''


def run(source,name,block=0,pod=()):
    lines=source.split('\n')
    start,end=find_body(source,name)
    return lines,segments(lines,blocks(source,start,end)[block][1],set(pod))


class Body(unittest.TestCase):
    def test_finds_the_definition_not_the_prototype_or_calls(self):
        start,end=find_body(SOURCE,'helper')
        self.assertEqual(SOURCE[start:end+1],'{ return x; }')

    def test_class_members(self):
        text='void Foo::Bar(int)\n{\n    int a;\n    int b;\n}\nvoid Bar(int) {}\n'
        start,end=find_body(text,'Foo::Bar')
        self.assertTrue(text[start:end+1].startswith('{\n    int a;'))
        self.assertEqual(text[slice(*find_body(text,'Bar'))]+'}','{}')

    def test_overloads_need_a_line(self):
        text='void f(int a)\n{\n}\nvoid f(float b)\n{\n}\n'
        with self.assertRaisesRegex(ValueError,'lines 1, 4'):find_body(text,'f')
        self.assertEqual(find_body(text,'f',4),(text.index('{',text.index('float')),text.rindex('}')))

    def test_source_names(self):
        self.assertEqual(source_name('zThing_Update__FP6zThingf'),'zThing_Update')
        self.assertEqual(source_name('Update__6zThingFf'),'zThing::Update')
        self.assertEqual(source_name('RwV3dNormalize'),'RwV3dNormalize')


class Segments(unittest.TestCase):
    def test_body_segments(self):
        _,segs=run(SOURCE,'zThing_Update')
        # pos is a struct (not --pod), the comment splits, scale reads speed, name calls,
        # ref is a reference.
        self.assertEqual([[d.name for _,d in s] for s in segs],[['ent','flags']])

    def test_pod_types_join(self):
        _,segs=run(SOURCE,'zThing_Update',pod=['xVec3'])
        self.assertEqual([[d.name for _,d in s] for s in segs],[['i','pos','ent','flags']])

    def test_inner_block(self):
        _,segs=run(SOURCE,'zThing_Update',block=1)
        self.assertEqual([[d.name for _,d in s] for s in segs],[['a','b']])

    def test_apply_reorders_only_the_segment(self):
        lines,segs=run(SOURCE,'zThing_Update',pod=['xVec3'])
        new=apply(lines,segs,[(3,2,1,0)])
        self.assertEqual(new[4:8],['    U32 flags = 0;','    zEnt* ent = t->ent;','    xVec3 pos;','    S32 i;'])
        self.assertEqual(new[8:],lines[8:])
        self.assertEqual(sorted(new),sorted(lines))


class Orders(unittest.TestCase):
    def test_candidates_start_as_written_then_reversed_and_cover_all(self):
        segs=[[0,1,2],[0,1]]
        orders=list(candidates(segs))
        self.assertEqual(orders[0],[(0,1,2),(0,1)])
        self.assertEqual(orders[1],[(2,1,0),(1,0)])
        self.assertEqual(len(orders),count(segs))
        self.assertEqual(len({tuple(o) for o in orders}),count(segs))

    def test_neighbours_move_one_declaration(self):
        moves=list(neighbours([(0,1,2)]))
        self.assertIn([(1,0,2)],moves)
        self.assertIn([(1,2,0)],moves)
        self.assertNotIn([(0,1,2)],moves)


if __name__=='__main__':
    unittest.main()
