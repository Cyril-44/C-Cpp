#include <stdio.h>
#include <string.h>
#include <vector>
#include <limits>
#include <array>
#include <tuple>
#include <bitset>
using LL = long long;
constexpr int N = 755, M = (int)1e7 + 5;
char str[M];
std::array<std::bitset<N>, N> adj, g, atp; // Substr Order Graph
namespace ACAM {
struct Node {
    int son[2], fa, fail, id;
};
std::array<Node, M> ac;
std::array<int, M> fa;
int tot = 0;
inline int newnode(int fa) {
    ac[++tot].fa = fa;
    return ++tot;
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
    que[tail++] = 0;
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
}
int idx[N];

struct MaxFlow {
    MaxFlow(int n_) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), maxflow(), n(n_), S(), T() {}
    inline void set(int s, int t) { S = s, T = t; }
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
private:
    using EdgeList = std::vector<std::tuple<int,int,int>>;
    EdgeList *g;
    int *dis, *head;
    int maxflow, n, S, T;
};
namespace BinaryGraph {

}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%s", str);
        idx[i] = ACAM::insert(str, i);
    }
    ACAM::build();
    for (int i = 1; i <= n; i++) {
        int u = idx[i];
        for (int u = idx[i]; u; u = ACAM::ac[u].fa) {
            int j = ACAM::find(u);
            if (j) adj[ACAM::ac[j].id][i] = true; // j \in i
        }
    }
    
    return 0;
}
