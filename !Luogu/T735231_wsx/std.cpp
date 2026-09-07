#include <cstdio>
#include <algorithm>

using namespace std;

// 使用全局数组以避免栈溢出，s数组约占144MB内存
int s[6005][6005];
int ans[1000005];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // 1. 统计每个点出现的频率
    for (int i = 0; i < n; ++i) {
        int w, v;
        scanf("%d %d", &w, &v);
        if (w <= 6000 && v <= 6000) {
            s[w][v]++;
        }
    }

    // 2. 计算二维后缀和，s[i][j] 表示 w >= i 且 v >= j 的人数
    for (int i = 6000; i >= 1; --i) {
        for (int j = 6000; j >= 1; --j) {
            s[i][j] = s[i][j] + s[i + 1][j] + s[i][j + 1] - s[i + 1][j + 1];
        }
    }

    // 3. 遍历所有阈值组合，更新对应人数的最大阈值和
    for (int i = 1; i <= 6000; ++i) {
        for (int j = 1; j <= 6000; ++j) {
            int count = s[i][j];
            if (count > 0) {
                // 如果人数超过n（理论上s[1][1]=n），则更新对应人数的答案
                if (count > n) count = n; 
                if (i + j > ans[count]) ans[count] = i + j;
            }
        }
    }

    // 4. 后缀最大值：至少交到 k 个朋友的最大和，不小于至少交到 k+1 个的
    for (int i = n - 1; i >= 1; --i) {
        if (ans[i + 1] > ans[i]) ans[i] = ans[i + 1];
    }

    // 5. 输出结果
    for (int i = 1; i <= n; ++i) {
        printf("%d%c", ans[i], i == n ? '\n' : ' ');
    }

    return 0;
}