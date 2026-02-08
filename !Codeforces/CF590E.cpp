#include <limits>
#include <stdio.h>
#include <string.h>
#include <type_traits>
#include <vector>
#include <array>
#include <tuple>
#include <bitset>
using LL = long long;
bool st_;
constexpr int N = 755, M = (int)1e6 + 5;
char str[M];
using Graph = std::array<std::bitset<N>, N>;
Graph adj; // Substr Order Graph
int idx[N];
namespace ACAM {
struct Node {
    int son[2], fa, fail, id;
};
std::vector<Node> ac(1);
std::array<int, M> fa;
int tot = 0;
inline int newnode(int fa) {
    ac[++tot].fa = fa;
    return tot;
}
inline int insert(char *s, int id) {
    int u = 0;
    for (; *s; ++s) {
        int &v = ac[u].son[*s - 'a'];
        if (!v) v = newnode(u);
        u = v;
    }
    ac[u].id = id;
    return u;
}
inline void build() {
    static std::array<int, M> que;
    int head = 0, tail = 0;
    for (int i = 0; i < 2; i++)
        if (ac[0].son[i]) que[tail++] = ac[0].son[i];
    while (head < tail) {
        int u = que[head++];
        for (int i = 0; i < 2; i++)
            if (ac[u].son[i]) { // u->son_i->fail = u->fail->son_i
                ac[ac[u].son[i]].fail = ac[ac[u].fail].son[i];
                que[tail++] = ac[u].son[i];
            } else { // u->son_i = u->fail->son_i
                ac[u].son[i] = ac[ac[u].fail].son[i];
            }
    }
    for (int i = 1; i <= tot; i++)
        fa[i] = ac[i].id ? i : ac[i].fail;
}
inline int find(int x) { return (x ^ fa[x]) ? (fa[x] = find(fa[x])) : x; }
inline void buildGraph(int n) {
    for (int i = 1; i <= n; i++) {
        int j = find(ac[idx[i]].fail);
        if (j) adj[ac[j].id][i] = true;
        for (int u = ac[idx[i]].fa; u; u = ac[u].fa) {
            int j = find(u);
            // printf("In %d, node %d(0_%d,1_%d,f_%d) found %d(0_%d,1_%d,f_%d,id:%d)\n", i, u, ac[u].son[0], ac[u].son[1], ac[u].fail, j, ac[j].son[0], ac[j].son[1], ac[j].fail, ac[j].id);
            if (j) adj[ac[j].id][i] = true; // j \in i
        }
    }
}
}

struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), maxflow(), n(n_), S(s), T(t) {}
    inline void addedg(int fr, int to, int c) {
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, 0, (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        static std::array<int, N * N * N> que;
        memset(dis, 0, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S; dis[S] = 1;
        while (hd < tl) {
            int u = que[hd++];
            for (const auto &[v, cap, bak] : g[u])
                if (cap && !dis[v]) {
                    dis[v] = dis[u] + 1;
                    if (v == T) return true;
                    que[tl++] = v;
                }
        }
        return false;
    }
    int dfs(int u, int infl) {
        if (u == T) return infl;
        int outfl = 0;
        for (int &i = head[u]; i ^ (int)g[u].size(); i++) {
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                int fl = dfs(v, std::min(infl, cap));
                if (fl) {
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                    outfl += fl, infl -= fl;
                    if (!infl) return outfl;
                }
                else dis[v] = -1;
            }
        }
        return outfl;
    }
    inline int operator()() {
        if (maxflow) return maxflow;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<decltype(maxflow)>::max());
        return maxflow;
    }

    using EdgeList = std::vector<std::tuple<int,int,int>>;
    EdgeList *g;
    int *dis, *head;
    int maxflow, n, S, T;
};
namespace Solver {
std::vector<int> edg[N << 1];
bool vis[N];
void dfs(int u) {
    if (vis[u]) return;
    vis[u] = true;
    for (int v : edg[u]) dfs(v);
}
inline void solve(Graph g, int n) {
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            if (g[i][k]) g[i] |= g[k];
    // for (int i = 1; i <= n; i++)
    //     for (int j = 1; j <= n; j++)
    //         if (g[i][j]) printf("%d %d\n", i, j);
    int S = 2*n + 1, T = 2*n + 2;
    const auto sender = [](int x) { return (x << 1) - 1; };
    const auto receiver = [](int x) { return x << 1; };
    MaxFlow mf(n * 2 + 2, S, T);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (i ^ j && g[i][j])
                mf.addedg(sender(i), receiver(j), 1);
    for (int i = 1; i <= n; i++)
        mf.addedg(S, sender(i), 1), mf.addedg(receiver(i), T, 1);
    printf("%d\n", n - mf());
    for (int i = 1; i <= n; i++) {
        for (const auto &[v, cap, bak] : mf.g[sender(i)])
            if (cap) // 非匹配点 Y 走非匹配边到 X
                edg[v].push_back(sender(i));
            else     // X 走匹配边到 Y
                edg[sender(i)].push_back(v);
    }
    for (int i = 1; i <= n; i++) dfs(receiver(i));
    for (int i = 1; i <= n; i++) { // 在 Y 中被访问到的点 \cap 在 X 中未被访问到的点
        if (vis[receiver(i)] && !vis[sender(i)])
            printf("%d ", i);
    }
    putchar('\n');
}
}
bool ed_;
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%s", str);
        idx[i] = ACAM::insert(str, i);
    }
    ACAM::build();
    ACAM::buildGraph(n);
    Solver::solve(adj, n);
    fprintf(stderr, "Used memory: %gMiB\n", (&ed_-&st_)/1024./1024.);
    // system("grep Vm /proc/$PPID/status");
    return 0;
}
