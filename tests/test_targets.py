"""Target identity, dependency scope and target-bound history invariants."""
import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from tools.targets import registry,select,build_targets
from tools.ci_targets import affected
from tools import public_report

ROOT=Path(__file__).resolve().parents[1]

class Targets(unittest.TestCase):
    def test_revision_identity_and_archived_support(self):
        value=registry()
        self.assertEqual(build_targets(value),['GIHE78-rev0'])
        self.assertEqual(select('GIHE78-rev1')['support'],{'build':False,'reportFormats':[]})
        with self.assertRaises(ValueError):select('GIHE78')
        self.assertEqual(public_report.target('GIHE78-rev0')['id'],'GIHE78-rev0')
        with self.assertRaises(SystemExit):public_report.target('GIHE78-rev1')

    def test_target_specific_shared_archived_unknown_and_deleted_inputs(self):
        value=registry();deps=json.loads((ROOT/'config/ci-targets.json').read_text())
        # Model a second independently buildable revision without asserting a real build.
        value['targets'][1]['support']={'build':True,'reportFormats':['report-v2']}
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

    def test_timeline_rejects_other_revision_but_accepts_explicit_legacy_alias(self):
        item=select('GIHE78-rev0');payload=json.dumps({'snapshots':[{'revision':'a'*40}]}).encode()
        module={'status':'supported','asset':{'path':'timeline.json','sha256':public_report.sha256(payload)}}
        manifest={'schemaVersion':1,'project':'n100f','repository':'mitsevox/n100f','target':item['legacyTargets'][0],'modules':{'timeline':module}}
        read=lambda path:payload
        self.assertEqual(public_report.package_timeline(json.dumps(manifest),read,'fixture'),[{'revision':'a'*40}])
        manifest['target']=select('GIHE78-rev1')['target'];manifest['schemaVersion']=2
        with self.assertRaises(SystemExit):public_report.package_timeline(json.dumps(manifest),read,'fixture')

    def test_no_synthetic_history_when_fetch_fails(self):
        with patch.object(public_report,'site_timelines',side_effect=RuntimeError('network')):
            with self.assertRaises(RuntimeError):public_report.timeline({},'2026-10-27T00:00:00Z')

    def test_new_revision_ignores_flat_rev0_bootstrap_history(self):
        old={'schemaVersion':1,'project':'n100f','repository':'mitsevox/n100f',
             'target':select('GIHE78-rev0')['legacyTargets'][0],
             'modules':{'timeline':{'status':'unsupported','reason':'fixture'}}}
        raw=json.dumps(old).encode();entry={'path':'data/n100f/snapshots/old','manifestSha256':public_report.sha256(raw)}
        def fetch(url,accept):
            if 'reports.lock.json' in url:return json.dumps({'schemaVersion':1,'projects':{'n100f':entry}}).encode()
            return raw
        with patch.object(public_report,'fetch',side_effect=fetch):
            self.assertEqual(public_report.site_timelines('GIHE78-rev1'),[])

    def test_flat_bootstrap_unknown_identity_and_wrong_repo_fail_closed(self):
        for update in ({'target':{'id':'UNKNOWN','console':'GameCube','sha1':'a'*40}},{'repository':'someone/else'}):
            old={'schemaVersion':1,'project':'n100f','repository':'mitsevox/n100f',
                 'target':select('GIHE78-rev0')['legacyTargets'][0],**update}
            raw=json.dumps(old).encode();entry={'path':'old','manifestSha256':public_report.sha256(raw)}
            def fetch(url,accept):return json.dumps({'projects':{'n100f':entry}}).encode() if 'reports.lock' in url else raw
            with self.subTest(update=update),patch.object(public_report,'fetch',side_effect=fetch),self.assertRaises(SystemExit):
                public_report.site_timelines('GIHE78-rev1')

    def test_pagination_never_silently_drops_jobs_or_artifacts(self):
        calls=[]
        def fetch(url,accept):
            calls.append(url)
            return json.dumps({'jobs':[{'id':i} for i in range(100)] if url.endswith('page=1') else [{'id':100}]}).encode()
        with patch.object(public_report,'fetch',side_effect=fetch):
            self.assertEqual(len(list(public_report.api_rows('https://example.test/jobs','jobs'))),101)
            self.assertEqual(len(calls),2)

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
