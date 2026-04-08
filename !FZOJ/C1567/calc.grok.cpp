#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 1000000007LL;

struct SegTree {
    int n;
    vector<ll> t, lz;
    SegTree(int _n) : n(_n), t(4 * _n + 10, 0), lz(4 * _n + 10, 0) {}
    void build(int node, int start, int end, vector<ll>& vals) {
        if (start == end) {
            t[node] = vals[start];
            return;
        }
        int mid = (start + end) / 2;
        build(2 * node, start, mid, vals);
        build(2 * node + 1, mid + 1, end, vals);
        t[node] = min(t[2 * node], t[2 * node + 1]);
    }
    void propagate(int node, int start, int end) {
        if (lz[node] != 0) {
            t[node] += lz[node];
            if (start != end) {
                lz[2 * node] += lz[node];
                lz[2 * node + 1] += lz[node];
            }
            lz[node] = 0;
        }
    }
    void update(int node, int start, int end, int l, int r, ll val) {
        propagate(node, start, end);
        if (start > end || start > r || end < l) return;
        if (l <= start && end <= r) {
            lz[node] += val;
            propagate(node, start, end);
            return;
        }
        int mid = (start + end) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        t[node] = min(t[2 * node], t[2 * node + 1]);
    }
    ll query(int node, int start, int end, int l, int r) {
        propagate(node, start, end);
        if (start > end || start > r || end < l) return LLONG_MAX / 2;
        if (l <= start && end <= r) return t[node];
        int mid = (start + end) / 2;
        ll p1 = query(2 * node, start, mid, l, r);
        ll p2 = query(2 * node + 1, mid + 1, end, l, r);
        return min(p1, p2);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll nn, mm;
    cin >> nn >> mm;
    int n = nn;
    ll m = mm;
    vector<int> p(n + 1);
    for (int i = 1; i <= n; i++) cin >> p[i];
    // inversion count
    vector<ll> fen(n + 2, 0);
    auto addf = [&](auto&& self, int x, ll v) -> void {
        for (; x <= n; x += x & -x) fen[x] += v;
    };
    auto qf = [&](auto&& self, int x) -> ll {
        ll s = 0;
        for (; x; x -= x & -x) s += fen[x];
        return s;
    };
    ll inv_cnt = 0;
    for (int i = 1; i <= n; i++) {
        inv_cnt += qf(qf, n) - qf(qf, p[i]);
        addf(addf, p[i], 1);
    }
    // flip positions
    vector<int> flip_at(n + 2, 0);
    for (int i = 1; i <= n; i++) {
        flip_at[p[i]] = i;
    }
    // segtree for r = 0 to n
    int NNN = n + 1;
    vector<ll> init_val(NNN);
    for (int i = 0; i < NNN; i++) init_val[i] = i;
    SegTree st(NNN);
    st.build(1, 0, NNN - 1, init_val);
    // sum_mins
    ll sum_mins = st.query(1, 0, NNN - 1, 0, NNN - 1);
    for (int l = 1; l <= n; l++) {
        int s = flip_at[l];
        st.update(1, 0, NNN - 1, s, NNN - 1, -2LL);
        ll curmin = st.query(1, 0, NNN - 1, 0, NNN - 1);
        sum_mins += curmin;
    }
    ll sum_l = n * 1LL * (n + 1) / 2;
    ll sum_c = sum_l + sum_mins;
    // precompute inverses 1 to n+1
    int maxk = n + 1;
    vector<ll> invs(maxk + 1);
    invs[1] = 1;
    for (int i = 2; i <= maxk; i++) {
        invs[i] = MOD - (MOD / i) * 1LL * invs[MOD % i] % MOD;
    }
    // binom func
    auto bin_cal = [&](ll N, int k) -> ll {
        if (k < 0 || N < k) return 0;
        ll res = 1;
        for (int i = 1; i <= k; i++) {
            ll up = (N - i + 1) % MOD;
            res = res * up % MOD;
            res = res * invs[i] % MOD;
        }
        return res;
    };
    ll N_total = n + m;
    ll c1 = bin_cal(N_total, n);
    ll c2 = bin_cal(N_total, n + 1);
    ll ans = (inv_cnt % MOD * c1 % MOD + ((sum_c % MOD + MOD) % MOD) * c2 % MOD) % MOD;
    cout << ans << endl;
}