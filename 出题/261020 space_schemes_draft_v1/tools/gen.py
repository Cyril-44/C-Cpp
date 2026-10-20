#!/usr/bin/env python3
"""Deterministic, legal input generator. Python 3.10+, standard library only."""
import argparse
import random
import sys
from pathlib import Path


def tree_intervals(n):
    result = []
    stack = [(1, n)]
    while stack:
        l, r = stack.pop()
        result.append((l, r))
        if l < r:
            mid = (l + r) // 2
            stack.append((mid + 1, r))
            stack.append((l, mid))
    return result


class Operations:
    def __init__(self, n, q, rng):
        self.n, self.q, self.rng = n, q, rng
        self.ops = []
        self.active = []
        self.where = {}

    def add(self, interval):
        if len(self.ops) >= self.q or interval in self.where:
            return False
        l, r = interval
        assert 1 <= l <= r <= self.n
        self.where[interval] = len(self.active)
        self.active.append(interval)
        self.ops.append(('+', l, r))
        return True

    def remove(self, interval):
        if len(self.ops) >= self.q or interval not in self.where:
            return False
        index = self.where.pop(interval)
        last = self.active.pop()
        if index < len(self.active):
            self.active[index] = last
            self.where[last] = index
        self.ops.append(('-', *interval))
        return True

    def toggle(self, interval):
        return self.remove(interval) if interval in self.where else self.add(interval)

    def random_remove(self):
        return self.remove(self.rng.choice(self.active)) if self.active else False


def generate(n, q, seed, mode):
    if not (1 <= n <= 100000 and 1 <= q <= 300000):
        raise ValueError('n or q outside global limits')
    rng = random.Random(seed)
    out = Operations(n, q, rng)
    nodes = tree_intervals(n)

    def random_interval():
        kind = rng.randrange(12)
        if kind == 0: return (1, n)
        if kind == 1:
            x = rng.randint(1, n)
            return (x, x)
        if kind == 2: return (1, rng.randint(1, n))
        if kind == 3: return (rng.randint(1, n), n)
        if kind == 4: return rng.choice(nodes)
        if kind == 5:
            l = rng.randint(1, n)
            return (l, min(n, l + rng.randint(0, 31)))
        l, r = rng.randint(1, n), rng.randint(1, n)
        return (min(l, r), max(l, r))

    def fill_random(candidate=random_interval, insert_only=False, add_bias=0.60):
        while len(out.ops) < q:
            if not insert_only and out.active and rng.random() > add_bias:
                out.random_remove()
                continue
            done = False
            for _ in range(40):
                if out.add(candidate()):
                    done = True
                    break
            if not done:
                if insert_only:
                    # Dense small universes: select a missing interval deterministically.
                    for l in range(1, n + 1):
                        for r in range(l, n + 1):
                            if out.add((l, r)):
                                done = True
                                break
                        if done: break
                    if not done: raise ValueError('too many distinct insertions')
                elif not out.random_remove():
                    raise ValueError('candidate pool is empty')

    if mode == 'gray':
        if n > 5: raise ValueError('gray mode requires n <= 5')
        universe = [(l, r) for l in range(1, n + 1) for r in range(l, n + 1)]
        period = 1 << len(universe)
        previous = 0
        for i in range(1, q + 1):
            step = i % period
            mask = step ^ (step >> 1)
            bit = (mask ^ previous).bit_length() - 1
            out.toggle(universe[bit])
            previous = mask
    elif mode == 'singletons':
        fill_random(lambda: (lambda x: (x, x))(rng.randint(1, n)))
    elif mode == 'whole-singleton':
        out.add((1, n))
        points = list(range(1, n + 1))
        rng.shuffle(points)
        for x in points:
            out.add((x, x))
            if len(out.ops) == q: break
        for x in range(1, n + 1):
            out.remove((x, x))
            if len(out.ops) == q: break
        while len(out.ops) < q:
            if rng.randrange(25) == 0:
                out.toggle((1, n))
            else:
                x = rng.randint(1, n)
                out.toggle((x, x))
    elif mode == 'short':
        for l, r in nodes:
            if r == l + 1:
                out.add((l, r))
                out.add((l, l))
                if len(out.ops) == q: break
        def short_interval():
            l = rng.randint(1, n)
            return (l, min(n, l + rng.randrange(2)))
        fill_random(short_interval)
    elif mode == 'node-only':
        fill_random(lambda: rng.choice(nodes))
    elif mode == 'insert-only':
        if q > n * (n + 1) // 2: raise ValueError('too many insertions')
        fill_random(insert_only=True)
    elif mode == 'disjoint':
        zones = []
        l = 1
        while l <= n:
            r = min(n, l + rng.randint(0, max(1, min(n, 193))))
            zones.append((l, r))
            l = r + 1
        current = [None] * len(zones)
        while len(out.ops) < q:
            j = rng.randrange(len(zones))
            if current[j] is not None:
                out.remove(current[j])
                current[j] = None
            else:
                l, r = zones[j]
                a, b = rng.randint(l, r), rng.randint(l, r)
                current[j] = (min(a, b), max(a, b))
                out.add(current[j])
    elif mode == 'ties':
        cherries = [v for v in nodes if v[1] == v[0] + 1]
        if not cherries:
            fill_random()
        else:
            for l, r in cherries:
                out.add((l, r))
                out.add((l, l))
                if len(out.ops) == q: break
            while len(out.ops) < q:
                l, r = rng.choice(cherries)
                out.toggle((l, l) if rng.randrange(2) else (r, r))
    elif mode == 'prefix-wave':
        pool = [(1, r) for r in range(1, n + 1)]
        rng.shuffle(pool)
        while len(out.ops) < q:
            for interval in pool:
                out.add(interval)
                if len(out.ops) == q: break
            rng.shuffle(pool)
            for interval in pool:
                out.remove(interval)
                if len(out.ops) == q: break
            rng.shuffle(pool)
    elif mode == 'hierarchy':
        for interval in nodes:
            out.add(interval)
            if len(out.ops) == q: break
        fill_random(lambda: rng.choice(nodes))
    elif mode == 'nested-path':
        path = []
        l, r = 1, n
        while True:
            path.append((l, r))
            if l == r: break
            mid = (l + r) // 2
            if rng.randrange(2): l = mid + 1
            else: r = mid
        while len(out.ops) < q:
            for interval in path: out.add(interval)
            order = path[:]
            rng.shuffle(order)
            for interval in order: out.remove(interval)
    elif mode == 'boundary':
        pool = {(1, n), (1, 1), (n, n)}
        for p in [2, 3, n // 2, (n + 1) // 2, n - 1, n - 2]:
            if 1 <= p <= n:
                pool.update({(1, p), (p, n), (p, p)})
                if p < n: pool.add((p, p + 1))
        ordered = sorted(pool)
        for interval in ordered: out.add(interval)
        fill_random(lambda: rng.choice(ordered))
    elif mode == 'shared':
        mid = (n + 1) // 2
        pool = [(1, r) for r in range(mid, n + 1)]
        pool += [(l, n) for l in range(1, mid + 1)]
        pool = sorted(set(pool))
        rng.shuffle(pool)
        for interval in pool:
            out.add(interval)
            if len(out.ops) == q: break
        fill_random(lambda: rng.choice(pool), add_bias=0.45)
    elif mode == 'random':
        fill_random()
    elif mode == 'mixed':
        out.add((1, n))
        for l, r in nodes:
            if r == l + 1:
                out.add((l, r))
                out.add((l, l))
            if len(out.ops) >= q // 4: break
        endpoints = list(range((n + 1) // 2, n + 1))
        rng.shuffle(endpoints)
        for r in endpoints:
            out.add((1, r))
            if len(out.ops) >= q // 2: break
        fill_random(add_bias=0.48)
    else:
        raise ValueError('unknown mode: ' + mode)
    assert len(out.ops) == q
    return out.ops


def write_case(path, n, operations):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open('w', encoding='ascii', newline='\n') as f:
        f.write(f'{n} {len(operations)}\n')
        for op, l, r in operations:
            f.write(f'{op} {l} {r}\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--n', type=int, required=True)
    parser.add_argument('--q', type=int, required=True)
    parser.add_argument('--seed', type=int, required=True)
    parser.add_argument('--mode', default='random', choices=[
        'gray', 'random', 'singletons', 'node-only', 'insert-only', 'disjoint',
        'ties', 'prefix-wave', 'hierarchy', 'nested-path', 'boundary', 'shared',
        'whole-singleton', 'short', 'mixed'])
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    operations = generate(args.n, args.q, args.seed, args.mode)
    if args.output:
        write_case(args.output, args.n, operations)
    else:
        sys.stdout.write(f'{args.n} {args.q}\n')
        sys.stdout.writelines(f'{op} {l} {r}\n' for op, l, r in operations)


if __name__ == '__main__':
    main()
