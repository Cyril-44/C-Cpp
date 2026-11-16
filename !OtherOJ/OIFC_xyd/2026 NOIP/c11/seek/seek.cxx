#include <bits/stdc++.h>
#define ll long long
using namespace std;
int tid, t, n, m, qu;
bool flag[185][185];
ll dis[185][185];
struct edge {
    int y;
    ll w;
};
vector<edge> ng1[185], g2[185];
struct node {
    int x, y;
    ll w;
    bool operator>(const node &x) const { return w > x.w; }
};
priority_queue<node, vector<node>, greater<node>> q;
void dij() {
    memset(dis, 0x3f, sizeof(dis));
    memset(flag, 0, sizeof(flag));
    for (int i = 1; i <= n; i++) {
        q.push({i, i, 0});
        dis[i][i] = 0;
    }
    while (!q.empty()) {
        node d = q.top();
        q.pop();
        if (d.w > dis[d.x][d.y]) continue;
        flag[d.x][d.y] = 1;
        for (int i = 1; i <= n; i++) {
            if (flag[d.y][i] && d.w + dis[d.y][i] < dis[d.x][i]) {
                dis[d.x][i] = d.w + dis[d.y][i];
                q.push({d.x, i, dis[d.x][i]});
            }
            if (flag[i][d.x] && d.w + dis[i][d.x] < dis[i][d.y]) {
                dis[i][d.y] = d.w + dis[i][d.x];
                q.push({i, d.y, dis[i][d.y]});
            }
        }
        for (edge x : ng1[d.x]) {
            for (edge y : g2[d.y]) {
                if (d.w + x.w + y.w < dis[x.y][y.y]) {
                    dis[x.y][y.y] = d.w + x.w + y.w;
                    q.push({x.y, y.y, d.w + x.w + y.w});
                }
            }
        }
    }
}
int main() {
    freopen("seek.in", "r", stdin);
    freopen("seek.out", "w", stdout);
    cin >> t >> tid;
    while (t--) {
        scanf("%d%d%d", &n, &m, &qu);
        for (int i = 1; i <= n; i++) { ng1[i].clear(), g2[i].clear(); }
        for (int i = 1; i <= m; i++) {
            int x, y, w, tp;
            scanf("%d%d%d%d", &x, &y, &w, &tp);
            if (tp == 1)
                ng1[y].push_back({x, w});
            else
                g2[x].push_back({y, w});
        }
        dij();
        while (qu--) {
            int x, y;
            scanf("%d%d", &x, &y);
            if (dis[x][y] > 1e16)
                puts("-1");
            else
                printf("%lld\n", dis[x][y]);
        }
    }
    return 0;
}