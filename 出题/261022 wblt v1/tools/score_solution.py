#!/usr/bin/env python3
"""Measure an executable's actual grouped score on the released test data."""
import argparse
import json
from pathlib import Path
from checker import check
from runner import run_measured

ROOT = Path(__file__).resolve().parents[1]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', required=True, type=Path)
    ap.add_argument('--work', required=True, type=Path)
    ap.add_argument('--report', required=True, type=Path)
    ap.add_argument('--timeout', type=float, default=30)
    ap.add_argument('--memory-mb', type=int, default=512)
    ap.add_argument('--group', type=int, action='append')
    args = ap.parse_args()
    config = json.loads((ROOT/'config/scoring.json').read_text(encoding='utf-8'))
    manifest = json.loads((ROOT/'data/generation_manifest.json').read_text(encoding='utf-8'))
    args.work.mkdir(parents=True, exist_ok=True); args.report.parent.mkdir(parents=True, exist_ok=True)
    report = {'executable': args.exe.name, 'time_limit_seconds': args.timeout,
              'memory_limit_mib': args.memory_mb, 'groups': [], 'score': 0}
    for group in config['groups']:
        if args.group and group['id'] not in args.group:
            continue
        result = {'id': group['id'], 'score': group['score'], 'passed': True, 'cases': []}
        for case in manifest['cases']:
            if case['group'] != group['id']:
                continue
            name = case['id']; output = args.work/(name+'.out')
            run = run_measured([args.exe.resolve()], ROOT/'data'/case['input'], output,
                               args.timeout, args.memory_mb)
            if run['status'] == 'OK':
                try:
                    check(output, ROOT/'data'/(name+'.ans'))
                except ValueError as exc:
                    run['status']='WA'; run['mismatch']=str(exc)
            result['cases'].append({'id':name, **run})
            print(name, run['status'], run['wall_seconds'], flush=True)
            if run['status'] != 'OK':
                result['passed']=False
                result['remaining_cases']='not run after first group failure'
                break
        if result['passed']:
            report['score'] += group['score']
        report['groups'].append(result)
        args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    report['complete']=True
    args.report.write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print('SCORE', report['score'], flush=True)

if __name__ == '__main__':
    main()
