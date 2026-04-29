#include <algorithm>
#include <cstdio>
#include <vector>
#include <numeric>
#include <cstring>
#include <cassert>
#include <unordered_map>
constexpr int _N = 20, N = 1<<_N;
int n;
int a[N], b[N];
int64_t c[N], d[N];
/* int mask;
std::bitset<1<<N> chs;
int ans = 0;
inline bool check() {
    for (int st = 0; st < (1<<n); st++) {
        if (__builtin_popcount(st) > 2) {
            bool first = true;
            for (int i = 0; i < st; i++)
                for (int j = 0; j < st; j++)
                    if (chs[i] == chs[j] && (i | j) == st) {
                        if (first) chs[st] = chs[i], first = false;
                        else if (!(chs[st] == chs[i])) return false;
                    }
            if (first) return false;
        }
    }
    return true;
}
void dfs(int st) {
    if (st == (1<<n)) return void(ans += check());
    switch (__builtin_popcount(st)) {
        case 0: case 1:
            chs[st] = 0, dfs(st+1);
            chs[st] = 1, dfs(st+1);
        case 2:
            if (chs[st&-st] == chs[st&st-1])
                return chs[st] = chs[st&-st], dfs(st+1);
            chs[st] = 0, dfs(st+1);
            chs[st] = 1, dfs(st+1);
        default:
            return dfs(st + 1);
    }
} */
std::vector<int> trans[N];
inline void fwt() {
	for (int o = 2, k = 1; o <= n; o <<= 1, k <<= 1)
		for (int i = 0; i < n; i += o)
			for (int j = 0; j < k; j++)
                trans[i+j].push_back(i+j+k);
}
inline void orfwt() {
	for (int o = 2, k = 1; o <= n; o <<= 1, k <<= 1)
		for (int i = 0; i < n; i += o)
			for (int j = 0; j < k; j++)
                c[i+j+k] += c[i+j];
}
int main() {
    const int sz = sizeof(std::unordered_map<int,int>);
    scanf("%d", &n); n = 1 << n;
    // dfs(0);
    fwt();
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    for (int i = 0; i < n; i++) scanf("%d", &b[i]);
    const int64_t sum = std::accumulate(a, a+n, 0ll);
    for (int i = 0; i < n; i++) c[i] = b[i] - a[i];
    orfwt();
    memset(d, 0x3f, sizeof d);
    d[0] = std::min((int64_t)0, c[0]);
    for (int i = 0; i < n; i++)
        for (int j : trans[i]) {
            d[j] = std::min({d[j], d[i], d[i]+c[j]-c[i]});
            // printf("%d->%d %ld\n", i, j, c[j] - c[i]);
        }
    printf("%ld\n", sum + d[n-1]);
    return 0;
}