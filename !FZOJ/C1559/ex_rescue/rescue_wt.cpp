#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
const int N = 510;
const int M = N * N;
int n, m, k, a[M];
int lis[M], cnt;
vector<int> g[M];
int fa[M], sz[M];
void init(int n) {
    for (int i = 0; i < n; i++) {
        fa[i] = i;
        sz[i] = 1;
    }
}
int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}
bool merge(int u, int v) {
    u = find(u);
    v = find(v);
    if (u == v) return 0;
    g[u].push_back(v);
    fa[v] = u;
    sz[u] += sz[v];
    return 1;
}
int fahi[M], son[M];
ll sum[M], mxv[M];
void dfs(int u) {
    mxv[u] = sum[u] = (fahi[u] - a[u]) * (ll)sz[u];
    for (int v : g[u]) {
        // printf("(%d,%d)->(%d,%d)\n",u/m,u%m,v/m,v%m);
        fahi[v] = a[u];
        dfs(v);
        sum[u] += sum[v];
        if (son[u] == -1)
            son[u] = v;
        else if (mxv[son[u]] < mxv[v])
            son[u] = v;
    }
    if (son[u] != -1) mxv[u] += mxv[son[u]];
    // printf("(%d,%d) %lld %lld\n",u/m,u%m,sum[u],mxv[u]);
}
priority_queue<pii> q;
ll ans;
void modify(int u) {
    if (son[u] != -1) modify(son[u]);
    for (int v : g[u]) {
        if (v == son[u]) continue;
        q.push({mxv[v], v});
    }
}
int main() {
    memset(fa, -1, sizeof(fa));
    memset(son, -1, sizeof(son));
    scanf("%d%d%d", &n, &m, &k);
    init(n * m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &a[cnt]);
            lis[cnt] = cnt;
            cnt++;
        }
    }
    sort(lis, lis + cnt, [&](int x, int y) { return a[x] < a[y]; });
    for (int i = 0; i < cnt; i++) {
        if (lis[i] % m != 0) {
            if (a[lis[i] - 1] <= a[lis[i]]) merge(lis[i], lis[i] - 1);
        }
        if (lis[i] % m != m - 1) {
            if (a[lis[i] + 1] <= a[lis[i]]) merge(lis[i], lis[i] + 1);
        }
        if (lis[i] - m >= 0) {
            if (a[lis[i] - m] <= a[lis[i]]) merge(lis[i], lis[i] - m);
        }
        if (lis[i] + m < cnt) {
            if (a[lis[i] + m] <= a[lis[i]]) merge(lis[i], lis[i] + m);
        }
    }
    int rt = find(0);
    fahi[rt] = a[rt];
    dfs(rt);
    q.push({mxv[rt], rt});
    ans = sum[rt];
    // printf("%lld\n",ans);
    ll res = 0;
    for (int i = 1; i <= k; i++) {
        if (q.empty()) break;
        int u = q.top().second;
        // printf("%d %lld\n",u,mxv[u]);
        ans -= mxv[u];
        q.pop();
        modify(u);
        res ^= ans;
        // printf("%lld\n",ans);
    }
    printf("%lld", res);
    return 0;
}