#include <bits/stdc++.h>
using namespace std;
string Alice(vector<int> a)
{
    int b[1010];
    for (int i = 0; i < a.size(); i++)
        b[i + 1] = a[i];
    sort(b + 1, b + a.size() + 1);
    bool type = 1;
    string res = "";
    int pre = -1;
    int nwlen = unique(b + 1, b + a.size() + 1) - b - 1;
    int cur;
    for (int x : a)
    {
        cur = lower_bound(b + 1, b + nwlen + 1, x) - b;
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
    return res;
}
vector<int> Bob(int n, string s)
{
    vector<int> a, b, c;
    s.push_back(s[s.size() - 1] == '1' ? '0' : '1');
    int cur = 0, lst = 0, mx = 0;
    for (int i = 1; i <= n; i++)
    {
        cur++;
        while (s[cur] == s[cur - 1])
            cur++;
        b.push_back(cur - lst);
        mx = max(mx, cur - lst);
        lst = cur;
    }
    c.push_back(0);
    int tot = 0;
    for (int i = 1; i <= mx; i++)
    {
        cur++;
        tot++;
        while (s[cur] == s[cur - 1])
            cur++, tot++;
        c.push_back(tot - 1);
    }
    for (int i = 0; i < n; i++)
        a.push_back(c[b[i]]);
    return a;
}