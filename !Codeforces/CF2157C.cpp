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

constexpr int N = 105;
bool vis[N];
int a[N];
int arr[N];
inline void solveSingleTestCase() {
    int n, k, q;
    cin >> n >> k >> q;
    iota(arr, arr+n, 0);
    vector<pair<int,int>> mex;
    mex.reserve(n+1);
    memset(vis, 0, sizeof vis);
    memset(a, -1, sizeof a);
	for (int c, l, r; q--;) {
		cin >> c >> l >> r;
		if (c == 1) For (i, l, r) a[i] = k + 1;
		else {
            mex.emplace_back(l, r);
            For (i, l, r) vis[i] = true;
        }
	}
    For (i, 1, n) if (!vis[i] && a[i] == k+1) a[i] = k;
	sort(mex.begin(), mex.end());
	for (auto [l, r] : mex) {
		set<int> st(arr, arr + k);
		For (i, l, r) if (st.count(a[i])) st.erase(a[i]);
		auto it = st.begin();
		For (i, l, r) {
			if (a[i] == -1) {
                if (st.empty()) break;
                a[i] = *it;
                it = st.erase(it);
            }
		}
	}
    For (i, 1, n) {if (a[i] == -1) a[i] = 0;}
	For (i, 1, n) cout << a[i] << (i==n?'\n':' ');
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