#!/usr/bin/env python3
"""Compile and independently review LRUD. Run from any working directory."""
from pathlib import Path
import os
import platform
import subprocess
import tempfile
import time

ROOT = Path(__file__).resolve().parents[1]
REPO = Path(subprocess.check_output(['git','-C',str(ROOT),'rev-parse','--show-toplevel'], text=True).strip())
CXX = os.environ.get('CXX','g++')
def call(args, **kw):
    return subprocess.run(list(map(str,args)), check=True, **kw)
def check(args, good=True, **kw):
    result = subprocess.run(list(map(str,args)), stdout=subprocess.PIPE, stderr=subprocess.PIPE, **kw)
    assert (result.returncode == 0) == good, (args, result.returncode, result.stdout, result.stderr)
    return result

with tempfile.TemporaryDirectory(prefix='.review.',dir=ROOT) as temp:
    build = Path(temp)
    sources = {'std':'sol/std_maximum.cpp','brute':'review/brute_subset.cpp','verify':'review/verify_construction.cpp','checker':'review/checker.cpp','val':'gen/val.cpp','gen':'gen/gen.cpp'}
    partials = ['partial_dfs_10','partial_subset_25','partial_odd_45','partial_bands_65']
    sources.update({s:'sol/'+s+'.cpp' for s in partials})
    for name, source in sources.items():
        call([CXX,'-std=c++17','-O2','-I',REPO,ROOT/source,'-o',build/name])
    start = time.perf_counter()
    verified = subprocess.check_output([build/'verify'],text=True).strip()
    elapsed = time.perf_counter()-start
    print(verified, f'({elapsed:.3f}s)', flush=True)
    brute = subprocess.check_output([build/'brute'],input='2\n3\n4\n',text=True).split()
    assert brute == ['2','4','8'], brute
    inp, ans, out = [build/s for s in ('case.in','case.ans','case.out')]
    inp.write_text('3\n2\n3\n4\n')
    with inp.open() as f, ans.open('w') as g: call([build/'std'],stdin=f,stdout=g)
    for part in partials[1:]:
        with inp.open() as f, out.open('w') as g: call([build/part],stdin=f,stdout=g)
        check([build/'checker',inp,out,ans])
    inp.write_text('2\n2\n3\n')
    with inp.open() as f, ans.open('w') as g: call([build/'std'],stdin=f,stdout=g)
    with inp.open() as f, out.open('w') as g: call([build/partials[0]],stdin=f,stdout=g)
    check([build/'checker',inp,out,ans])
    # Valid alternate N=4 path from the statement must be accepted.
    inp.write_text('1\n4\n')
    with inp.open() as f, ans.open('w') as g: call([build/'std'],stdin=f,stdout=g)
    out.write_text('8\nDDDXZDQZQDZXDDD\n')
    check([build/'checker',inp,out,ans])
    for text in ['8\nD\n','7\nDDDXZDQZQDZXDDD\n','8\nDDDXZDQZQDZXDDD\nextra\n','8\nWWWWWWWWWWWWWWW\n']:
        out.write_text(text)
        check([build/'checker',inp,out,ans],good=False)
    negative = [(5,'0\n'),(5,'6\n2\n2\n2\n2\n2\n2\n'),(5,'1\n1001\n'),(1,'1\n4\n'),(2,'1\n5\n'),(3,'1\n6\n'),(4,'1\n8\n'),(5,'1\n4\n0\n')]
    for st, text in negative:
        check([build/'val',f'--subtask={st}'],good=False,input=text.encode())
    # Fixed seeds reproduce the same input and satisfy each group's restriction.
    for st in range(1,6):
        cmd=[build/'gen',f'--subtask={st}','--mode=random','--t=5','--seed=20261022']
        a=subprocess.check_output(cmd)
        assert a==subprocess.check_output(cmd)
        check([build/'val',f'--subtask={st}'],input=a)
    data = ROOT/'../../data/lrud'
    matrix = [[0]*5 for _ in partials]
    totals = [0]*5
    for path in sorted(data.glob('??_??.in')):
        group=int(path.stem[:2])
        totals[group-1]+=1
        jury=path.with_suffix('.ans')
        with path.open() as f: check([build/'val',f'--subtask={group}'],stdin=f)
        check([build/'checker',path,jury,jury])
        for i, part in enumerate(partials):
            with path.open() as f, out.open('w') as g: call([build/part],stdin=f,stdout=g,timeout=5)
            verdict=subprocess.run([build/'checker',path,out,jury],stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL).returncode
            matrix[i][group-1]+=verdict==0
    assert totals==[6,5,8,8,10], totals
    for i, row in enumerate(matrix):
        assert all(row[g]==totals[g] for g in range(i+1)), row
        assert all(row[g]<totals[g] for g in range(i+1,5)), row
    # Measure output generation on the maximum single input (5 x N=1000).
    inp.write_text('5\n'+'1000\n'*5)
    times=[]
    for _ in range(3):
        begin=time.perf_counter()
        with inp.open() as f, out.open('w') as g: call([build/'std'],stdin=f,stdout=g)
        times.append(time.perf_counter()-begin)
    compiler=subprocess.check_output([CXX,'--version'],text=True).splitlines()[0]
    cpu=next((line.split(':',1)[1].strip() for line in Path('/proc/cpuinfo').read_text().splitlines() if line.startswith('model name')),platform.machine())
    report=['# LRUD 实测复核报告','','执行 `python3 review/run.py` 生成；时间会受机器负载影响。',f'环境：{cpu}；{compiler}；C++17 `-O2`。','',f'- {verified}，耗时 {elapsed:.3f} 秒。','- 独立反向子集 DP：N=2,3,4 的最优值分别为 2,4,8，与正解及路径恢复部分分一致。','- SPJ 接受题面中的另一条最优构造，并拒绝长度错误、最优值错误、额外输出、越界移动四种输出。','- validator 拒绝 8 类非法输入；各组固定种子随机生成可复现且通过组别校验。',f'- 正式数据 {sum(totals)} 点均通过 validator 与 SPJ。',f'- 最大单点（5×N=1000）三次生成答案耗时：'+', '.join(f'{t:.3f}s' for t in times)+'。','','## 部分分实测','','表中为通过点数 / 该组总点数。组别独立相加。','','| 实现 | 组1 | 组2 | 组3 | 组4 | 组5 | 得分 |','| --- | --- | --- | --- | --- | --- | ---: |']
    for name,row,score in zip(partials,matrix,[10,25,45,65]):
        report.append('| '+name+' | '+' | '.join(f'{a}/{b}' for a,b in zip(row,totals))+f' | {score} |')
    (ROOT/'review/report.md').write_text('\n'.join(report)+'\n')
    print('\n'.join(report),flush=True)
