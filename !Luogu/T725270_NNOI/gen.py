import os
import random

OUT_DIR = "data"

# ------------------ 通用：写文件 ------------------

def write_case(idx: int, n: int, perm: list[int], ans: int):
    in_path = os.path.join(OUT_DIR, f"{idx:02d}.in")
    ans_path = os.path.join(OUT_DIR, f"{idx:02d}.ans")
    with open(in_path, "w", encoding="utf-8") as f:
        f.write(f"{n}\n")
        f.write(" ".join(map(str, perm)) + "\n")
    with open(ans_path, "w", encoding="utf-8") as f:
        f.write(str(ans) + "\n")

# ------------------ 答案：可公式化的构造 ------------------

def ans_all_increasing(n: int) -> int:
    # a[i]=i：所有 (i, i+d, i+2d) 都满足
    m = (n - 1) // 2
    return m * (n - m - 1)

def pair_swap_perm(n: int) -> list[int]:
    # [2,1,4,3,6,5,...]
    a = list(range(1, n + 1))
    for i in range(0, n - 1, 2):
        a[i], a[i + 1] = a[i + 1], a[i]
    return a

def pair_swap_ans(n: int) -> int:
    # 闭式（已归纳可证）
    if n % 2 == 0:
        m = n // 2
        return (m - 1) * (m - 2)
    else:
        m = (n - 1) // 2
        return (m - 1) * (m - 1)

def reverse_perm(n: int) -> list[int]:
    return list(range(n, 0, -1))

def unique_one_triple(n: int, i: int, j: int) -> tuple[list[int], int]:
    """
    只存在 1 个解：
    在 (i,j,k) = (i, j, 2j-i) 放 1,2,3，
    其余位置按从大到小填 [n..4]，整体“值随位置严格下降”，不会产生额外上升三元组。
    """
    k = 2 * j - i
    assert 1 <= i < j < k <= n
    a = [0] * (n + 1)  # 1-index
    a[i], a[j], a[k] = 1, 2, 3
    rest = list(range(n, 3, -1))  # n..4
    p = 0
    for pos in range(1, n + 1):
        if a[pos] == 0:
            a[pos] = rest[p]
            p += 1
    return a[1:], 1

def blocks_descending(n: int, block: int) -> tuple[list[int], int]:
    """
    把值域分成块，每块内部递增，但块与块按值域从大到小拼接：
      例如 block=3：
        [n-2,n-1,n,  n-5,n-4,n-3,  ...]
    这样任何跨块三元组都不可能满足 a_i < a_j，
    所以解只可能完全落在同一块内部。
    - block=3：每块正好贡献 1 个 (p,p+1,p+2) -> ans = floor(n/3)
    - block=4：每块贡献 2 个 (p,p+1,p+2) 与 (p+1,p+2,p+3) -> ans = 2*floor(n/4)
    """
    assert block in (3, 4)
    perm = []
    t = n // block
    for b in range(t, 0, -1):
        L = (b - 1) * block + 1
        R = b * block
        perm.extend(range(L, R + 1))
    # 剩余尾巴（小值）放最后
    perm.extend(range(t * block + 1, n + 1))

    if block == 3:
        ans = t
    else:
        ans = 2 * t
    return perm, ans

# ------------------ 小规模暴力算真值（用于 Sub1 & Sub3 小 n） ------------------

def brute_ans(a: list[int]) -> int:
    # O(n^2)，适合 n<=8000 左右
    n = len(a)
    a = [0] + a  # 1-index
    ans = 0
    for j in range(2, n):
        aj = a[j]
        lim = j - 1
        if n - j < lim:
            lim = n - j
        for d in range(1, lim + 1):
            if a[j - d] < aj < a[j + d]:
                ans += 1
    return ans

# ------------------ 生成满足 |a_i - i| <= W 的“随机扰动”排列（不交叉交换） ------------------

def bounded_swap_perm(n: int, W: int, seed: int) -> list[int]:
    """
    从恒等排列出发，做一堆“互不相交”的交换，每次交换距离 <= W，
    则必然保证所有位置 |a_i - i| <= W。
    """
    random.seed(seed)
    a = list(range(1, n + 1))
    used = [False] * (n + 1)
    i = 1
    while i <= n:
        if used[i]:
            i += 1
            continue
        # 以一定概率尝试交换
        if random.random() < 0.6:
            j = i + random.randint(1, W)
            if j <= n and (not used[j]):
                # 交换 i 与 j：元素 i,j 的位移恰好是 |i-j|<=W
                a[i - 1], a[j - 1] = a[j - 1], a[i - 1]
                used[i] = used[j] = True
                i += 1
                continue
        used[i] = True
        i += 1
    return a

# ------------------ 主流程：20 个点按子任务分组 ------------------

def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    idx = 1

    # ---------- Subtask1 (15pts = 3 cases) ----------
    # 01: 边界
    n = 1
    write_case(idx, n, [1], 0); idx += 1

    # 02: n=1000 随机排列（暴力真值）
    n = 1000
    perm = list(range(1, n + 1))
    random.seed(20260126)
    random.shuffle(perm)
    ans = brute_ans(perm)
    write_case(idx, n, perm, ans); idx += 1

    # 03: n=1000 唯一解、d 很大（卡随机化/只扫小 d）
    n = 1000
    perm, ans = unique_one_triple(n, 1, 400)  # d=399 很大
    write_case(idx, n, perm, ans); idx += 1

    # ---------- Subtask2 (20pts = 4 cases) ----------
    # 04: 递减（ans=0）
    n = 10000
    write_case(idx, n, reverse_perm(n), 0); idx += 1

    # 05: 递增（公式，卡 int）
    n = 10000
    write_case(idx, n, list(range(1, n + 1)), ans_all_increasing(n)); idx += 1

    # 06: 相邻交换（公式）
    n = 9999
    write_case(idx, n, pair_swap_perm(n), pair_swap_ans(n)); idx += 1

    # 07: 3-块递减拼接（中等答案，非单调）
    n = 10000
    perm, ans = blocks_descending(n, 3)
    write_case(idx, n, perm, ans); idx += 1

    # ---------- Subtask3 (30pts = 6 cases, |a_i - i| <= 100) ----------
    # 08: 恒等 n=200000（全递增，溢出杀，且满足差值约束）
    n = 200000
    write_case(idx, n, list(range(1, n + 1)), ans_all_increasing(n)); idx += 1

    # 09: 恒等 n=199999（防写死）
    n = 199999
    write_case(idx, n, list(range(1, n + 1)), ans_all_increasing(n)); idx += 1

    # 10: 相邻交换 n=200000（满足差值约束）
    n = 200000
    write_case(idx, n, pair_swap_perm(n), pair_swap_ans(n)); idx += 1

    # 11: 相邻交换 n=199999（满足差值约束）
    n = 199999
    write_case(idx, n, pair_swap_perm(n), pair_swap_ans(n)); idx += 1

    # 12: n=5000 距离<=100 的随机不交叉交换（暴力真值）
    n = 5000
    perm = bounded_swap_perm(n, 100, seed=114514)
    ans = brute_ans(perm)
    write_case(idx, n, perm, ans); idx += 1

    # 13: n=8000 距离<=100 的随机不交叉交换（暴力真值）
    n = 8000
    perm = bounded_swap_perm(n, 100, seed=1919810)
    ans = brute_ans(perm)
    write_case(idx, n, perm, ans); idx += 1

    # ---------- Subtask4 (35pts = 7 cases, full strength) ----------
    # 14: 递减（ans=0）
    n = 200000
    write_case(idx, n, reverse_perm(n), 0); idx += 1

    # 15: 递增（最大级别，溢出杀）
    n = 200000
    write_case(idx, n, list(range(1, n + 1)), ans_all_increasing(n)); idx += 1

    # 16: 相邻交换（非单调但答案大且可解析）
    n = 200000
    write_case(idx, n, pair_swap_perm(n), pair_swap_ans(n)); idx += 1

    # 17: 唯一解 + 超大 d（卡随机化/只扫小 d/分块漏算）
    n = 200000
    perm, ans = unique_one_triple(n, 1, 100000)  # d=99999
    write_case(idx, n, perm, ans); idx += 1

    # 18: 唯一解换位置（防特判 i=1）
    n = 200000
    # 选一组保证 k<=n
    perm, ans = unique_one_triple(n, 50000, 120000)  # k=190000
    write_case(idx, n, perm, ans); idx += 1

    # 19: 3-块递减拼接（中等答案，很多人“只测单调”会翻车）
    n = 200000
    perm, ans = blocks_descending(n, 3)
    write_case(idx, n, perm, ans); idx += 1

    # 20: 4-块递减拼接（每块贡献 2 个解，答案不同于 3-块）
    n = 200000
    perm, ans = blocks_descending(n, 4)
    write_case(idx, n, perm, ans); idx += 1

    print(f"Generated {idx-1} cases into ./{OUT_DIR}/")

if __name__ == "__main__":
    main()
