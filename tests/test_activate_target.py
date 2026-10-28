"""The local objdiff view selector (tools/activate_target.py)."""
import json
from pathlib import Path
import tempfile
import unittest


class ActivateTarget(unittest.TestCase):
    def test_activation_replaces_binding_symlinks_and_refreshes_only_selected_target(self):
        from tools.activate_target import activate,refresh_active
        with tempfile.TemporaryDirectory() as temp:
            root=Path(temp);folder=root/'build/GIHE78-rev0';folder.mkdir(parents=True)
            command=['-C',str(root),'-f',str(folder/'build.ninja')]
            (folder/'objdiff.json').write_text(json.dumps({'custom_args':command,'units':[1]}))
            old=root/'old-view.json';old.write_text('{"original":true}')
            (root/'objdiff.json').symlink_to(old)
            activate('GIHE78-rev0',root)
            self.assertEqual(old.read_text(),'{"original":true}')
            self.assertFalse((root/'objdiff.json').is_symlink())
            (folder/'objdiff.json').write_text(json.dumps({'custom_args':command,'units':[2]}))
            refresh_active('GIHE78-rev0',root)
            self.assertEqual(json.loads((root/'objdiff.json').read_text())['units'],[2])

    def test_root_objdiff_build_command_matches_real_ninja_outputs(self):
        import shutil,subprocess
        ninja=shutil.which('ninja')
        if not ninja:self.skipTest('Ninja required for local GUI build-command integration')
        from tools.activate_target import activate
        with tempfile.TemporaryDirectory() as temp:
            root=Path(temp);folder=root/'build/GIHE78-rev0';folder.mkdir(parents=True)
            # A tiny real Ninja graph, no original game bytes/toolchain required.
            (folder/'build.ninja').write_text('rule create\n  command = mkdir -p build/GIHE78-rev0/src && echo built > $out\nbuild build/GIHE78-rev0/src/test.o: create\n')
            config={'custom_make':ninja,'custom_args':['-C',str(root),'-f',str(folder/'build.ninja')]}
            (folder/'objdiff.json').write_text(json.dumps(config));activate('GIHE78-rev0',root)
            active=json.loads((root/'objdiff.json').read_text())
            subprocess.run([active['custom_make'],*active['custom_args'],'build/GIHE78-rev0/src/test.o'],cwd=root,check=True,capture_output=True)
            self.assertEqual((folder/'src/test.o').read_text().strip(),'built')
