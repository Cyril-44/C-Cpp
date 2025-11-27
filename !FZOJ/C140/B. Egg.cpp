#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-6;
int n, m, vis[1005];
bool flag[1005];
double g[1005][1005], dis[1005];
queue<int> q;
bool check(double mid) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            g[i][j] -= mid;
        }
    }
    memset(dis, 0x4f, sizeof(dis));
    memset(flag, 0, sizeof(flag));
    memset(vis, 0, sizeof(vis));
    q.push(1);
    dis[1] = 0;
    flag[1] = vis[1] = 1;
    bool ok = 0;
    while (!ok && !q.empty()) {
        int x = q.front();
        q.pop();
        flag[x] = 0;
        for (int y = 1; y <= n; y++) {
            if (dis[x] + g[x][y] < dis[y]) {
                dis[y] = dis[x] + g[x][y];
                if (!flag[y]) {
                    flag[y] = 1;
                    vis[y]++;
                    q.push(y);
                    if (vis[y] > n) {
                        ok = 1;
                        break;
                    }
                }
            }
        }
    }
    while (!q.empty())
        q.pop();
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            g[i][j] += mid;
        }
    }
    return ok;
}
int main() {
    memset(g, 0x4f, sizeof(g));
    cin >> n >> m;
    double l = 0, r = 10000001, mid;
    for (int i = 1; i <= m; i++) {
        int x, y, w;
        scanf("%d%d%d", &x, &y, &w);
        g[x][y] = w;
    }
    while (l + eps <= r) {
        mid = (l + r) / 2;
        if (check(mid)) {
            r = mid;
        } else {
            l = mid;
        }
    }
    if (l > 10000000)
        printf("PaPaFish is laying egg!");
    else
        printf("%.2lf", l);
    return 0;
}
