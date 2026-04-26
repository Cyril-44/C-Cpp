#include <bits/stdc++.h>
using namespace std;

const long long NEG_INF = (long long)-4e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int L;
    if (!(cin >> n >> L)) return 0;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // Root the tree at 1, compute parent and depth
    const int LOG = 20; // since n <= 3e5, 2^19 < 3e5 < 2^20
    vector<int> depth(n + 1, 0);
    vector<array<int, LOG>> up(n + 1);
    vector<int> parent(n + 1, 0);

    // BFS or DFS to set parent and depth
    {
        queue<int> q;
        q.push(1);
        parent[1] = 0;
        depth[1] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) {
                if (v == parent[u]) continue;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                q.push(v);
            }
        }
    }

    // build up table
    for (int i = 1; i <= n; ++i) {
        up[i][0] = parent[i];
    }
    for (int k = 1; k < LOG; ++k) {
        for (int i = 1; i <= n; ++i) {
            int mid = up[i][k - 1];
            up[i][k] = (mid ? up[mid][k - 1] : 0);
        }
    }

    auto kth_ancestor = [&](int u, int dist) {
        // move dist steps up from u
        for (int k = 0; k < LOG && u; ++k) {
            if (dist & (1 << k)) {
                u = up[u][k];
                if (!u) break;
            }
        }
        return u;
    };

    // DP array
    vector<long long> f(n + 1, NEG_INF);
    f[1] = 0; // sequence starts at root with sum 0
    long long ans = 0;

    // We need an order where parent is processed before child: BFS order is fine
    vector<int> order;
    order.reserve(n);
    {
        queue<int> q;
        q.push(1);
        vector<int> vis(n + 1, 0);
        vis[1] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : g[u]) {
                if (!vis[v]) {
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }
    }

    // Random generator
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    const int SAMPLES = 40; // can be tuned

    for (int idx = 0; idx < (int)order.size(); ++idx) {
        int u = order[idx];
        if (u == 1) continue;

        long long best = NEG_INF;

        // candidate 1: root, if within L
        if (depth[u] <= L) {
            long long cand = f[1] + (long long)(a[1] % a[u]);
            if (cand > best) best = cand;
        }

        // candidate 2: parent
        int p = parent[u];
        if (p != 0 && depth[u] - depth[p] >= 1 && depth[u] - depth[p] <= L && f[p] != NEG_INF) {
            long long cand = f[p] + (long long)(a[p] % a[u]);
            if (cand > best) best = cand;
        }

        // random ancestors within distance <= L
        int maxd = min(L, depth[u]);
        if (maxd > 1) {
            uniform_int_distribution<int> dist(1, maxd);
            for (int s = 0; s < SAMPLES; ++s) {
                int d = dist(rng);
                int v = kth_ancestor(u, d);
                if (!v) continue;
                if (f[v] == NEG_INF) continue;
                long long cand = f[v] + (long long)(a[v] % a[u]);
                if (cand > best) best = cand;
            }
        }

        f[u] = best;
        if (f[u] > ans) ans = f[u];
    }

    cout << ans << "\n";
    return 0;
}
