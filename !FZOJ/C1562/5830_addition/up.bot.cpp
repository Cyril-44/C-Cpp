#include <bits/stdc++.h>
using namespace std;
using LL = long long;

struct Info {
    LL tot = 0;
    LL msuf = 0;
};

Info merge_info(const Info& a, const Info& b) {
    Info res;
    res.tot = a.tot + b.tot;
    res.msuf = max(b.msuf, b.tot + a.msuf);
    return res;
}

class SegTree {
public:
    vector<Info> tree;
    int n;
    SegTree(int _n) : n(_n), tree(4 * _n + 10) {}
    void build(int node, int start, int end, vector<LL>& d) {
        if (start == end) {
            tree[node].tot = d[start];
            tree[node].msuf = d[start];
            return;
        }
        int mid = (start + end) / 2;
        build(2 * node, start, mid, d);
        build(2 * node + 1, mid + 1, end, d);
        tree[node] = merge_info(tree[2 * node], tree[2 * node + 1]);
    }
    void update(int node, int start, int end, int idx, LL newd) {
        if (start == end) {
            tree[node].tot = newd;
            tree[node].msuf = newd;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) update(2 * node, start, mid, idx, newd);
        else update(2 * node + 1, mid + 1, end, idx, newd);
        tree[node] = merge_info(tree[2 * node], tree[2 * node + 1]);
    }
    Info query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return {0, 0};
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        Info p1 = query(2 * node, start, mid, l, r);
        Info p2 = query(2 * node + 1, mid + 1, end, l, r);
        return merge_info(p1, p2);
    }
    void build_init(vector<LL>& d) { if (n > 0) build(1, 0, n - 1, d); }
    void upd(int idx, LL newd) { if (n > 0) update(1, 0, n - 1, idx, newd); }
    Info get(int l, int r) {
        if (n == 0 || l > r) return {0, 0};
        return query(1, 0, n - 1, l, r);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t_id;
    cin >> t_id;
    int n, k, q;
    cin >> n >> k >> q;
    vector<vector<int>> g(n, vector<int>(k));
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < k; j++) {
            g[i][j] = s[j] - '0';
        }
    }
    vector<vector<LL>> ft(k, vector<LL>(n + 1, 0LL));
    auto ft_upd = [&](int c, int pos, LL val) {
        for (int idx = pos + 1; idx <= n; idx += idx & -idx) ft[c][idx] += val;
    };
    auto ft_pref = [&](int c, int pos) -> LL {
        LL s = 0;
        for (int idx = pos + 1; idx > 0; idx -= idx & -idx) s += ft[c][idx];
        return s;
    };
    auto get_cnt = [&](int c, int x, int y) -> LL {
        if (x > y) return 0;
        return ft_pref(c, y) - (x == 0 ? 0LL : ft_pref(c, x - 1));
    };
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < n; i++) {
            ft_upd(j, i, g[i][j]);
        }
    }
    // pair_sts[p][q] for p < q : diff = g[i][p] - g[i][q]
    vector<vector<SegTree>> pair_sts(k, vector<SegTree>(k, SegTree(n)));
    for (int p = 0; p < k; p++) {
        for (int q = p + 1; q < k; q++) {
            vector<LL> initd(n);
            for (int i = 0; i < n; i++) {
                initd[i] = (LL)g[i][p] - g[i][q];
            }
            pair_sts[p][q].build_init(initd);
        }
    }
    for (int qq = 0; qq < q; qq++) {
        char c;
        cin >> c;
        if (c == 'C') {
            int x, y;
            cin >> x >> y;
            int oldv = g[x][y];
            int newv = 1 - oldv;
            g[x][y] = newv;
            LL delta = newv - oldv;
            ft_upd(y, x, delta);
            // update all pairs involving column y
            for (int p = 0; p < y; p++) {
                LL newd = (LL)g[x][p] - g[x][y];
                pair_sts[p][y].upd(x, newd);
            }
            for (int qq_ = y + 1; qq_ < k; qq_++) {
                LL newd = (LL)g[x][y] - g[x][qq_];
                pair_sts[y][qq_].upd(x, newd);
            }
        } else {
            int x, y, l, r;
            cin >> x >> y >> l >> r;
            if (x > y || l > r) {
                cout << 0 << '\n';
                continue;
            }
            LL ans = LLONG_MAX / 2;
            for (int j = l; j <= r; j++) {
                LL cnt = get_cnt(j, x, y);
                ans = min(ans, cnt);
            }
            for (int j1 = l; j1 <= r; j1++) {
                for (int j2 = j1 + 1; j2 <= r; j2++) {
                    LL nleft = get_cnt(j1, x, y);
                    Info infd = pair_sts[j1][j2].get(x, y);
                    LL mdef = infd.msuf;
                    LL mat = nleft - mdef;
                    ans = min(ans, mat);
                }
            }
            cout << ans << '\n';
        }
    }
    return 0;
}