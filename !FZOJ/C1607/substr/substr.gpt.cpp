#include <bits/stdc++.h>
using namespace std;
static constexpr int MOD = 998244353;
inline int addmod(int a, int b) {
    int s = a + b;
    if (s >= MOD) s -= MOD;
    return s;
}
inline int submod(int a, int b) {
    int s = a - b;
    if (s < 0) s += MOD;
    return s;
}
inline int mulmod(long long a, long long b) {
    return int(a * b % MOD);
}
struct Fenwick {
    int n;
    vector<int> fw, fv;
    explicit Fenwick(int n = 0) : n(n), fw(n + 1, 0), fv(n + 1, 0) {}
    inline void add(int pos, int delta, int x) {
        int dv = mulmod(delta, x);
        for (int i = pos; i <= n; i += i & -i) {
            fw[i] += delta;
            if (fw[i] >= MOD) fw[i] -= MOD;
            fv[i] += dv;
            if (fv[i] >= MOD) fv[i] -= MOD;
        }
    }
    inline pair<int, int> query(int pos) const {
        int sw = 0, sv = 0;
        for (int i = pos; i > 0; i -= i & -i) {
            sw += fw[i];
            if (sw >= MOD) sw -= MOD;
            sv += fv[i];
            if (sv >= MOD) sv -= MOD;
        }
        return {sw, sv};
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    vector<int> p(n + 1);
    p[0] = 0;
    for (int i = 1; i <= n; ++i) {
        int x;
        cin >> x;
        int s = p[i - 1] + x;
        if (s >= k) s -= k;
        p[i] = s;
    }
    vector<int> vals = p;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    const int m = (int)vals.size();
    vector<int> rankPos(n + 1);
    for (int i = 0; i <= n; ++i) { rankPos[i] = int(lower_bound(vals.begin(), vals.end(), p[i]) - vals.begin()); }
    vector<int> totalCnt(m, 0);
    for (int i = 0; i <= n; ++i) { ++totalCnt[rankPos[i]]; }
    int validCnt = 0;
    for (int c = 0; c < m; ++c) {
        long long t = totalCnt[c];
        validCnt += int((t * (t - 1) / 2) % MOD);
        if (validCnt >= MOD) validCnt -= MOD;
    }
    vector<int> coords;
    coords.reserve(2 * m);
    for (int x : vals) coords.push_back(x);
    for (int x : vals) coords.push_back(x + k);
    const int SZ = 2 * m;
    vector<int> nearEnd(m);
    int ptr = 0;
    const int q = k / 2;
    for (int r = 0; r < m; ++r) {
        long long lim = (long long)vals[r] + q;
        while (ptr < SZ && coords[ptr] <= lim) { ++ptr; }
        nearEnd[r] = ptr;
    }
    Fenwick bit(SZ);
    vector<int> before(m, 0);
    int Wtot = 0;
    int Vtot = 0;
    long long contribution = 0;
    for (int i = 0; i <= n; ++i) {
        const int r = rankPos[i];
        const int x = vals[r];
        const int b = before[r];
        const int a = totalCnt[r] - b - 1;
        int delta = (b == 0 ? 0 : MOD - b);
        if (delta != 0) {
            bit.add(r + 1, delta, x);
            bit.add(r + 1 + m, delta, x + k);
            Wtot = addmod(Wtot, delta);
            Vtot = addmod(Vtot, mulmod(delta, x));
        }
        auto left = bit.query(r);
        auto pref = bit.query(nearEnd[r]);
        int Wnear = submod(pref.first, left.first);
        int Tnear = submod(pref.second, left.second);
        int Sall = Vtot;
        Sall = submod(Sall, mulmod(x, Wtot));
        Sall = addmod(Sall, mulmod(k, left.first));
        int Snear = submod(Tnear, mulmod(x, Wnear));
        int Wfar = submod(Wtot, Wnear);
        int cur = mulmod(2, Snear);
        cur = addmod(cur, mulmod(k, Wfar));
        cur = submod(cur, Sall);
        contribution += cur;
        if (contribution >= MOD) contribution -= MOD;
        delta = a;
        if (delta != 0) {
            bit.add(r + 1, delta, x);
            bit.add(r + 1 + m, delta, x + k);
            Wtot = addmod(Wtot, delta);
            Vtot = addmod(Vtot, mulmod(delta, x));
        }
        ++before[r];
    }
    long long totalIntervals = 1LL * n * (n + 1) / 2 % MOD;
    int ans = int(contribution % MOD);
    ans = submod(ans, int(totalIntervals));
    ans = addmod(ans, validCnt);
    cout << ans << '\n';
    return 0;
}