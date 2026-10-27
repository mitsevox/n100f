#!/usr/bin/env python3
"""Select a local objdiff/editor view; canonical generated outputs stay target-local.

objdiff v3 watches only its project directory. A root workspace view is therefore
needed to watch shared source; CLI measurement still reads the target-local config.
This command never builds or changes a target identity/report.
"""
import argparse
import json
import os
import tempfile
from pathlib import Path
try:
    from tools.targets import select
except ModuleNotFoundError:
    from targets import select

ROOT=Path(__file__).resolve().parents[1]


def activate(target_id,root=ROOT):
    if not select(target_id)['support']['build']:raise ValueError('Archived target has no build workspace')
    directory=root/'build'/target_id
    objdiff=directory/'objdiff.json'
    value=json.loads(objdiff.read_text())
    expected=['-C',str(root),'-f',str(directory/'build.ninja')]
    if value.get('custom_args')!=expected:raise ValueError('Generated objdiff configuration belongs to another workspace')
    # Ignored workspace bindings, not duplicated authored configuration. The
    # compiler command paths were generated for the root checkout already.
    for name in ('objdiff.json','compile_commands.json'):
        source=directory/name;destination=root/name
        if source.is_file():
            with tempfile.NamedTemporaryFile(dir=root,delete=False) as out:
                out.write(source.read_bytes());name=out.name
            os.replace(name,destination)
    return directory


def refresh_active(target_id,root=ROOT):
    path=root/'objdiff.json'
    if not path.is_file():return
    try:value=json.loads(path.read_text())
    except (ValueError,OSError):return
    expected=['-C',str(root),'-f',str(root/'build'/target_id/'build.ninja')]
    if value.get('custom_args')==expected:activate(target_id,root)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--target',required=True)
    print(activate(parser.parse_args().target))
