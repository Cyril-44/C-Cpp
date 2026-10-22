#!/usr/bin/env python3
"""Generate and validate all storage data and public samples from any cwd.

python3 /absolute/path/dev/storage/gen/genall.py [data_dir] [down_dir]
Explicit relative output paths are relative to the caller's working directory.
"""
import argparse
import json
import os
import pathlib
import re
import shlex
import shutil
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]


def check_answer(inp, ans):
    """Reject a wrong number of rows, malformed integers or illegal ranges."""
    with inp.open('rb') as stream:
        n, q = map(int, stream.readline().split())
    rows = ans.read_bytes().splitlines()
    if len(rows) != q:
        raise ValueError('{}: expected {} answer rows, got {}'.format(ans, q, len(rows)))
    for index, row in enumerate(rows, 1):
        if re.fullmatch(rb'[0-9]+ [0-9]+', row) is None:
            raise ValueError('{}:{} malformed answer row'.format(ans, index))
        cost, ways = map(int, row.split())
        if not (0 <= cost <= (2*n-1)*10**9 and 0 <= ways < 998244353):
            raise ValueError('{}:{} invalid answer range'.format(ans, index))


def statement_sample(case):
    """Read the exact input/output fenced blocks in the unchanged statement."""
    statement = (ROOT / case['source']).read_text(encoding='utf-8')
    blocks = re.findall(
        r'^### (输入|输出)[ \t]*\n(?:[ \t]*\n)*```(?:text)?[ \t]*\n(.*?)^```[ \t]*$',
        statement, flags=re.MULTILINE | re.DOTALL)
    if not blocks or len(blocks) % 2:
        raise ValueError('Statement must contain paired input/output sample blocks')
    samples = []
    for index in range(0, len(blocks), 2):
        if blocks[index][0] != '输入' or blocks[index+1][0] != '输出':
            raise ValueError('Statement sample input/output blocks are out of order')
        samples.append((blocks[index][1].encode('utf-8'), blocks[index+1][1].encode('utf-8')))
    return samples[case['sample_index']-1]


def publish(source, target, public_sample):
    """Stage on the destination filesystem and replace only this owned file."""
    destination = target / source.name
    mode = 0o664 if public_sample else (destination.stat().st_mode & 0o777 if destination.exists() else 0o644)
    stage = None
    try:
        with tempfile.NamedTemporaryFile(dir=str(target), prefix='.storage-'+source.stem+'-', delete=False) as stream:
            stage = pathlib.Path(stream.name)
        shutil.copyfile(str(source), str(stage))
        stage.chmod(mode)
        os.replace(str(stage), str(destination))
    finally:
        if stage is not None and stage.exists():
            stage.unlink()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('data_dir', nargs='?', type=pathlib.Path, default=ROOT.parents[1]/'data/storage')
    parser.add_argument('down_dir', nargs='?', type=pathlib.Path, default=ROOT.parents[1]/'down/storage')
    args = parser.parse_args()
    data, down = args.data_dir.resolve(), args.down_dir.resolve()
    repo = pathlib.Path(subprocess.check_output(
        ['git', '-C', str(ROOT), 'rev-parse', '--show-toplevel'], text=True).strip())
    if not (repo/'testlib.h').is_file():
        raise FileNotFoundError('Missing Git-root testlib.h: {}'.format(repo/'testlib.h'))
    plan = json.loads((ROOT/'gen/cases.json').read_text(encoding='utf-8'))
    for target in (data, down):
        target.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='.genall.', dir=str(ROOT)) as temp:
        build = pathlib.Path(temp)
        compiler = shlex.split(os.environ.get('CXX', 'g++'))
        for name, source in (('gen', 'gen/gen.cpp'), ('val', 'gen/val.cpp'), ('std', 'sol/std_hld.cpp')):
            subprocess.run(compiler+['-std=c++17', '-O2', '-DNDEBUG', '-I', str(repo),
                           str(ROOT/source), '-o', str(build/name)], check=True)
        for bucket, target in (('cases', data), ('statement_samples', down), ('large_samples', down)):
            public_sample = bucket != 'cases'
            for case in plan.get(bucket, []):
                name = case['id']
                if re.fullmatch(r'[A-Za-z0-9_]+', name) is None:
                    raise ValueError('Invalid generated case id: '+name)
                print('Generating {} n={} q={}'.format(name, case['n'], case['q']), flush=True)
                inp, ans = build/(name+'.in'), build/(name+'.ans')
                expected = None
                if bucket == 'statement_samples':
                    sample_input, expected = statement_sample(case)
                    inp.write_bytes(sample_input)
                else:
                    with inp.open('wb') as stream:
                        subprocess.run([str(build/'gen')]+[str(case[key]) for key in
                            ('n', 'q', 'shape', 'mode', 'weights', 'seed', 'numbering')], stdout=stream, check=True)
                with inp.open('rb') as stream:
                    if tuple(map(int, stream.readline().split())) != (case['n'], case['q']):
                        raise ValueError(name+': input header differs from generation plan')
                with inp.open('rb') as stream:
                    subprocess.run([str(build/'val'), '--group', str(case['subtask'])], stdin=stream, check=True)
                with inp.open('rb') as stream, ans.open('wb') as output:
                    subprocess.run([str(build/'std')], stdin=stream, stdout=output, timeout=30, check=True)
                check_answer(inp, ans)
                if expected is not None and ans.read_bytes() != expected:
                    raise ValueError(name+': standard solution output differs from the exact statement sample')
                for source in (inp, ans):
                    publish(source, target, public_sample)
    print('Done: {} test cases in {}, {} public samples in {}'.format(
        len(plan['cases']), data, len(plan.get('statement_samples', []))+len(plan['large_samples']), down), flush=True)


if __name__ == '__main__':
    main()
