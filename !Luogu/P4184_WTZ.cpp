#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 100010;
const int Mod = 100000007;
int n;
struct Node
{
    int a, b;
} x[N];
int upl[N], dnl[N];
int main()
{
    scanf("%d", &n);
    memset(upl, 0xc0, sizeof(upl));
    memset(dnl, 0x3f, sizeof(dnl));
    for (int i = 1; i <= n; i++)
    {
        scanf("%d%d", &x[i].a, &x[i].b);
        if (x[i].a)
            upl[x[i].a - 1] = x[i].b - 1;
        dnl[x[i].a] = x[i].b;
    }
    for (int i = n - 1; i >= 0; i--)
        upl[i] = max(upl[i], upl[i + 1]);
    for (int i = 1; i <= n; i++)
        dnl[i] = min(dnl[i], dnl[i - 1]);
    int l = INT_MAX, r = 0;
    for (int i = 0; i <= n; i++)
    {
        if (upl[i] >= dnl[i])
        {
            l = min(i, l);
            r = max(i, r);
        }
    }
    //    printf("%d %d\n",l,r);
    ll sum = 0, sq = 0, ans = 0;
    int to = l;
    for (int i = l; i <= r; i++)
    {
        while (to <= r && upl[to] >= dnl[i])
        {
            (sum += (upl[to] - dnl[i] + 1) * (upl[to] - dnl[i] + 2ll) / 2) %= Mod;
            sq += upl[to] - dnl[i] + 1;
            to++;
        }
        //        printf("%lld\n",sum);
        ans += sum;
        sum += (Mod - (upl[i] - dnl[i] + 1) * (upl[i] - dnl[i] + 2ll) / 2 % Mod);
        sum %= Mod;
        sq -= upl[i] - dnl[i] + 1;
        //        printf("%lld\n",sq);
        for (int j = dnl[i] - 1; j >= dnl[i + 1]; j--)
        {
            (ans += sq + 1) %= Mod;
            sq += to - i;
        }
    }
    printf("%lld", ans);
}
