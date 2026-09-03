#include <bits/extc++.h>
constexpr int N = 18;
__gnu_pbds::priority_queue<int> q[1<<N];
int64_t f[1<<N];
int r[1<<N];
/*
贪心的选取 r 更大的孩子胜出
f[u](i) 表示左端点为 i 时，最少的钱数
发现如果当前在 l, 应该是把函数 |x-l| 加到 f[u] 上。显然函数是凸的，考虑维护斜率变化的点
从子树转移来的，应该就是相加 ==> Slope Trick 合并变化点。
直观理解本题的变化点，最低点就是斜率从 -1 ~ 1 的位置，也就是最靠前的 l.
r 不够的时候拿过来转移，右端点扩展的代价就是 L - r.
本题显然不需要斜率 < 0 的部分，少维护一个堆。
f[u] 代表 i=q[u].top() 时 f 的值。后面的值根据斜率去推。
*/
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0, l; i < (1<<n); i++) {
        scanf("%d%d", &l, &r[i]);
        q[i].push(l);
    }
    for (int w = 0; w < n; w++) {
        for (int i = 0; i < (1<<n); i += 1<<w+1) {
            if (r[i] > r[i + (1<<w)]) std::swap(r[i], r[i + (1<<w)]);
            q[i].join(q[i + (1<<w)]);
            f[i] += f[i + (1<<w)];
            if (q[i].top() > r[i]) f[i] += q[i].top() - r[i], q[i].pop(), q[i].push(r[i]);
            std::swap(r[i], r[i + (1<<w)]);
        }
    }
    printf("%lld\n", f[0]);
    return 0;
}