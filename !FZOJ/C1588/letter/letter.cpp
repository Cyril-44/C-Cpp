#include <bits/stdc++.h>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
#define roF(i, s, t) for(int i = (s); i >= (t); i--)
constexpr int N = 200005, S = 26;
char s[N], t[N], fs[N], ft[N];
int beg[S], end[S], w[N], fsidx[N], border[N];
bool intr[N];
int64_t _f[2][N], wsum[N], fwsum[N];
constexpr int64_t INF = 0x3f3f3f3f3f3f3f3fll;
int main() {
#ifdef DEBUG
    freopen("ex_letter1.in", "r", stdin);
#endif
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
        For(i, 1, m) if (!beg[t[i]]) beg[t[i]] = i, intr[i+1] = true;
        roF(i, m, 1) if (!end[t[i]]) end[t[i]] = i, intr[i+1] = true;

        int64_t *f = _f[0], *nf = _f[1];
        memset(f, 0x3f, sizeof(int64_t) * (n+1));
        f[0] = 0;
        for (int prti = 1, idx = 1; idx <= m; prti++) {
            printf("Partition %d [%d~", prti, idx);
            bool drop[S]{};
            int tops = 0, topt = 0;
            For(j, 0, S-1) drop[j] = beg[j] > idx; // 这些字符不能进行一个前缀的删
            ft[++topt] = t[idx++];
            for (; !intr[idx]; idx++) ft[++topt] = t[idx];
            For(i, 1, n) if (!drop[s[i]]) fs[++tops] = s[i], fsidx[tops] = i, fwsum[tops] = fwsum[tops - 1] + w[i];
            int j = 0;
            printf("%d]: ", idx-1);
            For(i, 2, topt) {
                while (j && ft[j+1] != ft[i]) j = border[j];
                if (ft[j+1] == ft[i]) j++;
                border[i] = j;
            }
            memset(nf, 0x3f, sizeof(int64_t) * (n+1));
            For(i, 1, tops) fs[i] += 'a';
            For(i, 1, topt) ft[i] += 'a';
            printf("s=\"%*s\", t=\"%*s\"\n", tops, fs+1, topt, ft+1);
            For(i, 1, tops) fs[i] -= 'a';
            For(i, 1, topt) ft[i] -= 'a';
            For(i, 1, tops) {
                while (j == topt || j && ft[j+1] == fs[i]) j = border[j];
                if (ft[j+1] == fs[i]) j++;
                if (j == topt) {
                    printf("Matched on s[%d]\n", i);
                    For(k, fsidx[i - topt], fsidx[i - topt + 1] - 1) if (f[k] != INF) {
                        nf[fsidx[i]] = std::min(nf[fsidx[i]], f[k] + wsum[fsidx[i]] - wsum[k] - (fwsum[i] - fwsum[i - topt]));
                        fprintf(stderr, "%d <- %d, %lld\n", fsidx[i], k, f[k] + wsum[fsidx[i]] - wsum[k] - (fwsum[i] - fwsum[i - topt]));
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
