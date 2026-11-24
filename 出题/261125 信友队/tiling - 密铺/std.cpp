#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * 固定 N = 4。每一列用 4 位掩码表示（第 r 位为 1 表示第 r 行是白格）：
 *   15 = 1111（全白），用作分隔列
 *    9 = 1001（只有第 0、3 行是白格）
 *
 * 前缀状态向量 (x, y)：
 *   x = 轮廓线为空的方案数
 *   y = 第 0、3 行被横放骨牌占用的方案数
 * 把下列列段接在状态向量之后：
 *   L = 列 [15, 9]     : (x, y) -> (x,     x + y)
 *   F = 列 [15, 9, 9]  : (x, y) -> (x + y, x    )
 * 起点 (1,0)，末尾补一个全白列，方案数读出为 x + y。
 * 因此只需把 (K-r, r) 用逆欧几里得还原到 (1,0)，
 * 代价为 2*#L + 3*#F，枚举 r 取最小即可。
 */

static const ll INF = (ll)4e18;

static ll evalPair(ll x, ll y, ll bound, string* ops) {
    ll cost = 0;
    if (ops) ops->clear();
    while (!(x == 1 && y == 0)) {
        if (x >= y) {
            if (y == 0) return INF;          // gcd > 1，无解
            cost += 3;
            if (ops) ops->push_back('F');
            ll nx = y, ny = x - y;
            x = nx; y = ny;
        } else {                              // y > x >= 1，批量做 L 的逆操作
            ll k = (y - 1) / x;
            cost += 2 * k;
            if (cost >= bound) return INF;
            if (ops) ops->append((size_t)k, 'L');
            y -= k * x;
        }
        if (cost >= bound) return INF;
    }
    return cost;
}

int main() {
    ll T, K;
    if (scanf("%lld%lld", &T, &K) != 2) return 0;

    ll best = INF, bestR = -1;
    auto tryR = [&](ll r) {
        if (r < 0 || r > K - 1) return;
        ll c = evalPair(K - r, r, best, nullptr);
        if (c < best) { best = c; bestR = r; }
    };

    if (K <= 3000000) {
        for (ll r = 0; r < K; r++) tryR(r);
    } else {
        const long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
        ll center = (ll)llroundl((long double)K / (phi * phi));
        const ll W = 2000000;
        for (ll d = 0; d <= W; d++) {
            tryR(center + d);
            tryR(center - d);
        }
    }

    string rev;
    evalPair(K - bestR, bestR, INF, &rev);
    reverse(rev.begin(), rev.end());          // 转成正向操作序列

    vector<int> cols;
    for (char c : rev) {
        cols.push_back(15);
        cols.push_back(9);
        if (c == 'F') cols.push_back(9);
    }
    cols.push_back(15);

    int N = 4, M = (int)cols.size();
    printf("%d %d\n", N, M);
    for (int i = 0; i < N; i++) {
        string row(M, '1');
        for (int j = 0; j < M; j++)
            if (cols[j] >> i & 1) row[j] = '0';
        puts(row.c_str());
    }
    return 0;
}
