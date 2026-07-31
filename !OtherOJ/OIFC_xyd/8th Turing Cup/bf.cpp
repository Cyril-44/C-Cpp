#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 5005;
int a[N], p[N], end[N];
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    [[gnu::always_inline]] inline char gc() {
        if (p1 == p2) [[unlikely]] p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++; 
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
namespace FastO {
    char buf[1 << 20], *p1=buf;
    const char* p2 = buf + sizeof(buf);
    [[gnu::always_inline]] inline void pc(char x) {
        if (p1 == p2) [[unlikely]] fwrite(buf, sizeof buf, 1, stdout), p1 = buf;
        *p1++ = x;
    }
    struct Cleaner { ~Cleaner() { fwrite(buf, p1-buf, 1, stdout); } } _cleaner;
    inline void out(int x) {
        static int sta[15];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) pc(sta[sta[0]--] | '0');
        pc('\n');
    }
} using FastO::out;
int n;
struct BIT {
    int tr[N];
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
    int sum(int p) { int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
} f1, f2;
int f[N][N];
inline void solve() {
    in(n);
    for (int i = 1; i <= n; i++) in(a[i]);
    if (n <= 2) {
        printf("%d\n", n);
        return;
    }
    for (int i = 1; i <= n; i++)
        p[a[i]] = i;
    end[n-1] = n;
    for (int i = n - 2; i >= 1; i--) {
        if ((p[i] > p[i+1]) == (p[i+1] > p[i+2])) end[i] = end[i+1];
        else end[i] = i+1;
    }
    for (int i = 0; i <= n; i++) memset(f[i], 0x3f, sizeof(int) * (n+1));
    f[0][0] = 0;
    for (int i = 1; i <= n; i++) f1.upd(i, 1), f2.upd(i, 1);
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++)
            f[i][j+1] = std::min(f[i][j+1], f[i-1][j] + 1);
        if (i < n && end[i]-i+1 == (p[i] > p[i+1] ? f1.sum(p[i]) : f2.sum(n - p[i] + 1))) {
            // fprintf(stderr, "%d-->%d\n", i, end[i]);
            for (int j = 1; j < i; j++)
                f[end[i]][j-1] = std::min(f[end[i]][j-1], f[i-1][j] + 1);
        }
        f1.upd(p[i], -1);
        f2.upd(n - p[i] + 1, -1);
    }
    int ans = 0x3f3f3f3f;
    for (int i = 0; i <= n; i++)
        ans = std::min(ans, f[n][i]);
    out(ans);
}
int main() {
    int T = 1;
    in(T);
    while (T--) solve();
    return 0;
}