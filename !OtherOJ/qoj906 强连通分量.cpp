#include <cassert>
#include <cstdio>
#include <cstring>
#include <set>
#include <vector>
constexpr int N = 500005;
template<typename T> inline void umin(T &x, T y) { (x > y) && (x = y); }
struct Edge {
    int to, nxt;
} e[N];
int head[N];
inline void addedg(int fr, int to) {
    e[++head[0]] = {to, head[fr]};
    head[fr] = head[0];
}
int dfn[N], low[N], bl2[N], sta[N], ts;
bool insta[N];
std::vector<int> blk[N];
void tarjan(int u) {
    dfn[u] = low[u] = ++ts;
    sta[++sta[0]] = u, insta[u] = true;
    for (int i = head[u]; i; i = e[i].nxt) {
        int v = e[i].to;
        if (!dfn[v]) tarjan(v), umin(low[u], low[v]);
        else if (insta[v]) umin(low[u], dfn[v]);
    }
    if (low[u] == dfn[u]) {
        do {
            blk[u].push_back(sta[sta[0]]);
            insta[sta[sta[0]]] = false;
            bl2[sta[sta[0]]] = u;
        } while (sta[sta[0]--] != u);
    }
}
std::set<int> g[N];
int indeg[N], que[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v, i = 1; i <= m; i++) {
        scanf("%d%d", &u, &v);
        ++u, ++v;
        addedg(u, v);
    }
    for (int i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i);
    for (int i = 1; i <= n; i++)
        for (int j = head[i]; j; j = e[j].nxt)
            if (bl2[i] ^ bl2[e[j].to])
                g[bl2[i]].insert(bl2[e[j].to]);
    for (int i = 1; i <= n; i++)
        for (int j : g[i]) ++indeg[j];
    int hd=0, tl=0, cnt = 0;
    for (int i = 1; i <= n; i++)
        if (!blk[i].empty()) {
            ++cnt;
            if (!indeg[i]) que[tl++] = i;
        }
    printf("%d\n", cnt);
    while (hd^tl) {
        --cnt;
        int u = que[hd++];
        printf("%zu ", blk[u].size());
        for (int x : blk[u]) printf("%d ", x-1);
        putchar('\n');
        for (int i : g[u])
            if (--indeg[i] == 0) que[tl++] = i;
    }
    assert(cnt == 0);
    return 0;
}