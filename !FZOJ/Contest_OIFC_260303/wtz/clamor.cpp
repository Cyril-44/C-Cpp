#include <bits/stdc++.h>
using namespace std;
const int N = 1000010;
char str[N];
struct SegTree {
    int cnt[N << 1], all[N << 1], rev[N << 1];
#define ls (u << 1)
#define rs (u << 1 | 1)
    void pushup(int u) { cnt[u] = cnt[ls] + cnt[rs]; }
    void pushdown(int l, int r, int u) {
        int m = l + r >> 1;
        if (all[u] == -1) {
            cnt[ls] = cnt[rs] = 0;
            all[ls] = all[rs] = -1;
            rev[ls] = rev[rs] = 0;
        } else if (all[u] == 1) {
            cnt[ls] = m - l + 1;
            cnt[rs] = r - m;
            all[ls] = all[rs] = 1;
            rev[ls] = rev[rs] = 0;
        }
        all[u] = 0;
        if (rev[u]) {
            cnt[ls] = m - l + 1 - cnt[ls];
            cnt[rs] = r - m - cnt[rs];
            rev[ls] ^= 1;
            rev[rs] ^= 1;
            rev[u] = 0;
        }
    }
    void updall(int l, int r, int tl, int tr, int u, int x) {
        if (tl <= l && r <= tr) {
            if (x == 1)
                cnt[u] = r - l + 1;
            else
                cnt[u] = 0;
            all[u] = x;
            rev[u] = 0;
            return;
        }
        pushdown(l, r, u);
        int m = l + r >> 1;
        if (tl <= m) updall(l, m, tl, tr, ls, x);
        if (m + 1 <= tr) updall(m + 1, r, tl, tr, rs, x);
        pushup(u);
    }
    void updrev(int l, int r, int tl, int tr, int u) {
        if (tl <= l && r <= tr) {
            // printf("*** %d %d %d\n",tl,tr,u);
            cnt[u] = r - l + 1 - cnt[u];
            rev[u] ^= 1;
            return;
        }
        pushdown(l, r, u);
        int m = l + r >> 1;
        if (tl <= m) updrev(l, m, tl, tr, ls);
        if (m + 1 <= tr) updrev(m + 1, r, tl, tr, rs);
        pushup(u);
    }
    int query(int l, int r, int tl, int tr, int u) {
        if (tl <= l && r <= tr) return cnt[u];
        pushdown(l, r, u);
        int m = l + r >> 1, res = 0;
        if (tl <= m) res += query(l, m, tl, tr, ls);
        if (m + 1 <= tr) res += query(m + 1, r, tl, tr, rs);
        return res;
    }
} odd, even;
int n, q, op, l, r, x;
int main() {
    freopen("clamor.in", "r", stdin);
    freopen("clamor.out", "w", stdout);
    scanf("%d%d", &n, &q);
    int m = (n + 1) >> 1;
    scanf("%s", str + 1);
    for (int i = 1; i <= n; i++) {
        if (str[i] == ')') {
            if (i & 1)
                odd.updrev(1, m, i + 1 >> 1, i + 1 >> 1, 1);
            else
                even.updrev(1, m, i >> 1, i >> 1, 1);
        }
    }
    while (q--) {
        scanf("%d%d%d", &op, &l, &r);
        int oddl = (l >> 1) + 1, oddr = r + 1 >> 1;
        int evenl = l + 1 >> 1, evenr = r >> 1;
        if (op == 1) {
            if (oddl <= oddr) odd.updrev(1, m, oddl, oddr, 1);
            if (evenl <= evenr) even.updrev(1, m, evenl, evenr, 1);
        } else if (op == 2) {
            scanf("%d", &x);
            if (x == 0) x = -1;
            if (oddl <= oddr) odd.updall(1, m, oddl, oddr, 1, x);
            if (evenl <= evenr) even.updall(1, m, evenl, evenr, 1, x);
        } else {
            int len = r - l + 1;
            if (len & 1) {
                printf("0\n");
                continue;
            }
            int lcnt = 0, rcnt = 0, mn;
            if (oddl <= oddr) lcnt = odd.query(1, m, oddl, oddr, 1);
            if (evenl <= evenr) rcnt = even.query(1, m, evenl, evenr, 1);
            if (!(l & 1)) swap(lcnt, rcnt);
            mn = min(lcnt, rcnt);
            lcnt -= mn;
            rcnt -= mn;
            if (((8 + len - (lcnt + rcnt << 1)) & 2)) {
                printf("0\n");
                continue;
            }
            int ans = 0;
            if (rcnt) ans++, rcnt--, swap(lcnt, rcnt), len -= 2;
            ans += (len >> 1) - lcnt;
            printf("%d\n", ans);
        }
    }
}