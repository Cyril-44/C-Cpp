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
    vector<string> s(n+1);
    For(i, 1, n) {
        cin >> s[i];
        s[i] = ' ' + s[i];
    }
    vector<vector<bool>> vis(n+1, vector<bool>(m+1));
    vector<vector<bool>> flg(n+1, vector<bool>(m+1));
    queue<pair<int,int>> q;
    auto doit = [&]() {
        auto ns = s;
        For(x, 1, n) For(y, 1, m) {
            if (s[x][y] == '#') { ns[x][y] = '.'; continue; }
            ns[x][y] = '.';
            for (int dx : {-1,0,1}) for (int dy : {-1,0,1}) {
                int tx = x+dx, ty = y+dy;
                if (1<=tx && tx<=n && 1<=ty && ty<=m && s[tx][ty] == '#')
                    { ns[x][y] = '#'; break; }
            }
        }
        ns.swap(s);
    };
    doit(), doit();
    For(i, 1, n) For(j, 1, m)
        if (s[i][j] == '#')
            q.emplace(i, j), vis[i][j] = flg[i][j] = true;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        for (int dx : {-1,0,1}) for (int dy : {-1,0,1}) {
            int tx = x+dx, ty = y+dy;
            if (1<=tx && tx<=n && 1<=ty && ty<=m && !vis[tx][ty])
                q.emplace(tx, ty), vis[tx][ty] = true, flg[tx][ty] = !flg[x][y];
        }
    }
    For(i, 1, n) {
        For(j, 1, m)
            cout.put(flg[i][j] ? '#' : '.');
        cout.put('\n');
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