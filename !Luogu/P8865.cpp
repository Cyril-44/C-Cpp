#include <bits/stdc++.h>
constexpr int N = 1005, MOD = 998244353;
char s[N][N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this;  }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint(int v=0) : val(v % MOD) {}
private: int val;
};
int sumh[N][N], sumv[N][N];
int main() {
    int T, id, n, m, c, f;
    scanf("%d%d", &T, &id);
    while (T--) {
        scanf("%d%d%d%d", &n, &m, &c, &f);
        for (int i = 1; i <= n; i++) {
            scanf("%s", s[i] + 1);
            for (int j = m, cnt = 0; j >= 0; j--) {
                if (s[i][j] == '1') cnt = 0;
                else ++cnt;
                sumh[i][j] = cnt;
            }
        }
        for (int j = 1; j <= m; j++) {
            for (int i = n, cnt = 0; i >= 1; i--) {
                if (s[i][j] == '1') cnt = 0;
                else ++cnt;
                sumv[i][j] = cnt;
            }
            sumv[n+1][j] = 0;
        }
        Mint vc, vf;
        for (int j = 1; j <= m; j++) {
            int last = 0, que = 0;
            for (int i = 1; i <= n; i++) {
                if (s[i][j] == '1') { last = que = 0; continue; }
                vc += last * (sumh[i][j] - 1);
                vf += Mint(last) * (sumh[i][j] - 1) * sumv[i+1][j];
                last += que;
                que = sumh[i][j] - 1;
            }
        }
        printf("%d %d\n", vc * c, vf * f);
    }
}