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

constexpr int N = 0;

inline void solveSingleTestCase() {
    int a, b;
    cin >> a >> b;
    int la = Log2(a) + 1, lb = Log2(b) + 1;
    if (la < lb) { cout << "-1\n"; return; }
    int cnt1 = la - Popcnt(a);
    cout << cnt1 + 1 << '\n';
    for (int i = 0; i < la; ++i)
        if (!(a >> i & 1))
            cout << (1 << i) << ' ';
    cout << (((1 << la) - 1) ^ b) << '\n';
    // else cout << "-1\n";
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}

/*
1001
0110
*/