#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <numeric>
#include <algorithm>
#include <queue>
constexpr int N = 5005;
int h[N], oud[N], all[N];
char allg[N][N];
// 将 d 中的 (x, x) ==> (x-1, x+1) 实质上是减少一个三元环。
int64_t mxTri(int64_t n) {
    int64_t mnred = (n & 1) ? 
        (n/2)*(n/2-1)/2 * n : // binom((n-1)/2, 2) * n
        (n/2)*(n/2-1)/2 * (n/2) + (n/2-1)*(n/2-2)/2 * (n/2); // binom(n/2, 2) * n/2 + binom(n/2-1, 2) * n/2 
    return n*(n-1)*(n-2)/6 - mnred;
}
int main() {
    int q, n;
    int64_t m;
    scanf("%d", &q);
    for (int iq = 1; iq <= q; iq++) {
        scanf("%d%ld", &n, &m);
        if (mxTri(n) < m) { puts("No"); continue; }
        int cn = 3;
        while (mxTri(cn) < m) ++cn;
        m = mxTri(cn) - m; // 需要减少的三元环个数
        memset(h, 0, sizeof(int) * cn);
        std::queue<int> que({cn/2});
        if (cn & 1) h[cn/2] = cn;
        else h[cn/2-1] = h[cn/2] = cn/2, que.push(cn/2-1);
        while (!que.empty()) {
            int u = que.front(); que.pop();
            int hu = h[u], hupre = h[u-1], hunxt = h[u+1];
            if (m <= (hu >> 1)) { // 够了直接退出
                h[u-1] += m, h[u+1] += m;
                h[u] -= m << 1;
                break;
            }
            h[u-1] += hu >> 1, h[u+1] += hu >> 1, h[u] &= 1; // (x, x) ==> (x-1, x+1)
            m -= hu >> 1;
            if (u-1 > 0 && hupre <= 1 && h[u-1] > 1) que.push(u-1);
            if (u+1 < cn-1 && hunxt <= 1 && h[u+1] > 1) que.push(u+1);
        }
        for (int i = 1, now = 0; i <= cn; i++) {
            while (!h[now]) ++now;
            oud[n - cn + i] = now + (n - cn), --h[now];
        }
        std::iota(oud+1, oud+1 + n-cn, 0);
        for (int i = 1; i < n; i++) { // 向后面连接 oud[i] 个点，剩余的点连向自己。
            allg[i+1][i+1] = 0;
            std::iota(all+1, all+1+n-i, i+1);
            std::sort(all+1, all+1+n-i, [](int x,int y){ return oud[x] < oud[y]; }); // 连向自己要 --oud，尽量挑大的去减
            for (int j = 1; j <= oud[i]; j++) allg[all[j]][i] = '0';
            for (int j = oud[i]+1; j <= n-i; j++) allg[all[j]][i] = '1', --oud[all[j]];
        }
        puts("Yes");
        for (int i = 2; i <= n; i++)
            puts(allg[i] + 1);
    }
    return 0;
}