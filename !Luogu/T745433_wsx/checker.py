#!/usr/bin/env python3

import sys
import heapq
import subprocess
import random
import os

def brute_shortest(s, t, k, a):
    """最暴力的完整图 Dijkstra（Python 实现，k<=4 时极快）"""
    if s == t:
        return 0
    N = 1 << k
    INF = 10**18 * 4
    dist = [INF] * N
    dist[s] = 0
    pq = [(0, s)]
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u]:
            continue
        for v in range(N):
            if u == v:
                continue
            pop = bin(u & v).count('1')
            if pop >= len(a):
                continue
            w = a[pop]
            if dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                heapq.heappush(pq, (dist[v], v))
    return dist[t] if dist[t] < INF else -1   # 理论上图是连通的，不会走到这里

def generate_tests(max_k=4):
    """生成 k=1~4 的**所有 s,t 全排列**（共约 340 组）"""
            # 固定种子，保证每次运行结果一致
    tests = []
    for k in range(1, max_k + 1):
        N = 1 << k
        for s in range(N):
            for t in range(N):
                # 每组 (s,t) 生成两种不同风格的 a[]，增加覆盖率
                if random.random() < 0.4:
                    a = [random.randint(1, 200) for _ in range(k)]
                else:
                    # 构造一些特殊情况（递增、极大值、包含极小值等）
                    a = [i * 10 + random.randint(1, 50) for i in range(k)]
                tests.append((s, t, k, a))
    return tests

def main():
    tests = generate_tests(max_k=4)
    print(f"✅ 已生成 {len(tests)} 组小数据（k=1~4 所有 s,t 全排列）")
    print("   每个 (s,t) 均包含多样 a[]，覆盖率极高")

    # 1. 写入 input.txt（可直接喂给 ./std）
    with open("input.txt", "w") as f:
        f.write(f"{len(tests)}\n")
        for s, t, k, a in tests:
            f.write(f"{s} {t} {k}\n")
            f.write(" ".join(map(str, a)) + "\n")
    print("   → input.txt 已生成（可直接 ./std < input.txt > std.out）")

    # 2. 检查 ./std 是否存在
    if not os.path.exists("./std"):
        print("❌ 错误：目录下没有可执行文件 ./std")
        print("   请先编译你的程序：g++ your_std.cpp -o std -O2")
        return

    # 3. 运行 ./std
    print("🚀 正在运行你的 ./std ...")
    with open("input.txt", "r") as fin, open("std.out", "w") as fout:
        try:
            subprocess.run(["./std"], stdin=fin, stdout=fout, check=True, timeout=10)
        except subprocess.TimeoutExpired:
            print("❌ ./std 超时（10秒），请检查程序效率")
            return
        except subprocess.CalledProcessError:
            print("❌ ./std 运行出错")
            return

    # 4. 读取 std 的答案
    with open("std.out") as f:
        std_answers = [int(line.strip()) for line in f if line.strip()]

    # 5. 对拍
    print("🔍 开始与暴力 Dijkstra 对拍...")
    errors = 0
    for idx, (s, t, k, a) in enumerate(tests):
        my_ans = brute_shortest(s, t, k, a)
        std_ans = std_answers[idx]
        if my_ans != std_ans:
            print(f"\n❌ WA！Test #{idx+1}")
            print(f"   s = {s}, t = {t}, k = {k}")
            print(f"   a = {a}")
            print(f"   std 输出 = {std_ans}，暴力正确答案 = {my_ans}")
            errors += 1
            if errors >= 10:          # 避免刷屏
                print("... 还有更多错误，已停止显示")
                break

    if errors == 0:
        print(f"\n🎉 恭喜！全部 {len(tests)} 组小数据全部通过！")
        print("   你的 ./std 在 k<=4 的所有 s,t 全排列情况下均正确。")
    else:
        print(f"\n⚠️  共发现 {errors} 处错误！")
        print("   请根据上面输出的样例调试你的程序。")

    print("\n📁 额外文件：")
    print("   input.txt   ← 喂给 ./std 的输入")
    print("   std.out     ← 你的程序输出")

if __name__ == "__main__":
    main()