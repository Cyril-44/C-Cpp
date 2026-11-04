#include <bits/stdc++.h>
constexpr int N = 2333;
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= N) val -= N; return *this; }
    Mint& operator*=(const Mint o) { val = 1u * val * o.val % N; return *this; }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(short v=0) : val(v) {}
    template<class T> operator T() const { return static_cast<T>(val); }
private: short val;
} C[N][N], Cs[N][N];
Mint lucas(int64_t n, int64_t k) {
    if (n < k) return 0;
    if (n < N) return C[n][k];
    return lucas(n / N, k / N) * C[n % N][k % N];
}
Mint calc(int64_t n, int64_t k) {
    if (k == -1) return 0;
    k = std::min(k, n);
    if (n < N) return Cs[n][k];
    return calc(n / N, k / N - 1) * Cs[n % N][N-1] + lucas(n / N, k / N) * Cs[n % N][k % N];
}
int main() {
    int T; int64_t n, k;
    for (int i = 0; i < N; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    for (int i = 0; i < N; i++) {
        Cs[i][0] = C[i][0];
        for (int j = 1; j < N; j++)
            Cs[i][j] = Cs[i][j-1] + C[i][j];
    }
    scanf("%d", &T);
    while (T--) {
        scanf("%ld%ld", &n, &k);
        printf("%d\n", int(calc(n, k)));
    }
}