#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 200005, LG = 17;
char s[N];
int sa[N], rk[N], h[N], lcpst[N][LG+1], n, k;
inline void getSA(int n, char *s, int *sa, int *rk_, int *h, int sigma = 127) {
    static int cnt[N], tsa[N], tmp[N];
    int *rk = rk_, *tprk = tmp;
    tmp[n+1] = rk[n+1] = s[n+1] = 0;
    auto radixSort = [&]() {
        for (int i = 1; i <= n; i++) cnt[rk[i]]++;
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[tsa[i]]]--] = tsa[i];
        memset(cnt+1, 0, sizeof(int) * sigma);
    };
    for (int i = 1; i <= n; i++) tsa[i] = i, rk[i] = s[i];
    radixSort();
    for (int w = 1; w < n; w <<= 1) {
        int ord = 0;
        for (int i = n - w + 1; i <= n; i++) tsa[++ord] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > w) tsa[++ord] = sa[i] - w;
        radixSort(); sigma = 0;
        for (int i = 1; i <= n; i++)
            tprk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && rk[sa[i]+w] == rk[sa[i-1]+w]);
        std::swap(tprk, rk);
    }
    if (rk != rk_) memcpy(rk_+1, rk+1, sizeof(int) * n);
    for (int i = 1, w = 0; i <= n; i++) {
        if (rk[i] == 1) continue;
        if (w) --w;
        while (s[i+w] == s[sa[rk[i]-1]+w]) ++w;
        h[rk[i]] = w;
    }
    for (int i = 2; i <= n; i++) lcpst[i][0] = h[i];
    for (int k = 2; k <= LG; k++)
        for (int i = 2; i+(1<<k)-1 <= n; i++)
            lcpst[i][k] = std::min(lcpst[i][k-1], lcpst[i+(1<<k-1)][k-1]);
}
inline int inquireST(int l, int r) {
    int k = 31 - __builtin_clz(r - l++);
    return std::min(lcpst[l][k], lcpst[r-(1<<k)+1][k]);
}
inline int lcp(int l, int r) {
    if (l == r) return n - l + 1;
    if ((l = rk[l]) > (r = rk[r])) std::swap(l, r);
    return inquireST(l, r);
}
namespace BruteForce {
constexpr int N = 3005;
int rank[N][N];
unsigned f[2][N];
inline void work() {
    int ord = 0;
    for (int i = 1; i <= n; i++) {
        int start = sa[i], fall = i;
        while (h[fall]) --fall;
        for (int len = 1, end = start; end <= n; ++len, ++end) {
            while (fall < i && inquireST(fall, i) < len) ++fall;
            if (fall == i) rank[start][end] = ++ord;
            else rank[start][end] = rank[sa[fall]][sa[fall] + len - 1];
            // printf("len=%d, [%d,%d], fall on %d, rnk = %d\n", len, start, end, sa[fall], rank[start][end]);
        }
    }
    // for (int i = 1; i <= n; i++)
    //     for (int j = i; j <= n; j++)
    //         printf("%d%c", rank[i][j], " \n"[j==n]);
    auto fnow = f[1], fpre = f[0];
    for (int i = 1; i <= n; i++) fnow[i] = rank[1][i];
    for (int lv = 2; lv <= k; lv++) {
        std::swap(fnow, fpre);
        for (int i = lv; i <= n; i++) {
            fnow[i] = 0;
            for (int j = lv-1; j < i; j++)
                fnow[i] = std::max(fnow[i], fpre[j] + rank[j+1][i]);
        }
    }
    printf("%u\n", fnow[n]);
}
}
int main() {
    scanf("%d%d %s", &n, &k, s+1);
    getSA(n, s, sa, rk, h);
    // for (int i = 1; i <= n; i++) printf("%d%c", sa[i], " \n"[i==n]);
    // for (int i = 1; i <= n; i++) printf("%d%c", rk[i], " \n"[i==n]);
    // for (int i = 1; i <= n; i++) printf("%d%c", h[i], " \n"[i==n]);
    BruteForce::work();
    return 0;
}
