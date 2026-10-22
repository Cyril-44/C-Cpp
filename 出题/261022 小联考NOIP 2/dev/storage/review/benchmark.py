#!/usr/bin/env python3
"""Compare standard answers and benchmark selected generated files (stdout only)."""
import argparse, json, pathlib, platform, statistics, subprocess, tempfile

ROOT=pathlib.Path(__file__).resolve().parents[1]


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case',action='append',help='case ID; default: five representative files')
    parser.add_argument('--repeat',type=int,default=3,help='runs per case (default: 3)')
    args=parser.parse_args()
    if args.repeat<1:parser.error('--repeat must be positive')
    plan=json.loads((ROOT/'gen/cases.json').read_text(encoding='utf-8'))
    cases={case['id']:case for bucket in ('cases','statement_samples','large_samples') for case in plan.get(bucket,[])}
    selected=args.case or ['04_04','07_03','10_05','10_08','10_09']
    if set(selected)-cases.keys():parser.error('unknown case ID')
    records=[]
    (ROOT/'.build').mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='benchmark-',dir=str(ROOT/'.build')) as temp:
        temp=pathlib.Path(temp);program=temp/'std'
        subprocess.run(['g++','-std=c++17','-O2','-DNDEBUG',str(ROOT/'sol/std_hld.cpp'),'-o',str(program)],check=True)
        for name in selected:
            case=cases[name];times=[];memory=[]
            for repeat in range(args.repeat):
                metric=temp/'metric';out=temp/'answer'
                with (ROOT/case['input']).open('rb') as f,out.open('wb') as g:
                    subprocess.run(['/usr/bin/time','-f','%e %M','-o',str(metric),str(program)],stdin=f,stdout=g,check=True,timeout=30)
                assert out.read_bytes().split()==(ROOT/case['answer']).read_bytes().split(),name
                seconds,kib=metric.read_text().split();times.append(float(seconds));memory.append(int(kib))
            records.append({'id':name,'seconds':times,'median_seconds':statistics.median(times),'peak_rss_kib':max(memory)})
            print(name,records[-1],flush=True)
    report={'status':'passed','environment':platform.platform(),
            'compiler':subprocess.check_output(['g++','--version'],text=True).splitlines()[0],
            'runs':len(selected)*args.repeat,'max_median_seconds':max(r['median_seconds'] for r in records),
            'peak_rss_kib':max(r['peak_rss_kib'] for r in records)}
    print(json.dumps(report),flush=True)


if __name__=='__main__':main()
