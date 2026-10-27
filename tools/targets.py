"""Explicit target inventory; no inferred revision aliases or filesystem discovery."""
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]


def registry(path=None):
    value=json.loads(Path(path or ROOT/'config/targets.json').read_text())
    if set(value)!={'schemaVersion','project','defaultTarget','targets'} or value['schemaVersion']!=1 or value['project']!='n100f':
        raise ValueError('Invalid target registry')
    ids=set();identities=set()
    for item in value['targets']:
        if set(item)!={'target','status','support','legacyTargets'}:raise ValueError('Unknown target fields')
        target=item['target'];support=item['support']
        if set(target)!={'id','platform','gameId','region','discRevision','sha1'}:raise ValueError('Invalid target identity')
        if not (target['platform']=='gamecube' and re.fullmatch(r'[A-Z0-9]{6}',target['gameId'])
                and type(target['discRevision']) is int and 0<=target['discRevision']<=255
                and target['id']==target['gameId']+'-rev'+str(target['discRevision'])
                and isinstance(target['region'],str) and target['region'].strip()
                and re.fullmatch(r'[a-f0-9]{40}',target['sha1'])):raise ValueError('Invalid GameCube target')
        identity=(target['gameId'],target['discRevision'])
        if target['id'] in ids or identity in identities:raise ValueError('Duplicate target')
        ids.add(target['id']);identities.add(identity)
        if item['status'] not in ('active','archived') or set(support)!={'build','reportFormats'} or type(support['build']) is not bool:
            raise ValueError('Invalid target support')
        if not isinstance(support['reportFormats'],list) or any(x!='report-v2' for x in support['reportFormats']) or len(set(support['reportFormats']))!=len(support['reportFormats']):raise ValueError('Unknown report support')
        if support['reportFormats'] and not support['build']:raise ValueError('Report requires verified build support')
        for old in item['legacyTargets']:
            if set(old)!={'id','console','sha1'} or old['sha1']!=target['sha1']:raise ValueError('Invalid legacy identity')
    if value['defaultTarget'] not in build_targets(value):raise ValueError('Default target is not build-supported')
    return value


def build_targets(value=None):
    value=value or registry()
    return [item['target']['id'] for item in value['targets'] if item['support']['build']]


def select(target_id,value=None):
    value=value or registry()
    matches=[item for item in value['targets'] if item['target']['id']==target_id]
    if len(matches)!=1:raise ValueError('Unknown explicit target: '+target_id)
    return matches[0]
