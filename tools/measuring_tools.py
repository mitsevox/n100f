#!/usr/bin/env python3
"""Print the tools that measure each report, one name=identity per line, for the site's provenance:
the pinned build image (binutils, compilers and the patched dtk), dtk with its local patch, and
objdiff, which configure.py downloads at its pinned tag."""
import hashlib
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WORKFLOW = ROOT / '.github/workflows/validate.yml'
DTK_PATCH = ROOT / 'tools/patches/dtk-v1.8.3-rom-copy-info.patch'


def tools():
    image = re.findall(r'^\s*image: (ghcr\.io/mitsevox/n100f-build@sha256:[0-9a-f]{64})$', WORKFLOW.read_text(), re.M)
    if len(image) != 1:
        raise SystemExit('measuring_tools: validate.yml names no single pinned build image')
    configure = (ROOT / 'configure.py').read_text()
    tags = {}
    for tag in ('dtk_tag', 'objdiff_tag'):
        m = re.search(r'^config\.%s = "([^"]+)"$' % tag, configure, re.M)
        if not m:
            raise SystemExit('measuring_tools: configure.py has no %s' % tag)
        tags[tag] = m.group(1)
    patch = hashlib.sha256(DTK_PATCH.read_bytes()).hexdigest()
    return [('build-environment', image[0]),
            ('dtk', '%s + rom-copy-info patch sha256:%s' % (tags['dtk_tag'], patch)),
            ('objdiff', tags['objdiff_tag'])]


if __name__ == '__main__':
    print('\n'.join('%s=%s' % pair for pair in tools()))
