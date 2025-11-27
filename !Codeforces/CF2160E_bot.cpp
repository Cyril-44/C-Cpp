#include <bits/stdc++.h>
using namespace std;
#pragma GCC optimize(2)
typedef long long ll;
constexpr ll INF = 1LL << 60;
struct SegTr {
    vector<ll> mn, lz;
    int n;
    SegTr(int _n) : n(_n), mn(4 * (_n + 1), INF), lz(4 * (_n + 1), INF) {}
    void pushdown(int node, int s, int e) {
        if (lz[node] != INF) {
            mn[node] = min(mn[node], lz[node]);
            if (s != e) {
                lz[2 * node] = min(lz[2 * node], lz[node]);
                lz[2 * node + 1] = min(lz[2 * node + 1], lz[node]);
            }
            lz[node] = INF;
        }
    }
    void update(int L, int R, ll val) { update(1, 1, n, L, R, val); }
    void update(int u, int l, int r, int L, int R, ll val) {
        pushdown(u, l, r);
        if (l > r || l > R || r < L)
            return;
        if (L <= l && r <= R) {
            lz[u] = min(lz[u], val);
            pushdown(u, l, r);
            return;
        }
        int mid = (l + r) / 2;
        update(2 * u, l, mid, L, R, val);
        update(2 * u + 1, mid + 1, r, L, R, val);
        mn[u] = min(mn[2 * u], mn[2 * u + 1]);
    }
    ll query(int L, int R) { return query(1, 1, n, L, R); }
    ll query(int node, int s, int e, int L, int R) {
        pushdown(node, s, e);
        if (s > e || s > R || e < L)
            return INF;
        if (L <= s && e <= R)
            return mn[node];
        int mid = (s + e) / 2;
        return min(query(2 * node, s, mid, L, R),
                   query(2 * node + 1, mid + 1, e, L, R));
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    cin >> T;
    while (T--) {
        int n0, m0;
        cin >> n0 >> m0;
        vector<string> tg(n0 + 1);
        for (int i = 1; i <= n0; i++) {
            cin >> tg[i];
            tg[i] = " " + tg[i];
        }

        bool swapped = false;
        int n = n0, m = m0;
        vector<string> g;
        if (n0 > m0) {
            swapped = true;
            n = m0;
            m = n0;
            g.assign(n + 1, string());
            for (int i = 1; i <= n; ++i) {
                string row;
                row.resize(m + 1);
                row = " ";
                for (int j = 1; j <= m; ++j) {
                    row.push_back(tg[j][i]);
                }
                g[i] = row;
            }
        } else {
            g = tg;
        }

        vector<vector<int>> col(n + 1);
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (g[i][j] == '1')
                    col[i].push_back(j);
            }
        }
        vector<SegTr> f(m + 1, SegTr(n));
        for (int u = 1; u <= n; u++) {
            for (int d = u + 1; d <= n; d++) {
                vector<int> pos;
                set_intersection(col[u].begin(), col[u].end(), col[d].begin(),
                                 col[d].end(), back_inserter(pos));
                if (pos.size() < 2)
                    continue;
                int hh = d - u + 1;
                int leftm = pos[0], rightm = pos.back();
                for (int j = leftm; j <= rightm; j++) {
                    auto it = lower_bound(pos.begin(), pos.end(), j);
                    ll minw = INF;
                    bool at_point = false;
                    int idx = -1;
                    if (it != pos.end() && *it == j) {
                        at_point = true;
                        idx = it - pos.begin();
                    } else {
                        if (it != pos.begin() && it != pos.end()) {
                            auto prev_it = prev(it);
                            int prev = *prev_it;
                            int nxt = *it;
                            if (prev <= j && j <= nxt) {
                                minw = nxt - prev + 1;
                            }
                        } else if (it == pos.end() && !pos.empty()) {
                            if ((int)pos.size() >= 2) {
                                int prev = pos[pos.size() - 2];
                                int last = pos.back();
                                if (prev <= j && j <= last) {
                                    minw = last - prev + 1;
                                }
                            }
                        } else if (it == pos.begin() && !pos.empty()) {
                            if ((int)pos.size() >= 2) {
                                int first = pos[0];
                                int second = pos[1];
                                if (first <= j && j <= second) {
                                    minw = second - first + 1;
                                }
                            }
                        }
                    }
                    if (at_point) {
                        if (idx > 0) {
                            int prev = pos[idx - 1];
                            minw = min(minw, (ll)j - prev + 1);
                        }
                        if (idx + 1 < (int)pos.size()) {
                            int nxt = pos[idx + 1];
                            minw = min(minw, (ll)nxt - j + 1);
                        }
                    }
                    if (minw == INF)
                        continue;
                    ll area = hh * minw;
                    f[j].update(u, d, area);
                }
            }
        }

        vector<vector<ll>> ans(n + 1, vector<ll>(m + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                ll v = f[j].query(i, i);
                if (v == INF)
                    v = 0;
                ans[i][j] = v;
            }
        }

        if (!swapped) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= m; j++) {
                    cout << ans[i][j] << ' ';
                }
                cout << '\n';
            }
        } else {
            for (int i = 1; i <= n0; i++) {
                for (int j = 1; j <= m0; j++) {
                    cout << ans[j][i] << ' ';
                }
                cout << '\n';
            }
        }
    }
    return 0;
}
