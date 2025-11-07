#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...) for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

#define MULTI_TEST_CASES

inline void solveSingleTestCase() {
    int n, k;
    string s, t;
    cin >> n >> k >> s >> t;
    if (s == t) {
        cout << "0\n";
        return;
    }
    vector<int> r(n), now(n);
    Forv (i, 0, n-1, j=0) {
        if (i > j) { cout << "-1\n"; return; }
        while (j < n && (j-i) <= k && (t[j] == s[i])) ++j;
        if (i == j) { cout << "-1\n"; return; }
        r[i] = j - 1;
    }
    iota(now.begin(), now.end(), 0);
    vector<string> ans;
    Rep (k) {
        string current;
        current.resize(n);
        Forv (i, 0, n-1, p=0) {
            if (now[i] < r[i]) ++now[i];
            while (p <= now[i]) current[p++] = s[i];
        }
        if (ans.empty() || current != ans.back()) ans.emplace_back(current);
        else break;
    }
    cout << ans.size() << '\n';
    for (const auto &s : ans) cout << s << '\n';
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}