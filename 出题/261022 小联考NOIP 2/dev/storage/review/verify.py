#!/usr/bin/env python3
"""Compile and check testlib tools and independent full-program oracles.

Run from any cwd: python3 /path/dev/storage/review/verify.py [--data]
Results are printed to stdout; temporary binaries stay under ignored .build/.
"""
import argparse, itertools, json, pathlib, subprocess, sys, tempfile, time

ROOT=pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'gen'))
from genall import check_answer
REPO=pathlib.Path(subprocess.check_output(['git','-C',str(ROOT),'rev-parse','--show-toplevel'],text=True).strip())
SHAPES=('mid','random','left_comb','right_comb','zigzag_comb','heavy_right','bounded30')
MODES=('random','mixed','node_only','insert_only','root_singleton','alternating','adversarial','short')
WEIGHTS=('unit','random','large','tie')

def run(cmd,data=None,timeout=60,ok=True):
    p=subprocess.run(list(map(str,cmd)),input=data,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=timeout)
    if ok and p.returncode:
        raise RuntimeError('{} failed: {}'.format(cmd,p.stderr.decode(errors='replace')))
    return p

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--data',action='store_true',help='also verify generated final data')
    ap.add_argument('--data-only',action='store_true',help='skip generator and differential checks')
    ap.add_argument('--smoke',action='store_true',help='run a small generator/differential subset')
    ap.add_argument('--case',action='append',default=[],help='only these generated case IDs (with --data/--data-only)')
    args=ap.parse_args()
    if args.case and not (args.data or args.data_only):
        ap.error('--case requires --data or --data-only')
    started=time.monotonic()
    (ROOT/'.build').mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='verify-',dir=str(ROOT/'.build')) as temp:
        build=pathlib.Path(temp)
        sources={'gen':'gen/gen.cpp','val':'gen/val.cpp','std':'sol/std_hld.cpp',
                 'brute':'review/brute.cpp','recompute':'sol/partial_recompute.cpp',
                 'height':'sol/partial_height.cpp','short':'sol/partial_short.cpp',
                 'snapshot':'review/snapshot_oracle.cpp'}
        for name,source in sources.items():
            run(['g++','-std=c++17','-O2','-DNDEBUG','-Wall','-Wextra','-I',REPO,ROOT/source,'-o',build/name])
        def generate(n,q,shape,mode,weight,seed,numbering='random'):
            return run([build/'gen',n,q,shape,mode,weight,seed,numbering]).stdout
        def check(label,data,solvers):
            run([build/'val'],data)
            outputs={name:run([build/name],data).stdout.split() for name in solvers}
            q=int(data.splitlines()[0].split()[1]);expected=outputs[solvers[0]]
            if len(expected)!=2*q or any(x!=expected for x in outputs.values()):
                failure=ROOT/'.build/failure.in';failure.write_bytes(data)
                raise AssertionError('{} mismatch; input {}'.format(label,failure))
            return q
        if not args.data_only:
            tool_cases=0
            for n,shape,mode,weight in itertools.product((1,7) if args.smoke else (1,2,7,19),SHAPES,('mixed','short') if args.smoke else MODES,('tie',) if args.smoke else WEIGHTS):
                q=min(43,n*(n+1)//2) if mode=='insert_only' else 43
                data=generate(n,q,shape,mode,weight,17564+tool_cases)
                group={'mid':3,'right_comb':6,'heavy_right':7,'bounded30':4}.get(shape,10)
                run([build/'val','--group',group],data)
                special={'node_only':5,'insert_only':8,'short':9}.get(mode,10)
                run([build/'val','--group',special],data)
                tool_cases+=1
            for shape in (() if args.smoke else ('left_comb','right_comb','zigzag_comb')):
                run([build/'val'],generate(100000,7,shape,'adversarial','large',20261020))
                tool_cases+=1
            data=generate(37,101,'random','mixed','tie',998244353)
            assert data==generate(37,101,'random','mixed','tie',998244353)
            assert run([build/'gen',1,2,'mid','insert_only','unit',1,'random'],ok=False).returncode
            malformed=[b'1 2\n0 0 1\n+ 1 1\n+ 1 1\n',
                       b'1 1\n0 0 1\n- 1 1\n',b'1 1\n0 0 0\n+ 1 1\n',
                       b'2 1\n2 2 1\n0 0 1\n0 0 1\n+ 1 2\n',
                       b'2 1\n0 0 1\n3 1 1\n0 0 1\n+ 1 2\n',
                       b'1 1\n0 0 1\n+ 1 1\nextra\n']
            for data in malformed:assert run([build/'val'],data,ok=False).returncode
            # Each restriction must actually be enabled by --group. In this
            # repository registerValidation() does not prepare testlib options.
            bad_groups=[(1,generate(8,1,'mid','mixed','unit',1)),
                        (2,generate(2000,2000,'mid','mixed','unit',2)),
                        (3,generate(4,1,'right_comb','mixed','unit',3)),
                        (4,generate(33,1,'right_comb','mixed','unit',4)),
                        (5,b'3 1\n2 3 1\n4 5 1\n0 0 1\n0 0 1\n0 0 1\n+ 2 3\n'),
                        (6,generate(4,1,'left_comb','mixed','unit',6)),
                        (7,generate(4,1,'left_comb','mixed','unit',7)),
                        (8,generate(1,2,'mid','root_singleton','unit',8)),
                        (9,generate(3,1,'mid','root_singleton','unit',9))]
            for group,data in bad_groups:
                assert run([build/'val','--group',group],data,ok=False).returncode, ('ignored group',group)
            assert run([build/'val','--group',0],generate(1,1,'mid','mixed','unit',1),ok=False).returncode
            print('generator/validator checks passed: {} generated cases'.format(tool_cases),flush=True)
            records=[]
            for n in range(1,4 if args.smoke else 6):
                intervals=[(l,r) for l in range(1,n+1) for r in range(l,n+1)]
                active=set();ops=[]
                for i in range(1,1<<len(intervals)):
                    bit=(i&-i).bit_length()-1;l,r=intervals[bit]
                    op='-' if (l,r) in active else '+'
                    if op=='+':active.add((l,r))
                    else:active.remove((l,r))
                    ops.append('{} {} {}\n'.format(op,l,r))
                for shape,weight in itertools.product(('mid','left_comb','right_comb','zigzag_comb','random'),('unit','tie','large')):
                    rows=generate(n,1,shape,'mixed',weight,n*73+len(records)).splitlines(keepends=True)[1:2*n]
                    data=('{} {}\n'.format(n,len(ops))).encode()+b''.join(rows)+''.join(ops).encode()
                    label='gray-{}-{}-{}'.format(n,shape,weight)
                    count=check(label,data,('std','recompute','height','brute'))
                    records.append({'id':label,'operations':count,'brute':True})
                print('Gray-state traversal n={} passed'.format(n),flush=True)
            for seed in range(8 if args.smoke else 180):
                n=6+seed%3 if seed<36 else 9+seed*17%400
                q=350 if n<=8 else 700;shape=SHAPES[seed%7]
                mode=('mixed','adversarial','alternating','node_only','short','random')[seed%6]
                weight=('unit','tie','large','random')[seed%4]
                data=generate(n,q,shape,mode,weight,20261021000+seed)
                solvers=['std','recompute','height']+(['brute'] if n<=8 else [])+(['short'] if mode=='short' else [])
                check('random-'+str(seed),data,solvers)
                records.append({'id':'random-'+str(seed),'operations':q,'brute':n<=8})
                if seed%30==29:print('random {}/180 passed'.format(seed+1),flush=True)
            for seed,shape in enumerate(() if args.smoke else SHAPES):
                data=generate(1500,2500,shape,'mixed','tie',71234+seed)
                check('medium-'+shape,data,('std','recompute','height'))
                records.append({'id':'medium-'+shape,'operations':2500,'brute':False})
            report={'status':'passed','generated_cases':tool_cases,'invalid_inputs_rejected':len(malformed),
                    'compiler':run(['g++','--version']).stdout.decode().splitlines()[0],
                    'subtask_restrictions_rejected':len(bad_groups),'invalid_group_rejected':True,
                    'seed_reproducible':True,'differential_cases':len(records),
                    'operations':sum(r['operations'] for r in records),
                    'brute_operations':sum(r['operations'] for r in records if r['brute']),
                    'elapsed_seconds':round(time.monotonic()-started,3),'records':records}
            print(json.dumps({k:v for k,v in report.items() if k!='records'}),flush=True)
        if args.data or args.data_only:
            plan=json.loads((ROOT/'gen/cases.json').read_text(encoding='utf-8'))
            cases=[case for bucket in ('cases','statement_samples','large_samples') for case in plan.get(bucket,[])]
            unknown=set(args.case)-{case['id'] for case in cases}
            if unknown:ap.error('unknown case IDs: '+', '.join(sorted(unknown)))
            records=[];t0=time.monotonic()
            for case in cases:
                if args.case and case['id'] not in args.case:continue
                inp,ans=(ROOT/case[k] for k in ('input','answer'))
                data=inp.read_bytes()
                assert tuple(map(int,data.splitlines()[0].split()))==(case['n'],case['q']),case['id']
                if 'source' in case:
                    sample=ROOT/'review/samples'/('sample'+str(case['sample_index']))
                    assert data==sample.with_suffix('.in').read_bytes() and ans.read_bytes()==sample.with_suffix('.ans').read_bytes(),(case['id'],'saved sample differs')
                run([build/'val','--group',case['subtask']],data)
                check_answer(inp,ans)
                snapshot=run([build/'snapshot',inp,ans,20],timeout=120).stderr.decode().strip()
                refs=[]
                if case['n']*case['q']<=2000000:refs.append('recompute')
                if case['n']<=7 and case['q']<=100:refs.append('brute')
                if case['subtask'] in (3,4):refs.append('height')
                if case['subtask']==9:refs.append('short')
                expected=ans.read_bytes().split()
                for name in refs:assert run([build/name],data,timeout=90).stdout.split()==expected,(case['id'],name)
                records.append({'id':case['id'],'snapshot':snapshot,'full_references':refs})
                print(case['id']+': '+snapshot+'; full='+','.join(refs),flush=True)
            report={'status':'passed','cases':len(records),'elapsed_seconds':round(time.monotonic()-t0,3),'records':records}
            print(json.dumps({k:v for k,v in report.items() if k!='records'}),flush=True)
            print('All {} data files independently checked'.format(len(records)),flush=True)

if __name__=='__main__':main()
