#!/usr/bin/env python3
"""生成 10 组测试数据及 path2/path3 大样例。运行：python3 generator.py"""
from collections import deque
from pathlib import Path
import random

OUT = Path(__file__).resolve().parent / "data"
OUT.mkdir(exist_ok=True)


def answer(rows):
    n = len(rows[0])
    dist = [[-1] * n for _ in range(2)]
    q = deque()
    whites = sum(row.count(".") for row in rows)
    for r in range(2):
        if rows[r][0] == ".":
            dist[r][0] = 1
            q.append((r, 0))
    while q:
        r, c = q.popleft()
        if c == n - 1:
            return whites - dist[r][c]
        for nr, nc in ((1-r, c), (r, c-1), (r, c+1)):
            if 0 <= nc < n and rows[nr][nc] == "." and dist[nr][nc] == -1:
                dist[nr][nc] = dist[r][c] + 1
                q.append((nr, nc))
    raise ValueError("测试数据中没有左到右道路")


def save(idx, rows):
    assert len(rows) == 2 and len(rows[0]) == len(rows[1])
    n = len(rows[0])
    (OUT / f"{idx:02}.in").write_text(
        str(n) + "\n" + rows[0] + "\n" + rows[1] + "\n", encoding="ascii"
    )
    (OUT / f"{idx:02}.out").write_text(str(answer(rows)) + "\n", encoding="ascii")


def random_map_with_path(n, seed, white_probability=0.38, switch_probability=0.30):
    """随机生成棋盘，再随机选一条从左到右的路线并将路线格打白。"""
    rng = random.Random(seed)
    cells = [
        ["." if rng.random() < white_probability else "#" for _ in range(n)]
        for _ in range(2)
    ]

    # 路线每列向右走一格；部分列会先在当前列上下换行再向右。
    row = rng.randrange(2)
    rows_on_path = [row]
    for c in range(1, n):
        if rng.random() < switch_probability:
            row = 1 - row
        rows_on_path.append(row)

    for c, r in enumerate(rows_on_path):
        cells[r][c] = "."
        if c + 1 < n and rows_on_path[c + 1] != r:
            # 在本列上下移动后，再向右进入下一列。
            cells[1 - r][c] = "."

    return ["".join(cells[0]), "".join(cells[1])]


# 前 20%：n 不超过 10。
save(1, random_map_with_path(1, 101))
save(2, random_map_with_path(10, 102))

# 接下来的 30%：n 不超过 2000。
for idx, seed in zip((3, 4, 5), (203, 204, 205)):
    save(idx, random_map_with_path(2000, seed))

# 剩余 50%：最大规模，分别使用不同随机种子和白格比例。
for idx, seed, density in zip(
    (6, 7, 8, 9, 10),
    (306, 307, 308, 309, 310),
    (0.20, 0.35, 0.50, 0.65, 0.40),
):
    save(idx, random_map_with_path(200000, seed, density, 0.30))


def save_large_sample(name, n):
    rows = random_map_with_path(n, 8000 + n, 0.38, 0.30)
    folder = Path(__file__).resolve().parent / "samples"
    folder.mkdir(exist_ok=True)
    (folder / f"{name}.in").write_text(
        str(n) + "\n" + rows[0] + "\n" + rows[1] + "\n", encoding="ascii"
    )
    (folder / f"{name}.ans").write_text(str(answer(rows)) + "\n", encoding="ascii")


save_large_sample("path2", 2000)
save_large_sample("path3", 200000)

# 清理旧版本中多出的测试点，保证目录恰有 10 个测试点。
for idx in (11, 12):
    for suffix in ("in", "out"):
        old_file = OUT / f"{idx:02}.{suffix}"
        if old_file.exists():
            old_file.unlink()
print(f"已生成 10 组输入和输出：{OUT}")
