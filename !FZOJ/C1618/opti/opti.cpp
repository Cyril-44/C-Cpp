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
inline int calc(const int hi) {
    f[0] = INF;
    f[1] = a[1] > hi ? 0 : a[1];
    for (int i = 2; i <= n; i++) {
        if (a[i] > hi) f[i] = 0;
        else {
            f[i] = std::min(a[i], f[i-1]);
            if (a[i] + a[i-1] <= hi)
                f[i] = std::max(f[i], std::min(f[i-2], a[i] + a[i-1]));
        }
    }
    return hi - f[n];
}
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
    void build(int u=1, int l=1, int r=n) {
        if (l == r) { tr[u] = {a[l], -INF, -INF, INF}; return; }
        int mid = l + r >> 1;
        build(u<<1, l, mid); build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1|1] * tr[u<<1];
    }
    void update(int p) { P=p; upd(1, 2, n); }
    int inquire() { return (tr[1] * Mat{a[1], -INF, INF, -INF}).m[0][0]; }
};
int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> rngk(-1, 1), rngp(0, 1);
    int T; in(T);
    while (T--) {
        in(n);
        for (int i = 1; i <= n; i++) in(a[i]);
        int start = *std::max_element(a+1, a+1+n);
        std::iota(b+1, b+n, 2);
        std::sort(b+1, b+1+n, [](int x, int y) { return a[x] + a[x-1] < a[y] + a[y-1]; });
        
        for (int pi = 1; pi < n; pi++) {
            
        }
    }
}