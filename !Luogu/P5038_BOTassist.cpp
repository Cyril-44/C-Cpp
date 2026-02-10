#include <bits/stdc++.h>
constexpr int N = 45;
using LL = long long;
constexpr std::pair<int,int> Dir[]{{-1,0},{0,1},{1,0},{0,-1}};
int a[N][N];
/*
如果最终全部变成 x,
S 向 黑点连边，x - aij
黑点向白点连边，+oo
白点向 T 连边，x - aij
如果能跑满流就是合法
*/
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    [[gnu::always_inline]] inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    [[gnu::always_inline]] inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;

// HLPP implementation that preserves original MF namespace, function names and behavior.
// Replaces Dinic-based fetch() with a high-performance Highest-Label Preflow-Push (HLPP).
// Keeps: namespace MF, init, backup, restore, addedg, fetch, and the original edge storage layout.

namespace MF {
using EdgeList = std::vector<std::tuple<int, LL, int>>;

struct EdgeNode { int to, nxt; LL cap; };
static EdgeNode g[N*N << 4], gBak[N*N << 4];
static int head[N*N], headBak[N*N], cur[N*N], dis[N*N], que[N*N];
static int n, S, T;
static LL maxflow;

// Additional arrays for HLPP
static LL excessArr[N*N];
static int heightArr[N*N];
static int countHeight[N*N * 2];
static bool inBucket[N*N];
static std::vector<std::deque<int>> buckets;
static int maxHeight;
static int workSinceRelabel;
static const int RELABEL_FREQ = 1 << 16; // tunable

inline void init(int n_, int s, int t) {
    n = n_; S = s; T = t;
    // preserve original head semantics
    memset(head, 0, sizeof(int) * (n+1));
    head[0] = 1;
    // initialize HLPP arrays
    for (int i = 0; i <= n; ++i) {
        excessArr[i] = 0;
        heightArr[i] = 0;
        cur[i] = 0;
        inBucket[i] = false;
    }
    // buckets size: allow up to 2*n+5 heights
    buckets.assign(2 * n + 5, std::deque<int>());
    memset(countHeight, 0, sizeof(int) * (2 * n + 5));
    maxHeight = 0;
    workSinceRelabel = 0;
}

// backup/restore keep original semantics: copy g and head arrays (including head[0])
inline void backup() {
    memcpy(gBak, g, sizeof(EdgeNode) * (head[0] + 1));
    memcpy(headBak, head, sizeof(int) * (n + 1));
}
inline void restore() {
    memcpy(g, gBak, sizeof(EdgeNode) * (headBak[0] + 1));
    memcpy(head, headBak, sizeof(int) * (n + 1));
}

// keep original addedg interface (adds forward and reverse edges consecutively)
inline void addedg(int fr, int to, LL c) {
    g[++head[0]] = {to, head[fr], c}; head[fr] = head[0];
    g[++head[0]] = {fr, head[to], 0}; head[to] = head[0];
}

// Internal helper: push along edge index 'ei' from u
inline void hlpp_push(int u, int ei) {
    int v = g[ei].to;
    LL f = std::min(excessArr[u], g[ei].cap);
    if (f == 0) return;
    // valid push only if height[u] == height[v] + 1
    if (heightArr[u] != heightArr[v] + 1) return;
    g[ei].cap -= f;
    g[ei ^ 1].cap += f;
    excessArr[u] -= f;
    excessArr[v] += f;
    if (v != S && v != T && !inBucket[v] && excessArr[v] > 0) {
        buckets[heightArr[v]].push_front(v);
        inBucket[v] = true;
        if (heightArr[v] > maxHeight) maxHeight = heightArr[v];
    }
}

// Global relabel: BFS from sink over residual graph (using reverse-edge caps)
inline void globalRelabel() {
    int INFH = 2 * n + 2;
    for (int i = 0; i <= 2*n+2; ++i) {
        if (i < (int) (2*n+5)) countHeight[i] = 0;
        if (i < (int) buckets.size()) buckets[i].clear();
    }
    for (int i = 0; i <= n; ++i) {
        heightArr[i] = INFH;
        cur[i] = head[i];
        inBucket[i] = false;
    }
    std::queue<int> q;
    heightArr[T] = 0;
    q.push(T);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        // iterate outgoing edges from v; check reverse edge cap to find u with residual u->v > 0
        for (int ei = head[v]; ei; ei = g[ei].nxt) {
            int u = g[ei].to;
            // reverse edge index is ei^1, which goes from u -> v
            if (g[ei ^ 1].cap > 0 && heightArr[u] == INFH) {
                heightArr[u] = heightArr[v] + 1;
                q.push(u);
            }
        }
    }
    // unreachable nodes get height n+1
    for (int v = 0; v <= n; ++v) {
        if (heightArr[v] == INFH) heightArr[v] = n + 1;
        if (heightArr[v] < (int)(2*n+5)) countHeight[heightArr[v]]++;
    }
    // rebuild buckets with active vertices (exclude S and T)
    maxHeight = 0;
    for (int v = 0; v <= n; ++v) {
        cur[v] = head[v];
        inBucket[v] = false;
        if (v != S && v != T && excessArr[v] > 0 && heightArr[v] <= n) {
            buckets[heightArr[v]].push_front(v);
            inBucket[v] = true;
            if (heightArr[v] > maxHeight) maxHeight = heightArr[v];
        }
    }
    workSinceRelabel = 0;
}

// gap heuristic: when countHeight[g] == 0, for all h > g move vertices to height > n (n+1)
inline void gap(int g) {
    // for heights > g, clear buckets and set heights to n+1
    for (int h = g + 1; h <= maxHeight; ++h) {
        if (h >= (int)buckets.size()) break;
        while (!buckets[h].empty()) {
            int v = buckets[h].front();
            buckets[h].pop_front();
            inBucket[v] = false;
            if (heightArr[v] <= n) {
                countHeight[heightArr[v]]--;
                heightArr[v] = n + 1;
                countHeight[heightArr[v]]++;
            }
        }
    }
    // adjust maxHeight downwards
    while (maxHeight > 0 && (maxHeight >= (int)buckets.size() || buckets[maxHeight].empty())) --maxHeight;
}

// relabel vertex u: set height to min neighbor height + 1
inline void relabel(int u) {
    int minh = INT_MAX;
    for (int ei = head[u]; ei; ei = g[ei].nxt) {
        if (g[ei].cap > 0) {
            minh = std::min(minh, heightArr[g[ei].to]);
        }
    }
    int oldh = heightArr[u];
    int newh = (minh == INT_MAX) ? (n + 1) : (minh + 1);
    heightArr[u] = newh;
    // update counts
    if (oldh < (int)(2*n+5)) countHeight[oldh]--;
    if (newh < (int)(2*n+5)) countHeight[newh]++;
    cur[u] = head[u];
    if (newh > maxHeight) maxHeight = newh;
    workSinceRelabel++;
}

// HLPP main driver: replaces previous Dinic fetch()
inline LL fetch() {
    // initialize HLPP structures
    for (int i = 0; i <= n; ++i) {
        excessArr[i] = 0;
        heightArr[i] = 0;
        cur[i] = head[i];
        inBucket[i] = false;
    }
    int maxHSize = 2 * n + 4;
    for (int i = 0; i <= maxHSize; ++i) {
        countHeight[i] = 0;
        if (i < (int)buckets.size()) buckets[i].clear();
    }
    maxHeight = 0;
    workSinceRelabel = 0;

    // initial preflow from S
    heightArr[S] = n;
    countHeight[0] = n - 1;
    if (n < (int)(2*n+5)) countHeight[n] = 1;

    // push all outgoing edges from S
    for (int ei = head[S]; ei; ei = g[ei].nxt) {
        if (g[ei].cap > 0) {
            LL f = g[ei].cap;
            g[ei].cap -= f;
            g[ei ^ 1].cap += f;
            excessArr[g[ei].to] += f;
            excessArr[S] -= f;
            if (g[ei].to != S && g[ei].to != T && !inBucket[g[ei].to]) {
                buckets[heightArr[g[ei].to]].push_front(g[ei].to);
                inBucket[g[ei].to] = true;
                if (heightArr[g[ei].to] > maxHeight) maxHeight = heightArr[g[ei].to];
            }
        }
    }

    // initial global relabel to get good heights
    globalRelabel();

    // main loop: process active vertices from highest label down
    while (maxHeight >= 0) {
        if (maxHeight >= (int)buckets.size()) { --maxHeight; continue; }
        if (buckets[maxHeight].empty()) { --maxHeight; continue; }
        int u = buckets[maxHeight].front();
        buckets[maxHeight].pop_front();
        inBucket[u] = false;
        if (u == S || u == T) continue;

        // discharge u
        while (excessArr[u] > 0) {
            int ei = cur[u];
            if (!ei) {
                // no more admissible edges -> relabel
                int oldh = heightArr[u];
                relabel(u);
                if (countHeight[oldh] == 0) {
                    gap(oldh);
                }
                if (heightArr[u] > n) break; // isolated
                continue;
            }
            // find next edge with residual cap
            bool pushed = false;
            for (int &eii = cur[u]; eii; eii = g[eii].nxt) {
                if (g[eii].cap > 0 && heightArr[u] == heightArr[g[eii].to] + 1) {
                    hlpp_push(u, eii);
                    pushed = true;
                    if (excessArr[u] == 0) break;
                }
            }
            if (!pushed) {
                // no admissible edge found, set cur[u] = 0 to trigger relabel next
                cur[u] = 0;
            }
        }

        // if still active, reinsert into bucket at its (possibly new) height
        if (excessArr[u] > 0 && heightArr[u] <= n) {
            if (!inBucket[u]) {
                buckets[heightArr[u]].push_front(u);
                inBucket[u] = true;
                if (heightArr[u] > maxHeight) maxHeight = heightArr[u];
            }
        }

        if (++workSinceRelabel >= RELABEL_FREQ) {
            globalRelabel();
        }
    }

    // result: excess at T equals max flow
    maxflow = excessArr[T]; 
    return maxflow;
}

} // namespace MF


int main() {
    int T, n, m;
    in(T);
    while (T--) {
        in(n), in(m);
        int mx = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) {
                in(a[i][j]);
                mx = std::max(mx, a[i][j]);
            }
        int nm = n * m >> 1;
        LL black = 0, white = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                ((i & 1) ^ (j & 1) ? white : black) += a[i][j];
#define mapper(x, y) ((x - 1) * m + y)
        int S = n * m + 1, T = S + 1;
        MF::init(T, S, T);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                if ((i&1) ^ (j&1))
                    for (int k = 0; k ^ 4; k++) {
                        int tx = i + Dir[k].first, ty = j + Dir[k].second;
                        if (1 <= tx && tx <= n && 1 <= ty && ty <= m)
                            MF::addedg(mapper(tx, ty), mapper(i, j), (LL)1e13);
                    }
        MF::backup();
        auto check = [&](LL x) {
            MF::restore();
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= m; j++)
                    ((i&1) ^ (j&1)) ? 
                    MF::addedg(mapper(i, j), T, x - a[i][j]) : 
                    MF::addedg(S, mapper(i, j), x - a[i][j]);
#undef mapper
            // fprintf(stderr, "Working on %lld, expected %d*%lld-%lld, found %lld%s\n", x, nm, x, white, MF::fetch(), MF::fetch() == nm * x - white ? " (Equal)" : "");
            return MF::fetch() == nm * x - white;
        };
        // fprintf(stderr, "=========\n");
        if ((n&1) && (m&1)) {
            LL x = black - white;
            // fprintf(stderr, "%lld\n", x);
            x >= mx && check(x) ? printf("%lld\n", x * n * m - black - white >> 1) : puts("-1");
        }
        else if (black != white) puts("-1");
        else {
            LL l = mx, r = 1e13, mid;
            while (l <= r) {
                mid = l + r >> 1;
                if (check(mid)) r = mid - 1;
                else l = mid + 1;
            }
            printf("%lld\n", l > (LL)1e13 ? -1 : l * n * m - black - white >> 1); 
        }
    }
    return 0;
}