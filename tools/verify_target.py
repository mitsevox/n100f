#!/usr/bin/env python3
"""Verify the private original against the explicit registry before configuration."""
import argparse
import hashlib
from pathlib import Path
try:
    from tools.targets import select
except ModuleNotFoundError:
    from targets import select


def verify(target_id,root=Path('.')):
    item=select(target_id)
    if item['status']!='active':raise ValueError('Target is not active')
    original=root/'orig'/target_id/'sys/main.dol'
    if not original.is_file() or original.is_symlink():raise ValueError('Missing/unsafe target original')
    if hashlib.sha1(original.read_bytes()).hexdigest()!=item['sha1']:raise ValueError('Original executable SHA-1 differs')
    return item


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--target',required=True)
    print(verify(parser.parse_args().target)['id']+': original SHA-1 verified')
