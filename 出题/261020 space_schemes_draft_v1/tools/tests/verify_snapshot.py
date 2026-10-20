#!/usr/bin/env python3
"""Check snapshot oracle against brute/recompute and exact singleton answers."""

import json
import random
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def executable(name):
    return str(ROOT / (name + (".exe" if (ROOT / (name + ".exe")).exists() else "")))


def main():
    results = []
    with tempfile.TemporaryDirectory(dir=ROOT, prefix="snapshots-") as temporary:
        directory = Path(temporary)
        input_path = directory / "case.in"
        answer_path = directory / "case.ans"

        def check(label, input_text, answer_text, expected=0, checks=None):
            input_path.write_text(input_text, encoding="ascii")
            answer_path.write_text(answer_text, encoding="ascii")
            command = [executable("snapshot_oracle"), str(input_path), str(answer_path)]
            if checks is not None:
                command.append(str(checks))
            process = subprocess.run(command, text=True, capture_output=True, timeout=60)
            assert process.returncode == expected, (label, process.stderr)
            results.append({"label": label, "exit_code": process.returncode, "diagnostic": process.stderr.strip()})

        rng = random.Random(20261020)
        for n, q, oracle in ((8, 1500, "brute"), (257, 3000, "recompute")):
            active = set()
            operations = []
            for _ in range(q):
                l = rng.randrange(1, n + 1)
                r = rng.randrange(l, n + 1)
                interval = (l, r)
                if interval in active:
                    active.remove(interval)
                    operations.append(f"- {l} {r}")
                else:
                    active.add(interval)
                    operations.append(f"+ {l} {r}")
            data = f"{n} {q}\n" + "\n".join(operations) + "\n"
            answer = subprocess.run([executable(oracle)], input=data, text=True, capture_output=True, check=True).stdout
            check(f"random-n{n}-default-snapshots", data, answer)
            check(f"random-n{n}-all-snapshots", data, answer, checks=q)
            check(f"random-n{n}-first-last-only", data, answer, checks=0)

        n, q = 100000, 100000
        active = set()
        operations = []
        answers = []
        for _ in range(q):
            x = rng.randrange(1, n + 1)
            if x in active:
                active.remove(x)
                operations.append(f"- {x} {x}")
            else:
                active.add(x)
                operations.append(f"+ {x} {x}")
            answers.append(f"{len(active)} 1")
        data = f"{n} {q}\n" + "\n".join(operations) + "\n"
        answer = "\n".join(answers) + "\n"
        check("large-exact-singleton-answers", data, answer)
        for step in (1, 16, 50000, q):
            corrupted = answers.copy()
            space = int(corrupted[step - 1].split()[0])
            corrupted[step - 1] = f"{space} 2"
            check(f"detect-wrong-snapshot-{step}", data, "\n".join(corrupted) + "\n", 1)
        check("detect-trailing-answer-token", data, answer + "0\n", 1)
        check("detect-trailing-input-token", data + "0\n", answer, 1)
        check("detect-missing-final-answer", data, "\n".join(answers[:-1]) + "\n", 1)
        check("detect-invalid-final-answer", data, "\n".join(answers[:-1]) + "\n0 nope\n", 1)
        check("reject-negative-checks", data, answer, 1, checks=-1)
        check("reject-checks-trailing-text", data, answer, 1, checks="40 text")

    report = {"status": "passed", "tests": len(results), "results": results}
    (ROOT / "snapshot_report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(results)} snapshot-oracle tests")


if __name__ == "__main__":
    main()
