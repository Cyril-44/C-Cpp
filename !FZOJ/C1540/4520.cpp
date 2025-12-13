#include <stdio.h>
#include <string.h>
#define umin(x, y) (((x) < (y)) ? (x) : (x) = (y))
constexpr int N = 100005, M = 1000005, MOD = 998244353, K = 17;
int a[N], n;
namespace SegTree {
struct Node {
    int mn, tag;
} tr[N << 2];
inline void pushup(int u) {
    umin(tr[u<<1].mn, tr[u<<1|1].mn);
}
inline void pushdown(int u) {
    if (tr[u].tag) {
        umin(tr[u<<1].mn, tr[u].tag);
        umin(tr[u<<1|1].mn, tr[u].tag);
        umin(tr[u<<1].tag, tr[u].tag);
        umin(tr[u<<1|1].tag, tr[u].tag);
        tr[u].tag = 0;
    }
}
void build(int u=1, int l=1, int r=n) {
    if (l == r) return tr[u].mn = a[l], void();
    int mid = l + r >> 1;
    build(u<<1, l, mid);
    build(u<<1|1, mid+1, r);
    pushup(u);    
}
int P, X;
void upd(int u, int l, int r) {}
}
int main() {
    int m, k;
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    initst(n);
    return 0;
}