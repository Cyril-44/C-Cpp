#include <cstdio>
#include <cstring>
#include <cstdint>
#include <random>
#pragma GCC optimize(2, "unroll-loops")
constexpr int N = 805, T = 305;
struct Edge { int to, nxt; };
static Edge e[T][N << 1]; static int head[T][N];
[[gnu::always_inline]] inline void addedg(Edge* e, int* head, int fr, int to) {
    e[++head[0]] = Edge {to, head[fr]};
    head[fr] = head[0];
}
uint64_t fOr[N];
int ans[N][N];
[[gnu::always_inline]] inline void bfs(Edge *e, int *head, uint64_t cur, int u, int fa) {
    static int que[N][2];
    int hd=0, tl=1;
    que[0][0] = u, que[0][1] = fa;
    for (; hd ^ tl; ++hd) {
        u = que[hd][0], fa = que[hd][1], fOr[u] ^= cur;
        for (int i = head[u]; i; i = e[i].nxt) if (e[i].to != fa)
            que[tl][0] = e[i].to, que[tl++][1] = u;
    }
}
/*
经典 trick：判断点在路径上，只需要以该点为根，求 LCA(路径端点)==该点 那就是该点在路径上。
*/
int main() {
    std::mt19937 rng(std::random_device{}());
    int n, k;
    scanf("%d%d", &n, &k);
    for (int ik = 0; ik < k; ik++) {
        for (int u, v, i = 1; i < n; i++) {
            scanf("%d%d", &u, &v);
            addedg(e[ik], head[ik], u, v);
            addedg(e[ik], head[ik], v, u);
        }
    }
    for (int u = 1; u <= n; u++) {
        ans[u][u] = 1;
        memset(fOr, 0, sizeof fOr);
        for (int ik = 0; ik < k; ik++)
            for (int i = head[ik][u]; i; i = e[ik][i].nxt)
                bfs(e[ik], head[ik], rng(), e[ik][i].to, u);
        for (int x = 1; x <= n; x++)
            for (int y = x+1; y <= n; y++)
                if (fOr[x] ^ fOr[y]) ++ans[x][y];
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            printf("%d ", (i < j) ? ans[i][j] : ans[j][i]);
        putchar('\n');
    }
    return 0;
}