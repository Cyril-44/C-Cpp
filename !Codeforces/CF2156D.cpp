#include <bits/stdc++.h>
using namespace std;
using i64_t = long long;
using u64_t = unsigned long long;
using pii_t = pair<int, int>;
#ifdef ONLINE_JUDGE
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

constexpr int N = 20005;
int f[N];
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    int now = 0;
    memset(f, -1, sizeof(int) * (n+1));
    int ans = 0;
    while (true) {
        int cnt1 = 0, cnt2 = 0;
        For (i, 1, n-1) {
            if (~f[i]) continue;
            cout << "? " << i << ' ' << (1<<now) << endl;
            cin >> f[i];
            cnt1 += f[i];
        }
        For (i, 1, n) {
            if ((i & (1<<now)-1) == ans && (i & 1<<now)) ++cnt2;
        }
        if (!cnt2) break;
        if (cnt1 != cnt2) {
            ans |= 1 << now; // 1<<now 少一个
            For (i, 1, n-1) {
                if (f[i] == 0) f[i] = 2;
                else if (f[i] == 1) f[i] = -1;
            }
        }
        else { // 1<<now 为 0 少一个
            For (i, 1, n-1) {
                if (f[i] == 1) f[i] = 2;
                else if (f[i] == 0) f[i] = -1;
            }
        }
        ++now;
    }
    cout << "! " << ans << endl;
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}