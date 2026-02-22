#include <bits/stdc++.h>
using namespace std;

long long a[505][505];
int f[250005];
vector<int> p[250005];
vector<int> g[1000005];
int fa[250005];
int siz[250005];
long long lf[505][505];
int n, m, k, root;
int d[250005];
long long rp;
int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};
int find(int x) {
    if (f[x] == x) return x;
    return f[x] = find(f[x]);
}
void merge(int u, int v) {
    int x = find(u), y = find(v);
    if (x != y) {
        p[y].push_back(x);
        fa[x] = y;
        f[x] = y;
        d[y]++;
    }
}
long long l[250005], lp[250005];
long long ans[250005];
long long gg[250005];
long long pp[250005];
void dfs1(int w) {
    siz[w] = 1;
    for (int i = 0; i < p[w].size(); i++) dfs1(p[w][i]), siz[w] += siz[p[w][i]];
}
void dfs2(int w) {
    if (w == root)
        l[w] = lf[(w - 1) / m + 1][(w - 1) % m + 1], lp[w] = 0;
    else
        l[w] = lf[(w - 1) / m + 1][(w - 1) % m + 1] + l[fa[w]] +
               lf[(fa[w] - 1) / m + 1][(fa[w] - 1) % m + 1] * (siz[fa[w]] - siz[w] - 1),
        lp[w] = l[w] - lf[(w - 1) / m + 1][(w - 1) % m + 1];
    pp[w] = l[w];
    gg[w] = w;
    for (int i = 0; i < p[w].size(); i++) {
        dfs2(p[w][i]);
        if (pp[gg[p[w][i]]] > pp[gg[w]]) gg[w] = gg[p[w][i]], pp[w] = pp[gg[p[w][i]]];
    }
}
long long wg;
bool vis[250005];
void init() {
    priority_queue<pair<long long, int>> q;
    q.push({pp[root], gg[root]});
    for (int i = 1; i <= k; i++) {
        if (q.empty()) break;
        int x = q.top().second;
        long long sp = q.top().first;
        wg -= q.top().first;
        q.pop();
        int rx = 0;
        for (int j = x; j != 0 && (vis[j] == 0 || j == x); j = fa[j]) {
            vis[j] = 1;
            if (d[j] == 0) continue;
            for (int z = 0; z < p[j].size(); z++) {
                if (p[j][z] == rx) continue;
                //			cout<<p[j][z]<<" "<<lp[p[j][z]]<<endl;
                q.push({pp[p[j][z]] - lp[p[j][z]] - lf[(j - 1) / m + 1][(j - 1) % m + 1] * siz[p[j][z]], gg[p[j][z]]});
            }
            rx = j;
        }
        ans[i] = wg;
    }
}
int main() {
    // freopen("ex_rescue3.in", "r", stdin);
    // freopen("rescue.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> a[i][j];
            rp = max(rp, a[i][j]);
            g[a[i][j]].push_back((i - 1) * m + j);
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            lf[i][j] = rp - a[i][j];
            wg += lf[i][j];
        }
    }
    for (int i = 1; i <= n * m; i++) f[i] = i;
    for (int i = 1; i <= 1000000; i++) {
        for (int j = 0; j < g[i].size(); j++) {
            int rt = g[i][j];
            int xp = (rt - 1) / m + 1;
            int yp = (rt - 1) % m + 1;
            for (int d = 0; d < 4; d++) {
                int tx = xp + dx[d];
                int ty = yp + dy[d];
                if (tx > n || ty > m || tx < 1 || ty < 1 || a[tx][ty] > a[xp][yp]) continue;
                merge((tx - 1) * m + ty, rt);
            }
        }
    }
    for (int i = 1000000; i >= 1; i--) {
        if (g[i].size() > 0) {
            for (int j = 1; j < g[i].size(); j++) { merge(g[i][j - 1], g[i][j]); }
            root = g[i][g[i].size() - 1];
            break;
        }
    }
    dfs1(root);
    dfs2(root);
    //	for(int i=1;i<=n*m;i++) cout<<siz[i]<<endl;
    init();
    long long s = 0;
    // for (int i = 1; i <= k; i++) cout << ans[i] << endl;
    	for(int i=1;i<=k;i++) s^=ans[i];
    	cout<<s<<endl;
    return 0;
}
