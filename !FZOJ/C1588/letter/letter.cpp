#include <bits/stdc++.h>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
#define roF(i, s, t) for(int i = (s); i >= (t); i--)
constexpr int N = 200005, S = 26;
char s[N], t[N], prt[N];
int beg[S], end[S], w[N], border[N], fidx[N];
bool intr[N];
int64_t _f[2][N], wsum[N], fwsum[N];
constexpr int64_t INF = 0x3f3f3f3f3f3f3f3fll;
int main() {
    int Tid, T;
    scanf("%d%d", &Tid, &T);
    while (T--) {
        int n, m;
        scanf("%d%d %s %s", &n, &m, s+1, t+1);
        for (int i = 1; i <= n; i++)
            scanf("%d", &w[i]), wsum[i] = wsum[i-1] + w[i];
        For(i, 1, n) s[i] -= 'a';
        For(i, 1, m) t[i] -= 'a';
        memset(beg, 0, sizeof beg);
        memset(end, 0, sizeof end);
        memset(intr, 0, sizeof intr);
        For(i, 1, m) if (!beg[t[i]]) beg[t[i]] = i, intr[i] = true;
        roF(i, m, 1) if (!end[t[i]]) end[t[i]] = i, intr[i+1] = true;

        int64_t *f = _f[0], *nf = _f[1];
        memset(f, 0x3f, sizeof(int64_t) * (n+1));
        f[0] = 0;
        for (int prti = 1, idx = 1; idx <= m; prti++) {
            // printf("Partition %d [%d~", prti, idx);
            bool drop[S]{}, save[S]{};
            For(ch, 0, S-1)
                save[ch] = idx >= beg[ch] && idx <= end[ch],
                drop[ch] = idx <= beg[ch] || idx > end[ch];
            int t1 = 0, t2 = 0, j = 0; prt[++t1] = t[idx++];
            for (; !intr[idx]; idx++) prt[++t1] = t[idx];
            // printf("%d]:\n", idx-1);
            For(i, 2, t1) {
                while (j && prt[j+1] != prt[i]) j = border[j];
                if (prt[j+1] == prt[i]) j++;
                border[i] = j;
            }
            memset(nf, 0x3f, sizeof(int64_t) * (n+1));
            j = 0;
            For(i, 1, n) {
                if (drop[s[i]]) {
                    f[i] = std::min(f[i], f[i-1] + w[i]);
                    // printf("f[%d]=f[%d](%lld)+%lld\n", i, i-1, f[i-1], w[i]);
                }
                if (save[s[i]]) {
                    fidx[++t2] = i;
                    fwsum[t2] = fwsum[t2-1] + w[i];
                    while (j == t1 || j && prt[j+1] != s[i]) j = border[j];
                    if (prt[j+1] == s[i]) ++j;
                    // printf("i=%d: %d\n", i, j);
                    if (j == t1) {
                        nf[i] = std::min(nf[i], f[fidx[t2-t1+1]-1] + (wsum[i] - wsum[fidx[t2-t1+1]-1]) - (fwsum[t2] - fwsum[t2-t1]));
                        // printf("nf[%d]=f[%d](%lld)+%lld-%lld\n", i, fidx[t2-t1], f[fidx[t2-t1]], (wsum[i] - wsum[fidx[t2-t1]]), (fwsum[t2] - fwsum[t2-t1]));
                    }
                }
            }
            std::swap(f, nf);
        }
        int64_t ans = INF;
        For(i, 1, n) ans = std::min(ans, f[i] + wsum[n] - wsum[i]);
        printf("%lld\n", ans == INF ? -1 : ans);
    }
    return 0;
}
