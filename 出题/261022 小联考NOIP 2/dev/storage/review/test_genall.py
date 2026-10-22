#!/usr/bin/env python3
"""Smoke-test genall defaults, arbitrary cwd, spaced paths and unrelated files."""
import argparse, json, pathlib, shutil, subprocess, tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
ap=argparse.ArgumentParser(description=__doc__)
ap.add_argument('--mounted-parent',type=pathlib.Path,help='optional directory on another filesystem, e.g. /mnt/d/.../work')
args=ap.parse_args()
cross_filesystem=False
with tempfile.TemporaryDirectory(prefix='.path-test.',dir=str(ROOT)) as temp:
    base=pathlib.Path(temp)
    fixture=base/'contest'/'dev'/'storage'
    for folder in ('gen','sol','review'):(fixture/folder).mkdir(parents=True)
    for name in ('gen/gen.cpp','gen/val.cpp','gen/genall.sh','sol/std_hld.cpp','review/check_output.py'):
        shutil.copyfile(str(ROOT/name),str(fixture/name))
    plan=json.loads((ROOT/'review/cases.json').read_text())
    small=dict(plan['cases'][0]);small.update(id='01_01',n=7,q=100,subtask=1)
    large=dict(small);large.update(id='storage1',subtask=10)
    (fixture/'review/cases.json').write_text(json.dumps({'cases':[small],'large_samples':[large]}))
    custom_data,custom_down=base/'chosen data',base/'chosen samples'
    for target in (custom_data,custom_down):
        target.mkdir();(target/'unrelated.keep').write_text('preserve me\n')
    subprocess.run(['bash',str(fixture/'gen/genall.sh'),str(custom_data),str(custom_down)],cwd='/tmp',check=True)
    for target,name in ((custom_data,'01_01'),(custom_down,'storage1')):
        assert (target/'unrelated.keep').read_text()=='preserve me\n'
        assert (target/(name+'.in')).is_file() and (target/(name+'.ans')).is_file()
    subprocess.run(['bash',str(fixture/'gen/genall.sh')],cwd='/tmp',check=True)
    assert (base/'contest/data/storage/01_01.in').is_file()
    assert (base/'contest/down/storage/storage1.ans').is_file()
    if args.mounted_parent:
        with tempfile.TemporaryDirectory(prefix='storage path test ',dir=str(args.mounted_parent)) as mounted:
            mounted=pathlib.Path(mounted);d=mounted/'data';s=mounted/'samples'
            for target in (d,s):
                target.mkdir();(target/'unrelated.keep').write_text('preserve me\n')
            subprocess.run(['bash',str(fixture/'gen/genall.sh'),str(d),str(s)],cwd='/tmp',check=True)
            assert (d/'01_01.in').is_file() and (s/'storage1.ans').is_file()
            assert all((target/'unrelated.keep').read_text()=='preserve me\n' for target in (d,s))
            cross_filesystem=mounted.stat().st_dev!=base.stat().st_dev
report={'status':'passed','arbitrary_cwd':True,'default_paths':True,'spaced_override_paths':True,'unrelated_files_preserved':True,'cross_filesystem_target':cross_filesystem}
(ROOT/'review/genall_paths.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
