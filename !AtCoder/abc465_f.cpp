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

// #define MULTI_TEST_CASES

constexpr int N = 300005, B = 1771561, W = 1<<6;
int64_t fs[B];
struct Node {
    int s, v;
} a[N];
int n, base[6];
inline void read(int *arr) {
    char ch = cin.get();
    while (ch < '0' || ch > '9') ch = cin.get();
    For(i, 0, 5) arr[i] = ch - '0', ch = cin.get(); 
}
inline void f2a(int n, int *arr) {
    For(i, 0, 5) arr[i] = n % 11, n /= 11;
}
inline int a2f(int *arr) {
    int res = 0;
    roF(i, 5, 0) res = res * 11 + arr[i];
    return res;
}
inline void solveSingleTestCase() {
    int q;
    cin >> n;
    base[0] = 1; For(i, 1, 5) base[i] = base[i-1] * 11;
    For(i, 1, n) {
        int arr[6];
        read(arr); cin >> a[i].v;
        for (int &v : arr) ++v;
        fs[a2f(arr)] += a[i].v;
    }
    for(int d = 0; d < 6; d++) {
        for(int i = 0; i < B; i++) {
            int arr[6];
            f2a(i, arr);       
            if(arr[d] > 0) {
                int prev = i - base[d];
                fs[i] += fs[prev];
            }
        }
    }
    cin >> q;
    Rep(q) {
        int xarr[6], yarr[6], arr[6];
        read(xarr), read(yarr);
        for (int &v : yarr) ++v;
        int64_t ans = 0;
        For(i, 0, 5) if (xarr[i] > yarr[i]) ans = -1;
        if (ans == -1) { cout << 0 << '\n'; continue; }
        For(st, 0, W-1) {
            For(dgt, 0, 5)
                if (st >> dgt & 1)
                    arr[dgt] = yarr[dgt];
                else
                    arr[dgt] = xarr[dgt];
            if (Popcnt(st)%2 == 0)
                ans += fs[a2f(arr)];
            else ans -= fs[a2f(arr)];
        }
        cout << ans << '\n';
    }
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