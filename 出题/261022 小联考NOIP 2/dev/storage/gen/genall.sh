#!/usr/bin/env bash
# bash /absolute/path/dev/storage/gen/genall.sh [data_directory] [large_sample_directory]
set -euo pipefail
ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
REPO=$(git -C "$ROOT" rev-parse --show-toplevel)
[[ -f "$REPO/testlib.h" ]] || { echo "Missing Git-root testlib.h: $REPO/testlib.h" >&2; exit 1; }
DATA=${1:-"$ROOT/../../data/storage"}
DOWN=${2:-"$ROOT/../../down/storage"}
mkdir -p -- "$DATA" "$DOWN"
DATA=$(cd -- "$DATA" && pwd)
DOWN=$(cd -- "$DOWN" && pwd)
BUILD=$(mktemp -d "$ROOT/.genall.XXXXXX")
trap 'rm -rf -- "$BUILD"' EXIT
CXX=${CXX:-g++}
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/gen.cpp" -o "$BUILD/gen"
"$CXX" -std=c++17 -O2 -I "$REPO" "$ROOT/gen/val.cpp" -o "$BUILD/val"
"$CXX" -std=c++17 -O2 -DNDEBUG "$ROOT/sol/std_hld.cpp" -o "$BUILD/std"
python3 - "$ROOT" "$BUILD" "$DATA" "$DOWN" <<'PY'
import hashlib, json, os, pathlib, shutil, subprocess, sys, tempfile, time
root, build, data, down = map(pathlib.Path, sys.argv[1:])
plan = json.loads((root/'review/cases.json').read_text(encoding='utf-8'))
records = []
for bucket, target in (('cases',data),('large_samples',down)):
    public_samples = bucket == 'large_samples'
    for case in plan[bucket]:
        name = case['id']
        print('Generating {} n={} q={} {} {}'.format(name,case['n'],case['q'],case['shape'],case['mode']), flush=True)
        inp, ans = build/(name+'.in'), build/(name+'.ans')
        start = time.monotonic()
        with inp.open('wb') as f:
            subprocess.run([str(build/'gen')]+[str(case[k]) for k in
                ('n','q','shape','mode','weights','seed','numbering')],stdout=f,check=True)
        with inp.open('rb') as f:
            subprocess.run([str(build/'val'),'--group',str(case['subtask'])],stdin=f,check=True)
        with inp.open('rb') as f, ans.open('wb') as g:
            subprocess.run([str(build/'std')],stdin=f,stdout=g,timeout=30,check=True)
        subprocess.run([sys.executable,str(root/'review/check_output.py'),str(inp),str(ans)],check=True)
        info = dict(case)
        info['input'] = os.path.relpath(str(target/(name+'.in')),str(root))
        info['answer'] = os.path.relpath(str(target/(name+'.ans')),str(root))
        info['elapsed_seconds'] = round(time.monotonic()-start,3)
        info['input_sha256'] = hashlib.sha256(inp.read_bytes()).hexdigest()
        info['answer_sha256'] = hashlib.sha256(ans.read_bytes()).hexdigest()
        # Replace only the two files owned by this case. Never clear a directory.
        for source in (inp,ans):
            # Stage inside target so a Windows-mounted output directory also
            # works when the temporary build directory is on the WSL disk.
            stage = None
            try:
                with tempfile.NamedTemporaryFile(dir=str(target),prefix='.storage-'+name+'-',delete=False) as f:
                    stage = pathlib.Path(f.name)
                shutil.copyfile(str(source),str(stage))
                # Public samples must remain readable after an atomic replace;
                # NamedTemporaryFile creates files as 0600 regardless of umask.
                if public_samples:
                    stage.chmod(0o664)
                os.replace(str(stage),str(target/source.name))
            finally:
                if stage is not None and stage.exists():
                    stage.unlink()
        records.append(info)
manifest = {'status':'generated','generator':'testlib C++17','cases':records}
(root/'review/generated_manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print('Done: {} test cases in {}, {} large samples in {}'.format(len(plan['cases']),data,len(plan['large_samples']),down),flush=True)
PY
