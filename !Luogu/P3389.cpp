#include <cstdio>
#include <cstring>
#include <algorithm>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr int N = 105;
constexpr double ZERO = 1e-9;
using Mat = double[N][N];
static Mat m;
double ans[N];
inline bool guassElim(Mat m, double ans[], int n) {
    For(v, 1, n) { // 表示当前已经消了几个元（从左往右依次消元）
        { // 找到基准行（这一行的带消元的系数是最大的）
            int pivot = v;
            For(i, v+1, n) if (std::abs(m[i][v]) > std::abs(m[pivot][v])) pivot = i;
            if (std::abs(m[pivot][v]) < ZERO) return false;
            if (pivot != v) For(j, 1, n+1) std::swap(m[v][j], m[pivot][j]);
        }
        For(i, v+1, n) { // 用基准行加减消元
            double mul = m[i][v] / m[v][v];
            For(j, v+1, n+1) m[i][j] -= mul * m[v][j];
        }
    }
    roF(v, n, 1) { // 带回求值
        ans[v] = m[v][n+1];
        For(j, v+1, n) ans[v] -= m[v][j] * ans[j];
        ans[v] /= m[v][v];
    }
    return true;
}
int main() {
    int n, x;
    scanf("%d", &n);
    For(i, 1, n) For(j, 1, n+1) scanf("%d", &x), m[i][j] = x;
    if (!guassElim(m, ans, n)) return puts("No Solution"), 0;
    For(i, 1, n) printf("%.2lf\n", ans[i]);
    return 0;
}