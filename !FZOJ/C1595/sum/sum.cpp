#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 1000004;
namespace FastI {
char buf[1 << 20], *p1{}, *p2{};
inline char gc() {
    if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
    return *p1++;
}
inline void in(int &x) {
    char ch = gc();
    while (ch < '0' || ch > '9') ch = gc();
    for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
        x = (x << 3) + (x << 1) + (ch & 0xf);
}
} using FastI::in;
int a[N], digsum[N];
inline int calc(int x) {
    int res = 0;
    for (; x; x /= 10) res += x % 10;
    return res;
}
int main() {
    int n, mxai = 0;
    for (int i = 1; i <= 10000; i++)
        digsum[i] = digsum[i / 10] + i % 10;
    in(n);
    for (int i = 1; i <= n; i++)
        in(a[i]), mxai = std::max(mxai, a[i]);
    if (mxai <= 2000) {
        int ans = 1145141919;
        for (int x = n <= 2000 ? 8000 : 1000; x >= 0; x--) {
            int now = 0;
            for (int i = 1; i <= n; i++)
                now += digsum[a[i] + x];
            ans = std::min(ans, now);
        }
        printf("%d\n", ans);
    } else {
        int64_t ans = 11451419198101145ll;
        for (int x = 100; x >= 0; x--) {
            int64_t now = 0;
            for (int i = 1; i <= n; i++)
                now += calc(a[i] + x);
            ans = std::min(ans, now);
        }
        printf("%lld\n", ans);
    }
    return 0;
}