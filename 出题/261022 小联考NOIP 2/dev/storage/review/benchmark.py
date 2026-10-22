#!/usr/bin/env python3
"""Benchmark five representative generated files, each repeated three times."""
import json, pathlib, platform, statistics, subprocess, tempfile
ROOT=pathlib.Path(__file__).resolve().parents[1]
DATA=ROOT/'../../data/storage'
records=[]
with tempfile.TemporaryDirectory(prefix='.benchmark.',dir=str(ROOT)) as temp:
    temp=pathlib.Path(temp);program=temp/'std'
    subprocess.run(['g++','-std=c++17','-O2','-DNDEBUG',str(ROOT/'sol/std_hld.cpp'),'-o',str(program)],check=True)
    for name in ('04_04','07_03','10_05','10_08','10_09'):
        times=[];memory=[]
        for repeat in range(3):
            metric=temp/'metric';out=temp/'answer'
            with (DATA/(name+'.in')).open('rb') as f,out.open('wb') as g:
                subprocess.run(['/usr/bin/time','-f','%e %M','-o',str(metric),str(program)],stdin=f,stdout=g,check=True,timeout=30)
            assert out.read_bytes().split()==(DATA/(name+'.ans')).read_bytes().split()
            seconds,kib=metric.read_text().split();times.append(float(seconds));memory.append(int(kib))
        records.append({'id':name,'seconds':times,'median_seconds':statistics.median(times),'peak_rss_kib':max(memory)})
        print(name,records[-1],flush=True)
report={'status':'passed','environment':platform.platform(),
        'compiler':subprocess.check_output(['g++','--version'],text=True).splitlines()[0],
        'runs':15,'max_median_seconds':max(r['median_seconds'] for r in records),
        'peak_rss_kib':max(r['peak_rss_kib'] for r in records),'records':records}
(ROOT/'review/benchmark.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='records'}),flush=True)
