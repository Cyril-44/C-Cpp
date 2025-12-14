#include <stdio.h>
#include <string.h>
const int N = 500005;
struct Edge {
    int to, nxt;
} e[N<<1];
int head[N], f[N];
inline void addedg(const int &fr, const int &to) {
    e[++head[0]] = {to, head[fr]};
    head[fr] = head[0];
}
bool vis[N];
inline void dfs(const int &u, const int& d) {
    bool flag = true;
    for (int i = head[u]; i; i = e[i].nxt)
        if (!vis[e[i].to]) {
            vis[e[i].to] = true;
            flag = false;
            dfs(e[i].to, d + 1);
        }
    if (flag) ++f[d];
}
inline void umin(int &x, const int &y) {
    x = x < y ? x : y;
}
int main() {
    int t, n, u, v, ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(head, 0, sizeof(int) * (n+1));
        memset(f, 0, sizeof(int) * n);
        memset(vis, 0, sizeof(bool) * (n+1));
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &u, &v);
            addedg(u, v);
            addedg(v, u);
        }
        vis[1] = true, dfs(1, 0);
        for (int i = 1; i < n; i++)
            f[i] += f[i-1] + i;
        ans = n;
        for (int i = 0; i < n; i++)
            if (f[i])
                umin(ans, n - i - f[i]);
        printf("%d\n", ans);
    }
    return 0;
}
