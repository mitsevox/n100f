#!/usr/bin/env python3
"""Public report-v2 inputs for the shared site (https://mitsevox.github.io/), adapted from mvp2005.

inputs    objdiff's report.json -> summary.json (the measured code/data bytes and the function
          inventory), map.json (every function as a tile, by unit and by section) and
          timeline.json (the earlier main runs' rows plus this one), the site's report-v1 inputs
          (docs/public_report.md).
metadata  the execution facts the site's packager binds the report to: this checkout's commit
          and tree, the workflow file's Git blob, the pinned build image and tools, the Actions
          run and attempt, and the measurement and publication times.

Reads only public outputs (this repo's and the site's published reports); writes no game bytes. Standard library only.
"""
import argparse
import datetime
import hashlib
import io
import json
import math
import os
import re
import subprocess
import sys
import urllib.error
import urllib.request
import zipfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PROJECT = 'n100f'
CONSOLE = 'GameCube'
WORKFLOW = '.github/workflows/validate.yml'
LEGACY_ARTIFACT = 'public-report'
try:
    from tools.targets import registry,select
except ModuleNotFoundError:
    from targets import registry,select
DEFAULT_TARGET='GIHE78-rev0'
# Bump when what a number means changes (the site keeps old rows under their own methodology).
METHODOLOGY = 'objdiff-report-v1'
GENERATION = 1
FUNCTION_BASIS = 'objdiff-functions; named = symbol name other than an fn_ address label'
FILE_BASIS = ('objdiff-units, provisional boundaries (not yet checked against Heavy Iron objects); '
              'named = not a dtk auto-generated address range; exact = named, all code and data bytes matched')
DTK_PATCH = 'tools/patches/dtk-v1.8.3-rom-copy-info.patch'


def fail(message):
    sys.exit('public_report: ' + message)


def num(value):
    # objdiff writes 64-bit counts as strings and leaves out any measure that is zero.
    return int(value) if value is not None else 0


def fuzzy_bytes(function):
    # Size-weighted similarity; the summary and the map use this same expression so they agree.
    return num(function.get('size')) * min(function.get('fuzzy_match_percent', 0), 100) / 100


CODE_SECTIONS = ('.init', '.text')


def data_fuzzy_bytes(unit):
    # objdiff byte-diffs each data and bss section and reports its match percent per section
    # (it rolls only the 100% sections up into matched_data). Size-weighted, as for functions.
    return math.fsum(num(s.get('size')) * min(s.get('fuzzy_match_percent', 0), 100) / 100
                     for s in unit.get('sections', []) if s['name'] not in CODE_SECTIONS)


def summary(report):
    m = report.get('measures', {})
    total_code, total_data = num(m.get('total_code')), num(m.get('total_data'))
    functions = [f for u in report.get('units', []) for f in u.get('functions', [])]
    if len(functions) != num(m.get('total_functions')):
        fail('report lists %d functions, its measures say %s' % (len(functions), m.get('total_functions')))
    exact = sum(1 for f in functions if f.get('fuzzy_match_percent', 0) >= 100)
    if exact != num(m.get('matched_functions')):
        fail('report has %d exact functions, its measures say %s' % (exact, m.get('matched_functions')))
    named = sum(1 for f in functions if not f['name'].startswith('fn_'))
    # Fuzzy coverage weighted by function size; an exact function counts its whole size once.
    fuzzy = math.fsum(fuzzy_bytes(f) for f in functions)
    matched_code = num(m.get('matched_code'))
    if not matched_code <= fuzzy <= total_code:
        fail('fuzzy code bytes %.1f outside matched %d .. total %d' % (fuzzy, matched_code, total_code))
    data_fuzzy = math.fsum(data_fuzzy_bytes(u) for u in report.get('units', []))
    matched_data = num(m.get('matched_data'))
    if not matched_data <= data_fuzzy <= total_data:
        fail('fuzzy data bytes %.1f outside matched %d .. total %d' % (data_fuzzy, matched_data, total_data))
    return {
        'code': {
            'totalBytes': total_code,
            'linkedBytes': num(m.get('complete_code')),
            'matchedBytes': matched_code,
            'fuzzyEquivalentBytes': fuzzy,
        },
        'data': {
            'totalBytes': total_data,
            'linkedBytes': num(m.get('complete_data')),
            'matchedBytes': matched_data,
            'fuzzyEquivalentBytes': data_fuzzy,
        },
        'functions': {'total': len(functions), 'exact': exact, 'named': named, 'basis': FUNCTION_BASIS},
        'files': files(report),
    }


def files(report):
    """Units counted as files, provisionally: the game code still sits in dtk's auto-generated
    address ranges, not Heavy Iron's objects, until the unit boundaries are checked."""
    units = report.get('units', [])

    def auto(u):
        return u.get('metadata', {}).get('auto_generated', False)

    def exact(u):
        m = u.get('measures', {})
        return all(num(m.get('matched_' + k)) == num(m.get('total_' + k)) for k in ('code', 'data'))
    return {'total': len(units),
            'exact': sum(1 for u in units if exact(u) and not auto(u)),
            'named': sum(1 for u in units if not auto(u)),
            'basis': FILE_BASIS}


def hexaddr(value):
    return '0x%08X' % value


def merged(ranges):
    result = []
    for start, end in sorted(ranges):
        if result and start <= result[-1][1]:
            result[-1][1] = max(result[-1][1], end)
        else:
            result.append([start, end])
    return result


def extents(ranges):
    return [{'start': hexaddr(a), 'end': hexaddr(b)} for a, b in merged(ranges)]


def group(name, kind, children):
    # A group's measures are its children's sums; its extents are their merged ranges.
    node = {'name': name, 'kind': kind,
            'size': sum(c['size'] for c in children),
            'linked': sum(c['linked'] for c in children),
            'matched': sum(c['matched'] for c in children),
            'fuzzy': math.fsum(c['fuzzy'] for c in children)}
    node['_ranges'] = [tuple(r) for r in merged(r for c in children for r in c['_ranges'])]
    node['mapped_extents'] = extents(node['_ranges'])
    node['children'] = children
    return node


def map_document(report):
    """The site's map-v1 payload: every function as a tile, grouped by source unit (`files`, what
    the page draws) and by DOL section (`sections`, a cross-check over the same bytes).

    objdiff rolls exact data matches up per unit (matched_data), so a unit's data is one tile
    spanning its data sections, with fuzzy bytes summed over them, and the `sections` tree holds
    data as one group of those tiles."""
    code_by_section = {name: [] for name in CODE_SECTIONS}
    files, data_tiles = [], []
    for unit in report.get('units', []):
        m, md = unit.get('measures', {}), unit.get('metadata', {})
        name = unit['name'][len('main/'):] if unit['name'].startswith('main/') else unit['name']
        complete = bool(md.get('complete'))
        sections = [(s['name'], int(s['metadata']['virtual_address']), num(s.get('size')))
                    for s in unit.get('sections', []) if num(s.get('size'))]
        functions = []
        for f in unit.get('functions', []):
            start, size = int(f['metadata']['virtual_address']), num(f.get('size'))
            exact = f.get('fuzzy_match_percent', 0) >= 100
            leaf = {'name': f['name'], 'kind': 'code', 'type': 'function',
                    'address': hexaddr(start), 'size': size,
                    'linked': size if complete else 0, 'matched': size if exact else 0,
                    'fuzzy': fuzzy_bytes(f),
                    'function_address': hexaddr(start), 'original_size': size,
                    '_ranges': [(start, start + size)]}
            functions.append(leaf)
            section = next(s for s, a, n in sections if s in CODE_SECTIONS and a <= start < a + n)
            code_by_section[section].append(dict(leaf))
        if functions:
            files.append(group(name, 'code', functions))
        data = [(a, a + n) for s, a, n in sections if s not in CODE_SECTIONS]
        total_data = num(m.get('total_data'))
        if total_data:
            if sum(b - a for a, b in data) != total_data:
                fail('%s: data sections do not add up to total_data' % name)
            tile = {'name': name, 'kind': 'data', 'size': total_data,
                    'linked': total_data if complete else 0, 'matched': num(m.get('matched_data')),
                    'fuzzy': data_fuzzy_bytes(unit),
                    'mapped_extents': extents(data), '_ranges': data}
            files.append(tile)
            data_tiles.append(dict(tile))
    # The page draws tiles in list order: keep them in address order, as in the DOL.
    files.sort(key=lambda f: min(f['_ranges']))
    sections = [group(s, 'code', sorted(fs, key=lambda f: f['_ranges'][0])) for s, fs in code_by_section.items() if fs]
    sections.append(group('Data sections', 'data', sorted(data_tiles, key=lambda t: t['_ranges'][0])))

    def strip(node):
        node.pop('_ranges', None)
        for child in node.get('children', []):
            strip(child)
        return node
    return {'schemaVersion': 1, 'sections': [strip(s) for s in sections], 'files': [strip(f) for f in files]}


# The timeline (timeline-v1). Each main run restores the timeline of the last successful main run
# before it and appends its own row. Main runs queue (build.yml's concurrency group), so every
# row any earlier run published is carried, including rows the site has not imported yet.
SITE_API = 'https://api.github.com/repos/mitsevox/mitsevox.github.io/contents/'
# The site's durable store, then its main branch: each holds a lock naming its accepted package.
SITE_BRANCHES = ('report-data', 'main')


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, *args):
        return None


def fetch(url, accept, token=True):
    request = urllib.request.Request(url, headers={'Accept': accept})
    if token and os.environ.get('GH_TOKEN'):
        request.add_header('Authorization', 'Bearer ' + os.environ['GH_TOKEN'])
    with urllib.request.urlopen(request, timeout=60) as response:
        return response.read()


def sha256(raw):
    return hashlib.sha256(raw).hexdigest()


def package_timeline(manifest_raw, read, where,version=DEFAULT_TARGET):
    """The timeline rows of one package, its asset checked against its manifest; None when the
    package has no timeline."""
    manifest=json.loads(manifest_raw)
    item=select(version)
    if manifest.get('project')!=PROJECT or manifest.get('repository')!='mitsevox/n100f' or not (
            manifest.get('schemaVersion')==2 and manifest.get('target')==item['target']
            or manifest.get('schemaVersion')==1 and manifest.get('target') in item['legacyTargets']):
        fail('%s: timeline belongs to another target' % where)
    module=manifest['modules']['timeline']
    if module['status'] != 'supported':
        return None
    raw = read(module['asset']['path'])
    if sha256(raw) != module['asset']['sha256']:
        fail('%s: timeline differs from its manifest' % where)
    return json.loads(raw)['snapshots']


def site_timelines(version=DEFAULT_TARGET):
    """The timelines the site holds as accepted, from each branch of its store."""
    result = []
    for branch in SITE_BRANCHES:
        def read(path):
            return fetch(SITE_API + path + '?ref=' + branch, 'application/vnd.github.raw')
        entry = json.loads(read('data/reports.lock.json'))['projects'].get(PROJECT)
        if not entry:
            continue
        legacy_flat='targets' not in entry
        if not legacy_flat:entry=entry['targets'].get(version)
        if not entry:continue
        raw = read(entry['path'] + '/manifest.json')
        if sha256(raw) != entry['manifestSha256']:
            fail('site %s: accepted manifest differs from its lock' % branch)
        manifest=json.loads(raw);item=select(version)
        if legacy_flat:
            if manifest.get('project')!=PROJECT or manifest.get('repository')!='mitsevox/n100f':
                fail('site %s: accepted legacy identity belongs to another repository' % branch)
            known=any(manifest.get('target')==candidate['target'] or manifest.get('target') in candidate['legacyTargets']
                      for candidate in registry()['targets'])
            if not known:fail('site %s: accepted legacy target is unknown' % branch)
            if manifest.get('target')!=item['target'] and manifest.get('target') not in item['legacyTargets']:
                # A known other revision in a checked-in bootstrap is not this
                # target's history. Unknown/corrupt identities still fail closed.
                continue
        rows = package_timeline(raw, lambda path: read(entry['path'] + '/' + path), 'site ' + branch,version)
        if rows is not None:
            result.append(rows)
    return result


def api_rows(url,field):
    """Do not interpret a truncated job/artifact response as missing history."""
    for page in range(1,11):
        value=json.loads(fetch(url+('&' if '?' in url else '?')+'per_page=100&page='+str(page),'application/vnd.github+json'))
        batch=value[field]
        if not isinstance(batch,list) or len(batch)>100:fail('invalid paginated '+field)
        yield from batch
        if len(batch)<100:return
    fail(field+' exceeds retained scan bound')


def previous_run(version=DEFAULT_TARGET):
    """Restore this target only, including successful jobs in partially failed runs."""
    repository,run_id=os.environ.get('GITHUB_REPOSITORY'),os.environ.get('GITHUB_RUN_ID')
    if not (repository and run_id):return None
    item=select(version)
    api='%s/repos/%s/actions/' % (os.environ.get('GITHUB_API_URL','https://api.github.com'),repository)
    names=['report-v2-'+version]+([LEGACY_ARTIFACT] if item['legacyTargets'] else [])
    for page in range(1,11):
        runs=json.loads(fetch(api+'workflows/%s/runs?branch=main&status=completed&per_page=100&page=%d' %
                              (os.path.basename(WORKFLOW),page),'application/vnd.github+json'))['workflow_runs']
        for run in runs:
            if run['event'] not in ('push','workflow_dispatch') or str(run['id'])==run_id or run['conclusion'] not in ('success','failure'):continue
            artifacts=list(api_rows(api+'runs/%d/artifacts' % run['id'],'artifacts'))
            matches=[a for a in artifacts if a['name'] in names]
            if not matches:continue
            if len(matches)!=1:fail('run %d: ambiguous target history artifacts' % run['id'])
            artifact=matches[0]
            job_name='Validate '+version if artifact['name'].startswith('report-v2-') else 'Site report'
            jobs=list(api_rows(api+'runs/%d/attempts/%d/jobs' % (run['id'],run['run_attempt']),'jobs'))
            jobs=[j for j in jobs if j['name']==job_name and j['conclusion']=='success' and j['run_attempt']==run['run_attempt']]
            if len(jobs)!=1:continue
            if artifact['expired']:return 'expired'
            try:
                urllib.request.build_opener(NoRedirect).open(urllib.request.Request(api+'artifacts/%d/zip' % artifact['id'],
                    headers={'Authorization':'Bearer '+os.environ.get('GH_TOKEN','')}),timeout=60)
                fail('run %d: artifact download did not redirect' % run['id'])
            except urllib.error.HTTPError as redirect:
                if redirect.code!=302:raise
                location=redirect.headers['Location']
            return run['id'],zipfile.ZipFile(io.BytesIO(fetch(location,'*/*',token=False)))
        if len(runs)<100:return None
    fail('target history exceeds retained scan bound; restore an accepted checkpoint explicitly')


def timeline(summary_doc, measured_at,version=DEFAULT_TARGET):
    """Earlier rows unchanged, then this commit's measured row. A fetch failure stops the build:
    a timeline restarted from nothing would drop rows the site already published. It starts
    empty only when no earlier main run published a report and the site holds none."""
    revision = git('rev-parse', 'HEAD')
    accepted = site_timelines(version)
    previous = previous_run(version)
    rows = None
    if isinstance(previous, tuple):
        run_id, files = previous
        rows = package_timeline(files.read('manifest.json'), files.read, 'run %d' % run_id,version)
    if rows is None and accepted:
        rows = max(accepted, key=len)
    if rows is None:
        if previous == 'expired':
            fail('the last main report has expired and the site holds none; restore the timeline first')
        rows = []
    if not any(r['revision'] == revision for r in rows):
        rows.append({'revision': revision, 'built_at': measured_at,
                     'code': {k: summary_doc['code'][f] for k, f in (('total', 'totalBytes'), ('linked', 'linkedBytes'))},
                     'data': {k: summary_doc['data'][f] for k, f in (('total', 'totalBytes'), ('linked', 'linkedBytes'))},
                     'provenance': {'kind': 'measured',
                                    'source': 'validate.yml run %s' % os.environ.get('GITHUB_RUN_ID', '(local build)')}})
    # The site rejects a timeline that drops or changes a row it accepted; fail here first.
    held = {r['revision']: r for r in rows}
    for old in (r for t in accepted for r in t):
        if held.get(old['revision']) != old:
            fail('timeline lost or changed the row the site accepted for %s' % old['revision'])
    return {'schemaVersion': 1, 'snapshots': rows}


def git(*args):
    # Container checkout ownership can differ from the runtime user. Trust
    # only this checkout for this command; never a global wildcard exception.
    return subprocess.check_output(['git','-c','safe.directory='+ROOT,'-C',ROOT,*args],text=True).strip()


def tools():
    """The pinned build image (it holds binutils, the compilers and the patched dtk), dtk with
    its local patch, and objdiff, which configure.py downloads at its pinned tag."""
    workflow = open(os.path.join(ROOT, WORKFLOW)).read()
    image = re.findall(r'^\s*image: (ghcr\.io/mitsevox/n100f-build@sha256:[0-9a-f]{64})$', workflow, re.M)
    if len(image) != 1:
        fail('%s names no single pinned build image' % WORKFLOW)
    configure = open(os.path.join(ROOT, 'configure.py')).read()
    tags = {}
    for tag in ('dtk_tag', 'objdiff_tag'):
        m = re.search(r'^config\.%s = "([^"]+)"$' % tag, configure, re.M)
        if not m:
            fail('configure.py has no %s' % tag)
        tags[tag] = m.group(1)
    patch = sha256(open(os.path.join(ROOT, DTK_PATCH), 'rb').read())
    return [{'name': 'build-environment', 'identity': image[0]},
            {'name': 'dtk', 'identity': '%s + rom-copy-info patch sha256:%s' % (tags['dtk_tag'], patch)},
            {'name': 'objdiff', 'identity': tags['objdiff_tag']}]


def target(version):
    item=select(version)
    if not item['support']['build'] or 'report-v2' not in item['support']['reportFormats']:fail('target has no verified report support')
    line=open(os.path.join(ROOT,'config',version,'build.sha1')).read().split()
    if line!=[item['target']['sha1'],'build/'+version+'/main.dol']:fail('target checksum configuration differs from registry')
    return item['target']


def now():
    return datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')


def metadata(version, measured_at):
    if git('status', '--porcelain', '--untracked-files=no'):
        fail('the checkout has local changes; a report must describe a commit')
    env = {k: os.environ.get(k) for k in ('GITHUB_REPOSITORY', 'GITHUB_RUN_ID', 'GITHUB_RUN_ATTEMPT')}
    if not all(env.values()):
        fail('needs the GitHub Actions run (GITHUB_REPOSITORY, GITHUB_RUN_ID, GITHUB_RUN_ATTEMPT)')
    return {
        'project': PROJECT,
        'repository': env['GITHUB_REPOSITORY'],
        'target': target(version),
        'revision': git('rev-parse', 'HEAD'),
        'tree': git('rev-parse', 'HEAD^{tree}'),
        'methodology': METHODOLOGY,
        'generation': GENERATION,
        'measuredAt': measured_at,
        'publishedAt': now(),
        'tools': tools(),
        'runId': int(env['GITHUB_RUN_ID']),
        'attempt': int(env['GITHUB_RUN_ATTEMPT']),
        'workflowBlob': git('rev-parse', 'HEAD:' + WORKFLOW),
        'mode': 'full',
        'source': None,
    }


def write(path, value):
    with open(path, 'w') as f:
        json.dump(value, f, indent=2, sort_keys=True)
        f.write('\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='command', required=True)
    s = sub.add_parser('inputs', help='write summary.json, map.json and timeline.json into a new public-input folder')
    s.add_argument('--report', required=True, help="objdiff's report.json")
    s.add_argument('--measured-at', required=True)
    s.add_argument('--version',default=DEFAULT_TARGET)
    s.add_argument('--output', required=True, help='folder to create')
    sub.add_parser('now', help='print the current UTC time (the measurement time, right after ninja)')
    md = sub.add_parser('metadata', help="write the site packager's execution metadata")
    md.add_argument('--version',default=DEFAULT_TARGET)
    md.add_argument('--measured-at', required=True)
    md.add_argument('--output', required=True)
    args = parser.parse_args()
    if args.command == 'now':
        print(now())
    elif args.command == 'inputs':
        target(args.version)
        report = json.load(open(args.report))
        os.makedirs(args.output)
        summary_doc = summary(report)
        write(os.path.join(args.output, 'summary.json'), summary_doc)
        write(os.path.join(args.output, 'timeline.json'), timeline(summary_doc,args.measured_at,args.version))
        # Thousands of function tiles: written compact, unlike the small files above.
        with open(os.path.join(args.output, 'map.json'), 'w') as f:
            json.dump(map_document(report), f, separators=(',', ':'), sort_keys=True)
    else:
        write(args.output, metadata(args.version, args.measured_at))


if __name__ == '__main__':
    main()
