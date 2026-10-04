#include <cstdio>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <map>
#include <vector>
#include <bitset>
constexpr int N = 500005;
int a[N];
int p[N/4], lfac[N];
std::bitset<N> np;
std::vector<std::pair<int,int>> pfacs;
std::vector<int> facs;
std::map<std::pair<int,int>,int> f;
void dfs(int i=0, int num=1) {
    if (i == pfacs.size()) { facs.push_back(num); return; }
    for (int x = 0; x <= pfacs[i].second; x++)
        dfs(i+1, num), num *= pfacs[i].first;
}
int main() {
    // freopen("perm.in", "r", stdin);
    // freopen("perm.out", "r", stdout);
    for (int i = 2; i <= 500000; i++) {
        if (!np[i]) p[++p[0]] = i, lfac[i] = i;
        for (int j = 1; j <= p[0] && i * p[j] <= 500000; j++) {
            np[i * p[j]] = true;
            lfac[i * p[j]] = p[j];
            if (i % p[j] == 0) break;
        }
    }
    pfacs.reserve(50), facs.reserve(2000);
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        int64_t ans = 0;
        f.clear();
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        for (int i = 1; i <= n; i++) {
            int g = std::__gcd(a[i], i), x = a[i] / g, y = i / g;
            pfacs.clear(), facs.clear(); 
            while (x > 1) {
                int tmp = lfac[x], cnt = 0;
                do x /= tmp, ++cnt; while (x % tmp == 0);
                pfacs.emplace_back(tmp, cnt);
            }
            dfs();
            for (int j : facs) {
                auto it = f.find({j, y});
                if (it != f.end()) ans += it->second;
            }
            for (int j : facs) ++f[{y, j}];
        }
        printf("%lld\n", ans);
    }
}