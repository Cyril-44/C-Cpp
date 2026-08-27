#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 1000004, B = 10;
namespace FastI {
char buf[1 << 20], *p1{}, *p2{};
inline char gc() {
    if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
    if (p1 == p2) return EOF;
    return *p1++;
}
inline void in(int &x) {
    char ch = gc();
    while (ch < '0' || ch > '9') ch = gc();
    for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
        x = (x << 3) + (x << 1) + (ch & 0xf);
}
} using FastI::in;
int64_t f[2][N];
int a[N], _a[N], anow[N], n;
void radixSort() {
    int h[B]{};
    for (int i = 1; i <= n; i++) ++h[anow[i]];
    for (int v = B-2; v >= 0; v--) h[v] += h[v+1];
    for (int i = n; i >= 1; i--) _a[h[anow[i]]--] = a[i] / 10;
    memcpy(a, _a, sizeof a);
}
inline void umin(int64_t &x, int64_t y) { if (x > y) x = y; }
int main() {
    in(n);
    for (int i = 1; i <= n; i++)
        in(a[i]);
    memset(f[0], 0x3f, sizeof(f) / 2);
    f[0][0] = 0;
    for (int i = 1; i <= 10; i++) {
        memset(f[i&1], 0x3f, sizeof(f) / 2);
        for (int j = 1; j <= n; j++) anow[j] = a[j] % 10;
        for (int d = 0; d < B; d++) {
            int add = 0, cnt = 0; // 增量、进位个数
            for (int j = 1; j <= n; j++)
                if (anow[j] + d >= B) add += anow[j] + d - B, ++cnt;
                else add += anow[j] + d;
            // fprintf(stderr, "(%d,%d)", add, cnt);
            umin(f[i&1][cnt], f[i&1^1][0] + add);
            for (int j = 1; j <= n; j++) {
                if (anow[j] + d + 1 == B) add -= 9, ++cnt;
                else add++;
                // fprintf(stderr, "%d(%d,%d)", anow[j], add, cnt);
                umin(f[i&1][cnt], f[i&1^1][j] + add);
            }
            // fprintf(stderr, "\n");
        }
        if (i < 10) radixSort();
        // for (int j = 0; j <= n; j++)
        //     fprintf(stderr, "%lld%c", f[i&1][j], " \n"[j==n]);
    }
    printf("%lld\n", f[10&1][0]);
    return 0;
}