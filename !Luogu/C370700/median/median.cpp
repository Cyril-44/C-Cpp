#include <bits/stdc++.h>
constexpr int N = 100005;
constexpr int64_t INF = 1LL << 61;
int n, fa[N];
int64_t w[N], f[N], dp[N][3], x;
std::vector<int> g[N];
void dfs(int u) {
    std::vector<int> ord = {u}, st = {u};
    ord.clear();
    fa[u] = 0;
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        ord.push_back(u);
        for (int v : g[u]) if (v != fa[u]) {
            fa[v] = u;
            st.push_back(v);
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        int u = ord[i], m = 0;
        int64_t r[3] = {w[u], -INF, -INF};
        for (int v : g[u]) if (v != fa[u]) {
            int mx = f[v] + (dp[v][2] != -INF);
            int64_t b[3] = {-INF, -INF, -INF};
            b[mx - f[v]] = 0;
            for (int k = 0; k < 3; k++) if (dp[v][k] != -INF) {
                int d = mx - f[v] - k + 1;
                if (d >= 0 && d < 3) b[d] = std::max(b[d], dp[v][k]);
            }
            int64_t nr[3] = {-INF, -INF, -INF};
            for (int a = 0; a < 3; a++) if (r[a] != -INF)
                for (int k = 0; a + k < 3; k++) if (b[k] != -INF)
                    nr[a + k] = std::max(nr[a + k], r[a] + b[k]);
            for (int k = 0; k < 3; k++) r[k] = nr[k];
            m += mx;
        }
        f[u] = m - 1;
        for (int k = 0; k < 3; k++) if (r[k] != -INF)
            f[u] = std::max(f[u], (int64_t)(m - k + (r[k] >= x ? 1 : -1)));
        for (int k = 0; k < 3; k++) dp[u][k] = -INF;
        for (int k = 0; k < 3; k++) if (r[k] != -INF) {
            int d = m - k - f[u] + 1;
            if (d >= 0 && d < 3) dp[u][d] = r[k];
        }
    }
}
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int Cid, T; std::cin >> Cid >> T;
    while (T--) {
        std::cin >> n;
        for (int i = 1; i <= n; i++) std::cin >> w[i];
        std::fill(g + 1, g + 1 + n, std::vector<int>());
        for (int i = 1; i < n; i++) {
            int u, v; std::cin >> u >> v;
            g[u].push_back(v), g[v].push_back(u);
        }
        int64_t l = -1e18, r = 1e18;
        while (l < r) {
            x = l + (r - l + 1) / 2;
            dfs(1);
            if (f[1] > 0) l = x;
            else r = x - 1;
        }
        std::cout << l << '\n';
    }
}
