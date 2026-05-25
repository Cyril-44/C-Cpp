#include <cstdio>
#include <cstring>
#include <algorithm>
#include <valarray>
std::valarray<int> f;

#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr int N = 105;
constexpr double ZERO = 1e-4;
using Mat = double[N][N];
static Mat m;
double ans[N];
inline int guassElim(Mat m, double ans[], int n) {
    int idx = 1;
    For(v, 1, n) { // 表示当前已经消了几个元（从左往右依次消元）
        { // 找到基准行（这一行的带消元的系数是最大的）
            int pivot = idx;
            For(i, idx+1, n) if (std::abs(m[i][v]) > std::abs(m[pivot][v])) pivot = i;
            if (std::abs(m[pivot][v]) <= ZERO) continue; // 目前这个元是解不了的
            if (pivot != idx) For(j, 1, n+1) std::swap(m[idx][j], m[pivot][j]);
        }
        For(i, idx+1, n) { // 用基准行加减消元
            double mul = m[i][v] / m[idx][v];
            For(j, v+1, n+1) m[i][j] -= mul * m[idx][j];
        }
        ++idx; // 有一个式子废掉了
    }
    if (idx <= n) { // 元没有消干净
        for (; idx <= n; idx++) if (std::abs(m[idx][n+1]) > ZERO) return -1; // 0x != 0 ===> 无解
        return 0; // 0x = 0 ===> 无穷多组解
    }
    roF(v, n, 1) { // 带回求值
        ans[v] = m[v][n+1];
        For(j, v+1, n) ans[v] -= m[v][j] * ans[j];
        ans[v] /= m[v][v];
    }
    return 1;
}
int main() {
    int n, x;
    scanf("%d", &n);
    For(i, 1, n) For(j, 1, n+1) scanf("%d", &x), m[i][j] = x;
    switch (guassElim(m, ans, n)) {
        case 1: For(i, 1, n) printf("x%d=%.2lf\n", i, ans[i]); break;
        case 0: puts("0"); break;
        case -1: puts("-1"); break;
    }
    return 0;
}