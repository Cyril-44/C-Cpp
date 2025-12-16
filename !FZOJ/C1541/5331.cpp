#include <stdio.h>
#include <string.h>
#include <algorithm>
#define logprint(...) fprintf(stderr, __VA_ARGS__)
#pragma GCC optimize(2)
constexpr int N = 600005, K = 35;
int a[N << 1];
struct Sol_n2k {
    constexpr static int N = 2005;
    int f[N*2][N][2], g[N*2][N][2], h[N*2][N], o[N][K];
    int ans;
    Sol_n2k(int n, const int kk) {
        for (int i = 1; i <= (n<<1); ++i)
            for (int l = 1; l <= n; l++) {
                if (i + l - 1 > (n<<1)) break;
                if (a[i + l - 1] > f[i][l-1][0]) f[i][l][0] = a[i + l - 1], f[i][l][1] = f[i][l-1][1] + 1;
                else f[i][l][0] = f[i][l-1][0], f[i][l][1] = f[i][l-1][1];
            }
        for (int i = n*2; i >= 1; --i)
            for (int l = 1; l <= n; l++) {
                if (i - l + 1 < 1) break;
                if (a[i - l + 1] > g[i][l-1][0]) g[i][l][0] = a[i - l + 1], g[i][l][1] = g[i][l-1][1] + 1;
                else g[i][l][0] = g[i][l-1][0], g[i][l][1] = g[i][l-1][1];
            }
        for (int i = 1; i <= (n<<1); ++i) 
            for (int l = 1; l <= n; l++) {
                if (i + l - 1 > (n<<1)) break;
                h[i][l] = std::max(f[i][l][1], g[i+l-1][l][1]);
                /* logprint("[%d, %d] \n", i, i+l-1);
                for (int _ = i; _ <= j; ++_) logprint("%d ", a[_]);
                logprint(": mx=%d\n", h[i][l]); */
            }
        ans = 0;
        for (int st = 0; st < n; ++st) {
            memset(o, -0x3f, sizeof o);
            o[0][0] = 0;
            for (int i = 1; i <= n; i++)
                for (int k = 1; k <= kk; k++)
                    for (int j = 0; j < i; ++j)
                            o[i][k] = std::max(o[i][k], o[j][k-1] + h[j+1 + st][i-j]);
            ans = std::max(ans, o[n][kk]);
        }
    }
};
struct Sol_k_1 {
    int ans;
    Sol_k_1(int n) {
        int pos = 0;
        for (int i = 1; i <= n; i++)
            if (a[i] == n) { pos = i; break; }
        int end = pos + n - 1;
        ans = 0;
        int last = n + 1, cnt = 0;
        for (int i = pos; i <= end; i++)
            if (a[i] < last) last = a[i], ++cnt;
        ans = std::max(ans, cnt);
        last = 0, cnt = 0;
        for (int i = end; i >= pos; --i)
            if (a[i] > last) last = a[i], ++cnt;
        ans = std::max(ans, cnt);
    }
};
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    memcpy(a+n+1, a+1, sizeof(int) * n);
    if (k == 1) printf("%d\n", (new Sol_k_1(n)) -> ans);
    else printf("%d\n", (new Sol_n2k(n, k)) -> ans);
    return 0;
}