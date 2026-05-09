#include <cstdio>
#include <cmath>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stack>
#include <numeric>
constexpr int N = 50005, M = 100005;
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) ((void)0)
#endif
/*
先考虑只有询问怎么做。
如果只有询问，可以将所有的询问离线下来，然后询问的权值从小到大排序，按照权值依次加边，维护并查集大小即可。
考虑对修改
*/
struct Edge { int u, v, w; bool used; };
struct Modification { int ts, i, w; };
struct Query { int ts, u, w, id; };
static Edge e[M], e_sorted[M];
static std::vector<Modification> mods;
static std::vector<Query> ques;
static int n, m, fa[N], sz[N];
static std::stack<std::pair<int,int>> unions; // 0 --> 1

inline int find(int u) { return u ^ fa[u] ? find(fa[u]) : u; }
inline bool unite(int u, int v) {
    u = find(u), v = find(v);
    if (u != v) {
        if (sz[u] > sz[v]) std::swap(u, v);
        unions.emplace(u, v);
        fa[u] = v, sz[v] += sz[u];
        return true;
    }
    return false;
}
inline void rollback(int cnt = 1) {
    while (cnt--) {
        auto [u, v] = unions.top(); unions.pop();
        fa[u] = u, sz[v] -= sz[u];
    }
}

inline void solve() {
    auto mods_bk = mods;
    debug("New block started...\n");
    std::vector<int> modpos(mods.size());
    for (int i = 0; i < (int)mods.size(); i++)
        modpos[i] = mods[i].i;
    std::sort(modpos.begin(), modpos.end());
    modpos.erase(std::unique(modpos.begin(), modpos.end()), modpos.end());
    debug("Modified Positions: ");
    for (int i : modpos) debug("%d ", i);
    debug("\n");
    int tot = 0, qtot = 0, curidx = 0;
    for (int i = 1; i <= m; i++)
        if (curidx != (int)modpos.size() && modpos[curidx] == i) mods.push_back({0, i, e[i].w}), ++curidx;
        else e_sorted[++tot] = e[i];
    std::vector<int> ans(ques.size());
    for (auto &q : ques) q.id = qtot++;
    std::sort(e_sorted+1, e_sorted+1+tot, [](const Edge &x, const Edge &y) { return x.w > y.w; });
    std::sort(ques.begin(), ques.end(), [](const Query &x, const Query &y) { return x.w > y.w; });
    std::sort(mods.begin(), mods.end(), [](const Modification &x, const Modification &y) { return x.ts > y.ts; });
    int cur = 1;
    std::iota(fa+1, fa+1+n, 1);
    std::fill(sz+1, sz+1+n, 1);
    for (auto &q : ques) {
        debug("Inquiring #%d %d [%d]\n", q.ts, q.u, q.w);
        for (; cur <= tot && e_sorted[cur].w >= q.w; ++cur) {
            unite(e_sorted[cur].u, e_sorted[cur].v);
            debug("Applying (%d<->%d), w=%d\n", e_sorted[cur].u, e_sorted[cur].v, e_sorted[cur].w);
        }
        int cnt = 0;
        auto idx = std::lower_bound(mods.begin(), mods.end(), q.ts, [](const Modification& x, int y) { return x.ts > y; });
        for (auto it = idx; it != mods.end(); ++it)
            if (!e[it->i].used) {
                if (q.w <= it->w) {
                    debug("Temporarily added ts%dEdge#%d (%d<->%d), w=%d\n", it->ts, it->i, e[it->i].u, e[it->i].v, it->w);
                    cnt += unite(e[it->i].u, e[it->i].v);
                }
                e[it->i].used = true;
            }
        ans[q.id] = sz[find(q.u)];
        debug("The answer is %d\n", sz[find(q.u)]);
        rollback(cnt);
        for (auto it = idx; it != mods.end(); ++it)
            e[it->i].used = false;
    }
    for (int _ans : ans)
        printf("%d\n", _ans);
    for (const auto &mod : mods_bk) e[mod.i].w = mod.w; // Permanently Modify
}
int main() {
    int q, cnt = 0;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++)
        scanf("%d%d%d", &e[i].u, &e[i].v, &e[i].w);
    scanf("%d", &q);
    int B = sqrt(q * log2(m+1));
    mods.reserve(B*2), ques.reserve(B);
    for (int i = 1, t, x, y; i <= q; i++) {
        scanf("%d%d%d", &t, &x, &y);
        if (t == 1) mods.push_back({i, x, y});
        else ques.push_back({i, x, y, 0});
        if (++cnt == B || i == q) {
            solve();
            mods.clear(), ques.clear();
            cnt = 0;
        }
    }
    return 0;
}
