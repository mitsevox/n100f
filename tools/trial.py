#!/usr/bin/env python3
"""Compile one variant of a unit and say whether one function matches the target.

    python3 tools/trial.py <source> <symbol>                  the file as it is
    python3 tools/trial.py <variant> <symbol> --as <source>   a copy kept outside the tree
    python3 tools/trial.py ... --diff                         also list the differing lines

<source> is a unit's source path in this repository, as objdiff.json records it; <symbol> is the
function's symbol name (mangled for C++). The unit is compiled with its own command from the
target's build.ninja, from a scratch directory that mirrors the repository through links, so the
variant sits at the unit's own relative path: quoted includes and __FILE__ resolve exactly as in the
build, and nothing in the source tree or build/ is written. The object is compared with objdiff-cli's
one-shot diff against the unit's target object.

Exit status: 0 when the function is identical, 1 when it differs or is missing, 2 when the variant
does not compile. Output is one line, `<symbol> <percent>`, or JSON with --json.

Needs a configured and built target (configure.py, then ninja), so that build.ninja, objdiff.json,
the target objects and the tools exist."""
import argparse
import json
import math
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT=Path(__file__).resolve().parents[1]
DEFAULT_TARGET='GIHE78-rev0'


class Unit:
    """A unit of the target's objdiff.json: its target object, its built object and its source."""
    def __init__(self,target,source,root=ROOT):
        self.root=root
        self.directory=root/'build'/target
        config=json.loads((self.directory/'objdiff.json').read_text())
        source=os.path.abspath(source if os.path.isabs(source) else root/source)
        found=[u for u in config['units'] if u.get('metadata',{}).get('source_path')==source]
        if not found:raise ValueError('%s is not the source of any unit of %s'%(os.path.relpath(source,root),target))
        unit=found[0]
        if 'target_path' not in unit:raise ValueError('%s has no target object'%unit['name'])
        self.name=unit['name']
        self.target_path=unit['target_path']
        self.base=os.path.relpath(unit['base_path'],root)
        self.source=os.path.relpath(source,root)
        self._command=None

    def command(self):
        """The unit's compile command, as ninja would run it from the repository root."""
        if self._command is None:
            out=subprocess.run(['ninja','-f',str(self.directory/'build.ninja'),'-t','commands',self.base],
                               cwd=self.root,capture_output=True,text=True,check=True).stdout
            self._command=out.strip().split('\n')[-1]
        return self._command


def relocate(command,base,out):
    """`command` writing its object, dependency file and other outputs under `out` instead of the
    build directory that holds `base`."""
    directory=os.path.dirname(base)
    if directory not in command:raise ValueError('command does not write to %s'%directory)
    return command.replace(directory,out)


def mirror(root,source,scratch):
    """Links in `scratch` to every entry of `root` except along the path of `source`, where the
    directories are real and hold links to their other entries; returns where the variant goes."""
    parts=Path(source).parts
    here,there=Path(root),Path(scratch)
    for depth,part in enumerate(parts):
        for entry in os.listdir(here):
            if entry!=part:os.symlink(here/entry,there/entry)
        if depth<len(parts)-1:
            here,there=here/part,there/part
            there.mkdir()
    return there/parts[-1]


def compile_variant(unit,variant,scratch):
    """Compile `variant` as `unit` inside `scratch`; returns (object path, None) or (None, log)."""
    place=mirror(unit.root,unit.source,scratch)
    shutil.copyfile(variant,place)
    out=os.path.join(scratch,'.trial')
    os.makedirs(out)
    result=subprocess.run(relocate(unit.command(),unit.base,out),shell=True,cwd=scratch,capture_output=True,text=True)
    if result.returncode:return None,(result.stdout+result.stderr).strip()
    return os.path.join(out,os.path.basename(unit.base)),None


def objdiff_cli(root=ROOT):
    path=Path(root)/'build'/'tools'/'objdiff-cli'
    return str(path) if path.exists() else shutil.which('objdiff-cli') or str(path)


def compare(objdiff,target,base,symbol,strict=False):
    """(percent, [(target line, our line, kind)]) of `symbol` in the one-shot diff of the two
    objects. A function missing from either side scores 0. As in the progress report, a
    relocation that points at a differently named symbol does not count as a difference (literal
    pool entries are named @NNN in our objects and lbl_ in the target's) unless `strict`."""
    config=[] if strict else ['-c','functionRelocDiffs=none']
    result=subprocess.run([objdiff,'diff','-1',target,'-2',base,'-o','-','--format','json',*config,symbol],
                          capture_output=True,text=True,check=True)
    return score(json.loads(result.stdout),symbol)


def score(diff,symbol):
    """`diff` is objdiff-cli's JSON with the target object on the left. Symbols pair by name."""
    target={s['name']:s for s in diff['left']['symbols']}.get(symbol)
    ours={s['name']:s for s in diff['right']['symbols']}.get(symbol)
    if target is None or ours is None or target.get('target_symbol') is None:return 0.0,[]
    lines=[]
    for a,b in zip(target.get('instructions',[]),ours.get('instructions',[])):
        if 'diff_kind' in a or 'diff_kind' in b:
            lines.append((a.get('instruction',{}).get('formatted',''),b.get('instruction',{}).get('formatted',''),
                          a.get('diff_kind') or b.get('diff_kind')))
    return float(target.get('match_percent',0.0)),lines


def shown(percent):
    """`percent` to two places, rounded down so that only an identical function shows 100.00."""
    return '%.2f'%(math.floor(percent*100)/100)


def trial(unit,variant,symbol,objdiff=None,strict=False):
    """{'symbol', 'percent', 'compiled', 'log', 'diff'} for one variant."""
    with tempfile.TemporaryDirectory(prefix='trial-') as scratch:
        obj,log=compile_variant(unit,variant,scratch)
        if obj is None:return {'symbol':symbol,'percent':None,'compiled':False,'log':log,'diff':[]}
        percent,lines=compare(objdiff or objdiff_cli(unit.root),unit.target_path,obj,symbol,strict)
        return {'symbol':symbol,'percent':percent,'compiled':True,'log':None,'diff':lines}


def main(argv=None):
    parser=argparse.ArgumentParser(description=__doc__.split('\n')[0])
    parser.add_argument('variant',help='file to compile')
    parser.add_argument('symbol',help='function symbol to compare')
    parser.add_argument('--as',dest='source',help='unit source path the variant stands in for (default: the variant itself)')
    parser.add_argument('--target',default=DEFAULT_TARGET)
    parser.add_argument('--objdiff',help='objdiff-cli binary (default: build/tools/objdiff-cli)')
    parser.add_argument('--diff',action='store_true',help='list the differing instructions')
    parser.add_argument('--strict',action='store_true',help='also count relocations to differently named symbols')
    parser.add_argument('--json',action='store_true')
    args=parser.parse_args(argv)
    unit=Unit(args.target,args.source or args.variant)
    result=trial(unit,args.variant,args.symbol,args.objdiff,args.strict)
    if args.json:print(json.dumps(result))
    elif not result['compiled']:print(result['log'],file=sys.stderr)
    else:
        print('%s %s'%(args.symbol,shown(result['percent'])))
        for target,ours,kind in result['diff'] if args.diff else ():
            print('  %-40s %-40s %s'%(target,ours,kind))
    if not result['compiled']:return 2
    return 0 if result['percent']==100.0 else 1


if __name__=='__main__':
    sys.exit(main())
