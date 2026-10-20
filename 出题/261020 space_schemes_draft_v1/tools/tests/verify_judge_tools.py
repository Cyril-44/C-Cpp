#!/usr/bin/env python3
"""Positive and negative tests for val.cpp and checker.cpp."""

import json
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def executable(name):
    return str(ROOT / (name + (".exe" if (ROOT / (name + ".exe")).exists() else "")))


def main():
    results = []

    def run(label, command, expected_code, stdin=None):
        process = subprocess.run(command, input=stdin, text=True, capture_output=True, timeout=10)
        if process.returncode != expected_code:
            raise AssertionError(
                f"{label}: expected {expected_code}, got {process.returncode}; {process.stderr}"
            )
        assert not process.stdout, (label, "diagnostics must go to stderr")
        results.append({"label": label, "exit_code": process.returncode, "diagnostic": process.stderr.strip()})

    val_tests = [
        ("general-valid", "2 6\n+ 1 2\n+ 1 1\n+ 2 2\n- 1 1\n- 1 2\n- 2 2\n", [], 0),
        ("singleton-valid", "5 3\n+ 3 3\n- 3 3\n+ 3 3\n", ["--property", "singleton"], 0),
        ("singleton-invalid", "5 1\n+ 2 3\n", ["--property", "singleton"], 1),
        ("disjoint-adjacent-valid", "5 3\n+ 1 2\n+ 3 5\n- 1 2\n", ["--property", "disjoint"], 0),
        ("disjoint-reuse-valid", "5 3\n+ 1 2\n- 1 2\n+ 2 3\n", ["--property", "disjoint"], 0),
        ("disjoint-predecessor-overlap", "5 2\n+ 1 3\n+ 3 4\n", ["--property", "disjoint"], 1),
        ("disjoint-successor-overlap", "5 2\n+ 3 4\n+ 1 3\n", ["--property", "disjoint"], 1),
        ("disjoint-containment", "5 2\n+ 2 3\n+ 1 5\n", ["--property", "disjoint"], 1),
        ("disjoint-same-start", "5 2\n+ 2 3\n+ 2 4\n", ["--property", "disjoint"], 1),
        ("node-non-power-of-two-valid", "5 4\n+ 1 3\n+ 4 5\n+ 1 2\n+ 3 3\n", ["--property", "node"], 0),
        ("node-power-of-two-length-invalid", "5 1\n+ 2 3\n", ["--property", "node"], 1),
        ("node-other-tree-invalid", "5 1\n+ 1 4\n", ["--property", "node"], 1),
        ("insert-only-valid", "5 2\n+ 1 3\n+ 2 4\n", ["--property", "insert-only"], 0),
        ("insert-only-deletion-invalid", "5 2\n+ 1 3\n- 1 3\n", ["--property", "insert-only"], 1),
        ("duplicate-insertion", "5 2\n+ 1 3\n+ 1 3\n", [], 1),
        ("invalid-deletion", "5 1\n- 1 3\n", [], 1),
        ("interval-out-of-range", "5 1\n+ 1 6\n", [], 1),
        ("reversed-interval", "5 1\n+ 3 2\n", [], 1),
        ("invalid-operation", "5 1\n* 1 3\n", [], 1),
        ("missing-operation", "5 2\n+ 1 3\n", [], 1),
        ("missing-endpoint", "5 1\n+ 1\n", [], 1),
        ("non-integer-endpoint", "5 1\n+ 1.0 3\n", [], 1),
        ("overflow-endpoint", "5 1\n+ 1 99999999999999999999999999\n", [], 1),
        ("trailing-token", "5 1\n+ 1 3\n0\n", [], 1),
        ("n-global-bound", "100001 1\n+ 1 1\n", [], 1),
        ("q-global-bound", "5 300001\n", [], 1),
        ("n-zero", "0 1\n", [], 1),
        ("q-zero", "5 0\n", [], 1),
        ("custom-n-bound", "5 1\n+ 1 3\n", ["--max-n", "4"], 1),
        ("custom-q-bound", "5 2\n+ 1 3\n- 1 3\n", ["--max-q", "1"], 1),
        ("custom-bounds-valid", "5 2\n+ 1 3\n- 1 3\n", ["--max-n", "5", "--max-q", "2"], 0),
        ("unknown-option", "5 1\n+ 1 3\n", ["--unknown", "1"], 1),
        ("missing-option-value", "5 1\n+ 1 3\n", ["--max-n"], 1),
        ("unknown-property", "5 1\n+ 1 3\n", ["--property", "unknown"], 1),
        ("max-n-outside-global", "5 1\n+ 1 3\n", ["--max-n", "100001"], 1),
        ("whole-or-singleton-valid", "5 3\n+ 1 5\n+ 3 3\n- 1 5\n", ["--property", "whole-or-singleton"], 0),
        ("whole-or-singleton-invalid", "5 1\n+ 1 4\n", ["--property", "whole-or-singleton"], 1),
        ("short-valid", "5 2\n+ 2 3\n+ 3 3\n", ["--property", "short"], 0),
        ("short-invalid", "5 1\n+ 2 4\n", ["--property", "short"], 1),
        ("exact-n-valid", "5 1\n+ 1 3\n", ["--exact-n", "5"], 0),
        ("exact-n-invalid", "5 1\n+ 1 3\n", ["--exact-n", "4"], 1),
        ("exact-n-conflict", "5 1\n+ 1 3\n", ["--exact-n", "4", "--exact-n", "5"], 1),
        ("max-product-valid", "5 2\n+ 1 3\n- 1 3\n", ["--max-product", "10"], 0),
        ("max-product-invalid", "5 2\n+ 1 3\n- 1 3\n", ["--max-product", "9"], 1),
        ("max-product-no-override", "5 2\n+ 1 3\n- 1 3\n", ["--max-product", "9", "--max-product", "100"], 1),
        ("max-n-no-override", "5 1\n+ 1 3\n", ["--max-n", "4", "--max-n", "100"], 1),
        ("max-q-no-override", "5 2\n+ 1 3\n- 1 3\n", ["--max-q", "1", "--max-q", "100"], 1),
        ("properties-intersect", "5 1\n+ 1 3\n", ["--property", "singleton", "--property", "general"], 1),
        ("subtask1-valid", "1 2\n+ 1 1\n- 1 1\n", ["--subtask", "1"], 0),
        ("subtask1-invalid", "2 1\n+ 1 1\n", ["--subtask", "1"], 1),
        ("subtask2-valid", "8 100\n" + "+ 1 8\n- 1 8\n" * 50, ["--subtask", "2"], 0),
        ("subtask2-n-invalid", "9 1\n+ 1 1\n", ["--subtask", "2"], 1),
        ("subtask2-q-invalid", "8 101\n", ["--subtask", "2"], 1),
        ("subtask2-max-n-override", "9 1\n+ 1 1\n", ["--subtask", "2", "--max-n", "100"], 1),
        ("subtask2-reverse-option-order", "9 1\n+ 1 1\n", ["--max-n", "100", "--subtask", "2"], 1),
        ("subtask2-additional-limit", "8 1\n+ 1 1\n", ["--subtask", "2", "--max-n", "7"], 1),
        ("subtask3-boundary-valid", "100000 20\n" + "+ 1 1\n- 1 1\n" * 10, ["--subtask", "3"], 0),
        ("subtask3-product-invalid", "100000 21\n", ["--subtask", "3"], 1),
        ("subtask3-product-override", "100000 21\n", ["--subtask", "3", "--max-product", "30000000000"], 1),
        ("subtask4-valid", "5 1\n+ 2 2\n", ["--subtask", "4"], 0),
        ("subtask4-property-override", "5 1\n+ 1 3\n", ["--subtask", "4", "--property", "general"], 1),
        ("subtask5-valid", "5 2\n+ 1 2\n+ 3 5\n", ["--subtask", "5"], 0),
        ("subtask5-invalid", "5 2\n+ 1 3\n+ 3 5\n", ["--subtask", "5"], 1),
        ("subtask6-valid", "5 2\n+ 1 5\n+ 2 2\n", ["--subtask", "6"], 0),
        ("subtask6-invalid", "5 1\n+ 1 4\n", ["--subtask", "6"], 1),
        ("subtask7-valid", "5 1\n+ 2 3\n", ["--subtask", "7"], 0),
        ("subtask7-invalid", "5 1\n+ 2 4\n", ["--subtask", "7"], 1),
        ("subtask8-valid", "5 2\n+ 1 3\n+ 2 4\n", ["--subtask", "8"], 0),
        ("subtask8-invalid", "5 2\n+ 1 3\n- 1 3\n", ["--subtask", "8"], 1),
        ("subtask9-valid", "5 1\n+ 1 3\n", ["--subtask", "9"], 0),
        ("subtask9-invalid", "5 1\n+ 2 3\n", ["--subtask", "9"], 1),
        ("subtask10-valid", "5 2\n+ 2 4\n- 2 4\n", ["--subtask", "10"], 0),
        ("subtask10-global-bound", "100001 1\n+ 1 1\n", ["--subtask", "10"], 1),
        ("subtasks-cannot-override", "5 1\n+ 1 3\n", ["--subtask", "4", "--subtask", "10"], 1),
        ("subtask-zero", "5 1\n+ 1 3\n", ["--subtask", "0"], 1),
        ("subtask-eleven", "5 1\n+ 1 3\n", ["--subtask", "11"], 1),
    ]
    for label, data, arguments, expected_code in val_tests:
        run("val-" + label, [executable("val")] + arguments, expected_code, data)

    with tempfile.TemporaryDirectory(dir=ROOT, prefix="judge-tools-") as temporary:
        directory = Path(temporary)
        input_path = directory / "case.in"
        answer_path = directory / "case.ans"
        output_path = directory / "case.out"
        input_path.write_text("2 2\n+ 1 2\n+ 1 1\n", encoding="ascii")
        answer_path.write_text("1 1\n2 2\n", encoding="ascii")
        run("val-input-file", [executable("val"), "--input", str(input_path)], 0)

        checker_tests = [
            ("valid", "1 1\n2 2\n", 0),
            ("whitespace-valid", "  \t1\n1\t2\r\n2   \n", 0),
            ("integer-spelling-valid", "+01 0001\n+2 +002\n", 0),
            ("empty", "", 1),
            ("missing-token", "1 1\n2\n", 1),
            ("extra-token", "1 1\n2 2\n0\n", 1),
            ("extra-word", "1 1\n2 2\nhello\n", 1),
            ("wrong-answer", "1 1\n2 1\n", 1),
            ("non-integer", "1 1\n2 2.0\n", 1),
            ("nan", "1 1\n2 nan\n", 1),
            ("hex", "1 1\n2 0x2\n", 1),
            ("overflow", "1 1\n2 99999999999999999999999999\n", 1),
            ("negative-space", "-1 1\n2 2\n", 1),
            ("too-large-space", "3 1\n2 2\n", 1),
            ("negative-ways", "1 -1\n2 2\n", 1),
            ("too-large-ways", "1 998244353\n2 2\n", 1),
        ]
        command = [executable("checker"), str(input_path), str(output_path), str(answer_path)]
        for label, output, expected_code in checker_tests:
            output_path.write_text(output, encoding="ascii")
            run("checker-" + label, command, expected_code)

        output_path.write_text("1 1\n2 2\n", encoding="ascii")
        for label, bad_answer in [
            ("missing-answer-token", "1 1\n2\n"),
            ("extra-answer-token", "1 1\n2 2\n0\n"),
            ("invalid-answer-token", "1 1\n2 xyz\n"),
            ("invalid-answer-range", "1 1\n2 998244353\n"),
        ]:
            answer_path.write_text(bad_answer, encoding="ascii")
            run("checker-" + label, command, 3)
        run("checker-missing-arguments", [executable("checker")], 3)

    report = {"status": "passed", "tests": len(results), "results": results}
    (ROOT / "judge_tools_report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(results)} positive/negative judge-tool tests")


if __name__ == "__main__":
    main()
