#include <iostream>
#include <algorithm>

using namespace std;

// 真正对数级 O(log m) 的安全解法
long long solve_min(long long X, long long Y, long long m, long long L) {
    X = (X % m + m) % m;
    Y = (Y % m + m) % m;
    if (L <= 0) return m; 
    if (Y == 0) return X;
    if (X + Y * (L - 1) < m) return X; 

    // 【核心修复】：防止 Y 和 m 极其接近时产生 O(m) 退化
    if (Y > m / 2) {
        long long Y_prime = m - Y;
        if (Y_prime * L - X - 1 < 0) {
            // 说明即使 i 取到最大，也无法让 X - Y'*i 跌破 0，最小值在最后一项
            return (X - Y_prime * (L - 1)) % m;
        }
        long long L_new = (Y_prime * L - X - 1) / m + 1;
        return solve_min(X % Y_prime, m % Y_prime, Y_prime, L_new);
    }

    // 以下为原本 Y <= m / 2 时的标准类欧几里得逻辑，此时收敛速度天然极快
    if (X >= Y) {
        long long i1 = (m - X + Y - 1) / Y; 
        if (i1 < L) {
            return min(X, solve_min((X + Y * i1) % m, Y, m, L - i1));
        } else {
            return X;
        }
    } else {
        long long L_prime = (Y * L - 1) / m + 1;
        long long new_Y = (Y - m % Y) % Y;
        long long new_X = (X + new_Y) % Y; 
        return min(X, solve_min(new_X, new_Y, Y, L_prime - 1));
    }
}

// 求最大值（保持逻辑不变，直接调用安全的 solve_min）
long long solve_max(long long X, long long Y, long long m, long long L) {
    long long new_X = (-X - 1 % m + m) % m;
    long long new_Y = (-Y % m + m) % m;
    return m - 1 - solve_min(new_X, new_Y, m, L);
}

int main() {
    freopen("garden.in", "r", stdin);
    freopen("garden.out", "w", stdout);
    int T;
    scanf("%d", &T);
    while (T--) {
        long long X, Y, m, L;
        scanf("%lld %lld %lld %lld", &L, &X, &Y, &m);
        printf("%lld %lld\n", solve_max(X, Y, m, L), solve_min(X, Y, m, L));
    }
    return 0;
}