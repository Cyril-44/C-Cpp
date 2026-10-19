#include <bits/stdc++.h>
constexpr int N = 100005;
#ifdef CLANGD
#define N 15
#endif
int n;
std::vector<int> g[N];
class SegTr {
    struct Node {
        int cnt, ls, rs;
    } tr[N * 40];
    int rt[N], top, P, X;
    void upd(int& u, int l, int r) {
        tr[++top] = tr[u]; u = top;
        if (l == r) return void(tr[u].cnt += X);
        int mid = l + r >> 1;
        P <= mid ? upd(tr[u].ls, l, mid) : upd(tr[u].rs, mid+1, r);
        tr[u].cnt = tr[tr[u].ls].cnt + tr[tr[u].rs].cnt;
    }
    int mer(int u1, int u2, int l, int r) {
        if (!u1 || !u2) return u1 | u2;
        int u = ++top; tr[u] = {};
        tr[u].cnt = tr[u1].cnt + tr[u2].cnt;
        if (l != r) {
            int mid = l + r >> 1;
            tr[u].ls = mer(tr[u1].ls, tr[u2].ls, l, mid);
            tr[u].rs = mer(tr[u1].rs, tr[u2].rs, mid+1, r);
        }
        return u;
    }
public:
    void clear() { top = 0; memset(rt, 0, sizeof rt); }
    void inherit(int u, int fr) { rt[u] = rt[fr]; }
    void update(int u, int p, int x) { P=p, X=x; upd(rt[u], 0, n); }
    void merge(int u, int v) { rt[u] = mer(rt[u], rt[v], 0, n); }
    std::pair<int,int> lower_bound(int u, int v, int x) {
        u = rt[u], v = rt[v];
        int l = 0, r = n;
        while (l < r) {
            int mid = l + r >> 1, val = tr[tr[u].ls].cnt - tr[tr[v].ls].cnt;
            if (x <= val) r = mid, u = tr[u].ls, v = tr[v].ls;
            else x -= val, l = mid + 1, u = tr[u].rs, v = tr[v].rs;
        }
        return {l, x};
    }
} f;
std::vector<std::pair<int,int>> qry1[N];
std::vector<std::tuple<int,int,int>> qry2[N];
int sz[N], chain[N], pos[N];
uint64_t ans[N];
void dfs1(int u) {
    sz[u] = 1;
    for (int v : g[u]) dfs1(v), sz[u] += sz[v];
}
void dfs2(int u) {
    chain[++chain[0]] = u;
    pos[u] = chain[0];
    for (int v : g[u]) {
        f.inherit(v, u);
        f.update(v, u, sz[u] - sz[v]);
        // fprintf(stderr, "%d -> %d, f[%d] += %d\n", u, v, u, sz[u] - sz[v]);
        dfs2(v);
    }
    // fprintf(stderr, "f[%d] += %d, ", u, sz[u]);
    f.update(u, u, sz[u]);
    // for (int i = 1; i <= chain[0]; i++)
    //     fprintf(stderr, "%d ", chain[i]);
    // fprintf(stderr, "\n");
    for (auto [x, id] : qry1[u]) {
        auto [v, y] = f.lower_bound(u, 0, x);
        qry2[v].emplace_back(chain[pos[v] + 1], y, id);
    }
    chain[chain[0]--] = 0;
}
void dfs3(int u) {
    // fprintf(stderr, "%d\n", u);
    f.update(u, u, 1);
    for (int v : g[u]) dfs3(v), f.merge(u, v);
    for (auto [v, y, id] : qry2[u]) {
        // fprintf(stderr, "ans[%d] = (%d, %d, %d), f.lower_bound(%d,%d,%d)\n", id, (int)(ans[id] / n / n) + 1, u, f.lower_bound(u, v, y).first, u, v, y);
        ans[id] += (u - 1ull) * n + f.lower_bound(u, v, y).first - 1ull;
    }
}
int main() {
    int q; uint64_t kj;
    scanf("%d%d", &n, &q);
    for (int i = 1, pi; i <= n; i++) {
        scanf("%d", &pi);
        g[pi].push_back(i);
    }
    for (int j = 1; j <= q; j++) {
        scanf("%llu", &kj);
        int u = (kj - 1) / n + 1;
        ans[j] += (u - 1ull) * n * n;
        qry1[u].emplace_back(int(kj - (u - 1ull) * n), j);
    }
    int rt = g[0][0];
    dfs1(rt);
    dfs2(rt);
    f.clear(); dfs3(rt);
    for (int i = 1; i <= q; i++)
        printf("%llu\n", ans[i]);
}