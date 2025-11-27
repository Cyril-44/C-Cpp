#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ll;
const int P = 1e9 + 7, Q = 1e9 + 9, B = 457, C = 997;
ll next_hash(ll prev, int chr)
{
    ll a = prev >> 32, b = prev & -1u;
    chr++;
    a = (a * B + chr) % P;
    b = (b * C + chr) % Q;
    return (a << 32) | b;
}
const int maxl = 5002424;
const int maxn = 200083;
struct node
{
    int pt, nxt[27];
} trie[maxl * 2 + maxn];
struct pn
{
    int rt;
    unordered_map<ll, int> mp;
} pns[maxn];
int ctc, ptc;
char s[maxl], t[maxl];
void q_diff(int len, int &dnq, int &dxq)
{
    int nq = len + 1, xq = -1;
    int i;
    for (i = 0; i < len; i++)
        if (s[i] != t[i])
            nq = min(nq, i), xq = max(xq, i);
    dnq = nq;
    dxq = xq;
}
#define jnode(var, chr)           \
    if (trie[var].nxt[chr])       \
        var = trie[var].nxt[chr]; \
    else                          \
        var = trie[var].nxt[chr] = ++ctc
#define ord(ch) (ch == '#' ? 26 : ch - 'a')
void add_s(int idx)
{
    scanf("%s%s", s, t);
    int len = strlen(s);
    if (strcmp(s, t) == 0)
        return;
    int nq, xq;
    q_diff(len, nq, xq);
    int i, cur = 0;
    for (i = nq; i <= xq; i++)
    {
        jnode(cur, ord(s[i]));
        jnode(cur, ord(t[i]));
    }
    jnode(cur, 26);
    for (; i < len; i++)
        jnode(cur, ord(s[i]));
    int pt = trie[cur].pt;
    if (pt == 0)
        pt = trie[cur].pt = ++ptc;
    cur = pns[pt].rt;
    if (cur == 0)
        cur = pns[pt].rt = ++ctc;
    for (i = nq - 1; i >= 0; i--)
        jnode(cur, ord(s[i]));
    trie[cur].pt++;
}
void dfs_pn(int rt, int cur, ll ch, int cn)
{
    pns[rt].mp[ch] = cn = trie[cur].pt += cn;
    int i, m;
    for (i = 0; i < 27; i++)
        if (m = trie[cur].nxt[i])
            dfs_pn(rt, m, next_hash(ch, i), cn);
}
ll hashes[maxl];
int query(void)
{
    scanf("%s%s", s, t);
    int len = strlen(s);
    if (strlen(t) != len)
        return 0;
    int nq, xq;
    q_diff(len, nq, xq);
    int i, cur = 0;
    for (i = 1; i <= nq; i++)
        hashes[i] = next_hash(hashes[i - 1], ord(s[nq - i]));
    for (i = nq; i <= xq; i++)
    {
        if (0 == (cur = trie[cur].nxt[ord(s[i])]))
            return 0;
        if (0 == (cur = trie[cur].nxt[ord(t[i])]))
            return 0;
    }
    if (0 == (cur = trie[cur].nxt[26]))
        return 0;
    int ans = 0;
    for (i = xq; i < len; i++)
    {
        if (trie[cur].pt != 0)
        {
            int l = 0, r = nq;
            pn &cpn = pns[trie[cur].pt];
            while (l < r)
            {
                int m = l + r + 1 >> 1;
                if (cpn.mp.count(hashes[m]))
                    l = m;
                else
                    r = m - 1;
            }
            ans += cpn.mp[hashes[l]];
        }
        if (i != len - 1)
            if (0 == (cur = trie[cur].nxt[ord(t[i + 1])]))
                break;
    }
    return ans;
}
int main(void)
{
    int n, q;
    scanf("%d%d", &n, &q);
    int i;
    for (i = 1; i <= n; i++)
        add_s(i);
    for (i = 1; i <= ptc; i++)
        dfs_pn(i, pns[i].rt, 0, 0);
    for (i = 1; i <= q; i++)
        printf("%d\n", query());
}
