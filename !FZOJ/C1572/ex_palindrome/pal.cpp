#include <cinttypes>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <map>
#include <vector>
constexpr int N = 6005, MOD = (int)1e9 + 7;
uint64_t base[N], hash[N][2];
int b[N];

inline void inithash(int n, int *a, uint64_t (*hash)[2]) {
    hash[0][0] = hash[n+1][1] = 0;
    for (int i = 1; i <= n; i++)
        hash[i][0] = hash[i-1][0] * MOD + a[i];
    for (int i = n; i >= 1; i--)
        hash[i][1] = hash[i+1][1] * MOD + a[i];
}
inline bool isPalin(int l, int r, uint64_t (*hash)[2]) {
    return hash[r][0] - hash[l-1][0] * base[r-l+1] == hash[l][1] - hash[r+1][1] * base[r-l+1];
}
inline int calc(int n) {
    inithash(n, b, hash);
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j++)
            cnt += isPalin(i, j, hash);
    return cnt;
}
int main() {
    int n;
    uint a;
    scanf("%d", &n);
    base[0] = 1;
    for (int i = 1; i <= n; i++) base[i] = base[i-1] * MOD;
    for (int i = 1; i <= n; i++) scanf("%d", &b[i]);
    std::vector<std::vector<int>> bests;
    int mx = 0;
    std::sort(b+1, b+1+n);
    do {
        int now = calc(n);
        if (now > mx) bests.clear();
        if (now >= mx) bests.emplace_back(b+1, b+1+n), mx = now;
    } while (std::next_permutation(b+1, b+1+n));
    printf("%d %zu\n", mx, bests.size());
    for (const auto &i : bests) {
        for (int j : i)
            printf("%d ", j);
        putchar('\n');
    }
    return 0;
}