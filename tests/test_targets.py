"""Target identity and CI dependency scope."""
import copy
import json
from pathlib import Path
import tempfile
import unittest
from tools.targets import registry,select,build_targets
from tools.ci_targets import affected

ROOT=Path(__file__).resolve().parents[1]

class Targets(unittest.TestCase):
    def test_revision_identity_and_archived_support(self):
        value=registry()
        self.assertEqual(build_targets(value),['GIHE78-rev0'])
        self.assertEqual(select('GIHE78-rev1')['status'],'archived')
        with self.assertRaises(ValueError):select('GIHE78')
        for change in ({'status':'live'},{'platform':'GameCube'},{'sha1':'A'*40},{'extra':1}):
            broken=copy.deepcopy(value);broken['targets'][0].update(change)
            with self.subTest(change=change),tempfile.NamedTemporaryFile('w',suffix='.json') as f:
                json.dump(broken,f);f.flush()
                with self.assertRaises(ValueError):registry(f.name)

    def test_target_specific_shared_archived_unknown_and_deleted_inputs(self):
        value=registry();deps=json.loads((ROOT/'config/ci-targets.json').read_text())
        # Model a second independently buildable revision without asserting a real build.
        value['targets'][1]['status']='active'
        for paths,expected in [(['config/GIHE78-rev0/splits.txt'],['GIHE78-rev0']),
                               (['config/GIHE78-rev1/symbols.txt'],['GIHE78-rev1']),
                               (['src/game.cpp'],['GIHE78-rev0','GIHE78-rev1']),
                               (['unclassified/tool'],['GIHE78-rev0','GIHE78-rev1']),
                               (['docs/guide.md'],[]),
                               (['config/GIHE78-rev0/deleted.txt','config/GIHE78-rev1/renamed.txt'],['GIHE78-rev0','GIHE78-rev1'])]:
            with self.subTest(paths=paths):self.assertEqual(affected(paths,value,deps),expected)
        self.assertEqual(affected(['config/GIHE78-rev1/README.md'],registry(),deps),[])
        self.assertEqual(affected([],value,deps,force=True),['GIHE78-rev0','GIHE78-rev1'])
        with self.assertRaises(ValueError):affected(['../escape'],value,deps)

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
