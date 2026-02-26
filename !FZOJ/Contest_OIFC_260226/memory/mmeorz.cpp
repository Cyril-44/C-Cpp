#include <bits/stdc++.h>
using namespace std;
const int N = 500010;
int n, k;
char s[N], ans[N];
struct Node {
    int l, r;
    int lc, rc;
} a[N];
int cnt;
int type(char c) {
    if (c == '0') return 0;
    if (c == '1') return 1;
    return -1;
}
char res[N];
bool attempt(int rem) {
    for (int i = cnt; i >= 1; i--) {
        if (a[i].l > a[i].r) continue;
        if (rem < 0) {
            if (a[i].lc == 1 && a[i].rc == 1) {
                for (int j = a[i].l; j <= a[i].r; j++) res[j] = '1';
                rem += 2;
            } else
                for (int j = a[i].l; j <= a[i].r; j++) res[j] = '0';
        } else {
            for (int j = a[i].l; j <= a[i].r; j++) res[j] = '0';
            for (int j = (a[i].rc == 0 ? a[i].r : a[i].r - 1); j >= a[i].l; j -= 2) {
                if (j == a[i].l) {
                    if (rem > 0 && a[i].lc == 0) {
                        res[j] = '1';
                        rem -= 2;
                    }
                } else if (rem > 0) {
                    res[j] = '1';
                    rem -= 2;
                }
            }
        }
    }
    return !rem;
}
void update() {
    for (int i = 1; i <= n; i++)
        if (res[i] < ans[i]) {
            for (int j = 1; j <= n; j++) ans[j] = res[j];
            return;
        } else if (res[i] > ans[i])
            return;
}
void solve() {
    scanf("%d%d%s", &n, &k, s + 1);
    cnt = 0;
    memcpy(res, s, sizeof(char) * (n + 1));
    res[n + 1] = 0;
    for (int i = 1; i <= n; i++) ans[i] = 127;
    ans[n + 1] = 0;
    for (int i = 1; i <= n; i++) {
        if (s[i] == '?') {
            if (s[i - 1] == '?')
                a[cnt].r++;
            else
                a[++cnt] = {i, i, type(s[i - 1]), -1};
        } else {
            if (s[i - 1] == '?')
                a[cnt].rc = type(s[i]);
            else if (s[i] != s[i - 1] && i != 1)
                k--;
        }
    }
    for (int i = cnt; i >= 1; i--) {
        for (int j = a[i].l; j <= a[i].r; j++) res[j] = '0';
        k -= (a[i].lc == 1) + (int)(a[i].rc == 1);
    }
    if (a[1].lc == -1) a[1].l++;
    if (a[cnt].rc == -1) a[cnt].r--;
    if (a[1].lc == -1 && a[cnt].rc == -1) {
        a[1].lc = 0;
        a[cnt].rc = 0;
        if (attempt(k)) update();
        a[cnt].rc = 1;
        res[n] = '1';
        if (a[cnt].l == n && a[cnt].lc == 1)
            k++;
        else
            k--;
        if (attempt(k)) update();
        a[1].lc = 1;
        res[1] = '1';
        if (a[1].r == 1 && a[1].rc == 1)
            k++;
        else
            k--;
        if (attempt(k)) update();
        a[cnt].rc = 0;
        res[n] = '0';
        if (a[cnt].l == n && a[cnt].lc == 1)
            k--;
        else
            k++;
        if (attempt(k)) update();
    } else if (a[1].lc == -1) {
        a[1].lc = 0;
        if (attempt(k)) update();
        a[1].lc = 1;
        res[1] = '1';
        if (a[1].r == 1 && a[1].rc == 1)
            k++;
        else
            k--;
        if (attempt(k)) update();
    } else if (a[cnt].rc == -1) {
        a[cnt].rc = 0;
        if (attempt(k)) update();
        a[cnt].rc = 1;
        res[n] = '1';
        if (a[cnt].l == n && a[cnt].lc == 1)
            k++;
        else
            k--;
        if (attempt(k)) update();
    } else {
        if (attempt(k)) update();
    }
    if (ans[1] == 127)
        printf("-1\n");
    else
        printf("%s\n", ans + 1);
}
int main() {
    freopen("memory.in", "r", stdin);
    freopen("memory.out", "w", stdout);
    int t;
    scanf("%d", &t);
    while (t--) solve();
    return 0;
}