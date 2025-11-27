#include <bits/stdc++.h>
using namespace std;
const int maxn = 100010;
#define ull unsigned long long
int c, t, len, state;
string s;
int n[maxn];
long long sm[maxn][10], tag[10], cnt[10], vlu[10], e[10], f[10], deta[10];
long long ans = LLONG_MAX, nans;
long long mb[10] = {0, 1, 11, 111, 1111, 11111, 111111}, me[10] = {0, 9, 99, 999, 9999, 99999, 999999}, jc[10] = {0, 1, 10, 100, 1000, 10000, 100000, 1000000};
void pre()
{
    ans = 0;
    cin >> s;
    memset(n, 0, sizeof(n));
    memset(cnt, 0, sizeof(cnt));
    memset(tag, 0, sizeof(tag));
    memset(e, 0, sizeof(e));
    len = s.length();
    for (int i = 1; i <= len; i++)
    {
        n[i] = s[i - 1] - '0';
        cnt[n[i]]++;
        if (!tag[n[i]])
            tag[n[i]] = i;
        for (int x = 1; x <= 9; x++)
            sm[i][x] = sm[i - 1][x];
        sm[i][n[i]]++;
    }
    for (int i = 1; i <= 9; i++)
        cin >> vlu[i];
    return;
}
inline bool check(int mj)
{ //(l,r) check the under
    int l = tag[e[1]], r = len + 1, mid = (l + r) >> 1;
    int now = l;
    if (len - l + 1 < mj)
        return 0;
    for (int i = 2; i <= mj; i++)
    {
        l = now, r = len + 1;
        while (l < r - 1)
        {
            mid = (l + r) >> 1;
            if (sm[mid][e[i]] - sm[now][e[i]] > 0)
                r = mid;
            else
                l = mid;
        }
        now = r;
        if (now == len + 1)
            return 0;
    }
    return 1;
}
void slove()
{
    ans = LLONG_MAX;
    bool pd = 0;
    long long opmin = LLONG_MAX;
    for (int mj = 1; mj <= min(6, len); mj++)
    {
        for (int now = mb[mj]; now <= me[mj]; now++)
        {
            memset(deta, 0, sizeof(deta));
            nans = 0;
            pd = 0;
            int temp = now, pll = 0;
            while (temp)
            {
                f[++pll] = temp % 10;
                temp /= 10;
            }
            for (int i = 1; i <= mj; i++)
            {
                e[i] = f[mj - i + 1];
                deta[e[i]]++;
                if (!e[i])
                {
                    pd = 1;
                    break;
                }
                if (deta[e[i]] > cnt[e[i]])
                {
                    pd = 1;
                    break;
                }
            }
            if (pd)
                continue;
            if (!check(mj))
                continue;
            for (int i = 1; i <= 9; i++)
                nans += (cnt[i] - deta[i]) * vlu[i];
            if (nans >= ans)
                continue;
            if (len > 10)
            {
                ans = min(nans + now, ans);
                continue;
            }
            opmin = LLONG_MAX;
            for (int state = 0; state < (1 << mj + 1); state++)
            {
                long long op = 0, res = 0;
                for (int i = 0; i <= mj - 1; i++)
                {
                    if ((state >> i) & 1)
                    {
                        int ch = e[i + 1];
                        op += vlu[ch];
                    }
                    else
                        res = res * 10 + e[i + 1];
                }
                op += res;
                opmin = min(op, opmin);
            }
            nans += opmin;
            ans = min(nans, ans);
        }
    }
    return;
}
int main()
{
    //	freopen("bargain4.in","r",stdin);
    cin >> c >> t;
    while (t)
    {
        pre();
        slove();
        cout << ans;
        if (t != 1)
            cout << endl;
        t--;
    }
    return 0;
}
