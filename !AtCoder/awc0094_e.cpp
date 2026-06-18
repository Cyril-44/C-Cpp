#include <bits/extc++.h>
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

constexpr int N = 200005, INF = 0x3f3f3f3f, DR[4][2]={{-1,0},{0,1},{1,0},{0,-1}};
vector<pii_t> g[N];
int dis[N];
inline int dij(int s, int t) {
    memset(dis, 0x3f, sizeof dis);
    dis[s] = 0;
    priority_queue<pii_t, vector<pii_t>, greater<pii_t>> pq;
    pq.emplace(dis[s], s);
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du != dis[u]) continue;
        if (u == t) break;
        for (auto [v, d] : g[u])
            if (dis[v] > dis[u] + d)
                pq.emplace(dis[v] = dis[u] + d, v);
    }
    return dis[t] == INF ? -1 : dis[t];
}
inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    auto id = [m](int x, int y) { return x * m + y; };
    vector<string> mp(n);
    For(i, 0, n-1) cin >> mp[i];
    int st(-1), ed(-1);
    For(x, 0, n-1) For(y, 0, m-1) {
        if (mp[x][y] == 'X') continue;
        int idfr = id(x, y);
        if (mp[x][y] == 'S') st = idfr;
        if (mp[x][y] == 'G') ed = idfr;
        for(auto d : DR) {
            int tx = x + d[0], ty = y + d[1];
            if (tx < 0 || tx >= n || ty < 0 || ty >= m || mp[tx][ty] == 'X') continue;
            int idto = id(tx, ty);
            if (mp[tx][ty] != 'V') g[idfr].emplace_back(idto, 1);
            else if (mp[x][y] == 'V') g[idfr].emplace_back(idto, 0);
            else g[idfr].emplace_back(idto, 2);
        }
    }
    int res = dij(st, ed);
    if (res == -1) cout << "NO\n";
    else cout << res << '\n'; 
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
//P15949