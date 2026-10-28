#!/usr/bin/env python3
"""The units declared Matching on one target that a sibling target's map is ready to link.

    python3 tools/link_candidates.py <from id> <to id>            report only
    python3 tools/link_candidates.py <from id> <to id> --write    also widen each candidate's
                                                                  MatchingFor(...) in configure.py
                                                                  to name <to id>

A candidate is a unit whose configure.py declaration is MatchingFor(...) naming <from id> but not
<to id>, which the sibling's splits.txt carries over (tools/port_symbols.py), and every reference of
which, outside its own ranges, resolves to a name the sibling's symbols.txt defines: from its
functions' instructions (branch targets, small-data offsets, `lis` pairs that form an address) and
from the address words of its data. A reference into the middle of another unit's function (a
label such as OSContext's `__RAS_` edges) holds the unit back as well, because the linker resolves
it only when the sibling map names that label. The origin target's original must be at its
config.yml path.

This is a necessary condition, not proof: `--write` only proposes the declarations, and the
sibling's build check (`main.dol: OK`) decides. A widened unit that fails the build goes back to
naming <from id> alone, with the build's message as the reason."""
import argparse
import bisect
from collections import defaultdict
from pathlib import Path
import re
import sys
try:
    from tools.matching_scope import scopes
    from tools.port_symbols import Side,config,pointers,splits,symbols
except ModuleNotFoundError:
    from matching_scope import scopes
    from port_symbols import Side,config,pointers,splits,symbols

ROOT=Path(__file__).resolve().parents[1]
AUTO=re.compile(r'^(fn|lbl|jumptable)_[0-9A-F]{8}$')


def pending(text,src,dst):
    """[(row, unit, ids)] for every declaration that names `src` but not `dst`."""
    return [(row,unit,scope) for row,unit,scope in scopes(text)
            if isinstance(scope,list) and src in scope and dst not in scope]


def widen(text,rows,dst):
    """configure.py with MatchingFor(...) on each row given as (row, ids) extended by `dst`."""
    lines=text.split('\n')
    for row,ids in rows:
        old='MatchingFor(%s)'%', '.join('"%s"'%i for i in ids)
        line=lines[row-1]
        if line.count(old)!=1:raise ValueError('configure.py:%d: expected one %s: %s'%(row,old,line.strip()))
        lines[row-1]=line.replace(old,'MatchingFor(%s)'%', '.join('"%s"'%i for i in ids+[dst]))
    return '\n'.join(lines)


class Names:
    """What an address of the origin target is called: its own names, else the symbol it falls
    in, marked when that is a position inside a function."""
    def __init__(self,syms):
        self.at=defaultdict(list)
        for s in syms:
            if not AUTO.match(s.name):self.at[s.addr].append(s.name)
        self.syms=sorted([s for s in syms if s.kind!='label'],key=lambda s:s.addr)
        self.starts=[s.addr for s in self.syms]

    def owner(self,x):
        i=bisect.bisect_right(self.starts,x)-1
        while i>=0 and not self.syms[i].addr<=x<self.syms[i].addr+max(self.syms[i].size,1):
            if self.syms[i].addr<x-0x100000:return None
            i-=1
        return self.syms[i] if i>=0 else None

    def __call__(self,x):
        if self.at.get(x):return self.at[x]
        o=self.owner(x)
        if o is None:return []
        if o.kind=='function' and x!=o.addr:return ['<inside %s+0x%X>'%(o.name,x-o.addr)]
        return [o.name]


def unresolved(side,names,ranges,defined):
    """The names a unit's ranges reference outside themselves that `defined` lacks."""
    inside=lambda x:any(s<=x<e for _,s,e,_ in ranges)
    missing=set()
    for sec,s,e,_ in ranges:
        for sym in side.symbols:
            if sym.section!=sec or not s<=sym.addr<e:continue
            refs=[]
            if sym.kind=='function':
                if sym.addr in side.functions:refs=[ref for _,ref in side.functions[sym.addr][1] if ref is not None]
            elif sym.kind!='label':
                raw=side.dol.read(sym.addr,sym.size)
                if raw:refs=[int.from_bytes(raw[k:k+4],'big') for k in pointers(raw,side)]
            for ref in refs:
                if not inside(ref):missing.update(n for n in names(ref) if n not in defined)
    return sorted(missing)


def main():
    parser=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('source');parser.add_argument('target')
    parser.add_argument('--write',action='store_true',help='widen the candidates in configure.py')
    args=parser.parse_args()
    path=ROOT/'configure.py';text=path.read_text()
    todo=pending(text,args.source,args.target)
    side=Side(args.source);names=Names(side.symbols)
    ranges={n:r for n,_,r in splits(side.splits_path)[1]}
    _,sym_b,split_b=config(args.target)
    carried={n for n,_,_ in splits(split_b)[1]}
    defined={s.name for s in symbols(sym_b) if not AUTO.match(s.name)}
    ready=[];out=[]
    for row,unit,ids in todo:
        if unit not in carried:out.append('%s: not carried over to %s'%(unit,args.target));continue
        missing=unresolved(side,names,ranges[unit],defined)
        if missing:out.append('%s: references names %s does not define: %s'%(unit,args.target,', '.join(missing)));continue
        ready.append((row,ids));out.append('%s: candidate'%unit)
    print('\n'.join(out))
    print('%d of %d units declared for %s but not %s are candidates'%(len(ready),len(todo),args.source,args.target))
    if args.write and ready:
        path.write_text(widen(text,ready,args.target))
        print('widened %d declaration(s); build %s and check main.dol: OK before committing'%(len(ready),args.target))


if __name__=='__main__':
    try:main()
    except ValueError as error:sys.exit('link_candidates: '+str(error))
