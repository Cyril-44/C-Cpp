#include <cstdio>
#include <algorithm>
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FastI() : buf{}, p1{}, p2{} {}
    inline char get() {
        if (__builtin_expect(p1 == p2, 0)) *buf = '\0', p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void operator()(int &x) {
        char ch = get();
        bool flg = false;
        while ((ch < '0' || ch > '9') && (ch ^ '-')) ch = get();
        if (ch == '-') flg = true, ch = get();
        for (x = 0; ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
        if (flg) x = -x;
    }
} in;
struct FastO {
    char buf[1 << 20], *p1;
    const char *p2;
    FastO() : buf{}, p1(buf), p2(buf + sizeof(buf)) {}
    ~FastO() { fwrite(buf, p1 - buf, 1, stdout); }
    inline void put(char c) {
        if (__builtin_expect(p1 == p2, 0)) fwrite(buf, sizeof buf, 1, stdout), p1 = buf;
        *p1++ = c;
    }
    inline void operator()(int x) {
        if (x < 0) put('-'), x = -x;
        static int sta[20];
        sta[sta[0] = 1] = x % 10;
        while (sta[0]) put(sta[sta[0]--] | '0');
    }
} out;
int n;
#define X first
#define Y second
constexpr int N = 300005;
using pii_t = std::pair<int,int>;
using crpii_t = const pii_t&;
pii_t a[N];
inline void sub1() {
    out.put((a[1].Y >= abs(a[1].X)) | '0');
    out.put('\n');
}
inline void sub2() {
    std::sort(a+1, a+3, [](crpii_t a, crpii_t b){return a.Y > b.Y;});
    if (a[1].Y <= 0) return out.put('1'), out.put('\n');
    if (a[2].Y <= 0) return out.put('1'), out.put('\n');
    int f1 = a[1].Y - abs(a[1].X),
        f2 = a[2].Y - abs(a[2].X);
    if (!(f1>=0 | f2>=0)) return out.put('1'), out.put('\n');
    if (!(f1>=0 & f2>=0)) return out.put('1'), out.put('\n');
    bool flg = (a[2].Y - std::max(abs(a[1].X), a[1].Y) - a[1].Y >= abs(a[1].X - a[2].X));
    std::swap(f1, f2), std::swap(a[1], a[2]);
    flg &= (a[2].Y - std::max(abs(a[1].X), a[1].Y) - a[1].Y >= abs(a[1].X - a[2].X));
    out.put('1'), out.put('\n'); // 保 f1 争 f2
}
int main() {
    int T; in(T);
    for (int tc = 1; tc <= T; tc++) {
        in(n);
        for (int i = 0; i <= n; i++)
            in(a[i].X), in(a[i].Y);
        for (int i = 1; i <= n; i++) {
            a[i].X -= a[0].X;
            a[i].Y -= a[0].Y;
        }
        switch (T) {
        case (int)3e6: sub1(); break;
        case (int)1.5e6-1: case (int)1.5e6: sub2(); break;
        }
    }
    return 0;
}