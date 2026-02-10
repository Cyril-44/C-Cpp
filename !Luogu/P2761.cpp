#include <bits/stdc++.h>
constexpr int N = 20, M = 105;
char b[N+5], f[N+5];
struct Hotfix {
    int t, b1, b2, f1, f2;
} a[M];
bool vis[1 << N];
int dis[1 << N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d %s %s", &a[i].t, b, f);
        for (int j = 0; j < n; j++) {
            if (b[j] == '+') a[i].b1 |= 1 << j;
            else if (b[j] == '-') a[i].b2 |= 1 << j;
            if (f[j] == '-') a[i].f1 |= 1 << j;
            else if (f[j] == '+') a[i].f2 |= 1 << j;
        }
    }
    memset(dis, 0x3f, sizeof dis);
    // 一个状态 1/0 表示有没有 bug
    std::queue<int> q;
    q.push((1 << n) - 1);
    dis[q.front()] = 0;
    while (!q.empty()) {
        int s = q.front(); q.pop();
        vis[s] = false;
        for (int i = 1; i <= m; i++) {
            // fprintf(stderr, "%X %X\n", (s & a[i].b1) ^ a[i].b1, (s & a[i].b2));
            if ((s & a[i].b1) == a[i].b1 && !(s & a[i].b2)) {
                int ns = s & ~a[i].f1 | a[i].f2;
                if (dis[ns] > dis[s] + a[i].t) {
                    dis[ns] = dis[s] + a[i].t;
                    // fprintf(stderr, "Updated status %X from %X: %d=%d+%d\n", ns, s, dis[ns], dis[s], a[i].t);
                    if (!vis[ns]) vis[ns] = true, q.push(ns);
                }
            }
        }
    }
    printf("%d\n", dis[0] != 0x3f3f3f3f ? dis[0] : 0);
    return 0;
}
