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

constexpr int N = 500005;
int a[N], pos[N];
int tr[N], f[N], g[N];
bool vis[N];
int n;
inline void upd(int p, int x) {
    for (; p <= n; p += p & -p) tr[p] = max(tr[p], x);
}
inline int query(int p) {
    int val = 0;
    for (; p > 0; p -= p & -p) val = max(val, tr[p]);
    return val;
}
inline void solveSingleTestCase() {
    cin >> n; a[n+1] = 0;
    For (i, 1, n) {
        cin >> a[i];
        pos[a[i]] = i;
    }
    int ans = n;
    
    auto calc = [](int L, int R) {
        set<int> st;
        vector<int> bot;
        For (i, L, R) st.insert(a[i]);
        int tmpl = a[L-1], tmpr = a[R+1];
        a[L-1] = a[R+1] = n+1;
        int ans = 0;
        for (auto it = st.begin(); it != st.end(); it = st.erase(it)) {
            int x = *it, p = pos[x], l, r;
            for (l = p-1; l >= L-1; l--) if (a[l] < a[l+1]) break;
            for (r = p+1; r <= R+1; r++) if (a[r] < a[r-1]) break;
            ++l, --r;
            for (int i = l; i <= r; i++) if (i != p) st.erase(a[i]);
            int mn = min(a[l], a[r]);
            for (; a[l] > mn; ++l);
            for (; a[r] > mn; --r);
            bot.push_back(r - l);
        }
        a[L-1] = tmpl, a[R+1] = tmpr;
        sort(bot.begin(), bot.end());
        bot.pop_back();
        for (int x : bot) ans += x;
        return ans;
    };
    memset(tr, 0, sizeof(int) * (n+1));
    For (i, 1, pos[n]) upd(i, a[i]);
    int cnt = 0;
    roFv (i, pos[n], 2, j) {
        int nxt = query(i - 1);
        for (j = i - 1; a[j] != nxt; j--);
        ans = min(ans, calc(j, i) + j-1 + n-i - cnt++);
        i = j + 1;
    }
    memset(tr, 0, sizeof(int) * (n+1));
    roF (i, n, pos[n]) upd(n-i+1, a[i]);
    cnt = 0;
    Forv (i, pos[n], n-1, j) {
        int nxt = query(n-i+1 - 1);
        for (j = i + 1; a[j] != nxt; j++);
        ans = min(ans, calc(i, j) + i-1 + n-j - cnt++);
        i = j - 1;
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