
#include <bits/stdc++.h>
using namespace std;

constexpr int N = 200005, M = N << 1;
using Pii = pair<int, int>;

vector<Pii> g[N];
vector<int> ng[M];
int n, k, tot;
int cnt1[M], cnt2[M];

void add(int u, int v) {
    ng[u].push_back(v);
    ng[v].push_back(u);
}

namespace BuildGraph {
    int fa[N], col[N], rep[N];

    void work() {
        vector<int> ord(1, 1);
        for (int i = 0; i < (int)ord.size(); i++) {
            int u = ord[i];
            for (auto [v, c] : g[u]) if (v != fa[u]) {
                fa[v] = u;
                col[v] = c;
                ord.push_back(v);
            }
        }

        tot = n;
        for (int u : ord) {
            sort(g[u].begin(), g[u].end(),
                [](Pii x, Pii y) {
                    return x.second < y.second;
                });

            int last = 0, id = 0;
            for (auto [v, c] : g[u]) if (v != fa[u]) {
                if (c != last) {
                    if (c == col[u]) id = rep[u];
                    else {
                        id = ++tot;
                        add(u, id);
                    }
                    last = c;
                }
                add(id, v);
                rep[v] = id;
            }
        }
    }
}

namespace CalcCnt1 {
    int fa[M], sz[M];
    bool vis[M];

    int get(const vector<int>& a, int p) {
        return a[min(p, (int)a.size() - 1)];
    }

    void launchPointDivision() {
        vector<int> comps(1, 1), ord;
        vector<tuple<int, int, int>> nodes;
        ord.reserve(tot);

        while (!comps.empty()) {
            int rt = comps.back();
            comps.pop_back();
            if (vis[rt]) continue;

            ord.clear();
            ord.push_back(rt);
            fa[rt] = 0;

            for (int i = 0; i < (int)ord.size(); i++) {
                int u = ord[i];
                for (int v : ng[u])
                    if (!vis[v] && v != fa[u]) {
                        fa[v] = u;
                        ord.push_back(v);
                    }
            }

            int ss = (int)ord.size();
            int best = ss + 1, cen = 0;

            for (int i = ss - 1; i >= 0; i--) {
                int u = ord[i], mx = 0;
                sz[u] = 1;

                for (int v : ng[u])
                    if (!vis[v] && v != fa[u]) {
                        sz[u] += sz[v];
                        mx = max(mx, sz[v]);
                    }

                mx = max(mx, ss - sz[u]);
                if (mx < best) {
                    best = mx;
                    cen = u;
                }
            }

            vis[cen] = true;

            vector<int> all(1, cen <= n);
            vector<vector<int>> hs;
            nodes.clear();

            for (int v : ng[cen]) if (!vis[v]) {
                comps.push_back(v);

                int id = (int)hs.size();
                hs.emplace_back(1, 0);

                vector<tuple<int, int, int>> st;
                st.emplace_back(v, cen, 1);

                while (!st.empty()) {
                    auto [x, p, d] = st.back();
                    st.pop_back();

                    if (d > k) continue;
                    nodes.emplace_back(x, d, id);

                    if (x <= n) {
                        if ((int)all.size() <= d)
                            all.resize(d + 1);
                        if ((int)hs[id].size() <= d)
                            hs[id].resize(d + 1);

                        ++all[d];
                        ++hs[id][d];
                    }

                    if (d == k) continue;

                    for (int y : ng[x])
                        if (!vis[y] && y != p)
                            st.emplace_back(y, x, d + 1);
                }
            }

            for (int j = 1; j < (int)all.size(); j++)
                all[j] += all[j - 1];

            for (auto& h : hs)
                for (int j = 1; j < (int)h.size(); j++)
                    h[j] += h[j - 1];

            cnt1[cen] += all.back();

            for (auto [u, d, id] : nodes)
                cnt1[u] += get(all, k - d)
                         - get(hs[id], k - d);
        }
    }
}

namespace CalcCnt2 {
    int fa[M], dep[M], hv[M], dfn[M], f[M];

    void launchDP() {
        vector<int> ord(1, 1);

        for (int i = 0; i < (int)ord.size(); i++) {
            int u = ord[i];
            for (int v : ng[u]) if (v != fa[u]) {
                fa[v] = u;
                ord.push_back(v);
            }
        }

        for (int i = (int)ord.size() - 1; i >= 0; i--) {
            int u = ord[i];
            for (int v : ng[u])
                if (v != fa[u] && dep[v] + 1 > dep[u]) {
                    dep[u] = dep[v] + 1;
                    hv[u] = v;
                }
        }

        vector<int> seq, st(1, 1);
        seq.reserve(tot);
        int tim = 0;

        while (!st.empty()) {
            int u = st.back();
            st.pop_back();

            dfn[u] = ++tim;
            seq.push_back(u);

            for (int v : ng[u])
                if (v != fa[u] && v != hv[u])
                    st.push_back(v);

            if (hv[u]) st.push_back(hv[u]);
        }

        for (int i = (int)seq.size() - 1; i >= 0; i--) {
            int u = seq[i];

            if (u <= n) ++f[dfn[u]];

            for (int v : ng[u])
                if (v != fa[u] && v != hv[u])
                    for (int j = 0; j <= dep[v]; j++)
                        f[dfn[u] + j + 1] += f[dfn[v] + j];

            if (k <= dep[u])
                cnt2[u] = f[dfn[u] + k];
        }
    }
}

long long com3(long long x) {
    return x * (x - 1) * (x - 2) / 6;
}

int main() {
    scanf("%d%d", &n, &k);

    for (int i = 1, u, v, c; i < n; i++) {
        scanf("%d%d%d", &u, &v, &c);
        g[u].emplace_back(v, c);
        g[v].emplace_back(u, c);
    }

    if (n < 3) {
        puts("0");
        return 0;
    }

    if (k >= n - 1) {
        printf("%lld\n", com3(n));
        return 0;
    }

    BuildGraph::work();
    CalcCnt1::launchPointDivision();
    CalcCnt2::launchDP();

    long long ans = com3(cnt1[1]);

    for (int i = 2; i <= tot; i++)
        ans += com3(cnt1[i])
             - com3(cnt1[i] - cnt2[i]);

    printf("%lld\n", ans);
    return 0;
}
