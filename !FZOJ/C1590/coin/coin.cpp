#include <cstdio>
#include <set>
#include <cstdint>
constexpr int N = 100005;
int a[N], b[N], h[N];
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", a+i);
    for (int i = 1; i <= n; i++) scanf("%d", b+i);
    std::multiset<std::pair<int,int>> q1, q2, bak;
    for (int i = 1; i <= n; i++) q1.emplace(a[i], i), q2.emplace(a[i] + b[i], i);
    auto ins1a = [&q1](int p) { q1.emplace(a[p], p); };
    auto ins1b = [&q1](int p) { q1.emplace(b[p], p); };
    auto ins2  = [&q2](int p) { q2.emplace(a[p] + b[p], p); };
    auto era1a = [&q1](int p) { q1.erase(q1.find({a[p], p})); };
    auto era1b = [&q1](int p) { q1.erase(q1.find({b[p], p})); };
    auto era2  = [&q2](int p) { q2.erase({a[p] + b[p], p}); };
    int64_t sum = 0, ans = 0;
    for (int j = 1; j <= m; j++, ans ^= sum) {
        if (q1.empty()) continue;
        auto [wq1, fq1] = *q1.rbegin();
        if (q2.empty()) {
            ++h[fq1], sum += wq1;
            q1.erase({wq1, fq1});
            continue;
        }
        auto [wq2, fq2] = *q2.rbegin();
        // if (!bak.empty()) printf("\n%d %d ", bak.begin()->first, wq2);
        if (!bak.empty() && -bak.begin()->first + wq2 > wq1) {
            auto [wbk, fbk] = *bak.begin(); bak.erase({wbk, fbk});
            sum = sum - wbk + wq2;
            q2.erase({wq2, fq2});
            switch (--h[fbk]) {
                case 0: ins1a(fbk); era1b(fbk); break;
                case 1: ins1b(fbk); era2(fbk); break;
                case 2: ins1a(fbk);
            }
            switch (h[fq2] += 2) {
                case 3: era1b(fq2);
            }
            // printf("\n[%d]%2d{h=%d} [%d]%2d{h=%d} [%d]%2d{h=%d} ", fq1, wq1, h[fq1], fq2, wq2, h[fq2], fbk, wbk, h[fbk]);
        } else {
            sum += wq1;
            q1.erase({wq1, fq1});
            switch (++h[fq1]) {
                case 1: ins1b(fq1); break;
                case 2: ins1a(fq1); era2(fq1); bak.erase({a[fq1], fq1}); break;
                case 3: bak.erase({b[fq1], fq1});
            }
            bak.emplace(wq1, fq1);
            // printf("\n[%d]%2d{h=%d} ", fq1, wq1, h[fq1]);
        }
        // printf("%lld%c", sum, " \n"[j==m]);
        // fflush(stdout);
    }
    printf("%lld\n", ans);
    return 0;
}