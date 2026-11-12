#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int mod = 998244353;
char s[2005];
int n, m, v[4], b[4];
ll C[4005][4005], fac[4005], inv[4005], ifac[4005], ans;
vector<ll> v, lst;
vector<ll> mul(vector<ll> a, vector<ll> b) {
    vector<ll> tmp(a.size() + b.size() - 1, 0);
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) { tmp[i + j] = (tmp[i + j] + a[i] * b[j]) % mod; }
    }
    return tmp;
}
int main() {
    freopen("rna.in", "r", stdin);
    freopen("rna.out", "w", stdout);
    C[0][0] = fac[0] = inv[0] = inv[1] = ifac[0] = ifac[1] = 1;
    for (int i = 1; i <= 4000; i++) {
        fac[i] = fac[i - 1] * i % mod;
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) { C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % mod; }
        if (i > 1) {
            inv[i] = (mod - mod / i) * inv[mod % i] % mod;
            ifac[i] = ifac[i - 1] * inv[i] % mod;
        }
    }
    scanf("%s%d%d%d%d", s + 1, &b[0], &b[1], &b[2], &b[3]);
    n = strlen(s + 1);
    if (n == 1 && s[1] == '#') n = 0;
    m = b[0] + b[1] + b[2] + b[3];
    for (int i = 1; i <= n; i++) {
        if (s[i] == 'A')
            v[0]++;
        else if (s[i] == 'G')
            v[1]++;
        else if (s[i] == 'C')
            v[2]++;
        else if (s[i] == 'U')
            v[3]++;
    }
    for (int x = 0; x < 4; x++) {
        v.resize(b[x] + 1);
        if (!v[x]) {
            v[0] = ifac[b[x]];
            for (int i = 1; i <= b[x]; i++) v[i] = 0;
        } else {
            for (int i = 0; i <= b[x]; i++) {
                v[i] = (i & 1 ? mod - 1 : 1) * C[v[x] + i - 1][i] % mod * ifac[b[x] - i] % mod;
            }
        }
        if (!x)
            lst = v;
        else
            lst = mul(lst, v);
    }
    v = lst;
    for (int i = 0; i < v.size(); i++) { ans = (ans + fac[n + m] * v[i] % mod * ifac[n + i]) % mod; }
    cout << ans << endl;
    return 0;
}