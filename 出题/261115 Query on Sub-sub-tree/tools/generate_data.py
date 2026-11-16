#!/usr/bin/env python3
"""Generate deterministic input tests for the ssubtr problem."""

from pathlib import Path
import random

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / "data"


def edge_weight(i: int, seed: int) -> int:
    return (i * 73 + seed * 29 + (i * i // 7)) % 100 + 1


def make_tree(kind: str, n: int, seed: int = 1, constant_weight=None):
    rng = random.Random(seed)
    parent = [0] * (n + 1)
    weight = [0] * (n + 1)
    for i in range(2, n + 1):
        if kind == "chain":
            p = i - 1
        elif kind == "star":
            p = 1
        elif kind == "balanced":
            p = i // 2
        elif kind == "random":
            p = rng.randrange(1, i)
        elif kind == "comb":
            p = i - 1 if i % 2 else i - 2
            p = max(1, p)
        elif kind == "broom":
            handle = max(2, n // 2)
            p = i - 1 if i <= handle else handle - 1
        else:
            raise ValueError(kind)
        parent[i] = p
        weight[i] = constant_weight if constant_weight is not None else edge_weight(i, seed)
    return parent, weight


def tree_info(parent, n):
    children = [[] for _ in range(n + 1)]
    depth = [0] * (n + 1)
    sub_size = [1] * (n + 1)
    for u in range(2, n + 1):
        children[parent[u]].append(u)
        depth[u] = depth[parent[u]] + 1
    # Parent IDs are smaller than child IDs, so reverse order accumulates sizes.
    for u in range(n, 1, -1):
        sub_size[parent[u]] += sub_size[u]
    tin = [0] * (n + 1)
    order = [0] * n
    timer = 0
    stack = [(1, 0)]
    while stack:
        u, state = stack.pop()
        if state == 0:
            tin[u] = timer
            order[timer] = u
            timer += 1
            stack.append((u, 1))
            for v in reversed(children[u]):
                stack.append((v, 0))
    tout = [tin[u] + sub_size[u] - 1 for u in range(n + 1)]
    large = [u for u in range(1, n + 1) if sub_size[u] >= max(8, n // 64)]
    internal = [u for u in range(1, n + 1) if children[u]]
    leaves = [u for u in range(1, n + 1) if not children[u]]
    return children, depth, sub_size, tin, tout, order, large, internal, leaves


def random_ops(parent, n, m, seed):
    rng = random.Random(seed)
    _, depth, _, tin, tout, order, large, internal, leaves = tree_info(parent, n)
    ops = []
    for j in range(m):
        if j % 5 in (0, 2, 4):
            # Mix whole-tree, large-subtree, internal, leaf, and arbitrary updates.
            mode = j % 10
            if mode == 0:
                x = 1
            elif mode in (2, 4):
                pool = large or internal or [1]
                x = rng.choice(pool)
            elif mode == 6:
                x = rng.choice(internal or [1])
            else:
                x = rng.randint(1, n)

            y_mode = j % 4
            if y_mode == 0:
                y = order[rng.randint(tin[x], tout[x])]
            elif y_mode == 1:
                y = n
            elif y_mode == 2:
                y = 1 if x != 1 else rng.randint(1, n)
            else:
                y = rng.randint(1, n)
            ops.append(f"1 {x} {y}")
        else:
            if j % 10 == 1:
                x = 1
            elif j % 10 == 3 and large:
                x = rng.choice(large)
            elif j % 10 == 7 and leaves:
                x = rng.choice(leaves)
            else:
                x = rng.randint(1, n)
            ops.append(f"2 {x}")
    return ops


def write_case(group, name, parent, weight, ops):
    n = len(parent) - 1
    directory = DATA / group
    directory.mkdir(parents=True, exist_ok=True)
    lines = [f"{n} {len(ops)}"]
    lines.extend(f"{parent[i]} {weight[i]}" for i in range(2, n + 1))
    lines.extend(ops)
    (directory / f"{name}.in").write_text("\n".join(lines) + "\n")


def make_small_traps():
    n = 15
    parent = [0] * (n + 1)
    weight = [0] * (n + 1)
    pairs = [
        (0, 0), (0, 0),
        (1, 3), (1, 5), (2, 7), (2, 2), (4, 11), (4, 13),
        (3, 17), (3, 19), (8, 23), (8, 29), (5, 31), (5, 37),
        (9, 41), (9, 43),
    ]
    for i in range(2, n + 1):
        parent[i], weight[i] = pairs[i]
    ops = [
        "1 2 6", "2 2",       # y lies in a descendant branch; x itself is selected.
        "1 2 10", "2 2",      # y is outside subtree(2); affected vertices span branches.
        "1 1 14", "2 1",      # root update excludes its direct children.
        "1 5 13", "2 5",      # x is a leaf-side internal node.
        "2 4", "2 6", "2 10", "2 15",
        "1 3 8", "2 3", "2 1", "2 2",
        "1 9 14", "2 9", "2 14", "2 1",
    ]
    write_case("subtask1", "02_traps", parent, weight, ops)


def main():
    # Smallest boundary and hand-built wrong-answer traps.
    p, w = make_tree("star", 1, 1)
    write_case("subtask1", "01_singleton", p, w,
               ["1 1 1", "2 1"] * 5)
    make_small_traps()
    p, w = make_tree("random", 100, 11)
    write_case("subtask1", "03_random_100", p, w, random_ops(p, 100, 100, 12))

    # Maximum square-brute cases for the second group.
    for name, kind, seed in [("01_balanced_1000", "balanced", 21),
                             ("02_comb_1000", "comb", 22)]:
        p, w = make_tree(kind, 1000, seed)
        write_case("subtask2", name, p, w, random_ops(p, 1000, 1000, seed + 100))
    p, w = make_tree("chain", 1000, 23, constant_weight=100)
    write_case("subtask2", "03_int32_chain_1000", p, w,
               [op for _ in range(500) for op in ("1 1 1", "2 1")])

    # Chain: 50,000 large updates and 50,000 queries. Constant maximum weights
    # make 32-bit accumulators fail quickly and exercise both sides of y.
    n = 100_000
    p, w = make_tree("chain", n, 31, constant_weight=100)
    ops = []
    for j in range(50_000):
        ops.append("1 1 1")
        ops.append("2 1")
        if j + 1 < 50_000:
            # Keep the operation count exactly 100,000 by replacing every tenth
            # pair with an update/query whose y is at the far end of the chain.
            if j % 10 == 4:
                ops[-2] = f"1 1 {n}"
    write_case("subtask3", "01_chain_overflow_100k", p, w, ops)
    p, w = make_tree("chain", 30_000, 32)
    write_case("subtask3", "02_chain_mixed_30k", p, w,
               random_ops(p, 30_000, 50_000, 33))

    # Star: every update changes only a[x], but y may be in another branch.
    p, w = make_tree("star", 100_000, 41)
    write_case("subtask4", "01_star_100k", p, w,
               random_ops(p, 100_000, 100_000, 42))

    # General full-constraint tests with varied shapes and edge weights.
    for name, kind, seed in [
        ("01_balanced_100k", "balanced", 51),
        ("02_random_100k", "random", 52),
        ("03_comb_100k", "comb", 53),
        ("04_broom_100k", "broom", 54),
    ]:
        p, w = make_tree(kind, 100_000, seed)
        write_case("subtask5", name, p, w,
                   random_ops(p, 100_000, 100_000, seed + 1000))


if __name__ == "__main__":
    main()
