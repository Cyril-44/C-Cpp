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

// #define MULTI_TEST_CASES

constexpr int N = 0;
map<int, int> res;
int interact(int y) {
    if (res.count(y)) return res[y];
    cout << "? 1 " << y << endl;
    cout.flush();
    int tmp; cin >> tmp;
    if (tmp == -1) throw runtime_error("Out of limit!");
    res[y] = tmp;
    return tmp;
}
int interact(int x, int y) {
    cout << "? " << x << ' ' << y << endl;
    cout.flush();
    int tmp; cin >> tmp;
    if (tmp == -1) throw runtime_error("Out of limit!");
    return tmp;
}
constexpr int MX = 1e9;
inline void solveSingleTestCase() {
    int l = 1, r = MX, ml, mr, len1, len2;
    interact(1), interact(MX);
    while (r - l >= 3) {
        len1 = (r - l + 3) / 3;
        len2 = (r - l + 3) / 3;
        ml = l + len1, mr = r - len2;
        int ql = interact(ml), qr = interact(mr);
        switch ((res[l] - ql != len1) << 1 | (res[r] - qr != len2)) {
        case 0:
        case 3: l = ml, r = mr;
        break;
        case 2: r = ml;
        break;
        case 1: l = mr;
        break;
        }
        fprintf(stderr, "[%d,%d]\n", l, r);
    }
    interact(l + r >> 1);
    int left = 1e9;
    for (auto [_, i] : res) left = min(left, i);
    int uy = res[1] - left, dy = MX - (res[MX] - left);
    int x1 = interact(MX, 1);
    int right = x1 - uy;
    int ux = left + 1, dx = MX - right;
    cout << "! " << ux << ' ' << uy + 1 << ' ' << dx << ' ' << dy << endl;
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
/*
3
999999996
333333330
666666663
111111108
222222219
37037034 
74074071
12345676
24691355
4115223
8230450
1371739
2743481
457244
914492
152412
304829
50802
101607
16932
33867
5642
11287
1878
3761
624 
1251
206
415
66
137
20
43
4
13
1
2
1
1
999999998
*/