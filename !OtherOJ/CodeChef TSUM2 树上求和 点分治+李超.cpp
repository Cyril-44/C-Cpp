#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
constexpr int N = 50005;
std::vector<int> g[N];
int w[N];
bool ban[N];
int subsz[N], sz[N], cursz;
int getCentroid(int u, int fa) {
    sz[u] = 1;
    subsz[u] = cursz;
    int centroid = u;
    for (int v : g[u]) if (!ban[v] && v != fa) {
        int s = getCentroid(v, u);
        if (subsz[s] < subsz[centroid]) centroid = s;
        sz[u] += sz[v];
        subsz[u] = std::max(subsz[u], sz[v]);
    }
    subsz[u] = std::max(subsz[u], cursz - subsz[u]);
    if (subsz[u] < subsz[centroid]) return u;
    return centroid;
}
struct SegSegTr {
    constexpr static int MXN = 50000;
    int tot;
    struct Line {
        int k, b;
        inline int operator()(int x) const { return k * x + b; }
    } tr[MXN+1 << 2];
    inline void init(int n) {
        std::fill(tr+1, tr+1+(n<<2), Line{0, (int)-1e9});
        tot = n;
    }
    inline void pull(int u, int l, int r, Line s) {
        while (l != r) {
            int mid = l + r >> 1;
            if (tr[u](mid) < s(mid)) std::swap(s, tr[u]);
            if (tr[u](l) < s(l)) u = u<<1, r = mid;
            else if (tr[u](r) < s(r)) u=u<<1|1, l = mid+1;
            else return;
        }
        if (tr[u](l) < s(l)) tr[u] = s;
    }
    inline void insert(int k, int b) { pull(1, 1, tot, Line{k, b}); }
    inline int inquireMxval(int pos) {
        int mx = tr[1](pos), u = 1, l = 1, r = tot;
        while (l != r) {
            int mid = l + r >> 1;
            if (pos <= mid) u = u<<1, r = mid;
            else u = u<<1|1, l = mid+1;
            mx = std::max(mx, tr[u](pos));
        }
        return mx;
    }
} f;
int ans;
void dfsIns(int u, int fa, int dep, int sum, int val) { // 插入，{v} 的顺序是 ... -> fa -> u -> child -> ...
    ++dep; sum += w[u], val += dep * w[u];
    f.insert(sum, val); // 插入 sum * x + val
    for (int v : g[u]) if (!ban[v] && v != fa)
        dfsIns(v, u, dep, sum, val);
    std::reverse(g[u].begin(), g[u].end()); // 正着做一遍 反着做一遍的 concrete impl
}
void dfsUpd(int u, int fa, int dep, int sum, int val) { // 统计贡献，{v} 的顺序是 ... -> child -> u -> fa -> ...
    ++dep; sum += w[u]; val += sum;
    ans = std::max(ans, val + f.inquireMxval(dep));
    for (int v : g[u]) if (!ban[v] && v != fa)
        dfsUpd(v, u, dep + 1, sum + w[u], val + sum + w[u]);
}
void solve(int u, int fa) {
    getCentroid(u = getCentroid(u, fa), fa); // 确保是以 u 为根
    f.init(sz[u]);
    for (int v : g[u]) if (!ban[v]) {
        dfsUpd(v, u, 1, w[u], w[u]);
        dfsIns(v, u, 0, 0, 0);
    }
    ans = std::max(ans, w[u]);
    ans = std::max(ans, w[u] + f.inquireMxval(1));
    std::reverse(g[u].begin(), g[u].end());
    f.init(sz[u]);
    for (int v : g[u]) {
        dfsUpd(v, u, 1, w[u], w[u]);
        dfsIns(v, u, 0, 0, 0);
    }
    std::reverse(g[u].begin(), g[u].end());
    ban[u] = true;
    for (int v : g[u]) if (!ban[v]) solve(v, u);
}
inline void solveSingle() {
    int n;
    scanf("%d", &n);
    memset(ban+1, 0, sizeof(bool) * n);
    for (int i = 1; i <= n; i++) {
        g[i].clear();
        scanf("%d", &w[i]);
    }
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    ans = -1e9;
    solve(1, 0);
    printf("%d\n", ans);
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) solveSingle();
    return 0;
}