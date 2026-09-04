#include <cstdio>
#include <cstdint>
#include <algorithm>
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
constexpr int N = 4004, LIM = 1000, MOD = 999999999;
int a[N][N];
int64_t asum[N][N];
inline int operate(int op, int &x, int y) {
    int bak = x;
    switch (op) {
        case 1: x &= y; break;
        case 2: x |= y; break;
        case 3: x ^= y; break;
        case 4: x += y;
            if (x >= MOD) {
                x -= MOD;
                if (x >= MOD) x -= MOD;
            }
    }
    return x - bak;
}
struct Modify {
    int64_t d[N];
    int x;
    bool type;
    int64_t operator()(int p, int q) {
        if (type) // Col
            { if (x <= q) return d[p]; }
        else // Ln
            { if (x <= p) return d[q]; }
        return 0;
    }
} mods[LIM];
#pragma GCC optimize("unroll-loops")
int main() {
    int n, m; in(n), in(m);
    int cnt = 0;
    while (m--) {
        int op; in(op);
        if (op == 1) {
            int dir, i, w; in(dir), in(i), in(op), in(w);
            mods[cnt].x = i, mods[cnt].type = dir;
            for (int j = 1; j <= n; j++) {
                mods[cnt].d[j] = mods[cnt].d[j-1] + (dir ? 
                    operate(op, a[j][i], w) : // Col
                    operate(op, a[i][j], w)); // Ln
            }
            if (++cnt == LIM) {
                cnt = 0;
                for (int i = 1; i <= n; i++)
                    for (int j = 1; j <= n; j++)
                        asum[i][j] = asum[i-1][j] + asum[i][j-1] - asum[i-1][j-1] + a[i][j];
            }
        } else {
            int p, q; in(p), in(q);
            int64_t ans = asum[p][q];
            for (int k = 0; k < cnt; k++)
                ans += mods[k](p, q);
            out(ans), out.pc('\n');
        }
    }
    return 0;
}
