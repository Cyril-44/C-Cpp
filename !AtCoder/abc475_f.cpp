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
#define EXIT(s...) return (cout << s), void();
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    vector<string> mp(n+1);
    bool flg1 = true, flg2 = true;
    For(i, 1, n) {
        cin >> mp[i];
        mp[i] = " " + mp[i];
        if (mp[i].find('#') != string::npos) flg1 = false;
        if (mp[i].find('.') != string::npos) flg2 = false;
    }
    if (flg1) {
        cout << (n+1ll)*n/2 * (m+1ll)*m/2 + 1;
        return;
    }
    if (flg2) { cout << "1"; return; }
    if (n > m) {
        vector<string> mp2(m+1);
        For(i, 1, m) {
            mp2[i].resize(n+1);
            For(j, 1, n)
                mp2[i][j] = mp[j][i];
        }
        mp = mp2;
        std::swap(n, m);
    }
    vector<vector<int>> sum(n+1);
    sum[0].resize(m+1);
    For(i, 1, n) {
        sum[i].resize(m+1);
        For(j, 1, m) sum[i][j] = sum[i-1][j] + (mp[i][j]=='#');
    }
    int64_t ans = 0;
    vector<int> empt(m+1);
    For(l, 1, n) For(r, l, n) {
        int cont = 0;
        roF(i, m, 1) {
            if (mp[l][i] == '#') {
                ++cont;
                if (sum[r][i] - sum[l-1][i] == r-l+1)
                    ans += m-i+1;
                else ans += cont;
            } else cont = 0;
            // cerr << l << ',' << r << ',' << i << ": " << ans << endl;
        }
        fill(empt.begin(), empt.end(), 0);
        cont = 0;
        int last = 0;
        For(i, 1, m) {
            empt[i] = empt[i-1];
            if (mp[l][i] == '.') last = i;
            if (mp[r][i] == '#') ++cont;
            else cont = 0;
            if (sum[r][i] - sum[l-1][i] != r-l+1) {
                ++empt[i];
                if (last >= i - cont)
                    ans += empt[last] - empt[i - cont];
            } else {
                ans += empt[last];
            }
            // cerr << l << ',' << r << ',' << i << ": " << ans << ' ' << last << ' ' << empt[i] << endl;
        }
    }
    cout << (n+1ll)*n/2 * (m+1ll)*m/2 - ans + 1;
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