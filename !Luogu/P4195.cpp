#include <cstdio>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <cassert>
constexpr int N = 1 << 16;
inline int qpow(int b, int n, const int mod) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ul*res*b % mod;
        b = 1ul*b*b % mod, n >>= 1;
    }
    return res;
}
inline int gcd(int x, int y) {
    int z;
    while (y) z = x, x = y, y = z % y;
    return x;
}
int exgcd(int a, int b, int64_t& x, int64_t& y) {
    if (!b) { x = 1, y = 0; return a; }
    int g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g; 
}
// Find the min non-negative x such that a^x===b (mod m)
// or -1 representing no solution.
// Needs m to be a prime number and a to be a positive integer.
inline int solvePrime(const int a, const int b, const int m) {
    if (b == 1 || m == 1) return 0; // a^0 = 1
    int B = sqrt(m);
    std::unordered_map<int, int> mp;
    int now = 1;
    for (int i = 0, tmp = b; i < B; i++) {
        if (now == b) return i;
        if (mp.count(tmp)) return -1;
        mp[tmp] = i;
        now = 1ul*now * a % m;
        tmp = 1ul*tmp * a % m;
    }
    const int base = now;
    now = 1;
    for (int i = 1; i <= B; i++) {
        now = 1ul*now * base % m;
        if (mp.count(now)) return i * B - mp[now];
    }
    for (int i = B*B + 1; i < m; i++) {
        now = 1ul*now * a % m;
        if (now == b) return i;
    }
    return -1;
}
inline int solve(int a, int b, int m) {
    b %= m;
    int D = 1, _m = m, d;
    if (b == 1 || m == 1) return 0;
    int now = 1, n;
    for (n = 1; (d = gcd(a, m)) != 1; n++) {
        m /= d, D *= d;
        if (b % D) return -1;
        now = 1ul*now * a % _m;
        if (now == b) return n;
    }
    --n;
    int64_t inv, _; // get a^{-n} mod m
    int gcd = exgcd(qpow(a, n, m), m, inv, _);
    assert(gcd == 1);
    inv = ((inv % m) + m) % m;
    int res = solvePrime(a, 1l*b * inv % m, m);
    if (res == -1) return -1;
    return n + res;
}
int main() {
    int a, b, m;
    while (scanf("%d%d%d", &a, &m, &b) && m) {
        int res = solve(a, b, m);
        if (res != -1) printf("%d\n", res);
        else puts("No Solution");
    }
    return 0;
}