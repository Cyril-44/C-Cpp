#include <bits/stdc++.h>
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    inline char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        if (p1 == p2) return EOF;
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
constexpr int N = 200005, INF = 2.01e9;
int a[N], b[N], n;
class SegTr {
    struct Mat {
        int m[2][2]{INF, -INF, -INF, INF};
        friend Mat operator*(const Mat& a, const Mat& b) {
            Mat c{-INF, -INF, -INF, -INF};
            for (int i = 0; i ^ 2; i++)
                for (int j = 0; j ^ 2; j++)
                    for (int k = 0; k ^ 2; k++)
                        c.m[i][j] = std::max(c.m[i][j], std::min(a.m[i][k], b.m[k][j]));
            return c;
        }
    } tr[N << 2];
    int P;
    void upd(int u, int l, int r) {
        if (l == r) { tr[u].m[0][1] = a[P] + a[P-1]; return; }
        int mid = l + r >> 1;
        P <= mid ? upd(u<<1, l, mid) : upd(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1|1] * tr[u<<1];
    }
public:
    void build(int u=1, int l=2, int r=n) {
        if (l == r) { tr[u] = {a[l], -INF, INF, -INF}; return; }
        int mid = l + r >> 1;
        build(u<<1, l, mid); build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1|1] * tr[u<<1];
    }
    void update(int p) { P=p; upd(1, 2, n); }
    int inquire() { return (tr[1] * Mat{a[1], -INF, INF, -INF}).m[0][0]; }
} f;
int main() {
    int T; in(T);
    while (T--) {
        in(n);
        for (int i = 1; i <= n; i++) in(a[i]);
        if (n == 1) {
            puts("0");
            continue;
        }
        int cur = *std::max_element(a+1, a+1+n);
        std::iota(b+1, b+n, 2);
        std::sort(b+1, b+n, [](int x, int y) { return a[x] + a[x-1] < a[y] + a[y-1]; });
        f.build();
        int idx = 1;
        while (idx < n && a[b[idx]] + a[b[idx]-1] <= cur) f.update(b[idx++]);
        int ans = cur - f.inquire();
        for (; idx < n; idx++) {
            f.update(b[idx]);
            cur = a[b[idx]] + a[b[idx]-1];
            ans = std::min(ans, cur - f.inquire());
        }
        printf("%d\n", ans);
    }
}