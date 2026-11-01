#!/usr/bin/env python3
"""List what still keeps each unlinked unit from linking, ranked by the bytes its closure links.

A unit links all or nothing: every function and every data section has to equal the target, the
functions have to come out in the target's order, and the unit's data has to have ranges in
splits.txt. objdiff scores functions and symbols by name, so it reports neither the order nor a data
section that is missing from the source or from the split. This report puts the four side by side
for every unit that is not linked:

  - function misses: functions below 100% in the progress report, with how many are at 99% or more;
  - data misses: data sections below 100% in the progress report;
  - order: whether the compiled object emits the target's functions in the target's order (functions
    only the compiled object has are left out: the linker strips them);
  - sections: data sections whose compiled size differs from the target object's by 8 bytes or more,
    are missing from the compiled object, or are compiled with no range in splits.txt.

A compiled weak local static of an inline function (`_half$localstatic3$sqrtf__Ff`) is not counted
in its section's size: the linker keeps one copy for the whole executable.

    python3 tools/closure.py --target GIHE78-rev0 [--unit <name>] [--top N] [--json <file>]

It reads build/<target id>/objdiff.json for the units and their objects and build/<target id>/
report.json for the scores (`ninja -f build/<id>/build.ninja build/<id>/report.json` writes it).
Rows are ranked by kilobytes linked per blocker, where the blockers are the function misses, the
data misses, a wrong order and each section problem. The report changes nothing and fails on
nothing; bytes no unit owns are summed
per section at the end."""
import argparse
import json
from pathlib import Path
import sys

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT))

from tools.literal_check import Elf  # noqa: E402

DATA=('.rodata','.data','.bss','.sdata','.sbss','.sdata2','.sbss2','.ctors','.dtors')
CODE=('.init','.text')
SLACK=8


def section_sizes(elf):
    """Size per code or data section, less the weak local statics of inline functions."""
    sizes={}
    for s in elf.sections:
        if s['name'] in CODE+DATA:
            sizes[s['name']]=sizes.get(s['name'],0)+s['size']
    for sym in elf.symbols:
        if sym['bind']==2 and '$localstatic' in sym['name'] and 0<sym['shndx']<len(elf.sections):
            name=elf.sections[sym['shndx']]['name']
            if name in sizes:
                sizes[name]-=sym['size']
    return sizes


def section_issues(target,compiled):
    """Data sections the unit cannot link with: [(section, kind, target size, compiled size)], kind
    `size` (both have it, sizes differ), `missing` (only the target has it) or `unsplit` (only the
    compiled object has it, so splits.txt gives the unit no range for it)."""
    issues=[]
    for name in DATA:
        t=target.get(name,0);c=compiled.get(name,0)
        if t and not c:
            issues.append((name,'missing',t,c))
        elif c and not t:
            if c>=SLACK:
                issues.append((name,'unsplit',t,c))
        elif abs(t-c)>=SLACK:
            issues.append((name,'size',t,c))
    return issues


def functions(elf):
    """Code functions by offset: [(section, offset, name)]."""
    return sorted((elf.sections[shndx]['name'],value,name) for name,(shndx,value,_) in elf.functions.items()
                  if elf.sections[shndx]['name'] in CODE)


def order_state(target,compiled):
    """`same`, `reversed` or `mixed` for the functions both objects define, with the number of
    pairs out of order and the number of pairs; `-` when fewer than two are shared."""
    position={name:i for i,(_,_,name) in enumerate(compiled)}
    seq=[position[name] for _,_,name in target if name in position]
    pairs=len(seq)*(len(seq)-1)//2
    if pairs==0:
        return '-',0,0
    inverted=sum(1 for i in range(len(seq)) for j in range(i+1,len(seq)) if seq[i]>seq[j])
    if inverted==0:
        return 'same',0,pairs
    return ('reversed' if inverted==pairs else 'mixed'),inverted,pairs


def unit_row(unit,scores):
    """One unlinked unit's blockers."""
    measures=scores.get('measures',{})
    misses=[(f['name'],int(f.get('size',0)),f.get('fuzzy_match_percent',0)) for f in scores.get('functions',[])
            if f.get('fuzzy_match_percent',0)<100]
    data=[(s['name'],int(s.get('size',0)),s.get('fuzzy_match_percent',0)) for s in scores.get('sections',[])
          if s['name'] in DATA and int(s.get('size',0)) and s.get('fuzzy_match_percent',0)<100]
    row=dict(unit=unit['name'],category=(unit.get('metadata',{}).get('progress_categories') or ['-'])[0],
             bytes=int(measures.get('total_code',0))+int(measures.get('total_data',0)),
             function_misses=misses,data_misses=data,order='-',inverted=0,pairs=0,sections=[])
    base=unit.get('base_path');target=unit.get('target_path')
    if not base or not Path(base).exists():
        row['order']='no object'
    elif target and Path(target).exists():
        t=Elf(target);c=Elf(base)
        row['order'],row['inverted'],row['pairs']=order_state(functions(t),functions(c))
        row['sections']=section_issues(section_sizes(t),section_sizes(c))
    blockers=len(misses)+len(data)+len(row['sections'])+(row['order'] in ('reversed','mixed'))
    row['blockers']=blockers
    row['per_blocker']=row['bytes']/max(blockers,1)
    return row


def unowned(report):
    """Bytes per section in units the split generated for addresses no unit owns."""
    sizes={}
    for u in report['units']:
        if u.get('metadata',{}).get('auto_generated'):
            for s in u.get('sections',[]):
                sizes[s['name']]=sizes.get(s['name'],0)+int(s.get('size',0))
    return sizes


def describe(row):
    misses=row['function_misses']
    high=sum(1 for _,_,p in misses if p>=99)
    order=row['order']
    if order in ('reversed','mixed'):
        order=f"{order} {row['inverted']}/{row['pairs']}"
    sections=', '.join(f"{n} {k} {t}->{c}" for n,k,t,c in row['sections']) or '-'
    data=', '.join(f"{n} {p:.0f}%" for n,_,p in row['data_misses']) or '-'
    return (f"{row['per_blocker']/1024:7.1f} {row['bytes']/1024:7.1f}  {len(misses):3} ({high:2})  "
            f"{row['unit'].removeprefix('main/')}\n{'':27}data {data}; order {order}; sections {sections}")


def main(argv=None):
    parser=argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--target',required=True)
    parser.add_argument('--unit',action='append',help='only these units (repeatable)')
    parser.add_argument('--top',type=int,help='only the first N rows')
    parser.add_argument('--json',type=Path,help='also write every row to this file')
    args=parser.parse_args(argv)
    build=ROOT/'build'/args.target
    try:
        project=json.loads((build/'objdiff.json').read_text())
        report=json.loads((build/'report.json').read_text())
    except FileNotFoundError as e:
        sys.exit(f'{e.filename}: configure and build {args.target}, then build {build}/report.json')
    scores={u['name']:u for u in report['units']}
    rows=[]
    for unit in project['units']:
        meta=unit.get('metadata',{})
        if meta.get('complete') or meta.get('auto_generated'):
            continue
        if args.unit and unit['name'] not in args.unit and unit['name'].removeprefix('main/') not in args.unit:
            continue
        rows.append(unit_row(unit,scores.get(unit['name'],{})))
    rows.sort(key=lambda r:(-r['per_blocker'],r['unit']))
    if args.json:
        args.json.write_text(json.dumps(rows,indent=1)+'\n')
    shown=rows[:args.top] if args.top else rows
    print(f'{args.target}: {len(rows)} unlinked units, {sum(r["bytes"] for r in rows)/1024:.1f} KB')
    print('KB/block      KB  fn (>=99)  unit')
    for row in shown:
        print(describe(row))
    ready=[r['unit'] for r in rows if r['blockers']==0]
    print(f"no blocker found (try linking): {', '.join(ready) or 'none'}")
    if not args.unit:
        free=unowned(report)
        print('bytes no unit owns: '+(', '.join(f'{n} {s} B' for n,s in sorted(free.items())) or 'none'))
    return 0


if __name__=='__main__':
    sys.exit(main())
