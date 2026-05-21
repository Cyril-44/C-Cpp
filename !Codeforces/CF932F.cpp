#include <cstdio>
#include <cstring>
#include <vector>
#include <cstdint>
constexpr int N = 100005;
constexpr int64_t INF = 1e18;
int a[N], b[N];
std::vector<int> g[N];
namespace DYNSeg {
constexpr int MX = 1e5;
struct Line {
    int64_t k, b;
    inline int64_t operator()(int x) { return k * x + b; }
};
struct Node {
    Line dat;
    int ls, rs;
    inline int64_t operator()(int x) { return dat(x); }
    inline Line& operator()() { return dat; }
} tr[N*8]{{{0, INF},0,0}};
int alltop = 0;
inline int alloc(const Line &x) { return tr[++alltop]() = x, alltop; }
inline int alloc(int64_t k = 0, int64_t b = INF) { return alloc({k, b}); }
void update(int &u, int l, int r, Line s) {
    if (!u) u = alloc();
    if (l == r) {
        if (tr[u](l) > s(l)) tr[u]() = s;
        return;
    }
    int mid = l + r >> 1;
    if (tr[u](mid) > s(mid)) std::swap(tr[u](), s);
    if (tr[u](l) > s(l)) update(tr[u].ls, l, mid, s);
    else if (tr[u](r) > s(r)) update(tr[u].rs, mid+1, r, s);
}
int merge(int u, int v, int l, int r) {
    if (!u || !v) return u | v;
    int mid = l + r >> 1;
    if (tr[u](mid) > tr[v](mid)) std::swap(u, v);
    update(u, l, r, tr[v]());
    tr[u].ls = merge(tr[u].ls, tr[v].ls, l, mid);
    tr[u].rs = merge(tr[u].rs, tr[v].rs, mid+1, r);
    return u;
}
struct SegSegTr {
    int root;
    inline SegSegTr() : root() {}
    inline void insert(int64_t k, int64_t b) { update(root, -MX, MX, {k, b}); }
    inline void join(SegSegTr &sg) { root = merge(root, sg.root, -MX, MX); sg.root = 0; }
    inline int64_t inquire(int pos) {
        int u = root, l = -MX, r = MX;
        int64_t ans = INF;
        while (l != r) {
            if (!u) return ans;
            ans = std::min(ans, tr[u](pos));
            int mid = l + r >> 1;
            if (pos <= mid) u = tr[u].ls, r = mid;
            else u = tr[u].rs, l = mid+1;
        }
        ans = std::min(ans, tr[u](pos));
        return ans;
    }
} f[N];
} using DYNSeg::f;
int64_t fans[N];
void dfs(int u, int fa) {
    if (fa && g[u].size() == 1) {
        fans[u] = 0;
        f[u].insert(b[u], 0);
        return;
    }
    int64_t fu = INF;
    for (int v : g[u]) if (v != fa) {
        dfs(v, u);
        fu = std::min(fu, f[v].inquire(a[u]));
        f[u].join(f[v]);
    }
    f[u].insert(b[u], fu);
    fans[u] = fu;
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &b[i]);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; i++)
        printf("%lld ", fans[i]);
    putchar('\n');
    return 0;
}