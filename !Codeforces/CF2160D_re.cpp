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

constexpr int N = 605;
int a[N];

inline int query(const vector<int> &x) {
    cout << "? " << x.size();
    for (int i : x) cout << ' ' << i;
    cout << endl;
    int ans;
    cin >> ans;
    return ans;
}

inline void solveSingleTestCase() {
    int n;
    cin >> n;
    memset(a, -1, sizeof(int) * (2*n+1));
    vector<int> diffs{1};
    diffs.reserve(2 * n);
    Forv (i, 2, 2*n, q) {
        diffs.emplace_back(i);
        if (q = query(diffs)) {
            a[i] = q;
            diffs.pop_back();
        }
    }
    diffs.clear();
    For (i, 1, 2*n)
        if (~a[i]) diffs.emplace_back(i);
    For (i, 1, 2*n)
        if (a[i] == -1) {
            diffs.emplace_back(i);
            a[i] = query(diffs);
            diffs.pop_back();
        }
    cout << '!';
    For (i, 1, 2*n) cout << ' ' << a[i];
    cout << endl;
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}