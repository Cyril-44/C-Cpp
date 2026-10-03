#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
constexpr int N = 1005, M = 1000005;
char a[N], b[M];
int f[N];
std::vector<int> pos[26];
int main() {
    int n, m;
    scanf("%d%d %s %s", &n, &m, a+1, b+1);
    for (int j = 1; j <= m; j++) pos[b[j]-'A'].push_back(j);
    memset(f, 0x3f, sizeof f);
    f[0] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = i; j >= 1; j--) {
            auto it = std::upper_bound(pos[a[i]-'A'].begin(), pos[a[i]-'A'].end(), f[j-1]);
            if (it != pos[a[i]-'A'].end()) f[j] = std::min(f[j], *it);
        }
    for (int j = n; j >= 0; j--)
        if (f[j] <= m) { printf("%d\n", j); return 0; }
}