#include <bits/stdc++.h>
constexpr int N = 500005, K = 22;
#ifdef CLANGD
#define N 55
#endif
std::vector<std::pair<int,char>> g[N];
int r[N], sz[N], f[N][3];
std::map<int,int> mp[N][3];
inline int fix(int val) { return (val % 3 + 3) % 3; }
inline int fix2(int val) { return val >= 6 ? val - 6 : val >= 3 ? val - 3 : val; }
inline void umax(int &x, int y) { if (x < y) x = y; }
void dfs(int u, int now, int dep) {
    sz[u] = 1;
    mp[u][dep%3][now] = dep;
    f[u][0] = 0, f[u][1] = f[u][2] = -1;
    int fx = fix(-dep*2);
    for (auto [v, c] : g[u]) {
        dfs(v, now ^ (1<<c-'a'), dep+1);
        for (int i = 0; i < 3; i++) umax(f[u][i], f[v][i]);
        if (sz[u] < sz[v]) for (int i = 0; i < 3; i++) mp[u][i].swap(mp[v][i]);
        for (int j = 0; j < 3; j++)
            for (auto [s, d] : mp[v][j])
                for (int i = 0; i < 3; i++) {
                    const int ll = fix2(i + j + fx);
                    auto it = mp[u][i].find(s);
                    if (it != mp[u][i].end()) umax(f[u][ll], it->second + d - dep*2);
                    for (int k = 0; k < K; k++) {
                        it = mp[u][i].find(s ^ (1<<k));
                        if (it != mp[u][i].end()) umax(f[u][ll], it->second + d - dep*2);
                    }
                }
        for (int j = 0; j < 3; j++) {
            for (auto [s, d] : mp[v][j])
                umax(mp[u][j][s], d);
            mp[v][j].clear();
        }
        sz[u] += sz[v];
    }
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &r[i]);
    for (int i = 2; i <= n; i++) {
        int p; char ch; scanf("%d %c", &p, &ch);
        g[p].emplace_back(i, ch);
    }
    dfs(1, 0, 0);
    for (int i = 1; i <= n; i++)
        printf("%d ", f[i][r[i]]);
    putchar('\n');
}