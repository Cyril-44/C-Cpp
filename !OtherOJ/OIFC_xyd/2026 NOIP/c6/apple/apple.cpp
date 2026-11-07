#include <bits/stdc++.h>
constexpr int N = 1000005;
int a[N * 2];
int64_t f[N];
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    inline char gc() {
        if (p1 == p2) {
            p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
            if (p1 == p2) return EOF;
        }
        return *p1++;
    }
    template<class T> inline void in(T &x) {
        char ch = gc();
        while (ch < '0') ch = gc();
        for (x = 0; ch >= '0'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
struct FastO {
    char buf[1 << 20], *p1=buf;
    const char *p2 = buf + sizeof(buf);
    inline void flush() { fwrite(buf, p1-buf, 1, stdout), p1=buf; }
    inline void pc(char c) {
        if (p1 == p2) fwrite(buf, sizeof buf, 1, stdout), p1=buf;
        *p1++ = c;
    }
    template<class T> inline void operator()(T x, char end='\n') {
        int8_t sta[64];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) pc(sta[sta[0]--] | '0');
        if (end) pc(end);
    }
    inline void puts(const char *s) {
        for (int i = 0; s[i]; i++) pc(s[i]);
        pc('\n');
    }
    ~FastO() { flush(); }
} out;
int main() {
    freopen("apple.in", "r", stdin);
    freopen("apple.out", "w", stdout);
    int n, q, mxai = 0;
    in(n), in(q);
    int64_t sum = 0;
    for (int i = 1, ai; i <= n; i++) {
        in(ai);
        a[ai]++;
        sum += ai, mxai = std::max(mxai, ai);
        /* for (int l = 1, r, pre = 0; l <= ai; l = r + 1) {
            int now = ai / l;
            s[l] += now - pre;
            r = ai / now;
            pre = now;
        }
        s[ai+1] += -1; */
    }
    for (int i = 1; i <= mxai + mxai; i++) a[i] += a[i-1];
    for (int i = 1; i <= mxai; i++) {
        int64_t s = 0;
        for (int j = i, k = 1; j <= mxai; j += i, k++)
            s += 1ll * k * (a[j+i-1] - a[j-1]);
        f[i] = sum - s * (i-1);
    }
    std::sort(f+1, f+1+mxai);
    for (int i = 1; i <= q; i++) {
        int64_t bi; in(bi);
        int idx = std::lower_bound(f+1, f+1+mxai, bi) - f - 1;
        if (idx) out(f[idx]);
        else out.puts("No solotion");
    }
}