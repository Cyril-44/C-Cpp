#include <bits/stdc++.h>
using namespace std;
using i64_t = long long;
using u64_t = unsigned long long;
using pii_t = pair<int, int>;
#ifdef ONLINE_JUDGE
# pragma GCC optimize(2)
#endif
#pragma GCC target("tune=native")
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(i, c) for (int tempFor_##c = c; tempFor_##c; --tempFor_##c)
#define YES return cout << "YES\n", void()
#define NO return cout << "NO\n", void()
#define YESNO(j) cout << ((j) ? "YES\n" : "NO\n")
Inline int Popcnt(int x) { return __builtin_popcount((unsigned)x); }
Inline int Popcnt(unsigned x) { return __builtin_popcount(x); }
Inline int Popcnt(i64_t x) { return __builtin_popcountll((u64_t)x); }
Inline int Popcnt(u64_t x) { return __builtin_popcountll(x); }
Inline int Log2(int x) { return 31 - __builtin_clz((unsigned)x | 1); }
Inline int Log2(unsigned x) { return 31 - __builtin_clz(x | 1); }
Inline int Log2(i64_t x) { return 63 - __builtin_clzll((u64_t)x | 1); }
Inline int Log2(u64_t x) { return 63 - __builtin_clzll(x | 1); }

constexpr int N = 200005;
int a[N], b[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    For (i, 1, n) cin >> a[i];
    For (i, 1, n) cin >> b[i];
    map<int, int> fac;
    long long ans = 1ll << 60;
    set<int> purefac;

    For (i, 1, n) {
        vector<vector<int>> facsarr;
        For (now, a[i], a[i]+2) {
            int lmt = sqrt(now);
            int tp = now;
            vector<int> facs;
            For (j, 2, lmt) {
                if (tp % j == 0) {
                    while (tp % j == 0) tp /= j;
                    facs.push_back(j);
                }
            }
            if (tp > 1) facs.push_back(tp);
            facsarr.push_back(facs);
            int cur = b[i] * (now - a[i]);
            for (int j : facs)
                if (fac.count(j))
                    ans = min(ans, 0ll + fac[j] + cur);
            if (now == a[i])
                for (int j : facs)
                    purefac.insert(j);
        }
        int add = 0;
        for (const auto &facs : facsarr) {
            int cur = b[i] * (add);
            for (int j : facs) {
                if (fac.count(j)) fac[j] = min(fac[j], cur);
                else fac[j] = cur;
            }
            ++add;
        }
    }
    For (i, 1, n) {
        auto it = purefac.upper_bound(a[i]);
        if (it != purefac.end()) {
            ans = min(ans, 1ll * b[i] * (*it - a[i]));
        }
    }
    cout << ans << '\n';
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}

/*
1
2
385 16
1000000000 1
*/