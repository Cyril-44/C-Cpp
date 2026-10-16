#include <bits/stdc++.h>
constexpr int N = 1005;
int a[N]; uint64_t c[N];
struct BigInt {
    constexpr static int N = 1000;
    uint64_t a[N]; int n = 0;
    BigInt& operator*=(uint64_t v) {
        uint64_t rem = 0; int i;
        for (i = 0; i < n || rem; i++) {
            auto res = rem + (__uint128_t)a[i] * v;
            a[i] = res & (__uint128_t)(~0ull);
            rem = res >> 64;
        }
        n = i;
        return *this;
    }
    BigInt operator/=(uint64_t v) {
        uint64_t rem = 0;
        for (int i = n-1; i >= 0; i--) {
            auto val = ((__uint128_t)rem << 64) + a[i];
            a[i] = val / v;
            rem = val % v;
        }
        while (n && !a[n-1]) --n;
        return *this;
    }
    BigInt& operator+=(const BigInt &o) {
        uint64_t rem = 0; int i;
        for (i = 0; i < n || i < o.n || rem; i++) {
            auto res = rem + (__uint128_t)a[i] + o.a[i];
            a[i] = res & (__uint128_t)(~0ull);
            rem = res >> 64;
        }
        n = i;
        return *this;
    }
    friend bool operator==(const BigInt& x, const BigInt& y) {
        if (x.n != y.n) return false;
        for (int i = 0; i < x.n; i++)
            if (x.a[i] != y.a[i]) return false;
        return true;
    }
    BigInt operator*(uint64_t v) const { return BigInt(*this) *= v; }
    BigInt operator/(uint64_t v) const { return BigInt(*this) /= v; }
    BigInt operator+(const BigInt &o) const { return BigInt(*this) += o; }
    BigInt() : a{}, n() {}
    BigInt(uint64_t v) : a{}, n(1) { a[0] = v; }
    void debug(char ch = '\n') {
        printf("%llX", n ? a[n-1] : 0ull);
        for (int i = n-2; i >= 0; i--) printf("%016llX", a[i]);
        if (ch) putchar(ch);
    }
};
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    std::set<int> cst;
    for (int i = 1; i <= m; i++) {
        scanf("%llu", &c[i]);
        if (c[i] <= (int)1e9)
            cst.insert(c[i]);
    }
    BigInt P = 1, sum1, p2 = 1;
    for (int i = 1; i <= n; i++) P *= a[i];
    for (int i = 1; i <= n; i++)
        if (!cst.count(a[i]))
            sum1 += P / a[i];
    for (int i = 1; i <= m; i++) p2 *= c[i];
    puts(sum1 == p2 ? "S" : "N");
}