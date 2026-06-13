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

#define MULTI_TEST_CASES

constexpr int N = 300005, F=14, K = 11+F, INF = 0x3f3f3f3f;
int f[N][K][4][4];
char s[N];
int n;
inline int id(int p) {
    switch (s[p-1]) {
    case 'A': return 1;
    case 'B': return 2;
    case 'C': return 3;
    default:  return 0;
    }
}
// inline bool abc(int p) { return (s[p-3] == 'A') && (s[p-2] == 'B') && (s[p-1] == 'C'); }
inline int cost (int p) {
    return (s[p-3] != 'A') + (s[p-2] != 'B') + (s[p-1] != 'C');
}
// inline bool brk(int p) {
//     if (p > n-2) return false;
//     if ((p-2<3||abc(p-2)) || (p-1<3||abc(p-1)) || abc(p) || abc(p+1) || abc(p+2)) return true;
//     return false;
// }
inline void umin(int&x, int y) { if(x>y) x=y; }
inline void solveSingleTestCase() {
    int k;
    cin >> s >> k;
    n = strlen(s);
    s[n]=s[n+1]=0;
    memset(f, 0x3f, sizeof(int) * K * 16 * (n+1));
    f[0][F][0][0] = 0;
    f[1][F][0][id(1)] = 0;
    f[2][F][id(1)][id(2)] = 0;
    For(i, 3, n) {
        For(j, 0, k+F) {
            For(a, 0, 3) For(b, 0, 3)
                umin(f[i][j][b][id(i)], f[i-1][j][a][b]);
            if (i >= 3) For(a, 0, 3) For(b, 0, 3) {
                int des = 0;
                if (a==1 && b==2 && id(i-2)==3) ++des;
                if (b==1 && id(i-2)==2 && id(i-1)==3) ++des;
                if (id(i-2)==1 && id(i-1)==2 && id(i)==3) ++des;
                if (id(i-1)==1 && id(i)==2 && id(i+1)==3) ++des;
                if (id(i)==1 && id(i+1)==2 && id(i+2)==3) ++des;
                int pre = j-1+des;
                if (pre < 0 || pre > k+F) continue;
                umin(f[i][j][2][3], f[i-3][pre][a][b] + cost(i));
            }
        }
        // For(j, 0, k+F) {
        //     printf("i=%d, j=%d: ", i, j-F);
        //     For(a, 0, 3) For(b, 0, 3) printf("%d ", f[i][j][a][b]);
        //     putchar('\n');
        // }
    }
    int ans = INF;
    For(a, 0, 3) For(b, 0, 3) umin(ans, f[n][k+F][a][b]);
    cout << (ans == INF ? -1 : ans) << '\n';
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