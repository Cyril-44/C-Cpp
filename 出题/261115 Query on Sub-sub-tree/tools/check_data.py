#!/usr/bin/env python3
"""Check generated input constraints and answer line counts."""

from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / "data"


def check_file(path: Path):
    tokens = path.read_text().split()
    it = iter(tokens)
    n, m = int(next(it)), int(next(it))
    parent = [0] * (n + 1)
    for i in range(2, n + 1):
        parent[i] = int(next(it))
        w = int(next(it))
        assert 1 <= parent[i] < i, (path, i, parent[i])
        assert 1 <= w <= 100, (path, i, w)
    query_count = 0
    for _ in range(m):
        op = int(next(it))
        x = int(next(it))
        assert 1 <= x <= n, (path, x)
        if op == 1:
            y = int(next(it))
            assert 1 <= y <= n, (path, y)
        else:
            assert op == 2, (path, op)
            query_count += 1
    try:
        next(it)
        raise AssertionError(f"trailing input tokens in {path}")
    except StopIteration:
        pass

    group = path.parent.name
    if group == "subtask1":
        assert n <= 100 and m <= 100, path
    elif group == "subtask2":
        assert n <= 1000 and m <= 1000, path
    elif group == "subtask3":
        assert all(parent[i] == i - 1 for i in range(2, n + 1)), path
    elif group == "subtask4":
        assert all(parent[i] == 1 for i in range(2, n + 1)), path

    answer = path.with_suffix(".ans")
    assert answer.exists(), f"missing answer: {answer}"
    answer_lines = answer.read_text().splitlines()
    assert len(answer_lines) == query_count, (path, query_count, len(answer_lines))
    for line in answer_lines:
        assert line and line.lstrip("-").isdigit(), (answer, line)
    return n, m, query_count


def main():
    files = sorted(DATA.glob("subtask*/*.in"))
    assert files, "no generated input files"
    for path in files:
        n, m, q = check_file(path)
        print(f"PASS {path.relative_to(ROOT)}: n={n}, m={m}, queries={q}")
    print(f"Checked {len(files)} inputs and answers.")


if __name__ == "__main__":
    main()
