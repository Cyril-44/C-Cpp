#!/usr/bin/env python3
"""Replay the embedded 75-case validator acceptance/rejection matrix."""
import argparse
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', required=True, type=Path)
    ap.add_argument('--matrix', type=Path, default=ROOT/'audit'/'validator_tests.json')
    ap.add_argument('--report', type=Path, default=ROOT/'audit'/'validator_replay.json')
    args = ap.parse_args()
    matrix = json.loads(args.matrix.read_text(encoding='utf-8'))['validator_tests']['cases']
    results = []
    for case in matrix:
        result = subprocess.run([str(args.exe.resolve()),*case['args']],
                                input=case['input_ascii'].encode('utf-8'),capture_output=True,timeout=10)
        diagnostic = result.stderr.decode('utf-8',errors='replace')
        ok = result.returncode == case['expected_exit']
        expected = case.get('expected_diagnostic_contains')
        if expected and expected not in diagnostic:
            ok = False
        results.append({'name':case['name'],'passed':ok,'returncode':result.returncode})
    report = {'all_passed':all(x['passed'] for x in results),'count':len(results),'cases':results}
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    if not report['all_passed']:
        raise SystemExit('validator regression')
    print('PASS',len(results),'validator tests')

if __name__=='__main__':
    main()
