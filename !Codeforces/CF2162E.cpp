#include <bits/stdc++.h>
using namespace std;
using i64_t = long long;
using u64_t = unsigned long long;
using pii_t = pair<int, int>;
#ifndef ONLINE_JUDGE
# pragma GCC optimize(2)
#endif
#pragma GCC target("tune=native")
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_##c = c; tempFor_##c; --tempFor_##c)
#define Repv(c, ...) for (int tempFor_##c = c, __VA_ARGS__; tempFor_##c; --tempFor_##c)
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
int a[N * 2];
inline void solveSingleTestCase() {
    int n, k;
    cin >> n >> k;
    For (i, 1, n) cin >> a[i];
    set<int> st;
    For (i, 1, n) st.insert(i);
    For (i, 1, n) if (st.count(a[i])) st.erase(a[i]);
    auto it = st.begin();
    int m = unique(a+1, a+1 + n) - a-1;
    For (i, 1, min((int)st.size(), k)) cout << (a[++m] = *it++) << ' ';
    For (i, min((int)st.size(), k) + 1, k) {
        ++m;
        a[m] = a[m-3];
        cout << a[m] << ' ';
    }
    cout << '\n';
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}