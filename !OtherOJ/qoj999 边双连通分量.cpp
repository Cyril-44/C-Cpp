#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
template<typename T> inline void umin(T &x, T y) { (x > y) && (x = y); }
constexpr int N = 200005, M = 200005;
struct Edge {
    int to, nxt;
} e[M << 1];
int head[N]{1};
inline void addedg(int fr, int to) {
    e[++head[0]] = {to, head[fr]};
    head[fr] = head[0];
}
int dfn[N], low[N], ts, sta[N];
std::vector<int> blk[N];
void tarjan(int u, int faEdg) {
    dfn[u] = low[u] = ++ts;
    sta[++sta[0]] = u;
    for (int i = head[u]; i; i = e[i].nxt) {
        if ((i ^ faEdg) == 1) continue;
        int v = e[i].to;
        if (v == u) continue;
        if (!dfn[v]) tarjan(v, i), umin(low[u], low[v]);
        else umin(low[u], dfn[v]);
        if (low[v] > dfn[u]) {
            do {
                blk[v].push_back(sta[sta[0]]);
            } while (sta[sta[0]--] != v);
        }
    }
}
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        ++u, ++v;
        addedg(u, v);
        addedg(v, u);
    }
    for (int i = 1; i <= n; i++)
        if (!dfn[i]) {
            tarjan(i, 0);
            while (sta[0]) blk[i].push_back(sta[sta[0]--]);
        }
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        if (!blk[i].empty()) ++cnt;
    printf("%d\n", cnt);
    for (int i = 1; i <= n; i++)
        if (!blk[i].empty()) {
            printf("%zu ", blk[i].size());
            for (int v : blk[i]) printf("%d ", v-1);
            putchar('\n');
        }
    return 0;
}