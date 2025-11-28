#include <stdio.h>
#include <string.h>
#include <queue>
using std::max;
typedef std::pair<long long, int> PLI;
const int N = 400005;
int a[N], b[N];
struct Edge {
    int to, nxt, w;
} e[N << 1];
int head[N];
long long f[N], s[N];
bool vis[N];
inline void addedg(const int &fr, const int &to, const int &c) {
    e[++head[0]] = Edge{to, head[fr], c};
    head[fr] = head[0];
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(head, 0, n+2 << 2);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            if (i > 1) addedg(i, i-1, 0);
            s[i] = s[i-1] + a[i];
        }
        for (int j = 1; j <= n; j++) {
            scanf("%d", &b[j]);
            addedg(j, b[j], a[j]);
        }
        memset(f, 0x3f, n+2 << 3);
        memset(vis, 0, n+2);
        int u = 1;
        f[u] = 0;
        std::priority_queue<PLI> q;
        q.push(PLI(f[u], u));
        while (!q.empty()) {
            u = q.top().second, q.pop();
            if (vis[u]) continue;
            else vis[u] = true;
            for (int i = head[u], v; i; i = e[i].nxt) {
                if (!vis[v = e[i].to] && f[v] > f[u] + e[i].w) {
                    f[v] = f[u] + e[i].w;
                    q.push(PLI(-f[v], v));
                }
            }
        }
        long long ans = 0;
        for (int i = 1; i <= n; i++) {
            ans = max(ans, s[i] - f[i]);
        }
        printf("%lld\n", ans);
    }
    return 0;
}