#include <cstdio>
#include <algorithm>
#include <cmath>
#include <vector>
const int N = 100005;
int a[N];           // 初始情况
int __uni[N << 1];  // 离散化数组
int h[N];           // 当前情况
int blksz;          // 莫队块大小
struct Query {
    int pos; // 要查询 a[1..pos]
    int k;   // 查询的是数值为k的
    int ts;  // 查询时间
    int id;  // 查询编号
    int flg; // 是 [1..l-1] (-1) 还是 [1..r] (+1)
    inline bool operator<(const Query &x) const {
        return (ts / blksz ^ x.ts / blksz) ? (ts < x.ts) : (
            (ts / blksz & 1) ? (pos < x.pos) : (pos > x.pos)
        );
    }
} q[N << 1];
int modis[N][2]; // 每一次修改
int cnt[N << 1]; // 当前数值为 i 的个数
int ans[N];      // 最终答案
bool qans[N];    // 第 i 次查询是否为答案
int main() {
    int n, m, A, B, X;
    int ts = 0; // 时间戳
    int q0 = 0; // 询问次数
    int t0;     // 询问中 ts=0 的数量
    int t;      // 莫队-当前时间
    int r;      // 莫队-当前右端点
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        __uni[i] = a[i];
    }
    char ch;
    for (int i = 0; i < m; i++) {
        scanf(" %c", &ch);
        switch (ch) {
        case 'C': scanf("%d%d", &A, &X);
            ++ts;
            modis[ts][0] = A;
            modis[ts][1] = X;
            __uni[i + n + 1] = X;
            break;
        case 'Q': scanf("%d%d%d", &A, &B, &X);
            q[q0++] = {A-1, X, ts, i, -1};
            q[q0++] = {B, X, ts, i, 1};
            __uni[i + n + 1] = X;
            qans[i] = true;
            break;
        }
    }
    /* Unique */ {
    auto __beg = __uni+1, __end = __uni+1 + n+m;
    std::sort(__beg, __end);
    __end = std::unique(__beg, __end);
    for (int i = 1; i <= n; i++)
        a[i] = std::lower_bound(__beg, __end, a[i]) - __uni;
    for (int i = 0; i < q0; i++)
        q[i].k = std::lower_bound(__beg, __end, q[i].k) - __uni;
    for (int i = 1; i <= ts; i++)
        modis[ts][1] = std::lower_bound(__beg, __end, modis[ts][1]) - __uni;
    } /* End Unique */
    /* PreCalc ts=0 Query */ {
        std::sort(q, q+q0, [](const Query& x, const Query& y) {
            return x.ts < y.ts || (!x.ts && !y.ts && x.pos < y.pos); });
        for (t0 = t = r = 0; t0 < q0 && !q[t0].ts; ++t0) {
            while (r < q[t0].pos) ++cnt[a[++r]], h[r] = a[r];
            ans[q[t0].id] += q[t0].flg * cnt[q[t0].k];
        }
    } /* End PreCalc */
    blksz = sqrt(ts);
    std::sort(q + t0, q + q0);  // 不排前面 ts=0 的情况，避免转移到 ts=0
    // Mo-Algo-Func [
    const auto updt = [&r] (const int &t) ->void {
        if (modis[t][0] <= r) --cnt[h[modis[t][0]]];
        h[modis[t][0]] = modis[t][1];
        if (modis[t][0] <= r) ++cnt[modis[t][1]];
    };
    const auto addp = [&t] (const int &p) ->void {
        ++cnt[h[p]];
    };
    const auto delp = [] (const int &p) ->void {
        --cnt[h[p]];
    };
    // ] End Mo-Algo-Func
    for (int i = t0; i < q0; i++) {
        while (t > q[i].ts) updt(--t);
        while (t < q[i].ts) updt(++t);
        while (r < q[i].pos) addp(++r);
        while (r > q[i].pos) delp(r--);
        ans[q[i].id] += q[i].flg * cnt[q[i].k];
    }
    for (int i = 0; i < m; i++)
        if(qans[i]) printf("%d\n", ans[i]);
    return 0;
}