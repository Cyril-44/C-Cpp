#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll k;
char s[5010], t[12];
int len;
const ll Mod = 998244353;
struct Matrix
{
    ll a[12][12];
} A, B, C;
Matrix operator*(Matrix a, Matrix b)
{
    Matrix res;
    for (int i = 1; i <= len + 1; i++)
        for (int j = 1; j <= len + 1; j++)
            res.a[i][j] = 0;
    for (int i = 1; i <= len + 1; i++)
        for (int j = 1; j <= len + 1; j++)
            for (int k = 1; k <= len + 1; k++)
                res.a[i][j] = (res.a[i][j] + a.a[i][k] * b.a[k][j]) % Mod;
    return res;
}
Matrix operator^(Matrix a, ll b)
{
    Matrix res = A;
    while (b)
    {
        if (b & 1)
            res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}
int dp[5010][12];
int main()
{
    scanf("%lld%s%s", &k, s + 1, t + 1);
    len = strlen(t + 1);
    int n = strlen(s + 1);
    for (int i = 1; i <= len + 1; i++)
        A.a[i][i] = 1;
    for (int i = 1; i <= len; i++)
    {
        memset(dp, 0, sizeof(dp));
        dp[0][i] = 1;
        s[0] = t[i];
        for (int j = 1; j <= n; j++)
        {
            for (int p = 1; p <= len; p++)
                if (t[p] == s[j])
                {
                    int cur = p == 1 ? len : p - 1;
                    for (int q = 0; q < j; q++)
                        if (s[q] == t[cur])
                            dp[j][p == len ? p + 1 : p] = dp[j][p == len ? p + 1 : p] + dp[q][cur];
                    dp[j][p == len ? p + 1 : p] %= Mod;
                }
        }
        for (int j = 1; j <= len + 1; j++)
        {
            //            int x=j==len?j+1:j;
            for (int p = 0; p <= n; p++)
            {
                B.a[i][j] += dp [p][j];
            }
            B.a[i][j] = B.a[i][j] % Mod;
        }
    }
    B.a[len + 1][len + 1] = 1;
    C.a[1][len] = 1;
    B = B ^ k;
    C = C * B;
    printf("%lld", C.a[1][len + 1]);
    return 0;
}