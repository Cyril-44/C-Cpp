#!/usr/bin/env python3
"""生成 10 组测试数据及 path2/path3 大样例。运行：python3 generator.py"""
from collections import deque
from pathlib import Path
import random

ROOT = Path(__file__).resolve().parent
OUT = ROOT / "data"
OUT.mkdir(exist_ok=True)


def answer(rows):
    n = len(rows[0])
    dist = [[-1] * n for _ in range(2)]
    q = deque([(0, 0)])
    dist[0][0] = 1
    working = sum(row.count(".") for row in rows)
    while q:
        r, c = q.popleft()
        if (r, c) == (1, n - 1):
            return working - dist[r][c]
        for nr, nc in ((1-r, c), (r, c-1), (r, c+1)):
            if 0 <= nc < n and rows[nr][nc] == "." and dist[nr][nc] == -1:
                dist[nr][nc] = dist[r][c] + 1
                q.append((nr, nc))
    raise ValueError("数据中不存在从左上角到右下角的路线")


def save_test(idx, rows):
    n = len(rows[0])
    assert len(rows) == 2 and len(rows[1]) == n
    (OUT / f"{idx:02}.in").write_text(
        str(n) + "\n" + rows[0] + "\n" + rows[1] + "\n", encoding="ascii"
    )
    (OUT / f"{idx:02}.out").write_text(str(answer(rows)) + "\n", encoding="ascii")


def random_map_with_route(n, seed, white_probability=0.38, switch_probability=0.30):
    """先随机生成地图，再打通左上角到右下角的一条随机路线。"""
    rng = random.Random(seed)
    cells = [
        ["." if rng.random() < white_probability else "#" for _ in range(n)]
        for _ in range(2)
    ]

    if n == 1:
        cells[0][0] = cells[1][0] = "."
        return ["".join(cells[0]), "".join(cells[1])]

    rows_on_path = [0]
    row = 0
    for c in range(1, n):
        if rng.random() < switch_probability:
            row = 1 - row
        rows_on_path.append(row)
    # 路线必须以底行到达最后一列。
    rows_on_path[-1] = 1

    for c, r in enumerate(rows_on_path):
        cells[r][c] = "."
        if c + 1 < n and rows_on_path[c + 1] != r:
            # 在当前列先竖直换行，再向右走。
            cells[1 - r][c] = "."
    return ["".join(cells[0]), "".join(cells[1])]


# 前 20%：n 不超过 10。
save_test(1, random_map_with_route(1, 101))
save_test(2, random_map_with_route(10, 102))

# 接下来的 30%：n 不超过 2000。
for idx, seed in zip((3, 4, 5), (203, 204, 205)):
    save_test(idx, random_map_with_route(2000, seed))

# 剩余 50%：最大规模，并使用不同随机地图密度。
for idx, seed, density in zip(
    (6, 7, 8, 9, 10),
    (306, 307, 308, 309, 310),
    (0.20, 0.35, 0.50, 0.65, 0.40),
):
    save_test(idx, random_map_with_route(200000, seed, density, 0.30))


def save_sample(name, n, seed):
    rows = random_map_with_route(n, seed, 0.38, 0.30)
    folder = ROOT / "samples"
    folder.mkdir(exist_ok=True)
    (folder / f"{name}.in").write_text(
        str(n) + "\n" + rows[0] + "\n" + rows[1] + "\n", encoding="ascii"
    )
    (folder / f"{name}.ans").write_text(str(answer(rows)) + "\n", encoding="ascii")


sample1 = [".......", "....##."]
(ROOT / "samples" / "path1.in").write_text(
    "7\n" + sample1[0] + "\n" + sample1[1] + "\n", encoding="ascii"
)
(ROOT / "samples" / "path1.ans").write_text(str(answer(sample1)) + "\n", encoding="ascii")
save_sample("path2", 2000, 10002)
save_sample("path3", 200000, 10003)
print(f"已生成 10 组输入输出及 3 组样例：{ROOT}")
