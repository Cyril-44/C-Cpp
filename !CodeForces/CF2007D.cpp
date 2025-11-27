#include <stdio.h>
#include <string.h>
const int N = 100005;
struct Edge {
    int to, nxt;
} e[N << 1];
int head[N], f[N][3];
int a[N];
inline void addedg(const int &fr, const int &to) {
    e[++head[0]] = {to, head[fr]};
    head[fr] = head[0];
}
bool vis[N];
inline void dfs(const int &u) {
    for (int i = head[u], v; i; i = e[i].nxt)
    if (!vis[v = e[i].to]) {
        vis[v] = true;
        f[v][0] = f[u][0] + (a[v] == -1);
        f[v][1] = f[u][1];
        f[v][2] = f[u][2];
        if (a[u] == 1 && a[v] == 0)
            ++f[v][1];
        if (a[u] == 0 && a[v] == 1)
            ++f[v][2];
        dfs(v);
    }
}
inline void in(int &x) {
    char ch = getchar();
    while (1) {
        switch(ch) {
            case '1': x = 1; return;
            case '0': x = 0; return;
            case '?': x = -1; return;
            default: ch = getchar();
        }
    }
}
int main() {
    int t, n, u, v;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &u, &v);
            addedg(u, v);
            addedg(v, u);
        }
        for (int i = 1; i <= n; i++)
            in(a[i]);
        memset(vis, 0, n+1);
        f[1][1] = f[1][2] = 0;
        f[1][0] = a[1] == -1;
        vis[1] = true;
        dfs(1);
        for (int i = 1; i <= n; i++)
            printf("[%d,%d,%d]", f[i][0], f[i][1], f[i][2]);
        puts("ED");
    }
    return 0;
}