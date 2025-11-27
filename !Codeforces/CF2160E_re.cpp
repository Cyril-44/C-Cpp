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

constexpr int N = 0, INF = 1e9;

inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    vector<string> G;
    bool swapped = false;
    if (n > m) {
        swapped = true;
        vector<string> tg(n+1);
        G.resize(m+1, string(n+1, '\0'));
        For (i, 1, n)
            cin >> tg[i];
        For (j, 1, m)
            For (i, 1, n)
                G[j][i] = tg[i][j-1];
        swap(n, m);
    } else {
        G.resize(n+1, string(m+1, '\0'));
        For (i, 1, n) {
            cin >> G[i];
            G[i] = '\0' + G[i];
        }
    }
    vector<vector<int>> f(n+1, vector<int>(m+1, INF));
    For (l, 1, n-1) {
        vector<vector<int>> g(n+1, vector<int>(m+1, INF));
        For (r, l+1, n) {
            vector<int> ones;
            For (j, 1, m) if (G[l][j] == '1' && G[r][j] == '1') ones.emplace_back(j);
            For (i, 1, (int)ones.size() - 1)
                For (j, ones[i-1], ones[i])
                    g[r][j] = min(g[r][j], (r - l + 1) * (ones[i] - ones[i-1] + 1));
        }
        roF (i, n-1, l)
            For (j, 1, m)
                g[i][j] = min(g[i][j], g[i+1][j]);
        For (i, l, n)
            For (j, 1, m)
                f[i][j] = min(f[i][j], g[i][j]);
    }
    if (swapped) {
        For (j, 1, m) {
            For (i, 1, n)
                cout << (f[i][j] == INF ? 0 : f[i][j]) << ' ';
            cout << '\n';
        }
    } else {
        For (i, 1, n) {
            For (j, 1, m)
                cout << (f[i][j] == INF ? 0 : f[i][j]) << ' ';
            cout << '\n';
        }
    }
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}