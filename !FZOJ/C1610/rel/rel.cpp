#include <bits/stdc++.h>
constexpr int N = 1005;
std::bitset<N> bel[N], rel[N];
int p[N];
bool leaf[N];
int n, m, leafcnt;
int que[N], hd, tl;
inline void makeleaf(int u) {
    leaf[u] = true; ++leafcnt;
    for (int j = 1; j <= m; j++)
        if (bel[u][j]) {
            --p[j], rel[j].reset(u);
            if (p[j] == 1) {
                int v = rel[j]._Find_first(); 
                bel[v].reset(j);
                que[tl++] = v;
            }
        }
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) bel[i].reset();
        for (int j = 1; j <= m; j++) {
            scanf("%d", &p[j]);
            rel[j].reset();
            for (int x, _ = 0; _ < p[j]; _++) {
                scanf("%d", &x);
                rel[j].set(x);
                if (p[j] > 1) bel[x].set(j);
            }
        }
        memset(leaf, 0, sizeof leaf);
        leafcnt = hd = tl = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (i != j && (bel[i] & bel[j]) == bel[i] && (i < j || bel[i] != bel[j])) {
                    makeleaf(i);
                    break;
                }
        bool ans = true;
        while (leafcnt < n-1) {
            if (hd == tl) { ans = false; break; }
            int u = que[hd++];
            if (leaf[u]) continue;
            for (int i = 1; i <= n; i++)
                if (u != i && !leaf[i] && (bel[u] & bel[i]) == bel[u]) {
                    makeleaf(u);
                    break;
                }
        }
        puts(ans ? "YES" : "NO");
    }
    return 0;
}