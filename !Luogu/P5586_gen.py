#!/usr/bin/env python3
# Small random test generator + answer generator for Luogu P5586
# Usage:
#   python3 gen_p5586.py [seed] [n_max] [q_max]
# It outputs:
#   P5586.in  P5586.ans
import random, sys

MOD = 10**9 + 7

def gen_case(seed=1, n_max=25, q_max=80, v_max=50):
    rnd = random.Random(seed)
    n = rnd.randint(1, n_max)
    q = rnd.randint(1, q_max)
    a = [rnd.randint(0, v_max) % MOD for _ in range(n)]
    init = a[:]

    last = 0
    in_lines = [f"{n} {q}", " ".join(map(str, init))]
    ans_lines = []

    def rand_lr():
        l = rnd.randint(1, n)
        r = rnd.randint(1, n)
        if l > r: l, r = r, l
        return l, r

    def pick_two_disjoint_equal_len():
        if n < 2:
            return None
        max_len = min(6, max(1, n // 2))
        length = rnd.randint(1, max_len)
        l1 = rnd.randint(1, n - length + 1)
        r1 = l1 + length - 1
        for _ in range(200):
            l2 = rnd.randint(1, n - length + 1)
            r2 = l2 + length - 1
            if r2 < l1 or r1 < l2:
                return l1, r1, l2, r2
        # fallback construction
        if l1 == 1:
            l2 = r1 + 1
        else:
            l2 = 1
        l2 = min(l2, n - length + 1)
        r2 = l2 + length - 1
        if not (r2 < l1 or r1 < l2):
            return None
        return l1, r1, l2, r2

    types = [1,2,3,4,5,6]
    weights = [3,2,2,1,1,1]

    for _ in range(q):
        op = rnd.choices(types, weights=weights, k=1)[0]

        if op == 1:
            l, r = rand_lr()
            in_lines.append(f"1 {l ^ last} {r ^ last}")
            s = sum(a[l-1:r]) % MOD
            ans_lines.append(str(s))
            last = s
        elif op == 2:
            l, r = rand_lr()
            k = rnd.randint(0, 200)
            in_lines.append(f"2 {l ^ last} {r ^ last} {k ^ last}")
            kk = k % MOD
            for i in range(l-1, r):
                a[i] = kk
        elif op == 3:
            l, r = rand_lr()
            k = rnd.randint(0, 200)
            in_lines.append(f"3 {l ^ last} {r ^ last} {k ^ last}")
            kk = k % MOD
            for i in range(l-1, r):
                a[i] = (a[i] + kk) % MOD
        elif op == 4:
            pick = pick_two_disjoint_equal_len()
            if pick is None:
                # fallback to add
                l, r = rand_lr()
                k = rnd.randint(0, 200)
                in_lines.append(f"3 {l ^ last} {r ^ last} {k ^ last}")
                for i in range(l-1, r):
                    a[i] = (a[i] + k) % MOD
                continue
            l1, r1, l2, r2 = pick
            in_lines.append(f"4 {l1 ^ last} {r1 ^ last} {l2 ^ last} {r2 ^ last}")
            seg = a[l1-1:r1]
            a[l2-1:r2] = seg[:]
        elif op == 5:
            pick = pick_two_disjoint_equal_len()
            if pick is None:
                l, r = rand_lr()
                in_lines.append(f"6 {l ^ last} {r ^ last}")
                a[l-1:r] = reversed(a[l-1:r])
                continue
            l1, r1, l2, r2 = pick
            in_lines.append(f"5 {l1 ^ last} {r1 ^ last} {l2 ^ last} {r2 ^ last}")
            seg1 = a[l1-1:r1]
            seg2 = a[l2-1:r2]
            a[l1-1:r1] = seg2
            a[l2-1:r2] = seg1
        else:  # op == 6
            l, r = rand_lr()
            in_lines.append(f"6 {l ^ last} {r ^ last}")
            a[l-1:r] = list(reversed(a[l-1:r]))

    ans_lines.append(" ".join(str(x % MOD) for x in a))
    return "\n".join(in_lines) + "\n", "\n".join(ans_lines) + "\n"

def main():
    seed = int(sys.argv[1]) if len(sys.argv) >= 2 else 1
    n_max = int(sys.argv[2]) if len(sys.argv) >= 3 else 25
    q_max = int(sys.argv[3]) if len(sys.argv) >= 4 else 80

    inp, ans = gen_case(seed=seed, n_max=n_max, q_max=q_max)
    with open("P5586.in", "w", encoding="utf-8") as f:
        f.write(inp)
    with open("P5586.ans", "w", encoding="utf-8") as f:
        f.write(ans)

if __name__ == "__main__":
    main()
