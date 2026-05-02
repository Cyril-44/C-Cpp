#include <climits>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
constexpr int N = 200005;
std::vector<int> g[N];
int a[N], fs[N], ft[N];
bool ss[N], tt[N]; // 是否有效（是否大于前一个）
int64_t f[N], fg[N];
void bfs(int u, int *f) {
    static int que[N][2];
    static bool vis[N];
    memset(vis, 0, sizeof vis);
    int hd=0, tl=0;
    que[tl++][0] = u, vis[u] = true;
    for (int d; hd ^ tl; ++hd) {
        u = que[hd][0], d = que[hd][1];
        f[d] = std::max(f[d], a[u]);
        for (int v : g[u])
            if (!vis[v]) {
                vis[v] = true;
                que[tl][0] = v, que[tl][1] = d+1;
                ++tl;
            }
    }
}
int n;
int main() {
    int m, A, B, S, T;
    scanf("%d%d%d%d%d%d", &n, &m, &A, &B, &S, &T);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    bfs(S, fs), bfs(T, ft);
#define DO(a) a##a[0] = true; for (int i = 1; i <= n; i++) a##a[i] = f##a[i] > f##a[i-1], f##a[i] = std::max(f##a[i-1], f##a[i])
    DO(s); DO(t);
#define DBG(a) for (int i = 0; i <= n; i++) printf("%c%d%c ", a##a[i] ? '[' : ' ', f##a[i], a##a[i] ? ']' : ' '); putchar('\n')
    // DBG(s); DBG(t);
    int64_t premn = B;
    for (int i = n - 1; i >= 0; i--) {
        f[i] = f[i+1] + std::max(0, (fs[i] - (i ? std::max(ft[i], fs[i-1]) : ft[i]))) * 1l * A;
        fg[i] = f[i+1] + std::max(0, fs[i] - ft[i]) * 1l * A;
        // for (int j = 1; i+j <= n; j++)
        //     f[i] = std::min(f[i], f[i+j] + j * 1l * B);
        f[i] = std::min(f[i], premn);
        fg[i] = std::min(fg[i], premn);
        premn = std::min(premn + B, fg[i] + B);
    }
    // for (int i = 0; i <= n; i++) printf("%ld%c", f[i], " \n"[i==n]);
    printf("%ld\n", f[0]);
    return 0;
}