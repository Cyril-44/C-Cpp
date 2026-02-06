#include <stdio.h>
#include <algorithm>
constexpr int N = 100005;
/*
网络流建图：(S) ---p_i--> (i) ---c--> (j) ---s_j--> (T)
转换为最小割：设 f[i][j] 表示枚举到第 i 个点，前 i 个点中有 j 个属于 S 割。
    1. 如果钦定 i 属于 S 割，那要断掉 (i) ---s_i--> (T)，价值就是 f[i-1][j-1] + s[i]
    2. 如果钦定 i 属于 T 割，那要断掉 (S) ---p_i--> (i) 和 (j) ---c--> (i)，价值就是 f[i-1][j] + p[i] + c*j
考虑优化，设最终方案是属于 S{u_i} 和 T{v_j} 集合的割。那么价值就是 \sum_{u_i} s[u_i] + \sum_{v_j} p[v_j] + \sum{u_i, v_j} [u_i < v_j]
初始的时候设置所有点都在 T 集合中，当前价值是 \sum p[i]。
考虑一个一个将 i 放进 S 集合中，每放进一个 i 到 S，对权值的影响是 -p[i] + s[i] + c*(n - i - |S|)
这里的 n - i - |S| 是因为，如果放一个 i 进入 S，那么对于所有的 j > i 都要和 i 断边，但是又考虑到已经有 |S| 个元素在 S 中了，
    1. 如果元素 j 在 i 的右边，那么 i 不能和他们再断边了
    2. 如果元素 j 在 i 的左边，那么之前的决策 j 和 i 断边应该撤销
  所以综上就是 n - i - |S|
那解法就呼之欲出了，我们令 v[i] = -p[i] + s[i] + c*(n-i) 那么就从小到大排序
令初始的计数 sum = \sum p[i]，然后枚举 i = 1..n，sum += v[i] - c*(i-1) 这里是把 |S| 拎出来了
最后的 ans = \min sum
时间 O(n log n) 空间 O(n)
*/
int p[N];
long long v[N];
int main() {
    int n, c;
    scanf("%d%d", &n, &c);
    long long sum = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &p[i]);
        sum += p[i];
    }
    for (int i = 1, si; i <= n; i++) {
        scanf("%d", &si);
        v[i] = -p[i] + si + 1ll * c * (n - i);
    }
    std::sort(v+1, v+1 + n);
    long long ans = sum;
    for (int i = 1; i <= n; i++) {
        sum += v[i] - c * (i - 1ll);
        ans = std::min(ans, sum);
    }
    printf("%lld\n", ans);
    return 0;
}