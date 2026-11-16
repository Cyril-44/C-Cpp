# Reference solutions by subtask

| Statement group | Code | Method |
|---|---|---|
| 1 | `subtask1_brute.cpp` | Enumerate selected vertices and compute each distance directly. |
| 2 | `subtask2_square.cpp` | Precompute all-pairs distances and selected-set membership; process each operation in O(n). |
| 3 | `subtask3_chain.cpp` | On a chain, split each update into linear functions of the weighted position and use Fenwick trees. |
| 4 | `subtask4_star.cpp` | Each selected set is the queried vertex alone, so updates and queries take O(1). |
| 5 | `../std.cpp` | Full solution. |

The wrong-answer examples are under `wrong/`. `wrong/tle_naive.cpp` is logically
correct but too slow for the full constraints.
