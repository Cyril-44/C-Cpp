#include <cstdio>
#include <cstring>
#include <cmath>
#include <unordered_map>
constexpr int N = 1 << 16;
inline int qpow(int b, int n, const int mod) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ul*res*b % mod;
        b = 1ul*b*b % mod, n >>= 1;
    }
    return res;
}
// Find the min non-negative x such that a^x===b (mod m)
// or -1 representing no solution.
// Needs m to be a prime number and a to be a positive integer.
inline int solvePrime(const int a, const int b, const int m) {
    if (b == 1) return 0; // a^0 = 1
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
int main() {
    int a, b, m;
    scanf("%d%d%d", &m, &a, &b);
    int res = solvePrime(a, b, m);
    if (res != -1) printf("%d\n", res);
    else puts("no solution");
    return 0;
}