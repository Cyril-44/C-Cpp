#include <bits/stdc++.h>
constexpr int N = 50004, M = 6, B = 27;
uint64_t base[M], hash[1<<M];
std::unordered_map<uint64_t, int> mp[1<<M][1<<M];
int main() {
    base[0] = 1;
    for (int i = 1; i < M; i++) base[i] = base[i-1] * B;
    int n, m;
    scanf("%d%d", &n, &m);
    int64_t ans = 0;
    for (int idx = 1; idx <= n; idx++) {
        char str[M+1]; scanf("%s", str);
        int mask = 0;
        for (int i = 0; i < m; i++)
            if (str[i] != '?') mask |= 1<<i;
        memset(hash, 0, sizeof hash);
        for (int s = 0; s < (1<<m); s++)
            if ((mask & s) == s)
                for (int i = 0; i < m; i++)
                    if (s >> i & 1) hash[s] += (str[i] - 'a' + 1) * base[i];
        for (int s1 = 0; s1 < (1<<m); s1++) if ((mask & s1) == 0)
            for (int s2 = 0; s2 < (1<<m); s2++) if ((mask & s2) == s2)
                ans += mp[s1][s2][hash[mask ^ s2]];
        for (int s1 = 0; s1 < (1<<m); s1++) if ((mask & s1) == s1)
            for (int s2 = 0; s2 < (1<<m); s2++) if ((mask & s2) == 0)
                mp[s1][s2][hash[mask ^ s1]]++;
    }
    printf("%lld\n", ans);
}