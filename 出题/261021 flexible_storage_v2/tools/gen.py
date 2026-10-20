#!/usr/bin/env python3
"""Reproducible standard-library generator for flexible_storage_v2.

Example:
  python gen.py --n 100000 --q 100000 --shape zigzag_comb \
      --mode adversarial --weights unit --seed 17564 --out case.in

All trees have root 1. Other node labels are shuffled by default. Tree
construction, validation and output are iterative, including comb trees.
"""

from __future__ import annotations

import argparse
import json
import random
import sys
from dataclasses import dataclass
from pathlib import Path


SHAPES = ("mid", "random", "left_comb", "right_comb", "zigzag_comb",
          "heavy_right", "bounded30")
MODES = ("random", "mixed", "node_only", "insert_only", "root_singleton",
         "alternating", "adversarial", "short")
WEIGHTS = ("unit", "random", "large", "tie")


@dataclass
class Tree:
    n: int
    left: list[int]
    right: list[int]
    lo: list[int]
    hi: list[int]
    internal: list[int]
    cherries: list[int]
    max_depth: int


class ActiveSet:
    """O(1) toggle and random active-element selection."""

    def __init__(self):
        self.items = []
        self.position = {}

    def __bool__(self):
        return bool(self.items)

    def toggle(self, interval):
        if interval not in self.position:
            self.position[interval] = len(self.items)
            self.items.append(interval)
            return "+"
        index = self.position.pop(interval)
        last = self.items.pop()
        if index < len(self.items):
            self.items[index] = last
            self.position[last] = index
        return "-"


def build_tree(n, shape, rng):
    size = 2 * n - 1
    left = [-1] * size
    right = [-1] * size
    lo = [0] * size
    hi = [0] * size
    internal = []
    cherries = []
    next_id = 1
    max_depth = 0
    stack = [(0, 1, n, 0)]
    while stack:
        v, l, r, depth = stack.pop()
        lo[v], hi[v] = l, r
        max_depth = max(max_depth, depth)
        length = r - l + 1
        if length == 1:
            continue
        internal.append(v)
        if length == 2:
            cherries.append(v)
        if shape == "mid":
            left_size = (length + 1) // 2
        elif shape == "left_comb":
            left_size = length - 1
        elif shape == "right_comb":
            left_size = 1
        elif shape == "zigzag_comb":
            left_size = length - 1 if depth % 2 == 0 else 1
        elif shape == "heavy_right":
            left_size = rng.choice((1, length // 2, rng.randint(1, length // 2)))
        elif shape == "bounded30":
            remaining = 30 - depth
            if remaining <= 0:
                raise ValueError("bounded30 cannot accommodate this leaf count")
            child_capacity = 1 << (remaining - 1)
            low = max(1, length - child_capacity)
            high = min(length - 1, child_capacity)
            left_size = rng.randint(low, high)
        else:
            left_size = rng.randint(1, length - 1)
        a, b = next_id, next_id + 1
        next_id += 2
        left[v], right[v] = a, b
        mid = l + left_size - 1
        stack.append((b, mid + 1, r, depth + 1))
        stack.append((a, l, mid, depth + 1))
    assert next_id == size
    return Tree(n, left, right, lo, hi, internal, cherries, max_depth)


def make_node_rows(tree, weights, numbering, rng):
    size = len(tree.left)
    labels = list(range(1, size + 1))
    if numbering == "random":
        tail = labels[1:]
        rng.shuffle(tail)
        labels[1:] = tail
    elif numbering == "reverse":
        labels[1:] = reversed(labels[1:])
    else:
        # Actual preorder labels, independent of allocation order.
        stack, label = [0], 1
        while stack:
            v = stack.pop()
            labels[v] = label
            label += 1
            if tree.left[v] >= 0:
                stack.extend((tree.right[v], tree.left[v]))
    rows = [None] * size
    for v in range(size):
        if weights == "unit":
            weight = 1
        elif weights == "tie":
            # Every parent costs exactly as much as its two children.
            weight = tree.hi[v] - tree.lo[v] + 1
        elif weights == "large":
            weight = 10**9 - rng.randrange(1000001)
        else:
            bucket = rng.randrange(4)
            weight = rng.randint(1, 9) if bucket == 0 else (
                rng.randint(1, 10000) if bucket == 1 else rng.randint(10**8, 10**9))
        if tree.left[v] < 0:
            rows[labels[v] - 1] = (0, 0, weight)
        else:
            rows[labels[v] - 1] = (labels[tree.left[v]], labels[tree.right[v]], weight)
    return rows


def interval_of(tree, v):
    return tree.lo[v], tree.hi[v]


def random_interval(n, rng):
    kind = rng.randrange(6)
    if kind == 0:
        l = rng.randint(1, n)
        return l, min(n, l + rng.randrange(4))
    if kind == 1:
        x = rng.randint(1, n)
        return (1, x) if rng.randrange(2) else (x, n)
    l, r = rng.randint(1, n), rng.randint(1, n)
    return min(l, r), max(l, r)


def sharing_pairs(tree, limit=192):
    """Different original intervals sharing one canonical-decomposition node."""
    result = []
    count = len(tree.internal)
    if not count:
        return [((1, 1), (1, 1))]
    indices = sorted({i * (count - 1) // max(1, min(limit, count) - 1)
                      for i in range(min(limit, count))})
    for index in indices:
        v = tree.internal[index]
        a, b = tree.left[v], tree.right[v]
        if tree.hi[b] > tree.lo[b]:
            result.append((interval_of(tree, a), (tree.lo[a], tree.hi[a] + 1)))
        if tree.hi[a] > tree.lo[a]:
            result.append((interval_of(tree, b), (tree.lo[b] - 1, tree.hi[b])))
    return result or [((1, 1), (1, tree.n))]


def adversarial_stream(tree, weights, rng):
    n = tree.n
    if n == 1:
        while True:
            yield (1, 1)
    yield (1, n)
    pairs = sharing_pairs(tree)
    for a, b in pairs:
        # Both original intervals refer to one canonical block. Delete one
        # source while the other remains, then exercise the opposite order.
        yield a
        yield b
        yield a
        yield b
        yield b
        yield a
        yield b
        yield a
    for v in tree.cherries:
        yield interval_of(tree, v)
        if weights != "tie":
            yield (tree.lo[v], tree.lo[v])
    for v in reversed(tree.cherries):
        yield (tree.hi[v], tree.hi[v])
        yield (tree.lo[v], tree.lo[v])
    step = 0
    while True:
        if step % 12 < 8:
            a, b = pairs[(step // 12) % len(pairs)]
            yield (a, b, a, b, b, a, b, a)[step % 12]
        elif step % 12 == 8:
            v = tree.internal[rng.randrange(len(tree.internal))]
            yield interval_of(tree, v)
        elif step % 12 == 9:
            v = tree.internal[rng.randrange(len(tree.internal))]
            # One endpoint lies at a non-root maximal full-node boundary.
            yield (tree.lo[v], max(tree.lo[v], tree.hi[v] - 1))
        else:
            yield random_interval(n, rng)
        step += 1


def make_operations(tree, q, mode, weights, rng):
    n = tree.n
    if mode == "insert_only" and q > n * (n + 1) // 2:
        raise ValueError("insert_only requires q <= n*(n+1)/2")
    active = ActiveSet()
    operations = []
    fallback_l = fallback_r = 1
    pairs = sharing_pairs(tree)
    adversarial = adversarial_stream(tree, weights, rng)
    root_cycle = 2 * n + 2
    short_prefix = []
    if mode == "short":
        for a, b in pairs[:16]:
            if a[1] - a[0] <= 1 and b[1] - b[0] <= 1:
                short_prefix.extend((a, b, a, b))
        for v in tree.cherries:
            short_prefix.append(interval_of(tree, v))
            if weights != "tie":
                short_prefix.append((tree.lo[v], tree.lo[v]))
    for step in range(q):
        if mode == "short" and step < len(short_prefix):
            interval = short_prefix[step]
        elif mode == "root_singleton":
            at = step % root_cycle
            if n == 1:
                interval = (1, 1)
            elif at == 0 or at == 2 * n + 1:
                interval = (1, n)
            elif at <= n:
                interval = (at, at)
            else:
                x = 2 * n + 1 - at
                interval = (x, x)
        elif mode == "alternating":
            a, b = pairs[(step // 8) % len(pairs)]
            interval = (a, b, a, b, b, a, b, a)[step % 8]
        elif mode == "adversarial":
            interval = next(adversarial)
        elif mode == "insert_only":
            interval = None
            for attempt in range(24):
                if attempt % 4 == 0:
                    candidate = interval_of(tree, rng.randrange(len(tree.left)))
                else:
                    candidate = random_interval(n, rng)
                if candidate not in active.position:
                    interval = candidate
                    break
            if interval is None:
                while (fallback_l, fallback_r) in active.position:
                    fallback_r += 1
                    if fallback_r > n:
                        fallback_l += 1
                        fallback_r = fallback_l
                assert fallback_l <= n
                interval = (fallback_l, fallback_r)
        elif active and ((mode == "mixed" and len(active.items) >= 256)
                         or (mode == "random" and len(active.items) >= 512)
                         or rng.random() < 0.43):
            interval = active.items[rng.randrange(len(active.items))]
        elif mode == "short":
            l = rng.randint(1, n)
            interval = (l, l if l == n or rng.randrange(2) == 0 else l + 1)
        elif mode == "node_only":
            interval = interval_of(tree, rng.randrange(len(tree.left)))
        elif mode == "mixed" and step % 5 < 2:
            interval = interval_of(tree, rng.randrange(len(tree.left)))
        elif mode == "mixed" and step % 5 == 2:
            interval = (1, n)
        else:
            interval = random_interval(n, rng)
        operation = active.toggle(interval)
        if mode == "insert_only":
            assert operation == "+"
        operations.append((operation, interval[0], interval[1]))
    return operations


def generate_case(n, q, shape="mid", mode="random", weights="unit", seed=1,
                  numbering="random"):
    if not (1 <= n <= 100000 and 1 <= q <= 100000):
        raise ValueError("n and q must both lie in [1,100000]")
    rng = random.Random(seed)
    tree = build_tree(n, shape, rng)
    rows = make_node_rows(tree, weights, numbering, rng)
    operations = make_operations(tree, q, mode, weights, rng)
    return rows, operations, {"n": n, "q": q, "shape": shape, "mode": mode,
                              "weights": weights, "seed": seed,
                              "numbering": numbering, "max_leaf_depth": tree.max_depth}


def validate_generated(n, rows, operations, shape, mode):
    """Independent structural/legality audit, used by --self-test."""
    size = 2 * n - 1
    assert len(rows) == size
    indegree = [0] * size
    for ls, rs, weight in rows:
        assert 1 <= weight <= 10**9
        assert (ls == 0) == (rs == 0)
        if ls:
            assert 1 <= ls <= size and 1 <= rs <= size and ls != rs
            indegree[ls - 1] += 1
            indegree[rs - 1] += 1
    assert indegree[0] == 0 and all(x == 1 for x in indegree[1:])
    intervals = set()
    lengths = [0] * size
    lo = [0] * size
    leaf = 0
    visited = set()
    max_depth = 0
    stack = [(0, 0, False)]
    while stack:
        v, depth, exiting = stack.pop()
        ls, rs, _ = rows[v]
        if not exiting:
            assert v not in visited
            visited.add(v)
            max_depth = max(max_depth, depth)
            if not ls:
                leaf += 1
                lengths[v] = 1
                lo[v] = leaf
                intervals.add((leaf, leaf))
            else:
                stack.extend(((v, depth, True), (rs - 1, depth + 1, False),
                              (ls - 1, depth + 1, False)))
        else:
            a, b = lengths[ls - 1], lengths[rs - 1]
            lengths[v] = a + b
            lo[v] = lo[ls - 1]
            intervals.add((lo[v], lo[v] + a + b - 1))
            if shape == "mid":
                assert a == (a + b + 1) // 2
            elif shape == "left_comb":
                assert b == 1
            elif shape == "right_comb":
                assert a == 1
            elif shape == "heavy_right":
                assert b >= a
    assert leaf == n and len(visited) == size
    if shape == "bounded30":
        assert max_depth <= 30
    active = set()
    for operation, l, r in operations:
        assert 1 <= l <= r <= n
        interval = (l, r)
        if operation == "+":
            assert interval not in active
            active.add(interval)
        else:
            assert operation == "-" and interval in active
            active.remove(interval)
        if mode == "node_only":
            assert interval in intervals
        elif mode == "insert_only":
            assert operation == "+"
        elif mode == "root_singleton":
            assert l == r or (l == 1 and r == n)
        elif mode == "short":
            assert r - l <= 1


def self_test():
    tests = 0
    for n in (1, 2, 7, 19):
        for shape in SHAPES:
            for mode in MODES:
                for weights in WEIGHTS:
                    q = min(43, n * (n + 1) // 2) if mode == "insert_only" else 43
                    rows, operations, _ = generate_case(n, q, shape, mode, weights,
                                                       17564 + tests)
                    validate_generated(n, rows, operations, shape, mode)
                    tests += 1
    for shape in ("left_comb", "right_comb", "zigzag_comb"):
        rows, operations, _ = generate_case(100000, 7, shape, "adversarial",
                                           "large", 20261020)
        validate_generated(100000, rows, operations, shape, "adversarial")
        tests += 1
    first = generate_case(37, 101, "random", "mixed", "tie", 998244353)
    second = generate_case(37, 101, "random", "mixed", "tie", 998244353)
    assert first == second
    try:
        generate_case(1, 2, mode="insert_only")
    except ValueError:
        pass
    else:
        raise AssertionError("invalid insert_only capacity was not rejected")
    print(json.dumps({"status": "passed", "cases": tests,
                      "deterministic_seed": True, "invalid_insert_only_rejected": True}))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--n", type=int)
    parser.add_argument("--q", type=int)
    parser.add_argument("--shape", choices=SHAPES, default="mid")
    parser.add_argument("--mode", choices=MODES, default="random")
    parser.add_argument("--weights", choices=WEIGHTS, default="unit")
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--numbering", choices=("random", "preorder", "reverse"),
                        default="random")
    parser.add_argument("--out", type=Path)
    parser.add_argument("--self-test", action="store_true")
    args = parser.parse_args()
    if args.self_test:
        self_test()
        return
    if args.n is None or args.q is None or args.out is None:
        parser.error("--n, --q and --out are required unless --self-test is used")
    try:
        rows, operations, metadata = generate_case(
            args.n, args.q, args.shape, args.mode, args.weights, args.seed, args.numbering)
    except ValueError as error:
        parser.error(str(error))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("w", encoding="ascii", newline="\n") as output:
        output.write(f"{args.n} {args.q}\n")
        for ls, rs, weight in rows:
            output.write(f"{ls} {rs} {weight}\n")
        for operation, l, r in operations:
            output.write(f"{operation} {l} {r}\n")
    print(json.dumps({**metadata, "out": str(args.out)}, ensure_ascii=False), file=sys.stderr)


if __name__ == "__main__":
    main()
