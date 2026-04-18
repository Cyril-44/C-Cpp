#include <bits/stdc++.h>
constexpr int N = 1005, MOD = (int)1e9 + 7;
int v[N], now[N], fact[N];
bool vv[20][20];
inline int qpow(int b, int n) {
    int res = 1;
    while (n) {
        if (n & 1) res = 1ull * res * b % MOD;
        b = 1ull * b * b % MOD;
        n >>= 1;
    }
    return res;
}
int main() {
    int n;
    scanf("%d", &n);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = 1ull * fact[i-1] * i % MOD;
    bool TestFlagA = true, TestFlagB = true;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &v[i]);
        if (v[i] <= i) TestFlagA = false;
        if (v[i] != 0) TestFlagB = false;
    }
    if (TestFlagA) return puts("1"), 0;
    // else if (TestFlagB) return printf("%d\n", qpow(n, n)), 0;
    else {
        std::vector<int> vf(n), vb(n);
        std::iota(vf.begin(), vf.end(), 1);
        std::iota(vb.begin(), vb.end(), 1);
        std::set<std::vector<int>> st;
        unsigned long long ans = 0;
        do {
            do {
                for (int i = 1; i <= n; i++) {
                    int pl, pr;
                    for (int j = 0; j < n; j++) if (vf[j] == i) pl = j;
                    for (int j = 0; j < n; j++) if (vb[j] == i) pr = j;
                    std::vector<bool> vis(n+2);
                    for (int j = pl; j < n; j++) vis[vf[j]] = 1;
                    for (int j = 0; j <= pr; j++) vis[vb[j]] = 1;
                    for (now[i] = 1; vis[now[i]]; ++now[i]);
                }
                bool flg = true;
                for (int i = 1; i <= n; i++)
                    if (v[i] && v[i] != now[i])
                        { flg = false; break; }
                if (flg) {
                    st.insert(std::vector<int>(now+1, now+1+n));
                }
            } while (std::next_permutation(vb.begin(), vb.end()));
        } while (std::next_permutation(vf.begin(), vf.end()));
        for (const auto &v : st) {
            for (int i : v)
                printf("%d ", i);
            putchar('\n');
        }
        printf("%zu\n", st.size() % MOD);
    }
    return 0;
}