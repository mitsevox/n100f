#!/usr/bin/env python3
"""Print the tools that measure each report, one name=identity per line, for the site's provenance:
the digest-pinned build image from the BUILD_IMAGE environment variable (validate.yml exports the
image it runs in) and the tool tags pinned in configure.py. validate.yml passes this output to the
publish action's `tools` input. The build image carries dtk with this project's local patch, so
the dtk line names the patch by its SHA-256."""
import hashlib
import os
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIGURE = ROOT / 'configure.py'
IMAGE = re.compile(r'ghcr\.io/[^@\s]+@sha256:[0-9a-f]{64}')
# PARAMETER: the `config.<name> = "<version>"` lines of configure.py to report, as name=version.
TAGS = {'dtk': 'dtk_tag', 'objdiff': 'objdiff_tag'}
DTK_PATCH = ROOT / 'tools/patches/dtk-v1.8.3-rom-copy-info.patch'


def tools(image=None, configure=None):
    image = image if image is not None else os.environ.get('BUILD_IMAGE', '')
    if not IMAGE.fullmatch(image):
        raise SystemExit('measuring_tools: BUILD_IMAGE is not a digest-pinned image reference')
    configure = configure if configure is not None else CONFIGURE.read_text()
    result = [('build-environment', image)]
    for name, tag in TAGS.items():
        m = re.search(r'^config\.%s = "([^"]+)"$' % tag, configure, re.M)
        if not m:
            raise SystemExit('measuring_tools: configure.py has no %s' % tag)
        version = m.group(1)
        if name == 'dtk':
            version += ' + rom-copy-info patch sha256:' + hashlib.sha256(DTK_PATCH.read_bytes()).hexdigest()
        result.append((name, version))
    return result


if __name__ == '__main__':
    print('\n'.join('%s=%s' % pair for pair in tools()))
