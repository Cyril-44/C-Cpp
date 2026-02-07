#include <stdio.h>
#include <vector>
constexpr int N = 1 << 18;
int a[N];
std::vector<int> f[N << 2]; // 线段树上区间 [l,r] ^ x 后的答案
void build(int u, int l, int r, int len) {
    int mid = l + r >> 1;
    if (l == r - 1) {
        f[u] = {0};
        return;
    }
    build(u << 1, l, mid, len >> 1);
    build(u << 1 | 1, mid, r, len >> 1);
    f[u].resize(len);
    int mask = (len >> 1) - 1;
    for (int x = 0; x ^ len; ++x) {
        f[u][x] = f[u<<1][x & mask] + f[u<<1|1][x & mask] + (a[mid-1 ^ x] == a[mid ^ x]);
    }
}
int query(int u, int l, int r, int x, int L, int R, int k) {
    // fprintf(stderr, "u=%d [%d,%d) x=%d q[%d,%d) k%d\n", u, l, r, x, L, R, k);
    if (L <= l && r <= R) return f[u][x];
    int mid = l + r >> 1;
    if (x >> k & 1) {
        // fprintf(stderr, "Goto bit x = 1\n");
#define mask (1 << k)
        if (R <= mid) {
            // fprintf(stderr, "Detected [L,R] [%d,%d) in [l, mid] [%d,%d)!\n", L, R, l, mid);
            return query(u<<1|1, mid, r, x ^ mask, L + mask, R + mask, k - 1);
        }
        if (mid <= L) {
            // fprintf(stderr, "Detected [L,R] [%d,%d) in [mid, r] [%d,%d)!\n", L, R, mid, r);
            return query(u<<1, l, mid, x ^ mask, L - mask, R - mask, k - 1);
        }
        return query(u<<1|1, mid, r, x ^ mask, L + mask, r, k - 1)
             + query(u<<1, l, mid, x ^ mask, l, R - mask, k - 1)
             + (a[mid-1 ^ x] == a[mid ^ x]);
#undef mask
    }
    if (R <= mid) return query(u<<1, l, mid, x, L, R, k-1);
    if (mid <= L) return query(u<<1|1, mid, r, x, L, R, k-1);
    return query(u<<1, l, mid, x, L, mid, k-1)
         + query(u<<1|1, mid, r, x, mid, R, k-1)
         + (a[mid-1 ^ x] == a[mid ^ x]);
}
int main() {
    int T, k, m;
    scanf("%d%d%d", &T, &k, &m);
    int n = 1 << k;
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    build(1, 0, n, n);
    int x = 0, lastans = 0;
    for (int op, l, r; m--; ) {
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d", &l);
            l ^= T*lastans;
            x ^= l;
        } else {
            scanf("%d%d", &l, &r);
            l ^= T*lastans, r ^= T*lastans;
            if (l > r) std::swap(l, r);
            ++r;
            printf("%d\n", lastans = r - l - query(1, 0, n, x, l, r, k - 1));
            // fprintf(stderr, "End query!\n");
        }
    }
    return 0;
}