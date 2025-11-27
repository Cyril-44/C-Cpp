#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
string Alice(vector<ll> a)
{
    const int N = 1279200;
    int b[1610];
    string res = "";
    bool type = 1;
    for (int times = 0; times < 3; times++)
    {
        for (int i = 0; i < a.size(); i++)
            b[i + 1] = a[i] % N;
        sort(b + 1, b + a.size() + 1);
        //        bool type=1;
        int pre = -1;
        int nwlen = unique(b + 1, b + a.size() + 1) - b - 1;
        int cur;
        for (ll x : a)
        {
            cur = lower_bound(b + 1, b + nwlen + 1, x % N) - b;
            type ^= 1;
            for (int i = 0; i < cur; i++)
                res.push_back(type + '0');
        }
        for (int i = 1; i <= nwlen; i++)
        {
            type ^= 1;
            for (; pre < b[i]; pre++)
                res.push_back(type + '0');
        }
        for (int i = 0; i < a.size(); i++)
            a[i] /= N;
    }
    return res;
}
vector<ll> Bob(int n, string s)
{
    const int N = 1279200;
    ll mul = 1;
    vector<ll> a(n), b, c(n + 2);
    s.push_back(s[s.size() - 1] == '1' ? '0' : '1');
    int cur = 0, lst = 0, mx = 0;
    for (int times = 0; times < 3; times++)
    {
        mx = 0;
        lst = cur;
        b.clear();
        for (int i = 1; i <= n; i++)
        {
            cur++;
            while (s[cur] == s[cur - 1])
                cur++;
            b.push_back(cur - lst);
            mx = max(mx, cur - lst);
            lst = cur;
        }
        int tot = 0;
        for (int i = 1; i <= mx; i++)
        {
            cur++;
            tot++;
            while (s[cur] == s[cur - 1])
                cur++, tot++;
            c[i] = tot - 1;
        }
        for (int i = 0; i < n; i++)
            a[i] += c[b[i]] * mul;
        mul *= N;
    }
    return a;
}