#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MOD = 998244353;

struct Fenwick {
    int n;
    vector<ll> tree;
    Fenwick(int _n) : n(_n), tree(_n + 1, 0) {}
    void update(int idx, ll val) {
        val %= MOD;
        if (val < 0) val += MOD;
        while (idx <= n) {
            tree[idx] = (tree[idx] + val) % MOD;
            idx += idx & -idx;
        }
    }
    ll query(int idx) {
        ll sum = 0;
        while (idx > 0) {
            sum = (sum + tree[idx]) % MOD;
            idx -= idx & -idx;
        }
        return sum;
    }
    ll query(int l, int r) {
        if (l > r) return 0;
        return (query(r) - query(l - 1) + MOD) % MOD;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> intervals(m);
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        intervals[i] = {l, r};
    }
    sort(intervals.begin(), intervals.end());
    Fenwick ft(n + 1);
    ll ans = 0;
    for (auto& p : intervals) {
        int L = p.first;
        int R = p.second;
        ll prev = ft.query(L, n);
        ll dpi = (1 + prev) % MOD;
        ans = (ans + dpi) % MOD;
        ft.update(R, dpi);
    }
    cout << ans << endl;
    return 0;
}