namespace BF {
    constexpr int N = 5005;
    int sum[N/2][N];
    inline void work(int q) {
        std::reverse_copy(s+1, s+1+n, revs+1);
        saP.init(n, s);
        saS.init(n, revs);
        for (int len = 1; len*2 <= n; len++) {
            std::set<std::pair<int,int>> occurs;
            for (int i = len, j = len+len; j <= n; i += len, j += len) {
                int rs = lcp(i, j), ls = lcs(i-1, j-1);
                int l = i - ls, r = j-1 + rs;
                if (ls + rs >= len) {
                    if (r >= j+len) { // Goal: let j > r
                        int add = (r - j - len) / len * len;
                        i += add, j += add;
                    }
                    if (occurs.emplace(l, r).second)
                        for (int w = ls + rs; w >= len; w--, l++) ++sum[len][l];
                }
            }
        }
        for (int i = 1; i*2 <= n; i++)
            for (int j = 1; j <= n; j++)
                sum[i][j] += sum[i][j-1];
        while (q--) {
            int l, r;
            scanf("%d%d", &l, &r);
            int w = (r-l+1)/2;
            long long ans = 0;
            for (int i = 1; i <= w; i++)
                ans += sum[i][r - i*2+1] - sum[i][l-1];
            printf("%lld\n", ans);
        }
    }
}
namespace AllSame {
    inline void work(int q) {
        while (q--) {
            int l, r;
            scanf("%d%d", &l, &r);
            int m = r - l;
            if (m & 1) printf("%lld\n", (m + 1ll) * (m + 1ll) / 2 / 2);
            else printf("%lld\n", m * (m/2+1ll) / 2);
        }
    }
}