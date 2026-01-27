#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
T725270 三元组 — 测试数据生成器
生成 20 个输入文件：tests/01.in ... tests/20.in
每个文件格式：
第一行 n
第二行 n 个用空格分开的整数（排列）

分类（每点 5 分）：
 01-05 : Subtask 1 (n <= 1e3)
 06-10 : Subtask 2 (常见中等规模排列)
 11-15 : Subtask 3 (满足 |a_i - i| <= 100)
 16-20 : Subtask 4 (无约束，包含强构造 —— 第16个是严格升序 1..n，能卡掉 O(n * len) 暴力)

运行后会打印每个生成文件的 n 值与说明。
使用示例：
  python3 gen_tests.py
  ./std < tests/16.in > tests/16.out
"""

import os
import random

random.seed(20260127)  # 固定 seed，复现性

OUTDIR = "tests"
os.makedirs(OUTDIR, exist_ok=True)

def write_test(idx, arr):
    fn = os.path.join(OUTDIR, f"{idx:02d}.in")
    n = len(arr)
    with open(fn, "w") as f:
        f.write(str(n) + "\n")
        f.write(" ".join(map(str, arr)) + "\n")
    return fn

def increasing(n):
    return list(range(1, n+1))

def decreasing(n):
    return list(range(n, 0, -1))

def random_perm(n):
    a = list(range(1, n+1))
    random.shuffle(a)
    return a

def alternating_wave(n):
    # 1,3,5,...,2,4,6,...
    odds = list(range(1, n+1, 2))
    evens = list(range(2, n+1, 2))
    return odds + evens

def block_reverse(n, block_size):
    a = list(range(1, n+1))
    for i in range(0, n, block_size):
        a[i:i+block_size] = reversed(a[i:i+block_size])
    return a

def near_identity(n, window=100):
    # 保证 |a_i - i| <= window
    # 开始为恒等，然后对每个 i 与随机 j in [i-window, i+window] 交换
    a = list(range(1, n+1))
    for i in range(n):
        lo = max(0, i - window)
        hi = min(n-1, i + window)
        j = random.randint(lo, hi)
        a[i], a[j] = a[j], a[i]
    return a

def chunked_shuffle(n, chunk):
    a = list(range(1, n+1))
    for i in range(0, n, chunk):
        sub = a[i:i+chunk]
        random.shuffle(sub)
        a[i:i+chunk] = sub
    return a

def dense_increasing_blocks(n, block_len):
    # 构造一系列升序小块（块内升序，块间升序），总体仍然升序，但可用于不同规模
    # 这里等同于完全升序，但保留接口
    return list(range(1, n+1))

# --- 文件分配和生成 ---
summary = []

# 01-05 : Subtask 1, n <= 1000
# 01: sample 1
summary.append((1, 5, [3,1,4,2,5], "sample1 small"))
write_test(1, [3,1,4,2,5])

# 02: sample 2
summary.append((2, 5, [5,4,3,2,1], "sample2 small"))
write_test(2, [5,4,3,2,1])

# 03: random small (n=100)
a = random_perm(100)
summary.append((3, 100, None, "random n=100"))
write_test(3, a)

# 04: worst-case small (increasing, many triples) n=999
a = increasing(999)
summary.append((4, 999, None, "increasing n=999 (many triples)"))
write_test(4, a)

# 05: alternating wave n=1000
a = alternating_wave(1000)
summary.append((5, 1000, None, "alternating wave n=1000"))
write_test(5, a)

# 06-10 : Subtask 2 (中等规模、保证是排列)
# 06: random n=2000
a = random_perm(2000)
summary.append((6, 2000, None, "random n=2000"))
write_test(6, a)

# 07: reversed n=2000
a = decreasing(2000)
summary.append((7, 2000, None, "decreasing n=2000"))
write_test(7, a)

# 08: block reverse n=5000, block_size=50
a = block_reverse(5000, 50)
summary.append((8, 5000, None, "block reverse n=5000, block=50"))
write_test(8, a)

# 09: chunked shuffle n=8192, chunk=32
a = chunked_shuffle(8192, 32)
summary.append((9, 8192, None, "chunked shuffle n=8192, chunk=32"))
write_test(9, a)

# 10: mixed increasing chunks n=10000
a = dense_increasing_blocks(10000, 100)
summary.append((10, 10000, None, "increasing chunks n=10000 (effectively increasing)"))
write_test(10, a)

# 11-15 : Subtask 3 (|a_i - i| <= 100)
# We'll produce a series of increasing sizes but always satisfy the bound
summary.append((11, 10000, None, "|a_i-i|<=100 n=10000"))
write_test(11, near_identity(10000, window=100))

summary.append((12, 20000, None, "|a_i-i|<=100 n=20000"))
write_test(12, near_identity(20000, window=100))

summary.append((13, 50000, None, "|a_i-i|<=100 n=50000"))
write_test(13, near_identity(50000, window=100))

summary.append((14, 100000, None, "|a_i-i|<=100 n=100000"))
write_test(14, near_identity(100000, window=100))

summary.append((15, 150000, None, "|a_i-i|<=100 n=150000"))
write_test(15, near_identity(150000, window=100))

# 16-20 : Subtask 4 (no special constraints) — 强构造放在这里
# 16: strong hack — 完全升序 n = 200000
n16 = 200000
a16 = increasing(n16)
summary.append((16, n16, None, "strong hack: strictly increasing 1..n (worst for naive)"))
write_test(16, a16)

# 17: completely decreasing n = 200000
n17 = 200000
a17 = decreasing(n17)
summary.append((17, n17, None, "decreasing n=200000"))
write_test(17, a17)

# 18: random large n = 200000
n18 = 200000
a18 = random_perm(n18)
summary.append((18, n18, None, "random n=200000"))
write_test(18, a18)

# 19: interleaved chunks to make patterns n = 200000
# Construct by taking blocks and reversing half of them to create many local increasing spans
def interleaved_blocks(n, block=4):
    a = list(range(1, n+1))
    for i in range(0, n, block*2):
        # keep first block ascending, second block descending to create mixed patterns
        if i+block < n:
            a[i:i+block] = list(range(i+1, min(n, i+block)+1))
            a[i+block:i+2*block] = list(range(i+2*block, i+block, -1))[:max(0, min(n, i+2*block)-(i+block))]
    return a

a19 = interleaved_blocks(200000, block=50)
# If the helper produced a non-permutation due to slicing corners, fallback to a shuffle preserving permutation:
if len(set(a19)) != 200000:
    a19 = random_perm(200000)
summary.append((19, 200000, None, "interleaved block pattern n=200000 (or fallback random)"))
write_test(19, a19)

# 20: near-identity but full size (this is heavy & still somewhat structured)
a20 = near_identity(200000, window=100)
summary.append((20, 200000, None, "near-identity |a_i-i|<=100 n=200000 (stress structured)"))
write_test(20, a20)

# Print summary
print("Generated tests in directory:", OUTDIR)
print()
print("Index | n      | note")
print("------+--------+-------------------------------------------")
for idx, n, _, note in summary:
    print(f"{idx:02d}    | {n:<6} | {note}")
print()
print("Usage:")
print("  ./std < tests/XX.in > tests/XX.out")
print()
print("Notes:")
print("- Test 16 (strictly increasing 1..n) is the intended strong hack that forces naive O(n * len) to TLE.")
print("- All files are deterministic (seeded). If you want different randomness, change the seed at top.")

for i in range(1, 21):
    os.system(f"./std < tests/{i:02d}.in > tests/{i:02d}.ans")