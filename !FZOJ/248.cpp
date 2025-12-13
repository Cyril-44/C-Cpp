#include <cstdio>
#include <algorithm>
#include <cstring>
#include <unordered_map>
#pragma GCC optimize("unroll-loops", "inline", 2)
typedef unsigned long long u64;
constexpr int N = 100005;
int a[N][6];
static inline constexpr u64 rotl64(const u64& x, const int& r) {
    return (x << r) | (x >> (64 - r));
}
static inline u64 fmix64(u64 k) {
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccduLL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53uLL;
    k ^= k >> 33;
    return k;
}
u64 keygen(int* x) {
    static constexpr u64 C[6] = {
        0x9ae16a3b2f90404full, 0xc949d7c7509e6557ull,
        0xff51afd7ed558ccdull, 0xc4ceb9fe1a85ec53ull,
        0x9ddfea08eb382d69ull, 0x94d049bb133111ebull
    };

    u64 h = 0x85ebca6b; // 基础种子
    for (int i = 0; i < 6; ++i) {
        u64 v = (unsigned)x[i] * C[i];  // 用预混常数表
        h = (h ^ v) * 0xc2b2ae35;            // CRC-like 更新
    }
    return fmix64(h); // 快速 avalanche
}
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < 6; j++)
            scanf("%d", &a[i][j]);
    std::unordered_map<u64, int> mp[7];
    int arr[6]{};
    for (int i = 1; i <= n; i++)
        for (int s = 0; s < 1<<6; s++) {
            if (__builtin_popcount(s) < k) continue;
            for (int j = 0; j < 6; j++)
                if (s >> j & 1) arr[j] = a[i][j];
                else arr[j] = 0;
            ++mp[__builtin_popcount(s)][keygen(arr)];
        }
    long long ans = 0;
    int c[7][7]{};
    for (int i = 0; i <= 6; i++) {
        c[i][0] = c[i][i] = 1;
        for (int j = 1; j < i; j++)
            c[i][j] = c[i-1][j-1] + c[i-1][j];
    }
    for (int j = k; j <= 6; j++) {
        long long sum = 0;
        for (auto pi : mp[j]) {
            const int &tp = pi.second;
            sum += (tp - 1ll) * tp / 2;
        }
        if (k - j & 1) ans -= sum * c[j][k];
        else ans += sum * c[j][k];
    }
    printf("%lld\n", (long long)ans);
    return 0;
}