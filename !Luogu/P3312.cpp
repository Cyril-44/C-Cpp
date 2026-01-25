#include <stdio.h>
#include <vector>
#include <algorithm>
constexpr int N = 100005;
long long sigma[N];
int mu[N], _init_ = [](int n = 1e5) {
    mu[1] = 1;
    for (int i = 1; i <= (n >> 1); i++)
        for (int j = i << 1; j <= n; j += i)
            mu[j] -= mu[i];
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            sigma[j] += i;
return 0; }();
int n;
struct BIT {
    long long tr[N]{};
    inline long long sum(int p) {
        long long res = 0;
        for (; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
    inline long long operator()(int l, int r) { return sum(r) - sum(l-1); }
    inline void upd(int p, long long x) {
        for (; p <= n; p += p & -p) tr[p] += x;
    }
} f;
struct Node {int n, m, a, id;} q[N];
int ans[N];
int main() {
    int Q;
    scanf("%d", &Q);
    for (int i = 1; i <= Q; i++) {
        scanf("%d%d%d", &q[i].n, &q[i].m, &q[i].a);
        if (q[i].n > q[i].m) std::swap(q[i].n, q[i].m);
        q[i].id = i;
        n = std::max(n, q[i].n);
    }
    std::sort(q+1, q+1 + Q, [](const Node&x, const Node&y){return x.a < y.a;});
    std::vector<std::pair<long long,int>> sigmas(n);
    for (int i = 1; i <= n; i++) sigmas[i-1] = {sigma[i], i};
    std::sort(sigmas.begin(), sigmas.end());
    auto it = sigmas.begin();
    for (int i = 1; i <= Q; i++) {
        for (; it != sigmas.end() && it->first <= q[i].a; ++it)
            for (int j = it->second, k = 1; j <= n; j += it->second, k++)
                f.upd(j, it->first * mu[k]);
        long long res = 0;
        for (int l = 1, r, x, y; l <= q[i].n; l = r + 1) {
            r = std::min(q[i].n / (x = q[i].n / l), q[i].m / (y = q[i].m / l));
            res += f(l, r) * x * y;
        }
        ans[q[i].id] = res & 0x7fffffff;
    }
    for (int i = 1; i <= Q; i++) printf("%d\n", ans[i]);
    return 0;
}