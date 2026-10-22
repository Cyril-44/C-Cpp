#!/usr/bin/env python3
"""Compile 12 deliberately wrong std variants and catch them on generated/official data.

Run from any cwd: python3 /absolute/path/dev/storage/review/mutation_test.py
Fixed seeds and mutation anchors reproduce every result. Original sources and
official data are never modified; temporary binaries are removed at the end.
"""
import argparse, concurrent.futures, hashlib, json, pathlib, subprocess, tempfile, time
ROOT=pathlib.Path(__file__).resolve().parents[1]
REPO=pathlib.Path(subprocess.check_output(['git','-C',str(ROOT),'rev-parse','--show-toplevel'],text=True).strip())

def replace_one(source, old, new):
    if source.count(old) != 1:
        raise ValueError(f"mutation anchor must occur exactly once: {old!r}")
    return source.replace(old, new, 1)


def remove_point_push(source):
    start = source.index("    void pointRemake(")
    end = source.index("    Matrix query(", start)
    part = replace_one(source[start:end], "        push(p, a, b);\n", "        // BUG: skipped pending lazy tags.\n")
    return source[:start] + part + source[end:]


def wrong_boundaries(source):
    source = replace_one(source, "boundary(a, z, 1, delta);", "boundary(a, z, 0, delta);")
    return replace_one(source, "boundary(b, z, 0, delta);", "boundary(b, z, 1, delta);")


MUTATIONS = [
    ("ways_always_one", "Always output one optimal storage set", lambda s: replace_one(s, "<< ans.ways <<", "<< 1 <<")),
    ("drop_equal_cost_alternative", "Do not add ways when Value alternatives tie", lambda s: replace_one(s, "return {a.cost, w};", "return a;")),
    ("heavy_counter_is_boolean", "Forget shared references to a heavy child", lambda s: replace_one(s, "heavyCount[p] += delta;", "heavyCount[p] = delta > 0 ? 1 : 0;")),
    ("reverse_boundary_directions", "Use outside siblings instead of inside siblings", wrong_boundaries),
    ("do_not_lift_endpoints", "Start at raw leaves instead of maximal contained nodes", lambda s: replace_one(s, "int a = maximal(leafId[l], l, r), b = maximal(leafId[r], l, r);", "int a = leafId[l], b = leafId[r];")),
    ("ignore_root_requirement", "Ignore the root's direct requirement counter", lambda s: replace_one(s, "Value ans = rootCount ? B[1] : X[1];", "Value ans = X[1];")),
    ("ignore_right_zero_minimum", "Always treat right-light-child counters as positive in query", lambda s: replace_one(s, "int mask = (seg[p].minimum[0] == 0) | ((seg[p].minimum[1] == 0) << 1);", "int mask = (seg[p].minimum[0] == 0);")),
    ("matrix_ways_add_instead_multiply", "Add ways when concatenating independent matrix choices", lambda s: replace_one(s, "int count = int(1LL * a.w[ai] * b.w[bi] % MOD);", "int count = (a.w[ai] + b.w[bi]) % MOD;")),
    ("truncate_cost_to_int32", "Truncate the final minimum cost to signed 32 bits", lambda s: replace_one(s, "cout << ans.cost <<", "cout << int(ans.cost) <<")),
    ("point_remake_without_push", "Rebuild a point before pushing pending counter changes", remove_point_push),
    ("propagate_chains_top_down", "Propagate parent chains before changed child chains", lambda s: replace_one(s, "return depth[a] > depth[b];", "return depth[a] < depth[b];")),
    ("ignore_child_minimum_eligibility", "Treat a child's larger minima as global minima", lambda s: replace_one(s, "seg[right].product[mask & rightEligible]", "seg[right].product[mask]")),
]




def run(command,data=None):
    return subprocess.run(list(map(str,command)),input=data,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=30)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--generated-only',action='store_true',help='only run the small targeted tests')
    args=parser.parse_args()
    plan=json.loads((ROOT/'gen/cases.json').read_text(encoding='utf-8'))
    categories=('generated',) if args.generated_only else ('generated','official')
    started=time.monotonic()
    original=(ROOT/'sol/std_hld.cpp').read_bytes();source=original.decode()
    standard_hash=hashlib.sha256(original).hexdigest()
    (ROOT/'.build').mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='mutation-',dir=str(ROOT/'.build')) as temp:
        temp=pathlib.Path(temp)
        sources={'baseline':ROOT/'sol/std_hld.cpp','brute':ROOT/'review/brute.cpp',
                 'recompute':ROOT/'sol/partial_recompute.cpp','gen':ROOT/'gen/gen.cpp'}
        for name,_,change in MUTATIONS:
            path=temp/(name+'.cpp');path.write_text(change(source));sources[name]=path
        def compile_one(item):
            name,path=item
            p=run(['g++','-std=c++17','-O2','-DNDEBUG','-I',REPO,path,'-o',temp/name])
            assert p.returncode==0,(name,p.stderr.decode(errors='replace'))
        with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
            list(pool.map(compile_one,sources.items()))
        cases=[]
        def add(name,data):
            n,q=map(int,data.splitlines()[0].split())
            oracle='brute' if n<=8 else 'recompute'
            p=run([temp/oracle],data);assert p.returncode==0
            baseline=run([temp/'baseline'],data);assert baseline.returncode==0 and baseline.stdout.split()==p.stdout.split(),name
            cases.append({'name':name,'n':n,'q':q,'input':data,'answer':p.stdout,'oracle':oracle})
        def generate(n,q,shape,weight,seed,mode='adversarial'):
            p=run([temp/'gen',n,q,shape,mode,weight,seed,'random']);assert p.returncode==0
            return p.stdout
        def custom(name,n,shape,weight,ops):
            rows=generate(n,1,shape,weight,17564).splitlines(keepends=True)[1:2*n]
            add(name,('{} {}\n'.format(n,len(ops))).encode()+b''.join(rows)+(''.join(op+'\n' for op in ops)).encode())
        for case in plan.get('statement_samples',[]):
            sample=ROOT/'review/samples'/('sample'+str(case['sample_index']))
            sample_input,sample_answer=sample.with_suffix('.in').read_bytes(),sample.with_suffix('.ans').read_bytes()
            add(case['id'],sample_input)
            assert cases[-1]['answer']==sample_answer,(case['id'],'saved sample output differs')
        custom('shared_heavy_references',4,'mid','unit',['+ 3 4','+ 2 4','- 3 4','- 2 4'])
        custom('int64_costs',5,'random','large',['+ {} {}'.format(x,x) for x in range(1,6)])
        ops=['+ 1 31','+ 8 32','+ 9 32','- 8 32','- 1 31','+ 2 31','+ 1 30','- 9 32','- 2 31','- 1 30']
        custom('lazy_then_point_right',32,'right_comb','unit',ops)
        mirror=['{} {} {}'.format(op.split()[0],33-int(op.split()[2]),33-int(op.split()[1])) for op in ops]
        custom('lazy_then_point_left',32,'left_comb','unit',mirror)
        for i,shape in enumerate(('mid','random','left_comb','right_comb','zigzag_comb','heavy_right','bounded30')):
            for j,weight in enumerate(('unit','tie','random','large')):
                add('{}-{}'.format(shape,weight),generate(50,600,shape,weight,2026102200+i*10+j,'mixed' if i%2 else 'adversarial'))
        for j,weight in enumerate(('unit','tie','random','large')):
            add('brute8-'+weight,generate(8,400,'random',weight,998244350+j,'mixed'))
        official=plan['cases']
        results=[]
        for name,description,_ in MUTATIONS:
            record={'name':name,'description':description}
            for category in categories:
                record[category]={'status':'survived'}
                for c in cases if category=='generated' else official:
                    data=c['input'] if category=='generated' else (ROOT/c['input']).read_bytes()
                    answer=c['answer'] if category=='generated' else (ROOT/c['answer']).read_bytes()
                    p=run([temp/name],data)
                    if p.returncode or p.stdout.split()!=answer.split():
                        record[category]={'status':'wrong_output' if p.returncode==0 else 'runtime_error',
                                          'case':c['name'] if category=='generated' else c['id'],
                                          'return_code':p.returncode,
                                          'input_sha256':hashlib.sha256(data).hexdigest()}
                        break
            results.append(record)
            print(name+': '+json.dumps({category:record[category] for category in categories}),flush=True)
        unchanged=hashlib.sha256((ROOT/'sol/std_hld.cpp').read_bytes()).hexdigest()==standard_hash
        kills={category:sum(r[category]['status']=='wrong_output' for r in results) for category in categories}
        report={'status':'passed' if unchanged and all(k==len(MUTATIONS) for k in kills.values()) else 'needs_attention',
                'standard_sha256':standard_hash,'standard_unchanged':unchanged,
                'generated_cases':len(cases),'official_cases_checked':0 if args.generated_only else len(official),'mutants':len(MUTATIONS),
                'wrong_output_kills':kills,'elapsed_seconds':round(time.monotonic()-started,3),'results':results}
        print(json.dumps({k:v for k,v in report.items() if k!='results'}),flush=True)
        assert report['status']=='passed'

if __name__=='__main__':main()
