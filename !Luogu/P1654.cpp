#include <stdio.h>
/* 一道优美的概率 DP 题
 * 计算位置 i 相对于 i-1 的期望加分 \delta ans_i
 * 设位置 i-1 往前连续 j 个都为 1 的概率是 p_j
 * 那么我们有 \delta ans_i = P_i * \sum_{j=0}^{i-1} p_j * (3j^2 + 3j + 1)
 * 每次有转移 p_i := p_{i-1} * P_i，p_0 = 1 - P_i
 * 拆开 \delta ans_i = P_i * (3 \x2_i + 3 x_i + x0_i)
 * x2_i = (0 p_0 + 1 p_1 + 4 p_2 + ... + (i-1)^2 p_{i-1})
 * x1_i = (0 p_0 + 1 p_1 + 2 p_2 + ... + (i-1) p_{i-1})
 * x0_i = (p_0 + p_1 + p_2 + ... + p_{i-1})
 * 所以得到转移 x2_i = P_{i-1} * (x2_{i-1} + 2 * x1_{i-1} + x0_{i-1})
 * x1_i = P_{i-1} * (x1_{i-1} + x0_{i-1})
 * x0_i = (1 - P_{i-1}) + P_{i-1} * x0_{i-1}
 * 时间复杂度 O(n) 空间复杂度 O(1)
 */
int main() {
    int n;
    scanf("%d", &n);
    double ans, x[3]{1, 0, 0};
    scanf("%lf", &ans);
    for (double p = ans; --n; ) {
        x[2] = p * (x[2] + 2 * x[1] + x[0]);
        x[1] = p * (x[1] + x[0]);
        x[0] = (1 - p) + p * x[0];
        scanf("%lf", &p);
        ans += p * (3 * x[2] + 3 * x[1] + x[0]);
    }
    printf("%.1lf\n", ans);
    return 0;
}