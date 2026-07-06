#include <cstdio>
#include <map>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>
#include <random>
#include <iostream>
#include <memory>
constexpr int N = 500005;
using Pii = std::pair<int,int>;
#define L first
#define R second
std::mt19937 rng(std::random_device{}());
struct Node {
    uint64_t hash;
    int ls, rs;
    bool f;
} tr[N * 50];
uint64_t hs[N];
int64_t d[N];
int a[N], b[N], c[N], rt[N], tot, P; bool X;
inline void clone(int &u) {
    tr[++tot] = tr[u];
    u = tot;
}
inline void pushup(int u) {
    tr[u].hash = tr[tr[u].ls].hash ^ tr[tr[u].rs].hash;
}
void upd(int& u, int l, int r) {
    clone(u);
    if (l == r) {
        tr[u].f = X;
        tr[u].hash = X * hs[l];
        return;
    }
    int mid = l + r >> 1;
    if (P <= mid) upd(tr[u].ls, l, mid);
    else upd(tr[u].rs, mid+1, r);
    pushup(u);
}
int m;
inline void update(int i, int pos, bool v) {
    P = pos, X = v;
    upd(rt[i], 1, m);
}
bool comp(int u, int v, int l, int r) {
    if (l == r) return tr[u].f < tr[v].f;
    int mid = l + r >> 1;
    if (tr[tr[u].ls].hash == tr[tr[v].ls].hash) return comp(tr[u].rs, tr[v].rs, mid+1, r);
    return comp(tr[u].ls, tr[v].ls, l, mid);
}
Pii op[N];
std::vector<std::pair<int, bool>> ops[N];
int main() {
    freopen("exhibition.in", "r", stdin);
    freopen("exhibition.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::shared_ptr<
    int n;
    std::cin >> n >> m;
    for (int i = 1; i <= n; i++) std::cin >> a[i];
    for (int l, r, i = 1; i <= m; i++) {
        hs[i] = rng();
        std::cin >> l >> r;
        op[i] = Pii(l, r);
        ops[l].emplace_back(i, true);
        ops[r+1].emplace_back(i, false);
    }
    for (int i = 1; i <= n; i++) {
        rt[i] = rt[i-1];
        for (auto [x, w] : ops[i]) update(i, x, w);
    }
    std::sort(a+1, a+1+n);
    std::iota(b+1, b+1+n, 1);
    std::stable_sort(b+1,b+1+n, [](int x, int y){ return comp(rt[x], rt[y], 1, m); });
    for (int i = 1; i <= n; i++) c[b[i]] =  a[i];
    for (int i = 1; i <= n; i++) d[i] = d[i-1] + c[i];
    for (int i = 1; i <= m; i++) std::cout << d[op[i].R] - d[op[i].L-1] << '\n';
    return 0;
}