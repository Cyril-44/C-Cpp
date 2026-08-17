#include <bits/stdc++.h>
constexpr int N = 100004, M = 31;
std::string s, t[M];
std::vector<std::tuple<int,int,unsigned>> rgs[M];
int n, m, K;
unsigned p[N], w[M], c[M], f[N];
struct BIT {
    std::vector<unsigned> tr;
    unsigned _sum = 0;
    BIT() : tr(n+1) {}
    void upd(int p, unsigned x) { _sum += x; for (; p <= n; p += p & -p) tr[p] += x; }
    unsigned sum(int p) const { unsigned res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
    unsigned sum() const { return _sum; }
};
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int tid;
    std::cin >> n >> m >> K >> tid >> s;
    for (int i = 0; i < n; i++) std::cin >> p[i];
    for (int i = 1; i <= m; i++) {
        std::cin >> t[i] >> w[i] >> c[i];
        std::vector<int> border(t[i].size());
        int j = 0;
        for (int k = 1; k < (int)t[i].size(); k++) {
            while (j && t[i][j] != t[i][k]) j = border[j - 1];
            if (t[i][j] == t[i][k]) ++j;
            border[k] = j;
        }
        j = 0;
        for (int k = 0; k < (int)s.size(); k++) {
            while (j && t[i][j] != s[k]) j = border[j - 1];
            if (t[i][j] == s[k]) ++j;
            if (j == t[i].size()) {
                rgs[c[i]].emplace_back(k+1, k+1-j+1, w[i] + p[k - j + 1]);
                // fprintf(stderr, "(%d,%d, %u)\n", k+1, k+1-j+1, w[i] + p[k-j+1]);
                j = border[j - 1];
            }
        }
    }
    for (int i = 1; i <= m; i++) std::sort(rgs[i].begin(), rgs[i].end());
    if (tid <= 5) {
        unsigned ans = 0;
        for (int i = 1; i <= m; i++)
            for (auto [r, l, w] : rgs[i])
                ans += w;
        printf("%u\n", ans);
    }
    else if (tid <= 9 || 14 <= tid && tid <= 17) {
        int idx[M]{};
        unsigned ans = 0;
        std::vector<BIT> bit(m+1);
        for (int i = 1; i <= n; i++) {
            for (int k = 1; k <= m; k++) {
                for (; idx[k] < rgs[k].size() && std::get<0>(rgs[k][idx[k]]) == i; ++idx[k]) bit[k].upd(std::get<1>(rgs[k][idx[k]]), std::get<2>(rgs[k][idx[k]]));
                f[i] = std::max(f[i], bit[k].sum());
                for (int j = 0; j+2*K <= i; j++) f[i] = std::max(f[i], f[j] + bit[k].sum() - bit[k].sum(j+K));
            }
            ans = std::max(ans, f[i]);
        }
        printf("%u\n", ans);
    }
    else {
        int idx[M]{};
        unsigned ans = 0;
        for (int i = 1; i <= n; i++) {
            for (int k = 1; k <= m; k++) {
                if (idx[k] >= rgs[k].size() || std::get<0>(rgs[k][idx[k]]) != i) continue;
                while (idx[k] < rgs[k].size() && std::get<0>(rgs[k][idx[k]]) == i) ++idx[k];
                unsigned wsum = 0, w;
                for (int j = idx[k]-1, r, l; j >= 0; j--) {
                    std::tie(r, l, w) = rgs[k][j];
                    wsum += w;
                    if (l >= K) f[i] = std::max(f[i], f[l - K] + wsum);
                }
                f[i] = std::max(f[i], wsum);
            }
            ans = std::max(ans, f[i]);
        }
        printf("%u\n", ans);
    }
    return 0;
}