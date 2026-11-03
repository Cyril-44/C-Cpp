#include <bits/stdc++.h>
constexpr int N = 1000005;
#ifdef CLANGD
#define N 15
#endif
int a[N], b[N], p[N], h[N], lo[N];
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return -1;
        }
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '-') ch = gc();
        bool flg=false;
        if (ch == '-') flg=true, ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
        if (flg) x = -x;
    }
} using FastI::in;
int n;
struct BIT {
    struct BITBase {
        int64_t tr[N];
        void upd(int p, int64_t x) { for (; p <= n; p += p & -p) tr[p] += x; }
        int64_t inq(int p) { int64_t res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
    } fi, fs;
    int64_t tr[N];
    std::priority_queue<std::pair<int,int64_t>> pq;
    void update(int p, int64_t x, bool del = 0) {
        if (!del) pq.emplace(p, x);
        fi.upd(p, 1ll * p * x), fs.upd(p, x);
    }
    int64_t inquireK(int p) { return fs.inq(p); }
    int64_t inquireS(int p) { return (p+1) * fs.inq(p) - fi.inq(p); }
    void fix(int i) {
        while (!pq.empty()) {
            auto [p, x] = pq.top();
            if (p <= i) break;
            pq.pop();
            update(p, -x, true);
        }
        update(i+1, -inquireK(i));
    }
} f;
int main() {
    freopen("carefree.in", "r", stdin);
    freopen("carefree.out", "w", stdout);
    in(n);
    for (int i = 1; i <= n; i++) in(a[i]), in(b[i]);
    for (int i = 1; i <= n; i++) in(p[i]);
    int idx = 1, cur = 0, required = 0;
    for (int i = 1; i <= n; i++) {
        cur = std::max(cur, p[i] != -1 ? p[i] : i);
        while (idx <= cur) h[idx++] = i;
        if (p[i] != -1) required = std::max(required, p[i]);
    }
    idx = 1;
    int lastTime = 0;
    for (int i = 1; i <= n; i++) {
        if (p[i] == -1) continue;
        while (idx <= p[i]) lo[idx++] = lastTime;
        lastTime = i;
    }
    while (idx <= n) lo[idx++] = lastTime;
    int64_t y0 = 0;
    int64_t ans = required == 0 ? 0 : std::numeric_limits<int64_t>::min();
    for (int i = 1; i <= n; i++) {
        y0 += a[i];
        f.update(1, b[i]);
        int l = lo[i] + 1, r = h[i] - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (f.inquireK(mid) >= 0) l = mid + 1;
            else r = mid - 1;
        }
        r = std::max(lo[i], r);
        f.fix(r);
        // fprintf(stderr, "%d (y=%d, k=%d, lmt=%d): y0=%lld, r=%d, sum=%lld, ", i, a[i], b[i], h[i], y0, r, f.inquireS(r));
        // for (int i = 1; i <= n; i++) fprintf(stderr, "%d%c", f.inquireK(i), " |"[i==n]);
        // for (int i = 1; i <= n; i++) fprintf(stderr, "%lld%c", f.inquireS(i), " \n"[i==n]);
        if (i >= required) ans = std::max(ans, y0 + f.inquireS(r));
    }
    printf("%lld\n", static_cast<long long>(ans));
}