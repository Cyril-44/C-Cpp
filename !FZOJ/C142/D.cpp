#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int N = 200005;
char s[N], t[N], sp[N];
int p[N];
long long w[N];
using Hash_b = unsigned long long;
using Hash_t = std::pair<Hash_b, Hash_b>;
constexpr Hash_b B1 = 31, B2 = 37;
Hash_b Pow1[N], Pow2[N], __ = [](const int n = 200000) {
    Pow1[0] = Pow2[0] = 1;
    for (int i = 1; i <= n; i++) {
        Pow1[i] = Pow1[i-1] * B1;
        Pow2[i] = Pow2[i-1] * B2;
    }
    return 0ULL;
}();
Hash_t hash[N];
Hash_t gethash(int l, int r) { // (l, r]
    return Hash_t(hash[r].first - hash[l].first * Pow1[r - l], hash[r].second - hash[l].second * Pow2[r - l]);
}
int main() {
    int tid, T, n, m;
    scanf("%d%d", &tid, &T);
    while (T--) {
        scanf("%d%d %s %s", &n, &m, s+1, t+1);
        for (int i = 1; i <= n; i++) {
            scanf("%lld", &w[i]);
            w[i] += w[i-1];
        }
        bool charset[26]{};
        for (int i = 1; i <= m; i++)
            charset[t[i] - 'a'] = true;
        int len = 0;
        for (int i = 1; i <= n; i++) {
            if (charset[s[i] - 'a'])
                sp[++len] = s[i], p[len] = i;
        }
        for (int i = 1; i <= len; i++) {
            hash[i].first = hash[i-1].first * B1 + (sp[i] - 'a');
            hash[i].second = hash[i-1].second * B2 + (sp[i] - 'a');
        }
        Hash_t thash;
        for (int i = 1; i <= m; i++) {
            thash.first = thash.first * B1 + (t[i] - 'a');
            thash.second = thash.second * B2 + (t[i] - 'a');
        }
        long long ans = -1;
        p[len+1] = m+1;
        for (int i = m; i <= len; i++) {
            if (gethash(i - m, i) == thash) {
                ans = std::max(ans, w[p[i+1]-1] - w[p[i-m]]);
            }
        }
        if (ans == -1) puts("-1");
        else printf("%lld\n", w[n] - ans);
    }
    return 0;
}

/*
0 3
7 3
ababccb
abc
7 2 2 4 3 2 1
5 4
babab
baab
2 1 3 2 4
10 5
bbbbababaa
bbbab
5 1 1 0 5 1 1 2 4 4
*/
