#include <stdio.h>
#include <numeric>
#include <array>
constexpr int N = 55, M = 100005;
int n, T;
int edg[M][2];
using Node = std::array<int, N>;
namespace SegTr {
Node tr[M << 2];
inline void pushup(int u) {
    auto &us = tr[u];
    const auto& ls = tr[u<<1], rs = tr[u<<1|1];
    for (int i = 1; i <= T; i++)
        us[i] = (rs[ls[i]]) ? rs[ls[i]] : ls[i];
}
void build(int u = 1, int l = 1, int r = n) {
    if (l == r) {
        int x;
        scanf("%d", &x);
        std::iota(tr[u].begin(), tr[u].end(), 0);
        tr[u][edg[x][0]] = edg[x][1];
        return;
    }
    int mid = l + r >> 1;
    build(u << 1, l, mid);
    build(u << 1 | 1, mid + 1, r);
    pushup(u);
}
int P, X;
void upd(int u = 1, int l = 1, int r = n) {
    if (l == r) {
        std::iota(tr[u].begin(), tr[u].end(), 0);
        tr[u][edg[X][0]] = edg[X][1];
        return;
    }
    int mid = l + r >> 1;
    if (P <= mid) upd(u << 1, l, mid);
    else upd(u << 1 | 1, mid + 1, r);
    pushup(u);
}
int L, R;
Node que(int u = 1, int l = 1, int r = n) {
    if (L <= l && r <= R) return tr[u];
    int mid = l + r >> 1;
    if (L <= mid && mid < R) {
        auto left = que(u << 1, l, mid);
        auto right = que(u << 1 | 1, mid + 1, r);
        for (int i = 1; i <= T; i++)
            if (right[left[i]]) left[i] = right[left[i]];
        return left;
    }
    if (L <= mid) return que(u << 1, l, mid);
    return que(u << 1 | 1, mid + 1, r);
}
inline void update (int p, int x) { P = p, X = x; upd(); }
inline Node query (int l, int r) { L = l, R = r; return que(); }
}
int main() {
    int m, q;
    scanf("%d%d", &T, &m);
    for (int x, y, i = 1; i <= m; i++) {
        scanf("%d%d", &edg[i][0], &edg[i][1]);
    }
    scanf("%d", &n);
    SegTr::build();
    scanf("%d", &q);
    for (int op, x, y, z; q--; ) {
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1) scanf("%d", &z);
        if (op == 1) {
            Node get = SegTr::query(x, y);
            printf("%d\n", get[z]);
        }
        else SegTr::update(x, y);
    }
    return 0;
}