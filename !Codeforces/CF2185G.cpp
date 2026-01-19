#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
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

constexpr int N = 200005;
vector<int> a[N];
vector<int> as[N];
int64_t f[N];
inline void solveSingleTestCase() {
    int n, mxl = 0, totl = 0;
    cin >> n;
    Forv (i, 1, n, l) {
        cin >> l;
        a[i].resize(l);
        totl += l;
        mxl = max(mxl, l);
        map<int,int> mp;
        for (int &j : a[i]) cin >> j, ++mp[j];
        sort(a[i].begin(), a[i].end());
        a[i].erase(unique(a[i].begin(), a[i].end()), a[i].end());
        as[i].resize(l);
        For (j, 0, (int)a[i].size() - 1) as[i][j] = mp[a[i][j]];
    }
    memset(f, 0, sizeof(int64_t) * (mxl+5));
    int64_t ans = 0;
    For (i, 1, n) {
        if (a[i].front()) {
            int val = 0;
            for (; val < (int)a[i].size(); ++val)
                if (a[i][val] != val+1) break;
            f[0] += val + 1;
        } else {
            int val = 1;
            for (; val < (int)a[i].size(); ++val)
                if (a[i][val] != val) break;
            int tmp = val;
            for (; val < (int)a[i].size(); ++val)
                if (a[i][val] != val + 1) break;
            if (tmp == val) f[tmp]++;
            else f[tmp] += val + 1 - tmp;
            ans += tmp *1ll* totl * (n-1);
        }
    }
    // cerr << "f: ";
    // For (i, 0, mxl) cerr << f[i] << ' ';
    // cerr << '\n';
    // cerr << "orig ans: " << ans << '\n';
    For (i, 1, n) {
        if (a[i].front()) {
            For (jp, 0, (int)a[i].size()-1) {
                int j = a[i][jp];
                if (j < mxl + 5) ans += f[j] *1ll*as[i][jp];
                else break;
            }
        } else {
            int val = 1;
            for (; val < (int)a[i].size(); ++val)
                if (a[i][val] != val) break;
            For (jp, 0, (int)a[i].size()-1) {
                int j = a[i][jp];
                if (j < val) {
                    if (as[i][jp] > 1) ans += f[j] *1ll*as[i][jp];//, cerr << "****";
                    else ans += (j - val) * (n-1ll) + f[j];
                    // cerr << "ans=" << ans << '\n';
                } else {
                    if (j < mxl + 5) ans += f[j] *1ll* as[i][jp];
                    // cerr << "+" << f[j] *1ll* as[i][jp] << '\n';
                }
            }
        }
    }
    cout << ans << '\n';
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