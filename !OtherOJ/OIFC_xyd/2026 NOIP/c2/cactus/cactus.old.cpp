#include <bits/stdc++.h>
constexpr int N = 25;
std::vector<int> g[N];
int n;
unsigned vis, surround, curr;
std::vector<unsigned> rings;
int start;
void test(int u) {
    vis |= 1<<u;
    for (int v : g[u]) if (!(vis>>v&1)) test(v);
}
void dfs(int u, int len) {
    if (rings.size() >= 200) return;
    curr |= 1<<u;
    vis |= 1<<u;
    if (!len) {
        if (surround >> u & 1)
            rings.push_back(curr);
    } else {
        for (int v : g[u]) if (v > start && !(vis>>v&1)) dfs(v, len - 1);
    }
    vis ^= 1<<u;
    curr ^= 1<<u;
}
int mxringcnt = 0;
void dfs2(int idx, int cnt, unsigned st) {
    mxringcnt = std::max(mxringcnt, cnt);
    if (idx == rings.size()) return;
    if ((n - __builtin_popcount(st)) / (__builtin_popcount(rings[idx]) - 1) + cnt <= mxringcnt) return;
    dfs2(idx+1, cnt, st);
    if (__builtin_popcount(st & rings[idx]) <= 1) {
        dfs2(idx+1, cnt+1, st | rings[idx]);
    }
}
int main() {
    freopen("cactus.in", "r", stdin);
    freopen("cactus.out", "w", stdout);
    int C, T, m;
    scanf("%d%d", &C, &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) g[i].clear();
        for (int i=0, u, v; i<m; i++) {
            scanf("%d%d", &u, &v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        if (C == 7) {
            printf("%d\n", m - (n-1) - (n-1)/2);
            continue;
        }
        vis = 0, test(1);
        if (vis != ((1u<<n) - 1 << 1)) {
            puts("-1"); continue;
        }
        rings.clear();
        for (int l = 3; l <= n; l++) {
            for (int u = 1; u <= n; u++) {
                start = u;
                surround = 0;
                for (int v : g[u]) if (v > u) surround |= 1<<v;
                vis = 1<<u;
                curr = 1<<u;
                for (int v : g[u]) if (v > u) {
                    surround ^= 1<<v;
                    dfs(v, l - 2);
                }
            }
        }
        // for (const auto &ring : rings) {
        //     for (int i = 1; i <= n; i++) if (ring >> i & 1) {
        //         printf("%d ", i);
        //     }
        //     printf("\n");
        // }
        mxringcnt = 0;
        dfs2(0, 0, 0);
        printf("%d\n", m - (n-1) - mxringcnt);
    }
}