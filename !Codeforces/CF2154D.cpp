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

constexpr int N = 200005;
struct Edge {
    int to, nxt;
} e[N << 1];
int head[N];
inline void addedg(const int fr, const int to) {
    e[++head[0]] = {to, head[fr]};
    head[fr] = head[0];
}
vector<int> path;
map<int, int> nxt;
int n;
bool getpath(int u, int fa) {
    if (u == n) return path.emplace_back(u), true;
    for (int i = head[u], v; i; i = e[i].nxt) {
        v = e[i].to;
        if (v == fa) continue;
        if (getpath(v, u)) return path.emplace_back(u), true;
    }
    return false;
}
void dfs(int u, int fa) {
    vector<int> rem;
    for (int i = head[u], v; i; i = e[i].nxt) {
        v = e[i].to;
        if (v == fa || v == nxt[u]) continue;
        dfs(v, u);
        rem.emplace_back(v);
    }
}
inline void solveSingleTestCase() {
    cin >> n;
    for (int i = n, u, v; --i; ) {
        cin >> u >> v;
        addedg(u, v);
        addedg(v, u);
    }
    getpath(1, 0);
    nxt.clear();
    for (int i = path.size() - 1; i > 0; i--)
        nxt[path[i]] = path[i-1];
    dfs(1, 0);
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}