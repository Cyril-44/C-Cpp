#include <bits/stdc++.h>
#define LUOGU
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

constexpr int N = 400005;
int a[N], b[N];
char opts[N];
bool del[N];
bool _vis[N<<1], *vis = _vis+N;
vector<int> _vec[N<<1], *vec = _vec+N;
inline void solveSingleTestCase() {
    int n, m, k;
    cin >> n >> m >> k;
    For (i, 1, n) cin >> a[i];
    For (i, 1, m) cin >> b[i];
    cin >> opts;
    sort(a+1, a+1 + n), sort(b+1, b+1 + m);
    b[0] = -1e9, b[m+1] = 2e9;
    int ptr = 1;
    For (i, -k, k) vec[i].clear();
    For (i, 1, n) {
        while (ptr <= m && b[ptr] < a[i]) ++ptr;
        int left = a[i] - b[ptr-1], right = b[ptr] - a[i];
        if (left <= k) vec[-left].push_back(i);
        if (right <= k) vec[right].push_back(i);
    }
    memset(vis - k, 0, 2 * k + 1);
    memset(del, 0, n+1);
    int offset = 0, cnt = n;
    For (oi, 0, k-1) {
        offset += opts[oi] == 'R' ? 1 : -1;
        if (vis[offset]) goto output;
        else vis[offset] = true;
        for (int i : vec[offset])
            if (!del[i]) del[i] = true, --cnt;
        output: cout << cnt << ' ';
    }
    cout << '\n';
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