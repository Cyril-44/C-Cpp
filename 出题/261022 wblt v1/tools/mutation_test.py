#!/usr/bin/env python3
"""Compile deliberately wrong variants and require official tests to reject them."""
import argparse
import json
from pathlib import Path
import os
import subprocess
from checker import check
from runner import run_measured

ROOT = Path(__file__).resolve().parents[1]
MUTATIONS = [
 ('missing_r1', 'omit first right boundary correction', 'sub(0,add(da,ds))', '0'),
 ('missing_r2', 'omit second right boundary correction', '{r+2,da}', '{r+2,0}'),
 ('seed_shift', 'start periodic seed one place too early',
  'tree.slice(root,l+2,l+k+1)', 'tree.slice(root,l+1,l+k)'),
 ('second_moment', 'omit quadratic moment translation', 'mul(c2(a.n),b.s0)', 'mul(0,b.s0)'),
 ('lazy_composition', 'overwrite instead of compose multiplier', 'a.tag=mul(a.tag,k);', 'a.tag=k;'),
 ('shared_mutation', 'modify a shared node in place',
  'keep(a.l);keep(a.r);return alloc(a);', 'a.ref=t[x].ref;t[x]=a;return keep(x);'),
 ('overlap_drop', 'drop rather than add coincident endpoint corrections',
  'return edit(root,1,l,r,factor,c.data(),0,int(c.size()));',
  'c.erase(std::unique(c.begin(),c.end(),[](Change a,Change b){return a.pos==b.pos;}),c.end());\n        return edit(root,1,l,r,factor,c.data(),0,int(c.size()));'),
 ('overflow32', 'multiply residues with 32-bit arithmetic', 'return u64(a)*b%P;', 'return a*b%P;'),
 ('skip_one_change', 'skip calibration changing exactly one element', 'if(k!=len)', 'if(k+1<len)'),
]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--cxx', default='g++'); ap.add_argument('--work', required=True, type=Path)
    ap.add_argument('--report', type=Path, default=ROOT/'audit'/'mutation_tests.json')
    args = ap.parse_args(); args.work.mkdir(parents=True, exist_ok=True)
    source = (ROOT/'solutions'/'std.cpp').read_text(encoding='utf-8')
    cases = [(f'sample_{p.stem}', p, p.with_suffix('.out')) for p in sorted((ROOT/'samples').glob('*.in'))]
    cases += [(p.stem,p,p.with_suffix('.ans')) for p in sorted((ROOT/'data').glob('g1*.in'))]
    if any(not answer.exists() for _,_,answer in cases):
        raise SystemExit('Generate and verify official answers before mutation testing')
    report = {'kind': 'deliberately incorrect implementations', 'mutants': []}
    for name, purpose, old, new in MUTATIONS:
        if source.count(old) != 1:
            raise SystemExit(f'Mutation anchor {name} occurs {source.count(old)} times; inspect source')
        src = args.work/(name+'.cpp'); src.write_text(source.replace(old,new), encoding='utf-8')
        exe = args.work/(name+('.exe' if os.name=='nt' else ''))
        subprocess.run([args.cxx,'-std=c++17','-O2','-DNDEBUG',str(src),'-o',str(exe)],check=True)
        result = {'name':name,'purpose':purpose,'killed':False}
        for case_id, inp, answer in cases:
            out = args.work/(name+'.out')
            run = run_measured([exe.resolve()],inp,out,timeout=5,memory_mb=512)
            reason = run['status']
            if reason == 'OK':
                try:
                    check(out,answer)
                except ValueError as e:
                    reason='WA'; run['mismatch']=str(e)
            if reason!='OK':
                result.update(killed=True,case=case_id,verdict=reason,details=run)
                break
        report['mutants'].append(result)
        print(('KILLED' if result['killed'] else 'SURVIVED'),name,result.get('case',''),flush=True)
    report['all_killed']=all(x['killed'] for x in report['mutants'])
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    if not report['all_killed']:
        raise SystemExit('At least one bug escaped the test data; strengthen tests before release')

if __name__=='__main__':
    main()
