#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <algorithm>
constexpr int N = 10, N2 = N * N + 5;
int key[N2], wall[N2][2]; // 0->x+, 1->y+
std::vector<std::pair<int,int>> g[N2];
inline void addedg(int fr, int to, int w) {
    g[fr].emplace_back(to, w);
    g[to].emplace_back(fr, w);
}
int f[N2][1 << (N + 1) | 5];
std::pair<int,int> que[sizeof(f) / sizeof(int)];
int main() {
    int n, m, p;
    scanf("%d%d%d", &n, &m, &p);
    auto getid = [m](int x, int y) { return (x - 1) * m + y; };
    {   int k;
        scanf("%d", &k);
        for (int x1, y1, x2, y2, g; k--; ) {
            scanf("%d%d%d%d%d", &x1, &y1, &x2, &y2, &g);
            if (x1 > x2) std::swap(x1, x2);
            if (y1 > y2) std::swap(y1, y2);
            wall[getid(x1, y1)][y1 != y2] = g ? g : -1;
        }
    } // Read walls
    {   int s;
        scanf("%d", &s);
        for (int x, y, q; s--; ) {
            scanf("%d%d%d", &x, &y, &q);
            key[getid(x, y)] |= 1 << q;
        }
    } // Read keys
    for (int i = 1, id = 1; i <= n; i++)
        for (int j = 1; j <= m; j++, id++) {
            if ((i ^ n) && ~wall[id][0]) addedg(id, id + m, wall[id][0]);
            if ((j ^ m) && ~wall[id][1]) addedg(id, id + 1, wall[id][1]);
        }
    int hd = 0, tl = 0;
    memset(f, -1, sizeof f);
    f[getid(1, 1)][1 | key[1]] = 0;
    que[tl++] = {getid(1, 1), 1 | key[1]};
    while (hd ^ tl) {
        auto [u, s] = que[hd++];
        for (const auto &[v, w] : g[u]) {
            int ss = s | key[v];
            if (s >> w & 1 && f[v][ss] == -1) {
                f[v][ss] = f[u][s] + 1;
                fprintf(stderr, "Level %d: Trans (%d,%d)[%s] --> (%d,%d)[%s], with wall %d\n", f[v][ss], (u-1)/m+1, (u-1)%m+1, std::bitset<N+1>(s).to_string().c_str(), (v-1)/m+1, (v-1)%m+1, std::bitset<N+1>(ss).to_string().c_str(), w);
                que[tl++] = {v, ss};
            }
        }
    }
    int ans = 1145141919;
    for (int s = 1, id = n * m; s < (1 << p+1); s++)
        if (~f[id][s]) ans = std::min(ans, f[id][s]);
    printf("%d\n", ans == 1145141919 ? -1 : ans);
    return 0;
}
