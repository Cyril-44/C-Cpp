#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
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
constexpr int N = 2004;
std::vector<int> a[N];
int sum[N][N];
int n;
int main() {
    int testid, m, q;
    in(testid), in(n), in(m);
    for (int i = 1; i <= n; i++) {
        int k; in(k); a[i].resize(k);
        for (int j = 0; j < k; j++) {
            int col; in(col); ++sum[col][i];
            a[i][j] = col;
        }
    }
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            sum[i][j] += sum[i][j-1];
    in(q);
    for (int i = 1, l, r, p; i <= q; i++) {
        in(l), in(r), in(p);
        int ans = 0;
        for (int col : a[p]) {
            if (sum[col][r] - sum[col][l-1] == 1)
                ++ans;
        }
        printf("%d\n", ans);
    }
    return 0;
}