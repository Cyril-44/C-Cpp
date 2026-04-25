#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <map>
constexpr int N = 50005, M = 100005;
struct Point {
    int x, y, v;
    inline bool operator<(const Point &o) const { return x + y < o.x + o.y; }
} pnts[N << 1];
int64_t sum[M];
int n;
/*
考虑只有 Pos_b < Pos_c 或者 Pos_d < Pos_a 的时候 p 才会有贡献。
设置 F[i][j] 表示当前已经选了前 i 个红色
*/
struct BIT {
    int64_t tr[N];
    inline void update(int p, int64_t x) {
        for (; p <= n; p += p & -p) tr[p] = std::min(tr[p], x);
    }
    inline int64_t inquire(int p) {
        int64_t res = 0x3f3f3f3f3f3f3f3fll;
        for (; p > 0; p -= p & -p) res = std::min(res, tr[p]);
        return res;
    }
} f;
int main() {
    int m, k;
    scanf("%d%d%d", &n, &m, &k);
    std::map<std::pair<int,int>, int> mp;
    for (int i = 1; i <= k; i++) {
        int a, b, c, d, p;
        scanf("%d%d%d%d%d", &a, &b, &c, &d, &p);
        mp[{d,a}] += p;
        mp[{b,c}] -= p;
    }
    mp[{n,m}] += 0;
    int64_t ans;
    for (const auto &i : mp) {
        int y = i.first.second, v = i.second;
        ans = v + f.inquire(y);
        f.update(y, ans);
    }
    printf("%ld\n", ans);
    return 0;
}