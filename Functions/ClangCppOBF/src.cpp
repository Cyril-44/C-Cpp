#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int N = 600005;
int n, m, nw, id[N], t[N], sa[N], rk[N], tmp[N], h[N], c[N], ST[N][20];
ll ans;
char s[N];
char s0[200005];
void SA() {
    nw = 'z';
    memset(t, 0, sizeof(t));
    for (int i = 1; i <= n; i++) { t[rk[i] = s[i]]++; }
    for (int i = 1; i <= nw; i++) { t[i] += t[i - 1]; }
    for (int i = n; i >= 1; i--) { sa[t[rk[i]]--] = i; }
    for (int k = 1; k <= n; k <<= 1) {
        memset(t, 0, sizeof(t));
        int cnt = 0;
        for (int i = n - k + 1; i <= n; i++) { tmp[++cnt] = i; }
        for (int i = 1; i <= n; i++) {
            if (sa[i] > k) { tmp[++cnt] = sa[i] - k; }
        }
        for (int i = 1; i <= n; i++) { t[rk[i]]++; }
        for (int i = 1; i <= nw; i++) { t[i] += t[i - 1]; }
        for (int i = n; i >= 1; i--) { sa[t[rk[tmp[i]]]--] = tmp[i]; }
        swap(tmp, rk);
        rk[sa[1]] = 1, cnt = 1;
        for (int i = 2; i <= n; i++) {
            if (tmp[sa[i]] == tmp[sa[i - 1]] && tmp[sa[i] + k] == tmp[sa[i - 1] + k]) {
                rk[sa[i]] = cnt;
            } else {
                rk[sa[i]] = ++cnt;
            }
        }
        if (cnt == n) break;
        nw = cnt;
    }
}
void HEIG() {
    int k = 0;
    for (int i = 1; i <= n; i++) {
        if (rk[i] == 1) continue;
        if (k) k--;
        for (int j = sa[rk[i] - 1]; j + k <= n && i + k <= n && s[i + k] == s[j + k];) k++;
        h[rk[i]] = k;
    }
}
int query(int l, int r) {
    if (l > r) swap(l, r);
    l++;
    int lg2 = __lg(r - l + 1);
    return min(ST[l][lg2], ST[r - (1 << lg2) + 1][lg2]);
}
void work(int x, int len) {
    int nw = rk[x] - 1;
    for (int i = len; i >= 1; i--) {
        int l = 1, r = nw, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (query(mid, rk[x]) >= i)
                r = mid - 1;
            else
                l = mid + 1;
        }
        ans += 1ll * (c[nw] - c[l - 1]) * i;
        nw = l - 1;
    }
    nw = rk[x] + 1;
    for (int i = len; i >= 1; i--) {
        int l = nw, r = n, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (query(rk[x], mid) >= i)
                l = mid + 1;
            else
                r = mid - 1;
        }
        ans += 1ll * (c[l - 1] - c[nw - 1]) * i;
        nw = l;
    }
}
int main() {
    scanf("%s", s0 + 1);
    int nn = strlen(s0 + 1);
    memcpy(s, s0, sizeof(s0));
    n = nn;
    s[++n] = '#';
    for (int i = 1; i < nn; i += 2) {
        if (s0[i] == s0[i + 1])
            s[++n] = s0[i];
        else if (s[n] != '#')
            s[++n] = '#';
    }
    s[++n] = '#';
    for (int i = 2; i < nn; i += 2) {
        if (s0[i] == s0[i + 1])
            s[++n] = s0[i];
        else if (s[n] != '#')
            s[++n] = '#';
    }
    SA();
    HEIG();
    for (int i = 1; i <= n; i++) { ST[i][0] = h[i]; }
    for (int j = 1; (1 << j) <= n; j++) {
        for (int i = 1; i <= n; i++) {
            if (i + (1 << j - 1) > n)
                ST[i][j] = ST[i][j - 1];
            else
                ST[i][j] = min(ST[i][j - 1], ST[i + (1 << j - 1)][j - 1]);
        }
    }
    for (int i = 1; i <= nn; i++) { c[rk[i]]++; }
    for (int i = 1; i <= n; i++) { c[i] += c[i - 1]; }
    for (int i = nn + 1; i <= n;) {
        while (s[i] == '#') i++;
        int j;
        for (j = i + 1; j <= n && s[j] != '#'; j++);
        for (int k = i; k < j; k++) { work(k, j - k); }
        i = j + 1;
    }
    cout << ans % 17171717 << endl;
    return 0;
}