#include <bits/stdc++.h>
constexpr int N = 100004;
int t[N], ans[N];
struct Node {
    int t, p;
    bool operator<(const Node &o) const { return t > o.t; }
} s[N];
bool vis[N];
struct FastI {
    char buf[1 << 20], *p1{}, *p2{};
    [[gnu::always_inline]] char gc() {
        if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        if (p1 == p2) return EOF;
        return *p1++;
    }
    template<class T> void operator()(T &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} in;
struct FastO {
    char buf[1 << 20], *p1{buf};
    const char *p2{buf + sizeof(buf)};
    [[gnu::always_inline]] void pc(char ch) {
        if (p1 == p2) fwrite(buf, sizeof buf, 1, stdout), p1=buf;
        *p1++ = ch;
    }
    void flush() { fwrite(buf, p1-buf, 1, stdout), p1=buf; }
    template<class T> void operator()(T x) {
        static uint8_t sta[64];
        sta[sta[0] = 1] = x % 10 | '0';
        while (x /= 10) sta[++sta[0]] = x % 10 | '0';
        while (sta[0]) pc(sta[sta[0]--]);
    }
    ~FastO() { flush(); }
} out;
int main() {
    int T, n, m;
    in(T);
    while (T--) {
        in(n), in(m);
        for (int i = 1; i <= n; i++) in(s[i].t), s[i].p = i;
        for (int i = 1; i <= m; i++) in(t[i]);
        std::sort(s+1, s+1+n);
        memset(vis, 0, n + 2);
        int cur = 0;
        for (int i = 1, j = m; j >= 1; j--) {
            for (; i <= n && s[i].t > t[j]; i++) {
                vis[s[i].p] = true;
                cur += 1 - vis[s[i].p-1] - vis[s[i].p+1];
            }
            ans[j] = cur;
        }
        for (int j = 1; j <= m; j++)
            out(ans[j]), out.pc(' ');
        out.pc('\n');
    }
    return 0;
}