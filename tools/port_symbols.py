#!/usr/bin/env python3
"""Carry one target's symbols and unit splits over to a sibling target built from the same source.

    python3 tools/port_symbols.py <from id> <to id>            report only
    python3 tools/port_symbols.py <from id> <to id> --write    also rewrite the sibling's
                                                               symbols.txt and splits.txt

Both originals must be present at their config.yml paths. The sibling's symbols.txt is the
toolkit's own map of its original (the tool's input as well as its output): ported symbols replace
the toolkit's at the same address, and every address nothing maps to keeps the toolkit's symbol.

Functions pair in passes. A function whose instructions, with relocated fields masked (branch
targets, `lis` halves and their partner instructions, r2/r13 offsets), occur exactly once on each
side is an anchor; anchors that break address order are dropped. Between consecutive pairs, equal
runs of functions pair in order, by position alone. A branch target that resolves the same way
from every paired caller pairs too, and so does a function the sibling's toolkit merged into its
predecessor, at the same offset, when its masked instructions are equal there. Data symbols map
through the addresses that pairs with the same instruction shape and mapped pointer words resolve
to, by a majority over every reference into the symbol and only where the bytes are equal outside
pointer words; a run of unreferenced data between two mapped symbols takes their shifts where the
bytes allow. A label maps at its offset inside a mapped symbol whose contents are the same there.
Only pairs whose contents are the same apart from relocations are written; the report lists the
pairs found by position alone whose contents differ, which are not.

A unit's section range is ported only when both edges map, the range has the same length on both
sides, and every symbol in it maps with the range's shift and the same contents; any other unit
is left out of the sibling, with the reason in the report. The pairing establishes sameness of
contents, not that a name or boundary is right on either target: every ported name keeps
whatever evidence its origin target recorded."""
import argparse
import bisect
from collections import Counter, defaultdict
import hashlib
from pathlib import Path
import re
import struct
import sys

ROOT=Path(__file__).resolve().parents[1]
SYMBOL=re.compile(r'^(\S+) = (\.\w+):0x([0-9A-Fa-f]+);(?: // (.*))?$')
LOADS_STORES=set(range(32,56))|{14,24}
# Toolkit address labels: a ported one is relabelled with its sibling address.
AUTO=re.compile(r'^(fn|lbl|jumptable)_([0-9A-F]{8})(.*)$')


def config(target):
    text=(ROOT/'config'/target/'config.yml').read_text()
    get=lambda key:re.search(r'^'+key+r':\s*(\S+)',text,re.M).group(1)
    return ROOT/get('object_base')/get('object'),ROOT/get('symbols'),ROOT/get('splits')


class Dol:
    def __init__(self,path):
        self.data=path.read_bytes()
        h=struct.unpack('>18I18I18III',self.data[:0xE0])
        self.sections=[(h[18+i],h[i],h[36+i]) for i in range(18) if h[36+i]]
        self.bss=(h[54],h[55])
        words=struct.unpack('>%dI'%((len(self.data)-0x100)//4),self.data[0x100:0x100+(len(self.data)-0x100)//4*4])
        self.sda={}
        for a,b in zip(words,words[1:]):
            reg=(a>>21)&31
            # __init_registers: lis rN,hi; ori rN,rN,lo for r2 and r13, first occurrence each.
            if a>>26==15 and reg in (2,13) and (a>>16)&31==0 and b>>26==24 and (b>>21)&31==reg and (b>>16)&31==reg:
                self.sda.setdefault(reg,((a&0xFFFF)<<16)|(b&0xFFFF))

    def read(self,addr,size):
        for va,off,sz in self.sections:
            if va<=addr and addr+size<=va+sz:return self.data[off+addr-va:off+addr-va+size]
        return None


class Symbol:
    def __init__(self,line):
        m=SYMBOL.match(line)
        if not m:raise ValueError('Unreadable symbol line: '+line)
        self.name,self.section,self.addr=m.group(1),m.group(2),int(m.group(3),16)
        self.attrs=(m.group(4) or '').split()
        self.kind=self.attr('type')
        self.size=int(self.attr('size') or '0',16)

    def attr(self,key):
        return next((a.split(':',1)[1] for a in self.attrs if a.startswith(key+':')),None)

    def line(self,addr=None,size=None):
        attrs=[('size:0x%X'%size if a.startswith('size:') and size is not None else a) for a in self.attrs]
        name=self.name;m=AUTO.match(name)
        if addr is not None and m and int(m.group(2),16)==self.addr:name='%s_%08X%s'%(m.group(1),addr,m.group(3))
        return '%s = %s:0x%08X;%s'%(name,self.section,self.addr if addr is None else addr,' // '+' '.join(attrs) if attrs else '')


def symbols(path):
    return [Symbol(l) for l in path.read_text().splitlines() if l.strip()]


def splits(path):
    head,units,cur=[],[],None
    for line in path.read_text().splitlines():
        m=re.match(r'^(\S.*?):(.*)$',line)
        if m and line!='Sections:':
            cur=(m.group(1),m.group(2),[]);units.append(cur);continue
        m=re.match(r'^\s+(\.\w+)\s+start:0x([0-9A-F]+) end:0x([0-9A-F]+)(.*)$',line)
        if m and cur:cur[2].append((m.group(1),int(m.group(2),16),int(m.group(3),16),m.group(4)))
        elif cur is None:head.append(line)
    return head,units


def written(w):
    """The GPRs an instruction may write (conservative: X-forms give both register fields)."""
    op,rd,ra=w>>26,(w>>21)&31,(w>>16)&31
    if op in (7,8,12,13,14,15,32,33,34,35,40,41,42,43,46):return {rd}
    if op in (20,21,23,24,25,26,27,28,29):return {ra}
    if op==31:return {rd,ra}
    if op==18 and w&1:return {0}|set(range(3,13))
    if op in (19,16) and w&1:return {0}|set(range(3,13))
    return set()


def walk(code,sda):
    """Per instruction: the masked word, and the absolute address its relocated field names, if any.
    A `lis` half is followed only until its register is written again."""
    hi={};out=[]
    for pc,w in code:
        op,rd,ra=w>>26,(w>>21)&31,(w>>16)&31;ref=None;new=None
        if op==18 and not w&2:
            disp=w&0x03FFFFFC;ref=(pc+(disp-0x04000000 if disp&0x02000000 else disp))&0xFFFFFFFF;w&=~0x03FFFFFC
        elif op==15 and ra==0:
            new=(w&0xFFFF)<<16;w&=~0xFFFF
        elif op==24 and rd in hi:
            ref=hi[rd]|(w&0xFFFF);w&=~0xFFFF
        elif op in LOADS_STORES and op!=24 and (ra in sda or ra in hi):
            simm=(w&0xFFFF)-((w&0x8000)<<1)
            ref=((sda[ra] if ra in sda else hi[ra])+simm)&0xFFFFFFFF;w&=~0xFFFF
        for r in written(w):hi.pop(r,None)
        if new is not None:hi[rd]=new
        out.append((w,ref))
    return out


class Side:
    def __init__(self,target):
        dol,sym,split=config(target)
        self.dol=Dol(dol);self.symbols=symbols(sym);self.splits_path=split
        self.bss=self.dol.bss
        self.in_bss=lambda addr:self.bss[0]<=addr<self.bss[0]+self.bss[1] and self.dol.read(addr,1) is None
        self.functions={}
        for s in self.symbols:
            if s.kind!='function' or not s.size:continue
            found=self.code(s.addr,s.size)
            if found:self.functions[s.addr]=(s,)+found

    def code(self,addr,size):
        """The walked instructions and masked hash of `size` bytes of code at `addr`."""
        raw=self.dol.read(addr,size)
        if raw is None or size%4:return None
        walked=walk(list(zip(range(addr,addr+size,4),struct.unpack('>%dI'%(size//4),raw))),{2:self.dol.sda[2],13:self.dol.sda[13]})
        return walked,hashlib.sha1(struct.pack('>%dI'%len(walked),*[w for w,_ in walked])).digest()

    def adopt(self,addr,other):
        """Pair `other` (a function of the other side) with the code at `addr` when its masked
        instructions are the same there: the toolkit merged it into a neighbour on this side."""
        if addr in self.functions:return self.functions[addr][2]==other[2]
        found=self.code(addr,other[0].size)
        if not found or found[1]!=other[2]:return False
        self.functions[addr]=(None,)+found
        return True


def monotonic(pairs):
    """Longest chain of pairs increasing on both sides (patience sorting on the second address)."""
    pairs=sorted(pairs);tails=[];links={}
    keys=[]
    for p in pairs:
        i=bisect.bisect_left(keys,p[1])
        links[p]=tails[i-1] if i else None
        if i==len(keys):keys.append(p[1]);tails.append(p)
        else:keys[i]=p[1];tails[i]=p
    out=[];p=tails[-1] if tails else None
    while p:out.append(p);p=links[p]
    return dict(reversed(out))


def pair_functions(a,b):
    count_a=Counter(h for _,_,h in a.functions.values());count_b=Counter(h for _,_,h in b.functions.values())
    by_b={h:addr for addr,(_,_,h) in b.functions.items() if count_b[h]==1}
    anchors={addr:by_b[h] for addr,(_,_,h) in a.functions.items() if count_a[h]==1 and h in by_b}
    fmap=monotonic(anchors.items())
    for _ in range(4):
        # Equal runs between consecutive mapped functions pair in order.
        sa=sorted(a.functions);sb=sorted(b.functions);ib={x:i for i,x in enumerate(sb)}
        mapped=sorted(fmap.items());added={}
        for (a0,b0),(a1,b1) in zip(mapped,mapped[1:]):
            ra=[x for x in sa[sa.index(a0)+1:sa.index(a1)]];rb=sb[ib[b0]+1:ib[b1]]
            if ra and len(ra)==len(rb):added.update(zip(ra,rb))
        fmap.update(added)
        # Branch targets of identical-shape pairs.
        votes=defaultdict(Counter)
        for x,y in fmap.items():
            for (wa,ra),(wb,rb) in same_shape(a.functions[x][1],b.functions[y][1]):
                if ra in a.functions and rb is not None:votes[ra][rb]+=1
        before=len(fmap)
        for x,c in votes.items():
            if x not in fmap and len(c)==1:
                y=next(iter(c))
                if y not in fmap.values() and (y in b.functions or b.adopt(y,a.functions[x])):fmap[x]=y
        # A function the sibling's toolkit merged into the one before it sits at the same offset.
        sa=sorted(a.functions)
        for p,x in zip(sa,sa[1:]):
            if p in fmap and x not in fmap:
                y=fmap[p]+x-p
                if y not in fmap.values() and b.adopt(y,a.functions[x]):fmap[x]=y
        fmap=monotonic(fmap.items())
        if len(fmap)==before and not added:break
    return fmap


def same_shape(wa,wb):
    if len(wa)!=len(wb) or any(x>>26!=y>>26 for (x,_),(y,_) in zip(wa,wb)):return []
    return [(p,q) for p,q in zip(wa,wb) if p[1] is not None and q[1] is not None]


def pointers(raw,side):
    """Offsets of the words that hold an address inside one of the side's sections or its .bss
    (a tiny negative float can still land there; such a word is excused from the byte comparison)."""
    return [i for i in range(0,len(raw)-3,4) if section_of(side.dol,int.from_bytes(raw[i:i+4],'big'),side.bss) is not None]


def section_of(dol,addr,bss):
    for i,(va,_,sz) in enumerate(dol.sections):
        if va<=addr<va+sz:return i
    return 'bss' if bss[0]<=addr<bss[0]+bss[1] else None


def same_bytes(a,b,s,t):
    """Initialized bytes equal outside pointer words; both absent (.bss) counts as equal."""
    ra=a.dol.read(s.addr,s.size);rb=b.dol.read(t,s.size)
    if ra is None or rb is None:return ra is None and rb is None and a.in_bss(s.addr) and b.in_bss(t)
    ptr=set(pointers(ra,a))|set(pointers(rb,b))
    return all(ra[k:k+4]==rb[k:k+4] for k in range(0,len(ra)//4*4,4) if k not in ptr) and ra[len(ra)//4*4:]==rb[len(rb)//4*4:]


def port(a,b):
    fmap=pair_functions(a,b)
    refs=defaultdict(Counter)
    def vote(ra,rb):
        sa,sb=section_of(a.dol,ra,a.bss),section_of(b.dol,rb,b.bss)
        if sa is not None and sa==sb:refs[ra][rb-ra]+=1
    for x,y in fmap.items():
        for (_,ra),(_,rb) in same_shape(a.functions[x][1],b.functions[y][1]):vote(ra,rb)
    smap={id(s):fmap[s.addr] for s in a.symbols if s.kind=='function' and s.addr in fmap}
    by_section=defaultdict(list)
    for s in a.symbols:
        if s.kind!='function':by_section[s.section].append(s)
    for items in by_section.values():items.sort(key=lambda s:s.addr)
    done_ptr=set()
    while True:
        before=len(smap)
        starts=sorted(refs)
        for s in a.symbols:
            if s.kind=='function' or id(s) in smap:continue
            lo=bisect.bisect_left(starts,s.addr);hi=bisect.bisect_left(starts,s.addr+max(s.size,1))
            c=Counter()
            for r in starts[lo:hi]:c.update(refs[r])
            if c:
                delta,n=c.most_common(1)[0]
                if n*2>sum(c.values()) and same_bytes(a,b,s,s.addr+delta):smap[id(s)]=s.addr+delta
        # Runs of unmapped data between two mapped symbols: every symbol of the run keeps the
        # shift of the symbol before it up to one split point, then takes the shift of the one
        # after it, wherever the bytes allow.
        for items in by_section.values():
            known=[i for i,s in enumerate(items) if id(s) in smap]
            for i,j in zip(known,known[1:]):
                if j-i<2:continue
                dp=smap[id(items[i])]-items[i].addr;dn=smap[id(items[j])]-items[j].addr
                run=items[i+1:j]
                for k in range(len(run),-1,-1):
                    if all(same_bytes(a,b,s,s.addr+dp) for s in run[:k]) and all(same_bytes(a,b,s,s.addr+dn) for s in run[k:]):
                        for s in run[:k]:smap[id(s)]=s.addr+dp
                        for s in run[k:]:smap[id(s)]=s.addr+dn
                        break
        # Pointer words of mapped initialized data pair the addresses they hold.
        for s in a.symbols:
            if s.kind=='function' or id(s) not in smap or id(s) in done_ptr:continue
            done_ptr.add(id(s))
            ra=a.dol.read(s.addr,s.size);rb=b.dol.read(smap[id(s)],s.size)
            if ra is None or rb is None:continue
            for k in pointers(ra,a):
                if k+4<=len(rb):vote(int.from_bytes(ra[k:k+4],'big'),int.from_bytes(rb[k:k+4],'big'))
        if len(smap)==before:break
    # A label inside a mapped symbol whose contents are the same there keeps its offset.
    for s in a.symbols:
        if s.kind!='label' or id(s) in smap:continue
        for p in a.symbols:
            if p.kind!='label' and id(p) in smap and p.addr<=s.addr<p.addr+p.size and equal_at(a,b,p,smap[id(p)]):
                smap[id(s)]=smap[id(p)]+s.addr-p.addr;break
    consistent(a,smap)
    return fmap,smap


def equal_at(a,b,s,t):
    """A symbol's contents are the same at `t`: masked code for a function, bytes outside pointer
    words for data."""
    if s.kind=='function':
        here=b.code(t,s.size)
        return bool(here) and s.addr in a.functions and here[1]==a.functions[s.addr][2]
    return same_bytes(a,b,s,t)


def consistent(a,smap):
    """Keep, per section, the longest chain of mapped symbols in the same order on both sides, then
    drop any data symbol that would overlap the next one on the sibling."""
    by_section=defaultdict(list)
    for s in a.symbols:
        if id(s) in smap:by_section[s.section].append(s)
    for items in by_section.values():
        items.sort(key=lambda s:(s.addr,-s.size))
        keys=[];tails=[];links={}
        for s in items:
            i=bisect.bisect_right(keys,smap[id(s)])
            links[id(s)]=tails[i-1] if i else None
            if i==len(keys):keys.append(smap[id(s)]);tails.append(s)
            else:keys[i]=smap[id(s)];tails[i]=s
        keep=set();s=tails[-1] if tails else None
        while s is not None:keep.add(id(s));s=links[id(s)]
        chain=[s for s in items if id(s) in keep]
        for s,t in zip(chain,chain[1:]):
            if s.kind not in ('function','label') and t.addr!=s.addr and smap[id(t)]<smap[id(s)]+s.size:keep.discard(id(s))
        for s in items:
            if id(s) not in keep:del smap[id(s)]


def merge(toolkit,ported):
    """The sibling's symbol lines: every ported symbol, and every toolkit symbol that no ported
    symbol covers. A toolkit symbol running into a ported one is cut short at its start. Where the
    toolkit already gives the address that name as another kind of symbol (its signatures name
    gTRKInterruptVectorTable a label), the toolkit's symbol stays: the toolkit adds it again
    during analysis, and the name would be defined twice."""
    kinds={(s.name,s.addr):s.kind for s in toolkit}
    ported=[p for p in ported if kinds.get((p[3].split(' ',1)[0],p[1]),None) in (None,Symbol(p[3]).kind)]
    starts=defaultdict(list);cover=defaultdict(list)
    for section,t,size,_ in ported:
        starts[section].append(t);cover[section].append((t,t+max(size,1)))
    for v in starts.values():v.sort()
    for v in cover.values():v.sort()
    lines=[(t,0,line) for _,t,_,line in ported]
    for s in toolkit:
        st=starts.get(s.section,[]);cv=cover.get(s.section,[])
        i=bisect.bisect_right(cv,(s.addr,float('inf')))
        if any(a<=s.addr<b for a,b in cv[max(0,i-64):i]):continue
        size=s.size
        j=bisect.bisect_right(st,s.addr)
        if j<len(st) and st[j]<s.addr+size:
            if s.kind=='function':continue
            size=st[j]-s.addr
        lines.append((s.addr,1,s.line(size=size)))
    lines.sort(key=lambda x:(x[0],x[1]))
    # A label naming the same address as a symbol of the same name would be defined twice in the
    # toolkit's assembly for a unit that does not link its source.
    named={(t,l.split(' ',1)[0]) for t,_,l in lines if 'type:label' not in l}
    return [l for t,_,l in lines if 'type:label' not in l or (t,l.split(' ',1)[0]) not in named]


def port_splits(a,b,smap):
    """Each unit's section ranges on the sibling, or the reason the unit is left out. A range's
    start is the sibling address of the symbol at its start; its end is the sibling address of the
    symbol at its end (the next unit's first symbol), or the section's end."""
    head,units=splits(a.splits_path)
    at=defaultdict(list)
    for s in a.symbols:at[(s.section,s.addr)].append(s)
    sec_end_a={};sec_end_b={}
    for side,ends in ((a,sec_end_a),(b,sec_end_b)):
        for s in side.symbols:ends[s.section]=max(ends.get(s.section,0),s.addr+s.size)
        for s in side.symbols:
            for va,_,sz in side.dol.sections:
                if va<=s.addr<va+sz:ends[s.section]=max(ends[s.section],va+sz)
    b_size={s.addr:s.size for s in b.symbols if s.kind=='function'}
    real_a=defaultdict(list)
    for s in a.symbols:
        if s.kind!='label':real_a[s.section].append(s)
    for v in real_a.values():v.sort(key=lambda s:s.addr)
    starts_b=defaultdict(set)
    for s in b.symbols:starts_b[s.section].add(s.addr)
    for s in a.symbols:
        if id(s) in smap:starts_b[s.section].add(smap[id(s)])
    starts_b={k:sorted(v) for k,v in starts_b.items()}
    def pointers_agree(section,x,y,size):
        ra=a.dol.read(x,size);rb=b.dol.read(y,size)
        if ra is None or rb is None:return False
        fn={s.addr:smap[id(s)] for s in a.symbols if id(s) in smap}
        return all(fn.get(int.from_bytes(ra[k:k+4],'big'))==int.from_bytes(rb[k:k+4],'big') for k in pointers(ra,a)) and len(pointers(ra,a))==len(pointers(rb,b))
    def where(section,addr,end):
        found={smap[id(s)] for s in at.get((section,addr),[]) if id(s) in smap}
        if len(found)==1:return found.pop()
        if end and addr>=sec_end_a.get(section,0):return sec_end_b.get(section)
        items=real_a.get(section,[]);i=bisect.bisect_left([s.addr for s in items],addr)
        # The edge sits on an unmapped symbol: it moves with the end of the mapped symbol before
        # it, onto the first sibling symbol after that end.
        if i<len(items) and items[i].addr==addr and i and id(items[i-1]) in smap:
            p=items[i-1];pe=smap[id(p)]+(b_size.get(smap[id(p)],p.size) if p.kind=='function' else p.size)
            if p.addr+p.size<=addr:
                st=starts_b.get(section,[]);j=bisect.bisect_left(st,pe)
                if j<len(st):return st[j]
        # Constructor and destructor tables hold no symbols: an edge inside one moves with the
        # table when every pointer in the moved range points at the mapped function.
        if section in ('.ctors','.dtors'):
            base=[s for s in a.symbols if s.section==section and s.addr<=addr and id(s) in smap]
            if base:
                p=max(base,key=lambda s:s.addr);y=smap[id(p)]+addr-p.addr
                if pointers_agree(section,p.addr,smap[id(p)],addr-p.addr+4 if not end else addr-p.addr):return y
        return None
    out=[];left=[]
    for name,attrs,ranges in units:
        new=[];why=None;partial=[]
        for section,start,end,rest in ranges:
            s1,e1=where(section,start,False),where(section,end,True)
            if s1 is None or e1 is None:why='%s edge does not map'%section;break
            if e1-s1!=end-start:why='%s range is 0x%X bytes here, 0x%X on the source'%(section,e1-s1,end-start);break
            inside=[s for s in a.symbols if s.section==section and start<=s.addr<end]
            moved=[s.name for s in inside if id(s) not in smap or smap[id(s)]-s.addr!=s1-start]
            if moved:why='%s symbols unmapped or shifted: %s'%(section,' '.join(moved));break
            changed=[s.name for s in inside if s.kind!='label' and not equal_at(a,b,s,smap[id(s)])]
            if changed:why='%s contents differ: %s'%(section,' '.join(changed));break
            new.append((section,s1,e1,rest))
        if why:left.append((name,why));continue
        out.append((name,attrs,new,partial))
    # Units must keep their order and not overlap on the sibling.
    by_section=defaultdict(list)
    for u in out:
        for r in u[2]:by_section[r[0]].append((r[1],r[2],u[0]))
    bad=set()
    for rs in by_section.values():
        rs.sort()
        for (s0,e0,n0),(s1,e1,n1) in zip(rs,rs[1:]):
            if s1<e0:bad.update((n0,n1))
    left+=[(u[0],'ranges overlap another unit') for u in out if u[0] in bad]
    out=[u for u in out if u[0] not in bad]
    return out,left


def write_splits(path,head_path,units):
    head=[]
    for line in head_path.read_text().splitlines():
        if line and not line.startswith(('Sections:','\t',' ')):break
        head.append(line)
    while head and not head[-1].strip():head.pop()
    text='\n'.join(head)+'\n'
    for name,attrs,ranges,_ in units:
        text+='\n%s:%s\n'%(name,attrs)
        for section,start,end,rest in ranges:text+='\t%-11s start:0x%08X end:0x%08X%s\n'%(section,start,end,rest)
    path.write_text(text)


def main():
    p=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('source');p.add_argument('target');p.add_argument('--write',action='store_true')
    p.add_argument('--report',type=Path,help='write the per-symbol and per-unit report (TSV)')
    args=p.parse_args()
    a,b=Side(args.source),Side(args.target)
    fmap,smap=port(a,b)
    ported=[];report=[]
    for s in a.symbols:
        t=smap.get(id(s))
        if t is None:report.append(('symbol',s.name,'0x%08X'%s.addr,'-','unmapped'));continue
        if s.kind!='label' and not equal_at(a,b,s,t):
            report.append(('symbol',s.name,'0x%08X'%s.addr,'0x%08X'%t,'not written: paired by position only, contents differ'));continue
        ported.append((s.section,t,s.size,s.line(t)))
        report.append(('symbol',s.name,'0x%08X'%s.addr,'0x%08X'%t,'same contents'))
    lines=merge(b.symbols,ported)
    units,left=port_splits(a,b,smap)
    report+=[('unit',name,'-','-','ported') for name,_,_,_ in units]
    report+=[('unit',name,'-','-','left out: '+why) for name,why in left]
    fn=[s for s in a.symbols if s.kind=='function'];other=[s for s in a.symbols if s.kind!='function']
    same={(r[1],r[2]) for r in report if r[0]=='symbol' and r[4]=='same contents'}
    position=[r for r in report if r[0]=='symbol' and r[4].startswith('not written')]
    kinds={(s.name,'0x%08X'%s.addr):s.kind for s in a.symbols}
    print('functions: %d of %d written with the same contents, %d more paired by position only'%(sum(kinds[k]=='function' for k in same),len(fn),sum(kinds[(r[1],r[2])]=='function' for r in position)))
    print('other symbols: %d of %d written with the same contents, %d more paired by position only'%(sum(kinds[k]!='function' for k in same),len(other),sum(kinds[(r[1],r[2])]!='function' for r in position)))
    print('units: %d of %d ported, %d left out'%(len(units),len(units)+len(left),len(left)))
    if args.report:
        args.report.parent.mkdir(parents=True,exist_ok=True)
        args.report.write_text('kind\tname\tfrom\tto\tstate\n'+''.join('\t'.join(r)+'\n' for r in report))
    if args.write:
        b_sym=config(args.target)[1]
        b_sym.write_text(''.join(l+'\n' for l in lines))
        write_splits(b.splits_path,b.splits_path,units)


if __name__=='__main__':
    main()
