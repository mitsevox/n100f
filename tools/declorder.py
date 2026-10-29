#!/usr/bin/env python3
"""Search the order of a function's local declarations for the one that matches the target.

    python3 tools/declorder.py <source> <symbol> [--name <function>] [--block N] [--list]
    python3 tools/declorder.py ... --out <file>     write the best order found to <file>

On CodeWarrior the order of local declarations decides how the register allocator numbers, and so
ranks, the locals; a function that differs only in register choice often matches with the original
order. This tool reorders only declarations whose position cannot change what the code does:

- each sits on one line of its own in the run of declarations that opens a block;
- its type is a pointer, a scalar (built-in or one of the project's scalar typedefs) or a type named
  with --pod, which asserts that the type has trivial construction and destruction;
- its initializer, if any, calls nothing, assigns nothing and reads none of the other reordered
  locals.

Any other line in the run (a declaration that fails a rule, a comment, a blank line, a preprocessor
line) stays where it is and splits the run: declarations move only within their segment. Up to
--max orders are tried exhaustively, the reverse order first; above that, a deterministic local
search moves one declaration at a time while the score improves. Every order is compiled with
tools/trial.py, so the source tree is never written; --out writes the best order to a file of your
choice for review. A match found here is a hint about the original declaration order, not evidence
of it, and the result still has to read as plausible source.

<symbol> is the function's symbol for the comparison; --name is how the definition is spelled in
the source when that is not the symbol's base name (for example `Class::Method`)."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import itertools
import math
import os
import re
import sys
import tempfile
try:
    from tools.trial import DEFAULT_TARGET,Unit,trial
except ModuleNotFoundError:
    from trial import DEFAULT_TARGET,Unit,trial

SCALARS={'char','short','int','long','float','double','bool','unsigned','signed',
         'S8','U8','S16','U16','S32','U32','S64','U64','F32','F64',
         's8','u8','s16','u16','s32','u32','s64','u64','f32','f64',
         'RwInt8','RwUInt8','RwInt16','RwUInt16','RwInt32','RwUInt32','RwReal','RwBool',
         'BOOL','size_t','uint','ulong','ushort','uchar'}
DECL=re.compile(r'^(?P<type>(?:(?:const|register|unsigned|signed|long|short|struct|enum|union)\s+)*'
                r'[A-Za-z_]\w*(?:\s*::\s*[A-Za-z_]\w*)*)'
                r'(?P<stars>[\s*]*?)\s*(?P<name>[A-Za-z_]\w*)(?P<array>(?:\s*\[[^\]]*\])*)'
                r'(?:\s*=\s*(?P<init>.+?))?\s*;\s*$')
WORD=re.compile(r'[A-Za-z_]\w*')
KEYWORDS={'return','goto','break','continue','case','default','else','delete','new','throw','sizeof','typedef'}


def strip_code(text):
    """`text` with comments, string and character literals blanked, offsets kept."""
    out=list(text)
    i,n=0,len(text)
    while i<n:
        c=text[i]
        if text.startswith('//',i):
            j=text.find('\n',i)
            j=n if j<0 else j
        elif text.startswith('/*',i):
            j=text.find('*/',i+2)
            j=n if j<0 else j+2
        elif c in '"\'':
            j=i+1
            while j<n and text[j]!=c:j+=2 if text[j]=='\\' else 1
            j=min(j+1,n)
        else:
            i+=1
            continue
        for k in range(i,j):
            if out[k]!='\n':out[k]=' '
        i=j
    return ''.join(out)


def find_body(text,name,line=None):
    """(open, close) offsets of the braces of the definition of `name` in `text`; with several
    definitions (overloads), the one whose name is on `line` (1-based)."""
    code=strip_code(text)
    pattern=re.compile(r'(?<![\w:~])%s\s*\('%r'\s*::\s*'.join(map(re.escape,name.split('::'))))
    found=[]
    for m in pattern.finditer(code):
        depth,i=0,m.end()-1
        while i<len(code):
            if code[i]=='(':depth+=1
            elif code[i]==')':
                depth-=1
                if depth==0:break
            i+=1
        rest=re.match(r'\s*(?:const\s*)?\{',code[i+1:])
        if rest:found.append((code.count('\n',0,m.start())+1,i+1+rest.end()))
    if line is not None:found=[f for f in found if f[0]==line]
    if len(found)!=1:
        raise ValueError('%d definitions of %s%s'%(len(found),name,
                         ' (lines %s; choose one with --line)'%', '.join(str(f[0]) for f in found) if found else ''))
    start=found[0][1]-1
    return start,match_brace(code,start)


def match_brace(code,start):
    depth=0
    for i in range(start,len(code)):
        if code[i]=='{':depth+=1
        elif code[i]=='}':
            depth-=1
            if depth==0:return i
    raise ValueError('unbalanced braces')


def source_name(symbol):
    """How a CodeWarrior symbol's function is spelled at its definition: `f` for f__Fv,
    `Class::f` for f__5ClassFv, the symbol itself for a C function."""
    m=re.match(r'^(\w+?)__(\d+)(\w*)$',symbol)
    if m and len(m.group(3))>=int(m.group(2)):return '%s::%s'%(m.group(3)[:int(m.group(2))],m.group(1))
    m=re.match(r'^(\w+?)__F',symbol)
    return m.group(1) if m else symbol


class Decl:
    def __init__(self,line,match):
        self.line=line
        self.name=match.group('name')
        self.type=match.group('type')
        self.pointer='*' in match.group('stars')
        self.reference='&' in match.group('stars')
        self.init=match.group('init')

    def trivial(self,pod):
        if self.reference:return False
        words=self.type.replace('::',' :: ').split()
        return self.pointer or 'enum' in words or words[-1] in SCALARS or words[-1] in pod

    def reads(self):
        return set(WORD.findall(self.init or ''))

    def pure(self):
        """The initializer calls and assigns nothing."""
        if self.init is None:return True
        init=self.init
        if re.search(r'(\+\+|--|[^=!<>]=[^=]|\bnew\b|\bdelete\b)',' %s '%init):return False
        for m in re.finditer(r'([A-Za-z_]\w*)\s*\(',init):
            if m.group(1)!='sizeof':return False
        return True


def blocks(text,start,end):
    """[(first line index, [line indices of the leading run])] for each block of the body whose
    opening brace ends its line, in source order; line indices are into text.split('\\n')."""
    code=strip_code(text)
    lines=text.split('\n')
    codelines=code.split('\n')
    offsets=list(itertools.accumulate(len(l)+1 for l in lines))
    def line_of(offset):
        lo,hi=0,len(offsets)
        while lo<hi:
            mid=(lo+hi)//2
            if offsets[mid]<=offset:lo=mid+1
            else:hi=mid
        return lo
    out=[]
    for i in range(start,end):
        if code[i]!='{':continue
        row=line_of(i)
        if codelines[row][i-(offsets[row-1] if row else 0)+1:].strip():continue
        run=[]
        for r in range(row+1,len(lines)):
            raw,stripped=lines[r].strip(),codelines[r].strip()
            if raw.startswith('#'):break
            if stripped and not (DECL.match(stripped) and WORD.match(stripped).group(0) not in KEYWORDS):break
            run.append(r)
        while run and not codelines[run[-1]].strip():run.pop()
        out.append((row,run))
    return out


def segments(lines,run,pod):
    """The run split into segments of movable declarations: [[(line index, Decl)]]."""
    decls={}
    for r in run:
        m=DECL.match(strip_code(lines[r]).strip())
        if m:decls[r]=Decl(lines[r],m)
    names={d.name for d in decls.values()}
    out,current=[],[]
    for r in run:
        d=decls.get(r)
        if d and d.trivial(pod) and d.pure() and not (d.reads()&(names-{d.name})):current.append((r,d))
        else:
            if len(current)>1:out.append(current)
            current=[]
    if len(current)>1:out.append(current)
    return out


def apply(lines,segs,orders):
    """The source lines with each segment's declarations placed in the given order."""
    new=list(lines)
    for seg,order in zip(segs,orders):
        for (row,_),(_,decl) in zip(seg,(seg[k] for k in order)):new[row]=decl.line
    return new


def count(segs):
    return math.prod(math.factorial(len(s)) for s in segs)


def candidates(segs):
    """Every order: as written, the full reversal, then the rest."""
    first=[tuple(range(len(s))) for s in segs]
    rev=[tuple(reversed(range(len(s)))) for s in segs]
    seen={tuple(first),tuple(rev)}
    yield first
    yield rev
    for combo in itertools.product(*(itertools.permutations(range(len(s))) for s in segs)):
        if combo not in seen:yield list(combo)


def neighbours(orders):
    """Every order reachable by moving one declaration to another place in its segment."""
    for k,order in enumerate(orders):
        for i in range(len(order)):
            for j in range(len(order)):
                if i==j:continue
                moved=list(order)
                moved.insert(j,moved.pop(i))
                yield orders[:k]+[tuple(moved)]+orders[k+1:]


class Search:
    def __init__(self,unit,source,symbol,lines,segs,jobs,objdiff=None):
        self.unit,self.source,self.symbol,self.lines,self.segs=unit,source,symbol,lines,segs
        self.jobs,self.objdiff=jobs,objdiff
        self.scores={}
        self.tmp=tempfile.mkdtemp(prefix='declorder-')

    def score(self,orders):
        key=tuple(map(tuple,orders))
        if key not in self.scores:
            handle,path=tempfile.mkstemp(dir=self.tmp)
            with os.fdopen(handle,'w') as f:f.write('\n'.join(apply(self.lines,self.segs,orders)))
            result=trial(self.unit,path,self.symbol,self.objdiff)
            os.unlink(path)
            self.scores[key]=result['percent'] if result['compiled'] else -1.0
        return self.scores[key]

    def many(self,batch):
        batch=[b for b in batch if tuple(map(tuple,b)) not in self.scores]
        with ThreadPoolExecutor(self.jobs) as pool:list(pool.map(self.score,batch))

    def best(self):
        key,value=max(self.scores.items(),key=lambda kv:kv[1])
        return [list(k) for k in key],value

    def exhaustive(self,orders):
        batch=[]
        for order in orders:
            batch.append(order)
            if len(batch)==self.jobs*4:
                self.many(batch)
                batch=[]
                if self.best()[1]==100.0:return
        self.many(batch)

    def climb(self):
        current=[tuple(range(len(s))) for s in self.segs]
        self.many([current,[tuple(reversed(o)) for o in current]])
        current,value=self.best()
        while value<100.0:
            moves=list(neighbours([tuple(o) for o in current]))
            self.many(moves)
            step,better=max(((m,self.scores[tuple(map(tuple,m))]) for m in moves),key=lambda mv:mv[1])
            if better<=value:break
            current,value=step,better


def main(argv=None):
    parser=argparse.ArgumentParser(description=__doc__.split('\n')[0])
    parser.add_argument('variant',help='file holding the function')
    parser.add_argument('symbol',help='function symbol to compare')
    parser.add_argument('--as',dest='source',help='unit source path the variant stands in for')
    parser.add_argument('--name',help='the definition as spelled in the source (default: from the symbol)')
    parser.add_argument('--line',type=int,help='line of the definition, when the name is overloaded')
    parser.add_argument('--block',type=int,default=0,help='block of the function to reorder, 0 = the body (see --list)')
    parser.add_argument('--pod',action='append',default=[],help='type with trivial construction and destruction')
    parser.add_argument('--max',type=int,default=5040,help='largest number of orders tried exhaustively')
    parser.add_argument('--jobs','-j',type=int,default=os.cpu_count() or 1)
    parser.add_argument('--target',default=DEFAULT_TARGET)
    parser.add_argument('--objdiff',help='objdiff-cli binary')
    parser.add_argument('--list',action='store_true',help='show the blocks and their movable segments and stop')
    parser.add_argument('--out',help='write the source with the best order found to this file')
    args=parser.parse_args(argv)
    name=args.name or source_name(args.symbol)
    text=open(args.variant).read()
    lines=text.split('\n')
    start,end=find_body(text,name,args.line)
    found=blocks(text,start,end)
    pod=set(args.pod)
    if args.list:
        for k,(row,run) in enumerate(found):
            segs=segments(lines,run,pod)
            if segs or k==0:print('block %d at line %d: %s'%(k,row+1,'; '.join(', '.join(d.name for _,d in s) for s in segs) or 'nothing to reorder'))
        return 0
    if args.block>=len(found):parser.error('the function has %d blocks'%len(found))
    segs=segments(lines,found[args.block][1],pod)
    if not segs:
        print('nothing to reorder in block %d'%args.block,file=sys.stderr)
        return 2
    unit=Unit(args.target,args.source or args.variant)
    search=Search(unit,args.source or args.variant,args.symbol,lines,segs,args.jobs,args.objdiff)
    if count(segs)>args.max:search.climb()
    else:search.exhaustive(candidates(segs))
    best,value=search.best()
    base=search.scores[tuple(tuple(range(len(s))) for s in segs)]
    print('%s: %d orders tried, as written %.2f, best %.2f'%(args.symbol,len(search.scores),base,value))
    for seg,order in zip(segs,best):print('  '+', '.join(seg[k][1].name for k in order))
    if args.out:
        with open(args.out,'w') as f:f.write('\n'.join(apply(lines,segs,best)))
    return 0 if value==100.0 else 1


if __name__=='__main__':
    sys.exit(main())
