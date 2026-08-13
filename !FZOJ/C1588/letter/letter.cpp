#include <bits/stdc++.h>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
#define roF(i, s, t) for(int i = (s); i >= (t); i--)
constexpr int N = 200005, S = 26;
char s[N], t[N], fs[N], ft[N];
int beg[S], end[S], w[N], fsidx[N], border[N];
bool intr[N];
int64_t _f[2][N], wsum[N], fwsum[N];
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
        memset(f, 0, sizeof(int64_t) * (n+1));
        f[0] = 0;
        for (int prti = 1, idx = 1; idx <= m; prti++) {
            bool drop[S]{};
            int tops = 0, topt = 0;
            for (; !intr[idx]; idx++) ft[++topt] = t[idx];
            For(j, 0, S-1) drop[j] = beg[j] >= idx; // 这些字符不能进行一个前缀的删
            For(i, 1, n) if (!drop[s[idx]]) fs[++tops] = s[idx], fsidx[tops] = idx, fwsum[tops] = fwsum[tops - 1] + w[i];
            int j = 0;
            For(i, 1, topt) {
                while (j && ft[j+1] != ft[i]) j = border[j];
                if (ft[j+1] == ft[i]) j++;
                border[i] = j;
            }
            memset(nf, 0x3f, sizeof(int64_t) * (n+1));
            For(i, 1, tops) {
                while (j == topt || j && ft[j+1] == fs[i]) j = border[j];
                if (ft[j+1] == fs[i]) j++;
                if (j == topt)
                    For(k, fsidx[j - topt], fsidx[j - topt + 1] - 1)
                        nf[fsidx[j]] = std::min(f[fsidx[j]], f[k] + wsum[fsidx[j]] - wsum[j-1] - (fwsum[j] - fwsum[j - topt]));
            }
            std::swap(f, nf);
        }
    }
    return 0;
}
