#include <stdio.h>
#include <algorithm>
const int N = 40005;
typedef long long LL;
int _uni[N << 2], n;
struct SegNode {
    int h;
} tr[N << 3];
struct Modify {
    int a, b, h;
    inline bool operator<(const Modify &x) const {
        return h < x.h;
    }
} m[N];
inline void pushdown(const int &u) {
    if (tr[u].h) {
        tr[u<<1].h = tr[u].h;
        tr[u<<1|1].h = tr[u].h;
        tr[u].h = 0;
    }
}
inline void upd(const int &L, const int &R, const int &h, int u=1, int l=1, int r=n<<1) {
    if (L <= l && r <= R) {
        tr[u].h = h;
        return;
    }
    int mid = l + r >> 1;
    pushdown(u);
    if (mid >= L) upd(L, R, h, u<<1, l, mid);
    if (mid < R) upd(L, R, h, u<<1|1, mid+1, r);
    // pushup(u);
}
inline LL query(const int &L, const int &R, int u=1, int l=1, int r=n<<1) {
    if (l > R || r < L)
    if (tr[u].h) return tr[u].h * (r - l + 1ll);
    int mid = l + r >> 1;
    LL res = 0;
    if (mid >= L) res += query(L, R, u<<1, l, mid);
    if (mid < R) res += query(L, R, u<<1|1, mid+1, r);
    return res;
}
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d", &m[i].a, &m[i].b, &m[i].h);
        _uni[i<<1] = m[i].a;
        _uni[i<<1|1] = m[i].b;
    }
    int *_end = _uni + (n<<1);
    std::sort(_uni, _end);
    _end = std::unique(_uni, _end);
    for (int i = 0; i < n; i++) {
        m[i].a = std::lower_bound(_uni, _end, m[i].a) - _uni + 1;
        m[i].b = std::lower_bound(_uni, _end, m[i].b) - _uni + 1;
    }
    std::sort(m, m + n);
    for (int i = 0; i < n; i++)
        upd(m[i].a, m[i].b, m[i].h);
    printf("%lld\n", query(1, n<<1));
    return 0;
}