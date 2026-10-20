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
int a[N], f[N], n;
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
int main() {
    freopen("opti.in", "r", stdin);
    freopen("opti.out", "w", stdout);
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> rngk(-1, 1), rngp(0, 1);
    int T; in(T);
    while (T--) {
        in(n);
        for (int i = 1; i <= n; i++) in(a[i]);
        const int mn = *std::max_element(a+1, a+1+n);
        int x = mn + ((int)2e9 - mn) / 2, curfx = calc(x);
        int ansx = x, ansfx = curfx;
        constexpr double tk = 1e2;
        const double td = n <= 100 ? 0.998 : 0.97;
        const int mv = n <= 100 ? 2000 : 200;
        for (double t = 1e9; t >= tk; t *= td) {
            int nhi = std::max(mn, (int)(x + t * rngk(rng)));
            int now = calc(nhi);
            if (now < ansfx) ansfx = now, ansx = nhi;
            auto delta = std::pow(now, 2.25) - std::pow(curfx, 2.25);
            if (rngp(rng) < std::exp(-delta / t))
                curfx = now, x = nhi;
        }
        for (int i = std::max(x - mv, mn); i <= x + mv; i++)
            ansfx = std::min(ansfx, calc(i));
        printf("%d\n", ansfx);
    }
}